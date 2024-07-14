/**
 * @file RTC_handler.h
 * @brief
 */

#include "../../../Drivers/CDH/RTC/RTC.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

extern RTC_Info* const p_rtc_struct;

AppInfo RTC_update(void);

CCP_CmdRet Cmd_RTC_SET(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_RTC_calib(const CommonCmdPacket* packet);
