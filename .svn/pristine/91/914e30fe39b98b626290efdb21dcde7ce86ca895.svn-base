/*
 * Orbit.cpp
 *
 *  Created on: 2021/09/26
 *      Author: Inakawa
 *      Reference: （TEME座標系からJ2000座標系変換部分）
 *      　　　　　　　　　https://celestrak.org/publications/AIAA/2006-6753/AIAA-2006-6753-Rev3.pdf
 *                 https://github.com/egemenimre/satstuff/blob/master/notebooks/astropy/sgp4_teme_astropy.ipynb
 *                 Pythonのskyfield.sgp4libモジュール
 *                 by Tamaoki
 */

#include "sgp4.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //for uint8_t

#include "AOCScalc.h"
#include "GST.h" // for debug

static GST_CLASS GST_Class; // for debug

//軌道計算を行う。ここでは設定されたTLEからSGP4で伝搬し、位置と速度を計算しクラスに格納することとする

//======MRAM格納パラメータの実体宣言部======//
constexpr int  SGP4_CLASS::ep_year_;    /* Year of epoch, e.g. 94 for 1994, 100 for 2000AD */
constexpr double  SGP4_CLASS::ep_day_;  /* Day of epoch from 00:00 Jan 1st ( = 1.0 ) */
constexpr double  SGP4_CLASS::rev_;     /* Mean motion, revolutions per day */
constexpr double  SGP4_CLASS::bstar_;   /* Drag term .*/
constexpr double  SGP4_CLASS::eqinc_;   /* Equatorial inclination, deg */
constexpr double  SGP4_CLASS::ecc_;     /* Eccentricity */
constexpr double  SGP4_CLASS::mnan_;    /* Mean anomaly at epoch from elements, deg */
constexpr double  SGP4_CLASS::argp_;    /* Argument of perigee, deg */
constexpr double  SGP4_CLASS::ascn_;    /* Right ascension (ascending node), deg */

constexpr int  SGP4_CLASS::ep_year_opponent_;   /* Year of epoch, e.g. 94 for 1994, 100 for 2000AD */
constexpr double  SGP4_CLASS::ep_day_opponent_; /* Day of epoch from 00:00 Jan 1st ( = 1.0 ) */
constexpr double  SGP4_CLASS::rev_opponent_;        /* Mean motion, revolutions per day */
constexpr double  SGP4_CLASS::bstar_opponent_;  /* Drag term .*/
constexpr double  SGP4_CLASS::eqinc_opponent_;  /* Equatorial inclination, deg */
constexpr double  SGP4_CLASS::ecc_opponent_;        /* Eccentricity */
constexpr double  SGP4_CLASS::mnan_opponent_;   /* Mean anomaly at epoch from elements, deg */
constexpr double  SGP4_CLASS::argp_opponent_;   /* Argument of perigee, deg */
constexpr double  SGP4_CLASS::ascn_opponent_;   /* Right ascension (ascending node), deg */
//-----------------------------------------
// ADCS　計算関数宣言
//-----------------------------------------

/**
  * @brief 軌道計算に必要な情報の設定と計算への反映
  * ライブラリを汎用的な入出力に直すラッパ関数
  * @return
  */
void  SGP4_CLASS::sgp4_init(
        int     in_ep_year, /* Year of epoch, e.g. 94 for 1994, 100 for 2000AD */
        double  in_ep_day,  /* Day of epoch from 00:00 Jan 1st ( = 1.0 ) */
        double  in_rev,     /* Mean motion, revolutions per day */
        double  in_bstar,   /* Drag term .*/
        double  in_eqinc,   /* Equatorial inclination, radians */
        double  in_ecc,     /* Eccentricity */
        double  in_mnan,    /* Mean anomaly at epoch from elements, radians */
        double  in_argp,    /* Argument of perigee, radians */
        double  in_ascn /* Right ascension (ascending node), radians */)
{
    this->orbit_elements.ep_year = in_ep_year;
    this->orbit_elements.ep_day = in_ep_day;
    this->orbit_elements.rev = in_rev;
    this->orbit_elements.bstar = in_bstar;
    this->orbit_elements.eqinc = in_eqinc;
    this->orbit_elements.ecc = in_ecc;
    this->orbit_elements.mnan = in_mnan;
    this->orbit_elements.argp = in_argp;
    this->orbit_elements.ascn = in_ascn;

//  this->orbit_elements.satno = in_satno;

    init_sgdp4(&this->orbit_elements);
}

