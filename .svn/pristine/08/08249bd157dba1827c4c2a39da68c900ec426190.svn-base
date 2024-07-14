/*
 * ACS_cmd_EarthVector.cpp
 *
 *  Created on: 2021/11/14
 *      Author: masud
 */

#include "ACS_cmd_EarthVector.h"
#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../../Global_Sensor_Value.h"
#include <src_core/System/TimeManager/obc_time.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/EarthVector.h>
#include <src_user/Drivers/Sensor/GPS.h>


static EARTHVECTOR_CLASS EARTHVECTOR_class;
static EARTHVECTOR_STRUCT EARTHVECTOR_;


static void EarthVector_load_param(void);
static void EarthVector_initialize_param(void);

static void EarthVector_update_(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo EarthVector_param_update(void)
{
    return AI_create_app_info("EarthVector_PARAM", EarthVector_load_param, EarthVector_initialize_param);
}

AppInfo EarthVector_update(void)
{
    return AI_create_app_info("EarthVector", NULL, EarthVector_update_);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void EarthVector_load_param(void)//init
{
    g_acs.p_EARTHVECTOR = &EARTHVECTOR_;
    read_param(EARTHVECTOR_class.Target_Position,  EARTHVECTOR_CLASS::Target_Position_, (size_t)sizeof(EARTHVECTOR_class.Target_Position), (uint32_t)0x00007070);
    read_param(&EARTHVECTOR_class.Target_Phi,  &EARTHVECTOR_CLASS::Target_Phi_, (size_t)sizeof(EARTHVECTOR_class.Target_Phi), (uint32_t)0x0000707C);
    read_param(&EARTHVECTOR_class.Target_Lam,  &EARTHVECTOR_CLASS::Target_Lam_, (size_t)sizeof(EARTHVECTOR_class.Target_Lam), (uint32_t)0x00007080);
    read_param(&EARTHVECTOR_class.Target_Height,  &EARTHVECTOR_CLASS::Target_Height_, (size_t)(sizeof(EARTHVECTOR_class.Target_Height)), (uint32_t)0x00007084);

}

/**
 * @brief アプリケーション実行関数
 *
 */
static void EarthVector_initialize_param(void)//exe
{
	 printf("EarthVector initialize param.\n");
	 write_param(EARTHVECTOR_class.Target_Position, EARTHVECTOR_CLASS::Target_Position_, (size_t)(sizeof(EARTHVECTOR_class.Target_Position)), (uint32_t)0x00007070);
	 write_param(&EARTHVECTOR_class.Target_Phi, &EARTHVECTOR_CLASS::Target_Phi_, (size_t)(sizeof(EARTHVECTOR_class.Target_Phi)), (uint32_t)0x0000707C);
	 write_param(&EARTHVECTOR_class.Target_Lam, &EARTHVECTOR_CLASS::Target_Lam_, (size_t)(sizeof(EARTHVECTOR_class.Target_Lam)), (uint32_t)0x00007080);
	 write_param(&EARTHVECTOR_class.Target_Height, &EARTHVECTOR_CLASS::Target_Height_, (size_t)(sizeof(EARTHVECTOR_class.Target_Height)), (uint32_t)0x00007084);
}


static void EarthVector_update_(void)
{
//	float DCM[3][3] = {{0.0f, 0.0f, 1.0f},
//					   {1.0f, 0.0f, 0.0f},
//			           {0.0f, 1.0f, 0.0f}};
//	float gst = 2.0573;
//	float ECEF[3] = {-2578854.726f, 3135514.999f, -2381994.121f};


	EARTHVECTOR_class.EarthVector_calc(*g_ads.p_ADS->DCM_i2b, g_ads.p_GST->gst, g_ods.p_ODS->Position, EARTHVECTOR_class.Target_Position);
//	EARTHVECTOR_class.EarthVector_calc(*DCM, gst, ECEF);

	copy(EARTHVECTOR_class.EarthVector_Body, 3, 1, EARTHVECTOR_.EarthCenterVector_Body);
	copy(EARTHVECTOR_class.EarthVector_Point_Body, 3, 1, EARTHVECTOR_.EarthPointVector_Body);
}


