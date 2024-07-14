/**
* @file   di_sobc.h
* @brief  RXPICのDIとRXPICのcmd dispatcher
*/

#ifndef INCLUDED_DI_SOBC_H
#define INCLUDED_DI_SOBC_H

#include "../../Drivers/Com/SOBC/sobc.h"
//#include "../../Drivers/Com/SOBC/sobc_telemetry_definitions.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/command_dispatcher.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

extern SOBC_STRUCT* const sobc_driver;

extern const CommandDispatcher* const sobc_cdis;             //!< AOBC cmd dispatcher

AppInfo sobc_update(void);
/**
 * @brief  AOBC cmd dispatcher アプリ生成関数
 * @param  void
 * @return void
 */
AppInfo sobc_cmd_dispatcher(void);

CCP_CmdRet sobc_dispatch_command(const CommonCmdPacket* packet);
CCP_CmdRet sobc_debug_command_send(const CommonCmdPacket* packet);
_ACK SOBC_TCPacket_Fixedheader_Checker(void);
_ACK sobc_check_life_command(void);
void SOBC_Regular_Tlm_ganerator(void);
#endif