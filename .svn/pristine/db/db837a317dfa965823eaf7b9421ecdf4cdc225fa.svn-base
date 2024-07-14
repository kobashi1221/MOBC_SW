/**
  *****************************************************************************************
  * @file    : 1_debug_EEPROM.h
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/06/28
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/06/28  1.0   N.KISHI         First revision
  *
  *****************************************************************************************
  */

#pragma once
#ifndef ACS1_DEBUG_SUN_SENSOR_H_
#define ACS1_DEBUG_SUN_SENSOR_H_


#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>       //necessary to add cmd

/*以下の定義をSUNS.hから削除後、コメントアウトを外すこと  kimura*/

typedef struct {

//	// エラー番号の格納
    uint8_t state; //SUN_SENSOR_StateTypeDef

	//tunaterm
	float FILTERED_PHOTOCELL[4];
	float UNFILTERED_PHOTOCELL[4];
	float ANGULAR_POSITION[2];
	float SUNVector[3];
	int suns_ret;


} SUN_SENSOR_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

AppInfo ACS13_SUNS_update(void);
AppInfo ACS13_SUNS_param_update(void);
void ACS13_SUNS_Set_Duty_Counter(void);

#ifdef __cplusplus
 }
#endif

#endif // ACS1_DEBUG_SUN_SENSOR_H_

