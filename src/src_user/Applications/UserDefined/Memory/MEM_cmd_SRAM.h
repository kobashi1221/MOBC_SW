/**
  *****************************************************************************************
  * @file    : CDH1_debug_SRAM.h
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/09/29
  * @brief   : SRAM IS61WV5128BLL
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/09/29  1.0   N.KISHI         First revision
  *
  *****************************************************************************************
  */

#pragma once
#ifndef CDH1_DEBUG_SRAM_H_
#define CDH1_DEBUG_SRAM_H_

#include <stdint.h> //for uint8_t

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


AppInfo CDH1_debug_sram_update(void);

CCP_CmdRet Cmd_MEM_SRAM_write_data(const CommonCmdPacket* packet);

#endif // CDH1_DEBUG_SRAM_H_
