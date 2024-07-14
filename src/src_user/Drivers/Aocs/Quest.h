/*
 * Quest.h
 *
 *  Created on: 2022/06/24
 *      Author: Kanie
 */

#ifndef C2A_AOCS_QUEST_H_
#define C2A_AOCS_QUEST_H_

#include <stdint.h> //for uint8_t
#include <src_core/System/TImeManager/time_manager.h>//for delay()


//-----------------------------------------
// クラス宣言
//-----------------------------------------
class QUEST_CLASS
{
private:
	//-----------------------------------------
	// ADCS　計算関数宣言
	//-----------------------------------------

	// 実際にADCS系の計算を行う関数を宣言
	// 入出力は全て引数として定義（出力引数はポインタ渡し）
	void Quest(float *in_Vb, float *in_Vi, int vec_num, float out_Q[4], float out_DCM[3][3]);

public:
	//-----------------------------------------
	// 衛星ソフトウェア依存変数宣言
	//-----------------------------------------

	float A_mag; //磁気センサ重み係数
	static constexpr float A_mag_ = 0.5f;
	float A_sun; //太陽センサ重み係数 磁気センサの重み係数をもとに決定する
	float A_3; //3つ目のセンサ重み係数
	static constexpr float A_3_ = 0.5f;
	float A[3]; //磁気センサ，太陽センサ，3つ目のセンサの重み係数を格納する配列

	// 主には出力ベクトル、配列を宣言
	float OUT_DCM[3][3];//姿勢マトリクス
	float OUT_Q[4]; //クォータニオン
	float OUT_Q_Pre[4] = {0.0f, 0.0f, 0.0f, 1.0f}; //最大固有値計算用リファレンスクォータニオン（初期値を定義）

	float lamda;
	int conv_flag = 0;

	    //inakawa
		// TRIADによる計算DCMはTRIADクラスの中で格納し、
		// 衛星認識値DCMにTRIADの値を入れたい場合は、
		//　衛星認識値更新コマンドを用いると良いかも（ads.c内で宣言？）


	//-----------------------------------------
	// 衛星ソフトウェア依存関数宣言
	//-----------------------------------------

	// コマンドで呼び出される関数
	// 主にはADCS系の計算を実行し、結果を衛星ソフトウェア依存変数に格納する
//	void Addition_Vector(float a[3], float b[3], float A[2], float c[3]);
//	void Addition_matrix_Weight(float a[3][3], float b[3][3], float A[2], float c[3][3]);
//	void Trace_matrix(float a[3][3], float A[2], float sigma);

	void cal(float *in_Vb, float *in_Vi, int vec_num);
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


#endif /* C2A_AOCS_QUEST_H_ */
