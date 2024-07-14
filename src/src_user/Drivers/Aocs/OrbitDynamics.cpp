/*
 * OrbitDynamics.cpp
 *
 *  Created on: 2022/07/04
 *      Author: Noro
 */

#include "OrbitDynamics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <src_user/Drivers/Aocs/LinearAlgebra/LAFunction.h>//hファイルではインクルードしないこと！　野呂
#include "AOCScalc.h"

static AOCSMatrix tmp1, tmp2, tmp3;//計算の中間変数を毎回定義するのは呼び出しで結構時間を食うかと思うので，Static変数で定義

//-----------------------------------------
// ADCS　計算関数宣言
//-----------------------------------------

void OrbitDisturbances(AOCSVector*, AOCStype, AOCSVector*, ORBENV_STRUCT*);
void J2perturbation(AOCSVector*, AOCStype,AOCSVector*, ORBENV_STRUCT*);
void AirDrag(AOCSVector*, AOCStype, AOCSVector*, ORBENV_STRUCT*);
void SRP(AOCSVector*, AOCStype, AOCSVector*, ORBENV_STRUCT*);

void fJ2_r(AOCSMatrix&ret, AOCStype t, AOCSVector*y, ORBENV_STRUCT *orbenv_);
void fair_v(AOCSMatrix&ret, AOCStype t, AOCSVector*y, ORBENV_STRUCT *orbenv_);

void OrbitDynamics(AOCSVector* ret, AOCStype t, AOCSVector* y, void* arg){
//  DYNAMICS : xとvの時間微分をベクトルにしてretに渡す関数
    AOCSVector r = AOCSVecRetrieve(y,0,3);
    AOCSVector v = AOCSVecRetrieve(y,3,3);

    //ODS_cmd_ORBPROPで渡された引数をここで本当の構造体ポインタにキャスト
    ORBENV_STRUCT* orbenv_ =  (ORBENV_STRUCT*) arg;

    //norm計算
    AOCStype r_norm = AOCSNorm(&r);
    AOCStype J0gravity = mu_e/pow(r_norm,3);

    AOCSVector f = AOCSVectorHelper(3);
    reset(f);//0で初期化

    OrbitDisturbances(&f,t,y, orbenv_);

    for(int i=0;i<3;i++){
    ret->arr[i] = v.arr[i];
    ret->arr[i+3] = -J0gravity*r.arr[i]+f.arr[i];
    }
}

void OrbitDisturbances(AOCSVector* f, AOCStype t, AOCSVector* y, ORBENV_STRUCT* orbenv_){
    J2perturbation(f,t,y,orbenv_);
//  MoonGravity(f,t,y,orbenv_);
//  SunGravoty(f,t,y,orbenv_);
    AirDrag(f,t,y,orbenv_);
    SRP(f,t,y,orbenv_);

}

void J2perturbation(AOCSVector* f, AOCStype t,AOCSVector* y ,ORBENV_STRUCT* orbenv_){
// ミッション解析と軌道設計の基礎p303 Fd=-3J2*re^2*n^2*a^3/(2r^4)((1-5z^2/r^2)er+2z/r*ek)
    AOCStype z, r_norm, E, coef;
    AOCSVector r = AOCSVecRetrieve(y,0,3);
    AOCSVector v = AOCSVecRetrieve(y,3,3);
    AOCSVector er = AOCSVecNormalize(&r);
    z = r.arr[2];

    //rノルム計算
    r_norm = AOCSNorm(&r);
    //エネルギー計算
    E = pow(AOCSNorm(&v),2)/2 -mu_e/r_norm;
    //軌道長半径計算
//  a = -mu_e/(2*E);
//  n = sqrt(mu_e/pow(a,3));
    coef = -3*J2*pow(Re,2)*mu_e/(2*pow(r_norm,4));

    for(int i=0;i<3;i++) f->arr[i] += coef * (1-5*pow(z/r_norm,2))*er.arr[i];//erの要素
    f->arr[2] += coef *2*z/r_norm;//ekの要素
}

