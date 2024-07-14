/*
 * Inter_Sat.c
 *
 *  Created on: 2020/12/20
 *      Author: kouta
  *****************************************************************************************
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2021/11/26  2.0   Sakaguchi         second revision
  *
  *****************************************************************************************
 */

#include "Inter_Sat.h"

#include <string.h> //memcpy用
#include <stdio.h> //printf用
#include <stdint.h> //for uint8_t
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/TlmCmd/telemetry_definitions.h>


Queue intersat_queue;
static Buffer receive_buf[INTERSAT_MAX_RECEIVE_PACKET_NUM];
void INTERSAT_TLM_UPDATE(INTERSAT_STRUCT* inter_sat_driver);

static uint8_t Receive_Packet[1024]={0};
static uint8_t Receive_Flag=0;
static uint16_t Packet_len=5;
uint8_t InterSat_buf;

_ACK INTERSAT_check_receive_flag(INTERSAT_STRUCT* myinter_sat);

// Template of function -----------------------------------------------------------------*/
/**
  * @name   :
  * @brief  :
  * @param  :
  * @retval :
  */

/*
  * @name   :INTER_SAT_UART_init
  * @brief  :INTER_SAT用ハンドラー等の設定
  * @param  myINTER_SATuart:INTER_SAT用の固有の値を割り当てる
  * @return :void
  */
_ACK INTERSAT_UART_init(INTERSAT_STRUCT* myinter_sat){
	//for debug
	print(ISAT,"COM INTER_SAT_Driver init.\r\n");

	myinter_sat->uart_handler = &huart5;
	myinter_sat->packet_receive_Flag = 0;
	myinter_sat->TIMEOUT = 0x0f;
	INTERSAT_PORT_INIT(myinter_sat);
	return _SUCCESS;
}

/*
  * @name   :INTER_SAT_Send
  * @brief  :他機への送信関数
  * @param
  * myINTER_SATuart:INTER_SAT用構造体
  * data:送信データ
  * len:データ数
  * @return :void
  */
_ACK INTERSAT_Send(const INTERSAT_STRUCT* myinter_sat, uint8_t* data,size_t len){
	print(ISAT,"send success\r\n");
	int ret;
	ObcTime at_now=TMGR_get_master_clock();

	while(HAL_UART_Get_State_C2A(myinter_sat->uart_handler) != 32){
		ObcTime now=TMGR_get_master_clock();
		//UART TransmitのTimeoutは設定した時間以上送信にかかってたら終了するってやつ
		if(OBCT_diff_in_msec(&at_now ,&now)>((unsigned int)(myinter_sat->TIMEOUT))){
			break;
		}
	}

	ret = HAL_UART_Transmit_C2A(myinter_sat->uart_handler,data,len,myinter_sat->TIMEOUT);
	if(ret != HAL_OK){
		printf("inter-sat error\r\n");
	}

	while(HAL_UART_Get_State_C2A(myinter_sat->uart_handler) != 32){
		ObcTime now=TMGR_get_master_clock();
		//UART TransmitのTimeoutは設定した時間以上送信にかかってたら終了するってやつ
		if(OBCT_diff_in_msec(&at_now ,&now)>((unsigned int)(myinter_sat->TIMEOUT))){
			break;
		}
	}

	print(ISAT,"send packet to Inter-SAT is\r\n");
	for(int i = 0; i <get_packet_len(data); i++)
	{
		print(ISAT,"%02x ", data[i]);
	}
	print(ISAT,"\r\n ");

	return _SUCCESS;
}

/*
  * @name   :INTER_SAT_Receive
  * @brief  :他機からの受信関数
  * @param
  * myINTER_SATuart:INTER_SAT用構造体
  * data:格納する配列の先頭アドレス
  * @return :ret:HAL_Status
  * @comment:割込みをしていないのでテンポラリーに10Byte受信
  * 		  同期通信でいいのか微妙（ITを使うべき？）
  *          タイマー割込みできなくなる気がする
  */
