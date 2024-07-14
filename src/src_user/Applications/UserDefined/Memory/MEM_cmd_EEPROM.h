/**
  *****************************************************************************************
  * @file    : CDH1_debug_EEPROM.h
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/06/28
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
  * 2020/06/28  1.0   N.KISHI         First revision
  *
  *****************************************************************************************
  */

#pragma once
#ifndef CDH1_DEBUG_EEPROM_H_
#define CDH1_DEBUG_EEPROM_H_

#include <stdint.h> //for uint8_t

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo EEPROM_param_update(void);
AppInfo EEPROM_memory_patorol(void);

CCP_CmdRet Cmd_MEM_eeprom_writeData(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_eeprom_ReadData(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_eeprom_set_patrol_addr(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_eeprom_reset(const CommonCmdPacket* packet);


#ifdef __cplusplus
}
#endif

#endif // CDH1_DEBUG_EEPROM_H_
