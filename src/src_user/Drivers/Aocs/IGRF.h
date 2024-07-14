/*
 * IGRF.h
 *
 *  Created on: 2021/06/13
 *      Author: masud
 */

#ifndef C2A_AOCS_IGRF_H_
#define C2A_AOCS_IGRF_H_

#include <stdint.h> //for uint8_t
#include <src_core/System/TImeManager/time_manager.h>//for delay()


//-----------------------------------------
// クラス宣言
//-----------------------------------------
class IGRF_CLASS
{
private:
	//-----------------------------------------
	// ADCS　計算関数宣言
	//-----------------------------------------

	// 実際にADCS系の計算を行う関数を宣言
	// 入出力は全て引数として定義（出力引数はポインタ渡し）
	// メンバ変数を	用いる必要のある関数もここで定義する
	void calc_legendre_f(float in_theta, int in_max_n, float out_P[14][14], float out_Q[14][14]);
	void calc_gauss_coeff(float in_delta_year, int in_max_n, float out_g[14][14], float out_h[14][14]);
	void IGRF(float in_latitude, float in_longitude, float in_altitude, float in_julian_date, int in_max_n, float out_B[3]);



public:
	//-----------------------------------------
	// 衛星ソフトウェア依存変数宣言
	//-----------------------------------------

	// 主には出力ベクトル、配列を宣言

	//IGRFパラメータ
	//Magnaroでは、Mramへの値格納、読み出しのため、ROMに格納する初期値（プログラムベタ打ちの値（_あり配列））と
	//RAMに展開される実際の計算に使う値(_なし配列)の二つの値を宣言しておき、プログラム起動時に_あり配列から_なし配列にコピーする。
	//さらに、MRAMに格納した値を使用する場合は、MRAMの値を_なしの配列にコピーする。
	//メンバ定数にするにはstatic constexprが便利なので今回はこれを利用する

