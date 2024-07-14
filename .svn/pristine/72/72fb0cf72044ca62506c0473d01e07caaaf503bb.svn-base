#pragma section REPRO
/**
* @file   di_txpic.c
* @brief  txpicのDIとrxpicのcmd dispatcher
* @Auther 野呂拓臣
*/

#include "di_txpic.h"
#include "../../Drivers/Com/TXPIC/txpic.h"
#include "di_struct.h"
#include "../../TlmCmd/user_packet_handler.h"
#include <src_core/Library/print.h>
#include "../../Settings/port_config.h"
#include <string.h>
#include "../UserDefined/Cdh/Communication_Fault_Detection.h"
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include "../../Library/General/DBG_printf.h"
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_user/Applications/UserDefined/Memory/MEM_cmd_SD.h>

//#define PIC_DBG

static TXPIC_STRUCT txpic_driver_;
TXPIC_STRUCT* const txpic_driver = &txpic_driver_;

static CommandDispatcher txpic_cdis_;
const CommandDispatcher* const txpic_cdis = &txpic_cdis_;


static void txpic_init_(void);
static void txpic_update_(void);
static void txpic_cmd_dispatcher_init_(void);
static void txpic_cmd_dispatcher_(void);

const uint8_t TXPIC_CountResetThreshold_ = 20;//(s)
static uint8_t TXPIC_CountResetThreshold;

AppInfo txpic_update(void)
{
  return AI_create_app_info("update_txpic", txpic_init_, txpic_update_);
}

static void txpic_init_(void)
{
  int ret = TXPIC_UART_init(&txpic_driver_);
  TXPIC_CountResetThreshold = TXPIC_CountResetThreshold_; //一時的
  if (ret != 0)
  {
    Printf("txpic init Failed ! %d \n", ret);
  }
}

//TXPICからのパケット確認，アップデート
static void txpic_update_(void)
{
	for(int i=0; i<TXPIC_MAX_RECEIVE_PACKET_NUM;i++){
		//Packetが入ってないとき
		if(isEmpty(&(txpic_queue.list))) return;

		//パケットを移してくる
		txpic_driver_.ack = (_ACK)dequeue(&(txpic_queue.list),&(txpic_driver_.tcp));
		//todo : Check Type

		if (txpic_driver_.ack != _SUCCESS){
			// 受信データがあるが何らかの異常が発生した場合は処理打ち切り
			queue_reset(&(txpic_queue.list));//カウンタを全部消す
			return;
		}

		// 受信時刻情報を更新。
		ANOMALY_UART_Update_Time(UART_FAULT_TXPIC);
		txpic_driver_.last_recv_time = TMGR_get_master_total_cycle();

		// TCPacket Headerの固定値部分が妥当か確認する。
		if(TXPIC_TCPacket_Fixedheader_Checker() == _FAILED){
			queue_reset(&(txpic_queue.list));//カウンタを全部消す
			queue_count_reset(&txpic_queue);
			return;
		}

		txpic_driver_.ack = TXPIC_check_tcp_headers(&txpic_driver_);

		if ((txpic_driver_.ack != _SUCCESS) && (txpic_driver_.ack != _TLM_UPDATE_SUCCESS &&
				txpic_driver_.ack != _TLM_AH_UPDATE_SUCCESS))
		{
			queue_reset(&(txpic_queue.list));//カウンタを全部消す
			queue_count_reset(&txpic_queue);
			return;
		}

        //debug用
		print(TXPIC,"receive packet from TXPIC is\r\n");
		for(int i = 0; i < get_packet_len(txpic_driver_.tcp.packet); i++)
		{
		  print(TXPIC,"%02x ", txpic_driver_.tcp.packet[i]);
		}
		print(TXPIC,"\r\n");

		//TXPIC_TLM_UPDATE_SUCCESSの時はcheck_tcpで構造体にパース終わってるので，パケットハンドラーに渡さない
		if(txpic_driver_.ack == _SUCCESS || txpic_driver_.ack == _TLM_AH_UPDATE_SUCCESS)
			txpic_driver_.Info.c2a.ph_ack = PH_analyze_packet(&(txpic_driver_.tcp));
	}
	//ここまで来たら3パケットを処理した後
}

AppInfo txpic_cmd_dispatcher(void)
{
  return AI_create_app_info("txpic_cdis",
                         txpic_cmd_dispatcher_init_,
                         txpic_cmd_dispatcher_);
}

static void txpic_cmd_dispatcher_init_(void)
{
  txpic_cdis_ = CDIS_init(&PH_txpic_cmd_list);
}

static void txpic_cmd_dispatcher_(void)
{
  CDIS_dispatch_command(&txpic_cdis_);
}


