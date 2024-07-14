/*
 * COM_cmd_COBC.c
 *
 *  Created on: 2021/10/18
 *      Author: daiki
 */

#include "COM_cmd_COBC.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <string.h>//for memcpy
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.h>
#include <src_user/Drivers/large_packet_driver.h>
#include <src_user/Library/General/DBG_printf.h>


#ifdef TWO_U

static void COBC_BOOT_load_param(void);
static void COBC_BOOT_initialize_param(void);

static void COBC_communication_load_param(void);
static void COBC_communication_initialize_param(void);

static void cobc_large_packet_init_(void);
static void cobc_large_packet_receive_(void);

static uint8_t program_param;
const uint8_t program_param_ = 0x25;

static uint8_t repro_param;
const uint8_t repro_param_ = 0x00;

static uint8_t cobc_tlm_interval;
const uint8_t cobc_tlm_interval_ = 0x03;

const uint16_t cobc_large_packet_cmd_ = Cmd_COM_LARGE_PACKET_MODE;
const uint8_t cobc_large_packet_info = MCAM_INFO;
const uint8_t cobc_large_packet_cycle_ = 3;
const uint8_t cobc_large_packet_auto_off_ = 1;
const uint8_t cobc_tlm_info_ = ADS | ODS | TI;
const uint8_t cobc_regular_tlm_flag_ = 3;   //Regular_Tlm ON=3
const uint8_t cobc_periodic_tlm_flag_ = 0;  //Periodic_Tlm ON=3;

COBC_TLM_NUM tlm_counter = 0;
uint8_t count = 0;
uint8_t wait_time = 15;



AppInfo COBC_BOOT_param_update(void)
{
  return AI_create_app_info("cobc_boot_param_init",
          COBC_BOOT_load_param,
              COBC_BOOT_initialize_param);
}

AppInfo COBC_com_param_update(void){
  return AI_create_app_info("cobc_com_param_update",
          COBC_communication_load_param,
              COBC_communication_initialize_param);
}

AppInfo COBC_large_packet_receive(void)
{
  return AI_create_app_info("cobc_cdis",
          cobc_large_packet_init_,
              cobc_large_packet_receive_);
}

AppInfo COBC_Regular_Tlm(void){
  return AI_create_app_info("cobc_regular_tlm_ganerate", NULL, COBC_regular_tlm);
}

static void COBC_BOOT_load_param(void)
{
  read_param(&program_param, &program_param_, 1, (uint32_t)0x00003400);
  read_param(&repro_param, &repro_param_, 1, (uint32_t)0x00003408);
}

static void COBC_BOOT_initialize_param(void)
{
  write_param(&program_param, &program_param_, 1, (uint32_t)0x00003400);
  write_param(&repro_param, &repro_param_, 1, (uint32_t)0x00003408);
}

static void COBC_communication_load_param(void)
{
  read_param(&cobc_tlm_interval, &cobc_tlm_interval_, 1, (uint32_t)0x00003401);
  read_param(&cobc_driver->large_packet.cobc_large_packet_cmd, &cobc_large_packet_cmd_, 2, (uint32_t)0x00003402);
  read_param(&cobc_driver->large_packet.info, &cobc_large_packet_info, 1, (uint32_t)0x00003404);
  read_param(&cobc_driver->large_packet.cycle_count, &cobc_large_packet_cycle_, 1, (uint32_t)0x00003405);
  read_param(&cobc_driver->large_packet.auto_reset, &cobc_large_packet_auto_off_, 1, (uint32_t)0x00003406);
  read_param(&cobc_driver->cobc_tlm.cobc_tlm_info, &cobc_tlm_info_, 1, (uint32_t)0x00003407);
  read_param(&cobc_driver->cobc_tlm.cobc_regular_tlm_flag, &cobc_regular_tlm_flag_, 1, (uint32_t)0x00003409);
  read_param(&cobc_driver->cobc_tlm.cobc_periodic_tlm_flag, &cobc_periodic_tlm_flag_, 1, (uint32_t)0x0000340A);

  cobc_tlm_default_setting();
}