/**
  * @brief  修正ユリウス日と事前設定されたTLE情報から、SGP4で位置と速度を計算する
  * ライブラリを汎用的な配列での入出力に直すラッパ関数
  * @param  in_modified_julian_date  double  修正ユリウス日(秒単位まで対応する)
  * @param  *out_pos float 慣性系位置（km）
  * @param  *out_vel float 慣性系速度（km/s）
  * @return
  */
void SGP4_CLASS::sgp4(double in_modified_julian_date, float out_pos[3], float out_vel[3])
{
    xyz_t pos;
    xyz_t vel;
    satpos_xyz(in_modified_julian_date, &pos, &vel);
    out_pos[0] = (float)pos.x;
    out_pos[1] = (float)pos.y;
    out_pos[2] = (float)pos.z;
    out_vel[0] = (float)vel.x;
    out_vel[1] = (float)vel.y;
    out_vel[2] = (float)vel.z;

}



//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

/**
  * @brief 軌道計算に必要な情報の設定と計算への反映
  * 本クラスのメンバ変数を構造体に格納する　deg to rad 変換含む
  * メンバ変数に適切な値がセットされている前提
  * @return
  */
void  SGP4_CLASS::sgp4_setparam(void)
{
    this->orbit_elements.ep_year = this->ep_year;
    this->orbit_elements.ep_day = this->ep_day;
    this->orbit_elements.rev = this->rev;
    this->orbit_elements.bstar = this->bstar;
    this->orbit_elements.eqinc = this->eqinc / 180.0*M_PI;
    this->orbit_elements.ecc = this->ecc;
    this->orbit_elements.mnan = this->mnan / 180.0*M_PI;
    this->orbit_elements.argp = this->argp / 180.0*M_PI;
    this->orbit_elements.ascn = this->ascn / 180.0*M_PI;

//  this->orbit_elements.satno = in_satno;

    init_sgdp4(&this->orbit_elements);
}

void  SGP4_CLASS::sgp4_opponent_setparam(void)
{
    this->orbit_elements.ep_year = this->ep_year_opponent;
    this->orbit_elements.ep_day = this->ep_day_opponent;
    this->orbit_elements.rev = this->rev_opponent;
    this->orbit_elements.bstar = this->bstar_opponent;
    this->orbit_elements.eqinc = this->eqinc_opponent / 180.0*M_PI;
    this->orbit_elements.ecc = this->ecc_opponent;
    this->orbit_elements.mnan = this->mnan_opponent / 180.0*M_PI;
    this->orbit_elements.argp = this->argp_opponent / 180.0*M_PI;
    this->orbit_elements.ascn = this->ascn_opponent / 180.0*M_PI;

//  this->orbit_elements.satno = in_satno;

    init_sgdp4(&this->orbit_elements);
}

/**
  * @brief 軌道計算と結果のメンバ変数（配列）への格納
  * @return
  */
