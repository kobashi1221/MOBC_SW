/*
 * Bdot.cpp
 *
 *  Created on: 2021/03/26
 *      Author: masud
 */

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/Bdot.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Library/General/DBG_printf.h>

//======MRAM格納パラメータの実体宣言部======//
constexpr float BDOT_CLASS::Kb_[3];
constexpr float BDOT_CLASS::Kbd_[3];

//-----------------------------------------
// AODCS　計算関数定義
//-----------------------------------------

//ADCS計算関数を定義する
// // 入出力は全て引数として定義（出力引数はポインタ渡し）

/**
 * @brief  Bdot計算を行う
 * @param  in_B_fld_              地磁気センサ値　[nT]
 * @param  in_omega               機体角速度 [rad/s]
 * @param  out_Mag_moment_b_      必要磁気モーメント [Am^2]
 * @return  なし
 */
void BDOT_CLASS::Bdot(float in_B_fld_[3], float in_Omega[3], float out_Mag_moment_b_[3])
{
	float Bdot_[3];		//地磁場の時間変化
	float N_torque[3];	//要求制御トルク
	float mags;			//正規化された磁気ベクトル

	//20230616 takahashi
    //References:人工衛星の力学と制御ハンドブックp.723
	//ジャイロセンサと地磁気センサ使用

	//地磁気センサ値 B の絶対値をとって、結果を mags に格納(正規化用)
	mags = Fabs(in_B_fld_, 3);

	//B と　機体角速度 ω の外積をとって、結果を Bdot_ に格納
	Outer_product(in_B_fld_, in_Omega, Bdot_);

    //比例ゲイン Kb を Bdot_ に掛けて、結果を N_torque に格納
    for (int i = 0; i < 3; i++) { N_torque[i] = -Kb[i] * Bdot_[i]; }
    //N_torque を B の絶対値 で割って正規化し、結果を out_Mag_moment_b_ に格納
	CoeffVec(N_torque, 3, 1 / mags, out_Mag_moment_b_);
}

void BDOT_CLASS::B_delta(float in_B_fld_[3], float in_B_fld_pre_[3], float out_Mag_moment_b_[3])
{
	float Bdot_[3]; //地磁場の時間変化
	float N_torque[3]; //要求制御トルク
	float mags; //正規化された磁気ベクトル
	float dt = OBCT_diff_in_sec(&g_acs.p_GLOBAL_AOCS->MagPreTime , &g_acs.p_GLOBAL_AOCS->MagTime); //
//	printf("step_time: %f\r\n", dt);

	//20230616 takahashi
    //References:人工衛星の力学と制御ハンドブックp.723
	//地磁気センサのみ使用

	//行うことは Bdot制御測 と同じであり、 Bdot を B×ω ではなく B の時間微分で算出
	//時間幅dtが1sくらいのため、精度は従来のBdot制御測の求め方に劣る
	mags = Fabs(in_B_fld_, 3);

	print(DELTA_B,"Pre_MAG: %f, %f, %f\r\n", in_B_fld_pre_[0], in_B_fld_pre_[1], in_B_fld_pre_[2]);
	print(DELTA_B,"MAG: %f, %f, %f\r\n", in_B_fld_[0], in_B_fld_[1], in_B_fld_[2]);

	for (int i = 0; i < 3; i++) { Bdot_[i] = (in_B_fld_[i] - in_B_fld_pre_[i]) / dt; };

    for (int i = 0; i < 3; i++) { N_torque[i] = -Kbd[i] * Bdot_[i]; }
	CoeffVec(N_torque, 3, 1 / mags, out_Mag_moment_b_);
}

//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

//入力には接頭in_ 出力はout_を付ける
void BDOT_CLASS::cal(float in_B_fld[3], float in_Omega[3])
{
    Bdot(in_B_fld, in_Omega, this->OUT_torque);
}

void BDOT_CLASS::Dcal(float in_B_fld[3], float in_B_fld_pre[3])
{
	B_delta(in_B_fld, in_B_fld_pre, this->OUT_torque);
}

void BDOT_CLASS::reset(void)
{

}











