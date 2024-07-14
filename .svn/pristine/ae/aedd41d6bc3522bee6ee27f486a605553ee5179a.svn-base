/*
 * ADS_cmd_BBdot.cpp
 *
 *  Created on: 2022/06/13
 *      Author: Inakawa
 */

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_BBdot.h>
#include <stdlib.h>
#include <string.h>
#include "../ADS/ADS_cmd_IGRF.h"
#include "../../Global_Sensor_Value.h"
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/TRIAD.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/Middleware/MemoryMW.h>


static TRIAD_CLASS TRIAD_class;
static BBDOT_STRUCT BBDOT_;

static void BBdot_load_param(void);
static void BBdot_initialize_param(void);

static void BBdot_init(void);
static void BBdot_exe(void);

const float Time_threshold_ = 5.0f;
const BBDOT_MODE BBdot_mode_ = propagate;

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */

AppInfo BBdot_param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_BALLCOEF", BBdot_load_param, BBdot_initialize_param);
}

AppInfo BBdot_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_BBdot", BBdot_init, BBdot_exe);
}

static void BBdot_load_param(void)
{
	printf("BBdot_load_param\r\n");
    read_param(&BBDOT_.Time_threshold, &Time_threshold_, (uint16_t)(sizeof(BBDOT_.Time_threshold)), (uint32_t)0x00005C95);
    read_param(&BBDOT_.BBdot_mode, &BBdot_mode_, (uint16_t)(sizeof(BBDOT_.BBdot_mode)), (uint32_t)0x00005C99);
}

