/*
 * BC_LOAD_SeparationOff.h
 *
 *  Created on: 2021/05/29
 *      Author: kahaku
 *  Edited on: 2021/09/07
 *  	park
 */

#include "nbc_header.h"

#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>

void BC_load_separation_on(){
	BCL_tool_prepare_param_uint8(1);
	BCL_tool_register_cmd(     2, Cmd_CODE_PWR_SEPCOIL1_ON_OFF);

	BCL_tool_prepare_param_uint8(1);
	BCL_tool_register_cmd(      3, Cmd_CODE_PWR_SEPCOIL2_ON_OFF);

	BCL_tool_prepare_param_uint8(1);
	BCL_tool_register_cmd(     4, Cmd_CODE_PWR_SEPCOIL3_ON_OFF);

	BCL_tool_prepare_param_uint8(1);
	BCL_tool_register_cmd(     5, Cmd_CODE_PWR_SEPCOIL4_ON_OFF);
}

void BC_load_separation_off(){
	BCL_tool_prepare_param_uint8(0);
	BCL_tool_register_cmd(     2, Cmd_CODE_PWR_SEPCOIL1_ON_OFF);

	BCL_tool_prepare_param_uint8(0);
	BCL_tool_register_cmd(      3, Cmd_CODE_PWR_SEPCOIL2_ON_OFF);

	BCL_tool_prepare_param_uint8(0);
	BCL_tool_register_cmd(     4, Cmd_CODE_PWR_SEPCOIL3_ON_OFF);

	BCL_tool_prepare_param_uint8(0);
	BCL_tool_register_cmd(     5, Cmd_CODE_PWR_SEPCOIL4_ON_OFF);
}
