/*
 * PWR_cmd_AnomalyDetect.cpp
 *
 *  Created on: 2021/12/03
 *      Author: Otsuki
 */

#include "PWR_cmd_AnomalyDetect.h"
#include <stdio.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyHandle.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/DriverInstances/di_txpic.h>
#include <src_user/Applications/DriverInstances/mode_auto_transition.h>
#include <src_user/Applications/DriverInstances/block_command_handler.h>
#include <src_user/Settings/Modes/mode_definitions.h>
#include <src_user/Settings/Modes/Phase/phase_setting.h>

/* MOBCで取得する電流電圧値についてのアノマリ閾値
 * OC : Over Current
 * UC : Under Current
 * OV : Over Voltage
 * UV : Under Voltage
 * PSI : Power State Inconsistency(ON : MOBC think PWR OFF, OFF : MOBC think PWR ON)
 */
static PWR_ANOMALY_THRESHOLD_STRUCT pwr_thres;
const PWR_ANOMALY_THRESHOLD_STRUCT* g_pwr_thres;
const PWR_ANOMALY_THRESHOLD_STRUCT pwr_thres_ = {

		1220,				// CURS_MOBC OC:100mA
		1140,				// CURS_LORA OC:150mA

		1000,				// CURS_REG_CDH3V3 OC:1000[mA]
		1130,				// CURS_SEPCOIL OC:1500mA
		31850,	    		// CURS_MTQ OC:1350[mA]

		// FM 1U MTQ
		{{3880, 2320},		// CURS_MTQX_MOBC OC:190[mA],-190[mA]
		{3880, 2320},		// CURS_MTQY_MOBC OC:190[mA],-190[mA]
		{4050, 2160},		// CURS_MTQZ1Z2_MOBC OC:230[mA],-230[mA]
		{4020, 2160}},		// CURS_MTQZ3Z4_MOBC OC:230[mA],-230[mA]

        // FM 2U MTQ
		{{47190, 18350}, 	// CURS_MTQX_ACS1 OC:110[mA],-110[mA]
		{47190, 18350},		// CURS_MTQY_ACS1 OC:110[mA],-110[mA]
		{61600, 3930},		// CURS_MTQZ1_ACS1 OC:220[mA],-220[mA]
		{61610, 3930},		// CURS_MTQZ2_ACS1 OC:220[mA],-220[mA]
		{61610, 3930},		// CURS_MTQZ3_ACS1 OC:220[mA],-220[mA]
		{61610, 3930}},	    // CURS_MTQZ4_ACS1 OC:220[mA],-220[mA]

		{1860, 310},		// VOLS_GPS UV:3.0[V],PSI:0.5[V]
		{1860, 400},		// VOLS_SUNS UV:3.0[V],PSI:0.64[V] 電圧値があがってしまうことがある
		{1550, 480},		// VOLS_LORA UV:2.5[V],PSI:0.77[V] 電圧値があがってしまうことがある
		{2920, 310},		// VOLS_MTQ_MOBC UV:4.7[V],PSI:0.5[V]
		{1500, 250},		// VOLS_MIS1 UV:3.0[V],PSI:0.5[V]
		{1560, 167},		// VOLS_SEPCOIL UV:4.7V, PSI:0.5V
		{{30800, 3280},		// VOLS_MTQA_ACS1 UV:4.7[V],PSI:0.5[V]
		{30800, 3280}},      // VOLS_MTQO_AACS1 UV:4.7[V],PSI:0.5[V]

		{1750, 1500, 250},	//【FM】 VOLS_REG_CDH3V3 OV:3.5[V],UV:3.0[V],PSI:0.5V
//		{880, 750,125},	//【2ndEM】 VOLS_REG_CDH3V3 OV:3.5[V],UV:3.0[V],PSI:0.5V
		{1730, 1570},		// VOLS_REG_ACS1_5V OV:5.2[V],UV:4.7[V]
		{1730, 1570},		// VOLS_REG_ACS2_5V OV:5.2[V],UV:4.7[V]

};

/* TXPICで取得する電流電圧値についてのアノマリ閾値
 * OC : Over Current
 * UC : Under Current
 * OV : Over Voltage
 * UV : Under Voltage
 */
