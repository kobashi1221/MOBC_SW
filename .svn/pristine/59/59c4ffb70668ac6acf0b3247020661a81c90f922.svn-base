/*
 * CrossProduct.h
 *
 *  Created on: 2021/03/28
 *      Author: masud
 */

#ifndef C2A_AOCS_CROSSPRODUCT_H_
#define C2A_AOCS_CROSSPRODUCT_H_


#include <stdint.h> //for uint8_t
#include <src_core/System/TImeManager/time_manager.h>//for delay()

//-----------------------------------------
// クラス宣言
//-----------------------------------------
class CROSS_PRODUCT_CLASS
{
private:
	//-----------------------------------------
	// ADCS　計算関数宣言
	//-----------------------------------------

	// 実際にADCS系の計算を行う関数を宣言
	// 入出力は全て引数として定義（出力引数はポインタ渡し）
	void cross_product_Quaternion(float in_B_fld_[3], float in_Omega[3], float in_Quaternion_Body[4], const float in_Quaternion_goal_[4], float out_Moment_b_[3]);
	void cross_product_Pointing(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float in_TOme[3], float Kpp[3], float Kpd[3], float out_Moment_b_[3]);
	void cross_product_Pointing_eclipse(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float in_TOme[3], float Kpp[3], float Kpd[3], float out_Moment_b_[3]);
	void cross_product_Angular(float in_B_fld_[3], float in_Omega[3], float in_TH[3], float out_Moment_b_[3]);
	void cross_product_QFB(float in_B_fld_[3], float in_Omega[3], float in_Quaternion_Body[4], const float in_Quaternion_goal_[4], float out_Moment_b_[3]);
	void cross_product_SpinControl(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float Target_omega, float out_Moment_b_[3]);

public:
	//-----------------------------------------
	// 衛星ソフトウェア依存変数宣言
	//-----------------------------------------

	float TargetDirection[3];   //目標ベクトル（地球、太陽等）
	static constexpr float TargetDirection_[3] = {1.0f, 0.0f, 0.0f};
	float TargetQuaternion[4];  //目標クォータニオン
	static constexpr float TargetQuaternion_[4] = {1.0f, 0.0f, 0.0f, 0.0f};
	float TargetH[3];  //目標角運動量ベクトル
	static constexpr float TargetH_[3] = {1.0f, 0.0f, 0.0f};
	float TargetOmeA[3];  //目標角速度ベクトル
	static constexpr float TargetOmeA_[3] = {0.0f, 0.0f, 0.0f};
	float TargetOmeO[3];  //目標角速度ベクトル
	static constexpr float TargetOmeO_[3] = {0.0f, 0.0f, 0.0f};
	float TargetPosition[3];
	static constexpr float TargetPosition_[3] = {0.0f, 0.0f, 100000.0f}; // ECI[km]
	float TargetDirection_Spin[3];  //目標軸指定（慣性座標系）
	static constexpr float TargetDirection_sc[3] = {1.0f, 0.0f, 0.0f};
	float TargetOmega_spinup;	// 目標角速度の大きさ[rad/s]
	static constexpr float TargetOmega_spinup_ = 0.5f;
	float TargetOmega_spindown;	// 一時スピンダウンするときの目標角速度の大きさ[rad/s]
	static constexpr float TargetOmega_spindown_ = 0.0f;
	float TargetOmega_damping;	// タイムアウト後や分離後のダンピング用の目標角速度の大きさ[rad/s]
	static constexpr float TargetOmega_damping_ = 0.0f;

