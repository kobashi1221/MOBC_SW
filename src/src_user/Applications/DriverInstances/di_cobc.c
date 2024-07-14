/*
 * di_cobc.c
 *
 *  Created on: 2021/10/16
 *      Author: daiki
 *
 *  Updated on: 2022/01/12
 *      Autohr: Sakaguchi
 */

#include "di_cobc.h"
#include "../../Drivers/Com/COBC/cobc.h"
#include "../../TlmCmd/user_packet_handler.h"
#include <src_core/Library/print.h>
#include "../UserDefined/Cdh/Communication_Fault_Detection.h"
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Drivers/large_packet_driver.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Applications/DriverInstances/command_response_handler.h>

#include <src_user/Applications/UserDefined/Cdh/RTC_handler.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Applications/UserDefined/Com/COM_cmd_COBC.h>
#include <string.h> //memcpy用

#ifdef TWO_U
//__attribute__((section(".sram"))) uint8_t large_packet_data[512];

static COBC_STRUCT cobc_driver_;
COBC_STRUCT* const cobc_driver = &cobc_driver_;

static CommandDispatcher cobc_cdis_;
const CommandDispatcher* const cobc_cdis = &cobc_cdis_;

static void cobc_init_(void);
static void cobc_update_(void);

static void cobc_cmd_dispatcher_init_(void);
static void cobc_cmd_dispatcher_(void);

AppInfo cobc_update(void)
{
  return AI_create_app_info("update_cobc", cobc_init_, cobc_update_);
}

static void cobc_init_(void)
{
  int ret = COBC_UART_init(&cobc_driver_);
  if (ret != _SUCCESS)
  {
    Printf("cobc init Failed ! %d \n", ret);
  }
}

//COBCからのパケット確認，アップデート
static void cobc_update_(void)
{
  for(int i=0; i<COBC_MAX_RECEIVE_PACKET_NUM;i++){
    //Packetが入ってないとき
    if(isEmpty(&(cobc_queue.list))) return;

    //パケットを移してくる
    cobc_driver_.ack = (_ACK)dequeue(&(cobc_queue.list),&(cobc_driver_.tcp));

    //todo : Check Type
    if (cobc_driver_.ack != _SUCCESS){
      // 受信データがあるが何らかの異常が発生した場合は処理打ち切り
      queue_reset(&(cobc_queue.list));//カウンタを全部消す
      return;
    }

    // 受信時刻情報を更新。
    cobc_driver_.last_recv_time = TMGR_get_master_total_cycle();
    ANOMALY_UART_Update_Time(UART_FAULT_COBC);

    // TCPacket Headerの固定値部分が妥当か確認する。
    if(COBC_TCPacket_Fixedheader_Checker() == _FAILED) return;

    cobc_driver_.ack = COBC_check_tcp_headers(&cobc_driver_);

    if ((cobc_driver_.ack != _SUCCESS) & (cobc_driver_.ack != _TLM_UPDATE_SUCCESS )){
      queue_reset(&(cobc_queue.list));//カウンタを全部消す
      return;
    }

    print(COBC,"Ack is %d\r\n",cobc_driver_.ack);
    print(COBC,"receive packet from COBC is\r\n");
    for(int i = 0; i < get_packet_len(cobc_driver_.tcp.packet); i++){
      print(COBC,"%02x ", cobc_driver_.tcp.packet[i]);
    }
    print(COBC,"\r\n");

    if(cobc_check_life_command() == _SUCCESS){
        return;
    }

    //COBC_TLM_UPDATE_SUCCESSの時はcheck_tcpで構造体にパース終わってるので，パケットハンドラーに渡さない
    if(cobc_driver_.ack == _SUCCESS) cobc_driver_.Info.c2a.ph_ack = PH_analyze_packet(&(cobc_driver_.tcp));
  }
  //ここまで来たら3パケットを処理した後
}

