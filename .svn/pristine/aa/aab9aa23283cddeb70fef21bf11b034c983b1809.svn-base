/*
 * Compensation.h
 *
 *  Created on: 2022/03/24
 *      Author: sunaken
 */

#ifndef SRC_USER_DRIVERS_AOCS_COMPENSATION_H_
#define SRC_USER_DRIVERS_AOCS_COMPENSATION_H_


#include <stdint.h> //for uint8_t
#include <src_core/System/TImeManager/time_manager.h>//for delay()

//-----------------------------------------
// クラス宣言
//-----------------------------------------
class COMPENSATION_CLASS
{
private:
	//-----------------------------------------
	// ADCS　計算関数宣言
	//-----------------------------------------

	// 実際にADCS系の計算を行う関数を宣言
	// 入出力は全て引数として定義（出力引数はポインタ渡し）
	void compesation_EarthAngularVelocity(float in_B_fld_[3], float in_Omega[3], float in_pos[3], float in_vel[3], float out_Moment_b_[3]);
	void compesation_Coupling(float in_B_fld_[3], float in_Omega[3], float out_Moment_b_[3]);
	void compesation_RMM(float in_B_fld_[3], float RM_Moment[3], float out_Moment_b_[3]);
	void compesation_ShapeAnisortropy(float in_B_fld_[3], float in_Mag[3][3], float out_Moment_b_[3]);
	void compesation_EddyCurrent(float in_B_fld_[3], float in_EdCurT[3], float out_Moment_b_[3]);
	void compesation_ConstantMagnetic(float in_B_fld_[3], float in_Treq[3], float out_Moment_b_[3]);
public:
	//-----------------------------------------
	// 衛星ソフトウェア依存変数宣言
	//-----------------------------------------

	float Kco_eav[3]; //Gain for Compensation Earth Angular Velocity
	static constexpr float Kco_eav_[3] = {0.0005f, 0.0005f, 0.0001f};

	float Kco_cp[3]; //Gain for Compensation Coupling
	static constexpr float Kco_cp_[3] = {1.0f, 1.0f, 1.0f};

	float Kco_rmm[3]; //Gain for Compensation Residual Magnetic Moment
	static constexpr float Kco_rmm_[3] = {1.0f, 1.0f, 1.0f};

	float Kco_msa[3]; //Gain for Compensation Magnetic Shape Anisotropy
	static constexpr float Kco_msa_[3] = {1.0f, 1.0f, 1.0f};

	float Kco_ec[3]; //Gain for Compensation Eddy Current
	static constexpr float Kco_ec_[3] = {1.0f, 1.0f, 1.0f};

	// 蝕に入った時の制御則の選択フラグ
	// eclipse_mode
 	// 0 : MagMoment指令値を0にする
	// 1 : 切り替えなし（そのまま計算する）
	uint8_t eclipse_mode_eav;	// Compensation Earth Angular Velocity
	static constexpr uint8_t eclipse_mode_eav_ = 1;

	uint8_t eclipse_mode_cp;	// Compensation Coupling
	static constexpr uint8_t eclipse_mode_cp_ = 1;

	uint8_t eclipse_mode_rmm;	// Compensation Residual Magnetic Moment
	static constexpr uint8_t eclipse_mode_rmm_ = 1;

	uint8_t eclipse_mode_msa;	// Compensation Magnetic Shape Anisotropy
	static constexpr uint8_t eclipse_mode_msa_ = 1;

	uint8_t eclipse_mode_ed;	// Compensation Eddy Current
	static constexpr uint8_t eclipse_mode_ed_ = 1;

	uint8_t eclipse_mode_cm;	// Compensation Constant Magnetic
	static constexpr uint8_t eclipse_mode_cm_ = 1;

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
	void Eartcal(float in_B_fld_[3], float in_Omega[3], float in_pos[3], float in_vel[3]);
	void Coupcal(float in_B_fld_[3], float in_Omega[3]);
	void RMMcal(float in_B_fld_[3], float RM_Moment[3]);
	void Shapecal(float in_B_fld_[3], float in_Mag[3][3]);
	void Eddycal(float in_B_fld_[3], float in_EdCurT[3]);
	void Conscal(float in_B_fld_[3], float in_Treq[3]);

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



#endif /* SRC_USER_DRIVERS_AOCS_COMPENSATION_H_ */
