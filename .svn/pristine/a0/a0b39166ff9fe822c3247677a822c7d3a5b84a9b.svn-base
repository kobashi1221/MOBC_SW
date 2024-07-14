/*
 * BC_LOAD_AH_SENSOR.c
 *
 *  Created on: 2021/11/16
 *      Author: sakaguchi
 */

#include "nbc_header.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>
//#include "../../Settings/AnomalyHandlerRules/ah_rules.h"

//#include <src_user/Applications/app_registry.h>

void BC_load_AH_SENSOR(void){
	// SENSOR系のアノマリ用アプリを実行

	BCL_tool_register_cmd(1,Cmd_CODE_ACS_AH_MTQA);

	BCL_tool_register_cmd(2,Cmd_CODE_ACS_AH_MTQO);

}
