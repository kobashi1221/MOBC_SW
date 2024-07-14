/*
 * CDH_cmd_patch.c
 *
 *  Created on: 2022/04/15
 *      Author: SAT37
 */

#include "CDH_cmd_patch.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>


CCP_CmdRet Cmd_CDH_patch_seq_data(const CommonCmdPacket* packet){
	uint8_t* param;
	uint32_t block_adr;
	uint32_t adr;
	uint8_t data_len;
	uint8_t ret;

	param = CCP_get_param_head(packet);

	uint16_t seq_cnt;
	ENDIAN_memcpy(&seq_cnt, param, 2);

	ENDIAN_memcpy(&block_adr, param + 2, 4);
	ENDIAN_memcpy(&adr, param + 6, 4);

	if(CCP_get_param_len(packet) <= 10) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);

	data_len = (uint8_t)(CCP_get_param_len(packet) - 10);

	ret = sd_write_address(block_adr, adr, param + 10, data_len);

	//receive_flag書き込み
	uint8_t receive_frag = 0xff;
	ret = SD_write_param(&receive_frag, NULL, 1, (uint32_t)seq_cnt);
	if(ret != 0){
		printf("WRITE FLAG ERROR\r\n");
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_CDH_patch_scatter_data(const CommonCmdPacket* packet){
	uint8_t* param;
	uint32_t block_adr;
	uint16_t adr;
	uint8_t data_num;
	uint8_t read_data[512];
	uint8_t counter = 0;
	uint8_t ret;

	if(CCP_get_param_len(packet) <= 8) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);

	param = CCP_get_param_head(packet);

	uint16_t seq_cnt;
	ENDIAN_memcpy(&seq_cnt, param, 2);
	ENDIAN_memcpy(&block_adr, param + 2, 4);
	ENDIAN_memcpy(&adr, param + 6, 2);

	data_num = (uint8_t)((CCP_get_param_len(packet) - 9) / 2);

//	printf("block = %08x\r\n", block_adr);
//	printf("adr = %04x\r\n", adr);
//	printf("num = %d\r\n", data_num);

	ret = BSP_SD_ReadBlocks(read_data, block_adr, 1, 0xff);

	memcpy(&(read_data[adr]), param + 8, 1);

	while(data_num > counter){
		adr += *(param + 9 + 2 * counter);
		memcpy(&(read_data[adr]), param + 10 + 2 * counter, 1);
		counter++;
	}

	ret = BSP_SD_WriteBlocks(read_data, block_adr, 1, 0xff);

	//receive_flag書き込み
	uint8_t receive_frag = 0xff;
	ret = SD_write_param(&receive_frag, NULL, 1, (uint32_t)seq_cnt);
	if(ret != 0){
		printf("WRITE FLAG ERROR\r\n");
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
