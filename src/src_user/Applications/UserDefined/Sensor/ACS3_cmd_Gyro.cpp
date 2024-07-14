#ifdef ONE_U
#include "ACS3_cmd_Gyro.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <src_user/Drivers/Sensor/GYRO.h>
#include <src_user/Drivers/Sensor/9AX.h>
#include "../Global_Sensor_Value.h"
#include <src_user/Applications/DriverInstances/block_command_handler.h>    //for current mode id
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/DriverInstances/mode_auto_transition.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>

static ACS3_DEBUG_GYRO_STRUCT acs3_debug_gyro; //デバッグ用構造体
const ACS3_DEBUG_GYRO_STRUCT* p_acs3_debug_gyro; //デバッグ用構造体ポインタ格納用
static GYRO_CLASS gyro_class;
static GLOBAL_AOCS_PARAMETER_STRUCT AOCS_Param;

static void ACS3_debug_gyro_init(void);
static void ACS3_debug_gyro_exe(void);
static void ACS3_GYRO_initialize_param(void);
static void ACS3_GYRO_load_param(void);


//MOBC ADC
enum{ADC_BUFFER_LENGTH = 1024};
uint32_t ADCBuffer[ADC_BUFFER_LENGTH];

//PARK 2020.11.16
//extern uint16_t adc1_buf[60]; //from main

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */


AppInfo ACS3_debug_gyro_update(void)
{
    return AI_create_app_info("DBG_gyro", ACS3_debug_gyro_init, ACS3_debug_gyro_exe);
}

AppInfo ACS3_GYRO_param_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("sample_param_init", ACS3_GYRO_load_param, ACS3_GYRO_initialize_param);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void ACS3_debug_gyro_init(void)
{
    printf("ACS3 Gyro Debug App initialize.\n");
    g_sensor.p_gyro = &acs3_debug_gyro;

	//MEMS_GYROレンジの初期値を設定する ±75deg/s
	gyro_class.GYRO_SENSOR_CHANGE_RANGE(gyro_class.state);

}



static void ACS3_debug_gyro_exe(void)
{
    printf("sample driver execution.\r\n");

}
/**
 * @brief アプリケーション実行関数
 *
 */
