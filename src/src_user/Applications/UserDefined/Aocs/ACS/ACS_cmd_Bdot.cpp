/*
 * AOCS_debug_Bdot.cpp
 *
 *  Created on: 2021/03/27
 *      Author: masud
 */

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Bdot.h>
#include <stdlib.h>
#include <string.h>
#include "../../Global_Sensor_Value.h"
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/Bdot.h>
#include <src_user/Library/General/DBG_printf.h>

static BDOT_CLASS BDOT_class;
static BDOT_STRUCT BDOT_;

static void Bdot_load_param(void);
static void Bdot_initialize_param(void);

static void Bdot_update_(void);
static void deltaB_update_(void);


/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo Bdot_param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_Bdot", Bdot_load_param, Bdot_initialize_param);
}

AppInfo Bdot_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_Bdot", NULL, Bdot_update_);
}

AppInfo deltaB_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_deltaB", NULL, deltaB_update_);
}
/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void Bdot_load_param(void)
{
//    printf("sample driver initialize.\n");
	g_acs.p_BDOT = &BDOT_;
	read_param(&BDOT_class.Kb, &BDOT_CLASS::Kb_, (uint16_t)(sizeof(BDOT_class.Kb)), (uint32_t)0x00007064);
	read_param(&BDOT_class.Kbd, &BDOT_CLASS::Kbd_, (uint16_t)(sizeof(BDOT_class.Kbd)), (uint32_t)0x000071D8);
//    printf("Kb %f %f %f\r\n", BDOT_class.Kb[0], BDOT_class.Kb[1], BDOT_class.Kb[2]);

}

static void Bdot_initialize_param(void)
{
    printf("Bdot initialize_param.\n");
    write_param(&BDOT_class.Kb, &BDOT_CLASS::Kb_, (uint16_t)(sizeof(BDOT_class.Kb)), (uint32_t)0x00007064);
    write_param(&BDOT_class.Kbd, &BDOT_CLASS::Kbd_, (uint16_t)(sizeof(BDOT_class.Kbd)), (uint32_t)0x000071D8);
}


static void Bdot_update_(void){
	///Input: 地磁気，角速度，クォータニオン
	///Output: 磁気モーメント
		float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};
		float B_fld[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
//#ifdef HILS
////		B_fld[0] = magsbuf[0]*0.000000001;
////		B_fld[1] = magsbuf[1]*0.000000001;
////		B_fld[2] = magsbuf[2]*0.000000001;
////		omega[0] = gyrobuf[0];
////		omega[1] = gyrobuf[1];
////		omega[2] = gyrobuf[2];
//#endif

		BDOT_class.cal(B_fld, omega);
		copy(BDOT_class.OUT_torque, 3, 1, BDOT_.Mag_moment);


//		GLOBAL_AOCS_set_Mag_moment(BDOT_.Mag_moment);
		GLOBAL_ACS_set_Mag_moment_Cn(BDOT_.Mag_moment);

		print(BDOT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
		print(BDOT,"MAG %f %f %f\r\n", B_fld[0], B_fld[1], B_fld[2]);
		print(BDOT,"Req'd Mag mmt %f %f %f\r\n", BDOT_.Mag_moment[0], BDOT_.Mag_moment[1], BDOT_.Mag_moment[2]);

}




static void deltaB_update_(void){
	///Input: 地磁気，角速度，クォータニオン
	///Output: 磁気モーメント
		float B_fld[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
		float B_fld_pre[3] = {g_acs.p_GLOBAL_AOCS->pre_MagField[0], g_acs.p_GLOBAL_AOCS->pre_MagField[1], g_acs.p_GLOBAL_AOCS->pre_MagField[2]};

		BDOT_class.Dcal(B_fld, B_fld_pre);
		copy(BDOT_class.OUT_torque, 3, 1, BDOT_.Mag_moment);

		GLOBAL_ACS_set_Mag_moment_Cn(BDOT_.Mag_moment);

		print(DELTA_B,"MAG %f %f %f\r\n", B_fld[0], B_fld[1], B_fld[2]);
		print(DELTA_B,"Req'd Mag mmt %f %f %f\r\n", BDOT_.Mag_moment[0], BDOT_.Mag_moment[1], BDOT_.Mag_moment[2]);
}
