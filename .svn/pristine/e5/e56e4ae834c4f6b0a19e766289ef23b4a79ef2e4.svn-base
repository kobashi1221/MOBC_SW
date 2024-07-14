/*
 * PWR_cmd_AnomalyDetect.h
 *
 *  Created on: 2021/12/03
 *      Author: Otsuki
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_PWR_PWR_CMD_ANOMALYDETECT_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_PWR_PWR_CMD_ANOMALYDETECT_H_

#include <stdint.h> //for uint8_t
#include <stdbool.h> //for boolean
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>


typedef enum
{
	AL_CURS_MOBC=0,
	AL_CURS_LORA=1,

	AL_CURS_REG_CDH3V3=2,
	AL_CURS_SEPCOIL=3,
	AL_CURS_MTQ_ACS1=4,

	AL_CURS_MTQX_MOBC=5,
	AL_CURS_MTQY_MOBC=6,
	AL_CURS_MTQZ1Z2_MOBC=7,
	AL_CURS_MTQZ3Z4_MOBC=8,
	AL_CURS_MTQX_ACS1=9,
	AL_CURS_MTQY_ACS1=10,
	AL_CURS_MTQZ1_ACS1=11,
	AL_CURS_MTQZ2_ACS1=12,
	AL_CURS_MTQZ3_ACS1=13,
	AL_CURS_MTQZ4_ACS1=14,
	MAX_CURRENT_SENSOR,
//	// ADC_MOBC
//	AL_CURS_MOBC=0,
//	AL_CURS_LORA,
//	AL_CURS_MTQX_MOBC,
//	AL_CURS_MTQY_MOBC,
//	AL_CURS_MTQZ1Z2_MOBC,
//	AL_CURS_MTQZ3Z4_MOBC,
//	// ADC_PWR2
//	AL_CURS_REG_CDH3V3,
//	AL_CURS_SEPCOIL,
//	// ADC_ACS1
//	AL_CURS_MTQ_ACS1,
//	AL_CURS_MTQX_ACS1,
//	AL_CURS_MTQY_ACS1,
//	AL_CURS_MTQZ1_ACS1,
//	AL_CURS_MTQZ2_ACS1,
//	AL_CURS_MTQZ3_ACS1,
//	AL_CURS_MTQZ4_ACS1,
//	MAX_CURRENT_SENSOR,
} AL_PWR_CURS_ACK;

typedef enum
{
	AL_VOLS_GPS=0,
	AL_VOLS_SUNS=1,
	AL_VOLS_LORA=2,
	AL_VOLS_MTQA_MOBC=3,
	AL_VOLS_MIS1=4,
	AL_VOLS_SEPCOIL1=5,
	AL_VOLS_SEPCOIL2=6,
	AL_VOLS_SEPCOIL3=7,
	AL_VOLS_SEPCOIL4=8,
	AL_VOLS_MTQA_ACS1=9,
	AL_VOLS_MTQO_ACS1=10,

	AL_VOLS_REG_CDH3V3=11,
	AL_VOLS_REG_ACS1_5V=12,
	AL_VOLS_REG_ACS2_5V=13,

	MAX_VOLTAGE_SENSOR,
//	// ADC_MOBC
//	AL_VOLS_GPS,
//	AL_VOLS_SUNS,
//	AL_VOLS_LORA,
//	AL_VOLS_MTQA_MOBC,
//	// ADC_PWR2
//	AL_VOLS_REG_CDH3V3,
//	AL_VOLS_REG_ACS1_5V,
//	AL_VOLS_MIS1,
//	AL_VOLS_REG_ACS2_5V,
//	AL_VOLS_SEPCOIL1,
//	AL_VOLS_SEPCOIL2,
//	AL_VOLS_SEPCOIL3,
//	AL_VOLS_SEPCOIL4,
//	// ADC_ACS1
//	AL_VOLS_MTQA_ACS1,
//	AL_VOLS_MTQO_ACS1,
//	MAX_VOLTAGE_SENSOR,
} AL_PWR_VOLS_ACK;


typedef enum
{
	AL_TXPIC_CURS_SAP1=21,
	AL_TXPIC_CURS_SAP2=22,
	AL_TXPIC_VOLS_2ndBAT=23,
	MAX_TXPIC_VOLTAGE=24,
//	// ADC_MOBC
//	AL_VOLS_GPS,
//	AL_VOLS_SUNS,
//	AL_VOLS_LORA,
//	AL_VOLS_MTQA_MOBC,
//	// ADC_PWR2
//	AL_VOLS_REG_CDH3V3,
//	AL_VOLS_REG_ACS1_5V,
//	AL_VOLS_MIS1,
//	AL_VOLS_REG_ACS2_5V,
//	AL_VOLS_SEPCOIL1,
//	AL_VOLS_SEPCOIL2,
//	AL_VOLS_SEPCOIL3,
//	AL_VOLS_SEPCOIL4,
//	// ADC_ACS1
//	AL_VOLS_MTQA_ACS1,
//	AL_VOLS_MTQO_ACS1,
//	MAX_VOLTAGE_SENSOR,
} AL_PWR_TXPIC_CURSVOLS_ACK;



extern const PWR_ANOMALY_THRESHOLD_STRUCT* g_pwr_thres;
extern const PWR_ANOMALY_THRESHOLD_STRUCT pwr_thres_;

extern const PWR_TXPIC_ANOMALY_THRESHOLD_STRUCT* g_pwr_txpic_thres;
extern const PWR_TXPIC_ANOMALY_THRESHOLD_STRUCT pwr_txpic_thres_;

extern const uint8_t* g_pwr_awc_const;
extern const uint8_t pwr_awc_const_;
extern uint8_t g_pwr_awc;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo PWR_Anomaly_Detect_Param_update(void);

// Anomaly Detection
void AD_CURS(AL_PWR_CURS_ACK sensor,uint16_t rawData,const uint16_t* thres);
void AD_CURS_MTQ(AL_PWR_CURS_ACK sensor,uint16_t rawData,const uint16_t*thres,uint8_t GPIO_State);
void AD_VOLS_FET(AL_PWR_VOLS_ACK sensor,uint16_t rawData,const uint16_t*thres,uint8_t GPIO_State);
void AD_VOLS_REG_UNCONTROL(AL_PWR_VOLS_ACK sensor,uint16_t rawData,const uint16_t*thres);
void AD_VOLS_REG_CONTROL(AL_PWR_VOLS_ACK sensor,uint16_t rawData,const uint16_t*thres,uint8_t state);
void PWR_Reset_AWC(void);
void PWR_Decrement_AWC(void);

AppInfo PWR_TXPIC_SENSING_DETECT_create_app(void);
void PWR_TXPIC_SENSING_DETECT_init(void);
void PWR_TXPIC_SENSING_DETECT_exec(void);

void AD_TXPIC_CURS(AL_PWR_TXPIC_CURSVOLS_ACK curs, uint16_t rawData, const uint16_t*thres);
void AD_TXPIC_VOLS(AL_PWR_TXPIC_CURSVOLS_ACK vols, uint16_t rawData, const uint16_t*thres);

#ifdef __cplusplus
 }
#endif

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_PWR_PWR_CMD_ANOMALYDETECT_H_ */
