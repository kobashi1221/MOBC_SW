/*
 * ACS3_debug_9AX.cpp
 *
 *  Created on: 2020/12/09
 *      Author: hiroaki MASUDA
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
 * 2020/12/09  1.0   H.MASUDA      First revision
 *
  *****************************************************************************************
 */

#include "ACS23_cmd_9AX.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include "../Global_Sensor_Value.h"
#include <src_user/Applications/DriverInstances/block_command_handler.h>    //for current mode id
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include "../Cdh/Communication_Fault_Detection.h"
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Library/General/DBG_printf.h>
#include "math.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>


#define PI 3.14159f
static IMU_SENSOR_STRUCT imusensor;

//======MRAM格納パラメータの実体宣言部======//
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS BEGIN *** //
const float QAX_ACC_DCM_[3][3] = {-1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f}; //plus or minus sign x,y,z
const float qax_acc_scale_[3] = {1.0f, 1.0f, 1.0f};  //9AX_ACC scale factor
const float qax_acc_offset_[3] = {0.0f, 0.0f, 0.0f}; //9AX_ACC offset
const float acc_sf_t0_[3] = {25.0f, 25.0f, 25.0f}; // standard temperature of ACC ScaleFactor temperature compensation coefficient formula
const float acc_sf_t1_[3] = {0.0f, 0.0f, 0.0f}; // first term of ACC ScaleFactor temperature compensation coefficient formula
const float acc_sf_t2_[3] = {0.0f, 0.0f, 0.0f}; // const term of ACC ScaleFactor temperature compensation coefficient formula
const float acc_os_t0_[3] = {25.0f, 25.0f, 25.0f}; // standard temperature of ACC OFFSET temperature compensation coefficient formula
const float acc_os_t1_[3] = {0.0f, 0.0f, 0.0f}; // first term of ACC OFFSET temperature compensation coefficient formula
const float acc_os_t2_[3] = {0.0f, 0.0f, 0.0f}; // const term of ACC OFFSET temperature compensation coefficient formula

const float QAX_GYRO_DCM_[3][3] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}; //plus or minus sign x,y,z
const float qax_gyro_scale_[3] = {1.008f, 1.008f, 1.008f};  //9AX_GYRO scale factor
const float qax_gyro_offset_[3] = {0.0f, 0.0f, 0.0f}; //9AX_GYRO offset
const float gyro_sf_t0_[3] = {25.0f, 25.0f, 25.0f}; // standard temperature of GYRO ScaleFactor temperature compensation coefficient formula
const float gyro_sf_t1_[3] = {0.0f, 0.0f, 0.0f}; // first term of GYRO ScaleFactor temperature compensation coefficient formula
const float gyro_sf_t2_[3] = {0.0f, 0.0f, 0.0f}; // const term of GYRO ScaleFactor temperature compensation coefficient formula
const float gyro_os_t0_[3] = {25.0f, 25.0f, 25.0f}; // standard temperature of GYRO OFFSET temperature compensation coefficient formula
const float gyro_os_t1_[3] = {0.0f, 0.0f, 0.0f}; // first term of GYRO OFFSET temperature compensation coefficient formula
const float gyro_os_t2_[3] = {0.0f, 0.0f, 0.0f}; // const term of GYRO OFFSET temperature compensation coefficient formula

const float QAX_MAGS_DCM_[3][3] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}; //plus or minus sign x,y,z
const float qax_mag_scale_[3] = {1.0f, 1.0f, 1.8f};  //9AX_MAG scale factor
const float qax_mag_offset_[3] = {0.0f, 0.0f, 0.0f}; //9AX_MAG offset
const float mag_sf_t0_[3] = {25.0f, 25.0f, 25.0f}; // standard temperature of MAGS ScaleFactor temperature compensation coefficient formula
const float mag_sf_t1_[3] = {0.0f, 0.0f, 0.0f}; // first term of MAGS ScaleFactor temperature compensation coefficient formula
const float mag_sf_t2_[3] = {0.0f, 0.0f, 0.0f}; // const term of MAGS ScaleFactor temperature compensation coefficient formula
const float mag_os_t0_[3] = {25.0f, 25.0f, 25.0f}; // standard temperature of MAGS OFFSET temperature compensation coefficient formula
const float mag_os_t1_[3] = {0.0f, 0.0f, 0.0f}; // first term of MAGS OFFSET temperature compensation coefficient formula
const float mag_os_t2_[3] = {0.0f, 0.0f, 0.0f}; // const term of MAGS OFFSET temperature compensation coefficient formula

const float QAX_GYRO_THRESHOLD_[3] = {1.0f, 1.0f, 1.0f};//threshold for
const float QAX_GYRO_HIGHSPIN_THRESHOLD_[3] = {13.0f, 5.0f, 5.0f};
const uint8_t QAX_MAGS_Continuous_Time_THRESHOLD_ = 20*3;
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS END*** //


