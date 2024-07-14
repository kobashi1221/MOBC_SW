/*
 * AD_EKF.cpp
 *
 *  ver.1.0 2021/06/21 masuda
 *  ver.2.0 2022/06/30 tamura
 *
 */

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Library/General/DBG_printf.h>

#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Drivers/Aocs/AD_EKF.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>

#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>

//-----------------------------------------
// ADCS　計算関数定義
//-----------------------------------------

/**
 * Reference
 * 天文観測用科学衛星の姿勢決定におけるカルマンフィルタ　　 リセット型なので 微小状態量 = 0　 微小状態量伝播は必要ない
 * Extended Kalman Filter, https://en.wikipedia.org/wiki/Extended_Kalman_filter, accessed on 202206028
 * nano-JASMINE EKF_A.cpp, EKF_A.hpp
 */

/** Nonlinear system
 *  X = f(X,u) + g(X)w
 *  Z = h(X) + v
 *
 *  X_k = f(X_k-1, u_k) + g_w(X_k)w_k
 *  Z_k = h(X_k) + v_k
 */

/** Linear system around reference point
 *  X = xref + x
 *  xref = f(xref,u)
 *  zref = h(xref)
 *  x = Ax + B(w + T)
 *  z = Hx + v
 *
 *  x_k = A_k*x_k + B_w_k*(w_k + T)
 *  z_k = H_k*x_k + v_k
 */

/** Jacobians
 *  A = df/dx
 *  H = dh/dx
 *
 * 	A_k = df / dx at x_k-1\k-1 and u_k,
 * 	H_k = dh / dx at x_k\k-1,
 */

/** Discrete state transition matrix
 *  Uses the 2nd order of the matrix exponential function. Not sure if this accuracy is necessary
 *  F_k  = E + A_k*Δt
 *  Gw_K = E*Δt * B_k
 *  行列計算が多くなりすぎるので　 2次はいらない可能性。nano-JASMINではF_k　Δtの1次項まで
 *  F_k  = E + A_k*Δt + 1/2*A_k^2*Δt^2
 *  Gw_K = (E*Δt +1/2*A_k*Δt^2 + 1/6*A_k*Δt^3) * B_k
 */

/** Predict (Estimate)
 *  Predicted state estimate:             xref = f(xref,u)        xref_k\k-1 = f(xref_k-1\k-1, u_k)
 *  Predicted covariance estimate:        P    = FPF' + GwQGw'    P_k\k-1    = F_k * P_k-1\k-1 * F_k' + Gw_K * Q_k * Gw_K'
 */

/** Update (Observe)
 *  Innovation or measurement residual:   y = z - h(xref)         y_k   = z_k - h(xref_k\k-1)
 *  Innovation (or residual) covariance:  S = HPH' + R            S_k   = H_k * P_k\k-1 * H_k' + R_k
 *  Near-optimal Kalman gain:             K = P*H'S^-1            K_k   = P_k\k-1 * H_k' * S_k^-1
 *  Updated state estimate:               x = x + Ky              x_k\k = x_k\k-1 + K*y_k
 *  Updated covariance estimate:          P = (I - KH)P           P_k\k = (I - K_k * H_k)*P_k\k-1
 */



/**
 * @brief Calculate A
 * @param in_X_ref_renew
 */
void AD_EKF_CLASS::Calc_A(const float in_X_ref[7]) {

if(ADS_Mode(EKF_GYRO)){

	float TMP1[3][3] = {};   //単位行列(0.5)
	float TMP2[3][3] = {};   //L=Iw+h
	float TMP3[3][3] = {};   //
	float TMP4[3][3] = {};   //
	float TMP6[3][3] = {};   //
	float Omega_ref[3] = {};
//	float Quaternion_ref[4] = {};
//	float I[3][3] = {};
//	float Iinv[3][3] = {};

	for (int i = 0; i < 3; i++){
		Omega_ref[i] = in_X_ref[i + 4];
	}

	OmegaCross4Kalman(Omega_ref, M);     //M
	CrossOmega4Kalman(Omega_ref, mM);    //minusM

	// 1/2 E
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j ++){
			TMP1[i][j] = (i == j) ? 0.5f : 0.0f;
		}
	}

	 //L = Iω
	multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0][0]), Omega_ref, sizeof(Omega_ref) / sizeof(Omega_ref[0]), sizeof(Omega_ref[0]) / sizeof(Omega_ref[0]), L, sizeof(L) / sizeof(L[0]), sizeof(L[0]) / sizeof(L[0]));

