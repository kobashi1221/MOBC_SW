/*
 * GPIO_MOBC.cpp
 *
 *  Created on: 2021/12/03
 *      Author: Otsuki
 */

// 電源状態（PWR_State）変更と電源ON/OFFを実行
// 自由に使えるようにするため（ユーザー側で統一するため）には初期化動作を含んだ関数を用意しておいた方がいい？

#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>
#include <stdbool.h> //for boolean
#include "PWR_Operation.h"
#include "src_user/Library/Delay.h"
#include <src_user/IfWrapper/IF_class.h>
#include "MCP3421.h"
#include "MCP23017.h"

#include <stdio.h>
#include <stdint.h> //for uint8_t

#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Drivers/Sensor/MTQ.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include "src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.h"
#include "../../Applications/DriverInstances/di_LoRa.h"
#include "../../Applications/DriverInstances/di_cobc.h"
#include "src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.h"
#include "src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.h"
#include "src_user/Applications/UserDefined/Sensor/ACS23_cmd_PLASMA.h"

uint8_t MTQA_anomaly_counter = 12;//MATQAのアノマリenableまでのカウンタ初期値
uint8_t MTQO_anomaly_counter = 12;//MATQOのアノマリenableまでのカウンタ初期値

#define MCP3421_PWR2_ADDR 0xD4
#define MCP23017_PWR2_ADDR 0x48

static MTQ_CLASS MTQ_Class;

extern GPIO_Class gpios_out[GPIO_OUT_MAX];
extern GPIO_Class gpios_in[GPIO_OUT_MAX];

MCP23017 ioex_class(MCP23017_PWR2_ADDR, &hi2c2);
MCP3421 adc_class(MCP3421_PWR2_ADDR, &hi2c2); //ADC_PWR2 Object

