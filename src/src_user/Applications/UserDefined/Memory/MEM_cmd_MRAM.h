/**
  *****************************************************************************************
  * @file    : CDH1_debug_MRAM.h
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/09/30
  * @brief   : MRAM Application for CDH1 Board Debug
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/09/30  1.0   N.KISHI         First revision
  *
  *****************************************************************************************
  */

#pragma once
#ifndef CDH1_DEBUG_MRAM_H_
#define CDH1_DEBUG_MRAM_H_

#include <stdint.h> //for uint8_t

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


#ifdef __cplusplus
 extern "C" {
#endif

AppInfo MRAM_param_update(void);
AppInfo MRAM_memory_patorol(void);

CCP_CmdRet Cmd_MEM_mram_writeData(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_mram_ReadData(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_mram_set_patrol_addr(const CommonCmdPacket* packet);

#ifdef __cplusplus
 }
#endif

#endif // CDH1_DEBUG_MRAM_H_
