/*
 * EKF.cpp
 *
 *  ver.1.0 2022/07/02 tamura
 *
 */

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Library/General/DBG_printf.h>

#include <src_user/Applications/UserDefined/AOCS/Global_AOCS_Parameter.h>
#include <src_user/Drivers/Aocs/EKF.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/CrossProduct.h>

#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.h>
#include <src_user/Applications/DriverInstances/block_command_handler.h>
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include <src_core/TlmCmd/telemetry_frame.h>
#include <src_core/System/TimeManager/obc_time.h>

//======MRAM格納パラメータの実体宣言部======//
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS BEGIN *** //

constexpr float EKF_CLASS::ini_X_ref_0_[16];
constexpr float EKF_CLASS::ini_X_ref_1_[16];
constexpr float EKF_CLASS::ini_P_diag_0_[15];
constexpr float EKF_CLASS::ini_P_diag_1_[15];

constexpr float EKF_CLASS::std_Q_0_[15];
constexpr float EKF_CLASS::std_Q_1_[15];
constexpr float EKF_CLASS::std_R_0_[15];
constexpr float EKF_CLASS::std_R_1_[15];

constexpr uint8_t EKF_CLASS::adp_noise_flag_0_;
constexpr uint8_t EKF_CLASS::adp_noise_flag_1_;
constexpr float EKF_CLASS::alpha_0_;
constexpr float EKF_CLASS::alpha_1_;

constexpr float EKF_CLASS::const_K_diag_0_[15];
constexpr float EKF_CLASS::const_K_diag_1_[15];
constexpr uint8_t EKF_CLASS::P_constflag_0_;
constexpr uint8_t EKF_CLASS::P_constflag_1_;
constexpr float EKF_CLASS::const_P_diag_0_[15];
constexpr float EKF_CLASS::const_P_diag_1_[15];
constexpr float EKF_CLASS::conv_thr_0_;
constexpr float EKF_CLASS::conv_thr_1_;

constexpr uint16_t EKF_CLASS::max_prop_;
constexpr uint16_t EKF_CLASS::observe_time_;
constexpr uint16_t EKF_CLASS::dt_0_;
constexpr uint16_t EKF_CLASS::dt_1_;
constexpr uint16_t EKF_CLASS::max_prop_hispin_;
constexpr uint16_t EKF_CLASS::observe_time_hispin_;
constexpr uint16_t EKF_CLASS::dt_hispin_0_;
constexpr uint16_t EKF_CLASS::dt_hispin_1_;
constexpr int16_t EKF_CLASS::sep_time_under_limit_;

constexpr uint8_t EKF_CLASS::K_type_;
constexpr float EKF_CLASS::taub_;
constexpr float EKF_CLASS::taurmm_;
constexpr float EKF_CLASS::rmm_thr_;

constexpr float EKF_CLASS::omega_abs_thr_;
constexpr float EKF_CLASS::direction_err_b_;
constexpr float EKF_CLASS::err_cov_thr_[6];
constexpr float EKF_CLASS::direction_err_eci_;
constexpr float EKF_CLASS::sep_direction_err_;
constexpr float EKF_CLASS::separate_direction_[3];
constexpr float EKF_CLASS::pos_1U_b_[3];
constexpr float EKF_CLASS::omega_X_min_;
constexpr float EKF_CLASS::omega_X_max_;
constexpr float EKF_CLASS::omega_Y_min_;
constexpr float EKF_CLASS::omega_Y_max_;
constexpr float EKF_CLASS::omega_Z_min_;
constexpr float EKF_CLASS::omega_Z_max_;
constexpr float EKF_CLASS::angle_velocity2omega_;
constexpr float EKF_CLASS::angle_velocity2omega_err_;
constexpr float EKF_CLASS::angle_normal2vXomega_;
constexpr float EKF_CLASS::angle_normal2vXomega_err_;
constexpr float EKF_CLASS::sep_phase_;
constexpr float EKF_CLASS::sep_phase_err_;
constexpr uint8_t EKF_CLASS::width_;
constexpr uint8_t EKF_CLASS::select_ekf_;

constexpr float EKF_CLASS::mtq_rms_coeff_x_;
constexpr float EKF_CLASS::mtq_rms_coeff_y_;
constexpr float EKF_CLASS::mtq_rms_coeff_z_;

constexpr uint8_t EKF_CLASS::reltime_type_;
constexpr int16_t EKF_CLASS::sensor_reltime_to_obs_[4];
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS END*** //

// Definition of static member variables
int16_t EKF_CLASS::sep_time;
int16_t EKF_CLASS::sep_time_under_limit;

uint8_t EKF_CLASS::K_type;
float EKF_CLASS::taub;
float EKF_CLASS::rmm_thr;
float EKF_CLASS::taurmm;

float EKF_CLASS::omega_abs_thr;
float EKF_CLASS::direction_err_b;
float EKF_CLASS::err_cov_thr[6];
float EKF_CLASS::direction_err_eci;
float EKF_CLASS::sep_direction_err;
float EKF_CLASS::separate_direction[3];
float EKF_CLASS::pos_1U_b[3];
float EKF_CLASS::omega_X_min;
float EKF_CLASS::omega_X_max;
float EKF_CLASS::omega_Y_min;
float EKF_CLASS::omega_Y_max;
float EKF_CLASS::omega_Z_min;
float EKF_CLASS::omega_Z_max;
float EKF_CLASS::angle_velocity2omega;
float EKF_CLASS::angle_velocity2omega_err;
float EKF_CLASS::angle_normal2vXomega;
float EKF_CLASS::angle_normal2vXomega_err;
float EKF_CLASS::sep_phase;
float EKF_CLASS::sep_phase_err;
SEP_CONDITION_TLM EKF_CLASS::sep_condition_tlm;
uint8_t EKF_CLASS::width;
uint8_t EKF_CLASS::select_ekf;

float EKF_CLASS::mtq_rms_coeff_x;
float EKF_CLASS::mtq_rms_coeff_y;
float EKF_CLASS::mtq_rms_coeff_z;

uint16_t EKF_CLASS::max_prop;
uint16_t EKF_CLASS::observe_time;
uint16_t EKF_CLASS::max_prop_hispin;
uint16_t EKF_CLASS::observe_time_hispin;

uint8_t EKF_CLASS::reltime_type;
int16_t EKF_CLASS::sensor_reltime_to_obs[4];
//---

static CROSS_PRODUCT_CLASS Cross_class;

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
 * 2次はいらない可能性。nano-JASMINEではF_k　Δtの1次項まで
 *  F_k  = E + A_k*Δt + 1/2*A_k^2*Δt^2
 *  Gw_K = (E*Δt +1/2*A_k*Δt^2 + 1/6*A_k*Δt^3) * B_k
 */

/** Predict (Estimate)
 *  Predicted state estimate:             xref = f(xref,u)        xref_k\k-1 = f(xref_k-1\k-1, u_k)
 *  Predicted covariance estimate:        P    = FPF' + GwQGw'    P_k\k-1    = F_k * P_k-1\k-1 * F_k' + Gw_K * Q_k * Gw_K'
 */

/** Update (Observe)
 *  Innovation or measurement residual:   y = Z - h(xref)         y_k   = Z_k - h(xref_k\k-1)
 *  Innovation (or residual) covariance:  S = HPH' + R            S_k   = H_k * P_k\k-1 * H_k' + R_k
 *  Near-optimal Kalman gain:             K = P*H'S^-1            K_k   = P_k\k-1 * H_k' * S_k^-1
 *  Updated state estimate:               x = x + Ky              x_k\k = x_k\k-1 + K*y_k
 *  Updated covariance estimate:          P = (I - KH)P           P_k\k = (I - K_k * H_k)*P_k\k-1
 */



EKF_CLASS::EKF_CLASS() :
		ekf_valid(), dq_valid(), divg_valid()
{
}

EKF_CLASS::~EKF_CLASS(){

}

/**
 * @brief Calculate A
 * システム行列
 */
