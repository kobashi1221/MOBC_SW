/*
 * PWR_cmd_ADC_PWR2.c
 *
 *  Created on: 2021/11/20
 *      Author: SAT63
 */


#include "PWR_cmd_ADC_PWR2.h"
#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <string.h>

#include <src_user/Drivers/Power/MCP23017.h>
//#include <src_user/Drivers/PWR/MCP3421.h>
#include <src_user/IfWrapper/IF_class.h>
#include "../Global_Sensor_Value.h"
#include "src_user/Library/Delay.h"
#include "../IF_Instances.h"
#include "PWR_cmd_FETREG.h"
#include <src_user/Drivers/Power/MCP3421.h>
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include <src_user/Library/General/DBG_printf.h>
#include "PWR_cmd_AnomalyDetect.h"
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_user/Drivers/Cdh/Com_Fault_Detection_driver.h>
#include <src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.h>
#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

extern MCP23017 mcp23017_;  //

#define MCP3421_ADDR 0xD0
#define MCP3421_PWR2_ADDR 0xD4

static PWR_ADC_PWR2_STRUCT pwr_adc_pwr2;
//static PWR_FETREG_STRUCT pwr_fetreg;

MCP3421 adc1(MCP3421_PWR2_ADDR, &hi2c2); //ADC_PWR2 Object

static void pwr_ADC_PWR2_init(void);  // アプリ初期化のプロトタイプ線源
//void SELECT_PORT(MUX_PWR2_PORT port);
static void ADC_PWR2_initialize_param(void);
static void ADC_PWR2_load_param(void);

// MRAM parameter


AppInfo PWR_ADC_PWR2_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("ADC_PWR2", pwr_ADC_PWR2_init, NULL);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void pwr_ADC_PWR2_init(void)
{
	printf("PWR ADC_PWR2 App initialize.\n");
	g_sensor.p_pwr_adc_pwr2 = &pwr_adc_pwr2;
    adc1.INIT();
    c2a_delay_ms(10);
}

/**
 * @brief MRAMアプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo PWR_ADC_PWR2_Param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("MRAM_ADC_PWR2", ADC_PWR2_load_param, ADC_PWR2_initialize_param);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void ADC_PWR2_initialize_param(void)
{
	printf("ADC_PWR2 initialize param\r\n");

}

/**
 * @brief アプリケーション実行関数
 *
 */
static void ADC_PWR2_load_param(void)
{

}

/**
 * @brief Initialization of ADC_PWR2
 *
 */
