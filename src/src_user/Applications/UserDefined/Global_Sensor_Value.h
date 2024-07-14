/**
  *****************************************************************************************
  * @file    : NU1_globalValDefine.h
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/10/08
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  * This file defines parameters regarding pin assigns of General Classes of GPIO, UART, I2C, SPI.
  * Include this header file and use those.
  * If Pin Assign was changed, refer "main.h" and modify this source file.
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/10/08  1.0   N.KISHI         First revision
  *
  *****************************************************************************************
  */

#ifndef NU1_GLOBAL_VAL_DEFINE_H_
#define NU1_GLOBAL_VAL_DEFINE_H_


#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_ADC_MOBC.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_GPIO_input_MOBC.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_FETREG.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_IOEX_PWR2.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_ADC_PWR2.h>
#include <src_user/Applications/UserDefined/sample_code.h>
#include <src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.h>
#include <src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.h>
#include "Sensor/ACS1_cmd_ADC.h"
#include "Sensor/ACS3_cmd_Gyro.h"
#include "Sensor/ACS23_cmd_9AX.h"

typedef struct{
	// *** PWR *** //
//	const PWR_FETREG_STRUCT* p_pwr_fetreg;          // MOBCでFET操作するコンポのON/OFF状態
//	const PWR_IOEXPANDER_STRUCT* p_pwr_IOEXPANDER;  // IOEXPANDER_PWR2でFET操作するコンポのON/OFF状態
	const PWR_STATE_STRUCT* p_pwr_state;
	const uint8_t* p_pwr_spc_guard; // SEPCOILの誤操作防止
	const PWR_ADC_MOBC_STRUCT* p_pwr_adc_mobc;      // ADC_MOBCのADC値
	const PWR_GPIO_INPUT_MOBC_STRUCT* p_pwr_gpio_input_mobc;      // GPIOinput_MOBCのH/L
	const PWR_ADC_PWR2_STRUCT* p_pwr_adc_pwr2;      // ADC_PWR2のADC値


	// *** ACS *** //
	const ADC_STRUCT* p_adc;
	const SUN_SENSOR_STRUCT* p_suns;
	const IMU_SENSOR_STRUCT* p_imu;
	const GPS_SENSOR_STRUCT* p_gps;

	#ifdef ONE_U
	const ACS3_DEBUG_GYRO_STRUCT* p_gyro;
// NUcube1+ではMOBCでチップサンセンサを使用しない
//	const CSUN_SENSOR_STRUCT* p_csuns;
	#endif

    const GLOBAL_AOCS_PARAMETER_STRUCT* p_GLOBAL_AOCS;

}Global_SENSOR_STRUCT;


// *** global_sensor_value *** //
extern Global_SENSOR_STRUCT g_sensor;

#endif // NU1_GLOBAL_VAL_DEFINE_H_
