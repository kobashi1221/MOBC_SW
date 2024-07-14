#pragma section REPRO
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>
#include <src_user/TlmCmd/telemetry_definitions.h>


void BLC_load_separation_mode(void)
{
	//観測更新BC
//	BCL_tool_register_combine( 1, BC_AOCS_SENSOR_HANDLE);

	//プロパゲート
	BCL_tool_register_combine(1, BC_TLM_CMD_PACKET_HANDLER);//TXPIC,RXPIC,COBC,1U-2U 受信データをリストに追加
	BCL_tool_register_combine(10, BC_AC_TLM_CMD_DISPATCH);//RTC, TL1, TL2, GS Cmd dispatch

	BCL_tool_register_combine(50, BC_AOCS);

	BCL_tool_register_combine(410, BC_TLM_ROUTING);
	BCL_tool_register_rotate (430, BC_MEMORY_PATROL);

	BCL_tool_register_combine(465, BC_OBC_COMM);
	BCL_tool_register_combine(470, BC_PWR_BUILT_IN_ADC);//内蔵ADC
	BCL_tool_register_rotate (475, BC_PWR_I2C_ADC);//このADCをRotateで1周期ごとに１つずつ実行
	BCL_tool_register_combine(485, BC_TLM_HIGH_SPIN);

	BCL_tool_register_app    (535, AR_MEM_BD_APP);
	BCL_tool_register_combine(548, BC_ANOMALY_DETECTION_HANDLER );


	BCL_tool_register_combine(550, BC_AOCS);

	BCL_tool_register_combine(940, BC_TLM_HIGH_SPIN);
	BCL_tool_register_combine(960, BC_TLM_GENERATE_AND_SAVE);

}
#pragma section
