/*
 * AOCSVector.h
 *
 *  Created on: 2022/07/03
 *      Author: Noro
 */

#ifndef SRC_USER_DRIVERS_AOCS_AOCSVECTOR_H_
#define SRC_USER_DRIVERS_AOCS_AOCSVECTOR_H_

#include <stdio.h>
#include <stdint.h>
typedef double AOCStype;

#define MAX_AOCS_ARRAY 6
//　構造体で可変長配列を定義できなかったので大きめの箱を取るしかなさそう
//　ただの変数をクラスで作るのはコスト的に無駄な気がする

typedef struct{
	double arr[MAX_AOCS_ARRAY];  //AOCSのベクトル配列の箱
	uint8_t n;		//ベクトルのサイズ
} AOCSVector;

typedef struct{
	double arr[MAX_AOCS_ARRAY][MAX_AOCS_ARRAY];  //AOCSの配列
	uint8_t n;		//行列の縦サイズ
	uint8_t m;		//行列の横サイズ
} AOCSMatrix;

AOCSVector AOCSVectorHelper(uint8_t n);
AOCSMatrix AOCSMatrixHelper(uint8_t n, uint8_t m);
AOCSVector AOCSVecCopy(AOCSVector* a);
AOCSVector AOCSVecRetrieve(AOCSVector* a,uint8_t index, uint8_t length);
void AOCSVecCopy_(AOCSVector* a,AOCSVector* b);

double AOCSNorm(AOCSVector* a);
AOCSVector AOCSVecNormalize(AOCSVector* a);
int AOCSIsInforNan(AOCSVector* a);

void AOCSVecReset(AOCSVector* a);
double AOCSVecMax(AOCSVector*);
void AOCSVecUpdate(AOCSVector*a, AOCSVector* b);
void AOCSVecFourCalculus(AOCSVector*, uint8_t num, ...);
void Vecmul(AOCSVector* ret, float a, AOCSVector* b);

//行列計算
void AOCSMatScale(AOCSMatrix*a, AOCSMatrix* b, AOCStype k);
void AOCSMatSum(AOCSMatrix*a, AOCSMatrix* b, AOCSMatrix* c);
AOCSMatrix AOCSMatEye(uint8_t a);
AOCSMatrix AOCSTranspose(AOCSMatrix*a);
void AOCSMatFourCalculus(AOCSMatrix*, uint8_t num, ...);
void AOCSMatReset(AOCSMatrix* a);
void AOCSMatUpdate(AOCSMatrix*a, AOCSMatrix* b);

#endif /* SRC_USER_DRIVERS_AOCS_AOCSVECTOR_H_ */