static PWR_TXPIC_ANOMALY_THRESHOLD_STRUCT pwr_txpic_thres;
const PWR_TXPIC_ANOMALY_THRESHOLD_STRUCT* g_pwr_txpic_thres;
const PWR_TXPIC_ANOMALY_THRESHOLD_STRUCT pwr_txpic_thres_ = {
		0,			// TXPIC_CURS_SAP1 UC:0[mA] 打ち上げ後に0mAより高く設定することも可能
		0,			// TXPIC_CURS_SAP2 UC:0[mA]
		{2170, 1476},	// TXPIC_VOLS_2ndBAT OV:10.0[V],UV:6.8[V]  OVはPICで管理するので到達しえないほど高く設定 田村
//		{1628, 1519} // for test  TXPIC_VOLS_2ndBAT OV:7.4[V],UV:7.0[V]
};

/* FET操作直後など猶予のための アノマリ待機カウンタ
 * 電源系アノマリ検知回避を設定された部分（各センサ取得コードのif文）について、
 * アノマリ検知を行わない期間をC2Aサイクル数を整数で指定
 */
static uint8_t pwr_awc_const;
const uint8_t* g_pwr_awc_const;
const uint8_t pwr_awc_const_ = 1;
uint8_t g_pwr_awc;



static void PWR_Anomaly_Detect_initialize_param(void);
static void PWR_Anomaly_Detect_load_param(void);

AppInfo PWR_Anomaly_Detect_Param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("MRAM_PWR_ANOMALY", PWR_Anomaly_Detect_load_param, PWR_Anomaly_Detect_initialize_param);
}

