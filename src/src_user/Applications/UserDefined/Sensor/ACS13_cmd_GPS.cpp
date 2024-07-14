
/*
 * ACS1_debug_GPS.cpp
 *
 *  Created on: 2020/11/27
 *      Author: masudahiroaki
 *
 *****************************************************************************************
 *
 * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
 *
 * explain
 *
 *
 * UPDATE HISTORY
 * Date        Rev   Editor          Description
 * 2020/11/27  1.0   H.MASUDA      First revision
 *
  *****************************************************************************************
 */

#include <src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.h>

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <src_user/Drivers/Sensor/GPS.h>
#include "../Global_Sensor_Value.h"
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include "../Cdh/Communication_Fault_Detection.h"
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/Cdh/RTC_handler.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Drivers/Aocs/sgdp4h.h>   // for M_PI
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>

static GPS_STRUCT gps_;
static GPS_SENSOR_STRUCT gps_sensor;
const GPS_SENSOR_STRUCT* p_gps_sensor=&gps_sensor;

// RTC同期方法        0xAA:PPSで同期   0x55:validになったら自動で同期
const uint8_t RTC_UTC_Permission = 0xAA;

static void ACS13_GPS_init(void);
static void ACS13_GPS_exe(void);
static void ACS13_GPS_param_load(void);
static void ACS13_GPS_param_write(void);
void Velocity_calculate(void);
void ACS13_GPS_Time_update(void);
void ACS13_GPS_Time_reset(void);
void ACS13_GPS_value_reset(void);

//static char delimiter[] = ",";

/**
 * @brief App making function.
 * @note
 */
AppInfo ACS13_GPS_update(void)
{

    return AI_create_app_info("DBG_GPS", ACS13_GPS_init, ACS13_GPS_exe);
}

AppInfo ACS13_GPS_param_update(void)
{

    return AI_create_app_info("DBG_GPS", ACS13_GPS_param_load, ACS13_GPS_param_write);
}

static void ACS13_GPS_param_load(void){
	read_param(&gps_sensor.RTC_UTC_Permission, &RTC_UTC_Permission, 1, 0x0000496F);
}
static void ACS13_GPS_param_write(void){
	write_param(&gps_sensor.RTC_UTC_Permission, &RTC_UTC_Permission, 1, 0x0000496F);
}

/**
 * @brief App initialize function.
 * @note
 */
static void ACS13_GPS_init(void)
{
//    printf("ACS13 GPS debug initialize.\r\n");

    g_sensor.p_gps = &gps_sensor;
    ACS13_GPS_Time_reset();
    GPS_UART_init(&gps_);
}

/**
 * @brief App exe function.
 * @note
 */
