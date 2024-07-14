/*
 * ODS_cmd_SGP4.cpp
 *
 *  Created on: 2021/10/28
 *      Author: masud
 */

/*
 * ODS_cmd_sgp4.cpp
 *
 *  Created on: 2021/09/26
 *      Author: inakawa
 */

#include "ODS_cmd_SGP4.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include <src_core/System/TimeManager/obc_time.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/sgp4.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Library/General/DBG_printf.h>

#include <src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.h>  //for GPS Flag

static SGP4_CLASS SGP4_class;
static SGP4_STRUCT SGP4_;

static void SGP4_load_param(void);
static void SGP4_initialize_param(void);

static void ODS_sgp4_update_(void);
static void ODS_sgp4_opponent_update_(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo SGP4_param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_IGRF", SGP4_load_param, SGP4_initialize_param);
}

AppInfo SGP4_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_IGRF", NULL, ODS_sgp4_update_);
}

AppInfo SGP4_Opponent_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_IGRF", NULL, ODS_sgp4_opponent_update_);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void SGP4_load_param(void)
{
	g_ods.p_SGP4 = &SGP4_;
	printf("SGP4_load_param\r\n");
	read_param(&SGP4_class.ep_year, &SGP4_CLASS::ep_year_, (size_t)sizeof(SGP4_class.ep_year), (uint32_t)0x00008000);
	read_param(&SGP4_class.ep_day, &SGP4_CLASS::ep_day_, (size_t)sizeof(SGP4_class.ep_day), (uint32_t)0x00008004);
	read_param(&SGP4_class.rev, &SGP4_CLASS::rev_, (size_t)sizeof(SGP4_class.rev), (uint32_t)0x0000800C);
	read_param(&SGP4_class.bstar, &SGP4_CLASS::bstar_, (size_t)sizeof(SGP4_class.bstar), (uint32_t)0x00008014);
	read_param(&SGP4_class.eqinc, &SGP4_CLASS::eqinc_, (size_t)sizeof(SGP4_class.eqinc), (uint32_t)0x0000801C);
	read_param(&SGP4_class.ecc, &SGP4_CLASS::ecc_, (size_t)sizeof(SGP4_class.ecc), (uint32_t)0x00008024);
	read_param(&SGP4_class.mnan, &SGP4_CLASS::mnan_, (size_t)sizeof(SGP4_class.mnan), (uint32_t)0x0000802C);
	read_param(&SGP4_class.argp, &SGP4_CLASS::argp_, (size_t)sizeof(SGP4_class.argp), (uint32_t)0x00008034);
	read_param(&SGP4_class.ascn, &SGP4_CLASS::ascn_, (size_t)sizeof(SGP4_class.ascn), (uint32_t)0x0000803C);
	read_param(&SGP4_class.ep_year_opponent, &SGP4_CLASS::ep_year_opponent_, (size_t)sizeof(SGP4_class.ep_year_opponent), (uint32_t)0x0000808c);
	read_param(&SGP4_class.ep_day_opponent, &SGP4_CLASS::ep_day_opponent_, (size_t)sizeof(SGP4_class.ep_day_opponent), (uint32_t)0x00008090);
	read_param(&SGP4_class.rev_opponent, &SGP4_CLASS::rev_opponent_, (size_t)sizeof(SGP4_class.rev_opponent), (uint32_t)0x00008098);
	read_param(&SGP4_class.bstar_opponent, &SGP4_CLASS::bstar_opponent_, (size_t)sizeof(SGP4_class.bstar_opponent), (uint32_t)0x000080a0);
	read_param(&SGP4_class.eqinc_opponent, &SGP4_CLASS::eqinc_opponent_, (size_t)sizeof(SGP4_class.eqinc_opponent), (uint32_t)0x000080a8);
	read_param(&SGP4_class.ecc_opponent, &SGP4_CLASS::ecc_opponent_, (size_t)sizeof(SGP4_class.ecc_opponent), (uint32_t)0x000080b0);
	read_param(&SGP4_class.mnan_opponent, &SGP4_CLASS::mnan_opponent_, (size_t)sizeof(SGP4_class.mnan_opponent), (uint32_t)0x000080b8);
	read_param(&SGP4_class.argp_opponent, &SGP4_CLASS::argp_opponent_, (size_t)sizeof(SGP4_class.argp_opponent), (uint32_t)0x000080c0);
	read_param(&SGP4_class.ascn_opponent, &SGP4_CLASS::ascn_opponent_, (size_t)sizeof(SGP4_class.ascn_opponent), (uint32_t)0x000080c8);
//	printf("%d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\r\n", SGP4_class.ep_year, SGP4_class.ep_day,SGP4_class.rev,SGP4_class.bstar, SGP4_class.eqinc, SGP4_class.ecc, SGP4_class.mnan, SGP4_class.argp, SGP4_class.ascn);
	//mramから読み込み？？
	SGP4_class.sgp4_setparam();
	//tleからr,vを作成する
}

