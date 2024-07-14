/*
 * COM_cmd_Large_packet.h
 *
 *  Created on: 2022/06/29
 *      Author: sakaguchi
 */

#ifndef COM_CMD_LRAGE_PACKET_H_
#define COM_CMD_LRAGE_PACKET_H_


#include <src_core/System/ApplicationManager/app_info.h>
#include "../../DriverInstances/di_cobc.h"
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

AppInfo COM_large_packet_param(void);

CCP_CmdRet Cmd_change_large_packet_common_header(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_change_large_packet_tlm_header(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_change_large_packet_cmd_header(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_large_packet_enable(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_large_packet_SRAM_analyze(const CommonCmdPacket* packet);

#endif
