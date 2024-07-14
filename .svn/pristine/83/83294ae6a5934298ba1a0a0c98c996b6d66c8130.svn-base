/*
 * RXPICUART.c
 *
 *  Created on: 2020/08/20
 *      Author: Takumi NORO
 */



#include "RXPIC.h"

#include <string.h> //memcpy用
#include <stdio.h> //printf用
#include <stdint.h> //for uint8_t
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include <src_user/Library/General/DBG_printf.h>


Queue rxpic_queue;
static Buffer receive_buf[RXPIC_MAX_RECEIVE_PACKET_NUM];
void RXPIC_TLM_UPDATE(RXPIC_STRUCT* rxpic_driver);
void RXPIC_AH1_UPDATE(RXPIC_STRUCT* rxpic_driver);
void RXPIC_AH2_UPDATE(RXPIC_STRUCT* rxpic_driver);

/*
  * @name   :RXPIC_UART_init
  * @brief  :RXPIC用ハンドラー等の設定
  * @param  myrxpicuart:RXPIC用の固有の値を割り当てる
  * @return :void
  */
_ACK RXPIC_UART_init(RXPIC_STRUCT* rxpic_driver){
	rxpic_driver->uart_handler = &huart1;
	rxpic_driver->TIMEOUT = 0xff;
	rxpic_driver->sequence_counter = 0;
	rxpic_driver->retransmit_flag = 0;
	rxpic_driver->ah_no = RXPIC_NO_ANOMALY;
	RXPIC_PORT_INIT(rxpic_driver);
	return _SUCCESS;
}
/*
  * @name   :RXPIC_Send
  * @brief  :RXPICへの送信関数
  * @param
  * myrxpicuart:RXPIC用構造体
  * data:送信データ
  * len:データ数
  * @return :void
  * @comment:同期通信でいいのか微妙（ITを使うべき？）
  *          タイマー割込みできなくなる気がする
  */
_ACK RXPIC_Send(const RXPIC_STRUCT* myrxpic,uint8_t* data,size_t len){
	print(RXPIC,"send packet to RXPIC is\r\n");
	for(int i = 0; i <get_packet_len(data); i++)
	{
		print(RXPIC,"%02x ", data[i]);
	}
	print(RXPIC,"\r\n ");

	return (_ACK)HAL_UART_Transmit_C2A(myrxpic->uart_handler,data,len,myrxpic->TIMEOUT);
}

/*
  * @name   :RXPIC_Receive
  * @brief  :RXPICからの受信関数
  * @param
  * myrxpicuart:RXPIC用構造体
  * data:格納する配列の先頭アドレス
  * @return :ret:HAL_Status
  * @comment:割込みをしていないのでテンポラリーに10Byte受信
  * 		  同期通信でいいのか微妙（ITを使うべき？）
  *          タイマー割込みできなくなる気がする
  */
_ACK RXPIC_Receive(RXPIC_STRUCT* myrxpic,uint8_t* data){
#define testN 10
	int ret;
	uint8_t rxbuf[testN];

	//for debug
	printf("COM RXPIC_Driver Receive function.\r\n");

	ret = HAL_UART_Receive_C2A(myrxpic->uart_handler,(uint8_t *)&rxbuf,testN,myrxpic->TIMEOUT);
	//受信した分先頭ポインタからインクリメントしたアドレスの値に，受信した情報を格納
	for(int i = 0; i < testN; i++){
	*(data+i) = rxbuf[i];
	}
	return (_ACK)ret;
}


_ACK RXPIC_check_tcp_headers(RXPIC_STRUCT* rxpic_driver){
	int type=TCP_get_type(&(rxpic_driver->tcp));

	if(type==0){//Todo TLM, should be fixed using enum
		if(TCP_TLM_get_packet_id(&(rxpic_driver->tcp))==RXPIC_Tlm_CODE_RXPIC_TO_MOBC){
			RXPIC_TLM_UPDATE(rxpic_driver); //Tlm Update from RXPIC to MOBC
			return _TLM_UPDATE_SUCCESS;
		}
		else if(TCP_TLM_get_packet_id(&(rxpic_driver->tcp))==RXPIC_Tlm_CODE_RXPIC_AH1){
			RXPIC_AH1_UPDATE(rxpic_driver);
			rxpic_driver->ah_no = RXPIC_AH_1;
			return _TLM_AH_UPDATE_SUCCESS;
		}
		else if(TCP_TLM_get_packet_id(&(rxpic_driver->tcp))==RXPIC_Tlm_CODE_RXPIC_AH2){
			RXPIC_AH2_UPDATE(rxpic_driver);
			rxpic_driver->ah_no = RXPIC_AH_2;
			return _TLM_AH_UPDATE_SUCCESS;
		}
		else{//他のテレメトリの場合,もう片方の衛星のテレメトリがRXPICを介して来るとか？
			//ほんとはここでエラーチェック入れる必要有
			return _SUCCESS;
		}
	}
	else if(type==1){//CMD　RXPICから来るコマンド　（GSからのも含む）
		//ほんとはここでエラーチェック入れる必要有
		return _SUCCESS;
	}
	else return _SUCCESS;
}

