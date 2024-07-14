/**
  *****************************************************************************************
  * @file    : ACS2_debug_MTQ.h
  * @author  : Hoang Xuan Truong An
  * @version : 1.0
  * @date    : 2020/10/20
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
  * 2020/10/20  1.0   An              First revision
  * 2021/06/16  2.0   MASUDA
  *****************************************************************************************
  */

#ifndef ACS2_DEBUG_MTQ_H_
#define ACS2_DEBUG_MTQ_H_

#include <stdint.h> //for uint8_t

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

typedef struct
{
	// 試験用SWでMTQAをデバッグするためのフラグ
	uint8_t mtqa_debug_flag;

	// MTQAデバッグコマンドの指令値を格納する変数
	// 0:X 1:Y 2:Z
	uint8_t mtqa_debug_param[3];

	// 試験用SWでMTQOをデバッグするためのフラグ
	uint8_t mtqo_debug_flag;

	// MTQOデバッグコマンドの指令値を格納する変数
	uint8_t mtqo_debug_param;

} ACS23_DEBUG_MTQ_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif


AppInfo ACS23_debug_mtq_update(void);
AppInfo ACS23_MTQ_param_update(void);
AppInfo ACS23_mtqa4_update(void);
AppInfo ACS23_mtqo4_update(void);

CCP_CmdRet Cmd_ACS23_MTQ_Output_Zero(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS23_MTQ_Output_Return(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS23_debug_MTQA_Function_core(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS23_debug_MTQO_Function_core(const CommonCmdPacket* packet);

//for anomaly
CCP_CmdRet Cmd_ACS23_AH_MTQA_OT(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS23_AH_MTQO_OT(const CommonCmdPacket* packet);



#ifdef __cplusplus
 }
#endif

#endif /* ACS2_DEBUG_MTQ_H_ */
