/*
 * AD_EKF.h
 *
 *  ver.1.0 2021/06/21 masuda
 *  ver.2.0 2022/06/30 tamura
 *
 */

#ifndef C2A_AOCS_AD_EKF_H_
#define C2A_AOCS_AD_EKF_H_

#include <stdint.h> //for uint8_t
#include <src_core/System/TImeManager/time_manager.h>//for delay()

//-----------------------------------------
// クラス宣言
//-----------------------------------------
class AD_EKF_CLASS
{
private:
	//-----------------------------------------
	// ADCS　計算関数宣言
	//-----------------------------------------

	float dt = 0.2f;

	float A[6][6] = {};  //非線形状態方程式のヤコビアン　微小状態量システム行列
	float B[6][6] = {};  //微小状態量ノイズ行列
	float F[6][6] = {};  //状態遷移行列
	float Gw[6][6] = {}; //状態遷移行列
	float H[6][6] = {};  //非線形観測方程式のヤコビアン　微小状態量観測行列

	float std_Q1 = 0.0f;  //システムノイズ標準偏差1
	float std_Q2 = 0.001f;  //システムノイズ標準偏差2
//	float std_Q2 = 0.00001f;  //システムノイズ標準偏差2 for test
	float std_Q3 = 0.0001f;  //システムノイズ標準偏差3
	float std_Q4 = 0.0001f;  //システムノイズ標準偏差4
	float std_Q5 = 0.0001f;  //システムノイズ標準偏差5
	float Q[6][6] = {};

	float std_R1 = 0.01f;  //観測ノイズ標準偏差1
	float std_R2 = 0.001f;  //観測ノイズ標準偏差2
//	float std_R1 = 0.00001f;  //観測ノイズ標準偏差1 for test
//	float std_R2 = 0.00001f;  //観測ノイズ標準偏差2 for test
	float std_R3 = 0.0001f;  //観測ノイズ標準偏差3
	float std_R4 = 0.0001f;  //観測ノイズ標準偏差4
	float std_R5 = 0.0001f;  //観測ノイズ標準偏差5
	float R[6][6] = {};

	float y[7] = {};  //　イノベーション、残差、観測時誤差

	float L[3] = {};     //角運動量ベクトル
	float M[3][3] = {};  //[ω×]
	float mM[3][3] = {}; //minusM [×ω]

	// 定常カルマンゲイン用
	float const_P_diag[6] = {0.000015f, 0.000015f, 0.000048f, 0.000017f, 0.000017f, 0.000485f};

	void Calc_A(const float in_X_ref_renew[7]);
	void Calc_B();
	void Calc_Gw();
	void Calc_F();
	void Calc_y(float z[7], float in_X_ref[7]);
	void Calc_H();

	// 実際にADCS系の計算を行う関数を宣言
	// 入出力は全て引数として定義（出力引数はポインタ渡し）
	void Predict(float in_X_ref[7], float in_P_REF[6][6], float out_X_ref_next[7], float out_P_REF_NEXT[6][6]);
	void Observe(float in_Observed[7], float in_X_ref[7], float in_P_REF[6][6], float out_X_ref_renew[7], float out_P_REF_RENEW[6][6]);

//	void Update_P1(); //共分散行列時間更新・・・前半 （計算負荷軽減、一気にやると止まる　とnano-JASMINEより）
//	void Update_P2(); //共分散行列時間更新・・・前半 （計算負荷軽減、一気にやると止まる　とnano-JASMINEより）


public:
	//-----------------------------------------
	// 衛星ソフトウェア依存変数宣言
	//-----------------------------------------


	// 主には出力ベクトル、配列を宣言
	float OUT_DCM[3][3];

	//----事前推定値---------
	float X_ref[7];
	float P_REF[6][6];
	//----------------------

	//----観測更新-----------
	float X_ref_renew[7];
	float P_REF_RENEW[6][6];
	//-----------------------

	//-----推定値------------
	float X_ref_next[7];
	float P_REF_NEXT[6][6];
	//-----------------------

	float Observed_Value[7];

	//-----------------------------------------
	// 衛星ソフトウェア依存関数宣言
	//-----------------------------------------


	// 主にはADCS系の計算を実行し、結果を衛星ソフトウェア依存変数に格納する
	void cal(const float in_Quaternion[4], const float in_Omega[3]);


	// 衛星ソフトウェア依存変数の初期化関数
//	void reset(void);
	void reset(const float in_Quaternion[4], const float in_Omega[3]);

};



#endif /* C2A_AOCS_AD_EKF_H_ */