//	// TMP2
//	TMP2[0][0] = 0.0f;      TMP2[0][1] = L[2];       TMP2[0][2] = -L[1];
//	TMP2[1][0] = -L[2];     TMP2[1][1] = 0.0f;       TMP2[1][2] = L[0];
//	TMP2[2][0] = L[1];      TMP2[2][1] = -L[0];      TMP2[2][2] = 0.0f;

	// TMP2 tamura
	TMP2[0][0] = 0.0f;      TMP2[0][1] = -L[2];       TMP2[0][2] = L[1];
	TMP2[1][0] = L[2];     TMP2[1][1] = 0.0f;       TMP2[1][2] = -L[0];
	TMP2[2][0] = -L[1];      TMP2[2][1] = L[0];      TMP2[2][2] = 0.0f;

	// TMP3 = M*I = ω×I = -I×ω
	multiplication(*M, sizeof(M) / sizeof(M[0]), sizeof(M[0]) / sizeof(M[0][0]), *g_acs.p_GLOBAL_AOCS->Inertia_tensor, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0][0]), *TMP3, sizeof(TMP3) / sizeof(TMP3[0]), sizeof(TMP3[0]) / sizeof(TMP3[0][0]));

	// TMP4 = -TMP2 - M*I
 	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			TMP4[i][j] = -TMP2[i][j] - TMP3[i][j];
		}
	}

 	// TMP6 = I^-1*TMP4 = I^-1*(-TMP2 - M*I)
	multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][0]), *TMP4, sizeof(TMP4) / sizeof(TMP4[0]), sizeof(TMP4[0]) / sizeof(TMP4[0][0]), *TMP6, sizeof(TMP6) / sizeof(TMP6[0]), sizeof(TMP6[0]) / sizeof(TMP6[0][0]));


	A[0][0] = mM[0][0];     A[0][1] = mM[0][1];     A[0][2] = mM[0][2];      A[0][3] = TMP1[0][0];      A[0][4] = TMP1[0][1];       A[0][5] = TMP1[0][2];
	A[1][0] = mM[1][0];     A[1][1] = mM[1][1];     A[1][2] = mM[1][2];      A[1][3] = TMP1[1][0];      A[1][4] = TMP1[1][1];       A[1][5] = TMP1[1][2];
	A[2][0] = mM[2][0];     A[2][1] = mM[2][1];     A[2][2] = mM[2][2];      A[2][3] = TMP1[2][0];      A[2][4] = TMP1[2][1];       A[2][5] = TMP1[2][2];
	A[3][0] = 0.0f;        A[3][1] = 0.0f;        A[3][2] = 0.0f;         A[3][3] = TMP6[0][0];     A[3][4] = TMP6[0][1];      A[3][5] = TMP6[0][2];
	A[4][0] = 0.0f;        A[4][1] = 0.0f;        A[4][2] = 0.0f;         A[4][3] = TMP6[1][0];     A[4][4] = TMP6[1][1];      A[4][5] = TMP6[1][2];
	A[5][0] = 0.0f;        A[5][1] = 0.0f;        A[5][2] = 0.0f;         A[5][3] = TMP6[2][0];     A[5][4] = TMP6[2][1];      A[5][5] = TMP6[2][2];

}

}


/**
 * @brief Calculate B
 */
void AD_EKF_CLASS::Calc_B(){
if(ADS_Mode(EKF_GYRO)){
	B[0][0] = 0.0f;        B[0][1] = 0.0f;        B[0][2] = 0.0f;         B[0][3] = 0.0f;                                  B[0][4] = 0.0f;                                  B[0][5] = 0.0f;
	B[1][0] = 0.0f;        B[1][1] = 0.0f;        B[1][2] = 0.0f;         B[1][3] = 0.0f;                                  B[1][4] = 0.0f;                                  B[1][5] = 0.0f;
	B[2][0] = 0.0f;        B[2][1] = 0.0f;        B[2][2] = 0.0f;         B[2][3] = 0.0f;                                  B[2][4] = 0.0f;                                  B[2][5] = 0.0f;
	B[3][0] = 0.0f;        B[3][1] = 0.0f;        B[3][2] = 0.0f;         B[3][3] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][0];         B[3][4] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][1];         B[3][5] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][2];
	B[4][0] = 0.0f;        B[4][1] = 0.0f;        B[4][2] = 0.0f;         B[4][3] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][0];         B[4][4] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][1];         B[4][5] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][2];
	B[5][0] = 0.0f;        B[5][1] = 0.0f;        B[5][2] = 0.0f;         B[5][3] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][0];         B[5][4] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][1];         B[5][5] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][2];
}

