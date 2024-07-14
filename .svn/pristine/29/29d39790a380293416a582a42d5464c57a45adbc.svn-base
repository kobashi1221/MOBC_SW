
/**
  *****************************************************************************************
  * @file    : ACS1_debug_ADC.cpp
  * @author  : Satoshi NAKAYAMA
  * @version : 1.0
  * @date    : 2020/10/15
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/10/15  1.0   S.NAKAYAMA      First revision
  * 2020/12/01  2.0   H.MASUDA
  *
  *****************************************************************************************
  */
#ifdef TWO_U
#include "ACS1_cmd_ADC.h"

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../Global_Sensor_Value.h"
#include <src_user/Drivers/Sensor/ADC.h>
#include <src_user/Drivers/Sensor/GYRO.h>
#include <src_user/Drivers/Sensor/MAGS.h>
#include <src_user/Drivers/Sensor/PlatinumSensor.h>
#include <src_user/Drivers/Sensor/VOLS.h>
#include <src_user/Drivers/Sensor/CURS.h>
#include <src_user/Drivers/Sensor/PLASMA.h>
#include <src_user/Drivers/Sensor/MTQ.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Drivers/Sensor/MTQ.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.h>
#include <src_user/Library/Delay.h>
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/DriverInstances/block_command_handler.h>    //for current mode id
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>

static ADC_STRUCT adc_; //structure for ADC
static GYRO_CLASS gyro_class;
static MAGS_CLASS mag_class;
static PLATINUM_CLASS pt_class;
static VOLS_CLASS vol_class;
static CURS_CLASS cur_class;
static PLASMA_CLASS plasma_class;

static MTQ_CLASS MTQ_Class;

I2C_Class MTQ_ADC(&hi2c3, 0xA8, 0xA9, 0x0F);

static void ACS1_debug_adc_init(void);
static void ACS1_debug_adc_exe(void);

static void ADC_initialize_param(void);
static void ADC_load_param(void);


/**
 * @brief App making function
 *
 * @note
 * @return structure of App
 */

AppInfo ACS1_debug_adc_update(void)
{
    //create_app_info()を呼び出す
    //
    return AI_create_app_info("DBG_ADC", ACS1_debug_adc_init, NULL); //アプリを作成
}

AppInfo ACS1_param_update(void)
{
    //create_app_info()を呼び出す
    //
    return AI_create_app_info("sample_param_init", ADC_load_param, ADC_initialize_param);
}

/**
 * @brief App initialize function.
 * @note
 */
static void ACS1_debug_adc_init(void)
{
    int ret = 0;

//    printf("ACS1 ADC Debug App initialize.\r\n");
    g_sensor.p_adc = &adc_;

    gyro_class.GYRO_RESET_Port = RESET3V3_MOBCtoGYRO_GPIO_Port;
	gyro_class.GYRO_RESET_Pin = RESET3V3_MOBCtoGYRO_Pin;
    gyro_class.GYROX_SEL0_Port = SEL0_3V3_MOBCtoGYROX_GPIO_Port;
	gyro_class.GYROX_SEL0_Pin = SEL0_3V3_MOBCtoGYROX_Pin;
    gyro_class.GYROX_SEL1_Port = SEL1_3V3_MOBCtoGYROX_GPIO_Port;
	gyro_class.GYROX_SEL1_Pin = SEL1_3V3_MOBCtoGYROX_Pin;
    gyro_class.GYROY_SEL0_Port = SEL0_3V3_MOBCtoGYROY_GPIO_Port;
	gyro_class.GYROY_SEL0_Pin = SEL0_3V3_MOBCtoGYROY_Pin;
    gyro_class.GYROY_SEL1_Port = SEL1_3V3_MOBCtoGYROY_GPIO_Port;
	gyro_class.GYROY_SEL1_Pin = SEL1_3V3_MOBCtoGYROY_Pin;
    gyro_class.GYROZ_SEL0_Port = SEL0_3V3_MOBCtoGYROZ_GPIO_Port;
	gyro_class.GYROZ_SEL0_Pin = SEL0_3V3_MOBCtoGYROZ_Pin;
    gyro_class.GYROZ_SEL1_Port = SEL1_3V3_MOBCtoGYROZ_GPIO_Port;
	gyro_class.GYROZ_SEL1_Pin = SEL1_3V3_MOBCtoGYROZ_Pin;

	//MEMS_GYROレンジの初期値を設定する ±75deg/s
	for(int i=0; i<2; i++){
		gyro_class.state[i] = 0;
	}
	gyro_class.GYRO_SENSOR_CHANGE_RANGE(gyro_class.state);

    //Error handling
    if (ret != 0)
    {
        printf("ADC init Failed! %d \r\n", ret);
    }
}


static void ADC_load_param(void)
{
	printf("ADC_load_param\r\n");
//	printf("***************************\r\n");
    read_param(&gyro_class.state, &GYRO_CLASS::state_, (size_t)(sizeof(gyro_class.state)), (uint32_t)0x00004A00);  //uint8_t
    read_param(&gyro_class.k_freq, &GYRO_CLASS::k_freq_, (size_t)(sizeof(gyro_class.k_freq)), (uint32_t)0x00004A03);
    read_param(&gyro_class.k_gyro_scale, &GYRO_CLASS::k_gyro_scale_, (size_t)(sizeof(gyro_class.k_gyro_scale)), (uint32_t)0x00004A0F);
    read_param(&gyro_class.k_gyro_vols_bias, &GYRO_CLASS::k_gyro_vols_bias_, (size_t)(sizeof(gyro_class.k_gyro_vols_bias)), (uint32_t)0x00004A1B); // edit on 2023/09/11
    read_param(&gyro_class.k_InputVol, &GYRO_CLASS::k_InputVol_, (size_t)(sizeof(gyro_class.k_InputVol)), (uint32_t)0x00004A27);
    read_param(&gyro_class.k_K, &GYRO_CLASS::k_K_, (size_t)(sizeof(gyro_class.k_K)), (uint32_t)0x00004A33);
    read_param(&gyro_class.ACS1_GYRO_DCM, &GYRO_CLASS::ACS1_GYRO_DCM_, (size_t)(sizeof(gyro_class.ACS1_GYRO_DCM)), (uint32_t)0x00004A3F);
    read_param(&mag_class.k_mag_scale, &MAGS_CLASS::k_mag_scale_, (size_t)(sizeof(mag_class.k_mag_scale)), (uint32_t)0x00004A87);
    read_param(&mag_class.k_mag_offset, &MAGS_CLASS::k_mag_offset_, (size_t)(sizeof(mag_class.k_mag_offset)), (uint32_t)0x00004A93);
    read_param(&mag_class.MAGS_DCM, &MAGS_CLASS::MAGS_DCM_, (size_t)(sizeof(mag_class.MAGS_DCM)), (uint32_t)0x000049F);
    read_param(&pt_class.k_AppliedCurrent, &PLATINUM_CLASS::k_AppliedCurrent_, (size_t)sizeof(pt_class.k_AppliedCurrent), (uint32_t)0x00004AC3);
    read_param(&gyro_class.GYRO_HIGHSPIN_THRESHOLD,&GYRO_CLASS::GYRO_HIGHSPIN_THRESHOLD_,(size_t)(sizeof(gyro_class.GYRO_HIGHSPIN_THRESHOLD)),(uint32_t)0x00004ACF);
    read_param(&gyro_class.GYRO_THRESHOLD,&GYRO_CLASS::GYRO_THRESHOLD_,(size_t)(sizeof(gyro_class.GYRO_THRESHOLD)),(uint32_t)0x00004ADB);
    read_param(&mag_class.MAGS_Continuous_Time_THRESHOLD,&MAGS_CLASS::MAGS_Continuous_Time_THRESHOLD_,(size_t)(sizeof(mag_class.MAGS_Continuous_Time_THRESHOLD)),(uint32_t)0x00004AE7);
    read_param(&pt_class.PT_TEMP_THRESHOLD,&PLATINUM_CLASS::PT_TEMP_THRESHOLD_,(size_t)sizeof(pt_class.PT_TEMP_THRESHOLD),(uint32_t)0x00004AE8);
    read_param(&gyro_class.mems_gyro_sf_t0,&GYRO_CLASS::mems_gyro_sf_t0_,(size_t)sizeof(gyro_class.mems_gyro_sf_t0),(uint32_t)0x00004AEC);
    read_param(&gyro_class.mems_gyro_sf_t1,&GYRO_CLASS::mems_gyro_sf_t1_,(size_t)sizeof(gyro_class.mems_gyro_sf_t1),(uint32_t)0x00004AF8);
    read_param(&gyro_class.mems_gyro_sf_t2,&GYRO_CLASS::mems_gyro_sf_t2_,(size_t)sizeof(gyro_class.mems_gyro_sf_t2),(uint32_t)0x00004B04);
    read_param(&gyro_class.mems_gyro_os_t0,&GYRO_CLASS::mems_gyro_os_t0_,(size_t)sizeof(gyro_class.mems_gyro_os_t0),(uint32_t)0x00004B10);
    read_param(&gyro_class.mems_gyro_os_t1,&GYRO_CLASS::mems_gyro_os_t1_,(size_t)sizeof(gyro_class.mems_gyro_os_t1),(uint32_t)0x00004B1C);
    read_param(&gyro_class.mems_gyro_os_t2,&GYRO_CLASS::mems_gyro_os_t2_,(size_t)sizeof(gyro_class.mems_gyro_os_t2),(uint32_t)0x00004B28);
    read_param(&mag_class.mems_mag_sf_t0,&MAGS_CLASS::mems_mag_sf_t0_,(size_t)sizeof(mag_class.mems_mag_sf_t0),(uint32_t)0x00004B34);
    read_param(&mag_class.mems_mag_sf_t1,&MAGS_CLASS::mems_mag_sf_t1_,(size_t)sizeof(mag_class.mems_mag_sf_t1),(uint32_t)0x00004B40);
    read_param(&mag_class.mems_mag_sf_t2,&MAGS_CLASS::mems_mag_sf_t2_,(size_t)sizeof(mag_class.mems_mag_sf_t2),(uint32_t)0x00004B4C);
    read_param(&mag_class.mems_mag_os_t0,&MAGS_CLASS::mems_mag_os_t0_,(size_t)sizeof(mag_class.mems_mag_os_t0),(uint32_t)0x00004B58);
    read_param(&mag_class.mems_mag_os_t1,&MAGS_CLASS::mems_mag_os_t1_,(size_t)sizeof(mag_class.mems_mag_os_t1),(uint32_t)0x00004B64);
    read_param(&mag_class.mems_mag_os_t2,&MAGS_CLASS::mems_mag_os_t2_,(size_t)sizeof(mag_class.mems_mag_os_t2),(uint32_t)0x00004B70);
}


