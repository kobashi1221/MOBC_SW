/*
 * ADS_cmd_GST.h
 *
 *  Created on: 2021/06/23
 *      Author: inakawa
 */

#ifndef C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_GST_H_
#define C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_GST_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

typedef struct
{
	//----グローバル格納値---------
	float gst; //rad

} GST_STRUCT;

typedef struct
{
	//----グローバル格納値---------
	double juliandate;
	
} JULIANDATE_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo GST_update(void);

#ifdef __cplusplus
}
#endif



#endif /* C2A_APPLICATIONS_USERDEFINED_ADS_ADS_CMD_GST_H_ */
