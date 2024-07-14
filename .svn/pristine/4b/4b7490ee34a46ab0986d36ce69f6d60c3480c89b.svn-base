/*
 * JulianDate.cpp
 *
 *  Created on: 2021/07/11
 *      Author: inakawa
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/JulianDate.h>
#include <stdint.h> //for uint8_t


//基準時刻がJSTになる場合は以下の関数内の引数にオフセット持たす形になる

/**
  * @brief  UTCから修正ユリウス日を計算する
  * 参考元　姿勢ハンドブック　P65
  * @param  in_utc_year     float   utc year
  * @param  in_utc_month    float   utc_month
  * @param  in_utc_day      float   utc_day
  * @param  in_utc_hour     float   utc_hour
  * @param  in_utc_min      float   utc_min
  * @param  in_utc_sec      float   utc_sec
  * @return  double  修正ユリウス日　分秒まで含めるとfloatでは桁が足りない
  */
double JULIANDATE_CLASS::juliandate(float in_utc_year, float in_utc_month, float in_utc_day, float in_utc_hour, float in_utc_min, float in_utc_sec)
{
    double juliandate_temp = 0.0;

    //姿勢ハンドブックに誤植があるとともに、ほかの資料と異なる計算方法が表記されている（答えが一致しない）ため、
    //以下のサイトを参考にすることとした。
    //http://astronomy.webcrow.jp/time/gregoriancalendar-julianday.html
    //修正ユリウス日として2400000.5を引いた値を出力する。1721014 - 2400000.5 = -678986.5

    //1,2月は前年の13,14月として計算する
    if(in_utc_month < 3) {
    	in_utc_year -= 1;
    	in_utc_month += 12;
    }

    juliandate_temp = int(365.25 * in_utc_year) + int(in_utc_year / 400.0) - int(in_utc_year / 100.0) + int(30.59 * (in_utc_month - 2.0)) + in_utc_day + in_utc_hour / 24.0 + in_utc_min / 1440.0 + in_utc_sec / 86400.0 - 678912;

    return juliandate_temp;
}

/**
  * @brief  UTCから修正ユリウス日を計算し、値をpublic変数に格納する
  * 参考元　姿勢ハンドブック　P65
  * @param  in_utc_year     float   utc year
  * @param  in_utc_month    float   utc_month
  * @param  in_utc_day      float   utc_day
  * @param  in_utc_hour     float   utc_hour
  * @param  in_utc_sec      float   utc_sec
  */
void JULIANDATE_CLASS::juliandate_calc(float in_utc_year, float in_utc_month, float in_utc_day, float in_utc_hour, float in_utc_min, float in_utc_sec)
{
    this->ModifiedJulianDate = this->juliandate(in_utc_year, in_utc_month, in_utc_day, in_utc_hour, in_utc_min, in_utc_sec);
}
