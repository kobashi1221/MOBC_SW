/*
 * Global_AOCS_Param_Set.h
 *
 *  Created on: 2022/06/19
 *      Author: fujita
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_AOCS_GLOBAL_AOCS_PARAM_SET_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_AOCS_GLOBAL_AOCS_PARAM_SET_H_


#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo AOCS_Sensor_time_Set_update(void);
AppInfo ACS_Param_Set_update(void);
AppInfo ADS_Param_Set_update(void);
AppInfo OCS_Param_Set_update(void);
AppInfo ODS_Param_Set_update(void);
AppInfo ACS_Param_Reset(void);
AppInfo OCS_Param_Reset(void);

#ifdef __cplusplus
}
#endif

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_AOCS_GLOBAL_AOCS_PARAM_SET_H_ */