void AirDrag(AOCSVector* f, AOCStype t, AOCSVector* y,ORBENV_STRUCT* orbenv_){
    AOCSVector v = AOCSVecRetrieve(y,3,3);
    AOCSVector ev = AOCSVecNormalize(&v);
    AOCStype v_norm = AOCSNorm(&v);
    AOCStype coef = -1/2*orbenv_->rho_air*orbenv_->B*v_norm;//速度方向の反対側に1/2rho*B*v^2が出る=1/2rho*B*v*vn
    for(int i=0;i<3;i++){
        f->arr[i] += coef*v.arr[i];
    }
}

void SRP(AOCSVector* f, AOCStype t, AOCSVector* y, ORBENV_STRUCT* orbenv_){
    //太陽輻射圧 f=-Cr*A/m * P_sun * e_sun
    //Cr*A/m=B_SRP*B_SRP_scale
    int eclipse=*orbenv_->Eclipse_flag;//
    if(eclipse==1){
    AOCStype coef = -orbenv_->B_SRP*orbenv_->B_SRP_scale*P_sun;
    for(int i=0;i<3;i++){
        f->arr[i] += coef * orbenv_->e_suns[i];
    }
    }
}

void OrbitJacobian(AOCSMatrix* ret, AOCStype t, AOCSVector* y, void* arg){
//自分で構築していないので，太陽方向とかを取り入れるのが難しそう...
//SRPはほぼバイアス的に働くので，EKFで考えると勾配には影響を無視できそう
    AOCStype rn, rn2;
    AOCSMatrix &ret_=*ret;//こここれで大丈夫か？
    AOCSMatrix fx, fv;
    AOCSMatrix eye = AOCSMatEye(3);
    AOCSVector r = AOCSVecRetrieve(y,0,3);
    AOCSMatrix r_mat = convertVec2Mat(r);

    //ODS_cmd_ORBPROPで渡された引数をここで本当の構造体ポインタにキャスト
    ORBENV_STRUCT* orbenv_ =  (ORBENV_STRUCT*)arg;

    set_size(tmp1, 3, 3);
    set_size(tmp2, 3, 3);
    set_size(tmp3, 3, 3);
    set_size(fx,3,3);
    set_size(fv,3,3);
    reset(fx);
    reset(fv);
    reset(ret_);

    rn = AOCSNorm(&r);
    rn2 = pow(rn,2);

    //rdot=v
    partialsubstitute(ret_, eye, 0, 3);

    //Ir^2-3r・r
    Transpose(tmp1, r_mat);
    mul(tmp2, r_mat, tmp1);//r*r^T
    mul(tmp1, -3.0, tmp2);//-3(r*r^T)
    mul(tmp2, rn2, eye);
    add(tmp1, tmp1, tmp2);
    mul(fx, -mu_e/pow(rn,5),tmp1);

    fJ2_r(fx, t, y, orbenv_);
    fair_v(fv, t, y, orbenv_);

    //fxのヤコビアンを代入
    partialsubstitute(ret_, fx, 3, 0);
    //fvのヤコビアンを代入
    partialsubstitute(ret_, fv, 3, 3);

}


