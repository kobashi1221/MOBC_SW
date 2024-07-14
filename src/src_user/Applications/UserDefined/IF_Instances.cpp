/*
 * IF_Instances.cpp
 *
 *  Created on: 2021/06/25
 *      Author: kahaku
 */

#include "IF_Instances.h"
#include <src_user/IfWrapper/IF_class.h>
#include <src_user/IfWrapper/HAL.h>

//GPIO_Class* gpios_out = new GPIO_Class[GPIO_OUT_MAX];
//GPIO_Class* gpios_in = new GPIO_Class[GPIO_IN_MAX];

GPIO_Class gpios_out[GPIO_OUT_MAX];
GPIO_Class gpios_in[GPIO_OUT_MAX];

void IF_instances_init(void);


/**
 * @brief アプリケーション作成関数
 *
 * @note
 * @return アプリケーション構造体
 */
AppInfo IF_instances_update(void)
{
    return AI_create_app_info("IF_INSTANCE_INIT", IF_instances_init, NULL);
}


void IF_instances_init(void){

//	p_gpios_out = gpios_out;
//	p_gpios_in = gpios_in;

	//GATE3V3 1U2U
	gpios_out[GATE3V3_MOBCtoFET_GPS].SetPortAndPin(GATE3V3_MOBCtoFET_GPS_GPIO_Port,GATE3V3_MOBCtoFET_GPS_Pin);
//	gpios_out[GATE3V3_MOBCtoFET_SUNS].SetPortAndPin(GATE3V3_MOBCtoFET_SUNS_GPIO_Port,GATE3V3_MOBCtoFET_SUNS_Pin);
	gpios_out[GATE3V3_MOBCtoFET_ADCIOEX_PWR2].SetPortAndPin(GATE3V3_MOBCtoFET_ADCIOEX_PWR2_GPIO_Port, GATE3V3_MOBCtoFET_ADCIOEX_PWR2_Pin);
	gpios_out[GATE3V3_MOBCtoFET_MTQA].SetPortAndPin(GATE3V3_MOBCtoFET_MTQA_GPIO_Port, GATE3V3_MOBCtoFET_MTQA_Pin);
#ifdef TWO_U
	gpios_out[GATE3V3_MOBCtoFET_MTQO_PLASMA].SetPortAndPin(GATE3V3_MOBCtoFET_MTQO_GPIO_Port, GATE3V3_MOBCtoFET_MTQO_Pin); // only 2U
//	gpios_out[GATE3V3_MOBCtoFET_SEPW1].SetPortAndPin(GATE3V3_MOBCtoFET_SEPW1_GPIO_Port, GATE3V3_MOBCtoFET_SEPW1_Pin);// only 2U
//	gpios_out[GATE3V3_MOBCtoFET_SEPW2].SetPortAndPin(GATE3V3_MOBCtoFET_SEPW2_GPIO_Port, GATE3V3_MOBCtoFET_SEPW2_Pin);// only 2U
#endif
#ifdef ONE_U
	gpios_out[GATE3V3_MOBCtoFET_MTQO_PLASMA].SetPortAndPin(GATE3V3_MOBCtoFET_PLASMA_GPIO_Port, GATE3V3_MOBCtoFET_PLASMA_Pin); // only 1U
#endif
	gpios_out[GATE3V3_MOBCtoFET_LORA].SetPortAndPin(GATE3V3_MOBCtoFET_LORA_GPIO_Port, GATE3V3_MOBCtoFET_LORA_Pin);

	//REG_CDH3V3 1U2U
	gpios_out[EN3V3_MOBCtoREG_CDH3V3].SetPortAndPin(EN3V3_MOBCtoREG_CDH3V3_GPIO_Port, EN3V3_MOBCtoREG_CDH3V3_Pin);

	//GYRO 1U2U
	gpios_out[RESET3V3_MOBCtoGYRO].SetPortAndPin(RESET3V3_MOBCtoGYRO_GPIO_Port, RESET3V3_MOBCtoGYRO_Pin);
	gpios_out[SEL0_3V3_MOBCtoGYRO].SetPortAndPin(SEL0_3V3_MOBCtoGYROX_GPIO_Port, SEL0_3V3_MOBCtoGYROX_Pin);
	gpios_out[SEL1_3V3_MOBCtoGYRO].SetPortAndPin(SEL1_3V3_MOBCtoGYROX_GPIO_Port, SEL1_3V3_MOBCtoGYROX_Pin);

	//GPS 1U2U
	gpios_out[RESET3V3_MOBCtoGPS].SetPortAndPin(RESET3V3_MOBCtoGPS_GPIO_Port, RESET3V3_MOBCtoGPS_Pin);

	//LORA 1U2U
	gpios_out[RESET3V3_MOBCtoLORA].SetPortAndPin(RESET3V3_MOBCtoLORA_GPIO_Port, RESET3V3_MOBCtoLORA_Pin);

	//SOFTUART TXPIC 1U2U
//	gpios_out[TX_SOFTUART_MOBCtoTXPIC_M].SetPortAndPin(TX_SOFTUART_MOBCtoTXPIC_M_GPIO_Port, TX_SOFTUART_MOBCtoTXPIC_M_Pin);

	//GPS 1U2U
	gpios_in[PPS3V3_GPStoMOBC].SetPortAndPin(PPS3V3_GPStoMOBC_GPIO_Port, PPS3V3_GPStoMOBC_Pin);

	//SOFTUART TXPIC 1U2U
//	gpios_in[RX_SOFTUART_TXPIC].SetPortAndPin(RX_SOFTUART_TXPICtoMOBC_M_GPIO_Port, RX_SOFTUART_TXPICtoMOBC_M_Pin);

	//LORA 1U2U
	gpios_in[DIO0_3V3_LORAtoMOBC].SetPortAndPin(DIO0_3V3_LORAtoMOBC_GPIO_Port, DIO0_3V3_LORAtoMOBC_Pin);
	gpios_in[DIO1_3V3_LORAtoMOBC].SetPortAndPin(DIO1_3V3_LORAtoMOBC_GPIO_Port, DIO1_3V3_LORAtoMOBC_Pin);
	gpios_in[DIO2_3V3_LORAtoMOBC].SetPortAndPin(DIO2_3V3_LORAtoMOBC_GPIO_Port, DIO2_3V3_LORAtoMOBC_Pin);
	gpios_in[DIO3_3V3_LORAtoMOBC].SetPortAndPin(DIO3_3V3_LORAtoMOBC_GPIO_Port, DIO3_3V3_LORAtoMOBC_Pin);
	gpios_in[DIO4_3V3_LORAtoMOBC].SetPortAndPin(DIO4_3V3_LORAtoMOBC_GPIO_Port, DIO4_3V3_LORAtoMOBC_Pin);
	gpios_in[DIO5_3V3_LORAtoMOBC].SetPortAndPin(DIO5_3V3_LORAtoMOBC_GPIO_Port, DIO5_3V3_LORAtoMOBC_Pin);


#ifdef TWO_U
	//MUX 2U
		    gpios_out[A0_OD5V_MOBCtoMUX].SetPortAndPin(A0_OD3V3_MOBCtoMUX_GPIO_Port, A0_OD3V3_MOBCtoMUX_Pin);
		    gpios_out[A1_OD5V_MOBCtoMUX].SetPortAndPin(A1_OD3V3_MOBCtoMUX_GPIO_Port, A1_OD3V3_MOBCtoMUX_Pin);
		    gpios_out[A2_OD5V_MOBCtoMUX].SetPortAndPin(A2_OD3V3_MOBCtoMUX_GPIO_Port, A2_OD3V3_MOBCtoMUX_Pin);
		    gpios_out[A3_OD5V_MOBCtoMUX].SetPortAndPin(A3_OD3V3_MOBCtoMUX_GPIO_Port, A3_OD3V3_MOBCtoMUX_Pin);
		    gpios_out[A4_OD5V_MOBCtoMUX].SetPortAndPin(A4_OD3V3_MOBCtoMUX_GPIO_Port, A4_OD3V3_MOBCtoMUX_Pin);
		    gpios_out[CTL_OD5V_MOBCtoMUX_ANAS].SetPortAndPin(CTL_OD3V3_MOBCtoMUX_ANAS_GPIO_Port, CTL_OD3V3_MOBCtoMUX_ANAS_Pin);
		    gpios_out[CTL_OD5V_MOBCtoMUX_TEMS].SetPortAndPin(CTL_OD3V3_MOBCtoMUX_TEMS_GPIO_Port, CTL_OD3V3_MOBCtoMUX_TEMS_Pin);

		    //ADC OUT 2U
		    gpios_out[A0_3V3_MOBCtoADC_ACS1__D0_D_SC].SetPortAndPin(A0_3V3_MOBCtoADC_ACS1_GPIO_Port, A0_3V3_MOBCtoADC_ACS1_Pin);
		    gpios_out[AB_3V3_MOBCtoADC_ACS1__D1_D_SC].SetPortAndPin(AB_3V3_MOBCtoADC_ACS1_GPIO_Port, AB_3V3_MOBCtoADC_ACS1_Pin);
		    gpios_out[BYTESWAP3V3_MOBCtoADC_ACS1__D2_D_SC].SetPortAndPin(BYTESWAP3V3_MOBCtoADC_ACS1_GPIO_Port, BYTESWAP3V3_MOBCtoADC_ACS1_Pin);
		    gpios_out[CNVST3V3_MOBCtoADC_ACS1].SetPortAndPin(CNVST3V3_MOBCtoADC_ACS1_GPIO_Port, CNVST3V3_MOBCtoADC_ACS1_Pin);
		    gpios_out[RESET3V3_MOBCtoADC_ACS1__D3_D_SC].SetPortAndPin(RESET3V3_MOBCtoADC_ACS1_GPIO_Port, RESET3V3_MOBCtoADC_ACS1_Pin);

		    //COBC 2U
		   	gpios_out[RESET3V3_MOBCtoCOBC__HS_D_SC].SetPortAndPin(RESET3V3_MOBCtoCOBC_GPIO_Port, RESET3V3_MOBCtoCOBC_Pin);
//		   	gpios_out[BOOT1_3V3_MOBCtoCOBC].SetPortAndPin(BOOT1_3V3_MOBCtoCOBC_GPIO_Port, BOOT1_3V3_MOBCtoCOBC_Pin);

		    //Delta 2U
		    gpios_out[GPIO3V3_4_MOBCtoDelta].SetPortAndPin(GPIO3V3_4_MOBCtoDelta_GPIO_Port, GPIO3V3_4_MOBCtoDelta_Pin);
		    gpios_out[GPIO3V3_5_MOBCtoDelta].SetPortAndPin(GPIO3V3_5_MOBCtoDelta_GPIO_Port, GPIO3V3_5_MOBCtoDelta_Pin);
//		    gpios_out[GPIO3V3_6_MOBCtoDelta].SetPortAndPin(GPIO3V3_6_MOBCtoDelta_GPIO_Port, GPIO3V3_6_MOBCtoDelta_Pin); //1U2U
//		    gpios_out[GPIO3V3_7_MOBCtoDelta].SetPortAndPin(GPIO3V3_7_MOBCtoDelta_GPIO_Port, GPIO3V3_7_MOBCtoDelta_Pin);
//		    gpios_out[GPIO3V3_8_MOBCtoDelta].SetPortAndPin(GPIO3V3_8_MOBCtoDelta_GPIO_Port, GPIO3V3_8_MOBCtoDelta_Pin);
		    gpios_out[GPIO3V3_9_MOBCtoDelta].SetPortAndPin(GPIO3V3_9_MOBCtoDelta_GPIO_Port, GPIO3V3_9_MOBCtoDelta_Pin);
//		    gpios_out[GPIO3V3_10_MOBCtoDelta].SetPortAndPin(GPIO3V3_10_MOBCtoDelta_GPIO_Port, GPIO3V3_10_MOBCtoDelta_Pin);

		    //MOD 2U
//		    gpios_out[MOD_GPIO_MOBC_1].SetPortAndPin(MOD_GPIO_MOBC_1_GPIO_Port, MOD_GPIO_MOBC_1_Pin);
//		    gpios_out[MOD_GPIO_MOBC_2].SetPortAndPin(MOD_GPIO_MOBC_2_GPIO_Port, MOD_GPIO_MOBC_2_Pin);
		    gpios_out[MOD_GPIO_MOBC_6].SetPortAndPin(MOD_GPIO_MOBC_6_GPIO_Port, MOD_GPIO_MOBC_6_Pin);

		    //ADC IN 2U
		    gpios_in[BUSY3V1_ADC_ACS1toMOBC].SetPortAndPin(BUSY3V3_ADC_ACS1toMOBC_GPIO_Port, BUSY3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[EOC3V1_ADC_ACS1toMOBC].SetPortAndPin(EOC3V3_ADC_ACS1toMOBC_GPIO_Port, EOC3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D0_3V1_ADC_ACS1toMOBC].SetPortAndPin(D0_3V3_ADC_ACS1toMOBC_GPIO_Port, D0_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D1_3V1_ADC_ACS1toMOBC].SetPortAndPin(D1_3V3_ADC_ACS1toMOBC_GPIO_Port, D1_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D2_3V1_ADC_ACS1toMOBC].SetPortAndPin(D2_3V3_ADC_ACS1toMOBC_GPIO_Port, D2_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D3_3V1_ADC_ACS1toMOBC].SetPortAndPin(D3_3V3_ADC_ACS1toMOBC_GPIO_Port, D3_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D4_3V1_ADC_ACS1toMOBC].SetPortAndPin(D4_3V3_ADC_ACS1toMOBC_GPIO_Port, D4_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D5_3V1_ADC_ACS1toMOBC].SetPortAndPin(D5_3V3_ADC_ACS1toMOBC_GPIO_Port, D5_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D6_3V1_ADC_ACS1toMOBC].SetPortAndPin(D6_3V3_ADC_ACS1toMOBC_GPIO_Port, D6_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D7_3V1_ADC_ACS1toMOBC].SetPortAndPin(D7_3V3_ADC_ACS1toMOBC_GPIO_Port, D7_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D8_3V1_ADC_ACS1toMOBC].SetPortAndPin(D8_3V3_ADC_ACS1toMOBC_GPIO_Port, D8_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D9_3V1_ADC_ACS1toMOBC].SetPortAndPin(D9_3V3_ADC_ACS1toMOBC_GPIO_Port, D9_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D10_3V1_ADC_ACS1toMOBC].SetPortAndPin(D10_3V1_ADC_ACS1toMOBC_GPIO_Port, D10_3V1_ADC_ACS1toMOBC_Pin);
		    gpios_in[D11_3V1_ADC_ACS1toMOBC].SetPortAndPin(D11_3V1_ADC_ACS1toMOBC_GPIO_Port, D11_3V1_ADC_ACS1toMOBC_Pin);
		    gpios_in[D12_3V1_ADC_ACS1toMOBC].SetPortAndPin(D12_3V3_ADC_ACS1toMOBC_GPIO_Port, D12_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D13_3V1_ADC_ACS1toMOBC].SetPortAndPin(D13_3V3_ADC_ACS1toMOBC_GPIO_Port, D13_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D14_3V1_ADC_ACS1toMOBC].SetPortAndPin(D14_3V3_ADC_ACS1toMOBC_GPIO_Port, D14_3V3_ADC_ACS1toMOBC_Pin);
		    gpios_in[D15_3V1_ADC_ACS1toMOBC].SetPortAndPin(D15_3V3_ADC_ACS1toMOBC_GPIO_Port, D15_3V3_ADC_ACS1toMOBC_Pin);

#endif


#ifdef ONE_U

	    //SCAM 1U
	    gpios_out[RESET3V3_SCAM1U].SetPortAndPin(RESET3V3_SCAM_GPIO_Port, RESET3V3_SCAM_Pin);
	    gpios_out[STANDBY3V3_SCAM1U].SetPortAndPin(PWDN3V3_SCAM1U_GPIO_Port, PWDN3V3_SCAM1U_Pin);
//	    gpios_out[TX_SOFTUART3V3_GPS].SetPortAndPin(TX_SOFTUART3V3_GPS_GPIO_Port, TX_SOFTUART3V3_GPS_Pin); //FM時に削除(坂口)

	    //GPS 1U
//	    gpios_in[RX_SOFTUART3V3_GPS].SetPortAndPin(RX_SOFTUART3V3_GPS_GPIO_Port, RX_SOFTUART3V3_GPS_Pin); //1U //FM時に削除(坂口)

#endif




	    /*only 1U
	    out
	    RESET3V3_SCAM1U
	    STANDBY3V3_SCAM1U
		TX_SOFTUART3V3_GPS
	    in
	    RX_SOFTUART3V3_GPS

	    */


}
