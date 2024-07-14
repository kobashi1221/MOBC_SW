/*
 * AOCS_debug_TRIAD.h
 *
 *  Created on: 2021/03/27
 *      Author: masud
 */

#ifndef C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_TRIAD_H_
#define C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_TRIAD_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

typedef struct
{
	float DCM[3][3];	//出力DCM
	float quaternion[4];	//出力クォータニオン
} TRIAD_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo TRIAD_update(void);

//CCP_EXEC_STS Cmd_ADS_TRIAD_HILS(const CTCP* packet);

#ifdef __cplusplus
}
#endif



#endif /* C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_TRIAD_H_ */
