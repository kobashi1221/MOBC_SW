/*
 * ProjectedArea.cpp
 *
 *  Created on: 2022/07/19
 *      Author: Kanie
 */

#include <stdio.h>
#include <math.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/ProjectedArea.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //for uint8_t

#include <src_user/Applications/Userdefined/AOCS/Global_AOCS_Parameter.h>


//======MRAM格納パラメータの実体宣言部======//


//======計算部======//

/**
  * @brief  投影面積を計算する
  * @param  in_vector_b float   投影方向ベクトル（機体座標）
  * @param  SAT char      		衛星ID（1U, 2U, 3U）
  */
float PROJECTEDAREA_CLASS::ProjectedArea(float *in_vector_b, char *SAT)
{
	float Vb[3] = {0.0f};		// 投影方向ベクトル（機体座標）
	float cosX[6] = {0.0f};		// 投影方向と各面の法線ベクトルとのなす角のcos
	float Area_SUM = 0.0f;		// 投影面積

	// 各面の情報を持ったオブジェクト
	SURFACE_CLASS surface[6] = {	SURFACE_CLASS(SAT, (char*)"PX"),
									SURFACE_CLASS(SAT, (char*)"MX"),
									SURFACE_CLASS(SAT, (char*)"PY"),
									SURFACE_CLASS(SAT, (char*)"MY"),
									SURFACE_CLASS(SAT, (char*)"PZ"),
									SURFACE_CLASS(SAT, (char*)"MZ"),	};

	// 投影方向ベクトルの正規化
	Normalization(in_vector_b, 3, Vb);

//	printf("Vb: %f, %f, %f\r\n", Vb[0], Vb[1], Vb[2]);

	// 投影面積の計算
	for (int i = 0; i < 6; i++){
		// 法線ベクトルとのなす角のcos
		cosX[i] = surface[i].norm[0] * Vb[0] + surface[i].norm[1] * Vb[1] + surface[i].norm[2] * Vb[2];
//		printf("surface[%d].norm: %f, %f, %f\r\n", i, surface[i].norm[0], surface[i].norm[1], surface[i].norm[2]);

		// 投影方向に向いている面ならば投影面積を足す
		if (cosX[i] > 0){
			Area_SUM += surface[i].area * cosX[i];
//			printf("surface[%d].area: %f\r\n", i, surface[i].area);
		}
	}

//	printf("Area_SUM: %f\r\n", Area_SUM);

	return Area_SUM;
}


/**
  * @brief  投影面積を計算する
  * @param  in_vector_b float   投影方向ベクトル
  * @param  SAT char      		衛星ID（1U, 2U, 3U）
  */
void PROJECTEDAREA_CLASS::cal(float *in_vector_b, char *SAT)
{
//	printf("before_calc_SAT: %s\r\n", SAT);
	this->OUT_area = ProjectedArea(in_vector_b, SAT);
}