if(ADS_Mode(EKF_GYROBIAS)){

}
}



/*
 * @brief Calculate F
 * 行列指数関数を1次近似
 */
void AD_EKF_CLASS::Calc_F(){

	// E + AΔt
	for (int i = 0; i < 6; i++)	{
		for (int j = 0; j < 6; j++) {
			F[i][j] = (i == j) ? 1.0f + A[i][j]*dt : A[i][j]*dt;
		}
	}

//	// E + AΔt + 1/2 A^2 Δt^2
//	float BUF6[6][6] = {};
//		float BUF7[6][6] = {};
//		float BUF8[6][6] = {};
//
//		// E
//		for (int i = 0; i < 6; i++)	{
//			for (int j = 0; j < 6; j++) {
//				BUF8[i][j] = (i == j) ? 1.0f : 0.0f;
//			}
//		}
//
//		// AΔt
//		for (int i = 0; i < 6; i++)	{
//			for (int j = 0; j < 6; j++) {
//				BUF6[i][j] = A[i][j] * dt;
//			}
//		}
//
//		// (AΔt)^2
//		multiplication(*BUF6, sizeof(BUF6) / sizeof(BUF6[0]), sizeof(BUF6[0]) / sizeof(BUF6[0][0]), *BUF6, sizeof(BUF6) / sizeof(BUF6[0]), sizeof(BUF6[0]) / sizeof(BUF6[0][0]), *BUF7, sizeof(BUF7) / sizeof(BUF7[0]), sizeof(BUF7[0]) / sizeof(BUF7[0][0]));
//
//		// E + AΔt + 1/2 A^2 Δt^2
//		for (int i = 0; i < 6; i++)	{
//			for (int j = 0; j < 6; j++) {
//				F[i][j] = BUF8[i][j] + BUF6[i][j] + BUF7[i][j] * 0.5f;
//			}
//		}

}


/*
 * @brief Calculate Gw
 * 行列指数関数を0次近似
 */
void AD_EKF_CLASS::Calc_Gw(){


	// BΔt
	for (int i = 0; i < 6; i++)	{
		for (int j = 0; j < 6; j++) {
			Gw[i][j] = B[i][j] * dt;
		}
	}


//  // (E*Δt +1/2*A_k*Δt^2 + 1/6*A_k*Δt^3) * B_k
//
//	float BUF1[6][6] = {};
//	float BUF2[6][6] = {};
//	float BUF3[6][6] = {};
//	float BUF4[6][6] = {};
//	float BUF5[6][6] = {};
//
//	for (int i = 0; i < 6; i++)	{
//		for (int j = 0; j < 6; j++) {
//			BUF1[i][j] = (i == j) ? dt : 0.0f;
//		}
//	}
//
//	for (int i = 0; i < 6; i++)	{
//		for (int j = 0; j < 6; j++) {
//			BUF2[i][j] = A[i][j] * dt * dt * 0.5f;
//		}
//	}
//
//	multiplication(*A, sizeof(A) / sizeof(A[0]), sizeof(A[0]) / sizeof(A[0][0]), *A, sizeof(A) / sizeof(A[0]), sizeof(A[0]) / sizeof(A[0][0]), *BUF3, sizeof(BUF3) / sizeof(BUF3[0]), sizeof(BUF3[0]) / sizeof(BUF3[0][0]));
//
//	for (int i = 0; i < 6; i++)	{
//		for (int j = 0; j < 6; j++) {
//			BUF4[i][j] = BUF3[i][j] * dt * dt * dt / 6.0f;
//		} //行列指数関数を2次まで近似
//	}
//
//	for (int i = 0; i < 6; i++)	{
//		for (int j = 0; j < 6; j++) {
//			BUF5[i][j] = BUF1[i][j] + BUF2[i][j] + BUF4[i][j];
//		}
//	}
//
//	multiplication(*BUF5, sizeof(BUF5) / sizeof(BUF5[0]), sizeof(BUF5[0]) / sizeof(BUF5[0][0]), *B, sizeof(B) / sizeof(B[0]), sizeof(B[0]) / sizeof(B[0][0]), *Gw, sizeof(Gw) / sizeof(Gw[0]), sizeof(Gw[0]) / sizeof(Gw[0][0]));


}


