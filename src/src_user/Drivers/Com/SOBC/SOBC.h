/*
 * SOBC.h
 *
 *  Created on: 2023/08/24
 *      Author: Naoki KOBAYASHI
 */


/**
  *****************************************************************************************
  * @file    : SOBC.h
  * @author  : Naoki KOBAYASHI
  * @version : 1.0
  * @date    : 2023/08/24
  * @brief   :　SOBCとのSPI通信用
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2023 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2023/08/24  1.0   N.KOBAYASHI         First revision
  *
  *****************************************************************************************
  */

#ifndef SOBC_H_
#define SOBC_H_

#include <stdint.h> //for uint8_t
#include <src_user/IfWrapper/HAL.h>
#include <src_user/Library/Delay.h>
#include <src_user/Applications/DriverInstances/di_struct.h>
#include <src_user/TlmCmd/CCSDS/TCPacket.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Drivers/COM/SOBC/sobc_telemetry_data_definitions.h>
#include <src_user/Drivers/COM/SOBC/sobc_telemetry_definitions.h>

#include <src_user/TlmCmd/user_packet_handler.h>
#include <src_user/Drivers/Power/PWR_State.h>
#include "src_user/Drivers/COM/Circular_buffer.h"

#define SOBC_MAX_RECEIVE_PACKET_NUM 5

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

} SOBC_Info;

typedef struct{
  uint32_t com_success_counter;
  uint32_t com_anomaly_counter;
  uint32_t com_content_anomary;
  uint32_t com_exe_counter;
} SOBC_ANOMARY_STRUCT;

typedef struct
{
    int TIMEOUT;
    _ACK ack;
    TCP tcp;
    cycle_t last_recv_time;
    ObcTime first_rcv_time;
    uint16_t SOBC_reset_Pin;
    uint16_t SOBC_debug_Pin;
    uint8_t sobc_on_off_flag;
    uint8_t sobc_regular_tlm_flag;
    void *SOBC_reset_GPIO;
    void *SOBC_debug_GPIO;
    void *spi_handler;
    uint8_t spi_send_data[256];
    SOBC_TlmData tlm_data;
    SOBC_Info Info;
    SOBC_ANOMARY_STRUCT sobc_anomaly;
    uint8_t ah_no;
} SOBC_STRUCT;

extern Queue sobc_queue;

#ifdef __cplusplus
 extern "C" {
#endif
_ACK SOBC_SPI_init(SOBC_STRUCT* sobc_driver);
_ACK SOBC_SendReceive(const SOBC_STRUCT* mysobc,uint8_t* tx_data,size_t len);
void SOBC_reset(SOBC_STRUCT* sobc_driver);
void SOBC_debug_GPIO(SOBC_STRUCT* sobc_driver);
void SOBC_PORT_INIT(SOBC_STRUCT* sobc_driver);
//uint8_t* get_receive_user_data(SOBC_STRUCT* sobc_driver);
_ACK SOBC_check_tcp_headers(SOBC_STRUCT* sobc_driver);
#ifdef __cplusplus
}
#endif
/************************************** END OF FILE **************************************/


#endif
