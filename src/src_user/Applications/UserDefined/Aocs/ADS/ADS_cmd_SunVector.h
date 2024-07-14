/*
 * ADS_cmd_SunVector.h
 *
 *  Created on: 2021/0809
 *      Author: inakawa
 */

#ifndef C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_SUNVECTOR_H_
#define C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_SUNVECTOR_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


typedef struct
{
	//----グローバル格納値---------
	float sunvector[3];
	float SunVector_Body[3];
} SUNVECTOR_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

AppInfo SunVector_param_update(void);
AppInfo SunVector_update(void);

#ifdef __cplusplus
}
#endif



#endif /* C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_SUNVECTOR_H_ */
