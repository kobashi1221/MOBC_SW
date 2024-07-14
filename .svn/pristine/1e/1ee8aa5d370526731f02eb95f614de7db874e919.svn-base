/*
 * ODS_cmd_BallCoef.cpp
 *
 *  Created on: 2022/07/19
 *      Author: Kanie
 */

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_BallCoef.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Drivers/Aocs/ProjectedArea.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.h>

static PROJECTEDAREA_CLASS ProjectedArea_class;
static BALLCOEF_STRUCT BallCoef_;

static void BallCoef_load_param(void);
static void BallCoef_initialize_param(void);

static void ODS_BallCoef_init_(void);
static void ODS_BallCoef_update_(void);

const float Cd_ = 2.2f;	// 抗力係数初期値

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo BallCoef_param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_BALLCOEF", BallCoef_load_param, BallCoef_initialize_param);
}

AppInfo BallCoef_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_BALLCOEF", ODS_BallCoef_init_, ODS_BallCoef_update_);
}


/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void BallCoef_load_param(void)
{
	printf("BallCoef_load_param\r\n");
    read_param(&BallCoef_.Cd, &Cd_, (uint16_t)(sizeof(BallCoef_.Cd)), (uint32_t)0x00008110);
}

/**
 * @brief アプリケーション実行関数
 *
 */
static void BallCoef_initialize_param(void)
{
	printf("BallCoef_initialize_param\r\n");
    write_param(&BallCoef_.Cd, &Cd_, (uint16_t)(sizeof(BallCoef_.Cd)), (uint32_t)0x00008110);
}

static void ODS_BallCoef_init_(void){
	g_ods.p_BALLCOEF = &BallCoef_;
}

static void ODS_BallCoef_update_(void)
{
	float mass = 0.0f;
	float velocity_i[3] = {g_ods.p_ODS->Velocity[0], g_ods.p_ODS->Velocity[1], g_ods.p_ODS->Velocity[2]};
	float velocity_b[3];

	multiplication(&g_ads.p_ADS->DCM_i2b[0][0], 3, 3, velocity_i, 3, 1, velocity_b, 3, 1);

//	printf("Quaternion: %f %f %f %f ", g_ads.p_TRIAD->quaternion[0], g_ads.p_TRIAD->quaternion[1], g_ads.p_TRIAD->quaternion[2], g_ads.p_TRIAD->quaternion[3]);
//	printf("velocity_i: %f %f %f ", velocity_i[0], velocity_i[1], velocity_i[2]);
//	printf("velocity_b: %f %f %f ", velocity_b[0], velocity_b[1], velocity_b[2]);

	if (Separated()){

#ifdef TWO_U
		mass = 2.3753;	// 2U
		ProjectedArea_class.cal(velocity_b, (char*)"2U");
#endif

#ifdef ONE_U
		mass = 1.2051;	// 1U
		ProjectedArea_class.cal(velocity_b, (char*)"1U");
#endif

	}else{
		mass = 4.382;	// 3U
		ProjectedArea_class.cal(velocity_b, (char*)"3U");
	}

	BallCoef_.Ballistic_Coefficient = BallCoef_.Cd * ProjectedArea_class.OUT_area / mass;
	GLOBAL_ODS_set_BallCoef(BallCoef_.Ballistic_Coefficient);

//	printf("Cd: %f\r\n", BallCoef_.Cd);
//	printf("Area: %f\r\n", ProjectedArea_class.OUT_area);
//	printf("mass: %f\r\n", mass);
//	printf("BC: %f\r\n", BallCoef_.Ballistic_Coefficient);
}


