/*
 * Bdot.h
 *
 *  Created on: 2021/03/26
 *      Author: masud
 */

#ifndef C2A_AOCS_BDOT_H_
#define C2A_AOCS_BDOT_H_

#include <stdint.h> //for uint8_t
#include <src_core/System/TImeManager/time_manager.h>//for delay()

//-----------------------------------------
// クラス宣言
//-----------------------------------------
class BDOT_CLASS
{
private:
	//-----------------------------------------
	// ADCS　計算関数宣言
	//-----------------------------------------

	// 実際にADCS系の計算を行う関数を宣言
	// 入出力は全て引数として定義（出力引数はポインタ渡し）
	void Bdot(float in_B_fld_[3], float in_Omega[3], float out_Mag_moment_b_[3]); //Bdot関数
	void B_delta(float in_B_fld[3], float in_B_fld_pre_[3], float out_Mag_moment_b_[3]);

public:
	//-----------------------------------------
	// 衛星ソフトウェア依存変数宣言
	//-----------------------------------------
	float Kb[3]; //Angular Momentum Gain
	float Kbd[3]; //Angular Momentum Gain of delta B
//	static constexpr float Kb_ = 1.0f;
	static constexpr float Kb_[3] = {4.0f, 4.0f, 4.0f};
	static constexpr float Kbd_[3] = {4.0f, 4.0f, 4.0f};


	// 主には出力ベクトル、配列を宣言
	///出力トルク///
	float OUT_torque[3];
	float Pre_Mag[3] = {};
	    //inakawa
		// TRIADによる計算DCMはTRIADクラスの中で格納し、
		// 衛星認識値DCMにTRIADの値を入れたい場合は、
		//　衛星認識値更新コマンドを用いると良いかも（ads.c内で宣言？）


	//-----------------------------------------
	// 衛星ソフトウェア依存関数宣言
	//-----------------------------------------

	// コマンドで呼び出される関数
	// 主にはADCS系の計算を実行し、結果を衛星ソフトウェア依存変数に格納する
	void cal(float in_B_fld[3], float in_Omega[3]);
	void Dcal(float in_B_fld[3], float in_B_fld_pre[3]);
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

#endif /* C2A_AOCS_BDOT_H_ */
