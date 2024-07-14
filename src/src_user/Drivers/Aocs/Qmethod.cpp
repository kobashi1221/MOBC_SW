/*
 * Qmethod.cpp
 *
 *  Created on: 2021/03/27
 *      Author: masud
 *   Reference:
 *      	Qmethod法： 『人工衛星の力学と制御ハンドブック』 pp.452-pp.454
 *      	べき乗法：https://ja.wikipedia.org/wiki/%E3%81%B9%E3%81%8D%E4%B9%97%E6%B3%95
 */

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/Qmethod.h>
#include <stdlib.h>
#include <string.h>

#define N 1000       //べき乗法の繰り返し上限
#define EPS 1.0e-10  //最大固有値の推定誤差閾値

//======MRAM格納パラメータの実体宣言部======//
constexpr float QMETHOD_CLASS::A_mag_; //磁気センサ重み係数


//-----------------------------------------
// AODCS　計算関数定義
//-----------------------------------------

//ADCS計算関数を定義する
// // 入出力は全て引数として定義（出力引数はポインタ渡し）

/**
 * @brief  Qmethod計算を行う
 * @param  IN_vb1        機体座標系ベクトル1格納ポインタ（配列渡し）
 * @param  IN_vb2        機体座標系ベクトル2格納ポインタ（配列渡し）
 * @param  IN_vi1        慣性座標系ベクトル1格納ポインタ（配列渡し）
 * @param  IN_vi2        慣性座標系ベクトル2格納ポインタ（配列渡し）
 * @param  OUT_q         出力クォータニオン
 * @param  OUT_Mat_att   慣性座標系から機体座標系への出力DCM
 * @return  なし
 * 作成者:増田 2021/03/27
 * コメント記入者:玉置 2023/06/17
 */
