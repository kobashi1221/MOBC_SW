/**
  *****************************************************************************************
  * @file    : PWR_FETREG.h
  * @author  : Hiroki OTSUKI
  * @version : 1.0
  * @date    : 2020/07/20
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/07/20  1.0   H.OTSUKI        First revision
  *
  *****************************************************************************************
  */

#ifndef PWR_FETREG_H_
#define PWR_FETREG_H_


#include <stdint.h> //for uint8_t
#include <stdbool.h> //for boolean
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include "PWR_cmd_IOEX_PWR2.h"


//本来ここにドライバへのパスをinclude



#ifdef __cplusplus
 extern "C" {
#endif

 typedef struct
 {
 	uint8_t GPIO_State[5];
 } PWR_FETREG_STRUCT;

 AppInfo PWR_FETREG_update(void);
 AppInfo PWR_FETREG_param_update(void);

 // ON or OFF -> ON_OFF
 CCP_CmdRet Cmd_PWR_GPS_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_SUNS_ON_OFF(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_LORA_ON_OFF_ONInit(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_REG_CDH3V3_ON_OFF(const CommonCmdPacket* packet); // Only OFF
 CCP_CmdRet Cmd_PWR_IOEX_PWR2_ON_OFF_ONInit(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_MTQA_ON_OFF_ONInit(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_MTQO_PLASMA_ON_OFF_ONInit(const CommonCmdPacket* packet);
 CCP_CmdRet Cmd_PWR_LORA_AUTO_ONInit(const CommonCmdPacket* packet);
 // ON and Initialize/Reset -> ONInit/ONReset
// CCP_EXEC_STS Cmd_PWR_LORA_ONInit(const CTCP* packet);
// CCP_EXEC_STS Cmd_PWR_IOEX_PWR2_ONInit(const CTCP* packet);
// CCP_EXEC_STS Cmd_PWR_MTQA_ONInit(const CTCP* packet);
// CCP_EXEC_STS Cmd_PWR_MTQO_ONInit(const CTCP* packet);



#ifdef __cplusplus
}
#endif
#endif /* PWR_FETREG_H_ */
