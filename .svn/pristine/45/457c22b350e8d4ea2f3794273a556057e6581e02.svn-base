/*
 * SunVector.cpp
 *
 *  Created on: 2021/06/13
 *      Author: masud
 */

#include <stdio.h>
#include <math.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/SunVector.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //for uint8_t

#include <src_user/Applications/Userdefined/AOCS/Global_AOCS_Parameter.h>



//慣性座標系での太陽方向を計算する
//以下の二つのサイトを参考にするが、太陽方向計算自体は上のサイト参照。サイトは長沢工「日の出・日の入りの計算」を参考にしている。
//http://seppina.cocolog-nifty.com/blog/2017/06/post-0be2.html
//http://omatsuja2.blogspot.com/2021/02/calcLongitude.html

//======MRAM格納パラメータの実体宣言部======//
constexpr float SUNVECTOR_CLASS::SunCoef_[18][3];

//======計算部======//


/**
  * @brief  UTCから太陽黄経を計算する
  * 参考元　http://seppina.cocolog-nifty.com/blog/2017/06/post-0be2.html
  * 長沢工「日の出・日の入りの計算」を元に計算を行う。誤差は2020年で2aecsec程度だと考えられ、十分。
  * @param  in_julian_date  double  修正ユリウス日　秒単位を表すには、精度としてdoubleが必要。
  * @return  lambda         double  太陽黄経(rad)　精度としてfloatだとギリギリ1arcsec程度なので、高精度を求めるならdoubleにすべき
  */
float SUNVECTOR_CLASS::SunLambda_calc(double in_modified_julian_date)
{
    //まずは（J2000基準の）暦表時ETを求める
    //ET≒TT＝UT＋ΔT≒UTC＋ΔTであり、ΔTは時間変化する値である。
    //しかし、ΔTの25秒が太陽黄経1秒角に相当するとともに、
    //ΔTはこの20年で変化が10秒未満である。
    //よってΔTを逐次計算する必要はなく、2000年のΔTと現在（2021)のΔTの差を6秒の固定値として
    //計算してしまっても問題ないと判断する。

    double julian_date_J2000 = 51544.50000;
    float delta_t = 6.0; //sec

    //パラメータ基準時間はJ2000である
    double t = (in_modified_julian_date - julian_date_J2000 + delta_t / 86400.0) / 365.25;

    //printf("t %lf \r\n", t);

    float stor = (357.538f + 359.991f *t) / 180.0f * M_PI;
	float lambda = 280.4603f + 360.00769f * t;
	lambda = lambda + (1.9146f - 0.00005f * t) * sinf(stor);
	
	for(unsigned char i=0; i<18; i++)
	{
		stor = (this->SunCoef[i][1] + this->SunCoef[i][2] * t) / 180.0f * M_PI;
		lambda += this->SunCoef[i][0] * sinf(stor);
	}
	
	while(lambda >360.0f)
	{
		lambda -= 360.0f;
	}
	while(lambda < 0.00f)
	{
		lambda += 360.0f;
	}
	
	// 単位をradに変換
	lambda = lambda / 180.0f * M_PI;
	return lambda;
}

/**
  * @brief  UTCから太陽方向を計算する。計算結果はクラスのSunVector_ICRFに格納される
  * 参考元　http://seppina.cocolog-nifty.com/blog/2017/06/post-0be2.html, 姿勢ハンドブックP280
  * 長沢工「日の出・日の入りの計算」を元に計算を行う。誤差は2020年で2arecsec程度だと考えられ、十分。
  * @param  in_utc_year     float   utc year
  * @param  in_utc_month    float   utc_month
  * @param  in_utc_day      float   utc_day
  * @param  in_utc_hour     float   utc_hour
  * @param  in_utc_sec      float   utc_sec
  * @return  
  */
void SUNVECTOR_CLASS::SunVector_calc(double in_modified_julian_date)
{
    float epsilon = 23.43929111f / 180.0f * M_PI; //平均黄道傾斜角
    float lambda = this->SunLambda_calc(in_modified_julian_date);

    //慣性座標系に変換
    this->SunVector_ICRF[0] = cos(lambda);
    this->SunVector_ICRF[1] = sinf(lambda) * cosf(epsilon);
    this->SunVector_ICRF[2] = sinf(lambda) * sinf(epsilon);

//    for (int i = 0; i < 3; i++){
//    	printf("SunVector_ICRF[%d]:%f\r\n", i, SunVector_ICRF[i]);
//    }

}