void EKF_CLASS::Calc_A(const ADS_MODE ekf_id, float* A) {


	if(ekf_id == EKF_GYRO){

		// X_ref[7] = Quaternion[4], Omega[3]
		// Obs[7]   = Quaternion[4], Omega[3]
		float TMP2[3][3], TMP3[3][3], TMP4[3][3], TMP6[3][3];
		float Omega_ref[3];

		 if(ADS_Mode(EKF_GYROBIAS)){
			for (int i = 0; i < 3; i++){
			    // 観測更新したX_refからジャイロバイアスを引く
				Omega_ref[i] = X_ref[i+4] - Obs[i+7];
			}
//			printf("GBias %f %f %f\r\n", Obs[7], Obs[8], Obs[9]);
		 }
		 else{
			for (int i = 0; i < 3; i++){
				Omega_ref[i] = X_ref[i+4];
			}
		 }

		OmegaCross4Kalman(Omega_ref, M);     //M
		CrossOmega4Kalman(Omega_ref, mM);    //minusM

		// L = Iω
		multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0][0]), Omega_ref, sizeof(Omega_ref) / sizeof(Omega_ref[0]), sizeof(Omega_ref[0]) / sizeof(Omega_ref[0]), L, sizeof(L) / sizeof(L[0]), sizeof(L[0]) / sizeof(L[0]));

		// [L×]
		TMP2[0][0] = 0.0f;      TMP2[0][1] = -L[2];       TMP2[0][2] = L[1];
		TMP2[1][0] = L[2];     TMP2[1][1] = 0.0f;       TMP2[1][2] = -L[0];
		TMP2[2][0] = -L[1];      TMP2[2][1] = L[0];      TMP2[2][2] = 0.0f;

		// [ω×]I
		multiplication(*M, sizeof(M) / sizeof(M[0]), sizeof(M[0]) / sizeof(M[0][0]), *g_acs.p_GLOBAL_AOCS->Inertia_tensor, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0][0]), *TMP3, sizeof(TMP3) / sizeof(TMP3[0]), sizeof(TMP3[0]) / sizeof(TMP3[0][0]));

		// TMP4 = [L×] - [ω×]I
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				TMP4[i][j] = TMP2[i][j] - TMP3[i][j];
			}
		}

		// TMP6 = I^-1*([L×] - [ω×]I)
		multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][0]), *TMP4, sizeof(TMP4) / sizeof(TMP4[0]), sizeof(TMP4[0]) / sizeof(TMP4[0][0]), *TMP6, sizeof(TMP6) / sizeof(TMP6[0]), sizeof(TMP6[0]) / sizeof(TMP6[0][0]));

		A[0*Pdim+0] = mM[0][0];  A[0*Pdim+1] = mM[0][1];  A[0*Pdim+2] = mM[0][2];       A[0*Pdim+3] = 0.5f;  A[0*Pdim+4] = 0.0f;  A[0*Pdim+5] = 0.0f;
		A[1*Pdim+0] = mM[1][0];  A[1*Pdim+1] = mM[1][1];  A[1*Pdim+2] = mM[1][2];       A[1*Pdim+3] = 0.0f;  A[1*Pdim+4] = 0.5f;  A[1*Pdim+5] = 0.0f;
		A[2*Pdim+0] = mM[2][0];  A[2*Pdim+1] = mM[2][1];  A[2*Pdim+2] = mM[2][2];       A[2*Pdim+3] = 0.0f;  A[2*Pdim+4] = 0.0f;  A[2*Pdim+5] = 0.5f;

		A[3*Pdim+0] = 0.0f;  A[3*Pdim+1] = 0.0f;  A[3*Pdim+2] = 0.0f;         A[3*Pdim+3] = TMP6[0][0];  A[3*Pdim+4] = TMP6[0][1];  A[3*Pdim+5] = TMP6[0][2];
		A[4*Pdim+0] = 0.0f;  A[4*Pdim+1] = 0.0f;  A[4*Pdim+2] = 0.0f;         A[4*Pdim+3] = TMP6[1][0];  A[4*Pdim+4] = TMP6[1][1];  A[4*Pdim+5] = TMP6[1][2];
		A[5*Pdim+0] = 0.0f;  A[5*Pdim+1] = 0.0f;  A[5*Pdim+2] = 0.0f;         A[5*Pdim+3] = TMP6[2][0];  A[5*Pdim+4] = TMP6[2][1];  A[5*Pdim+5] = TMP6[2][2];


	}

	else if(ekf_id==EKF_GYROBIAS){

		// X_ref[7] =  Quaternion[4], GyroBias[3]
		// Obs[7]   =  Quaternion[4], Omega[3]

		// ジャイロセンサバイアス推定において、ジャイロセンサ出力は状態方程式に毎周期いれる
		float Omega_ref[3];
		float omega[3] = {Obs[4], Obs[5], Obs[6]};
		float gbias[3] = {X_ref[4], X_ref[5], X_ref[6]};

		for (int i = 0; i < 3; i++){
			Omega_ref[i] = omega[i] - gbias[i];
		}

		CrossOmega4Kalman(Omega_ref, mM);    //minusM

		A[0*Pdim+0] = mM[0][0];  A[0*Pdim+1] = mM[0][1];  A[0*Pdim+2] = mM[0][2];        A[0*Pdim+3] = -0.5f;   A[0*Pdim+4] = 0.0f;   A[0*Pdim+5] = 0.0f;
		A[1*Pdim+0] = mM[1][0];  A[1*Pdim+1] = mM[1][1];  A[1*Pdim+2] = mM[1][2];        A[1*Pdim+3] = 0.0f;    A[1*Pdim+4] = -0.5f;  A[1*Pdim+5] = 0.0f;
		A[2*Pdim+0] = mM[2][0];  A[2*Pdim+1] = mM[2][1];  A[2*Pdim+2] = mM[2][2];        A[2*Pdim+3] = 0.0f;    A[2*Pdim+4] = 0.0f;   A[2*Pdim+5] = -0.5f;

		A[3*Pdim+0] = 0.0f;  A[3*Pdim+1] = 0.0f;  A[3*Pdim+2] = 0.0f;         A[3*Pdim+3] = -1/taub;   A[3*Pdim+4] = 0.0f;      A[3*Pdim+5] = 0.0f;
		A[4*Pdim+0] = 0.0f;  A[4*Pdim+1] = 0.0f;  A[4*Pdim+2] = 0.0f;         A[4*Pdim+3] = 0.0f;      A[4*Pdim+4] = -1/taub;   A[4*Pdim+5] = 0.0f;
		A[5*Pdim+0] = 0.0f;  A[5*Pdim+1] = 0.0f;  A[5*Pdim+2] = 0.0f;         A[5*Pdim+3] = 0.0f;      A[5*Pdim+4] = 0.0f;      A[5*Pdim+5] = -1/taub;
	}

	else if(ekf_id==EKF_MAGSUNGYRO){

		// X_ref[7] = Quaternion[4], Omega[3]
		// Obs[7]   = Quaternion[4], Omega[3]
		float TMP2[3][3], TMP3[3][3], TMP4[3][3], TMP6[3][3];
		float Omega_ref[3];

		 if(ADS_Mode(EKF_GYROBIAS)){
			for (int i = 0; i < 3; i++){
				Omega_ref[i] = X_ref[i+4] - Obs[i+13];
			}
		 }
		 else{
			for (int i = 0; i < 3; i++){
				Omega_ref[i] = X_ref[i+4];
			}
		 }


		OmegaCross4Kalman(Omega_ref, M);     //M
		CrossOmega4Kalman(Omega_ref, mM);    //minusM

		// L = Iω
		multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0][0]), Omega_ref, sizeof(Omega_ref) / sizeof(Omega_ref[0]), sizeof(Omega_ref[0]) / sizeof(Omega_ref[0]), L, sizeof(L) / sizeof(L[0]), sizeof(L[0]) / sizeof(L[0]));

		// [L×]
		TMP2[0][0] = 0.0f;      TMP2[0][1] = -L[2];       TMP2[0][2] = L[1];
		TMP2[1][0] = L[2];     TMP2[1][1] = 0.0f;       TMP2[1][2] = -L[0];
		TMP2[2][0] = -L[1];      TMP2[2][1] = L[0];      TMP2[2][2] = 0.0f;

		// [ω×]I
		multiplication(*M, sizeof(M) / sizeof(M[0]), sizeof(M[0]) / sizeof(M[0][0]), *g_acs.p_GLOBAL_AOCS->Inertia_tensor, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0][0]), *TMP3, sizeof(TMP3) / sizeof(TMP3[0]), sizeof(TMP3[0]) / sizeof(TMP3[0][0]));

		// TMP4 = [L×] - [ω×]I
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				TMP4[i][j] = TMP2[i][j] - TMP3[i][j];
			}
		}

		// TMP6 = I^-1*([L×] - [ω×]I)
		multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][0]), *TMP4, sizeof(TMP4) / sizeof(TMP4[0]), sizeof(TMP4[0]) / sizeof(TMP4[0][0]), *TMP6, sizeof(TMP6) / sizeof(TMP6[0]), sizeof(TMP6[0]) / sizeof(TMP6[0][0]));

		A[0*Pdim+0] = mM[0][0];  A[0*Pdim+1] = mM[0][1];  A[0*Pdim+2] = mM[0][2];       A[0*Pdim+3] = 0.5f;  A[0*Pdim+4] = 0.0f;  A[0*Pdim+5] = 0.0f;
		A[1*Pdim+0] = mM[1][0];  A[1*Pdim+1] = mM[1][1];  A[1*Pdim+2] = mM[1][2];       A[1*Pdim+3] = 0.0f;  A[1*Pdim+4] = 0.5f;  A[1*Pdim+5] = 0.0f;
		A[2*Pdim+0] = mM[2][0];  A[2*Pdim+1] = mM[2][1];  A[2*Pdim+2] = mM[2][2];       A[2*Pdim+3] = 0.0f;  A[2*Pdim+4] = 0.0f;  A[2*Pdim+5] = 0.5f;

		A[3*Pdim+0] = 0.0f;  A[3*Pdim+1] = 0.0f;  A[3*Pdim+2] = 0.0f;         A[3*Pdim+3] = TMP6[0][0];  A[3*Pdim+4] = TMP6[0][1];  A[3*Pdim+5] = TMP6[0][2];
		A[4*Pdim+0] = 0.0f;  A[4*Pdim+1] = 0.0f;  A[4*Pdim+2] = 0.0f;         A[4*Pdim+3] = TMP6[1][0];  A[4*Pdim+4] = TMP6[1][1];  A[4*Pdim+5] = TMP6[1][2];
		A[5*Pdim+0] = 0.0f;  A[5*Pdim+1] = 0.0f;  A[5*Pdim+2] = 0.0f;         A[5*Pdim+3] = TMP6[2][0];  A[5*Pdim+4] = TMP6[2][1];  A[5*Pdim+5] = TMP6[2][2];


	}

	else if(ekf_id==EKF_EDDY){

	}

	else if(ekf_id==EKF_RESIDUAL_MAG){

		// X_ref[6] =  Omega[3], RMM[3]
		// Obs[6]   =  Omega[3], Mag[3]

		float TMP2[3][3], TMP3[3][3], TMP4[3][3], TMP6[3][3];
		float Omega_ref[3];

		for (int i = 0; i < 3; i++){
			Omega_ref[i] = X_ref[i];
		}

		OmegaCross4Kalman(Omega_ref, M);     //M
		CrossOmega4Kalman(Omega_ref, mM);    //minusM

		 //L = Iω
		multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0][0]), Omega_ref, sizeof(Omega_ref) / sizeof(Omega_ref[0]), sizeof(Omega_ref[0]) / sizeof(Omega_ref[0]), L, sizeof(L) / sizeof(L[0]), sizeof(L[0]) / sizeof(L[0]));

		// [L×]
		TMP2[0][0] = 0.0f;      TMP2[0][1] = -L[2];       TMP2[0][2] = L[1];
		TMP2[1][0] = L[2];     TMP2[1][1] = 0.0f;       TMP2[1][2] = -L[0];
		TMP2[2][0] = -L[1];      TMP2[2][1] = L[0];      TMP2[2][2] = 0.0f;

		// [ω×]I
		multiplication(*M, sizeof(M) / sizeof(M[0]), sizeof(M[0]) / sizeof(M[0][0]), *g_acs.p_GLOBAL_AOCS->Inertia_tensor, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor[0][0]), *TMP3, sizeof(TMP3) / sizeof(TMP3[0]), sizeof(TMP3[0]) / sizeof(TMP3[0][0]));

		// TMP4 = [L×] - [ω×]I
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				TMP4[i][j] = TMP2[i][j] - TMP3[i][j];
			}
		}

		// TMP6 = I^-1*([L×] - [ω×]I)
		multiplication(*g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv, sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0]), sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0]) / sizeof(g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][0]), *TMP4, sizeof(TMP4) / sizeof(TMP4[0]), sizeof(TMP4[0]) / sizeof(TMP4[0][0]), *TMP6, sizeof(TMP6) / sizeof(TMP6[0]), sizeof(TMP6[0]) / sizeof(TMP6[0][0]));

		A[0*Pdim+0] = TMP6[0][0];  A[0*Pdim+1] = TMP6[0][1];  A[0*Pdim+2] = TMP6[0][2];
		A[1*Pdim+0] = TMP6[1][0];  A[1*Pdim+1] = TMP6[1][1];  A[1*Pdim+2] = TMP6[1][2];
		A[2*Pdim+0] = TMP6[2][0];  A[2*Pdim+1] = TMP6[2][1];  A[2*Pdim+2] = TMP6[2][2];

		float Mag_ref[3];
		for (int i = 0; i < 3; i++){
			Mag_ref[i] = Obs[i+3];
			// Mag_ref[i] = Obs[i+3] - MBias[i];
		}
		// [B×]
		OmegaCross4Kalman(Mag_ref,TMP2);
		// -I^-1*[B×]
		CoeffMat(*g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv,3,3,-1.0f,*TMP3);
		multiplication(*TMP3,3,3,*TMP2,3,3,*TMP4,3,3);

		for (int i = 0; i < 3; i++){
			for (int j = 3; j < 6; j++){
				A[i*Pdim+j] = TMP4[i][j];
			}
		}

		for(int i = 3; i < 6; i++){
			for(int j = 0; j < 3; j++){
				A[i*Pdim+j] = 0.0f;
			}
		}

		for(int i = 3; i < 6; i++){
			for(int j = 3; j < 6; j++){
				A[i*Pdim+j] = i==j ? -1.0f/taurmm : 0.0f;
			}
		}
	}

	else if(ekf_id==EKF_ANISOTROPY_MAG){

	}
}


/**
 * @brief Calculate B
 * ノイズを制御入力とする状態方程式の行列
 */
