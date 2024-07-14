/*
 * ACS_cmd_EarthVector.h
 *
 *  Created on: 2021/11/14
 *      Author: masud
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ACS_ACS_CMD_EARTHVECTOR_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ACS_ACS_CMD_EARTHVECTOR_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


typedef struct
{
	//----グローバル格納値---------
	float Earthvector[3];
	float EarthCenterVector_Body[3];
	float EarthPointVector_Body[3];
} EARTHVECTOR_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

AppInfo EarthVector_param_update(void);
AppInfo EarthVector_update(void);


#ifdef __cplusplus
}
#endif



#endif /* SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ACS_ACS_CMD_EARTHVECTOR_H_ */
