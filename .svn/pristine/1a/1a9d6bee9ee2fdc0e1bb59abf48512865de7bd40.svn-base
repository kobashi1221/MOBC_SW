/*
 * Orbit.cpp
 *
 *  Created on: 2021/09/26
 *      Author: Inakawa
 */

#include "Integrater.h"
#include <math.h>
#include <src_user/Drivers/Aocs/LinearAlgebra/LAFunction.h>//hファイルではインクルードしないこと！　野呂
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <src_core/System/TimeManager/time_manager.h>

//軌道計算を行う。ここでは設定されたTLEからSGP4で伝搬し、位置と速度を計算しクラスに格納することとする

//-----------------------------------------
// ADCS　計算関数宣言
//-----------------------------------------

static AOCStype min(AOCStype a,AOCStype b);
static AOCStype max(AOCStype a,AOCStype b);
static AOCStype min(AOCStype a,AOCStype b){
    if(a<b) return a;
    else return b;
}
static AOCStype max(AOCStype a,AOCStype b){
    if(a>b) return a;
    else return b;
}
INTEGRATER_ACK ODE(AOCSVector* y,void (*fun)(AOCSVector*, AOCStype, AOCSVector*, void*), AOCSVector* y0, AOCStype t0, AOCStype tf, IntegraterOptions* options, void* arg){
    //他の人から見たらソルバーなんておそらくどうでもいいと思うので，
    //勝手にいい感じで計算してくれるようにODE関数を作った
    INTEGRATER_ACK ret = DOP45(y, fun, y0, t0, tf, options, arg);
    if(ret!=INTEGRATER_SUCCESS){
        //4次のルンゲクッタ
        ret = RK4(y, fun, y0, t0, tf, options, arg);
    }
    return ret;
}

INTEGRATER_ACK RK4(AOCSVector* y,void (*fun)(AOCSVector*, AOCStype, AOCSVector*, void*), AOCSVector* y0, AOCStype t0, AOCStype tf, IntegraterOptions* options, void* arg){
//  ４次のRungekutta手法の実装
//  計算にポインタアクセスを徹底しているので，計算効率は良いはず
//  fun : 被積分関数の関数ポインタ
//  y0 : 初期値の構造体
//  t0 : 初期時刻
//  T  : 積分区間
//  options : 積分オプション

    step_t s_time = TMGR_get_master_step();  //
    step_t f_time = TMGR_get_master_step() + options->timeout;
    step_t now_time = s_time;

    uint8_t vec_size = y0->n; //ベクトルサイズ
    AOCStype h = min(tf-t0, options->h);     //刻み幅
    if(h < 0){
        h = options->orbset_h;
    }

    AOCSVector k1 = AOCSVectorHelper(vec_size);
    AOCSVector k2 = AOCSVectorHelper(vec_size);
    AOCSVector k3 = AOCSVectorHelper(vec_size);
    AOCSVector k4 = AOCSVectorHelper(vec_size);

    //  中間変数 for文の中で毎回構造体を生成するのは避けるべきなので，手前で作ってポインタで再利用
    AOCSVector tmp_ret = AOCSVectorHelper(vec_size);
    AOCSVector tmp_y = AOCSVectorHelper(vec_size);

    //初期値代入
    AOCSVecCopy_(y,y0);
    AOCStype t = t0;
    int counter = 0;
    while(t<tf){
      if(now_time > f_time){
          printf("RK long time\n\r");
            return INTEGRATER_FAILED;
        }
        ++now_time;
        fun(&tmp_ret, t, y, arg);
        Vecmul(&k1, h, &tmp_ret);//k1 = h*fun()

        AOCSVecFourCalculus(&tmp_y,2, 1.0,y, 1/2.0,&k1);
        fun(&tmp_ret, t+h/2, &tmp_y, arg);
        Vecmul(&k2, h, &tmp_ret);//k2 = h*fun()

        AOCSVecFourCalculus(&tmp_y,2, 1.0,y, 1/2.0,&k2);
        fun(&tmp_ret, t+h/2, &tmp_y, arg);
        Vecmul(&k3, h, &tmp_ret);//k3 = h * fun()

        AOCSVecFourCalculus(&tmp_y,2, 1.0,y, 1.0,&k3);
        fun(&tmp_ret, t+h, &tmp_y, arg);
        Vecmul(&k4, h,&tmp_ret);//k4 = h * fun()

        AOCSVecFourCalculus(&tmp_y,5, 1.0,y, 1/6.0,&k1, 1/3.0,&k2, 1/3.0,&k3, 1/6.0,&k4);
        AOCSVecUpdate(y, &tmp_y);

        h = min(h,tf-t);//最後のところで時間が調整できるように
        if(h < 0){
            h = options->orbset_h;
        }
        t += h;
        ++counter;
        if(counter % 1000 == 0){
          printf("RK loop count is %d \r\n", counter);
        }
    }

    return INTEGRATER_SUCCESS;
}