void EKF_CLASS::Calc_B(const ADS_MODE ekf_id, float* B){

	if(ekf_id==EKF_GYRO){
		// X_ref[7] =  Quaternion[4], Omega[3]

		B[0*Qdim+0] = 0.0f;        B[0*Qdim+1] = 0.0f;        B[0*Qdim+2] = 0.0f;         B[0*Qdim+3] = 0.0f;                                  B[0*Qdim+4] = 0.0f;                                  B[0*Qdim+5] = 0.0f;
		B[1*Qdim+0] = 0.0f;        B[1*Qdim+1] = 0.0f;        B[1*Qdim+2] = 0.0f;         B[1*Qdim+3] = 0.0f;                                  B[1*Qdim+4] = 0.0f;                                  B[1*Qdim+5] = 0.0f;
		B[2*Qdim+0] = 0.0f;        B[2*Qdim+1] = 0.0f;        B[2*Qdim+2] = 0.0f;         B[2*Qdim+3] = 0.0f;                                  B[2*Qdim+4] = 0.0f;                                  B[2*Qdim+5] = 0.0f;
		B[3*Qdim+0] = 0.0f;        B[3*Qdim+1] = 0.0f;        B[3*Qdim+2] = 0.0f;         B[3*Qdim+3] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][0];         B[3*Qdim+4] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][1];         B[3*Qdim+5] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][2];
		B[4*Qdim+0] = 0.0f;        B[4*Qdim+1] = 0.0f;        B[4*Qdim+2] = 0.0f;         B[4*Qdim+3] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][0];         B[4*Qdim+4] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][1];         B[4*Qdim+5] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][2];
		B[5*Qdim+0] = 0.0f;        B[5*Qdim+1] = 0.0f;        B[5*Qdim+2] = 0.0f;         B[5*Qdim+3] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][0];         B[5*Qdim+4] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][1];         B[5*Qdim+5] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][2];
	}

	else if(ekf_id==EKF_GYROBIAS){
		// X_ref[7] =  Quaternion[4], GyroBias[3]

		B[0*Qdim+0] = -0.5f;       B[0*Qdim+1] = 0.0f;        B[0*Qdim+2] = 0.0f;         B[0*Qdim+3] = 0.0f;         B[0*Qdim+4] = 0.0f;         B[0*Qdim+5] = 0.0f;
		B[1*Qdim+0] = 0.0f;        B[1*Qdim+1] = -0.5f;       B[1*Qdim+2] = 0.0f;         B[1*Qdim+3] = 0.0f;         B[1*Qdim+4] = 0.0f;         B[1*Qdim+5] = 0.0f;
		B[2*Qdim+0] = 0.0f;        B[2*Qdim+1] = 0.0f;        B[2*Qdim+2] = -0.5f;        B[2*Qdim+3] = 0.0f;         B[2*Qdim+4] = 0.0f;         B[2*Qdim+5] = 0.0f;

		B[3*Qdim+0] = 0.0f;        B[3*Qdim+1] = 0.0f;        B[3*Qdim+2] = 0.0f;         B[3*Qdim+3] = 1.0f;         B[3*Qdim+4] = 0.0f;         B[3*Qdim+5] = 0.0f;
		B[4*Qdim+0] = 0.0f;        B[4*Qdim+1] = 0.0f;        B[4*Qdim+2] = 0.0f;         B[4*Qdim+3] = 0.0f;         B[4*Qdim+4] = 1.0f;         B[4*Qdim+5] = 0.0f;
		B[5*Qdim+0] = 0.0f;        B[5*Qdim+1] = 0.0f;        B[5*Qdim+2] = 0.0f;         B[5*Qdim+3] = 0.0f;         B[5*Qdim+4] = 0.0f;         B[5*Qdim+5] = 1.0f;
	}

	else if(ekf_id==EKF_MAGSUNGYRO){
		// X_ref[7] =  Quaternion[4], Omega[3]

		B[0*Qdim+0] = 0.0f;        B[0*Qdim+1] = 0.0f;        B[0*Qdim+2] = 0.0f;         B[0*Qdim+3] = 0.0f;                                  B[0*Qdim+4] = 0.0f;                                  B[0*Qdim+5] = 0.0f;
		B[1*Qdim+0] = 0.0f;        B[1*Qdim+1] = 0.0f;        B[1*Qdim+2] = 0.0f;         B[1*Qdim+3] = 0.0f;                                  B[1*Qdim+4] = 0.0f;                                  B[1*Qdim+5] = 0.0f;
		B[2*Qdim+0] = 0.0f;        B[2*Qdim+1] = 0.0f;        B[2*Qdim+2] = 0.0f;         B[2*Qdim+3] = 0.0f;                                  B[2*Qdim+4] = 0.0f;                                  B[2*Qdim+5] = 0.0f;
		B[3*Qdim+0] = 0.0f;        B[3*Qdim+1] = 0.0f;        B[3*Qdim+2] = 0.0f;         B[3*Qdim+3] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][0];         B[3*Qdim+4] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][1];         B[3*Qdim+5] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][2];
		B[4*Qdim+0] = 0.0f;        B[4*Qdim+1] = 0.0f;        B[4*Qdim+2] = 0.0f;         B[4*Qdim+3] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][0];         B[4*Qdim+4] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][1];         B[4*Qdim+5] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][2];
		B[5*Qdim+0] = 0.0f;        B[5*Qdim+1] = 0.0f;        B[5*Qdim+2] = 0.0f;         B[5*Qdim+3] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][0];         B[5*Qdim+4] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][1];         B[5*Qdim+5] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][2];
	}

	else if(ekf_id==EKF_EDDY){

	}

	else if(ekf_id==EKF_RESIDUAL_MAG){
		// X_ref[1] = Omega[3], ResMagMmt[3]

		B[0*Qdim+0] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][0];         B[0*Qdim+1] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][1];         B[0*Qdim+2] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[0][2];
		B[1*Qdim+0] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][0];         B[1*Qdim+1] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][1];         B[1*Qdim+2] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[1][2];
		B[2*Qdim+0] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][0];         B[2*Qdim+1] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][1];         B[2*Qdim+2] = g_acs.p_GLOBAL_AOCS->Inertia_tensor_inv[2][2];

		B[0*Qdim+3] = 0.0f;    B[0*Qdim+4] = 0.0f;    B[0*Qdim+5] = 0.0f;
		B[1*Qdim+3] = 0.0f;    B[1*Qdim+4] = 0.0f;    B[1*Qdim+5] = 0.0f;
		B[2*Qdim+3] = 0.0f;    B[2*Qdim+4] = 0.0f;    B[2*Qdim+5] = 0.0f;

		B[3*Qdim+0] = 0.0f;    B[3*Qdim+1] = 0.0f;    B[3*Qdim+2] = 0.0f;       B[3*Qdim+3] = 1.0f;    B[3*Qdim+4] = 0.0f;    B[3*Qdim+5] = 0.0f;
		B[4*Qdim+0] = 0.0f;    B[4*Qdim+1] = 0.0f;    B[4*Qdim+2] = 0.0f;       B[4*Qdim+3] = 0.0f;    B[4*Qdim+4] = 1.0f;    B[4*Qdim+5] = 0.0f;
		B[5*Qdim+0] = 0.0f;    B[5*Qdim+1] = 0.0f;    B[5*Qdim+2] = 0.0f;       B[5*Qdim+3] = 0.0f;    B[5*Qdim+4] = 0.0f;    B[5*Qdim+5] = 1.0f;


	}

	else if(ekf_id==EKF_ANISOTROPY_MAG){

	}


}


/*
 * @brief Calculate F
 * 離散状態遷移行列の計算
 * 行列指数関数を1次近似
 */
void EKF_CLASS::Calc_F(float* A, float* F){

	// E + AΔt
	for (int i = 0; i < Pdim; i++)	{
		for (int j = 0; j < Pdim; j++) {
			F[i*Pdim+j] = (i == j) ? 1.0f + A[i*Pdim+j]*dt_sec : A[i*Pdim+j]*dt_sec;
		}
	}
}


/*
 * @brief Calculate Gw
 * 離散状態遷移行列の計算
 * 行列指数関数を0次近似
 */
void EKF_CLASS::Calc_Gw(float* B, float* Gw){
	// BΔt
	for (int i = 0; i < Pdim; i++)	{
		for (int j = 0; j < Qdim; j++) {
			Gw[i*Qdim+j] = B[i*Qdim+j] * dt_sec;
		}
	}
}

/**
 * @brief Calculate Q
 * システムノイズ行列の計算
 */
void EKF_CLASS::Calc_Q(){

	for (int i = 0; i < Qdim; i++)	{
		for (int j = 0; j < Qdim; j++) {
			Q[i*Qdim+j] = i==j ? std_Q[i]*std_Q[i] : 0.0f ;
		}
	}
}

/**
 * @brief  カルマンフィルタにおける時間更新
 * @param  IN_x_ref_renew クォータニオン+角速度　事後推定値
 * @param  IN_P_ref_renew　事後誤差共分散行列
 * @param  OUT_x_ref_next クォータニオン+角速度　推定値
 * @param  OUT_P_ref_next 誤差共分散行列　推定値
 * @return  なし
 */

void EKF_CLASS::Predict(const ADS_MODE ekf_id)
{

	float A[Pdim*Pdim] = {};  //非線形状態方程式のヤコビアン　微小状態量システム行列
	float B[Pdim*Qdim] = {};  //微小状態量ノイズ行列
	float F[Pdim*Pdim] = {};  //状態遷移行列
	float Gw[Pdim*Qdim] = {}; //状態遷移行列

	float F_tra[Pdim*Pdim], Gw_TRA[Qdim*Pdim];
	float MagMmt[3] = {};
	float T[3] = {}; // Torque

	// 各種類の共分散フラグ（ENUMのekf_id+1）がONであれば共分散行列を計算
    if(ADS_Mode(ADS_MODE(ekf_id+1))){
    	if(ekf_id == EKF_GYRO || ekf_id == EKF_MAGSUNGYRO){
    		print(EKF,"CAL_COVARIANCE ");
    	}

		//---------------------状態方程式を線形化--------------------------------------------------
		Calc_A(ekf_id, A);
		Calc_B(ekf_id, B);
		//----------------------------------------------------------------------------------------------------------

		//------------------------------------------------離散化した状態方程式の係数行列F,Gwを求める(discretization)---------------------------------------------
		float BUF9[15*15], BUF10[15*15], BUF11[15*15], BUF12[15*15];

		//----------FPF'を計算-------------------------
		// F
		Calc_F(A, F);
		// F'
		trans(F, Pdim, Pdim, F_tra, Pdim, Pdim);
		// FP
		multiplication(F, Pdim, Pdim, P, Pdim, Pdim, BUF9, Pdim, Pdim);
		// FPF'
		multiplication(BUF9, Pdim, Pdim, F_tra, Pdim, Pdim, BUF10, Pdim, Pdim);
		//----------------------------------------------


		//----------GwQGw'を計算-----------------------
		// Gw
		Calc_Gw(B, Gw);
		// Gw'
		trans(Gw, Pdim, Qdim, Gw_TRA, Qdim, Pdim);
		// GwQ
		multiplication(Gw, Pdim, Qdim, Q, Qdim, Qdim, BUF11, Pdim, Qdim);
		// GwQGW'
		multiplication(BUF11, Pdim, Qdim, Gw_TRA, Qdim, Pdim, BUF12, Pdim, Pdim);
		//----------------------------------------------

		//-------------事後誤差共分散行列を更新-----------------------------------
		// P_next = F*P*F' + GW*Q*GW'
		add(BUF10, Pdim, Pdim, BUF12, Pdim, Pdim, P);
		//--------------------------------------------------------------------------
	}

    // カルマンゲインではなく、共分散行列一定とするフラグON時
	if(P_constflag == 1){
		copy(P, Pdim, Pdim, P);
	}

//	display(*BUF10,15,15);
//	display(*BUF12,15,15);
//	printf("A\r\n");
//	display(A,Pdim,Pdim);
//	printf("B\r\n");
//	display(B,Pdim,Qdim);
//	printf("P\r\n");
//  display_exp(P,Pdim,Pdim);


	//---------------------リファレンス状態量の更新--------------------------------------------------

	if(ekf_id == EKF_GYRO || ekf_id == EKF_MAGSUNGYRO){

#ifdef HILS
		MagMmt[0] = g_acs.p_GLOBAL_AOCS->Mag_moment[0];
		MagMmt[1] = g_acs.p_GLOBAL_AOCS->Mag_moment[1];
		MagMmt[2] = g_acs.p_GLOBAL_AOCS->Mag_moment[2];
#endif
#ifndef HILS
		// 磁気センサ取得時のMTQのDuty比を考慮した正味出力に合う実験的に得た係数を掛ける
		MagMmt[0] = mtq_rms_coeff_x * g_acs.p_GLOBAL_AOCS->Mag_moment[0];
		MagMmt[1] = mtq_rms_coeff_y * g_acs.p_GLOBAL_AOCS->Mag_moment[1];
		MagMmt[2] = mtq_rms_coeff_z * g_acs.p_GLOBAL_AOCS->Mag_moment[2];
#endif
		Outer_product(MagMmt, g_acs.p_GLOBAL_AOCS->MagField, T);
		RungeOneStep(dt_sec, X_ref, T, X_ref);
	}

	else if(ekf_id == EKF_GYROBIAS){

		float omega[3] = {Obs[4], Obs[5], Obs[6]}; //角速度が状態変数にない
		OneStepQuaGBias(dt_sec, X_ref, omega, taub, X_ref);

	}

	else if(ekf_id == EKF_RESIDUAL_MAG){

		float BFld[3] = {Obs[3], Obs[4], Obs[5]};
		OneStepOmeRmm(dt_sec, X_ref, BFld, taurmm, X_ref);

	}
	//　他の種類のEKFではルンゲクッタを回していない？

	//----------------------------------------------------------------------------------------------------------

}


