/*
 * AOCSVector.cpp
 *
 *  Created on: 2022/07/03
 *      Author: Noro
 */


#include <math.h>
#include "LAFunction.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void getCofactor(const AOCSMatrix& A, AOCSMatrix& temp, int p, int q, int n);
void adjoint(AOCSMatrix& A, AOCSMatrix& adj);

//******************************************//
//色々考えた結果めんどくさすぎて構造体のオペレータを定義
//クラスは各インスタンスごとにメソッドを持つのが無駄に感じられたので，構造体にした

void copy(AOCSMatrix&a, AOCSMatrix&b){
    set_size(a,b);
    for(int i=0; i < a.n; i++){
        for(int j=0; j < a.m; j++){
            a.arr[i][j] = b.arr[i][j];
        }
    }
}

void copy(AOCSVector&a, AOCSVector&b){
    set_size(a,b);
    memcpy(a.arr,b.arr,b.n*sizeof(b.arr[0]));
}

void partialsubstitute(AOCSMatrix&a, AOCSMatrix& b, uint8_t n,uint8_t m){
    if((a.n>=(b.n+n))&&(a.m>=(b.m+m))){
        for(int i=0; i < b.n; i++){
            for(int j=0; j < b.m; j++){
                a.arr[i+n][j+m] = b.arr[i][j];
            }
        }
    }
}

void partialsubstitute(AOCSVector&a, AOCSVector& b, uint8_t n){
    if (a.n >= (b.n+n)){
        for(int i=0; i < b.n; i++){
            a.arr[i+n] = b.arr[i];
        }
    }
}

AOCSVector copy_(AOCSVector& a){
    AOCSVector ret = similar(a);
    copy(ret,a);
    return ret;
}

AOCSMatrix copy_(AOCSMatrix& a){
    AOCSMatrix ret = similar(a);
    copy(ret,a);
    return ret;
}

//コピーすることになるのでその点は注意
AOCSMatrix convertVec2Mat(AOCSVector a){
    AOCSMatrix ret;
    ret.n = a.n;
    ret.m = 1;
    for(int i=0;i<a.n;i++){
        ret.arr[i][0]=a.arr[i];
    }
    return ret;
}

AOCSVector convertMat2Vec(AOCSMatrix a){
    AOCSVector ret;
    ret.n=a.n*a.m;
    for(int i=0;i<ret.n;i++){
        ret.arr[i] = a.arr[i][0];
    }
    return ret;
}

AOCSVector similar(AOCSVector& a){
    AOCSVector ret;
    ret.n=a.n;
    return ret;
}

AOCSMatrix similar(AOCSMatrix& a){
    AOCSMatrix ret;
    ret.n = a.n;
    ret.m = a.m;
    return ret;
}

void reset(AOCSVector& a){
    for(int i=0;i<a.n;i++){
        a.arr[i]=0.0;
    }
}

void reset(AOCSMatrix& a){
    for(int i=0;i<a.n;i++){
        for(int j=0; j < a.m; j++){
            a.arr[i][j]=0.0;
        }
    }
}

//行列式計算，正方行列が前提
AOCStype det(AOCSMatrix& a, uint8_t n){
    if (n == 2){
        return(a.arr[0][0] * a.arr[1][1] - a.arr[0][1] * a.arr[1][0]);
    }
    else{
        AOCSMatrix tmp = AOCSMatrixHelper(a.n-1, a.m-1);
        reset(tmp);//一応初期化

        AOCStype ret=0;
        int sign = 1;
        for (int x = 0; x < n; x++){
            getCofactor(a, tmp, 0, x, n);
            ret += sign*a.arr[0][x]*det(tmp, n-1);
            sign *= -1;
        }
    return ret;
    }
}

//余因子行列 正方行列が前提！
void adjoint(AOCSMatrix& A, AOCSMatrix& adj)
{
    if (A.n == 1)
    {
        adj.arr[0][0] = 1;
        return;
    }

    int sign = 1;

    AOCSMatrix temp = similar(A);
    reset(temp);

    for (int i = 0; i < A.n; i++)
    {
        for (int j = 0; j < A.m; j++)
        {
            getCofactor(A, temp, i, j, A.n);

            sign = ((i + j) % 2 == 0) ? 1 : -1;

            adj.arr[i][j] = (sign) * (det(temp, A.n - 1));
        }
    }
}

