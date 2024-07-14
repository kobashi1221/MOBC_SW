/*
 * ADS_cmd_SunVector.cpp
 *
 *  Created on: 2021/08/09
 *      Author: inakawa
 */

#include "ADS_cmd_SunVector.h"
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
#include <src_user/Drivers/Aocs/SunVector.h>

#include <src_user/Library/General/DBG_printf.h>



static SUNVECTOR_CLASS SUNVECTOR_class;
static SUNVECTOR_STRUCT SUNVECTOR_;
//const GST_STRUCT* GST;

static void SunVector_load_param(void);
static void SunVector_initialize_param(void);

static void SunVector_update_(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo SunVector_param_update(void)
{
    return AI_create_app_info("DBG_SunVector", SunVector_load_param, SunVector_initialize_param);
}

AppInfo SunVector_update(void)
{
    return AI_create_app_info("DBG_SunVector", NULL, SunVector_update_);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void SunVector_load_param(void)//init
{
    g_ads.p_SUNVECTOR = &SUNVECTOR_;
    read_param(&SUNVECTOR_class.SunCoef[0][0], &SUNVECTOR_CLASS::SunCoef_[0][0], (size_t)(sizeof(SUNVECTOR_class.SunCoef)), (uint32_t)0x00005B00);
}

/**
 * @brief アプリケーション実行関数
 *
 */
static void SunVector_initialize_param(void)//exe
{
	 printf("SunVector initialize param.\n");
	 write_param(&SUNVECTOR_class.SunCoef[0][0], &SUNVECTOR_CLASS::SunCoef_[0][0], (size_t)(sizeof(SUNVECTOR_class.SunCoef)), (uint32_t)0x00005B00);

}


static void SunVector_update_(void)
{

	//太陽方向を計算しグローバルで参照可能なよう格納する
	SUNVECTOR_class.SunVector_calc(g_ads.p_JULIANDATE->juliandate);

	copy(SUNVECTOR_class.SunVector_ICRF, 3, 1, SUNVECTOR_.sunvector);

	//DCMをかけて機体座標系に変換
	multiplication(&g_ads.p_ADS->DCM_i2b[0][0], 3, 3, SUNVECTOR_class.SunVector_ICRF, 3, 1, SUNVECTOR_.SunVector_Body, 3, 1);
}


