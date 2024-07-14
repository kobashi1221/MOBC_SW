/*
 * AOCSfunk.cpp
 *
 *  Created on: 2021/03/27
 *      Author: masud
 */

//#include <main.h>
#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <stdlib.h>
#include <string.h>


//======MRAM格納パラメータの実体宣言部======//



//--------------------------------------------衛星の力学計算-----------------------------------------------------------

/**
  * @brief  衛星のダイナミクスとキネマティクスを計算する
  * @param  in_Xk          float     状態量　クォータニオン+角速度
  * @param  in_Torque_b_   float     トルク
  * @param  out_K          float     更新状態量　クォータニオン+角速度
  */
void DynamicsKinematics(const float* in_Xk, const float* in_Torque_b_, float* out_K)
{
	float Quaternion_dot[4] = {};
	float Omega_dot[3] = {};
    float Buf1[3] = {};
    float Buf2[3] = {};
    float Quaternion_i2b_[4] = {};
    float Omega[3] = {};
    float OMEGA[4][4] = {};

    for (int i = 0; i < 4; i++) { Quaternion_i2b_[i] = in_Xk[i]; }
    for (int i = 0; i < 3; i++) { Omega[i] = in_Xk[i+4]; }


    //-----Kinematics-----//
    Omega4Kinematics(Omega, OMEGA);

    multiplication(*OMEGA, 4, 4, Quaternion_i2b_, 4, 1, Quaternion_dot, 4, 1);

    for (int i = 0; i < 4; i++)
    {
    	Quaternion_dot[i] = 0.5f * Quaternion_dot[i];
    }
    //--------------------//


    //-----Dynamics-----//
    // Iω
    multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor, 3, 3, Omega, 3, 1, Buf1,3, 1);

    // -Iω
    for (int i = 0; i < 3; i++) { Buf1[i] = -Buf1[i]; }

    // -ωxIω
    Outer_product(Omega, Buf1, Buf2);

    // -ωxIω + T
    for (int i = 0; i < 3; i++) { Buf2[i] += in_Torque_b_[i]; }

    // I^-1*(-ωxIω + T)
    multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv, 3, 3, Buf2, 3, 1, Omega_dot, 3, 1);
    //--------------------//

    for (int i = 0; i < 4; i++) { out_K[i] = Quaternion_dot[i]; }
    for (int i = 0; i < 3; i++) { out_K[i+4] = Omega_dot[i]; }

}


/**
  * @brief  4次ルンゲクッタを一回解く
  * @param  in_X           float     状態量　クォータニオン+角速度
  * @param  in_Torque_b_   float     トルク
  * @param  out_Next_K     float     更新状態量　クォータニオン+角速度
  */
void RungeOneStep(const float in_dt, const float* in_X, const float* in_Torque_b_, float* out_Next_x)
{
	float dt = in_dt;
	float Next_quaternion[4];
	float K1[7], K2[7], K3[7], K4[7];
	float Xk2[7], Xk3[7], Xk4[7];
	float Quaternion_i2b_[4];

	DynamicsKinematics(in_X, in_Torque_b_, K1);
	for (int i = 0; i < 7; i++) { Xk2[i] = in_X[i] + (dt / 2.0f) * K1[i]; }
	DynamicsKinematics(Xk2, in_Torque_b_, K2);
	for (int i = 0; i < 7; i++) { Xk3[i] = in_X[i] + (dt / 2.0f) * K2[i]; }
	DynamicsKinematics(Xk3, in_Torque_b_, K3);
	for (int i = 0; i < 7; i++) { Xk4[i] = in_X[i] + dt * K3[i]; }
	DynamicsKinematics(Xk4, in_Torque_b_, K4);
	for (int i = 0; i < 7; i++) { out_Next_x[i] = in_X[i] + (dt / 6.0f) * (K1[i] + 2.0f * K2[i] + 2.0f * K3[i] + K4[i]); }

	for (int i = 0; i < 4; i++) { Quaternion_i2b_[i] = out_Next_x[i]; }
	Normalization(Quaternion_i2b_, sizeof(Quaternion_i2b_) / sizeof(Quaternion_i2b_[0]), Next_quaternion);
	for (int i = 0; i < 4; i++) { out_Next_x[i] = Next_quaternion[i]; }
}



/**
  * @brief  衛星のキネマティクスとジャイロバイアスを計算する
  * @param  in_Xk          float     状態量　　クォータニオン+（角速度）+ジャイロバイアス
  * @param  out_K          float     更新状態量　　クォータニオン+（角速度）+ジャイロバイアス
  */
void FunctionQuaGBias(const float* in_Xk, const float* in_Omega_ref, const float in_tau, float* out_K)
{
	float Quaternion_i2b_[4], Quaternion_dot[4], GBias[3], GBias_dot[3];
	float OMEGA[4][4];

    for (int i = 0; i < 4; i++) { Quaternion_i2b_[i] = in_Xk[i]; }
    for (int i = 0; i < 3; i++) { GBias[i] = in_Xk[i+4]; }


    //-----Kinematics-----//
    Omega4Kinematics(in_Omega_ref, OMEGA);

    multiplication(*OMEGA, 4, 4, Quaternion_i2b_, 4, 1, Quaternion_dot, 4, 1);

    for(int i = 0; i < 4; i++) { Quaternion_dot[i] *= 0.5f; }
    //--------------------//

    //-----Gyro Bias-----//
    for(int i = 0; i < 4; i++)	{ GBias_dot[i] = -1.0f/in_tau * GBias[i]; }
    //--------------------//

    for (int i = 0; i < 4; i++) { out_K[i] = Quaternion_dot[i]; }
    for (int i = 0; i < 3; i++) { out_K[i+4] = GBias_dot[i]; }

}

/**
  * @brief  リファレンス　Quaternion　と　ジャイロバイアス　のプロパゲート
  * @param  in_X           float     状態量　クォータニオン+ジャイロバイアス
  * @param  out_Next_K     float     更新状態量　クォータニオン+ジャイロバイアス
  */