CCP_CmdRet txpic_dispatch_command(const CommonCmdPacket* packet)
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
  ret = (DS_CMD_ERR_CODE)TXPIC_Send(txpic_driver,pckt->packet,len);
  if(ret != DS_CMD_OK &&
		  TCP_CMD_get_channel_id(packet) != (CMD_CODE)0xc140 &&
		  	  txpic_driver-> reanalyze_packet == 0x01){//生存信号以外のコマンド(コマンド応答など)は再度アナライズ
	 printf("re-analyze packet\r\n");
	 PH_analyze_packet(pckt);
  }
  //todo : check type
  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_txpic_CDIS_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet)
{
  (void)packet;

  CDIS_clear_command_list(&txpic_cdis_);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_txpic_CDIS_CLEAR_ERR_LOG(const CommonCmdPacket* packet)
{
  (void)packet;

  // 記録されたエラー情報を解除。
  CDIS_clear_error_status(&txpic_cdis_);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

_ACK TXPIC_TCPacket_Fixedheader_Checker(void)
{
	if(TCP_get_ver(&txpic_driver_.tcp) == TCP_VER_2){             //HeaderのVersion_numberをcheck
		if(TCP_get_2nd_hdr_flag(&txpic_driver_.tcp) == TCP_2ND_HDR_PRESENT)  return _SUCCESS;       //HeaderのSecondary_Header_FlagをCheck
		else return _FAILED;
	}
	else return _FAILED;

//	  int packet_len;
//	  //PICからはTCPが送られてくるので受信パケットをそのままTCPにぶち込む
//	  packet_len = (txpic->packet_len);
//	  memcpy(&(txpic->tcp.packet), (txpic->receive_data), packet_len);
//	  CCP_set_dest_id(&(txpic->tcp),TCP_APID_MOBC_CMD);//とりあえずパケットのAPIDを東大のものに変換
//	  //APIDを名大のものに書き換えたので、上書きせず行けるはず
}

#ifdef ONE_U
//* *      Author: Osaki
// * @brief　  Cmd of sending image TLM to GS(txpic)
// * packet[15] : pic_num
// * packet[16] : packet num to send (1~3)
//*/
CCP_CmdRet Cmd_SendImageTLM_TXPIC(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);

	uint32_t SCAMstamp1_start_block_addr = Storage_Area[Tlm_CODE_1U_IMAGE_TLM_1*2];
	uint32_t SCAMstamp2_start_block_addr = Storage_Area[Tlm_CODE_1U_IMAGE_TLM_2*2];
	uint32_t SCAMstamp3_start_block_addr = Storage_Area[Tlm_CODE_1U_IMAGE_TLM_3*2];
	int SD_size = 512;
	uint8_t stamp_buf[512] = {0};
	TCP tcp;

    TCP_set_ver(&tcp, TCP_VER_2);    // 000
    TCP_set_type(&tcp, TCP_TYPE_CMD);    //0
    TCP_set_2nd_hdr_flag(&tcp, TCP_2ND_HDR_PRESENT); //
	TCP_set_apid_project_id(&tcp,MAGNAROII);
    TCP_set_apid_(&tcp, TCP_SAT_ID_GS_1U, TCP_EXE_ID_AFSK);
    TCP_set_seq_flag(&tcp, TCP_SEQ_SINGLE); //0b11
    TCP_set_seq_cnt(&tcp, 0x0000);
	TCP_set_route(&tcp, TCP_ROUTE_DIRECT);
    TCP_set_all_issuer_id(&tcp, TCP_SAT_ID_1U, TCP_EXE_ID_MOBC);
    TCP_TLM_set_ti(&tcp, 0x00000000);
    TCP_TLM_set_category(&tcp, 0x40); //0x40 : Mission TLM
    tcp.packet[12] = 0x00; //for blank of TLM_isuuer_id(temporary)

    // send TLM of only 1 picture
   for (int packet=0; packet<param[1]; packet++){
		if(packet==0){
		    TCP_set_packet_data_len(&tcp, 0x007B); //7+100+16 = 123 = 0x007B (Data <= 243byte)
			TCP_TLM_set_packet_id(&tcp, Tlm_CODE_1U_IMAGE_TLM_1); //1U_IMAGE_TLM_1
			int ret_r = sd_read_address(SCAMstamp1_start_block_addr + param[0],
						0x00000000, stamp_buf, SD_size); //MAX 2block
			if(ret_r != 0){
				printf("read err(init)\r\n");
			}
			for (int i=0; i<100; i++){
				memcpy(&tcp.packet[13 + i],	&stamp_buf[i], 1);
			}
		}
		else if(packet==1){
		    TCP_set_packet_data_len(&tcp, 0x00D1); //7+107+95 = 209 = 0x00D1 (Data <= 243byte)
			TCP_TLM_set_packet_id(&tcp, Tlm_CODE_1U_IMAGE_TLM_2); //1U_IMAGE_TLM_2
			int ret_r = sd_read_address(SCAMstamp2_start_block_addr + param[0],
						0x00000000, stamp_buf, SD_size); //MAX 2block
			if(ret_r != 0){
				printf("read err(init)\r\n");
			}
			for (int i=0; i<202; i++){
				memcpy(&tcp.packet[13 + i],	&stamp_buf[i], 1);
			}
		}
		else if(packet==2){
		    TCP_set_packet_data_len(&tcp, 0x00F9); //7+101+141 = 249 = 0x00F9 (Data <= 243byte)
			TCP_TLM_set_packet_id(&tcp, Tlm_CODE_1U_IMAGE_TLM_3); //1U_IMAGE_TLM_3
			int ret_r = sd_read_address(SCAMstamp3_start_block_addr + param[0],
						0x00000000, stamp_buf, SD_size); //MAX 2block
			if(ret_r != 0){
				printf("read err(init)\r\n");
			}
			for (int i=0; i<242; i++){
				memcpy(&tcp.packet[13 + i],	&stamp_buf[i], 1);
			}
		}
//    		for(int i = 13;i < 63; i++){
//    			printf("tcp.packet[%d] %02x \r\n", i, tcp.packet[i]);
//    		}
		PH_analyze_packet(&tcp);
	}
    printf("Send to GS(Image TLM)\n");
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
#endif/* for 1U*/

#pragma section
