/*
 * di_interst.c
 *
 *  Created on: 2021/11/26
 *      Author: sakaguchi
 */

#include "di_intersat.h"
#include "di_struct.h"
#include "../../Drivers/Com/INTER_SAT/Inter_Sat.h"
#include "../../TlmCmd/user_packet_handler.h"
#include <src_core/Library/print.h>
#include <src_user/Library/General/DBG_printf.h>
#include "../UserDefined/Cdh/Communication_Fault_Detection.h"
#include <src_user/Applications/DriverInstances/command_response_handler.h>


#include <string.h> //memcpy用

static INTERSAT_STRUCT intersat_driver_;
INTERSAT_STRUCT* const intersat_driver = &intersat_driver_;

static CommandDispatcher intersat_cdis_;
const CommandDispatcher* const intersat_cdis = &intersat_cdis_;

static void intersat_cmd_dispatcher_init_(void);
static void intersat_cmd_dispatcher_(void);

static void intersat_init_(void);
static void intersat_update_(void);

AppInfo COM_intersat_update(void)
{
	return AI_create_app_info("update_intersat", intersat_init_, intersat_update_);
}

static void intersat_init_(void){
	print(ISAT,"intersat_init\r\n");
	int ret = INTERSAT_UART_init(&intersat_driver_);
	if (ret != 0){
		print(ISAT,"inter_sat init Failed ! %d \n", ret);
	}
}

//他機からのパケット確認, update
static void intersat_update_(void){
	for(int i=0; i<INTERSAT_MAX_RECEIVE_PACKET_NUM;i++){
		//Packetが入ってないとき
		if(isEmpty(&(intersat_queue.list))) {
			return;
		}
		//パケットを移してくる
		intersat_driver_.ack = (_ACK)dequeue(&(intersat_queue.list),&(intersat_driver_.tcp));
		//todo : Check Type

		if (intersat_driver_.ack != _SUCCESS){
			// 受信データがあるが何らかの異常が発生した場合は処理打ち切り
			queue_reset(&(intersat_queue.list));//カウンタを全部消す
			return;
		}
		// 受信時刻情報を更新。
		ANOMALY_UART_Update_Time(UART_FAULT_MILLMAX);
		intersat_driver_.last_recv_time = TMGR_get_master_total_cycle();

		//debug用
		print(ISAT,"Ack is %d\r\n",intersat_driver_.ack);
		print(ISAT," receive packet from Inter-SAT is \r\n");
		for(int i = 0; i < get_packet_len(intersat_driver_.tcp.packet); i++)
		{
			print(ISAT,"%02x ", intersat_driver_.tcp.packet[i]);
		}
		print(ISAT,"\r\n");

		// TCPacket Headerの固定値部分が妥当か確認する。
		if(INTERSAT_TCPacket_Fixedheader_Checker() == _FAILED) return;

		intersat_driver_.ack = INTERSAT_check_tcp_headers(&intersat_driver_);

		if ((intersat_driver_.ack != _SUCCESS) && (intersat_driver_.ack != _TLM_UPDATE_SUCCESS))		{
			queue_reset(&(intersat_queue.list));//カウンタを全部消す
			return;
		}

		if(intersat_check_life_command() == _SUCCESS){
			print(ISAT, "life_command\r\n");
			return;
		}

		intersat_driver_.Info.c2a.ph_ack = PH_analyze_packet(&(intersat_driver_.tcp));
		//TLCは登録結果を記録する
		int ret = tl_command_checker(&(intersat_driver_.tcp));
		if(ret == _SUCCESS){
			p_gs_response_struct->tlc_register_sts = intersat_driver->Info.c2a.ph_ack;
		}

	}
	//ここまで来たら4パケットを処理した後
}

AppInfo intersat_cmd_dispatcher(void)
{
  return AI_create_app_info("intersat_cdis", intersat_cmd_dispatcher_init_, intersat_cmd_dispatcher_);
}

static void intersat_cmd_dispatcher_init_(void)
{
  intersat_cdis_ = CDIS_init(&PH_intersat_cmd_list);
}

static void intersat_cmd_dispatcher_(void)
{
  CDIS_dispatch_command(&intersat_cdis_);
}

