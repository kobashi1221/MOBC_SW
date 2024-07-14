/**
* @file   di_rxpic.h
* @brief  RXPICのDIとRXPICのcmd dispatcher
*/
#ifndef INCLUDED_DI_RXPIC_H
#define INCLUDED_DI_RXPIC_H

#include "../../Drivers/Com/RXPIC/rxpic.h"
#include "../../Drivers/Com/RXPIC/rxpic_telemetry_definitions.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/command_dispatcher.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

extern RXPIC_STRUCT* const rxpic_driver;

extern const CommandDispatcher* const rxpic_cdis;             //!< AOBC cmd dispatcher


/**
 * @brief  AOBC update アプリ生成関数
 * @param  void
 * @return void
 */
AppInfo rxpic_update(void);

/**
 * @brief  AOBC cmd dispatcher アプリ生成関数
 * @param  void
 * @return void
 */
AppInfo rxpic_cmd_dispatcher(void);

/**
 * @brief  CTCP を AOBC に送信する
 * @note   AOBC が自身のコマンドと解釈できるよう，Execution Type を上書きするため，packetをconst castする．[TODO] const castやめたい
 * @note   この関数を呼んでも良いのは，user_packet_handler のみ！
 * @param  *packet CTCP packet
 * @return CCP_EXEC_SUCCESS:        無事に転送された
 * @return それ以外: 転送失敗（DS_CMD_ERR_CODE を CCP_EXEC_STS に変換して返す．詳細エラーは DriverSuper を参照）
 */
CCP_CmdRet rxpic_dispatch_command(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_RXPIC_CDIS_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_RXPIC_CDIS_CLEAR_ERR_LOG(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_RXPIC_seq_reset(const CommonCmdPacket* packet);

_ACK RXPIC_TCPacket_Fixedheader_Checker(void);
void rxpic_unexpected_shutdown_send_data_generator(void);

#endif
