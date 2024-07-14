/*
 * TXPICUART.h
 *
 *  Created on: 2020/08/20
 *      Author: Takumi NORO
 */


/**
  *****************************************************************************************
  * @file    : TXPICUART.h
  * @author  : Takumi NORO
  * @version : 1.0
  * @date    : 2020/08/20
  * @brief   :　TXPICとのUART通信用
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/07/21  1.0   T.NORO         First revision
  *
  *****************************************************************************************
  */

#ifndef TXPICUART_H_
#define TXPICUART_H_

#include <stdint.h> //for uint8_t
#include <stdbool.h> //for boolean
//#include "main.h"
#include <src_user/Applications/DriverInstances/di_struct.h>
#include <src_user/IfWrapper/HAL.h>
#include <src_user/TlmCmd/CCSDS/TCPacket.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/Drivers/Super/driver_super.h>
#include <src_user/Drivers/COM/TXPIC/txpic_telemetry_data_definitions.h>
#include <src_user/Drivers/COM/TXPIC/txpic_telemetry_definitions.h>
#include "src_user/Drivers/COM/Circular_buffer.h"

#ifndef HAL_SILS
extern UART_HandleTypeDef huart3;
#endif
#ifdef HAL_SILS
extern void *huart3;
#endif
extern uint8_t TXPIC_rx_buf;
extern int cobc_flag;

#define TXPIC_MAX_RECEIVE_PACKET_NUM 3

extern Queue txpic_queue;

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
} TXPIC_Info;

//Driverの情報格納用構造体（素子ごととかに作る，名前は素子の名前とか）
//この構造体は別の場所で定義する（Driverフォルダ）
//素子ごとに特有な変数とか全てここに格納する．アクセスするアドレス値とか
typedef struct
{
	uint8_t packet_receive_Flag;
	uint8_t receive_data[TCP_MAX_LEN];
	int packet_len;
    int TIMEOUT;
    _ACK ack;
    TCP tcp;
    cycle_t last_recv_time;
    void *uart_handler;
	TXPIC_TlmData tlm_data;
	TXPIC_Info Info;
	uint8_t reanalyze_packet;
	uint8_t ah_no;
	uint16_t picture_id;
} TXPIC_STRUCT;

typedef enum{
	TXPIC_NO_ANOMALY,
	TXPIC_AH_1,
	TXPIC_AH_2
}TXPIC_AH;

#ifdef __cplusplus
 extern "C" {
#endif

_ACK TXPIC_UART_init(TXPIC_STRUCT* mytxpic);
_ACK TXPIC_Send(const TXPIC_STRUCT* mytxpic,uint8_t* data,int len);
_ACK TXPIC_Receive(TXPIC_STRUCT* mytxpic,uint8_t* data);
_ACK TXPIC_check_tcp_headers(TXPIC_STRUCT* txpic_driver);

void TXPIC_PORT_INIT(TXPIC_STRUCT* txpic_driver);
void TXPIC_Callback(uint8_t rx_buf);

#ifdef __cplusplus
}
#endif
/************************************** END OF FILE **************************************/


#endif /* SAMPLEDRIVER_H_ */