/**
 * @brief  カルマンフィルタにおける時間更新
 * @param  IN_x_ref_renew クォータニオン+角速度　事後推定値
 * @param  IN_P_ref_renew　事後誤差共分散行列
 * @param  OUT_x_ref_next クォータニオン+角速度　推定値
 * @param  OUT_P_ref_next 誤差共分散行列　推定値
 * @return  なし
 */
void AD_EKF_CLASS::Predict(float in_X_ref[7], float in_P_REF[6][6], float out_X_ref_next[7], float out_P_REF_NEXT[6][6])
{

	float BUF9[6][6] = {};
	float BUF10[6][6] = {};
	float BUF11[6][6] = {};
	float BUF12[6][6] = {};

	float F_tra[6][6] = {};
	float Gw_TRA[6][6] = {};

	float T[3] = {}; // Torque

	//システムの共分散行列
	Q[0][0] = std_Q1*std_Q1;            Q[0][1] = 0.0f;           Q[0][2] = 0.0f;           Q[0][3] = 0.0f;            Q[0][4] = 0.0f;             Q[0][5] = 0.0f;
	Q[1][0] = 0.0f;            Q[1][1] = std_Q1*std_Q1;           Q[1][2] = 0.0f;           Q[1][3] = 0.0f;            Q[1][4] = 0.0f;             Q[1][5] = 0.0f;
	Q[2][0] = 0.0f;            Q[2][1] = 0.0f;           Q[2][2] = std_Q1*std_Q1;           Q[2][3] = 0.0f;            Q[2][4] = 0.0f;             Q[2][5] = 0.0f;
	Q[3][0] = 0.0f;            Q[3][1] = 0.0f;           Q[3][2] = 0.0f;           Q[3][3] = std_Q2 * std_Q2;     Q[3][4] = 0.0f;             Q[3][5] = 0.0f;
	Q[4][0] = 0.0f;            Q[4][1] = 0.0f;           Q[4][2] = 0.0f;           Q[4][3] = 0.0f;            Q[4][4] = std_Q2 * std_Q2;      Q[4][5] = 0.0f;
	Q[5][0] = 0.0f;            Q[5][1] = 0.0f;           Q[5][2] = 0.0f;           Q[5][3] = 0.0f;            Q[5][4] = 0.0f;             Q[5][5] = std_Q2 * std_Q2;



	//---------------------リファレンス状態量の更新--------------------------------------------------

	Outer_product(g_acs.p_ACS->Mag_moment, g_acs.p_GLOBAL_AOCS->MagField, T);
//	printf("Mag_moment %f %f %f ", g_acs.p_ACS->Mag_moment[0], g_acs.p_ACS->Mag_moment[1], g_acs.p_ACS->Mag_moment[2]);
//	printf("MagFld %f %f %f ", g_acs.p_GLOBAL_AOCS->MagField[0], g_acs.p_GLOBAL_AOCS->MagField[1], g_acs.p_GLOBAL_AOCS->MagField[2]);
//	printf("T %f %f %f ", T[0], T[1], T[2]);

	RungeOneStep(dt, in_X_ref, T, out_X_ref_next);
	//----------------------------------------------------------------------------------------------------------

    if(ADS_Mode(EKF_GYRO_COV)){
	print(EKF,"COVARIANCE_CALCULATE ");
//	if(ADS_Mode(EKF_GYRO)){
		//---------------------状態方程式を線形化--------------------------------------------------
		Calc_A(in_X_ref);
		Calc_B();
		//----------------------------------------------------------------------------------------------------------

		//------------------------------------------------離散化した状態方程式の係数行列F,Gwを求める(discretization)---------------------------------------------
		//----------FPF'を計算-------------------------
		// F
		Calc_F();
		// F'
		trans(*F, sizeof(F) / sizeof(F[0]), sizeof(F[0]) / sizeof(F[0][0]), *F_tra, sizeof(F_tra) / sizeof(F_tra[0]), sizeof(F_tra[0]) / sizeof(F_tra[0][0]));
		// FP
		multiplication(*F, sizeof(F) / sizeof(F[0]), sizeof(F[0]) / sizeof(F[0][0]), *in_P_REF, 6, 6, *BUF9, sizeof(BUF9) / sizeof(BUF9[0]), sizeof(BUF9[0]) / sizeof(BUF9[0][0]));
		// FPF'
		multiplication(*BUF9, sizeof(BUF9) / sizeof(BUF9[0]), sizeof(BUF9[0]) / sizeof(BUF9[0][0]), *F_tra, sizeof(F_tra) / sizeof(F_tra[0]), sizeof(F_tra[0]) / sizeof(F_tra[0][0]), *BUF10, sizeof(BUF10) / sizeof(BUF10[0]), sizeof(BUF10[0]) / sizeof(BUF10[0][0]));
		//----------------------------------------------


		//----------GwQGw'を計算-----------------------
		// Gw
		Calc_Gw();
		// Gw'
		trans(*Gw, sizeof(Gw) / sizeof(Gw[0]), sizeof(Gw[0]) / sizeof(Gw[0][0]), *Gw_TRA, sizeof(Gw_TRA) / sizeof(Gw_TRA[0]), sizeof(Gw_TRA[0]) / sizeof(Gw_TRA[0][0]));
		// GwQ
		multiplication(*Gw, sizeof(Gw) / sizeof(Gw[0]), sizeof(Gw[0]) / sizeof(Gw[0][0]), *Q, sizeof(Q) / sizeof(Q[0]), sizeof(Q[0]) / sizeof(Q[0][0]), *BUF11, sizeof(BUF11) / sizeof(BUF11[0]), sizeof(BUF11[0]) / sizeof(BUF11[0][0]));
		// GwQGW'
		multiplication(*BUF11, sizeof(BUF11) / sizeof(BUF11[0]), sizeof(BUF11[0]) / sizeof(BUF11[0][0]), *Gw_TRA, sizeof(Gw_TRA) / sizeof(Gw_TRA[0]), sizeof(Gw_TRA[0]) / sizeof(Gw_TRA[0][0]), *BUF12, sizeof(BUF12) / sizeof(BUF12[0]), sizeof(BUF12[0]) / sizeof(BUF12[0][0]));
		//----------------------------------------------

		//-------------事後誤差共分散行列を更新-----------------------------------
		// P_next = F*P*F' + GW*Q*GW'
		add(*BUF10, sizeof(BUF10) / sizeof(BUF10[0]), sizeof(BUF10[0]) / sizeof(BUF10[0][0]), *BUF12, sizeof(BUF12) / sizeof(BUF12[0]), sizeof(BUF12[0]) / sizeof(BUF12[0][0]), *out_P_REF_NEXT);
		//--------------------------------------------------------------------------
	}
	else{
		copy(*in_P_REF,6,6,*out_P_REF_NEXT);
	}
}