void getCofactor(const AOCSMatrix& A, AOCSMatrix& temp, int p, int q, int n)
{
    int i = 0, j = 0;

    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (row != p && col != q)
            {
                temp.arr[i][j++] = A.arr[row][col];

                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

//逆行列計算
bool inverse(AOCSMatrix& inverse, AOCSMatrix& A)
{
    AOCStype det_ = det(A, A.n); // Determinant
    set_size(inverse,A);
    if (det_ <= 1e-10)
    {
        return false;
    }

    AOCSMatrix adj = similar(A);
    reset(adj);
    adjoint(A, adj);

    for (int i = 0; i < A.n; i++)
        for (int j = 0; j < A.m; j++)
            inverse.arr[i][j] = adj.arr[i][j] / det_;
    return true;
}

AOCSMatrix Transpose(AOCSMatrix& a){
    AOCSMatrix ret;
    Transpose(ret,a);
    return ret;
}

void Transpose(AOCSMatrix& a, AOCSMatrix& b){
    set_size(a, b.m, b.n);//転置なのでn×m⇒m×nになる
    for(int i=0; i < b.n; i++){
        for(int j=0; j< b.m; j++){
            a.arr[j][i] = b.arr[i][j];
        }
    }
}

void add(AOCSMatrix& a, AOCSMatrix& b, AOCSMatrix& c){
    set_size(a, b);
    for(int i=0; i < b.n; i++){
        for(int j=0; j< b.m; j++){
            a.arr[i][j] = b.arr[i][j]+c.arr[i][j];
        }
    }
}

void add(AOCSVector& a, AOCSVector& b, AOCSVector& c){
    set_size(a, b);
    for(int i=0; i< b.n; i++){
        a.arr[i]=b.arr[i]+c.arr[i];
    }
}

void pull(AOCSVector& a, AOCSVector& b, AOCSVector& c){
    set_size(a, b);
    for(int i=0; i< b.n; i++){
        a.arr[i]=b.arr[i]-c.arr[i];
    }
}

void pull(AOCSMatrix& a, AOCSMatrix& b, AOCSMatrix& c){
    set_size(a, b);
    for(int i=0; i< b.n; i++){
        for(int j=0;j <b.m;j++){
            a.arr[i][j]=b.arr[i][j]-c.arr[i][j];
        }
    }
}

void mul(AOCSVector& a, AOCSMatrix& b, AOCSVector& c){
    set_size(a, b.n);
    reset(a);//インクリメントするため中身を0にしておく必要がある
    for(int i=0; i< b.n; i++){
        for(int j=0;j <b.m; j++){
            a.arr[i]+=b.arr[i][j]*c.arr[j];
        }
    }
}

void mul(AOCSVector& a, AOCStype b, AOCSVector& c){
    set_size(a, c.n);
    for(int i=0; i< c.n; i++){
        a.arr[i]=b*c.arr[i];
    }
}

void mul(AOCSMatrix& a, AOCSMatrix& b, AOCSMatrix& c){
    set_size(a, b.n, c.m);
    reset(a);//インクリメントするため中身を0にしておく必要がある
    for(int i=0; i< b.n; i++){
        for(int j=0;j <c.m;j++){
            for(int k=0;k<b.m;k++){
                a.arr[i][j]+=b.arr[i][k]*c.arr[k][j];
            }
        }
    }
}

void mul(AOCSMatrix& a, AOCStype b, AOCSMatrix& c){
    set_size(a, c);
    for(int i=0; i< c.n; i++){
        for(int j=0;j <c.m;j++){
            a.arr[i][j]=b*c.arr[i][j];
        }
    }
}

void set_size(AOCSVector& a,AOCSVector& b){
    a.n=b.n;
}

void set_size(AOCSVector& a,uint8_t n){
    a.n=n;
}

void set_size(AOCSMatrix& a,AOCSMatrix& b){
    a.n=b.n;
    a.m=b.m;
}

void set_size(AOCSMatrix& a,uint8_t n, uint8_t m){
    a.n=n;
    a.m=m;
}

//Exp(At) = E + AΔt *1/2*(AΔt)^2+・・・
//今回は1次までで近似
void MatExp(AOCSMatrix& M, AOCSMatrix& A, AOCStype dt){
    AOCSMatrix eye=AOCSMatEye(A.n);
    reset(M);
    AOCSMatFourCalculus(&M, 2, 1.0,&eye, dt,&A);
}

/***************************************/
//ここ以降はオペレータ関数
//参照渡しで構造体同士の足し算を定義
AOCSVector operator +(AOCSVector& a, AOCSVector& b){
    AOCSVector ret = similar(a);
    add(ret,a,b);
    return ret;
}

AOCSVector operator -(AOCSVector& a, AOCSVector& b){
    AOCSVector ret = similar(a);
    pull(ret,a,b);
    return ret;
}

//ｎ×m　m
AOCSVector operator *(AOCSMatrix& a, AOCSVector& b){
    AOCSVector ret = AOCSVectorHelper(a.n);
    mul(ret,a,b);
    return ret;
}

AOCSMatrix operator +(AOCSMatrix& a, AOCSMatrix& b){
    AOCSMatrix ret = similar(a);
    add(ret, a, b);
    return ret;
}

AOCSMatrix operator -(AOCSMatrix& a, AOCSMatrix& b){
    AOCSMatrix ret = similar(a);
    pull(ret,a,b);
    return ret;
}

//(n_a×m_a)×（n_b×m_b）=n_a×m_b
AOCSMatrix operator *(AOCSMatrix& a, AOCSMatrix& b){
    AOCSMatrix ret = AOCSMatrixHelper(a.n,b.m);
    mul(ret, a, b);
    return ret;
}
