/*
 * Global_PWR_Parameter.h
 *
 *  Created on: 2021/12/10
 *      Author: SAT63
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_GLOBAL_PWR_PARAMETER_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_GLOBAL_PWR_PARAMETER_H_

#include "Power/PWR_cmd_FETREG.h"
#include "Power/PWR_cmd_IOEX_PWR2.h"

typedef enum
{
	SEPCOIL_1,
	SEPCOIL_2,
	SEPCOIL_3,
	SEPCOIL_4,
	SEPCOIL_MAX
} PWR_SEPCOIL_GUARD;

typedef struct
{
	uint16_t AT_CURS_MOBC;
	uint16_t AT_CURS_LORA;

	uint16_t AT_CURS_REG_CDH3V3;
	uint16_t AT_CURS_SEPCOIL;
	uint16_t AT_CURS_MTQ;

	uint16_t AT_CURS_MTQ_MOBC[4][2];
	uint16_t AT_CURS_MTQ_ACS1[6][2];

	uint16_t AT_VOLS_GPS[2];
	uint16_t AT_VOLS_SUNS[2];
	uint16_t AT_VOLS_LORA[2];
	uint16_t AT_VOLS_MTQ_MOBC[2];
	uint16_t AT_VOLS_MIS1[2];
	uint16_t AT_VOLS_SEPCOIL[2];
	uint16_t AT_VOLS_MTQ_ACS1[2][2];

	uint16_t AT_VOLS_REG_CDH3V3[3];
	uint16_t AT_VOLS_REG_ACS1_5V[2];
	uint16_t AT_VOLS_REG_ACS2_5V[2];

} PWR_ANOMALY_THRESHOLD_STRUCT;

typedef struct
{
	uint16_t AT_CURS_SAP1;
	uint16_t AT_CURS_SAP2;
	uint16_t AT_VOLS_2ndBAT[2];

} PWR_TXPIC_ANOMALY_THRESHOLD_STRUCT;

typedef enum
{
	// 電源状態の情報,
	// MOBC操作
	PWR_State_REG_CDH3V3,//PWR_State[0]
	PWR_State_IOEX_PWR2,//PWR_State[2]
	PWR_State_GPS,
	PWR_State_LORA,
	PWR_State_MTQA,  // for 1U and 2U
	PWR_State_MTQO_PLASMA,//PWR_State[3]
	// IOEXPANDER操作
    PWR_State_SUNS_MXPZ,
    PWR_State_SUNS_PXPYMY,
    PWR_State_GYRO,
    PWR_State_MAGS,
	PWR_State_MIS1,
    PWR_State_SEMP,
	PWR_State_SEPCOIL1, // PWR_State[5];
	PWR_State_SEPCOIL2,
	PWR_State_SEPCOIL3,	// PWR_State[6];
	PWR_State_SEPCOIL4,
    PWR_State_SUNS,//PWR_State[1]
	PWR_State_MAX
} PWR_STATE;

typedef struct
{
	uint32_t PWR_MODE_Number;
	uint8_t* State;
} PWR_STATE_STRUCT;


extern const PWR_ANOMALY_THRESHOLD_STRUCT* g_pwr_thres;

#ifdef __cplusplus
 extern "C" {
#endif

 AppInfo PWR_GLOBAL_Param_update(void);

void PWR_Mode_Set(uint32_t PWR_Mode_Num);
void PWR_Mode_Trans(void);
uint8_t PWR_GET_MODE(uint8_t num);

#ifdef __cplusplus
}
#endif

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_GLOBAL_PWR_PARAMETER_H_ */
