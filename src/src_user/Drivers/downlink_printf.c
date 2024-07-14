/*
 * downlink_printf.c
 *
 *  Created on: 2022/02/22
 *      Author: daiki
 */

#include "downlink_printf.h"
#include <string.h>

/*
 * あるデータをダウンリンクさせるときに利用する関数
 * packet: cmdのpacket
 * apid
 * data: ダウンリンクしたいデータの先頭ポインタ
 * len: ダウンリンクするデータ長
 * TLM_CODE
 */
PH_ACK DL_printf(const TCP* packet, uint16_t apid, uint8_t* data, uint16_t len, TLM_CODE tlm_code){
	TCP tcp;
	uint16_t cmd_id;
	uint8_t* cmd_data;
	uint16_t cmd_data_len;
	uint16_t tlm_len;
	PH_ACK ret;

	if (apid == 0xffff) return PH_ACK_SUCCESS;

	cmd_id = TCP_CMD_get_channel_id(packet);
	cmd_data = TCP_CMD_get_param_head(packet);
	cmd_data_len = TCP_get_packet_len(packet) - 9;

	tlm_len = 7 + 2 + cmd_data_len + len;

	TCP_TLM_setup_primary_hdr(&tcp, apid, tlm_len + TCP_PRM_HDR_LEN);
	TCP_TLM_set_ti(&tcp, (uint32_t)(TMGR_get_master_total_cycle()));
	TCP_TLM_set_category(&tcp, 0x40); //Mission Tlm
	TCP_TLM_set_packet_id(&tcp, tlm_code);
	TCP_TLM_set_telemetry_issuer_id(&tcp, MY_SAT_ID, TCP_EXE_ID_MOBC);

	ENDIAN_memcpy(TCP_TLM_get_user_data_head(&tcp), &cmd_id, 2);
	memcpy(TCP_TLM_get_user_data_head(&tcp) + 2, cmd_data, cmd_data_len);
	memcpy(TCP_TLM_get_user_data_head(&tcp) + 2 + cmd_data_len, data, len);

	ret = PH_analyze_packet(&tcp);

//	printf("packet is\r\n");
//	for(int i=0; i<tlm_len+6; i++){
//		printf("%02x ", tcp.packet[i]);
//	}
//	printf("\r\n");

	return ret;
}

/*
 * あるデータをログとして保存したいときに使用する関数
 * packet: cmdのpacket
 * apid
 * data: ダウンリンクしたいデータの先頭ポインタ
 * len: ダウンリンクするデータ長
 * TLM_CODE
 */
PH_ACK SAVE_printf_for_log(const TCP* packet, uint16_t apid, uint8_t* data, uint16_t len){
	TCP tcp;
	uint16_t cmd_id;
	uint8_t* cmd_data;
	uint16_t cmd_data_len;
	PH_ACK ret;

	if (apid == 0xffff) return PH_ACK_SUCCESS;

	cmd_id = TCP_CMD_get_channel_id(packet);
	cmd_data = TCP_CMD_get_param_head(packet);
	cmd_data_len = TCP_get_packet_len(packet) - 9;

	TCP_TLM_setup_primary_hdr(&tcp, apid, 250 + TCP_PRM_HDR_LEN);
	TCP_TLM_set_ti(&tcp, (uint32_t)(TMGR_get_master_total_cycle()));
	TCP_TLM_set_category(&tcp, 0x80); //ST Tlm
	TCP_TLM_set_packet_id(&tcp, Tlm_CODE_LOG);
	TCP_TLM_set_telemetry_issuer_id(&tcp, MY_SAT_ID, TCP_EXE_ID_MOBC);

	ENDIAN_memcpy(TCP_TLM_get_user_data_head(&tcp), &cmd_id, 2);
	memcpy(TCP_TLM_get_user_data_head(&tcp) + 2, cmd_data, cmd_data_len);
	memcpy(TCP_TLM_get_user_data_head(&tcp) + 2 + cmd_data_len, data, len);

	ret = PH_analyze_packet(&tcp);

//	printf("packet is\r\n");
//	for(int i=0; i<256; i++){
//		printf("%02x ", tcp.packet[i]);
//	}
//	printf("\r\n");

	return ret;
}
