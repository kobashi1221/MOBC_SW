/*
 * ADS_cmd_IGRF.h
 *
 *  Created on: 2021/06/13
 *      Author: masud
 */

#ifndef C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_IGRF_H_
#define C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_IGRF_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

typedef struct
{
	float IGRF_B[3];
	float IGRF_B_Norm[3];
} IGRF_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo IGRF_param_update(void);
AppInfo IGRF_update(void);

#ifdef __cplusplus
}
#endif


#endif /* C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_IGRF_H_ */