/**
 * @brief アプリケーション実行関数
 *
 */
static void SGP4_initialize_param(void)
{
	//mram初期化？？ mramからのよみこみチェックのため、某衛星のTLEを使用する
	printf("SGP4_initialize_param\r\n");
	write_param(&SGP4_class.ep_year, &SGP4_CLASS::ep_year_, (size_t)sizeof(SGP4_class.ep_year), (uint32_t)0x00008000);
	write_param(&SGP4_class.ep_day, &SGP4_CLASS::ep_day_, (size_t)sizeof(SGP4_class.ep_day), (uint32_t)0x00008004);
	write_param(&SGP4_class.rev, &SGP4_CLASS::rev_, (size_t)sizeof(SGP4_class.rev), (uint32_t)0x0000800C);
	write_param(&SGP4_class.bstar, &SGP4_CLASS::bstar_, (size_t)sizeof(SGP4_class.bstar), (uint32_t)0x00008014);
	write_param(&SGP4_class.eqinc, &SGP4_CLASS::eqinc_, (size_t)sizeof(SGP4_class.eqinc), (uint32_t)0x0000801C);
	write_param(&SGP4_class.ecc, &SGP4_CLASS::ecc_, (size_t)sizeof(SGP4_class.ecc), (uint32_t)0x00008024);
	write_param(&SGP4_class.mnan, &SGP4_CLASS::mnan_, (size_t)sizeof(SGP4_class.mnan), (uint32_t)0x0000802C);
	write_param(&SGP4_class.argp, &SGP4_CLASS::argp_, (size_t)sizeof(SGP4_class.argp), (uint32_t)0x00008034);
	write_param(&SGP4_class.ascn, &SGP4_CLASS::ascn_, (size_t)sizeof(SGP4_class.ascn), (uint32_t)0x0000803C);
	write_param(&SGP4_class.ep_year_opponent, &SGP4_CLASS::ep_year_opponent_, (size_t)sizeof(SGP4_class.ep_year_opponent), (uint32_t)0x0000808c);
	write_param(&SGP4_class.ep_day_opponent, &SGP4_CLASS::ep_day_opponent_, (size_t)sizeof(SGP4_class.ep_day_opponent), (uint32_t)0x00008090);
	write_param(&SGP4_class.rev_opponent, &SGP4_CLASS::rev_opponent_, (size_t)sizeof(SGP4_class.rev_opponent), (uint32_t)0x00008098);
	write_param(&SGP4_class.bstar_opponent, &SGP4_CLASS::bstar_opponent_, (size_t)sizeof(SGP4_class.bstar_opponent), (uint32_t)0x000080a0);
	write_param(&SGP4_class.eqinc_opponent, &SGP4_CLASS::eqinc_opponent_, (size_t)sizeof(SGP4_class.eqinc_opponent), (uint32_t)0x000080a8);
	write_param(&SGP4_class.ecc_opponent, &SGP4_CLASS::ecc_opponent_, (size_t)sizeof(SGP4_class.ecc_opponent), (uint32_t)0x000080b0);
	write_param(&SGP4_class.mnan_opponent, &SGP4_CLASS::mnan_opponent_, (size_t)sizeof(SGP4_class.mnan_opponent), (uint32_t)0x000080b8);
	write_param(&SGP4_class.argp_opponent, &SGP4_CLASS::argp_opponent_, (size_t)sizeof(SGP4_class.argp_opponent), (uint32_t)0x000080c0);
	write_param(&SGP4_class.ascn_opponent, &SGP4_CLASS::ascn_opponent_, (size_t)sizeof(SGP4_class.ascn_opponent), (uint32_t)0x000080c8);
//	SGP4_class.sgp4_setparam();

}


