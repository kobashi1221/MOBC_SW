/*
 * AOCS_debug_Bdot.h
 *
 *  Created on: 2021/03/27
 *      Author: masud
 */

#ifndef C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_BDOT_H_
#define C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_BDOT_H_


#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


typedef struct
{
	float Mag_moment[3];
} BDOT_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo Bdot_param_update(void);
AppInfo Bdot_update(void);

AppInfo deltaB_update(void);

#ifdef __cplusplus
}
#endif

#endif /* C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_BDOT_H_ */