static void ACS23_9AX_init(void); //アプリ初期化関数
static void ACS23_9AX_exe(void); //アプリ実行関数
static void QAX_initialize_param(void);
static void QAX_load_param(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note
 * @return アプリケーション構造体
 */
AppInfo ACS23_9AX_update(void)
{
	//create_app_info()を呼び出す．
	return AI_create_app_info("DBG_IMU", ACS23_9AX_init, ACS23_9AX_exe);
}

AppInfo ACS23_9AX_param_update(void)
{
    //create_app_info()アプリケーション作成関数
    //
    return AI_create_app_info("sample_param_init", QAX_load_param, QAX_initialize_param);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void ACS23_9AX_init(void)
{
	uint8_t ret = 0;

	g_sensor.p_imu = &imusensor;

	imusensor.lsm9ds1.i2c_driver = (void*)&hi2c3;
	//acc
	imusensor.lsm9ds1.acc_odr = ACC_ODR_952Hz;
	imusensor.lsm9ds1.acc_scale = ACC_SCALE_2g;
	imusensor.lsm9ds1.acc_bw_select = 0;
	imusensor.lsm9ds1.acc_bw = ACC_BW_408Hz;

	//gyro
	imusensor.lsm9ds1.gyro_odr = GYRO_ODR_14_9Hz;
	imusensor.lsm9ds1.gyro_scale = GYRO_SCALE_245dps;
	imusensor.lsm9ds1.gyro_bw = 0;
	imusensor.lsm9ds1.gyro_low_power = 0;

	//mag
	imusensor.lsm9ds1.mag_system_mode = CONTINUOUS_CONVERSION;
	imusensor.lsm9ds1.mag_mode = MEDIUM_PERFORM_MODE;
	imusensor.lsm9ds1.mag_odr = MAG_ODR_80Hz;
	imusensor.lsm9ds1.mag_scale = MAG_SCALE_4GAUSS;

	ret = lsm9ds1_power_on_init(&imusensor.lsm9ds1);
	if(ret != 0)ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, (HAL_StatusTypeDef)ret);
}


/**
 * @brief App exe function.
 * @note
 */
static void ACS23_9AX_exe(void)
{
	uint8_t ret = 0;

	for(int i = 0; i < 3; i++){
	  imusensor.ACC_HK[i] = 0;
	  imusensor.GYRO_HK[i] = 0;
	  imusensor.MAG_HK[i] = 0;
	}

	ret = lsm9ds1_get_acc_all(&imusensor.lsm9ds1, imusensor.ACC_HK);
	if(ret != 0)ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, (HAL_StatusTypeDef)ret);
	ret = lsm9ds1_get_gyro_all(&imusensor.lsm9ds1, imusensor.GYRO_HK);
	if(ret != 0)ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, (HAL_StatusTypeDef)ret);
	ret = lsm9ds1_get_mag_all(&imusensor.lsm9ds1, imusensor.MAG_HK);
	if(ret != 0)ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, (HAL_StatusTypeDef)ret);

	imusensor.qaxsensor_time_before = imusensor.qaxsensor_time_now;
	imusensor.qaxsensor_time_now = TMGR_get_master_clock();

	//ACC conversion
	float conversionFactorAcc = imusensor.lsm9ds1.acc_scale_value / 65536.0f * 9.8f;
	float acc[3];
	float acc_scale_temp[3];
	float acc_offset_temp[3];

#ifdef TWO_U
	if(ADS_Mode(NINEAX_ACC_TEMP)){
		for (int i = 0 ; i < 3; i++){
			acc_scale_temp[i] = 1.0f + imusensor.acc_sf_t1[i] * (g_sensor.p_adc->NINEAX_TEMP - imusensor.acc_sf_t0[i]) + imusensor.acc_sf_t2[i];
			acc_offset_temp[i] = imusensor.acc_os_t1[i] * (g_sensor.p_adc->NINEAX_TEMP - imusensor.acc_os_t0[i]) + imusensor.acc_os_t2[i];
		}
		//x軸は左手座標系から右手座標系に変換
	    acc[0] =  (-1.0f * (float)imusensor.ACC_HK[0] * conversionFactorAcc) * imusensor.qax_acc_scale[0] * acc_scale_temp[0] + imusensor.qax_acc_offset[0] + acc_offset_temp[0];
	    acc[1] =  (float)imusensor.ACC_HK[1] * conversionFactorAcc * imusensor.qax_acc_scale[1] * acc_scale_temp[1] + imusensor.qax_acc_offset[1] + acc_offset_temp[1];
	    acc[2] =  (float)imusensor.ACC_HK[2] * conversionFactorAcc * imusensor.qax_acc_scale[2] * acc_scale_temp[2] + imusensor.qax_acc_offset[2] + acc_offset_temp[2];
	}
