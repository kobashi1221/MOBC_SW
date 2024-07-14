#include "ADS_cmd_IGRF.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/IGRF.h>
#include <src_user/Library/General/DBG_printf.h>



static IGRF_CLASS IGRF_class;
static IGRF_STRUCT IGRF_;
//const IGRF_STRUCT* IGRF;

static void IGRF_load_param(void);
static void IGRF_initialize_param(void);

static void IGRF_update_(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo IGRF_param_update(void)
{
    return AI_create_app_info("IGRF_PARAM", IGRF_load_param, IGRF_initialize_param);
}

AppInfo IGRF_update(void)
{
    return AI_create_app_info("IGRF", NULL, IGRF_update_);
}


/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void IGRF_load_param(void)
{
//    printf("sample driver initialize.\n");
    g_ads.p_IGRF = &IGRF_;

    read_param(&IGRF_class.gnm_igrf[0][0], &IGRF_CLASS::gnm_igrf_[0][0], (size_t)(sizeof(IGRF_class.gnm_igrf)), (uint32_t)0x00005000);
//    printf("param:\r\n");
//    for(int i=0; i < 13; i++) {
//		for(int j=0; j < 14; j++) {
//			printf("gnm[%d][%d] = %f\n", i, j,IGRF_class.gnm_igrf[i][j]);
//		}
//	}
    read_param(&IGRF_class.gtnm_igrf[0][0], &IGRF_CLASS::gtnm_igrf_[0][0], (size_t)(sizeof(IGRF_class.gtnm_igrf)), (uint32_t)0x000052D8);
    read_param(&IGRF_class.hnm_igrf[0][0], &IGRF_CLASS::hnm_igrf_[0][0], (size_t)(sizeof(IGRF_class.hnm_igrf)), (uint32_t)0x000053F8);
//    printf("param:\r\n");
//    for(int i=0; i < 13; i++) {
//		for(int j=0; j < 14; j++) {
//			printf("hnm[%d][%d] = %f\n", i, j,IGRF_class.hnm_igrf[i][j]);
//		}
//	}
    read_param(&IGRF_class.htnm_igrf[0][0], &IGRF_CLASS::htnm_igrf_[0][0], (uint16_t)(sizeof(IGRF_class.htnm_igrf)), (uint32_t)0x000056D0);
    read_param(&IGRF_class.legendre_const[0][0], &IGRF_CLASS::legendre_const_[0][0], (uint16_t)(sizeof(IGRF_class.legendre_const)), (uint32_t)0x000057F0);

}

/**
 * @brief アプリケーション実行関数
 *
 */
static void IGRF_initialize_param(void)
{
    printf("IGRF initialize param.\n");
    write_param(&IGRF_class.gnm_igrf[0][0], &IGRF_CLASS::gnm_igrf_[0][0], (uint16_t)(sizeof(IGRF_class.gnm_igrf)), (uint32_t)0x00005000);
    write_param(&IGRF_class.gtnm_igrf[0][0], &IGRF_CLASS::gtnm_igrf_[0][0], (uint16_t)(sizeof(IGRF_class.gtnm_igrf)), (uint32_t)0x000052D8);
    write_param(&IGRF_class.hnm_igrf[0][0], &IGRF_CLASS::hnm_igrf_[0][0], (uint16_t)(sizeof(IGRF_class.hnm_igrf)), (uint32_t)0x000053F8);
    write_param(&IGRF_class.htnm_igrf[0][0], &IGRF_CLASS::htnm_igrf_[0][0], (uint16_t)(sizeof(IGRF_class.htnm_igrf)), (uint32_t)0x000056D0);
    write_param(&IGRF_class.legendre_const[0][0], &IGRF_CLASS::legendre_const_[0][0], (uint16_t)(sizeof(IGRF_class.legendre_const)), (uint32_t)0x000057F0);
}


static void IGRF_update_(void)
{
	//名大2号館座標
	//35.15563628207728 度(DD), 136.96494858160563度(DD)
	float in_latitude = g_ods.p_ODS->Latitude * M_PI / 180.0f; //rad
	float in_longitude = g_ods.p_ODS->Longitude * M_PI / 180.0f; //rad
	float in_altitude = g_ods.p_ODS->Altitude * 0.001f; //km
	//float in_julian_date = 59317.0; //2021/04/13
	int in_max_n = 13;

	IGRF_class.IGRF_ICRF(in_latitude, in_longitude, in_altitude, float(g_ads.p_JULIANDATE->juliandate), in_max_n, g_ads.p_GST->gst);

	//もしグローバルな認識磁場値なる格納場所ができれば、そちらに格納先を変更する
	//現状、クラスのメンバを同名の構造体にコピーしているだけ
	//実行した計算則でそれぞれ認識値を書き換えに行けばいい
	copy(IGRF_class.B_ICRF, 3, 1, IGRF_.IGRF_B);
	print(IGRF,"IGRF_B = %f\n", IGRF_class.B_ICRF[0]);

	for(int i = 0; i < 3; i++){
		IGRF_.IGRF_B_Norm[i] = IGRF_.IGRF_B[i] / Fabs(IGRF_.IGRF_B, 3);
		print(IGRF,"Bnorm[%d] = %f.\n", i, IGRF_.IGRF_B_Norm[i]);
	}

	Normalization(IGRF_.IGRF_B, 3, IGRF_.IGRF_B_Norm);
	print(IGRF,"Bnorm = %f\n", IGRF_.IGRF_B_Norm[0]);
}