void RXPIC_TLM_UPDATE(RXPIC_STRUCT* rxpic_driver){
	uint8_t* head_tlm;
	head_tlm=CTP_get_user_data_head(&(rxpic_driver->tcp));
	RXPIC_TlmData* tlm_data = &(rxpic_driver->tlm_data);
	memcpy(&(tlm_data->rxpic_to_mobc.flag), head_tlm, 2);
	memcpy(&(tlm_data->rxpic_to_mobc.Power_on_reset_count), head_tlm + 2, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.WDT_reset_count), head_tlm + 3, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.Software_reset_count), head_tlm + 4, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.RXPIC_Time), head_tlm + 5, 4);
	memcpy(&(tlm_data->rxpic_to_mobc.GPIO_State), head_tlm + 9, 1);

	memcpy(&(tlm_data->rxpic_to_mobc.cur_vols_pic.curs_tx), head_tlm + 10, 2);
	memcpy(&(tlm_data->rxpic_to_mobc.cur_vols_pic.curs_rx), head_tlm + 12, 2);
	memcpy(&(tlm_data->rxpic_to_mobc.cur_vols_pic.curs_txpic), head_tlm + 14, 2);
	memcpy(&(tlm_data->rxpic_to_mobc.cur_vols_pic.curs_rxpic), head_tlm + 16, 2);

	memcpy(&(tlm_data->rxpic_to_mobc.rssi), head_tlm + 18, 2);
	memcpy(&(tlm_data->rxpic_to_mobc.RXfromGS_count), head_tlm + 20, 2);
	memcpy(&(tlm_data->rxpic_to_mobc.RXfromSAT_count), head_tlm + 22, 2);
	memcpy(&(tlm_data->rxpic_to_mobc.latest_anomaly_ID), head_tlm + 24, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.anomaly_count), head_tlm + 25, 2);
	memcpy(&(tlm_data->rxpic_to_mobc.no_com_sec_txpic), head_tlm + 27, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.no_com_sec_mobc), head_tlm + 28, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.no_com_sec_gs), head_tlm + 29, 2);
	memcpy(&(tlm_data->rxpic_to_mobc.stackoverflow_count), head_tlm + 31, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.unrecognized_reset_count), head_tlm + 32, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.mobc_reset_count), head_tlm + 33, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.reg_rxtx5V_Disable_count), head_tlm + 34, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.txpic_enable_count), head_tlm + 35, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.TLC_registered_count), head_tlm + 36, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.TLC_next_exe_index), head_tlm + 37, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.TLC_next_exe_ChID), head_tlm + 38, 2);
	memcpy(&(tlm_data->rxpic_to_mobc.TLC_next_time), head_tlm + 40, 4);
	memcpy(&(tlm_data->rxpic_to_mobc.RXfromSAT_LatestTime), head_tlm + 44, 2);
    memcpy(&(tlm_data->rxpic_to_mobc.mobc_boot_count), head_tlm + 46, 1);
	memcpy(&(tlm_data->rxpic_to_mobc.RXfromGS_LatestTime), head_tlm + 47, 2);

	tlm_data->rxpic_to_mobc.Total_reset_count=
			(tlm_data->rxpic_to_mobc.Power_on_reset_count+
					tlm_data->rxpic_to_mobc.WDT_reset_count+
					tlm_data->rxpic_to_mobc.Software_reset_count+
					tlm_data->rxpic_to_mobc.stackoverflow_count +
					tlm_data->rxpic_to_mobc.unrecognized_reset_count)%256;
}

void RXPIC_AH1_UPDATE(RXPIC_STRUCT* rxpic_driver){
	uint8_t* head_tlm;
	head_tlm=CTP_get_user_data_head(&(rxpic_driver->tcp));
	RXPIC_TlmData* tlm_data = &(rxpic_driver->tlm_data);

	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time1), head_tlm, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID1), head_tlm + 2, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time2), head_tlm + 3, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID2), head_tlm + 5, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time3), head_tlm + 6, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID3), head_tlm + 8, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time4), head_tlm + 9, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID4), head_tlm + 11, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time5), head_tlm + 12, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID5), head_tlm + 14, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time6), head_tlm + 15, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID6), head_tlm + 17, 1);

	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time7), head_tlm + 18, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID7), head_tlm + 20, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time8), head_tlm + 21, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID8), head_tlm + 23, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time9), head_tlm + 24, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID9), head_tlm + 26, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time10), head_tlm + 27, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID10), head_tlm + 29, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time11), head_tlm + 30, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID11), head_tlm + 32, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time12), head_tlm + 33, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID12), head_tlm + 35, 1);

	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time13), head_tlm + 36, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID13), head_tlm + 38, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time14), head_tlm + 39, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID14), head_tlm + 41, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time15), head_tlm + 42, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID15), head_tlm + 44, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time16), head_tlm + 45, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID16), head_tlm + 47, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time17), head_tlm + 48, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID17), head_tlm + 50, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time18), head_tlm + 51, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID18), head_tlm + 53, 1);

	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time19), head_tlm + 54, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID19), head_tlm + 56, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time20), head_tlm + 57, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID20), head_tlm + 59, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time21), head_tlm + 60, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID21), head_tlm + 62, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time22), head_tlm + 63, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID22), head_tlm + 65, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time23), head_tlm + 66, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID23), head_tlm + 68, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time24), head_tlm + 69, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID24), head_tlm + 71, 1);

	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time25), head_tlm + 72, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID25), head_tlm + 74, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time26), head_tlm + 75, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID26), head_tlm + 77, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time27), head_tlm + 78, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID27), head_tlm + 80, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time28), head_tlm + 81, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID28), head_tlm + 83, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time29), head_tlm + 84, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID29), head_tlm + 86, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time30), head_tlm + 87, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID30), head_tlm + 89, 1);

	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time31), head_tlm + 90, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID31), head_tlm + 92, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time32), head_tlm + 93, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID32), head_tlm + 95, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time33), head_tlm + 96, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID33), head_tlm + 98, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time34), head_tlm + 99, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID34), head_tlm + 101, 1);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_time35), head_tlm + 102, 2);
	memcpy(&(tlm_data->rxpic_ah1.Anomaly_ID35), head_tlm + 104, 1);
}