#endif //for TWO_U
#ifdef ONE_U
	if(0){}// 1U not temperature compensation
#endif //for ONE_U
    else{
    	//x軸は左手座標系から右手座標系に変換
		acc[0] =  (-1.0f * (float)imusensor.ACC_HK[0] * conversionFactorAcc) * imusensor.qax_acc_scale[0] + imusensor.qax_acc_offset[0];
	    acc[1] =  (float)imusensor.ACC_HK[1] * conversionFactorAcc * imusensor.qax_acc_scale[1] + imusensor.qax_acc_offset[1];
	    acc[2] =  (float)imusensor.ACC_HK[2] * conversionFactorAcc * imusensor.qax_acc_scale[2] + imusensor.qax_acc_offset[2];
	}

	for (int i = 0; i < 3; i++){
		imusensor.ACC[i] = 0.0f;
		for (int j = 0; j < 3; j++) {
			imusensor.ACC[i] += imusensor.QAX_ACC_DCM[i][j] * acc[j];
		}
	}

	//GYRO conversion
	float conversionFactorGyro = imusensor.lsm9ds1.gyro_scale_value / 65536.0f * PI / 180.0f;
	float gyro[3];
	float gyro_scale_temp[3];
	float gyro_offset_temp[3];

#ifdef TWO_U
    if(ADS_Mode(NINEAX_GYRO_TEMP) == 1){
		for (int i = 0 ; i < 3; i++){
			gyro_scale_temp[i] = 1.0f + imusensor.gyro_sf_t1[i] * (g_sensor.p_adc->NINEAX_TEMP - imusensor.gyro_sf_t0[i]) + imusensor.gyro_sf_t2[i];
			gyro_offset_temp[i] = imusensor.gyro_os_t1[i] * (g_sensor.p_adc->NINEAX_TEMP - imusensor.gyro_os_t0[i]) + imusensor.gyro_os_t2[i];
		}
		//x軸は左手座標系から右手座標系に変換
	    gyro[0] = (-1.0f * (float)imusensor.GYRO_HK[0] * conversionFactorGyro) * imusensor.qax_gyro_scale[0] * gyro_scale_temp[0] + imusensor.qax_gyro_offset[0] + gyro_offset_temp[0];
   	    gyro[1] = (float)imusensor.GYRO_HK[1] * conversionFactorGyro * imusensor.qax_gyro_scale[1] * gyro_scale_temp[1] + imusensor.qax_gyro_offset[1] + gyro_offset_temp[1];
	    gyro[2] = (float)imusensor.GYRO_HK[2] * conversionFactorGyro * imusensor.qax_gyro_scale[2] * gyro_scale_temp[2] + imusensor.qax_gyro_offset[2] + gyro_offset_temp[2];
	}
#endif //for TWO_U
#ifdef ONE_U
    if(0){}// 1U not temperature compensation
#endif //for ONE_U
    else{
    	//x軸は左手座標系から右手座標系に変換
	    gyro[0] = (-1.0f * (float)imusensor.GYRO_HK[0] * conversionFactorGyro) * imusensor.qax_gyro_scale[0] + imusensor.qax_gyro_offset[0];
	    gyro[1] = (float)imusensor.GYRO_HK[1] * conversionFactorGyro * imusensor.qax_gyro_scale[1] + imusensor.qax_gyro_offset[1];
   	    gyro[2] = (float)imusensor.GYRO_HK[2] * conversionFactorGyro * imusensor.qax_gyro_scale[2] + imusensor.qax_gyro_offset[2];
	}

	for (int i = 0; i < 3; i++){
		imusensor.GYRO[i] = 0.0f;
		for (int j = 0; j < 3; j++) {
			imusensor.GYRO[i] += imusensor.QAX_GYRO_DCM[i][j] * gyro[j];
		}
	}

	//MAG conversion
	float conversionFactorMag = imusensor.lsm9ds1.mag_scale_value / 65536.0f * 0.0001f;
	float mag[3];
	float mag_scale_temp[3];
	float mag_offset_temp[3];

