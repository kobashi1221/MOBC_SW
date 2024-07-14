/*
 * AOCS_debug_Quest.h
 *
 *  Created on: 2022/06/24
 *      Author: Kanie
 */

#ifndef C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_QUEST_H_
#define C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_QUEST_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

typedef struct
{
	float quaternion[4];
	float DCM[3][3];

} QUEST_STRUCT;


extern const QUEST_STRUCT* QUEST;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo Quest_param_update(void);
AppInfo Quest_update(void);

//CCP_EXEC_STS Cmd_ADS_Quest_HILS(const CTCP* packet);

#ifdef __cplusplus
}
#endif



#endif /* C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_QUEST_H_ */
