/*
 * AOCS_debug_TRIAD.cpp
 *
 *  Created on: 2021/03/27
 *      Author: masud
 *  Reference 「人工衛星の力学と制御ハンドブック」P452
 */

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/AOCS/ADS/ADS_cmd_TRIAD.h>
#include <stdlib.h>
#include <string.h>
#include "../ADS/ADS_cmd_IGRF.h"
#include "../../Global_Sensor_Value.h"
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/Bdot.h>
#include <src_user/Drivers/Aocs/Qmethod.h>
#include <src_user/Drivers/Aocs/TRIAD.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>

static TRIAD_CLASS TRIAD_class;
static TRIAD_STRUCT TRIAD_;

static void TRIAD_init(void);
static void TRIAD_exe(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo TRIAD_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_Bdot", TRIAD_init, TRIAD_exe);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void TRIAD_init(void)
{
//    printf("sample driver initialize.\n");
	g_ads.p_TRIAD = &TRIAD_;
}

/**
 * @brief アプリケーション実行関数
 * @brief 関数の概要		TRIADを用いた姿勢決定
 * @param MagVec		磁気ベクトル（機体座標系）
 * @param SunVec		太陽ベクトル（機体座標系）
 * @param IGRF_B_Norm	磁気ベクトル（慣性座標系）
 * @param sunvector		太陽ベクトル（慣性座標系）
 * @return OUT_q		出力クォータニオン
 * @return OUT_DCM		出力DCM
 * 作成者　増田 2021/03/27
 * コメント　藤田 2023/06/22
 */
static void TRIAD_exe(void)
{

#ifndef HILS

	// 入力ベクトル：磁気ベクトル(機体座標系), 太陽ベクトル(機体座標系), 磁気ベクトル(慣性座標系), 太陽ベクトル(慣性座標系)
	TRIAD_class.cal(g_ads.p_ADS->MagVec, g_ads.p_ADS->SunVec, g_ads.p_IGRF->IGRF_B_Norm, g_ads.p_SUNVECTOR->sunvector);
#endif

#ifdef HILS


	//HILSでS2EDからの取得したデータの格納用
	float B_fld_[3] = { (float)(AOCS_hils->mags[0]*0.000000001f), (float)(AOCS_hils->mags[1] * 0.000000001f),(float)(AOCS_hils->mags[2] * 0.000000001f) };	//S2Eから受け取る磁気センサの値 [T]
	float NagVec_HILS[3];	//磁気センサの値を正規化
	//磁気データを正規化
	//磁気データを正規化
	Normalization(B_fld_, 3, NagVec_HILS);
	// 入力ベクトル：磁気ベクトル(機体座標系), 太陽ベクトル(機体座標系), 磁気ベクトル(慣性座標系), 太陽ベクトル(慣性座標系)
	TRIAD_class.cal(NagVec_HILS, g_acs.p_GLOBAL_AOCS->SunVec, g_ads.p_IGRF->IGRF_B_Norm, g_ads.p_SUNVECTOR->sunvector);
#endif
	print(SUNVECTOR,"sunvec %f %f %f\r\n", g_ads.p_SUNVECTOR->sunvector[0],g_ads.p_SUNVECTOR->sunvector[1],g_ads.p_SUNVECTOR->sunvector[2]);
	print(IGRF,"IGRF %f %f %f\r\n", g_ads.p_IGRF->IGRF_B_Norm[0],g_ads.p_IGRF->IGRF_B_Norm[1],g_ads.p_IGRF->IGRF_B_Norm[2]);

	//計算結果をTRIADの構造体に保存
	copy(*TRIAD_class.OUT_DCM, 3, 3, *TRIAD_.DCM);
	copy(TRIAD_class.OUT_q, 4, 1, TRIAD_.quaternion);

//	GLOBAL_AOCS_set_Quaternion(TRIAD_class.OUT_q);
//	GLOBAL_AOCS_set_DCM(*TRIAD_class.OUT_DCM);
	//計算結果をADS_parametrに格納
	GLOBAL_ADS_set_Quaternion(TRIAD_class.OUT_q);
	GLOBAL_ADS_set_DCM(*TRIAD_class.OUT_DCM);
	print(TRIAD,"TRIAD Quaternion %f %f %f %f\r\n", TRIAD_class.OUT_q[0], TRIAD_class.OUT_q[1], TRIAD_class.OUT_q[2], TRIAD_class.OUT_q[3]);
}

//#ifdef HILS
//CCP_EXEC_STS Cmd_ADS_TRIAD_HILS(const CTCP* packet)
//{
//
//	float B_fld_[3] = { (float)(magsbuf[0]*0.000000001), (float)(magsbuf[1] * 0.000000001),(float)(magsbuf[2] * 0.000000001) };
//	float Sunvector[3] = { (float)suns1buf[0], (float)suns1buf[1],(float)suns1buf[2] };
//
//	TRIAD_class.cal(B_fld_, Sunvector, g_ads.p_IGRF->IGRF_B_Norm, g_ads.p_SUNVECTOR->sunvector);
//
//	print(SUNS,"sunvec %f %f %f\r\n", g_ads.p_SUNVECTOR->sunvector[0],g_ads.p_SUNVECTOR->sunvector[1],g_ads.p_SUNVECTOR->sunvector[2]);
//	print(IGRF,"IGRF %f %f %f\r\n", g_ads.p_IGRF->IGRF_B_Norm[0],g_ads.p_IGRF->IGRF_B_Norm[1],g_ads.p_IGRF->IGRF_B_Norm[2]);
//
//	copy(*TRIAD_class.OUT_DCM, 3, 3, *TRIAD_.DCM);
//	copy(TRIAD_class.OUT_q, 4, 1, TRIAD_.quaternion);
//
//	GLOBAL_AOCS_set_Quaternion(TRIAD_class.OUT_q);
//	GLOBAL_AOCS_set_DCM(*TRIAD_class.OUT_DCM);
//
//	print(TRIAD,"TRIAD Quaternion %f %f %f %f\r\n", TRIAD_class.OUT_q[0], TRIAD_class.OUT_q[1], TRIAD_class.OUT_q[2], TRIAD_class.OUT_q[3]);
//
////	display(TRIAD_.quaternion, 4, 1);
////	printf("TRIAD DCM\r\n");
////	display(*AOCS_Param.DCM, 3, 3);
//
//	return CCP_EXEC_SUCCESS;
//}
//#endif

