/**
  *****************************************************************************************
  * @file    : NU1_globalValDefine.cpp
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
#include "GlobalMemoryParam.h"

#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include "Global_Sensor_Value.h"

Global_SENSOR_STRUCT g_sensor;

Global_ADS_STRUCT g_ads;
Global_ACS_STRUCT g_acs;
Global_ODS_STRUCT g_ods;
Global_OCS_STRUCT g_ocs;

extern PIC_PARAM_STRUCT _sdata_boot;
const PIC_PARAM_STRUCT* pic_data = &_sdata_boot;	//RAMの先頭アドレス
