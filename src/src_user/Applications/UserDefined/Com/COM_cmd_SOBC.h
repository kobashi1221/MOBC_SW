/**
  *****************************************************************************************
  * @file    : COM1_debug_RXPICUART.h
  * @author  : Takumi NORO
  * @version : 1.0
  * @date    : 2020/08/20
  * @brief   :　RXPICとのUART通信用
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

#ifndef COM1_SOBC_H_
#define COM1_SOBC_H_




#include <src_core/System/ApplicationManager/app_info.h>
#include "../../DriverInstances/di_sobc.h"
#include "../../../Drivers/Com/SOBC/SOBC.h"
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/TimeManager/time_manager.h>

CCP_CmdRet Cmd_SOBC_DEBUG_SPI_Send(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_SOBC_Reset(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_DEBUG_GPIO(const CommonCmdPacket* packet);
//CCP_CmdRet Cmd_DEBUG_regular_nop(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_DEBUG_regular_tlm_on(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_DEBUG_anomaly_count(const CommonCmdPacket* packet);
AppInfo SOBC_Regular_Tlm(void);

void SOBC_regular_tlm(void);

#endif

