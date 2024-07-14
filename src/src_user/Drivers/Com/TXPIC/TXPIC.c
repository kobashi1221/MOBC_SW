/*
 * TXPICUART.c
 *
 *  Created on: 2020/08/20
 *      Author: Takumi NORO
 */



#include <string.h> //memcpy用
#include <stdio.h> //printf用
#include <stdint.h> //for uint8_t

#include "TXPIC.h"
#include "../../../Applications/DriverInstances/di_struct.h"
#include <src_user/Library/General/DBG_printf.h>
//#include "stm32f4xx_hal.h" //HAL関数用

Queue txpic_queue;
static Buffer receive_buf[TXPIC_MAX_RECEIVE_PACKET_NUM];

void TXPIC_TLM_UPDATE(TXPIC_STRUCT* txpic_driver);
void TXPIC_AH1_UPDATE(TXPIC_STRUCT* txpic_driver);
void TXPIC_AH2_UPDATE(TXPIC_STRUCT* txpic_driver);
// Template of function -----------------------------------------------------------------*/
/**
  * @name   :
  * @brief  :
  * @param  :
  * @retval :
  */

/*
  * @name   :TXPIC_UART_init
  * @brief  :TXPIC用ハンドラー等の設定
  * @param  mytxpicuart:TXPIC用の固有の値を割り当てる
  * @return :void
  */
_ACK TXPIC_UART_init(TXPIC_STRUCT* mytxpic){
	//for debug
	printf("COM RXPIC_Driver init.\r\n");
	mytxpic->uart_handler = &huart3;
	mytxpic->TIMEOUT = 0x0f;
	mytxpic->ah_no = TXPIC_NO_ANOMALY;
	TXPIC_PORT_INIT(mytxpic);
	return _SUCCESS;
}

/*
  * @name   :TXPIC_Send
  * @brief  :TXPICへの送信関数
  * @param
  * mytxpicuart:TXPIC用構造体
  * data:送信データ
  * len:データ数
  * @return :void
  * @comment:同期通信でいいのか微妙（ITを使うべき？）
  *          タイマー割込みできなくなる気がする
  */
_ACK TXPIC_Send(const TXPIC_STRUCT* mytxpic,uint8_t* data,int len){
	int ret;
	ObcTime at_now=TMGR_get_master_clock();
//	TXPIC側の状態を見てデータを送るか判断
	if(HAL_GPIO_ReadPin_C2A(GPIOH, GPIO_PIN_3) == GPIO_PIN_RESET){
		return _TRANSMIT_FAILED;
	}
	//少し待ってからTXPICに送信
	else{
		// Timeout function
		while(HAL_UART_Get_State_C2A(mytxpic->uart_handler) != 32){
			ObcTime now=TMGR_get_master_clock();
			//UART TransmitのTimeoutは設定した時間以上送信にかかってたら終了するってやつ
			if(OBCT_diff_in_msec(&at_now ,&now)>((unsigned int)(mytxpic->TIMEOUT))){
				break;
			}
		}

		ret=HAL_UART_Transmit_IT_C2A(mytxpic->uart_handler,data,len);

		while(HAL_UART_Get_State_C2A(mytxpic->uart_handler) != 32){
			ObcTime now=TMGR_get_master_clock();
			if(OBCT_diff_in_msec(&at_now ,&now)>((unsigned int)(mytxpic->TIMEOUT))){
				break;
			}
		}

	    if(DBG_Mode_Analyze(TLM_DBG) && (data[0]&0x10) == 0x00){
	      HAL_UART_Transmit_C2A(&huart8, data, len, 0xff);
	      printf("\r\n");
	    }

		print(TXPIC,"send packet to TXPIC is\r\n");
		for(int i = 0; i <get_packet_len(data); i++)
		{
			print(TXPIC,"%02x ", data[i]);
		}
		print(TXPIC,"\r\n ");
	}

	return (_ACK)ret;
}


