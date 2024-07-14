#ifndef DBG_H_
#define DBG_H_

#include <stdint.h>  //for uint8_t
//#include "main.h"
#include <src_user/IfWrapper/HAL.h>
#include "../../TlmCmd/CCSDS/TCPacket.h"
extern uint8_t DBG_rx_buf;
//#define TCP_MAX_LEN 1024
typedef struct
{
	TCP tcp;
	uint8_t packet_receive_Flag;
	uint8_t receive_data[TCP_MAX_LEN];
	int packet_len;
}DBG_STRUCT;

typedef enum
{
	  DBG_SUCCESS,
	  DBG_TYPE_AD,
	  DBG_TYPE_BD,
	  DBG_TYPE_BC,
	  DBG_INIT_FAILED,
	  DBG_FILL_SETUP_FAILED,
	  DBG_ON_OFF_FAILED,
	  DBG_NOT_RECEIVED,
	  DBG_FECW_MISSMATCH,
	  DBG_FLAG_OK,
	  DBG_UNKNOWN
} DBG_ACK;

DBG_ACK DBG_init(DBG_STRUCT* dbg);
DBG_ACK DBG_receive_tcp(DBG_STRUCT* dbg);
DBG_ACK DBG_check_common_tcp_headers(DBG_STRUCT* dbg);

void DBG_Callback(uint8_t DBG_rx_buf);

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);
//割込み用コールバック
#endif /* C2A_DRIVERS_DBG_DBG_H_ */