void SGP4_CLASS::sgp4_calc(double in_modified_julian_date)
{
    this->sgp4(in_modified_julian_date, this->sgp4_position, this->sgp4_velocity);

    float p_Position_teme[3] = {0.0f};
    float p_Velocity_teme[3] = {0.0f};

    for (int i = 0; i < 3; i++) { // convert km ->m
      p_Position_teme[i] = this->sgp4_position[i] * 1000.0f;
      p_Velocity_teme[i] = this->sgp4_velocity[i] * 1000.0f * 86400.0f; // skyfieldではなぜか86400かけて後で割ってる
    }

//    printf("Position_TEME:%f, %f, %f\r\n", p_Position_teme[0], p_Position_teme[1], p_Position_teme[2]);
//    printf("Velocity_TEME:%f, %f, %f\r\n", p_Velocity_teme[0]/86400.0f, p_Velocity_teme[1]/86400.0f, p_Velocity_teme[2]/86400.0f);
    // TEME座標系からJ2000座標系(ICRF座標系)への変換 //m, m/s
    TEME2J2000(in_modified_julian_date, p_Position_teme, p_Velocity_teme, this->p_Position_icrf, this->p_Velocity_icrf);
//    printf("p_Position_icrf:%f, %f, %f\r\n", p_Position_icrf[0], p_Position_icrf[1], p_Position_icrf[2]);
//    printf("p_Velocity_icrf:%f, %f, %f\r\n", p_Velocity_icrf[0], p_Velocity_icrf[1], p_Velocity_icrf[2]);

}

/**
 * @brief TEME座標系からJ2000座標系に変換する J2000座標系=ICRF座標系=GCRF座標系ととみなす
 * @param　in_modified_julian_date 修正ユリウス日(秒単位まで対応する) [day]
 * @param in_p_Pos_teme TEME座標系の位置x,y,z [m]
 * @param　in_p_Pos_teme TEME座標系の速度vx,vy,vz [m/s]
 * @param out_p_Pos_j2000 J2000座標系の位置x,y,z [m]
 * @param　out_p_Vel_j2000 J2000座標系の速度vx,vy,vz [m/s]
 * @return　なし
 * 作成者:玉置 2023/10/12
 */
void SGP4_CLASS::TEME2J2000(double in_modified_julian_date, float in_p_Pos_teme[3], float in_p_Vel_teme[3], float out_p_Pos_j2000[3], float out_p_Vel_j2000[3])
{
  float p_Position_itrf[3] = {0.0f}; // ITRF座標系の位置x,y,z [m]
  float p_Velocity_itrf[3] = {0.0f}; // ITRF座標系の速度vx,vy,vz [m/s]

  //TEME座標系からITRF座標系への変換
  TEME2ITRF(in_modified_julian_date, in_p_Pos_teme, in_p_Vel_teme, p_Position_itrf, p_Velocity_itrf);
//  printf("Position_ITRF:%f, %f, %f\r\n", p_Position_itrf[0], p_Position_itrf[1], p_Position_itrf[2]);
//  printf("Velocity_ITRF:%f, %f, %f\r\n", p_Velocity_itrf[0], p_Velocity_itrf[1], p_Velocity_itrf[2]);

  //ITRF座標系からJ2000座標系への変換 ref: https://g-spatial.com/knowledge/serialization/rensai/%e7%ac%ac4%e5%9b%9e%e3%80%80%e5%a4%a9%e4%bd%93%e5%ba%a7%e6%a8%99%e7%b3%bb%e3%81%a8%e5%9c%b0%e7%90%83%e5%ba%a7%e6%a8%99%e7%b3%bb%e3%81%a8%e3%81%ae%e9%96%a2%e4%bf%82/
  ITRF2GCRF(in_modified_julian_date, p_Position_itrf, p_Velocity_itrf, out_p_Pos_j2000, out_p_Vel_j2000);
}

/**
 * @brief TEME座標系からITRF座標系に変換する
 * @param　in_modified_julian_date 修正ユリウス日(秒単位まで対応する) [day]
 * @param in_p_Pos_teme TEME座標系の位置x,y,z [m]
 * @param　in_p_Vel_teme TEME座標系の速度vx,vy,vz [m/s]
 * @param out_p_Pos_itrf ITRF座標系の位置x,y,z [m]
 * @param　out_p_Vel_itrf ITRF座標系の速度vx,vy,vz [m/s]
 * @return　なし
 * 作成者:玉置 2023/10/12
 */
