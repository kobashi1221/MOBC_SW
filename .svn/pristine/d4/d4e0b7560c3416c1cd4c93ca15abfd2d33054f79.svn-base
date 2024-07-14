/*
 * IGRF.cpp
 *
 *  Created on: 2021/06/13
 *      Author: masud
 */


/*
 * IGRF.cpp
 *
 *  Created on: 2021/06/05
 *      Author: inakawa
 */
#include <stdio.h>
#include <math.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/IGRF.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //for uint8_t



//IGRF13を用い、測地座標系から地磁場を計算する
//測地点での磁場ベクトル出力となるため、慣性座標系へは変換が必要

//======IGRFパラメータ======//
//参照元　https://www.ngdc.noaa.gov/IAGA/vmod/igrf.html
//2021年現在の最新版IGRF13を使用
//単位はnTに対応
//パラメータは小数点第一位までの提供となっており、精度はfloatで十分

//Magnaroでは、Mramへの値格納、読み出しのため、ROMに格納する初期値（プログラムベタ打ちの値（_あり配列））と
//RAMに展開される実際の計算に使う値(_なし配列)の二つの値を宣言しておき、プログラム起動時に_あり配列から_なし配列にコピーする。
//さらに、MRAMに格納した値を使用する場合は、MRAMの値を_なしの配列にコピーする。

////20210822 inakawa MRAM格納のため.hに移動

