/*
 * PWR_cmd_ADC_PWR2.h
 *
 *  Created on: 2021/11/20
 *      Author: SAT63
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_PWR_PWR_CMD_ADC_PWR2_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_PWR_PWR_CMD_ADC_PWR2_H_

#include <stdint.h> //for uint8_t
#include <stdbool.h> //for boolean
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
//#include <src_user/Applications/UserDefined/PWR/PWR_cmd_FETREG.h>
//#include "PWR_cmd_FETREG.h"


typedef enum{
	// HW固定
	PORT_CURS_REG_CDH3V3,
	PORT_CURS_SEPCOIL,
	PORT_VOLS_GYRO,
	PORT_VOLS_MAGS,
	PORT_VOLS_REG_CDH3V3,
	PORT_VOLS_REG_ACS1_5V,
	PORT_VOLS_MIS1,
	PORT_VOLS_REG_ACS2_5V,
	PORT_VOLS_SEPCOIL1,
	PORT_VOLS_SEPCOIL2,
	PORT_VOLS_SEPCOIL3,
	PORT_VOLS_SEPCOIL4,
	PORT_VOLS_SUNS_MXPZ,
	PORT_VOLS_SUNS_PXPYMY,
	PORT_CURS_SEMP,
	PORT_VOLS_SEMP,
	PORT_VOLS_HEAT3,	//使用しない．後ほど削除する
	PORT_VOLS_HEAT4,	//使用しない．後ほど削除する
}MUX_PWR2_PORT;

typedef struct
{
	uint16_t ADC_I_REG_CDH3V3; //ADC出力値
	uint16_t ADC_I_SEPC; //ADC出力値
	uint16_t ADC_V_GYRO; //ADC出力値
	uint16_t ADC_V_MAGS; //ADC出力値
	uint16_t ADC_V_REG_CDH3V3; //ADC出力値
	uint16_t ADC_V_REG_ACS1_5V; //ADC出力値
	uint16_t ADC_V_MIS1; //ADC出力値
	uint16_t ADC_V_REG_ACS2_5V; //ADC出力値
	uint16_t ADC_V_SEPC1; //ADC出力値
	uint16_t ADC_V_SEPC2; //ADC出力値
	uint16_t ADC_V_SEPC3; //ADC出力値
	uint16_t ADC_V_SEPC4; //ADC出力値
	uint16_t ADC_V_SUNS_MXPZ; //ADC出力値
	uint16_t ADC_V_SUNS_PXPYMY; //ADC出力値
	uint16_t ADC_I_SEMP; //ADC出力値
	uint16_t ADC_V_SEMP; //ADC出力値
	uint16_t ADC_V_HEAT3;	//使用しない．後ほど削除する
	uint16_t ADC_V_HEAT4;	//使用しない．後ほど削除する

} PWR_ADC_PWR2_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif


//#include <src_user/Drivers/PWR/MCP23017.h>


 AppInfo PWR_ADC_PWR2_update(void);
 AppInfo PWR_ADC_PWR2_Param_update(void);
 CCP_CmdRet Cmd_PWR_ADC_PWR2_Init(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_ADC_PWR2_Read(const CommonCmdPacket* packet);//For Continuous Receiveing at BC
 void SELECT_PORT(MUX_PWR2_PORT port);

#ifdef __cplusplus
}
#endif


#endif /* SRC_USER_APPLICATIONS_USERDEFINED_PWR_PWR_CMD_ADC_PWR2_H_ */
