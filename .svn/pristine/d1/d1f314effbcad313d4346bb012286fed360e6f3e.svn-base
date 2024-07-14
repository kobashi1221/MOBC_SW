
#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/Compensation.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <stdlib.h>
#include <string.h>

//======MRAM格納パラメータの実体宣言部======//

constexpr float COMPENSATION_CLASS::Kco_eav_[3];
constexpr float COMPENSATION_CLASS::Kco_cp_[3];
constexpr float COMPENSATION_CLASS::Kco_rmm_[3];
constexpr float COMPENSATION_CLASS::Kco_msa_[3];
constexpr float COMPENSATION_CLASS::Kco_ec_[3];
constexpr uint8_t COMPENSATION_CLASS::eclipse_mode_eav_;
constexpr uint8_t COMPENSATION_CLASS::eclipse_mode_cp_;
constexpr uint8_t COMPENSATION_CLASS::eclipse_mode_rmm_;
constexpr uint8_t COMPENSATION_CLASS::eclipse_mode_msa_;
constexpr uint8_t COMPENSATION_CLASS::eclipse_mode_ed_;
constexpr uint8_t COMPENSATION_CLASS::eclipse_mode_cm_;


//-----------------------------------------
// AODCS　計算関数定義
//-----------------------------------------

//ADCS計算関数を定義する
// // 入出力は全て引数として定義（出力引数はポインタ渡し）

/**
 * @brief  地球角速度の補償則
 * @param  in_B_fld_ 　　　　　　　　　　地磁気センサ値
 * @param  in_Omega              機体角速度
 * @param  in_pos         		  位置 [m]
 * @param  in_vel            	速度 [m/s]
 * @param  out_Moment_b_         必要磁気モーメント
 * Reference 地球角速度補償：人工衛星の力学と制御ハンドブックp.147
 * 作成者　田村　2022/07/19
 * コメント記入者 藤田 2023/06/23
 * @return  なし
 */
void COMPENSATION_CLASS::compesation_EarthAngularVelocity(float in_B_fld_[3], float in_Omega[3], float in_pos[3], float in_vel[3], float out_Moment_b_[3])
{
	float mags; //磁気ベクトルの絶対値
	float Buf[3] = {}; //外積の計算結果
	float Treq[3] = {}; //要求トルク

	float ome_orbit[3] = {0.0f}; //軌道周回角速度
	float ome_orbit_b[3] = {0.0f}; //軌道周回角速度（機体座標系）
	float r_norm = 0.0f; //　距離の絶対値

	//面積速度一定の法則から角運動量を算出
	Outer_product(in_pos, in_vel, Buf);
	//　距離の大きさ
	r_norm = Fabs(in_pos,3);
	// r×v=r×rωのため、r^2を算出
	float coeff = 1.0f / (r_norm * r_norm);
	//軌道周回角速度の算出
	CoeffVec(Buf, 3, coeff, ome_orbit);
	//軌道周回角速度（機体座標系）の算出
	multiplication(*g_ads.p_ADS->DCM_i2b, 3, 3, ome_orbit, 3, 1, ome_orbit_b, 3, 1);

	//　Kco_eavを制御則のDゲインと合わせる
	for (int i = 0; i < 3; i++) {
			Treq[i] = Kco_eav[i] * ome_orbit_b[i];
	}

    Outer_product(in_B_fld_, Treq, Buf);
    mags = Fabs(in_B_fld_, 3);
    //要求磁気モーメントの算出
    CoeffVec(Buf, 3, 1 / (mags * mags), OUT_Moment_b_);

//   printf("EAVTreq %.2e %.2e %.2e ", Treq[0], Treq[1], Treq[2]);
}

/**
 * @brief カップリングトルクの補償則
 * @param in_B_fld_       磁場
 * @param in_Omega        衛星角速度（機体座標系）
 * @param out_Moment_b_   補償磁気モーメント
 * @Reference
 * @return  なし
 */
void COMPENSATION_CLASS::compesation_Coupling(float in_B_fld_[3], float in_Omega[3], float out_Moment_b_[3])
{
	float mags;
	float Buf[3] = {};
	float Treq[3] = {};

	mags = Fabs(in_B_fld_, 3);
	multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor, 3, 3, in_Omega, 3, 1, Buf, 3, 1);

	//　カップリングトルク自体は-ω×Iω　これを打ち消す
	Outer_product(in_Omega, Buf, Treq);

	for (int i = 0; i < 3; i++) {
			Treq[i] = Kco_cp[i] * Treq[i];
	}

	Outer_product(in_B_fld_, Treq, Buf);
	CoeffVec(Buf, 3, 1 / (mags * mags), OUT_Moment_b_);

//	printf("CPTreq %.2e %.2e %.2e ", Treq[0], Treq[1], Treq[2]);
}

/**
 * @brief 残留磁気モーメントの補償則
 * @param in_B_fld_       磁場
 * @param RM_Moment       残留磁気モーメント
 * @param out_Moment_b_   補償磁気モーメント
 * @Reference
 * @return  なし
 */
