#pragma section REPRO
/**
* @file   di_rxpic->c
* @brief  rxpicのDIとrxpicのcmd dispatcher
* @Auther 野呂拓臣
*/

#include "di_rxpic.h"
#include "../../Drivers/Com/RXPIC/rxpic.h"
#include "di_struct.h"
#include "../../TlmCmd/user_packet_handler.h"
#include <src_core/Library/print.h>
#include "../UserDefined/Cdh/Communication_Fault_Detection.h"
#include "../../Library/General/DBG_printf.h"
#include <src_user/Applications/DriverInstances/command_response_handler.h>
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include <src_user/Applications/Middleware/MemoryMW.h>

//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyHandle.h>
#include <src_user/Applications/UserDefined/Cdh/ResetUtility.h>
#include <src_user/Applications/DriverInstances/block_command_handler.h>

#include <string.h> //memcpy用

//#define PIC_DBG
static RXPIC_STRUCT rxpic_driver_;
RXPIC_STRUCT* const rxpic_driver = &rxpic_driver_;

static CommandDispatcher rxpic_cdis_;
const CommandDispatcher* const rxpic_cdis = &rxpic_cdis_;

static void rxpic_init_(void);
static void rxpic_update_(void);

static void rxpic_cmd_dispatcher_init_(void);
static void rxpic_cmd_dispatcher_(void);
static _ACK check_rxpic_seq_(void);
static _ACK check_rxpic_seq_cnt_(void);

const uint8_t RXPIC_CountResetThreshold_ = 20;//(s)
static uint8_t RXPIC_CountResetThreshold;
AppInfo rxpic_update(void)
{
  return AI_create_app_info("update_rxpic", rxpic_init_, rxpic_update_);
}

static void rxpic_init_(void)
{
  int ret = RXPIC_UART_init(&rxpic_driver_);
  RXPIC_CountResetThreshold = RXPIC_CountResetThreshold_; //一時的
  SD_read_param(&(rxpic_driver_.sequence_counter), NULL, 2, 0x0010FAE0);
  rxpic_driver_.sequence_counter++;
  if (ret != 0)
  {
    Printf("rxpic init Failed ! %d \n", ret);
  }
}

//RXPICからのパケット確認，アップデート
static void rxpic_update_(void)
{
	for(int i=0; i<RXPIC_MAX_RECEIVE_PACKET_NUM;i++){
	//Packetが入ってないとき
		if(isEmpty(&(rxpic_queue.list))) return;
		//パケットを移してくる
		rxpic_driver_.ack = (_ACK)dequeue(&(rxpic_queue.list),&(rxpic_driver_.tcp));
		//todo : Check Type

		if (rxpic_driver_.ack != _SUCCESS){
			// 受信データがあるが何らかの異常が発生した場合は処理打ち切り
			queue_reset(&(rxpic_queue.list));//カウンタを全部消す
			return;
		}

		// 受信時刻情報を更新。
		ANOMALY_UART_Update_Time(UART_FAULT_RXPIC);
		rxpic_driver_.last_recv_time = TMGR_get_master_total_cycle();

		//debug用
		print(RXPIC,"Ack is %d\r\n",rxpic_driver_.ack);
		print(RXPIC,"receive packet form RXPIC is\r\n");
		for(int i = 0; i < get_packet_len(rxpic_driver_.tcp.packet); i++)
		{
			print(RXPIC,"%02x ", rxpic_driver_.tcp.packet[i]);
		}
		print(RXPIC,"\r\n");

		// TCPacket Headerの固定値部分が妥当か確認する。
		if(RXPIC_TCPacket_Fixedheader_Checker() == _FAILED) return;

		rxpic_driver_.ack = RXPIC_check_tcp_headers(&rxpic_driver_);

		if ((rxpic_driver_.ack != _SUCCESS) && (rxpic_driver_.ack != _TLM_UPDATE_SUCCESS)
				&& (rxpic_driver_.ack != _TLM_AH_UPDATE_SUCCESS))
		{
			queue_reset(&(rxpic_queue.list));//カウンタを全部消す
			queue_count_reset(&rxpic_queue);
			return;
		}

		//RXPIC_TLM_UPDATE_SUCCESSの時はcheck_tcpで構造体にパース終わってるので，パケットハンドラーに渡さない
		if(rxpic_driver_.ack == _SUCCESS || rxpic_driver_.ack == _TLM_AH_UPDATE_SUCCESS){
			rxpic_driver_.Info.c2a.ph_ack = PH_analyze_packet(&(rxpic_driver_.tcp));
			int ret = tl_command_checker(&(rxpic_driver_.tcp));
			if(ret == _SUCCESS){
				p_gs_response_struct->tlc_register_sts = rxpic_driver->Info.c2a.ph_ack;
			}
		}
	}
  //ここまで来たら4パケットを処理した後
}

AppInfo rxpic_cmd_dispatcher(void)
{
	return AI_create_app_info("rxpic_cdis",
                         rxpic_cmd_dispatcher_init_,
                         rxpic_cmd_dispatcher_);
}

