/*
 * ODS_cmd_BallCoef.h

 *
 *  Created on: 2022/07/19
 *      Author: Kanie
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_BALLCOEF_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_BALLCOEF_H_


#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


typedef struct{
	float Ballistic_Coefficient;
	float Cd;	// 抗力係数
}BALLCOEF_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo BallCoef_param_update(void);
AppInfo BallCoef_update(void);

#ifdef __cplusplus
}
#endif

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_BALLCOEF_H_ */
