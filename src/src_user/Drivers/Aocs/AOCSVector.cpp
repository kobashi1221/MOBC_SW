/*
 * AOCSVector.cpp
 *
 *  Created on: 2022/07/03
 *      Author: Noro
 */


#include "AOCSVector.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

AOCSVector AOCSVectorHelper(uint8_t n){
	AOCSVector ret;
	ret.n = n;
	return ret;
}

AOCSMatrix AOCSMatrixHelper(uint8_t n, uint8_t m){
	AOCSMatrix ret;
	ret.n = n;
	ret.m = m;
	return ret;
}

AOCSVector AOCSVeccopy(AOCSVector* a){
	AOCSVector ret = AOCSVectorHelper(a->n);
	memcpy(ret.arr,a->arr,sizeof(a->arr));
	return ret;
}

void AOCSVecCopy_(AOCSVector* a,AOCSVector* b){
	memcpy(a->arr,b->arr,a->n*sizeof(a->arr[0]));
}

AOCSVector AOCSVecRetrieve(AOCSVector* a,uint8_t index, uint8_t length){
	AOCSVector ret = AOCSVectorHelper(length);
	memcpy(ret.arr,&(a->arr[index]),length*sizeof(a->arr[0]));
	return ret;
}

void AOCSVecReset(AOCSVector* a){
	uint8_t vec_size = a->n;
	for(int i=0;i<vec_size;i++){
		a->arr[i]=0.0;
	}
}

void AOCSVecUpdate(AOCSVector*a, AOCSVector* b){
	memcpy(a->arr,b->arr,b->n*sizeof(b->arr[0]));
}

double AOCSNorm(AOCSVector* a){
	double ret=0;
	for(int i=0;i<a->n;i++){
		ret+=pow(a->arr[i],2);
	}
	ret=sqrt(ret);
	return ret;
}

AOCSVector AOCSVecNormalize(AOCSVector* a){
	double r_norm = AOCSNorm(a);
	AOCSVector ret;
	ret.n = a->n;
	Vecmul(&ret,1/r_norm,a);
	return ret;
}

int AOCSIsInforNan(AOCSVector* a){
	for(int i=0;i<a->n;i++){
		if(isinf(a->arr[i])+isnan(a->arr[i])!=0){
			return true;
		}
	}
	return false;
}


void Vecmul(AOCSVector* ret, float a, AOCSVector* b){
// ベクトルの各要素に係数をかけてretに代入
	uint8_t vec_size = b->n;
	for(int i=0; i<vec_size; i++){
		ret->arr[i] = a*b->arr[i];
	}
}

double AOCSVecMax(AOCSVector* a){
	uint8_t vec_size = a->n;
	double ret = abs(a->arr[0]);
	for(int i=0; i<vec_size; i++){
		if(abs(a->arr[i])>ret){
			ret = abs(a->arr[i]);
		}
	}
	return ret;
}

void AOCSVecFourCalculus(AOCSVector* ret, uint8_t num, ...){
//	四則演算のクラス
//	numでa*AOCSVectorの組がいくつあるかを渡す
//	a*Vec+b*Vec+c*Vec+...って形式
	va_list ap;
	uint8_t num_=2*num;
	va_start(ap, num_);
	AOCSVecReset(ret);
	double coef[num];
	AOCSVector* AOCS_coef[num];
	for(int i=0; i<num; i++){
		coef[i] = va_arg(ap, double);
		AOCS_coef[i] = va_arg(ap, AOCSVector*);
	}
	uint8_t vec_size = ret->n;
	for(int i=0; i<vec_size; i++){
		for(int j=0; j<num; j++){
		ret->arr[i]+=coef[j]*AOCS_coef[j]->arr[i];
		}
	}
}

void AOCSMatScale(AOCSMatrix*a, AOCSMatrix* b, AOCStype k){
    for(int i=0; i < b->n; i++){
        for(int j=0; j < b->m; j++){
            a->arr[i][j] = k * b->arr[i][j];
        }
    }
}

void AOCSMatSum(AOCSMatrix*a, AOCSMatrix* b, AOCSMatrix* c){
    for(int i=0; i < b->n; i++){
        for(int j=0; j < b->m; j++){
            a->arr[i][j] = b->arr[i][j] * c->arr[i][j];
        }
    }
}

AOCSMatrix AOCSMatEye(uint8_t a){
    AOCSMatrix ret = AOCSMatrixHelper(a,a);
    AOCSMatReset(&ret);//0で初期化
    for(int i=0; i < ret.n; i++){
        ret.arr[i][i]=1.0;
    }
    return ret;
}

void AOCSMatFourCalculus(AOCSMatrix* ret, uint8_t num, ...){
    //  四則演算の関数
    //  numでa*AOCSMatrixの組がいくつあるかを渡す
    //  a*Mat+b*Mat+c*Mat+...って形式
    va_list ap;
    va_start(ap, 2*num);
    AOCSMatReset(ret);
    AOCStype coef[num];
    AOCSMatrix* AOCS_coef[num];
    for(int i=0; i<num; i++){
        coef[i] = va_arg(ap, AOCStype);
        AOCS_coef[i] = va_arg(ap, AOCSMatrix*);
    }

    for(int i=0; i<ret->n; i++){
        for(int j=0; j<ret->m;j++){
            for(int k=0; k<num; k++){
                ret->arr[i][j] += coef[k]*AOCS_coef[k]->arr[i][j];
            }
        }
    }
}

void AOCSMatReset(AOCSMatrix* a){
    for(int i=0; i < a->n; i++){
        for(int j=0; j < a->m; j++){
            a->arr[i][j]=0.0;
        }
    }
}

void AOCSMatUpdate(AOCSMatrix*a, AOCSMatrix* b){
    for(int i=0; i < a->n; i++){
        for(int j=0; j < a->m; j++){
            a->arr[i][j]=b->arr[i][j];
        }
    }
}

