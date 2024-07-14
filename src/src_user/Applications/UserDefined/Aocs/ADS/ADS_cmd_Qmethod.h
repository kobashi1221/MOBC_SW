/*
 * AOCS_debug_Qmethod.h
 *
 *  Created on: 2021/03/27
 *      Author: masud
 */

#ifndef C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_QMETHOD_H_
#define C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_QMETHOD_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

typedef struct
{
	float quaternion[4];
	float DCM[3][3];

} QMETHOD_STRUCT;


extern const QMETHOD_STRUCT* QMETHOD;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo Qmethod_param_update(void);
AppInfo Qmethod_update(void);

//CCP_EXEC_STS Cmd_ADS_Qmethod_HILS(const CTCP* packet);

#ifdef __cplusplus
}
#endif



#endif /* C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_QMETHOD_H_ */
