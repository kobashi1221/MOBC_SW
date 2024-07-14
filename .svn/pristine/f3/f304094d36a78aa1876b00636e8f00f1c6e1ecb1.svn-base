/*
 * EarthVector.h
 *
 *  Created on: 2021/11/14
 *      Author: masud
 */

#ifndef SRC_USER_DRIVERS_AOCS_EARTHVECTOR_H_
#define SRC_USER_DRIVERS_AOCS_EARTHVECTOR_H_

#include <stdint.h> //for uint8_t
#include <src_core/System/TImeManager/time_manager.h>//for delay()


//-----------------------------------------
// クラス宣言
//-----------------------------------------
class EARTHVECTOR_CLASS
{
private:
	//-----------------------------------------
	// ADCS　計算関数宣言
	//-----------------------------------------
	void EarthCenterVector_Body_calc(const float *in_DCM, const float in_GST_rad, const float *in_ECI_Position, float *out_EarthCenterVector_Body);
	void EarthPointVector_Body_calc(const float *in_DCM, const float in_GST_rad, const float *in_ECI_Position, const float *in_ECEF_Position, float *out_EarthPointVector_Body);

	// 実際にADCS系の計算を行う関数を宣言
	// 入出力は全て引数として定義（出力引数はポインタ渡し）


public:
	//-----------------------------------------
	// 衛星ソフトウェア依存変数宣言
	//-----------------------------------------

	//目標座標（ECEF座標）　名大工学2号館座標
	float Target_Position[3];
	static constexpr float Target_Position_[3] = {-3815854.726f, 3562714.999f, 3651994.121f};

	//目標座標（緯度、経度、高度）　名大工学2号館座標
	float Target_Phi;
	static constexpr float Target_Phi_ = 35.155353593535494f;
	float Target_Lam;
	static constexpr float Target_Lam_ = 136.9646694399292f;
	float Target_Height;
	static constexpr float Target_Height_ = 0.0f;

	// 主には出力ベクトル、配列を宣言

	//Magnaroでは、Mramへの値格納、読み出しのため、ROMに格納する初期値（プログラムベタ打ちの値（_あり配列））と
	//RAMに展開される実際の計算に使う値(_なし配列)の二つの値を宣言しておき、プログラム起動時に_あり配列から_なし配列にコピーする。
	//さらに、MRAMに格納した値を使用する場合は、MRAMの値を_なしの配列にコピーする。
	//メンバ定数にするにはstatic constが便利なので今回はこれを利用する



	float EarthVector_ICRF[3]; //[0]:x [1]:y [2]:z
	float EarthVector_Body[3]; //[0]:x [1]:y [2]:z

	float EarthVector_Point_ICRF[3]; //[0]:x [1]:y [2]:z
	float EarthVector_Point_Body[3]; //[0]:x [1]:y [2]:z


	//-----------------------------------------
	// 衛星ソフトウェア依存関数宣言
	//-----------------------------------------

	void EarthVector_calc(const float *in_DCM, const float in_GST_rad, const float *in_ECI_Position, const float *in_ECEF_Position);

};




#endif /* SRC_USER_DRIVERS_AOCS_EARTHVECTOR_H_ */
