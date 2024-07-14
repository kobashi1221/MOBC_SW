
#pragma section REPRO
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>
#include <src_user/TlmCmd/telemetry_definitions.h>



void BCL_load_hispin_mode(void)
{
	BCL_tool_register_combine( 1, BC_AOCS);
	BCL_tool_register_combine(150, BC_OBC_COMM);
	BCL_tool_register_combine(160, BC_TLM_HIGH_SPIN);
	BCL_tool_register_app    (162, AR_MEM_SD_APP);

	BCL_tool_register_combine(201, BC_AOCS);
	BCL_tool_register_combine(350, BC_TLM_CMD_PACKET_HANDLER);//TXPIC,RXPIC,COBC,1U-2U 受信データをリストに追加
	BCL_tool_register_combine(360, BC_AC_TLM_CMD_DISPATCH);//RTC, TL1, TL2, GS Cmd dispatch
	BCL_tool_register_combine(380, BC_TLM_ROUTING);
	BCL_tool_register_combine(395, BC_TLM_HIGH_SPIN);

	BCL_tool_register_combine(401, BC_AOCS);
	BCL_tool_register_combine(550, BC_PWR_BUILT_IN_ADC);//内蔵ADC
	BCL_tool_register_rotate (552, BC_PWR_I2C_ADC);//このADCをRotateで1周期ごとに１つずつ実行
	BCL_tool_register_combine(568, BC_TLM_HIGH_SPIN);
	BCL_tool_register_app    (570, AR_MEM_SD_APP);

	BCL_tool_register_combine(601, BC_AOCS);
	BCL_tool_register_app    (750, AR_MEM_BD_APP);
	BCL_tool_register_combine(755, BC_ANOMALY_DETECTION_HANDLER );
	BCL_tool_register_combine(758, BC_TLM_HIGH_SPIN);
	BCL_tool_register_combine(760, BC_TLM_GENERATE_AND_SAVE);

	BCL_tool_register_combine(801, BC_AOCS);
	BCL_tool_register_combine(950, BC_TLM_ROUTING);
	BCL_tool_register_rotate (970, BC_MEMORY_PATROL);
	BCL_tool_register_combine(990, BC_TLM_HIGH_SPIN);
}

void BCL_tlm_hispin_mode(void)
{
	BCL_tool_prepare_param_uint16(AFSK_GS);
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80);
	BCL_tool_prepare_param_int8(Tlm_CODE_SPIN);
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 1, Cmd_CODE_GENERATE_TLM_USER);

	BCL_tool_prepare_param_uint16(AFSK_GS);
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80);
	BCL_tool_prepare_param_int8(Tlm_CODE_AOCS80);
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 2, Cmd_CODE_GENERATE_TLM_USER);

	BCL_tool_prepare_param_uint16(AFSK_GS);
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80);
	BCL_tool_prepare_param_int8(Tlm_CODE_AOCS160);
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 3, Cmd_CODE_GENERATE_TLM_USER);

	BCL_tool_prepare_param_uint16(AFSK_GS);
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80);
	BCL_tool_prepare_param_int8(Tlm_CODE_AOCS240);
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 4, Cmd_CODE_GENERATE_TLM_USER);
}



#pragma section
