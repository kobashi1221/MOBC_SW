/*
 * PreciseRelativeOrbitControl.c
 *
 *  Created on: 2022/08/03
 *      Author: sakaguchi
 */

#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>

#include "src_core/TlmCmd/block_command_table.h"
#include <src_core/TlmCmd/block_command_loader.h>

#include "src_user/TlmCmd/block_command_definitions.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/command_definitions.h"
#include "src_user/Settings/Modes/mode_definitions.h"


void BCL_load_to_earth_center_pointing(){

		BCL_tool_prepare_param_uint16(MD_AOCSFLAG_EARTH_CENTER_POINTING);
		BCL_tool_register_cmd(OBCT_sec2cycle(4), Cmd_CODE_SAME_TL_MODE_TRANS);
		BCL_tool_register_cmd(OBCT_sec2cycle(5), Cmd_CODE_MM_FINISH_TRANSITION);
}



