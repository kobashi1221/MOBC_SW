/*
 * BC_LOAD_AH_ComPort_Reset.c
 *
 *  Created on: 2021/11/16
 *      Author: Noro
 */

#include "nbc_header.h"

#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>
//#include "../../Settings/AnomalyHandlerRules/ah_rules.h"
//よく考えたら，フラグで管理してるから一個のBCで全部確認しちゃっても問題ない気がする
void BC_load_AH_ComPort_RESET(void){
	BCL_tool_register_cmd(1,Cmd_CODE_AH_UART_RESET_LV1);
	BCL_tool_register_cmd(2,Cmd_CODE_AH_UART_RESET_LV2);
	BCL_tool_register_cmd(3,Cmd_CODE_AH_UART_RESET_LV3);
	BCL_tool_register_cmd(4,Cmd_CODE_AH_I2C_RESET);
	BCL_tool_register_cmd(5,Cmd_CODE_AH_SPI_RESET);
	BCL_tool_register_cmd(6,Cmd_CODE_AH_COM_IOEX_RESET);
	BCL_tool_register_cmd(7,Cmd_CODE_AH_I2C_OFF);
	BCL_tool_register_cmd(8,Cmd_CODE_ACS_SUBCAM_RE_INIT);

//	BCL_tool_prepare_param_uint8(AH_RULE_UART_ERROR_LV1);
//	BCL_tool_register_cmd(9,Cmd_CODE_AH_ACTIVATE_RULE);
//
//	BCL_tool_prepare_param_uint8(AH_RULE_UART_ERROR_LV2);
//	BCL_tool_register_cmd(10,Cmd_CODE_AH_ACTIVATE_RULE);
//
//	BCL_tool_prepare_param_uint8(AH_RULE_UART_ERROR_LV3);
//	BCL_tool_register_cmd(11,Cmd_CODE_AH_ACTIVATE_RULE);
//
//	BCL_tool_prepare_param_uint8(AH_RULE_I2C_ERROR);
//	BCL_tool_register_cmd(12,Cmd_CODE_AH_ACTIVATE_RULE);
//
//	BCL_tool_prepare_param_uint8(AH_RULE_SPI_ERROR);
//	BCL_tool_register_cmd(13,Cmd_CODE_AH_ACTIVATE_RULE);
//
//	BCL_tool_prepare_param_uint8(AH_RULE_IOEX_ERROR);
//	BCL_tool_register_cmd(14,Cmd_CODE_AH_ACTIVATE_RULE);
}