////IGRF　gパラメータ
////n=1, m=0に対応する値は配列の[0][0]に存在することに注意
//const float gnm_igrf[13][14] =
//{
//	//{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
//	{-29404.8, -1450.9,    0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{ -2499.6,  2982.0, 1677.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{  1363.2, -2381.2, 1236.2, 525.7,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{   903.0,   809.5,   86.3,-309.4,  48.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{  -234.3,   363.2,  187.8,-140.7,-151.2,  13.5,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{    66.0,    65.5,   72.9,-121.5, -36.2,  13.5, -64.7,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{    80.6,   -76.7,   -8.2,  56.5,  15.8,   6.4,  -7.2,   9.8,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{    23.7,     9.7,  -17.6,  -0.5, -21.1,  15.3,  13.7, -16.5,  -0.3,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{     5.0,     8.4,    2.9,  -1.5,  -1.1, -13.2,   1.1,   8.8,  -9.3, -11.9,   0.0,   0.0,   0.0,   0.0},
//	{    -1.9,    -6.2,   -0.1,   1.7,  -0.9,   0.7,  -0.9,   1.9,   1.4,  -2.4,  -3.8,   0.0,   0.0,   0.0},
//	{     3.0,    -1.4,   -2.5,   2.3,  -0.9,   0.3,  -0.7,  -0.1,   1.4,  -0.6,   0.2,   3.1,   0.0,   0.0},
//	{    -2.0,    -0.1,    0.5,   1.3,  -1.2,   0.7,   0.3,   0.5,  -0.3,  -0.5,   0.1,  -1.1,  -0.3,   0.0},
//	{     0.1,    -0.9,    0.5,   0.7,  -0.3,   0.8,   0.0,   0.8,   0.0,   0.4,   0.1,   0.5,  -0.5,  -0.4},
//
//};
//
////IGRF　gパラメータ永年変化項
////n=1, m=0に対応する値は配列の[0][0]に存在することに注意
//const float gtnm_igrf[8][9] =
//{
//	//{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
//	{   5.7,   7.4,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{ -11.0,  -7.0,  -2.1,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{   2.2,  -5.9,   3.1, -12.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{  -1.2,  -1.6,  -5.9,   5.2,  -5.1,   0.0,   0.0,   0.0,   0.0},
//	{  -0.3,   0.5,  -0.6,   0.2,   1.3,   0.9,   0.0,   0.0,   0.0},
//	{  -0.5,  -0.3,   0.4,   1.3,  -1.4,   0.0,   0.9,   0.0,   0.0},
//	{  -0.1,  -0.2,   0.0,   0.7,   0.1,  -0.5,  -0.8,   0.8,   0.0},
//	{   0.0,   0.1,  -0.1,   0.4,  -0.1,   0.4,   0.3,  -0.1,   0.4},
//};
//
////IGRF　hパラメータ
////n=1, m=1に対応する値は配列の[0][1]に存在することに注意
//const float hnm_igrf[13][14] =
//{
//	//{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
//	{0.0, 4652.5,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{0.0,-2991.6,-734.6,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{0.0,  -82.1, 241.9,-543.4,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{0.0,  281.9,-158.4, 199.7,-349.7,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{0.0,   47.7, 208.3,-121.2,  32.3,  98.9,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{0.0,  -19.1,  25.1,  52.8, -64.5,   8.9,  68.1,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{0.0,  -51.5, -16.9,   2.2,  23.5,  -2.2, -27.2,  -1.8,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{0.0,    8.4, -15.3,  12.8, -11.7,  14.9,   3.6,  -6.9,   2.8,   0.0,   0.0,   0.0,   0.0,   0.0},
//    {0.0,  -23.4,  11.0,   9.8,  -5.1,  -6.3,   7.8,   0.4,  -1.4,   9.6,   0.0,   0.0,   0.0,   0.0},
//	{0.0,    3.4,  -0.2,   3.6,   4.8,  -8.6,  -0.1,  -4.3,  -3.4,  -0.1,  -8.8,   0.0,   0.0,   0.0},
//	{0.0,    0.0,   2.5,  -0.6,  -0.4,   0.6,  -0.2,  -1.7,  -1.6,  -3.0,  -2.0,  -2.6,   0.0,   0.0},
//	{0.0,   -1.2,   0.5,   1.4,  -1.8,   0.1,   0.8,  -0.2,   0.6,   0.2,  -0.9,   0.0,   0.5,   0.0},
//    {0.0,   -0.9,   0.6,   1.4,  -0.4,  -1.3,  -0.1,   0.3,  -0.1,   0.5,   0.5,  -0.4,  -0.4,  -0.6},
//};
//
////IGRF　hパラメータ永年変化項
////n=1, m=1に対応する値は配列の[0][1]に存在することに注意
//const float htnm_igrf[8][9] =
//{
//	//{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
//	{   0.0, -25.9,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{   0.0, -30.2, -22.4,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{   0.0,   6.0,  -1.1,   0.5,   0.0,   0.0,   0.0,   0.0,   0.0},
//	{   0.0,  -0.1,   6.5,   3.6,  -5.0,   0.0,   0.0,   0.0,   0.0},
//	{   0.0,   0.0,   2.5,  -0.6,   3.0,   0.3,   0.0,   0.0,   0.0},
//	{   0.0,   0.0,  -1.6,  -1.3,   0.8,   0.0,   1.0,   0.0,   0.0},
//	{   0.0,   0.6,   0.6,  -0.8,  -0.2,  -1.1,   0.1,   0.3,   0.0},
//	{   0.0,  -0.2,   0.6,  -0.2,   0.5,  -0.3,  -0.4,   0.5,   0.0},
//};
//
////ルジャンドル陪関数　擬似正規化係数
//const float legendre_const[14][14] =
//{
//    {1.0,  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//    {1.0,  1.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//    {1.0,  0.577350269189626,   0.288675134594813,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//    {1.0,  0.408248290463863,   0.129099444873581,   0.052704627669473,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//    {1.0,  0.316227766016838,   0.074535599249993,   0.0199204768222399,   0.00704295212273764,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//    {1.0,  0.258198889747161,   0.0487950036474267,  0.00996023841111995,  0.00234765070757921,   0.000742392338645623,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//    {1.0,  0.218217890235992,   0.0345032779671177,  0.00575054632785295,  0.00104990131391452,   0.000223839712229272,   0.0000646169590554494,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//    {1.0,  0.188982236504614,   0.025717224993682,   0.00363696483726654,  0.000548293079133141,  0.0000913821798555235,  0.0000179215199337677,   0.00000478972767445702,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
//    {1.0,  0.166666666666667,   0.0199204768222399,  0.00245204119306875,  0.000316557156832328,  0.0000438985792528482,  0.00000677369783729086,  0.00000123670236773605,   0.000000309175591934014,   0.0,   0.0,   0.0,   0.0,   0.0},
//    {1.0,  0.149071198499986,   0.0158910431540932,  0.00173385495536766,  0.000196320414650061,  0.0000234647776186144,  0.00000302928976464514,  0.000000437240315267812,  0.0000000749860954356942,  0.000000017674392192427,   0.0,   0.0,   0.0,   0.0},
//    {1.0,  0.134839972492648,   0.012974982402692,   0.00127230170115096,  0.000128521880085575,  0.0000135473956745817,  0.00000151464488232257,  0.000000183677671621093,  0.0000000249953651452314,  0.00000000405478365554177,   0.000000000906677188784648,   0.0,   0.0,   0.0},
//    {1.0,  0.123091490979333,   0.0107958379271883,  0.000961769683968529, 0.0000877971585056964, 0.00000829605168657828, 0.000000821431519387298, 0.0000000865864847705541, 0.00000000993215097345444, 0.00000000128223517707356,   0.000000000197853183261687,   0.0000000000421824404046294,   0.0,   0.0},
//    {1.0,  0.11322770341446,    0.00912414835752265, 0.000744983593779457, 0.0000620819661482881, 0.00000532348467864827, 0.0000004742537088391,   0.0000000444179294788697, 0.0000000444179294788697,  0.000000000484639342976502,  0.0000000000596549793142217,  0.00000000000879564684446878,   0.00000000000179540389388913,   0.0},
//    {1.0,  0.104828483672192,   0.00781345384897492, 0.000588961243950183, 0.0000451712653897785, 0.00000354898978576551, 0.000000287861083312721, 0.0000000243287019332506, 0.00000000216737303093259, 0.000000000206650910193405,  0.0000000000215448467266695,  0.00000000000253908453667547,   0.000000000000359080778777825,  0.0000000000000704215345363163},
//
//};