static void rxpic_cmd_dispatcher_init_(void)
{
  rxpic_cdis_ = CDIS_init(&PH_rxpic_cmd_list);
}

static void rxpic_cmd_dispatcher_(void)
{
  CDIS_dispatch_command(&rxpic_cdis_);
}

//RXPICへのコマンド送信
CCP_CmdRet rxpic_dispatch_command(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  TCP* pckt = (TCP*)packet; // const_cast
  // ここでTCP_CMD_DEST_TYPEを宛先で受理できるように変更（なので const cast が発生している．．．）

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
//
//  // 配送先OBCが最終到達地なので
//  CCP_set_dest_type(pckt, TCP_CMD_DEST_TYPE_TO_ME);
  int len =TCP_get_packet_len(pckt) + TCP_PRM_HDR_LEN;
  ret = (DS_CMD_ERR_CODE)RXPIC_Send(rxpic_driver,pckt->packet,len);
  //todo: check type
  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_RXPIC_CDIS_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet)
{
  (void)packet;

  CDIS_clear_command_list(&rxpic_cdis_);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_RXPIC_CDIS_CLEAR_ERR_LOG(const CommonCmdPacket* packet)
{
  (void)packet;

  // 記録されたエラー情報を解除。
  CDIS_clear_error_status(&rxpic_cdis_);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*
 * @brief シーケンスカウントリセット
 */
CCP_CmdRet Cmd_RXPIC_seq_reset(const CommonCmdPacket* packet){
	  rxpic_driver_.sequence_counter = 0;
	  SD_write_param(&(rxpic_driver_.sequence_counter), NULL, 2, 0x0010FAE0);
	  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


_ACK RXPIC_TCPacket_Fixedheader_Checker(void){
	if(TCP_get_ver(&rxpic_driver_.tcp) == TCP_VER_2){             //HeaderのVersion_numberをcheck
		if(TCP_get_2nd_hdr_flag(&rxpic_driver_.tcp) == TCP_2ND_HDR_PRESENT)  return _SUCCESS;       //HeaderのSecondary_Header_Flagをcheck
		else return _FAILED;
	}
	else return _FAILED;
}

static _ACK check_rxpic_seq_(void){
	//Seaquence Count に基づいたpacket再送制御を行う
	switch(TCP_get_seq_flag(&rxpic_driver_.tcp)){
	case TCP_SEQ_SINGLE:
	  break;
	case TCP_SEQ_LAST:
	  rxpic_driver_.sequence_counter = 0;
	  break;
	case TCP_SEQ_CONT:
	  rxpic_driver_.ack = check_rxpic_seq_cnt_();
	  break;
	case TCP_SEQ_FIRST:
	  rxpic_driver_.sequence_counter = 0;
	  SD_write_param(&(rxpic_driver_.sequence_counter), NULL, 2, 0x0010FAE0);
	  (rxpic_driver_.sequence_counter++);
	  break;
	default:
		rxpic_driver_.ack = _UNKNOWN;
	}
	if(rxpic_driver_.ack == _UNKNOWN){
		printf("seq_cnt error. %d is correct\r\n", rxpic_driver_.sequence_counter);
	}
	return rxpic_driver_.ack;
}


static _ACK check_rxpic_seq_cnt_(void)
{
//	  _ACK ack = _UNKNOWN;
	  int seq_diff;

	  /* ここはいらないかも	kimura 20211112
	  // Lockout状態なら受信フレームを破棄し処理打ち切り
	  if (gph_.gstos1.lockout_flag == 1)
	  {
	    return GSTOS_IN_LOCKOUT;
	  }

	  ack = check_tcf_contents_(tcf);

	  if (ack != GSTOS_SUCCESS) return ack;
	   */

	  // 受信フレームのSequence Counterと内部管理のType-A Countereの差分を計算
	  seq_diff = TCP_get_seq_cnt(&rxpic_driver_.tcp) - rxpic_driver_.sequence_counter;


	  if (seq_diff == 0)	//　正常受信
	  {
	    // seq_diffが0なら正常受信
	    // シーケンス数のインクリメント
		  SD_write_param(&(rxpic_driver_.sequence_counter), NULL, 2, 0x0010FAE0);
	    ++(rxpic_driver_.sequence_counter);

	    return _SUCCESS;
	  }
	  else{
		  return _UNKNOWN;
	  }
//	  }else if(seq_diff < 0){
//		  printf("seq_cnt should be %d\r\n", rxpic_driver_.sequence_counter);
//		  return _TYPE_AD;
//	  }
//	  else if (seq_diff > 0)	// 飛んだSequence No.を受信
//	  {
//		  if(rxpic_driver_.retransmit_flag == 0){	//1回だけ再送命令
//			  RXPIC_add_retransmit_cmd();
//		  }
//		  printf("seq_cnt should be %d\r\n", rxpic_driver_.sequence_counter);
//		  rxpic_driver_.retransmit_flag = 1;
//		  return _TYPE_AD;
//	  }else{
//		  return _UNKNOWN;
//	  }
}

void RXPIC_add_retransmit_cmd(void){
//	CTCP retransmit_packet;
//		uint8_t issuer_id;
//		uint8_t sat_id;
//
//		int type = TCP_get_type(&rxpic_driver->tcp);	//コマンドかテレメトリかの確認
//
//		if(type == 1){
//			issuer_id = TCP_CMD_get_issuer_id(&rxpic_driver->tcp);
//			sat_id = TCP_CMD_get_issuer_sat_id(&rxpic_driver->tcp);
//		}else if(type ==0){
//			issuer_id = TCP_TLM_get_issuer_id(&rxpic_driver->tcp);
//			sat_id = TCP_TLM_get_issuer_sat_id(&rxpic_driver->tcp);
//		}
//
//		TCP_set_ver(&retransmit_packet, TCP_VER_2);
//		TCP_set_type(&retransmit_packet, TCP_TYPE_CMD);
//		TCP_set_2nd_hdr_flag(&retransmit_packet, TCP_2ND_HDR_PRESENT);
//Commented out Noro it should be fixed
		//APIDを設定
//		if(issuer_id == TCP_EXE_ID_GS){
//			TCP_set_apid_(&retransmit_packet, sat_id, TCP_ROUTE_DIRECT, TCP_EXE_ID_TXPIC);
//		}else{
//			TCP_set_apid_(&retransmit_packet, sat_id, (TCP_ROUTE)TCP_get_apid_route(&retransmit_packet), issuer_id);
//		}
//
//		TCP_set_seq_flag(&retransmit_packet, TCP_SEQ_SINGLE);
//		TCP_set_seq_cnt(&retransmit_packet, 0);
//	#ifdef TWO_U
//		TCP_CMD_set_issuer_id(&retransmit_packet, TCP_SAT_ID_MOTHER, TCP_EXE_ID_MOBC);
//	#endif
//	#ifdef ONE_U
//		TCP_CMD_set_issuer_id(&retransmit_packet, TCP_SAT_ID_CHILD, TCP_EXE_ID_MOBC);
//	#endif
//		TCP_CMD_set_type(&retransmit_packet, TCP_CMD_TYPE_SM);
//		TCP_CMD_set_channel_id(&retransmit_packet, Cmd_CODE_COM_REQUIRE_RETRANSMIT);
//		TCP_CMD_set_exec_type(&retransmit_packet, TCP_CMD_EXEC_TYPE_RT);
//		TCP_set_packet_data_len(&retransmit_packet, TCP_CMD_2ND_HDR_LEN + 8);
//		TCP_CMD_set_param(&retransmit_packet, &rxpic_driver_.sequence_counter, 1);
//
//		PH_analyze_packet(&retransmit_packet);
}


/**
 * @brief  MOBCが自爆した時、RXPICに送る遺言
 * @note   MOBCが落ちたOBC時刻(4byte)＋アノマリグループ･ローカル(各1byte)＋PWRの残り対応回数(1byte)＋Mode_ID(1byte)+リセット回数(2byte)
 * @note   UserDataの合計長 10byte
 */
void rxpic_unexpected_shutdown_send_data_generator(void){
	    static TCP tcp;
		uint8_t contents[10]={0};  //UserDataの部分

		// UserDataの部分        ※リトルエンディアンで送る
	    uint32_t temp0 = TMGR_get_master_total_cycle();                         //落ちたOBC時刻(4byte)
//		uint8_t temp1 = (uint8_t)AL_get_latest_record()->code.group;            //アノマリグループ(1byte)
//		uint8_t temp2 = (uint8_t)AL_get_latest_record()->code.local;            //アノマリローカル(1byte)
		//		  pwr_anomaly.shut_down_limit;                                  //PWRの残り対応回数(1byte)
        //       user_mode->current_mode_id ModeID;                             //再起動したい状態かどうか判断するため(1byte)
		uint16_t temp4 = (uint16_t)p_reset_struct->reset_count[TOTAL_RESET];    //リセット回数(2byte)

		memcpy(contents ,&temp0 ,4);
//		memcpy(contents+4 ,&temp1 ,1);
//		memcpy(contents+5 ,&temp2 ,1);
		memcpy(contents+6 ,&pwr_anomaly.shut_down_limit ,1);
		memcpy(contents+7, &user_mode->current_mode_id ,1);
		memcpy(contents+8 ,&temp4 ,2);

#ifdef TWO_U
		CCP_form_rtc_to_other_obc(&tcp, TCP_APID_RXPIC_2U, (CMD_CODE)0x3148,0x00,sizeof(contents));
#endif
#ifdef ONE_U
		CCP_form_rtc_to_other_obc(&tcp, TCP_APID_RXPIC_1U, (CMD_CODE)0x3148,0x00,sizeof(contents));
#endif
		int packet_len =TCP_get_packet_len(&tcp) + TCP_PRM_HDR_LEN;
		RXPIC_Send(rxpic_driver ,&tcp.packet[0] ,packet_len);

}

#pragma section