//他の衛星宛てのコマンドを自機から送るコマンド
//cobc_dispatch_commandは直接他の衛星に送る時に使うので別で定義
CCP_CmdRet Cmd_intersat_Transmit_command(const CommonCmdPacket* packet){

	intersat_driver->uart_handler = &huart5;
	intersat_driver->TIMEOUT = 0xff;
	TCP* pckt = (TCP*)packet; // const_cast
	// ここでTCP_CMD_DEST_TYPEを宛先で受理できるように変更（なので const cast が発生している．．．）

	const int len =TCP_get_packet_len(pckt) + TCP_PRM_HDR_LEN;

	//実行タイプはリアルタイムコマンドで定義
	CCP_set_exec_type(pckt, CCP_EXEC_TYPE_RT);

	//MOBCでコマンドの実行を行うので
	TCP_set_apid_executer_id(pckt, TCP_EXE_ID_MOBC);

	//chID分長さが変わるから
	TCP_set_packet_data_len(pckt, TCP_get_packet_len(pckt) -2);

	//ここまででcmdとparam以外はcobc宛てに置き換わった
	uint8_t intersat_packet[len - 3];

	//一旦, 相手衛星宛てのpacketにコピー
	for(int i = 0;i < len - 3; i++){
		intersat_packet[i] =  pckt->packet[i];
	}

	intersat_packet[1] ^= 0b00010000;

	intersat_packet[8] = pckt->packet[17];
	intersat_packet[9] = pckt->packet[18];

	if(len >17){
		for(int i = 0;i < (len - 19); i++){
			intersat_packet[i + 17] =  pckt->packet[i + 19];
		}
	}
	print(ISAT,"len is %d\r\n", len);

	for(int i=0;i<len - 1;i++) print(ISAT,"%02x ",pckt->packet[i]);
	print(ISAT,"\r\n");

	for(int i=0;i<len - 1;i++) print(ISAT,"%02x ",intersat_packet[i]);
	print(ISAT,"\r\n");
	INTERSAT_Send(intersat_driver,intersat_packet,len-2);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet intersat_dispatch_command(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  TCP* pckt = (TCP*)packet; // const_cast
  // ここでTCP_CMD_DEST_TYPEを宛先で受理できるように変更（なので const cast が発生している．．．）

  //comment out by sakaguchi
  //この実装では配送先で強制的にRTCに書き換わる
  //TLCとかGSCでも行けるようにする
  //  if (CCP_get_dest_type(pckt) != TCP_CMD_DEST_TYPE_TO_ME)
  //  {
  //    // MOBCのキューに入らず直接転送
  //    // そのままの EXEC_TYPE で転送．なにもしない
  //  }
  //  else
  //  {
  //    // MOBCのキューに溜まった後に実行されたもの
  //    // 配送先OBCではMOBC側のTLなどの影響は受けないはずなのでRTCへ変換
  //    CCP_set_exec_type(pckt, CCP_EXEC_TYPE_RT);
  //  }

	switch(CCP_get_exec_type(pckt)){
		case CCP_EXEC_TYPE_TL_FROM_GS:
//		case CCP_EXEC_TYPE_TL_DEPLOY_BC:
//		case CCP_EXEC_TYPE_TL_DEPLOY_TLM:
			if(p_gs_response_struct->tlc_goal_response & TO_MILLMAX){
				CCP_set_exec_type(pckt, CCP_EXEC_TYPE_GS);
			}
			else{
				CCP_set_exec_type(pckt, CCP_EXEC_TYPE_RT);
			}
			break;
		default:
			break;
	}

  int len =TCP_get_packet_len(pckt) + TCP_PRM_HDR_LEN;
  ret = (DS_CMD_ERR_CODE)INTERSAT_Send(intersat_driver,pckt->packet,len);
  //todo : check type
  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_intersat_CDIS_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet)
{
  (void)packet;

  CDIS_clear_command_list(&intersat_cdis_);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_intersat_CDIS_CLEAR_ERR_LOG(const CommonCmdPacket* packet)
{
  (void)packet;

  // 記録されたエラー情報を解除。
  CDIS_clear_error_status(&intersat_cdis_);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//生存信号かの確認
_ACK intersat_check_life_command(void){
	if(CCP_get_id(&(intersat_driver_.tcp)) == Cmd_CODE_AM_EXECUTE_APP){
		const uint8_t* param = CCP_get_param_head(&(intersat_driver_.tcp));
		uint32_t app_id;
		app_id = param[0]<<24 | param[1]<<16 | param[2]<<8 | param[3];
		if(app_id == AR_NOP){
			return _SUCCESS;
		}
	}
	return _FAILED;
}

_ACK INTERSAT_TCPacket_Fixedheader_Checker(void){
	if(TCP_get_ver(&intersat_driver_.tcp) == TCP_VER_2){             //HeaderのVersion_numberをcheck
		if(TCP_get_2nd_hdr_flag(&intersat_driver_.tcp) == TCP_2ND_HDR_PRESENT)  return _SUCCESS;       //HeaderのSecondary_Header_FlagをCheck
		else return _FAILED;
	}
	else return _FAILED;

}


#pragma section