static void COBC_communication_initialize_param(void)
{
  write_param(&cobc_tlm_interval, &cobc_tlm_interval_, 1, (uint32_t)0x00003401);
  write_param(&cobc_driver->large_packet.cobc_large_packet_cmd, &cobc_large_packet_cmd_, 2, (uint32_t)0x00003402);
  write_param(&cobc_driver->large_packet.info, &cobc_large_packet_info, 1, (uint32_t)0x00003404);
  write_param(&cobc_driver->large_packet.cycle_count, &cobc_large_packet_cycle_, 1, (uint32_t)0x00003405);
  write_param(&cobc_driver->large_packet.auto_reset, &cobc_large_packet_auto_off_, 1, (uint32_t)0x00003406);
  write_param(&cobc_driver->cobc_tlm.cobc_tlm_info, &cobc_tlm_info_, 1, (uint32_t)0x00003407);
  write_param(&cobc_driver->cobc_tlm.cobc_regular_tlm_flag, &cobc_regular_tlm_flag_, 1, (uint32_t)0x00003409);
  write_param(&cobc_driver->cobc_tlm.cobc_periodic_tlm_flag, &cobc_periodic_tlm_flag_, 1, (uint32_t)0x0000340A);
}

CCP_CmdRet Cmd_COBC_Reset(const CommonCmdPacket* packet){
  printf("COBC RESET\r\n");
  COBC_reset(cobc_driver);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_COBC_BOOT(const CommonCmdPacket* packet){
  TCP tcp;
  read_param(&program_param, &program_param_, 1, (uint32_t)0x00003400);
  read_param(&repro_param, &repro_param_, 1, (uint32_t)0x00003408);

  uint8_t data[2] = {program_param, repro_param};
  uint8_t length = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN + sizeof(data);

  CCP_form_rtc_to_other_obc(&tcp, TCP_APID_COBC_2U, 0xFFFF, data, sizeof(data));

  COBC_Send(cobc_driver, tcp.packet, length);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//COBCにAOCSのデータを送る
CCP_CmdRet Cmd_COBC_send_AOCS(const CommonCmdPacket* packet){
  if(cobc_driver->cobc_on_off_flag == 0){//COBCが落ちてるときは送らない
    wait_time = 15;
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  }

  if(wait_time > 0){
    --wait_time;
  }
  else{
    uint8_t tlm_info = cobc_driver->cobc_tlm.cobc_tlm_info;
    for(int i = 0;i < 8; i++){
      if(((tlm_counter >> i) & 0b00000001) == 0b00000001){
        int temp_a, temp_b;
        temp_a = temp_b = 2;
        for(int j = 1; j < i; j++){
            temp_b = temp_a*temp_b;
        }
        tlm_info = tlm_info & temp_b;
        break;
      }
    }
    //そもそも送るかどうか
    if(cobc_driver->cobc_tlm.cobc_tlm_info == LIFE){
      TCP tcp;
      uint32_t ap_id = 0x00000000;
      uint8_t* param;
      param = (uint8_t*)&ap_id;
      uint8_t length = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN + 0x0004;
      CCP_form_rtc_to_other_obc(&tcp, TCP_APID_COBC_2U, (CMD_CODE)0x0005,param,0x0004);
      COBC_Send(cobc_driver,(uint8_t *)&tcp.packet,length);
    }
    else{
      switch(tlm_info){
        case ADS:
          cobc_AOCS_send_data_generator(Tlm_CODE_ADS);
          tlm_counter = tlm_counter ^ ADS;
          break;
        case ACS:
          cobc_AOCS_send_data_generator(Tlm_CODE_ACS);
          tlm_counter = tlm_counter ^ ACS;
          break;
        case ODS:
          cobc_AOCS_send_data_generator(Tlm_CODE_ODS);
          tlm_counter = tlm_counter ^ ODS;
          break;
        case OCS:
          cobc_AOCS_send_data_generator(Tlm_CODE_OCS);
          tlm_counter = tlm_counter ^ OCS;
          break;
        case TI:
          cobc_AOCS_send_data_generator(Tlm_CODE_TI);
          tlm_counter = tlm_counter ^ TI;
          break;
        case TAMURA:
  //        cobc_AOCS_send_data_generator(Tlm_CODE_TAMURA);
          tlm_counter = tlm_counter ^ TAMURA;
          break;
        default:
          break;
      }
      if(tlm_counter == NONE_TLM){
          tlm_counter = cobc_driver->cobc_tlm.cobc_tlm_info;
      }
    }
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/* @function 定期テレメとAOCSのテレメをCOBCに送る関数
 * @note  Regular_Tlm + 3s周期_Tlm × 3
 */
void COBC_regular_tlm(void){
  if(cobc_driver->cobc_on_off_flag == 0){//COBCが落ちてるときは送らない
    wait_time = 15;
  }

  if(wait_time > 0){
    --wait_time;
  }
  else{
    static uint8_t T_counter = 0;  //何周期目かを表すparameter

    // ここで毎周期送るTlmを作成,送信
    if(cobc_driver->cobc_tlm.cobc_regular_tlm_flag == 3){
      COBC_Regular_Tlm_ganerator();
    }

    // ここで3ｓ周期のテレメを作成,送信.基本的にdisable
    if(cobc_driver->cobc_tlm.cobc_periodic_tlm_flag == 3){
      COBC_Periodic_Tlm_generator(T_counter);
    }

    T_counter++;
    if(T_counter == 3) T_counter=0;  //3 step cycle
  }
}

/*
* *      Author: Sakaguchi
 * @brief　  どのデータをCOBCから受け取るかを選ぶコマンド(UART通信を想定)
 * packet[15]:  MCAM:0x00, SCAM:0x01
 * packet[16]:  spi :0x00, uart:0x01
*/
CCP_CmdRet Cmd_COBC_Choose_Info(const CommonCmdPacket* packet){
  const uint8_t* param = CCP_get_param_head(packet);

  if(param[0] == 0){
    cobc_driver->large_packet.info = MCAM_INFO;
  }
  else if(param[0] == 1){
    cobc_driver->large_packet.info = SCAM_INFO;
  }
  if(param[1] == 0){
    cobc_driver->large_packet.comm_method = SPI_METHOD;
  }
  else if(param[1] == 1){
    cobc_driver->large_packet.comm_method = UART_METHOD;
  }

  printf("info is %d \r\n",cobc_driver->large_packet.info);
  printf("comm_method is %d \r\n",cobc_driver->large_packet.comm_method);

  TCP tcp;

  uint8_t dummy[4];
  bct_id_t BC_ID, ALT_BC_ID;
  BC_ID = BC_COBC_LARGE_PACKET_SET;
  ALT_BC_ID = BC_ALT_LARGE_PACKET;
  dummy[0] = (uint8_t)(BC_ID << 8);
  dummy[1] = (uint8_t)(BC_ID);
  dummy[2] = (uint8_t)(ALT_BC_ID << 8);
  dummy[3] = (uint8_t)(ALT_BC_ID);

  CCP_form_rtc(&tcp,Cmd_CODE_BCT_CHANGE_ALT_BC, &dummy[0], 4);
  PH_analyze_packet(&tcp);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);

}

//COBCにコマンドを送り, 何の情報を貰うか指定する
CCP_CmdRet Cmd_COBC_SEND_INFORMATION(const CommonCmdPacket* packet){
    cobc_driver->uart_handler = &huart2;
    cobc_driver->TIMEOUT = 0xff;
    TCP* pckt = (TCP*)packet; // const_cast
    // ここでTCP_CMD_DEST_TYPEを宛先で受理できるように変更（なので const cast が発生している．．．）

    const int len =TCP_get_packet_len(pckt) + TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN - 1;

    for(int i=0;i<len - 1;i++) printf("%02x ",pckt->packet[i]);
    printf("\r\n");

    //実行タイプはリアルタイムコマンドで定義
    CCP_set_exec_type(pckt, CCP_EXEC_TYPE_RT);

    //COBCでコマンドの実行を行うので
    TCP_set_apid_executer_id(pckt, TCP_EXE_ID_COBC);

    //chID分長さが変わるから
    TCP_set_packet_data_len(pckt, TCP_get_packet_len(pckt) -2);

    //ここまででcmdとparam以外はcobc宛てに置き換わった
    uint8_t cobc_packet[len - 2];

    //一旦, cobc宛てのpacketにコピー
    for(int i = 0;i < len - 2; i++){
        cobc_packet[i] =  pckt->packet[i];
    }

    //cobc側での実行関数(デフォルトでは0x00df)
    cobc_packet[8] = pckt->packet[15];
    cobc_packet[9] = pckt->packet[16];

    if(cobc_driver->large_packet.info == MCAM_INFO){
        if(cobc_driver->large_packet.comm_method == SPI_METHOD){
            cobc_packet[15] = 0x00;//COBCからpacket情報を送ってもらう
            cobc_packet[16] = SPI_METHOD;//spi
            cobc_packet[17] = MCAM_INFO;//MCAM info
        }
        else if(cobc_driver->large_packet.comm_method == UART_METHOD){
            cobc_packet[15] = 0x00;//COBCからpacket情報を送ってもらう
            cobc_packet[16] = UART_METHOD;//uart
            cobc_packet[17] = MCAM_INFO;//MCAM info
        }
    }
    else if(cobc_driver->large_packet.info == SCAM_INFO){
        if(cobc_driver->large_packet.comm_method == SPI_METHOD){
            cobc_packet[15] = 0x00;//COBCからpacket情報を送ってもらう
            cobc_packet[16] = SPI_METHOD;//spi
            cobc_packet[17] = SCAM_INFO;//SCAM info
        }
        else if(cobc_driver->large_packet.comm_method == UART_METHOD){
            cobc_packet[15] = 0x00;//COBCからpacket情報を送ってもらう
            cobc_packet[16] = UART_METHOD;//uart
            cobc_packet[17] = SCAM_INFO;//SCAM info
        }
    }
#ifdef COBC_DBG
    printf("len is %d\r\n", len);

    for(int i=0;i<len - 1;i++) printf("%02x ",pckt->packet[i]);
    printf("\r\n");

    for(int i=0;i<len - 1;i++) printf("%02x ",cobc_packet[i]);
    printf("\r\n");
#endif
    COBC_Send(cobc_driver,cobc_packet,len-2);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_COBC_large_packet_mode_enable(const CommonCmdPacket* packet){
  const uint8_t* param = CCP_get_param_head(packet);

  if(param[0]==2){//disableだけ
    cobc_driver->large_packet.enable = COBC_DISABLE;
  }
  if(param[0]==1){
    cobc_driver->large_packet.enable = COBC_ENABLE;
    cobc_driver->cobc_tlm.cobc_tlm_info = NONE_TLM;
  }
  else if(param[0]==0){//初期化も
    cobc_driver->large_packet.enable = COBC_DISABLE;
    COBC_large_packet_init_setting();
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//COBCからのデータをMOBCに移す
CCP_CmdRet Cmd_COBC_large_packet_mode(const CommonCmdPacket* packet){
  const uint8_t* param = CCP_get_param_head(packet);

  TCP tcp;
  //COBCに情報をセット
  if(param[0] == 0){
    uint8_t dummy[3];
    dummy[0] = 0x00;
    dummy[1] = 0x00;
    dummy[2] = BC_COBC_LARGE_PACKET_SET;
    const uint8_t* bc_id = &dummy[0];

    CCP_form_rtc(&tcp,Cmd_CODE_TLCD_DEPLOY_BLOCK, &bc_id[0], 3);
    PH_analyze_packet(&tcp);
  }
  //COBCから情報を貰う
  else if(param[0] ==1){
    uint8_t dummy[3];
    dummy[0] = 0x00;
    dummy[1] = 0x00;
    dummy[2] = BC_COBC_LARGE_PACKET_RECEIVE;
    const uint8_t* bc_id = &dummy[0];
    cobc_driver->cobc_tlm.cobc_tlm_info = NONE_TLM;

    CCP_form_rtc(&tcp,Cmd_CODE_TLCD_DEPLOY_BLOCK, &bc_id[0], 3);
    PH_analyze_packet(&tcp);
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//ダウンリンク
CCP_CmdRet Cmd_COBC_large_packet_downlink(const CommonCmdPacket* packet){
  const uint8_t* param = CCP_get_param_head(packet);
  TCP tcp;

  //ダウンリンク開始
  uint8_t dummy[2];
  dummy[0] = param[0];
  dummy[1] = param[1];//ダウンリンク中のMODE移動先
  const uint8_t* data = &dummy[0];
  printf("mode is \r\n");
  for(int i = 0; i < 2; i++){
    printf("mode is %x", dummy[i]);
  }
  printf("\r\n");
  CCP_form_rtc(&tcp,Cmd_CODE_SAME_TL_MODE_TRANS, &data[0], 2);
  PH_analyze_packet(&tcp);
  if(cobc_driver->large_packet.info == MCAM_INFO){
    large_packet_TLM_analyze(cobc_driver->large_packet.sram_bank_addr, cobc_driver->large_packet.ImSize, COBC_MCAM_TLM);
  }
  else if(cobc_driver->large_packet.info == SCAM_INFO){
    large_packet_TLM_analyze(cobc_driver->large_packet.sram_bank_addr, cobc_driver->large_packet.ImSize, COBC_SCAM_TLM);
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//MOBCとCOBCの写真大容量通信パラをALL reset
CCP_CmdRet Cmd_COBC_large_packet_all_reset(const CommonCmdPacket* packet){
  TCP tcp;
  COBC_large_packet_init_setting();
  analyzer_init();
  CCP_form_rtc_to_other_obc(&tcp, TCP_APID_COBC_2U, (CMD_CODE)Cmd_DEFAULT_SETTING,NULL,0);
  PH_analyze_packet(&tcp);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

static void cobc_large_packet_init_(void){
  cobc_driver->large_packet.sram_input_num = 0;
  COBC_large_packet_init_setting();
}

//COBCからのデータを受け取る　1周期に受け取るpacket数は要検討(MRAMで変更可能)
static void cobc_large_packet_receive_(void){

}

void cobc_tlm_default_setting(){
  tlm_counter = cobc_driver->cobc_tlm.cobc_tlm_info;
  cobc_driver->cobc_tlm.cobc_tlm_info_temp = cobc_driver->cobc_tlm.cobc_tlm_info;
}

//Osaki added
//COBC GPIO input
CCP_CmdRet Cmd_COBC_GPIO_in(const CommonCmdPacket* packet){
  int state;

  state = HAL_GPIO_ReadPin_C2A(GPIO3V3_COBCtoMOBC_1_GPIO_Port, GPIO3V3_COBCtoMOBC_1_Pin);
  printf("GPIO_in1 = %d\n", state);
  state = HAL_GPIO_ReadPin_C2A(GPIO3V3_COBCtoMOBC_2_GPIO_Port, GPIO3V3_COBCtoMOBC_2_Pin);
  printf("GPIO_in2 = %d\n", state);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//Osaki added
//COBC GPIO output
CCP_CmdRet Cmd_COBC_GPIO_out(const CommonCmdPacket* packet){
  const uint8_t* param = CCP_get_param_head(packet);
  int state;

  if(param[0] == 1){ //GPIO1
    if(param[1] == 0){ //Low
      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_1_GPIO_Port, GPIO3V3_MOBCtoCOBC_1_Pin);
      printf("GPIO_out1 = %d\n", state);

      HAL_GPIO_WritePin_C2A(GPIO3V3_MOBCtoCOBC_1_GPIO_Port, GPIO3V3_MOBCtoCOBC_1_Pin, GPIO_PIN_RESET);

      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_1_GPIO_Port, GPIO3V3_MOBCtoCOBC_1_Pin);
      printf("GPIO_out1 = %d\n", state);
    }
    else if(param[1] == 1){ //High
      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_1_GPIO_Port, GPIO3V3_MOBCtoCOBC_1_Pin);
      printf("GPIO_out1 = %d\n", state);

      HAL_GPIO_WritePin_C2A(GPIO3V3_MOBCtoCOBC_1_GPIO_Port, GPIO3V3_MOBCtoCOBC_1_Pin, GPIO_PIN_SET);

      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_1_GPIO_Port, GPIO3V3_MOBCtoCOBC_1_Pin);
      printf("GPIO_out1 = %d\n", state);
    }
    else{
    }
  }

  else if(param[0] == 2){ //GPIO2
    if(param[1] == 0){ //Low
      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_2_GPIO_Port, GPIO3V3_MOBCtoCOBC_2_Pin);
      printf("GPIO_out2 = %d\n", state);

      HAL_GPIO_WritePin_C2A(GPIO3V3_MOBCtoCOBC_2_GPIO_Port, GPIO3V3_MOBCtoCOBC_2_Pin, GPIO_PIN_RESET);

      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_2_GPIO_Port, GPIO3V3_MOBCtoCOBC_2_Pin);
      printf("GPIO_out2 = %d\n", state);
    }
    else if(param[1] == 1){ //High
      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_2_GPIO_Port, GPIO3V3_MOBCtoCOBC_2_Pin);
      printf("GPIO_out2 = %d\n", state);

      HAL_GPIO_WritePin_C2A(GPIO3V3_MOBCtoCOBC_2_GPIO_Port, GPIO3V3_MOBCtoCOBC_2_Pin, GPIO_PIN_SET);

      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_2_GPIO_Port, GPIO3V3_MOBCtoCOBC_2_Pin);
      printf("GPIO_out2 = %d\n", state);
    }
    else{
    }
  }
  else if(param[0] == 3){ //GPIO3
    if(param[1] == 0){ //Low
      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_3_GPIO_Port, GPIO3V3_MOBCtoCOBC_3_Pin);
      printf("GPIO_out3 = %d\n", state);

      HAL_GPIO_WritePin_C2A(GPIO3V3_MOBCtoCOBC_3_GPIO_Port, GPIO3V3_MOBCtoCOBC_3_Pin, GPIO_PIN_RESET);

      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_3_GPIO_Port, GPIO3V3_MOBCtoCOBC_3_Pin);
      printf("GPIO_out3 = %d\n", state);
    }
    else if(param[1] == 1){ //High
      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_3_GPIO_Port, GPIO3V3_MOBCtoCOBC_3_Pin);
      printf("GPIO_out3 = %d\n", state);

      HAL_GPIO_WritePin_C2A(GPIO3V3_MOBCtoCOBC_3_GPIO_Port, GPIO3V3_MOBCtoCOBC_3_Pin, GPIO_PIN_SET);

      state = HAL_GPIO_ReadPin_C2A(GPIO3V3_MOBCtoCOBC_3_GPIO_Port, GPIO3V3_MOBCtoCOBC_3_Pin);
      printf("GPIO_out3 = %d\n", state);
    }
    else{
    }
  }
    else{
    }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#endif