void OneStepQuaGBias(const float in_dt, const float* in_X, const float* in_Omega_gyro, const float in_tau, float* out_Next_x)
{

	float Quaternion_i2b_[4], Next_quaternion[4], Omega_ref[3];

	float K1[7], K2[7], K3[7], K4[7];
	float Xk2[7], Xk3[7], Xk4[7];

	for (int i = 0; i < 3; i++) { Omega_ref[i] = in_Omega_gyro[i] - in_X[i+4]; }

	FunctionQuaGBias(in_X, Omega_ref, in_tau, K1);
	for (int i = 0; i < 7; i++) { Xk2[i] = in_X[i] + (in_dt / 2.0f) * K1[i]; }
	FunctionQuaGBias(Xk2, Omega_ref, in_tau, K2);
	for (int i = 0; i < 7; i++) { Xk3[i] = in_X[i] + (in_dt / 2.0f) * K2[i]; }
	FunctionQuaGBias(Xk3, Omega_ref, in_tau, K3);
	for (int i = 0; i < 7; i++) { Xk4[i] = in_X[i] + in_dt * K3[i]; }
	FunctionQuaGBias(Xk4, Omega_ref, in_tau, K4);
	for (int i = 0; i < 7; i++) { out_Next_x[i] = in_X[i] + (in_dt / 6.0f) * (K1[i] + 2.0f * K2[i] + 2.0f * K3[i] + K4[i]); }

	for (int i = 0; i < 4; i++) { Quaternion_i2b_[i] = out_Next_x[i]; }
	Normalization(Quaternion_i2b_, sizeof(Quaternion_i2b_) / sizeof(Quaternion_i2b_[0]), Next_quaternion);
	for (int i = 0; i < 4; i++) { out_Next_x[i] = Next_quaternion[i]; }
}


/**
  * @brief  残留磁気モーメントによる角速度変化を計算する
  * @param  in_Xk          float     状態量　　角速度+残留磁気モーメント
  * @param  out_K          float     更新状態量　　角速度+残留磁気モーメント
  */
void FunctionOmeRmm(const float* in_Xk, const float* in_B, const float in_tau, float* out_K)
{
	float Omega[3] = {};
	float Rmm[3] = {};
	float M[3][3] = {};
	float OMEGA[3][3] = {};

	for(int i = 0; i < 3; i++) { Omega[i] = in_Xk[i]; Rmm[i] = in_Xk[i+3]; }

    //-----Omega Propagation-----//
    float TMP1[3];
	float TMP2[3];
	float TMP3[3];

    // Rmm×B
//    OmegaCross4Kalman(Rmm, M);
//    multiplication(*M,3,3,in_B,3,1,TMP1,3,1);
    Outer_product(Rmm, in_B, TMP1);

    // Iω
    multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor, 3, 3, Omega, 3, 1, TMP2,3, 1);
    // ωxIω
    Outer_product(Omega, TMP2, TMP3);
    // Rmm×B - ωxIω
    sub(TMP1,3,1,TMP3,3,1,TMP2);
    // I^-1 (Rmm×B - ωxIω)
    multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv, 3, 3, TMP2, 3, 1, TMP1, 3, 1);


//    // Iω
//    multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor, 3, 3, Omega, 3, 1, TMP2,3, 1);
//    // ωxIω
//    OmegaCross4Kalman(Omega, OMEGA);
//    multiplication(*OMEGA,3,3,TMP2,3,1,TMP3,3,1);
//    for(int i=0; i<3; i++){
//    	TMP3[i] = -TMP3[i];
//    }
//    // I^-1 *-(-ωxIω)
//    multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv, 3, 3, TMP3, 3, 1, TMP1, 3, 1);

	for(int i = 0; i < 3; i++){
		out_K[i]   = TMP1[i];
		out_K[i+3] = -1.0f/in_tau * Rmm[i]; //-1/tau*x <-残留磁気そのものでなく推定値(差分)が変化する
	}


}

/**
  * @brief  リファレンス　角速度　と　残留磁気モーメント　のプロパゲート
  * @param  in_X           float     状態量　角速度+残留磁気モーメント
  * @param  out_Next_K     float     更新状態量　角速度+残留磁気モーメント
  */
void OneStepOmeRmm(const float in_dt, const float* in_X,  const float* in_B, const float in_tau, float* out_Next_x)
{
	float K1[6], K2[6], K3[6], K4[6];
	float Xk2[6], Xk3[6], Xk4[6];

	FunctionOmeRmm(in_X, in_B, in_tau, K1);
	for (int i = 0; i < 6; i++) { Xk2[i] = in_X[i] + (in_dt / 2.0f) * K1[i]; }
	FunctionOmeRmm(Xk2, in_B, in_tau, K2);
	for (int i = 0; i < 6; i++) { Xk3[i] = in_X[i] + (in_dt / 2.0f) * K2[i]; }
	FunctionOmeRmm(Xk3, in_B, in_tau, K3);
	for (int i = 0; i < 6; i++) { Xk4[i] = in_X[i] + in_dt * K3[i]; }
	FunctionOmeRmm(Xk4, in_B, in_tau, K4);
	for (int i = 0; i < 6; i++)	{ out_Next_x[i] = in_X[i] + (in_dt / 6.0f) * (K1[i] + 2.0f * K2[i] + 2.0f * K3[i] + K4[i]); }

}


/**
  * @brief  ダイナミクスキネマティクス計算のためのオメガ行列作成
  * @param  in_omega      float     角速度
  * @param  out_OMEGA     float     オメガ行列
  */
void Omega4Kinematics(const float* in_omega, float out_OMEGA[4][4])
{
    out_OMEGA[0][0] = 0.0f;           out_OMEGA[0][1] = in_omega[2];    out_OMEGA[0][2] = -in_omega[1];   out_OMEGA[0][3] = in_omega[0];
    out_OMEGA[1][0] = -in_omega[2];   out_OMEGA[1][1] = 0.0f;           out_OMEGA[1][2] = in_omega[0];    out_OMEGA[1][3] = in_omega[1];
    out_OMEGA[2][0] = in_omega[1];    out_OMEGA[2][1] = -in_omega[0];   out_OMEGA[2][2] = 0.0f;           out_OMEGA[2][3] = in_omega[2];
    out_OMEGA[3][0] = -in_omega[0];   out_OMEGA[3][1] = -in_omega[1];   out_OMEGA[3][2] = -in_omega[2];   out_OMEGA[3][3] = 0.0f;
}