void fJ2_r(AOCSMatrix&ret, AOCStype t, AOCSVector*y, ORBENV_STRUCT *orbenv_){
    AOCSMatrix ez_T, ez;
    AOCSMatrix tmpret = similar(ret);
    set_size(ez_T, 1, 3);
    reset(ez_T);
    ez_T.arr[0][2] = 1.0;
    Transpose(ez, ez_T);
    AOCStype rn, rn2, coef, z, z_r;

    AOCSVector r = AOCSVecRetrieve(y,0,3);
    AOCSMatrix r_mat = convertVec2Mat(r);
    AOCSMatrix r_T = Transpose(r_mat);
    AOCSMatrix eye = AOCSMatEye(3);
    AOCSVector r_norm = AOCSVecNormalize(&r);
    AOCSMatrix er = convertVec2Mat(r_norm);

    rn = AOCSNorm(&r);
    rn2 = pow(rn,2);
    z = r.arr[2];
    z_r = z/rn;

    coef=-3.0/J2*pow(Re,2)*mu_e/2.0;
    //{(1-5(r/z)^2)ez+2z/r*ek}(-4r^T/rn^6)
    mul(tmp1, (1.0-5.0*pow(z_r,2)),er);
    mul(tmp2, 2*z_r, ez_T);
    add(tmp3, tmp1, tmp2);
    mul(tmp1, -4.0/pow(rn,6),r_T);
    mul(tmpret, tmp3, tmp1);

    mul(tmp1, rn2, eye);
    mul(tmp2, r_mat,r_T);
    pull(tmp3, tmp1, tmp2);
    mul(tmp3,(1.0-5.0*pow(z_r,2))*pow(rn,-3),tmp3);

    mul(tmp1, rn2*z,ez_T);
    mul(tmp2, -pow(z,2),r_T);
    add(tmp1, tmp1, tmp2);
    mul(tmp2, -10.0*pow(rn,-4),tmp1);
    mul(tmp1, er, tmp2);

    add(tmp3,tmp3,tmp1);

    mul(tmp1, rn2, ez_T);
    mul(tmp2, -z, r_T);
    add(tmp1, tmp1, tmp2);
    mul(tmp2, ez, tmp1);
    mul(tmp1,2.0*pow(rn,-3),tmp2);

    add(tmp3,tmp3,tmp1);
    mul(tmp1,pow(rn,-4),tmp3);
    add(tmpret,tmpret,tmp1);
    mul(tmpret,coef,tmpret);

    add(ret,ret,tmpret);

}

void fair_v(AOCSMatrix&ret, AOCStype t, AOCSVector*y, ORBENV_STRUCT *orbenv_){
    AOCSVector v = AOCSVecRetrieve(y,3,3);
    AOCSMatrix v_mat = convertVec2Mat(v);
    AOCSMatrix v_T = Transpose(v_mat);
    AOCSMatrix eye = AOCSMatEye(3);
    AOCStype vn = AOCSNorm(&v);
    AOCStype vn2=pow(vn,2);
    AOCStype coef = -1/2*orbenv_->rho_air*orbenv_->B;

    mul(tmp1,vn2,eye);
    mul(tmp2,v_mat,v_T);
    pull(tmp1,tmp1,tmp2);
    mul(tmp1,coef,tmp1);

    //他の外力項がfvに入ってきても対応できるよう
    //既にあるfvにインクリメントの形で空力項を追加
    add(ret,ret,tmp1);
}

void UpdateBfromBstar(ORBENV_STRUCT* orbenv){
    //弾道係数 Cd*S/m TLEのBstarから変換+B_scalecoef
    //計算した結果Reはkmをかけている模様
    //"Estimation of ballistic coefficients of low altitude debris objects from historical two line elements"
    //では通常の定義では小さすぎるらしい
    orbenv->B = orbenv->B_scalecoef * (2*orbenv->B_star/(Re*1e-3*rho_air_0))*10;
}

void OrbitSysnoiseJacobian(AOCSMatrix* ret, AOCStype t, AOCSVector* y, void* arg){
    AOCSMatrix &ret_=*ret;//こここれで大丈夫か？
    //ノイズは未知の外力項にだけ入れる
    set_size(ret_,6,3);
    reset(ret_);
    for(int i=0;i<3;i++){
        ret_.arr[3+i][i]=1.0;
    }
}
/****************************/
//ほんとはここに書くべきではないが，めんどくさいのでGPSもここに入れちゃう
//h(y)=y 位置，速度がそのまま観測
void GPSobs(AOCSVector*ret, AOCStype t, AOCSVector* y, void* arg){
    AOCSVector& ret_= *ret;
    set_size(ret_, 6);
    for(int i=0;i<6;i++){
        ret_.arr[i]=y->arr[i];
    }
}

//ヤコビアンはただの単位行列
void GPSobsJacobian(AOCSMatrix *ret, AOCStype t, AOCSVector* y, void* arg){
    AOCSMatrix & ret_= *ret;
    set_size(ret_, 6, 6);
    AOCSMatrix eye = AOCSMatEye(6);
    copy(ret_, eye);
}