/*
  * @name   :TXPIC_Receive
  * @brief  :TXPICからの受信関数
  * @param
  * mytxpicuart:TXPIC用構造体
  * @return :rxbuf（10byteの受信データ）
  * @comment:同期通信でいいのか微妙（ITを使うべき？）
  *          タイマー割込みできなくなる気がする
  */
_ACK TXPIC_Receive(TXPIC_STRUCT* mytxpic,uint8_t* data){
	int ret;
	//int testN=10;
#define testN 10
	uint8_t rxbuf[testN];

	//for debug
	printf("COM TXPIC_Driver Receive function.\r\n");

	ret=HAL_UART_Receive_C2A(mytxpic->uart_handler,(uint8_t *)&rxbuf,testN,mytxpic->TIMEOUT);
	//受信した分先頭ポインタからインクリメントしたアドレスの値に，受信した情報を格納
	for(int i=0;i<testN;i++){
	*(data+i)=rxbuf[i];
	}
	return _SUCCESS;
}


_ACK TXPIC_check_tcp_headers(TXPIC_STRUCT* txpic_driver){
	int type=TCP_get_type(&(txpic_driver->tcp));

	if(type==0){//TLM, should be fixed using enum
		if(TCP_TLM_get_packet_id(&(txpic_driver->tcp))==TXPIC_Tlm_CODE_TXPIC_TO_MOBC){
			TXPIC_TLM_UPDATE(txpic_driver); //Tlm Update from RXPIC to MOBC
			return _TLM_UPDATE_SUCCESS;
		}
		else if(TCP_TLM_get_packet_id(&(txpic_driver->tcp))==TXPIC_Tlm_CODE_TXPIC_AH1){
			TXPIC_AH1_UPDATE(txpic_driver);
			txpic_driver->ah_no = TXPIC_AH_1;
			return _TLM_AH_UPDATE_SUCCESS;
		}
		else if(TCP_TLM_get_packet_id(&(txpic_driver->tcp))==TXPIC_Tlm_CODE_TXPIC_AH2){
			TXPIC_AH2_UPDATE(txpic_driver);
			txpic_driver->ah_no = TXPIC_AH_2;
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
	else return _UNKNOWN;
}


void TXPIC_TLM_UPDATE(TXPIC_STRUCT* txpic_driver){
	uint8_t* head_tlm;
	head_tlm=CTP_get_user_data_head(&(txpic_driver->tcp));
	TXPIC_TlmData* tlm_data = &(txpic_driver->tlm_data);

	memcpy(&(tlm_data->txpic_to_mobc.flag), head_tlm, 2);
	memcpy(&(tlm_data->txpic_to_mobc.Power_on_reset_count), head_tlm + 2,1);
	memcpy(&(tlm_data->txpic_to_mobc.WDT_reset_count), head_tlm + 3, 1);
	memcpy(&(tlm_data->txpic_to_mobc.Software_reset_count), head_tlm + 4, 1);
	memcpy(&(tlm_data->txpic_to_mobc.TXPIC_Time), head_tlm + 5, 4);
	memcpy(&(tlm_data->txpic_to_mobc.GPIO_State), head_tlm + 9, 3);

	memcpy(&(tlm_data->txpic_to_mobc.cur_vols_pic.curs_txpic), head_tlm + 12, 2);
	memcpy(&(tlm_data->txpic_to_mobc.cur_vols_pic.curs_rxpic), head_tlm + 14, 2);
	memcpy(&(tlm_data->txpic_to_mobc.cur_vols_pic.curs_tx), head_tlm + 16, 2);

	memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.curs_rxtx5v), head_tlm + 18, 2);
	memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.curs_mobc), head_tlm + 20, 2);
	memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.vols_txpic), head_tlm + 22, 2);
	memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.vols_rxpic), head_tlm + 24, 2);
	memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.vols_rxtx5v), head_tlm + 26, 2);
	memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.vols_tx), head_tlm + 28, 2);

    memcpy(&(tlm_data->txpic_to_mobc.temp.txpic), head_tlm + 30, 2);
    memcpy(&(tlm_data->txpic_to_mobc.temp.rxpic), head_tlm + 32, 2);
	memcpy(&(tlm_data->txpic_to_mobc.temp.strmx), head_tlm + 34, 2);
	memcpy(&(tlm_data->txpic_to_mobc.temp.strmy), head_tlm + 36, 2);
	memcpy(&(tlm_data->txpic_to_mobc.temp.strmz), head_tlm + 38, 2);
	memcpy(&(tlm_data->txpic_to_mobc.temp.strpx), head_tlm + 40, 2);
	memcpy(&(tlm_data->txpic_to_mobc.temp.strpy), head_tlm + 42, 2);
    memcpy(&(tlm_data->txpic_to_mobc.temp.strpz), head_tlm + 44, 2);
    memcpy(&(tlm_data->txpic_to_mobc.temp.rx), head_tlm + 46, 2);

    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.curs_sap1_px), head_tlm + 48, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.curs_sap1_py), head_tlm + 50, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.curs_sap1_pz), head_tlm + 52, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.curs_sap2_mx), head_tlm + 54, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.curs_sap2_my), head_tlm + 56, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.vols_sap1), head_tlm + 58, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.vols_sap2), head_tlm + 60, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.vols_ioex), head_tlm + 62, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.vols_2ndbat), head_tlm + 64, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.vols_bus), head_tlm + 66, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.curs_2ndbat), head_tlm + 68, 2);
    memcpy(&(tlm_data->txpic_to_mobc.cur_vols_ioex.curs_bus), head_tlm + 70, 2);

	memcpy(&(tlm_data->txpic_to_mobc.temp._2ndbat1), head_tlm + 72, 2);
	memcpy(&(tlm_data->txpic_to_mobc.temp._2ndbat2), head_tlm + 74, 2);
    memcpy(&(tlm_data->txpic_to_mobc.temp._2ndbat3), head_tlm + 76, 2);
    memcpy(&(tlm_data->txpic_to_mobc.temp._2ndbat4), head_tlm + 78, 2);

    memcpy(&(tlm_data->txpic_to_mobc.heat_flag), head_tlm + 80, 1);
	memcpy(&(tlm_data->txpic_to_mobc.dac_mppt1_vol), head_tlm + 81, 2);
	memcpy(&(tlm_data->txpic_to_mobc.dac_mppt2_vol), head_tlm + 83, 2);
	memcpy(&(tlm_data->txpic_to_mobc.dac_charge_vol), head_tlm + 85, 2);
	memcpy(&(tlm_data->txpic_to_mobc.downlink_count), head_tlm + 87, 2);
	memcpy(&(tlm_data->txpic_to_mobc.satellite_com_count), head_tlm + 89, 2);
	memcpy(&(tlm_data->txpic_to_mobc.latest_anomaly_ID), head_tlm + 91, 1);
	memcpy(&(tlm_data->txpic_to_mobc.anomaly_count), head_tlm + 92, 2);
	memcpy(&(tlm_data->txpic_to_mobc.no_com_sec_rxpic), head_tlm + 94, 1);
	memcpy(&(tlm_data->txpic_to_mobc.no_com_sec_mobc), head_tlm + 95, 1);
	memcpy(&(tlm_data->txpic_to_mobc.no_com_sec_gs), head_tlm + 96, 2);
	memcpy(&(tlm_data->txpic_to_mobc.stackoverflow_count), head_tlm + 98, 1);
	memcpy(&(tlm_data->txpic_to_mobc.unrecognized_reset_count), head_tlm + 99, 1);
	memcpy(&(tlm_data->txpic_to_mobc.mobc_enable_count), head_tlm + 100, 1);
	memcpy(&(tlm_data->txpic_to_mobc.reg_rxtx5V_Disable_count), head_tlm + 101, 1);
	memcpy(&(tlm_data->txpic_to_mobc.my_time), head_tlm + 102, 4);

	memcpy(&(tlm_data->txpic_to_mobc.cw_message_ratio), head_tlm + 106, 1);
    memcpy(&(tlm_data->txpic_to_mobc.cw_message_start_num), head_tlm + 107, 1);
    memcpy(&(tlm_data->txpic_to_mobc.cw_message_end_num), head_tlm + 108, 1);

	tlm_data->txpic_to_mobc.Total_reset_count=
			(tlm_data->txpic_to_mobc.Power_on_reset_count+
			 tlm_data->txpic_to_mobc.WDT_reset_count+
			 tlm_data->txpic_to_mobc.Software_reset_count +
			 tlm_data->txpic_to_mobc.stackoverflow_count +
			 tlm_data->txpic_to_mobc.unrecognized_reset_count)%256;
}