void Quaternion_obs_update(const float* Quaternion_ref, const float* Dqv, float* Dq_ref, uint8_t* p_dq_valid){

	float Dq[4];

    Dq[0] = Dqv[0];
    Dq[1] = Dqv[1];
    Dq[2] = Dqv[2];

    if ( 1 < Dqv[0] * Dqv[0] + Dqv[1] * Dqv[1] + Dqv[2] * Dqv[2] ){
    	// avoid nan
    	*p_dq_valid = 0;
    	Dq[0] = 0; Dq[1] = 0; Dq[2] = 0; Dq[3] = 1;
    }
    else {
    	*p_dq_valid = 1;
    	Dq[3] = sqrtf(1 - (Dqv[0] * Dqv[0] + Dqv[1] * Dqv[1] + Dqv[2] * Dqv[2]));    //ノルムが1となるようにdq4を決定
    }

    Quaternion_Product(Quaternion_ref, Dq, Dq_ref);

}


/**
 * @brief  カルマンフィルタに用いる角速度行列の生成 [ω×]
 * @param  in_omega 角速度
 * @param  OUT_M 3*3行列
 * @return  なし
 */
void OmegaCross4Kalman(const float in_omega[3], float OUT_M[3][3])
{
	OUT_M[0][0] = 0.0f;          OUT_M[0][1] = -in_omega[2];   OUT_M[0][2] = in_omega[1];
	OUT_M[1][0] = in_omega[2];  OUT_M[1][1] = 0.0f;          OUT_M[1][2] = -in_omega[0];
	OUT_M[2][0] = -in_omega[1];   OUT_M[2][1] = in_omega[0];  OUT_M[2][2] = 0.0f;
}


/**
 * @brief  カルマンフィルタに用いる角速度行列の生成 [×ω]
 * @param  in_omega 角速度
 * @param  OUT_M 3*3行列
 * @return  なし
 */
void CrossOmega4Kalman(const float in_omega[3], float OUT_M[3][3])
{
	OUT_M[0][0] = 0.0f;          OUT_M[0][1] = in_omega[2];   OUT_M[0][2] = -in_omega[1];
	OUT_M[1][0] = -in_omega[2];  OUT_M[1][1] = 0.0f;          OUT_M[1][2] = in_omega[0];
	OUT_M[2][0] = in_omega[1];   OUT_M[2][1] = -in_omega[0];  OUT_M[2][2] = 0.0f;
}

/**
 * @brief  クォータニオン積を求める
 * @param  in_q1 クォータニオン
 * @param  in_q2 クォータニオン
 * @param  OUT_q1q2 クォータニオン積
 * クォータニオン計算便利ノート
 * @return  なし
 */
void Quaternion_Product(const float in_q1[4], const float in_q2[4], float out_q1q2[4])
{
    float Mat[4][4];
    Mat[0][0] = in_q1[3];      Mat[0][1] = -in_q1[2];    Mat[0][2] = in_q1[1];   Mat[0][3] = in_q1[0];
    Mat[1][0] = in_q1[2];      Mat[1][1] = in_q1[3];     Mat[1][2] = -in_q1[0];  Mat[1][3] = in_q1[1];
    Mat[2][0] = -in_q1[1];     Mat[2][1] = in_q1[0];     Mat[2][2] = in_q1[3];   Mat[2][3] = in_q1[2];
    Mat[3][0] = -in_q1[0];     Mat[3][1] = -in_q1[1];    Mat[3][2] = -in_q1[2];  Mat[3][3] = in_q1[3];

    multiplication(*Mat, sizeof(Mat) / sizeof(Mat[0]), sizeof(Mat[0]) / sizeof(Mat[0][0]), in_q2, 4, 1, out_q1q2, 4, 1);
}


/**
 * @brief  共役クォータニオン
 * @param  in_q クォータニオン
 * @param  out_q_conj 共役クォータニオン
 * クォータニオン計算便利ノート
 * @return  なし
 */
void Quaternion_Conjugate(float in_q[4], float out_q1_conj[4])
{
    for(int i=0; i<3; i++){
    	out_q1_conj[i] = -in_q[i];
    }
    out_q1_conj[3] = in_q[3];
}

//---------------------------------------------------------------------------------------------------------------------------



/**
  * @brief  クォータニオンからDCMへの変換
  * @param  In_Qua float       クォータニオン
  * @param  Out_DCM float      DCM
  */
void Quaternion_To_DCM(const float* in_Qua, float* out_DCM)
{
	float QMAT1[3][3] = {}, QMAT2[3][3] = {};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j){
				QMAT1[i][j] = 0.0f;
				QMAT1[i][j] = in_Qua[3] * in_Qua[3] - in_Qua[0] * in_Qua[0] - in_Qua[1] * in_Qua[1] - in_Qua[2] * in_Qua[2];
			}
			else
				QMAT1[i][j] = 0.0f;
		}
	}

	multiplication(in_Qua, 3, 1, in_Qua, 1, 3, *QMAT2, 3, 3);

	float QMAT3[3][3] = { {0.0f,       -in_Qua[2], in_Qua[1]},
				          {in_Qua[2],  0.0f,      -in_Qua[0]},
				          {-in_Qua[1], in_Qua[0],  0.0f     } };

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			out_DCM[i * 3 + j] = QMAT1[i][j] + 2.0f * QMAT2[i][j] - 2.0f * in_Qua[3] * QMAT3[i][j];
		}
	}

}


/**
  * @brief  DCMからクォータニオンへの変換
  * @param  In_DCM  float      DCM
  * @param  Out_Qua float      クォータニオン
  */
