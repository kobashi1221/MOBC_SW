/*
 * dbg_packet_handler.c
 *
 *  Created on: 2020/10/21
 *      Author: Noro
 */

#include "dbg_packet_handler.h"
#include <string.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/TlmCmd/packet_handler.h>

static DPH dph_;
const DPH *dph;

//定期的にDBGのポートリセットをかける
static int8_t port_reset_count = 20;

static void DBG_cmd_packet_handler_init_(void);
static void DBG_cmd_packet_handler_(void);
void DBG_get_TCPacket(DBG_STRUCT*);

AppInfo DBG_cmd_packet_handler(void)	//Noro
{
  return AI_create_app_info("DBG_CMD",
                         DBG_cmd_packet_handler_init_,
                         DBG_cmd_packet_handler_);
}

static void DBG_cmd_packet_handler_init_(void)	//Noro
{
  int ret;

  dph = &dph_;
  //何をInitするのかはとりあえず未定だが，GSTOSからの置き換えとして置いている
  //dｂｇ１の各種メンバーの初期化を行う部分
  //恐らくUART割込みのフラグ等を扱う？
  ret = DBG_init(&(dph_.dbg1));

  if (ret != 0)
  {
    printf("!! DBG Init Error %d !!\n", ret);
  }
  //ここでdphの初期化を行う

}

//ここの関数でデータ受信後のパケットの処理を行ってる
static void DBG_cmd_packet_handler_(void)	//Noro
{
  DBG_ACK ack = DBG_receive_tcp(&(dph_.dbg1));

  if (ack == DBG_NOT_RECEIVED)
  {
    // 割込みコールバックでバッファにパケットが入ってない場合は処理を打ち切る
    //　定期的にポートリセットを行う
    -- port_reset_count;
    if(port_reset_count <= 0){
      HAL_UART_DeInit_C2A(&huart8);
      HAL_UART_Init_C2A(&huart8);
      dph_.dbg1.packet_receive_Flag = 0;
      HAL_UART_Receive_IT_C2A(&huart8,(uint8_t *)&DBG_rx_buf,1);
      port_reset_count = 20;
    }
	  return;
  }

  // 受信データ有りの場合は受信時刻情報を更新。
  dph_.tcp_last_recv_time = TMGR_get_master_total_cycle();

  dph_.tcp_ack = ack;

  if (dph_.tcp_ack != DBG_SUCCESS)
  {
    // 受信データがあるが何らかの異常が発生した場合は処理打ち切り
    return;
  }

  DBG_get_TCPacket(&(dph_.dbg1));
  // TCPacket Headerの固定値部分が妥当か確認する。
  // とりあえず今はいらない？
  dph_.tcp_ack = DBG_check_common_tcp_headers(&(dph_.dbg1));

  if (dph_.tcp_ack != DBG_SUCCESS)
  {
    // 内容に異常がある場合はTCFrameを破棄する。
    return;
  }

  // PacketHandlerで受信コマンドパケットを解析
  dph_.cmd_ack = PH_analyze_packet(&(dph_.dbg1.tcp));
}

#ifndef FULLPACKETDBG
 void DBG_get_TCPacket(DBG_STRUCT* dbg)	//Noro
{
	  uint16_t cmd_id;
	  uint8_t param[TCP_MAX_LEN-TCP_PRM_HDR_LEN-TCP_CMD_2ND_HDR_LEN-TCP_CMD_USER_HDR_LEN];
	  int packet_len;

	  packet_len=(dbg->packet_len)-2;
	  cmd_id=(dbg->receive_data[0]<<8)+(dbg->receive_data[1]);
	  memcpy(&(param),&(dbg->receive_data[2]),packet_len);
	  printf("cmd id is %d\r\n" , cmd_id);

//	  CCP_form_tlc(&(dbg->tcp),TMGR_get_master_total_cycle()+5,cmd_id,param,packet_len);
	  CCP_form_rtc(&(dbg->tcp),cmd_id,param,packet_len);
	  /////////////response test//////////////////////
//	  CCP_set_exec_type(&(dbg->tcp), CCP_EXEC_TYPE_GS);
//	  TCP_CMD_set_res_packet_id(&(dbg->tcp), 0x4f);//HK
//	  TCP_CMD_set_res_apid_(&(dbg->tcp), TCP_SAT_ID_GS_2U, TCP_EXE_ID_AFSK);
//	  TCP_CMD_set_res_route(&(dbg->tcp), TCP_ROUTE_DIRECT);
	  ////////////////////////////////////////////////////

	  printf("DBG Packet is \r\n");
	  uint16_t cmd_len = TCP_get_packet_len(&dbg->tcp) + TCP_PRM_HDR_LEN;
	  if(cmd_len > 50)cmd_len = 50;
      for(int i=0; i<cmd_len; i++) printf("%02x " ,dbg->tcp.packet[i]);
      printf("\r\n");
}
#endif

#ifdef FULLPACKETDBG
 void DBG_get_TCPacket(DBG_STRUCT* dbg)	//kimura
{
	  int packet_len;
	  //PICからはTCPが送られてくるので受信パケットをそのままTCPにぶち込む
	  packet_len = (dbg->packet_len);
	  memcpy(&(dbg->tcp.packet), (dbg->receive_data), packet_len);
//	  CCP_set_dst_id(&(dbg->tcp),NU1_TCP_APID_PROCYON_CMD);//とりあえずパケットのAPIDを東大のものに変換
}
#endif
