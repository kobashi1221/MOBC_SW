/*
 * di_LoRa.c
 *
 *  Created on: 2021/11/16
 *      Author: sakaguchi
 *
 *  update : 2023/11/14
 *      Author: sakaguchi
 */

#include "di_LoRa.h"
#include "../../Drivers/Cdh/LoRa/LoRa_Struct.h"
#include "../../Drivers/Cdh/LoRa/LoRa_func.h"

#include <string.h> //for memcpy
#include <stdlib.h> //for malloc
#include <string.h>
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include "../../TlmCmd/user_packet_handler.h"
#include <src_user/Applications/DriverInstances/command_response_handler.h>
#include <src_user/Library/General/DBG_printf.h>

static uint8_t counter;

Queue lora_queue;
static Buffer receive_buf[LORA_MAX_RECEIVE_PACKET_NUM];

static LoRa_STRUCT lora_driver_;
LoRa_STRUCT* const lora_driver = &lora_driver_;

static LoRa_Register lora_register_;
LoRa_Register* const lora_register = &lora_register_;

static CommandDispatcher lora_cdis_;
const CommandDispatcher* const lora_cdis = &lora_cdis_;

static void COM_LoRa_receive_exe(void);
static void lora_update_(void);

static void lora_cmd_dispatcher_init_(void);
static void lora_cmd_dispatcher_(void);

AppInfo COM_LoRa_receive_update(void) {
  return AI_create_app_info("LoRa_rec", NULL, COM_LoRa_receive_exe);
}

AppInfo lora_update(void) {
  return AI_create_app_info("LoRa_update", NULL, lora_update_);
}

AppInfo lora_cmd_dispathcer(void) {
  return AI_create_app_info("cobc_cdis", lora_cmd_dispatcher_init_,lora_cmd_dispatcher_);
}


//LoRaの初期化関数
void COM_LoRa_init(void) {
  print(LORA, "LoRaInitialize!\r\n");
  LORA_COM_init(lora_driver);
  queue_init(&lora_queue, LORA_MAX_RECEIVE_PACKET_NUM, receive_buf);

  c2a_delay_ms(10);

  if (!LoRa_begin(lora_driver)) {
    printf("LoRa initialized failed!!!!!!\r\n");
  }
  LoRa_load_default_setting(lora_driver);

  memcpy(&counter, &lora_driver->wait_counter,1); //MRAMパラメータで設定した受信待機秒数をstatic変数に代入

  // 受信待機へ
  LORA_Receive_Prepare(lora_driver);
}

static void lora_cmd_dispatcher_init_(void) {
  lora_cdis_ = CDIS_init(&PH_lora_cmd_list);
}

static void lora_cmd_dispatcher_(void) {
  CDIS_dispatch_command(&lora_cdis_);
}