void DCM_To_Quaternion(const float* in_DCM, float* out_Qua)
{
	float Mat1[3][3] = {};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Mat1[i][j] = in_DCM[i * 3 + j];
		}
	}

	float q_Buf[4] = {sqrtf(1 + Mat1[0][0] - Mat1[1][1] - Mat1[2][2]) * 0.5f,
					  sqrtf(1 - Mat1[0][0] + Mat1[1][1] - Mat1[2][2]) * 0.5f,
					  sqrtf(1 - Mat1[0][0] - Mat1[1][1] + Mat1[2][2]) * 0.5f,
					  sqrtf(1 + Mat1[0][0] + Mat1[1][1] + Mat1[2][2]) * 0.5f };

    float max = 0;        /* 最大値格納用の変数maxを0で初期化 */

	for (int i = 0; i < 4; i++) {
		 if (q_Buf[i] > max) {   /* 配列要素の値がmaxより大きかったら */
				  max = q_Buf[i]; /* その値を新しい最大値としてmaxに格納 */
		 }
	}

	//最大のクォータニオン成分により場合分け
	if (max == q_Buf[0]){
		out_Qua[0] = q_Buf[0];
		out_Qua[1] = 0.25f * (Mat1[0][1] + Mat1[1][0]) / out_Qua[0];
		out_Qua[2] = 0.25f * (Mat1[0][2] + Mat1[2][0]) / out_Qua[0];
		out_Qua[3] = 0.25f * (Mat1[1][2] - Mat1[2][1]) / out_Qua[0];
	}

	if (max == q_Buf[1]){
		out_Qua[1] = q_Buf[1];
		out_Qua[0] = 0.25f * (Mat1[0][1] + Mat1[1][0]) / out_Qua[1];
		out_Qua[2] = 0.25f * (Mat1[2][1] + Mat1[1][2]) / out_Qua[1];
		out_Qua[3] = 0.25f * (Mat1[2][0] - Mat1[0][2]) / out_Qua[1];
	}

	if (max == q_Buf[2]){
		out_Qua[2] = q_Buf[2];
		out_Qua[0] = 0.25f * (Mat1[2][0] + Mat1[0][2]) / out_Qua[2];
		out_Qua[1] = 0.25f * (Mat1[2][1] + Mat1[1][2]) / out_Qua[2];
		out_Qua[3] = 0.25f * (Mat1[0][1] - Mat1[1][0]) / out_Qua[2];
	}

	if (max == q_Buf[3]){
		out_Qua[3] = q_Buf[3];
		out_Qua[0] = 0.25f * (Mat1[1][2] - Mat1[2][1]) / out_Qua[3];
		out_Qua[1] = 0.25f * (Mat1[2][0] - Mat1[0][2]) / out_Qua[3];
		out_Qua[2] = 0.25f * (Mat1[0][1] - Mat1[1][0]) / out_Qua[3];
	}


}



//--------------------------------------------座標計算(GPS)------------------------------------------------------------------------

/**
  * @brief  緯度,経度,高さから ECEF 座標に変換
  * @param  in_phi     float      緯度
  * @param  in_ramda   float      経度
  * @param  in_height  float      高度
  * @param  out_ECEF   float      ECEF座標におけるGPS座標
  * https://www.enri.go.jp/~fks442/K_MUSEN/1st/1st060428rev2.pdf
  */
void blh2ecef(float in_phi, float in_ramda, float in_height, float *out_ECEF)
{
	if(in_phi == 0.0f && in_ramda == 0.0f && in_height == 0.0f){
		out_ECEF[0] = 0.0f;
		out_ECEF[1] = 0.0f;
		out_ECEF[2] = 0.0f;
		return;
	}
	out_ECEF[0] = (NN(in_phi) + in_height) * cosf(in_phi * M_PI / 180.0f) * cosf(in_ramda * M_PI / 180.0f);
	out_ECEF[1] = (NN(in_phi) + in_height) * cosf(in_phi * M_PI / 180.0f) * sinf(in_ramda * M_PI / 180.0f);
	out_ECEF[2] = (NN(in_phi) * (1.0f - E2) + in_height) * sinf(in_phi * M_PI / 180.0f);
}




/**
  * @brief  ECEF 座標から WGS84 の{緯度,経度,楕円体高}へ変換
  * @param  in_ECEF     float      ECEF座標におけるGPS座標
  * @param  out_blh     float      緯度,経度,楕円体高
  * https://www.enri.go.jp/~fks442/K_MUSEN/1st/1st060428rev2.pdf
  */
void ecef2blh(float *in_ECEF, float *out_blh)
{
	float p;
	float sita;
	p = sqrtf(in_ECEF[0] * in_ECEF[0] + in_ECEF[1] * in_ECEF[1]);
	sita = (180 / M_PI) * atan2f(in_ECEF[2] * SemiA, p * SemiB);
	/*--- 緯度 */
	out_blh[0] = (180 / M_PI) * atan2f(in_ECEF[2] + ED2 * SemiB * (CUB(sinf(sita * M_PI / 180))), (p - E2 * SemiA * (CUB(cosf(sita * M_PI / 180)))));
	/*--- 経度 */
	out_blh[1] = (180 / M_PI) * atan2f(in_ECEF[1], in_ECEF[0]);
	/*--- 高さ */
	out_blh[2] = (p / cosf(out_blh[0] * M_PI / 180)) - NN(out_blh[0]);
}

//---------------------------------------------------------------------------------------------------------------------

/**
  * @brief  ECI座標系からLVLH座標系へのDCMを計算する
  * @param  in_eci_pos     	float	ECI座標系での位置ベクトル
  * @param  in_eci_vel     	float	ECI座標系での速度ベクトル
  * @param  out_DCM    		float   ECI座標系からLVLH座標系へのDCM
  */
void DCMeci2lvlh(float *in_eci_pos, float *in_eci_vel, float *out_DCM)
{
	float h[3] = {0.0f};	// 軌道面外方向ベクトル
	float ex[3] = {0.0f};
	float ey[3] = {0.0f};	// LVLH座標系の基底ベクトル
	float ez[3] = {0.0f};

	Normalization(in_eci_pos, 3, ex);

	Outer_product(in_eci_pos, in_eci_vel, h);
	Normalization(h, 3, ez);

	Outer_product(ez, ex, ey);

	for (int i = 0; i < 3; i++){
		out_DCM[i] = ex[i];
		out_DCM[3 + i] = ey[i];
		out_DCM[6 + i] = ez[i];
	}
}



