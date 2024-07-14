/**
  *****************************************************************************************
  * @file    : COM_cmd_INTERSAT.h
  * @author  : Sakaguchi
  * @version : 1.0
  * @date    : 2021/11/26
  * @brief   :　他機とのUART通信用
  *****************************************************************************************
  */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_COMMUNICATION_COM_CMD_INTERSAT_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_COMMUNICATION_COM_CMD_INTERSAT_H_


#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../DriverInstances/di_intersat.h"
#include "../../../Drivers/Com/INTER_SAT/Inter_Sat.h"
#include <src_core/System/ApplicationManager/app_info.h>

typedef enum{
	SEND_TLM,
	LIFE_COUNT,
	NONE,
}INTERSAT_TLM_ENUM;

CCP_EXEC_STS Cmd_COM_debug_Intersat_Send(const CommonCmdPacket* packet);

AppInfo COM_intersat_param_update(void);
AppInfo COM_intersat_tlm_update(void);

#endif
