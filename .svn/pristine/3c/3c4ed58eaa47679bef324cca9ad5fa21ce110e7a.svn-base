/*
 * ODS_cmd_ORBPROP.cpp
 *
 *  Created on: 2022/07/04
 *      Author: Noro
 */

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Applications/UserDefined/AOCS/ODS/ODS_cmd_ORBPROP.h>
//#include <src_core/System/TimeManager/obc_time.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/AOCS/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Drivers/AOCS/sgp4.h>
#include <src_user/Drivers/AOCS/LinearAlgebra/LAfunction.h>//ヘッダーでインクルードしないこと！
#include <src_user/Library/General/DBG_printf.h>

static ORBPROP_STRUCT orbprop;

static void ORBPROP_load_param(void);
static void ORBPROP_initialize_param(void);

static void ODS_ORBPROP_init_(void);
static void ODS_ORBPROP_update_(void);
static void ODS_ORBPROP_opponent_update_(void);

void ODS_ORBPROP_Time_update(void);
void ODS_ORBPROP_Connect_Param(void);

//MRAMパラメータのROM保存変数（デフォルト）
const int    integrate_nmax = 500;//最大繰り返し回数 [-]
const int    integrate_maxtry = 10;//最大時間幅変更回数[-]
const AOCStype integrate_h = 10.0;//初期時間刻み幅 [s] DOPは可変，RKはこのまま
const AOCStype integrate_tol = 10;//許容誤差[-] 10mの誤差を許容
const double integrate_set_h = 0.1;//最小時間刻み幅 [s]
const step_t integrate_set_timeout = 50;//タイムアウト時間 [ms]

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
AppInfo ORBPROP_param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_IGRF", ORBPROP_load_param, ORBPROP_initialize_param);
}

AppInfo ORBPROP_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_IGRF", ODS_ORBPROP_init_, ODS_ORBPROP_update_);
}

AppInfo ORBPROP_Opponent_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_IGRF", NULL, ODS_ORBPROP_opponent_update_);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void ORBPROP_load_param(void)
{
    //BstarはSGP４のものを引用
    read_param(&orbprop.orbenv.B_star, &SGP4_CLASS::bstar_opponent_, (size_t)sizeof(orbprop.orbenv.B_star), (uint32_t)0x000080a0);
    read_param(&orbprop.orbprop_set.rev, &SGP4_CLASS::rev_, (size_t)sizeof(orbprop.orbprop_set.rev), (uint32_t)0x0000800C);
    read_param(&orbprop.orbprop_set.bstar, &SGP4_CLASS::bstar_, (size_t)sizeof(orbprop.orbprop_set.bstar), (uint32_t)0x00008014);
    read_param(&orbprop.orbprop_set.eqinc, &SGP4_CLASS::eqinc_, (size_t)sizeof(orbprop.orbprop_set.eqinc), (uint32_t)0x0000801C);
    read_param(&orbprop.orbprop_set.ecc, &SGP4_CLASS::ecc_, (size_t)sizeof(orbprop.orbprop_set.ecc), (uint32_t)0x00008024);
    read_param(&orbprop.orbprop_set.mnan, &SGP4_CLASS::mnan_, (size_t)sizeof(orbprop.orbprop_set.mnan), (uint32_t)0x0000802C);
    read_param(&orbprop.orbprop_set.argp, &SGP4_CLASS::argp_, (size_t)sizeof(orbprop.orbprop_set.argp), (uint32_t)0x00008034);
    read_param(&orbprop.orbprop_set.ascn, &SGP4_CLASS::ascn_, (size_t)sizeof(orbprop.orbprop_set.ascn), (uint32_t)0x0000803C);

    //積分の設定
    read_param(&orbprop.options.nmax, &integrate_nmax, (size_t)sizeof(integrate_nmax),(uint32_t)0x000080d0);
    read_param(&orbprop.options.max_try, &integrate_maxtry, (size_t)sizeof(integrate_maxtry),(uint32_t)0x000080d4);
    read_param(&orbprop.options.h, &integrate_h, (size_t)sizeof(integrate_h),(uint32_t)0x000080d8);
    read_param(&orbprop.options.tol, &integrate_tol, (size_t)sizeof(integrate_tol),(uint32_t)0x000080e0);
    read_param(&orbprop.options.orbset_h, &integrate_set_h, (size_t)sizeof(integrate_set_h),(uint32_t)0x00008114);
    read_param(&orbprop.options.timeout, &integrate_set_timeout, (size_t)sizeof(integrate_set_timeout),(uint32_t)0x0000811c);

    //ダイナミクス設定
    read_param(&orbprop.orbenv.rho_air, &orbenv_rho_air, (size_t)sizeof(orbenv_rho_air), (uint32_t)0x000080e8);
    read_param(&orbprop.orbenv.rho_air_scale, &orbenv_rho_air_scale, (size_t)sizeof(orbenv_rho_air_scale), (uint32_t)0x000080f0);
    read_param(&orbprop.orbenv.B_scalecoef, &orbenv_B_scalecoef, (size_t)sizeof(orbenv_B_scalecoef), (uint32_t)0x000080f8);
    read_param(&orbprop.orbenv.B_SRP, &orbenv_B_SRP, (size_t)sizeof(orbprop.orbenv.B_star), (uint32_t)0x00008100);
    read_param(&orbprop.orbenv.B_SRP_scale, &orbenv_B_SRP_scale, (size_t)sizeof(orbenv_B_scalecoef), (uint32_t)0x00008108);
    UpdateBfromBstar(&orbprop.orbenv);
    printf("ORBPROP_load_param\r\n");
}

