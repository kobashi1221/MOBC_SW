/*
 * Operator.h
 *
 *  Created on: 2022/08/03
 *      Author: Noro
 */

#ifndef SRC_LAFUNCTION_H_
#define SRC_LAFUNCTION_H_

#include <stdio.h>
#include "../AOCSVector.h"

//オペレータを新たに定義する関係で影響域が大きすぎるので，.cファイルでだけインクルードすること
//Namespaceとか使いたいが，今更やりだすと可読性が悪いので，しないこととした
//基本関数
void copy(AOCSMatrix&a, AOCSMatrix&b);
void copy(AOCSVector&a, AOCSVector&b);
AOCSVector copy_(AOCSVector& a);
AOCSMatrix copy_(AOCSMatrix& a);
void partialsubstitute(AOCSMatrix&a, AOCSMatrix& b, uint8_t n,uint8_t m);
void partialsubstitute(AOCSVector&a, AOCSVector& b, uint8_t n);
AOCSMatrix convertVec2Mat(AOCSVector a);
AOCSVector convertMat2Vec(AOCSMatrix a);
AOCSVector similar(AOCSVector& a);
AOCSMatrix similar(AOCSMatrix& a);
void set_size(AOCSVector& a,AOCSVector& b);
void set_size(AOCSVector& a,uint8_t n);
void set_size(AOCSMatrix& a,AOCSMatrix& b);
void set_size(AOCSMatrix& a,uint8_t n, uint8_t m);
void reset(AOCSVector& a);
void reset(AOCSMatrix& a);

void add(AOCSVector& a, AOCSVector& b, AOCSVector& c);
void add(AOCSMatrix& a, AOCSMatrix& b, AOCSMatrix& c);
void pull(AOCSVector& a, AOCSVector& b, AOCSVector& c);
void pull(AOCSMatrix& a, AOCSMatrix& b, AOCSMatrix& c);

void mul(AOCSVector& a, AOCSMatrix& b, AOCSVector& c);
void mul(AOCSVector& a, AOCStype b, AOCSVector& c);
void mul(AOCSMatrix& a, AOCSMatrix& b, AOCSMatrix& c);
void mul(AOCSMatrix& a, AOCStype b, AOCSMatrix& c);
AOCStype det(AOCSMatrix& a);
bool inverse(AOCSMatrix& A, AOCSMatrix& inverse);
AOCSMatrix Transpose(AOCSMatrix& a);
void Transpose(AOCSMatrix& a, AOCSMatrix& b);

//EKFで使う少し特別な関数
void MatExp(AOCSMatrix& M, AOCSMatrix& A, AOCStype dt);
/****************************************/
//オペレータの関数定義
//便利だけど，毎回中で返り値を生成するので，安易に使わないこと
//ベクトルはオペレータでは和と差だけ定義，積が2種類あるため
//AOCSVectorは縦ベクトルで定義
AOCSVector operator +(AOCSVector& a, AOCSVector& b);

AOCSVector operator -(AOCSVector& a, AOCSVector& b);
AOCSVector operator *(AOCSMatrix& a, AOCSVector& b);//行列×ベクトル⇒ベクトル

AOCSMatrix operator +(AOCSMatrix& a, AOCSMatrix& b);
AOCSMatrix operator -(AOCSMatrix& a, AOCSMatrix& b);
AOCSMatrix operator *(AOCSMatrix& a, AOCSMatrix& b);


#endif /* SRC_USER_DRIVERS_AOCS_AOCSVECTOR_H_ */
