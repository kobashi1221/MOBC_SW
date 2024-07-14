/*
 * EarthVector.cpp
 *
 *  Created on: 2021/11/14
 *      Author: masud
 */


#include <stdio.h>
#include <math.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/EarthVector.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //for uint8_t



//GPSから得られたECEF座標系における座標から,慣性座標系,機体座標系での地球中心,地球一点方向を計算する

//======MRAM格納パラメータの実体宣言部======//
constexpr float EARTHVECTOR_CLASS::Target_Position_[3];
constexpr float EARTHVECTOR_CLASS::Target_Phi_;
constexpr float EARTHVECTOR_CLASS::Target_Lam_;
constexpr float EARTHVECTOR_CLASS::Target_Height_;

//======計算部======//


void EARTHVECTOR_CLASS::EarthCenterVector_Body_calc(const float *in_DCM, const float in_GST_rad, const float *in_ECI_Position, float *out_EarthCenterVector_Body)
{
//	float ECEF_Position_Norm[3] = {};
	float ECI_Position_Norm[3] = {};
//	float ICRF_Position_Norm[3] = {};
	float Body_Position_Norm[3] = {};

	Normalization(in_ECI_Position, 3, ECI_Position_Norm);
//	Rotate_one_axis(ECEF_Position_Norm, 0, 2, -in_GST_rad, ICRF_Position_Norm);
	multiplication(in_DCM, 3, 3, ECI_Position_Norm, 3, 1, Body_Position_Norm, 3, 1);
	CoeffVec(Body_Position_Norm, 3 , -1.0, out_EarthCenterVector_Body);
}


void EARTHVECTOR_CLASS::EarthPointVector_Body_calc(const float *in_DCM, const float in_GST_rad, const float *in_ECI_Position, const float *in_ECEF_Position, float *out_EarthPointVector_Body)
{
//	float ECEF_Diff[3] = {};
	float ECI_Diff[3] = {};
//	float ECEF_Position_Norm[3] = {};
	float ECI_Position_Norm[3] = {};
//	float ICRF_Position_Norm[3] = {};
	float Body_Position_Norm[3] = {};
	float ECI_Target_Position_Norm[3] = {};

//	Rotate_one_axis(this->Target_Position, 0, 2, -in_GST_rad, ECI_Target_Position_Norm);
	Rotate_one_axis(in_ECEF_Position, 0, 2, -in_GST_rad, ECI_Target_Position_Norm);
	sub(in_ECI_Position, 3, 1, ECI_Target_Position_Norm, 3, 1, ECI_Diff);
	Normalization(ECI_Diff, 3, ECI_Position_Norm);
//	Rotate_one_axis(ECEF_Position_Norm, 0, 2, -in_GST_rad, ICRF_Position_Norm);
	multiplication(in_DCM, 3, 3, ECI_Position_Norm, 3, 1, Body_Position_Norm, 3, 1);
	CoeffVec(Body_Position_Norm, 3 , -1.0f, out_EarthPointVector_Body);
}


/**
  * @param  in_DCM            float   慣性座標系から機体座標系へのDCM
  * @param  in_GST_rad        float   グリニッジ平均恒星時
  * @param  in_ECEF_Position  float   ECEF座標系における座標
  * @return
  */
void EARTHVECTOR_CLASS::EarthVector_calc(const float *in_DCM, const float in_GST_rad, const float *in_ECI_Position, const float *in_ECEF_Position)
{
	EarthCenterVector_Body_calc(in_DCM, in_GST_rad, in_ECI_Position, this->EarthVector_Body);

	EarthPointVector_Body_calc(in_DCM, in_GST_rad, in_ECI_Position, in_ECEF_Position, this->EarthVector_Point_Body);
}

