/*
 * COBC.h
 *
 *  Created on: 2020/08/20
 *      Author: Takumi NORO
 *
 *  Updated on: 2021/12/22
 *      Author: Yuta Sakaguchi
 */

#ifndef COBC_H_
#define COBC_H_

#include <stdint.h> //for uint8_t
#include <stdbool.h> //for boolean
#include <math.h>
#include <src_user/IfWrapper/HAL.h>
#include <src_user/Library/Delay.h>
#include <src_user/Applications/DriverInstances/di_struct.h>
#include <src_user/TlmCmd/CCSDS/TCPacket.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/Drivers/Super/driver_super.h>
#include <src_user/Drivers/COM/COBC/cobc_telemetry_data_definitions.h>
#include <src_user/Drivers/COM/COBC/cobc_telemetry_definitions.h>
#include "cobc_cmd_list.h"
#include <src_user/TlmCmd/user_packet_handler.h>
#include <src_core/Drivers/Super/driver_super.h>
#include "src_user/Drivers/COM/Circular_buffer.h"

#define COBC_MAX_RECEIVE_PACKET_NUM 4

#define COBC_UART_TLM_BUFFER 243

extern Queue cobc_queue;
typedef struct
{
  // C2A情報
  struct
  {
    PH_ACK ph_ack;                            //!< 受信したコマンドをPHに渡したときの返り値
  } c2a;                                                   //!< C2A情報
  // 通信情報
  struct
  {
//    MOBC_TX_ERR_CODE tx_err_code;                          //!< DriverSuperではなくDriverが持つべき通信エラー情報
//    MOBC_RX_ERR_CODE rx_err_code;                          //!< DriverSuperではなくDriverが持つべき通信エラー情報
  } comm;                                                  //!< 通信情報
} COBC_Info;

typedef enum{
  COBC_DISABLE,
  COBC_ENABLE
}COBC_FLAG;

typedef struct
{
  uint8_t info;//なんの情報をもらうか
  uint8_t comm_method;//方法(spiで統一?)
  uint16_t cobc_large_packet_cmd;
  uint32_t ImSize;//画像サイズ
  uint32_t start_block_addr;//先頭ブロックアドレス
  uint32_t addr;//ブロックアドレスのどのアドレスか
  uint16_t picture_id;
  int pacekt_counter;//何個のパケットをもらうか
  int count;//今いくつ受け取ったか
  uint8_t cycle_count;//一周期に何回パケットを貰うか
  uint16_t sram_input_num;//何枚の写真をSRAMに入れたか
  uint8_t auto_reset;
  uint8_t enable;
  uint8_t* sram_bank_addr;
}COBC_LARGE_PACKET;

typedef struct{
  uint8_t cobc_tlm_info;
  uint8_t cobc_tlm_info_temp;//一時的に変更するときに必要
  uint8_t cobc_tlm_count;
  uint8_t cobc_regular_tlm_flag; //COBCへの定期テレメをenable,disableするフラグ
  uint8_t cobc_periodic_tlm_flag; //COBCへの3秒周期テレメをenable,disableするフラグ
}COBC_SEND_TLM_STRUCT;

typedef struct
{
  int TIMEOUT;
  _ACK ack;
  TCP tcp;
  cycle_t last_recv_time;
  uint8_t cobc_on_off_flag;
  uint16_t COBC_GPIO_Pin;
  void* COBC_GPIO_PORT;
  uint16_t COBC_reset_Pin;
  void* COBC_reset_GPIO;
  UART_HandleTypeDef *uart_handler;
  COBC_TlmData tlm_data;
  COBC_Info Info;
  COBC_SEND_TLM_STRUCT cobc_tlm;
  COBC_LARGE_PACKET large_packet;
} COBC_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

_ACK COBC_UART_init(COBC_STRUCT* mycobc);
_ACK COBC_Send(COBC_STRUCT* mycobc,uint8_t* data, size_t len);
_ACK COBC_check_tcp_headers(COBC_STRUCT* cobc_driver);
void COBC_reset(COBC_STRUCT* cobc_driver);
void COBC_PORT_INIT(COBC_STRUCT* mycobc);
#ifdef __cplusplus
}
#endif
/************************************** END OF FILE **************************************/


#endif
