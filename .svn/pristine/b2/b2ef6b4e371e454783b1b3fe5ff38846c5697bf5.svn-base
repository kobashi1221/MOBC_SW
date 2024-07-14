/*
 * ADS_cmd_IGRF.cpp
 *
 *  Created on: 2021/06/13
 *      Author: inakawa
 */

#include "ADS_cmd_GST.h"
#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../../Global_Sensor_Value.h"
#include <src_core/System/TimeManager/obc_time.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/GST.h>
#include <src_user/Drivers/Aocs/JulianDate.h>
#include <src_user/Drivers/Cdh/RTC/RTC.h>
#include <src_user/Applications/UserDefined/Cdh/RTC_handler.h>
#include <src_user/Library/General/DBG_printf.h>

static GST_CLASS GST_class;
static GST_STRUCT GST_;
//const GST_STRUCT* GST;

static JULIANDATE_CLASS JULIANDATE_class;
static JULIANDATE_STRUCT JULIANDATE_;
//const JULIANDATE_STRUCT* JULIANDATE;

static void GST_init(void);
static void GST_exe(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo GST_update(void)
{
    return AI_create_app_info("DBG_GST", GST_init, GST_exe);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void GST_init(void)
{
   //printf("sample driver initialize.\n");
    g_ads.p_GST = &GST_;
	g_ads.p_JULIANDATE = &JULIANDATE_;
}

/**
 * @brief アプリケーション実行関数
 *
 */
static void GST_exe(void)
{
	//ユリウス日と恒星時計算を行う。
	//ユリウス日のみ求めて恒星時計算を行わないということはないはずなので、同一コマンドとする。

	//RTCでインクリメントしている年月日時分秒を用いる（GPSと同期しているときはGPSUTC）

	Get_RTC_Time_Date(p_rtc_struct);


	//2021の春分は3/20

	float year = float(p_rtc_struct->year);
	float month = float(p_rtc_struct->sDate.Month);
	float day = float(p_rtc_struct->sDate.Date);
	float hour = float(p_rtc_struct->sTime.Hours);
	float min = float(p_rtc_struct->sTime.Minutes);
	float sec = float(p_rtc_struct->sTime.Seconds + p_rtc_struct->ms);//msまでとろう

	print(GST,"p_rtc; Year %d, month %d, day %d, hour %d, min %d, sec %d (day)\r\n", p_rtc_struct->year, p_rtc_struct->sDate.Month, p_rtc_struct->sDate.Date, p_rtc_struct->sTime.Hours, p_rtc_struct->sTime.Minutes, p_rtc_struct->sTime.Seconds);


	print(GST,"Year %f, month %f, day %f, hour %f, min %f, sec %f (day)\r\n", year, month, day, hour, min, sec);


	JULIANDATE_class.juliandate_calc(year,month,day,hour,min,sec);

	//グローバル参照できるように格納
	JULIANDATE_.juliandate = JULIANDATE_class.ModifiedJulianDate;
	print(GST,"JulianDate = %lf (day)\r\n", JULIANDATE_.juliandate);

//	printf("JD = %lf(day)\r\n", JULIANDATE_.juliandate);
//	printf("JD = %lf(day)\r\n", g_ads.p_JULIANDATE->juliandate);

	//printf("GST\r\n");

	//float in_julian_date = 58849.0; //2020/1/1
	//float in_julian_date = 59215.0; //2021/1/1

	GST_class.GST_calc(JULIANDATE_.juliandate);

	//グローバル参照できるように格納
	GST_.gst = GST_class.gst_rad;

	print(GST,"GST = %f (rad)\r\n", GST_.gst);
}
