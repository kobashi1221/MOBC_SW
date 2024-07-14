#pragma section REPRO
/*
 * debug_nop.c
 *
 *  Created on: 2023/07/14
 *      Author: saakguchi
 */
#include "debug_nop.h"
#include <stddef.h>
#include <stdio.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

CCP_CmdRet Cmd_debug_nop(const CommonCmdPacket* packet)
{
  (void)packet;
  printf("CMD_NOP\r\n");
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
