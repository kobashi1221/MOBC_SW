#include "nbc_header.h"

#include "src_user/Applications/app_registry.h"
//#include <src_core/Applications/anomaly_handler.h>
#include "src_user/TlmCmd/block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>
#include "../telemetry_definitions.h"
//#include "../../Settings/AnomalyHandlerRules/ah_rules.h"

void BC_load_ah_al_full(void)
{
  // まず，ALのTLMをおろす
  // 次にAHのTLMを下ろす
  // さらに次にAH respondのTLMを下ろす
  // この間に，このALのAHが動いてAHのログが記録されるはず

  // このBCが動いている間にもALがうごくかもなので，
  // page0からtlm生成する
  BCL_tool_register_combine(0, BC_AH_AL);

  BCL_tool_register_combine(5, BC_AH_AH);

  BCL_tool_register_combine(10, BC_AH_AH_RES);

  // 最後に，AHとALのログを消す
//  BCL_tool_register_cmd(12,	Cmd_CODE_AH_CLEAR_LOG);
//  BCL_tool_register_cmd(13,	Cmd_CODE_AHRES_LOG_CLEAR);

  //アノマリ処理有効化
//  BCL_tool_prepare_param_uint8(AH_RULE_AL_FULL);
//  BCL_tool_register_cmd(14,	Cmd_CODE_AH_ACTIVATE_RULE);
}

void BC_load_ah_al(void){
//	cycle_t ti=1;
//	for(uint8_t page = 0; page < AH_TLM_PAGE_MAX; ++page){
//		BCL_tool_prepare_param_uint8(page);
//		BCL_tool_register_cmd(ti,Cmd_CODE_AL_SET_PAGE_FOR_TLM);
//		ti++;
//
//		BCL_tool_prepare_param_uint16(AFSK_GS);
//		BCL_tool_prepare_param_int8(0);
//	    BCL_tool_prepare_param_uint8(0x80); //ST
//	    BCL_tool_prepare_param_uint8(Tlm_CODE_AL);
//	    BCL_tool_prepare_param_uint8(1);
//	    BCL_tool_register_cmd(ti,Cmd_CODE_GENERATE_TLM_USER);
//	    ti++;
//	}
}

void BC_load_ah_ah(void){
//	cycle_t ti=1;
//	for(uint8_t page = 0; page < AH_TLM_PAGE_MAX; ++page){
//		BCL_tool_prepare_param_uint8(page);
//		BCL_tool_register_cmd(ti,Cmd_CODE_AH_SET_PAGE_FOR_TLM);
//		ti++;
//
//		BCL_tool_prepare_param_uint16(AFSK_GS);
//		BCL_tool_prepare_param_int8(0);
//	    BCL_tool_prepare_param_uint8(0x80); //ST
//	    BCL_tool_prepare_param_uint8(Tlm_CODE_AH);
//	    BCL_tool_prepare_param_uint8(1);
//	    BCL_tool_register_cmd(ti,Cmd_CODE_GENERATE_TLM_USER);
//	    ti++;
//	}
}

void BC_load_ah_ah_res(void){
//	cycle_t ti=1;
//	for(uint8_t page = 0; page < AH_TLM_PAGE_MAX; ++page){
//		BCL_tool_prepare_param_uint8(page);
//		BCL_tool_register_cmd(ti,Cmd_CODE_AHRES_LOG_SET_PAGE_FOR_TLM);
//		ti++;
//
//		BCL_tool_prepare_param_uint16(AFSK_GS);
//		BCL_tool_prepare_param_int8(0);
//	    BCL_tool_prepare_param_uint8(0x80); // ST
//	    BCL_tool_prepare_param_uint8(Tlm_CODE_AH_RES);
//	    BCL_tool_prepare_param_uint8(1);
//	    BCL_tool_register_cmd(ti,Cmd_CODE_GENERATE_TLM_USER);
//	    ti++;
//	}
}
