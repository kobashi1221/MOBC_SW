/*
 * large_packet_driver.c
 *
 *  Created on: 2021/12/20
 *      Author: sakaguchi
 */
//大容量通信用の汎用性関数
//packetをヘッダごと生成しsequence countをインクリメントしpacket analyzeする
#include <string.h> //memcpy用
#include <stdio.h> //printf用
#include <stdint.h> //for uint8_t
#include <math.h>
#include "../TlmCmd/user_packet_handler.h"
#include <src_core/System/TimeManager/time_manager.h>
#include <src_user/Drivers/large_packet_driver.h>
#include <src_user/Library/General/DBG_printf.h>

static LARGE_PACKET_STRUCT large_driver_;
LARGE_PACKET_STRUCT* const large_driver = &large_driver_;

//最初にこの関数を呼び出し, analyzeするデータの先頭ポインタと大きさ, テレメトリIDを渡す
void large_packet_TLM_analyze(uint8_t* data, size_t size, TLM_CODE tlm){
	if(size == 0 || size > 0x00300000){
		return;
	}
	analyzer_init();
	if(large_driver->analyze.function_flag == LARGE_PACKET_FLAG_OFF){
		large_driver->large_packet = (void*) data;
		large_driver->analyze.packet_size = size;
		large_driver->tlm.packet_identifer = tlm;
		large_driver->common.type = TCP_TYPE_TLM;
		large_driver->analyze.header_length =  TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN;
		uint16_t tlm_data_size;
		switch(large_driver->tlm.packet_identifer){
			case COBC_MCAM_TLM:
			case COBC_SCAM_TLM:
			case Tlm_CODE_SUBCAM:
				tlm_data_size = large_driver->analyze.per_packet_size - large_driver->analyze.header_length - ID_SIZE;
				switch(large_driver->analyze.packet_size % tlm_data_size){
					case 0:
						large_driver->analyze.packet_counter = (large_driver->analyze.packet_size / tlm_data_size);
						large_driver->analyze.pre_packet_counter = (large_driver->analyze.packet_size / tlm_data_size);
						break;
					default:
						large_driver->analyze.packet_counter = (large_driver->analyze.packet_size / tlm_data_size) + 1;//切り上げ
						large_driver->analyze.pre_packet_counter = (large_driver->analyze.packet_size / tlm_data_size) + 1;//切り上げ
						break;
				}
			break;
			default:
				tlm_data_size = large_driver->analyze.per_packet_size - large_driver->analyze.header_length;
				switch(large_driver->analyze.packet_size % tlm_data_size){
					case 0:
						large_driver->analyze.packet_counter = (large_driver->analyze.packet_size / tlm_data_size);
						large_driver->analyze.pre_packet_counter = (large_driver->analyze.packet_size / tlm_data_size);
						break;
					default:
						large_driver->analyze.packet_counter = (large_driver->analyze.packet_size / tlm_data_size) + 1;//切り上げ
						large_driver->analyze.pre_packet_counter = (large_driver->analyze.packet_size / tlm_data_size) + 1;//切り上げ
						break;
				}
			break;
		}
		printf("large_driver->analyze.pre_packet_counter is %d \r\n", large_driver->analyze.pre_packet_counter);
		printf("large_driver->analyze.per_packet_size is %d \r\n", large_driver->analyze.per_packet_size);
		large_packet_set(&large_driver->analyze.function_flag);//exe関数を通過するか
		large_packet_set(&large_driver->analyze.large_packet_flag);//TXPICに送るhandler
	}
}

//このアプリを呼び出すとlarge_packet_analyzeで登録されたデータを自動でanalyzeする
//使うときはTask Listに入れておく
AppInfo COM_large_packet_Analyzer(void){
	return AI_create_app_info("large_packet", large_packet_init, large_packet_exe);
}

