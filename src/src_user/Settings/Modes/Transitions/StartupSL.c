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

void BCL_load_standby_to_startup()
{
	BCL_tool_register_cmd(OBCT_sec2cycle(10), Cmd_CODE_MM_FINISH_TRANSITION);
}