static void ODS_sgp4_update_(void)
{
	SGP4_class.sgp4_setparam();

	//計算時間計測
//	int start, end;
	//start = master_clock->step;

	//計算
	SGP4_class.sgp4_calc(g_ads.p_JULIANDATE->juliandate);
//	printf("juliandate %f\n", g_ads.p_JULIANDATE->juliandate);
	//end = master_clock->step;
	//printf("SGP4 %d ms\n", end-start);
	//20211009 inakawa 上の出力は1msであった

	//計算結果の構造体への格納
	//もしグローバルな認識値格納場所ができれば、そちらに格納先を変更する
	//現状、クラスのメンバを同名の構造体にコピーしているだけ
	//実行した計算則でそれぞれ認識値を書き換えに行けばいい
	copy(SGP4_class.sgp4_position,3,1,SGP4_.sgp4_pos);
	copy(SGP4_class.sgp4_velocity,3,1,SGP4_.sgp4_vel);

	print(SGP4, "pos[km] x=%f, y=%f, z=%f.\n", SGP4_.sgp4_pos[0],SGP4_.sgp4_pos[1],SGP4_.sgp4_pos[2]);
	print(SGP4,"velo[km/s] x=%f, y=%f, z=%f.\n", SGP4_.sgp4_vel[0],SGP4_.sgp4_vel[1],SGP4_.sgp4_vel[2]);
	Normalization(SGP4_class.sgp4_velocity, 3, SGP4_.sgp4_Velocity_Vector_i);
	multiplication(&g_ads.p_ADS->DCM_i2b[0][0], 3, 3, SGP4_.sgp4_Velocity_Vector_i, 3, 1, SGP4_.sgp4_Velocity_Vector_b, 3, 1);
//	print(SGP4,"sgp4_Velocity_Vector_b x=%f, y=%f, z=%f.\n", SGP4_.sgp4_Velocity_Vector_b[0],SGP4_.sgp4_Velocity_Vector_b[1],SGP4_.sgp4_Velocity_Vector_b[2]);

         //original
//	GLOBAL_AOCS_set_Position(SGP4_class.sgp4_position);
//	GLOBAL_AOCS_set_Velocity(SGP4_class.sgp4_velocity);
//	GLOBAL_ODS_set_Position(SGP4_class.sgp4_position);
//	GLOBAL_ODS_set_Velocity(SGP4_class.sgp4_velocity);

	//Switching between SGP4 and GPS (not tested yet) _taguchi
	if(p_gps_sensor->GPS_Status != 0x04){
			float buf_pos[3] = {SGP4_class.sgp4_position[0]*1000, SGP4_class.sgp4_position[1]*1000, SGP4_class.sgp4_position[2]*1000};	//buf_pos[m]
			float buf_vel[3] = {SGP4_class.sgp4_velocity[0]*1000, SGP4_class.sgp4_velocity[1]*1000, SGP4_class.sgp4_velocity[2]*1000};	//buf_vel[m/s]
			//計算値がnan,infであるかの確認
			buf_vel[1] = 0.0/0.0;
			AOCS_STATUS_STRUCT ret1 = GLOBAL_AOCS_set_Status(buf_pos, 3);
			AOCS_STATUS_STRUCT ret2 = GLOBAL_AOCS_set_Status(buf_vel, 3);
			if(ret1 == CALCULATION_FAILED){
		      //printf("SGP4 pos fail\r\n");   //DBG用
		      return ;
		    }else{
		      //計算値をglabal変数に格納
	          GLOBAL_ODS_set_Position(buf_pos);
	          //printf("SGP4 pos success\r\n");   //DBG用
		    }
            if(ret2 == CALCULATION_FAILED){
              //printf("SGP4 vel fail\r\n");   //DBG用
              return ;
            }else{
              //計算値をglabal変数に格納
              GLOBAL_ODS_set_Velocity(buf_vel);
              //printf("SGP4 vel success\r\n");   //DBG用
            }
			float sgp4_ECEF[3];
			float sgp4_llh[3];
			Rotate_one_axis(buf_pos, 1, 2, g_ads.p_GST->gst, sgp4_ECEF);
			ecef2blh(sgp4_ECEF, sgp4_llh);
		//	print(SGP4,"SGP4 ECEF %f %f %f \r\n",sgp4_ECEF[0], sgp4_ECEF[1], sgp4_ECEF[2]);
			print(SGP4,"SGP4          LLH %f %f %f \r\n",sgp4_llh[0], sgp4_llh[1], sgp4_llh[2]);
			GLOBAL_ODS_set_Latitude(sgp4_llh[0]);
			GLOBAL_ODS_set_Longitude(sgp4_llh[1]);
			GLOBAL_ODS_set_Altitude(sgp4_llh[2]);
			GLOBAL_ODS_set_Position_ECEF(sgp4_ECEF);
	}


#ifdef HILS
	float sgp4_ECEF[3];
	float sgp4_blh[3];
	Rotate_one_axis(SGP4_class.sgp4_position, 1, 2, g_ads.p_GST->gst, sgp4_ECEF);
	float buf[3] = {sgp4_ECEF[0]*1000, sgp4_ECEF[1]*1000, sgp4_ECEF[2]*1000};
	ecef2blh(buf, sgp4_blh);
//	print(SGP4,"SGP4 ECEF %f %f %f \r\n",sgp4_ECEF[0], sgp4_ECEF[1], sgp4_ECEF[2]);
	print(SGP4,"SGP4          LLH %f %f %f \r\n",sgp4_blh[0], sgp4_blh[1], sgp4_blh[2]);

#endif

}