void large_packet_init(void){
	header_init();
	analyzer_init();
	large_driver->analyze.analyze_cycle = 3;
	large_packet_reset(&large_driver->analyze.re_send_flag);
	if(large_driver->analyze.per_packet_size < TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN|| large_driver->analyze.per_packet_size > TCP_MAX_LEN){//まずい条件は256で固定する
		large_driver->analyze.per_packet_size = TCP_MAX_LEN;
	}
}

void large_packet_exe(void){
	if(large_driver->analyze.function_flag == 1 || large_driver->analyze.re_send_flag != 0){
		print(LARGE_PACKET,"large_packet_exe\r\n");
		switch(large_driver->tlm.packet_identifer){
			case COBC_MCAM_TLM:
			case COBC_SCAM_TLM:
			case Tlm_CODE_SUBCAM:
				picture_tlm_analyze();
				break;
			default:
				default_tlm_analyze();
				break;
		}
	}
}

void default_tlm_analyze(void){
	int per_data_size = large_driver->analyze.per_packet_size - large_driver->analyze.header_length;//初期化
	int size = 0;//初期化
	int count;

	for(int i = 0; i < large_driver->analyze.analyze_cycle; i++){//連続してパケット生成
		if(large_driver->analyze.packet_counter > 1){//長さ固定
			size = large_driver->analyze.per_packet_size - large_driver->analyze.header_length;
		}
		else if(large_driver->analyze.packet_counter == 1){//最後のpacketは長さを合わせる
			size = (large_driver->analyze.packet_size - (large_driver->analyze.per_packet_size - large_driver->analyze.header_length)
					* (large_driver->analyze.pre_packet_counter - large_driver->analyze.packet_counter));
		}
		uint8_t large_packet[size];

		memset(large_packet,'\0',size);
		count = large_driver->analyze.pre_packet_counter - large_driver->analyze.packet_counter;
		if(!PL_is_full(&PH_rt_tlm_list) &&
				PL_count_inactive_nodes(&PH_txpic_tlm_list) > large_driver->analyze.analyze_cycle){//msテレメトリが満タンでなければ入れる
			for(int j = 0; j < size; j++){
				large_packet[j] = large_driver->large_packet[j + per_data_size * count];
			}

			set_packet_header(large_packet, size);

			-- large_driver->analyze.packet_counter;

			if(large_driver->analyze.packet_counter <= 0){
				large_packet_reset(&large_driver->analyze.function_flag);
				break;
			}
		}
	}
}

void picture_tlm_analyze(void){//ユーザーデータの先頭2byteにIDを付加する
	int per_data_size = large_driver->analyze.per_packet_size - large_driver->analyze.header_length - ID_SIZE;//初期化
	int size = 0;//初期化
	int count;

	for(int i = 0; i < large_driver->analyze.analyze_cycle; i++){//連続してパケット生成
		if(large_driver->analyze.packet_counter > 1){//長さ固定
			size = large_driver->analyze.per_packet_size - large_driver->analyze.header_length - ID_SIZE;
		}
		else if(large_driver->analyze.packet_counter == 1){//最後のpacketは長さを合わせる
			size = (large_driver->analyze.packet_size - (large_driver->analyze.per_packet_size - large_driver->analyze.header_length - ID_SIZE)
					* (large_driver->analyze.pre_packet_counter - large_driver->analyze.packet_counter));
		}
		uint8_t large_packet[size + ID_SIZE];
		memset(large_packet,'\0',size + ID_SIZE);
		count = large_driver->analyze.pre_packet_counter - large_driver->analyze.packet_counter;
		if(!PL_is_full(&PH_rt_tlm_list) &&
				PL_count_inactive_nodes(&PH_txpic_tlm_list) > large_driver->analyze.analyze_cycle){//msテレメトリが満タンでなければ入れる
			switch(large_driver->tlm.packet_identifer){
				case COBC_MCAM_TLM:
				case COBC_SCAM_TLM:
					#ifdef TWO_U
					memcpy(&large_packet[0], &cobc_driver->large_packet.picture_id, 2);
					#endif
					break;
				case Tlm_CODE_SUBCAM:
					#ifdef ONE_U
					memcpy(&large_packet[0], &txpic_driver->picture_id, 2);
					#endif
					break;
				default:
					break;
			}
			for(int j = 0; j < size; j++){
				large_packet[j + ID_SIZE] = large_driver->large_packet[j + per_data_size * count];
			}

			set_packet_header(large_packet, size + ID_SIZE);

			-- large_driver->analyze.packet_counter;

//			printf("large_driver->analyze.packet_counter is %d\r\n",large_driver->analyze.packet_counter);

			if(large_driver->analyze.packet_counter <= 0){
				large_packet_reset(&large_driver->analyze.function_flag);
				break;
			}
		}
	}
}

