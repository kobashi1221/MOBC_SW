/*
 * ODS_cmd_ORBEKF.cpp
 *  Created on: 2022/07/04
 *      Author: Noro
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "ODS_cmd_ORBEKF.h"
#include "ODS_cmd_ORBPROP.h"
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/AOCS/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Drivers/AOCS/LinearAlgebra/LAfunction.h>//hファイルではインクルードしないこと　Noro
#include <src_user/Library/General/DBG_printf.h>

static ORBEKF_STRUCT orbekf;

static void ORBEKF_load_param(void);
static void ORBEKF_initialize_param(void);

static void ODS_ORBEKF_init_(void);
static void ODS_ORBEKF_update_(void);

void ODS_ORBEKF_Time_update(void);
void ODS_ORBEKF_Connect_Param(void);

//MRAMパラメータのROM保存変数（デフォルト）
const int    integrate_nmax = 500;//最大繰り返し回数 [-]
const int    integrate_maxtry = 10;//最大時間幅変更回数[-]
const AOCStype integrate_h = 10.0;//初期時間刻み幅 [s] DOPは可変，RKはこのまま
const AOCStype integrate_tol = 10;//許容誤差[-] 10mの誤差を許容

const AOCStype orbenv_rho_air = rho_air_0;//標準大気密度
const AOCStype orbenv_rho_air_scale = 1.0;//標準大気密度
const AOCStype orbenv_B_scalecoef = 1.0;//弾道係数のTLEからのスケール誤差（能動変化も含む）
const AOCStype orbenv_B_SRP = 1.1*0.01/1.2;//標準大気密度
const AOCStype orbenv_B_SRP_scale = 1.0;//弾道係数のTLEからのスケール誤差（能動変化も含む）

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo ORBEKF_param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_IGRF", ORBEKF_load_param, ORBEKF_initialize_param);
}

AppInfo ORBEKF_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_IGRF", ODS_ORBEKF_init_, ODS_ORBEKF_update_);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void ORBEKF_load_param(void)
{

    //積分の設定
//  read_param(&orbprop.options.nmax, &integrate_nmax, (size_t)sizeof(integrate_nmax),(uint32_t)0x000080d0);
//  read_param(&orbprop.options.max_try, &integrate_maxtry, (size_t)sizeof(integrate_maxtry),(uint32_t)0x000080d4);
//  read_param(&orbprop.options.h, &integrate_h, (size_t)sizeof(integrate_h),(uint32_t)0x000080d8);
//  read_param(&orbprop.options.tol, &integrate_tol, (size_t)sizeof(integrate_tol),(uint32_t)0x000080e0);
    printf("ORBEKF_load_param\r\n");
}

/**
 * @brief アプリケーション実行関数
 *
 */
static void ORBEKF_initialize_param(void)
{
    //mram初期化？？ mramからのよみこみチェックのため、某衛星のTLEを使用する
    printf("ORBEKF_init_param\r\n");

    //積分の設定
//  write_param(&orbprop.options.nmax, &integrate_nmax, (size_t)sizeof(integrate_nmax),(uint32_t)0x000080d0);
//  write_param(&orbprop.options.max_try, &integrate_maxtry, (size_t)sizeof(integrate_maxtry),(uint32_t)0x000080d4);
//  write_param(&orbprop.options.h, &integrate_h, (size_t)sizeof(integrate_h),(uint32_t)0x000080d8);
//  write_param(&orbprop.options.tol, &integrate_tol, (size_t)sizeof(integrate_tol),(uint32_t)0x000080e0);

}

static void ODS_ORBEKF_init_(void){
    g_ods.p_ORBEKF = &orbekf;
    set_size(orbekf.ekf.y_,6);

    ODS_ORBEKF_Time_update();
    ODS_ORBEKF_Connect_Param();//この中でORBPROPの構造体使ってyも更新されてる
    orbekf.ekf.t=(AOCStype) orbekf.last_recv_rtc.d_unix;
    orbekf.ekf.dt = 1.0;//絶対更新されるので気にする必要がないが，未定義のままなのは気持ち悪いので定義しておく
    //観測行列
    AOCSVector sigma_gps = AOCSVectorHelper(6);
    AOCSVector sigma_sys = AOCSVectorHelper(3);
    AOCSVector sigma_cov = AOCSVectorHelper(6);
    for(int i=0;i<3;i++){
        //OrionB16のパフォーマンスシートより
        sigma_gps.arr[i] = 0.4; //
        sigma_gps.arr[i+3] = 0.06;
        //空力はこれくらいなのでシステムノイズもこれくらいで入れておく．
        sigma_sys.arr[i] = 1e-7;
        //誤差共分散行列
        sigma_cov.arr[i]= 1.0;
        sigma_cov.arr[i+3]=1.0;
    }
    Set_SysNoises(&orbekf.ekf.std, &sigma_sys, &sigma_gps);
    AOCSMatrix tmp;
    bool ret = inverse(tmp,orbekf.ekf.std.R);
    ret = inverse(tmp,orbekf.ekf.std.Q);
    //EKFのドライバ用に今回のGPSでの観測問題を繋ぐ処理
    Set_SysFunction(&orbekf.fun, OrbitDynamics, OrbitJacobian, OrbitSysnoiseJacobian, GPSobs, GPSobsJacobian);

    //時間更新から入るので，初期値は事後推定量・共分散に入れる
    //誤差共分散行列
    Set_CovMatrix(&orbekf.ekf.P, &sigma_cov);
    ODS_ORBEKF_update_();
}

