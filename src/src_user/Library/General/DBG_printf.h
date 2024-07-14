/*
 * DBG_printf.h
 *
 *  Created on: 2021/10/29
 *      Author: scath
 */

#ifndef SRC_USER_LIBRARY_GENERAL_DBG_PRINTF_H_
#define SRC_USER_LIBRARY_GENERAL_DBG_PRINTF_H_
#include "../../Settings/TlmCmd/common_tlm_cmd_packet_define.h"
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

typedef enum{
	//	***	CDH	***	//
	CMD = 0,
	TLM,
	BC,
	ANOMALY,
	TIME,
	REPRO,
	HK_DBG,
	ANOMALY_LOG,
	TLM_DBG,
	//	***	Memory	***	//
	SD = 10,
	MRAM_EEPROM,
	MRAM,
	EEPROM,
	SRAM,
	SPIPROM,
	Flash,//大文字だとエラー出る
	Debug_RTC,

	//	***	Sensor	***	//
	ACS_ADC = 20,
	_PLASMA,
	Pt_TEMP,
	GYRO,
	MAGS,
	_9AX,
	GPS,
	SUNS,
	MTQ,
	SUBCAM,

	//	***	Power	***	//
	FET = 30,
	PWR_ADC_PWR2,
	PWR_ADC_MOBC,
	PWR_ADC_ACS1,
	PWR_ANOMALY,
	PWR_OPERATION,
	PWR_MODE_FLAG,
	PWR_STATE_MOBC,

	//	*** Communication	***	//
	RXPIC = 40,
	TXPIC,
	COBC,
	SOBC,
	LORA,
	ISAT,
	SILS,
	LARGE_PACKET,

	//	***	AOCS sub***	// AOCSあふれたので。ちゃんと場所整えたい
	EKF,

	//	***	AOCS	***	//
	BDOT = 50,
	CPRODUCT,
	AD_EKF,
	GST,
	IGRF,
	QMETH,
	SUNVECTOR,
	TRIAD,
	SGP4,
	TEST,
	MAX_SIZE,
	QEST,
	DELTA_B,


	// *** Anomaly ***//
	ANO = 63
}DBG_MODE_LIST;

#ifdef __cplusplus
 extern "C" {
#endif

CCP_CmdRet Cmd_DBG_SELECT_printf(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_DBG_Set_Print(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_DBG_Reset_Print(const CommonCmdPacket* packet);

uint8_t DBG_Mode_Analyze(DBG_MODE_LIST mode);
void print(DBG_MODE_LIST mode, const char* format, ...);

#ifdef __cplusplus
 }
#endif

#endif /* SRC_USER_LIBRARY_GENERAL_DBG_PRINTF_H_ */
