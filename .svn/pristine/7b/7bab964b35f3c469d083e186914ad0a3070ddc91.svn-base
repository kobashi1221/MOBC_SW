/*
 * AOCS_debug_CrossProduct.h
 *
 *  Created on: 2021/03/27
 *      Author: masud
 */

#ifndef C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_CROSSPRODUCT_H_
#define C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_CROSSPRODUCT_H_



#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


typedef struct
{
	float Mag_moment[3];
} CROSS_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo CrossProduct_param_update(void);
AppInfo ACS_CrossProduct_AngularMomentum_update(void);
AppInfo ACS_CrossProduct_Quaternion_update(void);
AppInfo ACS_CrossProduct_Sun_Pointing_update(void);
AppInfo ACS_CrossProduct_Earth_Center_Pointing_update(void);
AppInfo ACS_CrossProduct_Earth_Point_Pointing_update(void);
AppInfo ACS_CrossProduct_Inertial_Pointing_update(void);
AppInfo ACS_CrossProduct_Velocity_Pointing_update(void);
AppInfo ACS_CrossProduct_Trajectry_Pointing_update(void);
AppInfo ACS_CrossProduct_QFB_update(void);


CCP_CmdRet Cmd_ACS_CrossProduct_SpinControl_HILS(const CommonCmdPacket* packet);

//CCP_EXEC_STS Cmd_ACS_CrossProduct_AngularMomentum_HILS(const CTCP* packet);
//CCP_EXEC_STS Cmd_ACS_CrossProduct_Quaternion(const CTCP* packet);
//CCP_EXEC_STS Cmd_ACS_CrossProduct_Quaternion_HILS(const CTCP* packet);
//CCP_EXEC_STS Cmd_ACS_CrossProduct_Pointing(const CTCP* packet);
//CCP_EXEC_STS Cmd_ACS_CrossProduct_Pointing_HILS(const CTCP* packet);
//CCP_EXEC_STS Cmd_ACS_CrossProduct_QFB(const CTCP* packet);
//CCP_EXEC_STS Cmd_ACS_CrossProduct_QFB_HILS(const CTCP* packet);


#ifdef __cplusplus
}
#endif



#endif /* C2A_APPLICATIONS_DRIVERINSTANCES_NU1_AOCS_AOCS_DEBUG_CROSSPRODUCT_H_ */