static void ODS_ORBEKF_update_(void)
{
    //SGP4⇒ORBPROP⇒GPSって順番にODSの中身を更新する

    AOCSVector z = AOCSVectorHelper(6);
    float r[3],v[3];

    //現在時刻の更新
    ODS_ORBEKF_Time_update();

    //GPSが最新の場合観測更新を行う
  if(g_sensor.p_gps->last_recv_time==TMGR_get_master_total_cycle()){
//    if(1){
        orbekf.ekf.obs_flag = 1;//ON

        //dtはGPSの最終受信時刻（ms精度）-前のループの時間が入る
        //観測が入った時は，OD_EKFの時間更新がGPSの受信時刻までで止まる．
        //⇒時間更新と観測が同じ時間になるので．時刻ずれを考えなくてよい！！！
        orbekf.ekf.dt =((AOCStype) g_sensor.p_gps->last_recv_rtc.d_unix) - orbekf.ekf.t;
        //orbekf.ekf.dt=(AOCStype) 2.0;
        //観測値
        for(int i=0;i<3;i++){
            z.arr[i] = (AOCStype) g_sensor.p_gps->ECI[i];
            z.arr[i+3] = (AOCStype) g_sensor.p_gps->Vel_ECI[i];
        }
        //z.arr[0]=sqrt(pow(orbekf.ekf.y.arr[0],2)+pow(orbekf.ekf.y.arr[1],2)+pow(orbekf.ekf.y.arr[3],2));
        //z.arr[5]=8000.0;
        Set_Observation(&orbekf.ekf, &z);
      printf("GPS propagate \n\r");
//      printf("ORBPROP orbprop.current_y.arr[[m] x=%f, y=%f, z=%f.\n", orbprop.current_y.arr[0],orbprop.current_y.arr[1],orbprop.current_y.arr[2]);
//      printf("ORBPROP orbprop.current_y[m/s] vx=%f, vy=%f, vz=%f.\n", orbprop.current_y.arr[3], orbprop.current_y.arr[4], orbprop.current_y.arr[5]);
//      printf("ORBPROP orbprop.y0.arr[[m] x=%f, y=%f, z=%f.\n", y0.arr[0],y0.arr[1],y0.arr[2]);
//      printf("ORBPROP orbprop.y0[m/s] vx=%f, vy=%f, vz=%f.\n", y0.arr[3], y0.arr[4], y0.arr[5]);
    }
    else{//GPSの更新が入っていないとき何もしなくていい
        //dtは今の時間-前のループの時間が入る
        orbekf.ekf.dt =((AOCStype) orbekf.last_recv_rtc.d_unix) - orbekf.ekf.t;
    }
    //天文衛星のカルマンフィルタと伝搬と観測の順番をわざと逆にしている点に注意
    bool ret = OD_EKF(&orbekf.ekf, &orbekf.fun, orbekf.orbenv);
    orbekf.ekf.obs_flag = 0;
    if(ret == INTEGRATER_SUCCESS){
      //計算結果にnanやinfがなければ更新
      if(AOCSIsInforNan(&orbekf.ekf.y_)==false){
          for(int i=0;i<3;i++){
            r[i] =(float) orbekf.ekf.y_.arr[i];
            v[i] =(float) orbekf.ekf.y_.arr[i+3];
          }
          GLOBAL_ODS_set_Position(r);
          GLOBAL_ODS_set_Velocity(v);
          print(SGP4,"ORBEKF pos[m] x=%f, y=%f, z=%f.\n", r[0],r[1],r[2]);
          print(SGP4,"ORBEKF velo[m/s] x=%f, y=%f, z=%f.\n", v[0],v[1],v[2]);
      }
      else{
          print(SGP4,"ORBPROP nan or inf occured\r\n");
      }
    }else if(ret == INTEGRATER_FAILED){
      //copy(orbprop.current_y, y0);//計算が途中で失敗した場合、前回の軌道情報をそのまま代入
      return ;
    }
}

void ODS_ORBEKF_Time_update(void){
    //時刻関連の初期化，ORBPROPの監視用
    //orbekf.ekf.tは観測の時刻ずれとかに上手く対応するためにOD_EKFでわざと更新しているのでここには入れないこと
    orbekf.last_recv_time = TMGR_get_master_total_cycle();
    Get_RTC_Time_Date(&orbekf.last_recv_rtc);

}

void ODS_ORBEKF_Connect_Param(void){
    //ORBPROPで定義されたダイナミクス・積分オプションを共有したい
    //AOCSParameterで管理されるのはReadOnlyのポインタ変数であるため，直接ポインタを取得してORBEKと繋いだ
    ORBPROP_STRUCT* orbprop_ = ODS_ORBPROP_Get_OrbpropPointer();
    orbekf.orbenv = &orbprop_->orbenv;//ダイナミクスの諸元
    orbekf.ekf.options =&orbprop_->options;//積分ソルバーの諸元
    copy(orbekf.ekf.y, orbprop_->current_y);
    //printf("ORBPROP orbekf.ekf.y[[m] x=%f, y=%f, z=%f.\n", orbekf.ekf.y[0],orbekf.ekf.y[1],orbekf.ekf.y[2]);
}