static void ACS13_GPS_exe(void)
{
	if(PWR_State_GET(PWR_State_GPS)){

		for(int i=0; i<GPS_MAX_RECEIVE_PACKET_NUM; i++){
			//Packetが入ってないとき
			if(isEmpty_GPS(&(gps_queue.list))) break;

			ANOMALY_UART_Update_Time(UART_FAULT_GPS);

			//パケットを移してくる
			gps_.ack = (GPS_ACK)gps_dequeue(&(gps_queue.list), gps_.buf.data, &(gps_.len));

//			HAL_UART_Transmit_C2A(&huart8, gps_.buf.data, gps_.len, 0xff);

			for(int i = 0; i < gps_.len; i++) print(GPS, "%c", gps_.buf.data[i]);

			if(gps_.ack != GPS_SUCCESS){
				gps_queue_reset(&(gps_queue.list));//カウンタを全部消す
				return;
			}

			//uart anomaly

			gps_.header = GPS_check_header(&gps_);

			//緯度、経度、高度、年月日、時分秒 (GGAとRMCだけでも十分)
			switch (gps_.header){
			case GGA:
				GPS_GGA_analyze(&gps_);
				break;
			case GSA:
				//使わない
				break;
			case GLL:
				GPS_GLL_analyze(&gps_);
				break;
			case RMC:
				GPS_RMC_analyze(&gps_);
				break;
			case VTG:
				GPS_VTG_analyze(&gps_);
				break;
			case ZDA:
				GPS_ZDA_analyze(&gps_);
				break;
			case GPS_UNKKOWN:
				break;
			}
		}


			gps_sensor.GPS_Status = gps_.Status;                  // GPSがどこまで情報を取得しているか、知るためのもの
//			gps_sensor.GPS_Mode_indicator = gps_.Mode_indicator[0];
//			gps_sensor.GPS_Navigation_status = gps_.Navigation_status[0];
//			gps_sensor.GPS_SyncFlag = gps_.GPS_SyncFlag;
			gps_sensor.GPS_UTC_YEAR = gps_.GPS_UTC_YEAR;
			gps_sensor.GPS_UTC_MONTH = gps_.GPS_UTC_MONTH;
			gps_sensor.GPS_UTC_DAY = gps_.GPS_UTC_DAY;
			gps_sensor.GPS_UTC_HOUR = gps_.GPS_UTC_HOUR;
			gps_sensor.GPS_UTC_MIN = gps_.GPS_UTC_MIN;
			gps_sensor.GPS_UTC_SEC = gps_.GPS_UTC_SEC;     //secの小数点以下を四捨五入         ver.2
//			gps_sensor.GPS_UTC = gps_.GPS_UTC;
			gps_sensor.Latitude = gps_.Latitude;
			gps_sensor.Longitude = gps_.Longitude;
			gps_sensor.Altitude = gps_.Altitude;
			gps_sensor.GPS_UTC_YEAR_L2 = gps_.GPS_UTC_YEAR_L2;
			gps_sensor.GPS_valid_flag = gps_.Valid_flag;           //  GPS/SGP4_flagの代わり

		//緯度,経度,高さから ECEF 座標に変換
		blh2ecef(gps_sensor.Latitude, gps_sensor.Longitude, gps_sensor.Altitude, gps_sensor.ECEF);
		//ECEFからECIに変換
		Rotate_one_axis(gps_sensor.ECEF, 1, 2, -g_ads.p_GST->gst, gps_sensor.ECI);

//		print(GPS,"-----GPS DATA %d %d %d / %d %d %d / %f %f %f \r\n",gps_sensor.GPS_UTC_HOUR, gps_sensor.GPS_UTC_MIN, gps_sensor.GPS_UTC_SEC, gps_sensor.GPS_UTC_DAY, gps_sensor.GPS_UTC_MONTH, gps_sensor.GPS_UTC_YEAR_L2, gps_sensor.Latitude, gps_sensor.Longitude, gps_sensor.Altitude);
//		print(GPS," MOBC_RTC %d %d %d %d %d %d% \r\n",p_rtc_struct->sTime.Hours,p_rtc_struct->sTime.Minutes,p_rtc_struct->sTime.Seconds,p_rtc_struct->sDate.Date,p_rtc_struct->sDate.Month,p_rtc_struct->year);
		print(GPS,"ECEF (%f , %f , %f)\r\n",gps_sensor.ECEF[0],gps_sensor.ECEF[1],gps_sensor.ECEF[2]);
//		print(GPS,"ECI (%f  %f  %f)\r\n",gps_sensor.ECI[0],gps_sensor.ECI[1],gps_sensor.ECI[2]);
//		print(GPS,"llh (%f, %f, %f)\r\n", gps_sensor.Latitude, gps_sensor.Longitude, gps_sensor.Altitude);

		//GPS_Status の細分化
		if(gps_.Valid_flag == 0xff) gps_sensor.GPS_Status = 0x04;   		//この時だけGPSの情報は有効で計算に使える
		else if(gps_.Valid_flag != 0xff){                              // 有効ではないとき↓↓↓
			if(gps_.YMD_flag == 0x00) gps_sensor.GPS_Status = 0x01;
			else if(gps_.YMD_flag == 0xff){                               // 年月日を取得できている(同時に時分秒も取得するはず)
				if(gps_.LLA_flag == 0x00) gps_sensor.GPS_Status = 0x11;             // 緯度経度高度が取得できていて、いずれかが0
				else if(gps_.LLA_flag == 0x55) {
					if(gps_.VTG_flag == 0x00) gps_sensor.GPS_Status = 0x31;
				    if(gps_.VTG_flag == 0xff) gps_sensor.GPS_Status = 0xB1;              // velocityが取得できている
				    else if(gps_.VTG_flag != 0x00 && gps_.VTG_flag != 0xff) gps_sensor.GPS_Status = 0x31;
				}
				else if(gps_.LLA_flag == 0xAA){                                  // 緯度経度高度が取得できている
					if(gps_.VTG_flag == 0x00) gps_sensor.GPS_Status = 0x51;
				    if(gps_.VTG_flag == 0xff) gps_sensor.GPS_Status = 0xD1;              // velocityが取得できている
				    else if(gps_.VTG_flag != 0x00 && gps_.VTG_flag != 0xff) gps_sensor.GPS_Status = 0x51;
				}
				else gps_sensor.GPS_Status =0x11;                      //いちおうelse
			}
			else gps_sensor.GPS_Status = 0x01;
		}
//		print(GPS,"GPS_Status : %d\r\n",gps_sensor.GPS_Status);

		GLOBAL_AOCS_set_GPS_Status(gps_sensor.GPS_Status);

		if(gps_sensor.GPS_Status == 0x04){              //GPS情報が有効の時
			ACS13_GPS_Time_update();//時間更新
			Velocity_calculate();             // calculate velocity

			float buf_vel[3] = {gps_sensor.Vel_ECI[0] * 1000, gps_sensor.Vel_ECI[1] * 1000, gps_sensor.Vel_ECI[2] * 1000}; // buf_vel[m/s]

			GLOBAL_AOCS_set_Position(gps_sensor.ECI);
			GLOBAL_AOCS_set_Velocity(buf_vel);
			GLOBAL_AOCS_set_velocity_norm(gps_sensor.vel_norm);
			GLOBAL_AOCS_set_Position_ECEF(gps_sensor.ECEF);
			GLOBAL_AOCS_set_Latitude(gps_sensor.Latitude);
			GLOBAL_AOCS_set_Longitude(gps_sensor.Longitude);
			GLOBAL_AOCS_set_Altitude(gps_sensor.Altitude);

			if(p_gps_sensor->RTC_UTC_Permission == 0x55){       // 自動でRTCが設定されるが、1秒遅れ

				p_rtc_struct->sTime.Hours   = p_gps_sensor->GPS_UTC_HOUR;
				p_rtc_struct->sTime.Minutes = p_gps_sensor->GPS_UTC_MIN;
				p_rtc_struct->sTime.Seconds = p_gps_sensor->GPS_UTC_SEC;
				p_rtc_struct->sDate.Month   = p_gps_sensor->GPS_UTC_MONTH;
				p_rtc_struct->sDate.Date    = p_gps_sensor->GPS_UTC_DAY;
				p_rtc_struct->year          = p_gps_sensor->GPS_UTC_YEAR;
				Set_RTC_Time_Date(p_rtc_struct);

			}

//			printf("GAP_pos  (%f , %f , %f)\r\n",g_acs.p_GLOBAL_AOCS->Position[0],g_acs.p_GLOBAL_AOCS->Position[1],g_acs.p_GLOBAL_AOCS->Position[2]);
//			printf("GAP_vel  (%f , %f , %f)\r\n",g_acs.p_GLOBAL_AOCS->Velocity[0],g_acs.p_GLOBAL_AOCS->Velocity[1],g_acs.p_GLOBAL_AOCS->Velocity[2]);
//			printf("Vel_ECI_calc  (%f , %f , %f)\r\n",gps_sensor.Vel_ECI[0],gps_sensor.Vel_ECI[1],gps_sensor.Vel_ECI[2]);

		}
		//printf("%u:%u:%u,%u%u%u,", p_rtc_struct->sTime.Hours, p_rtc_struct->sTime.Minutes, p_rtc_struct->sTime.Seconds, p_rtc_struct->year, p_rtc_struct->sDate.Month, p_rtc_struct->sDate.Date);


		//original
//		if(ODS_Mode(GPS_TWO_ANTENNA)){
//			GLOBAL_AOCS_set_Position(gps_sensor.ECI);
//			GLOBAL_AOCS_set_Velocity();
//		}

    }
	else{ // GPSの電源がOFFの場合
		//Reset Flags
		gps_.YMD_flag = 0x00;
		gps_.LLA_flag = 0x00;
		gps_.VTG_flag = 0x00;
		gps_.Valid_flag = 0x00;
		gps_sensor.GPS_Status = 0x00;

		// 構造体の値の初期化
		ACS13_GPS_value_reset();
		print(GPS,"ECEF (%f , %f , %f)\r\n",gps_sensor.ECEF[0],gps_sensor.ECEF[1],gps_sensor.ECEF[2]);
	}

#ifdef HILS
	if(PWR_State_GET(PWR_State_GPS)){

      if(AOCS_hils->gps_visible){
          float sat_pos[3] = {AOCS_hils->llh[0], AOCS_hils->llh[1], AOCS_hils->llh[2]};
          float s2e_ecefv[3] = {AOCS_hils->ecef_velocity[0]*0.001f, AOCS_hils->ecef_velocity[1]*0.001f, AOCS_hils->ecef_velocity[2]*0.001f}; // [km/s]
  //		float blh[3];
  //		float GPS_UTC_YEAR_L2_HILS;
          float vel_ECEF_to_ECI[3] = {0};
          float temp[3] = {0};
          float w[3] = {0 ,0 ,-2*M_PI/86400};     //Earth's rotation vector:w=[0,0,-2PI/86400]       for ECEF to ECI

          if(sat_pos[1] > M_PI){
              sat_pos[1] =  sat_pos[1] - 2 * M_PI;
          }

          // UTC year, month, day, hour, minute, second
          //gps_sensor.GPS_Status = gps_.Status[0];
          //gps_sensor.GPS_SyncFlag = gps_.GPS_SyncFlag;
          gps_sensor.GPS_UTC_YEAR = (uint16_t)AOCS_hils->utc_year;
          gps_sensor.GPS_UTC_MONTH = (uint8_t)AOCS_hils->utc_month;
          gps_sensor.GPS_UTC_DAY = (uint8_t)AOCS_hils->utc_day;
          gps_sensor.GPS_UTC_HOUR = (uint8_t)AOCS_hils->utc_hour;
          gps_sensor.GPS_UTC_MIN = (uint8_t)AOCS_hils->utc_minute;
          gps_sensor.GPS_UTC_SEC = (uint8_t)AOCS_hils->utc_second;
          //gps_sensor.GPS_UTC = gps_.GPS_UTC;
          gps_sensor.Latitude = sat_pos[0] * 180.0f / M_PI;
          gps_sensor.Longitude = sat_pos[1]* 180.0f / M_PI;
          gps_sensor.Altitude = sat_pos[2];
          //gps_sensor.GPS_UTC_YEAR_L2 = gps_.GPS_UTC_YEAR_L2;
          //RTC UTC sync GPSを取得したらRTCをUTCに同期させる
          p_rtc_struct->sTime.Hours   = p_gps_sensor->GPS_UTC_HOUR;
          p_rtc_struct->sTime.Minutes = p_gps_sensor->GPS_UTC_MIN;
          p_rtc_struct->sTime.Seconds = p_gps_sensor->GPS_UTC_SEC;
          p_rtc_struct->sDate.Month   = p_gps_sensor->GPS_UTC_MONTH;
          p_rtc_struct->sDate.Date    = p_gps_sensor->GPS_UTC_DAY;
          p_rtc_struct->year          = p_gps_sensor->GPS_UTC_YEAR;
          Set_RTC_Time_Date(p_rtc_struct);

          //緯度,経度,高さから ECEF 座標に変換
          blh2ecef(gps_sensor.Latitude, gps_sensor.Longitude, gps_sensor.Altitude, gps_sensor.ECEF);

          //ECEFからECIに変換
          Rotate_one_axis(gps_sensor.ECEF, 1, 2, -g_ads.p_GST->gst, gps_sensor.ECI);

          // convert velocity from ECEF into ECI
          Rotate_one_axis(s2e_ecefv, 1, 2, -g_ads.p_GST->gst,vel_ECEF_to_ECI);
          Outer_product(w,p_gps_sensor->ECI,temp);
          for(int i=0 ;i<3 ;i++){
              gps_sensor.Vel_ECI[i] = vel_ECEF_to_ECI[i] - temp[i]/1000;   //velocity [km/s]
          }

          // Vel_ECIのノルムを計算
          gps_sensor.vel_norm = Fabs(s2e_ecefv, 3);

          if(gps_sensor.Altitude!=0){
              ACS13_GPS_Time_update();
  //			gps_sensor.last_recv_time = TMGR_get_master_total_cycle();
  //		    Get_RTC_Time_Date(&gps_sensor.last_recv_rtc);

              GLOBAL_AOCS_set_Position(gps_sensor.ECI);
              GLOBAL_AOCS_set_Velocity(gps_sensor.Vel_ECI);
              GLOBAL_AOCS_set_velocity_norm(gps_sensor.vel_norm);
              GLOBAL_AOCS_set_Position_ECEF(gps_sensor.ECEF);
              GLOBAL_AOCS_set_Latitude(gps_sensor.Latitude);
              GLOBAL_AOCS_set_Longitude(gps_sensor.Longitude);
              GLOBAL_AOCS_set_Altitude(gps_sensor.Altitude);

          }
      //	ecef2blh(gps_sensor.ECEF, blh);
  //		print(GPS,"GPS DATA %d %d %d %d %d %d %f %f %f \r\n",gps_sensor.GPS_UTC_HOUR, gps_sensor.GPS_UTC_MIN, gps_sensor.GPS_UTC_SEC, gps_sensor.GPS_UTC_DAY, gps_sensor.GPS_UTC_MONTH, gps_sensor.GPS_UTC_YEAR, gps_sensor.Latitude, gps_sensor.Longitude, gps_sensor.Altitude);
  //		print(GPS,"GPS ECEF[m] %f %f %f \r\n",gps_sensor.ECEF[0]/1000, gps_sensor.ECEF[1]/1000, gps_sensor.ECEF[2]/1000);
          print(GPS,"GPS ECI[m] %f %f %f \r\n",gps_sensor.ECI[0], gps_sensor.ECI[1], gps_sensor.ECI[2]);
  //		print(GPS,"S2E ECI[m] %f %f %f \r\n",s2e_eci[0]/1000, s2e_eci[1]/1000, s2e_eci[2]/1000);
          print(GPS,"GAP_pos  (%f , %f , %f)\r\n",g_acs.p_GLOBAL_AOCS->Position[0],g_acs.p_GLOBAL_AOCS->Position[1],g_acs.p_GLOBAL_AOCS->Position[2]);

  //		print(GPS,"GPS LLH[deg deg m] %f %f %f \r\n",gps_sensor.Latitude, gps_sensor.Longitude, gps_sensor.Altitude/1000);
      //	print(GPS,"ECEF to LLH %f %f %f \r\n",blh[0], blh[1], blh[2]);
          hils_UTC_flag = 0;
      }
	}
    else { // GPSの電源がOFFの場合

      // 初期化
      ACS13_GPS_value_reset();
    }

	//*******************************************
#endif
}

