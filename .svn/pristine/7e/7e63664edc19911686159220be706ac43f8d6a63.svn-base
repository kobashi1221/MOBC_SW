/**
  *****************************************************************************************
  * @file    : COM1_debug_RXPICUART.h
  * @author  : Yuta Sakaguchi
  * @version : 1.0
  * @date    : 2022/06/17
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/07/21  1.0   T.NORO         First revision
  *
  *****************************************************************************************
  */

#ifndef COM_RELAY_H_
#define COM_RELAY_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../DriverInstances/di_rxpic.h"
#include "../../DriverInstances/di_txpic.h"
#include "../../DriverInstances/di_intersat.h"
#include "../../DriverInstances/di_LoRa.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define ADDIITIONAL_DATA (8)
#define MESSAGE_HEADER (2)
#define MESSAGE_FLAG_ON (1)
#define MESSAGE_FLAG_OFF (0)
#define MESSAGE_MAX_SIZE (256)

typedef struct{
	uint16_t length;
	//uint32_t time;
}MESSAGE_INFO_FROM_GS;

typedef struct{
	uint16_t length;
	//uint32_t time;
}MESSAGE_INFO_TO_GS;

typedef struct{
	MESSAGE_INFO_FROM_GS from_gs_info[100];
	MESSAGE_INFO_TO_GS to_gs_info[100];
}MESSAGE_STRUCT;

AppInfo COM_message_param_update(void);

CCP_CmdRet COM_message_from_GS(const CommonCmdPacket* packet);
CCP_CmdRet COM_message_from_SAT(const CommonCmdPacket* packet);
CCP_CmdRet COM_message_to_SAT(const CommonCmdPacket* packet);
CCP_CmdRet COM_message_to_GS(const CommonCmdPacket* packet);

extern const MESSAGE_STRUCT* p_message_struct;


#ifdef __cplusplus
}
#endif


#endif
