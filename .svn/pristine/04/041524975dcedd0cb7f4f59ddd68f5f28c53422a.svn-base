/*
 * BC_load_obc_ph.h
 *
 *  Created on: 2021/05/29
 *      Author: kahak
 *  Edited: 2021/09/07
 *  	park
 */

#include "nbc_header.h"
#include <src_core/TlmCmd/block_command_table.h>
#include "../block_command_definitions.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>

void BC_load_built_in_ADC(void){
	// Build-in-ADC の DMAContinuousRequests で値自体は常に取得している。
	// 1.6mA per ADC
	// ここで呼ばれるコマンドはバッファーの値を構造体に格納しているだけ
	BCL_tool_prepare_param_uint8(0); // ORDER_CURS_MOBC
	BCL_tool_register_cmd    (1,Cmd_CODE_PWR_ADC_MOBC_READ);
	BCL_tool_prepare_param_uint8(1); // ORDER_CURS_LORA
	BCL_tool_register_cmd    (2,Cmd_CODE_PWR_ADC_MOBC_READ);
	BCL_tool_prepare_param_uint8(2); // ORDER_VOLS_GPS
	BCL_tool_register_cmd    (3,Cmd_CODE_PWR_ADC_MOBC_READ);
	BCL_tool_prepare_param_uint8(3); // ORDER_VOLS_LORA
	BCL_tool_register_cmd    (4,Cmd_CODE_PWR_ADC_MOBC_READ);
#ifdef ONE_U
	BCL_tool_prepare_param_uint8(4); // ORDER_VOLS_MTQA
	BCL_tool_register_cmd    (5,Cmd_CODE_PWR_ADC_MOBC_READ);
	BCL_tool_prepare_param_uint8(5); // ORDER_CURS_MTQZ3Z4
	BCL_tool_register_cmd    (6,Cmd_CODE_PWR_ADC_MOBC_READ);
	BCL_tool_prepare_param_uint8(6); // ORDER_CURS_MTQX
	BCL_tool_register_cmd    (7,Cmd_CODE_PWR_ADC_MOBC_READ);
	BCL_tool_prepare_param_uint8(7); // ORDER_CURS_MTQY
	BCL_tool_register_cmd    (8,Cmd_CODE_PWR_ADC_MOBC_READ);
	BCL_tool_prepare_param_uint8(8); // ORDER_CURS_MTQZ1Z2
	BCL_tool_register_cmd    (9,Cmd_CODE_PWR_ADC_MOBC_READ);

	//PSTATE
	BCL_tool_prepare_param_uint8(0);
	BCL_tool_register_cmd    (10,Cmd_CODE_PWR_GPIO_INPUT_MOBC_READ);
#endif
    BCL_tool_prepare_param_uint8(1); // ORDER_VOLS_SEPD_MSW
    BCL_tool_register_cmd    (11,Cmd_CODE_PWR_GPIO_INPUT_MOBC_READ);
    BCL_tool_prepare_param_uint8(2); // ORDER_VOLS_SEPD_MILLMAX
    BCL_tool_register_cmd    (12,Cmd_CODE_PWR_GPIO_INPUT_MOBC_READ);
}

void BC_load_i2c_ADC(void){
//	BCL_tool_register_cmd    (34, Cmd_CODE_PWR_ADC_SAMPLE);
	BCL_tool_prepare_param_uint8(0);
	BCL_tool_register_cmd    (1, Cmd_CODE_PWR_ADC_PWR2_READ);
    BCL_tool_prepare_param_uint8(1);
    BCL_tool_register_cmd    (2, Cmd_CODE_PWR_ADC_PWR2_READ);
	BCL_tool_prepare_param_uint8(2);
	BCL_tool_register_cmd    (3, Cmd_CODE_PWR_ADC_PWR2_READ);
	BCL_tool_prepare_param_uint8(4);
	BCL_tool_register_cmd    (4, Cmd_CODE_PWR_ADC_PWR2_READ);
	BCL_tool_prepare_param_uint8(5);
	BCL_tool_register_cmd    (5, Cmd_CODE_PWR_ADC_PWR2_READ);
    BCL_tool_prepare_param_uint8(6);
    BCL_tool_register_cmd    (16, Cmd_CODE_PWR_ADC_PWR2_READ);
	BCL_tool_prepare_param_uint8(7);
	BCL_tool_register_cmd    (6, Cmd_CODE_PWR_ADC_PWR2_READ);
    BCL_tool_prepare_param_uint8(8);
    BCL_tool_register_cmd    (7, Cmd_CODE_PWR_ADC_PWR2_READ);
    BCL_tool_prepare_param_uint8(9);
    BCL_tool_register_cmd    (8, Cmd_CODE_PWR_ADC_PWR2_READ);
    BCL_tool_prepare_param_uint8(10);
    BCL_tool_register_cmd    (9, Cmd_CODE_PWR_ADC_PWR2_READ);
    BCL_tool_prepare_param_uint8(11);
    BCL_tool_register_cmd    (10, Cmd_CODE_PWR_ADC_PWR2_READ);
	BCL_tool_prepare_param_uint8(12);
	BCL_tool_register_cmd    (11, Cmd_CODE_PWR_ADC_PWR2_READ);
	BCL_tool_prepare_param_uint8(13);
	BCL_tool_register_cmd    (12, Cmd_CODE_PWR_ADC_PWR2_READ);
	BCL_tool_prepare_param_uint8(14);
	BCL_tool_register_cmd    (13, Cmd_CODE_PWR_ADC_PWR2_READ);
	BCL_tool_prepare_param_uint8(15);
	BCL_tool_register_cmd    (14, Cmd_CODE_PWR_ADC_PWR2_READ);
#ifdef TWO_U
	BCL_tool_prepare_param_uint8(3);
	BCL_tool_register_cmd    (15, Cmd_CODE_PWR_ADC_PWR2_READ);
#endif
}