void set_packet_header(uint8_t* packet, size_t size){
	for(int i = 0; i < size; i++){
		large_driver->tcp.packet[i + large_driver->analyze.header_length] = packet[i];
	}
	//set common header
	TCP_set_ver(&large_driver->tcp, large_driver->common.ver);
    TCP_set_type(&large_driver->tcp, large_driver->common.type);    //1
    TCP_set_2nd_hdr_flag(&large_driver->tcp, large_driver->common.hdr_flag); //1
    TCP_set_apid_project_id(&large_driver->tcp, large_driver->common.project_id);
    TCP_set_apid_sat_id(&large_driver->tcp, large_driver->common.sat_id);
    TCP_set_apid_executer_id(&large_driver->tcp, large_driver->common.exe_id);

    //set TLM header
	if(large_driver->common.type == TCP_TYPE_TLM){
	    TCP_set_packet_data_len(&large_driver->tcp, size + TCP_TLM_2ND_HDR_LEN);
	    TCP_set_route(&large_driver->tcp, large_driver->common.route);
#ifdef TWO_U
		TCP_set_all_issuer_id(&large_driver->tcp, TCP_SAT_ID_2U, TCP_EXE_ID_MOBC);
#endif
#ifdef ONE_U
		TCP_set_all_issuer_id(&large_driver->tcp, TCP_SAT_ID_1U, TCP_EXE_ID_MOBC);
#endif
		TCP_TLM_set_ti(&large_driver->tcp, (uint32_t)(TMGR_get_master_total_cycle()));
		TCP_TLM_set_category(&large_driver->tcp, large_driver->tlm.category);//ms tlm
		TCP_TLM_set_packet_id(&large_driver->tcp, large_driver->tlm.packet_identifer);
	}

	//set cmd header
	if(large_driver->common.type == TCP_TYPE_CMD){
	    TCP_set_route(&large_driver->tcp, large_driver->common.route);
#ifdef TWO_U
		TCP_set_all_issuer_id(&large_driver->tcp, TCP_SAT_ID_2U, TCP_EXE_ID_MOBC);
#endif
#ifdef ONE_U
		TCP_set_all_issuer_id(&large_driver->tcp, TCP_SAT_ID_1U, TCP_EXE_ID_MOBC);
#endif
	    TCP_CMD_set_exec_type(&large_driver->tcp, large_driver->cmd.cmd_exe_type);   //0x03
	    TCP_CMD_set_ti(&large_driver->tcp, (uint32_t)large_driver->cmd.time);
	}

	if(large_driver->analyze.re_send_flag == 0){
		//set seq
		if(large_driver->analyze.packet_counter == large_driver->analyze.pre_packet_counter){
			TCP_set_seq_flag(&large_driver->tcp, TCP_SEQ_FIRST);
			TCP_set_seq_cnt(&large_driver->tcp, 0);
		}
		else if(large_driver->analyze.packet_counter == 1){
			TCP_set_seq_flag(&large_driver->tcp, TCP_SEQ_LAST);
			TCP_set_seq_cnt(&large_driver->tcp, large_driver->analyze.pre_packet_counter - 1);
			printf("last 1\r\n");
			printf("size is %d \r\n", size);
		}
		else{
			TCP_set_seq_flag(&large_driver->tcp, TCP_SEQ_CONT);
			TCP_set_seq_cnt(&large_driver->tcp, large_driver->analyze.pre_packet_counter
					- large_driver->analyze.packet_counter);
		}
	}
	else if(large_driver->analyze.re_send_flag == 1){
		TCP_set_seq_flag(&large_driver->tcp, TCP_SEQ_CONT);
		TCP_set_seq_cnt(&large_driver->tcp, large_driver->analyze.seq_count);
		++large_driver->analyze.seq_count;
	}
	else if(large_driver->analyze.re_send_flag == 2){
		TCP_set_seq_flag(&large_driver->tcp, TCP_SEQ_CONT);
		TCP_set_seq_cnt(&large_driver->tcp, large_driver->analyze.seq_count);
		++large_driver->analyze.seq_count;
		large_packet_reset(&large_driver->analyze.function_flag);
		large_driver->analyze.resend_last_one = LARGE_PACKET_FLAG_ON;
		printf("seq last\r\n");
	}

	if(large_driver->analyze.packet_counter > 1){//長さを256に固定
		for(int i = 0; i <256; i++)
		{
		  print(LARGE_PACKET, "%02x ", large_driver->tcp.packet[i]);
		}
		print(LARGE_PACKET,"\r\n");
	}
	PH_analyze_packet(&(large_driver->tcp));
}