static void PWR_Anomaly_Detect_initialize_param(void)
{

	printf("PWR Anomaly Detect initialize(write) param\r\n");
	// MOBC's Sensors Anomaly Threshold
	write_param(&pwr_thres.AT_CURS_MOBC, &pwr_thres_.AT_CURS_MOBC, (size_t)(sizeof(pwr_thres_.AT_CURS_MOBC)),(uint32_t)0x00004600);
	write_param(&pwr_thres.AT_CURS_LORA, &pwr_thres_.AT_CURS_LORA, (size_t)(sizeof(pwr_thres_.AT_CURS_LORA)),(uint32_t)0x00004602);

	write_param(&pwr_thres.AT_CURS_REG_CDH3V3, &pwr_thres_.AT_CURS_REG_CDH3V3, (size_t)(sizeof(pwr_thres_.AT_CURS_REG_CDH3V3)),(uint32_t)0x00004618);
	write_param(&pwr_thres.AT_CURS_SEPCOIL, &pwr_thres_.AT_CURS_SEPCOIL, (size_t)(sizeof(pwr_thres_.AT_CURS_SEPCOIL)),(uint32_t)0x0000461A);
	write_param(&pwr_thres.AT_CURS_MTQ, &pwr_thres_.AT_CURS_MTQ, (size_t)(sizeof(pwr_thres_.AT_CURS_SEPCOIL)),(uint32_t)0x0000461C);

	write_param(pwr_thres.AT_CURS_MTQ_MOBC, pwr_thres_.AT_CURS_MTQ_MOBC, (size_t)(sizeof(pwr_thres_.AT_CURS_MTQ_MOBC)),(uint32_t)0x00004646);
	write_param(pwr_thres.AT_CURS_MTQ_ACS1, pwr_thres_.AT_CURS_MTQ_ACS1, (size_t)(sizeof(pwr_thres_.AT_CURS_MTQ_ACS1)),(uint32_t)0x00004656);

	write_param(pwr_thres.AT_VOLS_GPS, pwr_thres_.AT_VOLS_GPS, (size_t)(sizeof(pwr_thres_.AT_VOLS_GPS)),(uint32_t)0x00004682);
	write_param(pwr_thres.AT_VOLS_SUNS, pwr_thres_.AT_VOLS_SUNS, (size_t)(sizeof(pwr_thres_.AT_VOLS_SUNS)),(uint32_t)0x00004686);
	write_param(pwr_thres.AT_VOLS_LORA, pwr_thres_.AT_VOLS_LORA, (size_t)(sizeof(pwr_thres_.AT_VOLS_LORA)),(uint32_t)0x0000468A);
	write_param(pwr_thres.AT_VOLS_MTQ_MOBC, pwr_thres_.AT_VOLS_MTQ_MOBC, (size_t)(sizeof(pwr_thres_.AT_VOLS_MTQ_MOBC)),(uint32_t)0x0000468E);
	write_param(pwr_thres.AT_VOLS_MIS1, pwr_thres_.AT_VOLS_MIS1, (size_t)(sizeof(pwr_thres_.AT_VOLS_MIS1)),(uint32_t)0x00004692);
	write_param(pwr_thres.AT_VOLS_SEPCOIL, pwr_thres_.AT_VOLS_SEPCOIL, (size_t)(sizeof(pwr_thres_.AT_VOLS_SEPCOIL)),(uint32_t)0x00004696);
	write_param(pwr_thres.AT_VOLS_MTQ_ACS1, pwr_thres_.AT_VOLS_MTQ_ACS1, (size_t)(sizeof(pwr_thres_.AT_VOLS_MTQ_ACS1)),(uint32_t)0x0000469A);

	write_param(pwr_thres.AT_VOLS_REG_CDH3V3, pwr_thres_.AT_VOLS_REG_CDH3V3, (size_t)(sizeof(pwr_thres_.AT_VOLS_REG_CDH3V3)),(uint32_t)0x000046B2);
	write_param(pwr_thres.AT_VOLS_REG_ACS1_5V,pwr_thres_.AT_VOLS_REG_ACS1_5V, (size_t)(sizeof(pwr_thres_.AT_VOLS_REG_ACS1_5V)),(uint32_t)0x000046B8);
	write_param(pwr_thres.AT_VOLS_REG_ACS2_5V, pwr_thres_.AT_VOLS_REG_ACS2_5V, (size_t)(sizeof(pwr_thres_.AT_VOLS_REG_ACS2_5V)),(uint32_t)0x000046BC);


	// TXPIC's Sensors Anomaly Threshold
	write_param(&pwr_txpic_thres.AT_CURS_SAP1, &pwr_txpic_thres_.AT_CURS_SAP1, (size_t)(sizeof(pwr_txpic_thres_.AT_CURS_SAP1)),(uint32_t)0x0000471C);
	write_param(&pwr_txpic_thres.AT_CURS_SAP2, &pwr_txpic_thres_.AT_CURS_SAP2, (size_t)(sizeof(pwr_txpic_thres_.AT_CURS_SAP2)),(uint32_t)0x0000471E);
	write_param(pwr_txpic_thres.AT_VOLS_2ndBAT, pwr_txpic_thres_.AT_VOLS_2ndBAT, (size_t)(sizeof(pwr_txpic_thres_.AT_VOLS_2ndBAT)),(uint32_t)0x00004720);


	// Anomaly Waiting Counter
	write_param(&pwr_awc_const, &pwr_awc_const_,(size_t)(sizeof(pwr_awc_const_)),(uint32_t)0x00004724);

//	g_pwr_thres = &pwr_thres;
//	g_pwr_txpic_thres = &pwr_txpic_thres;
//	g_pwr_awc_const = &pwr_awc_const;
}