void TXPIC_AH1_UPDATE(TXPIC_STRUCT* txpic_driver){
	uint8_t* head_tlm;
	head_tlm=CTP_get_user_data_head(&(txpic_driver->tcp));
	TXPIC_TlmData* tlm_data = &(txpic_driver->tlm_data);

	memcpy(&(tlm_data->txpic_ah1.Anomaly_time1), head_tlm, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID1), head_tlm + 2, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time2), head_tlm + 3, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID2), head_tlm + 5, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time3), head_tlm + 6, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID3), head_tlm + 8, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time4), head_tlm + 9, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID4), head_tlm + 11, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time5), head_tlm + 12, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID5), head_tlm + 14, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time6), head_tlm + 15, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID6), head_tlm + 17, 1);

	memcpy(&(tlm_data->txpic_ah1.Anomaly_time7), head_tlm + 18, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID7), head_tlm + 20, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time8), head_tlm + 21, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID8), head_tlm + 23, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time9), head_tlm + 24, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID9), head_tlm + 26, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time10), head_tlm + 27, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID10), head_tlm + 29, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time11), head_tlm + 30, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID11), head_tlm + 32, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time12), head_tlm + 33, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID12), head_tlm + 35, 1);

	memcpy(&(tlm_data->txpic_ah1.Anomaly_time13), head_tlm + 36, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID13), head_tlm + 38, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time14), head_tlm + 39, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID14), head_tlm + 41, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time15), head_tlm + 42, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID15), head_tlm + 44, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time16), head_tlm + 45, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID16), head_tlm + 47, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time17), head_tlm + 48, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID17), head_tlm + 50, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time18), head_tlm + 51, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID18), head_tlm + 53, 1);

	memcpy(&(tlm_data->txpic_ah1.Anomaly_time19), head_tlm + 54, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID19), head_tlm + 56, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time20), head_tlm + 57, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID20), head_tlm + 59, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time21), head_tlm + 60, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID21), head_tlm + 62, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time22), head_tlm + 63, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID22), head_tlm + 65, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time23), head_tlm + 66, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID23), head_tlm + 68, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time24), head_tlm + 69, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID24), head_tlm + 71, 1);

	memcpy(&(tlm_data->txpic_ah1.Anomaly_time25), head_tlm + 72, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID25), head_tlm + 74, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time26), head_tlm + 75, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID26), head_tlm + 77, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time27), head_tlm + 78, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID27), head_tlm + 80, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time28), head_tlm + 81, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID28), head_tlm + 83, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time29), head_tlm + 84, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID29), head_tlm + 86, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time30), head_tlm + 87, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID30), head_tlm + 89, 1);

	memcpy(&(tlm_data->txpic_ah1.Anomaly_time31), head_tlm + 90, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID31), head_tlm + 92, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time32), head_tlm + 93, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID32), head_tlm + 95, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time33), head_tlm + 96, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID33), head_tlm + 98, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time34), head_tlm + 99, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID34), head_tlm + 101, 1);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_time35), head_tlm + 102, 2);
	memcpy(&(tlm_data->txpic_ah1.Anomaly_ID35), head_tlm + 104, 1);
}

