/*
 * OD_EKF.h
 *
 *  ver.1.0 2022/08/21 Noro
 *
 */

#ifndef C2A_AOCS_OD_EKF_H_
#define C2A_AOCS_OD_EKF_H_

#include <src_core/System/TImeManager/time_manager.h>//for delay()

#include <stdarg.h>
#include <stdbool.h>
#include "Physical_Constants.h"
#include "AOCSVector.h"
#include "Integrater.h"

typedef struct{
    AOCSMatrix Q;//システムノイズ
    AOCSMatrix R;//観測ノイズ
} EKF_std;

typedef struct{
    AOCSMatrix P; //事後誤差共分散行列
    AOCSMatrix P_;//事前誤差共分散行列
    AOCSVector y; //事後状態量
    AOCSVector y_;//事前推定状態量
    AOCSVector z; //観測量
    AOCStype t;   //時刻
    AOCStype dt;      //時間幅
    EKF_std std;  //ノイズの分散
    uint8_t obs_flag;//観測出来たかどうか
    IntegraterOptions* options;//時間更新の設定
} EKF_Param;


//EKFの中で利用されるダイナミクス，ヤコビアン，ノイズにおけるヤコビアン，積分ソルバーを定義
//実体はアプリで初期化され，ORBPROPの関数とつなげる！
typedef struct{
    void (*f)(AOCSVector*,  AOCStype, AOCSVector*, void*);//ダイナミクス
    void (*fx)(AOCSMatrix*, AOCStype, AOCSVector*, void*);//ダイナミクスの状態量xでのヤコビアン
    void (*fw)(AOCSMatrix*, AOCStype, AOCSVector*, void*);//ダイナミクスのノイズwでのヤコビアン
    void (*h)(AOCSVector*,  AOCStype, AOCSVector*, void*);//観測方程式
    void (*hx)(AOCSMatrix*, AOCStype, AOCSVector*, void*);//観測方程式の状態量xでのヤコビアン
} EKF_fun;

//C2AのヘッダーにインクルードされるファイルではC++でしか定義できない記法は利用できない
bool OD_EKF(EKF_Param* ekf, EKF_fun* fun, void* arg);
void Set_SysNoises(EKF_std* std, AOCSVector* sigma_sys, AOCSVector* sigma_obs);
void Set_CovMatrix(AOCSMatrix* P_, AOCSVector* ref);
void Set_SysFunction(EKF_fun *fun, void (*f)(AOCSVector*,  AOCStype, AOCSVector*, void*),
        void (*fx)(AOCSMatrix*, AOCStype, AOCSVector*, void*),
        void (*fw)(AOCSMatrix*, AOCStype, AOCSVector*, void*),
        void (*h)(AOCSVector*,  AOCStype, AOCSVector*, void*),
        void (*hx)(AOCSMatrix*, AOCStype, AOCSVector*, void*));
void Set_Observation(EKF_Param* ekf, AOCSVector* z);

#endif /* C2A_AOCS_OD_EKF_H_ */
