/**
 * @file   block_command_handler.h
 * @brief  自動モード遷移
 * @Authe Yuta Sakaguchi
 */
#ifndef MODE_AUTO_TRANSITION_H_
#define MODE_AUTO_TRANSITION_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include "src_user/Drivers/CDH/CDH_Mode_Transition_driver.h"

extern MODE_AUTO_TRANSITION* const mode_trans_driver;

AppInfo CDH_mode_param_update(void);
AppInfo CDH_mode_auto_transition(void);
CCP_CmdRet Cmd_MODE_AUTO_Transition(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MODE_auto_flag_setting(const CommonCmdPacket* packet);

#endif
