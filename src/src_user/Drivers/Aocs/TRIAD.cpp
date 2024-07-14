/*
 * TRIAD.cpp
 *
 *  Created on: 2021/03/27
 *      Author: masud
 *      Reference 「人工衛星の力学と制御ハンドブック」P452
 */

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/TRIAD.h>
#include <stdlib.h>
#include <string.h>

#include <src_user/Library/General/DBG_printf.h>


//-----------------------------------------
// AODCS　計算関数定義
//-----------------------------------------

//ADCS計算関数を定義する
// // 入出力は全て引数として定義（出力引数はポインタ渡し）

/**
 * @brief  triad計算を行う
 * @param  in_vb1           機体座標系ベクトル1
 * @param  in_vb2           機体座標系ベクトル2
 * @param  in_vi1           慣性座標系ベクトル1
 * @param  in_vi2           慣性座標系ベクトル2
 * @param  out_Mat_att      出力DCM
 * @param  out_Q            出力クォータニオン
 * @return  なし
 * 作成者　masuda
 * コメント　fujita
 */
void TRIAD_CLASS::triad(const float in_Vb1[3], const float in_Vb2[3], const float in_Vi1[3], const float in_Vi2[3], float out_DCM[3][3], float out_Q[4])
{
    //各々のADCSでの計算に使うベクトル，行列のバッファ
        //おそらくフォルダでのグローバルにする必要はない
	float Bd1[3], Bd2[3], Bd3[3];	//機体座標系ベクトルの格納
    float In1[3], In2[3], In3[3];	//慣性座標系ベクトルの格納
    float MAT_REF_TRA[3][3];		// 基準ベクトルで構成された直交行列
    float Trivec1[3], Trivec2[3];	//外積した結果のベクトル
    float Q_b[4] = {};				//クォータニオンの差分

	///Body frame///
    //観測値ベクトルの代入
	for (int i = 0; i < 3; i++) {
		Bd1[i] = in_Vb1[i];
	}
	//観測値ベクトルの外積
	Outer_product(in_Vb1, in_Vb2, Trivec1);

	Normalization(Trivec1, sizeof(Trivec1) / sizeof(Trivec1[0]), Bd2);

	Outer_product(Bd1, Bd2, Bd3);

	Normalization(Bd3, sizeof(Bd3) / sizeof(Bd3[0]), Bd3);
	// 観測値で構成された直交行列
	float MAT_BD[3][3] = { {Bd1[0], Bd2[0], Bd3[0]},
						   {Bd1[1], Bd2[1], Bd3[1]},
						   {Bd1[2], Bd2[2], Bd3[2]} };

	///Orthogonal frame///
    //基準ベクトルの代入
	for (int i = 0; i < 3; i++) {
		In1[i] = in_Vi1[i];
	}
	//基準ベクトル(機体座標系)の外積
	Outer_product(in_Vi1, in_Vi2, Trivec2);
	//ベクトルの正規化
	Normalization(Trivec2, sizeof(Trivec2) / sizeof(Trivec2[0]), In2);
	//基準ベクトル(慣性座標系)の外積
	Outer_product(In1, In2, In3);
	//ベクトルの正規化
	Normalization(In3, sizeof(In3) / sizeof(In3[0]), In3);
	// 基準ベクトルで構成された直交行列
	float MAT_REF[3][3] = { {In1[0], In2[0], In3[0]},
							{In1[1], In2[1], In3[1]},
							{In1[2], In2[2], In3[2]} };

	///Calculate attitude matrix////
	// 基準ベクトルで構成された直交行列の転置
	trans(*MAT_REF, sizeof(MAT_REF) / sizeof(MAT_REF[0]), sizeof(MAT_REF[0]) / sizeof(MAT_REF[0][0]), *MAT_REF_TRA, sizeof(MAT_REF_TRA) / sizeof(MAT_REF_TRA[0]), sizeof(MAT_REF_TRA[0]) / sizeof(MAT_REF_TRA[0][0]));

	//printf("%d %d %d %d \r\n", sizeof(OUT_Mat_att), sizeof(*OUT_Mat_att), sizeof(OUT_Mat_att[0]), sizeof(OUT_Mat_att[0][0]));
	// 観測値で構成された直交行列と基準ベクトルで構成された直交行列の積
	multiplication(*MAT_BD, sizeof(MAT_BD) / sizeof(MAT_BD[0]), sizeof(MAT_BD[0]) / sizeof(MAT_BD[0][0]), *MAT_REF_TRA, sizeof(MAT_REF_TRA) / sizeof(MAT_REF_TRA[0]), sizeof(MAT_REF_TRA[0]) / sizeof(MAT_REF_TRA[0][0]), *out_DCM, sizeof(*out_DCM) / sizeof(out_DCM[0][0]), sizeof(out_DCM[0]) / sizeof(out_DCM[0][0]));

	DCM_To_Quaternion(*out_DCM, out_Q);

	sub(out_Q, 4, 1, OUT_q_Pre, 4, 1, Q_b);
	//1ステップ前のクォータニオンとの成分の差分が大きいとき符号を反転
	if(fabs(Q_b[0]) > 1.0 || fabs(Q_b[1]) > 1.0 || fabs(Q_b[2]) > 1.0 || fabs(Q_b[3]) > 1.0 ){
		out_Q[0] = -out_Q[0];
		out_Q[1] = -out_Q[1];
		out_Q[2] = -out_Q[2];
		out_Q[3] = -out_Q[3];
	}

	Normalization(out_Q, 4, out_Q);
	//出力されたクォータニオンを保存する
	copy(out_Q, 4, 1, OUT_q_Pre);

}

//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

//入力には接頭in_ 出力はout_を付ける
void TRIAD_CLASS::cal(const float in_Vb1[3], const float in_Vb2[3], const float in_Vi1[3], const float in_Vi2[3])
{
	reset();
    triad(in_Vb1, in_Vb2, in_Vi1, in_Vi2, this->OUT_DCM, this->OUT_q);
}

void TRIAD_CLASS::reset(void)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			OUT_DCM[i][j] = 0.0f;
	}
}

