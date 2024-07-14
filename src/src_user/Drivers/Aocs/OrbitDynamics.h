/*
 * Integrater.h
 *
 *  Created on: 2022/07/03
 *      Author: Noro
　　  被積分関数を渡して実行する形式の積分ソルバーを複数示す．
 */

#ifndef C2A_AOCS_ORBITDYNAMICS_H_
#define C2A_AOCS_ORBITDYNAMICS_H_

#include <stdarg.h>
#include "Physical_Constants.h"
#include "AOCSVector.h"


typedef struct{
    AOCStype rho_air;//大気密度
    AOCStype rho_air_scale;//大気密度のスケール誤差
    AOCStype B;//弾道係数
    AOCStype B_star;//TLEのBstar
    AOCStype B_scalecoef;//弾道係数のTLEからのスケール誤差
    AOCStype B_SRP;//SRP用の弾道係数
    AOCStype B_SRP_scale;//SRP用の弾道係数のスケール誤差
    const float* e_suns;//太陽方向ベクトル Global_AOCSパラメータとポインタでつなげる
    const uint8_t* Eclipse_flag;//蝕フラグ， Global_AOCSパラメータとポインタでつなげる
}ORBENV_STRUCT;

void OrbitDynamics(AOCSVector*, AOCStype, AOCSVector*, void*);
void OrbitJacobian(AOCSMatrix*, AOCStype, AOCSVector*, void*);
void OrbitSysnoiseJacobian(AOCSMatrix*, AOCStype, AOCSVector*, void*);
void GPSobs(AOCSVector*, AOCStype, AOCSVector*, void*);
void GPSobsJacobian(AOCSMatrix*, AOCStype, AOCSVector*, void*);

void UpdateBfromBstar(ORBENV_STRUCT*);
#endif /* C2A_AOCS_ORBITDYNAMICS_H_ */
