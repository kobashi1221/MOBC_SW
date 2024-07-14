/*
 * ACS23_cmd_PLASMA.h
 *
 *  Created on: 2023/06/14
 *      Author: daiki
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_SENSOR_ACS23_CMD_PLASMA_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_SENSOR_ACS23_CMD_PLASMA_H_


#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

typedef struct{

}PLASMA_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

void ACS23_PLASMA_init(void);
void ACS23_PLASMA_off(void);
AppInfo ACS23_PLASMA_update(void);
CCP_CmdRet Cmd_PLASMA_change_voltage(const CommonCmdPacket* packet);

#ifdef __cplusplus
 }
#endif


#endif /* SRC_USER_APPLICATIONS_USERDEFINED_SENSOR_ACS23_CMD_PLASMA_H_ */
