/*
 * CDH_cmd_c2a_repro.c
 *
 *  Created on: 2021/12/22
 *      Author: SAT37
 */
#include "CDH_cmd_c2a_repro.h"

#include <stdio.h>
#include <src_core/Library/endian.h>
#include <src_core/TlmCmd/command_analyze.h>
#include <src_core/TlmCmd/telemetry_frame.h>
#include <src_core/System/ApplicationManager/app_manager.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Settings/TlmCmd/command_analyze_params.h>
#include <src_user/Settings/TlmCmd/telemetry_frame_params.h>
#include <src_user/Settings/System/app_manager_params.h>
#include <src_user/Drivers/Cdh/SD/SD.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>

static uint32_t VMA_Size;	// size of virtual memory area to dispatch CMD,TLM, and APP functions
const uint32_t VMA_Size_ = 0;

static uint32_t VMA_Address;	// size of virtual memory area to dispatch CMD,TLM, and APP functions
const uint32_t VMA_Address_ = 0x60000000;

static uint32_t VMA_Block = 0x018FA100;

static uint8_t Read_cmd_table_repro(void);
static uint8_t Write_cmd_table_repro(void);
static uint8_t Read_am_ais_repro(void);
static uint8_t Write_am_ais_repro(void);
static uint8_t Read_tlm_table_repro(void);
static uint8_t Write_tlm_table_repro(void);
static uint8_t Read_function_data(void);

static void CDH_Function_Uplink_param_load(void);
static void CDH_Function_Uplink_param_init(void);

static void CDH_cmd_table_param_load(void);
static void CDH_cmd_table_param_init(void);

static void CDH_am_ais_param_load(void);
static void CDH_am_ais_param_init(void);

static void CDH_tlm_table_param_load(void);
static void CDH_tlm_table_param_init(void);

AppInfo CDH_FU_vma_update(void)
{
    return AI_create_app_info("c2a_repro", CDH_Function_Uplink_param_load, CDH_Function_Uplink_param_init);
}

AppInfo CDH_FU_cmd_table_update(void)
{
    return AI_create_app_info("c2a_repro", CDH_cmd_table_param_load, CDH_cmd_table_param_init);
}

AppInfo CDH_FU_am_ais_update(void)
{
    return AI_create_app_info("c2a_repro", CDH_am_ais_param_load, CDH_am_ais_param_init);
}

AppInfo CDH_FU_tlm_table_update(void)
{
    return AI_create_app_info("c2a_repro", CDH_tlm_table_param_load, CDH_tlm_table_param_init);
}

static void CDH_Function_Uplink_param_load(void){
	SD_read_param(&VMA_Size, &VMA_Size_, sizeof(VMA_Size_), 0x00001200);
	SD_read_param(&VMA_Address, &VMA_Address_, sizeof(VMA_Size_), 0x00001204);

	if(VMA_Size != 0 && pic_data->Repro == 0x0A){
		Read_function_data();
	}
}

static void CDH_Function_Uplink_param_init(void){
	SD_write_param(&VMA_Size, &VMA_Size_, sizeof(VMA_Size_), 0x00001200);
	SD_write_param(&VMA_Address, &VMA_Address_, sizeof(VMA_Size_), 0x00001204);
}

static void CDH_cmd_table_param_load(void){//377ms 721ms
	if(VMA_Size != 0 && pic_data->Repro == 0x0A){
		Read_cmd_table_repro();
	}
}

static void CDH_cmd_table_param_init(void){//922ms 1590ms
	Write_cmd_table_repro();
}

static void CDH_am_ais_param_load(void){//184ms 353ms
	if(VMA_Size != 0 && pic_data->Repro == 0x0A){
		Read_am_ais_repro();
	}
}

static void CDH_am_ais_param_init(void){//439ms 790ms
	Write_am_ais_repro();
}

static void CDH_tlm_table_param_load(void){//9ms 16ms
	if(VMA_Size != 0 && pic_data->Repro == 0x0A){
		Read_tlm_table_repro();
	}
}

static void CDH_tlm_table_param_init(void){//20ms 35ms
	Write_tlm_table_repro();
}

CCP_CmdRet Cmd_CDH_set_code_to_SD(const CommonCmdPacket* packet){
	uint16_t param_len = CCP_get_param_len(packet);
	uint8_t* param_head = (uint8_t*)CCP_get_param_head(packet);

	uint32_t addr;
	uint8_t ret;

	ENDIAN_memcpy(&addr, param_head, 4);
	printf("sd addres is %08x", addr);

	ret = sd_write_address(VMA_Block, addr, param_head + 4, param_len - 4);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}



static uint8_t Read_cmd_table_repro(void){
	uint8_t ret;

	uint32_t size = sizeof(CA_CmdInfo)*CA_MAX_CMDS;
	uint32_t block_num = size / SD_BLOCKSIZE;
	uint32_t rest_num = size % SD_BLOCKSIZE;

	if(block_num > 0){
		for(int i = 0; i < block_num; i++){
			ret += SD_read_param((void*)((uint8_t*)command_analyze->cmd_table + SD_BLOCKSIZE * i), NULL, SD_BLOCKSIZE, 0x000FF6B0 + SD_BLOCKSIZE * i);
			WDT_clear_count();
		}
	}

	if(rest_num > 0){
		ret += SD_read_param((void*)((uint8_t*)command_analyze->cmd_table + SD_BLOCKSIZE * block_num), NULL, rest_num, 0x000FF6B0 + SD_BLOCKSIZE * block_num);
	}
	return ret;
}