_ACK INTERSAT_Receive(INTERSAT_STRUCT* myinter_sat,uint8_t* data){
#define testN  10
	int ret;
	uint8_t rxbuf[testN];

	//for debug
	print(ISAT,"Inter_Sat_Driver Receive function.\r\n");

	ret = HAL_UART_Receive_C2A(myinter_sat->uart_handler,(uint8_t *)&rxbuf,testN,myinter_sat->TIMEOUT);
	//受信した分先頭ポインタからインクリメントしたアドレスの値に，受信した情報を格納
	for(int i = 0; i < testN; ++i){
	*(data + i) = rxbuf[i];
	}
	return _SUCCESS;
}

_ACK INTERSAT_Receive_tcp(INTERSAT_STRUCT* myiniter_sat){
	if (INTERSAT_check_receive_flag(myiniter_sat)==_FLAG_OK){
		memcpy(myiniter_sat->receive_data, Receive_Packet, Packet_len);
		myiniter_sat->packet_len=Packet_len;
		return _SUCCESS;
	}
	else return _NOT_RECEIVED;
}

_ACK INTERSAT_check_receive_flag(INTERSAT_STRUCT* myiniter_sat){
	myiniter_sat->packet_receive_Flag = Receive_Flag;
	if(Receive_Flag == 1){
		Receive_Flag=0;
		return _FLAG_OK;
	}
	else return _NOT_RECEIVED;
}

_ACK INTERSAT_check_tcp_headers(INTERSAT_STRUCT* intersat_driver){
	int type=TCP_get_type(&(intersat_driver->tcp));

	if(type==0){//TLM, should be fixed using enum
		if(TCP_TLM_get_packet_id(&(intersat_driver->tcp))==Tlm_CODE_INTERSAT){
			INTERSAT_TLM_UPDATE(intersat_driver); //Tlm Update from INTERSAT to MOBC
			return _TLM_UPDATE_SUCCESS;
		}
		else{//他のテレメトリの場合,もう片方の衛星のテレメトリがINTERSATを介して来るとか？
			//ほんとはここでエラーチェック入れる必要有
			return _SUCCESS;
		}
	}
	else if(type==1){//CMD　INTERSATから来るコマンド　（GSからのも含む）
		//ほんとはここでエラーチェック入れる必要有
		return _SUCCESS;
	}
	else return _SUCCESS;
}


