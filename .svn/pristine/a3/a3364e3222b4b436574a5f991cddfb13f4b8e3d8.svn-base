/**
* @file   di_txpic.h
* @brief  TXPICのDIとAOBCのcmd dispatcher
* @Auther 野呂拓臣
*/
#ifndef INCLUDED_DI_TXPIC_H
#define INCLUDED_DI_TXPIC_H

#include "../../Drivers/Com/TXPIC/txpic.h"
#include "../../Drivers/Com/TXPIC/txpic_telemetry_definitions.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/command_dispatcher.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

#ifdef __cplusplus
 extern "C" {
#endif

//extern const TXPIC_STRUCT* const txpic_driver;    //!< AOBC driver
extern TXPIC_STRUCT* const txpic_driver;    //!< AOBC driver
extern const CommandDispatcher* const txpic_cdis;             //!< AOBC cmd dispatcher

/**
 * @brief  AOBC update アプリ生成関数
 * @param  void
 * @return void
 */
AppInfo txpic_update(void);

/**
 * @brief  AOBC cmd dispatcher アプリ生成関数
 * @param  void
 * @return void
 */
AppInfo txpic_cmd_dispatcher(void);

/**
 * @brief  CTCP を AOBC に送信する
 * @note   AOBC が自身のコマンドと解釈できるよう，Execution Type を上書きするため，packetをconst castする．[TODO] const castやめたい
 * @note   この関数を呼んでも良いのは，user_packet_handler のみ！
 * @param  *packet CTCP packet
 * @return CCP_EXEC_SUCCESS:        無事に転送された
 * @return それ以外: 転送失敗（DS_CMD_ERR_CODE を CCP_EXEC_STS に変換して返す．詳細エラーは DriverSuper を参照）
 */
CCP_CmdRet txpic_dispatch_command(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TXPIC_CDIS_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TXPIC_CDIS_CLEAR_ERR_LOG(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_SendImageTLM_TXPIC(const CommonCmdPacket* packet);

_ACK TXPIC_TCPacket_Fixedheader_Checker(void);

#ifdef __cplusplus
 }
#endif

#endif
