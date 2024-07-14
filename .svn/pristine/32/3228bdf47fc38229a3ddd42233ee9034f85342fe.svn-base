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

#ifndef COMMAND_RESPONSE_STURCT_H_
#define COMMAND_RESPONSE_STURCT_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/command_dispatcher.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

#ifdef __cplusplus
 extern "C" {
#endif

 typedef enum{
 	TO_COBC = 0b00000001,
 	TO_MILLMAX = 0b00000010,
 	TO_LORA = 0b00000100,
 }TO_GOAL;

typedef enum{
	NON_RES,
	GSC_RES,
	TLC_RES,
}RESPONSE_NUM;

typedef enum{
	ONLY_RESULT_RESPONSE = 0xfe,
	NON_RESPONSE = 0xff,
}COMMAND_RES_CODE;


#ifdef __cplusplus
}
#endif

#endif /* SAMPLEDRIVER_H_ */
