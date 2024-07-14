/*
 * sample_code.c
 *
 *  Created on: 2022/07/06
 *      Author: fujita
 *   Reference: src/src_core/Docs/General/coding_rule.md
 *              https://github.com/ut-issl/c2a-core/blob/develop/Docs/General/coding_rule.md
 */

/**
 * 後で消す
 * 無駄な.hファイルはincludeしない
 * 以下sample_code.cにおけるincludeの順番
 * sample_code.h
 * <空行>
 * C システムヘッダ
 * C++ システムヘッダ
 * <空行>
 * 他のライブラリの.hファイル
 * プロジェクトの.hファイル
 */
#include "sample_code.h"

#include <src_core/TlmCmd/common_cmd_packet_util.h>

/**
 * 後で消す
 * 他のファイルからアクセスされたくないもの(C++でいうprivateのメンバ)はファイル上部でstatic属性として宣言する
 * 逆に他のファイルからアクセスしたいもの(C++でいうpublicのメンバ)は.hファイルで宣言する
 * static属性のものは接尾辞 _ をつける
 */
static void CDH_sample_code_init_(void);
static void CDH_sample_code_update_(void);

/**
 * 後で消す
 * 他のファイルから構造体のメンバが書き換えられるのを防ぐため，
 * 構造体の実体はstatic属性として宣言し，構造体のポインタはconst属性として.hファイルでexternする
 */
static SAMPLE_CODE_STRUCT sample_code_;
const SAMPLE_CODE_STRUCT* const sample_code = &sample_code_;

/**
 * @brief アプリケーション作成関数
 * @param  void
 * @return アプリケーション構造体
 */
AppInfo CDH_sample_code_create_app(void)
{
  return AI_create_app_info("sample_code", CDH_sample_code_init_, CDH_sample_code_update_);
}

/**
 * @brief  App初期化関数sample
 * @param  void
 * @return void
 */
static void CDH_sample_code_init_(void)
{
  // sample_code_構造体の初期化 処理の説明はコードの直上に記述する
  sample_code_.test = 0;
  sample_code_.test_test = 0.0f;
  sample_code_.is_inited = 1;

}

/**
 * @brief  App実行関数sample
 * @param  void
 * @return void
 */
static void CDH_sample_code_update_(void)
{
  float p_Omega_b[3] = {0.0f};         // 機体座標系の角速度[rad/s]
  float p_Mag_fld_i[3] = {0.0f};       // 慣性座標系の地球磁場[T]
  float p_Sun_vec_c[3] = {0.0f};       // センサ座標系の太陽方向ベクトル[-] vecとつけた場合正規化したベクトルを意味する
  float velocity_norm = 0.0f;          // 衛星の速度ノルム[m/s]
  float p_GYRO_DCM_C2B[3][3] = {{0.0f}}; // ジャイロセンサのセンサ座標系から機体座標系へのDCM DCMは負の値も取れるようにuintは使用しない

  uint8_t a = 1; // aの説明
  uint8_t b = 1; // bの説明

  // 意図しない分岐を防ぐため，条件分岐においてif-elseの入れ子は条件ごとに分ける
  // AND(&&)やOR(||)は使用しない
  if (a == 1) { // aが1の場合
  	if (b == 0) {
  	  // hogehoge
  	}
  	else if (b == 1) {
  	  // hugahuga
  	}
  	else { // 何もしない処理
  	  // THROUGH
  	}
  }
  else { // aが1でない場合
	if (b == 0) {
	  // hogehoge
	}
	else if (b == 1) {
	  // hugahuga
	}
	else { //予期しない条件
	  // エラー処理
	}
  }

  for (int i = 0; i < 3; i++) {
	for (int j = 0; j < 3; j++) {
	  // DCMの各要素に0.0を加算
	  p_GYRO_DCM_C2B[i][j] += 0.0f;
	}
  }

  int num = 3; // numの説明
  //numが0になるまでhogehogeの処理を繰り返す
  while ( num > 0) {
	  //hogehoge
	  --num;
  }

  switch (a){
  case SAMPLE_CASE_HOGEHOGE:
	// hogehoge
	// FALL THROUGH breakしない場合
  case SAMPLE_CASE_HUGAHUGA:
	// hugahuga
    break;
  case SAMPLE_CASE_HOGEHUGA:
	// hogehuga
    break;
  default: // 予期しないケース
	// エラー処理
	break;
  }

#ifdef TWO_U //2Uのみで行う処理
  // hogehoge
#endif /*for 2U*/

#ifdef ONE_U //1Uのみで行う処理
  // hugahuga
#endif /*for 1U*/

#ifndef HILS //HILS以外で行う処理
  // hogehuga
#endif /*for other than HILS*/

}

/**
 * @brief  コマンドsample
 * @param[uint8_t] example
 * @return CCP_EXEC_STS
 */
CCP_CmdRet Cmd_CDH_SAMPLE_CODE_TEST(const CommonCmdPacket* packet)
{
  uint8_t example = CCP_get_param_from_packet(packet, 0, uint8_t);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

// 後で消す
// Unexpected EOFのエラーを防ぐため，ファイル末尾には必ず空行を入れる
