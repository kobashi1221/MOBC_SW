#pragma section REPRO
/**
* @file   di_sobc->c
* @brief  rxpicのDIとrxpicのcmd dispatcher
* @Auther 小林尚生
*/

#include "di_sobc.h"
#include "../../Drivers/Com/SOBC/SOBC.h"
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

static SOBC_STRUCT sobc_driver_;
SOBC_STRUCT* const sobc_driver = &sobc_driver_;
TCP tcp_temp;

static CommandDispatcher sobc_cdis_;
const CommandDispatcher* const sobc_cdis = &sobc_cdis_;

static void sobc_init_(void);
static void sobc_update_(void);

static void sobc_cmd_dispatcher_init_(void);
static void sobc_cmd_dispatcher_(void);

AppInfo sobc_update(void)
{
  return AI_create_app_info("update_sobc", sobc_init_,sobc_update_);
}

const uint8_t SOBC_CountResetThreshold_ = 20;//(s)
static uint8_t SOBC_CountResetThreshold;

static void sobc_init_(void)
{
  int ret = SOBC_SPI_init(&sobc_driver_);
  SOBC_CountResetThreshold = SOBC_CountResetThreshold_; //一時的
//  SD_read_param(&(sobc_driver_.sequence_counter), NULL, 2, 0x0010FAE0);
//  sobc_driver_.sequence_counter++;
  if (ret != 0)
  {
    Printf("sobc init Failed ! %d \n", ret);
  }
}


//他機からのパケット確認,アップデート
static void sobc_update_(void){
//  printf("update");
    for(int i=0; i<SOBC_MAX_RECEIVE_PACKET_NUM;i++){
        //Packetが入ってないとき
        if(isEmpty(&(sobc_queue.list))) {
            return;
        }
        //パケットを移してくる
        sobc_driver_.ack = (_ACK)dequeue(&(sobc_queue.list),&(sobc_driver_.tcp));
        //todo : Check Type

        if (sobc_driver_.ack != _SUCCESS){
            // 受信データがあるが何らかの異常が発生した場合は処理打ち切り
            queue_reset(&(sobc_queue.list));//カウンタを全部消す
            return;
        }

        // 受信時刻情報を更新。
        sobc_driver_.last_recv_time = TMGR_get_master_total_cycle();

        //debug用
        print(SOBC,"Ack is %d\r\n",sobc_driver_.ack);
        print(SOBC," receive packet from SOBC is \r\n");
        for(int i = 0; i < get_packet_len(sobc_driver_.tcp.packet); i++)
        {
            print(SOBC,"%02x ", sobc_driver_.tcp.packet[i]);
        }
        print(SOBC,"\r\n");

        // TCPacket Headerの固定値部分が妥当か確認する。
        if(SOBC_TCPacket_Fixedheader_Checker() == _FAILED) return;

        sobc_driver_.ack = SOBC_check_tcp_headers(&sobc_driver_);

        if ((sobc_driver_.ack != _SUCCESS) && (sobc_driver_.ack != _TLM_UPDATE_SUCCESS)){
            queue_reset(&(sobc_queue.list));//カウンタを全部消す
            return;
        }

        if(sobc_check_life_command() == _SUCCESS){
//            printf("life_command\r\n");
            return;
        }

        sobc_driver_.Info.c2a.ph_ack = PH_analyze_packet(&(sobc_driver_.tcp));
        //TLCは登録結果を記録する
        int ret = tl_command_checker(&(sobc_driver_.tcp));
        if(ret == _SUCCESS){
            p_gs_response_struct->tlc_register_sts = sobc_driver->Info.c2a.ph_ack;
        }

    }
    //ここまで来たら4パケットを処理した後
}

AppInfo sobc_cmd_dispatcher(void)
{
  return AI_create_app_info("sobc_cdis",
                         sobc_cmd_dispatcher_init_,
                         sobc_cmd_dispatcher_);
}

static void sobc_cmd_dispatcher_init_(void)
{
  sobc_cdis_ = CDIS_init(&PH_sobc_cmd_list);
}

static void sobc_cmd_dispatcher_(void)
{
    CDIS_dispatch_command(&sobc_cdis_);
}

//SOBCへのコマンド送信
CCP_CmdRet sobc_dispatch_command(const CommonCmdPacket* packet)
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
  ret = (DS_CMD_ERR_CODE)SOBC_SendReceive(sobc_driver,pckt->packet, len);
  //todo: check type
  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}


_ACK SOBC_TCPacket_Fixedheader_Checker(void){
    if(TCP_get_ver(&sobc_driver_.tcp) == TCP_VER_2){             //HeaderのVersion_numberをcheck
        if(TCP_get_2nd_hdr_flag(&sobc_driver_.tcp) == TCP_2ND_HDR_PRESENT)  return _SUCCESS;       //HeaderのSecondary_Header_FlagをCheck
        else return _FAILED;
    }
    else return _FAILED;

}