/**
 * @brief アプリケーション実行関数
 *
 */
static void ORBPROP_initialize_param(void)
{
    //mram初期化？？ mramからのよみこみチェックのため、某衛星のTLEを使用する
    printf("ORBPROP_initialize_param\r\n");
    //SGP４のTLEを引用
    write_param(&orbprop.orbprop_set.rev, &SGP4_CLASS::rev_, (size_t)sizeof(orbprop.orbprop_set.rev), (uint32_t)0x0000800C);
    write_param(&orbprop.orbprop_set.bstar, &SGP4_CLASS::bstar_, (size_t)sizeof(orbprop.orbprop_set.bstar), (uint32_t)0x00008014);
    write_param(&orbprop.orbprop_set.eqinc, &SGP4_CLASS::eqinc_, (size_t)sizeof(orbprop.orbprop_set.eqinc), (uint32_t)0x0000801C);
    write_param(&orbprop.orbprop_set.ecc, &SGP4_CLASS::ecc_, (size_t)sizeof(orbprop.orbprop_set.ecc), (uint32_t)0x00008024);
    write_param(&orbprop.orbprop_set.mnan, &SGP4_CLASS::mnan_, (size_t)sizeof(orbprop.orbprop_set.mnan), (uint32_t)0x0000802C);
    write_param(&orbprop.orbprop_set.argp, &SGP4_CLASS::argp_, (size_t)sizeof(orbprop.orbprop_set.argp), (uint32_t)0x00008034);
    write_param(&orbprop.orbprop_set.ascn, &SGP4_CLASS::ascn_, (size_t)sizeof(orbprop.orbprop_set.ascn), (uint32_t)0x0000803C);

    //積分の設定
    write_param(&orbprop.options.nmax, &integrate_nmax, (size_t)sizeof(integrate_nmax),(uint32_t)0x000080d0);
    write_param(&orbprop.options.max_try, &integrate_maxtry, (size_t)sizeof(integrate_maxtry),(uint32_t)0x000080d4);
    write_param(&orbprop.options.h, &integrate_h, (size_t)sizeof(integrate_h),(uint32_t)0x000080d8);
    write_param(&orbprop.options.tol, &integrate_tol, (size_t)sizeof(integrate_tol),(uint32_t)0x000080e0);
    write_param(&orbprop.options.orbset_h, &integrate_set_h, (size_t)sizeof(integrate_set_h),(uint32_t)0x00008114);
    write_param(&orbprop.options.timeout, &integrate_set_timeout, (size_t)sizeof(integrate_set_timeout),(uint32_t)0x0000811c);

    //ダイナミクス設定
    write_param(&orbprop.orbenv.rho_air, &orbenv_rho_air, (size_t)sizeof(orbenv_rho_air), (uint32_t)0x000080e8);
    write_param(&orbprop.orbenv.rho_air_scale, &orbenv_rho_air_scale, (size_t)sizeof(orbenv_rho_air_scale), (uint32_t)0x000080f0);
    write_param(&orbprop.orbenv.B_scalecoef, &orbenv_B_scalecoef, (size_t)sizeof(orbenv_B_scalecoef), (uint32_t)0x000080f8);
    write_param(&orbprop.orbenv.B_SRP, &orbenv_B_SRP, (size_t)sizeof(orbprop.orbenv.B_star), (uint32_t)0x00008100);
    write_param(&orbprop.orbenv.B_SRP_scale, &orbenv_B_SRP_scale, (size_t)sizeof(orbenv_B_scalecoef), (uint32_t)0x00008108);

}