CCP_CmdRet lora_dispatch_command(const CommonCmdPacket* packet) {
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

  switch(CCP_get_exec_type(pckt)) {
    case CCP_EXEC_TYPE_TL_FROM_GS:
//      case CCP_EXEC_TYPE_TL_DEPLOY_BC:
//      case CCP_EXEC_TYPE_TL_DEPLOY_TLM:
      if(p_gs_response_struct->tlc_goal_response & TO_LORA) {
        CCP_set_exec_type(pckt, CCP_EXEC_TYPE_GS);
      }
      else {
        CCP_set_exec_type(pckt, CCP_EXEC_TYPE_RT);
      }
      break;
    default:
      break;
  }

  int len = TCP_get_packet_len(pckt) + TCP_PRM_HDR_LEN;
  ret = (DS_CMD_ERR_CODE)LORA_Send(lora_driver,pckt->packet,len);
  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

static void lora_update_(void) {
  for(int i=0; i<LORA_MAX_RECEIVE_PACKET_NUM;i++) {
    //Packetが入ってないとき
    if(isEmpty(&(lora_queue.list))) return;
    //パケットを移してくる
    lora_driver_.ack = (_ACK)dequeue(&(lora_queue.list),&(lora_driver_.tcp));

    if (lora_driver_.ack != _SUCCESS) {
    // 受信データがあるが何らかの異常が発生した場合は処理打ち切り
      queue_reset(&(lora_queue.list));//カウンタを全部消す
      return;
    }

    // 受信時刻情報を更新。
    lora_driver_.last_recv_time = TMGR_get_master_total_cycle();

    //debug用
    print(LORA,"receive packet from LoRa is\r\n");
    for(int i = 0; i < get_packet_len(lora_driver_.tcp.packet); i++) {
      print(LORA,"%02x ", lora_driver_.tcp.packet[i]);
    }
    print(LORA,"\r\n");

    // TCPacket Headerの固定値部分が妥当か確認する。
    if(lora_TCPacket_Fixedheader_Checker() == _FAILED) return;

    if(lora_check_life_command() == _SUCCESS) {
      return;
    }

    lora_driver_.Info.c2a.ph_ack = PH_analyze_packet(&(lora_driver_.tcp));
    //TLCは登録結果を記録する
    int ret = tl_command_checker(&(lora_driver_.tcp));
    if(ret == _SUCCESS) {
      p_gs_response_struct->tlc_register_sts = lora_driver_.Info.c2a.ph_ack;
    }
  }
  //ここまで来たら4パケットを処理した後
}

// Headerの固定値部分が妥当か確認する関数
_ACK lora_TCPacket_Fixedheader_Checker(void) {
  if(TCP_get_ver(&lora_driver_.tcp) == TCP_VER_2) { // HeaderのVersion_numberをcheck
    if(TCP_get_2nd_hdr_flag(&lora_driver_.tcp) == TCP_2ND_HDR_PRESENT)  return _SUCCESS; // HeaderのSecondary_Header_FlagをCheck
    else return _FAILED;
  }
  else return _FAILED;
}

//生存信号かの確認
_ACK lora_check_life_command(void) {
  if(CCP_get_id(&(lora_driver_.tcp)) == Cmd_CODE_AM_EXECUTE_APP) {
    const uint8_t* param = CCP_get_param_head(&(lora_driver_.tcp));
    uint32_t app_id;
    app_id = param[0]<<24 | param[1]<<16 | param[2]<<8 | param[3];
    if(app_id == AR_NOP) {
      return _SUCCESS;
    }
  }
  return _FAILED;
}

//LoRaの受信関数
static void COM_LoRa_receive_exe(void) {
  uint8_t state = 0;
  state = PWR_State_GET(PWR_State_LORA);

  if(state == 0x01) { // LoRa OFFならそもそも関数へ入らない
    // receive flagが立っていないとき(=>データ送信中, 前の受信データを処理できていない)
    if(lora_driver->receive_enable == 0) {
      // receive flagが立てられるかを確認, 立てられるなら受信待機へ
      if(check_recieve_enable(lora_driver) == _SUCCESS) {
        LORA_Receive_Prepare(lora_driver);
      }
      else {
        return;
      }
    }

    // ここまで来たら受信待機
    // RX_CADモードの場合は, Preambleを検知したらRX_CONTINUOUSへ変更する必要がある
    if(lora_driver->lora_rx_mode == RX_CAD) { // LoRa変調を受信したら一時的にRX_CONTINUOUSへ移行し, ペイロードを受信
      if(HAL_GPIO_ReadPin_C2A(lora_driver->dio4_GPIO, lora_driver->dio4_pin) == GPIO_PIN_SET){
        LoRa_writeRegister(lora_driver, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
      }
      else { // LoRa変調を受け取るまでは, CADで常に待機
        LoRa_mode_cad(lora_driver);
      }
    }

    //毎周期データが受信しているかを確認し, FIFOにデータが入っていたら受信処理へ
    uint16_t BufSize = 0;
    TCP in_buf, out_buf;
    BufSize = LoRa_Rx_Size(lora_driver); // 受信したbyte数(LoRaのFIFOに入っているbyte数)
    print(LORA,"LoRa BufSize : %02x \r\n", (uint8_t) BufSize); // for debug

    if(BufSize > 0 && counter != 0) { //　データをすべて受信するまで(1sで減少するcounterが0になるまで)待機
      //受信待機のcounterはMRAMパラメータで設定（これを設定しないと, 受信しきる前にパケット処理に回してしまうため)
      //地上試験では, は1sで設定
      --counter;
      print(LORA,"counter is %d \r\n",counter);
      return;
    }
    else if(counter == 0) { // データをすべて受信したら,　受信->復号->packet analyze(この時, LoRaは再度受信モードへ)
      memcpy(&counter, &lora_driver->wait_counter,1);
      LORA_Recieve(lora_driver, &in_buf.packet[0], 256);

      // RSSIを取得
      lora_driver->lora_tlm.reg_info.receive_info = LoRa_readRegister(lora_driver, REG_RSSI_WIDEBAND) << 24 |
          LoRa_readRegister(lora_driver, REG_PKT_SNR_VALUE) << 16 | LoRa_readRegister(lora_driver, REG_PKT_RSSI_VALUE) << 8 | LoRa_readRegister(lora_driver, REG_RSSI_VALUE);

      if(lora_decryption(lora_driver, &in_buf, &out_buf, BufSize) != _SUCCESS) { // 復号化失敗なら
        LORA_Receive_Prepare(lora_driver);
        return;
      }

      if(lora_length_checker(&out_buf.packet[0], BufSize)) { // success
        print(LORA,"success\r\n");
        enqueue(&(lora_queue.list), &out_buf.packet[0]); //queueに入れる
        LORA_Receive_Prepare(lora_driver);
      }
      else {
        LORA_Receive_Prepare(lora_driver);
      }

      ///////////////////////////debug LoRa/////////////////////////////////////////////////////////////////////////
//      for(int i = 0; i < BufSize; i++) {
//        print(LORA,"%02x ", out_buf.packet[0]);
//      }
    }
  }
}

//受信したbyte数の確認
uint8_t lora_length_checker(uint8_t* buffer, size_t size) {
  uint16_t packet_length;
  ENDIAN_memcpy(&packet_length, &buffer[4], 2);
  packet_length = packet_length + TCP_PRM_HDR_LEN + 1; // hedear+0起算の１

  if(packet_length == size && packet_length < TCP_MAX_LEN) {
    return 1;
  }
  else { //受信byteがおかしければ, packet analyzeすることなく再度受信モードへ
    LORA_Receive_Prepare(&lora_driver_);
    return 0;
  }
}