#ifdef TWO_U
	//フラグにより温度補正の有無を選択
	if(ADS_Mode(NINEAX_MAG_TEMP) == 1){
		for (int i = 0 ; i < 3; i++){
			mag_scale_temp[i] = 1.0f + imusensor.mag_os_t1[i] * (g_sensor.p_adc->NINEAX_TEMP - imusensor.mag_os_t0[i]) + imusensor.mag_os_t2[i];
			mag_offset_temp[i] = imusensor.mag_os_t1[i] * (g_sensor.p_adc->NINEAX_TEMP - imusensor.mag_os_t0[i]) + imusensor.mag_os_t2[i];
		}
		mag[0] = (float)imusensor.MAG_HK[0] * conversionFactorMag * imusensor.qax_mag_scale[0] * mag_scale_temp[0] + imusensor.qax_mag_offset[0] + mag_offset_temp[0];
		mag[1] = (float)imusensor.MAG_HK[1] * conversionFactorMag * imusensor.qax_mag_scale[1] * mag_scale_temp[1] + imusensor.qax_mag_offset[1] + mag_offset_temp[1];
		mag[2] = (float)imusensor.MAG_HK[2] * conversionFactorMag * imusensor.qax_mag_scale[2] * mag_scale_temp[2] + imusensor.qax_mag_offset[2] + mag_offset_temp[2];
	}
#endif //for TWO_U
#ifdef ONE_U
	if(0){}// 1U not temperature compensation
