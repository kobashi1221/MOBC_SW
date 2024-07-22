/**
  *****************************************************************************************
  * @file    : commnad_response_handler.h
  * @author  : Yuta Sakaguchi
  * @version : 1.0
  * @date    : 2022/06/06
  * @brief   :　コマンド応答
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2022/06/06  1.0   Y.Sakaguchi    First revision
  *
  *****************************************************************************************
  */

#ifndef COM_CMD_RESPONSE_HANDLER_H_
#define COM_CMD_RESPONSE_HANDLER_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/command_dispatcher.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Drivers/Com/GS/command_response_struct.h>
#include <src_user/Drivers/Com/RXPIC/RXPIC.h>
#include <src_user/Drivers/Cdh/LoRa/LoRa_Struct.h>
#include <src_user/Drivers/Com/INTER_SAT/Inter_Sat.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum{
	TLM_DISABLE,
	TLM_ENABLE,
}TLM_INFO_FLAG;

 typedef struct{
	uint8_t res_flag;
	uint16_t res_apid;
	uint8_t res_packet_id;
	uint8_t res_sat_id;
	uint8_t res_exe_id;
	uint8_t res_route;
	uint8_t res_seq_count;

	uint16_t tlc_register_id;
	uint32_t tlc_register_time;
	int32_t tlc_register_sts;

	TCP tcp;
	uint8_t tlc_goal_response;
	uint8_t packet_header[6];
}GS_RESPONS_STRUCT;

 extern GS_RESPONS_STRUCT* const p_gs_response_struct;

 void command_response_handler(const TCP* packet);
 _ACK gs_command_checker(const TCP* packet);
 _ACK tl_command_checker(const TCP* packet);

#ifdef __cplusplus
}
#endif

#endif