static void ADC_initialize_param(void)
{
	printf("ADC_initialize_param.\n");
    write_param(&gyro_class.state, &GYRO_CLASS::state_, (size_t)(sizeof(gyro_class.state)), (uint32_t)0x00004A00);  //uint8_t
    write_param(&gyro_class.k_freq, &GYRO_CLASS::k_freq_, (size_t)(sizeof(gyro_class.k_freq)), (uint32_t)0x00004A03);
    write_param(&gyro_class.k_gyro_scale, &GYRO_CLASS::k_gyro_scale_, (size_t)(sizeof(gyro_class.k_gyro_scale)), (uint32_t)0x00004A0F);
    write_param(&gyro_class.k_gyro_vols_bias, &GYRO_CLASS::k_gyro_vols_bias_, (size_t)(sizeof(gyro_class.k_gyro_vols_bias)), (uint32_t)0x00004A1B);
    write_param(&gyro_class.k_InputVol, &GYRO_CLASS::k_InputVol_, (size_t)(sizeof(gyro_class.k_InputVol)), (uint32_t)0x00004A27);
    write_param(&gyro_class.k_K, &GYRO_CLASS::k_K_, (size_t)(sizeof(gyro_class.k_K)), (uint32_t)0x00004A33);
    write_param(&gyro_class.ACS1_GYRO_DCM, &GYRO_CLASS::ACS1_GYRO_DCM_, (size_t)(sizeof(gyro_class.ACS1_GYRO_DCM)), (uint32_t)0x00004A3F);
    write_param(&mag_class.k_mag_scale, &MAGS_CLASS::k_mag_scale_, (size_t)(sizeof(mag_class.k_mag_scale)), (uint32_t)0x00004A87);
    write_param(&mag_class.k_mag_offset, &MAGS_CLASS::k_mag_offset_, (size_t)(sizeof(mag_class.k_mag_offset)), (uint32_t)0x00004A93);
    write_param(&mag_class.MAGS_DCM, &MAGS_CLASS::MAGS_DCM_, (size_t)(sizeof(mag_class.MAGS_DCM)), (uint32_t)0x000049F);
    write_param(&pt_class.k_AppliedCurrent, &PLATINUM_CLASS::k_AppliedCurrent_, (size_t)sizeof(pt_class.k_AppliedCurrent), (uint32_t)0x00004AC3);
    write_param(&gyro_class.GYRO_HIGHSPIN_THRESHOLD,&GYRO_CLASS::GYRO_HIGHSPIN_THRESHOLD_,(size_t)(sizeof(gyro_class.GYRO_HIGHSPIN_THRESHOLD)),(uint32_t)0x00004ACF);
    write_param(&gyro_class.GYRO_THRESHOLD,&GYRO_CLASS::GYRO_THRESHOLD_,(size_t)(sizeof(gyro_class.GYRO_THRESHOLD)),(uint32_t)0x00004ADB);
    write_param(&mag_class.MAGS_Continuous_Time_THRESHOLD,&MAGS_CLASS::MAGS_Continuous_Time_THRESHOLD_,(size_t)(sizeof(mag_class.MAGS_Continuous_Time_THRESHOLD)),(uint32_t)0x00004AE7);
    write_param(&pt_class.PT_TEMP_THRESHOLD,&PLATINUM_CLASS::PT_TEMP_THRESHOLD_,(size_t)sizeof(pt_class.PT_TEMP_THRESHOLD),(uint32_t)0x00004AE8);
    write_param(&gyro_class.mems_gyro_sf_t0,&GYRO_CLASS::mems_gyro_sf_t0_,(size_t)sizeof(gyro_class.mems_gyro_sf_t0),(uint32_t)0x00004AEC);
    write_param(&gyro_class.mems_gyro_sf_t1,&GYRO_CLASS::mems_gyro_sf_t1_,(size_t)sizeof(gyro_class.mems_gyro_sf_t1),(uint32_t)0x00004AF8);
    write_param(&gyro_class.mems_gyro_sf_t2,&GYRO_CLASS::mems_gyro_sf_t2_,(size_t)sizeof(gyro_class.mems_gyro_sf_t2),(uint32_t)0x00004B04);
    write_param(&gyro_class.mems_gyro_os_t0,&GYRO_CLASS::mems_gyro_os_t0_,(size_t)sizeof(gyro_class.mems_gyro_os_t0),(uint32_t)0x00004B10);
    write_param(&gyro_class.mems_gyro_os_t1,&GYRO_CLASS::mems_gyro_os_t1_,(size_t)sizeof(gyro_class.mems_gyro_os_t1),(uint32_t)0x00004B1C);
    write_param(&gyro_class.mems_gyro_os_t2,&GYRO_CLASS::mems_gyro_os_t2_,(size_t)sizeof(gyro_class.mems_gyro_os_t2),(uint32_t)0x00004B28);
    write_param(&mag_class.mems_mag_sf_t0,&MAGS_CLASS::mems_mag_sf_t0_,(size_t)sizeof(mag_class.mems_mag_sf_t0),(uint32_t)0x00004B34);
    write_param(&mag_class.mems_mag_sf_t1,&MAGS_CLASS::mems_mag_sf_t1_,(size_t)sizeof(mag_class.mems_mag_sf_t1),(uint32_t)0x00004B40);
    write_param(&mag_class.mems_mag_sf_t2,&MAGS_CLASS::mems_mag_sf_t2_,(size_t)sizeof(mag_class.mems_mag_sf_t2),(uint32_t)0x00004B4C);
    write_param(&mag_class.mems_mag_os_t0,&MAGS_CLASS::mems_mag_os_t0_,(size_t)sizeof(mag_class.mems_mag_os_t0),(uint32_t)0x00004B58);
    write_param(&mag_class.mems_mag_os_t1,&MAGS_CLASS::mems_mag_os_t1_,(size_t)sizeof(mag_class.mems_mag_os_t1),(uint32_t)0x00004B64);
    write_param(&mag_class.mems_mag_os_t2,&MAGS_CLASS::mems_mag_os_t2_,(size_t)sizeof(mag_class.mems_mag_os_t2),(uint32_t)0x00004B70);
}