void RXPIC_AH2_UPDATE(RXPIC_STRUCT* rxpic_driver){
	uint8_t* head_tlm;
	head_tlm=CTP_get_user_data_head(&(rxpic_driver->tcp));
	RXPIC_TlmData* tlm_data = &(rxpic_driver->tlm_data);

	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time36), head_tlm, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID36), head_tlm + 2, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time37), head_tlm + 3, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID37), head_tlm + 5, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time38), head_tlm + 6, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID38), head_tlm + 8, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time39), head_tlm + 9, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID39), head_tlm + 11, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time40), head_tlm + 12, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID40), head_tlm + 14, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time41), head_tlm + 15, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID41), head_tlm + 17, 1);

	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time42), head_tlm + 18, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID42), head_tlm + 20, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time43), head_tlm + 21, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID43), head_tlm + 23, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time44), head_tlm + 24, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID44), head_tlm + 26, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time45), head_tlm + 27, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID45), head_tlm + 29, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time46), head_tlm + 30, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID46), head_tlm + 32, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time47), head_tlm + 33, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID47), head_tlm + 35, 1);

	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time48), head_tlm + 36, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID48), head_tlm + 38, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time49), head_tlm + 39, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID49), head_tlm + 41, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time50), head_tlm + 42, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID50), head_tlm + 44, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time51), head_tlm + 45, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID51), head_tlm + 47, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time52), head_tlm + 48, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID52), head_tlm + 50, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time53), head_tlm + 51, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID53), head_tlm + 53, 1);

	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time54), head_tlm + 54, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID54), head_tlm + 56, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time55), head_tlm + 57, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID55), head_tlm + 59, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time56), head_tlm + 60, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID56), head_tlm + 62, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time57), head_tlm + 63, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID57), head_tlm + 65, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time58), head_tlm + 66, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID58), head_tlm + 68, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time59), head_tlm + 69, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID59), head_tlm + 71, 1);

	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time60), head_tlm + 72, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID60), head_tlm + 74, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time61), head_tlm + 75, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID61), head_tlm + 77, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time62), head_tlm + 78, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID62), head_tlm + 80, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time63), head_tlm + 81, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID63), head_tlm + 83, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time64), head_tlm + 84, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID64), head_tlm + 86, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time65), head_tlm + 87, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID65), head_tlm + 89, 1);

	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time66), head_tlm + 90, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID66), head_tlm + 92, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time67), head_tlm + 93, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID67), head_tlm + 95, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time68), head_tlm + 96, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID68), head_tlm + 98, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time69), head_tlm + 99, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID69), head_tlm + 101, 1);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_time70), head_tlm + 102, 2);
	memcpy(&(tlm_data->rxpic_ah2.Anomaly_ID70), head_tlm + 104, 1);
	memcpy(&(tlm_data->rxpic_ah2.position), head_tlm + 105, 1);
	memcpy(&(tlm_data->rxpic_ah2.flag), head_tlm + 106, 1);
}

void RXPIC_PORT_INIT(RXPIC_STRUCT* rxpic_driver){
	HAL_UART_Init_C2A(rxpic_driver->uart_handler);
	queue_init(&rxpic_queue, RXPIC_MAX_RECEIVE_PACKET_NUM, receive_buf);
	int ret=HAL_UARTEx_ReceiveToIdle_DMA_C2A(rxpic_driver->uart_handler, &rxpic_queue.dri.receive_buf[0], TCP_MAX_LEN);
	if(ret!=HAL_OK) printf("RXPIC Packet Receive function fail!\r\n");
}


/************************************** END OF FILE **************************************/
