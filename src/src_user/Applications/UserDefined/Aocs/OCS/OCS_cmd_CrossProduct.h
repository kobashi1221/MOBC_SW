/*
 * OCS_cmd_Deceleration.h
 *
 *  Created on: 2022/04/20
 *      Author: sunaken
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_AOCS_OCS_OCS_CMD_CROSSPRODUCT_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_AOCS_OCS_OCS_CMD_CROSSPRODUCT_H_


#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


typedef struct
{
	float Mag_moment[3];
} OCS_CROSS_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo OCS_CrossProduct_update(void);
AppInfo OCS_CrossProduct_Ave_Deceleration_update(void);
AppInfo OCS_CrossProduct_Min_Deceleration_update(void);
AppInfo OCS_CrossProduct_Max_Deceleration_update(void);
AppInfo OCS_CrossProduct_Keep_Deceleration_update(void);


#ifdef __cplusplus
}
#endif

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_AOCS_OCS_OCS_CMD_CROSSPRODUCT_H_ */
