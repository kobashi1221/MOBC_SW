#include "nbc_header.h"

#include "src_user/Applications/app_registry.h"
//#include <src_core/Applications/anomaly_handler.h>
#include "src_user/TlmCmd/block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>


//分離直後から使用するパラメータ群を登録する
//最大数24に注意
void BC_load_after_sep_param_instance(void)
{
	BCL_tool_prepare_param_uint8(0x01);//uart
	BCL_tool_prepare_param_uint8(0x03);//millmax
	BCL_tool_register_cmd    ( 1, Cmd_CODE_AH_COMPORT_ANOMALY_DISABLE);

}

//分離直後から使用するパラメータ群を登録する
//最大数24に注意
void BC_load_after_sep_param_delay(void)
{
	BCL_tool_register_combine(1, BC_SEPARATION_OFF);
}