static void ACS3_GYRO_load_param(void)
{
	printf("ACS3_GYRO_load_param\r\n");
//	printf("***************************\r\n");
    read_param(&gyro_class.state, &GYRO_CLASS::state_, (size_t)(sizeof(gyro_class.state)), (uint32_t)0x00004A00);  //uint8_t
    read_param(&gyro_class.k_freq, &GYRO_CLASS::k_freq_, (size_t)(sizeof(gyro_class.k_freq)), (uint32_t)0x00004A03);
    read_param(&gyro_class.k_gyro_scale, &GYRO_CLASS::k_gyro_scale_, (size_t)(sizeof(gyro_class.k_gyro_scale)), (uint32_t)0x00004A0F);
    read_param(&gyro_class.k_gyro_vols_bias, &GYRO_CLASS::k_gyro_vols_bias_, (size_t)(sizeof(gyro_class.k_gyro_vols_bias)), (uint32_t)0x00004A1B);
    read_param(&gyro_class.k_InputVol, &GYRO_CLASS::k_InputVol_, (size_t)(sizeof(gyro_class.k_InputVol)), (uint32_t)0x00004A27);
    read_param(&gyro_class.k_K, &GYRO_CLASS::k_K_, (size_t)(sizeof(gyro_class.k_K)), (uint32_t)0x00004A33);
    read_param(&gyro_class.ACS3_GYRO_DCM, &GYRO_CLASS::ACS3_GYRO_DCM_, (size_t)(sizeof(gyro_class.ACS3_GYRO_DCM)), (uint32_t)0x00004A63);
    read_param(&gyro_class.GYRO_HIGHSPIN_THRESHOLD,&GYRO_CLASS::GYRO_HIGHSPIN_THRESHOLD_,(size_t)(sizeof(gyro_class.GYRO_HIGHSPIN_THRESHOLD)),(uint32_t)0x00004ACF);
    read_param(&gyro_class.GYRO_THRESHOLD,&GYRO_CLASS::GYRO_THRESHOLD_,(size_t)(sizeof(gyro_class.GYRO_THRESHOLD)),(uint32_t)0x00004ADB);
    read_param(&gyro_class.mems_gyro_sf_t0,&GYRO_CLASS::mems_gyro_sf_t0_,(size_t)sizeof(gyro_class.mems_gyro_sf_t0),(uint32_t)0x00004AEC);
    read_param(&gyro_class.mems_gyro_sf_t1,&GYRO_CLASS::mems_gyro_sf_t1_,(size_t)sizeof(gyro_class.mems_gyro_sf_t1),(uint32_t)0x00004AF8);
    read_param(&gyro_class.mems_gyro_sf_t2,&GYRO_CLASS::mems_gyro_sf_t2_,(size_t)sizeof(gyro_class.mems_gyro_sf_t2),(uint32_t)0x00004B04);
    read_param(&gyro_class.mems_gyro_os_t0,&GYRO_CLASS::mems_gyro_os_t0_,(size_t)sizeof(gyro_class.mems_gyro_os_t0),(uint32_t)0x00004B10);
    read_param(&gyro_class.mems_gyro_os_t1,&GYRO_CLASS::mems_gyro_os_t1_,(size_t)sizeof(gyro_class.mems_gyro_os_t1),(uint32_t)0x00004B1C);
    read_param(&gyro_class.mems_gyro_os_t2,&GYRO_CLASS::mems_gyro_os_t2_,(size_t)sizeof(gyro_class.mems_gyro_os_t2),(uint32_t)0x00004B28);
}

static void ACS3_GYRO_initialize_param(void)
{
	printf("ACS3_GYRO_initialize_param.\n");
    write_param(&gyro_class.state, &GYRO_CLASS::state_, (size_t)(sizeof(gyro_class.state)), (uint32_t)0x00004A00);  //uint8_t
    write_param(&gyro_class.k_freq, &GYRO_CLASS::k_freq_, (size_t)(sizeof(gyro_class.k_freq)), (uint32_t)0x00004A03);
    write_param(&gyro_class.k_gyro_scale, &GYRO_CLASS::k_gyro_scale_, (size_t)(sizeof(gyro_class.k_gyro_scale)), (uint32_t)0x00004A0F);
    write_param(&gyro_class.k_gyro_vols_bias, &GYRO_CLASS::k_gyro_vols_bias_, (size_t)(sizeof(gyro_class.k_gyro_vols_bias)), (uint32_t)0x00004A1B);
    write_param(&gyro_class.k_InputVol, &GYRO_CLASS::k_InputVol_, (size_t)(sizeof(gyro_class.k_InputVol)), (uint32_t)0x00004A27);
    write_param(&gyro_class.k_K, &GYRO_CLASS::k_K_, (size_t)(sizeof(gyro_class.k_K)), (uint32_t)0x00004A33);
    write_param(&gyro_class.ACS3_GYRO_DCM, &GYRO_CLASS::ACS3_GYRO_DCM_, (size_t)(sizeof(gyro_class.ACS3_GYRO_DCM)), (uint32_t)0x00004A63);
    write_param(&gyro_class.GYRO_HIGHSPIN_THRESHOLD,&GYRO_CLASS::GYRO_HIGHSPIN_THRESHOLD_,(size_t)(sizeof(gyro_class.GYRO_HIGHSPIN_THRESHOLD)),(uint32_t)0x00004ACF);
    write_param(&gyro_class.GYRO_THRESHOLD,&GYRO_CLASS::GYRO_THRESHOLD_,(size_t)(sizeof(gyro_class.GYRO_THRESHOLD)),(uint32_t)0x00004ADB);
    write_param(&gyro_class.mems_gyro_sf_t0,&GYRO_CLASS::mems_gyro_sf_t0_,(size_t)sizeof(gyro_class.mems_gyro_sf_t0),(uint32_t)0x00004AEC);
    write_param(&gyro_class.mems_gyro_sf_t1,&GYRO_CLASS::mems_gyro_sf_t1_,(size_t)sizeof(gyro_class.mems_gyro_sf_t1),(uint32_t)0x00004AF8);
    write_param(&gyro_class.mems_gyro_sf_t2,&GYRO_CLASS::mems_gyro_sf_t2_,(size_t)sizeof(gyro_class.mems_gyro_sf_t2),(uint32_t)0x00004B04);
    write_param(&gyro_class.mems_gyro_os_t0,&GYRO_CLASS::mems_gyro_os_t0_,(size_t)sizeof(gyro_class.mems_gyro_os_t0),(uint32_t)0x00004B10);
    write_param(&gyro_class.mems_gyro_os_t1,&GYRO_CLASS::mems_gyro_os_t1_,(size_t)sizeof(gyro_class.mems_gyro_os_t1),(uint32_t)0x00004B1C);
    write_param(&gyro_class.mems_gyro_os_t2,&GYRO_CLASS::mems_gyro_os_t2_,(size_t)sizeof(gyro_class.mems_gyro_os_t2),(uint32_t)0x00004B28);
}