/*
 * @brief Calculate y
 * イノベーションの計算
 * イノベーション　＝　観測値　ー　事前推定値
 */
void EKF_CLASS::Calc_y(const ADS_MODE ekf_id, float* y){
	//イノベーションの計算
	//観測量と事前推定量の差

	float q[4], qref[4], qref_conj[4], err_q[4];
	float ome[3], omeref[3], err_ome[3];

	if(ekf_id==EKF_GYRO){

		for(int i=0; i<4; i++){
			q[i] = Obs[i];
			qref[i] = X_ref_pre[i];
		}

		// 直列EKFでGYROBIASがONの時、推定されたジャイロバイアスを差し引く
		 if(ADS_Mode(EKF_GYROBIAS)){
			for (int i = 0; i < 3; i++){
				ome[i] = Obs[i+4];
				omeref[i] = X_ref_pre[i+4] - Obs[i+7];
			}
		 }
		 else{
			for (int i = 0; i < 3; i++){
				ome[i] = Obs[i+4];
				omeref[i] = X_ref_pre[i+4];
			}
		 }

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

	else if (ekf_id==EKF_GYROBIAS){

		for(int i=0; i<4; i++){
			q[i] = Obs[i];
			qref[i] = X_ref_pre[i];
		}

		//Quaternion error
		Quaternion_Conjugate(qref,qref_conj);
		Quaternion_Product(qref_conj,q,err_q);

		y[0] = err_q[0];
		y[1] = err_q[1];
		y[2] = err_q[2];

	}

	else if (ekf_id==EKF_MAGSUNGYRO){

		// 直列EKFでGYROBIASがONの時、推定されたジャイロバイアスを差し引く
		 if(ADS_Mode(EKF_GYROBIAS)){
			for (int i = 0; i < 3; i++){
				ome[i] = Obs[i+4];
				omeref[i] = X_ref_pre[i+4] - Obs[i+13];
			}
		 }
		 else{
			for (int i = 0; i < 3; i++){
				ome[i] = Obs[i+4];
				omeref[i] = X_ref_pre[i+4];
			}
		 }

		//Omega error
		sub(ome,3,1,omeref,3,1,err_ome);

		for(int i=0; i<4; i++){
			qref[i] = X_ref_pre[i];
		}
		float mag_i[3], mag_b[3], magsen_b[3], err_mag[3], sun_i[3], sun_b[3], sunsen_b[3], err_sun[3];
		float dcm[3][3];

		for(int i=0; i<3; i++){
			mag_i[i] = g_ads.p_IGRF->IGRF_B_Norm[i];
			magsen_b[i] = Obs[i+7];
			sun_i[i] = g_ads.p_SUNVECTOR->sunvector[i];
			sunsen_b[i] = Obs[i+10];
		}

		Quaternion_To_DCM(qref, *dcm);
		multiplication(*dcm,3,3,mag_i,3,1,mag_b,3,1);
		multiplication(*dcm,3,3,sun_i,3,1,sun_b,3,1);

		for(int i=0; i<3; i++){
			err_mag[i] = magsen_b[i] - mag_b[i];
			err_sun[i] = sunsen_b[i] - sun_b[i];
		}

		// Mag Vector
		y[0] = err_mag[0];
		y[1] = err_mag[1];
		y[2] = err_mag[2];

		// Sun Vector
		if(!vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag)){
			y[3] = err_sun[0];
			y[4] = err_sun[1];
			y[5] = err_sun[2];
		}

		// Omega
		y[6] = err_ome[0];
		y[7] = err_ome[1];
		y[8] = err_ome[2];


		printf("qref %f %f %f %f ", qref[0], qref[1], qref[2], qref[3]);
		printf("mag_i %f %f %f ", mag_i[0], mag_i[1], mag_i[2]);
		printf("mag_b %f %f %f ", mag_b[0], mag_b[1], mag_b[2]);
		printf("magsen_b %f %f %f ", magsen_b[0], magsen_b[1], magsen_b[2]);
		printf("sun_i %f %f %f ", sun_i[0], sun_i[1], sun_i[2]);
		printf("sun_b %f %f %f ", sun_b[0], sun_b[1], sun_b[2]);
		printf("sunsen_b %f %f %f ", sunsen_b[0], sunsen_b[1], sunsen_b[2]);
		printf("omeref %f %f %f ", omeref[0], omeref[1], omeref[2]);
		printf("ome %f %f %f ", ome[0], ome[1], ome[2]);
		printf("\r\n");

	}

	else if(ekf_id==EKF_EDDY){

	}

	else if(ekf_id==EKF_RESIDUAL_MAG){

		for(int i=0; i<3; i++){
			ome[i] = Obs[i];
			omeref[i] = X_ref_pre[i];
		}

		//Omega error
		sub(ome,3,1,omeref,3,1,err_ome);

		y[0] = err_ome[0];
		y[1] = err_ome[1];
		y[2] = err_ome[2];

		// 推定されたジャイロバイアスを使用
//		y[0] = err_ome[0] - X_ref[7];
//		y[1] = err_ome[1] - X_ref[8];
//		y[2] = err_ome[2] - X_ref[9];
	}

	else if(ekf_id==EKF_ANISOTROPY_MAG){

	}


}

/*
 * @brief Calculate r
 * 残差の計算
 * 残差　＝　観測値　ー　事後推定値
 */
void EKF_CLASS::Calc_r(const ADS_MODE ekf_id, float* r){
	//残差の計算
	//観測量と事後推定量の差

	float q[4], qref[4], qref_conj[4], err_q[4], ome[3], omeref[3], err_ome[3];

	if(ekf_id==EKF_GYRO){

		for(int i=0; i<4; i++){
			q[i] = Obs[i];
			qref[i] = X_ref_post[i];
		}

		// 直列EKFでGYROBIASがONの時、推定されたジャイロバイアスを差し引く
		 if(ADS_Mode(EKF_GYROBIAS)){
			for (int i = 0; i < 3; i++){
				ome[i] = Obs[i+4];
				omeref[i] = X_ref_post[i+4] - Obs[i+7];
			}
		 }
		 else{
			for (int i = 0; i < 3; i++){
				ome[i] = Obs[i+4];
				omeref[i] = X_ref_post[i+4];
			}
		 }

		//Quaternion error
		Quaternion_Conjugate(qref,qref_conj);
		Quaternion_Product(qref_conj,q,err_q);

		//Omega error
		sub(ome,3,1,omeref,3,1,err_ome);

		r[0] = err_q[0];
		r[1] = err_q[1];
		r[2] = err_q[2];
		r[3] = err_ome[0];
		r[4] = err_ome[1];
		r[5] = err_ome[2];

	}

	else if (ekf_id==EKF_GYROBIAS){

		for(int i=0; i<4; i++){
			q[i] = Obs[i];
			qref[i] = X_ref_post[i];
		}

		//Quaternion error
		Quaternion_Conjugate(qref,qref_conj);
		Quaternion_Product(qref_conj,q,err_q);

		r[0] = err_q[0];
		r[1] = err_q[1];
		r[2] = err_q[2];

	}

	else if (ekf_id==EKF_MAGSUNGYRO){

		// 直列EKFでGYROBIASがONの時、推定されたジャイロバイアスを差し引く
		 if(ADS_Mode(EKF_GYROBIAS)){
			for (int i = 0; i < 3; i++){
				ome[i] = Obs[i+4];
				omeref[i] = X_ref_post[i+4] - Obs[i+13];
			}
		 }
		 else{
			for (int i = 0; i < 3; i++){
				ome[i] = Obs[i+4];
				omeref[i] = X_ref_post[i+4];
			}
		 }

		//Omega error
		sub(ome,3,1,omeref,3,1,err_ome);

		for(int i=0; i<4; i++){
			qref[i] = X_ref_post[i];
		}
		float mag_i[3], mag_b[3], magsen_b[3], err_mag[3], sun_i[3], sun_b[3], sunsen_b[3], err_sun[3];
		float dcm[3][3];

		for(int i=0; i<3; i++){
			mag_i[i] = g_ads.p_IGRF->IGRF_B_Norm[i];
			magsen_b[i] = Obs[i+7];
			sun_i[i] = g_ads.p_SUNVECTOR->sunvector[i];
			sunsen_b[i] = Obs[i+10];
		}

		Quaternion_To_DCM(qref, *dcm);
		multiplication(*dcm,3,3,mag_i,3,1,mag_b,3,1);
		multiplication(*dcm,3,3,sun_i,3,1,sun_b,3,1);

		for(int i=0; i<3; i++){
			err_mag[i] = magsen_b[i] - mag_b[i];
			err_sun[i] = sunsen_b[i] - sun_b[i];
		}

		// Mag Vector
		r[0] = err_mag[0];
		r[1] = err_mag[1];
		r[2] = err_mag[2];

		// Sun Vector
		if(!vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag)){
			r[3] = err_sun[0];
			r[4] = err_sun[1];
			r[5] = err_sun[2];
		}

		// Omega
		r[6] = err_ome[0];
		r[7] = err_ome[1];
		r[8] = err_ome[2];
	}

	else if(ekf_id==EKF_EDDY){

	}

	else if(ekf_id==EKF_RESIDUAL_MAG){

		for(int i=0; i<3; i++){
			ome[i] = Obs[i];
			omeref[i] = X_ref_post[i];
		}

		//Omega error
		sub(ome,3,1,omeref,3,1,err_ome);

		r[0] = err_ome[0];
		r[1] = err_ome[1];
		r[2] = err_ome[2];
	}

	else if(ekf_id==EKF_ANISOTROPY_MAG){

	}


}


/*
 * @brief Calculate H
 * 観測行列の計算
 */
