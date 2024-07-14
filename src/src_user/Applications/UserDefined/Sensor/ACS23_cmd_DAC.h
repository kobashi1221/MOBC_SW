/*
 * ACS23_cmd_DAC.h
 *
 *  Created on: 2023/06/14
 *      Author: daiki
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_SENSOR_ACS23_CMD_DAC_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_SENSOR_ACS23_CMD_DAC_H_

#include "src_user/Drivers/Sensor/DAC.h"
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

extern const DAC_STRUCT* dac_struct;

AppInfo ACS23_DAC_update(void);


#endif /* SRC_USER_APPLICATIONS_USERDEFINED_SENSOR_ACS23_CMD_DAC_H_ */
