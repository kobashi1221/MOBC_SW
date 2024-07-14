/*
 * GST.cpp
 *
 *  Created on: 2021/07/11
 *      Author: inakawa
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/GST.h>
#include <stdint.h> //for uint8_t


//グリニッジ恒星時を計算する

//-----------------------------------------
// ADCS　計算関数定義
//-----------------------------------------

/**
  * @brief  UTCからグリニッジ恒星時を計算する
  * 慣性座標と地表点の相関を計算するため、J2000.0での平均春分点を基準とした恒星時（GMST）で計算を行う
  * 参考　姿勢ハンドブック　P61　一部誤植があるため、以下のサイトも参考にした
  * https://kotetsu1701.com/blog/greenwich-mean-time/
  * @param  in_modified_julian_date  double  修正ユリウス日(秒単位まで対応する)
  * @return  float  春分点方向と本初子午線のなす角（rad, 0以上2pi未満）
  *
  * 参考文献　ミッション解析と軌道設計の基礎
  */
float GST_CLASS::GST(double in_modified_julian_date)
{
  //2000/1/1 UT1 12:00を基準とした時間（36525day単位）
  double T = (in_modified_julian_date -  51544.50000) / 36525.0;

  //日単位での時角の計算
//  double Tg = (24110.54841 + 8640184.812866 * T + 0.093104 * T * T - 0.0000062 * T * T * T) / 86400.0;
  double Tg = 67310.54841 + (876600.0 * 3600 +8640184.812866)* T + 0.093104 * T * T - 0.0000062 * T * T * T;

  //ラジアン単位に変換する
//  double gst_rad = 2 * M_PI * (Tg - int(Tg));
  double gst_rad = fmod(Tg * M_PI /180 /240, 2 * M_PI);

  if(gst_rad < 0){
	  gst_rad = gst_rad + 2 * M_PI;
  }

  return float(gst_rad);

}

//以下の関数はメンバ変数として計算値を格納しておく必要がない場合不要

/**
  * @brief  UTCからグリニッジ恒星時を計算し、値をpublic変数に格納する
  * 慣性座標と地表点の相関を計算するため、J2000.0の平均春分点を基準とした恒星時で計算を行う
  * 参考　姿勢ハンドブック　P61　一部誤植があるため、以下のサイトも参考にした
  * https://kotetsu1701.com/blog/greenwich-mean-time/
  * @param  in_modified_julian_date  double  修正ユリウス日(秒単位まで対応)
  */
void GST_CLASS::GST_calc(double in_modified_julian_date)
{
  this->gst_rad = this->GST(in_modified_julian_date);
}
