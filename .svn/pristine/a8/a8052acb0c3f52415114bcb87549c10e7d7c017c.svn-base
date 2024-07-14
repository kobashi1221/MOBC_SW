/*
 * di_intersat.h
 *
 *  Created on: 2021/11/26
 *      Author: Sakaguchi
 */

#ifndef SRC_USER_APPLICATIONS_DRIVERINSTANCES_DI_INTERSAT_H_
#define SRC_USER_APPLICATIONS_DRIVERINSTANCES_DI_INTERSAT_H_

#include "../../Drivers/Com/INTER_SAT/inter_sat.h"
#include "../../Drivers/Com/COBC/cobc_telemetry_definitions.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/command_dispatcher.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

extern INTERSAT_STRUCT* const intersat_driver;

extern const CommandDispatcher* const intersat_cdis;             //!<cmd dispatcher

/**
 * @brief  AOBC update アプリ生成関数
 * @param  void
 * @return void
 */
AppInfo COM_intersat_update(void);

/**
 * @brief  AOBC cmd dispatcher アプリ生成関数
 * @param  void
 * @return void
 */
AppInfo intersat_cmd_dispatcher(void);

/**
 * @brief  CTCP を AOBC に送信する
 * @note   AOBC が自身のコマンドと解釈できるよう，Execution Type を上書きするため，packetをconst castする．[TODO] const castやめたい
 * @note   この関数を呼んでも良いのは，user_packet_handler のみ！
 * @param  *packet CTCP packet
 * @return CCP_EXEC_SUCCESS:        無事に転送された
 * @return それ以外: 転送失敗（DS_CMD_ERR_CODE を CCP_EXEC_STS に変換して返す．詳細エラーは DriverSuper を参照）
 */
CCP_CmdRet Cmd_intersat_Transmit_command(const CommonCmdPacket* packet);

CCP_CmdRet intersat_dispatch_command(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_intersat_CDIS_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_intersat_CDIS_CLEAR_ERR_LOG(const CommonCmdPacket* packet);

_ACK INTERSAT_TCPacket_Fixedheader_Checker(void);
_ACK intersat_check_life_command(void);


#endif