void analyzer_init(void){
	large_driver->analyze.packet_counter = 0x00;
	large_driver->analyze.pre_packet_counter = 0x00;
	large_driver->analyze.send_counter = 0x00;
	large_driver->analyze.header_length = TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN;
	large_packet_reset(&large_driver->analyze.function_flag);
	large_packet_reset(&large_driver->analyze.large_packet_flag);
	large_driver->analyze.resend_last_one = LARGE_PACKET_FLAG_OFF;
}

void header_init(void){
	header_common_init();
	header_tlm_init();
	header_cmd_init();
}

void header_common_init(void){
	//common
	large_driver->common.ver = (TCP_VER)TCP_VER_2;
	large_driver->common.type = (TCP_TYPE)TCP_TYPE_TLM;
	large_driver->common.hdr_flag = (TCP_2ND_HDR_FLAG)TCP_2ND_HDR_PRESENT;
	large_driver->common.project_id = (TCP_PROJECT_ID)MAGNAROII;
	large_driver->common.route = (TCP_ROUTE)TCP_ROUTE_DIRECT;
#ifdef ONE_U
	large_driver->common.sat_id = (TCP_SAT_ID)TCP_SAT_ID_1U;
#endif
#ifdef TWO_U
	large_driver->common.sat_id = (TCP_SAT_ID)TCP_SAT_ID_2U;
#endif
	large_driver->common.exe_id = (TCP_EXE_ID)TCP_EXE_ID_AFSK;
	large_driver->common.seq_flag = (TCP_SEQ_FLAG)TCP_SEQ_SINGLE;
}

void header_tlm_init(void){
	large_driver->tlm.category = 0x40;
	large_driver->tlm.packet_identifer = Tlm_CODE_HK;
#ifdef ONE_U
	large_driver->tlm.tlm_issuer_sat_id = TCP_SAT_ID_1U;
#endif
#ifdef TWO_U
	large_driver->tlm.tlm_issuer_sat_id = TCP_SAT_ID_2U;
#endif
	large_driver->tlm.tlm_issuer_id = (TCP_EXE_ID)TCP_EXE_ID_MOBC;
}

void header_cmd_init(void){
//	large_driver->cmd.cmd_type= (TCP_CMD_TYPE)TCP_CMD_TYPE_SM;
	large_driver->cmd.cmd_exe_type= (TCP_CMD_EXEC_TYPE)TCP_CMD_EXEC_TYPE_RT;
	large_driver->cmd.time= 0x00000000;
}

void large_packet_set(uint8_t* flag){
	*flag = LARGE_PACKET_FLAG_ON;
}

void large_packet_reset(uint8_t* flag){
	*flag = LARGE_PACKET_FLAG_OFF;
}
