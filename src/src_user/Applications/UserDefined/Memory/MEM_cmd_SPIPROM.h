/**
  *****************************************************************************************
  * @file    : CDH1_debug_SPIPROM.h
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/09/29
  * @brief   : SPIPROM IS25LP ドライバーインスタンス
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
#ifndef CDH1_DEBUG_SPIPROM_H_
#define CDH1_DEBUG_SPIPROM_H_

#include <stdint.h> //for uint8_t

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_user/Drivers/Cdh/SPIPROM/SPI_ROM.h>
#include <src_user/Applications/DriverInstances/di_struct.h>
#include <src_core/TlmCmd/telemetry_frame.h>
#include <src_user/Drivers/Com/Circular_buffer.h>


#ifdef __cplusplus
 extern "C" {
#endif

#define PROM_MAX_PACKET_NUM (5)
#define INSTANCE (0)
#define PER_5 (5)
#define CYCLE_10 (10)
#define CYCLE_20 (20)
#define CYCLE_30 (30)
#define CYCLE_40 (40)
#define CYCLE_50 (50)
#define CYCLE_60 (60)
#define NONE	 (65535)

typedef enum{
	P_ROM_DISALE,
	P_ROM_ENABLE
}P_ROM_SAVE_ENUM;

//TOLERANCE_NUM_DATA == sectorSize / 156
#define TOLERANCE_NUM_DATA (26)
#define START_POSITION (32)

AppInfo CDH1_spiprom_update(void);
AppInfo CDH1_spiprom_param_update(void);
AppInfo CDH1_spiprom_queue_save_update(void);
AppInfo CDH1_spiprom_condition_save(void);
AppInfo CDH1_spiprom_operation_update(void);
AppInfo CDH1_spiprom_check_ready(void);


CCP_CmdRet Cmd_MEM_spiprom_write_data(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_spiprom_write_buffer_data(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_spiprom_erase_Sectors(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_spiprom_read_page(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_spiprom_send_condition(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_spiprom_Generate_RPTLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_spiprom_ALL_Reset(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_SPIPROM_memory_patrol(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_SPIPROM_set_patrol_block(const CommonCmdPacket* packet);



#ifdef __cplusplus
}
#endif


#endif // CDH1_DEBUG_SPIPROM_H_