void EKF_CLASS::Calc_H(const ADS_MODE ekf_id, float* H){
	//非線形観測方程式のヤコビアン　微小状態量観測行列

	if(ekf_id==EKF_GYRO){
		// Quaternionベクトル成分と角速度を観測
		for(int i=0; i<Rdim; i++){
			for(int j=0; j<Pdim; j++){
				H[i*Pdim+j] = i==j ? 1.0f : 0.0f;
			}
		}
	}

	else if(ekf_id==EKF_GYROBIAS){
		// Quaternionベクトル成分を観測
		for(int i=0; i<Rdim; i++){
			for(int j=0; j<Pdim; j++){
				H[i*Pdim+j] = i==j ? 1.0f : 0.0f;
			}
		}
	}

	else if(ekf_id==EKF_MAGSUNGYRO){
		// 角速度と磁場方向と太陽方向を観測

		float qref[4];
		for(int i=0; i<4; i++){
			qref[i] = X_ref[i];
		}

		float mag_i[3];
		float sun_i[3];

		for(int i=0; i<3; i++){
			mag_i[i] = g_ads.p_IGRF->IGRF_B_Norm[i];
		}
		for(int i=0; i<3; i++){
			sun_i[i] = g_ads.p_SUNVECTOR->sunvector[i];
		}


		// 微小状態量　[dmag[3] dsun[3] dome[3]]’
		// Mag Vector
		H[0*Pdim+0] = 2*(qref[0]*mag_i[0] + qref[1]*mag_i[1] + qref[2]*mag_i[2]);
		H[0*Pdim+1] = 2*(-qref[1]*mag_i[0] + qref[0]*mag_i[1] - qref[3]*mag_i[2]);
		H[0*Pdim+2] = 2*(-qref[2]*mag_i[0] + qref[3]*mag_i[1] + qref[0]*mag_i[2]);
		H[1*Pdim+0] = 2*(qref[1]*mag_i[0] - qref[0]*mag_i[1] + qref[3]*mag_i[2]);
		H[1*Pdim+1] = 2*(qref[0]*mag_i[0] + qref[1]*mag_i[1] + qref[2]*mag_i[2]);
		H[1*Pdim+2] = 2*(-qref[3]*mag_i[0] - qref[2]*mag_i[1] + qref[1]*mag_i[2]);
		H[2*Pdim+0] = 2*(qref[2]*mag_i[0] - qref[3]*mag_i[1] - qref[0]*mag_i[2]);
		H[2*Pdim+1] = 2*(qref[3]*mag_i[0] + qref[2]*mag_i[1] - qref[1]*mag_i[2]);
		H[2*Pdim+2] = 2*(qref[0]*mag_i[0] + qref[1]*mag_i[1] + qref[2]*mag_i[2]);
		for(int i=0; i<3; i++){
			for(int j=3; j<6; j++){
				H[i*Pdim+j] = 0.0f;
			}
		}

		// Sun Vector
		if(!vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag)){
			H[3*Pdim+0] = 2*(qref[0]*sun_i[0] + qref[1]*sun_i[1] + qref[2]*sun_i[2]);
			H[3*Pdim+1] = 2*(-qref[1]*sun_i[0] + qref[0]*sun_i[1] - qref[3]*sun_i[2]);
			H[3*Pdim+2] = 2*(-qref[2]*sun_i[0] + qref[3]*sun_i[1] + qref[0]*sun_i[2]);
			H[4*Pdim+0] = 2*(qref[1]*sun_i[0] - qref[0]*sun_i[1] + qref[3]*sun_i[2]);
			H[4*Pdim+1] = 2*(qref[0]*sun_i[0] + qref[1]*sun_i[1] + qref[2]*sun_i[2]);
			H[4*Pdim+2] = 2*(-qref[3]*sun_i[0] - qref[2]*sun_i[1] + qref[1]*sun_i[2]);
			H[5*Pdim+0] = 2*(qref[2]*sun_i[0] - qref[3]*sun_i[1] - qref[0]*sun_i[2]);
			H[5*Pdim+1] = 2*(qref[3]*sun_i[0] + qref[2]*sun_i[1] - qref[1]*sun_i[2]);
			H[5*Pdim+2] = 2*(qref[0]*sun_i[0] + qref[1]*sun_i[1] + qref[2]*sun_i[2]);
			for(int i=0; i<3; i++){
				for(int j=3; j<6; j++){
					H[i*Pdim+j] = 0.0f;
				}
			}
		}else{
			for(int i=3; i<6; i++){
				for(int j=0; j<6; j++){
					H[i*Pdim+j] = 0.0f;
				}
			}
		}

		// Omega
		for(int i=6; i<9; i++){
			for(int j=0; j<6; j++){
				H[i*Pdim+j] = i==j+3 ? 1.0f : 0.0f;
			}
		}
	}

	else if(ekf_id==EKF_EDDY){

	}

	else if(ekf_id==EKF_RESIDUAL_MAG){
		// 角速度を観測
		for(int i=0; i<Rdim; i++){
			for(int j=0; j<Pdim; j++){
				H[i*Pdim+j] = i==j ? 1.0f : 0.0f;
			}
		}
	}

	else if(ekf_id==EKF_ANISOTROPY_MAG){

	}

}

/**
 * @brief Calculate R
 * 観測ノイズ行列の計算
 */
