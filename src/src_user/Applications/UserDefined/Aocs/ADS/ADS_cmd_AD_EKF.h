/*
 * ADS_cmd_AD_EKF.h
 *
 *  Created on: 2021/06/23
 *      Author: masud
 */

#ifndef C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_AD_EKF_H_
#define C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_AD_EKF_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


typedef struct
{
	//----事前推定値---------
	float x_ref_pre[7];
	float P_ref_pre[6][6];
	//----------------------

	//----観測更新-----------
	float x_ref_renew[7];
	float P_ref_renew[6][6];
	//-----------------------

	//-----推定値------------
	float x_ref_next[7];
	float P_ref_next[6][6];
	//-----------------------
} AD_EKF_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo AD_EKF_update(void);

CCP_CmdRet Cmd_ADS_AD_EKF(const CommonCmdPacket* packet);


#ifdef __cplusplus
}
#endif



#endif /* C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_AD_EKF_H_ */
