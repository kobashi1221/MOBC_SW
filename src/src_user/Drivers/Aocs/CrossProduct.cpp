/*
 * CrossProduct.cpp
 *
 *  Created on: 2021/03/28
 *      Author: masuda
 *   Reference:
 *   cross_product_Pointing : S. Ikari et al. "Attitude Determination and Control System for the
 *                            PROCYON Micro-Spacecraft," Trans. Japan Soc. Aero. Space Sci. Vol. 60,
 *                            No. 3, pp. 181-191, 2017
 *                            人工衛星の力学と制御ハンドブックp.725 式(9.3-26) ただし，k = 1
 */


#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/CrossProduct.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Library/General/DBG_printf.h>

//======MRAM格納パラメータの実体宣言部======//
constexpr float CROSS_PRODUCT_CLASS::TargetQuaternion_[4];
constexpr float CROSS_PRODUCT_CLASS::TargetH_[3];
constexpr float CROSS_PRODUCT_CLASS::TargetOmeA_[3];
constexpr float CROSS_PRODUCT_CLASS::TargetOmeO_[3];
constexpr float CROSS_PRODUCT_CLASS::TargetPosition_[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_s[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_ec[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_ep[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_i[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_v[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_m[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_ave[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_min[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_max[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_keep[3];
constexpr float CROSS_PRODUCT_CLASS::ControlDirection_sc[3];
constexpr float CROSS_PRODUCT_CLASS::TargetDirection_sc[3];
constexpr float CROSS_PRODUCT_CLASS::TargetOmega_spinup_;
constexpr float CROSS_PRODUCT_CLASS::TargetOmega_spindown_;
constexpr float CROSS_PRODUCT_CLASS::TargetOmega_damping_;


constexpr float CROSS_PRODUCT_CLASS::Kc_[3];
constexpr float CROSS_PRODUCT_CLASS::Kpp_s[3];
constexpr float CROSS_PRODUCT_CLASS::Kpd_s[3];
constexpr float CROSS_PRODUCT_CLASS::Kqp_[3];
constexpr float CROSS_PRODUCT_CLASS::Kqd_[3];
constexpr float CROSS_PRODUCT_CLASS::Kpp_ec[3];
constexpr float CROSS_PRODUCT_CLASS::Kpd_ec[3];
constexpr float CROSS_PRODUCT_CLASS::Kpp_ep[3];
constexpr float CROSS_PRODUCT_CLASS::Kpd_ep[3];
constexpr float CROSS_PRODUCT_CLASS::Kpp_i[3];
constexpr float CROSS_PRODUCT_CLASS::Kpd_i[3];
constexpr float CROSS_PRODUCT_CLASS::Kpp_v[3];
constexpr float CROSS_PRODUCT_CLASS::Kpd_v[3];
constexpr float CROSS_PRODUCT_CLASS::Kpp_m[3];
constexpr float CROSS_PRODUCT_CLASS::Kpd_m[3];
constexpr float CROSS_PRODUCT_CLASS::Kpp_sc[3];
constexpr float CROSS_PRODUCT_CLASS::Kpd_sc[3];
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_c_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_s_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_ec_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_ep_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_i_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_v_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_m_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_q_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_sc_;

constexpr float CROSS_PRODUCT_CLASS::Kp_ave[3];
constexpr float CROSS_PRODUCT_CLASS::Kd_ave[3];
constexpr float CROSS_PRODUCT_CLASS::Kp_min[3];
constexpr float CROSS_PRODUCT_CLASS::Kd_min[3];
constexpr float CROSS_PRODUCT_CLASS::Kp_max[3];
constexpr float CROSS_PRODUCT_CLASS::Kd_max[3];
constexpr float CROSS_PRODUCT_CLASS::Kp_keep[3];
constexpr float CROSS_PRODUCT_CLASS::Kd_keep[3];
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_ave_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_min_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_max_;
constexpr uint8_t CROSS_PRODUCT_CLASS::eclipse_mode_keep_;

constexpr uint8_t CROSS_PRODUCT_CLASS::timeout_damping_;
constexpr uint8_t CROSS_PRODUCT_CLASS::timeout_spindown_;
constexpr float CROSS_PRODUCT_CLASS::control_dir_cor_;
constexpr float CROSS_PRODUCT_CLASS::control_norm_dir_cor_;
constexpr float CROSS_PRODUCT_CLASS::Cd_Td_thr_;
constexpr float CROSS_PRODUCT_CLASS::B_Cd_thr_;


//-----------------------------------------
// AODCS　計算関数定義
//-----------------------------------------

//ADCS計算関数を定義する
// // 入出力は全て引数として定義（出力引数はポインタ渡し）


/**
 * @brief  CrossProdutによるポインティング制御
 * @param  in_B_fld_         機体座標系の地磁気ベクトル [T]
 * @param  in_Omega          機体座標系の角速度ベクトル [rad/s]
 * @param  in_TD             機体座標系の目標ベクトル [-] 目標物がある方向
 * @param  in_CD             機体座標系の制御方向ベクトル [-]　目標物に向けたい機体の方向
 * @param  in_TOme           機体座標系の目標角速度ベクトル [-]
 * @param  Kpp               Pゲイン [-]
 * @param  Kpd　　　　　　　　　　　　Dゲイン [-]
 * @param  out_Moment_b_     機体座標系の要求磁気モーメント [Am^2]
 * @return  なし
 *
 * 作成者　増田 2021/03/28
 * コメント記入者　浮田 2023/06/22
 */
void CROSS_PRODUCT_CLASS::cross_product_Pointing(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float in_TOme[3], float Kpp[3], float Kpd[3], float out_Moment_b_[3])
{
	float Td_Norm[3] = {0.0f};
	float Cd_Norm[3] = {0.0f};
	float Treq[3] = {0.0f};
	float Buf1[3] = {0.0f};
	float Bufacos = 0.0f;
	float Buf2[3] = {0.0f};
	float mags = 0.0f;
	float coeff = 0.0f;

	// 目標方向ベクトルの正規化
	Normalization(in_TD, 3, Td_Norm);
	// 制御方向ベクトルの正規化
	Normalization(in_CD, 3, Cd_Norm);

	// Buf1: 制御方向ベクトルと目標方向ベクトルの外積
	Outer_product(Cd_Norm, Td_Norm, Buf1); // FIXME: referenceと逆?
	Normalization(Buf1, 3, Buf1);

	// Bufacos: 制御方向ベクトルと目標方向ベクトルの内積値のアークコサイン
	for(int i = 0; i < 3; i ++){
		Bufacos += Cd_Norm[i] * Td_Norm[i];
	}
	if(Bufacos > 1.0f) { Bufacos = 1.0f; }
	else if(Bufacos < -1.0f) { Bufacos = -1.0f; }

	Bufacos = acosf(Bufacos);

	// 制御トルクを計算 reference 式(6)参照
	// FIXME: in_TOmeに目標角速度の大きさと目標方向ベクトルの大きさの情報が含まれている
	//       目標方向ベクトルを変更する場合，in_TOmeも変更しなければならない設計となっている
    for (int i = 0; i < 3; i++) {
    		Treq[i] = Kpp[i] * Bufacos * Buf1[i] + Kpd[i] * (in_TOme[i] - in_Omega[i]);
    }

    // 磁気姿勢制御則（クロスプロダクト則）により要求磁気モーメントを計算
//    printf("Treq %f %f %f\n\r", Treq[0], Treq[1], Treq[2]);
    // 地磁気ベクトルと制御トルクの外積
    Outer_product(in_B_fld_, Treq, Buf2);
    // mags: 地磁気の絶対値
    mags = Fabs(in_B_fld_, 3);
    coeff =  1.0f / (mags * mags);
    CoeffVec(Buf2, 3, coeff, out_Moment_b_);

}

/**
 * @brief  CrossProdutによるポインティング制御（蝕時にダンピングのみを行う）
 * @param  in_B_fld_         機体座標系の地磁気ベクトル [T]
 * @param  in_Omega          機体座標系の角速度ベクトル [rad/s]
 * @param  in_TD             機体座標系の目標ベクトル [-] 目標物がある方向
 * @param  in_CD             機体座標系の制御方向ベクトル [-]　目標物に向けたい機体の方向
 * @param  in_TOme           機体座標系の目標角速度ベクトル [-]
 * @param  Kpp               Pゲイン [-]
 * @param  Kpd　　　　　　　　　　　　Dゲイン [-]
 * @param  out_Moment_b_    機体座標系の要求磁気モーメント [Am^2]
 * @return  なし

 * 作成者　増田 2021/03/28
 * コメント記入者　浮田 2023/06/22
 */
void CROSS_PRODUCT_CLASS::cross_product_Pointing_eclipse(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float in_TOme[3], float Kpp[3], float Kpd[3], float out_Moment_b_[3])
{
	float Td_Norm[3] = {0.0f};
	float Cd_Norm[3] = {0.0f};
	float Treq[3] = {0.0f};
	float Buf1[3] = {0.0f};
	float Bufacos = 0.0f;
	float Buf2[3] = {0.0f};
	float mags = 0.0f;
	float coeff = 0.0f;

	// 目標方向ベクトルの正規化
	Normalization(in_TD, 3, Td_Norm);
	// 制御方向ベクトルの正規化
	Normalization(in_CD, 3, Cd_Norm);

	// Buf1: 制御方向ベクトルと目標方向ベクトルの外積
	Outer_product(Cd_Norm, Td_Norm, Buf1); // FIXME: referenceと逆?
	Normalization(Buf1, 3, Buf1);

	// Bufacos: 制御方向ベクトルと目標方向ベクトルの内積値のアークコサイン
	for(int i = 0; i < 3; i ++){
		Bufacos += Cd_Norm[i] * Td_Norm[i];
	}
	if(Bufacos > 1.0f) { Bufacos = 1.0f; }
	else if(Bufacos < -1.0f) { Bufacos = -1.0f; }

	Bufacos = acosf(Bufacos);

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 1){
		// 蝕時はダンピングのみ
	    for (int i = 0; i < 3; i++) {
	    		Treq[i] = Kpd[i] * (in_TOme[i] - in_Omega[i]);
	    }
	} else{
		// 蝕でないときは通常の制御トルク計算
	    for (int i = 0; i < 3; i++) {
	    		Treq[i] = Kpp[i] * Bufacos * Buf1[i] + Kpd[i] * (in_TOme[i] - in_Omega[i]);
	    }
	}

	// 磁気姿勢制御則（クロスプロダクト則）により要求磁気モーメントを計算
//    printf("Treq %f %f %f\n\r", Treq[0], Treq[1], Treq[2]);
    // 地磁気ベクトルと制御トルクの外積
    Outer_product(in_B_fld_, Treq, Buf2);
    // mags: 地磁気の絶対値
    mags = Fabs(in_B_fld_, 3);
    coeff =  1.0f / (mags * mags);
    CoeffVec(Buf2, 3, coeff, out_Moment_b_);

}

/**
 * @brief  CrossProdutによる角運動量ベクトル管理
 * @param  in_B_fld_        地磁気センサ値
 * @param  in_Omega         機体角速度
 * @param  in_TD            目標角運動量ベクトル
 * @param  out_Moment_b_    必要磁気モーメント
 * @return  なし
 */
void CROSS_PRODUCT_CLASS::cross_product_Angular(float in_B_fld_[3], float in_Omega[3], float in_TH[3], float out_Moment_b_[3])
{
	float mags;
	float H_sat[3] = {};
	float delta_H[3] = {};
	float Buf[3] = {};
	float Buf2[3] = {};

	mags = Fabs(in_B_fld_, 3);

	multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor, 3, 3, in_Omega, 3, 1, H_sat, 3, 1);

	sub(H_sat, 3, 1, TargetH, 3, 1, delta_H);

	Outer_product(in_B_fld_, delta_H, Buf);

	for(int i = 0; i < 3; i ++){
		Buf2[i] = Buf[i] * Kc[i];
	}

	CoeffVec(Buf2, 3, 1.0f / (mags * mags), OUT_Moment_b_);

}

/**
 * @brief  CrossProdutによる目標クォータニオン指向制御
 * @param  in_B_fld_ 　　　　　　　　　　地磁気センサ値
 * @param  in_Omega              機体角速度
 * @param  in_Quaternion_Body    現在クォータニオン（機体座標系）
 * @param  in_Quaternion_goal_   目標クォータニオン（機体座標系）
 * @param  out_Moment_b_         必要磁気モーメント
 * @Reference 超小型人工衛星における姿勢決定・制御系の開発
 * @return  なし
 */
void CROSS_PRODUCT_CLASS::cross_product_Quaternion(float in_B_fld_[3], float in_Omega[3], float in_Quaternion_Body[4], const float in_Quaternion_goal_[4], float out_Moment_b_[3])
{

	float Buf1[3], Buf2[3];
	float mags;
	//目標クォータニオン//
	float Qt_norm[4];
	float Qt[3];
	float q4t;
	//現在クォータニオン//
	float Q_norm[4];
	float Q[3];
	float q4;
	////////////////////

	float N_torque[3];
	//クォータニオンエラー//
	float Qe[3];

    // 実計算部

	//Normalize
	Normalization(in_Quaternion_Body, 4, Q_norm);
	Normalization(in_Quaternion_goal_, 4, Qt_norm);

	///Body frame///
    mags = Fabs(in_B_fld_, 3);

    for (int i = 0; i < 3; i++) { Qt[i] = Qt_norm[i]; }
    q4t = Qt_norm[3];

    for (int i = 0; i < 3; i++) { Q[i] = Q_norm[i]; }
    q4 = Q_norm[3];

    Outer_product(Q, Qt, Buf1);

    for (int i = 0; i < 3; i++) { Qe[i] = -q4 * Q[i] + q4 * Qt[i] - Buf1[i]; }

    for (int i = 0; i < 3; i++) { N_torque[i] = Kqp[i] * Qe[i] - Kqd[i] * in_Omega[i]; }

    Outer_product(in_B_fld_, N_torque, Buf2);

    CoeffVec(Buf2, 3, 1.0f / (mags * mags), out_Moment_b_);
}

/**
 * @brief  CrossProdutによるクォータニオン積を用いたクォータニオンフィードバック
 * @param  in_B_fld_ 　　　　　　　　　　地磁気センサ値
 * @param  in_Omega              機体角速度
 * @param  in_Quaternion_Body    現在クォータニオン（機体座標系）
 * @param  in_Quaternion_goal_   目標クォータニオン（機体座標系）
 * @param  out_Moment_b_         必要磁気モーメント
 * @Reference  人工衛星の力学と姿勢ハンドブックp733
 * @return  なし
 */
void CROSS_PRODUCT_CLASS::cross_product_QFB(float in_B_fld_[3], float in_Omega[3], float in_Quaternion_Body[4], const float in_Quaternion_goal_[4], float out_Moment_b_[3])
{

	float Buf1[3];
	float mags;
	//目標クォータニオン//
	float Qt[4];

	//現在クォータニオン//
	float Qb[4];
	////////////////////

	float N_torque[3];
	//クォータニオンエラー//
	float Qe[4];

    // 実計算部

	///Body frame///
    mags = Fabs(in_B_fld_, 3);

    Normalization(in_Quaternion_goal_, 4, Qt);
    Normalization(in_Quaternion_Body, 4, Qb);

    Quaternion_Product(Qb, Qt, Qe);

    for (int i = 0; i < 3; i++) { N_torque[i] = -Kqp[i] * Qe[i]*Qe[3] - Kqd[i] * in_Omega[i]; }
    Outer_product(in_B_fld_, N_torque, Buf1);

    CoeffVec(Buf1, 3, 1.0f / (mags * mags), out_Moment_b_);
}






void CROSS_PRODUCT_CLASS::cross_product_SpinControl(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float Target_omega, float out_Moment_b_[3])
{
	float mags = 0.0f;
	float Td_Norm[3] = {};
	float Cd_Norm[3] = {};
	float Normalized_in_B_fld_[3]={};
	float Buf1[3] = {};
	float Condition1 = 0.0f;
	float Buf2[3] = {};
	float Condition2 = 0.0f;
	float Treq[3] = {};
	float Buf3[3] = {};

	// 目標軸方向
	Normalization(in_TD, 3, Td_Norm);

	// 制御軸方向
	Normalization(in_CD, 3, Cd_Norm);

	// 磁気ベクトル
	mags = Fabs(in_B_fld_, 3);
	for(int i = 0; i < 3; i++){
		Normalized_in_B_fld_[i] = in_B_fld_[i] / mags;
	}

	Outer_product(Cd_Norm, Td_Norm, Buf1);
	Condition1 = Fabs(Buf1, 3);

//	Outer_product(Normalized_in_B_fld_, ex, Buf2);
	Outer_product(Normalized_in_B_fld_, Cd_Norm, Buf2);
	Condition2 = Fabs(Buf2, 3);

	// 目標角速度を制御軸方向と制御軸と直交する方向へ分解する
	float omega_t[3];	// 目標角速度
	float omega_t_c[3];	// 制御軸方向
	float omega_t_n[3];	// 制御軸と直交方向
	CoeffVec(Cd_Norm, 3, Target_omega, omega_t);
	float dot1 = omega_t[0] * Cd_Norm[0] + omega_t[1] * Cd_Norm[1] + omega_t[2] * Cd_Norm[2];
	CoeffVec(Cd_Norm, 3, dot1, omega_t_c);
	sub(omega_t, 3, 1, omega_t_c, 3, 1, omega_t_n);

	// 現在角速度を制御軸方向と制御軸と直交する方向へ分解する
	float omega_c[3];	// 制御軸方向
	float omega_n[3];	// 制御軸と直交方向
	float dot2 = in_Omega[0] * Cd_Norm[0] + in_Omega[1] * Cd_Norm[1] + in_Omega[2] * Cd_Norm[2];
	CoeffVec(Cd_Norm, 3, dot2, omega_c);
	sub(in_Omega, 3, 1, omega_c, 3, 1, omega_n);

	if(Condition1 > Cd_Td_thr){
		for(int i = 0;i < 3; i++){
			Treq[i] = Kppsc[i] * Buf1[i] - Kpdsc[i] * in_Omega[i];
		}
	}

	else{
		if(Condition2 > B_Cd_thr){
//			Treq[0] = Kppsc[0] * Buf1[0] + Kpdsc[0] * 0.2f * pow(Condition2, 2.0) * (Target_omega - in_Omega[0]);
//			for(int i = 1; i < 3; i++){
//				Treq[i] = Kppsc[i] * Buf1[i] - Kpdsc[i] * 0.002f * in_Omega[i];
//			}

			for (int i = 0; i < 3; i++){
				Treq[i] = Kppsc[i] * Buf1[i]
						   + control_dir_cor * Condition2 * Condition2 * Kpdsc[i] * (omega_t_c[i] - omega_c[i])
						   + control_norm_dir_cor * Kpdsc[i] * (omega_t_n[i] - omega_n[i]);
			}
		}
		else{
//			Treq[0] = Kppsc[0] * Buf1[0];
//			for(int i = 1; i < 3; i++){
//				Treq[i] = Kppsc[i] * Buf1[i] - Kpdsc[i] * in_Omega[i];
//			}

			for (int i = 0; i < 3; i++){
				Treq[i] = Kppsc[i] * Buf1[i] + Kpdsc[i] * (omega_t_n[i] - omega_n[i]);
			}
		}
	}

    Outer_product(in_B_fld_, Treq, Buf3);

    CoeffVec(Buf3, 3, 1.0f / mags, out_Moment_b_);
}


//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

/**
 * @brief  CrossProdutによるポインティング制御
 * @param  in_B_fld_         機体座標系の地磁気ベクトル [T]
 * @param  in_Omega          機体座標系の角速度ベクトル [rad/s]
 * @param  in_TD             機体座標系の目標ベクトル [-] 目標物がある方向
 * @param  in_CD             機体座標系の制御方向ベクトル [-]　目標物に向けたい機体の方向
 * @param  in_TOme           機体座標系の目標角速度ベクトル [-]
 * @param  Kpp               Pゲイン [-]
 * @param  Kpd　　　　　　　　　　　　Dゲイン [-]
 * @return  なし
 *
 * 作成者　増田 2021/03/28
 * コメント記入者　浮田 2023/06/19
 */
void CROSS_PRODUCT_CLASS::Poincal(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float in_TOme[3], float Kpp[3], float Kpd[3])
{
    cross_product_Pointing(in_B_fld_, in_Omega, in_TD, in_CD, in_TOme, Kpp, Kpd, this->OUT_Moment_b_);
}


/**
 * @brief  CrossProdutによるポインティング制御（蝕時にダンピングのみを行う）
 * @param  in_B_fld_         機体座標系の地磁気ベクトル [T]
 * @param  in_Omega          機体座標系の角速度ベクトル [rad/s]
 * @param  in_TD             機体座標系の目標ベクトル [-] 目標物がある方向
 * @param  in_CD             機体座標系の制御方向ベクトル [-]　目標物に向けたい機体の方向
 * @param  in_TOme           機体座標系の目標角速度ベクトル [-]
 * @param  Kpp               Pゲイン [-]
 * @param  Kpd　　　　　　　　　　　　Dゲイン [-]
 * @return  なし

 * 作成者　増田 2021/03/28
 * コメント記入者　浮田 2023/06/22
 */
void CROSS_PRODUCT_CLASS::Poincal_eclipse(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float in_TOme[3], float Kpp[3], float Kpd[3])
{
    cross_product_Pointing_eclipse(in_B_fld_, in_Omega, in_TD, in_CD, in_TOme, Kpp, Kpd, this->OUT_Moment_b_);
}

void CROSS_PRODUCT_CLASS::Angcal(float in_B_fld_[3], float in_Omega[3], float in_TH[3])
{
    cross_product_Angular(in_B_fld_, in_Omega, in_TH, this->OUT_Moment_b_);
}

void CROSS_PRODUCT_CLASS::Quacal(float in_B_fld_[3], float in_Omega[3], float in_Quaternion_Body[4], const float in_Quaternion_goal_[4])
{
    cross_product_Quaternion(in_B_fld_, in_Omega, in_Quaternion_Body, in_Quaternion_goal_, this->OUT_Moment_b_);
}

void CROSS_PRODUCT_CLASS::QFBcal(float in_B_fld_[3], float in_Omega[3], float in_Quaternion_Body[4], const float in_Quaternion_goal_[4])

{
    cross_product_QFB(in_B_fld_, in_Omega, in_Quaternion_Body, in_Quaternion_goal_, this->OUT_Moment_b_);
}

void CROSS_PRODUCT_CLASS::Spincal(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float Target_omega)

{
    cross_product_SpinControl(in_B_fld_, in_Omega, in_TD, in_CD, Target_omega, this->OUT_Moment_b_);
}

void CROSS_PRODUCT_CLASS::reset(void)
{

}