#endif //for ONE_U
	else{
		mag[0] = (float)imusensor.MAG_HK[0] * conversionFactorMag * imusensor.qax_mag_scale[0] + imusensor.qax_mag_offset[0];
		mag[1] = (float)imusensor.MAG_HK[1] * conversionFactorMag * imusensor.qax_mag_scale[1] + imusensor.qax_mag_offset[1];
		mag[2] = (float)imusensor.MAG_HK[2] * conversionFactorMag * imusensor.qax_mag_scale[2] + imusensor.qax_mag_offset[2];
}

	for (int i = 0; i < 3; i++){
		imusensor.MAG[i] = 0.0f;
		for (int j = 0; j < 3; j++) {
			imusensor.MAG[i] += imusensor.QAX_MAGS_DCM[i][j] * mag[j];
		}
	}
	float bunbo = sqrtf(imusensor.MAG[0]*imusensor.MAG[0] + imusensor.MAG[1]*imusensor.MAG[1] +imusensor.MAG[2]*imusensor.MAG[2]);
	 //not to divide by zero    _taguchi
    if(bunbo == 0){
        bunbo += 0.01;
    }
    imusensor.MAGVec[0] = imusensor.MAG[0] / bunbo;
    imusensor.MAGVec[1] = imusensor.MAG[1] / bunbo;
    imusensor.MAGVec[2] = imusensor.MAG[2] / bunbo;

    print(_9AX, "ACC, %d, %f, %d, %f, %d, %f\r\n", imusensor.ACC_HK[0], imusensor.ACC[0], imusensor.ACC_HK[1], imusensor.ACC[1], imusensor.ACC_HK[2], imusensor.ACC[2]);
    print(_9AX, "GYRO, %d, %f, %d, %f, %d, %f\r\n", imusensor.GYRO_HK[0], imusensor.GYRO[0], imusensor.GYRO_HK[1], imusensor.GYRO[1], imusensor.GYRO_HK[2], imusensor.GYRO[2]);
    print(_9AX, "MAG, %d, %f, %d, %f, %d, %f\r\n", imusensor.MAG_HK[0], imusensor.MAG[0], imusensor.MAG_HK[1], imusensor.MAG[1], imusensor.MAG_HK[2], imusensor.MAG[2]);

		//Anomaly detection _GYRO
	for(int i = 0; i < 3; i++){
	if(user_mode->current_mode_id == MD_AOCSFLAG_HIGH_RATE_SPIN){
		if(imusensor.GYRO[i] > imusensor.QAX_GYRO_HIGHSPIN_THRESHOLD[i] || imusensor.GYRO[i] < - imusensor.QAX_GYRO_HIGHSPIN_THRESHOLD[i]){
			print(ANOMALY,"QAX_Gyro_OT_add_anomaly \r\n");
//			AL_add_anomaly(AL_GROUP_QAX,AL_OT_QAX_GYRO);
//			Auto_trans_set(&mode_trans_driver->aocs.enable);    //自動モード遷移検討中
		}
	}
	else{
		if(imusensor.GYRO[i] > imusensor.QAX_GYRO_THRESHOLD[i] || imusensor.GYRO[i] < - imusensor.QAX_GYRO_THRESHOLD[i]){
				print(ANOMALY,"QAX_Gyro_OT_add_anomaly \r\n");
//				AL_add_anomaly(AL_GROUP_QAX,AL_OT_QAX_GYRO);
//				Auto_trans_set(&mode_trans_driver->aocs.enable);    //自動モード遷移検討中
			}
		 }
	}

	              /* Standby と Startup Mode を除く */
	if( (user_mode->current_mode_id != MD_AOCSFLAG_START_UP) && (user_mode->current_mode_id != MD_AOCSFLAG_STANDBY) ){

		/* 9AX と MEMS_GYRO の両方のフラグが下がっているときの対処　 */
		if(!ADS_Mode(NINEAX_GYRO) && !ADS_Mode(MEMS_GYRO)){
				uint32_t current_ads_flag=ADS_Mode_Get();

				if(g_ads.p_ADS->ads_gyro_sensor_choice == NINEAX_GYRO){
					uint32_t temp = 0b0001 << NINEAX_GYRO; //=(10)4
					ADS_Mode_Set(current_ads_flag | temp);         // NINEAX_GYRO=2 のフラグを上げる
				}
				else if(g_ads.p_ADS->ads_gyro_sensor_choice == MEMS_GYRO){
					uint32_t temp= 0b0001 << MEMS_GYRO;
					ADS_Mode_Set(current_ads_flag | temp);        // MEMS_GYRO=8　のフラグを上げる
				}
				else{
					uint32_t temp = 0b0001 << NINEAX_GYRO;
					ADS_Mode_Set(current_ads_flag | temp);        //もしMRAMが変な値の時,9AX_GYROフラグを上げる
				}
		}
	}

		if (ADS_Mode(NINEAX_GYRO)){
			GLOBAL_AOCS_set_GYROX(imusensor.GYRO[0]);
			GLOBAL_AOCS_set_GYROY(imusensor.GYRO[1]);
			GLOBAL_AOCS_set_GYROZ(imusensor.GYRO[2]);
			GLOBAL_AOCS_set_GYRO_TIME(imusensor.qaxsensor_time_before, imusensor.qaxsensor_time_now);
			GLOBAL_AOCS_set_NineAx_GET_TIME(imusensor.qaxsensor_time_now);//可変テレメ用

#ifdef HILS
			float omega[3] = {AOCS_hils->gyro[0], AOCS_hils->gyro[1], AOCS_hils->gyro[2]};
			GLOBAL_AOCS_set_GYROX(omega[0]);
			GLOBAL_AOCS_set_GYROY(omega[1]);
			GLOBAL_AOCS_set_GYROZ(omega[2]);
			imusensor.qaxsensor_time_before_hils = imusensor.qaxsensor_time_now_hils;
			imusensor.qaxsensor_time_now_hils = TMGR_get_master_clock();
			GLOBAL_AOCS_set_GYRO_TIME(imusensor.qaxsensor_time_before_hils, imusensor.qaxsensor_time_now_hils);
#endif
		}

		static int jj = 0;
		// Anomaly detection _MAGS
		for(int i = 0; i < 3; i++){
		if(imusensor.MAGVec[i] == g_acs.p_GLOBAL_AOCS->MagVec[i]) jj++;
		else  jj=0;
		}
		if(jj == imusensor.QAX_MAGS_Continuous_Time_THRESHOLD){
			uint32_t curnt_flag=0;    //current ADS mode flag
			uint32_t temp_flag1=0;    // for calculation
			uint32_t temp_flag2=0;

    //		printf("QAX_MAGS_CONST_AL! \r\n");
			print(ANOMALY,"QAX_MAGS_CONST_add_anomaly \r\n");
//			AL_add_anomaly(AL_GROUP_QAX,AL_CONSTANT_QAX_MAGS);
			// 9AX_MAGSフラグを下げて、precise_MAGSフラグを上げる
			curnt_flag = ADS_Mode_Get();
	//		printf("curnt_flag = %08x ",curnt_flag);
			temp_flag1 = curnt_flag ^ 0x0001;                //  9AX_MAGS Flag = 0
	//		printf("temp_flag1 = %08x ",temp_flag1);
			temp_flag2 = temp_flag1 | 0x0400;                //  precise MAG Flag = 10
	//		printf("temp_flag2 = %08x ",temp_flag2);
//			ADS_Mode_Set(temp_flag2);                 　　　　　　　　　　　　　　 // 自動モード遷移検討中

			jj=0;                                             // reset j
		}

		if (ADS_Mode(NINEAX_MAG)){
			GLOBAL_AOCS_set_MAGX(imusensor.MAG[0]);
			GLOBAL_AOCS_set_MAGY(imusensor.MAG[1]);
			GLOBAL_AOCS_set_MAGZ(imusensor.MAG[2]);
			GLOBAL_AOCS_set_MAGVec(imusensor.MAGVec);
			GLOBAL_AOCS_set_MAG_TIME(imusensor.qaxsensor_time_before, imusensor.qaxsensor_time_now);
			GLOBAL_AOCS_set_NineAx_GET_TIME(imusensor.qaxsensor_time_now);//可変テレメ用


#ifdef HILS
			float B_fld_[3] = { (float)(AOCS_hils->mags[0]*0.000000001f), (float)(AOCS_hils->mags[1] * 0.000000001f),(float)(AOCS_hils->mags[2] * 0.000000001f) };
			float MAGVEC_HILS[3];
			Normalization(B_fld_, 3, MAGVEC_HILS);
			GLOBAL_AOCS_set_MAGX(B_fld_[0]);
			GLOBAL_AOCS_set_MAGY(B_fld_[1]);
			GLOBAL_AOCS_set_MAGZ(B_fld_[2]);
			GLOBAL_AOCS_set_MAGVec(MAGVEC_HILS);
			GLOBAL_AOCS_set_GYRO_TIME(imusensor.qaxsensor_time_before_hils, imusensor.qaxsensor_time_now_hils);
#endif
		}
}