/*
 * @brief Calculate y
 */
void AD_EKF_CLASS::Calc_y(float z[7], float in_X_ref[7]){
	//イノベーションの計算

	float q[4];
	float qref[4];
	float qref_conj[4];
	float err_q[4];
	float ome[3];
	float omeref[3];
	float err_ome[3];

	for(int i=0; i<4; i++){
		q[i] = z[i];
		qref[i] = in_X_ref[i];
	}
	for(int i=0; i<3; i++){
		ome[i] = z[i+4];
		omeref[i] = in_X_ref[i+4];
	}

	if(ADS_Mode(EKF_EDDY)){

	}
	else{
		//Quaternion error
		Quaternion_Conjugate(qref,qref_conj);
		Quaternion_Product(qref_conj,q,err_q);

		//Omega error
		sub(ome,3,1,omeref,3,1,err_ome);

		y[0] = err_q[0];
		y[1] = err_q[1];
		y[2] = err_q[2];
		y[3] = err_ome[0];
		y[4] = err_ome[1];
		y[5] = err_ome[2];
	}


}

/*
 * @brief Calculate H
 */
void AD_EKF_CLASS::Calc_H(){
	//非線形観測方程式のヤコビアン　微小状態量観測行列

	if(ADS_Mode(EKF_EDDY)){

	}
	else{
		Identity(sizeof(H) / sizeof(H[0]), sizeof(H[0]) / sizeof(H[0][0]), *H);
	}


}


/**
 * @brief  カルマンフィルタにおける観測更新
 * @param  IN_Observed クォータニオン+角速度　観測値
 * @param  IN_x_ref クォータニオン+角速度　事前推定値
 * @param  IN_P_ref　事前誤差共分散行列
 * @param  OUT_x_ref_renew クォータニオン+角速度　事後推定値
 * @param  OUT_P_ref_renew 事後誤差共分散行列
 * @return  なし
 */
