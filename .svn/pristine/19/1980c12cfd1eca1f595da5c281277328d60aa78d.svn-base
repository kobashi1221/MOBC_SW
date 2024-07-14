/*
 * AOCS_debug_Quest.cpp
 *
 *  Created on: 2022/06/24
 *      Author: Kanie
 *   Reference:
 */

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Quest.h>
#include <stdlib.h>
#include <string.h>

#include <src_user/Applications/UserDefined/AOCS/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/Quest.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>


static QUEST_CLASS Quest_class;
static QUEST_STRUCT QUEST_;

static void Quest_load_param(void);
static void Quest_initialize_param(void);

static void Quest_update_(void);

/**
 * @brief アプリケーション作成関数
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 * 作成者:蟹江 2022/06/24
 * コメント記入者:玉置 2023/06/17
 */
AppInfo Quest_param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_Bdot", Quest_load_param, Quest_initialize_param);
}

AppInfo Quest_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_Bdot", NULL, Quest_update_);
}

/**
 * @brief アプリケーション初期化関数　構造体内の初期化が必要な変数を初期化する．
 * @param　なし
 * @return　なし
 * 作成者:蟹江 2022/06/24
 * コメント記入者:玉置 2023/06/17
 */
static void Quest_load_param(void)
{
//    printf("sample driver initialize.\n");
    g_ads.p_QUEST = &QUEST_;
    read_param(&Quest_class.A_mag, &QUEST_CLASS::A_mag_, (uint16_t)(sizeof(Quest_class.A_mag)), (uint32_t)0x00005bd8);
    read_param(&Quest_class.A_3, &QUEST_CLASS::A_3_, (uint16_t)(sizeof(Quest_class.A_3)), (uint32_t)0x00005c90);
    //check_param (check_paramの関数はドライバに作成)
}

/**
 * @brief  アプリケーション実行関数
 * @param　  なし
 * @return　なし
 * 作成者:蟹江 2022/06/24
 * コメント記入者:玉置 2023/06/17
 */
static void Quest_initialize_param(void)
{
    printf("Quest initialize_param.\n");
    write_param(&Quest_class.A_mag, &QUEST_CLASS::A_mag_, (uint16_t)(sizeof(Quest_class.A_mag)), (uint32_t)0x00005bd8);
    write_param(&Quest_class.A_3, &QUEST_CLASS::A_3_, (uint16_t)(sizeof(Quest_class.A_3)), (uint32_t)0x00005c90);
}

/**
 * @brief Quest法によりQuaternionと慣性座標系から機体座標系へのDCMを推定する
 * @param　  なし
 * @return　なし
 * 作成者:蟹江 2022/06/24
 * コメント記入者:玉置 2023/06/17
 */