//-------------------------------------------------汎用性をもつ行列演算-----------------------------------------------------
// ベクトル，行列ともに何行何列で考える（[1,1,1]みたいなベクトルの場合は3行1列）
//
// TODO　行列などの計算時エラー文表示。行数と列数を間違えて引数に渡したときなど
// TODO　もしくは、行列、ベクトルの受け渡しミスを減らすためテンプレート化
//
//------------------------------------------------------------------------------------------------------------------------
/**
  * @brief  行列のターミナル表示
  * @param  in_Mat float       表示させたい行列(ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  * @param  in_Matline int     行列の行数
  * @param  in_Matrow int      行列の列数(ベクトルの場合は1)
  */
void display(float *in_Mat, int in_Matline, int in_Matrow)
{
    for (int i = 0; i < in_Matline; i++) {
        for (int j = 0; j < in_Matrow; j++) {
            printf("%f ", in_Mat[i * in_Matrow + j]);
        }
        if(in_Matrow != 1) printf("\n");
    }
    printf("\r\n");
}


/**
  * @brief  行列のターミナル表示の指数表示バージョン
  * @param  in_Mat float       表示させたい行列(ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  * @param  in_Matline int     行列の行数
  * @param  in_Matrow int      行列の列数(ベクトルの場合は1)
  */
void display_exp(float *in_Mat, int in_Matline, int in_Matrow)
{
    for (int i = 0; i < in_Matline; i++) {
        for (int j = 0; j < in_Matrow; j++) {
            printf("%.2e ", in_Mat[i * in_Matrow + j]);
        }
        if(in_Matrow != 1) printf("\n");
    }
    printf("\r\n");
}



/**
  * @brief  外積計算
  * @param  in_Vec1   float     被演算ベクトル1
  * @param  in_Vec2   float     被演算ベクトル2
  * @param  out_Vec3  float     演算後ベクトル
  * out_Vec3 = inVec1 x inVec2
  */
void Outer_product(const float* in_Vec1, const float* in_Vec2, float* out_Vec3)
{
    out_Vec3[0] = in_Vec1[1] * in_Vec2[2] - in_Vec1[2] * in_Vec2[1];
    out_Vec3[1] = in_Vec1[2] * in_Vec2[0] - in_Vec1[0] * in_Vec2[2];
    out_Vec3[2] = in_Vec1[0] * in_Vec2[1] - in_Vec1[1] * in_Vec2[0];
}




/**
  * @brief  行列の足し算
  * @param  in_Mat1 float       被演算行列1(ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  * @param  in_Matline1 int     被演算行列1の行数
  * @param  in_Matrow1 int      被演算行列1の列数(ベクトルの場合は1)
  * @param  in_Mat2 float       被演算行列2(ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  * @param  in_Matline2 int     被演算行列2の行数
  * @param  in_Matrow2 int      被演算行列2の列数(ベクトルの場合は1)
  * @param  out_Mat float       演算後行列(ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  */
void add(const float* in_Mat1, int in_Matline1, int in_Matrow1, const float* in_Mat2, int in_Matline2, int in_Matrow2, float* out_Mat)
{
    for (int i = 0; i < in_Matline1; i++) {
        for (int j = 0; j < in_Matrow1; j++) {
        	out_Mat[i * in_Matrow1 + j] = 0.0;
            out_Mat[i * in_Matrow1 + j] = in_Mat1[i * in_Matrow1 + j] + in_Mat2[i * in_Matrow2 + j];
        }
    }
}


/**
  * @brief  行列の引き算
  * @param  in_Mat1 float       被演算行列1(ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  * @param  in_Matline1 int     被演算行列1の行数
  * @param  in_Matrow1 int      被演算行列1の列数(ベクトルの場合は1)
  * @param  in_Mat2 float       被演算行列2(ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  * @param  in_Matline2 int     被演算行列2の行数
  * @param  in_Matrow2 int      被演算行列2の列数(ベクトルの場合は1)
  * @param  out_Mat float       演算後行列(ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  */
void sub(const float *in_Mat1, int in_Matline1, int in_Matrow1, const float *in_Mat2, int in_Matline2, int in_Matrow2, float *out_Mat)
{
    for (int i = 0; i < in_Matline1; i++) {
        for (int j = 0; j < in_Matrow1; j++) {
        	out_Mat[i * in_Matrow1 + j] = 0.0;
            out_Mat[i * in_Matrow1 + j] = in_Mat1[i * in_Matrow1 + j] - in_Mat2[i * in_Matrow2 + j];
        }
    }
}


/**
  * @brief  行列の掛け算
  * @param  in_Mat1 float       被演算行列1 シングルポインタ （*Mat1のように）渡す
  * @param  in_Matrow1 int     被演算行列1の行数
  * @param  in_Matcol1 int      被演算行列1の列数
  * @param  in_Mat2 float       被演算行列2 シングルポインタ （*Mat1のように）渡す
  * @param  in_Matrow2 int     被演算行列2の行数
  * @param  in_Matcol2 int      被演算行列2の列数
  * @param  out_Mat float       演算後行列シングルポインタ （*Mat1のように）渡す
  * @param  out_Matrow int     演算後行列の行数
  * @param  out_Matcol int      演算後行列の列数
  * @cation テンソル積の場合は第5引数と第6引数を入れ替える(ベクトルの行を列に、列を行にする感じ)
  */
void multiplication(const float *in_Mat1, int in_Matrow1, int in_Matcol1, const float *in_Mat2, int in_Matrow2, int in_Matcol2, float *out_Mat, int out_Matrow, int out_Matcol)
{

//	if ( sizeof(in_Mat1[0][0]) != in_Matcol1 ){
//		printf("Mat1 size error\r\n");
//	}
//
//	if (sizeof(*in_Mat2) / sizeof(in_Mat2[0]) != in_Matrow2 || sizeof(in_Mat2[0]) / sizeof(in_Mat2[0][0]) != in_Matcol2 ){
//		printf("Mat1 size error\r\n");
//	}

    for (int i = 0; i < in_Matrow1; i++) {
        for (int j = 0; j < in_Matcol2; j++) {
        	out_Mat[i * out_Matcol + j] = 0.0;
            for (int k = 0; k < in_Matcol1; k++) {
            	out_Mat[i * out_Matcol + j] += in_Mat1[i * in_Matcol1 + k] * in_Mat2[k * in_Matcol2 + j];
            }
        }
    }
}


