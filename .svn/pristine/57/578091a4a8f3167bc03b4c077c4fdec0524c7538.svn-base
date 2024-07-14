/*
 * OD_EKF.cpp
 *
 *  ver.1.0 2022/07/27 Noro
 *
 */

//汎用性を高めるために関数ポインタでここでは具体的なFとかはここで入れない

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <src_user/Library/General/DBG_printf.h>
#include "OD_EKF.h"
#include <src_user/Drivers/Aocs/LinearAlgebra/LAFunction.h>//hファイルではインクルードしないこと！　野呂
#include <src_user/Applications/UserDefined/AOCS/Global_AOCS_Parameter.h>

void SensorObservation(EKF_Param&, EKF_fun&, void*);
void TimePropagation(EKF_Param&, EKF_fun&, void*);
uint8_t CheckObsFlag(EKF_Param& ekf);


/** Nonlinear system
 *  X = f(X,u) + g(X)w
 *  Z = h(X) + v
 *
 *  X_k = f(X_k-1, u_k) + g_w(X_k)w_k
 *  Z_k = h(X_k) + v_k
 */

/** Linear system around reference point
 *  X = xref + x
 *  xref = f(xref,u)
 *  zref = h(xref)
 *  x = Ax + B(w + T)
 *  z = Hx + v
 *
 *  x_k = A_k*x_k + B_w_k*(w_k + T)
 *  z_k = H_k*x_k + v_k
 */

/** Jacobians
 *  A = df/dx
 *  H = dh/dx
 *
 *  A_k = df / dx at x_k-1\k-1 and u_k,
 *  H_k = dh / dx at x_k\k-1,
 */

/** Discrete state transition matrix
 *  Uses the 2nd order of the matrix exponential function. Not sure if this accuracy is necessary
 *  F_k  = E + A_k*Δt
 *  Gw_K = E*Δt * B_k
 *  行列計算が多くなりすぎるので　 2次はいらない可能性。nano-JASMINではF_k　Δtの1次項まで
 *  F_k  = E + A_k*Δt + 1/2*A_k^2*Δt^2
 *  Gw_K = (E*Δt +1/2*A_k*Δt^2 + 1/6*A_k*Δt^3) * B_k
 */

/** Predict (Estimate)
 *  Predicted state estimate:             xref = f(xref,u)        xref_k\k-1 = f(xref_k-1\k-1, u_k)
 *  Predicted covariance estimate:        P    = FPF' + GwQGw'    P_k\k-1    = F_k * P_k-1\k-1 * F_k' + Gw_K * Q_k * Gw_K'
 */

/** Update (Observe)
 *  Innovation or measurement residual:   y = z - h(xref)         y_k   = z_k - h(xref_k\k-1)
 *  Innovation (or residual) covariance:  S = HPH' + R            S_k   = H_k * P_k\k-1 * H_k' + R_k
 *  Near-optimal Kalman gain:             K = P*H'S^-1            K_k   = P_k\k-1 * H_k' * S_k^-1
 *  Updated state estimate:               x = x + Ky              x_k\k = x_k\k-1 + K*y_k
 *  Updated covariance estimate:          P = (I - KH)P           P_k\k = (I - K_k * H_k)*P_k\k-1
 */
bool OD_EKF(EKF_Param* ekf_, EKF_fun* fun_, void* arg){
    //（funの関数で使う構造体をここで触らないで処理できるように）
    //このEKFのドライバはセンサに依存しない抽象的な形でコーディングを行った．（すみわけのためODと命名したが，このドライバはカルマンフィルタ全般で利用できる）
    //そのため推定系を新たに構築したい場合はアプリケーション側で新しいEKF_ParamとEKF_funを作って，OD_EKFを呼ぶだけでよい
    EKF_Param& ekf = *ekf_;
    EKF_fun& fun= *fun_;
    //ORBENV_STRUCT* orbenv_ =  (ORBENV_STRUCT*)arg;
    //天文衛星のカルマンフィルタとは逆にしている
    //これはi+1番目の時間更新をしようにも，実際次のループのここにたどり着くまでの時間に誤差があるため
    //マイコンのRTC使って正確な時間間隔を使って今の時間まで時間伝搬するのが賢い
    TimePropagation(ekf, fun, arg);
    SensorObservation(ekf, fun, arg);
    return true;
}

