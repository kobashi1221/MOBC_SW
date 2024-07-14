/* COM1_debug_TXPICUART.h*/



/**
  *****************************************************************************************
  * @file    : COM1_debug_TXPICUART.h
  * @author  : Takumi NORO
  * @version : 1.0
  * @date    : 2020/08/20
  * @brief   :　TXPICとのUART通信用
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

#ifndef COM1_TXPIC_H_
#define COM1_TXPIC_H_

#include <stdint.h> //for uint8_t

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../DriverInstances/di_txpic.h"

AppInfo COM_txpic_clear_count(void);
CCP_CmdRet Cmd_COM_scam_downlink(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COM_picture_data_resend(const CommonCmdPacket* packet);
/************************************** END OF FILE **************************************/


#endif
