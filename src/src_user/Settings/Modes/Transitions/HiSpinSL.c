/*
 * HiSpinSL.c
 *
 *  Created on: 2022/07/07
 *      Author: SAT37
 */

#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>

#include "src_user/TlmCmd/block_command_definitions.h"
#include "src_user/TlmCmd/command_definitions.h"

#include "src_core/Applications/timeline_command_dispatcher.h"
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/System/TimeManager/obc_time.h>
#include "src_user/Settings/Modes/mode_definitions.h"

void BCL_load_to_hispin(){

	BCL_tool_prepare_param_uint8(0x03);
	BCL_tool_prepare_param_uint8(0x03);
	BCL_tool_prepare_param_uint8(0x03);
	BCL_tool_register_cmd( 1, Cmd_CODE_ACS_CHANGE_GYRO_RANGE); // 0x030303  : GYRO_RANGE(x,y,z) ±900deg/s

	BCL_tool_register_cmd( 2, Cmd_CODE_TLM_CHANGE_FREQ);

	BCL_tool_prepare_param_uint16(MD_AOCSFLAG_HIGH_RATE_SPIN);
	BCL_tool_register_cmd(OBCT_sec2cycle(4), Cmd_CODE_SAME_TL_MODE_TRANS);
	BCL_tool_register_cmd(OBCT_sec2cycle(5), Cmd_CODE_MM_FINISH_TRANSITION);
}