	//IGRF　gパラメータ
	//20210822 inakawa MRAM格納のため.hに移動
	//n=1, m=0に対応する値は配列の[0][0]に存在することに注意
	float gnm_igrf[13][14];
	static constexpr float gnm_igrf_[13][14] =
	{
		//{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
		{-29404.8, -1450.9,    0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{ -2499.6,  2982.0, 1677.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{  1363.2, -2381.2, 1236.2, 525.7,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{   903.0,   809.5,   86.3,-309.4,  48.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{  -234.3,   363.2,  187.8,-140.7,-151.2,  13.5,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{    66.0,    65.5,   72.9,-121.5, -36.2,  13.5, -64.7,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{    80.6,   -76.7,   -8.2,  56.5,  15.8,   6.4,  -7.2,   9.8,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{    23.7,     9.7,  -17.6,  -0.5, -21.1,  15.3,  13.7, -16.5,  -0.3,   0.0,   0.0,   0.0,   0.0,   0.0},
		{     5.0,     8.4,    2.9,  -1.5,  -1.1, -13.2,   1.1,   8.8,  -9.3, -11.9,   0.0,   0.0,   0.0,   0.0},
		{    -1.9,    -6.2,   -0.1,   1.7,  -0.9,   0.7,  -0.9,   1.9,   1.4,  -2.4,  -3.8,   0.0,   0.0,   0.0},
		{     3.0,    -1.4,   -2.5,   2.3,  -0.9,   0.3,  -0.7,  -0.1,   1.4,  -0.6,   0.2,   3.1,   0.0,   0.0},
		{    -2.0,    -0.1,    0.5,   1.3,  -1.2,   0.7,   0.3,   0.5,  -0.3,  -0.5,   0.1,  -1.1,  -0.3,   0.0},
		{     0.1,    -0.9,    0.5,   0.7,  -0.3,   0.8,   0.0,   0.8,   0.0,   0.4,   0.1,   0.5,  -0.5,  -0.4},
	};

	//IGRF　gパラメータ永年変化項
	//n=1, m=0に対応する値は配列の[0][0]に存在することに注意
	float gtnm_igrf[8][9];
    static constexpr float gtnm_igrf_[8][9] =
	{
		//{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
		{   5.7,   7.4,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{ -11.0,  -7.0,  -2.1,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{   2.2,  -5.9,   3.1, -12.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{  -1.2,  -1.6,  -5.9,   5.2,  -5.1,   0.0,   0.0,   0.0,   0.0},
		{  -0.3,   0.5,  -0.6,   0.2,   1.3,   0.9,   0.0,   0.0,   0.0},
		{  -0.5,  -0.3,   0.4,   1.3,  -1.4,   0.0,   0.9,   0.0,   0.0},
		{  -0.1,  -0.2,   0.0,   0.7,   0.1,  -0.5,  -0.8,   0.8,   0.0},
		{   0.0,   0.1,  -0.1,   0.4,  -0.1,   0.4,   0.3,  -0.1,   0.4},
	};

	//IGRF　hパラメータ
	//n=1, m=1に対応する値は配列の[0][1]に存在することに注意
	float hnm_igrf[13][14];
    static constexpr float hnm_igrf_[13][14] =
	{
		//{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
		{0.0, 4652.5,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{0.0,-2991.6,-734.6,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{0.0,  -82.1, 241.9,-543.4,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{0.0,  281.9,-158.4, 199.7,-349.7,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{0.0,   47.7, 208.3,-121.2,  32.3,  98.9,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{0.0,  -19.1,  25.1,  52.8, -64.5,   8.9,  68.1,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{0.0,  -51.5, -16.9,   2.2,  23.5,  -2.2, -27.2,  -1.8,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{0.0,    8.4, -15.3,  12.8, -11.7,  14.9,   3.6,  -6.9,   2.8,   0.0,   0.0,   0.0,   0.0,   0.0},
	    {0.0,  -23.4,  11.0,   9.8,  -5.1,  -6.3,   7.8,   0.4,  -1.4,   9.6,   0.0,   0.0,   0.0,   0.0},
		{0.0,    3.4,  -0.2,   3.6,   4.8,  -8.6,  -0.1,  -4.3,  -3.4,  -0.1,  -8.8,   0.0,   0.0,   0.0},
		{0.0,    0.0,   2.5,  -0.6,  -0.4,   0.6,  -0.2,  -1.7,  -1.6,  -3.0,  -2.0,  -2.6,   0.0,   0.0},
		{0.0,   -1.2,   0.5,   1.4,  -1.8,   0.1,   0.8,  -0.2,   0.6,   0.2,  -0.9,   0.0,   0.5,   0.0},
	    {0.0,   -0.9,   0.6,   1.4,  -0.4,  -1.3,  -0.1,   0.3,  -0.1,   0.5,   0.5,  -0.4,  -0.4,  -0.6},
	};

	//IGRF　hパラメータ永年変化項
	//n=1, m=1に対応する値は配列の[0][1]に存在することに注意
	float htnm_igrf[8][9];
    static constexpr float htnm_igrf_[8][9] =
    {
		//{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
		{   0.0, -25.9,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{   0.0, -30.2, -22.4,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
		{   0.0,   6.0,  -1.1,   0.5,   0.0,   0.0,   0.0,   0.0,   0.0},
		{   0.0,  -0.1,   6.5,   3.6,  -5.0,   0.0,   0.0,   0.0,   0.0},
		{   0.0,   0.0,   2.5,  -0.6,   3.0,   0.3,   0.0,   0.0,   0.0},
		{   0.0,   0.0,  -1.6,  -1.3,   0.8,   0.0,   1.0,   0.0,   0.0},
		{   0.0,   0.6,   0.6,  -0.8,  -0.2,  -1.1,   0.1,   0.3,   0.0},
		{   0.0,  -0.2,   0.6,  -0.2,   0.5,  -0.3,  -0.4,   0.5,   0.0},
	};

	//ルジャンドル陪関数　擬似正規化係数
	float legendre_const[14][14];
    static constexpr float legendre_const_[14][14] =
    {
	    {1.0,  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	    {1.0,  1.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	    {1.0,  0.577350269189626,   0.288675134594813,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	    {1.0,  0.408248290463863,   0.129099444873581,   0.052704627669473,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	    {1.0,  0.316227766016838,   0.074535599249993,   0.0199204768222399,   0.00704295212273764,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	    {1.0,  0.258198889747161,   0.0487950036474267,  0.00996023841111995,  0.00234765070757921,   0.000742392338645623,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	    {1.0,  0.218217890235992,   0.0345032779671177,  0.00575054632785295,  0.00104990131391452,   0.000223839712229272,   0.0000646169590554494,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	    {1.0,  0.188982236504614,   0.025717224993682,   0.00363696483726654,  0.000548293079133141,  0.0000913821798555235,  0.0000179215199337677,   0.00000478972767445702,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	    {1.0,  0.166666666666667,   0.0199204768222399,  0.00245204119306875,  0.000316557156832328,  0.0000438985792528482,  0.00000677369783729086,  0.00000123670236773605,   0.000000309175591934014,   0.0,   0.0,   0.0,   0.0,   0.0},
	    {1.0,  0.149071198499986,   0.0158910431540932,  0.00173385495536766,  0.000196320414650061,  0.0000234647776186144,  0.00000302928976464514,  0.000000437240315267812,  0.0000000749860954356942,  0.000000017674392192427,   0.0,   0.0,   0.0,   0.0},
	    {1.0,  0.134839972492648,   0.012974982402692,   0.00127230170115096,  0.000128521880085575,  0.0000135473956745817,  0.00000151464488232257,  0.000000183677671621093,  0.0000000249953651452314,  0.00000000405478365554177,   0.000000000906677188784648,   0.0,   0.0,   0.0},
	    {1.0,  0.123091490979333,   0.0107958379271883,  0.000961769683968529, 0.0000877971585056964, 0.00000829605168657828, 0.000000821431519387298, 0.0000000865864847705541, 0.00000000993215097345444, 0.00000000128223517707356,   0.000000000197853183261687,   0.0000000000421824404046294,   0.0,   0.0},
	    {1.0,  0.11322770341446,    0.00912414835752265, 0.000744983593779457, 0.0000620819661482881, 0.00000532348467864827, 0.0000004742537088391,   0.0000000444179294788697, 0.0000000444179294788697,  0.000000000484639342976502,  0.0000000000596549793142217,  0.00000000000879564684446878,   0.00000000000179540389388913,   0.0},
	    {1.0,  0.104828483672192,   0.00781345384897492, 0.000588961243950183, 0.0000451712653897785, 0.00000354898978576551, 0.000000287861083312721, 0.0000000243287019332506, 0.00000000216737303093259, 0.000000000206650910193405,  0.0000000000215448467266695,  0.00000000000253908453667547,   0.000000000000359080778777825,  0.0000000000000704215345363163},

	};
	

	float B_geodetic[3]; //[0]:B_north [1]:B_east [2]:B_down
	float B_ICRF[3]; //[0]:B_x [1]:B_y [2]:B_z //名称はout_Vとすべきか？

	uint8_t init_flag;

	//-----------------------------------------
	// 衛星ソフトウェア依存関数宣言
	//-----------------------------------------
	
	void IGRF_ICRF(float in_latitude, float in_longitude, float in_altitude, float in_julian_date, int in_max_n, float in_GST);
	CCP_EXEC_STS IGRF_init(void);
};


#endif /* C2A_AOCS_IGRF_H_ */
