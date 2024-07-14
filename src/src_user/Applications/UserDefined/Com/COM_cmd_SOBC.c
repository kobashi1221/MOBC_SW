/*
 * COM_cmd_SOBC.c
 *
 *  Created on: 2023/08/28
 *      Author: kobayashi
 */

#include "COM_cmd_SOBC.h"
#include <stdio.h> //for printf
#include <string.h> //for memcpy
#include <stdlib.h> //for malloc
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/middleware/MemoryMW.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/TlmCmd/block_command_definitions.h>

uint16_t counter;
AppInfo SOBC_Regular_Tlm(void){
  return AI_create_app_info("sobc_regular_tlm_ganerate", NULL, SOBC_regular_tlm);
}

CCP_CmdRet Cmd_SOBC_DEBUG_SPI_Send(const CommonCmdPacket* packet){
    printf("SOBC_SEND\r\n");
    for(int i = 0;i < 256; i++){
        sobc_driver->spi_send_data[i] = i+1;
    }

    SOBC_SendReceive(sobc_driver, sobc_driver->spi_send_data, 256);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_SOBC_Reset(const CommonCmdPacket* packet){
  SOBC_reset(sobc_driver);
  printf("SOBC RESET\r\n");
  sobc_driver->sobc_anomaly.com_content_anomary = 0;
  sobc_driver->sobc_anomaly.com_anomaly_counter = 0;
  sobc_driver->sobc_anomaly.com_exe_counter = 0;
  sobc_driver->sobc_anomaly.com_success_counter = 0;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_DEBUG_GPIO(const CommonCmdPacket* packet){
  SOBC_debug_GPIO(sobc_driver);
//  printf("SOBC GPIO DEBUG");
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


//仮の生存信号テレメをSOBCに送る関数(tasklistで回してる)
void SOBC_regular_tlm(void){
//  counter ++;
//  if(counter % 2 == 0){
//    step_t start_time = TMGR_get_master_step();
//    counter = 0;
    SOBC_Regular_Tlm_ganerator();
//    step_t finish_time = TMGR_get_master_step();
//    printf("spi com time = %d ms\r\n",(int)finish_time - (int)start_time);
//  }
}


//仮の生存信号の用コマンド(引数が1,2,3,4,...239のこのコマンドが毎周期SOBCから送られてくる)
//CCP_CmdRet Cmd_DEBUG_regular_nop(const CommonCmdPacket* packet){
////  printf("cmd_success\r\n");
//  TCP* pckt = (TCP*)packet;
//  for(int i = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN; i < TCP_MAX_LEN ; i ++){
//    if(pckt->packet[i] != i-17){
////      printf("ERROR: packet[%d] should %d but %d\r\n", i, i-17, pckt->packet[i]);
//
//      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
//    }
//  }
////  printf("DEBUG_NOP_SOBC\r\n");
////  printf("a");
//  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
//}

CCP_CmdRet Cmd_DEBUG_regular_tlm_on(const CommonCmdPacket* packet){
  const uint8_t* param =  CCP_get_param_head(packet);
  if(param[0] == 0x00){
    sobc_driver->sobc_regular_tlm_flag = 0;
    printf("regular_tlm_off\r\n");
  }
  else if(param[0] == 0x01){
    sobc_driver->sobc_regular_tlm_flag = 1;
    printf("regular_tlm_on\r\n");
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_DEBUG_anomaly_count(const CommonCmdPacket* packet){
    printf(" SEND = %lu\r\n OK_M = %lu\r\n NG_M = %lu\r\n", sobc_driver->sobc_anomaly.com_exe_counter
        ,sobc_driver->sobc_anomaly.com_success_counter,sobc_driver->sobc_anomaly.com_content_anomary);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
