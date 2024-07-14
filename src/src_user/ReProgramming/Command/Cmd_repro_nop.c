/*
 * Cmd_repro_nop.c
 *
 *  Created on: 2023/12/14
 *      Author: daiki
 */

#include "Cmd_repro_nop.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "stdio.h"

uint8_t repro_nop_sample = 10;
float repro_nop_float = 1.0;

void CDH_repro_nop_init_(void) {
  printf("repro nop init\r\n");
}

void CDH_repro_nop_update_(void){
  printf("repro nop %d, %f\r\n", repro_nop_sample, repro_nop_float);
}

CCP_CmdRet Cmd_Repro_NOP(const CommonCmdPacket* packet)
{
  uint8_t sample_param = CCP_get_param_from_packet(packet, 0, uint8_t);
  printf("repro nop command %d\r\n", sample_param);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