static void Quest_update_(void){

	float p_vector_b[6]; //機体座標系のベクトルを格納する配列
	float p_vector_i[6]; //慣性座標系のベクトルを格納する配列
	int vec_num;         //計算に使用するベクトルの種類（e.g.地磁気ベクトル，太陽ベクトル）を格納する変数

if (ADS_Mode(QUEST_CALC)){ //AOCSフラグ ADSモードにおいてQUEST_CALCに1が立っている場合

#ifndef HILS //通常の処理
	memcpy(&p_vector_b[0], &g_acs.p_GLOBAL_AOCS->MagVec[0], sizeof(g_acs.p_GLOBAL_AOCS->MagVec)); //磁気センサの地磁気ベクトル（機体座標系）の値を*p_vector_b[0]にコピー
	memcpy(&p_vector_b[3], &g_acs.p_GLOBAL_AOCS->SunVec[0], sizeof(g_acs.p_GLOBAL_AOCS->SunVec)); //太陽センサの太陽ベクトル（機体座標系）の値を*p_vector_b[3]にコピー

	memcpy(&p_vector_i[0], &g_ads.p_IGRF->IGRF_B_Norm[0], sizeof(g_ads.p_IGRF->IGRF_B_Norm)); //IGRFの地磁気ベクトル（慣性座標系）の値を*p_vector_b[0]にコピー
	memcpy(&p_vector_i[3], &g_ads.p_SUNVECTOR->sunvector[0], sizeof(g_ads.p_SUNVECTOR->sunvector)); //J2000系の太陽ベクトル（慣性座標系）の値を*p_vector_b[3]にコピー

	//計算に使用するベクトルは2種類（地磁気ベクトル，太陽ベクトル）
	vec_num = 2;
#endif /*for 1U or 2U*/

#ifdef HILS //HILSの処理

	//S2Eから受け渡された機体座標系の地球磁場を単位変換[nT]→[T]して格納
	float B_fld_[3] = { (float)(AOCS_hils->mags[0]*0.000000001f), (float)(AOCS_hils->mags[1] * 0.000000001f),(float)(AOCS_hils->mags[2] * 0.000000001f)};

	//機体座標系の地球磁場方向ベクトルを格納する変数
	float NagVec_HILS[3];

	//S2Eからの地球磁場を正規化してNagVec_HILSに格納
	Normalization(B_fld_, 3, NagVec_HILS);

	print(QEST,"B %f %f %f\r\n", NagVec_HILS[0], NagVec_HILS[1], NagVec_HILS[2]);
	print(QEST,"S %f %f %f\r\n", g_acs.p_GLOBAL_AOCS->SunVec[0], g_acs.p_GLOBAL_AOCS->SunVec[1], g_acs.p_GLOBAL_AOCS->SunVec[2]);
	print(QEST,"IGRF_norm %f %f %f\r\n", g_ads.p_IGRF->IGRF_B_Norm[0], g_ads.p_IGRF->IGRF_B_Norm[1], g_ads.p_IGRF->IGRF_B_Norm[2]);
	print(QEST,"Sunvector %f %f %f\r\n", g_ads.p_SUNVECTOR->sunvector[0], g_ads.p_SUNVECTOR->sunvector[1], g_ads.p_SUNVECTOR->sunvector[2]);

	memcpy(&p_vector_b[0], &NagVec_HILS[0], sizeof(NagVec_HILS)); //S2Eからの地磁気ベクトル（機体座標系）の値を*p_vector_b[0]にコピー
	memcpy(&p_vector_b[3], &g_acs.p_GLOBAL_AOCS->SunVec[0], sizeof(g_acs.p_GLOBAL_AOCS->SunVec)); //太陽センサの太陽ベクトル（機体座標系）の値を*p_vector_b[3]にコピー

	memcpy(&p_vector_i[0], &g_ads.p_IGRF->IGRF_B_Norm[0], sizeof(g_ads.p_IGRF->IGRF_B_Norm)); //IGRFの地磁気ベクトル（慣性座標系）の値を*p_vector_b[0]にコピー
	memcpy(&p_vector_i[3], &g_ads.p_SUNVECTOR->sunvector[0], sizeof(g_ads.p_SUNVECTOR->sunvector)); //J2000系の太陽ベクトル（慣性座標系）の値を*p_vector_b[3]にコピー

	vec_num = 2; //計算に使用するベクトルは2種類（地球磁場方向ベクトル，太陽方向ベクトル）
#endif /*for HILS*/
}
    //QUEST法の計算
	Quest_class.cal(p_vector_b, p_vector_i, vec_num);

	//Quest_classクラスの出力Quaternionと出力DCMをQUEST_構造体にコピー
	copy(Quest_class.OUT_Q, 4, 1, QUEST_.quaternion);
	copy(*Quest_class.OUT_DCM, 3, 3, *QUEST_.DCM);

	//出力Quaternionと出力DCMをGlobal_AOCS_Parameterにセット
	GLOBAL_ADS_set_Quaternion(Quest_class.OUT_Q);
	GLOBAL_ADS_set_DCM(*Quest_class.OUT_DCM);
	print(QEST,"QUEST Quaternion %f %f %f %f\r\n", Quest_class.OUT_Q[0], Quest_class.OUT_Q[1], Quest_class.OUT_Q[2], Quest_class.OUT_Q[3]);

}


//#ifdef HILS
//CCP_EXEC_STS Cmd_ADS_Quest_HILS(const CTCP* packet)
//{
//	float B_fld_[3] = { (float)(magsbuf[0]*0.000000001), (float)(magsbuf[1] * 0.000000001),(float)(magsbuf[2] * 0.000000001) };
//	float Sunvector[3] = { (float)suns1buf[0], (float)suns1buf[1],(float)suns1buf[2] };
//	print(SUNS,"sunvec %f %f %f\r\n", g_ads.p_SUNVECTOR->sunvector[0],g_ads.p_SUNVECTOR->sunvector[1],g_ads.p_SUNVECTOR->sunvector[2]);
//	print(IGRF,"IGRF %f %f %f\r\n", g_ads.p_IGRF->IGRF_B_Norm[0],g_ads.p_IGRF->IGRF_B_Norm[1],g_ads.p_IGRF->IGRF_B_Norm[2]);
//
//	Quest_class.cal(B_fld_, Sunvector, g_ads.p_IGRF->IGRF_B_Norm, g_ads.p_SUNVECTOR->sunvector);
//
//	copy(Quest_class.OUT_Q, 4, 1, QUEST_.quaternion);
//	copy(*Quest_class.OUT_DCM, 3, 3, *QUEST_.DCM);
//
//	GLOBAL_AOCS_set_Quaternion(Quest_class.OUT_Q);
//	GLOBAL_AOCS_set_DCM(*Quest_class.OUT_DCM);
////    printf("Qmetod %f %f %f %f\n\r", Quest_class.OUT_Q[0], Quest_class.OUT_Q[1], Quest_class.OUT_Q[2], Quest_class.OUT_Q[3]);
//	return CCP_EXEC_SUCCESS;
//}
//#endif
