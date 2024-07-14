/*
 * Orbit.h
 *
 *  Created on: 2021/09/26
 *      Author: Inakawa
 */

#ifndef C2A_AOCS_SGP4_H_
#define C2A_AOCS_SGP4_H_

#include "sgdp4h.h"

// クラス宣言
//-----------------------------------------
class SGP4_CLASS
{
private:
    //-----------------------------------------
    // ADCS　計算用変数宣言
    //-----------------------------------------

    //軌道要素など TLEの情報に平均運動の微分などの情報が追加されている。
    //初期化するのはTLEの情報で構わない
    orbit_t orbit_elements;

    //-----------------------------------------
    // ADCS　計算関数宣言
    //-----------------------------------------

    // 実際にADCS系の計算を行う関数を宣言
    // 入出力は全て引数として定義（出力引数はポインタ渡し）
    //outはkm,km/s単位,floatで出力する
    void sgp4(double in_modified_julian_date, float out_pos[3], float out_vel[3]);
    void TEME2J2000(double in_modified_julian_date, float in_p_Pos_teme[3], float in_p_Vel_teme[3], float out_p_Pos_j2000[3], float out_p_Vel_j2000[3]);
    void TEME2ITRF(double in_modified_julian_date, float in_p_Pos_teme[3], float in_p_Vel_teme[3], float out_p_Pos_itrf[3], float out_p_Vel_itrf[3]);
    void calc_GMST_angle_angular_velocity(double in_modified_julian_date, float &out_gmst_angle, float &out_gmst_angular_velocity);
    void ITRF2GCRF(double in_modified_julian_date, float in_p_Pos_itrf[3], float in_p_Vel_itrf[3], float out_p_Pos_gcrf[3], float out_p_Vel_gcrf[3]);

    void sgp4_init(
            int     in_ep_year, /* Year of epoch, e.g. 94 for 1994, 100 for 2000AD */
            double  in_ep_day,  /* Day of epoch from 00:00 Jan 1st ( = 1.0 ) */
            double  in_rev,     /* Mean motion, revolutions per day */
            double  in_bstar,   /* Drag term .*/
            double  in_eqinc,   /* Equatorial inclination, radians */
            double  in_ecc,     /* Eccentricity */
            double  in_mnan,    /* Mean anomaly at epoch from elements, radians */
            double  in_argp,    /* Argument of perigee, radians */
            double  in_ascn /* Right ascension (ascending node), radians */
            //double    smjaxs, /* Semi-major axis, km */
            //double  ndot2,
            //double  nddot6,  /* Mean motion derivatives */
            //char    desig[10], /* International designation */
            //long  norb,   /* Orbit number, for elements */
            //int       in_satno    /* Satellite number. */
            );



public:
    //-----------------------------------------
    // 衛星ソフトウェア依存変数宣言
    //-----------------------------------------

    // 主には出力ベクトル、配列を宣言
    // float B_geodetic[3]; //[0]:B_north [1]:B_east [2]:B_down

    //Magnaroでは、Mramへの値格納、読み出しのため、ROMに格納する初期値（プログラムベタ打ちの値（_あり配列））と
    //RAMに展開される実際の計算に使う値(_なし配列)の二つの値を宣言しておき、プログラム起動時に_あり配列から_なし配列にコピーする。
    //さらに、MRAMに格納した値を使用する場合は、MRAMの値を_なしの配列にコピーする。
    //メンバ定数にするにはstatic constが便利なので今回はこれを利用する

    int     ep_year; /* Year of epoch, e.g. 94 for 1994, 100 for 2000AD */
    double  ep_day; /* Day of epoch from 00:00 Jan 1st ( = 1.0 ) */
    double  rev;    /* Mean motion, revolutions per day */
    double  bstar;  /* Drag term .*/
    double  eqinc;  /* Equatorial inclination, deg */
    double  ecc;    /* Eccentricity */
    double  mnan;   /* Mean anomaly at epoch from elements, deg */
    double  argp;   /* Argument of perigee, deg */
    double  ascn;   /* Right ascension (ascending node), deg */


    static constexpr int ep_year_ = 121;            /* Year of epoch, e.g. 94 for 1994, 100 for 2000AD *///121;
    static constexpr double ep_day_ = 313.08812500; /* Day of epoch from 00:00 Jan 1st ( = 1.0 ) *///268.77854633;
    static constexpr double rev_ = 15.02586955;     /* Mean motion, revolutions per day *///15.17196292;
    static constexpr double bstar_ = 0.10270 / 1000;    /* Drag term .*///0.61111 / 10000;
    static constexpr double eqinc_ = 97.5940;       /* Equatorial inclination, deg *///97.4579;
    static constexpr double ecc_ =  0.0020612;      /* Eccentricity *///0.0012821;
    static constexpr double mnan_ = 173.8897;   /* Mean anomaly at epoch from elements, deg *///130.7387;
    static constexpr double argp_ = 230.9696;   /* Argument of perigee, deg *///229.2734;230.9696
    static constexpr double ascn_ = 11.8867;    /* Right ascension (ascending node), deg *///330.7688;

    int     ep_year_opponent; /* Year of epoch, e.g. 94 for 1994, 100 for 2000AD */
    double  ep_day_opponent;    /* Day of epoch from 00:00 Jan 1st ( = 1.0 ) */
    double  rev_opponent;   /* Mean motion, revolutions per day */
    double  bstar_opponent; /* Drag term .*/
    double  eqinc_opponent; /* Equatorial inclination, deg */
    double  ecc_opponent;    /* Eccentricity */
    double  mnan_opponent;  /* Mean anomaly at epoch from elements, deg */
    double  argp_opponent;  /* Argument of perigee, deg */
    double  ascn_opponent;  /* Right ascension (ascending node), deg */


    static constexpr int ep_year_opponent_ = 122;           /* Year of epoch, e.g. 94 for 1994, 100 for 2000AD *///121;
    static constexpr double ep_day_opponent_ = 166.01653287;    /* Day of epoch from 00:00 Jan 1st ( = 1.0 ) *///268.77854633;
    static constexpr double rev_opponent_ = 15.50005610344847;      /* Mean motion, revolutions per day *///15.17196292;
    static constexpr double bstar_opponent_ = 0.13275 / 1000;   /* Drag term .*///0.61111 / 10000;
    static constexpr double eqinc_opponent_ = 51.6446;      /* Equatorial inclination, deg *///97.4579;
    static constexpr double ecc_opponent_ =  0.0004306;     /* Eccentricity *///0.0012821;
    static constexpr double mnan_opponent_ = 262.8526;  /* Mean anomaly at epoch from elements, deg *///130.7387;
    static constexpr double argp_opponent_ = 239.1928;  /* Argument of perigee, deg *///229.2734;
    static constexpr double ascn_opponent_ = 347.3387;  /* Right ascension (ascending node), deg *///330.7688;

    //計算結果
    float sgp4_position[3];//km
    float sgp4_velocity[3];//km/s
    float p_Position_icrf[3]; //m
    float p_Velocity_icrf[3]; //m/s

    float gmst_angle; // rad

    //-----------------------------------------
    // 衛星ソフトウェア依存関数宣言
    //-----------------------------------------

    void sgp4_setparam(void);
    void sgp4_opponent_setparam(void);
    void sgp4_calc(double in_modified_julian_date);


};



#endif /* C2A_AOCS_SGP4_H_ */
