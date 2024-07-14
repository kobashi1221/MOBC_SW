/*
 * ProjectedArea.h
 *
 *  Created on: 2022/07/19
 *      Author: Kanie
 */

#ifndef C2A_AOCS_PROJECTEDAREA_H_
#define C2A_AOCS_PROJECTEDAREA_H_

#include <stdint.h> //for uint8_t
#include <src_core/System/TImeManager/time_manager.h>//for delay()


//-----------------------------------------
// クラス宣言
//-----------------------------------------
class PROJECTEDAREA_CLASS
{
private:
	//-----------------------------------------
	// ADCS　計算関数宣言
	//-----------------------------------------

	// 実際にADCS系の計算を行う関数を宣言
	// 入出力は全て引数として定義（出力引数はポインタ渡し）

	float ProjectedArea(float *in_vector_b, char *SAT);

public:
	//-----------------------------------------
	// 衛星ソフトウェア依存変数宣言
	//-----------------------------------------

	// 主には出力ベクトル、配列を宣言

	float OUT_area;

	//-----------------------------------------
	// 衛星ソフトウェア依存関数宣言
	//-----------------------------------------
	void cal(float *in_vector_b, char *SAT);
	
};

class SURFACE_CLASS
{
public:
	float norm[3];
	float area;
	float width;	// X
	float length;	// Y
	float height;	// Z

	SURFACE_CLASS(char *sat, char *surface)
	{
		if(sat == (char*)"1U"){
			width = 0.1;
			length = 0.1;
			height = 0.1205;
		}
		if(sat == (char*)"2U"){
			width = 0.1;
			length = 0.1;
			height = 0.220;
		}
		if(sat == (char*)"3U"){
			width = 0.1;
			length = 0.1;
			height = 0.3405;
		}

		if(surface == (char*)"PX"){
			norm[0] = 1;
			norm[1] = 0;
			norm[2] = 0;
			area = length * height;
		}
		if(surface == (char*)"MX"){
			norm[0] = -1;
			norm[1] = 0;
			norm[2] = 0;
			area = length * height;
		}
		if(surface == (char*)"PY"){
			norm[0] = 0;
			norm[1] = 1;
			norm[2] = 0;
			area = width * height;
		}
		if(surface == (char*)"MY"){
			norm[0] = 0;
			norm[1] = -1;
			norm[2] = 0;
			area = width * height;
		}
		if(surface == (char*)"PZ"){
			norm[0] = 0;
			norm[1] = 0;
			norm[2] = 1;
			area = width * length;
		}
		if(surface == (char*)"MZ"){
			norm[0] = 0;
			norm[1] = 0;
			norm[2] = -1;
			area = width * length;
		}
	}
};



#endif /* C2A_AOCS_PROJECTEDAREA_H_ */
