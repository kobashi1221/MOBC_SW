/*
 * Quest.cpp
 *
 *  Created on: 2022/06/24
 *      Author: Kanie
 *   Reference:
 *      	QUEST法： 『人工衛星の力学と制御ハンドブック』 pp.455-pp.456
 *      	ニュートン法： https://ja.wikipedia.org/wiki/%E3%83%8B%E3%83%A5%E3%83%BC%E3%83%88%E3%83%B3%E6%B3%95
 */

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/Quest.h>
#include <stdlib.h>
#include <string.h>

#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>

#define N 1000      //ニュートン法の繰り返し上限
#define EPS 1.0e-10 //f(λ)推定誤差の閾値

//======MRAM格納パラメータの実体宣言部======//
constexpr float QUEST_CLASS::A_mag_; //磁気センサ重み係数
constexpr float QUEST_CLASS::A_3_;   //3つ目のセンサ重み係数


//-----------------------------------------
// AODCS　計算関数定義
//-----------------------------------------

//ADCS計算関数を定義する
// // 入出力は全て引数として定義（出力引数はポインタ渡し）

/**
 * @brief  Quest計算を行う
 * @param  in_Vb        機体座標系ベクトル格納ポインタ
 * @param  in_Vi        慣性座標系ベクトル格納ポインタ
 * @param  vec_num      リファレンスベクトルの数
 * @param  out_Q        出力Quaternion
 * @param  out_DCM   	出力DCM
 * @return なし
 * 作成者:蟹江 2022/06/24
 * コメント記入者:玉置 2023/06/17
 */