void COMPENSATION_CLASS::compesation_RMM(float in_B_fld_[3], float RM_Moment[3], float out_Moment_b_[3])
{
	float mags;
	float Buf[3] = {};
	float Treq[3] = {};

	mags = Fabs(in_B_fld_, 3);

	//　打ち消すためのトルク.外積を逆順で計算
	Outer_product(in_B_fld_, RM_Moment, Treq);

	for (int i = 0; i < 3; i++) {
			Treq[i] = Kco_rmm[i] * Treq[i];
	}

	Outer_product(in_B_fld_, Treq, Buf);
	CoeffVec(Buf, 3, 1 / (mags * mags), OUT_Moment_b_);

//	printf("RMMTreq %.2e %.2e %.2e ", Treq[0], Treq[1], Treq[2]);
}

/**
 * @brief 形状磁気異方性の補償則
 * @param in_B_fld_       磁場
 * @param in_Mag          形状磁気異方性テンソル
 * @param out_Moment_b_   補償磁気モーメント
 * @Reference 浮田君の卒論
 * @return  なし
 */
void COMPENSATION_CLASS::compesation_ShapeAnisortropy(float in_B_fld_[3], float in_Mag[3][3], float out_Moment_b_[3])
{
	float mags;
	float Buf[3] = {};
	float Treq[3] = {};

	mags = Fabs(in_B_fld_, 3);
	multiplication(*in_Mag, 3, 3, in_B_fld_, 3, 1, Buf, 3, 1);

	//　打ち消すためのトルク.外積を逆順で計算
	Outer_product(in_B_fld_, Buf, Treq);

	for (int i = 0; i < 3; i++) {
			Treq[i] = Kco_msa[i] * Treq[i];
	}

	Outer_product(in_B_fld_, Treq, Buf);
	CoeffVec(Buf, 3, 1 / (mags * mags), OUT_Moment_b_);

//	printf("Treq %.2e %.2e %.2e ", Treq[0], Treq[1], Treq[2]);
}

/**
 * @brief 渦電流トルクの補償則
 * @param in_B_fld_       磁場
 * @param in_EdCurT         渦電流トルク
 * @param out_Moment_b_   補償磁気モーメント
 * @Reference 玉置君の卒論
 * @return  なし
 */
void COMPENSATION_CLASS::compesation_EddyCurrent(float in_B_fld_[3], float in_EdCurT[3], float out_Moment_b_[3])
{
	float mags;
	float Buf[3] = {};
	float Treq[3] = {};

	mags = Fabs(in_B_fld_, 3);

	for (int i = 0; i < 3; i++) {
			Treq[i] = -in_EdCurT[i];
			Treq[i] =  Kco_ec[i] * Treq[i];
	}

	Outer_product(in_B_fld_, Treq, Buf);
	CoeffVec(Buf, 3, 1 / (mags * mags), OUT_Moment_b_);

//	printf("Treq %.2e %.2e %.2e ", Treq[0], Treq[1], Treq[2]);
}

/**
 * @brief 一定磁気モーメント出力
 * @param in_B_fld_       磁場
 * @param in_Treq         要求トルク
 * @param out_Moment_b_   補償磁気モーメント
 * @Reference
 * @return  なし
 */
void COMPENSATION_CLASS::compesation_ConstantMagnetic(float in_B_fld_[3], float in_Treq[3], float out_Moment_b_[3])
{
	float mags;
	float Buf[3] = {};

	mags = Fabs(in_B_fld_, 3);

	Outer_product(in_B_fld_, in_Treq, Buf);
	CoeffVec(Buf, 3, 1 / (mags * mags), OUT_Moment_b_);
}


//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

//入力には接頭in_ 出力はout_を付ける
void COMPENSATION_CLASS::Eartcal(float in_B_fld_[3], float in_Omega[3], float in_pos[3], float in_vel[3])
{
	compesation_EarthAngularVelocity(in_B_fld_, in_Omega, in_pos, in_vel, this->OUT_Moment_b_);
}

void COMPENSATION_CLASS::Coupcal(float in_B_fld_[3], float in_Omega[3])
{
	compesation_Coupling(in_B_fld_, in_Omega, this->OUT_Moment_b_);
}

void COMPENSATION_CLASS::RMMcal(float in_B_fld_[3], float RM_Moment[3])
{
	compesation_RMM(in_B_fld_, RM_Moment, this->OUT_Moment_b_);
}

void COMPENSATION_CLASS::Shapecal(float in_B_fld_[3], float in_Mag[3][3])

{
	compesation_ShapeAnisortropy(in_B_fld_, in_Mag, this->OUT_Moment_b_);
}

void COMPENSATION_CLASS::Eddycal(float in_B_fld_[3], float in_EdCurT[3])

{
	compesation_EddyCurrent(in_B_fld_, in_EdCurT, this->OUT_Moment_b_);
}

void COMPENSATION_CLASS::Conscal(float in_B_fld_[3], float in_Treq[3])

{
	compesation_ConstantMagnetic(in_B_fld_, in_Treq, this->OUT_Moment_b_);
}

void COMPENSATION_CLASS::reset(void)
{

}