CCP_CmdRet sobc_debug_command_send(const CommonCmdPacket* packet){
  TCP* pckt = (TCP*)packet; // const_cast
  // ここでTCP_CMD_DEST_TYPEを宛先で受理できるように変更（なので const cast が発生している．．．）
  const int len =TCP_get_packet_len(pckt) + TCP_PRM_HDR_LEN;

  for(int i=0;i<len;i++) printf("%02x ",pckt->packet[i]);
  printf("\r\n");

  //実行タイプはリアルタイムコマンドで定義
  CCP_set_exec_type(pckt, CCP_EXEC_TYPE_RT);
  TCP_CMD_set_ti(pckt, 0x00000000);
#ifdef TWO_U
  TCP_set_apid_sat_id(pckt, TCP_SAT_ID_2U);
  TCP_set_all_issuer_id(pckt, TCP_SAT_ID_2U, TCP_EXE_ID_MOBC);
#endif
#ifdef ONE_U
  TCP_set_apid_sat_id(pckt, TCP_SAT_ID_1U);
  TCP_set_all_issuer_id(pckt, TCP_SAT_ID_1U, TCP_EXE_ID_MOBC);
#endif
  //SOBCでコマンドの実行を行うので
  TCP_set_apid_executer_id(pckt, TCP_EXE_ID_SOBC);

  //chID分長さが変わるから
  TCP_set_packet_data_len(pckt, TCP_get_packet_len(pckt) -2);

  //ここまででcmdとparam以外はsobc宛てに置き換わった
  uint8_t sobc_packet[len - 3];

  //一旦, sobc宛てのpacketにコピー
  for(int i = 0;i < len - 3; i++){
    sobc_packet[i] =  pckt->packet[i];
  }

  sobc_packet[8] = pckt->packet[17];
  sobc_packet[9] = pckt->packet[18];

  if(len >19){
    for(int i = 0;i < (len - 19); i++){
      sobc_packet[i + 17] =  pckt->packet[i + 19];
    }
  }
  for(int i=0;i<len-2;i++) printf("%02x ",sobc_packet[i]);
  printf("\r\n");

  int ret = SOBC_SendReceive(sobc_driver, &sobc_packet[0], TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN);
  return CCP_make_cmd_ret_without_err_code(ret);
}


//SOBCからの生存信号の確認関数(引数が1,2,3,4,...239のこのコマンドが毎周期SOBCから送られてくる)
_ACK sobc_check_life_command(void){
//  const uint8_t* param = CCP_get_param_head(&(sobc_queue.dri.receive_buf));
  const uint8_t* param = &(sobc_queue.dri.receive_buf[TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN]);
  for(int i = 0; i < TCP_MAX_LEN - TCP_PRM_HDR_LEN - TCP_CMD_2ND_HDR_LEN - TCP_CMD_USER_HDR_LEN ; i ++){
    if(param[i] != i){
      print(SOBC,"ERROR: packet[%d] should %d but %d\r\n", i, i, param[i]);
      return _FAILED;
    }
  }
    return _SUCCESS;
  print(SOBC,"DEBUG_NOP_SOBC\r\n");
}


