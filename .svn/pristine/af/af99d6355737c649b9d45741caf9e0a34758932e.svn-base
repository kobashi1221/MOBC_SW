/*
 * ACS3_debug_9AX.cpp
 *
 *  Created on: 2020/12/09
 *      Author: hiroaki MASUDA
 *
 *****************************************************************************************
 *
 * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
 *
 * explain
 *
 *
 * UPDATE HISTORY
 * Date        Rev   Editor          Description
 * 2020/12/09  1.0   H.MASUDA      First revision
 *
  *****************************************************************************************
 */


#pragma once
#ifndef ACS23_DEBUG_9AX_H_
#define ACS23_DEBUG_9AX_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include "ACS123_cmd_AnomalyDetect.h"
#include <src_user/Drivers/Cdh/CDH_Mode_Transition_driver.h>
#include <src_user/Drivers/Sensor/9AX.h>


// 個々のデバイスの情報を格納する構造体
typedef struct {

	// エラー番号の格納
	int8_t state;

	//0:X 1:Y 2:Z
	float ACC[3];
	float GYRO[3];
	float MAG[3];

	int16_t ACC_HK[3];
	int16_t GYRO_HK[3];
	int16_t MAG_HK[3];

	float MAGVec[3];

	LSM9DS1 lsm9ds1;

	ObcTime qaxsensor_time_before;
	ObcTime qaxsensor_time_now;

	ObcTime qaxsensor_time_before_hils;
	ObcTime qaxsensor_time_now_hils;

	//MRAM
	float QAX_ACC_DCM[3][3];
	float qax_acc_scale[3];
	float qax_acc_offset[3];
	float acc_sf_t0[3];
	float acc_sf_t1[3];
	float acc_sf_t2[3];
	float acc_os_t0[3];
	float acc_os_t1[3];
	float acc_os_t2[3];

	float QAX_GYRO_DCM[3][3];
	float qax_gyro_scale[3];
	float qax_gyro_offset[3];
	float gyro_sf_t0[3];
	float gyro_sf_t1[3];
	float gyro_sf_t2[3];
	float gyro_os_t0[3];
	float gyro_os_t1[3];
	float gyro_os_t2[3];

	float QAX_MAGS_DCM[3][3];
	float qax_mag_scale[3];
	float qax_mag_offset[3];
	float mag_sf_t0[3];
	float mag_sf_t1[3];
	float mag_sf_t2[3];
	float mag_os_t0[3];
	float mag_os_t1[3];
	float mag_os_t2[3];

	float QAX_GYRO_THRESHOLD[3];
	float QAX_GYRO_HIGHSPIN_THRESHOLD[3];
	uint8_t QAX_MAGS_Continuous_Time_THRESHOLD;

} IMU_SENSOR_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo ACS23_9AX_update(void);
AppInfo ACS23_9AX_param_update(void);

CCP_CmdRet Cmd_9AX_change_acc_config(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_9AX_change_gyro_config(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_9AX_change_mag_config(const CommonCmdPacket* packet);

#ifdef __cplusplus
 }
#endif

#endif /* C2A_APPLICATIONS_DRIVERINSTANCES_NU1_ACS_ACS3_DEBUG_9AX_H_ */