void PWR_Operate_ON_OFF(PWR_STATE num,uint8_t data){
	// data=0 -> Disable, data=1 -> Enable
	if(data==0 || data==1){

		// SUNS以外でFETを操作する時 Anomaly Waiting Counter リセット
		// SUNS は電力管理のため Duty比を落とすので頻繁にFET操作
		if(num!=PWR_State_SUNS){
			PWR_Reset_AWC();
		}

		uint8_t flag;
		MCP23017PortACK ack = PWR_OK;
		switch(num){
		case PWR_State_REG_CDH3V3:
//			data==1 ? printf("REG_CDH3V3 Enable(MOBC ON)\r\n") : printf("REG_CDH3V3 Disable(MOBC OFF)\r\n");
			data==1 ? print(PWR_OPERATION,"REG_CDH3V3 Enable(MOBC ON)\r\n") : print(PWR_OPERATION,"REG_CDH3V3 Disable(MOBC OFF)\r\n");
			PWR_State_SET(num,data);
			gpios_out[EN3V3_MOBCtoREG_CDH3V3].Write(data);
			break;
		case PWR_State_GPS:
//			data==1 ? printf("GPS Power ON\r\n") : printf("GPS Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"GPS Power ON\r\n") : print(PWR_OPERATION,"GPS Power OFF\r\n");
			PWR_State_SET(num,data);
			gpios_out[GATE3V3_MOBCtoFET_GPS].Write(data==0);
//			if(data == 0) {c2a_delay_ms(100);}
			data== 1 ? ANOMALY_UART_Set_Enable(UART_FAULT_GPS) : ANOMALY_UART_Reset_Enable(UART_FAULT_GPS);
			break;
		case PWR_State_SUNS:
//			data==1 ? printf("SUNS Power ON\r\n") : printf("SUNS Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"SUNS Power ON\r\n") : print(PWR_OPERATION,"SUNS Power OFF\r\n");
			if (data==1) ACS13_SUNS_Set_Duty_Counter();
			PWR_State_SET(num,data);
			gpios_out[GATE3V3_MOBCtoFET_SUNS].Write(data==0);
			// SUNS は Duty比でONOFFが頻繁なのでAnomaly Waiting Counter対象外
			// 電圧計測用Delay（これがないと電圧が中途半端になり、電圧計測アノマリに引っかかる恐れがある）
			if(data == 0) {c2a_delay_ms(5);}
			data== 1 ? ANOMALY_SPI_Set_Enable(SPI_FAULT_SUNS) : ANOMALY_SPI_Reset_Enable(SPI_FAULT_SUNS);
			break;
		case PWR_State_LORA:
//			data==1 ? printf("LORA Power ON\r\n") : printf("LORA Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"LORA Power ON\r\n") : print(PWR_OPERATION,"LORA Power OFF\r\n");
//			if(data == 0){
//				 lora_driver->lora_mode.mode = 0;
//			}
			PWR_State_SET(num,data);
			gpios_out[GATE3V3_MOBCtoFET_LORA].Write(data==0);
			// 電圧計測用Delay（これがないと電圧が中途半端になり、電圧計測アノマリに引っかかる恐れがある）
			// 計測値 ON:3ms, OFF:80ms
			break;
		case PWR_State_IOEX_PWR2:
//			data==1 ? printf("IOEX/ADC_PWR2 Power ON\r\n") : printf("IOEX/ADC_PWR2 Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"IOEX/ADC_PWR2 Power ON\r\n") : print(PWR_OPERATION,"IOEX/ADC_PWR2 Power OFF\r\n");
			PWR_State_SET(num,data);
			gpios_out[GATE3V3_MOBCtoFET_ADCIOEX_PWR2].Write(data);
			// 計測値 ON:8ms, OFF:400ms（こんなに待たなくてもいいと思う）
			data== 1 ? ANOMALY_I2C_Set_Enable(I2C_FAULT_ADCIOEX) : ANOMALY_I2C_Reset_Enable(I2C_FAULT_ADCIOEX);
			break;
		case PWR_State_MTQA:
//			data==1 ? printf("MTQA Power ON\r\n") : printf("MTQA Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"MTQA Power ON\r\n") : print(PWR_OPERATION,"MTQA Power OFF\r\n");
			PWR_State_SET(num,data);
//			gpios_out[GATE3V3_MOBCtoFET_MTQA].Write(data);
			gpios_out[GATE3V3_MOBCtoFET_MTQA].Write(data==0); // Low->HIGH->OFF
			MTQA_anomaly_counter = 5;
			// 電圧計測用Delay（これがないと電圧が中途半端になり、電圧計測アノマリに引っかかる恐れがある）
			// 計測値 ON:2ms, OFF:1ms
//			data ==1 ? c2a_delay_ms(10) : c2a_delay_ms(200);
			break;
		case PWR_State_SUNS_MXPZ:
//			data==1 ? printf("SUNS_MXPZ Power ON\r\n") : printf("SUNS_MXPZ Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"SUNS_MXPZ Power ON\r\n") : print(PWR_OPERATION,"SUNS_MXPZ Power OFF\r\n");
			if(data == 1){
				ack = ioex_class.digitalWrite(IOEX_PORT_SUNS_MXPZ,data);
//				c2a_delay_ms(50);
			}
			PWR_State_SET(num,data);

			// 通信アノマリ処理 未実装

			ack = ioex_class.digitalWrite(IOEX_PORT_SUNS_MXPZ,data == 0);
			// 電圧計測用Delay（これがないと電圧が中途半端になり、電圧計測アノマリに引っかかる恐れがある）
			// 未計測 ON:　ms,OFF:　m
//			if(data == 0) {c2a_delay_ms(600);}
			break;
		case PWR_State_SUNS_PXPYMY:
//			data==1 ? printf("SUNS_PXPYMY Power ON\r\n") : printf("SUNS_PXPYMY Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"SUNS_PXPYMY Power ON\r\n") : print(PWR_OPERATION,"SUNS_PXPYMY Power OFF\r\n");
			if(data == 1){
				ack = ioex_class.digitalWrite(IOEX_PORT_SUNS_PXPYMY,data);
//				c2a_delay_ms(50);
			}
			PWR_State_SET(num,data);

			// 通信アノマリ処理 未実装

			ack = ioex_class.digitalWrite(IOEX_PORT_SUNS_PXPYMY,data == 0);
			// 電圧計測用Delay（これがないと電圧が中途半端になり、電圧計測アノマリに引っかかる恐れがある）
			// 未計測 ON:　ms,OFF:　m
//			if(data == 0) {c2a_delay_ms(600);}
			break;
		case PWR_State_GYRO:
//			data==1 ? printf("GYRO Power ON\r\n") : printf("GYRO Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"GYRO Power ON\r\n") : print(PWR_OPERATION,"GYRO Power OFF\r\n");
			if(data == 1){
				ack = ioex_class.digitalWrite(IOEX_PORT_GYRO,data);
//				c2a_delay_ms(50);
			}
			PWR_State_SET(num,data);

			// 通信アノマリ処理 未実装

			ack = ioex_class.digitalWrite(IOEX_PORT_GYRO,data == 0);
			// 電圧計測用Delay（これがないと電圧が中途半端になり、電圧計測アノマリに引っかかる恐れがある）
			// 未計測 ON:　ms,OFF:　m
//			if(data == 0) {c2a_delay_ms(600);}
			break;
		case PWR_State_SEMP:
//			data==1 ? printf("SEMP Power ON\r\n") : printf("SEMP Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"SEMP Power ON\r\n") : print(PWR_OPERATION,"SEMP Power OFF\r\n");
			if(data == 1){
				ack = ioex_class.digitalWrite(IOEX_PORT_SEMP,data);
//				c2a_delay_ms(50);
			}
			PWR_State_SET(num,data);

			// 通信アノマリ処理 未実装

			ack = ioex_class.digitalWrite(IOEX_PORT_SEMP,data == 0);
			// 電圧計測用Delay（これがないと電圧が中途半端になり、電圧計測アノマリに引っかかる恐れがある）
			// 未計測 ON:　ms,OFF:　m
//			if(data == 0) {c2a_delay_ms(600);}
			break;
		case PWR_State_MTQO_PLASMA:
//			data==1 ? printf("MTQO Power ON\r\n") : printf("MTQO Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"MTQO PLASMA Power ON\r\n") : print(PWR_OPERATION,"MTQO PLASMA Power OFF\r\n");
			PWR_State_SET(num,data);
			MTQA_anomaly_counter = 5;
			ACS23_PLASMA_off();  //FETOFF後PLASMAを0V出力
			// 2U(MTQO, PLASMA):LowでON, 1U(PLASMA):HighでON
#ifdef TWO_U
			gpios_out[GATE3V3_MOBCtoFET_MTQO_PLASMA].Write(data==0);
#endif
#ifdef ONE_U
			gpios_out[GATE3V3_MOBCtoFET_MTQO_PLASMA].Write(data);
#endif
			// 電圧計測用Delay（これがないと電圧が中途半端になり、電圧計測アノマリに引っかかる恐れがある）
			// 計測値 ON:2ms,OFF:150m
//			data ==1 ? c2a_delay_ms(10) : c2a_delay_ms(200);
			break;
        case PWR_State_SEPCOIL1:
            flag = PWR_Sepcoil_Guard_GET(SEPCOIL_1);
//          print(PWR_OPERATION,"SEPCOIL GUARD 1. Flag: %d\r\n", flag);
//          print(PWR_OPERATION,"g_sensor spc: %d\r\n", g_sensor.p_pwr_spc_guard[0]);
            if(flag){
    //          data==1 ? printf("SEPCOIL1 Power ON\r\n") : printf("SEPCOIL1 Power OFF\r\n");
                data==1 ? print(PWR_OPERATION,"SEPCOIL1 Power ON\r\n") : print(PWR_OPERATION,"SEPCOIL1 Power OFF\r\n");
                PWR_State_SET(num,data);
                ack = ioex_class.digitalWrite(IOEX_PORT_SEPCOIL1,data);
            }
            break;
        case PWR_State_SEPCOIL2:
            flag = PWR_Sepcoil_Guard_GET(SEPCOIL_2);
            if(flag){
    //          data==1 ? printf("SEPCOIL2 Power ON\r\n") : printf("SEPCOIL2 Power OFF\r\n");
                data==1 ? print(PWR_OPERATION,"SEPCOIL2 Power ON\r\n") : print(PWR_OPERATION,"SEPCOIL2 Power OFF\r\n");
                PWR_State_SET(num,data);
                ack = ioex_class.digitalWrite(IOEX_PORT_SEPCOIL2,data);
            }
            break;
        case PWR_State_SEPCOIL3:
            flag = PWR_Sepcoil_Guard_GET(SEPCOIL_3);
            if(flag){
    //          data==1 ? printf("SEPCOIL3 Power ON\r\n") : printf("SEPCOIL3 Power OFF\r\n");
                data==1 ? print(PWR_OPERATION,"SEPCOIL3 Power ON\r\n") : print(PWR_OPERATION,"SEPCOIL3 Power OFF\r\n");
                PWR_State_SET(num,data);
                ack = ioex_class.digitalWrite(IOEX_PORT_SEPCOIL3,data);
            }
            break;
        case PWR_State_SEPCOIL4:
            flag = PWR_Sepcoil_Guard_GET(SEPCOIL_4);
            if(flag){
    //          data==1 ? printf("SEPCOIL4 Power ON\r\n") : printf("SEPCOIL4 Power OFF\r\n");
                data==1 ? print(PWR_OPERATION,"SEPCOIL4 Power ON\r\n") : print(PWR_OPERATION,"SEPCOIL4 Power OFF\r\n");
                PWR_State_SET(num,data);
                ack = ioex_class.digitalWrite(IOEX_PORT_SEPCOIL4,data);
            }
            break;
		case PWR_State_MIS1:
//			data==1 ? printf("MIS1 Power ON\r\n") : printf("MIS1 Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"MIS1 Power ON\r\n") : print(PWR_OPERATION,"MIS1 Power OFF\r\n");
			if(data == 1){
				ack = ioex_class.digitalWrite(IOEX_PORT_MIS1,data); // 【要修正】MAGNARO引き渡し後発見。 モード遷移時にMIS1の電源が勝手にONになってしまう。
//				c2a_delay_ms(50);
			}
			PWR_State_SET(num,data);
#ifdef TWO_U
			data ==1 ? cobc_driver->cobc_on_off_flag = 1 : cobc_driver->cobc_on_off_flag = 0;
			data== 1 ? ANOMALY_UART_Set_Enable(UART_FAULT_COBC) : ANOMALY_UART_Reset_Enable(UART_FAULT_COBC);
			data== 1 ? ANOMALY_SPI_Set_Enable(SPI_FAULT_COBC) : ANOMALY_SPI_Reset_Enable(SPI_FAULT_COBC);
#endif
			ack = ioex_class.digitalWrite(IOEX_PORT_MIS1,data == 0); // 【要修正】MAGNARO引き渡し後発見。 モード遷移時にMIS1の電源が勝手にONになってしまう。
//			if(data == 0) {c2a_delay_ms(600);}  // 計測値 Otsuki
			break;
#ifdef TWO_U
		case PWR_State_MAGS:
//			data==1 ? printf("SEMP Power ON\r\n") : printf("SEMP Power OFF\r\n");
			data==1 ? print(PWR_OPERATION,"MAGS Power ON\r\n") : print(PWR_OPERATION,"MAGS Power OFF\r\n");
			if(data == 1){
				ack = ioex_class.digitalWrite(IOEX_PORT_MAGS,data);
//				c2a_delay_ms(50);
			}
			PWR_State_SET(num,data);

			// 通信アノマリ処理 未実装

			ack = ioex_class.digitalWrite(IOEX_PORT_MAGS,data == 0);
			// 電圧計測用Delay（これがないと電圧が中途半端になり、電圧計測アノマリに引っかかる恐れがある）
			// 未計測 ON:　ms,OFF:　m
//			if(data == 0) {c2a_delay_ms(600);}
			break;
#endif
		}
		ANOMALY_I2C_Set_Status(I2C_FAULT_ADCIOEX, (HAL_StatusTypeDef)ack);
	}
}

