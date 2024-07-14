/*
 * DBG_printf.c
 *
 *  Created on: 2021/10/29
 *      Author: scath
 */
#define DBG
//#include "debug_printf.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

static uint8_t dbg_mode[8] = {0, 0, 0, 0, 0, 0, 0, 0x80};
//static uint64_t dbg_mode = 0x0000000000004000;//only SD
static void DBG_Mode_Set(DBG_MODE_LIST mode);
static void DBG_Mode_Reset(DBG_MODE_LIST mode);

CCP_CmdRet Cmd_DBG_SELECT_printf(const CommonCmdPacket* packet){//format
	int mode = *CCP_get_param_head(packet);
	if(mode == 0){
		for(int i = 0; i < 9;i++){
			dbg_mode[i] = 0;
		}
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DBG_Set_Print(const CommonCmdPacket* packet){//format
	uint8_t mode = CCP_get_param_head(packet)[0];
	DBG_Mode_Set((DBG_MODE_LIST)mode);
	for(int i = 0; i < 64; i++){
		print((DBG_MODE_LIST)i, "enabled: %d\r\n", i);
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DBG_Reset_Print(const CommonCmdPacket* packet){
	uint8_t mode = CCP_get_param_head(packet)[0];
	DBG_Mode_Reset((DBG_MODE_LIST)mode);

	for(uint64_t i = 0; i < 64; i++){
		print((DBG_MODE_LIST)i, "disabled: %d\r\n", i);
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}




uint8_t DBG_Mode_Analyze(DBG_MODE_LIST mode){
	uint8_t ret = 0;
	uint8_t byte = mode/8;
	uint8_t rest = mode%8;
	ret = (dbg_mode[byte] >> rest)&0x01;

	return ret; //1: printf enabled, 0: printf disabled
}

static void DBG_Mode_Set(DBG_MODE_LIST mode){
	uint8_t byte = mode/8;
	uint8_t rest = mode%8;
	dbg_mode[byte] |= (0x01 << rest);
	return;
}

static void DBG_Mode_Reset(DBG_MODE_LIST mode){
	uint8_t byte = mode/8;
	uint8_t rest = mode%8;
	uint64_t temp = 0xff^(0x01 << rest);
	dbg_mode[byte] &= temp;
	return;
}


void print(DBG_MODE_LIST mode,const char* format, ...){
#ifdef DBG
	if(DBG_Mode_Analyze(mode)){
		char str[256];
		va_list ap;
		va_start(ap, format);
		vsprintf(str, format, ap);
		printf("%s", str);
		va_end(ap);
	}
#endif
}

//dbg_printf(AOCS, MAGS:%d, mags);


