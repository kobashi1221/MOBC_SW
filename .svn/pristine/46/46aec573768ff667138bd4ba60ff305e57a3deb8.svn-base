/*
 * AOCS_debug_Qmethod.cpp
 *
 *  Created on: 2021/03/27
 *      Author: masud
 */


#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/AOCS/ADS/ADS_cmd_Qmethod.h>
#include <stdlib.h>
#include <string.h>

#include <src_user/Applications/UserDefined/AOCS/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/Qmethod.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>


static QMETHOD_CLASS Qmethod_class;
static QMETHOD_STRUCT QMETHOD_;

static void Qmethod_load_param(void);
static void Qmethod_initialize_param(void);
static void Qmethod_update_(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 * 作成者:増田 2021/03/27
 * コメント記入者:玉置 2023/06/28
 */
AppInfo Qmethod_param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_Bdot", Qmethod_load_param, Qmethod_initialize_param);
}

AppInfo Qmethod_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_Bdot", NULL, Qmethod_update_);
}

/**
 * @brief アプリケーション初期化関数 構造体内の初期化が必要な変数を初期化する．
 * @param　なし
 * @return　なし
 * 作成者:増田 2021/03/27
 * コメント記入者:玉置 2023/06/28
 */
static void Qmethod_load_param(void)
{
//    printf("sample driver initialize.\n");
    g_ads.p_QMETHOD = &QMETHOD_;
    read_param(&Qmethod_class.A_mag, &QMETHOD_CLASS::A_mag_, (uint16_t)(sizeof(Qmethod_class.A_mag)), (uint32_t)0x00005bd8);
}

/**
 * @brief アプリケーション実行関数
 * @param　なし
 * @return　なし
 * 作成者:増田 2021/03/27
 * コメント記入者:玉置 2023/06/28
 */
static void Qmethod_initialize_param(void)
{
    printf("Qmethod initialize_param.\n");
    write_param(&Qmethod_class.A_mag, &QMETHOD_CLASS::A_mag_, (uint16_t)(sizeof(Qmethod_class.A_mag)), (uint32_t)0x00005bd8);
}

/**
 * @brief Qmethod法によりQuaternionと慣性座標系から機体座標系へのDCMを推定する
 * @param　  なし
 * @return　なし
 * 作成者:増田 2021/03/27
 * コメント記入者:玉置 2023/06/17
 */
static void Qmethod_update_(void){

#ifndef HILS // 通常の処理
        // Qmethod法の計算
	Qmethod_class.cal(g_acs.p_GLOBAL_AOCS->MagVec, g_acs.p_GLOBAL_AOCS->SunVec, g_ads.p_IGRF->IGRF_B_Norm, g_ads.p_SUNVECTOR->sunvector);
#endif /*for 1U or 2U*/

#ifdef HILS //HILSの処理

	//S2Eから受け渡された機体座標系の地球磁場を単位変換[nT]→[T]して格納
	float B_fld_[3] = { (float)(AOCS_hils->mags[0]*0.000000001f), (float)(AOCS_hils->mags[1] * 0.000000001f),(float)(AOCS_hils->mags[2] * 0.000000001f) };

	//機体座標系の地球磁場方向ベクトルを格納する変数
	float NagVec_HILS[3];

	//S2Eからの地球磁場を正規化してNagVec_HILSに格納
	Normalization(B_fld_, 3, NagVec_HILS);

	//Qmethod法の計算
	Qmethod_class.cal(NagVec_HILS, g_acs.p_GLOBAL_AOCS->SunVec, g_ads.p_IGRF->IGRF_B_Norm, g_ads.p_SUNVECTOR->sunvector);
#endif /*for HILS*/

	//Qmethod_classクラスの出力Quaternionと出力DCMをQMETHOD_構造体にコピー
	copy(Qmethod_class.OUT_Q, 4, 1, QMETHOD_.quaternion);
	copy(*Qmethod_class.OUT_DCM, 3, 3, *QMETHOD_.DCM);

//	GLOBAL_AOCS_set_Quaternion(Qmethod_class.OUT_Q);
//	GLOBAL_AOCS_set_DCM(*Qmethod_class.OUT_DCM);

	//出力Quaternionと出力DCMをGlobal_AOCS_Parameterにセット
	GLOBAL_ADS_set_Quaternion(Qmethod_class.OUT_Q);
	GLOBAL_ADS_set_DCM(*Qmethod_class.OUT_DCM);
	print(QMETH,"QMETH Quaternion %f %f %f %f\r\n", Qmethod_class.OUT_Q[0], Qmethod_class.OUT_Q[1], Qmethod_class.OUT_Q[2], Qmethod_class.OUT_Q[3]);
}


//#ifdef HILS
//CCP_EXEC_STS Cmd_ADS_Qmethod_HILS(const CTCP* packet)
//{
//	float B_fld_[3] = { (float)(magsbuf[0]*0.000000001), (float)(magsbuf[1] * 0.000000001),(float)(magsbuf[2] * 0.000000001) };
//	float Sunvector[3] = { (float)suns1buf[0], (float)suns1buf[1],(float)suns1buf[2] };
//	print(SUNS,"sunvec %f %f %f\r\n", g_ads.p_SUNVECTOR->sunvector[0],g_ads.p_SUNVECTOR->sunvector[1],g_ads.p_SUNVECTOR->sunvector[2]);
//	print(IGRF,"IGRF %f %f %f\r\n", g_ads.p_IGRF->IGRF_B_Norm[0],g_ads.p_IGRF->IGRF_B_Norm[1],g_ads.p_IGRF->IGRF_B_Norm[2]);
//
//	Qmethod_class.cal(B_fld_, Sunvector, g_ads.p_IGRF->IGRF_B_Norm, g_ads.p_SUNVECTOR->sunvector);
//
//	copy(Qmethod_class.OUT_Q, 4, 1, QMETHOD_.quaternion);
//	copy(*Qmethod_class.OUT_DCM, 3, 3, *QMETHOD_.DCM);
//
//	GLOBAL_AOCS_set_Quaternion(Qmethod_class.OUT_Q);
//	GLOBAL_AOCS_set_DCM(*Qmethod_class.OUT_DCM);
////    printf("Qmetod %f %f %f %f\n\r", Qmethod_class.OUT_Q[0], Qmethod_class.OUT_Q[1], Qmethod_class.OUT_Q[2], Qmethod_class.OUT_Q[3]);
//	return CCP_EXEC_SUCCESS;
//}
//#endif
