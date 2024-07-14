
#pragma section REPRO
#include "nbc_header.h"

#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/block_command_definitions.h"

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_tlm_cmd_dispatch()
{
  BCL_tool_register_app    (0, AR_CORE_RTC_DISPATCHER);
  BCL_tool_register_app    (1, AR_TLC_DISPATCHER_GS);
  BCL_tool_register_app    (2, AR_TLC_DISPATCHER_BC);
  BCL_tool_register_app    (3, AR_TLC_DISPATCHER_TLM);
  BCL_tool_register_app    (4, AR_CORE_GSC_DISPATCHER);
//  BCL_tool_register_app    (3, AR_DI_GSTOS_MST_PH);
//  BCL_tool_register_app    (4, AR_DI_GSTOS_RPT_PH);
//  BCL_tool_register_app    (5, AR_MW_CCSDS_MS_TX);
}

#pragma section
