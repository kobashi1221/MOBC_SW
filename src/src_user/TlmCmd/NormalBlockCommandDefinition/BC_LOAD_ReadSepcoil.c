/*
 * BC_read_sepcoil.c
 *
 *  Created on: 2021/05/29
 *      Author: kahaku
 */

#include "nbc_header.h"
#include "src_core/TlmCmd/block_command_table.h"
#include "src_user/TlmCmd/block_command_definitions.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/command_definitions.h"

void BC_load_read_sepcoil(){
	// 使っていないと思われるのでコメントアウト 2021/11/30 Otsuki
//	BCL_tool_register_cmd    (30, Cmd_CODE_PWR_SEPCOIL_CURS_READ);
//	BCL_tool_register_cmd    (30, Cmd_CODE_PWR_REG_ACS2_5V_VOLS_READ);
//	BCL_tool_register_cmd    (30, Cmd_CODE_PWR_SEPCOIL1_VOLS_READ);
//	BCL_tool_register_cmd    (30, Cmd_CODE_PWR_SEPCOIL2_VOLS_READ);
//	BCL_tool_register_cmd    (30, Cmd_CODE_PWR_SEPCOIL3_VOLS_READ);
//	BCL_tool_register_cmd    (30, Cmd_CODE_PWR_SEPCOIL4_VOLS_READ);
}