static void PWR_Anomaly_Detect_load_param(void)
{
	printf("PWR Anomaly Detect load(read) param\r\n");
	// MOBC's Sensors Anomaly Threshold
	read_param(&pwr_thres.AT_CURS_MOBC, &pwr_thres_.AT_CURS_MOBC,(size_t)(sizeof(pwr_thres_.AT_CURS_MOBC)),(uint32_t)0x00004600);
	read_param(&pwr_thres.AT_CURS_LORA, &pwr_thres_.AT_CURS_LORA,(size_t)(sizeof(pwr_thres_.AT_CURS_LORA)),(uint32_t)0x00004602);

	read_param(&pwr_thres.AT_CURS_REG_CDH3V3, &pwr_thres_.AT_CURS_REG_CDH3V3, (size_t)(sizeof(pwr_thres_.AT_CURS_REG_CDH3V3)),(uint32_t)0x00004618);
	read_param(&pwr_thres.AT_CURS_SEPCOIL, &pwr_thres_.AT_CURS_SEPCOIL, (size_t)(sizeof(pwr_thres_.AT_CURS_SEPCOIL)),(uint32_t)0x0000461A);
	read_param(&pwr_thres.AT_CURS_MTQ, &pwr_thres_.AT_CURS_MTQ, (size_t)(sizeof(pwr_thres_.AT_CURS_SEPCOIL)),(uint32_t)0x0000461C);

	read_param(pwr_thres.AT_CURS_MTQ_MOBC, pwr_thres_.AT_CURS_MTQ_MOBC, (size_t)(sizeof(pwr_thres_.AT_CURS_MTQ_MOBC)),(uint32_t)0x00004646);
	read_param(pwr_thres.AT_CURS_MTQ_ACS1, pwr_thres_.AT_CURS_MTQ_ACS1, (size_t)(sizeof(pwr_thres_.AT_CURS_MTQ_ACS1)),(uint32_t)0x00004656);

	read_param(pwr_thres.AT_VOLS_GPS, pwr_thres_.AT_VOLS_GPS, (size_t)(sizeof(pwr_thres_.AT_VOLS_GPS)),(uint32_t)0x00004682);
	read_param(pwr_thres.AT_VOLS_SUNS, pwr_thres_.AT_VOLS_SUNS, (size_t)(sizeof(pwr_thres_.AT_VOLS_SUNS)),(uint32_t)0x00004686);
	read_param(pwr_thres.AT_VOLS_LORA, pwr_thres_.AT_VOLS_LORA, (size_t)(sizeof(pwr_thres_.AT_VOLS_LORA)),(uint32_t)0x0000468A);
	read_param(pwr_thres.AT_VOLS_MTQ_MOBC, pwr_thres_.AT_VOLS_MTQ_MOBC, (size_t)(sizeof(pwr_thres_.AT_VOLS_MTQ_MOBC)),(uint32_t)0x0000468E);
	read_param(pwr_thres.AT_VOLS_MIS1, pwr_thres_.AT_VOLS_MIS1, (size_t)(sizeof(pwr_thres_.AT_VOLS_MIS1)),(uint32_t)0x00004692);
	read_param(pwr_thres.AT_VOLS_SEPCOIL, pwr_thres_.AT_VOLS_SEPCOIL, (size_t)(sizeof(pwr_thres_.AT_VOLS_SEPCOIL)),(uint32_t)0x00004696);
	read_param(pwr_thres.AT_VOLS_MTQ_ACS1, pwr_thres_.AT_VOLS_MTQ_ACS1, (size_t)(sizeof(pwr_thres_.AT_VOLS_MTQ_ACS1)),(uint32_t)0x0000469A);

	read_param(pwr_thres.AT_VOLS_REG_CDH3V3, pwr_thres_.AT_VOLS_REG_CDH3V3, (size_t)(sizeof(pwr_thres_.AT_VOLS_REG_CDH3V3)),(uint32_t)0x000046B2);
	read_param(pwr_thres.AT_VOLS_REG_ACS1_5V, pwr_thres_.AT_VOLS_REG_ACS1_5V, (size_t)(sizeof(pwr_thres_.AT_VOLS_REG_ACS1_5V)),(uint32_t)0x000046B8);
	read_param(pwr_thres.AT_VOLS_REG_ACS2_5V, pwr_thres_.AT_VOLS_REG_ACS2_5V, (size_t)(sizeof(pwr_thres_.AT_VOLS_REG_ACS2_5V)),(uint32_t)0x000046BC);

	// TXPIC's Sensors Anomaly Threshold
	read_param(&pwr_txpic_thres.AT_CURS_SAP1, &pwr_txpic_thres_.AT_CURS_SAP1, (size_t)(sizeof(pwr_txpic_thres_.AT_CURS_SAP1)),(uint32_t)0x0000471C);
	read_param(&pwr_txpic_thres.AT_CURS_SAP2, &pwr_txpic_thres_.AT_CURS_SAP2, (size_t)(sizeof(pwr_txpic_thres_.AT_CURS_SAP2)),(uint32_t)0x0000471E);
	read_param(pwr_txpic_thres.AT_VOLS_2ndBAT, pwr_txpic_thres_.AT_VOLS_2ndBAT, (size_t)(sizeof(pwr_txpic_thres_.AT_VOLS_2ndBAT)),(uint32_t)0x00004720);


	// Anomaly Waiting Counter
	read_param(&pwr_awc_const, &pwr_awc_const_,(size_t)(sizeof(pwr_awc_const_)),(uint32_t)0x00004724);

	g_pwr_thres = &pwr_thres;
	g_pwr_txpic_thres = &pwr_txpic_thres;
	g_pwr_awc_const = &pwr_awc_const;
}





