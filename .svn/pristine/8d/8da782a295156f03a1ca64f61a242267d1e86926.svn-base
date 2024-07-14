/*
 * ACS_cmd_Compensation.h
 *
 *  Created on: 2022/03/24
 *      Author: sunaken
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ACS_ACS_CMD_COMPENSATION_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ACS_ACS_CMD_COMPENSATION_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


typedef struct
{
	float Mag_moment[3];

	uint8_t constResMM_flag;
	float constResMM[3];       // [Am2]
	uint8_t constShapeAn_flag;
	float constShapeAn[3][3];  // [Am2/T]
	uint8_t constEdCTorq_flag;
	float constEdCTorq[3];     // [Nm]
	float constMagMoment[3];     // [Am2]

} COMPENSATION_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo Compensation_param_update(void);
AppInfo ACS_Compesation_EarthAngularVelocity_update(void);
AppInfo ACS_Compesation_Coupling_update(void);
AppInfo ACS_Compesation_RMM_update(void);
AppInfo ACS_Compesation_EddyCurrent_update(void);
AppInfo ACS_Compesation_ConstantMagnetic_update(void);

CCP_CmdRet Cmd_ACS_Compesation_ShapeAnisortropy(const CommonCmdPacket* packet);
#ifdef __cplusplus
}
#endif




#endif /* SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ACS_ACS_CMD_COMPENSATION_H_ */