/**
  * @brief  行列の転置
  * @param  in_Mat float       被転置行列1 (ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  * @param  in_Matline int     被転置行列1の行数
  * @param  in_Matrow int      被転置行列1の列数(ベクトルの場合は1)
  * @param  in_Mat float       転置後行列2 (ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  * @param  inout_Matline int  転置後行列1の行数(ベクトルの場合は1)
  * @param  inout_Matrow int   転置後行列1の列数
  */
void trans(float *in_Mat, int in_Matline, int in_Matrow, float *out_Mat, int inout_Matline, int inout_Matrow)
{
    for (int i = 0; i < in_Matrow; i++) {
        for (int j = 0; j < in_Matline; j++) {
        	out_Mat[i * inout_Matrow + j] = 0.0;
        	out_Mat[i * inout_Matrow + j] = in_Mat[j * in_Matrow + i];
        }
    }
}


/**
  * @brief  ベクトルに係数をかける
  * @param  in_Vec float       ベクトル　配列の先頭ポインタを渡す
  * @param  in_Vecsize int     ベクトルの次元
  * @param  in_coeff float     係数
  * @param  out_Vec float      計算後ベクトル
  */
void CoeffVec(float *in_Vec, int in_Vecsize, float in_coeff, float *out_Vec)
{
    for (int i = 0; i < in_Vecsize; i++) {
    	//out_Vec[i] = 0.0; 20220618 inakawa この行をコメントアウトすると入出力を同一ベクトルとできる
    	out_Vec[i] = in_coeff * in_Vec[i];
    }
}


/**
  * @brief  ベクトルを正規化
  * @param  in_Vec float       正規化するベクトル　配列の先頭ポインタを渡す
  * @param  in_Vecsize int     ベクトルの次元
  * @param  out_Vec            計算後ベクトル
  */
void Normalization(const float *in_Vec, int in_Vecsize, float *out_Vec)
{
    //一旦バッファを取り、処理としてはin_Vecとout_Vecに同一アドレスを指定しても正常に動作するようにする
    float temp_Vec[in_Vecsize];
    for(int i = 0; i < in_Vecsize; i++) {
        temp_Vec[i] = in_Vec[i];
    }

    float buf = 0.0;
    for (int i = 0; i < in_Vecsize; i++) {
    	buf += temp_Vec[i] * temp_Vec[i];
    }

   // not to divide by zero     _taguchi
    if(buf == 0){
        buf += 0.01;
    }
	buf = sqrtf(buf);
    for (int i = 0; i < in_Vecsize; i++) {
    	out_Vec[i] = temp_Vec[i] / buf;
    }
}

/**
  * @brief  ベクトルの絶対値を計算
  * @param  in_Vec float       絶対値を取るベクトル　配列の先頭ポインタを渡す
  * @param  in_Vecsize int     ベクトルの次元
  */
float Fabs(const float *in_Vec, int in_Vecsize)
{
    float out_fabs = 0.0f;
    for (int i = 0; i < in_Vecsize; i++) {
        out_fabs += in_Vec[i] * in_Vec[i];
    }
    out_fabs = sqrtf(out_fabs);

   // not to divide by zero
   // prevent  1/(mags * mags)  from being infinity
   // For example in Compensation.cpp             _taguchi
    if(out_fabs == 0){
        out_fabs += 0.01;
    }
    return out_fabs;
}

/**
  * @brief  n次の単位行列を生成する
  * @param  in_Matline int    生成する単位行列の行数
  * @param  in_Matrow int     生成する単位行列の列数
  * @param  out_Mat           出力単位行列
  */
void Identity(int in_Matline, int in_Matrow, float *out_Mat)
{
    for (int i = 0; i < in_Matrow; i++) {
        for (int j = 0; j < in_Matline; j++) {
        	out_Mat[i * in_Matrow + j] = 0.0f;
            if ((i * in_Matrow + j) % (in_Matrow + 1) == 0) {
            	out_Mat[i * in_Matrow + j] = 1.0f;
            }
            else {
            	out_Mat[i * in_Matrow + j] = 0.0f;
            }
        }
    }
}

/**
  * @brief  n次のゼロ行列を生成する
  * @param  in_Matline int    生成する単位行列の行数
  * @param  in_Matrow int     生成する単位行列の列数
  * @param  out_Mat           出力行列
  */
void Zeros(int in_Matline, int in_Matrow, float *out_Mat)
{
    for (int i = 0; i < in_Matline; i++) {
        for (int j = 0; j < in_Matrow; j++) {
        	out_Mat[i * in_Matrow + j] = 0.0f;
        }
    }
}

/**
  * @brief  行列に係数をかける
  * @param  in_Mat float       被演算行列1 シングルポインタ （*Mat1のように）渡す
  * @param  in_Matrow int      被演算行列1 の行数
  * @param  in_Matcol int      被演算行列1 の列数
  * @param  in_coeff float     係数
  * @param  out_Mat float      演算後行列1 シングルポインタ （*Mat1のように）渡す
  */
void CoeffMat(const float *in_Mat, int in_Matrow, int in_Matcol, float in_coeff, float *out_Mat)
{
//	float TMP[in_Matrow * in_Matcol] = {};
	for (int i = 0; i < in_Matrow; i++) {
		for (int j = 0; j < in_Matcol; j++) {
			out_Mat[i * in_Matrow + j] = in_coeff * in_Mat[i * in_Matrow + j];
		}
    }
}

/**
  * @brief  行列の左上一部を抜き出す
  * @param  in_Mat float       被演算行列1 シングルポインタ （*Mat1のように）渡す
  * @param  out_Matrow int     演算後行列1 の行数
  * @param  out_Matcol int     演算後行列1 の列数
  * @param  out_Mat float      演算後行列1 シングルポインタ （*Mat1のように）渡す
  */