void AD_EKF_CLASS::Observe(float in_Observed[7], float in_X_ref[7], float in_P_REF[6][6], float out_X_ref_renew[7], float out_P_REF_RENEW[6][6])
{
    float BUF1[6][6] = {};
    float BUF2[6][6] = {};
    float BUF3[6][6] = {};
    float BUF4[6][6] = {};
    float BUF5[6][6] = {};
    float BUF6[6][6] = {};
    float BUF7[6][6] = {};
    float BUF8[6][6] = {};

    float H_TRA[6][6] = {};   //Hの転置
    float K[6][6] = {};       //カルマンゲイン
    float Dx_renew[6] = {}; //観測更新微小量
    float Dqv[4] = {};
    float Dq[4] = {};
    float Domega[3] = {};
    float Omega_ref[3] = {};
    float Quaternion_ref[4] = {};
    float Dq_ref[4] = {};
    float Domega_ref[3] = {};


    //観測ノイズの共分散行列
    R[0][0] = std_R1 * std_R1;    R[0][1] = 0.0f;          R[0][2] = 0.0f;          R[0][3] = 0.0f;           R[0][4] = 0.0f;         R[0][5] = 0.0f;
    R[1][0] = 0.0f;           R[1][1] = std_R1 * std_R1;   R[1][2] = 0.0f;          R[1][3] = 0.0f;           R[1][4] = 0.0f;         R[1][5] = 0.0f;
    R[2][0] = 0.0f;           R[2][1] = 0.0f;          R[2][2] = std_R1 * std_R1;   R[2][3] = 0.0f;           R[2][4] = 0.0f;         R[2][5] = 0.0f;
    R[3][0] = 0.0f;           R[3][1] = 0.0f;          R[3][2] = 0.0f;          R[3][3] = std_R2 * std_R2;    R[3][4] = 0.0f;         R[3][5] = 0.0f;
    R[4][0] = 0.0f;           R[4][1] = 0.0f;          R[4][2] = 0.0f;          R[4][3] = 0.0f;           R[4][4] = std_R2 * std_R2;  R[4][5] = 0.0f;
    R[5][0] = 0.0f;           R[5][1] = 0.0f;          R[5][2] = 0.0f;          R[5][3] = 0.0f;           R[5][4] = 0.0f;         R[5][5] = std_R2 * std_R2;


    //----------------------イノベーション y = z - h(x_ref) を計算-------------------------------------------
    Calc_y(in_Observed, in_X_ref);
//    printf("Calc_y %f %f %f %f %f %f ", y[0], y[1], y[2] ,y[3], y[4] ,y[5]);
    //-----------------------------------------------------------------------------------------------

    //----------------------カルマンゲイン K = PH'(HPH'+R)^-1 を計算-------------------------------------------
    // H
    Calc_H();
    // H'
    trans(*H, sizeof(H) / sizeof(H[0]), sizeof(H[0]) / sizeof(H[0][0]), *H_TRA, sizeof(H_TRA) / sizeof(H_TRA[0]), sizeof(H_TRA[0]) / sizeof(H_TRA[0][0]));
    // PH'
    multiplication(*in_P_REF, 6, 6, *H_TRA, sizeof(H_TRA) / sizeof(H_TRA[0]), sizeof(H_TRA[0]) / sizeof(H_TRA[0][0]), *BUF1, sizeof(BUF1) / sizeof(BUF1[0]), sizeof(BUF1[0]) / sizeof(BUF1[0][0]));
    // HP
    multiplication(*H, sizeof(H) / sizeof(H[0]), sizeof(H[0]) / sizeof(H[0][0]), *in_P_REF, 6, 6, *BUF2, sizeof(BUF2) / sizeof(BUF2[0]), sizeof(BUF2[0]) / sizeof(BUF2[0][0]));
    // HPH'
    multiplication(*BUF2, sizeof(BUF2) / sizeof(BUF2[0]), sizeof(BUF2[0]) / sizeof(BUF2[0][0]), *H_TRA, sizeof(H_TRA) / sizeof(H_TRA[0]), sizeof(H_TRA[0]) / sizeof(H_TRA[0][0]), *BUF3, sizeof(BUF3) / sizeof(BUF3[0]), sizeof(BUF3[0]) / sizeof(BUF3[0][0]));
    // HPH'+R
    add(*BUF3, sizeof(BUF3) / sizeof(BUF3[0]), sizeof(BUF3[0]) / sizeof(BUF3[0][0]), *R, sizeof(R) / sizeof(R[0]), sizeof(R[0]) / sizeof(R[0][0]), *BUF4);
    // (HPH'+R)^-1
    Inverse(*BUF4, sizeof(BUF4) / sizeof(BUF4[0]), sizeof(BUF4[0]) / sizeof(BUF4[0][0]), *BUF8, *BUF5);
    // K = PH'(HPH'+R)^-1
    multiplication(*BUF1, sizeof(BUF1) / sizeof(BUF1[0]), sizeof(BUF1[0]) / sizeof(BUF1[0][0]), *BUF5, sizeof(BUF5) / sizeof(BUF5[0]), sizeof(BUF5[0]) / sizeof(BUF5[0][0]), *K, sizeof(K) / sizeof(K[0]), sizeof(K[0]) / sizeof(K[0][0]));
    //--------------------------------------------------------------------------------------------------------

//    display(*H,6,6);
//    display(*H_TRA,6,6);
//    display(*BUF1,6,6);
//    display(*BUF2,6,6);
//    display(*BUF3,6,6);
//    display(*BUF4,6,6);
//    display(*BUF5,6,6);
//    display(*K,6,6);


    if(ADS_Mode(EKF_GYRO_COV)){
//	if(ADS_MODE(EKF_GYRO)){
		//-------------------------------誤差共分散行列を更新 P_ref_renew = (I - KH)P_ref -----------------------------
		// KH
		multiplication(*K, sizeof(K) / sizeof(K[0]), sizeof(K[0]) / sizeof(K[0][0]), *H, sizeof(H) / sizeof(H[0]), sizeof(H[0]) / sizeof(H[0][0]), *BUF6, sizeof(BUF6) / sizeof(BUF6[0]), sizeof(BUF6[0]) / sizeof(BUF6[0][0]));
		// KHP_ref
		multiplication(*BUF6, sizeof(BUF6) / sizeof(BUF6[0]), sizeof(BUF6[0]) / sizeof(BUF6[0][0]), *in_P_REF, 6, 6, *BUF7, sizeof(BUF7) / sizeof(BUF7[0]), sizeof(BUF7[0]) / sizeof(BUF7[0][0]));
		// P_ref_renew = P_ref - KHP_ref
		sub(*in_P_REF, 6, 6, *BUF7, sizeof(BUF7) / sizeof(BUF7[0]), sizeof(BUF7[0]) / sizeof(BUF7[0][0]), *out_P_REF_RENEW);
		//------------------------------------------------------------------------------------------------------------
    }
    else{
//    	copy(*in_P_REF,6,6,*out_P_REF_RENEW);
		for (int i = 0; i < 6; i++)	{
			for (int j = 0; j < 6; j++) {
				P_REF_RENEW[i][j] = (i == j) ? const_P_diag[j] : 0.0f;
			}
		}
    }
    // 修正を行った推定値
	// Dx_renew = Ky
	multiplication(*K, sizeof(K) / sizeof(K[0]), sizeof(K[0]) / sizeof(K[0][0]), y, 6, 1, Dx_renew, sizeof(Dx_renew) / sizeof(Dx_renew[0]), sizeof(Dx_renew[0]) / sizeof(Dx_renew[0]));
//	printf("Dx_renew %f %f %f %f %f %f ", Dx_renew[0], Dx_renew[1], Dx_renew[2], Dx_renew[3], Dx_renew[4], Dx_renew[5]);

    for (int i = 0; i < 3; i++){
    	Dqv[i] = Dx_renew[i];
    }

    Dq[0] = Dqv[0];
    Dq[1] = Dqv[1];
    Dq[2] = Dqv[2];
    Dq[3] = sqrtf(1 - (Dqv[0] * Dqv[0] + Dqv[1] * Dqv[1] + Dqv[2] * Dqv[2]));    //ノルムが1となるようにdq4を決定

    for (int i = 0; i < 4; i++){
    	Quaternion_ref[i] = in_X_ref[i];
    }

    Quaternion_Product(Quaternion_ref, Dq, Dq_ref);

    for (int i = 0; i < 3; i++){
    	Domega[i] = Dx_renew[i + 3];
    }

    for (int i = 0; i < 3; i++){
    	Omega_ref[i] = in_X_ref[i + 4];
    }

    for (int i = 0; i < 3; i++){
    	Domega_ref[i] = Omega_ref[i] + Domega[i];
    }

    out_X_ref_renew[0] = Dq_ref[0];
    out_X_ref_renew[1] = Dq_ref[1];
    out_X_ref_renew[2] = Dq_ref[2];
    out_X_ref_renew[3] = Dq_ref[3];
    out_X_ref_renew[4] = Domega_ref[0];
    out_X_ref_renew[5] = Domega_ref[1];
    out_X_ref_renew[6] = Domega_ref[2];

}



