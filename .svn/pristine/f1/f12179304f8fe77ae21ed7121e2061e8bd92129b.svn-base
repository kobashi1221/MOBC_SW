#pragma section REPRO
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_startup_mode()
{
    BCL_tool_register_combine( 1, BC_TLM_CMD_PACKET_HANDLER);
	BCL_tool_register_combine( 5, BC_AC_TLM_CMD_DISPATCH);//RTC, TL1, TL2, GS Cmd dispatch
#ifndef HILS

	BCL_tool_register_combine( 300, BC_PWR_BUILT_IN_ADC);

	BCL_tool_register_rotate(  305, BC_PWR_I2C_ADC);//12このADCをRotateで1周期ごとに１つずつ実行

	BCL_tool_register_combine( 340, BC_OBC_COMM);
#endif
	//MTQの電圧・電流，Pt,9ax,MAGS,gyro,gps,suns取得
	BCL_tool_register_combine( 15, BC_AOCS_SENSOR_HANDLE);

	BCL_tool_register_app    (540, AR_CDH_RTC_APP);

	BCL_tool_register_combine(545, BC_TLM_GENERATE_AND_SAVE);

	BCL_tool_register_combine(700, BC_TLM_ROUTING);

    BCL_tool_register_app    (900, AR_EVENT_UTILITY);
}

#pragma section