	//ACS
	float ControlDirection[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_[3] = {1.0f, 0.0f, 0.0f};
	float ControlDirection_Sun[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_s[3] = {1.0f, 0.0f, 0.0f};
	float ControlDirection_EarthC[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_ec[3] = {0.0f, 0.0f, 1.0f};
	float ControlDirection_EarthP[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_ep[3] = {0.0f, 0.0f, 1.0f};
	float ControlDirection_Ine[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_i[3] = {1.0f, 0.0f, 0.0f};
	float ControlDirection_Vel[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_v[3] = {1.0f, 0.0f, 0.0f};
	float ControlDirection_M[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_m[3] = {1.0f, 0.0f, 0.0f};
	float ControlDirection_Spin[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_sc[3] = {1.0f, 0.0f, 0.0f};

	//OCS
	float ControlDirection_Ave[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_ave[3] = {1.0f, 0.0f, 0.0f};
	float ControlDirection_Min[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_min[3] = {0.0f, 0.0f, 1.0f};
	float ControlDirection_Max[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_max[3] = {1.0f, 0.0f, 0.0f};
	float ControlDirection_Keep[3];  //制御軸指定（機体座標系）
	static constexpr float ControlDirection_keep[3] = {1.0f, 0.0f, 0.0f};

	//ACS gain
	float Kc[3]; //Angular Momentum Gain
	static constexpr float Kc_[3] = {0.0001f, 0.0001f, 0.0001f};

	float Kpps[3]; //Sun Pointing gain(P)
	static constexpr float Kpp_s[3] = {0.0001f, 0.00001f, 0.00001f};
	float Kpds[3]; //Sun Pointing gain(D)
	static constexpr float Kpd_s[3] = {0.00005f, 0.0005f, 0.0005f};

	// MATLAB検討ゲイン
	float Kppec[3]; //EarthCenter Pointing gain(P)
	static constexpr float Kpp_ec[3] = {0.0001f, 0.0001f, 0.0001f};
	float Kpdec[3]; //EarthCenter Pointing gain(D)
	static constexpr float Kpd_ec[3] = {0.001f, 0.001f, 0.0001f};

	float Kppep[3]; //Earth Point Pointing gain(P)
	static constexpr float Kpp_ep[3] = {0.0001f, 0.0001f, 0.0001f};
	float Kpdep[3]; //Earth Point Pointing gain(D)
	static constexpr float Kpd_ep[3] = {0.001f, 0.001f, 0.0001f};

	float Kppi[3]; //Inertia Pointing gain(P)
	static constexpr float Kpp_i[3] = {0.0001f, 0.0001f, 0.001f};
	float Kpdi[3]; //Inertia Point Pointing gain(D)
	static constexpr float Kpd_i[3] = {0.0005f, 0.0005f, 0.0001f};

	float Kppv[3]; //Velocity Pointing gain(P)
	static constexpr float Kpp_v[3] = {0.001f, 0.0001f, 0.0001f};
	float Kpdv[3]; //Velocity Pointing gain(D)
	static constexpr float Kpd_v[3] = {0.0005f, 0.005f, 0.005f};

	float Kppm[3]; //姿勢トラジェクトリ Pointing gain(P)
	static constexpr float Kpp_m[3] = {0.0001f, 0.0001f, 0.0001f};
	float Kpdm[3]; //姿勢トラジェクトリ Point Pointing gain(D)
	static constexpr float Kpd_m[3] = {0.001f, 0.001f, 0.0001f};

	float Kqp[3]; //Pointing gain(P) @QFB
	static constexpr float Kqp_[3] = {0.0001f, 0.0001f, 0.0001f};
	float Kqd[3]; //Pointing gain(D) @QFB
	static constexpr float Kqd_[3] = {0.0001f, 0.0001f, 0.0001f};

	float Kppsc[3]; //Spin Control gain(P)
	static constexpr float Kpp_sc[3] = {10.0f, 10.0f, 10.0f};
	float Kpdsc[3]; //Spin Control gain(D)
	static constexpr float Kpd_sc[3] = {50.0f, 500.0f, 500.0f};


	//OCS gain
	float Kpave[3]; //平均減速　gain(P)
	static constexpr float Kp_ave[3] = {0.0001f, 0.0001f, 0.0001f};
	float Kdave[3]; //平均減速 gain(D)
	static constexpr float Kd_ave[3] = {0.01f, 0.01f, 0.00001f};

	float Kpmin[3]; //最小減速　gain(P)
	static constexpr float Kp_min[3] = {0.0001f, 0.0001f, 0.0001f};
	float Kdmin[3]; //最小減速 gain(D)
	static constexpr float Kd_min[3] = {0.01f, 0.01f, 0.00001f};

	float Kpmax[3]; //最大減速　gain(P)
	static constexpr float Kp_max[3] = {0.0001f, 0.0001f, 0.0001f};
	float Kdmax[3]; //最大減速 gain(D)
	static constexpr float Kd_max[3] = {0.01f, 0.01f, 0.00001f};

	float Kpkeep[3]; //維持減速　gain(P)
	static constexpr float Kp_keep[3] = {0.0001f, 0.0001f, 0.0001f};
	float Kdkeep[3]; //維持減速 gain(D)
	static constexpr float Kd_keep[3] = {0.01f, 0.01f, 0.00001f};

	// 蝕に入った時の制御則の選択フラグ
	// eclipse_mode
 	// 0 : MagMoment指令値を0にする
	// 1 : ダンピング項のみ計算する	（AngularMomentum, QFB, SpinControlでは指令値0）
	// 2 : 切り替えなし（そのまま計算する）
	// ACS
	uint8_t eclipse_mode_c; //Angular Momentum
	static constexpr uint8_t eclipse_mode_c_ = 2;

	uint8_t eclipse_mode_s; //Sun Pointing
	static constexpr uint8_t eclipse_mode_s_ = 0;

	uint8_t eclipse_mode_ec; //EarthCenter Pointing
	static constexpr uint8_t eclipse_mode_ec_ = 0;

	uint8_t eclipse_mode_ep; //Earth Point Pointing
	static constexpr uint8_t eclipse_mode_ep_ = 0;

	uint8_t eclipse_mode_i; //Inertia Pointing
	static constexpr uint8_t eclipse_mode_i_ = 0;

	uint8_t eclipse_mode_v; //Velocity Pointing
	static constexpr uint8_t eclipse_mode_v_ = 0;

	uint8_t eclipse_mode_m; //Trajectory
	static constexpr uint8_t eclipse_mode_m_ = 0;

	uint8_t eclipse_mode_q; //Pointing @QFB
	static constexpr uint8_t eclipse_mode_q_ = 0;

	uint8_t eclipse_mode_sc; //Spin Control
	static constexpr uint8_t eclipse_mode_sc_ = 2;

	//OCS gain
	uint8_t eclipse_mode_ave; //平均減速
	static constexpr uint8_t eclipse_mode_ave_ = 0;

	uint8_t eclipse_mode_min; //最小減速
	static constexpr uint8_t eclipse_mode_min_ = 0;

	uint8_t eclipse_mode_max; //最大減速
	static constexpr uint8_t eclipse_mode_max_ = 0;

	uint8_t eclipse_mode_keep; //維持減速
	static constexpr uint8_t eclipse_mode_keep_ = 0;

	// スピン制御用パラメータ
	uint8_t timeout_damping;
	static constexpr uint8_t timeout_damping_ = 0x00;

	uint8_t timeout_spindown;
	static constexpr uint8_t timeout_spindown_ = 0x00;

	float control_dir_cor;	// スピンアップ時の制御軸方向ゲイン補正値
	static constexpr float control_dir_cor_ = 0.2f;

	float control_norm_dir_cor;	// スピンアップ時の制御軸直交方向ゲイン補正値
	static constexpr float control_norm_dir_cor_ = 0.002f;

	float Cd_Td_thr;	// 制御軸と目標方向の角度に対する閾値
	static constexpr float Cd_Td_thr_ = 0.2f;

	float B_Cd_thr;	// 磁場と制御軸に対する閾値
	static constexpr float B_Cd_thr_ = 0.9f;

	// 主には出力ベクトル、配列を宣言
	float OUT_Moment_b_[3];
	    //inakawa
		// TRIADによる計算DCMはTRIADクラスの中で格納し、
		// 衛星認識値DCMにTRIADの値を入れたい場合は、
		//　衛星認識値更新コマンドを用いると良いかも（ads.c内で宣言？）


	//-----------------------------------------
	// 衛星ソフトウェア依存関数宣言
	//-----------------------------------------

	// コマンドで呼び出される関数
	// 主にはADCS系の計算を実行し、結果を衛星ソフトウェア依存変数に格納する
	void Quacal(float in_B_fld_[3], float in_Omega[3], float in_Quaternion_Body[4], const float in_Quaternion_goal_[4]);
	void Poincal(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float in_TOme[3], float Kpp[3], float Kpd[3]);
	void Poincal_eclipse(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float in_TOme[3], float Kpp[3], float Kpd[3]);
	void Angcal(float in_B_fld_[3], float in_Omega[3], float in_TH[3]);
	void QFBcal(float in_B_fld_[3], float in_Omega[3], float in_Quaternion_Body[4], const float in_Quaternion_goal_[4]);
	void Spincal(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float Target_omega);
    	//inakawa
		// 呼び出し時にここでの定義名を意識しなくて済むよう、統一された汎用的な名前で宣言する（ポリモーフィズム）
		//（calでいいかは議論してください）

	// 衛星ソフトウェア依存変数の初期化関数
	void reset(void);
    	//inakawa
		// おそらく格納配列の初期化関数はあった方がいいでしょう。
		// 初期化したいところで毎回単位行列生成関数を呼ぶのは非効率なので、
		// クラス変数の編集を楽にする関数はあった方がいいと思います。

		// Tips!
		//　基底ADCSクラスを作り、それを継承させると、上記に書いたことが実現しやすくなります。
};

#endif /* C2A_AOCS_CROSSPRODUCT_H_ */