static void ODS_sgp4_opponent_update_(void)
{
	SGP4_class.sgp4_opponent_setparam();

	//計算時間計測
//	int start, end;
	//start = master_clock->step;

	//計算
	SGP4_class.sgp4_calc(g_ads.p_JULIANDATE->juliandate);
//	printf("juliandate %f\n", g_ads.p_JULIANDATE->juliandate);
	//end = master_clock->step;
	//printf("SGP4 %d ms\n", end-start);
	//20211009 inakawa 上の出力は1msであった

	//計算結果の構造体への格納
	//もしグローバルな認識値格納場所ができれば、そちらに格納先を変更する
	//現状、クラスのメンバを同名の構造体にコピーしているだけ
	//実行した計算則でそれぞれ認識値を書き換えに行けばいい
	copy(SGP4_class.sgp4_position,3,1,SGP4_.sgp4_pos);
	copy(SGP4_class.sgp4_velocity,3,1,SGP4_.sgp4_vel);

//	print(SGP4,"OPPONENT pos[km] x=%f, y=%f, z=%f.\n", SGP4_.sgp4_pos[0],SGP4_.sgp4_pos[1],SGP4_.sgp4_pos[2]);
//	print(SGP4,"OPPONENT velo[km/s] x=%f, y=%f, z=%f.\n", SGP4_.sgp4_vel[0],SGP4_.sgp4_vel[1],SGP4_.sgp4_vel[2]);
	Normalization(SGP4_class.sgp4_velocity, 3, SGP4_.sgp4_Velocity_Vector_i);
	multiplication(&g_ads.p_ADS->DCM_i2b[0][0], 3, 3, SGP4_.sgp4_Velocity_Vector_i, 3, 1, SGP4_.sgp4_Velocity_Vector_b, 3, 1);
//	print(SGP4,"sgp4_Velocity_Vector_b x=%f, y=%f, z=%f.\n", SGP4_.sgp4_Velocity_Vector_b[0],SGP4_.sgp4_Velocity_Vector_b[1],SGP4_.sgp4_Velocity_Vector_b[2]);

//  printf("%f,%f,%f,%f,%f,%f\n", SGP4_.sgp4_pos[0]*1000.0f,SGP4_.sgp4_pos[1]*1000.0f,SGP4_.sgp4_pos[2]*1000.0f,SGP4_.sgp4_vel[0]*1000.0f,SGP4_.sgp4_vel[1]*1000.0f,SGP4_.sgp4_vel[2]*1000.0f);
//  printf("%f,%f,%f,%f,%f,%f\n", SGP4_class.p_Position_icrf[0],SGP4_class.p_Position_icrf[1],SGP4_class.p_Position_icrf[2],SGP4_class.p_Velocity_icrf[0],SGP4_class.p_Velocity_icrf[1],SGP4_class.p_Velocity_icrf[2]);


//	GLOBAL_AOCS_set_Position(SGP4_class.sgp4_position);
//	GLOBAL_AOCS_set_Velocity(SGP4_class.sgp4_velocity);
	GLOBAL_ODS_set_Position(SGP4_class.sgp4_position);
	GLOBAL_ODS_set_Velocity(SGP4_class.sgp4_velocity);

#ifdef HILS
	float sgp4_ECEF[3];
	float sgp4_blh[3];
	Rotate_one_axis(SGP4_class.sgp4_position, 1, 2, g_ads.p_GST->gst, sgp4_ECEF);
	float buf[3] = {sgp4_ECEF[0]*1000, sgp4_ECEF[1]*1000, sgp4_ECEF[2]*1000};
	ecef2blh(buf, sgp4_blh);
//	print(SGP4,"OPPONENT SGP4 ECEF %f %f %f \r\n",sgp4_ECEF[0], sgp4_ECEF[1], sgp4_ECEF[2]);
	print(SGP4,"OPPONENT SGP4 LLH %f %f %f \r\n",sgp4_blh[0], sgp4_blh[1], sgp4_blh[2]);

#endif

}

