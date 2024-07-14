#pragma section REPRO
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>



//TLの予備
void BCL_load_reserved1_mode()
{

      BCL_tool_register_combine( 1, BC_TLM_CMD_PACKET_HANDLER);

      BCL_tool_register_combine( 3, BC_AC_TLM_CMD_DISPATCH);//RTC, TL1, TL2, GS Cmd dispatch

	  BCL_tool_register_combine(45, BC_PWR_BUILT_IN_ADC);

	  BCL_tool_register_rotate(47, BC_PWR_I2C_ADC);//このADCをRotateで1周期ごとに１つずつ実行

	  BCL_tool_register_combine(65, BC_OBC_COMM);

	  BCL_tool_register_combine(75, BC_TLM_ROUTING);//坂口added
	  //MTQの電圧・電流，Pt,9ax,MAGS,gyro,gps,suns取得
	  BCL_tool_register_combine( 85, BC_AOCS_SENSOR_HANDLE);

	  BCL_tool_register_combine( 360, BC_TLM_ROUTING);//坂口added
	  BCL_tool_register_combine( 370, BC_ODS);
	  BCL_tool_register_combine( 375, BC_ADS);
	  BCL_tool_register_combine( 380, BC_ACS);
	  BCL_tool_register_combine( 385, BC_OCS);
	  BCL_tool_register_combine( 390, BC_ACTUATOR);


	  BCL_tool_register_app    (410, AR_MEM_BD_APP);

	  BCL_tool_register_combine(420, BC_TLM_GENERATE_AND_SAVE);

	  BCL_tool_register_combine(570, BC_MEMORY_PATROL);

	  BCL_tool_register_combine(630, BC_TLM_ROUTING);

	  BCL_tool_register_combine(680, BC_ANOMALY_DETECTION_HANDLER );

	  BCL_tool_register_combine( 700, BC_TLM_ROUTING);//坂口added
}

//TLの予備
void BCL_load_reserved2_mode()
{

      BCL_tool_register_combine( 1, BC_TLM_CMD_PACKET_HANDLER);

      BCL_tool_register_combine( 3, BC_AC_TLM_CMD_DISPATCH);//RTC, TL1, TL2, GS Cmd dispatch

	  BCL_tool_register_combine(45, BC_PWR_BUILT_IN_ADC);

	  BCL_tool_register_rotate(47, BC_PWR_I2C_ADC);//このADCをRotateで1周期ごとに１つずつ実行

	  BCL_tool_register_combine(65, BC_OBC_COMM);

	  BCL_tool_register_combine(75, BC_TLM_ROUTING);//坂口added
	  //MTQの電圧・電流，Pt,9ax,MAGS,gyro,gps,suns取得
	  BCL_tool_register_combine( 85, BC_AOCS_SENSOR_HANDLE);

	  BCL_tool_register_combine( 360, BC_TLM_ROUTING);//坂口added
	  BCL_tool_register_combine( 370, BC_ODS);
	  BCL_tool_register_combine( 375, BC_ADS);
	  BCL_tool_register_combine( 380, BC_ACS);
	  BCL_tool_register_combine( 385, BC_OCS);
	  BCL_tool_register_combine( 390, BC_ACTUATOR);


	  BCL_tool_register_app    (410, AR_MEM_BD_APP);

	  BCL_tool_register_combine(420, BC_TLM_GENERATE_AND_SAVE);

	  BCL_tool_register_combine(570, BC_MEMORY_PATROL);

	  BCL_tool_register_combine(630, BC_TLM_ROUTING);

	  BCL_tool_register_combine(680, BC_ANOMALY_DETECTION_HANDLER );

	  BCL_tool_register_combine( 700, BC_TLM_ROUTING);//坂口added
}

//TLの予備
void BCL_load_reserved3_mode()
{

      BCL_tool_register_combine( 1, BC_TLM_CMD_PACKET_HANDLER);

      BCL_tool_register_combine( 3, BC_AC_TLM_CMD_DISPATCH);//RTC, TL1, TL2, GS Cmd dispatch

	  BCL_tool_register_combine(45, BC_PWR_BUILT_IN_ADC);

	  BCL_tool_register_rotate(47, BC_PWR_I2C_ADC);//このADCをRotateで1周期ごとに１つずつ実行

	  BCL_tool_register_combine(65, BC_OBC_COMM);

	  BCL_tool_register_combine(75, BC_TLM_ROUTING);//坂口added
	  //MTQの電圧・電流，Pt,9ax,MAGS,gyro,gps,suns取得
	  BCL_tool_register_combine( 85, BC_AOCS_SENSOR_HANDLE);

	  BCL_tool_register_combine( 360, BC_TLM_ROUTING);//坂口added
	  BCL_tool_register_combine( 370, BC_ODS);
	  BCL_tool_register_combine( 375, BC_ADS);
	  BCL_tool_register_combine( 380, BC_ACS);
	  BCL_tool_register_combine( 385, BC_OCS);
	  BCL_tool_register_combine( 390, BC_ACTUATOR);


	  BCL_tool_register_app    (410, AR_MEM_BD_APP);

	  BCL_tool_register_combine(420, BC_TLM_GENERATE_AND_SAVE);

	  BCL_tool_register_combine(570, BC_MEMORY_PATROL);

	  BCL_tool_register_combine(630, BC_TLM_ROUTING);

	  BCL_tool_register_combine(680, BC_ANOMALY_DETECTION_HANDLER );

	  BCL_tool_register_combine( 700, BC_TLM_ROUTING);//坂口added
}

//TLの予備
void BCL_load_reserved4_mode()
{

      BCL_tool_register_combine( 1, BC_TLM_CMD_PACKET_HANDLER);

      BCL_tool_register_combine( 3, BC_AC_TLM_CMD_DISPATCH);//RTC, TL1, TL2, GS Cmd dispatch

	  BCL_tool_register_combine(45, BC_PWR_BUILT_IN_ADC);

	  BCL_tool_register_rotate(47, BC_PWR_I2C_ADC);//このADCをRotateで1周期ごとに１つずつ実行

	  BCL_tool_register_combine(65, BC_OBC_COMM);

	  BCL_tool_register_combine(75, BC_TLM_ROUTING);//坂口added
	  //MTQの電圧・電流，Pt,9ax,MAGS,gyro,gps,suns取得
	  BCL_tool_register_combine( 85, BC_AOCS_SENSOR_HANDLE);

	  BCL_tool_register_combine( 360, BC_TLM_ROUTING);//坂口added
	  BCL_tool_register_combine( 370, BC_ODS);
	  BCL_tool_register_combine( 375, BC_ADS);
	  BCL_tool_register_combine( 380, BC_ACS);
	  BCL_tool_register_combine( 385, BC_OCS);
	  BCL_tool_register_combine( 390, BC_ACTUATOR);


	  BCL_tool_register_app    (410, AR_MEM_BD_APP);

	  BCL_tool_register_combine(420, BC_TLM_GENERATE_AND_SAVE);

	  BCL_tool_register_combine(570, BC_MEMORY_PATROL);

	  BCL_tool_register_combine(630, BC_TLM_ROUTING);

	  BCL_tool_register_combine(680, BC_ANOMALY_DETECTION_HANDLER );

	  BCL_tool_register_combine( 700, BC_TLM_ROUTING);//坂口added
}

#pragma section
