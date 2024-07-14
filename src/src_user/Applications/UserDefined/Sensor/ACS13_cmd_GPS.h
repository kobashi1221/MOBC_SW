/*
 * ACS1_debug_GPS.h
 *
 *  Created on: 2020/11/27
 *      Author: masudahiroaki
 */

#pragma once
#ifndef ACS13_DEBUG_GPS_H_
#define ACS13_DEBUG_GPS_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_user/Drivers/Cdh/RTC/RTC.h>
//#include "src_user/IfWrapper/HAL.h"

typedef struct
{
	uint8_t GPS_data[1024];
	uint8_t GPS_Status;
	uint8_t GPS_Mode_indicator;     //GNRMCにおけるMode indicator
	uint8_t GPS_Navigation_status;  //GNRMCにおけるNavigation indicator
	uint8_t GPS_SyncFlag;
	float Altitude; //m
	float Latitude; //deg
	float Longitude;//deg
	float ECEF[3];
	float ECI[3];
	float Vel_ECI[3];
	float vel_norm; //km/s
	uint32_t GPS_UTC;
	uint16_t GPS_UTC_YEAR;
	uint8_t GPS_UTC_YEAR_L2;
	uint8_t GPS_UTC_MONTH;
	uint8_t GPS_UTC_DAY;
	uint8_t GPS_UTC_HOUR;
	uint8_t GPS_UTC_MIN;
	uint8_t GPS_UTC_SEC;

	uint8_t RTC_UTC_Permission;
    cycle_t last_recv_time;//サイクル数での最終受信時刻も保持
	RTC_Info last_recv_rtc;//高精度な最終受信時刻が軌道計算には必要⇒msまで持ってるRTC_Infoで値保持

	uint8_t GPS_valid_flag;      // SGP4/GPSフラグ


}GPS_SENSOR_STRUCT;

extern const GPS_SENSOR_STRUCT* p_gps_sensor;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo ACS13_GPS_update(void);
AppInfo ACS13_GPS_param_update(void);
CCP_CmdRet Cmd_ACS13_GPS_SET_RTC_BY_UTC(const CommonCmdPacket* packet);

#ifdef __cplusplus
}
#endif


#endif /* C2A_APPLICATIONS_DRIVERINSTANCES_NU1_ACS_ACS1_DEBUG_GPS_H_ */