void SGP4_CLASS::TEME2ITRF(double in_modified_julian_date, float in_p_Pos_teme[3], float in_p_Vel_teme[3], float out_p_Pos_itrf[3], float out_p_Vel_itrf[3])
{
  float gmst_angle = 0.0f;            // グリニッジ恒星時(GMST)[rad]
  float gmst_angular_velocity = 0.0f; // 論文とか見ると7.2921151467e-5 rad/sぐらい
  float xp = 0.0f;                    // x軸まわりの極運動に関する項
  float yp = 0.0f;                    // y軸まわりの極運動に関する項

  float p_Omega[3] = {0.0f};                     // 地球の平均自転角速度[rad/s]
  float p_Omega_cross_r_pef[3] = {0.0f};        // 自転角速度分のオフセット
  float p_MAT_TEME2PEF[3][3] = {{0.0f}};         // TEME座標系からPEF座標系に変換する回転行列
  float p_MAT_TEME2PEF_times_v_teme[3] = {0.0f}; // 回転行列 * TEME座標系のv
  float p_MAT_POLAR_MOTION_X[3][3] = {{0.0f}};   // x軸極運動回転行列
  float p_MAT_POLAR_MOTION_Y[3][3] = {{0.0f}};   // y軸極運動回転行列
  float p_MAT_W[3][3] = {{0.0f}};                // x軸極運動回転行列 * y軸極運動回転行列

  float p_Pos_pef[3] = {0.0f}; // PEF座標系の位置x,y,z [m] PEF座標系=TIRF座標系とみなす ITRF座標系ではない
  float p_Vel_pef[3] = {0.0f}; // PEF座標系の速度vx,vy,vz [m/s]


  // GMSTの角度と角速度を計算
  calc_GMST_angle_angular_velocity(in_modified_julian_date, gmst_angle, gmst_angular_velocity);

  // 計算した値ではなく論文の値を使用（デバッグ用）
  gmst_angular_velocity = 7.2921151467f * 0.00001f;

  float p_Temp[3] = {0.0f, 0.0f, -1.0f}; //地球の平均自転角速度計算用

  //　地球の平均自転角速度[rad/s]を計算 [0, 0, gmst_angular_velocity]
  CoeffVec(p_Temp, 3, gmst_angular_velocity, p_Omega);

//  printf("gmst_angle:%f\r\n", gmst_angle);

  // TEME座標系からPEF座標系に変換する回転行列 // skyfieldと回転方向が逆回りに定義している
  make_rotate_matrix(2, -gmst_angle, *p_MAT_TEME2PEF);

  // PEF座標系のr = 回転行列 * TEME座標系のr
  multiplication(*p_MAT_TEME2PEF, 3, 3, in_p_Pos_teme, 3, 1, p_Pos_pef, 3, 1);

  // 自転角速度分のオフセットを計算 ω ×　r_PEF
  Outer_product(p_Omega, p_Pos_pef, p_Omega_cross_r_pef);

  // PEF座標系のv = 回転行列 * TEME座標系のv - ω × PEF座標系のr
  multiplication(*p_MAT_TEME2PEF, 3, 3, in_p_Vel_teme, 3, 1, p_MAT_TEME2PEF_times_v_teme, 3, 1);
  sub(p_MAT_TEME2PEF_times_v_teme, 3, 1, p_Omega_cross_r_pef, 3, 1, p_Vel_pef);

//  printf("p_Pos_pef:%f, %f, %f\r\n", p_Pos_pef[0], p_Pos_pef[1], p_Pos_pef[2]);
//  printf("p_Vel_pef:%f, %f, %f\r\n", p_Vel_pef[0], p_Vel_pef[1], p_Vel_pef[2]);


  if (xp == 0.0f && yp == 0.0f) { //極運動を考慮しない場合
    // ITRF座標系のr =  PEF座標系のr
    for (int i = 0; i < 3; i++) {
      out_p_Pos_itrf[i] = p_Pos_pef[i];
    }
    // ITRF座標系のv =  PEF座標系のv
    for (int i = 0; i < 3; i++) {
      out_p_Vel_itrf[i] = p_Vel_pef[i];
    }
  }
  else { //極運動を考慮する場合
    // x軸極運動回転行列を生成
    make_rotate_matrix(0, yp, *p_MAT_POLAR_MOTION_X); // TODO:回転方向確認

    // y軸極運動回転行列を生成
    make_rotate_matrix(1, xp, *p_MAT_POLAR_MOTION_Y); // TODO:回転方向確認

    // W = x軸極運動回転行列 * y軸極運動回転行列
    multiplication(*p_MAT_POLAR_MOTION_X, 3, 3, *p_MAT_POLAR_MOTION_Y, 3, 3, *p_MAT_W, 3, 3);

    // ITRF座標系のr = W * PEF座標系のr
    multiplication(*p_MAT_W, 3, 3, p_Pos_pef, 3, 1, out_p_Pos_itrf, 3, 1);

    // ITRF座標系のv = W * PEF座標系のv
    multiplication(*p_MAT_W, 3, 3, p_Vel_pef, 3, 1, out_p_Vel_itrf, 3, 1);
  }

  for (int i = 0; i < 3; i++) {
    out_p_Vel_itrf[i] = out_p_Vel_itrf[i] / 86400.0f; // skyfieldではなぜか86400かけて後で割ってる
  }

}

