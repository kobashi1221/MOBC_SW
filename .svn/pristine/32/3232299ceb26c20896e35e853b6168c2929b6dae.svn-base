/**
  *****************************************************************************************
  * @file    : commnad_response_handler.c
  * @author  : Yuta Sakaguchi
  * @version : 1.0
  * @date    : 2022/06/06
  * @brief   :　コマンド応答
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2022/06/06  1.0   Y.Sakaguchi    First revision
  *
  *****************************************************************************************
  */
#include "command_response_handler.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <string.h>//for memcpy

static GS_RESPONS_STRUCT gs_response_struct;
GS_RESPONS_STRUCT* const p_gs_response_struct=&gs_response_struct;

void command_response_handler(const TCP* packet){
	if(gs_command_checker(packet) == _SUCCESS){
		if(CCP_get_id(packet) == Cmd_CODE_CDH_PROGRAM_TO_SD){//リプロコマンドはシーケンスカウントの位置が異なる
			const uint8_t* param = TCP_CMD_get_param_head(packet);
			ENDIAN_memcpy(&p_gs_response_struct->res_seq_count, param ,2);
		}
		else{
			p_gs_response_struct->res_seq_count = TCP_get_seq_cnt(packet);
		}
		p_gs_response_struct->res_packet_id = TCP_CMD_get_res_packet_id(packet);
		p_gs_response_struct->res_apid = MAGNAROII << 8 | TCP_CMD_get_res_apid_(packet);
		p_gs_response_struct->res_sat_id = TCP_CMD_get_res_apid_sat_id(packet);
		p_gs_response_struct->res_exe_id = TCP_CMD_get_res_apid_executer_id(packet);
		p_gs_response_struct->res_route = TCP_CMD_get_res_route(packet);

		p_gs_response_struct->res_flag = GSC_RES;
	}
	else if(tl_command_checker(packet) == _SUCCESS){
		if(CCP_get_id(packet) == Cmd_CODE_CDH_PROGRAM_TO_SD){//リプロコマンドはシーケンスカウントの位置が異なる
			const uint8_t* param = TCP_CMD_get_param_head(packet);
			ENDIAN_memcpy(&p_gs_response_struct->res_seq_count, param ,2);
		}
		else{
			p_gs_response_struct->res_seq_count = TCP_get_seq_cnt(packet);
		}
		p_gs_response_struct->res_packet_id = TCP_CMD_get_res_packet_id(packet);
		p_gs_response_struct->res_apid = MAGNAROII << 8 | TCP_CMD_get_res_apid_(packet);
		p_gs_response_struct->res_sat_id = TCP_CMD_get_res_apid_sat_id(packet);
		p_gs_response_struct->res_exe_id = TCP_CMD_get_res_apid_executer_id(packet);
		p_gs_response_struct->res_route = TCP_CMD_get_res_route(packet);

		p_gs_response_struct->tlc_register_id = CCP_get_id(packet);
		p_gs_response_struct->tlc_register_time = CCP_get_ti(packet);

		p_gs_response_struct->res_flag = TLC_RES;
	}
	else{
		//GSからのTLC or GSCでないと何もしない
	}
}

_ACK gs_command_checker(const TCP* packet){
	if(CTCP_get_packet_type(packet) != CTCP_PACKET_TYPE_CMD){//そもそもコマンドじゃない
		return _UNKNOWN;
	}
	else{
		switch(CCP_get_exec_type(packet)){
		//TLCなら_SUCCESSをreturn
		case CCP_EXEC_TYPE_GS:
			return _SUCCESS;
		default:
			return _UNKNOWN;
		}
	}
	return _UNKNOWN;
}

_ACK tl_command_checker(const TCP* packet){
	if(CTCP_get_packet_type(packet) != CTCP_PACKET_TYPE_CMD){//そもそもコマンドじゃない
		return _UNKNOWN;
	}
	else{
		switch(CCP_get_exec_type(packet)){
		//TLCなら_SUCCESSをreturn
		case CCP_EXEC_TYPE_TL_FROM_GS:
			return _SUCCESS;
		default:
			return _UNKNOWN;
		}
	}
	return _UNKNOWN;
}