void PWR_Operate_ONInit(PWR_STATE num){
		switch(num){
		case PWR_State_LORA:
			PWR_Operate_ON_OFF(PWR_State_LORA,1);
			COM_LoRa_init();
			break;
		case PWR_State_IOEX_PWR2:
			PWR_Operate_ON_OFF(PWR_State_IOEX_PWR2,1);
		    c2a_delay_ms(20);
		    print(PWR_OPERATION,"IOEX/ADC_PWR2 Initialize \r\n");
		    ioex_class.init();
		    c2a_delay_ms(10);
		    adc_class.INIT();
		    PWR_Operate_IOEX_init();    // この処理をしないとIOEX初期化後にトリップ付きFETが全てEnableされてしまう　2023/08/09 Kanie
		    break;
		case PWR_State_MTQA:
			PWR_Operate_ON_OFF(PWR_State_MTQA,1);
			c2a_delay_ms(50);
//			printf("MTQA Initialize \r\n");
			print(PWR_OPERATION,"MTQA Initialize \r\n");
			MTQ_Class.MTQA_init();  //FETON後MTQAを零点出力
			break;
		case PWR_State_MTQO_PLASMA:
			PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,1);
			c2a_delay_ms(50);
//			printf("MTQO Initialize \r\n");
			print(PWR_OPERATION,"MTQO PLASMA Initialize \r\n");
#ifdef TWO_U
			MTQ_Class.MTQO_init();  //FETON後MTQOを零点出力
#endif
			ACS23_PLASMA_init();
			break;
		case PWR_State_MIS1:
			PWR_Operate_ON_OFF(PWR_State_MIS1,1);
#ifdef TWO_U
			c2a_delay_ms(50);
			gpios_out[RESET3V3_MOBCtoCOBC__HS_D_SC].WriteL();
			gpios_out[BOOT1_3V3_MOBCtoCOBC].WriteH();
			c2a_delay_ms(10);
			gpios_out[RESET3V3_MOBCtoCOBC__HS_D_SC].WriteH();
			gpios_out[BOOT1_3V3_MOBCtoCOBC].WriteL();
#endif
		}
}