// Anomaly detection for current sensor only over current
void AD_CURS(AL_PWR_CURS_ACK curs,uint16_t rawData,const uint16_t*thres){
	PWR_ANOMALY_DRIVER_STRUCT *oc = &pwr_anomaly.curs[curs].oc.anomaly;
	if (rawData>thres[0]){
		print(PWR_ANOMALY, "Anomaly detection, Over current, ID : %d, count : %d\r\n",curs,oc->count);
		EL_record_event(EL_GROUP_OVERCURRENT, curs, EL_ERROR_LEVEL_HIGH, 0);        // ERROR_LEVELは暫定．2024/06/24 Kanie
		if (oc->count >0){
			oc->flag = 1;
			print(PWR_ANOMALY, "Anomaly detection, Over current Flag set: %d\r\n", oc->flag);
		}
	}
}

// Anomaly detection for MTQ current sensor(having reference voltage)
// thres[positive,negative]
void AD_CURS_MTQ(AL_PWR_CURS_ACK curs,uint16_t rawData,const uint16_t*thres,uint8_t GPIO_State){
	PWR_ANOMALY_DRIVER_STRUCT *oc = &pwr_anomaly.curs[curs].oc.anomaly;
//	uint8_t* fwe = &pwr_thres_.AW_CURS_MTQ_MOBC;
	if (GPIO_State==1){
		if (rawData>thres[0] || rawData<thres[1]){
			print(PWR_ANOMALY, "Anomaly detection, Over current, ID : %d, count : %d\r\n",curs,oc->count);
//			AL_add_anomaly(AL_GROUP_OVERCURRENT,curs);
			if (oc->count >0){
				oc->flag = 1;
			}
		}
	}
}

// Anomaly detection for voltage sensor of FET
// thres[0] : UV, thres[1] : PSI
void AD_VOLS_FET(AL_PWR_VOLS_ACK vols,uint16_t rawData,const uint16_t* thres,uint8_t GPIO_State){
	PWR_ANOMALY_DRIVER_STRUCT *uv = &pwr_anomaly.vols[vols].uv;
	PWR_ANOMALY_DRIVER_STRUCT *psi_on = &pwr_anomaly.vols[vols].psi_on;
	PWR_ANOMALY_DRIVER_STRUCT *psi_off = &pwr_anomaly.vols[vols].psi_off.anomaly;
//	if(vols ==9){
//		print(PWR_ANOMALY, PWR_ANOMALY, "ADC_Value : %d\r\n",rawData);
//		print(PWR_ANOMALY, PWR_ANOMALY, "AD_VOLS_FET threshold, ID : %d, thres : %d,  %d\r\n",vols,thres[0],thres[1]);
//	}

	if (GPIO_State==1){
		if (rawData>thres[1] && rawData<thres[0]) {
			print(PWR_ANOMALY, "Anomaly detection, Under Voltage, ID : %d, count : %d\r\n", vols,uv->count);
//			print(PWR_ANOMALY, PWR_ANOMALY, "UV_count : %d\r\n",uv->count);
//			AL_add_anomaly(AL_GROUP_UNDERVOLTAGE,vols);
			if (uv->count >0){
				uv->flag = 1;
				print(PWR_ANOMALY, "UV_count : %d, UV_flag : %d\r\n",uv->count,uv->flag);
			}
		}
		else if (rawData<thres[1]) {
			print(PWR_ANOMALY, "Anomaly detection, Power state inconsistency_OFF, ID : %d, count : %d\r\n", vols,psi_off->count);
//			AL_add_anomaly(AL_GROUP_PSI_OFF,vols);
			if (psi_off->count >0){
				psi_off->flag = 1;
			}
		}
		else {
//			print(PWR_ANOMALY, "PWR ON, No Anomaly : %d\r\n",vols);
//			print(PWR_ANOMALY, "ADC_Value : %d,  UY :%d\r\n",rawData,UV);
		}
	}
	else if(GPIO_State==0){
		if (rawData>thres[1]){
			print(PWR_ANOMALY, "Anomaly detection, Power state inconsistency_ON, ID : %d, count : %d\r\n", vols,psi_on->count);
//			AL_add_anomaly(AL_GROUP_PSI_ON,vols);
			if (psi_on->count >0){
				print(PWR_ANOMALY, "PSI_ON_ID : %d\r\n",vols);
				psi_on->flag = 1;
			}
		}
	}
}


