/*
 * packet_spliter.h
 *
 *  Created on: 2020/08/20
 *      Author: sakaguchi
 */

#ifndef PACKET_SPLITTER_H_
#define PACKET_SPLITTER_H_

#include <stdint.h> //for uint8_t
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/TlmCmd/packet_handler.h>

typedef struct{
	struct{
		TCP packet1;
		TCP packet2;
		TCP packet3;
		TCP packet4;
	}splitted;//80に分けるにしても140に分けるにしても同じ構造体から使う
	uint16_t last_len;
	uint8_t packet_num;//80に分けたときにいくつに分解したか
}PACKET_SPLIT_STRUCT;

extern PACKET_SPLIT_STRUCT* const split_driver;

#ifdef __cplusplus
 extern "C" {
#endif




#ifdef __cplusplus
}
#endif
/************************************** END OF FILE **************************************/


#endif /* SAMPLEDRIVER_H_ */