//======MRAM格納パラメータの実体宣言部======//
constexpr float IGRF_CLASS::gnm_igrf_[13][14];
constexpr float IGRF_CLASS::gtnm_igrf_[8][9];
constexpr float IGRF_CLASS::hnm_igrf_[13][14];
constexpr float IGRF_CLASS::htnm_igrf_[8][9];
constexpr float IGRF_CLASS::legendre_const_[14][14];


//======計算部======//

//======測地系変換======//
//参考アルゴリズム:https://www.gsj.jp/data/openfile/no0614/igrf.5.pdf

/**
  * @brief  緯度、高度から地心距離、地心緯度を計算(WGS84測地モデル)
  * @param  in_rat     float  緯度(rad)
  * @param  in_h       float  高度(km)
  * @param  out_theta  float  地心余緯度(rad)
  * @param  out_r      float  地心距離(km)
  */
static void calc_colat(float in_lat, float in_h, float *out_theta, float *out_r)
{
    const float re = 6378.13700; //km
    const float re2 = re * re;
    const float re4 = re2 * re2;
    const float rp = 6356.75231; //km
    const float rp2 = rp * rp;
    const float rp4 = rp2 * rp2;

    float sinlat = sinf(in_lat);
	float sinlat2 = sinlat * sinlat;
    float coslat = cosf(in_lat);
    float coslat2 = coslat * coslat;

	//地心と測位地点間の距離Rmの計算
 	float rm2 = re2 * coslat2 + rp2 * sinlat2;
    float rm = sqrtf(rm2);

    //地心距離
	float r =sqrtf(in_h * in_h + 2.0f * rm * in_h +
	    (re4 * coslat2 + rp4 * sinlat2) / rm2);

    *out_r = r;

    //地心余緯度（北極から測定した緯度）の計算
	*out_theta = acosf(sinlat / r * (rp2/rm + in_h));

}

/**
  * @brief  地心座標系から測地座標系に任意のベクトルを変換する（座標の回転）
  * @param  in_lat     float  測地系緯度(rad)
  * @param  in_theta   float 地心余緯度(rad)
  * @param  in_v       float  地心座標系ベクトル
  * @param  out_v      float  測地系ベクトル
  */
static void conv_geocentric_to_geodetic(float in_lat, float in_theta, float in_v[3], float out_v[3])
{
    float cos_theta = cosf(in_theta);
    float sin_theta = sinf(in_theta);
    float cos_lat = cosf(in_lat);
    float sin_lat = sinf(in_lat);

    out_v[0] =   in_v[0] * (cos_theta * sin_lat + sin_theta * cos_lat)
               - in_v[2] * (cos_theta * cos_lat - sin_theta * sin_lat);

    out_v[1] = in_v[1];

    out_v[2] =   in_v[0] * (cos_theta * cos_lat - sin_theta * sin_lat)
               + in_v[2] * (cos_theta * sin_lat + sin_theta * cos_lat);
}