CCP_CmdRet Cmd_PWR_ADC_PWR2_Init(const CommonCmdPacket* packet){
    (void)packet;
    print(PWR_ADC_PWR2,"ADC_INIT\r\n");
    g_sensor.p_pwr_adc_pwr2 = &pwr_adc_pwr2;
    adc1.INIT();
    c2a_delay_ms(10);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//BC_Rotate用に作ったコマンドなので，普通には使わないこと！
//ピン設定とADCの順番を高速化のため意図的にずらしてます　Noro
CCP_CmdRet Cmd_PWR_ADC_PWR2_Read(const CommonCmdPacket* packet){
	size_t len=CCP_get_param_len(packet);
	const uint8_t *head_param = CCP_get_param_head(packet);
	uint8_t param[1];
	memcpy(param,head_param,len);

	// MUX切り替え時に，FET操作用のGPIOのH/Lが変化しないようにするための処理
	uint8_t selector = param[0] & 0x0f;		// CMD引数からMUXに送るアドレスを抽出
	uint8_t state = mcp23017_.readPort(MCP23017Port::A) & 0xf0;		// 現在のポートAの状態のうち，MUXに関係ないピン(FETに使うピン)の状態を取得
	uint8_t write_value = selector | state;

	mcp23017_.writePort(MCP23017Port::A, write_value);
	c2a_delay_ms(10); // c2a_delay_ms(7)だと稀に異常値となる （2U）
	uint32_t rawData = 0;
	uint16_t rawD = 0;
	HAL_StatusTypeDef i2c_status = HAL_OK;
	float data = adc1.readADC(&rawData,&i2c_status);
//	printf("ic2_status : %d\r\n",i2c_status);

	ANOMALY_I2C_Set_Status(I2C_FAULT_ADCIOEX,i2c_status);  // 通信アノマリ検知
	if(i2c_status != 0){
		rawD = 0xfff; // 通信ができない場合は4095を返す
//		printf("error of communication to ADC_PWR2, HK data is not correct.\r\n");
		pwr_adc_pwr2.ADC_I_REG_CDH3V3 = rawD;
		pwr_adc_pwr2.ADC_V_HEAT3 = rawD;
		pwr_adc_pwr2.ADC_V_HEAT4 = rawD;
		pwr_adc_pwr2.ADC_V_REG_CDH3V3 = rawD;
		pwr_adc_pwr2.ADC_V_REG_ACS2_5V = rawD;
        pwr_adc_pwr2.ADC_V_REG_ACS1_5V = rawD;
		pwr_adc_pwr2.ADC_V_SUNS_MXPZ = rawD;
		pwr_adc_pwr2.ADC_V_SUNS_PXPYMY = rawD;
		pwr_adc_pwr2.ADC_I_SEMP = rawD;
		pwr_adc_pwr2.ADC_V_SEMP = rawD;
        pwr_adc_pwr2.ADC_I_SEPC = rawD;
        pwr_adc_pwr2.ADC_V_SEPC1 = rawD;
        pwr_adc_pwr2.ADC_V_SEPC2 = rawD;
        pwr_adc_pwr2.ADC_V_SEPC3 = rawD;
        pwr_adc_pwr2.ADC_V_SEPC4 = rawD;
		pwr_adc_pwr2.ADC_V_MIS1 = rawD;
#ifdef TWO_U
		pwr_adc_pwr2.ADC_V_MAGS = rawD;
#endif

		print(PWR_ANOMALY,"AWC: %d\r\n", g_pwr_awc);
		// C2Aサイクル中 PWR2 ADC のここ一か所でのみ ディクリメント
		if(g_pwr_awc > 0) {
			PWR_Decrement_AWC();
		}

		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}
	else{
	uint8_t state;  // for getting power state considered by MOBC

//		uint8_t state;  // for getting power state considered by MOBC
		rawD = (uint16_t) rawData;

		switch(param[0]){
		case PORT_CURS_REG_CDH3V3:
			pwr_adc_pwr2.ADC_I_REG_CDH3V3 = rawD;  //LT8613
			AD_CURS(AL_CURS_REG_CDH3V3,rawD,&g_pwr_thres->AT_CURS_REG_CDH3V3);
			print(PWR_ADC_PWR2, "ADC_I_REG_CDH3V3 raw data: %d\r\n",rawD);
			print(PWR_ADC_PWR2, "ADC_I_REG_CDH3V3 current: %d mA\r\n",rawD*2);
			break;
		case PORT_VOLS_GYRO:
			pwr_adc_pwr2.ADC_V_GYRO = rawD;
			print(PWR_ADC_PWR2, "VOLS_GYRO raw data: %d\r\n",rawD);

			// 電源アノマリー処理

			break;
		case PORT_VOLS_REG_CDH3V3:
			pwr_adc_pwr2.ADC_V_REG_CDH3V3 = rawD;
			state = PWR_State_GET(PWR_State_REG_CDH3V3);
			AD_VOLS_REG_CONTROL(AL_VOLS_REG_CDH3V3,rawD,g_pwr_thres->AT_VOLS_REG_CDH3V3,state);

			print(PWR_ADC_PWR2, "ADC_VOLS_REG_CDH3V3 : %d\r\n",rawD);
			print(PWR_ADC_PWR2, "ADC_VOLS_REG_CDH3V3 OV_THRES : %d\r\n",g_pwr_thres->AT_VOLS_REG_CDH3V3[0]);
			print(PWR_ADC_PWR2, "ADC_VOLS_REG_CDH3V3 UV_THRES : %d\r\n",g_pwr_thres->AT_VOLS_REG_CDH3V3[1]);
			print(PWR_ADC_PWR2, "ADC_VOLS_REG_CDH3V3 PSI_THRES : %d\r\n",g_pwr_thres->AT_VOLS_REG_CDH3V3[2]);
			print(PWR_ADC_PWR2, "ADC_VOLS_REG_CDH3V3 State: %d\r\n",state);
			break;
		case PORT_VOLS_REG_ACS2_5V:
			pwr_adc_pwr2.ADC_V_REG_ACS2_5V = rawD;
			AD_VOLS_REG_UNCONTROL(AL_VOLS_REG_ACS2_5V,rawD,g_pwr_thres->AT_VOLS_REG_ACS2_5V);

			print(PWR_ADC_PWR2, "ADC_VOLS_REG_ACS2_5V : %d\r\n",rawD);
			print(PWR_ADC_PWR2, "ADC_VOLS_REG_ACS2_5V OV_THRES : %d\r\n",g_pwr_thres->AT_VOLS_REG_ACS2_5V[0]);
			print(PWR_ADC_PWR2, "ADC_VOLS_REG_ACS2_5V UV_THRES : %d\r\n",g_pwr_thres->AT_VOLS_REG_ACS2_5V[1]);
			break;
		case PORT_VOLS_SUNS_MXPZ:
			pwr_adc_pwr2.ADC_V_SUNS_MXPZ = rawD;
			print(PWR_ADC_PWR2, "VOLS_SUNS_MXPZ raw data: %d\r\n",rawD);

			// 電源アノマリー処理

			break;
		case PORT_VOLS_SUNS_PXPYMY:
			pwr_adc_pwr2.ADC_V_SUNS_PXPYMY = rawD;
			print(PWR_ADC_PWR2, "VOLS_SUNS_PXPYMY raw data: %d\r\n",rawD);

			// 電源アノマリー処理

			break;
		case PORT_CURS_SEMP:
			pwr_adc_pwr2.ADC_I_SEMP = rawD;
			print(PWR_ADC_PWR2, "CURS_SEMP raw data: %d\r\n",rawD);

			// 電源アノマリー処理

			break;
		case PORT_VOLS_SEMP:
			pwr_adc_pwr2.ADC_V_SEMP = rawD;
			print(PWR_ADC_PWR2, "VOLS_SEMP raw data: %d\r\n",rawD);

			// 電源アノマリー処理

			break;
        case PORT_VOLS_REG_ACS1_5V:
            pwr_adc_pwr2.ADC_V_REG_ACS1_5V = rawD;
            AD_VOLS_REG_UNCONTROL(AL_VOLS_REG_ACS1_5V,rawD,g_pwr_thres->AT_VOLS_REG_ACS1_5V);

            print(PWR_ADC_PWR2,"ADC_VOLS_REG_ACS1_5V : %d\r\n",rawD);
            print(PWR_ADC_PWR2, "ADC_VOLS_REG_ACS1_5V OV_THRES : %d\r\n",g_pwr_thres->AT_VOLS_REG_ACS1_5V[0]);
            print(PWR_ADC_PWR2, "ADC_VOLS_REG_ACS1_5V UV_THRES : %d\r\n",g_pwr_thres->AT_VOLS_REG_ACS1_5V[1]);
            break;
        case PORT_CURS_SEPCOIL:
            pwr_adc_pwr2.ADC_I_SEPC = rawD; //LT6106
            print(PWR_ADC_PWR2,"CURS_SEPCOIL : %d\r\n",rawD);
            AD_CURS(AL_CURS_SEPCOIL,rawD,&g_pwr_thres->AT_CURS_SEPCOIL);
            break;
        case PORT_VOLS_SEPCOIL1:
            pwr_adc_pwr2.ADC_V_SEPC1 = rawD;
            print(PWR_ADC_PWR2,"VOLS_SEPCOIL1  rawData : %d\r\n",rawD);
            state = PWR_State_GET(PWR_State_SEPCOIL1);
            AD_VOLS_FET(AL_VOLS_SEPCOIL1,rawD,g_pwr_thres->AT_VOLS_SEPCOIL,state);
            break;
        case PORT_VOLS_SEPCOIL2:
            pwr_adc_pwr2.ADC_V_SEPC2 = rawD;
            print(PWR_ADC_PWR2,"VOLS_SEPCOIL2  rawData : %d\r\n",rawD);
            state = PWR_State_GET(PWR_State_SEPCOIL2);
            AD_VOLS_FET(AL_VOLS_SEPCOIL1,rawD,g_pwr_thres->AT_VOLS_SEPCOIL,state);
            break;
        case PORT_VOLS_SEPCOIL3:
            pwr_adc_pwr2.ADC_V_SEPC3 = rawD;
            print(PWR_ADC_PWR2,"VOLS_SEPCOIL3  rawData : %d\r\n",rawD);
            state = PWR_State_GET(PWR_State_SEPCOIL3);
            AD_VOLS_FET(AL_VOLS_SEPCOIL1,rawD,g_pwr_thres->AT_VOLS_SEPCOIL,state);
            break;
        case PORT_VOLS_SEPCOIL4:
            pwr_adc_pwr2.ADC_V_SEPC4 = rawD;
            print(PWR_ADC_PWR2,"VOLS_SEPCOIL4  rawData : %d\r\n",rawD);
            state = PWR_State_GET(PWR_State_SEPCOIL4);
            AD_VOLS_FET(AL_VOLS_SEPCOIL1,rawD,g_pwr_thres->AT_VOLS_SEPCOIL,state);
            break;
		case PORT_VOLS_MIS1:
			pwr_adc_pwr2.ADC_V_MIS1 = rawD;
			print(PWR_ADC_PWR2,"VOLS_MIS1  rawData : %d\r\n",rawD);
			state = PWR_State_GET(PWR_State_MIS1);
			if(g_pwr_awc==0){
				AD_VOLS_FET(AL_VOLS_MIS1,rawD,g_pwr_thres->AT_VOLS_MIS1,state);
			}
			break;
#ifdef TWO_U

		case PORT_VOLS_MAGS:
			pwr_adc_pwr2.ADC_V_MAGS = rawD;
			print(PWR_ADC_PWR2, "VOLS_MAGS raw data: %d\r\n",rawD);

			// 電源アノマリー処理

			break;
#endif

		// 使用しない．後ほど削除
		case PORT_VOLS_HEAT3:
			// アノマリ検知はTXPICで実施
			pwr_adc_pwr2.ADC_V_HEAT3 = rawD;
			print(PWR_ADC_PWR2, "PORT_VOLS_HEAT3 raw data: %d\r\n",rawD);
			break;
		// 使用しない．後ほど削除
		case PORT_VOLS_HEAT4:
			// アノマリ検知はTXPICで実施
			pwr_adc_pwr2.ADC_V_HEAT4 = rawD;
			print(PWR_ADC_PWR2, "PORT_VOLS_HEAT4 raw data: %d\r\n",rawD);
			break;

			}

		// C2Aサイクル中 PWR2 ADC のここ一か所でのみ ディクリメント
//		print(PWR_ANOMALY,"AWC: %d\r\n", g_pwr_awc);
		if(g_pwr_awc > 0) {
			PWR_Decrement_AWC();
		}

		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}
}



void SELECT_PORT(MUX_PWR2_PORT port){
    uint8_t selector = port & 0x0f;
    mcp23017_.writePort(MCP23017Port::A, selector);
    c2a_delay_ms(10);
}

//void AD_Convert_PWR2(adc_pwr2_PORT port,uint32_t *rawData){
//    uint8_t selector = port & 0x0f;
//    mcp23017_.writePort(MCP23017Port::A, selector);
//    c2a_delay_ms(10);
//    float data = adc1.readADC(rawData);
//
////    return rawData;
//}