static void QAX_initialize_param(void)
{
	printf("9AX param initialize param.\n");
    write_param(&imusensor.QAX_ACC_DCM, &QAX_ACC_DCM_, (size_t)(sizeof(imusensor.QAX_ACC_DCM)), (uint32_t)0x00004B7C);
    write_param(&imusensor.QAX_GYRO_DCM, &QAX_GYRO_DCM_, (size_t)(sizeof(imusensor.QAX_GYRO_DCM)), (uint32_t)0x00004BA0);
    write_param(&imusensor.QAX_MAGS_DCM, &QAX_MAGS_DCM_, (size_t)(sizeof(imusensor.QAX_MAGS_DCM)), (uint32_t)0x00004BC4);
    write_param(&imusensor.QAX_GYRO_THRESHOLD, &QAX_GYRO_THRESHOLD_, (size_t)(sizeof(imusensor.QAX_GYRO_THRESHOLD)), (uint32_t)0x00004BE8);
    write_param(&imusensor.QAX_GYRO_HIGHSPIN_THRESHOLD, &QAX_GYRO_HIGHSPIN_THRESHOLD_, (size_t)(sizeof(imusensor.QAX_GYRO_HIGHSPIN_THRESHOLD)), (uint32_t)0x00004BF4);
    write_param(&imusensor.QAX_MAGS_Continuous_Time_THRESHOLD,&QAX_MAGS_Continuous_Time_THRESHOLD_,(size_t)(sizeof(imusensor.QAX_MAGS_Continuous_Time_THRESHOLD)),(uint32_t)0x00004C00);
    write_param(&imusensor.qax_acc_scale, &qax_acc_scale_, (float)(sizeof(imusensor.qax_acc_scale)), (uint32_t)0x00004C01);
    write_param(&imusensor.qax_acc_offset, &qax_acc_offset_, (float)(sizeof(imusensor.qax_acc_offset)), (uint32_t)0x00004C0D);
    write_param(&imusensor.qax_gyro_scale, &qax_gyro_scale_, (float)(sizeof(imusensor.qax_gyro_scale)), (uint32_t)0x00004C19);
    write_param(&imusensor.qax_gyro_offset, &qax_gyro_offset_, (float)(sizeof(imusensor.qax_gyro_offset)), (uint32_t)0x00004C25);
    write_param(&imusensor.qax_mag_scale, &qax_mag_scale_, (float)(sizeof(imusensor.qax_mag_scale)), (uint32_t)0x00004C31);
    write_param(&imusensor.qax_mag_offset, &qax_mag_offset_, (float)(sizeof(imusensor.qax_mag_offset)), (uint32_t)0x00004C3D);
    write_param(&imusensor.acc_sf_t0, &acc_sf_t0_, (float)(sizeof(imusensor.acc_sf_t0)), (uint32_t)0x00004C49);
    write_param(&imusensor.acc_sf_t1, &acc_sf_t1_, (float)(sizeof(imusensor.acc_sf_t1)), (uint32_t)0x00004C55);
    write_param(&imusensor.acc_sf_t2, &acc_sf_t2_, (float)(sizeof(imusensor.acc_sf_t2)), (uint32_t)0x00004C61);
    write_param(&imusensor.acc_os_t0, &acc_os_t0_, (float)(sizeof(imusensor.acc_os_t0)), (uint32_t)0x00004C6D);
    write_param(&imusensor.acc_os_t1, &acc_os_t1_, (float)(sizeof(imusensor.acc_os_t1)), (uint32_t)0x00004C79);
    write_param(&imusensor.acc_os_t2, &acc_os_t2_, (float)(sizeof(imusensor.acc_os_t2)), (uint32_t)0x00004C85);
    write_param(&imusensor.gyro_sf_t0, &gyro_sf_t0_, (float)(sizeof(imusensor.gyro_sf_t0)), (uint32_t)0x00004C91);
    write_param(&imusensor.gyro_sf_t1, &gyro_sf_t1_, (float)(sizeof(imusensor.gyro_sf_t1)), (uint32_t)0x00004C9D);
    write_param(&imusensor.gyro_sf_t2, &gyro_sf_t2_, (float)(sizeof(imusensor.gyro_sf_t2)), (uint32_t)0x00004CA9);
    write_param(&imusensor.gyro_os_t0, &gyro_os_t0_, (float)(sizeof(imusensor.gyro_os_t0)), (uint32_t)0x00004CB5);
    write_param(&imusensor.gyro_os_t1, &gyro_os_t1_, (float)(sizeof(imusensor.gyro_os_t1)), (uint32_t)0x00004CC1);
    write_param(&imusensor.gyro_os_t2, &gyro_os_t2_, (float)(sizeof(imusensor.gyro_os_t2)), (uint32_t)0x00004CCD);
    write_param(&imusensor.mag_sf_t0, &mag_sf_t0_, (float)(sizeof(imusensor.mag_sf_t0)), (uint32_t)0x00004CD9);
    write_param(&imusensor.mag_sf_t1, &mag_sf_t1_, (float)(sizeof(imusensor.mag_sf_t1)), (uint32_t)0x00004CE5);
    write_param(&imusensor.mag_sf_t2, &mag_sf_t2_, (float)(sizeof(imusensor.mag_sf_t2)), (uint32_t)0x00004CF1);
    write_param(&imusensor.mag_os_t0, &mag_os_t0_, (float)(sizeof(imusensor.mag_os_t0)), (uint32_t)0x00004CFD);
    write_param(&imusensor.mag_os_t1, &mag_os_t1_, (float)(sizeof(imusensor.mag_os_t1)), (uint32_t)0x00004D09);
    write_param(&imusensor.mag_os_t2, &mag_os_t2_, (float)(sizeof(imusensor.mag_os_t2)), (uint32_t)0x00004D15);
}