//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

//入力には接頭in_ 出力はout_を付ける
void AD_EKF_CLASS::cal(const float in_Quaternion[4], const float in_Omega[3])
{
	int itr = floorf(1/dt);

//	printf("entered_in_cal ");
//	printf("X_ref %f %f %f %f %f %f %f ",X_ref[0],X_ref[1],X_ref[2],X_ref[3],X_ref[4],X_ref[5],X_ref[6]);


//	printf("itr %d ", itr);
	for(int i=0; i<itr; i++){
		Predict(X_ref, P_REF, X_ref_next, P_REF_NEXT);

		copy(X_ref_next, 7, 1, X_ref);
		copy(*P_REF_NEXT, 6, 6, *P_REF);

//		printf("Observed_Value %f %f %f %f %f %f %f ", Observed_Value[0], Observed_Value[1], Observed_Value[2], Observed_Value[3], Observed_Value[4], Observed_Value[5], Observed_Value[6]);
//		print(EKF,"y %f %f %f %f %f %f ", y[0], y[1], y[2], y[3], y[4], y[5]);
		print(EKF,"Obs %f %f %f %f %f %f %f ", Observed_Value[0], Observed_Value[1], Observed_Value[2], Observed_Value[3], Observed_Value[4], Observed_Value[5], Observed_Value[6]);
		print(EKF,"X_ref %f %f %f %f %f %f %f \r\n", X_ref[0], X_ref[1], X_ref[2], X_ref[3], X_ref[4], X_ref[5], X_ref[6]);
//		print(EKF,"P_REF %f %f %f %f %f %f\r\n", P_REF[0][0], P_REF[1][1], P_REF[2][2], P_REF[3][3], P_REF[4][4], P_REF[5][5]);

	}

//	printf("After_predict ");
//	printf("X_ref %f %f %f %f %f %f %f ",X_ref[0],X_ref[1],X_ref[2],X_ref[3],X_ref[4],X_ref[5],X_ref[6]);

	float Observed_Value_buf[7] = { in_Quaternion[0], in_Quaternion[1], in_Quaternion[2], in_Quaternion[3], in_Omega[0], in_Omega[1], in_Omega[2]};
	copy(Observed_Value_buf, 7, 1, Observed_Value);

	Observe(Observed_Value, X_ref, P_REF, X_ref_renew, P_REF_RENEW);

//	printf("After_observe ");
//	printf("X_ref %f %f %f %f %f %f %f ",X_ref[0],X_ref[1],X_ref[2],X_ref[3],X_ref[4],X_ref[5],X_ref[6]);

	copy(X_ref_renew, 7, 1, X_ref);
	copy(*P_REF_RENEW, 6, 6, *P_REF);

//	printf("Observed_Value %f %f %f %f %f %f %f ", Observed_Value[0], Observed_Value[1], Observed_Value[2], Observed_Value[3], Observed_Value[4], Observed_Value[5], Observed_Value[6]);
//	printf("X_ref %f %f %f %f %f %f %f ", X_ref[0], X_ref[1], X_ref[2], X_ref[3], X_ref[4], X_ref[5], X_ref[6]);
//	printf("P_REF %f %f %f %f %f %f\r\n", P_REF[0][0], P_REF[1][1], P_REF[2][2], P_REF[3][3], P_REF[4][4], P_REF[5][5]);

}

//void AD_EKF_CLASS::reset(void)
void AD_EKF_CLASS::reset(const float in_Quaternion[4], const float in_Omega[3])
{
	float X_ref_buf[7] = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};  //初期リファレンスクォータニオン+角速度

	float P_REF_BUF[6][6];
	Identity(6, 6, *P_REF_BUF);

//	float coeff = 0.0001;
//	CoeffMat(*P_REF_BUF, 6, 6, coeff, *P_REF);

	copy(X_ref_buf, 7, 1, X_ref);
	copy(*P_REF_BUF, 6, 6, *P_REF);

}


