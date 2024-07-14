/*
 * SOBC.c
 *
 *  Created on: 2023/08/24
 *      Author: Naoki KOBAYASHI
 */

#include "SOBC.h"

#include <string.h> //memcpy用
#include <stdio.h> //printf用
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include <src_user/Library/General/DBG_printf.h>

Queue sobc_queue;
static Buffer receive_buf[SOBC_MAX_RECEIVE_PACKET_NUM];

void SOBC_TLM_UPDATE(SOBC_STRUCT* sobc_driver);

//_ACK SOBC_SPI_init(SOBC_STRUCT* sobc_driver){
//  sobc_driver->spi_handler = &hspi4;
//  sobc_driver->TIMEOUT = 0xff;
//  sobc_driver->SOBC_reset_Pin = RESET3V3_MOBCtoSOBC_Pin;
//  sobc_driver->SOBC_reset_GPIO = RESET3V3_MOBCtoSOBC_GPIO_Port;
//  SOBC_PORT_INIT(sobc_driver);
//  return _SUCCESS;
//}

_ACK SOBC_SPI_init(SOBC_STRUCT* sobc_driver){
  sobc_driver->spi_handler = &hspi4;
  sobc_driver->TIMEOUT = 0xfff;
  sobc_driver->SOBC_reset_Pin = RESET3V3_MOBCtoSOBC_Pin;
  sobc_driver->SOBC_reset_GPIO = RESET3V3_MOBCtoSOBC_GPIO_Port;
  sobc_driver->SOBC_debug_Pin = GPIO_3V3_MOBCtoSOBC_Pin;
  sobc_driver->SOBC_debug_GPIO = GPIO_3V3_MOBCtoSOBC_GPIO_Port;

  SOBC_PORT_INIT(sobc_driver);
  return _SUCCESS;
}
/*
  * @name   :SOBC_SendReceive
  * @brief  :SOBCとの送受信関数
  * @param
  * mysobc:SOBC用構造体
  * data:送信データ
  * len:データ数
  * @return :_ACK
  */

_ACK SOBC_SendReceive(const SOBC_STRUCT* mysobc,uint8_t* tx_data, size_t len){
//  uint8_t state = 0;
//  state = PWR_State_GET(PWR_State_SEMP);
//  if(state == 0x01){
//    step_t start_time = TMGR_get_master_step();
    HAL_GPIO_WritePin_C2A(CS_SPI3V3_MOBCtoSOBC_GPIO_Port,CS_SPI3V3_MOBCtoSOBC_Pin,GPIO_PIN_RESET);
    HAL_StatusTypeDef ret = HAL_SPI_TransmitReceive_C2A(mysobc->spi_handler, tx_data, sobc_queue.dri.receive_buf, len, mysobc->TIMEOUT);
    HAL_GPIO_WritePin_C2A(CS_SPI3V3_MOBCtoSOBC_GPIO_Port,CS_SPI3V3_MOBCtoSOBC_Pin,GPIO_PIN_SET);
//    step_t finish_time = TMGR_get_master_step();
//    printf("spi com time = %d ms\r\n",(int)finish_time - (int)start_time);
//    printf("receive =");
//    for(int i=0;i<len;i++){
//      printf("%02x ",sobc_queue.dri.receive_buf[i]);
//    }
//    printf("\r\n");
  //  printf("transmit = ");
  //  for(int i = 0; i<len; i++){
  //    printf("%02x ", tx_data[i]);
  //  }
  //  printf("\r\n");

    Spi_Callback_Packet(&sobc_queue, len);

    return ret;
//  }
  return _SUCCESS;
}

//uint8_t* get_receive_user_data(SOBC_STRUCT* sobc_driver){
//  return &(sobc_queue.dri.receive_buf[TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN]);
//}


void SOBC_reset(SOBC_STRUCT* sobc_driver){
  HAL_GPIO_WritePin_C2A(sobc_driver->SOBC_reset_GPIO, sobc_driver->SOBC_reset_Pin, GPIO_PIN_RESET);
  c2a_delay_ms(5);
  HAL_GPIO_WritePin_C2A(sobc_driver->SOBC_reset_GPIO, sobc_driver->SOBC_reset_Pin, GPIO_PIN_SET);
}


//MOBCからSOBCへのGPIO線の動作確認用関数
void SOBC_debug_GPIO(SOBC_STRUCT* sobc_driver){
  HAL_GPIO_WritePin_C2A(sobc_driver->SOBC_debug_GPIO, sobc_driver->SOBC_debug_Pin, GPIO_PIN_RESET);
  c2a_delay_ms(1000);
  HAL_GPIO_WritePin_C2A(sobc_driver->SOBC_debug_GPIO, sobc_driver->SOBC_debug_Pin, GPIO_PIN_SET);
}


void SOBC_PORT_INIT(SOBC_STRUCT* sobc_driver){
  queue_init(&sobc_queue, SOBC_MAX_RECEIVE_PACKET_NUM, receive_buf);
  HAL_SPI_Init_C2A(sobc_driver->spi_handler);
}


_ACK SOBC_check_tcp_headers(SOBC_STRUCT* sobc_driver){
  int type=TCP_get_type(&(sobc_driver->tcp));
  if(type==TCP_TYPE_TLM){//TLM, should be fixed using enum
    if(TCP_TLM_get_packet_id(&(sobc_driver->tcp))==SOBC_Tlm_CODE_SOBC_TO_MOBC){
      SOBC_TLM_UPDATE(sobc_driver); //Tlm Update from
      return _TLM_UPDATE_SUCCESS;
    }
    else{//他のテレメトリの場合,もう片方の衛星のテレメトリがSOBCを介して来るとか？
      //ほんとはここでエラーチェック入れる必要有
      return _SUCCESS;
    }
  }
  else if(type==1){//CMD　SOBCから来るコマンド　（GSからのも含む）
    //ほんとはここでエラーチェック入れる必要有
    return _SUCCESS;
  }
  else return _SUCCESS;
}


void SOBC_TLM_UPDATE(SOBC_STRUCT* sobc_driver){
    uint8_t* head_tlm;
    head_tlm=CTP_get_user_data_head(&(sobc_driver->tcp));
    SOBC_TlmData* tlm_data = &(sobc_driver->tlm_data);
//    system
    memcpy(&(tlm_data->system.SOBC_Time), head_tlm, 4);
    memcpy(&(tlm_data->system.anomaly_count), head_tlm + 4, 1);
    memcpy(&(tlm_data->system.latest_anomaly_ID), head_tlm + 5, 1);
    memcpy(&(tlm_data->system.reset_count), head_tlm + 6, 1);
//    curs
    memcpy(&(tlm_data->curs.curs_sobc), head_tlm + 7, 2);
//    vols
    memcpy(&(tlm_data->vols.vols_sobc), head_tlm + 9, 2);
    memcpy(&(tlm_data->vols.vols_mags), head_tlm + 11, 2);
    memcpy(&(tlm_data->vols.vols_suns), head_tlm + 13, 2);
    memcpy(&(tlm_data->vols.vols_tofs), head_tlm + 15, 2);
//    temp
    memcpy(&(tlm_data->temp.temp_PX), head_tlm + 17, 2);
    memcpy(&(tlm_data->temp.temp_PY), head_tlm + 19, 2);

}
