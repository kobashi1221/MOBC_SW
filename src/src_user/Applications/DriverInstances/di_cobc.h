/*
 * di_cobc.h
 *
 *  Created on: 2021/10/16
 *      Author: daiki
 *
 *  Updated on: 2021/12/22
 *      Author: Yuta Sakaguchi
 */

#ifndef DI_COBC_H_
#define DI_COBC_H_

#include "../../Drivers/Com/COBC/cobc.h"
#include "../../Drivers/Com/COBC/cobc_telemetry_definitions.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/command_dispatcher.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/TlmCmd/telemetry_definitions.h>

extern COBC_STRUCT* const cobc_driver;

extern const CommandDispatcher* const cobc_cdis;             //!< AOBC cmd dispatcher

/**
 * @brief  AOBC update アプリ生成関数
 * @param  void
 * @return void
 */
AppInfo cobc_update(void);
AppInfo cobc_cmd_dispatcher(void);

CCP_CmdRet Cmd_cobc_Transmit_command(const CommonCmdPacket* packet);
CCP_CmdRet cobc_dispatch_command(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_CDIS_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_CDIS_CLEAR_ERR_LOG(const CommonCmdPacket* packet);
_ACK cobc_AOCS_send_data_generator(TLM_CODE id);
_ACK COBC_TCPacket_Fixedheader_Checker(void);
void COBC_large_packet_init_setting();
_ACK cobc_check_life_command(void);

_ACK COBC_Regular_Tlm_ganerator(void);
_ACK COBC_Additional_Tlm_generator(const uint8_t* param);
_ACK COBC_Periodic_Tlm_generator(uint8_t T_counter);

#endif /* SRC_USER_APPLICATIONS_DRIVERINSTANCES_DI_COBC_H_ */
