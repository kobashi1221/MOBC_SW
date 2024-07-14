#pragma section REPRO
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_mission1_mode(void){

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
	BCL_tool_register_combine(570, BC_TLM_HIGH_SPIN);
	BCL_tool_register_app    (572, AR_MEM_SD_APP);

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

//デフォルトでは分離TLを設定
void BCL_load_mission2_mode(void){
	//観測更新BC
//	BCL_tool_register_combine( 1, BC_AOCS_SENSOR_HANDLE);

	//プロパゲート
	BCL_tool_register_combine(1, BC_TLM_CMD_PACKET_HANDLER);//TXPIC,RXPIC,COBC,1U-2U 受信データをリストに追加
	BCL_tool_register_combine(10, BC_AC_TLM_CMD_DISPATCH);//RTC, TL1, TL2, GS Cmd dispatch

	BCL_tool_register_combine(30, BC_AOCS);

	BCL_tool_register_combine(370, BC_OBC_COMM);
	BCL_tool_register_combine(400, BC_PWR_BUILT_IN_ADC);//内蔵ADC
	BCL_tool_register_rotate (415, BC_PWR_I2C_ADC);//このADCをRotateで1周期ごとに１つずつ実行
	BCL_tool_register_combine(455, BC_TLM_HIGH_SPIN);

	BCL_tool_register_app    (500, AR_MEM_BD_APP);
	BCL_tool_register_combine(515, BC_ANOMALY_DETECTION_HANDLER );

	BCL_tool_register_combine(530, BC_AOCS);

	BCL_tool_register_combine(870, BC_TLM_HIGH_SPIN);
	BCL_tool_register_combine(910, BC_TLM_GENERATE_AND_SAVE);

	BCL_tool_register_combine(950, BC_TLM_ROUTING);
	BCL_tool_register_rotate (980, BC_MEMORY_PATROL);
}


void BCL_load_mission3_mode(void){
    BCL_tool_register_combine( 1, BC_TLM_CMD_PACKET_HANDLER);
	  BCL_tool_register_combine( 5, BC_AC_TLM_CMD_DISPATCH);//RTC, TL1, TL2, GS Cmd dispatch

	  BCL_tool_register_combine( 6, BC_PWR_BUILT_IN_ADC);

	  BCL_tool_register_rotate(  7, BC_PWR_I2C_ADC);//12このADCをRotateで1周期ごとに１つずつ実行

	  BCL_tool_register_combine( 10, BC_OBC_COMM);

	  //MTQの電圧・電流，Pt,9ax,MAGS,gyro,gps,suns取得
//	  printf("aaa\n");
	  BCL_tool_register_combine( 15, BC_AOCS_SENSOR_HANDLE);
#ifdef HILS
	  BCL_tool_register_app    (25, AR_ACS_GPS_APP);
#endif

	  BCL_tool_register_combine( 50, BC_ODS);
	  BCL_tool_register_combine( 53, BC_ADS);
	  BCL_tool_register_combine( 56, BC_ACS);
	  BCL_tool_register_combine( 59, BC_OCS);
	  BCL_tool_register_combine( 61, BC_ACTUATOR);


	  BCL_tool_register_app    (65, AR_CDH_RTC_APP);

	  BCL_tool_register_combine(75, BC_TLM_GENERATE_AND_SAVE);

	  BCL_tool_register_combine(90, BC_TLM_ROUTING);

	  BCL_tool_register_combine(95, BC_MEMORY_PATROL);

	  BCL_tool_register_combine(100, BC_ANOMALY_DETECTION_HANDLER );
}

#pragma section
