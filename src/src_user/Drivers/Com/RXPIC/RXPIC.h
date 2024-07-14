/*
 * RXPICUART.h
 *
 *  Created on: 2020/08/20
 *      Author: Takumi NORO
 */


/**
  *****************************************************************************************
  * @file    : RXPICUART.h
  * @author  : Takumi NORO
  * @version : 1.0
  * @date    : 2020/08/20
  * @brief   :　RXPICとのUART通信用
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

#ifndef RXPICUART_H_
#define RXPICUART_H_

#include <stdint.h> //for uint8_t
#include <stdbool.h> //for boolean
//#include "main.h"
#include <src_user/IfWrapper/HAL.h>
#include <src_user/Applications/DriverInstances/di_struct.h>
#include <src_user/TlmCmd/CCSDS/TCPacket.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/Drivers/Super/driver_super.h>
#include <src_user/Drivers/COM/RXPIC/rxpic_telemetry_data_definitions.h>
#include <src_user/Drivers/COM/RXPIC/rxpic_telemetry_definitions.h>
#include <src_user/TlmCmd/user_packet_handler.h>
#include "src_user/Drivers/COM/Circular_buffer.h"
#include <src_user/Drivers/COM/GS/command_response_struct.h>

#ifndef HAL_SILS
extern UART_HandleTypeDef huart1;
#endif
#ifdef HAL_SILS
extern void *huart1;
#endif

#define RXPIC_MAX_RECEIVE_PACKET_NUM 6

extern Queue rxpic_queue;
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

} RXPIC_Info;

typedef struct
{
    int TIMEOUT;
	int retransmit_flag;
    _ACK ack;
    TCP tcp;
    cycle_t last_recv_time;
    void *uart_handler;
	RXPIC_TlmData tlm_data;
	RXPIC_Info Info;
	uint8_t ah_no;
	uint16_t sequence_counter;
} RXPIC_STRUCT;

typedef enum{
	RXPIC_NO_ANOMALY,
	RXPIC_AH_1,
	RXPIC_AH_2
}RXPIC_AH;

#ifdef __cplusplus
 extern "C" {
#endif

_ACK RXPIC_UART_init(RXPIC_STRUCT* myrxpic);
_ACK RXPIC_Send(const RXPIC_STRUCT* myrxpic,uint8_t* data,size_t len);
_ACK RXPIC_Receive(RXPIC_STRUCT* myrxpic,uint8_t* data);
_ACK RXPIC_check_tcp_headers(RXPIC_STRUCT* rxpic_driver);

void RXPIC_PORT_INIT(RXPIC_STRUCT* rxpic_driver);

#ifdef __cplusplus
}
#endif
/************************************** END OF FILE **************************************/


#endif /* SAMPLEDRIVER_H_ */