void QUEST_CLASS::Quest(float *in_Vb, float *in_Vi, int vec_num, float out_Q[4], float out_DCM[3][3])
{
	float in_Vi_buf[3][3], in_Vb_buf[3][3];      //機体座標系のベクトル，慣性座標系のベクトルをそれぞれ3種類（地球磁場方向ベクトル，太陽方向ベクトル，3つ目の方向ベクトル）まで格納する配列
	float Vb[3] = {}, Vi[3] = {};                //機体座標系のベクトル，慣性座標系のベクトルを格納する配列
	float Z[3] = {};                             //QUEST法におけるz	ベクトル
	float Z_buf[3][3] = {};                      //zベクトルを3種類まで格納する配列
	float MAT[3][3][3] = {};                     //テンソル積の結果を3種類まで格納する3次元配列
	float B[3][3] = {}, B_TRA[3][3] = {};        //QUEST法におけるB行列とその転置行列
	float S[3][3] = {};                          //QUEST法におけるS行列
	float ZtZ = 0.0f, ZtSZ = 0.0f, ZtS2Z = 0.0f; //ztz,ztSz,ztS^2zの初期値をそれぞれ0.0に設定
	float ZtS[3] = {}, SZ[3] = {}, S2Z[3] = {};  //ztS,Sz,S^2zの計算結果を格納する配列
	float X[3] = {};                             //QUEST法におけるX (出力Quaternionのq1,q2,q3)

	print(QEST,"Number of Vector : %d\r\n", vec_num);

	for (int i = 0; i < vec_num; i++){
		for (int j = 0; j < 3; j++){
			//i(1~3)種類の機体座標系ベクトル，慣性座標系ベクトルをまとめて格納
			in_Vb_buf[i][j] = in_Vb[i * 3 + j];
			in_Vi_buf[i][j] = in_Vi[i * 3 + j];
		}
	}

    ////////////////////////////// S行列，zベクトル，σの計算 //////////////////////////////
	for (int i = 0; i < vec_num; i++){
		for (int j = 0; j < 3; j++){
			//j(1~3)種類目の機体座標系ベクトル，慣性座標系ベクトルを取り出す
			Vb[j] = in_Vb_buf[i][j];
			Vi[j] = in_Vi_buf[i][j];
		}

		float buf1[3][3] = {}, buf2[3] = {}; //テンソル積(3×3の行列)の結果を格納する行列，外積の結果を格納する行列

		//機体座標系ベクトルと慣性座標系ベクトルのテンソル積
		multiplication(Vb, 3, 1, Vi, 1, 3, *buf1, sizeof(buf1) / sizeof(buf1[0]), sizeof(buf1[0]) / sizeof(buf1[0][0]));

		//機体座標系ベクトルと慣性座標系ベクトルの外積
		Outer_product(Vb, Vi, buf2);


		for (int k = 0; k < 3; k++){
			for (int l = 0; l < 3; l++){
				//i(1~3)種類目の機体座標系ベクトルと慣性座標系ベクトルのテンソル積の結果を格納
				MAT[i][k][l] = buf1[k][l];
			}
			//i(1~3)種類目の機体座標系ベクトルと慣性座標系ベクトルの外積の結果を格納
			Z_buf[i][k] = buf2[k];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++){
			for (int k = 0; k < vec_num; k++){
				//B行列を計算
				B[i][j] +=  A[k] * MAT[k][i][j];
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < vec_num; j++){
			//zベクトルを計算
			Z[i] += A[j] * Z_buf[j][i];
		}
	}

	float sigma = 0.0f; //QUEST法におけるσを0.0に設定
	for (int i = 0; i < vec_num; i++) {
		for (int j = 0; j < 3; j++){
			//σ = TrB (B行列の対角成分の和)
			sigma += A[i] * in_Vb_buf[i][j] * in_Vi_buf[i][j];
		}
	}

	//B行列の転置行列Btを計算
	trans(*B, sizeof(B) / sizeof(B[0]), sizeof(B[0]) / sizeof(B[0][0]), *B_TRA, sizeof(B_TRA) / sizeof(B_TRA[0]), sizeof(B_TRA[0]) / sizeof(B_TRA[0][0]));

	//S行列を計算  S = B + Bt
	add(*B, sizeof(B) / sizeof(B[0]), sizeof(B[0]) / sizeof(B[0][0]), *B_TRA, sizeof(B_TRA) / sizeof(B_TRA[0]), sizeof(B_TRA[0]) / sizeof(B_TRA[0][0]), *S);
	////////////////////////////// S行列，zベクトル，σの計算 //////////////////////////////

	////////////////////////////// 固有方程式の係数計算 //////////////////////////////
	float det_S = 1.0f; //S行列の行列式の初期値を1.0に設定

	//S行列の行列式を計算
	det_S = determinant(*S, sizeof(S) / sizeof(S[0]));

	//QUEST法におけるκを計算  κ = Tr(adj(S)) (adj(S):S行列の余因子行列　Tr():行列の対角成分の和)
	float kappa = S[1][1]*S[2][2]-S[1][2]*S[2][1]+S[0][0]*S[2][2]-S[0][2]*S[2][0]+S[0][0]*S[1][1]-S[0][1]*S[1][0];

	//S行列の行列式を参考資料に合わせてdeltaと置く
	float delta = det_S;

	//ztベクトルとS行列の掛け算
	multiplication(*S, 3, 3, Z, 3, 1, ZtS, 3, 1);

	//S行列とzベクトルの掛け算
	multiplication(*S, 3, 3, Z, 3, 1, SZ, 3, 1);

	for (int i = 0; i < 3; i++){
		//ztz,ztSz,ztS^2zを計算
		ZtZ += Z[i] * Z[i];
		ZtSZ += ZtS[i] * Z[i];
		ZtS2Z += ZtS[i] * SZ[i];
	}

	// 固有方程式の係数a,b,c,dを計算
	float a = sigma * sigma - kappa;
	float b = sigma * sigma + ZtZ;
	float c = delta + ZtSZ;
	float d = ZtS2Z;
	////////////////////////////// 固有方程式の係数計算 //////////////////////////////

	////////////////////////////// ニュートン法により固有方程式を解く //////////////////////////////
	float lamda = 1.0f; //最大固有値の初期値を1.0に設定
	float f_lamda = 0.0f, f_lamda_prime = 0.0f; //今回計算するλと前回のλをぞれぞれ0.0に設定

	//最大1000回イタレーションを行う
	for (int i = 0; i < N; i++){
		//関数f(λ) = λ^4 - (a+b)λ^2 - cλ +(ab+cσ-d)
		f_lamda = powf(lamda, 4.0) - (a + b) * powf(lamda, 2.0) - c * lamda + (a * b + c * sigma - d);

		//f(λ)の絶対値が1e-10より小さくなった場合
		if (fabs(f_lamda) < EPS){
		    //最大固有値をその時点におけるλとしてイタレーションを抜ける
			break;
		}
		//関数f'(λ) = 4λ^3 - 2(a+b)λ - c
		f_lamda_prime = 4 * powf(lamda, 3.0) - 2 * (a + b) * lamda - c;

		//f'(λ)の値が0になった場合
		if (f_lamda_prime == 0){
			//0割りを避けるためf'(λ)の値を0.01に設定
			f_lamda_prime = 0.01;
		}
		//λを更新　λ(n+1) = λ(n) - f(λ(n)) / f'(λ(n))
		lamda = lamda - f_lamda / f_lamda_prime;
	}
	////////////////////////////// ニュートン法により固有方程式を解く //////////////////////////////

	////////////////////////////// Quatenion計算 //////////////////////////////
	float alpha = lamda * lamda - sigma * sigma + kappa; //QUEST法におけるα　α = λ^2 - σ^2 + κ
	float beta = lamda - sigma;                          //QUEST法におけるβ β = λ - σ
	float gamma = (lamda + sigma) * alpha - delta;       //QUEST法におけるγ γ = (λ+σ)α - β

	//S行列とSzベクトルの掛け算
	multiplication(*S, 3, 3, SZ, 3, 1, S2Z, 3, 1);

	for (int i = 0; i < 3; i++){
		//QUEST法におけるX X = αz + βSz +S^2z
		X[i] = alpha * Z[i] + beta * SZ[i] + S2Z[i];
	}

	//出力Quaternionのノルムの逆数を計算 q_norm = 1/√(γ^2 + X^2)
	float q_norm = 1.0 / sqrt(gamma * gamma + Fabs(&X[0], 3));

	if (q_norm < 0.03 || q_norm > 30.0 || isnan(q_norm)){ //q_normが0~30の範囲外，またはNAN値の場合
		//出力Quaternionに前回の出力Quaternionをコピーして計算終了
		copy(OUT_Q_Pre, 4, 1, out_Q);
		return;
	}
	for (int i = 0; i < 3; i++){
		//出力Quaternion(q1,q2,q3)を計算
		out_Q[i] = X[i] * q_norm;
	}
	//出力Quaternion(q1,q2,q3)を計算
	out_Q[3] = gamma * q_norm;

	//正規化
	Normalization(out_Q, 4, out_Q);
	////////////////////////////// Quatenion計算 //////////////////////////////

	/**
	最大固有値を求めるにあたりそれに対応する固有ベクトルが前のクォータニオンと符号が逆転したもの（姿勢表現としては同じものを示す）と
	なることがあるため、連続性を補償する
	前のQuaternionとの内積が負なら全部の符号を逆転させる
	 */
	float dot = 0.0f; //内積の初期値を0.0に設定
	for (int i = 0; i < 4; i++){
		//今回計算したQuaternionと前回計算したQuaternionの内積を計算
		dot += out_Q[i] * OUT_Q_Pre[i];
	}

	if (dot < 0){ //内積が0未満の場合
		//今回計算したQuaternionの符号を逆転させる
		out_Q[0] = -out_Q[0];
		out_Q[1] = -out_Q[1];
		out_Q[2] = -out_Q[2];
		out_Q[3] = -out_Q[3];
	}

	//前回の出力Quaternionに出力Quaternionをコピー
	copy(out_Q, 4, 1, OUT_Q_Pre);

	//QuaternionからDCMへの変換
	Quaternion_To_DCM(out_Q, *out_DCM);
}

//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

/**
 * @brief  Quest.cppのメイン関数
 * @param[in]  in_Vb        機体座標系ベクトル格納ポインタ
 * @param[out]  in_Vi        慣性座標系ベクトル格納ポインタ
 * @param  vec_num      計算に使用するベクトルの種類（e.g.地球磁場方向ベクトル，太陽方向ベクトル）を格納する変数
 * @return  なし
 * 作成者:蟹江 2022/06/24
 * コメント記入者:玉置 2023/06/17
 */
void QUEST_CLASS::cal(float *in_Vb, float *in_Vi, int vec_num)
{
    //変数の初期化
	reset();

	//Quest法の計算
    Quest(in_Vb, in_Vi, vec_num, this->OUT_Q, this->OUT_DCM);
}

/**
 * @brief  変数の初期化を行う
 * @param  なし
 * @return なし
 * 作成者:蟹江 2022/06/24
 * コメント記入者:玉置 2023/06/17
 * TODO:毎回重み付け係数を設定するのは処理として不必要．一度だけ実行するset,get関数を用意する
 * TODO:2種類のベクトルを使用する場合と3種類のベクトルを使用する場合でそれぞれ重み付け行列を用意する
 */
void QUEST_CLASS::reset(void)
{
	//出力Quaternionの初期化
	for (int i = 0; i < 4; i++) {
		OUT_Q[i] = 0.0f;
	}

	//出力DCMの初期化
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			OUT_DCM[i][j] = 0.0f;
	}

	//AOCSフラグ ADSモードにおいてQUEST_CALCに1が立っている場合
	if(ADS_Mode(QUEST_CALC)){

		//磁気センサ重み係数が0~1の範囲外の場合
		if(A_mag > 1 || A_mag < 0){

			//磁気センサ重み係数を0.5に設定
			A_mag = 0.5f;
		}

		//太陽センサ重み係数を計算
		//磁気センサと太陽センサの重み係数の合計は1になる
		A_sun = 1.0f - A_mag;

		//各センサの重み係数を格納する配列
		A[0] = A_mag; //磁気センサの重み係数を格納
		A[1] = A_sun; //太陽センサの重み係数を格納
		A[2] = 0.0f;  //3つ目のセンサは使用しないので0.0に設定
	}
	//AOCSフラグ ADSモードにおいてQUEST_CALCが0の場合
	else{
		// 磁気センサと3つ目のセンサの重み係数が0未満，または2つの合計が1より大きい場合
		if(A_mag + A_3 > 1 || A_mag < 0 || A_3 < 0){

			//磁気センサ重み係数を0.3に設定
			A_mag = 0.3f;

			//3つ目のセンサ重み係数を0.3に設定
			A_3 = 0.3f;
		}

		//太陽センサ重み係数を計算
		//3つのセンサの重み係数の合計は1になる
		A_sun = 1.0f - A_mag - A_3;

		//各センサの重み係数を格納する配列
		A[0] = A_mag; //磁気センサの重み係数を格納
		A[1] = A_sun; //太陽センサの重み係数を格納
		A[2] = A_3;   //3つ目のセンサの重み係数を格納
	}

}