void Velocity_calculate(void){
	float ENU_to_ECEF[3][3] = {0};
	float r = gps_.speed/3600;  //km/h -> km/s
	float deg = gps_.Degree_true;
	float vel_ENU[3] = {0};
	float vel_ECEF[3] = {0};
	float vel_ECEF_to_ECI[3] = {0};
	float temp[3] = {0};
	float w[3] = {0 ,0 ,-2*M_PI/86400};     //Earth's rotation vector:w=[0,0,-2PI/86400]       for ECEF to ECI

	// DCM from ENU to ECEF
	ENU_to_ECEF[0][0] = -sinf(gps_.Longitude*M_PI/180);  ENU_to_ECEF[0][1] = -cosf(gps_.Longitude*M_PI/180)*sinf(gps_.Latitude*M_PI/180);  ENU_to_ECEF[0][2] = cosf(gps_.Longitude*M_PI/180)*cosf(gps_.Latitude*M_PI/180);
	ENU_to_ECEF[1][0] = cosf(gps_.Longitude*M_PI/180);   ENU_to_ECEF[1][1] = -sinf(gps_.Longitude*M_PI/180)*sinf(gps_.Latitude*M_PI/180);  ENU_to_ECEF[1][2] = sinf(gps_.Longitude*M_PI/180)*cosf(gps_.Latitude*M_PI/180);
	ENU_to_ECEF[2][0] = 0;                               ENU_to_ECEF[2][1] = cosf(gps_.Latitude*M_PI/180);                                 ENU_to_ECEF[2][2] = sinf(gps_.Latitude*M_PI/180);

	// Velocity in ENU coordinate
	vel_ENU[0] = r*sinf(deg*M_PI/180);  vel_ENU[1] = r*cosf(deg*M_PI/180);   vel_ENU[2] = 0;
//	printf("vel_ENU (%f , %f , %f)\r\n",vel_ENU[0],vel_ENU[1],vel_ENU[2]);  //DBG用

	// calculating velocity in ECEF
	for(int i=0 ;i<3 ;i++){
		for(int j=0 ;j<3 ;j++){
			vel_ECEF[i] += ENU_to_ECEF[i][j]*vel_ENU[j];
		}
	}
//	print(GPS,"vel_ECEF (%f , %f , %f)\r\n",vel_ECEF[0],vel_ECEF[1],vel_ECEF[2]);  //DBG用

	// convert velocity from ECEF into ECI
	Rotate_one_axis(vel_ECEF, 1, 2, -g_ads.p_GST->gst,vel_ECEF_to_ECI);
	Outer_product(w,p_gps_sensor->ECI,temp);
	for(int i=0 ;i<3 ;i++){
		gps_sensor.Vel_ECI[i] =	vel_ECEF_to_ECI[i] - temp[i]/1000;   //velocity [km/sec]
	}
	gps_sensor.vel_norm = r; // velocity norm [km/sec] Vel_ECIのノルムを取った方が良い？
//	printf("vel_ECEF_ECI (%f ,%f ,%f)\r\n",vel_ECEF_to_ECI[0],vel_ECEF_to_ECI[1],vel_ECEF_to_ECI[2]);
//	printf("w cross r_ECI (%f ,%f ,%f)\r\n",temp[0],temp[1],temp[2]);
//	print(GPS,"-------------vel_ECI  (%f  %f  %f)\r\n",gps_sensor.Vel_ECI[0]*1000,gps_sensor.Vel_ECI[1]*1000,gps_sensor.Vel_ECI[2]*1000);  //DBG用

}

