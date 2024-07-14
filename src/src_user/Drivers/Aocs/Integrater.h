/*
 * Integrater.h
 *
 *  Created on: 2022/07/03
 *      Author: Noro
　　  被積分関数を渡して実行する形式の積分ソルバーを複数示す．
 */

#ifndef C2A_AOCS_INTEGRATER_H_
#define C2A_AOCS_INTEGRATER_H_

#include <stdint.h> //for uint8_t
#include "AOCSVector.h"
#include <src_core/System/TimeManager/obc_time.h>


typedef struct{
	double h;//初期の刻み時間幅 [s]
	double orbset_h;//刻み時間幅 [s]
	step_t timeout; //タイムアウト時間幅[ms]
	double tol;//許容精度　[-]
	int nmax;//最大積分回数
	int max_try;//誤差を満たすための刻み幅増大の試行回数
} IntegraterOptions;

typedef enum{
	INTEGRATER_SUCCESS,
	INTEGRATER_FAILED,  //MAX_ITERATIONS,
	DYNAMICS_FAILED//0割り対策
}INTEGRATER_ACK;

INTEGRATER_ACK ODE(AOCSVector*, void (*fun)(AOCSVector*, AOCStype, AOCSVector*, void*), AOCSVector*, AOCStype, AOCStype, IntegraterOptions*, void*);
INTEGRATER_ACK RK4(AOCSVector*, void (*fun)(AOCSVector*, AOCStype, AOCSVector*, void* ), AOCSVector*, AOCStype, AOCStype, IntegraterOptions*, void*);
INTEGRATER_ACK DOP45(AOCSVector*, void (*fun)(AOCSVector*, AOCStype, AOCSVector*, void* ), AOCSVector*, AOCStype, AOCStype, IntegraterOptions*, void*);

#endif /* C2A_AOCS_SGP4_H_ */