// Anomaly detection for voltage sensor of uncontrolled regulator (no distinction between ON/OFF)
// thres[OV,UV]
void AD_VOLS_REG_UNCONTROL(AL_PWR_VOLS_ACK vols,uint16_t rawData,const uint16_t*thres) {
	PWR_ANOMALY_DRIVER_STRUCT *ov = &pwr_anomaly.vols[vols].ov.anomaly;
	PWR_ANOMALY_DRIVER_STRUCT *uv = &pwr_anomaly.vols[vols].uv;
	if (rawData>thres[0]){
		print(PWR_ANOMALY, "Anomaly detection, Over Voltage, ID : %d, count : %d\r\n",vols,ov->count);
//		AL_add_anomaly(AL_GROUP_OVERVOLTAGE,vols);
		if (ov->count >0){
			ov->flag = 1;
		}
	}
	else if (rawData<thres[1]) {
		print(PWR_ANOMALY, "Anomaly detection, Under Voltage, ID : %d, count : %d\r\n",vols,uv->count);
//		AL_add_anomaly(AL_GROUP_UNDERVOLTAGE,vols);
		if (uv->count >0){
			uv->flag = 1;
		}
	}
}

// Anomaly detection for voltage sensor of controlled regulator
// thres[OV,UV,PSI]
void AD_VOLS_REG_CONTROL(AL_PWR_VOLS_ACK vols,uint16_t rawData,const uint16_t*thres,uint8_t PWR_State)
{
	PWR_ANOMALY_DRIVER_STRUCT *ov = &pwr_anomaly.vols[vols].ov.anomaly;
	PWR_ANOMALY_DRIVER_STRUCT *uv = &pwr_anomaly.vols[vols].uv;
	PWR_ANOMALY_DRIVER_STRUCT *psi_on = &pwr_anomaly.vols[vols].psi_on;

	if(PWR_State == 1){
		if(rawData>thres[0]){
			print(PWR_ANOMALY, "Anomaly detection, Over Voltage, ID : %d, count : %d\r\n",vols,ov->count);
//			AL_add_anomaly(AL_GROUP_OVERVOLTAGE,vols);
				if (ov->count >0){
					ov->flag = 1;
				}
		}
		else if (rawData<thres[1]) {
			print(PWR_ANOMALY, "Anomaly detection, Under Voltage, ID : %d, count : %d\r\n",vols,uv->count);
//			AL_add_anomaly(AL_GROUP_UNDERVOLTAGE,vols);
			if (uv->count >0){
				uv->flag = 1;
			}
		}
	}
	else if(PWR_State == 0){
		print(PWR_ANOMALY, "Anomaly detection, Power state inconsistency_ON, ID : %d, count : %d\r\n", vols,psi_on->count);
//		AL_add_anomaly(AL_GROUP_PSI_ON,vols);
		if (psi_on->count >0){
			psi_on->flag = 1;
		}
	}
}





