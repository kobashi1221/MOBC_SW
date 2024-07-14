/*
 * LargePacketTL.c
 *
 *  Created on: 2022/03/07
 *      Author: Sakaguchi
 */
#include "nbc_header.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_core/TlmCmd/block_command_loader.h>

#include <src_user/Drivers/Com/COBC/COBC.h>
#include <src_user/Applications/DriverInstances/di_cobc.h>
#include <src_user/Settings/Modes/mode_definitions.h>
#include <src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.h>

void BCL_load_cobc_large_packet_set(){
#ifdef TWO_U
  BCL_tool_prepare_param_uint8(2);
  BCL_tool_register_cmd( 1, Cmd_CODE_PWR_MIS1_ON_OFF_ONRESET);

  BCL_tool_prepare_param_uint16(cobc_driver->large_packet.cobc_large_packet_cmd);//相手側の実行コマンド
  BCL_tool_prepare_param_uint8(0);//SRAMに情報をセットする
  BCL_tool_prepare_param_uint8(cobc_driver->large_packet.comm_method);//method
  BCL_tool_prepare_param_uint8(cobc_driver->large_packet.info);//info
  BCL_tool_register_cmd	   ( 15, Cmd_CODE_COM_COBC_SEND_COMMAND);//cobcのモード切替
#endif
}

void BCL_load_cobc_large_packet_receive(){
#ifdef TWO_U
	  //受け取るデータ情報をもらう
//BCL_tool_register_cmd	   ( 1, Cmd_CODE_COM_COBC_GET_INFO);

  BCL_tool_prepare_param_uint8(1);//受取り関数のenable
  BCL_tool_register_cmd	   ( 2, Cmd_CODE_COM_COBC_RECEIVE_ENABLE);

  BCL_tool_prepare_param_uint8(0x01);//UART
  BCL_tool_prepare_param_uint8(UART_FAULT_COBC);//COBC
  BCL_tool_register_cmd	   ( 3, Cmd_CODE_AH_COMPORT_ANOMALY_DISABLE);

#endif
}