INTEGRATER_ACK DOP45(AOCSVector* y, void (*fun)(AOCSVector*, AOCStype, AOCSVector*, void*), AOCSVector* y0, AOCStype t0, AOCStype tf, IntegraterOptions* options, void* arg){
//5次の埋め込み型ルンゲクッタ法の実装（刻み時間が精度に応じて自動制御されRK4より計算が速くなる）
//  参照：https://github.com/bastikr/DormandPrince.jl/blob/master/src/DormandPrince.jl
//       https://github.com/MartinK-99/LotkaVolterraEquationsDormandPrince45/blob/main/main.py
//  MATLABのODE45で一般に使われているソルバー
    //  fun : 被積分関数の関数ポインタ
    //  y0 : 初期値の構造体
    //  t0 : 初期時刻
    //  T  : 積分区間
    //  options : 積分オプション

    step_t s_time = TMGR_get_master_step();  //
    step_t f_time = TMGR_get_master_step() + options->timeout;
    step_t now_time = s_time;

    //初期値代入
    AOCSVecCopy_(y,y0);
    AOCStype t = t0;
    AOCStype h = min(tf-t0,options->h);//1回目のイタレーションでtfを乗り越える可能性を除去するため
    if(h < 0){
        h = options->orbset_h;
    }
    AOCStype tol = options->tol;
    int nmax = options->nmax;
    int try_num = 0;//試行回数

//  ステップサイズの調整パラメータ
    AOCStype a_max = 2.0; //# new step can be a max of two times as big
    AOCStype a_min = 1.0/2.0; //# new step can be min a half as big
    AOCStype b = 0.8; // new time step will be multiplied by 0.8 to account for errors when calculating stepsize
    AOCStype h_new;

    uint8_t vec_size = y0->n;
    AOCSVector k1 = AOCSVectorHelper(vec_size);
    AOCSVector k2 = AOCSVectorHelper(vec_size);
    AOCSVector k3 = AOCSVectorHelper(vec_size);
    AOCSVector k4 = AOCSVectorHelper(vec_size);
    AOCSVector k5 = AOCSVectorHelper(vec_size);
    AOCSVector k6 = AOCSVectorHelper(vec_size);
    AOCSVector u5 = AOCSVectorHelper(vec_size);
    AOCSVector k7 = AOCSVectorHelper(vec_size);
    AOCSVector u4 = AOCSVectorHelper(vec_size);

    AOCSVector tmp_y = AOCSVectorHelper(vec_size);

    int i = 0;
    while (i < nmax){
      if(now_time > f_time){
          printf("DOP45 long time\n\r");
            return INTEGRATER_FAILED;
        }
        ++now_time;
        fun(&k1, t, y, arg);

        AOCSVecFourCalculus(&tmp_y, 2, 1.0,y, h/5.0,&k1);
        fun(&k2, t+h/5.0, &tmp_y, arg);

        AOCSVecFourCalculus(&tmp_y, 3, 1.0,y, h*3.0/40.0,&k1, h*9.0/40.0,&k2);
        fun(&k3, t+h*3.0/10.0, &tmp_y, arg);

        AOCSVecFourCalculus(&tmp_y, 4, 1.0,y, h*44.0/45.0,&k1, h*(-56.0/15.0),&k2, h*32.0/9.0,&k3);
        fun(&k4, t+h*4.0/5.0, &tmp_y, arg);

        AOCSVecFourCalculus(&tmp_y, 5, 1.0,y, h*19372.0/6561.0,&k1, h*(-25360.0/2187.0),&k2, h*64448.0/6561.0,&k3, h*(-212.0/729.0),&k4);
        fun(&k5, t+h*8.0/9.0, &tmp_y, arg);

        AOCSVecFourCalculus(&tmp_y, 6, 1.0,y, h*(9017.0/3168.0),&k1, h*(-355.0/33.0),&k2, h*(46732.0/5247.0),&k3, (49.0/176.0),&k4, h*(-5103.0/18656.0),&k5);
        fun(&k6, t+h, &tmp_y, arg);

        AOCSVecFourCalculus(&u5, 6, 1.0,y, h*(35.0/384.0),&k1, h*(500.0/1113.0),&k3,h*(125.0/192.0),&k4, h*(-2187.0/6784.0),&k5, h*(11.0/84.0),&k6);

        fun(&k7, t+h, &u5, arg);

        AOCSVecFourCalculus(&u4, 7, 1.0,y,  h*(5179.0/57600.0),&k1, h*(7571.0/16695.0),&k3, h*(393.0/640.0),&k4, h*(-92097.0/339200.0),&k5, h*(187.0/2100.0),&k6, h*1.0/40.0,&k7);

//      L_infノルムの計算
        AOCSVecFourCalculus(&tmp_y,2, 1.0,&u5, -1.0,&u4);
        AOCStype u_err = AOCSVecMax(&tmp_y);
//      ステップサイズ調整

        if(u_err == 0){
            h_new = a_max * h;
        }
        else{
            h_new = max(h * min(a_max, max(a_min, b * pow(tol/u_err, 1.0/5.0))),0.05);//h=0.05s以下は計算時間的に無理なので，下限を設定
        }
        // if truncation error bigger than toleranc, try again
        if(u_err > tol){
            h = h_new;
            try_num++;
            if(try_num>options->max_try){
                //u_errが許容誤差より大きすぎて試行が多すぎるとき
                return INTEGRATER_FAILED;
            }
            continue;
        }
        //last stepsize cannnot overshoot T
        h = min(tf-t, h_new);
        if(h < 0){
            h = options->orbset_h;
        }
        t += h;
        AOCSVecUpdate(y, &u5);

        if(tf-t<=1e-30){
            options->h = h;
            return INTEGRATER_SUCCESS;
        }
        i++;
    }
    return INTEGRATER_FAILED;
}