/**
 * @brief GYRO sensor.
 * @note
 */
CCP_CmdRet Cmd_ACS1_debug_GYRO(const CommonCmdPacket* packet)
{
		gyro_class.GYRO_SENSOR_init(gyro_class.state);

		gyro_class.GYRO_SENSOR_get_all();

	  //Anomaly detection
for(int i = 0; i < 3; i++){
	if(user_mode->current_mode_id == MD_AOCSFLAG_HIGH_RATE_SPIN){
			if(gyro_class.OMEGA[i] > gyro_class.GYRO_HIGHSPIN_THRESHOLD[i] || gyro_class.OMEGA[i] < -gyro_class.GYRO_HIGHSPIN_THRESHOLD[i]){
//				printf("OMEGA[%d]=%f THRESH[%d]=%f\r\n",i,gyro_class.OMEGA[i],i,gyro_class.GYRO_HIGHSPIN_THRESHOLD[i]);
				print(ANOMALY,"Gyro_OT_add_anomaly \r\n");
//				AL_add_anomaly(AL_GROUP_GYRO,AL_OT_GYRO);
//				Auto_trans_set(&mode_trans_driver->aocs.enable);
			}
		}
	else{
			if(gyro_class.OMEGA[i] > gyro_class.GYRO_THRESHOLD[i] || gyro_class.OMEGA[i] < -gyro_class.GYRO_THRESHOLD [i]){
//				printf("OMEGA[%d]=%f THRESH[%d]=%f\r\n",i,gyro_class.OMEGA[i],i,gyro_class.GYRO_THRESHOLD[i]);
				print(ANOMALY,"Gyro_OT_add_anomaly \r\n");
//				AL_add_anomaly(AL_GROUP_GYRO,AL_OT_GYRO);
//				Auto_trans_set(&mode_trans_driver->aocs.enable); // temporary tamaoki
			}
		}
}

		adc_.ADC_OMEGAX = gyro_class.ADC_omegax;
		adc_.OMEGAX = gyro_class.OMEGA[0];

		adc_.ADC_OMEGAY = gyro_class.ADC_omegay;
		adc_.OMEGAY = gyro_class.OMEGA[1];

		adc_.ADC_OMEGAZ = gyro_class.ADC_omegaz;
		adc_.OMEGAZ = gyro_class.OMEGA[2];

//		printf("GYROXYZ,%u,%u,%u\r\n", adc_.ADC_OMEGAX, adc_.ADC_OMEGAY, adc_.ADC_OMEGAZ);
//		print(GYRO, "OMEGAXYZ,%u,%u,%u, %f,%f,%f\r\n", adc_.ADC_OMEGAX, adc_.ADC_OMEGAY, adc_.ADC_OMEGAZ, adc_.OMEGAX, adc_.OMEGAY, adc_.OMEGAZ);
		print(GYRO, "OMEGAX, %u, %f\r\n", adc_.ADC_OMEGAX, adc_.OMEGAX);
		print(GYRO, "OMEGAY, %u, %f\r\n", adc_.ADC_OMEGAY, adc_.OMEGAY);
		print(GYRO, "OMEGAZ, %u, %f\r\n\r\n", adc_.ADC_OMEGAZ, adc_.OMEGAZ);

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

		if (ADS_Mode(MEMS_GYRO)){
			GLOBAL_AOCS_set_GYROX(adc_.OMEGAX);
			GLOBAL_AOCS_set_GYROY(adc_.OMEGAY);
			GLOBAL_AOCS_set_GYROZ(adc_.OMEGAZ);
			GLOBAL_AOCS_set_GYRO_TIME(gyro_class.gyrosensor_time_before, gyro_class.gyrosensor_time_now);
			GLOBAL_AOCS_set_MEMS_GYRO_GET_TIME(gyro_class.gyrosensor_time_now);//可変テレメ用

//			printf("gyro_pre:%d\r\n", gyro_class.gyrosensor_time_before.step);
//			printf("gyro_now:%d\r\n", gyro_class.gyrosensor_time_now.step);
//			uint32_t dt = OBCT_diff_in_step(&gyro_class.gyrosensor_time_before, &gyro_class.gyrosensor_time_now);
//			printf("dt:%d\r\n", dt);
		}
#ifdef HILS
		float omega[3] = {AOCS_hils->gyro[0], AOCS_hils->gyro[1], AOCS_hils->gyro[2]};
		GLOBAL_AOCS_set_GYROX(omega[0]);
		GLOBAL_AOCS_set_GYROY(omega[1]);
		GLOBAL_AOCS_set_GYROZ(omega[2]);
		gyro_class.GYRO_SENSOR_get_time_HILS();
		GLOBAL_AOCS_set_GYRO_TIME(gyro_class.gyrosensor_time_before_hils, gyro_class.gyrosensor_time_now_hils);
//		printf("gyro_pre_HILS:%d\r\n", gyro_class.gyrosensor_time_before_hils.step);
//		printf("gyro_now_HILS:%d\r\n", gyro_class.gyrosensor_time_now_hils.step);
//		uint32_t dt = OBCT_diff_in_step(&gyro_class.gyrosensor_time_before_hils, &gyro_class.gyrosensor_time_now_hils);
//		printf("dt:%d\r\n", dt);
#endif

		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief change GYRO sensor range
 * @note
 */
CCP_CmdRet Cmd_ACS1_Change_GYRO_Range(const CommonCmdPacket* packet)
{
	uint8_t param[3] = {0};

	param[0] = CCP_get_param_head(packet)[0];
	param[1] = CCP_get_param_head(packet)[1];
	param[2] = CCP_get_param_head(packet)[2];

/**
* param[0](hex value) | mems gyro sensor range
* ------------------------------------------
*  0x00               | MEMS_GYRO_RANGE_75
*  0x01               | MEMS_GYRO_RANGE_150
*  0x02               | MEMS_GYRO_RANGE_300
*  0x03               | MEMS_GYRO_RANGE_900
*/

	// GYROのレンジが上記以外の場合，コマンドのパラメータエラーとして処理する
	for  (int i = 0; i < 3; i++){
	  if (param[i] < 0x00 || param[i] > 0x03) {
	    printf("invalid range param\r\n");
	    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	  }
	  else {
	    // THROUGH
	  }
	}

	for (int i = 0; i < 3; i++){
		gyro_class.state[i] = param[i];
		adc_.state[i] = gyro_class.state[i];
	}

	gyro_class.GYRO_SENSOR_CHANGE_RANGE(gyro_class.state);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/*
 * @brief MAG sensor.
 * @note
 */
//----------------------------------MAGS PX-----------------------------------------------

CCP_CmdRet Cmd_ACS1_debug_MAGS(const CommonCmdPacket* packet)
{
	static uint8_t j=0;         		// Counter for Continuous Anomaly

	mag_class.MAGS_SENSOR_init();
	mag_class.MAGS_SENSOR_get_all();

	adc_.ADC_MAGSX = mag_class.ADC_magx;
	adc_.MAGSX = mag_class.MAG[0];

	adc_.ADC_MAGSY = mag_class.ADC_magy;
	adc_.MAGSY = mag_class.MAG[1];

	adc_.ADC_MAGSZ = mag_class.ADC_magz;
	adc_.MAGSZ = mag_class.MAG[2];

	adc_.ADC_MAGSPX = mag_class.ADC_magpx;
	adc_.MAGSPX = mag_class.magpx;
	adc_.ADC_MAGSMX = mag_class.ADC_magmx;
	adc_.MAGSMX = mag_class.magmx;
	adc_.ADC_MAGSPY = mag_class.ADC_magpy;
	adc_.MAGSPY = mag_class.magpy;
	adc_.ADC_MAGSMY = mag_class.ADC_magmy;
	adc_.MAGSMY = mag_class.magmy;
	adc_.ADC_MAGSPZ = mag_class.ADC_magpz;
	adc_.MAGSPZ = mag_class.magpz;
	adc_.ADC_MAGSMZ = mag_class.ADC_magmz;
	adc_.MAGSMZ = mag_class.magmz;

	print(MAGS, "MAGSX,%d, %f\r\n", adc_.ADC_MAGSX, adc_.MAGSX);
	print(MAGS, "MAGSY,%d, %f\r\n", adc_.ADC_MAGSY, adc_.MAGSY);
	print(MAGS, "MAGSZ,%d, %f\r\n\r\n", adc_.ADC_MAGSZ, adc_.MAGSZ);
//	printf("MAGSXYZ,%f,%f,%f\r\n", adc_.MAGSX, adc_.MAGSY, adc_.MAGSZ);

	for(int i = 0; i < 3; i ++){
		adc_.MAGVec[i] = mag_class.MAGVec[i];
	}

	// Anomaly detection for CONST Value
	for(int i = 0; i < 3; i++){
			if(adc_.MAGVec[i] == g_acs.p_GLOBAL_AOCS->MagVec[i]) j++;
			else j=0;
		}

//	printf("MAGS___j=%d\r\n",j);
//	printf("MAGXYZ (%f %f %f)\r\n",adc_.MAGSX, adc_.MAGSY, adc_.MAGSZ);
//	printf("MAGVec (%f %f %f)\r\n",adc_.MAGVec[0], adc_.MAGVec[1], adc_.MAGVec[2]);

    // Anomaly handler
	if(j == mag_class.MAGS_Continuous_Time_THRESHOLD){
		uint32_t curnt_flag=0;    //current ADS mode flag
		uint32_t temp_flag1=0;    // for calculation
		uint32_t temp_flag2=0;

//		printf("MAGS_CONST_AL! \r\n");
		print(ANOMALY,"MAGS_CONST_add_anomaly \r\n");
//		AL_add_anomaly(AL_GROUP_MAGS,AL_CONSTANT_MAGS);
		// precise_MAGSフラグを下げて、9AX_MAGSフラグを上げる
		curnt_flag = ADS_Mode_Get();
//		printf("curnt_flag = %08x ",curnt_flag);
		temp_flag1 = curnt_flag ^ 0x0400;          // precise MAG Flag = 10
//		printf("temp_flag1 = %08x ",temp_flag1);
		temp_flag2 = temp_flag1 | 1;                // 9AX_MAGS Flag = 0
//		printf("temp_flag2 = %08x ",temp_flag2);
//		ADS_Mode_Set(temp_flag2);                  // 動作確認できるまでコメントアウト

		j=0;                                         // reset j
	}


	if (ADS_Mode(PRECIZE_MAG)){
		GLOBAL_AOCS_set_MAGX(adc_.MAGSX);
		GLOBAL_AOCS_set_MAGY(adc_.MAGSY);
		GLOBAL_AOCS_set_MAGZ(adc_.MAGSZ);
		GLOBAL_AOCS_set_MAGVec(adc_.MAGVec);
		GLOBAL_AOCS_set_MAG_TIME(mag_class.magsensor_time_before, mag_class.magsensor_time_now);
		GLOBAL_AOCS_set_MEMS_MAG_GET_TIME(mag_class.magsensor_time_now);//可変テレメ用
	}

//	printf("MAG_field (%f %f %f)\r\n",g_acs.p_GLOBAL_AOCS->MagField[0], g_acs.p_GLOBAL_AOCS->MagField[1], g_acs.p_GLOBAL_AOCS->MagField[2]);

//	printf("Magfield (%f %f %f)\r\n",g_acs.p_GLOBAL_AOCS->MagField[0], g_acs.p_GLOBAL_AOCS->MagField[1], g_acs.p_GLOBAL_AOCS->MagField[2]);

#ifdef HILS
	float B_fld_[3] = {(float)(AOCS_hils->mags[0]*0.000000001f), (float)(AOCS_hils->mags[1] * 0.000000001f),(float)(AOCS_hils->mags[2] * 0.000000001f) };
	float MAGVEC_HILS[3];
	Normalization(B_fld_, 3, MAGVEC_HILS);
	GLOBAL_AOCS_set_MAGX(B_fld_[0]);
	GLOBAL_AOCS_set_MAGY(B_fld_[1]);
	GLOBAL_AOCS_set_MAGZ(B_fld_[2]);
	GLOBAL_AOCS_set_MAGVec(MAGVEC_HILS);
	mag_class.MAG_SENSOR_get_time_HILS();
	GLOBAL_AOCS_set_MAG_TIME(mag_class.magsensor_time_before_hils, mag_class.magsensor_time_now_hils);
#endif

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief CURS sensor on ACS2
 * @note
 */
CCP_CmdRet Cmd_ACS1_debug_CURS(const CommonCmdPacket* packet)
{
	const uint8_t *param_head;
	int len=1;
	uint8_t param[1]={0};
	param_head = CCP_get_param_head(packet);
	memcpy (param, param_head, len);

	cur_class.CURS_SENSOR_init();

	if(param[0] == 0){
		cur_class.CURS_SENSOR_get_all();

		adc_.CURS_MTQZ4 = cur_class.curmtqz4;
		adc_.ADC_CURS_MTQZ4 = cur_class.ADC_curmtqz4;

		adc_.CURS_MTQZ3 = cur_class.curmtqz3;
		adc_.ADC_CURS_MTQZ3 = cur_class.ADC_curmtqz3;

		adc_.CURS_MTQZ2 = cur_class.curmtqz2;
		adc_.ADC_CURS_MTQZ2 = cur_class.ADC_curmtqz2;

		adc_.CURS_MTQZ1 = cur_class.curmtqz1;
		adc_.ADC_CURS_MTQZ1 = cur_class.ADC_curmtqz1;

		adc_.CURS_MTQY = cur_class.curmtqy;
		adc_.ADC_CURS_MTQY = cur_class.ADC_curmtqy;

		adc_.CURS_MTQX = cur_class.curmtqx;
		adc_.ADC_CURS_MTQX = cur_class.ADC_curmtqx;

		adc_.CURS_MTQTWO_U = cur_class.curmtq2u;
		adc_.ADC_CURS_MTQ2U = cur_class.ADC_curmtq2u;

        adc_.CURS_TX = cur_class.curtx;
        adc_.ADC_CURS_TX = cur_class.ADC_curtx;

        print(PWR_ADC_ACS1, "cursmtqz4 data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQZ4, g_pwr_thres->AT_CURS_MTQ_ACS1[5][0], g_pwr_thres->AT_CURS_MTQ_ACS1[5][1]);
        print(PWR_ADC_ACS1, "cursmtqz3 data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQZ3, g_pwr_thres->AT_CURS_MTQ_ACS1[4][0], g_pwr_thres->AT_CURS_MTQ_ACS1[4][1]);
        print(PWR_ADC_ACS1, "cursmtqz2 data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQZ2, g_pwr_thres->AT_CURS_MTQ_ACS1[3][0], g_pwr_thres->AT_CURS_MTQ_ACS1[3][1]);
        print(PWR_ADC_ACS1, "cursmtqz1 data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQZ1, g_pwr_thres->AT_CURS_MTQ_ACS1[2][0], g_pwr_thres->AT_CURS_MTQ_ACS1[2][1]);
        print(PWR_ADC_ACS1, "cursmtqy data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQY, g_pwr_thres->AT_CURS_MTQ_ACS1[1][0], g_pwr_thres->AT_CURS_MTQ_ACS1[1][1]);
        print(PWR_ADC_ACS1, "cursmtqx data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQX, g_pwr_thres->AT_CURS_MTQ_ACS1[0][0], g_pwr_thres->AT_CURS_MTQ_ACS1[0][1]);
        print(PWR_ADC_ACS1, "cursmtq2u data, oc: %d, %d\r\n", adc_.ADC_CURS_MTQ2U, g_pwr_thres->AT_CURS_MTQ);
        print(PWR_ADC_ACS1, "CURS_TX,%u,%f\r\n", adc_.ADC_CURS_TX, adc_.CURS_TX);
	}
	else if(param[0] == 1) {
		cur_class.CURS_SENSOR_get_MTQZ4();

		adc_.CURS_MTQZ4 = cur_class.curmtqz4;
		adc_.ADC_CURS_MTQZ4 = cur_class.ADC_curmtqz4;
//		print(ACS_ADC, "CURS_MTQZ4,%u,%f\r\n", adc_.ADC_CURS_MTQZ4, adc_.CURS_MTQZ4);
		uint8_t state = PWR_State_GET(PWR_State_MTQO_PLASMA);
		if(g_pwr_awc==0){
			AD_CURS_MTQ(AL_CURS_MTQZ4_ACS1,adc_.ADC_CURS_MTQZ4,g_pwr_thres->AT_CURS_MTQ_ACS1[5],state);
			print(PWR_ADC_ACS1, "cursmtqz4 data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQZ4, g_pwr_thres->AT_CURS_MTQ_ACS1[5][0], g_pwr_thres->AT_CURS_MTQ_ACS1[5][1]);
		}
	}
	else if(param[0] == 2) {
		cur_class.CURS_SENSOR_get_MTQZ3();

		adc_.CURS_MTQZ3 = cur_class.curmtqz3;
		adc_.ADC_CURS_MTQZ3 = cur_class.ADC_curmtqz3;
//		print(ACS_ADC, "CURS_MTQZ3,%u,%f\r\n", adc_.ADC_CURS_MTQZ3, adc_.CURS_MTQZ3);
		uint8_t state = PWR_State_GET(PWR_State_MTQO_PLASMA);
		if(g_pwr_awc==0){
			AD_CURS_MTQ(AL_CURS_MTQZ3_ACS1,adc_.ADC_CURS_MTQZ3,g_pwr_thres->AT_CURS_MTQ_ACS1[4],state);
			print(PWR_ADC_ACS1, "cursmtqz3 data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQZ3, g_pwr_thres->AT_CURS_MTQ_ACS1[4][0], g_pwr_thres->AT_CURS_MTQ_ACS1[4][1]);
		}
	}
	else if(param[0] == 3) {
		cur_class.CURS_SENSOR_get_MTQZ2();

		adc_.CURS_MTQZ2 = cur_class.curmtqz2;
		adc_.ADC_CURS_MTQZ2 = cur_class.ADC_curmtqz2;
//		print(ACS_ADC, "CURS_MTQZ2,%u,%f\r\n", adc_.ADC_CURS_MTQZ2, adc_.CURS_MTQZ2);
		uint8_t state = PWR_State_GET(PWR_State_MTQO_PLASMA);
		if(g_pwr_awc==0){
			AD_CURS_MTQ(AL_CURS_MTQZ2_ACS1,adc_.ADC_CURS_MTQZ2,g_pwr_thres->AT_CURS_MTQ_ACS1[3],state);
			print(PWR_ADC_ACS1, "cursmtqz2 data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQZ2, g_pwr_thres->AT_CURS_MTQ_ACS1[3][0], g_pwr_thres->AT_CURS_MTQ_ACS1[3][1]);
		}
	}
	else if(param[0] == 4) {
		cur_class.CURS_SENSOR_get_MTQZ1();

		adc_.CURS_MTQZ1 = cur_class.curmtqz1;
		adc_.ADC_CURS_MTQZ1 = cur_class.ADC_curmtqz1;
//		print(ACS_ADC, "CURS_MTQZ1,%u,%f\r\n", adc_.ADC_CURS_MTQZ1, adc_.CURS_MTQZ1);
		uint8_t state = PWR_State_GET(PWR_State_MTQA);
		if(g_pwr_awc==0){
			AD_CURS_MTQ(AL_CURS_MTQZ1_ACS1,adc_.ADC_CURS_MTQZ1,g_pwr_thres->AT_CURS_MTQ_ACS1[2],state);
			print(PWR_ADC_ACS1, "cursmtqz1 data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQZ1, g_pwr_thres->AT_CURS_MTQ_ACS1[2][0], g_pwr_thres->AT_CURS_MTQ_ACS1[2][1]);
		}
	}
	else if(param[0] == 5) {
		cur_class.CURS_SENSOR_get_MTQY();

		adc_.CURS_MTQY = cur_class.curmtqy;
		adc_.ADC_CURS_MTQY = cur_class.ADC_curmtqy;
//		print(ACS_ADC, "CURS_MTQY,%u,%f\r\n", adc_.ADC_CURS_MTQY, adc_.CURS_MTQY);
		uint8_t state = PWR_State_GET(PWR_State_MTQA);
		if(g_pwr_awc==0){
			AD_CURS_MTQ(AL_CURS_MTQY_ACS1,adc_.ADC_CURS_MTQY,g_pwr_thres->AT_CURS_MTQ_ACS1[1],state);
			print(PWR_ADC_ACS1, "cursmtqy data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQY, g_pwr_thres->AT_CURS_MTQ_ACS1[1][0], g_pwr_thres->AT_CURS_MTQ_ACS1[1][1]);
		}
	}
	else if(param[0] == 6) {
		cur_class.CURS_SENSOR_get_MTQX();

		adc_.CURS_MTQX = cur_class.curmtqx;
		adc_.ADC_CURS_MTQX = cur_class.ADC_curmtqx;
//		print(ACS_ADC, "CURS_MTQX,%u,%f\r\n", adc_.ADC_CURS_MTQX, adc_.CURS_MTQX);
//		printf("curs_mtqx is %f [mA] \r\n", cur_class.curmtqx * 100.0f - 250.0f);
		uint8_t state = PWR_State_GET(PWR_State_MTQA);
		if(g_pwr_awc==0){
			AD_CURS_MTQ(AL_CURS_MTQX_ACS1,adc_.ADC_CURS_MTQX,g_pwr_thres->AT_CURS_MTQ_ACS1[0],state);
			print(PWR_ADC_ACS1, "cursmtqx data, oc, uc: %d, %d, %d\r\n", adc_.ADC_CURS_MTQX, g_pwr_thres->AT_CURS_MTQ_ACS1[0][0], g_pwr_thres->AT_CURS_MTQ_ACS1[0][1]);
		}
	}
	else if(param[0] == 7) {
		cur_class.CURS_SENSOR_get_MTQ2U();

		adc_.CURS_MTQTWO_U = cur_class.curmtq2u;
		adc_.ADC_CURS_MTQ2U = cur_class.ADC_curmtq2u;
		print(ACS_ADC, "CURS_MTQ_2U,%u,%f[V]\r\n", adc_.ADC_CURS_MTQ2U, adc_.CURS_MTQTWO_U);
		if(g_pwr_awc==0){
			AD_CURS(AL_CURS_MTQ_ACS1,adc_.ADC_CURS_MTQ2U,&g_pwr_thres->AT_CURS_MTQ);
			print(PWR_ADC_ACS1, "cursmtq2u data, oc: %d, %d\r\n", adc_.ADC_CURS_MTQ2U, g_pwr_thres->AT_CURS_MTQ);
		}
	}
    else if(param[0] == 8) {
        cur_class.CURS_SENSOR_get_TX();

        adc_.CURS_TX = cur_class.curtx;
        adc_.ADC_CURS_TX = cur_class.ADC_curtx;
        print(PWR_ADC_ACS1, "CURS_TX,%u,%f\r\n", adc_.ADC_CURS_TX, adc_.CURS_TX);
    }

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief VOLS sensor on ACS2
 * @note
 */
CCP_CmdRet Cmd_ACS1_debug_VOLS(const CommonCmdPacket* packet)
{
	const uint8_t *param_head;
	int len=1;
	uint8_t param[1]={0};
	param_head = CCP_get_param_head(packet);
	memcpy (param, param_head, len);

	vol_class.VOLS_SENSOR_init();

	if(param[0] == 0){
		vol_class.VOLS_SENSOR_get_all();
		print(PWR_ADC_ACS1, "VOLS_SENSOR get all %d\r\n");

		adc_.VOLS_MTQA = vol_class.volmtqa;
		adc_.ADC_VOLS_MTQA = vol_class.ADC_volmtqa;
		print(PWR_ADC_ACS1, "int VOLS_MTQA is %d\r\n", adc_.ADC_VOLS_MTQA);

		adc_.VOLS_MTQO = vol_class.volmtqo;
		adc_.ADC_VOLS_MTQO = vol_class.ADC_volmtqo;
		print(PWR_ADC_ACS1, "int VOLS_MTQO is %d\r\n", adc_.ADC_VOLS_MTQO);
	}
	else if(param[0] == 1) {
		vol_class.VOLS_SENSOR_get_MTQA();

		adc_.VOLS_MTQA = vol_class.volmtqa;
		adc_.ADC_VOLS_MTQA = vol_class.ADC_volmtqa;
		print(ACS_ADC, "VOLS_MTQA,%u,%f\r\n", adc_.ADC_VOLS_MTQA, adc_.VOLS_MTQA);
		uint8_t state = PWR_State_GET(PWR_State_MTQA);
		if(g_pwr_awc==0){
			AD_VOLS_FET(AL_VOLS_MTQA_ACS1,adc_.ADC_VOLS_MTQA,g_pwr_thres->AT_VOLS_MTQ_ACS1[0],state);
		}
//		print(PWR_ADC_ACS1, "float VOLS_MTQA is %f\r\n", adc_.VOLS_MTQA);
//		print(PWR_ADC_ACS1, "int VOLS_MTQA is %d\r\n", adc_.ADC_VOLS_MTQA);
//		print(PWR_ADC_ACS1, "VOLS_MTQA UV_THRES %d\r\n", g_pwr_thres->AT_VOLS_MTQ_ACS1[0][0]);
//		print(PWR_ADC_ACS1, "VOLS_MTQA PSION_THRES %d\r\n", g_pwr_thres->AT_VOLS_MTQ_ACS1[0][1]);
	}
	else if(param[0] == 2) {
		vol_class.VOLS_SENSOR_get_MTQO();
		adc_.VOLS_MTQO = vol_class.volmtqo;
		adc_.ADC_VOLS_MTQO = vol_class.ADC_volmtqo;
		print(ACS_ADC, "VOLS_MTQO,%u,%f\r\n", adc_.ADC_VOLS_MTQO, adc_.VOLS_MTQO);
		uint8_t state = PWR_State_GET(PWR_State_MTQO_PLASMA);
		if(g_pwr_awc==0){
			AD_VOLS_FET(AL_VOLS_MTQO_ACS1,adc_.ADC_VOLS_MTQO,g_pwr_thres->AT_VOLS_MTQ_ACS1[1],state);
		}
//		print(PWR_ADC_ACS1, "float VOLS_MTQO is %f\r\n", adc_.VOLS_MTQO);
//		print(PWR_ADC_ACS1, "int VOLS_MTQO is %d\r\n", adc_.ADC_VOLS_MTQO);
//		print(PWR_ADC_ACS1, "VOLS_MTQO UV_THRES %d\r\n", g_pwr_thres->AT_VOLS_MTQ_ACS1[1][0]);
//		print(PWR_ADC_ACS1, "VOLS_MTQO PSION_THRES %d\r\n", g_pwr_thres->AT_VOLS_MTQ_ACS1[1][1]);
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief PLASMA sensor on ACS1
 * @note
 */
CCP_CmdRet Cmd_ACS1_debug_PLASMA(const CommonCmdPacket* packet)
{
	const uint8_t *param_head;
	int len=1;
	uint8_t param[1]={0};
	param_head = CCP_get_param_head(packet);
	memcpy (param, param_head, len);

	plasma_class.PLASMA_SENSOR_init();

	if(param[0] == 0){
		plasma_class.PLASMA_SENSOR_init();

		plasma_class.PLASMA_SENSOR_get_all();

		adc_.VOLS_PLASMA1 = plasma_class.PLASMA1;
		adc_.ADC_VOLS_PLASMA1 = plasma_class.ADC_PLASMA1;

		adc_.VOLS_PLASMA2 = plasma_class.PLASMA2;
		adc_.ADC_VOLS_PLASMA2 = plasma_class.ADC_PLASMA2;

		adc_.VOLS_PLASMA3 = plasma_class.PLASMA3;
		adc_.ADC_VOLS_PLASMA3 = plasma_class.ADC_PLASMA3;

		adc_.VOLS_PLASMA4 = plasma_class.PLASMA4;
		adc_.ADC_VOLS_PLASMA4 = plasma_class.ADC_PLASMA4;

		adc_.VOLS_PLASMA5 = plasma_class.PLASMA5;
		adc_.ADC_VOLS_PLASMA5 = plasma_class.ADC_PLASMA5;

		adc_.VOLS_PLASMA6 = plasma_class.PLASMA6;
		adc_.ADC_VOLS_PLASMA6 = plasma_class.ADC_PLASMA6;

		print(_PLASMA, "VOLS_PLASMAPX, %f\r\n", adc_.VOLS_PLASMA1);
		print(_PLASMA, "VOLS_PLASMAMX, %f\r\n", adc_.VOLS_PLASMA2);
		print(_PLASMA, "VOLS_PLASMAPY, %f\r\n", adc_.VOLS_PLASMA3);
		print(_PLASMA, "VOLS_PLASMAMY, %f\r\n", adc_.VOLS_PLASMA4);
		print(_PLASMA, "VOLS_PLASMAPZ, %f\r\n", adc_.VOLS_PLASMA5);
		print(_PLASMA, "VOLS_PLASMAMZ, %f\r\n\r\n", adc_.VOLS_PLASMA6);

	}
	else if(param[0] == 1) {
		plasma_class.PLASMA_SENSOR_get_PLASMA1();

		adc_.VOLS_PLASMA1 = plasma_class.PLASMA1;
		adc_.ADC_VOLS_PLASMA1 = plasma_class.ADC_PLASMA1;
		print(_PLASMA,"VOLS_PLASMAPX,%f\r\n", adc_.VOLS_PLASMA1);
	}
	else if(param[0] == 2) {
		plasma_class.PLASMA_SENSOR_get_PLASMA2();

		adc_.VOLS_PLASMA2 = plasma_class.PLASMA2;
		adc_.ADC_VOLS_PLASMA2 = plasma_class.ADC_PLASMA2;
		print(_PLASMA,"VOLS_PLASMAMX,%f\r\n", adc_.VOLS_PLASMA2);
	}
	else if(param[0] == 3) {
		plasma_class.PLASMA_SENSOR_get_PLASMA3();

		adc_.VOLS_PLASMA3 = plasma_class.PLASMA3;
		adc_.ADC_VOLS_PLASMA3 = plasma_class.ADC_PLASMA3;
		print(_PLASMA,"VOLS_PLASMAPY,%f\r\n", adc_.VOLS_PLASMA3);
	}
	else if(param[0] == 4) {
		plasma_class.PLASMA_SENSOR_get_PLASMA4();

		adc_.VOLS_PLASMA4 = plasma_class.PLASMA4;
		adc_.ADC_VOLS_PLASMA4 = plasma_class.ADC_PLASMA4;
		print(_PLASMA,"VOLS_PLASMAMY,%f\r\n", adc_.VOLS_PLASMA4);
	}
	else if(param[0] == 5) {
		plasma_class.PLASMA_SENSOR_get_PLASMA5();

		adc_.VOLS_PLASMA5 = plasma_class.PLASMA5;
		adc_.ADC_VOLS_PLASMA5 = plasma_class.ADC_PLASMA5;
		print(_PLASMA,"VOLS_PLASMAPZ,%f\r\n", adc_.VOLS_PLASMA5);
	}
	else if(param[0] == 6) {
		plasma_class.PLASMA_SENSOR_get_PLASMA6();

		adc_.VOLS_PLASMA6 = plasma_class.PLASMA6;
		adc_.ADC_VOLS_PLASMA6 = plasma_class.ADC_PLASMA6;
		print(_PLASMA,"VOLS_PLASMAMZ,%f\r\n", adc_.VOLS_PLASMA6);
	}

		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief Pt temperature sensor.
 * @note
 */
CCP_CmdRet Cmd_ACS1_debug_PT_TEMS(const CommonCmdPacket* packet)
{
	const uint8_t *param_head;
	int len=1;
	uint8_t param[1]={0};
	param_head = CCP_get_param_head(packet);
	memcpy (param, param_head, len);

	pt_class.PLATINUM_SENSOR_init();

//	printf("Pt__THRES = %f\r\n",pt_class.PT_TEMP_THRESHOLD);

	if(param[0] == 0){
		pt_class.PLATINUM_SENSOR_get_all();

		adc_.ADC_MOBC_TEMP = pt_class.ADC_PtMOBC;
		adc_.MOBC_TEMP = pt_class.PtMOBC;
			// Anomaly detection
			if ( adc_.MOBC_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MOBC_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_ADC_TEMP = pt_class.ADC_PtADC;
		adc_.ADC_TEMP = pt_class.PtADC;
		// Anomaly detection
			if ( adc_.ADC_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.ADC_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_MAGS_TEMP = pt_class.ADC_PtMAGS;
		adc_.MAGS_TEMP = pt_class.PtMAGS;
		// Anomaly detection
			if ( adc_.MAGS_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MAGS_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_GPS_TEMP = pt_class.ADC_PtGPS;
		adc_.GPS_TEMP = pt_class.PtGPS;
		// Anomaly detection
			if ( adc_.GPS_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.GPS_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_NINEAX_TEMP = pt_class.ADC_Pt9AX;
		adc_.NINEAX_TEMP = pt_class.Pt9AX;
		// Anomaly detection
			if ( adc_.NINEAX_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.NINEAX_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_LENSPX_TEMP = pt_class.ADC_PtLENSPX;
		adc_.LENSPX_TEMP = pt_class.PtLENSPX;
		// Anomaly detection
			if ( adc_.LENSPX_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.LENSPX_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_LENSPY_TEMP = pt_class.ADC_PtLENSPY;
		adc_.LENSPY_TEMP = pt_class.PtLENSPY;
		// Anomaly detection
			if ( adc_.LENSPY_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.LENSPY_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_IMAGE_TEMP = pt_class.ADC_PtIMAGE;
		adc_.IMAGE_TEMP = pt_class.PtIMAGE;
		// Anomaly detection
			if ( adc_.IMAGE_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.IMAGE_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_LENSMX_TEMP = pt_class.ADC_PtLENSMX;
		adc_.LENSMX_TEMP = pt_class.PtLENSMX;
		// Anomaly detection
			if ( adc_.LENSMX_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.LENSMX_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_GYROX_TEMP = pt_class.ADC_PtGYROX;
		adc_.GYROX_TEMP = pt_class.PtGYROX;
		// Anomaly detection
			if ( adc_.GYROX_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.GYROX_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_GYROY_TEMP = pt_class.ADC_PtGYROY;
		adc_.GYROY_TEMP = pt_class.PtGYROY;
		// Anomaly detection
			if ( adc_.GYROY_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.GYROY_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_GYROZ_TEMP = pt_class.ADC_PtGYROZ;
		adc_.GYROZ_TEMP = pt_class.PtGYROZ;
		// Anomaly detection
			if ( adc_.GYROZ_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.GYROZ_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_MTQZ1_TEMP = pt_class.ADC_PtMTQZ1;
		adc_.MTQZ1_TEMP = pt_class.PtMTQZ1;
		// Anomaly detection
			if ( adc_.MTQZ1_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MTQZ1_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_MTQZ2_TEMP = pt_class.ADC_PtMTQZ2;
		adc_.MTQZ2_TEMP = pt_class.PtMTQZ2;
		// Anomaly detection
			if ( adc_.MTQZ2_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MTQZ2_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_MTQZ3_TEMP = pt_class.ADC_PtMTQZ3;
		adc_.MTQZ3_TEMP = pt_class.PtMTQZ3;
		// Anomaly detection
			if ( adc_.MTQZ3_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MTQZ3_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}

		adc_.ADC_MTQZ4_TEMP = pt_class.ADC_PtMTQZ4;
		adc_.MTQZ4_TEMP = pt_class.PtMTQZ4;
		// Anomaly detection
			if ( adc_.MTQZ4_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MTQZ4_TEMP < - pt_class.PT_TEMP_THRESHOLD){
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
	}
	else if(param[0] == 1){
		pt_class.PLATINUM_SENSOR_get_MOBC();
		adc_.ADC_MOBC_TEMP = pt_class.ADC_PtMOBC;
		adc_.MOBC_TEMP = pt_class.PtMOBC;
		print(Pt_TEMP, "Pt_MOBC, %u, %f\r\n", adc_.ADC_MOBC_TEMP, adc_.MOBC_TEMP);
		// Anomaly detection
		if ( adc_.MOBC_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MOBC_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//			printf("temp = %f , THRES = %f\r\n",adc_.MOBC_TEMP,pt_class.PT_TEMP_THRESHOLD);
			print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
			print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//			AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
		}
//		printf("MOBC %f\r\n",adc_.MOBC_TEMP);
	}
	else if(param[0] == 2){
		pt_class.PLATINUM_SENSOR_get_ADC();
		adc_.ADC_ADC_TEMP = pt_class.ADC_PtADC;
		adc_.ADC_TEMP = pt_class.PtADC;
		print(Pt_TEMP, "Pt_ADC, %u, %f\r\n", adc_.ADC_ADC_TEMP, adc_.ADC_TEMP);
		// Anomaly detection
			if ( adc_.ADC_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.ADC_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.ADC_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("ADC %f\r\n",adc_.ADC_TEMP);
	}
	else if(param[0] == 3){
		pt_class.PLATINUM_SENSOR_get_MAGS();
		adc_.ADC_MAGS_TEMP = pt_class.ADC_PtMAGS;
		adc_.MAGS_TEMP = pt_class.PtMAGS;
		print(Pt_TEMP, "Pt_MAGS, %u, %f\r\n", adc_.ADC_MAGS_TEMP, adc_.MAGS_TEMP);
		// Anomaly detection
			if ( adc_.MAGS_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MAGS_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.MAGS_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("MAGS %f\r\n",adc_.MAGS_TEMP);
	}
	else if(param[0] == 4){
		pt_class.PLATINUM_SENSOR_get_GPS();
		adc_.ADC_GPS_TEMP = pt_class.ADC_PtGPS;
		adc_.GPS_TEMP = pt_class.PtGPS;
		print(Pt_TEMP, "Pt_GPS, %u, %f\r\n", adc_.ADC_GPS_TEMP, adc_.GPS_TEMP);
		// Anomaly detection
			if ( adc_.GPS_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.GPS_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.GPS_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("GPS %f\r\n",adc_.GPS_TEMP);
	}
	else if(param[0] == 5){
		pt_class.PLATINUM_SENSOR_get_9AX();
		adc_.ADC_NINEAX_TEMP = pt_class.ADC_Pt9AX;
		adc_.NINEAX_TEMP = pt_class.Pt9AX;
		print(Pt_TEMP, "Pt_9AX, %u, %f\r\n", adc_.ADC_NINEAX_TEMP, adc_.NINEAX_TEMP);
		// Anomaly detection
			if ( adc_.NINEAX_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.NINEAX_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.NINEAX_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("9AX %f\r\n",adc_.NINEAX_TEMP);
	}
	else if(param[0] == 6){
		pt_class.PLATINUM_SENSOR_get_LENSPX();
		adc_.ADC_LENSPX_TEMP = pt_class.ADC_PtLENSPX;
		adc_.LENSPX_TEMP = pt_class.PtLENSPX;
		print(Pt_TEMP, "Pt_LENSPX, %u, %f\r\n", adc_.ADC_LENSPX_TEMP, adc_.LENSPX_TEMP);
		// Anomaly detection
			if ( adc_.LENSPX_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.LENSPX_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.LENSPX_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("LENSPX %f\r\n",adc_.LENSPX_TEMP);
	}
	else if(param[0] == 7){
		pt_class.PLATINUM_SENSOR_get_LENSPY();
		adc_.ADC_LENSPY_TEMP = pt_class.ADC_PtLENSPY;
		adc_.LENSPY_TEMP = pt_class.PtLENSPY;
		print(Pt_TEMP, "Pt_LENSPY, %u, %f\r\n", adc_.ADC_LENSPX_TEMP, adc_.LENSPX_TEMP);
		// Anomaly detection
			if ( adc_.LENSPY_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.LENSPY_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.LENSPY_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("LENSPY %f\r\n",adc_.LENSPY_TEMP);
	}
	else if(param[0] == 8){
		pt_class.PLATINUM_SENSOR_get_IMAGE();
		adc_.ADC_IMAGE_TEMP = pt_class.ADC_PtIMAGE;
		adc_.IMAGE_TEMP = pt_class.PtIMAGE;
		print(Pt_TEMP, "Pt_IMGSEN, %u, %f\r\n", adc_.ADC_IMAGE_TEMP, adc_.IMAGE_TEMP);
		// Anomaly detection
			if ( adc_.IMAGE_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.IMAGE_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.IMAGE_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("IMAGE %f\r\n",adc_.IMAGE_TEMP);
	}
	else if(param[0] == 9){
		pt_class.PLATINUM_SENSOR_get_LENSMX();
		adc_.ADC_LENSMX_TEMP = pt_class.ADC_PtLENSMX;
		adc_.LENSMX_TEMP = pt_class.PtLENSMX;
		print(Pt_TEMP, "Pt_LENSMX, %u, %f\r\n", adc_.ADC_LENSMX_TEMP, adc_.LENSMX_TEMP);
		// Anomaly detection
			if ( adc_.LENSMX_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.LENSMX_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.LENSMX_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("LENSMX %f\r\n", adc_.LENSMX_TEMP);
	}
	else if(param[0] == 10){
		pt_class.PLATINUM_SENSOR_get_GYROX();
		adc_.ADC_GYROX_TEMP = pt_class.ADC_PtGYROX;
		adc_.GYROX_TEMP = pt_class.PtGYROX;
		print(Pt_TEMP, "Pt_GYROX, %u, %f\r\n", adc_.ADC_GYROX_TEMP, adc_.GYROX_TEMP);
		// Anomaly detection
			if ( adc_.GYROX_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.GYROX_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.GYROX_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("GYROX %f\r\n",adc_.GYROX_TEMP);
	}
	else if(param[0] == 11){
		pt_class.PLATINUM_SENSOR_get_GYROY();
		adc_.ADC_GYROY_TEMP = pt_class.ADC_PtGYROY;
		adc_.GYROY_TEMP = pt_class.PtGYROY;
		print(Pt_TEMP, "Pt_GYROY, %u, %f\r\n", adc_.ADC_GYROY_TEMP, adc_.GYROY_TEMP);
		// Anomaly detection
			if ( adc_.GYROY_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.GYROY_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.GYROY_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("GYROY %f\r\n",adc_.GYROY_TEMP);
	}
	else if(param[0] == 12){
		pt_class.PLATINUM_SENSOR_get_GYROZ();
		adc_.ADC_GYROZ_TEMP = pt_class.ADC_PtGYROZ;
		adc_.GYROZ_TEMP = pt_class.PtGYROZ;
		print(Pt_TEMP, "Pt_GYROZ, %u, %f\r\n", adc_.ADC_GYROZ_TEMP, adc_.GYROZ_TEMP);
		// Anomaly detection
			if ( adc_.GYROZ_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.GYROZ_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.GYROZ_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("GYROZ %f\r\n",adc_.GYROZ_TEMP);
	}
	else if(param[0] == 13){
		pt_class.PLATINUM_SENSOR_get_MTQZ1();
		adc_.ADC_MTQZ1_TEMP = pt_class.ADC_PtMTQZ1;
		adc_.MTQZ1_TEMP = pt_class.PtMTQZ1;
		print(Pt_TEMP, "Pt_MTQZ1, %u, %f\r\n", adc_.ADC_MTQZ1_TEMP, adc_.MTQZ1_TEMP);
		// Anomaly detection
			if ( adc_.MTQZ1_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MTQZ1_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.MTQZ1_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("MTQZ1 %f\r\n",adc_.MTQZ1_TEMP);
	}
	else if(param[0] == 14){
		pt_class.PLATINUM_SENSOR_get_MTQZ2();
		adc_.ADC_MTQZ2_TEMP = pt_class.ADC_PtMTQZ2;
		adc_.MTQZ2_TEMP = pt_class.PtMTQZ2;
		print(Pt_TEMP, "Pt_MTQZ2, %u, %f\r\n", adc_.ADC_MTQZ2_TEMP, adc_.MTQZ2_TEMP);
		// Anomaly detection
			if ( adc_.MTQZ2_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MTQZ2_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.MTQZ2_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("MTQZ1 %f\r\n",adc_.MTQZ2_TEMP);
	}
	else if(param[0] == 15){
		pt_class.PLATINUM_SENSOR_get_MTQZ3();
		adc_.ADC_MTQZ3_TEMP = pt_class.ADC_PtMTQZ3;
		adc_.MTQZ3_TEMP = pt_class.PtMTQZ3;
		print(Pt_TEMP, "Pt_MTQZ3, %u, %f\r\n", adc_.ADC_MTQZ3_TEMP, adc_.MTQZ3_TEMP);
		// Anomaly detection
			if ( adc_.MTQZ3_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MTQZ3_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.MTQZ3_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("MTQZ1 %f\r\n",adc_.MTQZ3_TEMP);
	}
	else if(param[0] == 16){
		pt_class.PLATINUM_SENSOR_get_MTQZ4();
		adc_.ADC_MTQZ4_TEMP = pt_class.ADC_PtMTQZ4;
		adc_.MTQZ4_TEMP = pt_class.PtMTQZ4;
		print(Pt_TEMP, "Pt_MTQZ4, %u, %f\r\n", adc_.ADC_MTQZ4_TEMP, adc_.MTQZ4_TEMP);
		// Anomaly detection
			if ( adc_.MTQZ4_TEMP > pt_class.PT_TEMP_THRESHOLD || adc_.MTQZ1_TEMP < - pt_class.PT_TEMP_THRESHOLD){
//				printf("temp = %f , THRES = %f\r\n",adc_.MTQZ4_TEMP,pt_class.PT_TEMP_THRESHOLD);
				print(ANOMALY,"PT_TEMS_OT_add_anomaly \r\n");
				print(ANOMALY," No.%d is Over Threshold. \r\n",param[0]);
//				AL_add_anomaly(AL_GROUP_PT_TEMS,AL_OT_PT_TEMS);
			}
//		printf("MTQZ1 %f\r\n",adc_.MTQZ4_TEMP);
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#endif/*for 2U*/