/**
 * @brief MOBC電流取得コマンド実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
CCP_CmdRet Cmd_ACS3_debug_gyro_read(const CommonCmdPacket* packet)
{

    int ret = 0;
    gyro_class.GYRO_SENSOR_init(gyro_class.state);
//    printf("ACS3 GYRO\r\n");

    //GYROセンサーの値（出力値は電圧）を読む

    acs3_debug_gyro.ADC_OMEGAX = adc1_buf[ORDER_ANA_GYROX] * 16; // convert 12bit to 16bit
    acs3_debug_gyro.ADC_OMEGAY = adc1_buf[ORDER_ANA_GYROY] * 16; // convert 12bit to 16bit
    acs3_debug_gyro.ADC_OMEGAZ = adc1_buf[ORDER_ANA_GYROZ] * 16; // convert 12bit to 16bit

    acs3_debug_gyro.omega[0] = gyro_class.Convert_Voltage2GYRO(0, 3.3f*(float)adc1_buf[ORDER_ANA_GYROX]/4096.0f);
    acs3_debug_gyro.omega[1] = gyro_class.Convert_Voltage2GYRO(1, 3.3f*(float)adc1_buf[ORDER_ANA_GYROY]/4096.0f);
    acs3_debug_gyro.omega[2] = gyro_class.Convert_Voltage2GYRO(2, 3.3f*(float)adc1_buf[ORDER_ANA_GYROZ]/4096.0f);

    print(GYRO, "OMEGAX, %u, %f\r\n", acs3_debug_gyro.ADC_OMEGAX, acs3_debug_gyro.omega[0]);
    print(GYRO, "OMEGAY, %u, %f\r\n", acs3_debug_gyro.ADC_OMEGAY, acs3_debug_gyro.omega[1]);
    print(GYRO, "OMEGAZ, %u, %f\r\n\r\n", acs3_debug_gyro.ADC_OMEGAZ, acs3_debug_gyro.omega[2]);

    gyro_class.GYRO_Vector_DCM_Sensor2Body_Calc(acs3_debug_gyro.omega, gyro_class.ACS3_GYRO_DCM, acs3_debug_gyro.OMEGA);

//	  printf("1UOMEGAXYZ,%u,%u,%u\r\n", acs3_debug_gyro.ADC_OMEGAX, acs3_debug_gyro.ADC_OMEGAY, acs3_debug_gyro.ADC_OMEGAZ);
//	  print(GYRO, "1UOMEGAXYZ,%f,%f,%f\r\n", acs3_debug_gyro.OMEGA[0], acs3_debug_gyro.OMEGA[1], acs3_debug_gyro.OMEGA[2]);

	  //Anomaly detection
for(int i = 0; i < 3; i++){
	if(user_mode->current_mode_id == MD_AOCSFLAG_HIGH_RATE_SPIN){
			if(acs3_debug_gyro.OMEGA[i] > gyro_class.GYRO_HIGHSPIN_THRESHOLD[i] || acs3_debug_gyro.OMEGA[i] < -gyro_class.GYRO_HIGHSPIN_THRESHOLD[i]){
//				printf("OMEGA_1U[%d]=%f THRESH_1U[%d]=%f\r\n",i,acs3_debug_gyro.OMEGA[i],i,gyro_class.GYRO_HIGHSPIN_THRESHOLD[i]);
				print(ANOMALY,"Gyro_OT_add_anomaly \r\n");
//				AL_add_anomaly(AL_GROUP_GYRO,AL_OT_GYRO);
			}
	  	}
	else{
			if(acs3_debug_gyro.OMEGA[i] > gyro_class.GYRO_THRESHOLD[i] || acs3_debug_gyro.OMEGA[i] < -gyro_class.GYRO_THRESHOLD [i]){
//				printf("OMEGA_1U[%d]=%f THRESH_1U[%d]=%f\r\n",i,acs3_debug_gyro.OMEGA[i],i,gyro_class.GYRO_THRESHOLD[i]);
				print(ANOMALY,"Gyro_OT_add_anomaly \r\n");
//				AL_add_anomaly(AL_GROUP_GYRO,AL_OT_GYRO);
			}
	  	}
//	printf("OMEGA_1U[%d]=%f THRESH_1U[%d]=%f\r\n",i,acs3_debug_gyro.OMEGA[i],i,gyro_class.GYRO_THRESHOLD[i]);
//	printf("adc_buf[%d] = %02x \r\n", i,adc1_buf[i + 10]);
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


	if (ADS_Mode(MEMS_GYRO)){
		GLOBAL_AOCS_set_GYROX(acs3_debug_gyro.OMEGA[0]);
		GLOBAL_AOCS_set_GYROY(acs3_debug_gyro.OMEGA[1]);
		GLOBAL_AOCS_set_GYROZ(acs3_debug_gyro.OMEGA[2]);
		GLOBAL_AOCS_set_GYRO_TIME(gyro_class.gyrosensor_time_before, gyro_class.gyrosensor_time_now);

//			printf("ACS1 GYROX used\r\n");
#ifdef HILS
			float omega[3] = {AOCS_hils->gyro[0], AOCS_hils->gyro[1], AOCS_hils->gyro[2]};
			GLOBAL_AOCS_set_GYROX(omega[0]);
			GLOBAL_AOCS_set_GYROY(omega[1]);
			GLOBAL_AOCS_set_GYROZ(omega[2]);
			gyro_class.GYRO_SENSOR_get_time_HILS();
			GLOBAL_AOCS_set_GYRO_TIME(gyro_class.gyrosensor_time_before_hils, gyro_class.gyrosensor_time_now_hils);
#endif
	}


    //assign in struct
//    pwr_debug_adc_mobc.I_MOBC = I_MOBC;

	step_t write_a = TMGR_get_master_step(); // 時間計測終了


    if (ret != 0)
    {
        printf("Read GYRO  Failed! %d \r\n", ret); //printfの内容変更する
        return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
    }

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief change GYRO sensor range
 * @note
 */
CCP_CmdRet Cmd_ACS3_Change_GYRO_Range(const CommonCmdPacket* packet)
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

		acs3_debug_gyro.state[i] = gyro_class.state[i];

	}
	gyro_class.GYRO_SENSOR_CHANGE_RANGE(gyro_class.state);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#endif/*for 1U*/