void TXPIC_AH2_UPDATE(TXPIC_STRUCT* txpic_driver){
	uint8_t* head_tlm;
	head_tlm=CTP_get_user_data_head(&(txpic_driver->tcp));
	TXPIC_TlmData* tlm_data = &(txpic_driver->tlm_data);

	memcpy(&(tlm_data->txpic_ah2.Anomaly_time36), head_tlm, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID36), head_tlm + 2, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time37), head_tlm + 3, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID37), head_tlm + 5, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time38), head_tlm + 6, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID38), head_tlm + 8, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time39), head_tlm + 9, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID39), head_tlm + 11, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time40), head_tlm + 12, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID40), head_tlm + 14, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time41), head_tlm + 15, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID41), head_tlm + 17, 1);

	memcpy(&(tlm_data->txpic_ah2.Anomaly_time42), head_tlm + 18, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID42), head_tlm + 20, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time43), head_tlm + 21, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID43), head_tlm + 23, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time44), head_tlm + 24, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID44), head_tlm + 26, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time45), head_tlm + 27, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID45), head_tlm + 29, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time46), head_tlm + 30, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID46), head_tlm + 32, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time47), head_tlm + 33, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID47), head_tlm + 35, 1);

	memcpy(&(tlm_data->txpic_ah2.Anomaly_time48), head_tlm + 36, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID48), head_tlm + 38, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time49), head_tlm + 39, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID49), head_tlm + 41, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time50), head_tlm + 42, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID50), head_tlm + 44, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time51), head_tlm + 45, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID51), head_tlm + 47, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time52), head_tlm + 48, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID52), head_tlm + 50, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time53), head_tlm + 51, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID53), head_tlm + 53, 1);

	memcpy(&(tlm_data->txpic_ah2.Anomaly_time54), head_tlm + 54, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID54), head_tlm + 56, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time55), head_tlm + 57, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID55), head_tlm + 59, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time56), head_tlm + 60, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID56), head_tlm + 62, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time57), head_tlm + 63, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID57), head_tlm + 65, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time58), head_tlm + 66, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID58), head_tlm + 68, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time59), head_tlm + 69, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID59), head_tlm + 71, 1);

	memcpy(&(tlm_data->txpic_ah2.Anomaly_time60), head_tlm + 72, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID60), head_tlm + 74, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time61), head_tlm + 75, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID61), head_tlm + 77, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time62), head_tlm + 78, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID62), head_tlm + 80, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time63), head_tlm + 81, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID63), head_tlm + 83, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time64), head_tlm + 84, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID64), head_tlm + 86, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time65), head_tlm + 87, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID65), head_tlm + 89, 1);

	memcpy(&(tlm_data->txpic_ah2.Anomaly_time66), head_tlm + 90, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID66), head_tlm + 92, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time67), head_tlm + 93, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID67), head_tlm + 95, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time68), head_tlm + 96, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID68), head_tlm + 98, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time69), head_tlm + 99, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID69), head_tlm + 101, 1);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_time70), head_tlm + 102, 2);
	memcpy(&(tlm_data->txpic_ah2.Anomaly_ID70), head_tlm + 104, 1);
	memcpy(&(tlm_data->txpic_ah2.position), head_tlm + 105, 1);
	memcpy(&(tlm_data->txpic_ah2.flag), head_tlm + 106, 1);
}
void TXPIC_PORT_INIT(TXPIC_STRUCT* txpic_driver){
	HAL_UART_Init_C2A(txpic_driver->uart_handler);
	queue_init(&txpic_queue, TXPIC_MAX_RECEIVE_PACKET_NUM, receive_buf);
	int ret=HAL_UARTEx_ReceiveToIdle_DMA_C2A(txpic_driver->uart_handler, &txpic_queue.dri.receive_buf[0], TCP_MAX_LEN);
	if(ret!=HAL_OK) printf("TXPIC Packet Receive function fail!\r\n");
}


/************************************** END OF FILE **************************************/
