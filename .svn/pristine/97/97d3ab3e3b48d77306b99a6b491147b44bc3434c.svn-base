/*
 * JulianDate.h
 *
 *  Created on: 2021/07/11
 *      Author: inakawa
 */

#ifndef C2A_AOCS_JULIANDATE_H_
#define C2A_AOCS_JULIANDATE_H_

// クラス宣言
//-----------------------------------------
class JULIANDATE_CLASS
{
private:
	//-----------------------------------------
	// ADCS　計算関数宣言
	//-----------------------------------------

	// 実際にADCS系の計算を行う関数を宣言
	// 入出力は全て引数として定義（出力引数はポインタ渡し）

	double juliandate(float in_utc_year, float in_utc_month, float in_utc_day, float in_utc_hour, float in_utc_min, float in_utc_sec);
	


public:
	//-----------------------------------------
	// 衛星ソフトウェア依存変数宣言
	//-----------------------------------------

	// 主には出力ベクトル、配列を宣言

    // 修正ユリウス日
    // 秒単位に対応するためdoubleになっている
	// [！！注意！！]float関数に渡す際はキャストが必要！！！
    double ModifiedJulianDate;


	//-----------------------------------------
	// 衛星ソフトウェア依存関数宣言
	//-----------------------------------------

	void juliandate_calc(float in_utc_year, float in_utc_month, float in_utc_day, float in_utc_hour, float in_utc_min, float in_utc_sec);
};

#endif
