/**
  *****************************************************************************************
  * @file    : COM_cmd_response.h
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

#ifndef COM_CMD_RESPONSE_H_
#define COM_CMD_RESPONSE_H_

#include <stdint.h> //for uint8_t
#include <stdbool.h> //for boolean
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/TaskManager/task_dispatcher.h>
#include <src_user/IfWrapper/HAL.h>
#include "../../DriverInstances/di_rxpic.h"
#include "../../DriverInstances/di_txpic.h"
#include "../../DriverInstances/di_intersat.h"
#include "../../DriverInstances/di_lora.h"


#ifdef __cplusplus
 extern "C" {
#endif

typedef enum{
	SD_PAGE0,
	SD_PAGE1,
	SD_PAGE2,
	SD_PAGE3,
	SD_PAGE4,
	SD_PAGE_MAX,
}SD_PAGE;

typedef enum{
	SD_TLM_INFO,
	PROM_TLM_INFO
}WHO_TOM;

AppInfo command_response_update(void);
AppInfo command_response_param_update(void);

void response_init(void);
void gsc_response_handler(void);
void tlc_response_handler(void);
void tlm_response_handler(void);
void parse_tlm_info(uint8_t* in_buf);

#ifdef __cplusplus
}
#endif

#endif