void ExtractMat(const float *in_Mat, int out_Matrow, int out_Matcol, float *out_Mat)
{
	for(int i=0; i<out_Matrow; i++){
		for(int j=0; j<out_Matcol; j++){
			out_Mat[i * out_Matrow + j] = in_Mat[i * out_Matrow + j];
		}
	}
}

/**
  * @brief  ベクトルの一部を抜き出す
  * @param  in_Vec float       被演算ベクトル1 シングルポインタ （*Mat1のように）渡す
  * @param  out_Start int		抜き出し開始位置インデックス
  * @param  out_End int 		 抜き出し最後位置インデックス
  * @param  out_Vec float      演算後ベクトル1 シングルポインタ （*Mat1のように）渡す
  */
void ExtractVec(const float *in_Vec, int out_Start, int out_End, float *out_Vec){
	for(int i=out_Start; i<out_End+1; i++){
		out_Vec[i-out_Start] = in_Vec[i];
	}
}

/**
  * @brief  行列に少なくとも1要素NaNか含まれているか判断する
  * @param  in_Mat float     被演算行列1 シングルポインタ （*Mat1のように）渡す
  * @param  in_Matrow int    被演算行列1 の行数
  * @param  in_Matrow int    被演算行列1 の列数
  */
int isnanMat(const float *in_Mat, int in_Matrow, int in_Matcol){

	for(int i=0; i<in_Matrow; i++){
		for(int j=0; j<in_Matcol; j++){
			if(isnan(in_Mat[i * in_Matrow + j])) return 1;
		}
	}
	return 0;
}


/**
  * @brief  正方行列の行列式を計算する
  * @param  in_Mat1 float       被演算行列  2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す
  * @param  deg int     		被演算行列の次数
  * @param  out_det float       行列式の値
  */
float determinant(float *in_Mat1, int in_deg)
{
    //元と行列が書き変わらないようにtemp_Matに格納
	float temp_Mat[in_deg * in_deg] = {};
    for(int i = 0; i < in_deg; i++) {
    	for (int j = 0; j < in_deg; j++){
            temp_Mat[i * in_deg + j] = in_Mat1[i * in_deg + j];
    	}
    }

	int x, y, i;
	double det = 1, r;
	float buf = 0.0f;

	// 上三角行列に変換しつつ、対角成分の積を計算する。
	for(y = 0; y < in_deg - 1; y++){
		if(temp_Mat[y * in_deg + y] == 0){
			// 対角成分が0だった場合は、その列の値が0でない行と交換する
			for(i = y + 1; i < in_deg; i++){
				if(temp_Mat[i * in_deg + y] != 0){
					break;
				}
			}
			if(i < in_deg){
				for(x = 0; x < in_deg; x++){
					buf = temp_Mat[i * in_deg + x];
					temp_Mat[i * in_deg + x] = temp_Mat[y * in_deg + x];
					temp_Mat[y * in_deg + x] = buf;
				}
				// 列を交換したので行列式の値の符号は反転する。
				det = -det;
			}
		}
		for(i = y + 1; i < in_deg; i++){
			r = temp_Mat[i * in_deg + y] / temp_Mat[y * in_deg + y];
			for(x = y; x < in_deg; x++){
				temp_Mat[i * in_deg + x] -= r * temp_Mat[y * in_deg + x];
			}
		}
		det *= temp_Mat[y * in_deg + y];
	}
	det *= temp_Mat[y * in_deg + y];

	return det;
}

/**
  * @brief  逆行列を計算する
  * @param  in_Mat1 float       被演算行列1 2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す
  * @param  in_Matline1 int     被演算行列1の行数
  * @param  in_Matrow1 int      被演算行列1の列数
  * @param  in_Mat2 float       バッファ行列 2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す
  * @param  out_Mat float       演算後行列 2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す
  */
void Inverse(float *in_Mat1, int in_Matline1, int in_Matrow1, float *in_Mat2, float *out_Mat)
{
    float buf1 = 0.0f;

    for (int i = 0; i < in_Matline1; i++) {
        for (int j = 0; j < in_Matrow1; j++) {
        	in_Mat2[i * in_Matrow1 + j] = in_Mat1[i * in_Matrow1 + j];
        }
    }

    Identity(in_Matline1, in_Matrow1, out_Mat);


    for (int i = 0; i < in_Matrow1; i++) {
        for (int j = 0; j < in_Matline1; j++) {
            if ((i * in_Matrow1 + j) % (in_Matrow1 + 1) == 0) {
                if(in_Mat2[i * in_Matrow1 + j] == 0){
                	// nan 回避
                	buf1 = 1.0f;
                }
                else{
                	buf1 = 1 / in_Mat2[i * in_Matrow1 + j];
                }
            }
        }
        for (int j = 0; j < in_Matline1; j++) {
        	in_Mat2[i * in_Matrow1 + j] *= buf1;
        	out_Mat[i * in_Matrow1 + j] *= buf1;
        }

        for (int j = 0; j < in_Matline1; j++) {
            if ((i * in_Matrow1 + j) % (in_Matrow1 + 1) != 0) {
                buf1 = in_Mat2[j * in_Matrow1 + i];
                for (int k = 0; k < in_Matrow1; k++) {
                	in_Mat2[j * in_Matrow1 + k] -= in_Mat2[i * in_Matrow1 + k] * buf1;
                    out_Mat[j * in_Matrow1 + k] -= out_Mat[i * in_Matrow1 + k] * buf1;
                }
            }
        }

    }

}

/**
  * @brief  行列のコピー
  * @param  in_Mat      float       コピー元行列(ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  * @param  in_Matline  int         コピー元の行数
  * @param  in_Matrow   int         コピー元の列数(ベクトルの場合は1)
  * @param  out_Mat     float       コピー先行列(ベクトル含む) 　2次元配列(行列)の場合、シングルポインタ （*Mat1のように）渡す 1次元配列(ベクトル)の場合、配列の先頭ポインタを渡す
  */
