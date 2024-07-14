/*
 * COM_cmd_LoRa.h
 *
 *  Created on: 2021/07/14
 *      Author: kahaku
 */

#ifndef C2A_APPLICATIONS_DRIVERINSTANCES_COMMUNICATION_COM_CMD_LORA_H_
#define C2A_APPLICATIONS_DRIVERINSTANCES_COMMUNICATION_COM_CMD_LORA_H_

#include "../../../Drivers/Cdh/LoRa/LoRa.h"
#include "../../../Drivers/Cdh/LoRa/LoRa_Struct.h"
#include "../../DriverInstances/di_LoRa.h"
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include "src_user/Library/Delay.h"
#include <src_user/IfWrapper/HAL.h> //necessary to add HAL func
#include <src_user/Library/Delay.h>

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo CDH1_lora_param_update(void);

CCP_CmdRet Cmd_COM_LoRa_Debug_Set(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COM_LoRa_Debug_Check(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COM_LoRa_Debug_Transmit(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_COM_user_RTCmd_send(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COM_user_TLCmd_send(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COM_LoRa_default_setting(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COM_LoRa_tlm_update(const CommonCmdPacket* packet);

 #ifdef __cplusplus
  }
 #endif

 #endif