static void BBdot_initialize_param(void)
{
	printf("BBdot_initialize_param\r\n");
    write_param(&BBDOT_.Time_threshold, &Time_threshold_, (uint16_t)(sizeof(BBDOT_.Time_threshold)), (uint32_t)0x00005C95);
    write_param(&BBDOT_.BBdot_mode, &BBdot_mode_, (uint16_t)(sizeof(BBDOT_.BBdot_mode)), (uint32_t)0x00005C99);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void BBdot_init(void)
{
//    printf("sample driver initialize.\n");
	g_ads.p_BBDOT = &BBDOT_;
	BBDOT_.initflg = 0;
	BBDOT_.hist_num = 0;
	Zeros(HISTMAX, 3, &BBDOT_.bdot_obs_vec_hist[0][0]);
}

/**
 * @brief アプリケーション実行関数
 * 磁場と磁場変化ベクトルで姿勢決定する。磁気センサ取得周期と実行周期が同一である前提
 */
static void BBdot_exe(void)
{

//	float dt = OBCT_diff_in_sec(&g_acs.p_GLOBAL_AOCS->MagPreTime , &g_acs.p_GLOBAL_AOCS->MagTime);
	float dt = OBCT_diff_in_sec(&BBDOT_.PreTime , &g_acs.p_GLOBAL_AOCS->MagTime);
//	printf("Bdot dt = %f\r\n",dt);

	// 前回センサ取得時間との差が大きい場合は初期化する
	if (dt > BBDOT_.Time_threshold){
		BBdot_init();
	}
	// センサ取得時間を保持する
	BBDOT_.PreTime = g_acs.p_GLOBAL_AOCS->MagTime;

	float omega_ave[3];
	float b_ave[3];
	float dbdt[3];
	float bdot_obs[3];
	float bdot_est[3];
	float omegab[3];

	float bdot_obs_ave[3];
	float vecbuf[3];

	//propagate用
	float q_prev2now[4];
	float Dcm_prev2now[3][3];
	float Xprev[7], Xnow[7];//q+omegaの状態量

	int i;

	//前回2今回の姿勢情報初期化
		initQuaternion(q_prev2now);
		Identity(3, 3, *Dcm_prev2now);

		//単純差分での計算モード
		if (BBDOT_.BBdot_mode == diff) {
			//初回実行時、前回値を保存して抜ける
			//TODO timeout
			if(BBDOT_.initflg == 0){
				copy(g_acs.p_GLOBAL_AOCS->MagField, 3, 1, BBDOT_.b_obs_prev);
				copy(g_acs.p_GLOBAL_AOCS->Omega, 3, 1, BBDOT_.omega_prev);
				copy(g_ads.p_IGRF->IGRF_B, 3, 1, BBDOT_.b_est_prev);
				BBDOT_.initflg = 1;
				return;
			}

			//domega/dtを計算する
			//どこから時間差を取得する？？
			//b-bprev
			sub(g_acs.p_GLOBAL_AOCS->MagField, 3, 1, BBDOT_.b_obs_prev, 3, 1, dbdt);

			//(b-bprev)/dt 入力と出力同一ポインタでも動作するはず
			CoeffVec(dbdt, 3, 1.0/dt, dbdt);
			//printf("dbdt %f %f %f\r\n", dbdt[0], dbdt[1], dbdt[2]);

			//omega x bで使用するomega　と　b　を現在値と前回値の平均にする
			//上記の微分が二時刻の中間での値とみなせるため
			add(g_acs.p_GLOBAL_AOCS->MagField, 3, 1, BBDOT_.b_obs_prev, 3, 1, b_ave);
			add(g_acs.p_GLOBAL_AOCS->Omega, 3, 1, BBDOT_.omega_prev, 3, 1, omega_ave);
			CoeffVec(b_ave, 3, 0.5, b_ave);
			CoeffVec(omega_ave, 3, 0.5, omega_ave);

			//omega x bの計算
			Outer_product(omega_ave, b_ave, omegab);
			//printf("omegab %f %f %f\r\n", omegab[0], omegab[1], omegab[2]);

			//機体座標系表示の（慣性系）磁場変化は (b-bpreb)/dt + omega x B
			add(dbdt, 3, 1, omegab, 3, 1, bdot_obs);

	//		printf("bdot_obs");
	//		printf("%f,%f,%f,", bdot_obs[0], bdot_obs[1], bdot_obs[2]);
	//		printf("\r\n");

			//正規化
			Normalization(bdot_obs, 3, bdot_obs);
			//printf("bdot_obs vec %f %f %f\r\n", bdot_obs[0], bdot_obs[1], bdot_obs[2]);

			//計測ベクトル保存
			copy(bdot_obs, 3, 1, BBDOT_.bdot_obs_vec_hist[BBDOT_.hist_num]);
			BBDOT_.hist_num += 1;
			if(BBDOT_.hist_num >= HISTMAX) {
				BBDOT_.hist_num = 0;
			}


			//磁場微分は平均値をもちいてTRIADに使用することとする
			//ノイズ対策に機体座標系ベクトルの平均が有効のようだ
			//平均時間分出力姿勢が現在時間からずれるが、いったんノーケア
			Zeros(3, 1, bdot_obs_ave);
			for(i = 0; i < HISTMAX; i++) {
				add(BBDOT_.bdot_obs_vec_hist[i], 3, 1, bdot_obs_ave, 3, 1,vecbuf);
				copy(vecbuf, 3, 1, bdot_obs_ave);
			}
			Normalization(bdot_obs_ave, 3, bdot_obs_ave);


			//慣性系予測値
			sub(g_ads.p_IGRF->IGRF_B, 3, 1, BBDOT_.b_est_prev, 3, 1, bdot_est);

			//(b-bprev)/dt 入力と出力同一ポインタでも動作するはず
			CoeffVec(bdot_est, 3, 1.0/dt, bdot_est);

	//		printf("bdot_est");
	//		printf("%f,%f,%f,", bdot_est[0], bdot_est[1], bdot_est[2]);
	//		printf("\r\n");


			//正規化
			Normalization(bdot_est, 3, bdot_est);

			//前回値保存
			copy(g_acs.p_GLOBAL_AOCS->MagField, 3, 1, BBDOT_.b_obs_prev);
			copy(g_acs.p_GLOBAL_AOCS->Omega, 3, 1, BBDOT_.omega_prev);
			copy(g_ads.p_IGRF->IGRF_B, 3, 1, BBDOT_.b_est_prev);
		}


		if (BBDOT_.BBdot_mode == propagate) {
			//初回実行時、取得磁場@前回機体座標系、角速度、IGRF計算値を保存して抜ける
			//TODO timeout
			if(BBDOT_.initflg == 0){
				copy(g_acs.p_GLOBAL_AOCS->MagField, 3, 1, BBDOT_.b_obs_prev);
				copy(g_acs.p_GLOBAL_AOCS->Omega, 3, 1, BBDOT_.omega_prev);
				copy(g_ads.p_IGRF->IGRF_B, 	3, 1, BBDOT_.b_est_prev);
				BBDOT_.initflg = 1;
				return;
			}

			//====== 計測磁場微分計算 =====//
//			printf("b nT %f %f %f\r\n", g_acs.p_GLOBAL_AOCS->MagField[0], g_acs.p_GLOBAL_AOCS->MagField[1], g_acs.p_GLOBAL_AOCS->MagField[2]);

			//前回姿勢からのプロパゲートを行う
			//前回姿勢q=0001として計算
			//まずは状態量を格納　これで動作するはず
			copy(q_prev2now, 4, 1, Xprev);
			copy(g_acs.p_GLOBAL_AOCS->Omega, 3, 1, &Xprev[4]);
			float torque[3];
			Zeros(3, 1, torque);
			RungeOneStep(dt, Xprev, torque, Xnow);
			copy(Xnow, 4, 1, q_prev2now);
//			printf("qprev2now");
//			printf("%f,%f,%f,%f\r\n", q_prev2now[0], q_prev2now[1], q_prev2now[2], q_prev2now[3]);
			Quaternion_To_DCM(q_prev2now, *Dcm_prev2now);

			//今回機体座標系表示の、前回取得磁場を計算
			multiplication(*Dcm_prev2now, 3, 3, BBDOT_.b_obs_prev, 3, 1, vecbuf, 3, 1);

			//今回取得磁場との差分により、磁場変化@今回機体座標系を計算する
			//この後平均化処理を行うので、ここは物理値で計算保存するのが重要
			sub(g_acs.p_GLOBAL_AOCS->MagField, 3, 1, vecbuf, 3, 1, bdot_obs);
			CoeffVec(bdot_obs, 3, 1.0/dt, bdot_obs);
//			printf("bdot_obs nT %f %f %f\r\n", bdot_obs[0], bdot_obs[1], bdot_obs[2]);

			//磁場微分の座標系を現在の機体座標系に変換
			//BBDOT_.bdot_obs_vec_histには最新計算時の機体座標系表示の、磁場変化ベクトルが格納される
			for(i = 0; i < HISTMAX; i++) {
				multiplication(*Dcm_prev2now, 3, 3, BBDOT_.bdot_obs_vec_hist[i], 3, 1, vecbuf, 3, 1);
				copy(vecbuf, 3, 1, BBDOT_.bdot_obs_vec_hist[i]);
			}

			//計測ベクトル保存
			copy(bdot_obs, 3, 1, BBDOT_.bdot_obs_vec_hist[BBDOT_.hist_num]);
			BBDOT_.hist_num += 1;
			if(BBDOT_.hist_num >= HISTMAX) {
				BBDOT_.hist_num = 0;
			}

			//磁場微分の平均値を算出
			Zeros(3, 1, bdot_obs_ave);
			for(i = 0; i < HISTMAX; i++) {
				add(BBDOT_.bdot_obs_vec_hist[i], 3, 1, bdot_obs_ave, 3, 1,vecbuf);
				copy(vecbuf, 3, 1, bdot_obs_ave);
			}

			//方向ベクトルにする(と同時に平均化する)
			Normalization(bdot_obs_ave, 3, bdot_obs_ave);

			//====== 理論計算磁場微分計算 =====//

			//慣性系予測値の差分をとる
			sub(g_ads.p_IGRF->IGRF_B, 3, 1, BBDOT_.b_est_prev, 3, 1, bdot_est);

			//(b-bprev)/dt 入力と出力同一ポインタでも動作するはず
			//20220712 inakawa 以下で正規化するので不要
			//CoeffVec(bdot_est, 3, 1.0/dt, bdot_est);

//			printf("bdot_est ");
//			printf("%f,%f,%f,", bdot_est[0], bdot_est[1], bdot_est[2]);
//			printf("\r\n");

			//正規化
			Normalization(bdot_est, 3, bdot_est);

			//前回値保存
			copy(g_acs.p_GLOBAL_AOCS->MagField, 3, 1, BBDOT_.b_obs_prev);
			copy(g_acs.p_GLOBAL_AOCS->Omega, 3, 1, BBDOT_.omega_prev);
			copy(g_ads.p_IGRF->IGRF_B, 3, 1, BBDOT_.b_est_prev);

		}

//		for (int i = 0; i < HISTMAX; i++){
//			printf("Bdot[%d]: %f %f %f\r\n", i, BBDOT_.bdot_obs_vec_hist[i][0]*1000000000, BBDOT_.bdot_obs_vec_hist[i][1]*1000000000, BBDOT_.bdot_obs_vec_hist[i][2]*1000000000);
//		}
//		printf("\r\n");

	//	TRIAD_class.cal(g_acs.p_GLOBAL_AOCS->MagVec, bdot_obs, g_ads.p_IGRF->IGRF_B_Norm, bdot_est);
		TRIAD_class.cal(g_acs.p_GLOBAL_AOCS->MagVec, bdot_obs_ave, g_ads.p_IGRF->IGRF_B_Norm, bdot_est);

		copy(*TRIAD_class.OUT_DCM, 3, 3, *BBDOT_.DCM);
		copy(TRIAD_class.OUT_q, 4, 1, BBDOT_.quaternion);

//		GLOBAL_AOCS_set_Quaternion(TRIAD_class.OUT_q);
//		GLOBAL_AOCS_set_DCM(*TRIAD_class.OUT_DCM);
		GLOBAL_ADS_set_Quaternion(TRIAD_class.OUT_q);
		GLOBAL_ADS_set_DCM(*TRIAD_class.OUT_DCM);

//		printf("BBdot Quaternion");
//		printf("%f,%f,%f,%f,", TRIAD_class.OUT_q[0], TRIAD_class.OUT_q[1], TRIAD_class.OUT_q[2], TRIAD_class.OUT_q[3]);
//		printf("\r\n");

}