AppInfo cobc_cmd_dispatcher(void)
{
  return AI_create_app_info("cobc_cdis",
                         cobc_cmd_dispatcher_init_,
                         cobc_cmd_dispatcher_);
}

static void cobc_cmd_dispatcher_init_(void)
{
  cobc_cdis_ = CDIS_init(&PH_cobc_cmd_list);
}

static void cobc_cmd_dispatcher_(void)
{
  for(int i = 0; i < 3; i++){
    CDIS_dispatch_command(&cobc_cdis_);
  }
}

/**
 * @brief   MOBCからCOBCにコマンドを送るコマンド
 *          packet handlerを経由
 * packet[17~168]:   Ch ID
 * packet[18~]:     User Data
 *
 */CCP_CmdRet cobc_dispatch_command(const CommonCmdPacket* packet)
{
  print(COBC,"cobc_send\r\n");
  DS_CMD_ERR_CODE ret;
  TCP* pckt = (TCP*)packet; // const_cast
  // ここでTCP_CMD_DEST_TYPEを宛先で受理できるように変更（なので const cast が発生している．．．）

  switch(CCP_get_exec_type(pckt)){
    case CCP_EXEC_TYPE_TL_FROM_GS:
//      case CCP_EXEC_TYPE_TL_DEPLOY_BC:
//      case CCP_EXEC_TYPE_TL_DEPLOY_TLM:
      if(p_gs_response_struct->tlc_goal_response & TO_COBC){
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
  ret = (DS_CMD_ERR_CODE)COBC_Send(cobc_driver,pckt->packet,len);
  //todo: check type
  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}


CCP_CmdRet Cmd_cobc_CDIS_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet)
{
  (void)packet;

  CDIS_clear_command_list(&cobc_cdis_);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_cobc_CDIS_CLEAR_ERR_LOG(const CommonCmdPacket* packet)
{
  (void)packet;

  // 記録されたエラー情報を解除。
  CDIS_clear_error_status(&cobc_cdis_);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

_ACK COBC_TCPacket_Fixedheader_Checker(void)
{
  if(TCP_get_ver(&cobc_driver_.tcp) == TCP_VER_2){             //HeaderのVersion_numberをcheck
    if(TCP_get_2nd_hdr_flag(&cobc_driver_.tcp) == TCP_2ND_HDR_PRESENT)  return _SUCCESS;       //HeaderのSecondary_Header_FlagをCheck
    else return _FAILED;
  }
  else return _FAILED;
}

/**
 * @brief   MOBCからCOBCにコマンドを送るコマンド
 *          packet handlerを経由せずに直接COBCに送信するDBG用コマンド
 * packet[17~18]:   Ch ID
 * packet[18~]:     User Data
 *
 */
CCP_CmdRet Cmd_cobc_Transmit_command(const CommonCmdPacket* packet){
  cobc_driver->uart_handler = &huart2;
  cobc_driver->TIMEOUT = 0xff;
  TCP* pckt = (TCP*)packet; // const_cast
  // ここでTCP_CMD_DEST_TYPEを宛先で受理できるように変更（なので const cast が発生している．．．）

  const int len =TCP_get_packet_len(pckt) + TCP_PRM_HDR_LEN;

  for(int i=0;i<len;i++) printf("%02x ",pckt->packet[i]);
  printf("\r\n");

  //実行タイプはリアルタイムコマンドで定義
  CCP_set_exec_type(pckt, CCP_EXEC_TYPE_RT);

  //COBCでコマンドの実行を行うので
  TCP_set_apid_executer_id(pckt, TCP_EXE_ID_COBC);

  //chID分長さが変わるから
  TCP_set_packet_data_len(pckt, TCP_get_packet_len(pckt) -2);

  //ここまででcmdとparam以外はcobc宛てに置き換わった
  uint8_t cobc_packet[len - 3];

  //一旦, cobc宛てのpacketにコピー
  for(int i = 0;i < len - 3; i++){
    cobc_packet[i] =  pckt->packet[i];
  }

  cobc_packet[8] = pckt->packet[17];
  cobc_packet[9] = pckt->packet[18];

  if(len >19){
    for(int i = 0;i < (len - 19); i++){
      cobc_packet[i + 17] =  pckt->packet[i + 19];
    }
  }

  COBC_Send(cobc_driver,cobc_packet,len-2);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

_ACK cobc_AOCS_send_data_generator(TLM_CODE id){
  static TCP tcp;
  uint16_t len;
  TF_TLM_FUNC_ACK ret;
  ret = TF_generate_contents((int)id,
                 tcp.packet,
                 &len,
                 TCP_MAX_LEN);
  // 範囲外のTLM IDを除外
  if (ret == TF_TLM_FUNC_ACK_NOT_DEFINED) return _FAILED;
  if (len < 0) return _FAILED;     // TODO: lenがマイナスの値たちをどうするか？

  //set header
  TCP_set_ver(&tcp, TCP_VER_2);
  TCP_set_type(&tcp, TCP_TYPE_CMD);
  TCP_set_2nd_hdr_flag(&tcp, TCP_2ND_HDR_PRESENT);
  TCP_set_apid_project_id(&tcp,MAGNAROII);
  TCP_set_apid_sat_id(&tcp, TCP_SAT_ID_2U);
  TCP_set_apid_executer_id(&tcp, TCP_EXE_ID_COBC);
  TCP_set_seq_flag(&tcp, TCP_SEQ_SINGLE);
  TCP_set_seq_cnt(&tcp, 0x0000);
  TCP_set_packet_len(&tcp, len);
  TCP_set_route(&tcp, TCP_ROUTE_DIRECT);
  TCP_set_all_issuer_id(&tcp, TCP_SAT_ID_2U, TCP_EXE_ID_MOBC);
  if(id == Tlm_CODE_ADS){
    TCP_CMD_set_channel_id(&tcp, 0x04C2);
  }
  else if(id == Tlm_CODE_ACS){
    TCP_CMD_set_channel_id(&tcp, 0x04C1);
  }
  else if(id == Tlm_CODE_ODS){
    TCP_CMD_set_channel_id(&tcp, 0x04C4);
  }
  else if(id == Tlm_CODE_OCS){
    TCP_CMD_set_channel_id(&tcp, 0x04C3);
  }
  else if(id == Tlm_CODE_TI){
    TCP_CMD_set_channel_id(&tcp, 0x04C5);
  }
//  else if(id == Tlm_CODE_TAMURA){
//      TCP_CMD_set_channel_id(&tcp, 0x0239);
//  }
  else{
      return _FAILED;
  }
  TCP_CMD_set_res_packet_id(&tcp, 0xff);
  TCP_CMD_set_res_apid_(&tcp, TCP_SAT_ID_MAX, TCP_EXE_ID_MAX);
  TCP_CMD_set_res_route(&tcp, TCP_ROUTE_MAX);
  TCP_CMD_set_exec_type(&tcp, TCP_CMD_EXEC_TYPE_RT);
  TCP_CMD_set_ti(&tcp, 0x00000000);
  int packet_len =TCP_get_packet_len(&tcp) + TCP_PRM_HDR_LEN;
  COBC_Send(cobc_driver, &tcp.packet[0], packet_len);
  return _SUCCESS;
}

void COBC_large_packet_init_setting(){
  cobc_driver->large_packet.pacekt_counter = 0;
  cobc_driver->large_packet.count = 0;
  cobc_driver->large_packet.sram_bank_addr = (uint8_t*)0x60000000;
}

//生存信号かの確認
_ACK cobc_check_life_command(void){
  if(CCP_get_id(&(cobc_driver_.tcp)) == Cmd_CODE_AM_EXECUTE_APP){
    const uint8_t* param = CCP_get_param_head(&(cobc_driver_.tcp));
    uint32_t app_id;
    app_id = param[0]<<24 | param[1]<<16 | param[2]<<8 | param[3];
    if(app_id == AR_NOP){
        return _SUCCESS;
    }
  }
  return _FAILED;
}

/* COBCに毎周期送るテレメのUser_Data部分を作成する関数
 * @ TI MOBC時刻,RTC
 * @ ADS 角速度、クォータ二オン
 * @ ODS 衛星位置(ECI)、GPS_高度、高度
 * @ total 56byte
 */
_ACK COBC_Regular_Tlm_ganerator(void){
  static TCP tcp;
  const uint8_t *param;
  param = TCP_CMD_get_param_head(&tcp);

  //setting header
  TCP_set_ver(&tcp, TCP_VER_2);
  TCP_set_type(&tcp, TCP_TYPE_CMD);
  TCP_set_2nd_hdr_flag(&tcp, TCP_2ND_HDR_PRESENT);
  TCP_set_apid_project_id(&tcp,MAGNAROII);
  TCP_set_apid_sat_id(&tcp, TCP_SAT_ID_2U);
  TCP_set_apid_executer_id(&tcp, TCP_EXE_ID_COBC);
  TCP_set_seq_flag(&tcp, TCP_SEQ_SINGLE);
  TCP_set_seq_cnt(&tcp, 0x0000);
  TCP_set_route(&tcp, TCP_ROUTE_DIRECT);
  TCP_set_all_issuer_id(&tcp, TCP_SAT_ID_2U, TCP_EXE_ID_MOBC);
  TCP_CMD_set_channel_id(&tcp, 0x04C6);
  TCP_CMD_set_res_packet_id(&tcp, 0xff);
  TCP_CMD_set_res_apid_(&tcp, TCP_SAT_ID_MAX, TCP_EXE_ID_MAX);
  TCP_CMD_set_res_route(&tcp, TCP_ROUTE_MAX);
  TCP_CMD_set_exec_type(&tcp, TCP_CMD_EXEC_TYPE_RT);
  TCP_CMD_set_ti(&tcp, 0x00000000);

  uint8_t every_TI_contents[8]={0};     // 毎周期送るTlmのTI
  uint8_t every_ADS_contents[32]={0};       // 毎周期送るTlmのADS
  uint8_t every_ODS_contents[16]={0};       // 毎周期送るTlmのODS
  uint8_t every_contents[56]={0};         // All

  // Regular TI
  uint32_t temp0 = TMGR_get_master_total_cycle();
  Get_RTC_Time_Date(p_rtc_struct);
//  uint32_t temp1 = p_rtc_struct->tlm_time;
  memcpy(&every_TI_contents[0],&temp0,4);
//  memcpy(&every_TI_contents[4],&temp1,4);
  // Regular ADS
  memcpy(&every_ADS_contents[0],&g_ads.p_ADS->Omega[0],4);
  memcpy(&every_ADS_contents[4],&g_ads.p_ADS->Omega[1],4);
  memcpy(&every_ADS_contents[8],&g_ads.p_ADS->Omega[2],4);
  memcpy(&every_ADS_contents[12],&g_ads.p_ADS->Quaternion[0],4);
  memcpy(&every_ADS_contents[16],&g_ads.p_ADS->Quaternion[1],4);
  memcpy(&every_ADS_contents[20],&g_ads.p_ADS->Quaternion[2],4);
  memcpy(&every_ADS_contents[24],&g_ads.p_ADS->Quaternion[3],4);
  memcpy(&every_ADS_contents[28], &g_ads.p_GST->gst, 4);

  // Regular ODS
  memcpy(&every_ODS_contents[0],&g_ods.p_ODS->Position[0],4);
  memcpy(&every_ODS_contents[4],&g_ods.p_ODS->Position[1],4);
  memcpy(&every_ODS_contents[8],&g_ods.p_ODS->Position[2],4);
  memcpy(&every_ODS_contents[12],&g_ods.p_ODS->Altitude,4);

  // All
  memcpy(&every_contents[0], &every_TI_contents[0], sizeof(every_TI_contents));
  memcpy(&every_contents[8], &every_ADS_contents[0], sizeof(every_ADS_contents));
  memcpy(&every_contents[40], &every_ODS_contents[0], sizeof(every_ODS_contents));

  memcpy(param,every_contents, sizeof(every_contents));

  TCP_set_packet_data_len(&tcp, sizeof(every_contents) + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN);
  int packet_len = TCP_get_packet_len(&tcp) + TCP_PRM_HDR_LEN;

//  print(COBC,"\r\n");
//  print(COBC,"Regular_packet_len = %d \r\n", packet_len);

  COBC_Send(cobc_driver, &tcp.packet[0], packet_len);
  return _SUCCESS;
}

/* COBCに3周期に1回送るAOCS以外のテレメ
 * @RXPIC_Time,TXPIC_Time,カメラのtemp
 */
_ACK COBC_Additional_Tlm_generator(const uint8_t* param){
  uint8_t contents[28]={0};
  uint32_t temp0 = rxpic_driver->tlm_data.rxpic_to_mobc.RXPIC_Time;
  uint32_t temp1 = txpic_driver->tlm_data.txpic_to_mobc.TXPIC_Time;

  memcpy(&contents[0], &temp0, 4);
  memcpy(&contents[4], &temp1, 4);
  memcpy(&contents[8], &g_sensor.p_adc->LENSMX_TEMP, 4);
  memcpy(&contents[12], &g_sensor.p_adc->LENSPX_TEMP, 4);
  memcpy(&contents[16], &g_sensor.p_adc->LENSPY_TEMP, 4);
  memcpy(&contents[20], &g_sensor.p_adc->IMAGE_TEMP, 4);
  memcpy(&contents[24], &g_sensor.p_gps->Altitude, 4);

  memcpy(param, contents, sizeof(contents));

  return _SUCCESS;
}

/* 3cycle周期のAOCS_Tlm */
_ACK COBC_Periodic_Tlm_generator(uint8_t T_counter){
  static TCP tcp;
  //setting header
  TCP_set_ver(&tcp, TCP_VER_2);
  TCP_set_type(&tcp, TCP_TYPE_CMD);
  TCP_set_2nd_hdr_flag(&tcp, TCP_2ND_HDR_PRESENT);
  TCP_set_apid_project_id(&tcp,MAGNAROII);
  TCP_set_apid_sat_id(&tcp, TCP_SAT_ID_2U);
  TCP_set_apid_executer_id(&tcp, TCP_EXE_ID_COBC);
  TCP_set_seq_flag(&tcp, TCP_SEQ_SINGLE);
  TCP_set_seq_cnt(&tcp, 0x0000);
  TCP_set_route(&tcp, TCP_ROUTE_DIRECT);
  TCP_set_all_issuer_id(&tcp, TCP_SAT_ID_2U, TCP_EXE_ID_MOBC);
  TCP_CMD_set_channel_id(&tcp, 0x04C6);
  TCP_CMD_set_res_packet_id(&tcp, 0xff);
  TCP_CMD_set_res_apid_(&tcp, TCP_SAT_ID_MAX, TCP_EXE_ID_MAX);
  TCP_CMD_set_res_route(&tcp, TCP_ROUTE_MAX);
  TCP_CMD_set_exec_type(&tcp, TCP_CMD_EXEC_TYPE_RT);
  TCP_CMD_set_ti(&tcp, 0x00000000);

  static int len_ADS=0;
  static int len_ODS=0;
  static int len_ACS=0;
  static int len_OCS=0;
  static uint8_t ADS_contents[107] = {0};
  static uint8_t ODS_contents[91] = {0};
  static uint8_t ACS_contents[101] = {0};
  static uint8_t OCS_contents[141] = {0};

  //Dataの取得時間を統一するために,1回で全てのデータを保存する
  //まだここではパケットに入れない
  if(T_counter == 0){
//  len_ADS = TF_generate_contents((int)Tlm_CODE_ADS, ADS_contents,
//                  TCP_MAX_LEN - TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN);
//  len_ODS = TF_generate_contents((int)Tlm_CODE_ODS, ODS_contents,
//                  TCP_MAX_LEN - TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN);
//  len_ACS = TF_generate_contents((int)Tlm_CODE_ACS, ACS_contents,
//                  TCP_MAX_LEN - TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN);
//  len_OCS = TF_generate_contents((int)Tlm_CODE_OCS, OCS_contents,
//                  TCP_MAX_LEN - TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN);
  }

  //sending packet
  //First ADS+ODS
  if(T_counter == 0 ){
    print(COBC,"\r\n");
    print(COBC,"T_counter = %d \r\n",T_counter);

    // 範囲外のTLM IDを除外
    if (len_ADS == TF_TLM_FUNC_ACK_NOT_DEFINED) return _FAILED;
    else if (len_ADS < 0) return _FAILED;
    if (len_ODS == TF_TLM_FUNC_ACK_NOT_DEFINED) return _FAILED;
    else if (len_ODS < 0) return _FAILED;

    memcpy(TCP_CMD_get_param_head(&tcp), ADS_contents, len_ADS);
    memcpy(TCP_CMD_get_param_head(&tcp) + len_ADS, ODS_contents, len_ODS);     // ADSのテレメの最後にODSのテレメをくっつける

    TCP_CMD_set_channel_id(&tcp, 0x04C7);
    TCP_set_packet_data_len(&tcp, len_ADS + len_ODS + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN);
  }
  //Second  ACS + additional_tlm
  else if(T_counter == 1){
    print(COBC,"\r\n");
    print(COBC,"T_counter = %d \r\n",T_counter);

    static uint8_t contents[28]={0};    //additional_Tlmを入れる箱を用意 <--べた書き直したい

    COBC_Additional_Tlm_generator(contents);

    // 範囲外のTLM IDを除外
    if (len_ACS == TF_TLM_FUNC_ACK_NOT_DEFINED) return _FAILED;
    else if (len_ACS < 0) return _FAILED;

    uint8_t contents_len = sizeof(contents);

    memcpy(TCP_CMD_get_param_head(&tcp), ACS_contents, len_ACS);
    memcpy(TCP_CMD_get_param_head(&tcp) + len_ACS, contents, contents_len);     // ACSのテレメの最後にadditional_Tlmをくっつける

    TCP_CMD_set_channel_id(&tcp, 0x04C8);
    TCP_set_packet_data_len(&tcp, len_ACS + contents_len + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN);
  }
  //Third  OCS
  else if(T_counter == 2){
    print(COBC,"\r\n");
    print(COBC,"T_counter = %d \r\n",T_counter);

    // 範囲外のTLM IDを除外
    if (len_OCS == TF_TLM_FUNC_ACK_NOT_DEFINED) return _FAILED;
    else if (len_OCS < 0) return _FAILED;

    memcpy(TCP_CMD_get_param_head(&tcp), OCS_contents, len_OCS);

    TCP_CMD_set_channel_id(&tcp, 0x04C3);
    TCP_set_packet_data_len(&tcp, len_OCS + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN);
  }
  int packet_len =TCP_get_packet_len(&tcp) + TCP_PRM_HDR_LEN;

//  print(COBC,"\r\n");
//  print(COBC,"Periodic_packet_len = %d \r\n", packet_len);

  COBC_Send(cobc_driver, &tcp.packet[0], packet_len);
  return _SUCCESS;
}

#endif