void EKF_CLASS::Calc_R(){

	for (int i = 0; i < Rdim; i++)	{
		for (int j = 0; j < Rdim; j++) {
			R[i*Rdim+j] = i==j ? std_R[i]*std_R[i] : 0.0f ;
		}
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
void EKF_CLASS::Observe(const ADS_MODE ekf_id)
{
	float H[Rdim*Pdim] = {};  //非線形観測方程式のヤコビアン　微小状態量観測行列
	float K[Pdim*Rdim] = {}; //カルマンゲイン
	float y[Rdim] = {};  //　innovation
	float r[Rdim] = {};  // residual 残差

	float BUF1[15*15], BUF2[15*15], BUF3[15*15], BUF4[15*15];
	float H_TRA[Pdim*Rdim];   //Hの転置
    float Dx_renew[Pdim]; //観測更新微小量


    //----------------------イノベーション y = z - h(x_ref) を計算-------------------------------------------
    // y = 観測値　ー　事前推定値
    Calc_y(ekf_id, y);
    //-----------------------------------------------------------------------------------------------

    //----------------------カルマンゲイン K = PH'(HPH'+R)^-1 を計算-------------------------------------------
    if(ADS_Mode(ADS_MODE(ekf_id+1))){
		// H
		Calc_H(ekf_id, H);
		// H'
		trans(H, Rdim, Pdim, H_TRA, Pdim, Rdim);
		// PH'
		multiplication(P_PRE, Pdim, Pdim, H_TRA, Pdim, Rdim, BUF1, Pdim, Rdim);
		// HPH'
		multiplication(H, Rdim, Pdim, BUF1, Pdim, Rdim, BUF3, Rdim, Rdim);
		// HPH'+R
		add(BUF3, Rdim, Rdim, R, Rdim, Rdim, BUF4);
		// (HPH'+R)^-1
		// Diverse Check 逆行列が計算できないときにフラグを立てる
		// H行列の使用しない成分によってアルゴリズム的に0割が発生してしまうが、中でnan回避
		Inverse(BUF4, Rdim, Rdim, BUF3, BUF2);

		// K = PH'(HPH'+R)^-1
		multiplication(BUF1, Pdim, Rdim, BUF2, Rdim, Rdim, K, Pdim, Rdim);
    }
    else{
    	print(EKF,"Constant_K ");
		for (int i = 0; i < Pdim; i++)	{
			for (int j = 0; j < Rdim; j++) {
				K[i*Rdim+j] = (i == j) ? const_K_diag[j] : 0.0f;
			}
		}
    }

    //--------------------------------------------------------------------------------------------------------


    if(ADS_Mode(ADS_MODE(ekf_id+1))){
		//-------------------------------誤差共分散行列を更新 P_ref_renew = (I - KH)P_ref -----------------------------
		// KH
		multiplication(K, Pdim, Rdim, H, Rdim, Pdim, BUF1, Pdim, Pdim);
		// KHP_ref
		multiplication(BUF1, Pdim, Pdim, P_PRE, Pdim, Pdim, BUF2, Pdim, Pdim);
		// P_ref_renew = P_ref - KHP_ref
		sub(P_PRE, Pdim, Pdim, BUF2, Pdim, Pdim, P);
		//------------------------------------------------------------------------------------------------------------
    }

	if(P_constflag == 1){
		print(EKF,"Constant_P ");
		for (int i = 0; i < Pdim; i++)	{
			for (int j = 0; j < Pdim; j++) {
				P[i*Pdim+j] = (i == j) ? const_P_diag[j] : 0.0f;
			}
		}
	}


    // 修正を行った推定値
	// Dx_renew = Ky
	multiplication(K, Pdim, Rdim, y, Rdim, 1, Dx_renew, Pdim, 1);


	//--------------------リファレンス更新値の最終計算------------------------------------
    float Dqv[3], Quaternion_ref[4], Dq_ref[4];
    float Domega[3], Omega_ref[3], Domega_ref[3];
    float Dgbias[3], GBias_ref[3], Dgbias_ref[3];
    float Drmm[3], Rmm_ref[3], Drmm_ref[3];


	if(ekf_id==EKF_GYRO){

		float qua_check = 0.0f;
		for (int i = 0; i < 4; i++){
		  // Quaternion_refに1サイクル前に計算しておいた事前推定値を代入
			Quaternion_ref[i] = X_ref_pre[i];
			qua_check += Quaternion_ref[i];
		}
		if(qua_check==0) Quaternion_ref[3] = 1.0f;

        // Dx_renew = Ky で計算した観測更新微小量DqをDqvに代入
	    for (int i = 0; i < 3; i++){ Dqv[i] = Dx_renew[i]; }
	    // Quaternion積でQuaternionを観測更新
	    Quaternion_obs_update(Quaternion_ref, Dqv, Dq_ref, &dq_valid);

		for (int i = 0; i < 3; i++){
			Domega[i] = Dx_renew[i+3];
			Omega_ref[i] = X_ref_pre[i+4];
			Domega_ref[i] = Omega_ref[i] + Domega[i];
		}

	    X_ref[0] = Dq_ref[0]; //観測更新したQuaternionをX_refに代入
	    X_ref[1] = Dq_ref[1];
	    X_ref[2] = Dq_ref[2];
	    X_ref[3] = Dq_ref[3];
	    X_ref[4] = Domega_ref[0];
	    X_ref[5] = Domega_ref[1];
	    X_ref[6] = Domega_ref[2];

	}

	else if(ekf_id==EKF_GYROBIAS){

		float qua_check = 0.0f;
		for (int i = 0; i < 4; i++){
			Quaternion_ref[i] = X_ref_pre[i];
			qua_check += Quaternion_ref[i];
		}
		if(qua_check==0) Quaternion_ref[3] = 1.0f;

	    for (int i = 0; i < 3; i++){ Dqv[i] = Dx_renew[i]; }
	    Quaternion_obs_update(Quaternion_ref, Dqv, Dq_ref, &dq_valid);

		for (int i = 0; i < 3; i++){
			Dgbias[i] = Dx_renew[i+3];
			GBias_ref[i] = X_ref_pre[i+4];
			Dgbias_ref[i] = GBias_ref[i] + Dgbias[i];
		}

		X_ref[0] = Dq_ref[0];
		X_ref[1] = Dq_ref[1];
		X_ref[2] = Dq_ref[2];
	    X_ref[3] = Dq_ref[3];
	    X_ref[4] = Dgbias_ref[0];
	    X_ref[5] = Dgbias_ref[1];
	    X_ref[6] = Dgbias_ref[2];

	}

	else if (ekf_id==EKF_MAGSUNGYRO) {

	    // 修正を行った推定値
		// Dx_renew = Ky
		// Omega に関する　K　の修正
		if(K_type == 1){
			// 角速度のみ一定カルマンゲイン （EKFが収束しなくて考えた苦肉の策その1）
			for(int i=3; i<Pdim; i++){
				for(int j=0; j<Rdim; j++){
					K[i*Rdim+j] = i+3==j ? const_K_diag[j] : 0.0f;
				}
			}
		}
		else if(K_type == 2){
			// Quaternion　と　角速度　の　カルマンゲインを分離 （EKFが収束しなくて考えた苦肉の策その2）
			for(int i=0; i<3; i++){
				for(int j=6; j<Rdim; j++){
					K[i*Rdim+j] = 0.0f;
				}
			}
			for(int i=3; i<Pdim; i++){
				for(int j=0; j<6; j++){
					K[i*Rdim+j] = 0.0f;
				}
			}
		}
		multiplication(K, Pdim, Rdim, y, Rdim, 1, Dx_renew, Pdim, 1);
//printf("K\r\n");
//display_exp(K,Pdim,Rdim);
//printf("ekf_valid %u dq_valid %u divg_valid %u ", ekf_valid, dq_valid, divg_valid);
		float qua_check = 0.0f;
		for (int i = 0; i < 4; i++){
			Quaternion_ref[i] = X_ref_pre[i];
			qua_check += Quaternion_ref[i];
		}
		if(qua_check==0) Quaternion_ref[3] = 1.0f;

	    for (int i = 0; i < 3; i++){ Dqv[i] = Dx_renew[i]; }
	    Quaternion_obs_update(Quaternion_ref, Dqv, Dq_ref, &dq_valid);

		for (int i = 0; i < 3; i++){
			Domega[i] = Dx_renew[i+3];
			Omega_ref[i] = X_ref_pre[i+4];
			Domega_ref[i] = Omega_ref[i] + Domega[i];
		}

	    X_ref[0] = Dq_ref[0];
	    X_ref[1] = Dq_ref[1];
	    X_ref[2] = Dq_ref[2];
	    X_ref[3] = Dq_ref[3];
	    X_ref[4] = Domega_ref[0];
	    X_ref[5] = Domega_ref[1];
	    X_ref[6] = Domega_ref[2];
	}

	else if(ekf_id==EKF_EDDY){

	}

	else if(ekf_id==EKF_RESIDUAL_MAG){

		dq_valid = 1; // 微小Quaternionの判定は正常にしておく

		for (int i = 0; i < 3; i++){
			Domega[i] = Dx_renew[i];
			Omega_ref[i] = X_ref_pre[i];
			Domega_ref[i] = Omega_ref[i] + Domega[i];

		    Drmm[i] = Dx_renew[i+3];
		    Rmm_ref[i] = X_ref_pre[i+3];
		    Drmm_ref[i] = Rmm_ref[i] + Drmm[i];
		}

		X_ref[0] = Domega_ref[0];
		X_ref[1] = Domega_ref[1];
		X_ref[2] = Domega_ref[2];
		X_ref[3] = Drmm_ref[0];
		X_ref[4] = Drmm_ref[1];
		X_ref[5] = Drmm_ref[2];
	}

	else if(ekf_id==EKF_ANISOTROPY_MAG){

	}

	//--------------------------------------------------------------------------


    if(ADS_Mode(ADS_MODE(ekf_id+1)) && adp_noise_flag){
    /* --- Adaptive Noise Covariance ---
	 * reference: Shahrokh Akhiaghi et al.
	 * "Adaptive adjustment of noise covariance in Kalman filter for dynamic state estimation," 2017 IEEE
	 */
    // ノイズ適応型EKF。論文の理論通りではRの計算が出来ないので1ステップ前のRの値を利用。

		Calc_r(ekf_id, r);

	    float K_tr[15*15], H_tr[15*15];
	    float y_tr[15], r_tr[15];

/*** 使用しない方がよい
	    //State Covariance Update
	    if(Pdim == Qdim){
			trans(y,Rdim,1,y_tr,1,Rdim);
			trans(K,Pdim,Rdim,K_tr,Rdim,Pdim);
			CoeffMat(Q,Qdim,Qdim,alpha,BUF1);
			multiplication(y,Rdim,1,y_tr,1,Rdim,BUF2,Rdim,Rdim);
			multiplication(K,Pdim,Rdim,BUF2,Rdim,Rdim,BUF3,Pdim,Rdim);
			multiplication(BUF3,Pdim,Rdim,K_tr,Rdim,Pdim,BUF4,Pdim,Pdim);
			CoeffMat(BUF4,Pdim,Pdim,1.0f-alpha,BUF2);
			add(BUF1,Qdim,Qdim,BUF2,Pdim,Pdim,Q);
	    }
***/
		//Measurement Covariance Update
		trans(r,Rdim,1,r_tr,1,Rdim);
		trans(H,Rdim,Pdim,H_tr,Pdim,Rdim);
		CoeffMat(R,Rdim,Rdim,alpha,BUF1);
		multiplication(r,Rdim,1,r_tr,1,Rdim,BUF2,Rdim,Rdim);
		multiplication(H,Rdim,Pdim,P,Pdim,Pdim,BUF3,Rdim,Pdim);
		multiplication(BUF3,Rdim,Pdim,H_tr,Pdim,Rdim,BUF4,Rdim,Rdim);
		add(BUF2,Rdim,Rdim,BUF4,Rdim,Rdim,BUF3);
		CoeffMat(BUF3,Rdim,Rdim,1.0f-alpha,BUF2);
		add(BUF1,Rdim,Rdim,BUF2,Rdim,Rdim,R);

    }

    // ---------------------------------------------


//	printf("H\r\n");
//  display_exp(H,Rdim,Pdim);
//	printf("K\r\n");
//	display_exp(K,Pdim,Rdim);
//	printf("Q\r\n");
//	display_exp(Q,Qdim,Qdim);
//	printf("R\r\n");
//	display_exp(R,Rdim,Rdim);

}

uint8_t EKF_CLASS::Sep_Att_Check(int process_cnt, uint itr){

	if(vote_sep_flag(&p_sep_driver->sep_condition_check.separate_checker_flag)){

		// 条件判断に使用するパラメータ
		float quaternion[4] = {X_ref[0], X_ref[1], X_ref[2], X_ref[3]};
		float omega_b[3] = {X_ref[4], X_ref[5], X_ref[6]};
		float pos[3] = {};
		float vel[3] = {};
		Normalization(g_ods.p_ODS->Position, 3, pos);
		Normalization(g_ods.p_ODS->Velocity, 3, vel);

		// 角度誤差評価用の変数
		float err_deg = 0.0f;

		// フラグの初期化（条件を満たしていなければフラグを落としていく）
		sep_condition_tlm.Condition_flag[0] = 1;
		sep_condition_tlm.Condition_flag[1] = 1;
		sep_condition_tlm.Condition_flag[2] = 1;
		sep_condition_tlm.Condition_flag[3] = 1;
		sep_condition_tlm.Condition_flag[4] = 1;
		sep_condition_tlm.Condition_flag[5] = 1;
		sep_condition_tlm.Condition_flag[6] = 1;
		sep_condition_tlm.Condition_flag[7] = 1;

		// 機体座標から慣性座標へのDCM計算
		float DCMi2b[3][3] = {};
		float DCMb2i[3][3] = {};
		Quaternion_To_DCM(quaternion, &DCMi2b[0][0]);
		trans(&DCMi2b[0][0], 3, 3, &DCMb2i[0][0], 3, 3);

		// 慣性座標系からLVLH座標系へのDCM計算
		float DCMi2lvlh[3][3] = {};
		DCMeci2lvlh(pos, vel, &DCMi2lvlh[0][0]);

//		printf("quaternion: %f, %f, %f, %f\r\n", quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
//		printf("omega_b: %f, %f, %f, %f\r\n", omega_b[0], omega_b[1], omega_b[2]);

		// 角速度の大きさが十分か
		if (vote_sep_flag(&p_sep_driver->sep_condition_check.rotation_speed_checker)){
//			printf("rotation_speed_checker\r\n");
//			printf("%f, %f\r\n", Fabs(omega_b, 3), omega_abs_thr);

			// 条件判断
			if (Fabs(omega_b, 3) < omega_abs_thr){
//				printf("return\r\n");
				sep_condition_tlm.Condition_flag[0] = 0;
			}
		}

		// 機体座標系に対する回転軸の方向が正しいか
		if (vote_sep_flag(&p_sep_driver->sep_condition_check.rotation_direction_b_checker)){
//			printf("rotation_direction_b_checker\r\n");

			// 制御軸
			float Cd[3] = {};
			copy(Cross_class.ControlDirection_Spin, 3, 1, Cd);

			// 回転軸と制御軸の角度誤差を計算
			err_deg = VecAngle(Cd, omega_b, 3, 1);

//			printf("%f, %f\r\n", err_deg, direction_err_b);

			// 条件判断
			if (err_deg > direction_err_b){
//				printf("return\r\n");
				sep_condition_tlm.Condition_flag[1] = 0;
			}
		}

		// 機体座標系での角速度が正しいか(各軸判定)
		if (vote_sep_flag(&p_sep_driver->sep_condition_check.angular_velocity_checker)){
			if (omega_b[0] < omega_X_min || omega_b[0] > omega_X_max ||
				omega_b[1] < omega_Y_min || omega_b[1] > omega_Y_max ||
				omega_b[2] < omega_Z_min || omega_b[2] > omega_Z_max){
				sep_condition_tlm.Condition_flag[2] = 0;
			}
		}

		// 姿勢決定精度が十分か
		if (vote_sep_flag(&p_sep_driver->sep_condition_check.attitude_accuracy_checker)){
//			printf("attitude_accuracy_checker\r\n");

			// 一つでも共分散が閾値より大きいならばフラグを下げる
			for (int i = 0; i < 6; i++){
//				printf("%f, %f\r\n", P_REF[i * Pdim + i], err_cov_thr);
				if (P[i*Pdim+i] > err_cov_thr[i]){
//					printf("return\r\n");
					sep_condition_tlm.Condition_flag[3] = 0;
				}
			}
		}

		// ECI座標系に対する回転軸の方向が正しいか
		if (vote_sep_flag(&p_sep_driver->sep_condition_check.rotation_direction_i_checker)){
//			printf("rotation_direction_i_checker\r\n");

			// 慣性座標系における回転軸
			float omega_eci[3] = {};
			multiplication(&DCMb2i[0][0], 3, 3, omega_b, 3, 1, omega_eci, 3, 1);

//			printf("omega_eci: %f %f %f\r\n", omega_eci[0], omega_eci[1], omega_eci[2]);

			// 目標方向
			float Td[3] = {};
			copy(Cross_class.TargetDirection_Spin, 3, 1, Td);

			// 回転軸と目標方向の角度誤差を計算
			err_deg = VecAngle(Td, omega_eci, 3, 1);

//			printf("%f, %f\r\n", err_deg, direction_err_eci);

			// 条件判断
			if (err_deg > direction_err_eci){
//				printf("return\r\n");
				sep_condition_tlm.Condition_flag[4] = 0;
			}
		}

		// LVLH座標系に対する回転軸の方向が正しいか
		if (vote_sep_flag(&p_sep_driver->sep_condition_check.separate_direction_checker)){
//			printf("separate_direction_checker\r\n");
			float sep_direction_b[3] = {};
			float sep_direction_eci[3] = {};
			float sep_direction_lvlh[3] = {};

			// 機体座標系における分離方向を計算
			Outer_product(omega_b, pos_1U_b, sep_direction_b);

			// 慣性座標系における分離方向を計算
			multiplication(&DCMb2i[0][0], 3, 3, sep_direction_b, 3, 1, sep_direction_eci, 3, 1);

//			printf("DCMi2lvlh\r\n%f %f %f\r\n%f %f %f\r\n%f %f %f\r\n", DCMi2lvlh[0][0], DCMi2lvlh[0][1], DCMi2lvlh[0][2], DCMi2lvlh[1][0], DCMi2lvlh[1][1], DCMi2lvlh[1][2], DCMi2lvlh[2][0], DCMi2lvlh[2][1], DCMi2lvlh[2][2]);

			// LVLH座標系における分離方向を計算
			multiplication(&DCMi2lvlh[0][0], 3, 3, sep_direction_eci, 3, 1, sep_direction_lvlh, 3, 1);

			// 目標分離方向との角度誤差を計算
			err_deg = VecAngle(separate_direction, sep_direction_lvlh, 3, 1);

//			printf("%f, %f\r\n", err_deg, sep_direction_err);

			// 条件判断
			if (err_deg > sep_direction_err){
//				printf("return\r\n");
				sep_condition_tlm.Condition_flag[5] = 0;
			}
		}

		float orbit_normal_i[3] = {};
		float orbit_normal_b[3] = {};
		Outer_product(pos, vel, orbit_normal_i);
		multiplication(&DCMi2b[0][0], 3, 3, orbit_normal_i, 3, 1, orbit_normal_b, 3, 1);

		// 速度方向に対する回転軸方向が正しいか
		if (vote_sep_flag(&p_sep_driver->sep_condition_check.rotation_direction_v_checker)){
			float vel_b[3] = {};

			multiplication(&DCMi2b[0][0], 3, 3, vel, 3, 1, vel_b, 3, 1);

			err_deg = fabs(VecAngle(omega_b, vel_b, 3, 1) - angle_velocity2omega);

			if (err_deg > angle_velocity2omega_err){
				sep_condition_tlm.Condition_flag[6] = 0;
			}

			float V_cross_Omega[3] = {};

			Outer_product(vel_b, omega_b, V_cross_Omega);

			err_deg = fabs(VecAngle(orbit_normal_b, V_cross_Omega, 3, 1) - angle_normal2vXomega);

			if (err_deg > angle_normal2vXomega_err){
				sep_condition_tlm.Condition_flag[6] = 0;
			}
		}

		// 分離時の回転位相が正しいか
		if (vote_sep_flag(&p_sep_driver->sep_condition_check.rotation_phase_checker)){

			err_deg = fabs(VecAngle(pos_1U_b, orbit_normal_b, 3, 1) - sep_phase);
			if (err_deg > sep_phase_err){
				sep_condition_tlm.Condition_flag[7] = 0;
			}
		}

//		printf("Estimated Parameter\r\n");
//		printf("condition_flag: %d %d %d %d %d %d %d %d\r\n", sep_condition_tlm.Condition_flag[0], sep_condition_tlm.Condition_flag[1], sep_condition_tlm.Condition_flag[2], sep_condition_tlm.Condition_flag[3], sep_condition_tlm.Condition_flag[4], sep_condition_tlm.Condition_flag[5], sep_condition_tlm.Condition_flag[6], sep_condition_tlm.Condition_flag[7]);
//		printf("quaternion: %f %f %f %f\r\n", quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
//		printf("omega: %f %f %f\r\n", omega_b[0], omega_b[1], omega_b[2]);

		if(ekf_num == select_ekf){
			parse_tlm(process_cnt, itr);
		}

		// 条件をすべて満たしていれば1を返す
		if (sep_condition_tlm.Condition_flag[0] * sep_condition_tlm.Condition_flag[1] * sep_condition_tlm.Condition_flag[2] * sep_condition_tlm.Condition_flag[3] *
			sep_condition_tlm.Condition_flag[4] * sep_condition_tlm.Condition_flag[5] * sep_condition_tlm.Condition_flag[6] * sep_condition_tlm.Condition_flag[7] == 1){
//			printf("sep OK\r\n");
//			printf("\r\n");
			return 1;
		}else{
//			printf("return\r\n");
//			printf("\r\n");
			return 0;
		}

	}else{
		return 0;
	}
}


// 二つのクォータニオンの差分を計算
void EKF_CLASS::Quaternion_delta(float* q_acq, float* q_obs, float* q_delta){

	float q_acq_conj[4];
	Quaternion_Conjugate(q_acq, q_acq_conj);
	Quaternion_Product(q_acq_conj, q_obs, q_delta);

}

// 観測更新用。物理量取得時刻の衛星Quaternion　と　基準観測時刻の衛星Quaternionの差を計算
void EKF_CLASS::Calc_Qua_Delta_AcqTime2ObsTime(float* q_at_acq_mult, float* q_at_obs, float* q_delta_mult){

	for(int i=0; i<4; i++){
		Quaternion_delta(&q_at_acq_mult[i*4], q_at_obs, &q_delta_mult[i*4]);
	}

}

// 観測更新用。物理量を取得時刻から基準観測時刻への値へと座標変換
// 詳しくは「MAGNARO搭載EKF補足資料.pptx」を参照
void EKF_CLASS::Adjust_Obs_Time(const ADS_MODE ekf_id){

	// 初期化
	for(int i=0; i<4; i++){
		q_delta[i*4+0] = 0.0f;
		q_delta[i*4+1] = 0.0f;
		q_delta[i*4+2] = 0.0f;
		q_delta[i*4+3] = 1.0f;
	}

	uint8_t sun=0; uint8_t mag=1; uint8_t qua=2; uint8_t gyro=3;
	float deltaDCM[3*3];

	// 各物理値を取得した時のQuaternionと観測更新基準時Quaternionから、観測基準時へ物理量を座標変換

	if(ekf_id == EKF_GYRO){

		float q_at_obs[4] = {X_ref_pre[0], X_ref_pre[1], X_ref_pre[2], X_ref_pre[3]};
		Calc_Qua_Delta_AcqTime2ObsTime(q_at_acq, q_at_obs, q_delta);

		float qua_state_at_acq[4] = {Obs[0], Obs[1], Obs[2], Obs[3]};
		float qua_state_at_obs[4];
		Quaternion_Product(qua_state_at_acq, &q_delta[qua*4], qua_state_at_obs);
		for(int i=0; i<4; i++){
			Obs[i] = qua_state_at_obs[i];
		}

		Quaternion_To_DCM(&q_delta[gyro*4],deltaDCM);
		float ome_state_at_acq[3] = {Obs[4], Obs[5], Obs[6]};
		float ome_state_at_obs[3];
		multiplication(deltaDCM,3,3,ome_state_at_acq,3,1,ome_state_at_obs,3,1);
		for(int i=0; i<3; i++){
			Obs[i+4] = ome_state_at_obs[i];
		}
	}

	else if(ekf_id == EKF_GYROBIAS){

		float q_at_obs[4] = {X_ref_pre[0], X_ref_pre[1], X_ref_pre[2], X_ref_pre[3]};
		Calc_Qua_Delta_AcqTime2ObsTime(q_at_acq, q_at_obs, q_delta);

		float qua_state_at_acq[4] = {Obs[0], Obs[1], Obs[2], Obs[3]};
		float qua_state_at_obs[4];
		Quaternion_Product(qua_state_at_acq, &q_delta[qua*4], qua_state_at_obs);
		for(int i=0; i<4; i++){
			Obs[i] = qua_state_at_obs[i];
		}

		Quaternion_To_DCM(&q_delta[gyro*4],deltaDCM);
		float ome_state_at_acq[3] = {Obs[4], Obs[5], Obs[6]};
		float ome_state_at_obs[3];
		multiplication(deltaDCM,3,3,ome_state_at_acq,3,1,ome_state_at_obs,3,1);
		for(int i=0; i<3; i++){
			Obs[i+4] = ome_state_at_obs[i];
		}
	}

	else if(ekf_id == EKF_MAGSUNGYRO){

		float q_at_obs[4] = {X_ref_pre[0], X_ref_pre[1], X_ref_pre[2], X_ref_pre[3]};
		Calc_Qua_Delta_AcqTime2ObsTime(q_at_acq, q_at_obs, q_delta);

		float qua_state_at_acq[4] = {Obs[0], Obs[1], Obs[2], Obs[3]};
		float qua_state_at_obs[4];
		Quaternion_Product(qua_state_at_acq, &q_delta[qua*4], qua_state_at_obs);
		for(int i=0; i<4; i++){
			Obs[i] = qua_state_at_obs[i];
		}

		Quaternion_To_DCM(&q_delta[gyro*4],deltaDCM);
		float ome_state_at_acq[3] = {Obs[4], Obs[5], Obs[6]};
		float ome_state_at_obs[3];
		multiplication(deltaDCM,3,3,ome_state_at_acq,3,1,ome_state_at_obs,3,1);
		for(int i=0; i<3; i++){
			Obs[i+4] = ome_state_at_obs[i];
		}

		Quaternion_To_DCM(&q_delta[mag*4],deltaDCM);
		float mag_state_at_acq[3] = {Obs[7], Obs[8], Obs[9]};
		float mag_state_at_obs[3];
		multiplication(deltaDCM,3,3,mag_state_at_acq,3,1,mag_state_at_obs,3,1);
		for(int i=0; i<3; i++){
			Obs[i+7] = mag_state_at_obs[i];
		}

		Quaternion_To_DCM(&q_delta[sun*4],deltaDCM);
		float sun_state_at_acq[3] = {Obs[10], Obs[11], Obs[12]};
		float sun_state_at_obs[3];
		multiplication(deltaDCM,3,3,sun_state_at_acq,3,1,sun_state_at_obs,3,1);
		for(int i=0; i<3; i++){
			Obs[i+10] = sun_state_at_obs[i];
		}
	}

}

//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

//入力には接頭in_ 出力はout_を付ける
void EKF_CLASS::cal(const ADS_MODE ekf_id, const float* in_X, uint8_t in_refdim, uint8_t in_obsdim, uint8_t in_Pdim, uint8_t in_Qdim, uint8_t in_Rdim)
{
	refdim = in_refdim;
	obsdim = in_obsdim;
	Pdim = in_Pdim;
	Qdim = in_Qdim;
	Rdim = in_Rdim;


	uint itr;
	dt_sec = dt/1000.0f;
	itr = floorf(max_prop/dt);

	//　テレメ用に１サイクル前の事前推定値をコピー
	memcpy(X_ref_pre_previous, X_ref_pre, sizeof(X_ref_pre));
	memcpy(P_PRE_previous, P_PRE, sizeof(P_PRE));

	//まず観測し、そこからプロパゲート
	//-----観測-----//
	for(int i=0; i<obsdim; i++){
		Obs[i] = in_X[i];
	}

	// 観測値の時刻ずれ補正
	if(reltime_type > 0){
		Adjust_Obs_Time(ekf_id);
	}

	Observe(ekf_id);

	memcpy(X_ref_post, X_ref, sizeof(X_ref));
	memcpy(P_POST, P, sizeof(P));

	//-----推測-----//

	if(reltime_type == 2){

		// 次回観測値更新のため、時間ずれ調整用に観測基準時刻からの相対時間を取得
		// sun sensor
		sensor_reltime_to_obs[0] = - OBCT_diff_in_step(&g_acs.p_GLOBAL_AOCS->SunTime, &g_acs.p_GLOBAL_AOCS->OmeTime);
		// magnetic sensor
		sensor_reltime_to_obs[1] = - OBCT_diff_in_step(&g_acs.p_GLOBAL_AOCS->MagTime, &g_acs.p_GLOBAL_AOCS->OmeTime);
		// quaternion TRIAD,QUEST,BBDOT などで計算されるQuaternionはSUNとMAGの真ん中の時間とする
		sensor_reltime_to_obs[2] = floorf((sensor_reltime_to_obs[0] + sensor_reltime_to_obs[1]) / 2);
		// gyro sensor
		sensor_reltime_to_obs[3] = 0;

	}
//	for(int phys=0; phys<4; phys++) printf("reltime %d ", sensor_reltime_to_obs[phys]);
//	printf("\r\n");

	for(int i=0; i<itr; i++){

		Predict(ekf_id);

		//　次サイクル観測更新用。各センサ取得時Quaternion
		// sun, mag, omega, quaternion
		// 詳しくは「MAGNARO搭載EKF補足資料.pptx」を参照
		if(reltime_type > 0){
			for(int phys=0; phys<4; phys++){
				if(i*dt < (observe_time + sensor_reltime_to_obs[phys])
						&& (observe_time + sensor_reltime_to_obs[phys]) <= (i+1)*dt){
					for(int q=0; q<4; q++) q_at_acq[phys*4+q] = X_ref[q];
				}
			}
		}

		// 次サイクル観測更新用。事前推定値
		if(i*dt < observe_time && observe_time <= (i+1)*dt){
			memcpy(X_ref_pre, X_ref, sizeof(X_ref));
			memcpy(P_PRE, P, sizeof(P));
		}

//		if(ekf_id == EKF_GYRO || ekf_id == EKF_MAGSUNGYRO){
//		print(EKF, "!eclipse_flag %u ", !g_acs.p_GLOBAL_AOCS->Eclipse_flag);
			print(EKF,"ekf_id %u ", ekf_id);
			print(EKF,"ekf_valid %u dq_valid %u divg_valid %u ", ekf_valid, dq_valid, divg_valid);
//			print(EKF,"X_pre %f %f %f %f %f %f %f ", X_ref_pre_previous[0], X_ref_pre_previous[1], X_ref_pre_previous[2], X_ref_pre_previous[3], X_ref_pre_previous[4], X_ref_pre_previous[5], X_ref_pre_previous[6]);
			print(EKF,"Obs %f %f %f %f %f %f %f ", in_X[0], in_X[1], in_X[2], in_X[3], in_X[4], in_X[5], in_X[6]);
			print(EKF,"X_ref %f %f %f %f %f %f %f ", X_ref[0], X_ref[1], X_ref[2], X_ref[3], X_ref[4], X_ref[5], X_ref[6]);
			print(EKF,"P %.2e %.2e %.2e %.2e %.2e %.2e\r\n", P[0*Pdim+0], P[1*Pdim+1], P[2*Pdim+2], P[3*Pdim+3], P[4*Pdim+4], P[5*Pdim+5]);
//		}
	}

	// 発散チェック
	for(int i=0; i<Pdim; i++){
		if(P[i*Pdim+i] > conv_thr || P[i*Pdim+i] < -conv_thr || isnan(P[i*Pdim+i]) || isinf(P[i*Pdim+i]) ){
			divg_valid = 0;
			break;
		}
		else{
			divg_valid = 1;
		}
	}

	// EKFの有効性
	if(isnanMat(X_ref,refdim,1)) {
		ekf_valid = 0;
		Zeros(refdim,1,X_ref);
	}
	else if(dq_valid * divg_valid == 0){
		ekf_valid = 0;
	}
	else{
		ekf_valid = 1;
	}

}



int16_t EKF_CLASS::cal_hispin(const ADS_MODE ekf_id, const float* in_X, uint8_t in_refdim, uint8_t in_obsdim, uint8_t in_Pdim, uint8_t in_Qdim, uint8_t in_Rdim)
{
	refdim = in_refdim;
	obsdim = in_obsdim;
	Pdim = in_Pdim;
	Qdim = in_Qdim;
	Rdim = in_Rdim;

	uint itr;
	dt_sec = dt_hispin/1000.0f;
	itr = floorf(max_prop_hispin/dt_hispin);
//	printf("Pdim Qdim Rdim %u %u %u\r\n", Pdim, Qdim, Rdim);

	//　テレメ用に１サイクル前の事前推定値をコピー
	memcpy(X_ref_pre_previous, X_ref_pre, sizeof(X_ref_pre));
	memcpy(P_PRE_previous, P_PRE, sizeof(P_PRE));

	//まず観測し、そこからプロパゲート
	//-----観測-----//
	for(int i=0; i<obsdim; i++){
		Obs[i] = in_X[i];
	}

	// 観測値の時刻ずれ補正
	if(reltime_type > 0){
		Adjust_Obs_Time(ekf_id);
	}

	Observe(ekf_id);

	memcpy(X_ref_post, X_ref, sizeof(X_ref));
	memcpy(P_POST, P, sizeof(P));

	//-----推測-----//

	if(reltime_type == 2){

		// 次回観測値更新のため、時間ずれ調整用に観測基準時刻からの相対時間を取得
		// sun sensor
		sensor_reltime_to_obs[0] = - OBCT_diff_in_step(&g_acs.p_GLOBAL_AOCS->SunTime, &g_acs.p_GLOBAL_AOCS->OmeTime);
		// magnetic sensor
		sensor_reltime_to_obs[1] = - OBCT_diff_in_step(&g_acs.p_GLOBAL_AOCS->MagTime, &g_acs.p_GLOBAL_AOCS->OmeTime);
		// quaternion TRIAD,QUEST,BBDOT などで計算されるQuaternionはSUNとMAGの真ん中の時間とする
		sensor_reltime_to_obs[2] = floorf((sensor_reltime_to_obs[0] + sensor_reltime_to_obs[1]) / 2);
		// gyro sensor
		sensor_reltime_to_obs[3] = 0;

	}
//	for(int phys=0; phys<4; phys++) printf("reltime %d ", sensor_reltime_to_obs[phys]);
//	printf("\r\n");

	count = 0;
//	print(EKF,"dt %u ", dt);
	for(int i=0; i<itr; i++){

		Predict(ekf_id);

		//　次サイクル観測更新用。各センサ取得時Quaternion
		// sun, mag, omega, quaternion
		// 詳しくは「MAGNARO搭載EKF補足資料.pptx」を参照
		if(reltime_type > 0){
			for(int phys=0; phys<4; phys++){
				if(i*dt_hispin < (observe_time_hispin + sensor_reltime_to_obs[phys])
						&& (observe_time_hispin + sensor_reltime_to_obs[phys]) <= (i+1)*dt_hispin){
					for(int q=0; q<4; q++) q_at_acq[phys*4+q] = X_ref[q];
				}
			}
		}

		// 次サイクル観測更新用の事前推定値
		if(i*dt_hispin < observe_time_hispin && observe_time_hispin <= (i+1)*dt_hispin){
			print(EKF,"SavedXandP ");
			memcpy(X_ref_pre, X_ref, sizeof(X_ref));
			memcpy(P_PRE, P, sizeof(P));
		}

		//　プロパゲートステップごとに姿勢軌道状態から分離条件判断
		if(Sep_Att_Check(i, itr) && i*dt_hispin > sep_time_under_limit) {
			sep_time = i * dt_hispin;
			break;
		}else{
			sep_time = -1;
		}

//		if(ekf_id == EKF_GYRO){
//		print(EKF, "!eclipse_flag %u ", !g_acs.p_GLOBAL_AOCS->Eclipse_flag);
			print(EKF,"ekf_id %u ", ekf_id);
			print(EKF,"ekf_valid %u dq_valid %u divg_valid %u ", ekf_valid, dq_valid, divg_valid);
//			print(EKF,"X_pre %f %f %f %f %f %f %f ", X_ref_pre_previous[0], X_ref_pre_previous[1], X_ref_pre_previous[2], X_ref_pre_previous[3], X_ref_pre_previous[4], X_ref_pre_previous[5], X_ref_pre_previous[6]);
			print(EKF,"Obs %f %f %f %f %f %f %f ", in_X[0], in_X[1], in_X[2], in_X[3], in_X[4], in_X[5], in_X[6]);
			print(EKF,"X_ref %f %f %f %f %f %f %f ", X_ref[0], X_ref[1], X_ref[2], X_ref[3], X_ref[4], X_ref[5], X_ref[6]);
			print(EKF,"P %.2e %.2e %.2e %.2e %.2e %.2e\r\n", P[0*Pdim+0], P[1*Pdim+1], P[2*Pdim+2], P[3*Pdim+3], P[4*Pdim+4], P[5*Pdim+5]);
//		}

	}
//	printf("sensor_qua_hispin %f %f %f %f %f %f %f \r\n", sensor_qua_hispin[0][0], sensor_qua_hispin[1][0], sensor_qua_hispin[2][0], sensor_qua_hispin[3][0], sensor_qua_hispin[6][0], sensor_qua_hispin[7][0], sensor_qua_hispin[8][0]);

	// 発散チェック
	for(int i=0; i<Pdim; i++){
		if(P[i*Pdim+i] > conv_thr || P[i*Pdim+i] < -conv_thr || isnan(P[i*Pdim+i]) || isinf(P[i*Pdim+i]) ){
			divg_valid = 0;
			break;
		}
		else{
			divg_valid = 1;
		}
	}

	// EKFの有効性
	if(isnanMat(X_ref,refdim,1)) {
		ekf_valid = 0;
		Zeros(refdim,1,X_ref);
		return -2;
	}
	else if(dq_valid * divg_valid == 0){
		ekf_valid = 0;
		return -2;
	}
	else{
		ekf_valid = 1;
		return sep_time;
	}

}


void EKF_CLASS::parse_tlm(int process_cnt, uint itr){

	uint8_t data_len = 36;
	uint8_t max_cnt = (TCP_MAX_LEN - TCP_PRM_HDR_LEN - TCP_TLM_2ND_HDR_LEN) / data_len;

	if(count >= max_cnt){
		count = 0;
		return;
	}

	if(process_cnt % width == 0){
		uint8_t* param = TCP_TLM_get_user_data_head(&pckt) + data_len * count;
		TF_copy_float(param     , X_ref[0]);
		TF_copy_float(param +  4, X_ref[1]);
		TF_copy_float(param +  8, X_ref[2]);
		TF_copy_float(param + 12, X_ref[3]);
		TF_copy_float(param + 16, X_ref[4]);
		TF_copy_float(param + 20, X_ref[5]);
		TF_copy_float(param + 24, X_ref[6]);
		TF_copy_u8(param + 28, sep_condition_tlm.Condition_flag[0]);
		TF_copy_u8(param + 29, sep_condition_tlm.Condition_flag[1]);
		TF_copy_u8(param + 30, sep_condition_tlm.Condition_flag[2]);
		TF_copy_u8(param + 31, sep_condition_tlm.Condition_flag[3]);
		TF_copy_u8(param + 32, sep_condition_tlm.Condition_flag[4]);
		TF_copy_u8(param + 33, sep_condition_tlm.Condition_flag[5]);
		TF_copy_u8(param + 34, sep_condition_tlm.Condition_flag[6]);
		TF_copy_u8(param + 35, sep_condition_tlm.Condition_flag[7]);
		count++;
	}

	uint8_t flag1;
	uint8_t flag2;
	//カウントMAX or 終了時にたまっていたら保存
	sep_condition_tlm.Condition_flag[0] * sep_condition_tlm.Condition_flag[1] * sep_condition_tlm.Condition_flag[2] *
	sep_condition_tlm.Condition_flag[3] * sep_condition_tlm.Condition_flag[4] * sep_condition_tlm.Condition_flag[5] *
	sep_condition_tlm.Condition_flag[6] * sep_condition_tlm.Condition_flag[7]== 1 && count != 0 ? flag1 = 1 : flag1 = 0;
	process_cnt == (int)(itr - 1) && count != 0 ? flag2 = 1 : flag2 = 0;

	if(count == max_cnt || flag1 == 1 || flag2 == 1){
		TCP_TLM_setup_primary_hdr(&pckt, (TCP_APID)AFSK_GS, (uint16_t)(max_cnt * data_len + TCP_TLM_2ND_HDR_LEN + TCP_PRM_HDR_LEN));
		TCP_set_route(&pckt, TCP_ROUTE_DIRECT);
		TCP_set_all_issuer_id(&pckt, (TCP_SAT_ID)MY_SAT_ID, TCP_EXE_ID_MOBC);
		TCP_TLM_set_ti(&pckt, (uint32_t)(TMGR_get_master_total_cycle()));
		TCP_TLM_set_category(&pckt, 0x80);
		TCP_TLM_set_packet_id(&pckt, Tlm_CODE_SEPARATION);
		PH_analyze_packet(&pckt);
		count = 0;
		memset(pckt.packet, 0x00, TCP_MAX_LEN);
	}
}


void EKF_CLASS::reset(uint8_t in_refdim, uint8_t in_Pdim, uint8_t in_Qdim, uint8_t in_Rdim)
{
	//初期状態量
	for (int i = 0; i < in_refdim; i++) {
		X_ref[i] = ini_X_ref[i];
	}

	for (int i = 0; i < in_Pdim; i++) {
		for (int j = 0; j < in_Pdim; j++) {
			P[i*in_Pdim+j] = (i == j) ? ini_P_diag[i] : 0.0f;
		}
	}


	//システムノイズの共分散行列
	Qdim = in_Qdim;
	Calc_Q();

    // 観測ノイズの共分散行列
	Rdim = in_Rdim;
    Calc_R();
}