//======三角関数事前計算======//
/**
  * @brief  三角関数の倍角(sin(mθ),cos(mθ))をあらかじめ計算する
  * @param  in_theta  float    角度(rad)
  * @param  in_max_m  int       最高倍数
  * @param  out_sin_m float    出力sin配列へのポインタ
  * @param  out_cos_m float    出力cos配列へのポインタ
  */
static void calc_trigonometric_f(float in_theta, int in_max_m, float *out_sin_m, float *out_cos_m)
{
	out_sin_m[0] = 0.0f;
	out_cos_m[0] = 1.0f;

	out_sin_m[1] = sinf(in_theta);
	out_cos_m[1] = cosf(in_theta);

    for(int i = 2; i <= in_max_m; i++) {

        //COS(na)=COS((n-1)a)COS(a)-SIN((n-1)a)SIN(a)
        out_cos_m[i] = out_cos_m[i-1] * out_cos_m[1] - out_sin_m[i-1] * out_sin_m[1];

        //COS(na)=SIN((n-1)a)COS(a)+COS((n-1)a)SIN(a)
        out_sin_m[i] = out_sin_m[i-1] * out_cos_m[1] + out_cos_m[i-1] * out_sin_m[1];
    }

}

//======ルジャンドル賠関数陪関数======//
//参考アルゴリズム:https://www.gsj.jp/data/openfile/no0614/igrf.5.pdf

/**
  * @brief  ルジャンドル陪関数を計算する
  * @param  in_theta  float    地心余緯度角度(rad)
  * @param  in_max_n  int       計算するルジャンドル陪関数の次数n
  * @param  out_P float    出力P配列 Pnm=out_P[n][m] 配列サイズはP[14][14]で確保しておく
  * @param  out_Q float    出力Q配列 Qnm=out_Q[n][m] 配列サイズはQ[14][14]で確保しておく
  */
void IGRF_CLASS::calc_legendre_f(float in_theta, int in_max_n, float out_P[14][14], float out_Q[14][14])
{
    int i = 0;
    int j = 0;
    float cos_theta = cosf(in_theta);
    float sin_theta = sinf(in_theta);

    //まずは非正規化ルジャンドル陪関数を計算する
    out_P[0][0] = 1.0f;
    out_P[1][0] = cos_theta;
    out_P[1][1] = sin_theta;

    //P[n][0]とP[n][n]の計算
    for(i = 2; i <= in_max_n; i++) {
        //P[n][0]
        out_P[i][0] = 1.0f / float(i) * ((2.0f * float(i) - 1.0f) * cos_theta * out_P[i-1][0] - (float(i) - 1.0f) * out_P[i-2][0]);

        //P[n][n]
        out_P[i][i] = (2.0f * float(i) - 1.0f) * sin_theta * out_P[i-1][i-1];
    }

    //P[n][m]の計算
    for(i = 2; i<= in_max_n; i++) {
        for(j = 1; j < i; j++) {

            //thetaの値により場合分けすることで、計算速度と精度の両立を可能にしている
            //非極域　測位緯度80deg()以下　地心余緯度で計算するのでsin_thetaが0.175以上になることに注意
            if(sin_theta > 0.175f) {
                out_P[i][j] = 1.0f / sin_theta * ((float(i) + float(j) - 1.0f) * out_P[i-1][j-1] - (float(i) - float(j) + 1.0f) * cos_theta * out_P[i][j-1]);
            } else {
                //極域
                out_P[i][j] = 1.0f / cos_theta * (out_P[i-1][j] + (float(i) - float(j) + 1.0f) * sin_theta * out_P[i][j-1]);
            }
        }
    }

    out_Q[0][0] = 0;

    //Q[n][0]とQ[n][n]の計算
    for(i = 1; i <= in_max_n; i++) {
        //Q[n][0]
        out_Q[i][0] = - out_P[i][1];

        //Q[n][n]
        //参考アルゴリズムの説明文章に誤植あり。sin_theta * out_Q[i-1][i-1]の前の符号は+が正しい
        out_Q[i][i] =  (2.0f * float(i) - 1.0f) * (cos_theta * out_P[i-1][i-1] + sin_theta * out_Q[i-1][i-1]);
    }

    //Q[n][m]の計算
    for(i = 2; i <= in_max_n; i++) {
        for(j = 1; j < i; j++) {
            out_Q[i][j] = 0.5f * ((float(i) + float(j)) * (float(i) - float(j) + 1.0f) * out_P[i][j-1] - out_P[i][j+1]);
        }
    }

    //シュミット擬正規化ルジャンドル陪関数に変換
    for(i = 0; i <= in_max_n; i++) {
        for(j = 0; j <= i; j++) {
        	out_P[i][j] = out_P[i][j] *  legendre_const[i][j];
        	out_Q[i][j] = out_Q[i][j] *  legendre_const[i][j];
        }
    }
}