//仮の生存信号用の関数(SOBCのテレメトリ要求が決まり次第関数の中身を変更)
void SOBC_Regular_Tlm_ganerator(void){
  static TCP tcp;
//  TCP tcp_temp;
  uint8_t check_ret;
//  printf("flag = %d",sobc_driver->sobc_regular_tlm_flag);
  if(sobc_driver->sobc_regular_tlm_flag == 1){
    //setting header
    TCP_set_ver(&tcp, TCP_VER_2);
    TCP_set_type(&tcp, TCP_TYPE_CMD);
    TCP_set_2nd_hdr_flag(&tcp, TCP_2ND_HDR_PRESENT);
    TCP_set_apid_project_id(&tcp,MAGNAROII);
  #ifdef TWO_U
      TCP_set_apid_sat_id(&tcp, TCP_SAT_ID_2U);
      TCP_set_all_issuer_id(&tcp, TCP_SAT_ID_2U, TCP_EXE_ID_MOBC);
  #endif
  #ifdef ONE_U
      TCP_set_apid_sat_id(&tcp, TCP_SAT_ID_1U);
      TCP_set_all_issuer_id(&tcp, TCP_SAT_ID_1U, TCP_EXE_ID_MOBC);
  #endif
    TCP_set_apid_executer_id(&tcp, TCP_EXE_ID_SOBC);
    TCP_set_seq_flag(&tcp, TCP_SEQ_SINGLE);
    TCP_set_seq_cnt(&tcp, 0x0000);
    TCP_set_route(&tcp, TCP_ROUTE_DIRECT);
    TCP_CMD_set_channel_id(&tcp, 0xff06);//仮で置いている(SOBC側で生存信号のchannel IDが決定し次第変更)
    TCP_CMD_set_res_packet_id(&tcp, 0xff);
    TCP_CMD_set_res_apid_(&tcp, TCP_SAT_ID_MAX, TCP_EXE_ID_MAX);
    TCP_CMD_set_res_route(&tcp, TCP_ROUTE_MAX);
    TCP_CMD_set_exec_type(&tcp, TCP_CMD_EXEC_TYPE_RT);
    TCP_CMD_set_ti(&tcp, 0x00000000);
    //setting user data
    for(int i = 0; i < 256 - TCP_PRM_HDR_LEN - TCP_CMD_2ND_HDR_LEN - TCP_CMD_USER_HDR_LEN; i++){
      tcp.packet[i + TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN] = i;
    }
    TCP_set_packet_data_len(&tcp, 239 + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN);
    SOBC_SendReceive(sobc_driver, tcp.packet, 256);
//    printf("receive = ");
    //receiveしたデータをtcp_tempに格納(一時的な確認用扱いやすい)
    for(int i=0; i < 256; i ++){
      tcp_temp.packet[i] = sobc_queue.dri.receive_buf[i];
//      printf("sobc = %d,tcp = %d\r\n",sobc_queue.dri.receive_buf[i], tcp_temp.packet[i]);
    }
//    printf("\r\n");
    //----------------------------------通信のアノマリのカウントアップ-------------------------------------
    sobc_driver->sobc_anomaly.com_exe_counter++; //関数の総実行回数
    //-------------receive dataの中身確認
    check_ret = _SUCCESS;
#ifdef TWO_U
  //  check primary header
    if(TCP_get_ver(&tcp_temp) != TCP_VER_2 || TCP_get_type(&tcp_temp) != TCP_TYPE_CMD || TCP_get_2nd_hdr_flag(&tcp_temp) != TCP_2ND_HDR_PRESENT
        || TCP_get_apid_(&tcp_temp) != TCP_APID_MOBC_2U || TCP_get_seq_flag(&tcp_temp) != TCP_SEQ_SINGLE
        || TCP_get_seq_cnt(&tcp_temp) != 0x00000000 || TCP_get_packet_len(&tcp_temp) != 239 + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN){
//      printf("primary\r\n");
      check_ret = _FAILED;
    }
    //check secondary header
    if(TCP_get_route(&tcp_temp) != TCP_ROUTE_DIRECT || TCP_get_issuer_sat_id(&tcp_temp) != TCP_SAT_ID_2U || TCP_get_issuer_id(&tcp_temp) != TCP_EXE_ID_SOBC){
//      printf("secondary\r\n");
      check_ret = _FAILED;
    }
#endif
#ifdef ONE_U
    //  check primary header
      if(TCP_get_ver(&tcp_temp) != TCP_VER_2 || TCP_get_type(&tcp_temp) != TCP_TYPE_CMD || TCP_get_2nd_hdr_flag(&tcp_temp) != TCP_2ND_HDR_PRESENT
          || TCP_get_apid_(&tcp_temp) != TCP_APID_MOBC_1U || TCP_get_seq_flag(&tcp_temp) != TCP_SEQ_SINGLE
          || TCP_get_seq_cnt(&tcp_temp) != 0x00000000 || TCP_get_packet_len(&tcp_temp) != 239 + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN){
  //      printf("primary\r\n");
        check_ret = _FAILED;
      }
      //check secondary header
      if(TCP_get_route(&tcp_temp) != TCP_ROUTE_DIRECT || TCP_get_issuer_sat_id(&tcp_temp) != TCP_SAT_ID_1U || TCP_get_issuer_id(&tcp_temp) != TCP_EXE_ID_SOBC){
  //      printf("secondary\r\n");
        check_ret = _FAILED;
      }
#endif
    //check application data
    uint8_t* param = &(sobc_queue.dri.receive_buf[TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN]);
    for(int i = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN; i < TCP_MAX_LEN; i ++){
      if(param[i - (TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN)] != i - (TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN)){
//        printf("content[%d] should %d but %d\r\n",i, i - (TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN), param[i]);
        check_ret = _FAILED;
      }
    }
    if(check_ret == _FAILED){
      sobc_driver->sobc_anomaly.com_content_anomary ++; //受信したデータが異なる場合の回数
    }
    else if(check_ret == _SUCCESS){
      sobc_driver->sobc_anomaly.com_success_counter++;
    }

    if(sobc_driver->sobc_anomaly.com_exe_counter % 5 == 0){
      print(SOBC," execution = %d\r\n transmit_success = %d\r\n hard anomaly = %d\r\n receive contents anomaly = %d\r\n", sobc_driver->sobc_anomaly.com_exe_counter
          ,sobc_driver->sobc_anomaly.com_success_counter,sobc_driver->sobc_anomaly.com_anomaly_counter,sobc_driver->sobc_anomaly.com_content_anomary);
    }
  }
}