/**
 * @brief グリニッジ恒星時(GMST)の角度と角速度を計算する
 * @param　in_modified_julian_date 修正ユリウス日(秒単位まで対応する) [day]
 * @param &out_gmst_angle グリニッジ恒星時(GMST) [rad] ポインタとしてではなく参照として宣言する｡これで、参照に加えられた変更は元の変数自体に反映される｡
 * @param　&out_gmst_angular_velocity グリニッジ恒星時(GMST)の角速度 [rad/s]
 * @return　なし
 * 作成者:玉置 2023/10/12
 */
void SGP4_CLASS::calc_GMST_angle_angular_velocity(double in_modified_julian_date, float &out_gmst_angle, float &out_gmst_angular_velocity)
{
/**
  Return the angle of Greenwich Mean Standard Time 1982 given the JD.

  This angle defines the difference between the idiosyncratic True
  Equator Mean Equinox (TEME) frame of reference used by SGP4 and the
  more standard Pseudo Earth Fixed (PEF) frame of reference.  The UT1
  time should be provided as a Julian date.  Theta is returned in
  radians, and its velocity in radians per day of UT1 time.

  From AIAA 2006-6753 Appendix C.
*/

  // 2000/1/1 UT1 12:00を基準とした時間（36525day単位）
  double ut1 = (in_modified_julian_date - JD2000) / 36525.0;
//  printf("in_modified_julian_date:%lf\r\n", in_modified_julian_date);
//  printf("ut1:%lf\r\n", ut1);

  // 日（or秒?）単位でのIAU1982モデルのグリニッジ恒星時(GMST)を計算 //876600.0 * 3600これがおかしい？
//  double gmst_time = 67310.54841 + (876600.0 * 3600 + 8640184.812866)* ut1 + 0.093104 * ut1 * ut1 - 0.0000062 * ut1 * ut1 * ut1; // sec
//  double gmst_time = 67310.54841 + (8640184.812866)* ut1 + 0.093104 * ut1 * ut1 - 0.0000062 * ut1 * ut1 * ut1; // sec
  double gmst_time = 67310.54841 + (8640184.812866 + (0.093104 + (-6.2e-6) * ut1) * ut1) * ut1;
//  printf("gmst_time:%lf\r\n", gmst_time);

  // GMSTをut1で微分
//  double dot_gmst_time = 876600.0 * 3600 + 8640184.812866 + 2.0 * 0.093104 * ut1 - 3.0 * 0.0000062 * ut1 * ut1; // sec/36525day?
  double dot_gmst_time = 8640184.812866 + 2.0 * 0.093104 * ut1 - 3.0 * 0.0000062 * ut1 * ut1; // sec/36525day?
//  printf("dot_gmst_time:%f\r\n", dot_gmst_time);

  // GMSTをrad単位に変換
//  out_gmst_angle = fmod(gmst_time * M_PI /180 /240, 2 * M_PI); // rad  //360/86400 = 1/240
    out_gmst_angle = fmod(fmod(in_modified_julian_date + 2400000.5, 1.0f) + 0.0f + fmod(gmst_time / 86400, 1.0f), 1.0f) * 2 * M_PI ; // 2400000.5 修正ユリウス日→ユリウス日
//  out_gmst_angle = fmod(gmst_time * M_PI /180 /480, 2 * M_PI); // rad
//    printf("fmod(ut1, 1.0f):%f\r\n", fmod(ut1, 1.0f));
//    printf("fmod(gmst_time / 86400, 1.0f):%f\r\n", fmod(gmst_time / 86400, 1.0f));
//    printf("fmod(fmod(in_modified_julian_date, 1.0f) + 0.0f + fmod(gmst_time / 86400, 1.0f), 1.0f):%f\r\n", fmod(fmod(in_modified_julian_date, 1.0f) + 0.0f + fmod(gmst_time / 86400, 1.0f), 1.0f)));
//    printf("out_gmst_angle:%f\r\n", out_gmst_angle);

  // 0 <= GMST < 2π に収まるように処理
  if(out_gmst_angle < 0){
    out_gmst_angle = out_gmst_angle + 2 * M_PI;
  }
  this->gmst_angle = out_gmst_angle; //ITRF→GCRF用

  // GMSTの角速度を計算
//  out_gmst_angular_velocity = (1.0 + dot_gmst_time / (86400.0 * 36525.0)) * 2 * M_PI; // rad/day?
  out_gmst_angular_velocity = (1.0 + dot_gmst_time / 36525.0) * 2 * M_PI; // rad/sec? なんで1.0を足すのか

}

