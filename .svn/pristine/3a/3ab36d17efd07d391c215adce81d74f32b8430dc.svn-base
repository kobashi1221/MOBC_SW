#pragma once
#ifndef ACS3_CMD_SUBCAM_H_
#define ACS3_CMD_SUBCAM_H_
#ifdef ONE_U
//#include <src_user/IfWrapper/HAL.h>
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_user/Drivers/Sensor/SUBCAM.h>
#include "src_user/Drivers/Cdh/Com_Fault_Detection_driver.h"


#ifdef __cplusplus
 extern "C" {
#endif

#define ANALYZE_SIZE 512

AppInfo ACS3_SUBCAM_update(void);
AppInfo ACS3_SUBCAM_param_update(void);
AppInfo ACS3_SUBCAM_move_data(void);

CCP_CmdRet Cmd_ACS3_SUBCAM(const CommonCmdPacket* packet); //ここで全ての処理を行っています
CCP_CmdRet Cmd_ACS3_SUBCAM_initialize(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS3_SUBCAM_take_pic(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS3_SUBCAM_read_reg(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CAMaddr_Renew(const CommonCmdPacket* packet);
CCP_CmdRet SDcontents_init(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS3_SUBCAM_re_initialize(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_ACS3_SUBCAM_default_setting(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS3_SUBCAM_SD_to_SRAM(const CommonCmdPacket* packet);


#ifdef __cplusplus
}
#endif

#endif/* for 1U*/

#endif // SAMPLE_DRIVER_H_
