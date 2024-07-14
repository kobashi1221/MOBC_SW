/*
 * COBC.c
 *
 *  Created on: 2020/08/20
 *      Author: Takumi NORO
 *
 *  Updated on: 2021/12/22
 *      Author: Yuta Sakaguchi
 */

#include "COBC.h"
#include "../../../Applications/DriverInstances/di_struct.h"

#include <string.h> //memcpy用
#include <stdio.h> //printf用
#include <stdint.h> //for uint8_t
#include <src_user/Library/General/DBG_printf.h>

#ifdef TWO_U

int uart_cycle_counter = 0;//カメラの大容量通信時に何パケット送られたかをカウント

Queue cobc_queue;
static Buffer receive_buf[COBC_MAX_RECEIVE_PACKET_NUM];
void COBC_TLM_UPDATE(COBC_STRUCT* cobc_driver);
void COBC_CAM_INFO_UPDATE(COBC_STRUCT* cobc_driver);
void COBC_CAM_DATA_UPDATE(COBC_STRUCT* cobc_driver);
void COBC_GPIO_ON_OFF(COBC_STRUCT* cobc_driver, int flag);

/*
  * @name   :COBC_PORT_INIT
  * @brief  :COBC用通信ハンドラの初期化設定
  * @return :void
  */
void COBC_PORT_INIT(COBC_STRUCT* mycobc){
  HAL_UART_Init_C2A(mycobc->uart_handler);
  queue_init(&cobc_queue, COBC_MAX_RECEIVE_PACKET_NUM, receive_buf);
  int ret = HAL_UART_Receive_DMA_C2A(mycobc->uart_handler,(uint8_t*)&(cobc_queue.dri.byte_buf),1);
  if(ret!=HAL_OK) printf("COBC Packet Receive function fail!\r\n");
}

/*
  * @name   :COBC_UART_init
  * @brief  :COBC用ハンドラー等の設定
  * @param  mycobduart:COBC用の固有の値を割り当てる
  * @return :_ACK
  */
_ACK COBC_UART_init(COBC_STRUCT* cobc_driver){
  cobc_driver->uart_handler = &huart2;
  cobc_driver->TIMEOUT = 0xff;
  cobc_driver->COBC_GPIO_Pin = GPIO3V3_MOBCtoCOBC_1_Pin;
  cobc_driver->COBC_GPIO_PORT = GPIO3V3_MOBCtoCOBC_1_GPIO_Port;
  cobc_driver->COBC_reset_Pin = RESET3V3_MOBCtoCOBC_Pin;
  cobc_driver->COBC_reset_GPIO = RESET3V3_MOBCtoCOBC_GPIO_Port;
  COBC_PORT_INIT(cobc_driver);
  return _SUCCESS;
}

void COBC_GPIO_ON_OFF(COBC_STRUCT* cobc_driver, int flag){
  if(flag == 1){
    HAL_GPIO_WritePin_C2A(cobc_driver->COBC_GPIO_PORT, cobc_driver->COBC_GPIO_Pin, GPIO_PIN_SET);
  }
  else{
    HAL_GPIO_WritePin_C2A(cobc_driver->COBC_GPIO_PORT, cobc_driver->COBC_GPIO_Pin, GPIO_PIN_RESET);
  }
}


void COBC_reset(COBC_STRUCT* cobc_driver){
  HAL_GPIO_WritePin_C2A(cobc_driver->COBC_reset_GPIO, cobc_driver->COBC_reset_Pin, GPIO_PIN_RESET);
  c2a_delay_ms(5);
  HAL_GPIO_WritePin_C2A(cobc_driver->COBC_reset_GPIO, cobc_driver->COBC_reset_Pin, GPIO_PIN_SET);
}

/*
  * @name   :COBC_Send
  * @brief  :COBCへの送信関数
  * @param
  * mycobcuart:COBC用構造体
  * data:送信データ
  * len:データ数
  * @return :_ACK
  */