/**
 * @brief ITRF座標系からICRF=GCRF座標系に変換する
 * @param in_p_Pos_itrf  ITRF座標系の位置x,y,z [m]
 * @param　in_p_Vel_itrf  ITRF座標系の速度vx,vy,vz [m/s]
 * @param　out_p_Pos_gcrf GCRF座標系の位置x,y,z [m]
 * @param　out_p_Vel_gcrf GCRF座標系の速度vx,vy,vz [m/s]
 * @return　なし
 * 作成者:玉置 2023/10/18
 */
void SGP4_CLASS::ITRF2GCRF(double in_modified_julian_date, float in_p_Pos_itrf[3], float in_p_Vel_itrf[3], float out_p_Pos_gcrf[3], float out_p_Vel_gcrf[3])
{

  //2000/1/1 UT1 12:00を基準とした時間（36525day単位）
  double T = (in_modified_julian_date -  51544.50000) / 36525.0;

  //日単位での時角の計算
//  double Tg = (24110.54841 + 8640184.812866 * T + 0.093104 * T * T - 0.0000062 * T * T * T) / 86400.0;
  double Tg = 67310.54841 + (876600.0 * 3600 +8640184.812866)* T + 0.093104 * T * T - 0.0000062 * T * T * T;

  //ラジアン単位に変換する
//  double gst_rad = 2 * M_PI * (Tg - int(Tg));
  double gst_rad = fmod(Tg * M_PI /180 /240, 2 * M_PI);
//  printf("gst_rad:%lf\r\n", gst_rad);

  if(gst_rad < 0){
      gst_rad = gst_rad + 2 * M_PI;
  }

  // ITRF座標系(ECEF)からGCRF座標系(ECI)への変換
  Rotate_one_axis(in_p_Pos_itrf, 1, 2, -gst_rad, out_p_Pos_gcrf);
  Rotate_one_axis(in_p_Vel_itrf, 1, 2, -gst_rad, out_p_Vel_gcrf);

}