void INTERSAT_TLM_UPDATE(INTERSAT_STRUCT* intersat_driver){
	uint8_t* head_tlm;
	head_tlm=CTP_get_user_data_head(&(intersat_driver->tcp));
	INTERSAT_TlmData* tlm_data = &(intersat_driver->tlm_data);
	ENDIAN_memcpy(&(tlm_data->sensor_info.ACCX), head_tlm, 4);
	ENDIAN_memcpy(&(tlm_data->sensor_info.ACCY), head_tlm+4, 4);
	ENDIAN_memcpy(&(tlm_data->sensor_info.ACCZ), head_tlm+8, 4);

	ENDIAN_memcpy(&(tlm_data->sensor_info.OMEGAX), head_tlm+12, 4);
	ENDIAN_memcpy(&(tlm_data->sensor_info.OMEGAY), head_tlm+16, 4);
	ENDIAN_memcpy(&(tlm_data->sensor_info.OMEGAZ), head_tlm+20, 4);

	ENDIAN_memcpy(&(tlm_data->sensor_info.MAGSX), head_tlm+24, 4);
	ENDIAN_memcpy(&(tlm_data->sensor_info.MAGSY), head_tlm+28, 4);
	ENDIAN_memcpy(&(tlm_data->sensor_info.MAGSZ), head_tlm+32, 4);

	ENDIAN_memcpy(&(tlm_data->sensor_info.SUNSX), head_tlm+36, 4);
	ENDIAN_memcpy(&(tlm_data->sensor_info.SUNSY), head_tlm+40, 4);
	ENDIAN_memcpy(&(tlm_data->sensor_info.SUNSZ), head_tlm+44, 4);


	ENDIAN_memcpy(&(tlm_data->system.rssi), head_tlm+48, 2);
	ENDIAN_memcpy(&(tlm_data->system.lora_rssi), head_tlm+50, 4);
	ENDIAN_memcpy(&(tlm_data->system.rxpic_time), head_tlm+54, 4);
	ENDIAN_memcpy(&(tlm_data->system.txpic_time), head_tlm+58, 4);
	ENDIAN_memcpy(&(tlm_data->system.mobc_time), head_tlm+62, 4);

	ENDIAN_memcpy(&(tlm_data->orbit.juliandate), head_tlm+66, 8);
	ENDIAN_memcpy(&(tlm_data->orbit.utc_year), head_tlm+74, 2);
	ENDIAN_memcpy(&(tlm_data->orbit.utc_month), head_tlm+76, 1);
	ENDIAN_memcpy(&(tlm_data->orbit.utc_day), head_tlm+77, 1);
	ENDIAN_memcpy(&(tlm_data->orbit.utc_hour), head_tlm+78, 1);
	ENDIAN_memcpy(&(tlm_data->orbit.utc_min), head_tlm+79, 1);
	ENDIAN_memcpy(&(tlm_data->orbit.utc_sec), head_tlm+80, 1);

	ENDIAN_memcpy(&(tlm_data->orbit.latitude), head_tlm+81, 4);
	ENDIAN_memcpy(&(tlm_data->orbit.Longitude), head_tlm+85, 4);
	ENDIAN_memcpy(&(tlm_data->orbit.Altitude), head_tlm+89, 4);

	ENDIAN_memcpy(&(tlm_data->orbit.PositionX), head_tlm+93, 4);
	ENDIAN_memcpy(&(tlm_data->orbit.PositionY), head_tlm+97, 4);
	ENDIAN_memcpy(&(tlm_data->orbit.PositionZ), head_tlm+101, 4);

	ENDIAN_memcpy(&(tlm_data->orbit.VelocityX), head_tlm+105, 4);
	ENDIAN_memcpy(&(tlm_data->orbit.VelocityY), head_tlm+109, 4);
	ENDIAN_memcpy(&(tlm_data->orbit.VelocityZ), head_tlm+113, 4);

	ENDIAN_memcpy(&(tlm_data->orbit.RelativePositionX), head_tlm+117, 4);
	ENDIAN_memcpy(&(tlm_data->orbit.RelativePositionY), head_tlm+121, 4);
	ENDIAN_memcpy(&(tlm_data->orbit.RelativePositionZ), head_tlm+125, 4);

	ENDIAN_memcpy(&(tlm_data->orbit.RelativeVelocityX), head_tlm+129, 4);
	ENDIAN_memcpy(&(tlm_data->orbit.RelativeVelocityY), head_tlm+133, 4);
	ENDIAN_memcpy(&(tlm_data->orbit.RelativeVelocityZ), head_tlm+137, 4);

	ENDIAN_memcpy(&(tlm_data->orbit.Ballistic_Coefficient), head_tlm+141, 4);
}

void INTERSAT_PORT_INIT(INTERSAT_STRUCT* intersat_driver){
	HAL_UART_Init_C2A(intersat_driver->uart_handler);
	queue_init(&intersat_queue, INTERSAT_MAX_RECEIVE_PACKET_NUM, receive_buf);
	uint8_t ret = HAL_UART_Receive_DMA_C2A(intersat_driver->uart_handler,(uint8_t*)&(intersat_queue.dri.byte_buf),1);
	if(ret!=HAL_OK) printf("INTERSAT Packet Receive function fail!\r\n");
}