void copy(const float *in_Mat, int in_Matline, int in_Matrow, float *out_Mat)
{
    for (int i = 0; i < in_Matline; i++) {
        for (int j = 0; j < in_Matrow; j++) {
        	out_Mat[i * in_Matrow + j] = 0.0;
            out_Mat[i * in_Matrow + j] = in_Mat[i * in_Matrow + j];
        }
    }
}

/**
  * @brief  quaternionのクリア
  * @param  inout_quaternion  初期化したいクオータニオン
  */
void initQuaternion(float *inout_quaternion)
{
	inout_quaternion[0] = 0.0f;
	inout_quaternion[1] = 0.0f;
	inout_quaternion[2] = 0.0f;
	inout_quaternion[3] = 1.0f;
}

/**
  * @brief  2本のベクトルのなす角を求める
  * @param  in_vec_1	入力ベクトル1
  * @param  in_vec_2 	入力ベクトル2
  * @param  vec_size  	ベクトルのサイズ
  * @param  mode  		出力角度の単位（0:[rad], 1:[deg]）
  */
float VecAngle(float *in_vec_1, float *in_vec_2, int vec_size, int mode){

	float vec1[3] = {0.0f};
	float vec2[3] = {0.0f};
	float dot = 0.0f;
	float angle = 0.0f;

	Normalization(in_vec_1, vec_size, vec1);
	Normalization(in_vec_2, vec_size, vec2);

	for (int i = 0; i < vec_size; i++){
		dot += vec1[i] * vec2[i];
	}

	if(dot > 1.0f) { dot = 1.0f; }
	else if(dot < -1.0f) { dot = -1.0f; }

	if(mode == 0){
		angle = acosf(dot);
	}
	else{
		angle = acosf(dot) * 180.0f / (float)M_PI;
	}

	return angle;
}


//-------------------------------------------------汎用性をもつ回転行列演算-----------------------------------------------------
// ベクトル，行列ともに三次元のもののみ考える
//
//
//------------------------------------------------------------------------------------------------------------------------

/**
  * @brief  ベクトルに対し、一次軸での回転を計算する。ベクトル回転と座標系回転両方に対応
  * @param  in_Vec      float   回転を施すベクトル　三次元のみ対応
  * @param  in_mode     int     計算モード      0:ベクトル回転　1:座標系回転
  * @param  in_axis     int     回転軸         0:x 1:y 2:z
  * @param  in_theta    float   回転角度(rad)
  * @param  out_Vec     float   回転後のベクトル 一旦バッファを取るため、処理としてはin_Vecとout_Vecに同一アドレスを指定しても正常に動作する
  */
void Rotate_one_axis(const float* in_Vec, int in_mode, int in_axis, float in_theta, float* out_Vec)
{
    //座標系回転なら角度が逆のベクトル計算として計算
    if(in_mode == 1) {
        in_theta = - in_theta;
    }

    //一旦バッファを取り、処理としてはin_Vecとout_Vecに同一アドレスを指定しても正常に動作するようにする
    float temp_Vec[3];
    for(int i = 0; i < 3; i++) {
        temp_Vec[i] = in_Vec[i];
    }

    switch (in_axis)
    {
    case 0: // x
        /* code */
        out_Vec[0] = temp_Vec[0];
        out_Vec[1] = cosf(in_theta) * temp_Vec[1] - sinf(in_theta) * temp_Vec[2];
        out_Vec[2] = sinf(in_theta) * temp_Vec[1] + cosf(in_theta) * temp_Vec[2];

        break;

    case 1: //y

        out_Vec[0] = cosf(in_theta) * temp_Vec[0] + sinf(in_theta) * temp_Vec[2];
        out_Vec[1] = temp_Vec[1];
        out_Vec[2] = -sinf(in_theta) * temp_Vec[0] + cosf(in_theta) * temp_Vec[2];

        break;

    case 2: //z

        out_Vec[0] = cosf(in_theta) * temp_Vec[0] - sinf(in_theta) * temp_Vec[1];
        out_Vec[1] = sinf(in_theta) * temp_Vec[0] + cosf(in_theta) * temp_Vec[1];
        out_Vec[2] = temp_Vec[2];
        break;

    
    default:
    	//引数が定義外の場合は計算を行わない
        break;
    }
}

/**
  * @brief  3*3の回転行列を生成する
  * @param  in_axis     int     回転軸         0:x 1:y 2:z
  * @param  in_theta    float   回転角度(rad)
  * @param  out_MATRIX  float   生成された回転行列
  */
void make_rotate_matrix(int in_axis, float in_theta, float* out_MATRIX)
{
  switch (in_axis)
  {
  case 0: // x
      out_MATRIX[0] = 1.0f; out_MATRIX[1] = 0.0f;           out_MATRIX[2] = 0.0f;
      out_MATRIX[3] = 0.0f; out_MATRIX[4] = cosf(in_theta); out_MATRIX[5] = -sinf(in_theta);
      out_MATRIX[6] = 0.0f; out_MATRIX[7] = sinf(in_theta); out_MATRIX[8] = cosf(in_theta);
      break;

  case 1: //y
      out_MATRIX[0] = cosf(in_theta);  out_MATRIX[1] = 0.0f; out_MATRIX[2] = sinf(in_theta);
      out_MATRIX[3] = 0.0f;            out_MATRIX[4] = 1.0f; out_MATRIX[5] = 0.0f;
      out_MATRIX[6] = -sinf(in_theta); out_MATRIX[7] = 0.0f; out_MATRIX[8] = cosf(in_theta);
      break;

  case 2: //z
      out_MATRIX[0] = cosf(in_theta);  out_MATRIX[1] = -sinf(in_theta); out_MATRIX[2] = 0.0f;
      out_MATRIX[3] = sinf(in_theta);  out_MATRIX[4] = cosf(in_theta);  out_MATRIX[5] = 0.0f;
      out_MATRIX[6] = 0.0f;            out_MATRIX[7] = 0.0f;            out_MATRIX[8] = 1.0f;
      break;


  default:
      //引数が定義外の場合は計算を行わない
      break;
  }
}
