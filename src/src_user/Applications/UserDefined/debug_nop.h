/*
 * debug_nop.h
 *
 *  Created on: 2023/07/14
 *      Author: sakaguchi
 */
#ifndef DEBUG_NOP_H_
#define DEBUG_NOP_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

CCP_CmdRet Cmd_debug_nop(const CommonCmdPacket* packet);

#endif
