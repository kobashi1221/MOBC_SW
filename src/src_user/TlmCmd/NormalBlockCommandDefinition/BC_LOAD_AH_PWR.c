/*
 * BC_LOAD_AH_PWR_OFF.c
 *
 *  Created on: 2021/11/16
 *      Author: pooh_
 */

#include "nbc_header.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>
//#include "../../Settings/AnomalyHandlerRules/ah_rules.h"

//#include <src_user/Applications/app_registry.h>

void BC_load_AH_PWR(void){
	// PWR系のアノマリ用アプリを実行

	BCL_tool_register_cmd(1,Cmd_CODE_PWR_AH_OC);
	BCL_tool_register_cmd(2,Cmd_CODE_PWR_AH_OV);
	BCL_tool_register_cmd(3,Cmd_CODE_PWR_AH_UC);
	BCL_tool_register_cmd(4,Cmd_CODE_PWR_AH_UV);
	BCL_tool_register_cmd(5,Cmd_CODE_PWR_AH_PSI_OFF);
	BCL_tool_register_cmd(6,Cmd_CODE_PWR_AH_PSI_ON);
	BCL_tool_register_cmd(7,Cmd_CODE_PWR_AH_SYSTEM_ENABLE);

}