void QMETHOD_CLASS::Qmethod(const float in_Vb1[3], const float in_Vb2[3], const float in_Vi1[3], const float in_Vi2[3], float out_Q[4], float out_DCM[3][3])
{
	float Z1[3] = {}, Z2[3] = {};                        //機体座標系ベクトルと慣性座標系ベクトルの外積結果を格納する配列
	float Z[3] = {};                                     //Qmethod法におけるzベクトル
	float Y[4] = {};                                     //K行列とQuaternionの掛け算を格納する配列
	float Q_b[4] = {};                                   //今回の出力Quaternionと前回の出力Quaternionの差を格納する配列
	float MAT1[3][3] = {}, MAT2[3][3] = {};              //機体座標系ベクトルと慣性座標系ベクトルのテンソル積結果を格納する行列
	float B[3][3] = {}, B_TRA[3][3] = {}, S[3][3] = {};  //Qmethod法におけるB行列，B行列の転置行列，S行列
	float d_tmp1 = 0.0f, d_tmp2 = 0.0f;                  //近似固有値の計算に使用する変数をそれぞれ0.0に設定
	float dlamda = 0.0f, d_before = 0.0, sigma = 0.0f;   //近似固有値，前回の近似固有値，Qmethod法におけるσをそれぞれ0.0に設定

	////////////////////////////// K行列の計算 //////////////////////////////
	//1種類目の機体座標系ベクトルと慣性座標系ベクトルのテンソル積
	multiplication(in_Vb1, 3, 1, in_Vi1, 1, 3, *MAT1, sizeof(MAT1) / sizeof(MAT1[0]), sizeof(MAT1[0]) / sizeof(MAT1[0][0]));

	//2種類目の機体座標系ベクトルと慣性座標系ベクトルのテンソル積
	multiplication(in_Vb2, 3, 1, in_Vi2, 1, 3, *MAT2, sizeof(MAT2) / sizeof(MAT2[0]), sizeof(MAT2[0]) / sizeof(MAT2[0][0]));

	//1種類目の機体座標系ベクトルと慣性座標系ベクトルの外積
	Outer_product(in_Vb1, in_Vi1, Z1);

	//2種類目の機体座標系ベクトルと慣性座標系ベクトルの外積
	Outer_product(in_Vb2, in_Vi2, Z2);

	for (int j = 0; j < 3; j++) {
	    //zベクトルを計算
	    Z[j] += A_mag * Z1[j] + A_sun * Z2[j];
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
		    //B行列を計算
			B[i][j] = 0;
			B[i][j] = A_mag * MAT1[i][j] +  A_sun * MAT2[i][j];
		}
	}

	for (int i = 0; i < 3; i++) {
	    //σの計算　σ = TrB (B行列の対角成分の和)
		sigma += A_mag * in_Vb1[i] * in_Vi1[i] + A_sun * in_Vb2[i] * in_Vi2[i];
	}

	//B行列の転置行列Btを計算
	trans(*B, sizeof(B) / sizeof(B[0]), sizeof(B[0]) / sizeof(B[0][0]), *B_TRA, sizeof(B_TRA) / sizeof(B_TRA[0]), sizeof(B_TRA[0]) / sizeof(B_TRA[0][0]));

	//S行列を計算  S = B + Bt
	add(*B, sizeof(B) / sizeof(B[0]), sizeof(B[0]) / sizeof(B[0][0]), *B_TRA, sizeof(B_TRA) / sizeof(B_TRA[0]), sizeof(B_TRA[0]) / sizeof(B_TRA[0][0]), *S);

	//Qmethod法におけるK行列
	float K[4][4] = {{ S[0][0] - sigma,       S[0][1],             S[0][2],            Z[0] },
					 { S[1][0],               S[1][1] - sigma,     S[1][2],            Z[1] },
					 { S[2][0],               S[2][1],             S[2][2] - sigma,    Z[2] },
					 { Z[0],                  Z[1],                Z[2],               sigma } };
	////////////////////////////// K行列の計算 //////////////////////////////

	////////////////////////////// べき乗法によりK行列の最大固有値を求める //////////////////////////////
	//出力Quaternionの初期値を設定(収束性向上のため前回の出力Quaternionの値を使用)
	out_Q[0] = OUT_Q_Pre[0];
	out_Q[1] = OUT_Q_Pre[1];
	out_Q[2] = OUT_Q_Pre[2];
	out_Q[3] = OUT_Q_Pre[3];

	//正規化
	Normalization(out_Q, 4, out_Q);

	//printf("%f\n", d_tmp1);

	//最大1000回イタレーションを行う
	for (int i = 0; i < N; i++) {

	    //K行列とQuaternionの掛け算　Y = K * q (x_k = A * x_k-1) //TODO:OUT_Q→out_Qではないか確認
		multiplication(*K, sizeof(K) / sizeof(K[0]), sizeof(K[0]) / sizeof(K[0][0]), OUT_Q, 4, 1, Y, sizeof(Y) / sizeof(Y[0]), sizeof(Y[0]) / sizeof(Y[0]));

		d_tmp1 = 0.0f; //内積結果を格納する変数を0.0に設定
		d_tmp2 = 0.0f; //内積結果を格納する変数を0.0に設定
		for (int j = 0; j < 4; j++) {
		    //YベクトルとYベクトルの内積を計算 (xt_k * x_k)
			d_tmp1 += Y[j] * Y[j];
			//Yベクトルと出力Quaternionの内積を計算 (xt_k * x_k-1)
			d_tmp2 += Y[j] * out_Q[j];
		}

		//近似固有値dλを計算 (dλ = xt_k * x_k / xt_k * x_k-1)
		dlamda = d_tmp1 / d_tmp2;
		//d_before = 0.0;

		//前回のdλと今回のdλの差の絶対値が1e-10より小さくなった場合
		if (fabsf(d_before - dlamda) < EPS) {
		    //その時点におけるdλを最大固有値λとしてイタレーションを抜ける
			lamda = dlamda;
//			printf("lambda = %f\n", lamda);
			break;
		}
		//前回のdλと今回のdλの差の絶対値が1e-10以上の場合
		else {
		    //前回のdλを今回のdλに更新
			d_before = dlamda;

			d_tmp1 = 0.0f; //内積結果を格納する変数を0.0に設定
			for (int h = 0; h < 4; h++) {
			    //YベクトルとYベクトルの内積を計算
				d_tmp1 += Y[h] * Y[h];
			}
			//ノルムの逆数を計算
			d_tmp1 = 1.0f / sqrt(d_tmp1);
			for (int k = 0; k < 4; k++) {
			    //正規化した値を出力Quaternionに格納(x_k→x_k-1と置き直す)
				out_Q[k] = Y[k] * d_tmp1;
			}
		}
	}
	////////////////////////////// べき乗法によりK行列の最大固有値を求める //////////////////////////////

	/**
	最大固有値を求めるにあたりそれに対応する固有ベクトルが前のクォータニオンと符号が逆転したもの（姿勢表現としては同じものを示す）と
	なることがあるため、連続性を補償する
	4成分のうち一つでも前のクォータニオンとの差の絶対値が1を超えたら全部の符号を逆転させる
	 */
	//TODO:Quaternionの符号反転処理を統一する

	//今回の出力Quaternionと前回の出力Quaternionの引き算　Q_b = out_Q - OUT_Q_Pre
	sub(out_Q, 4, 1, OUT_Q_Pre, 4, 1, Q_b);

	//差の絶対値が1を超える要素がある場合
	if(fabs(Q_b[0]) > 1.0 || fabs(Q_b[1]) > 1.0 || fabs(Q_b[2]) > 1.0 || fabs(Q_b[3]) > 1.0  ){
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
 * @brief  Qmethod.cppのメイン関数
 * @param  in_Vb1[3]        機体座標系ベクトル1格納ポインタ（配列渡し）
 * @param  in_Vb2[3]        機体座標系ベクトル2格納ポインタ（配列渡し）
 * @param  in_Vi1[3]        慣性座標系ベクトル1格納ポインタ（配列渡し）
 * @param  in_Vi2[3]        慣性座標系ベクトル2格納ポインタ（配列渡し）
 * @return  なし
 * 作成者:増田 2021/03/27
 * コメント記入者:玉置 2023/06/29
 */
void QMETHOD_CLASS::cal(const float in_Vb1[3], const float in_Vb2[3], const float in_Vi1[3], const float in_Vi2[3])
{
  //変数の初期化
  reset();

  //Qmethod法の計算
  Qmethod(in_Vb1, in_Vb2, in_Vi1, in_Vi2, this->OUT_Q, this->OUT_DCM);
}

/**
 * @brief  変数の初期化を行う
 * @param   なし
 * @return  なし
 * 作成者:増田 2021/03/27
 * コメント記入者:玉置 2023/06/29
 * TODO:毎回重み付け係数を設定するのは処理として不必要．一度だけ実行するset,get関数を用意する
 */
void QMETHOD_CLASS::reset(void)
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

	//磁気センサ重み係数が0~1の範囲外の場合
	if(A_mag > 1 || A_mag < 0){

	    //磁気センサ重み係数を0.5に設定
		A_mag = 0.5f;
	}

	//太陽センサ重み係数を計算
	//磁気センサと太陽センサの重み係数の合計は1になる
	A_sun = 1.0f - A_mag;
}