CCP_CmdRet Cmd_ACS13_GPS_SET_RTC_BY_UTC(const CommonCmdPacket* packet){
	p_rtc_struct->sTime.Hours   = p_gps_sensor->GPS_UTC_HOUR;
	p_rtc_struct->sTime.Minutes = p_gps_sensor->GPS_UTC_MIN;
	p_rtc_struct->sTime.Seconds = p_gps_sensor->GPS_UTC_SEC;
	p_rtc_struct->sDate.Month   = p_gps_sensor->GPS_UTC_MONTH;
	p_rtc_struct->sDate.Date    = p_gps_sensor->GPS_UTC_DAY;
	p_rtc_struct->year          = p_gps_sensor->GPS_UTC_YEAR;
	Set_RTC_Time_Date(p_rtc_struct);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

void ACS13_GPS_Time_update(void){
    //時刻関連の更新，ORBPROPの監視用
    gps_sensor.last_recv_time = TMGR_get_master_total_cycle();
    Get_RTC_Time_Date(&gps_sensor.last_recv_rtc);
}

void ACS13_GPS_Time_reset(void){
    //時刻関連の初期化，ORBPROPの監視用
    gps_sensor.last_recv_time = 0;
    Get_RTC_Time_Date(&gps_sensor.last_recv_rtc);
}

/**
 * @brief GPSの構造体メンバ変数の値をリセットする関数
 * @param なし
 * @return なし
 * @作成者：玉置 2023/09/26
 */
void ACS13_GPS_value_reset(void)
{
    // 構造体の初期化
    gps_ = {0};       // ドライバ側の構造体
    gps_sensor = {0}; // アプリケーション側の構造体

    // GlobalAOCSパラメータを0に設定
    GLOBAL_AOCS_set_Position(gps_sensor.ECI);
    GLOBAL_AOCS_set_Velocity(gps_sensor.Vel_ECI);
    GLOBAL_AOCS_set_velocity_norm(gps_sensor.vel_norm);
    GLOBAL_AOCS_set_Position_ECEF(gps_sensor.ECEF);
    GLOBAL_AOCS_set_Latitude(gps_sensor.Latitude);
    GLOBAL_AOCS_set_Longitude(gps_sensor.Longitude);
    GLOBAL_AOCS_set_Altitude(gps_sensor.Altitude);

}
