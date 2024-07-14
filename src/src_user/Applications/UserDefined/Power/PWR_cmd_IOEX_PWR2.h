/**
  *****************************************************************************************
  * @file    : PWR_IOEX_ADC_MUX.h
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/09/25
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  * This is driver instance of the driver of IOEXPANDER made by Mr.Okada.
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/09/25  1.0   N.KISHI         First revision
  *
  *****************************************************************************************
  */

#ifndef PWR_IOEX_ADC_MUX_H_
#define PWR_IOEX_ADC_MUX_H_


#include <stdint.h> //for uint8_t
#include <stdbool.h> //for boolean
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

//#include "PWR_cmd_FETREG.h"

//extern const PWR_IOEXPANDER_STRUCT* pwr_IOEXPANDER;

#ifdef __cplusplus
 extern "C" {
#endif

 typedef struct
 {
 	uint8_t GPIO_State[3];
 }PWR_IOEXPANDER_STRUCT;

//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
//#include <src_user/Applications/UserDefined/PWR/PWR_cmd_ADC_MOBC.h>

//#include "PWR_cmd_FETREG.h"

 AppInfo PWR_IOEXPANDER_update(void);
 CCP_CmdRet Cmd_PWR_IOEX_PWR2_Init(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SUNS_MXPZ_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SUNS_PXPYMY_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_MIS1_ON_OFF_ONReset(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SEPCOIL1_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SEPCOIL2_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SEPCOIL3_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SEPCOIL4_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_GYRO_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_MAGS_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SEMP_ON_OFF(const CommonCmdPacket* packet);

 CCP_CmdRet Cmd_PWR_SEPCOIL1_GUARD_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SEPCOIL2_GUARD_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SEPCOIL3_GUARD_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SEPCOIL4_GUARD_ON_OFF(const CommonCmdPacket* packet);

// CCP_EXEC_STS Cmd_PWR_MIS1_ONReset(const CTCP* packet);

#ifdef __cplusplus
}
#endif

#endif /* PWR_IOEX_ADC_MUX_H_ */
