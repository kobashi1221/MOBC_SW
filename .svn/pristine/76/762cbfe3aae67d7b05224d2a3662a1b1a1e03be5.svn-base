/*
 * debug_hk.c
 *
 *  Created on: 2022/04/06
 *      Author: SAT37
 */

#include <src_core/TlmCmd/telemetry_frame.h>
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include <src_user/Applications/UserDefined/CDH/ResetUtility.h>
#include <src_user/IfWrapper/HAL.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/UserDefined/Cdh/telemetry_user.h>
#include <src_user/Applications/UserDefined/Cdh/ResetUtility.h>

CCP_CmdRet Cmd_GENERATE_TLM_user(const CommonCmdPacket* packet)
{
  static TCP tcp_;
  const uint8_t* param = CCP_get_param_head(packet);
  uint16_t apid;
  uint8_t route;  //名大だとpacket配送ルートが必要になる
  uint8_t category;
  uint8_t id;  //名大だとTLMのIDが必要になる
  uint8_t num_dumps;
  uint16_t len;

  // パラメータ読み出し
  ENDIAN_memcpy(&apid,param,2);
  route = param[2];
  category = param[3];
  id = param[4];
  num_dumps = param[5];
  if (num_dumps >= 8)
  {
    // パケット生成回数の上限は8回とする。
    // 32kbpsでのDL時に8VCDU/secで1秒分の通信量。
    // これを超える場合は複数回コマンドを送信して対応する。
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  // ADU生成
  // ADU分割が発生しない場合に限定したコードになっている。
  // TLM定義シート上で定義するADUはADU長をADU分割が発生しない長さに制限する。
  TF_TLM_FUNC_ACK ret;
  ret = TF_generate_contents((int)id,
		  	  	  tcp_.packet,
				  &len,
				  TCP_MAX_LEN);

  // 範囲外のTLM IDを除外
  if (ret == TF_TLM_FUNC_ACK_NOT_DEFINED) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  if (len < 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);     // TODO: lenがマイナスの値たちをどうするか？

  // TCPacketヘッダ設定
  TCP_TLM_setup_primary_hdr(&tcp_, apid, len);
  TCP_set_route(&tcp_, route);
  TCP_set_all_issuer_id(&tcp_, MY_SAT_ID, TCP_EXE_ID_MOBC);
  TCP_TLM_set_startup_count(&tcp_, (uint8_t)p_reset_struct->reset_count[TOTAL_RESET]);
  TCP_TLM_set_ti(&tcp_, (uint32_t)(TMGR_get_master_total_cycle()));
  TCP_TLM_set_category(&tcp_, category);
  TCP_TLM_set_packet_id(&tcp_, id);

  // 生成したパケットを指定された回数配送処理へ渡す
  while (num_dumps != 0)
  {
    PH_analyze_packet(&tcp_);
    --num_dumps;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_GENERATE_HK_DEBUG(const CommonCmdPacket* packet)
{
  static TCP tcp_;
  uint16_t apid = AFSK_GS;
  uint8_t route = TCP_ROUTE_DIRECT;
  uint8_t category = 0x20;
  uint8_t id = Tlm_CODE_HK;
  uint8_t num_dumps = 1;
  uint16_t len;

  if (num_dumps >= 8)
  {
    // パケット生成回数の上限は8回とする。
    // 32kbpsでのDL時に8VCDU/secで1秒分の通信量。
    // これを超える場合は複数回コマンドを送信して対応する。
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  // ADU生成
  // ADU分割が発生しない場合に限定したコードになっている。
  // TLM定義シート上で定義するADUはADU長をADU分割が発生しない長さに制限する。
  TF_TLM_FUNC_ACK ret;
  ret = TF_generate_contents((int)id,
  	  	  	  	  tcp_.packet,
				  &len,
				  TCP_MAX_LEN);

  // 範囲外のTLM IDを除外
  if (ret == TF_TLM_FUNC_ACK_NOT_DEFINED) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  if (len < 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);     // TODO: lenがマイナスの値たちをどうするか？

  // TCPacketヘッダ設定
  TCP_TLM_setup_primary_hdr(&tcp_, apid, len);
  TCP_set_route(&tcp_, route);
  TCP_set_all_issuer_id(&tcp_, MY_SAT_ID, TCP_EXE_ID_MOBC);
  TCP_TLM_set_startup_count(&tcp_, (uint8_t)p_reset_struct->reset_count[TOTAL_RESET]);
  TCP_TLM_set_ti(&tcp_, (uint32_t)(TMGR_get_master_total_cycle()));
  TCP_TLM_set_category(&tcp_, category); // パラメータによる指定
  TCP_TLM_set_packet_id(&tcp_, id);


  // 生成したパケットを指定された回数配送処理へ渡す
  while (num_dumps != 0)
  {
    if(DBG_Mode_Analyze(HK_DBG)){
      HAL_UART_Transmit_C2A(&huart8, tcp_.packet, len, 0xff);
      printf("\r\n");
    }
    --num_dumps;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