_ACK COBC_Send(COBC_STRUCT* mycobc,uint8_t* data,size_t len){
  int ret;
  ObcTime at_now=TMGR_get_master_clock();

  while(HAL_UART_Get_State_C2A(mycobc->uart_handler) != 32){
    ObcTime now=TMGR_get_master_clock();
    if(OBCT_diff_in_msec(&at_now ,&now)>((unsigned int)(mycobc->TIMEOUT))){
      break;
    }
  }
  COBC_GPIO_ON_OFF(mycobc, 1);
  ret = (_ACK)HAL_UART_Transmit_C2A(mycobc->uart_handler,data,len, 0xff);
  COBC_GPIO_ON_OFF(mycobc, 0);
  while(HAL_UART_Get_State_C2A(mycobc->uart_handler) != 32){
    ObcTime now=TMGR_get_master_clock();
    if(OBCT_diff_in_msec(&at_now ,&now)>((unsigned int)(mycobc->TIMEOUT))){
      break;
    }
  }

  print(COBC,"send packet to COBC is\r\n");
  for(int i = 0; i <get_packet_len(data); i++)
  {
    print(COBC,"%02x ", data[i]);
  }
  print(COBC, "\r\n");
  return ret;
}

/*
  * @name   :COBC_check_tcp_headers
  * @brief  :COBCから受け取ったデータをパース関数に振り分ける
  * @return :_ACK
  */
_ACK COBC_check_tcp_headers(COBC_STRUCT* cobc_driver){
  int type=TCP_get_type(&(cobc_driver->tcp));
  if(type==TCP_TYPE_TLM){//TLM, should be fixed using enum
    if(TCP_TLM_get_packet_id(&(cobc_driver->tcp))==COBC_Tlm_CODE_COBC_TO_MOBC){
      COBC_TLM_UPDATE(cobc_driver); //Tlm Update from
      return _TLM_UPDATE_SUCCESS;
    }
    if(TCP_TLM_get_packet_id(&(cobc_driver->tcp))==COBC_CAMINFO_BY_UART){
      COBC_CAM_INFO_UPDATE(cobc_driver); //CAMDATA Update from
      return _TLM_UPDATE_SUCCESS;
    }
    if(TCP_TLM_get_packet_id(&(cobc_driver->tcp))==COBC_CAMDATA_BY_UART){
      COBC_CAM_DATA_UPDATE(cobc_driver); //CAMDATA Update from
      return _TLM_UPDATE_SUCCESS;
    }
    else{//他のテレメトリの場合,もう片方の衛星のテレメトリがCOBCを介して来るとか？
      //ほんとはここでエラーチェック入れる必要有
      return _SUCCESS;
    }
  }
  else if(type==1){//CMD　COBCから来るコマンド　（GSからのも含む）
    //ほんとはここでエラーチェック入れる必要有
    return _SUCCESS;
  }
  else return _SUCCESS;
}

/*
  * @name   :COBC_TLM_UPDATE
  * @brief  :COBCから受け取ったTLMをパースする関数
  * @return :void
  */
void COBC_TLM_UPDATE(COBC_STRUCT* cobc_driver){
  uint8_t* head_tlm;
  head_tlm=CTP_get_user_data_head(&(cobc_driver->tcp));
  COBC_TlmData* tlm_data = &(cobc_driver->tlm_data);

  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.temp.cobc), head_tlm, 2);

  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cur_vols.curs_cobc), head_tlm+2, 2);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cur_vols.curs_mcam), head_tlm+4, 2);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cur_vols.curs_scam12), head_tlm+6, 2);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cur_vols.curs_scam28), head_tlm+8, 2);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cur_vols.vols_mcam), head_tlm+10, 2);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cur_vols.vols_heat), head_tlm+12, 2);

  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.COBC_Time), head_tlm+14, 4);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cobc_reset_conut), head_tlm+18, 1);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.anomaly_count), head_tlm+19, 1);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cobc_event_time), head_tlm+20, 4);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cobc_event_group), head_tlm+24, 1);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cobc_event_local), head_tlm+25, 2);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cobc_event_err_level), head_tlm+27, 1);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cobc_mode), head_tlm+28, 1);

  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cnt_mcam), head_tlm+29, 1);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cnt_scam), head_tlm+30, 1);
  ENDIAN_memcpy(&(tlm_data->cobc_to_mobc.cycle_cnt), head_tlm+31, 2);

  for(int i = 0; i < 3; i ++){
    ENDIAN_memcpy(&(tlm_data->cobc_aocs_tlm.target_vector[i]), head_tlm+33+(i * 4), 4);
  }
  ENDIAN_memcpy(&(tlm_data->cobc_aocs_tlm.AttitudeControl_flag), head_tlm+45, 1);
}

/*
  * @name   :COBC_CAM_INFO_UPDATE
  * @brief  :COBCから受け取ったCAM INFO TLMをパースする関数
  * @return :void
  */
