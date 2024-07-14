#include "nbc_header.h"

#include "src_user/Applications/app_registry.h"
//#include <src_core/Applications/anomaly_handler.h>
#include "src_user/TlmCmd/block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>
#include "../telemetry_definitions.h"
//#include "../../Settings/AnomalyHandlerRules/ah_rules.h"

void BC_load_AH_MODE_Auto_Trans(void)
{
	BCL_tool_register_cmd(1,Cmd_CODE_MODE_AUTO_TRANS);

	BCL_tool_prepare_param_uint8(0x00);
	BCL_tool_prepare_param_uint8(0x00);
	BCL_tool_register_cmd(3,Cmd_CODE_AUTO_TRANS_FLAG_SETTING);

	//anomaly enableの処理は各自で
//	BCL_tool_prepare_param_uint8(AH_RULE_QAX_GYRO_OT);
//	BCL_tool_register_cmd(4,Cmd_CODE_AH_ACTIVATE_RULE);

}
