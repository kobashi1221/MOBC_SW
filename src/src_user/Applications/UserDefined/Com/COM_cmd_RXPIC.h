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

#ifndef COM1_RXPIC_H_
#define COM1_RXPIC_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../DriverInstances/di_rxpic.h"

AppInfo COM_rxpic_clear_count(void);
AppInfo COM_DBG_counter(void);

#endif
