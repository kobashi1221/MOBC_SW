#include "DBG.h"

#include <stdint.h> //for uint8_t

static int i = 0;
static uint8_t hex_count = 0;
static uint8_t hex_Packet[2] = {0};
static uint8_t Receive_Packet[TCP_MAX_LEN+2] = {0};
static uint8_t Receive_Flag = 0;
static uint8_t Packet_len = 0;
uint8_t DBG_rx_buf;


DBG_ACK DBG_check_receive_flag(DBG_STRUCT* dbg);

DBG_ACK DBG_init(DBG_STRUCT* dbg)
{
//DBG_STRUCTのインスタンスの構造体の初期化を行う
	dbg->packet_receive_Flag = 0;
	HAL_UART_Receive_IT_C2A(&huart8,(uint8_t *)&DBG_rx_buf,1);
	return DBG_SUCCESS;
}

DBG_ACK DBG_receive_tcp(DBG_STRUCT* dbg){
//受信処理を行う
	if(DBG_check_receive_flag(dbg) == DBG_FLAG_OK)
	{
//		Receive_Flag=0;	//kimura

		//Packet_lenはi+1が入っている
		memcpy(dbg->receive_data, Receive_Packet, Packet_len);
		dbg->packet_len = Packet_len;

		return DBG_SUCCESS;
	}
	else return DBG_NOT_RECEIVED;
}

DBG_ACK DBG_check_common_tcp_headers(DBG_STRUCT* dbg)
{
	return DBG_SUCCESS;
}

DBG_ACK DBG_check_receive_flag(DBG_STRUCT* dbg)
{
	dbg->packet_receive_Flag = Receive_Flag;
	if(Receive_Flag == 1)
	{
	Receive_Flag = 0;	//kimura memcpyでパケットを保存する前に書き換えられる可能性を考えて変更
	return DBG_FLAG_OK;
	}
	else return DBG_NOT_RECEIVED;
}

void DBG_Callback(uint8_t DBG_rx_buf)
{
	//DBGはパケットをASCIIで送ってくるので全て0,1,\r,\nのみが送られる
    //x41 43 61 63みたいな感じでPacletLengthとChanel ID 引数をhexで並べて入力させる
	if(Receive_Flag == 0){
		if (DBG_rx_buf >= 0x61 && DBG_rx_buf <= 0x66){    //a~fまで
			hex_Packet[hex_count] = DBG_rx_buf - 0x61 + 10;
			hex_count++;
		}
		else if (DBG_rx_buf >= 0x41 && DBG_rx_buf <= 0x46){	//A~Fまで
				hex_Packet[hex_count] = DBG_rx_buf - 0x37;
				hex_count++;
		}
		else if (DBG_rx_buf >= 0x30 && DBG_rx_buf <= 0x39){//0~1まで
			hex_Packet[hex_count] = DBG_rx_buf - 0x30;
			hex_count++;
		}
		else if (i >= 2 && DBG_rx_buf == 0x0a){//\n
			Receive_Flag = 1;
			Packet_len = i;
			hex_count = 0;
			i = 0;
		}else{
				//何も処理しない
		}

		if(hex_count == 2)
		{
			hex_count = 0;
			Receive_Packet[i] = (hex_Packet[0]<<4)|hex_Packet[1];
			i++;
		}
	}
}