AppInfo PWR_TXPIC_SENSING_DETECT_create_app(void)
{
    //create_app_info()を呼び出す．

    return AI_create_app_info("PWR_TXPIC_Sensing_Detect", PWR_TXPIC_SENSING_DETECT_init, PWR_TXPIC_SENSING_DETECT_exec);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
void PWR_TXPIC_SENSING_DETECT_init(void)
{
	printf("PWR TXPIC FAULT DETECT initialize.\n");
}


/**
 * @brief アプリケーション実行関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
void PWR_TXPIC_SENSING_DETECT_exec(void){

	uint16_t val;

//	val = txpic_driver->tlm_data.txpic_to_mobc.cur_vols_ioex.curs_sap1;
//	AD_TXPIC_CURS(AL_TXPIC_CURS_SAP1, val, &g_pwr_txpic_thres->AT_CURS_SAP1);
//
//	val = txpic_driver->tlm_data.txpic_to_mobc.cur_vols_ioex.curs_sap2;
//	AD_TXPIC_CURS(AL_TXPIC_CURS_SAP2, val, &g_pwr_txpic_thres->AT_CURS_SAP2);

	val= txpic_driver->tlm_data.txpic_to_mobc.cur_vols_ioex.vols_2ndbat;
	AD_TXPIC_VOLS(AL_TXPIC_VOLS_2ndBAT, val, g_pwr_txpic_thres->AT_VOLS_2ndBAT);

//	val = txpic_driver->tlm_data.txpic_to_mobc.cur_vols_ioex.vols_sap1;
//	AD_TXPIC_VOLS(AL_TXPIC_VOLS_SAP1, val, g_pwr_txpic_thres->AT_VOLS_SAP1);
//
//	val = txpic_driver->tlm_data.txpic_to_mobc.cur_vols_ioex.vols_sap2;
//	AD_TXPIC_VOLS(AL_TXPIC_VOLS_SAP2, val, g_pwr_txpic_thres->AT_VOLS_SAP2);

}

// Anomaly detection for voltage sensor by TXPIC
// thres[OV,UV]
void AD_TXPIC_VOLS(AL_PWR_TXPIC_CURSVOLS_ACK vols, uint16_t rawData, const uint16_t*thres){
//	print(PWR_ANOMALY, "AD VOLS. rawData, ulim, llim: %d, %d, %d\r\n", rawData, thres[0], thres[1]);

	uint8_t trans_to = 1;
	switch(p_phase_driver->current_phase){
	case CRITICAL:
		if(mode_trans_driver->phase0_transition_to == 0xff000000){
			trans_to = MD_AOCSFLAG_STANDBY; //Stand-by
		}else if(mode_trans_driver->phase0_transition_to == 0x00ff0000){
			trans_to = MD_AOCSFLAG_MISSION_STANDBY; //Mission standby
		}else if(mode_trans_driver->phase0_transition_to == 0x0000ff00){
			trans_to = MD_AOCSFLAG_DAMPING; //Damping
		}else if(mode_trans_driver->phase0_transition_to == 0x000000ff){
			trans_to = MD_AOCSFLAG_SUN_POINTING; //Sunpointing
		}
		break;
	case INITIAL_OPERATION:
		if(mode_trans_driver->phase1_transition_to == 0xff000000){
			trans_to = MD_AOCSFLAG_STANDBY; //Stand-by
		}else if(mode_trans_driver->phase1_transition_to == 0x00ff0000){
			trans_to = MD_AOCSFLAG_MISSION_STANDBY; //Mission standby
		}else if(mode_trans_driver->phase1_transition_to == 0x0000ff00){
			trans_to = MD_AOCSFLAG_DAMPING; //Damping
		}else if(mode_trans_driver->phase1_transition_to == 0x000000ff){
			trans_to = MD_AOCSFLAG_SUN_POINTING; //Sunpointing
		}
		break;
	case NORMAL_OPERATION:
		if(mode_trans_driver->phase2_transition_to == 0xff000000){
			trans_to = MD_AOCSFLAG_STANDBY; //Stand-by
		}else if(mode_trans_driver->phase2_transition_to == 0x00ff0000){
			trans_to = MD_AOCSFLAG_MISSION_STANDBY; //Mission standby
		}else if(mode_trans_driver->phase2_transition_to == 0x0000ff00){
			trans_to = MD_AOCSFLAG_DAMPING; //Damping
		}else if(mode_trans_driver->phase2_transition_to == 0x000000ff){
			trans_to = MD_AOCSFLAG_SUN_POINTING; //Sunpointing
		}
		break;
	default:
		break;
	}

	if(trans_to != user_mode->current_mode_id){
//		print(PWR_ANOMALY,"trans to %d\r\n" ,trans_to);
//		print(PWR_ANOMALY,"current id %d\r\n",user_mode->current_mode_id);
		if (rawData>thres[0]){
//			printf("Anomaly detection, TXPIC Sensor Over Voltage, ID : %d\r\n",vols);
//			printf("AD VOLS. rawData, ulim, llim: %d, %d, %d\r\n", rawData, thres[0], thres[1]);
//			AL_add_anomaly(AL_GROUP_OVERVOLTAGE,vols);
		}
		else if (rawData<thres[1]){
//			printf("Anomaly detection, TXPIC Sensor Under Voltage, ID : %d\r\n",vols);
//			printf("AD VOLS. rawData, ulim, llim: %d, %d, %d\r\n", rawData, thres[0], thres[1]);
//			AL_add_anomaly(AL_GROUP_UNDERVOLTAGE,vols);
		}
	}
}

// Anomaly detection for current sensor by TXPIC
// thres UC
void AD_TXPIC_CURS(AL_PWR_TXPIC_CURSVOLS_ACK curs, uint16_t rawData, const uint16_t*thres){
//	print(PWR_ANOMALY, "AD CURS. rawData, llim : %d, %d\r\n", rawData, thres[0]);

	uint8_t trans_to = 1;
	switch(p_phase_driver->current_phase){
	case CRITICAL:
		if(mode_trans_driver->phase0_transition_to == 0xff000000){
			trans_to = MD_AOCSFLAG_STANDBY; //Stand-by
		}else if(mode_trans_driver->phase0_transition_to == 0x00ff0000){
			trans_to = MD_AOCSFLAG_MISSION_STANDBY; //Mission standby
		}else if(mode_trans_driver->phase0_transition_to == 0x0000ff00){
			trans_to = MD_AOCSFLAG_DAMPING; //Damping
		}else if(mode_trans_driver->phase0_transition_to == 0x000000ff){
			trans_to = MD_AOCSFLAG_SUN_POINTING; //Sunpointing
		}
		break;
	case INITIAL_OPERATION:
		if(mode_trans_driver->phase1_transition_to == 0xff000000){
			trans_to = MD_AOCSFLAG_STANDBY; //Stand-by
		}else if(mode_trans_driver->phase1_transition_to == 0x00ff0000){
			trans_to = MD_AOCSFLAG_MISSION_STANDBY; //Mission standby
		}else if(mode_trans_driver->phase1_transition_to == 0x0000ff00){
			trans_to = MD_AOCSFLAG_DAMPING; //Damping
		}else if(mode_trans_driver->phase1_transition_to == 0x000000ff){
			trans_to = MD_AOCSFLAG_SUN_POINTING; //Sunpointing
		}
		break;
	case NORMAL_OPERATION:
		if(mode_trans_driver->phase2_transition_to == 0xff000000){
			trans_to = MD_AOCSFLAG_STANDBY; //Stand-by
		}else if(mode_trans_driver->phase2_transition_to == 0x00ff0000){
			trans_to = MD_AOCSFLAG_MISSION_STANDBY; //Mission standby
		}else if(mode_trans_driver->phase2_transition_to == 0x0000ff00){
			trans_to = MD_AOCSFLAG_DAMPING; //Damping
		}else if(mode_trans_driver->phase2_transition_to == 0x000000ff){
			trans_to = MD_AOCSFLAG_SUN_POINTING; //Sunpointing
		}
		break;
	default:
		break;
	}
	if(trans_to != user_mode->current_mode_id){
		if (rawData<thres[0]){
			printf("Anomaly detection, TXPIC Sensor Under Current, ID : %d\r\n",curs);
			printf("AD CURS. rawData, llim : %d, %d\r\n", rawData, thres[0]);
//			AL_add_anomaly(AL_GROUP_UNDERCURRENT,curs);
		}
	}
}

void PWR_Reset_AWC(void)
{
	g_pwr_awc = *g_pwr_awc_const;
}


void PWR_Decrement_AWC(void){
	g_pwr_awc--;
//	print(PWR_ANOMALY, "awc decrement\n");
}