static void QAX_load_param(void)
{
	printf("9AX_load_param\r\n");
    read_param(&imusensor.QAX_ACC_DCM, &QAX_ACC_DCM_, (size_t)(sizeof(imusensor.QAX_ACC_DCM)), (uint32_t)0x00004B7C);
    read_param(&imusensor.QAX_GYRO_DCM, &QAX_GYRO_DCM_, (size_t)(sizeof(imusensor.QAX_GYRO_DCM)), (uint32_t)0x00004BA0);
    read_param(&imusensor.QAX_MAGS_DCM, &QAX_MAGS_DCM_, (size_t)(sizeof(imusensor.QAX_MAGS_DCM)), (uint32_t)0x00004BC4);
    read_param(&imusensor.QAX_GYRO_THRESHOLD, &QAX_GYRO_THRESHOLD_, (size_t)(sizeof(imusensor.QAX_GYRO_THRESHOLD)), (uint32_t)0x00004BE8);
    read_param(&imusensor.QAX_GYRO_HIGHSPIN_THRESHOLD, &QAX_GYRO_HIGHSPIN_THRESHOLD_, (size_t)(sizeof(imusensor.QAX_GYRO_HIGHSPIN_THRESHOLD)), (uint32_t)0x00004BF4);
    read_param(&imusensor.QAX_MAGS_Continuous_Time_THRESHOLD,&QAX_MAGS_Continuous_Time_THRESHOLD_,(size_t)(sizeof(imusensor.QAX_MAGS_Continuous_Time_THRESHOLD)),(uint32_t)0x00004C00);
	read_param(&imusensor.qax_acc_scale, &qax_acc_scale_, (float)(sizeof(imusensor.qax_acc_scale)), (uint32_t)0x00004C01);
	read_param(&imusensor.qax_acc_offset, &qax_acc_offset_, (float)(sizeof(imusensor.qax_acc_offset)), (uint32_t)0x00004C0D);
	read_param(&imusensor.qax_gyro_scale, &qax_gyro_scale_, (float)(sizeof(imusensor.qax_gyro_scale)), (uint32_t)0x00004C19);
	read_param(&imusensor.qax_gyro_offset, &qax_gyro_offset_, (float)(sizeof(imusensor.qax_gyro_offset)), (uint32_t)0x00004C25);
	read_param(&imusensor.qax_mag_scale, &qax_mag_scale_, (float)(sizeof(imusensor.qax_mag_scale)), (uint32_t)0x00004C31);
	read_param(&imusensor.qax_mag_offset, &qax_mag_offset_, (float)(sizeof(imusensor.qax_mag_offset)), (uint32_t)0x00004C3D);
	read_param(&imusensor.acc_sf_t0, &acc_sf_t0_, (float)(sizeof(imusensor.acc_sf_t0)), (uint32_t)0x00004C49);
    read_param(&imusensor.acc_sf_t1, &acc_sf_t1_, (float)(sizeof(imusensor.acc_sf_t1)), (uint32_t)0x00004C55);
    read_param(&imusensor.acc_sf_t2, &acc_sf_t2_, (float)(sizeof(imusensor.acc_sf_t2)), (uint32_t)0x00004C61);
    read_param(&imusensor.acc_os_t0, &acc_os_t0_, (float)(sizeof(imusensor.acc_os_t0)), (uint32_t)0x00004C6D);
    read_param(&imusensor.acc_os_t1, &acc_os_t1_, (float)(sizeof(imusensor.acc_os_t1)), (uint32_t)0x00004C79);
    read_param(&imusensor.acc_os_t2, &acc_os_t2_, (float)(sizeof(imusensor.acc_os_t2)), (uint32_t)0x00004C85);
    read_param(&imusensor.gyro_sf_t0, &gyro_sf_t0_, (float)(sizeof(imusensor.gyro_sf_t0)), (uint32_t)0x00004C91);
    read_param(&imusensor.gyro_sf_t1, &gyro_sf_t1_, (float)(sizeof(imusensor.gyro_sf_t1)), (uint32_t)0x00004C9D);
    read_param(&imusensor.gyro_sf_t2, &gyro_sf_t2_, (float)(sizeof(imusensor.gyro_sf_t2)), (uint32_t)0x00004CA9);
    read_param(&imusensor.gyro_os_t0, &gyro_os_t0_, (float)(sizeof(imusensor.gyro_os_t0)), (uint32_t)0x00004CB5);
    read_param(&imusensor.gyro_os_t1, &gyro_os_t1_, (float)(sizeof(imusensor.gyro_os_t1)), (uint32_t)0x00004CC1);
    read_param(&imusensor.gyro_os_t2, &gyro_os_t2_, (float)(sizeof(imusensor.gyro_os_t2)), (uint32_t)0x00004CCD);
    read_param(&imusensor.mag_sf_t0, &mag_sf_t0_, (float)(sizeof(imusensor.mag_sf_t0)), (uint32_t)0x00004CD9);
    read_param(&imusensor.mag_sf_t1, &mag_sf_t1_, (float)(sizeof(imusensor.mag_sf_t1)), (uint32_t)0x00004CE5);
    read_param(&imusensor.mag_sf_t2, &mag_sf_t2_, (float)(sizeof(imusensor.mag_sf_t2)), (uint32_t)0x00004CF1);
    read_param(&imusensor.mag_os_t0, &mag_os_t0_, (float)(sizeof(imusensor.mag_os_t0)), (uint32_t)0x00004CFD);
    read_param(&imusensor.mag_os_t1, &mag_os_t1_, (float)(sizeof(imusensor.mag_os_t1)), (uint32_t)0x00004D09);
    read_param(&imusensor.mag_os_t2, &mag_os_t2_, (float)(sizeof(imusensor.mag_os_t2)), (uint32_t)0x00004D15);
}


