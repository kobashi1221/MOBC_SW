/*
 * StandbySL.cpp
 *
 *  Created on: 2021/05/29
 *      Author: kahak
 *  Edited on: 2021/09/07
 *  	park
 */

#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>

#include "src_core/TlmCmd/block_command_table.h"
#include <src_core/TlmCmd/block_command_loader.h>

#include "src_user/TlmCmd/block_command_definitions.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/command_definitions.h"
#include "src_user/Settings/Modes/mode_definitions.h"


void BCL_load_standby_to_standby()
{
//	BCL_tool_prepare_param_int8(1);
//	BCL_tool_register_cmd(OBCT_sec2cycle(2), Cmd_CODE_PWR_DEBUG_IOEX_MOBC_FET);
//	BCL_tool_register_cmd(OBCT_sec2cycle(3), Cmd_CODE_PWR_DEBUG_IOEX_INIT);
//	BCL_tool_register_cmd(OBCT_sec2cycle(4), Cmd_CODE_PWR_DEBUG_ADC_INIT);
	BCL_tool_register_cmd(OBCT_sec2cycle(5), Cmd_CODE_MM_FINISH_TRANSITION);
}


void BCL_load_startup_to_standby(){
		BCL_tool_prepare_param_uint16(BC_TLM_LOW_RATE_1);
		BCL_tool_prepare_param_uint16(1);
		BCL_tool_register_cmd(OBCT_sec2cycle(2), Cmd_CODE_BCE_SET_ROTATE_INTERVAL);

		BCL_tool_prepare_param_uint16(BC_TLM_LOW_RATE_2);
		BCL_tool_prepare_param_uint16(1);
		BCL_tool_register_cmd(OBCT_sec2cycle(3), Cmd_CODE_BCE_SET_ROTATE_INTERVAL);

		BCL_tool_prepare_param_uint16(MD_AOCSFLAG_STANDBY);
		BCL_tool_register_cmd(OBCT_sec2cycle(4), Cmd_CODE_SAME_TL_MODE_TRANS);
		BCL_tool_register_cmd(OBCT_sec2cycle(5), Cmd_CODE_MM_FINISH_TRANSITION);
}

void BCL_load_to_standby(){
		BCL_tool_prepare_param_uint16(MD_AOCSFLAG_STANDBY);
		BCL_tool_register_cmd(OBCT_sec2cycle(4), Cmd_CODE_SAME_TL_MODE_TRANS);
		BCL_tool_register_cmd(OBCT_sec2cycle(5), Cmd_CODE_MM_FINISH_TRANSITION);
}

void BCL_load_to_mission_standby(){
		BCL_tool_prepare_param_uint8(0x00);
		BCL_tool_prepare_param_uint8(0x00);
		BCL_tool_prepare_param_uint8(0x00);
		BCL_tool_register_cmd( 1, Cmd_CODE_ACS_CHANGE_GYRO_RANGE); // 0x000000  : GYRO_RANGE(x,y,z) ±75deg/s

		BCL_tool_register_cmd( 2, Cmd_CODE_TLM_CHANGE_FREQ);

		BCL_tool_prepare_param_uint16(MD_AOCSFLAG_MISSION_STANDBY);
		BCL_tool_register_cmd(OBCT_sec2cycle(4), Cmd_CODE_SAME_TL_MODE_TRANS);
		BCL_tool_register_cmd(OBCT_sec2cycle(5), Cmd_CODE_MM_FINISH_TRANSITION);
}