static uint8_t Write_cmd_table_repro(void){
	uint8_t ret;

	uint32_t size = sizeof(CA_CmdInfo)*CA_MAX_CMDS;
	uint32_t block_num = size / SD_BLOCKSIZE;
	uint32_t rest_num = size % SD_BLOCKSIZE;

	if(block_num > 0){
		for(int i = 0; i < block_num; i++){
			ret += SD_write_param((void*)((uint8_t*)command_analyze->cmd_table + SD_BLOCKSIZE * i), NULL, SD_BLOCKSIZE, 0x000FF6B0 + SD_BLOCKSIZE * i);
			WDT_clear_count();
		}
	}

	if(rest_num > 0){
		ret += SD_write_param((void*)((uint8_t*)command_analyze->cmd_table + SD_BLOCKSIZE * block_num), NULL, rest_num, 0x000FF6B0 + SD_BLOCKSIZE * block_num);
	}
	return ret;
}

static uint8_t Read_am_ais_repro(void){
	uint8_t ret;

	uint32_t size = sizeof(AppInfo)*AM_MAX_APPS;
	uint32_t block_num = size / SD_BLOCKSIZE;
	uint32_t rest_num = size % SD_BLOCKSIZE;

	if(block_num > 0){
		for(int i = 0; i < block_num; i++){
			ret += SD_read_param((void*)((uint8_t*)app_manager->ais + SD_BLOCKSIZE * i), NULL, SD_BLOCKSIZE, 0x00103670 + SD_BLOCKSIZE * i);
			WDT_clear_count();
		}
	}

	if(rest_num > 0){
		ret += SD_read_param((void*)((uint8_t*)app_manager->ais + SD_BLOCKSIZE * block_num), NULL, rest_num, 0x00103670 + SD_BLOCKSIZE * block_num);
	}
	return ret;
}

static uint8_t Write_am_ais_repro(void){
	uint8_t ret;

	uint32_t size = sizeof(AppInfo)*AM_MAX_APPS;
	uint32_t block_num = size / SD_BLOCKSIZE;
	uint32_t rest_num = size % SD_BLOCKSIZE;

	if(block_num > 0){
		for(int i = 0; i < block_num; i++){
			ret += SD_write_param((void*)((uint8_t*)app_manager->ais + SD_BLOCKSIZE * i), NULL, SD_BLOCKSIZE, 0x00103670 + SD_BLOCKSIZE * i);
			WDT_clear_count();
		}
	}

	if(rest_num > 0){
		ret += SD_write_param((void*)((uint8_t*)app_manager->ais + SD_BLOCKSIZE * block_num), NULL, rest_num, 0x00103670 + SD_BLOCKSIZE * block_num);
	}
	return ret;
}

static uint8_t Read_tlm_table_repro(void){
	uint8_t ret;

	uint32_t size = sizeof(TF_TlmInfo)*TF_MAX_TLMS;
	uint32_t block_num = size / SD_BLOCKSIZE;
	uint32_t rest_num = size % SD_BLOCKSIZE;

	if(block_num > 0){
		for(int i = 0; i < block_num; i++){
			ret += SD_read_param((void*)((uint8_t*)telemetry_frame->tlm_table + SD_BLOCKSIZE * i), NULL, SD_BLOCKSIZE, 0x0010E8E0 + SD_BLOCKSIZE * i);
			WDT_clear_count();
		}
	}

	if(rest_num > 0){
		ret += SD_read_param((void*)((uint8_t*)telemetry_frame->tlm_table + SD_BLOCKSIZE * block_num), NULL, rest_num, 0x0010E8E0 + SD_BLOCKSIZE * block_num);
	}
	return ret;
}

static uint8_t Write_tlm_table_repro(void){
	uint8_t ret;

	uint32_t size = sizeof(TF_TlmInfo)*TF_MAX_TLMS;
	uint32_t block_num = size / SD_BLOCKSIZE;
	uint32_t rest_num = size % SD_BLOCKSIZE;

	if(block_num > 0){
		for(int i = 0; i < block_num; i++){
			ret += SD_write_param((void*)((uint8_t*)telemetry_frame->tlm_table + SD_BLOCKSIZE * i), NULL, SD_BLOCKSIZE, 0x0010E8E0 + SD_BLOCKSIZE * i);
			WDT_clear_count();
		}
	}

	if(rest_num > 0){
		ret += SD_write_param((void*)((uint8_t*)telemetry_frame->tlm_table + SD_BLOCKSIZE * block_num), NULL, rest_num, 0x0010E8E0 + SD_BLOCKSIZE * block_num);
	}
	return ret;
}

static uint8_t Read_function_data(void){
	uint8_t ret;

	uint32_t block_num = VMA_Size / SD_BLOCKSIZE;
	uint32_t rest_num = VMA_Size % SD_BLOCKSIZE;
	uint8_t* p_data = (uint8_t*)VMA_Address;

	if(block_num > 0){
		for(int i = 0; i < block_num; i++){
			ret = sd_read_address(VMA_Block, SD_BLOCKSIZE * i, p_data + SD_BLOCKSIZE * i, SD_BLOCKSIZE);
			WDT_clear_count();
		}
	}

	if(rest_num > 0){
		ret = sd_read_address(VMA_Block, SD_BLOCKSIZE * block_num, p_data + SD_BLOCKSIZE * block_num, rest_num);
	}
	return ret;
}