CCP_CmdRet Cmd_9AX_change_acc_config(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t ret = 0;

	if(param[0] > 3)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	if(param[1] > 6)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	if(param[2] > 3)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	if(param[3] > 1)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	imusensor.lsm9ds1.acc_scale = (ACC_SCALE)param[0];
	imusensor.lsm9ds1.acc_odr = (ACC_ODR)param[1];
	imusensor.lsm9ds1.acc_bw = (ACC_BANDWIDTH)param[2];
	imusensor.lsm9ds1.acc_bw_select = param[3];

	ret = lsm9ds1_power_on_init(&imusensor.lsm9ds1);
	if(ret != 0)ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, (HAL_StatusTypeDef)ret);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_9AX_change_gyro_config(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t ret = 0;

	if(param[0] > 3)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    if(param[1] > 6)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    if(param[2] > 3)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    if(param[3] > 1)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	imusensor.lsm9ds1.gyro_scale = (GYRO_SCALE)param[0];
    imusensor.lsm9ds1.gyro_odr = (GYRO_ODR)param[1];
    imusensor.lsm9ds1.gyro_bw = param[2];
    imusensor.lsm9ds1.gyro_low_power = param[3];

	ret = lsm9ds1_power_on_init(&imusensor.lsm9ds1);
	if(ret != 0)ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, (HAL_StatusTypeDef)ret);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_9AX_change_mag_config(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t ret = 0;

	if(param[0] > 3)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    if(param[1] > 7)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    if(param[2] > 3)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    if(param[3] > 3)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	imusensor.lsm9ds1.mag_scale = (MAG_SCALE)param[0];
    imusensor.lsm9ds1.mag_odr = (MAG_ODR)param[1];
    imusensor.lsm9ds1.mag_system_mode = (MAG_SYSTEM_MODE)param[2];
    imusensor.lsm9ds1.mag_mode = (MAG_MODE)param[3];

	ret = lsm9ds1_power_on_init(&imusensor.lsm9ds1);
	if(ret != 0)ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, (HAL_StatusTypeDef)ret);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
