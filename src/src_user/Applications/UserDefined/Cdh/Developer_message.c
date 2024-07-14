/*
 * Developer_message.c
 *
 *  Created on: 2022/08/16
 *      Author: daiki
 */

#include "Developer_message.h"

#include "string.h"
#include <src_core/Library/endian.h>
#include <src_user/Drivers/CDH/SD/SD.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

#define MSG_STRUCT_SIZE sizeof(MSG_STRUCT)
#define TXPIC_DEVELOP_ID (0xc11c)
static uint32_t Develop_block = 0x01902D00;


CCP_CmdRet Cmd_CDH_develop_register_msg(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t ret = 0;
	uint16_t msg_num;
	MSG_STRUCT msg;

	msg.len = CCP_get_param_len(packet) - 2;
	if(msg.len > 63) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

	ENDIAN_memcpy(&msg_num, param, 2);
	memcpy(msg.msg, param + 2, msg.len);

	ret = sd_write_address(Develop_block, msg_num * MSG_STRUCT_SIZE, &msg, MSG_STRUCT_SIZE);

	if(ret != 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_CDH_develop_msg_send_txpic(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t ret = 0;
	uint16_t param_len;
	uint16_t msg_num;
	uint8_t downlink_flag;
	MSG_STRUCT msg;
	TCP tcp;

	ENDIAN_memcpy(&msg_num, param, 2);
	memcpy(&downlink_flag, param + 2, 1);

	ret = sd_read_address(Develop_block, msg_num * MSG_STRUCT_SIZE, &msg, MSG_STRUCT_SIZE);
	if(ret != 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
	if(msg.len > 63) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

	param_len = msg.len + 2 * sizeof(uint8_t);
	uint8_t cmd_param[param_len];
	memcpy(cmd_param, &downlink_flag, 1);
	memcpy(cmd_param + 1, &msg, param_len - 1);
#ifdef TWO_U
	CCP_form_rtc_to_other_obc(&tcp, TCP_APID_TXPIC_2U, (CMD_CODE)TXPIC_DEVELOP_ID,cmd_param,param_len);
#endif
#ifdef ONE_U
	CCP_form_rtc_to_other_obc(&tcp, TCP_APID_TXPIC_1U, (CMD_CODE)TXPIC_DEVELOP_ID,cmd_param,param_len);
#endif

	PH_analyze_packet(&tcp);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
