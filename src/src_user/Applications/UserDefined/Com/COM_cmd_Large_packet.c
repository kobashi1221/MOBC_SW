/*
 * COM_cmd_Large_packet.c
 *
 *  Created on: 2022/06/29
 *      Author: sakaguchi
 */

#include "COM_cmd_Large_packet.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <string.h>//for memcpy
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Drivers/large_packet_driver.h>
#include <src_user/Library/General/DBG_printf.h>

static void large_packet_param_load(void);
static void large_packet_param_init(void);

const TCP_ROUTE route_ = TCP_ROUTE_DIRECT;
#ifdef TWO_U
const TCP_SAT_ID sat_id_ = TCP_SAT_ID_2U;
#endif
#ifdef ONE_U
const TCP_SAT_ID sat_id_ = TCP_SAT_ID_1U;
#endif
const TCP_EXE_ID exe_id = TCP_EXE_ID_AFSK;
const uint8_t analyze_cycle_ = 8;
const uint8_t send_cycle_ = 2;
const uint16_t per_packet_size_ = 256;

AppInfo COM_large_packet_param(void){
	return AI_create_app_info("large_packet", large_packet_param_load, large_packet_param_init);
}

static void large_packet_param_load(void){
	read_param(&large_driver->common.route, &route_, 1,(uint32_t)0x0000300f);
	read_param(&large_driver->common.sat_id, &sat_id_, 1,(uint32_t)0x00003010);
	read_param(&large_driver->common.exe_id, &exe_id, 1,(uint32_t)0x00003011);
	read_param(&large_driver->analyze.analyze_cycle, &analyze_cycle_, 1,(uint32_t)0x00003012);
	read_param(&large_driver->analyze.send_cycle, &send_cycle_, 1,(uint32_t)0x00003013);
	read_param(&large_driver->analyze.per_packet_size, &per_packet_size_, 2,(uint32_t)0x00003014);
}

static void large_packet_param_init(void){
	write_param(&large_driver->common.route, &route_, 1,(uint32_t)0x0000300f);
	write_param(&large_driver->common.sat_id, &sat_id_, 1,(uint32_t)0x00003010);
	write_param(&large_driver->common.exe_id, &exe_id, 1,(uint32_t)0x00003011);
	write_param(&large_driver->analyze.analyze_cycle, &analyze_cycle_, 1,(uint32_t)0x00003012);
	write_param(&large_driver->analyze.send_cycle, &send_cycle_, 1,(uint32_t)0x00003013);
	write_param(&large_driver->analyze.per_packet_size, &per_packet_size_, 2,(uint32_t)0x00003014);
}


/**
 * @brief	change common header.
 * packet[15]:  route
 * packet[16]:  sat id(apid)
 * packet[17]:	exe id(apid)
 *
 */
CCP_CmdRet Cmd_change_large_packet_common_header(const CommonCmdPacket* packet){
	size_t bufsize = CCP_get_param_len(packet);
	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t data[bufsize];
	memcpy(&data, param,bufsize);

	large_driver->common.route = (TCP_ROUTE)param[0];
	large_driver->common.sat_id = (TCP_SAT_ID)param[1];
	large_driver->common.exe_id = (TCP_EXE_ID)param[2];

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief	change tlm header.
 * packet[15]: 	カテゴリ
 * packet[16]:  issuer sat id
 * packet[17]:  issuer exe id
 *
 */
CCP_CmdRet Cmd_change_large_packet_tlm_header(const CommonCmdPacket* packet){
	size_t bufsize = CCP_get_param_len(packet);
	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t data[bufsize];
	memcpy(&data, param,bufsize);

	large_driver->tlm.category = data[0];
	large_driver->tlm.tlm_issuer_sat_id = (TCP_SAT_ID)data[1];
	large_driver->tlm.tlm_issuer_id = (TCP_EXE_ID)data[2];

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief	change cmd header.
 * packet[15]: 	cmd type
 * packet[16]:  exec type
 * packet[17] ~ [20]:  time
 *
 */
CCP_CmdRet Cmd_change_large_packet_cmd_header(const CommonCmdPacket* packet){
	size_t bufsize = CCP_get_param_len(packet);
	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t data[bufsize];
	memcpy(&data, param,bufsize);

//	large_driver->cmd.cmd_type = (TCP_CMD_TYPE)data[0];
	large_driver->cmd.cmd_exe_type = (TCP_CMD_EXEC_TYPE)data[1];
	ENDIAN_memcpy(&large_driver->cmd.time, &data[2], 4);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief	大容量通信の関数のenable of disables
 * packet[15]: 	enable or disable
 *
 */
CCP_CmdRet Cmd_large_packet_enable(const CommonCmdPacket* packet){
	size_t bufsize = CCP_get_param_len(packet);
	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t data[bufsize];
	memcpy(&data, param,bufsize);

	large_driver->analyze.function_flag = param[0];

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief	外部RAMのアドレスを指定して, large_packet_anlyzeする
 * packet[15]~[18]:	address
 * packet[19]~[23]: size(byte)
 * packet[24]: tlm_id
 *
*/
CCP_CmdRet Cmd_large_packet_SRAM_analyze(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);
	uint32_t address;
	size_t analyze_size;
	TLM_CODE id;

	ENDIAN_memcpy(&address, &param[0], 4);
	ENDIAN_memcpy(&analyze_size, &param[4], 4);
	memcpy(&id, &param[8], 1);

	if(address < 0x60000000){
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	printf("analyze_large_packet_init \r\n");

	uint8_t* analyze_data = (uint8_t*)address;

	large_packet_TLM_analyze(analyze_data, analyze_size, id);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);

}
