#ifndef PHASE_SETTING_H_
#define PHASE_SETTING_H_
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include "phase_setting_driver.h"
#ifdef __cplusplus
 extern "C" {
#endif
AppInfo phase_param_update(void);
CCP_CmdRet Cmd_chage_phase_temp(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_mode_transition_tabel_load(const CommonCmdPacket* packet);

extern PHASE_STRUCT* const p_phase_driver;

#ifdef __cplusplus
 }
#endif

#endif