void COBC_CAM_INFO_UPDATE(COBC_STRUCT* cobc_driver){
  printf("large_packet_1\r\n");
  for(int i=0;i<256;i++) print(LARGE_PACKET,"%02x ", cobc_driver->tcp.packet[i]);
  print(LARGE_PACKET,"\r\n");
  uint8_t* head_tlm;
  head_tlm=CTP_get_user_data_head(&(cobc_driver->tcp));
  ENDIAN_memcpy(&(cobc_driver->large_packet.ImSize), head_tlm, 4);
  ENDIAN_memcpy(&(cobc_driver->large_packet.start_block_addr), head_tlm+4, 4);
  ENDIAN_memcpy(&(cobc_driver->large_packet.addr), head_tlm+8, 2);
  ENDIAN_memcpy(&(cobc_driver->large_packet.picture_id), head_tlm+12, 2);
  printf("ImSize is %d \r\n", (int)cobc_driver->large_packet.ImSize);
  printf("start_block_addr is %08x \r\n", cobc_driver->large_packet.start_block_addr);
  printf("addr is %08x \r\n", cobc_driver->large_packet.addr);
  printf("picture_id is %02x \r\n", (uint16_t)cobc_driver->large_packet.picture_id);

  uart_cycle_counter = 0;
}

/*
  * @name   :COBC_CAM_DATA_UPDATE
  * @brief  :COBCから受け取ったCAM DATA TLMをパースする関数
  * @return :void
  */
void COBC_CAM_DATA_UPDATE(COBC_STRUCT* cobc_driver){
  uint8_t* head_tlm;
  head_tlm=CTP_get_user_data_head(&(cobc_driver->tcp));
  memcpy(cobc_driver->large_packet.sram_bank_addr + (COBC_UART_TLM_BUFFER * uart_cycle_counter),
          &head_tlm[0], COBC_UART_TLM_BUFFER);

  printf("large_packet\r\n");
  printf("cycle_counter is %d \r\n",uart_cycle_counter);
  for(int i=0;i<255;i++) print(LARGE_PACKET,"%02x ",
    cobc_driver->large_packet.sram_bank_addr[i + COBC_UART_TLM_BUFFER * uart_cycle_counter]);
  print(LARGE_PACKET,"\r\n");

  ++uart_cycle_counter;

  switch(cobc_driver->large_packet.ImSize % COBC_UART_TLM_BUFFER){
    case 0:
      if(uart_cycle_counter == cobc_driver->large_packet.ImSize / COBC_UART_TLM_BUFFER){
        cobc_driver->large_packet.sram_bank_addr = (uint8_t*)0x60000000;
        ++cobc_driver->large_packet.sram_input_num;
        cobc_driver->cobc_tlm.cobc_tlm_info = cobc_driver->cobc_tlm.cobc_tlm_info_temp;
        TCP tcp;
        if(cobc_driver->large_packet.auto_reset == 1){
          uint8_t dummy[1];
          dummy[0] = 0x00;
          const uint8_t* data = &dummy[0];
          CCP_form_rtc(&tcp,Cmd_CODE_PWR_MIS1_ON_OFF_ONRESET, &data[0], 1);
        }
        else{
          CCP_form_rtc_to_other_obc(&tcp, TCP_APID_COBC_2U, (CMD_CODE)Cmd_DEFAULT_SETTING,NULL, 0);
        }
        PH_analyze_packet(&tcp);
        printf("finish send\r\n");
      }
      else{
        //続行
      }
      break;
    default:
      if(uart_cycle_counter == (cobc_driver->large_packet.ImSize / COBC_UART_TLM_BUFFER) + 1){
        cobc_driver->large_packet.sram_bank_addr = (uint8_t*)0x60000000;
        ++cobc_driver->large_packet.sram_input_num;
        cobc_driver->cobc_tlm.cobc_tlm_info = cobc_driver->cobc_tlm.cobc_tlm_info_temp;
        TCP tcp;
        if(cobc_driver->large_packet.auto_reset == 1){
          uint8_t dummy[1];
          dummy[0] = 0x00;
          const uint8_t* data = &dummy[0];
          CCP_form_rtc(&tcp,Cmd_CODE_PWR_MIS1_ON_OFF_ONRESET, &data[0], 1);
        }
        else{
          CCP_form_rtc_to_other_obc(&tcp, TCP_APID_COBC_2U, (CMD_CODE)Cmd_DEFAULT_SETTING,NULL, 0);
        }
        PH_analyze_packet(&tcp);
        printf("finish send\r\n");
      }
      else{
        //続行
      }
    break;
  }
}

#endif

/************************************** END OF FILE **************************************/
