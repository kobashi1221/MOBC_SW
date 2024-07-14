/*
 * CDH_cmd_c2a_repro.h
 *
 *  Created on: 2021/12/22
 *      Author: SAT37
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_CDH_C2A_REPRO_CDH_CMD_C2A_REPRO_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_CDH_C2A_REPRO_CDH_CMD_C2A_REPRO_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

AppInfo CDH_FU_vma_update(void);
AppInfo CDH_FU_cmd_table_update(void);
AppInfo CDH_FU_am_ais_update(void);
AppInfo CDH_FU_tlm_table_update(void);

CCP_CmdRet Cmd_CDH_set_code_to_SD(const CommonCmdPacket* packet);

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_CDH_C2A_REPRO_CDH_CMD_C2A_REPRO_H_ */