void PWR_Operate_Restart(PWR_STATE num){
	PWR_Operate_ON_OFF(num,0);
//	c2a_delay_ms(1000);

	switch(num){
	case PWR_State_GPS:
		PWR_Operate_ON_OFF(num,1);
		break;
	case PWR_State_SUNS:
		PWR_Operate_ON_OFF(num,1);
		break;
	case PWR_State_LORA:
		PWR_Operate_ONInit(num);
		break;
	case PWR_State_IOEX_PWR2:
		PWR_Operate_ONInit(num);
		break;
	case PWR_State_MTQA:
		PWR_Operate_ONInit(num);
		break;
	case PWR_State_MTQO_PLASMA:
		PWR_Operate_ONInit(num);
		break;
	case PWR_State_MIS1:
		PWR_Operate_ONInit(num);
		break;
	}
}

// MCP23017(IOEX)の初期化後にACTIVE_LOWのFET（トリップ付きFET）のGATE線を初期化する関数　2023/08/09 Kanie
void PWR_Operate_IOEX_init(){
    PWR_Operate_ON_OFF(PWR_State_SUNS_MXPZ,PWR_VOTE(PWR_State_SUNS_MXPZ));
    PWR_Operate_ON_OFF(PWR_State_SUNS_PXPYMY,PWR_VOTE(PWR_State_SUNS_PXPYMY));
    PWR_Operate_ON_OFF(PWR_State_GYRO,PWR_VOTE(PWR_State_GYRO));
    PWR_Operate_ON_OFF(PWR_State_SEMP,PWR_VOTE(PWR_State_SEMP));
    PWR_Operate_ON_OFF(PWR_State_MIS1,PWR_VOTE(PWR_State_MIS1));
#ifdef TWO_U
    PWR_Operate_ON_OFF(PWR_State_MAGS,PWR_VOTE(PWR_State_MAGS));
#endif
}