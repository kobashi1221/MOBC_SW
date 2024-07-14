/*
 * packet_spliter.h
 *
 *  Created on: 2021/07/07
 *      Author: sakaguchi
 */

#include "packet_splitter.h"
#include <string.h> // for memcpy

static PACKET_SPLIT_STRUCT split_driver_;
PACKET_SPLIT_STRUCT* const split_driver = &split_driver_;


void splitted_by_140(const TCP* packet){//140にパケットを分割する
	int len =TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN ;
	if(TCP_get_type(packet) == TCP_TYPE_CMD){//コマンドをダウンリンクすることはないはず⇒何もしない
		return;
	}
	if(len > 140){
		TCP_copy_packet(&split_driver->splitted.packet1, packet);//一旦コピー
		TCP_set_seq_flag(&split_driver->splitted.packet1, TCP_SEQ_FIRST);
		TCP_set_seq_cnt(&split_driver->splitted.packet1, 0x00);
		TCP_set_packet_len(&split_driver->splitted.packet1, 140 - TCP_PRM_HDR_LEN);

		uint16_t rest_len = len - 140;
		TCP_copy_packet(&split_driver->splitted.packet2, packet);//一旦コピー
		memcpy(&split_driver->splitted.packet2.packet[TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN],
				&packet->packet[140], rest_len);//残りのTLM部分を入れる
		TCP_set_seq_flag(&split_driver->splitted.packet2, TCP_SEQ_LAST);
		TCP_set_seq_cnt(&split_driver->splitted.packet2, 0x01);
		TCP_set_packet_len(&split_driver->splitted.packet2, rest_len + TCP_TLM_2ND_HDR_LEN);
		split_driver->last_len = rest_len + TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN;
	}
}

void splitted_by_80(const TCP* packet){//140にパケットを分割する
	int len =TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN ;
	int next_user_data_len;
	if(TCP_get_type(packet) == TCP_TYPE_CMD){//コマンドをダウンリンクすることはないはず⇒何もしない
		return;
	}
	if(len > 80){
		TCP_copy_packet(&split_driver->splitted.packet1, packet);//一旦コピー
		TCP_set_seq_flag(&split_driver->splitted.packet1, TCP_SEQ_FIRST);
		TCP_set_seq_cnt(&split_driver->splitted.packet1, 0x00);
		TCP_set_packet_len(&split_driver->splitted.packet1, 80 - TCP_PRM_HDR_LEN);

		uint16_t rest_len = len - 80;
		if(rest_len > 80){
			next_user_data_len = 80 - TCP_PRM_HDR_LEN - TCP_TLM_2ND_HDR_LEN;
		}
		else{
			next_user_data_len = rest_len;
		}
		TCP_copy_packet(&split_driver->splitted.packet2, packet);//一旦コピー
		memcpy(&split_driver->splitted.packet2.packet[TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN],
				&packet->packet[80], rest_len);//残りのTLM部分を入れる
		TCP_set_seq_flag(&split_driver->splitted.packet2, TCP_SEQ_CONT);
		TCP_set_seq_cnt(&split_driver->splitted.packet2, 0x01);
		TCP_set_packet_len(&split_driver->splitted.packet2, next_user_data_len + TCP_TLM_2ND_HDR_LEN);

		if(len < 0){
			split_driver->last_len = rest_len + TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN;
			split_driver->packet_num = 2;
			return;
		}

		rest_len = rest_len - 80;
		if(rest_len > 80){
			next_user_data_len = 80 - TCP_PRM_HDR_LEN - TCP_TLM_2ND_HDR_LEN;
		}
		else{
			next_user_data_len = rest_len;
		}
		TCP_copy_packet(&split_driver->splitted.packet3, packet);//一旦コピー
		memcpy(&split_driver->splitted.packet3.packet[TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN],
				&packet->packet[160], next_user_data_len);//残りのTLM部分を入れる
		TCP_set_seq_flag(&split_driver->splitted.packet3, TCP_SEQ_CONT);
		TCP_set_seq_cnt(&split_driver->splitted.packet3, 0x02);
		TCP_set_packet_len(&split_driver->splitted.packet3, next_user_data_len + TCP_TLM_2ND_HDR_LEN);

		if(len < 0){
			split_driver->last_len = rest_len + TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN;
			split_driver->packet_num = 3;
			return;
		}

		rest_len = rest_len - 80;
		TCP_copy_packet(&split_driver->splitted.packet4, packet);//一旦コピー
		memcpy(&split_driver->splitted.packet4.packet[TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN],
				&packet->packet[240], rest_len);//残りのTLM部分を入れる
		TCP_set_seq_flag(&split_driver->splitted.packet4, TCP_SEQ_LAST);
		TCP_set_seq_cnt(&split_driver->splitted.packet4, 0x03);
		TCP_set_packet_len(&split_driver->splitted.packet4, rest_len + TCP_TLM_2ND_HDR_LEN);

		split_driver->last_len = rest_len + TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN;
		split_driver->packet_num = 4;
	}
}
