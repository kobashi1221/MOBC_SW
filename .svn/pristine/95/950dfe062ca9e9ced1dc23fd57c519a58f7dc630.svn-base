/*
 * Inter_Sat.h
 *
 *  Created on: 2020/12/20
 *      Author: Takumi Noro
 ***********************************************************************
 *  modified on: 2021/11/26
 *  	Author: Sakaguchi
 */

#ifndef C2A_DRIVERS_COM_INTER_SAT_INTER_SAT_H_
#define C2A_DRIVERS_COM_INTER_SAT_INTER_SAT_H_

#include <stdint.h> //for uint8_t
#include <src_user/IfWrapper/HAL.h>
#include <src_user/Applications/DriverInstances/di_struct.h>
#include <src_user/TlmCmd/CCSDS/TCPacket.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_user/Drivers/COM/INTER_SAT/Inter_Sat_telemetry_data_definitions.h>
#include <src_user/Drivers/COM/INTER_SAT/Inter_Sat_telemetry_definitions.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/Drivers/Super/driver_super.h>
#include <src_user/Drivers/COM/Circular_buffer.h>
#include <src_user/Drivers/COM/GS/command_response_struct.h>

#define INTERSAT_MAX_RECEIVE_PACKET_NUM 4
extern Queue intersat_queue;
extern uint8_t InterSat_buf;

typedef struct{
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
} InterSat_Info;

//Driverの情報格納用構造体（素子ごととかに作る，名前は素子の名前とか）
//この構造体は別の場所で定義する（Driverフォルダ）
//素子ごとに特有な変数とか全てここに格納する．アクセスするアドレス値とか
typedef struct
{
    int TIMEOUT;
    _ACK ack;
    TCP tcp;
	int packet_len;
	cycle_t last_recv_time;
	void *uart_handler;
	INTERSAT_TlmData tlm_data;
	uint8_t packet_receive_Flag;
	uint8_t receive_data[TCP_MAX_LEN];
	InterSat_Info Info;
	uint8_t uart_data[3];
	uint16_t sequence_counter;
	uint16_t divide_count;
	uint16_t lora_divide_count;
	uint8_t intersat_tlm_flag;
	int retransmit_flag;
} INTERSAT_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

_ACK INTERSAT_UART_init(INTERSAT_STRUCT* myinter_sat);
_ACK INTERSAT_Send(const INTERSAT_STRUCT* myinter_sat, uint8_t* data,size_t len);
_ACK INTERSAT_Receive(INTERSAT_STRUCT* myinter_sat,uint8_t* data);
_ACK INTERSAT_Receive_tcp(INTERSAT_STRUCT* myinter_sat);
_ACK INTERSAT_check_tcp_headers(INTERSAT_STRUCT* intersat_driver);

void INTERSAT_PORT_INIT(INTERSAT_STRUCT* intersat_driver);

#ifdef __cplusplus
}
#endif



#endif /* C2A_DRIVERS_COM_INTER_SAT_INTER_SAT_H_ */
