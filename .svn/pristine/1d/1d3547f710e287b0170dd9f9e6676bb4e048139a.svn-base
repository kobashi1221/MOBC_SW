/*
 * COM_cmd_COBC.h
 *
 *  Created on: 2021/10/18
 *      Author: daiki
 *
 *  Updated on: 2021/12/22
 *      Author: Yuta Sakaguchi
 */

#ifndef COM_CMD_COBC_H_
#define COM_CMD_COBC_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include "../../DriverInstances/di_cobc.h"
#include "../../../Drivers/Com/COBC/COBC.h"
#include "../../../Drivers/Com/TXPIC/TXPIC.h"
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/System/TimeManager/time_manager.h>

typedef enum{
  NONE_TLM  = 0b00000000,
  LIFE  = 0b00000001,
  ADS   = 0b00000010,
  ACS   = 0b00000100,
  ODS   = 0b00001000,
  OCS   = 0b00010000,
  TI    = 0b00100000,
  TAMURA= 0b01000000,
  ALL   = 0b00111111,
}COBC_TLM_NUM;

typedef enum{
  MCAM_INFO = COBC_MCAM_TLM,
  SCAM_INFO = COBC_SCAM_TLM,
}COBC_INFO;

typedef enum{
  SPI_METHOD,
  UART_METHOD
}COBC_METHOD;

#ifdef TWO_U

AppInfo COBC_BOOT_param_update(void);
AppInfo COBC_com_param_update(void);
AppInfo COBC_large_packet_receive(void);
AppInfo COBC_Regular_Tlm(void);

CCP_CmdRet Cmd_COBC_Reset(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_BOOT(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_Choose_Info(const CommonCmdPacket* padket);
CCP_CmdRet Cmd_COBC_SEND_INFORMATION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_send_AOCS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_large_packet_mode_enable(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_large_packet_mode(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_large_packet_downlink(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_large_packet_all_reset(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_GPIO_in(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_COBC_GPIO_out(const CommonCmdPacket* packet);

void cobc_tlm_default_setting();
void COBC_regular_tlm(void);
#endif

#endif