void SensorObservation(EKF_Param& ekf, EKF_fun& fun, void* arg){
    if(CheckObsFlag(ekf)){//センサー観測が出来たら
        AOCSMatrix& R = ekf.std.R;//無駄なメモリを使いたくないので，参照で定義
        AOCSMatrix& P_ = ekf.P_;
        AOCSMatrix tmp1, tmp2, ht_T, K;
        AOCSVector dy = similar(ekf.y);
        AOCSVector z_ = similar(ekf.z);//事後推定状態量で想定される観測量
        AOCSMatrix ht = AOCSMatrixHelper(ekf.z.n, ekf.y.n);
        reset(z_);
        reset(ht);

        fun.h(&z_, ekf.t, &ekf.y_, arg);//h(yt)
        AOCSVector dz = ekf.z - z_;//参照点からの観測量の誤差

        fun.hx(&ht, ekf.t, &ekf.y_, arg);//hx_t
        ht_T = Transpose(ht);

        //ht*P_*ht_R+R
        mul(tmp1, ht, P_);
        mul(tmp2, tmp1, ht_T);
        add(tmp1, tmp2, R);

        if(inverse(tmp2, tmp1)==true){//tmp=inv(tmp)
            //K = P_*ht_T*inv(tmp)
            mul(tmp1, ht_T, tmp2);
            mul(K, P_, tmp1);
            //P=P_-K*Ht*P_
            mul(tmp1, K, ht);
            mul(tmp2, tmp1, P_);
            pull(ekf.P, P_, tmp2);
            //dy=K*dz
            mul(dy, K, dz);
            //y=y_+dy
            add(ekf.y, ekf.y_, dy);
        }
        else{//逆行列が解けない＝ランクが落ちてるときは更新しない
            copy(ekf.P,ekf.P_);//誤差共分散行列に事前誤差共分散行列をそのまま代入
            copy(ekf.y,ekf.y_);//推定状態量に事前推定状態量をそのまま代入
        }
    }
  else{
      //観測できなかった場合，そのまま事前推定値を推定値とする
      copy(ekf.P,ekf.P_);//誤差共分散行列に事前誤差共分散行列をそのまま代入
      copy(ekf.y,ekf.y_);//推定状態量に事前推定状態量をそのまま代入
  }
}

void TimePropagation(EKF_Param& ekf, EKF_fun& fun, void* arg){
    AOCSMatrix& Q = ekf.std.Q;//メモリを無駄にせず・簡単にアクセスするため参照体で置く
    static AOCSMatrix tmp1,tmp2;
    AOCSMatrix Gw = AOCSMatrixHelper(ekf.y_.n, Q.n);
    AOCSMatrix F = AOCSMatrixHelper(ekf.y_.n, ekf.y_.n);

    set_size(tmp1, ekf.y_.n, ekf.y_.n);
    fun.fx(&tmp1, ekf.t, &ekf.y, arg);
    MatExp(F, tmp1, ekf.dt);
    set_size(tmp2, ekf.y_.n, ekf.y_.n);
    //Gwの計算(0次近似 Gw=B*dt)
    fun.fw(&Gw, ekf.t, &ekf.y, arg);//B = df/dw
    mul(Gw, ekf.dt, Gw);
    AOCSMatrix Gw_T=Transpose(Gw);

    //P_=F*P*F^T+Gw*Q*Fw^T
    mul(tmp1, F, ekf.P);
    AOCSMatrix F_T = Transpose(F);
    mul(ekf.P_, tmp1, F_T);

    mul(tmp1, Gw, Q);
    mul(tmp2, tmp1, Gw_T);////////////printfのエラーの原因
    add(ekf.P_, ekf.P_, tmp2);
    //ほんとは複合演算子的に使うのは良くない（中で書き換わる可能性があるため）
    //が，addでは書き換わらないのを知っているのでメモリ効率重視でこの処理にした

    //ここのtは前のループの時間
    //dtは今の時間-前のループの時間が入っていて，前のループの事後推定値を使って今の時間の状態量を計算している
    ODE(&ekf.y_, fun.f, &ekf.y, ekf.t, ekf.t+ekf.dt, ekf.options, arg);
    //計算が終わった後に，tを今の時間に更新する
    ekf.t+=ekf.dt;//時間更新
}

uint8_t CheckObsFlag(EKF_Param& ekf){
    if(ekf.obs_flag==1) return true;
    else return false;
}

void Set_SysNoises(EKF_std* std, AOCSVector* sigma_sys, AOCSVector* sigma_obs){
    set_size(std->Q, sigma_sys->n, sigma_sys->n);
    set_size(std->R, sigma_obs->n, sigma_obs->n);
    for(int i=0; i<sigma_sys->n; i++){
        std->Q.arr[i][i] = pow(sigma_sys->arr[i], 2);
    }

    for(int i=0; i<sigma_obs->n; i++){
        std->R.arr[i][i] = pow(sigma_obs->arr[i], 2);
    }
}

void Set_CovMatrix(AOCSMatrix* P_, AOCSVector* ref){
    AOCSMatrix& P_tmp = *P_;
    set_size(P_tmp, ref->n, ref->n);
    for(int i=0; i < ref->n; i++){
        P_tmp.arr[i][i] = pow(ref->arr[i],2);
    }
}

//関数を登録する関数
void Set_SysFunction(EKF_fun *fun,
                    void (*f)(AOCSVector*,  AOCStype, AOCSVector*, void*),
                    void (*fx)(AOCSMatrix*, AOCStype, AOCSVector*, void*),
                    void (*fw)(AOCSMatrix*, AOCStype, AOCSVector*, void*),
                    void (*h)(AOCSVector*,  AOCStype, AOCSVector*, void*),
                    void (*hx)(AOCSMatrix*, AOCStype, AOCSVector*, void*)){
    fun->f=f;
    fun->fx=fx;
    fun->fw=fw;
    fun->h=h;
    fun->hx=hx;
}

void Set_Observation(EKF_Param* ekf, AOCSVector* z){
    EKF_Param& ekf_=*ekf;
    AOCSVector& z_ = *z;

    set_size(ekf_.z, z_);
    copy(ekf_.z, z_);
}