//======IGRF計算関数======//
//参考アルゴリズム:nanoJasmine MicroBlaze2_OBC搭載ソフト IGRF.cpp

/**
  * @brief  ある時刻におけるGパラメータとHパラメータを計算する
  * @param  in_delta_year float 基準日からの時間（year)
  * @param  in_max_n  int       計算するIGRFの次数n
  * @param  out_g  float    出力g配列(nT)　サイズは[14][14]確保しておく
  * @param  out_h  float    出力h配列(nT)  サイズは[14][14]確保しておく
  */
void IGRF_CLASS::calc_gauss_coeff(float in_delta_year, int in_max_n, float out_g[14][14], float out_h[14][14])
{
    int i, j;
    out_g[0][0] = 0.0f;
    out_h[0][0] = 0.0f;

    //IGRFは13次まで
    if(in_max_n > 13) {
        in_max_n = 13;
    }

    for(i = 1; i <= in_max_n; i++) {
		out_g[0][i] = 0.0f;
		out_h[0][i] = 0.0f;

		for(j=0; j <= i; j++) {

            //IGRFでは永年項は８次まで
			if(i<9) {
			    out_g[i][j] = (j<9)? gnm_igrf[i-1][j] + in_delta_year * gtnm_igrf[i-1][j]:gnm_igrf[i-1][j];
			    out_h[i][j] = (j<9)? hnm_igrf[i-1][j] + in_delta_year * htnm_igrf[i-1][j]:hnm_igrf[i-1][j];
			}else{
			    out_g[i][j] = gnm_igrf[i-1][j];
			    out_h[i][j] = hnm_igrf[i-1][j];
			}
		}
	}
}

/**
  * @brief  IGRFモデルにより磁場を計算する（出力は地心座標系であるため、測地座標系や慣性座標系には変換が必要）
  * @param  in_latitude    float    測地緯度角度(rad)
  * @param  in_longitude   float    測地経度角度(rad)
  * @param  in_altitude    float    高度(km)
  * @param  in_julian_date float    計算する時刻(修正ユリウス日)
  * @param  in_max_n       int       計算するIGRFの次数n
  * @param  out_B[3]       float    出力B配列(nT)
  *                                  測地座標への変換を容易にするため、以下の方向で定義
  *                                  B[0]=B_theta(北向き正）B[1]=B_lambda(東向き正) B[2]=B_r(地心方向正)
  */
