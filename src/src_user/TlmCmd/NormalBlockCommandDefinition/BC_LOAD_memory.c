/*
 * BC_LOAD_memory.c
 *
 *  Created on: 2022/06/22
 *      Author: daiki
 */


#include "nbc_header.h"

#include "src_core/TlmCmd/block_command_table.h"
#include "src_user/TlmCmd/block_command_definitions.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>
#include "src_user/Applications/UserDefined/Memory/MEM_cmd_SD.h"

void BC_load_memory_patrol(void){

	BCL_tool_register_app(1, AR_MEM_MRAM_PATROL_APP );

	BCL_tool_register_app(2, AR_MEM_EEPROM_PATROL_APP );

	BCL_tool_register_app(3, AR_MEM_SD_MEMORY_PATROL );

	BCL_tool_register_cmd(4, Cmd_CODE_CDH_SPIPROM_MEMORY_PATROL);

	BCL_tool_register_app(5, AP_MEM_SD_COMPARE_PROGRAM_DATA );

	BCL_tool_register_cmd(6, Cmd_CODE_MEM_FLASH_MEMORY_PATROL );
}