static void ODS_ORBPROP_init_(void){
    g_ods.p_ORBPROP = &orbprop;
    orbprop.current_y.n = 6;

    ORBPROP_setparam(&orbprop.orbprop_set, &orbprop.current_y);
    for(int i=0;i<3;i++){
        orbprop.current_y.arr[i] =(AOCStype) g_ods.p_ODS->Position[i];
        orbprop.current_y.arr[i+3] =(AOCStype) g_ods.p_ODS->Velocity[i];
    }
//  printf("ORBPROP orbprop.current_y.arr[[m] x=%f, y=%f, z=%f.\n", orbprop.current_y.arr[0],orbprop.current_y.arr[1],orbprop.current_y.arr[2]);
//  printf("ORBPROP orbprop.current_y[m/s] vx=%f, vy=%f, vz=%f.\n", orbprop.current_y.arr[3], orbprop.current_y.arr[4], orbprop.current_y.arr[5]);
//  orbprop.current_y.arr[0] = Re+600.0*1e3;
//  orbprop.current_y.arr[4] = sqrt(mu_e/(Re+600*1e3));

    ODS_ORBPROP_Time_update();
    ODS_ORBPROP_Connect_Param();
}

static void ODS_ORBPROP_update_(void)
{
    //SGP4⇒ORBPROP⇒GPSって順番にODSの中身を更新する
    AOCSVector y0 = AOCSVectorHelper(6);
    float r[3],v[3];
    AOCStype t0, tf;
    //GPSが最新の場合何しない
    if(g_sensor.p_gps->last_recv_time==TMGR_get_master_total_cycle()){
      //printf("GPS on\n\r");
        return;
    }

    //GPSの更新がORBPROPの保持している伝搬情報より新しい場合GPSから情報を持ってくる
    if(g_sensor.p_gps->last_recv_time >= orbprop.last_recv_time){
        t0 = g_sensor.p_gps->last_recv_rtc.d_unix;//GPSの最終受信時刻（ms精度）
        for(int i=0;i<3;i++){
            y0.arr[i] = (AOCStype) g_sensor.p_gps->ECI[i];
            y0.arr[i+3] = (AOCStype) g_sensor.p_gps->Vel_ECI[i];
        }
//      printf("GPS propagate \n\r");
   }
    else{//GPSの更新が入っていない場合前回の軌道伝搬の値から情報を持ってくる
        t0 = orbprop.last_recv_rtc.d_unix;
        copy(y0, orbprop.current_y);//前回の軌道情報をそのまま代入
      //printf("GPS propagate OFF \n\r");
    }

    //最終計算時刻の更新
    ODS_ORBPROP_Time_update();
    tf = orbprop.last_recv_rtc.d_unix;

    //最初は5次の適応時間RK⇒失敗したら時間固定のRK4
    //5次のドルマンプリンスによる数値積分

    INTEGRATER_ACK ret = ODE(&orbprop.current_y, OrbitDynamics, &y0, t0, tf, &orbprop.options, &orbprop.orbenv);
//  if(DOP45(&orbprop.current_y, OrbitDynamics, &y0, t0, tf, &orbprop.options,&orbprop.orbenv)!=INTEGRATER_SUCCESS){
//      //4次のルンゲクッタ
//
//      RK4(&orbprop.current_y, OrbitDynamics, &y0, t0, tf, &orbprop.options, &orbprop.orbenv);
//  }

    //計算結果にnanやinfがなければ更新
    if(AOCSIsInforNan(&orbprop.current_y)==false){
        for(int i=0;i<3;i++){
            r[i] =(float) orbprop.current_y.arr[i];
            v[i] =(float) orbprop.current_y.arr[i+3];


        GLOBAL_ODS_set_Position(r);
        GLOBAL_ODS_set_Velocity(v);
        }
        print(SGP4,"ORBPROP pos[m] x=%f, y=%f, z=%f.\n", r[0],r[1],r[2]);
        print(SGP4,"ORBPROP velo[m/s] x=%f, y=%f, z=%f.\n", v[0],v[1],v[2]);
    }
    else{
        print(SGP4,"ORBPROP nan or inf occured\r\n");
    }
}

static void ODS_ORBPROP_opponent_update_(void)
{
}

void ODS_ORBPROP_Time_update(void){
    //時刻関連の初期化，ORBPROPの監視用
    orbprop.last_recv_time = TMGR_get_master_total_cycle();
    Get_RTC_Time_Date(&orbprop.last_recv_rtc);
}

void ODS_ORBPROP_Connect_Param(void){
    //Global AOCS Parametersのポインタとダイナミクスの構造体をつなげる
    orbprop.orbenv.e_suns = g_acs.p_GLOBAL_AOCS->SunVec;//太陽方向
    orbprop.orbenv.Eclipse_flag = &g_acs.p_GLOBAL_AOCS->Eclipse_flag;//蝕フラグ
}

ORBPROP_STRUCT* ODS_ORBPROP_Get_OrbpropPointer(void){
    //グローバル変数にするのはやりたくない
    //ポインタを渡して向こうのポインタ変数とつなげば向こうでも値を触れる（ベストプラクティスではない感じがする．．．）　Noro
    //ここら辺の情報の授受が場当たり的に作ってしまったので，将来的にはここら辺は統一的に行えるような設計を考えたほうがよさそう
    return &orbprop;
}