void IGRF_CLASS::IGRF(float in_latitude, float in_longitude, float in_altitude, float in_julian_date, int in_max_n, float out_B[3])
{
	//IGRFは13次まで
	if(in_max_n > 13) {
		in_max_n = 13;
	}

	//基準からの時刻を計算
	//IGRF13の基準時刻は2020/1/1（modified jd = 58849.0)である
	float time_from_reference_year = (in_julian_date -  58849.0f) / 365.25f;
	//printf("time from reference %lf \r\n", time_from_reference_year);

	//事前計算する経度の三角倍角関数の格納配列
	//IGRF13は13次まで項が存在するため、確保配列は14要素
	float sin_m[14];
	float cos_m[14];
	calc_trigonometric_f(in_longitude, in_max_n,sin_m, cos_m);

	//地心距離と地心余緯度の実体と計算
	float r, theta;
	calc_colat(in_latitude, in_altitude, &theta, &r);

	//ルジャンドル陪関数の実体と計算
	float P[14][14];
	float Q[14][14];
	calc_legendre_f(theta, in_max_n, P, Q);

	// 磁場ポテンシャル表記に用いるガウス係数
	float gnm[14][14];
	float hnm[14][14];
	calc_gauss_coeff(time_from_reference_year, in_max_n, gnm, hnm);

	float B_r = 0.0f;
	float B_theta = 0.0f;
	float B_lambda = 0.0f;

	float fn_0 = 6371.2f / r; //(a/r)
	float fn = fn_0 * fn_0;  //(a/r)^n

	//not to divide by zero
	 //prevent  1.0f/sin(theta)  from being infinity   _taguchi
    if(theta == 0){
        theta += 0.01;
    }
	float inv_s = 1.0f / sin(theta);     //floatなので0割にはならないはずだが、要動作検証

	for (int n = 1; n <= in_max_n; n++ ) {
		float c1_n = 0.0f;
		float c2_n = 0.0f;
		float c3_n = 0.0f;
		for (int m = 0; m <= n; m++ ) {
			//ガウス係数部分の計算
			float tmp = (gnm[n][m] * cos_m[m] + hnm[n][m] * sin_m[m]);
			c1_n += tmp * P[n][m];
			c2_n += tmp * Q[n][m];
			c3_n +=  m * (gnm[n][m] * sin_m[m] - hnm[n][m] * cos_m[m]) * P[n][m];
		}

		fn *= fn_0;

		B_r -= (n + 1) * c1_n * fn;
		B_theta += c2_n * fn;
		B_lambda += c3_n * fn * inv_s;
	}

	out_B[0] = B_theta;
	out_B[1] = B_lambda;
	out_B[2] = B_r;

    //測地座標での計算
	conv_geocentric_to_geodetic(in_latitude, theta, out_B, this->B_geodetic);


	//debug
	//float B_geodetic[3];//[0]:B_north [1]:B_east [2]:B_down



//	printf("IGRF B_geodetic\r\n");
//	for(int i = 0; i < 3; i++){
//		printf("%f  ", this->B_geodetic[i]);
//	}
//	printf("\r\n");
	//ここでB_geodeticを表示させると、以下のページと比較できます
	//http://wdc.kugi.kyoto-u.ac.jp/igrf/point/index-j.html

}

/**
  * @brief  IGRFモデルにより磁場を計算し、慣性座標系表示ICRFでのBを計算し、構造体のメンバ変数に格納する
  * @param  in_latitude    float    測地緯度角度(rad)
  * @param  in_longitude   float    測地経度角度(rad)
  * @param  in_altitude    float    高度(km)
  * @param  in_julian_date float    計算する時刻(修正ユリウス日)
  * @param  in_max_n       int      計算するIGRFの次数n\
  * @param  in_GST         float    慣性座標系へ変換するために用いる恒星時（rad）
  */
void IGRF_CLASS::IGRF_ICRF(float in_latitude, float in_longitude, float in_altitude, float in_julian_date, int in_max_n, float in_GST)
{
    float B_temp[3] = {0, 0, 0}; // 計算用バッファ nT
    float B_geocent[3] = {0, 0, 0}; // 地心座標系

    this->IGRF(in_latitude, in_longitude, in_altitude, in_julian_date, in_max_n, B_geocent);

    //地心座標系から、局地でのz:地球回転軸、x:赤道垂直方向　y:東向き　に変換
    //一軸回転で計算を行うため変換　x:測地系地心逆方向 y:東向き　z:北向き

    B_temp[0] = - B_geocent[2];
    B_temp[1] = B_geocent[1];
    B_temp[2] = B_geocent[0];

    //緯度分,y座標負方向に座標回転
    Rotate_one_axis(B_temp, 1, 1, in_latitude, B_temp);

    //（経度＋恒星時）分,z座標負方向に座標回転し、慣性座標系に変換
    Rotate_one_axis(B_temp, 1, 2, - (in_longitude + in_GST), this->B_ICRF);
}


/**
  * @brief  IGRFモデルの計算のための初期化を行う
  *         初期化が行われるたびに呼び出されるように配置する必要がある
  * @return err ER error_code
  */
CCP_EXEC_STS  IGRF_CLASS::IGRF_init(void)
{
    //初期化時に読み込まれるパラメータのチェック
    int i = 0;
    int j = 0;

    //パラメータチェック
    //意図せずエラー判定しないように気をつけないと、
    //初期化フラグが一向に立ち上がらず何にも計算しなくなる
    for (i = 0; i < 13; i++) {
        for (j = 0; j < 14; i++) {
            if((gnm_igrf[i][j] < - 35000.0f) || (35000.0f < gnm_igrf[i][j])) {

                //値異常なら初期化フラグを下ろす
                init_flag = 0;
                return CCP_EXEC_SUCCESS;
            }
        }
    }

    //OKなら初期化完了フラグを立てて戻る
    this->init_flag = 1;
    return CCP_EXEC_SUCCESS;
}
