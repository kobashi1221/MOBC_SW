/*
 * IF_Instances.h
 *
 *  Created on: 2021/06/25
 *      Author: kahaku
 */

#ifndef C2A_APPLICATIONS_IF_INSTANCES_H_
#define C2A_APPLICATIONS_IF_INSTANCES_H_



#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

#define HIGH 0x01
#define LOW 0x00

typedef enum{
	//GATE3V3
	GATE3V3_MOBCtoFET_GPS,
	GATE3V3_MOBCtoFET_SUNS,
	GATE3V3_MOBCtoFET_ADCIOEX_PWR2,
	GATE3V3_MOBCtoFET_MTQA,
	GATE3V3_MOBCtoFET_MTQO_PLASMA,
	GATE3V3_MOBCtoFET_SEPW1,
	GATE3V3_MOBCtoFET_SEPW2,
	GATE3V3_MOBCtoFET_LORA,

	//REG_CDH3V3
	EN3V3_MOBCtoREG_CDH3V3,

	//GYRO
	RESET3V3_MOBCtoGYRO,
	SEL0_3V3_MOBCtoGYRO,
	SEL1_3V3_MOBCtoGYRO,

	//GPS
	RESET3V3_MOBCtoGPS,

	//LORA
	RESET3V3_MOBCtoLORA,

	//SOFTUART TXPIC
	TX_SOFTUART_MOBCtoTXPIC_M,

#ifdef TWO_U
	//MUX
	A0_OD5V_MOBCtoMUX,
	A1_OD5V_MOBCtoMUX,
	A2_OD5V_MOBCtoMUX,
	A3_OD5V_MOBCtoMUX,
	A4_OD5V_MOBCtoMUX,
	CTL_OD5V_MOBCtoMUX_ANAS,
	CTL_OD5V_MOBCtoMUX_TEMS,

	//ADC OUT 2U
	A0_3V3_MOBCtoADC_ACS1__D0_D_SC,
	AB_3V3_MOBCtoADC_ACS1__D1_D_SC,
	BYTESWAP3V3_MOBCtoADC_ACS1__D2_D_SC,
	CNVST3V3_MOBCtoADC_ACS1,
	RESET3V3_MOBCtoADC_ACS1__D3_D_SC,

	//COBC 2U
	RESET3V3_MOBCtoCOBC__HS_D_SC,
	BOOT1_3V3_MOBCtoCOBC,

	//Delta 2U
	GPIO3V3_4_MOBCtoDelta,
	GPIO3V3_5_MOBCtoDelta,
	GPIO3V3_6_MOBCtoDelta,
	GPIO3V3_7_MOBCtoDelta,
	GPIO3V3_8_MOBCtoDelta,
	GPIO3V3_9_MOBCtoDelta,
	GPIO3V3_10_MOBCtoDelta,

	//MOD 2U
	MOD_GPIO_MOBC_1,
	MOD_GPIO_MOBC_2,
	MOD_GPIO_MOBC_5,
	MOD_GPIO_MOBC_6,
#endif
#ifdef ONE_U
	//SCAM 1U
	RESET3V3_SCAM1U,
	STANDBY3V3_SCAM1U,
	TX_SOFTUART3V3_GPS,

#endif

	GPIO_OUT_MAX
}GPIO_OUT_PINS;


typedef enum{
	//GPS 1U2U
	PPS3V3_GPStoMOBC,

	//SOFTUART TXPIC 1U2U
	RX_SOFTUART_TXPIC,

	//LORA 1U2U
	DIO0_3V3_LORAtoMOBC,
	DIO1_3V3_LORAtoMOBC,
	DIO2_3V3_LORAtoMOBC,
	DIO3_3V3_LORAtoMOBC,
	DIO4_3V3_LORAtoMOBC,
	DIO5_3V3_LORAtoMOBC,

#ifdef TWO_U
	BUSY3V1_ADC_ACS1toMOBC,
	EOC3V1_ADC_ACS1toMOBC,
	D0_3V1_ADC_ACS1toMOBC,
	D1_3V1_ADC_ACS1toMOBC,
	D2_3V1_ADC_ACS1toMOBC,
	D3_3V1_ADC_ACS1toMOBC,
	D4_3V1_ADC_ACS1toMOBC,
	D5_3V1_ADC_ACS1toMOBC,
	D6_3V1_ADC_ACS1toMOBC,
	D7_3V1_ADC_ACS1toMOBC,
	D8_3V1_ADC_ACS1toMOBC,
	D9_3V1_ADC_ACS1toMOBC,
	D10_3V1_ADC_ACS1toMOBC,
	D11_3V1_ADC_ACS1toMOBC,
	D12_3V1_ADC_ACS1toMOBC,
	D13_3V1_ADC_ACS1toMOBC,
	D14_3V1_ADC_ACS1toMOBC,
	D15_3V1_ADC_ACS1toMOBC,

	//BOOT 2U
	BOOT1_3V3_EXTtoMOBC,
#endif
#ifdef ONE_U
	//GPS 1U
	RX_SOFTUART3V3_GPS,
#endif
	GPIO_IN_MAX
}GPIO_IN_PINS;


#ifdef __cplusplus
 extern "C" {
#endif

 AppInfo IF_instances_update(void);

#ifdef __cplusplus
 }
#endif


//extern GPIO_Class* p_gpios_out;
//extern GPIO_Class* p_gpios_in;

//extern GPIO_Class* gpios_out;
//extern GPIO_Class* gpios_in;

#endif /* C2A_APPLICATIONS_IF_INSTANCES_H_ */