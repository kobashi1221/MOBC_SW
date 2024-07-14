/*
 * ACS13_debug_SUNS.cpp
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

#include <src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.h>

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>
//#include <vector.h> //src_core/Library/

#include <src_user/Drivers/SENSOR/SUNS.h>
#include "../Global_Sensor_Value.h"
#include <src_user/Applications/UserDefined/AOCS/Global_AOCS_Parameter.h>
#include "../CDH/Communication_Fault_Detection.h"
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Drivers/Power/PWR_Operation.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Library/Delay.h>
#include <src_user/Applications/DriverInstances/block_command_handler.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include <src_user/Applications/DriverInstances/mode_auto_transition.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>

const float k_error_ = 0.1f + 0.5f; //[deg] 0.1f：precision, 0.5f：3siguma tamaoki add
const uint8_t counter_feton_2U_ = 1;
const uint8_t counter_getdata_2U_ = 2;
const uint8_t counter_fetoff_2U_ = 4;
const uint8_t counter_feton_1U_ = 2;
const uint8_t counter_getdata_1U_ = 3;
const uint8_t counter_fetoff_1U_ = 4;
const float ANGULAR_THRESHOLD_ = 55.0f;
const float ACS1_SUNSPX_DCM_[3][3] = {0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f};
const float ACS1_SUNSMX_DCM_[3][3] = {0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f};
const float ACS1_SUNSPY_DCM_[3][3] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f};
const float ACS1_SUNSMY_DCM_[3][3] = {-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f};
const float ACS1_SUNSPZ_DCM_[3][3] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
const float ACS3_SUNSMX_DCM_[3][3] = {0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f};
const float ACS3_SUNSPY_DCM_[3][3] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f};
const float ECLIPSE_THRESHOLD_2U_ = 3000.0f; // [mV]
const float ECLIPSE_THRESHOLD_1U_ = 3000.0f; // [mV]
const uint8_t UNFLTERED_FILTERED_FLAG_ = 0; // 0->UNFLTERED&FILTERED 1->UNFLTERED 2->FILTERED
const uint8_t AVE_UNI_FLAG_ = 0; // 0->select method 1->avearge method
const uint8_t UNI_METHOD_FLAG_ = 1; // 0->irradiance 1->angle position
const uint8_t SUNS_USE_FLAG_[5] = {1, 1, 1, 1, 1};    // 1U:MX, PY 0:no use 1:use
const float voting_sunvec_threshold_ = 0.9f; // 基準の太陽ベクトルとの誤差が20deg以内のときアノマリーをすり抜ける
const float anomaly_sunvec_threshold_ = 0.0f; // 1step前と90deg違う場合アノマリー検知


// 6個のサンセンサを格納する構造体
static SUN_SENSOR_STRUCT suns[5];
static SunSensorDriver suns_driver[5];
static SunSensorInfo suns_info;

const SunSensorDriver* suns_driver_ = suns_driver;
const SunSensorInfo* const suns_info_ = &suns_info;

// 各サンセンサの名称
#ifdef TWO_U
//const char tag[5][4] = { "SPX", "SMX", "SPY", "SMY", "SPZ" }; // MX for PSD
const char tag[4][4] = { "SPX", "SPY", "SMY", "SPZ" };
#endif //for 2U
#ifdef ONE_U
//const char tag[2][4] = { "SMX", "SPY" }; // MX for PSD
const char tag[1][4] = { "SPY" };
#endif //for 1U

// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS BEGIN *** //

// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS END*** //
// ファイルを抜けても値を保持しておく変数
static uint8_t counter = 0;
static float preSUNVec[3] = {0.0f};
static uint8_t hispin_counter = 0;
#ifdef TWO_U

	// CSピンのリスト ポート // MX for PSD
//	GPIO_TypeDef *CS_PORTS[] = { CS_SPI3V3_MOBCtoSUNSPX_GPIO_Port, CS_SPI3V3_MOBCtoSUNSMX_GPIO_Port,
//			                     CS_SPI3V3_MOBCtoSUNSPY_GPIO_Port, CS_SPI3V3_MOBCtoSUNSMY_GPIO_Port,
//								 CS_SPI3V3_MOBCtoSUNSPZ_GPIO_Port }; //CS_SPI3V3_MOBCtoSUNSMZ_GPIO_Port 基板修正が遅かったので残っているがPWR2でプルダウンしてある
//
//	// CSピンのリスト ピン // MX for PSD
//	uint16_t CS_PINS[] = { CS_SPI3V3_MOBCtoSUNSPX_Pin, CS_SPI3V3_MOBCtoSUNSMX_Pin,
//			               CS_SPI3V3_MOBCtoSUNSPY_Pin, CS_SPI3V3_MOBCtoSUNSMY_Pin,
//						   CS_SPI3V3_MOBCtoSUNSPZ_Pin }; //CS_SPI3V3_MOBCtoSUNSMZ_GPIO_Port 基板修正が遅かったので残っているがPWR2でプルダウンしてある

  GPIO_TypeDef *CS_PORTS[] = { CS_SPI3V3_MOBCtoSUNSPX_GPIO_Port,
                               CS_SPI3V3_MOBCtoSUNSPY_GPIO_Port, CS_SPI3V3_MOBCtoSUNSMY_GPIO_Port,
                               CS_SPI3V3_MOBCtoSUNSPZ_GPIO_Port }; //CS_SPI3V3_MOBCtoSUNSMZ_GPIO_Port 基板修正が遅かったので残っているがPWR2でプルダウンしてある

  // CSピンのリスト ピン
  uint16_t CS_PINS[] = { CS_SPI3V3_MOBCtoSUNSPX_Pin,
                         CS_SPI3V3_MOBCtoSUNSPY_Pin, CS_SPI3V3_MOBCtoSUNSMY_Pin,
                         CS_SPI3V3_MOBCtoSUNSPZ_Pin }; //CS_SPI3V3_MOBCtoSUNSMZ_GPIO_Port 基板修正が遅かったので残っているがPWR2でプルダウンしてある


#endif /*for 2U*/

#ifdef ONE_U

	//精サンセンサ MX PY MZ
	//チップサンセンサ PX MY

//	// CSピンのリスト ポート
//	GPIO_TypeDef *CS_PORTS[] = { CS_SPI3V3_MOBCtoSUNSMX_GPIO_Port, CS_SPI3V3_SUNSPY_GPIO_Port}; // MX for PSD
//	// CSピンのリスト ピン
//	uint16_t CS_PINS[] = { CS_SPI3V3_MOBCtoSUNSMX_Pin, CS_SPI3V3_SUNSPY_Pin};  // MX for PSD

    // CSピンのリスト ポート
    GPIO_TypeDef *CS_PORTS[] = { CS_SPI3V3_SUNSPY_GPIO_Port };
    // CSピンのリスト ピン
    uint16_t CS_PINS[] = { CS_SPI3V3_SUNSPY_Pin };

#endif /*for 1U*/

static void ACS13_SUNS_init(void); //アプリ初期化関数
static void ACS13_SUNS_FET_ON(uint8_t counter_feton);
static void ACS13_SUNS_FET_OFF(uint8_t counter_feton, uint8_t counter_fetoff);
static void ACS13_SUNS_SOLAR_IRRADIANCE(int i, int flag, int eclipse_flag[5]);
static void ACS13_SUNS_ANGULAR_POSITION(int i);
static int ACS13_SUNS_AVE_Calc(int i, int eclipse_flag[5], float SUNVector_SUM[3], float SUNVector_AVE[3]);
static int ACS13_SUNS_UNI_Calc(int i, int eclipse_flag[5], float SUNVector_UNI[3], uint8_t sun_uni_method, float* sun_uni_theshold, int* sun_calc_counter, float voting_vector[3], int* sunsensor_id);
static void ACS13_SUNS_VOTING_METHOD(int* sun_calc_counter, float voting_vector[3], float SUNVector_UNI[3]);
static void ACS13_SUNS_ANOMALY(int suns_ret, float SUNVector[3]);
static void ACS13_SUNS_GLOBAL_AOCS_set_SUN_TIME(int i, int suns_ret);
static void ACS13_SUNS_exe(void); //アプリ実行関数
static void ACS13_SUNS_initialize_param(void); //MRAM初期化関数
static void ACS13_SUNS_load_param(void); //MRAM読み込み関数
float VectorInnerProduct(const float* v1, const float* v2);
void VectorCopy(const float* v, float* ans);
static void ACS13_sun_vector_propagate(const float in_Pre_sun_vector[3], const float in_Pre_gyro_vector[3], float out_Propagated_sun_vector[3]);

/**
 * @brief アプリケーション作成関数
 *
 * @note
 * @return アプリケーション構造体
 */
AppInfo ACS13_SUNS_update(void)
{
	//create_app_info()を呼び出す．
	return AI_create_app_info("DBG_SUN_SENSOR", ACS13_SUNS_init, ACS13_SUNS_exe); //アプリを作成
}

AppInfo ACS13_SUNS_param_update(void)
{
	//create_app_info()を呼び出す．
	return AI_create_app_info("sample_param_init", ACS13_SUNS_load_param, ACS13_SUNS_initialize_param); //アプリを作成 //initializeとload逆？
}


static void ACS13_SUNS_initialize_param(void)
{
	printf("ACS13 SUNS initialize param.\n");
#ifdef TWO_U
	write_param(&suns_info.counter_feton, &counter_feton_2U_, (uint16_t)(sizeof(counter_feton_2U_)), (uint32_t)0x00004800);
	write_param(&suns_info.counter_getdata, &counter_getdata_2U_, (uint16_t)(sizeof(counter_getdata_2U_)), (uint32_t)0x00004801);
	write_param(&suns_info.counter_fetoff, &counter_fetoff_2U_, (uint16_t)(sizeof(counter_fetoff_2U_)), (uint32_t)0x00004802);
#endif
#ifdef ONE_U
	write_param(&suns_info.counter_feton, &counter_feton_1U_, (uint16_t)(sizeof(counter_feton_1U_)), (uint32_t)0x00004803);
	write_param(&suns_info.counter_getdata, &counter_getdata_1U_, (uint16_t)(sizeof(counter_getdata_1U_)), (uint32_t)0x00004804);
	write_param(&suns_info.counter_fetoff, &counter_fetoff_1U_, (uint16_t)(sizeof(counter_fetoff_1U_)), (uint32_t)0x00004805);
#endif
	write_param(&suns_info.ANGULAR_THRESHOLD, &ANGULAR_THRESHOLD_, (uint16_t)(sizeof(ANGULAR_THRESHOLD_)), (uint32_t)0x00004806);
#ifdef TWO_U
	write_param(&suns_driver[0].SUNS_DCM[0][0], &ACS1_SUNSPX_DCM_[0][0], (uint16_t)(sizeof(ACS1_SUNSPX_DCM_)), (uint32_t)0x0000480A);
    write_param(&suns_driver[1].SUNS_DCM[0][0], &ACS1_SUNSMX_DCM_[0][0], (uint16_t)(sizeof(ACS1_SUNSMX_DCM_)), (uint32_t)0x0000482E);
    write_param(&suns_driver[2].SUNS_DCM[0][0], &ACS1_SUNSPY_DCM_[0][0], (uint16_t)(sizeof(ACS1_SUNSPY_DCM_)), (uint32_t)0x00004852);
    write_param(&suns_driver[3].SUNS_DCM[0][0], &ACS1_SUNSMY_DCM_[0][0], (uint16_t)(sizeof(ACS1_SUNSMY_DCM_)), (uint32_t)0x00004876);
    write_param(&suns_driver[4].SUNS_DCM[0][0], &ACS1_SUNSPZ_DCM_[0][0], (uint16_t)(sizeof(ACS1_SUNSPZ_DCM_)), (uint32_t)0x0000489A);
//  write_param(&suns_class[5].ACS1_SUNSMZ_DCM, &SUNS_CLASS::ACS1_SUNSMZ_DCM_, (uint16_t)(sizeof(suns_class[5].ACS1_SUNSMZ_DCM)), (uint32_t)0x000048BE);
#endif
#ifdef ONE_U
    write_param(&suns_driver[0].SUNS_DCM, &ACS3_SUNSMX_DCM_[0][0], (uint16_t)(sizeof(ACS3_SUNSMX_DCM_)), (uint32_t)0x000048E2);
    write_param(&suns_driver[1].SUNS_DCM, &ACS3_SUNSPY_DCM_[0][0], (uint16_t)(sizeof(ACS3_SUNSPY_DCM_)), (uint32_t)0x00004906);
//  write_param(&suns_class[2].ACS3_SUNSMZ_DCM, &SUNS_CLASS::ACS3_SUNSMZ_DCM_, (uint16_t)(sizeof(suns_class[2].ACS3_SUNSMZ_DCM)), (uint32_t)0x0000492A);
#endif
#ifdef TWO_U
	write_param(&suns_info.ECLIPSE_THRESHOLD, &ECLIPSE_THRESHOLD_2U_, (uint16_t)(sizeof(ECLIPSE_THRESHOLD_2U_)), (uint32_t)0x0000494E);
#endif
#ifdef ONE_U
	write_param(&suns_info.ECLIPSE_THRESHOLD, &ECLIPSE_THRESHOLD_1U_, (uint16_t)(sizeof(ECLIPSE_THRESHOLD_1U_)), (uint32_t)0x00004952);
#endif
    write_param(&suns_info.UNFLTERED_FILTERED_FLAG, &UNFLTERED_FILTERED_FLAG_, (uint16_t)(sizeof(UNFLTERED_FILTERED_FLAG_)), (uint32_t)0x0000956);
    write_param(&suns_info.AVE_UNI_FLAG, &AVE_UNI_FLAG_, (uint16_t)(sizeof(AVE_UNI_FLAG_)), (uint32_t)0x00004957);
    write_param(&suns_info.UNI_METHOD_FLAG, &UNI_METHOD_FLAG_, (uint16_t)(sizeof(UNI_METHOD_FLAG_)), (uint32_t)0x00004958);
    write_param(suns_info.SUNS_USE_FLAG, SUNS_USE_FLAG_, (uint16_t)(sizeof(SUNS_USE_FLAG_)), (uint32_t)0x00004959);
    write_param(&suns_info.voting_sunvec_threshold, &voting_sunvec_threshold_, (uint16_t)(sizeof(voting_sunvec_threshold_)), (uint32_t)0x0000495F);
    write_param(&suns_info.anomaly_sunvec_threshold, &anomaly_sunvec_threshold_, (uint16_t)(sizeof(anomaly_sunvec_threshold_)), (uint32_t)0x00004963);
}

static void ACS13_SUNS_load_param(void)
{
	printf("ACS13 SUNS load param.\n");
#ifdef TWO_U
	read_param(&suns_info.counter_feton, &counter_feton_2U_, (uint16_t)(sizeof(counter_feton_2U_)), (uint32_t)0x00004800);
	read_param(&suns_info.counter_getdata, &counter_getdata_2U_, (uint16_t)(sizeof(counter_getdata_2U_)), (uint32_t)0x00004801);
	read_param(&suns_info.counter_fetoff, &counter_fetoff_2U_, (uint16_t)(sizeof(counter_fetoff_2U_)), (uint32_t)0x00004802);
#endif
#ifdef ONE_U
	read_param(&suns_info.counter_feton, &counter_feton_1U_, (uint16_t)(sizeof(counter_feton_1U_)), (uint32_t)0x00004803);
	read_param(&suns_info.counter_getdata, &counter_getdata_1U_, (uint16_t)(sizeof(counter_getdata_1U_)), (uint32_t)0x00004804);
	read_param(&suns_info.counter_fetoff, &counter_fetoff_1U_, (uint16_t)(sizeof(counter_fetoff_1U_)), (uint32_t)0x00004805);
#endif
	read_param(&suns_info.ANGULAR_THRESHOLD, &ANGULAR_THRESHOLD_, (uint16_t)(sizeof(ANGULAR_THRESHOLD_)), (uint32_t)0x00004806);
#ifdef TWO_U
	read_param(&suns_driver[0].SUNS_DCM[0][0], &ACS1_SUNSPX_DCM_[0][0], (uint16_t)(sizeof(ACS1_SUNSPX_DCM_)), (uint32_t)0x0000480A);
	read_param(&suns_driver[1].SUNS_DCM[0][0], &ACS1_SUNSMX_DCM_[0][0], (uint16_t)(sizeof(ACS1_SUNSMX_DCM_)), (uint32_t)0x0000482E);
	read_param(&suns_driver[2].SUNS_DCM[0][0], &ACS1_SUNSPY_DCM_[0][0], (uint16_t)(sizeof(ACS1_SUNSPY_DCM_)), (uint32_t)0x00004852);
	read_param(&suns_driver[3].SUNS_DCM[0][0], &ACS1_SUNSMY_DCM_[0][0], (uint16_t)(sizeof(ACS1_SUNSMY_DCM_)), (uint32_t)0x00004876);
	read_param(&suns_driver[4].SUNS_DCM[0][0], &ACS1_SUNSPZ_DCM_[0][0], (uint16_t)(sizeof(ACS1_SUNSPZ_DCM_)), (uint32_t)0x0000489A);
//  read_param(&suns_class[5].ACS1_SUNSMZ_DCM, &SUNS_CLASS::ACS1_SUNSMZ_DCM_, (uint16_t)(sizeof(suns_class[5].ACS1_SUNSMZ_DCM)), (uint32_t)0x000048BE);
#endif
#ifdef ONE_U
	read_param(&suns_driver[0].SUNS_DCM, &ACS3_SUNSMX_DCM_[0][0], (uint16_t)(sizeof(ACS3_SUNSMX_DCM_)), (uint32_t)0x000048E2);
	read_param(&suns_driver[1].SUNS_DCM, &ACS3_SUNSPY_DCM_[0][0], (uint16_t)(sizeof(ACS3_SUNSPY_DCM_)), (uint32_t)0x00004906);
//  read_param(&suns_class[2].ACS3_SUNSMZ_DCM, &SUNS_CLASS::ACS3_SUNSMZ_DCM_, (uint16_t)(sizeof(suns_class[2].ACS3_SUNSMZ_DCM)), (uint32_t)0x0000492A);
#endif
#ifdef TWO_U
	read_param(&suns_info.ECLIPSE_THRESHOLD, &ECLIPSE_THRESHOLD_2U_, (uint16_t)(sizeof(ECLIPSE_THRESHOLD_2U_)), (uint32_t)0x0000494E);
#endif
#ifdef ONE_U
	read_param(&suns_info.ECLIPSE_THRESHOLD, &ECLIPSE_THRESHOLD_1U_, (uint16_t)(sizeof(ECLIPSE_THRESHOLD_1U_)), (uint32_t)0x00004952);
#endif
	read_param(&suns_info.UNFLTERED_FILTERED_FLAG, &UNFLTERED_FILTERED_FLAG_, (uint16_t)(sizeof(UNFLTERED_FILTERED_FLAG_)), (uint32_t)0x0000956);
	read_param(&suns_info.AVE_UNI_FLAG, &AVE_UNI_FLAG_, (uint16_t)(sizeof(AVE_UNI_FLAG_)), (uint32_t)0x00004957);
	read_param(&suns_info.UNI_METHOD_FLAG, &UNI_METHOD_FLAG_, (uint16_t)(sizeof(UNI_METHOD_FLAG_)), (uint32_t)0x00004958);
	read_param(suns_info.SUNS_USE_FLAG, SUNS_USE_FLAG_, (uint16_t)(sizeof(SUNS_USE_FLAG_)), (uint32_t)0x00004959);
	read_param(&suns_info.voting_sunvec_threshold, &voting_sunvec_threshold_, (uint16_t)(sizeof(voting_sunvec_threshold_)), (uint32_t)0x0000495F);
	read_param(&suns_info.anomaly_sunvec_threshold, &anomaly_sunvec_threshold_, (uint16_t)(sizeof(anomaly_sunvec_threshold_)), (uint32_t)0x00004963);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void ACS13_SUNS_init(void)
{
//	printf("ACS1 SUN_SENSOR Debug App initialize.\n");
	g_sensor.p_suns = suns;

#ifdef TWO_U
//	int suns_num = 5; // MX for PSD
    int suns_num = 4;
#endif
#ifdef ONE_U
//	int suns_num = 2; // MX for PSD
	int suns_num = 1;
#endif
	// 全てのサンセンサをインスタンスを生成
	for (int i = 0; i < suns_num; i++) {
		SUN_SENSOR_init(&suns_driver[i], (void*)&hspi4, (void*)CS_PORTS[i], CS_PINS[i]);
	}
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void ACS13_SUNS_exe(void)
{

    float SUNVec_SUM[3] = {0.0f};
    float SUNVec[3] = {0.0f};
	int eclipse_flag[5] = {0};
	GLOBAL_AOCS_set_Eclipse();
	float sun_uni_thes = 0.0f;
	int suns_ret = 1;
	int sun_clac_ctr = 0;
	float voting_vec[3] = {0.0f};
	int sun_id = 0;
#ifdef TWO_U
//  int suns_num = 5; // MX for PSD
    int suns_num = 4;
#endif
#ifdef ONE_U
//  int suns_num = 2; // MX for PSD
    int suns_num = 1;
#endif

	// センサ値格納変数を初期化
	for (int i = 0; i < suns_num; i++){
		suns[i].UNFILTERED_PHOTOCELL[0] = 3300.0f;
		suns[i].UNFILTERED_PHOTOCELL[1] = 3300.0f;
		suns[i].UNFILTERED_PHOTOCELL[2] = 3300.0f;
		suns[i].UNFILTERED_PHOTOCELL[3] = 3300.0f;
		suns[i].FILTERED_PHOTOCELL[0] = 3300.0f;
		suns[i].FILTERED_PHOTOCELL[1] = 3300.0f;
		suns[i].FILTERED_PHOTOCELL[2] = 3300.0f;
		suns[i].FILTERED_PHOTOCELL[3] = 3300.0f;
		suns[i].ANGULAR_POSITION[0] = 60.0f;
		suns[i].ANGULAR_POSITION[1] = 60.0f;
		suns[i].SUNVector[0] = 0.0f;
		suns[i].SUNVector[1] = 0.0f;
		suns[i].SUNVector[2] = 0.0f;
	}

    if (!(user_mode->current_mode_id == MD_AOCSFLAG_HIGH_RATE_SPIN)){// normal mode
		uint8_t counter_feton = suns_info.counter_feton;
		uint8_t counter_getdata = suns_info.counter_getdata;
		uint8_t counter_fetoff = suns_info.counter_fetoff;

	    print(SUNS, "counter : %u \r\n", counter);
	    print(SUNS, "on get off: %u %u %u\r\n", counter_feton, counter_getdata, counter_fetoff);

	    // SUNS FET ON by duty counter
	    ACS13_SUNS_FET_ON(counter_feton);

		// Always perform sensing when counter_feton is set 0
		if (PWR_State_GET(PWR_State_SUNS_PXPYMY) && PWR_State_GET(PWR_State_SUNS_MXPZ) && (counter >= counter_getdata || counter_feton == 0) ){

			for (int i = 0; i < suns_num; i++) {
				if (suns_info.SUNS_USE_FLAG[i] == 0){ //サンセンサ使用フラグが0
					suns[i].state = 0xff;
				}
				else{//サンセンサ使用フラグが1
					int ret = SUN_SENSOR_get_all(&suns_driver[i]);
#ifndef HILS
					if(ret != 0){ // SPIエラー
						ANOMALY_SPI_Set_Status(SPI_FAULT_SUNS, ANOMALY_Replace_Status(ret, SPI_ERROR));
						print(SUNS,"ACS1 SUN_SENSOR_get_all %s Failed!\r\n", tag[i]);
					}
					else{ // SPIエラーなし
#endif
//					    printf("angular_position3:%f %f \r\n", suns_class[0].angular_position[0], suns_class[0].angular_position[1]);

						ACS13_SUNS_SOLAR_IRRADIANCE(i, suns_info.UNFLTERED_FILTERED_FLAG, eclipse_flag); //flagはのちにAOCS_FLAGに変更

						ACS13_SUNS_ANGULAR_POSITION(i);

						if (suns_info.AVE_UNI_FLAG == 0){  // 選択則
							suns[i].suns_ret = ACS13_SUNS_UNI_Calc(i, eclipse_flag, SUNVec, suns_info.UNI_METHOD_FLAG, &sun_uni_thes, &sun_clac_ctr, voting_vec, &sun_id);
						}
						else { // 平均則
							suns[i].suns_ret = ACS13_SUNS_AVE_Calc(i, eclipse_flag, SUNVec_SUM, SUNVec);
						}
						suns_ret = suns_ret * suns[i].suns_ret; // 1つでも精サンセンサによる太陽ベクトルが入れば0になる
						print(SUNS,"SUNVec,%f,%f,%f\r\n",SUNVec[0],SUNVec[1],SUNVec[2]);
#ifndef HILS
					}
#endif
				}
				suns[i].state = suns_driver[i].state;
				GLOBAL_AOCS_set_SUNS_GET_TIME(i, suns_driver[i].sunsensor_time_now);
			}
#ifdef TWO_U
			// 5面すべての蝕フラグが立った場合，GLOBALの蝕フラグを立てる
			if (eclipse_flag[0] * eclipse_flag[1] * eclipse_flag[2] * eclipse_flag[3] * eclipse_flag[4] == 1){
				GLOBAL_AOCS_set_Eclipse();
			}
			else{
			  GLOBAL_AOCS_reset_Eclipse();
			}
#endif/*for 2U*/
#ifdef ONE_U
			// 精2面の蝕フラグが立った場合，GLOBALの蝕フラグを立てる
			if (eclipse_flag[0] * eclipse_flag[1] == 1){
				GLOBAL_AOCS_set_Eclipse();
			}
			else{
			  GLOBAL_AOCS_reset_Eclipse();
			}
#endif/*for 1U*/
//			printf("eclipse_flag 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d\r\n", eclipse_flag[0], eclipse_flag[1], eclipse_flag[2], eclipse_flag[3], eclipse_flag[4], eclipse_flag[5]);
//			printf("Eclipse_flag, %d, \r\n", g_acs.p_GLOBAL_AOCS->Eclipse_flag);

			// Anomaly detection： 蝕フラグが立っていないのに太陽光を検知しない時
			ACS13_SUNS_ANOMALY(suns_ret, SUNVec);
		}

		// SUNS FET OFF by duty counter
		ACS13_SUNS_FET_OFF(counter_feton, counter_fetoff);
    }
    else { // high spin rate mode or separation mode

    	if(PWR_State_GET(PWR_State_SUNS_PXPYMY) && PWR_State_GET(PWR_State_SUNS_MXPZ) && suns_info.hi_spin_flag == 1){
    		for (int i = 0; i < suns_num; i++) {
				if (suns_info.SUNS_USE_FLAG[i] == 0){ //サンセンサ使用フラグが0
					suns[i].state = 0xff;
				}
				else{ // サンセンサ使用フラグが0
					int ret = SUN_SENSOR_get_only_angle(&suns_driver[i]);
#ifndef HILS
					if(ret != 0){ // SPIエラー
						ANOMALY_SPI_Set_Status(SPI_FAULT_SUNS, ANOMALY_Replace_Status(ret, SPI_ERROR));
						print(SUNS,"ACS1 SUN_SENSOR_get_all %s Failed!\r\n", tag[i]);
					}
					else{ // SPIエラーなし
#endif
						ACS13_SUNS_ANGULAR_POSITION(i);

						if (suns_info.AVE_UNI_FLAG == 0){  // 選択則
							suns[i].suns_ret = ACS13_SUNS_UNI_Calc(i, eclipse_flag, SUNVec, 1, &sun_uni_thes, &sun_clac_ctr, voting_vec, &sun_id);//ACS13_SUNS_SOLAR_IRRADIANCE通らないのでeclipse_flagは必ず0　 選択則はANGLE_POSITIONのみ
						}
						else { // 平均則
							suns[i].suns_ret = ACS13_SUNS_AVE_Calc(i, eclipse_flag, SUNVec_SUM, SUNVec);//ACS13_SUNS_SOLAR_IRRADIANCE通らないのでeclipse_flagは必ず0
						}
				        suns_ret = suns_ret * suns[i].suns_ret; // 1つでも精サンセンサによる太陽ベクトルが入れば0になる
						print(SUNS,"SUNVec,%f,%f,%f\r\n",SUNVec[0],SUNVec[1],SUNVec[2]);
#ifndef HILS
					}
#endif
				}
    			suns[i].state = suns_driver[i].state;
    			GLOBAL_AOCS_set_SUNS_GET_TIME(i, suns_driver[i].sunsensor_time_now);
    	    }

#ifdef TWO_U
			// 5面すべての蝕フラグが立った場合，GLOBALの蝕フラグを立てる
			if (eclipse_flag[0] * eclipse_flag[1] * eclipse_flag[2] * eclipse_flag[3] * eclipse_flag[4] == 1){
				GLOBAL_AOCS_set_Eclipse();
			}
#endif/*for 2U*/
#ifdef ONE_U
			// 精2面の蝕フラグが立った場合，GLOBALの蝕フラグを立てる
			if (eclipse_flag[0] * eclipse_flag[1] == 1){
				GLOBAL_AOCS_set_Eclipse();
			}
#endif/*for 1U*/

    		print(SUNS,"SUNS SyncFlag enable \r\n");
    	}
    	hispin_counter += 1;
    	print(SUNS,"hispin_counter:%u\r\n", hispin_counter);
    	if (hispin_counter == 5){
    		suns_info.hi_spin_flag = 1; //FETがONになってから200ms　*　5 = 1000ms待つ　その後ずっとSuns_class.hi_spin_flagは1
    		hispin_counter = 0;
    	}
    } // end of high spin rate mode

    print(SUNS, "preSUNVec %f %f %f\r\n", preSUNVec[0], preSUNVec[1],preSUNVec[2]);
    print(SUNS, "SUNVec %f %f %f\r\n", SUNVec[0], SUNVec[1],SUNVec[2]);

    // サンセンサの取得時刻をGlobal AOCS parameterに設定
    ACS13_SUNS_GLOBAL_AOCS_set_SUN_TIME(sun_id, suns_ret);

    // 太陽ベクトルがゼロベクトルの場合サンセンサプロパゲート　TODO:if文の条件をサンセンサ値有効フラグが立っていない場合に修正
    if (Fabs(SUNVec,3) <= 0.01f){
      ACS13_sun_vector_propagate(preSUNVec, g_acs.p_GLOBAL_AOCS->Omega, SUNVec); // タスクリストの順番的にSUNSが9Axis,GYROより先になっているのでOmegaはまだ1サイクル前の角速度が格納されたまま
    }
    // サンセンサ値をGlobal AOCS parameterに設定
    GLOBAL_AOCS_set_SUNVec(SUNVec);

    // 今回の太陽ベクトルを1step前の太陽ベクトルに格納
    VectorCopy(SUNVec, preSUNVec);

} // end of sun sensor main code


/**
  * @name   :ACS13_SUNS_FET_ON
  * @brief  :カウンタが設定値になったらサンセンサのFETをONにする
  * @param  :
  * @retval :
  */
static void ACS13_SUNS_FET_ON(uint8_t counter_feton)
{

    if(PWR_GET_MODE(PWR_State_SUNS_PXPYMY) && PWR_GET_MODE(PWR_State_SUNS_MXPZ)){
		// Ignore fet operation when counter_feton is set 0
		if(counter == counter_feton && counter_feton != 0){
			PWR_Operate_ON_OFF(PWR_State_SUNS_PXPYMY, 1);
			PWR_Operate_ON_OFF(PWR_State_SUNS_MXPZ, 1);
            print(SUNS,"PwrON\r\n\r\n");
//		    c2a_delay_ms(1000);
		}
	}
}

/**
  * @name   :ACS13_SUNS_FET_OFF
  * @brief  :カウンタが設定値になったらサンセンサのFETをOFFにしてカウンタをリセット
  * @param  :
  * @retval :float
  */
static void ACS13_SUNS_FET_OFF(uint8_t counter_feton, uint8_t counter_fetoff)
{
	if(PWR_GET_MODE(PWR_State_SUNS_PXPYMY) && PWR_GET_MODE(PWR_State_SUNS_MXPZ)){

		// Ignore fet operation when counter_feton is set 0
		if(counter >= counter_fetoff && counter_feton != 0){
            PWR_Operate_ON_OFF(PWR_State_SUNS_PXPYMY, 0);
            PWR_Operate_ON_OFF(PWR_State_SUNS_MXPZ, 0);

			print(SUNS,"PwrOFF\r\n");
			counter = 0;

//			// サンセンサのFETをOFFにするときpreSUNVecを初期化→サンセンサプロパゲートを行うので初期化しない
//			for (int i = 0; i < 3; i++){
//				preSUNVec[i] = 0.0f;
//			}
//			print(SUNS,"SUNS in Duty cycle\r\n\r\n");
		}
		counter += 1;
	}
}

static void ACS13_SUNS_SOLAR_IRRADIANCE(int i, int flag, int eclipse_flag[5])
{

#ifdef HILS

	float HILS_ECLIPSE_THRESHOLD = 500;

#ifdef TWO_U
	// HILS用の蝕フラグを立てる
	if(AOCS_hils->suns1[2] < HILS_ECLIPSE_THRESHOLD){
		eclipse_flag[0] = 1;
	}
	if(AOCS_hils->suns2[2] < HILS_ECLIPSE_THRESHOLD){
		eclipse_flag[1] = 1;
	}
	if(AOCS_hils->suns3[2] < HILS_ECLIPSE_THRESHOLD){
		eclipse_flag[2] = 1;
	}
	if(AOCS_hils->suns4[2] < HILS_ECLIPSE_THRESHOLD){
		eclipse_flag[3] = 1;
	}
	if(AOCS_hils->suns5[2] < HILS_ECLIPSE_THRESHOLD){
		eclipse_flag[4] = 1;
	}
//	if(suns6buf[2] < HILS_ECLIPSE_THRESHOLD){
//		eclipse_flag[5] = 1;
//	}
//	for (int j = 0; j < 4; j++) {
//		suns_class[0].unfiltered_photocell_voltage[j] = 1.0f;
//		suns_class[1].unfiltered_photocell_voltage[j] = 1.0f;
//		suns_class[2].unfiltered_photocell_voltage[j] = 1.0f;
//		suns_class[3].unfiltered_photocell_voltage[j] = 1.0f;
//		suns_class[4].unfiltered_photocell_voltage[j] = 1.0f;
//		suns_class[5].unfiltered_photocell_voltage[j] = 1.0f;
//		suns_class[0].filtered_photocell_voltage[j] = 1.0f;
//		suns_class[1].filtered_photocell_voltage[j] = 1.0f;
//		suns_class[2].filtered_photocell_voltage[j] = 1.0f;
//		suns_class[3].filtered_photocell_voltage[j] = 1.0f;
//		suns_class[4].filtered_photocell_voltage[j] = 1.0f;
//		suns_class[5].filtered_photocell_voltage[j] = 1.0f;
//	}
#endif/*for 2U*/

#ifdef ONE_U
	// HILS用の蝕フラグを立てる
	// 精サンセンサ用
	if(AOCS_hils->suns2[2] < HILS_ECLIPSE_THRESHOLD){	// MX
		eclipse_flag[0] = 1;
	}
	if(AOCS_hils->suns3[2] < HILS_ECLIPSE_THRESHOLD){	// PY
		eclipse_flag[1] = 1;
	}
#endif/*for 1U*/

#endif/*for HILS*/

	if (flag == 0){ // UNFILTERED & FILTERED
		for (int j = 0; j < 4; j++) {
			suns[i].UNFILTERED_PHOTOCELL[j] = 1000.0f * suns_driver[i].unfiltered_photocell_voltage[j];
			suns[i].FILTERED_PHOTOCELL[j] = 1000.0f * suns_driver[i].filtered_photocell_voltage[j];
			print(SUNS, "suns[%d].UNFILTERED[%d],%f\r\n", i, j, suns[i].UNFILTERED_PHOTOCELL[j]);
			print(SUNS, "suns[%d].FILTERED[%d],%f\r\n", i, j, suns[i].FILTERED_PHOTOCELL[j]);

#ifndef HILS
			// 閾値以上の電圧なら蝕フラグを立てる
			if (suns[i].UNFILTERED_PHOTOCELL[j] > suns_info.ECLIPSE_THRESHOLD) eclipse_flag[i] = 1;
#endif/*for HILS*/
		}
	}
	else if (flag == 1){ // only UNFILTERED
		for (int j = 0; j < 4; j++) {
			suns[i].UNFILTERED_PHOTOCELL[j] = 1000.0f * suns_driver[i].unfiltered_photocell_voltage[j];
#ifndef HILS
			// 閾値以上の電圧なら蝕フラグを立てる
			if (suns[i].UNFILTERED_PHOTOCELL[j] > suns_info.ECLIPSE_THRESHOLD) eclipse_flag[i] = 1;
#endif/*for HILS*/
		}
	}
	else if (flag == 2){ // only FILTERED
		for (int j = 0; j < 4; j++) {
			suns[i].FILTERED_PHOTOCELL[j] = 1000.0f * suns_driver[i].filtered_photocell_voltage[j];
#ifndef HILS
			// 閾値以上の電圧なら蝕フラグを立てる
			if (suns[i].FILTERED_PHOTOCELL[j] > suns_info.ECLIPSE_THRESHOLD) eclipse_flag[i] = 1;
#endif/*for HILS*/
		}
	}
}

static void ACS13_SUNS_ANGULAR_POSITION(int i)
{
#ifdef HILS
#ifdef TWO_U
	for (int j = 0; j < 2; j++) {
		// suns1buf[3] = {alpha, beta, irradiance}
	  suns_driver[0].angular_position[j] = AOCS_hils->suns1[j]; // PX
	  suns_driver[1].angular_position[j] = AOCS_hils->suns2[j]; // MX
	  suns_driver[2].angular_position[j] = AOCS_hils->suns3[j]; // PY
	  suns_driver[3].angular_position[j] = AOCS_hils->suns4[j]; // MY
	  suns_driver[4].angular_position[j] = AOCS_hils->suns5[j]; // PZ
//		suns_class[5].angular_position[j] = suns6buf[j]; // MZ
	}
	SUNS_Vector_Calc(suns_driver[0].angular_position[0], suns_driver[0].angular_position[1], suns_driver[0].SUN_Vec);
	SUNS_Vector_Calc(suns_driver[1].angular_position[0], suns_driver[1].angular_position[1], suns_driver[1].SUN_Vec);
	SUNS_Vector_Calc(suns_driver[2].angular_position[0], suns_driver[2].angular_position[1], suns_driver[2].SUN_Vec);
	SUNS_Vector_Calc(suns_driver[3].angular_position[0], suns_driver[3].angular_position[1], suns_driver[3].SUN_Vec);
	SUNS_Vector_Calc(suns_driver[4].angular_position[0], suns_driver[4].angular_position[1], suns_driver[4].SUN_Vec);
#endif/*for 2U*/

#ifdef ONE_U
	for (int j = 0; j < 2; j++) {
	    //suns1buf[3] = {alpha, beta, irradiance}
	  suns_driver[0].angular_position[j] = AOCS_hils->suns2[j]; // MX
	  suns_driver[1].angular_position[j] = AOCS_hils->suns3[j]; // PY
//		suns_class[2].angular_position[j] = suns6buf[j]; // MZ
	}
    SUNS_Vector_Calc(suns_driver[0].angular_position[0], suns_driver[0].angular_position[1], suns_driver[0].SUN_Vec);
    SUNS_Vector_Calc(suns_driver[1].angular_position[0], suns_driver[1].angular_position[1], suns_driver[1].SUN_Vec);
#endif/*for 1U*/

	SUN_SENSOR_get_time_HILS(&suns_driver[i]);

#endif/*for HILS*/

	for (int j = 0; j < 2; j++) {
		suns[i].ANGULAR_POSITION[j] = (suns_driver[i].angular_position[j]);
		print(SUNS,"suns[%d].ANGULAR_POSITION[%d],%f\r\n", i, j, suns[i].ANGULAR_POSITION[j]);
	}

}

static int ACS13_SUNS_AVE_Calc(int i, int eclipse_flag[5], float SUNVector_SUM[3], float SUNVector_AVE[3])
{

	// angular position αとβの絶対値が60°未満かつ蝕フラグが立っていない場合有効とみなす
	if (abs(suns[i].ANGULAR_POSITION[0]) < suns_info.ANGULAR_THRESHOLD && abs(suns[i].ANGULAR_POSITION[1]) < suns_info.ANGULAR_THRESHOLD && eclipse_flag[i] == 0){ // 有意な角度の場合のみ方向ベクトルを計算

#ifdef HILS // S2Eのangular positionを代入しているのでget_angular_positionのベクトル計算がされてない
		SUNS_Vector_Calc(suns_driver[i].angular_position[0], suns_driver[i].angular_position[1], suns_driver[i].SUN_Vec);
#endif/*for HILS*/

		// センサ座標系太陽ベクトルを機体座標系太陽ベクトルに変換
		SUN_Vector_DCM_Sensor2Body_Calc(&suns_driver[i], suns[i].SUNVector);

		for (int j = 0; j < 3; j++) {
			SUNVector_SUM[j] += suns[i].SUNVector[j];
		}
		Normalization(SUNVector_SUM, 3, SUNVector_AVE); // 足し合わせたベクトルを正規化

		VectorCopy(SUNVector_AVE, SUNVector_SUM);

		return 0;
    }
	else {
		return 1;
	}

}


static int ACS13_SUNS_UNI_Calc(int i, int eclipse_flag[5], float SUNVector_UNI[3], uint8_t sun_uni_method, float* sun_uni_theshold, int* sun_calc_counter, float voting_vector[3], int* sunsensor_id)
{
	float irradiance_sum = 0.0f;
	float angle_sum = 0.0f;

	// angular position αとβの絶対値が60°未満かつ蝕フラグが立っていない場合有効とみなす
	if (abs(suns[i].ANGULAR_POSITION[0]) < suns_info.ANGULAR_THRESHOLD && abs(suns[i].ANGULAR_POSITION[1]) < suns_info.ANGULAR_THRESHOLD && eclipse_flag[i] == 0){

		if (sun_uni_method == 0){ // UNFILTERED_PHOTOCELLとFILTERED_PHOTOCELLの和が最小の太陽ベクトルを選択
			// UNFILTERED_PHOTOCELLとFILTERED_PHOTOCELLの和を求める
			for (int j = 0; j < 4; j++){
					irradiance_sum += suns[i].UNFILTERED_PHOTOCELL[j] + suns[i].FILTERED_PHOTOCELL[j];
			}

			// 太陽強度がひとつ前の面の値より大きい場合=irradiance_sumが小さい場合 太陽ベクトルを更新 　とりあえず最初のやつをsun_uni_thesholdの初期値，Voting則の比較基準にする
			if (*sun_calc_counter == 0 || irradiance_sum < *sun_uni_theshold){

				// センサ座標系太陽ベクトルを機体座標系太陽ベクトルに変換
				SUN_Vector_DCM_Sensor2Body_Calc(&suns_driver[i], suns[i].SUNVector);
				VectorCopy(suns[i].SUNVector, SUNVector_UNI);


				*sunsensor_id = i;

				memcpy(sun_uni_theshold, &irradiance_sum, 4);//sun_uni_thesholdを更新

				ACS13_SUNS_VOTING_METHOD(sun_calc_counter, voting_vector, SUNVector_UNI);

			}
		}
		else { // ANGULAR_POSITIONの絶対和が最小の太陽ベクトルを選択 　とりあえず最初のやつをsun_uni_thesholdの初期値，Voting則の比較基準にする
			angle_sum = fabs(suns[i].ANGULAR_POSITION[0]) + fabs(suns[i].ANGULAR_POSITION[1]);

			if (*sun_calc_counter == 0 || angle_sum < *sun_uni_theshold) {

				SUN_Vector_DCM_Sensor2Body_Calc(&suns_driver[i], suns[i].SUNVector);

				VectorCopy(suns[i].SUNVector, SUNVector_UNI);

				*sunsensor_id = i;

				memcpy(sun_uni_theshold, &angle_sum, 4); //sun_uni_thesholdを更新

				ACS13_SUNS_VOTING_METHOD(sun_calc_counter, voting_vector, SUNVector_UNI);

			}
		}

		return 0;
	}
	else {
		return 1;
	}
}

static void ACS13_SUNS_VOTING_METHOD(int* sun_calc_counter, float voting_vector[3], float SUNVector_UNI[3])
{
	if (*sun_calc_counter == 0){//最初の太陽ベクトルをvoting則の基準ベクトルにする

		VectorCopy(SUNVector_UNI, voting_vector);

		*sun_calc_counter = 1; //最初の太陽ベクトルが計算された後カウンタを上げる 次の太陽ベクトルからこの基準太陽ベクトルと比較をする
	}
	else if (*sun_calc_counter == 1 && VectorInnerProduct(voting_vector, SUNVector_UNI) > suns_info.voting_sunvec_threshold){

		suns_info.sun_anomaly_state = 0;
	}

}

static void ACS13_SUNS_ANOMALY(int suns_ret, float SUNVector[3])
{
	// 最初に計算された太陽ベクトルをpreSUNVecに代入
	if (preSUNVec[0] + preSUNVec[1] + preSUNVec[2] == 0.0f){
		VectorCopy(SUNVector, preSUNVec);
	}
	// Anomaly detection：計算された太陽ベクトルが異常値だった場合
	if(suns_ret == 0 && suns_info.sun_anomaly_state == 1){
		if(SUNVector[0] + SUNVector[1] + SUNVector[2] == 0 || VectorInnerProduct(SUNVector, preSUNVec) < suns_info.anomaly_sunvec_threshold){
//			printf("SUNS_anomaly\r\n");
			print(ANOMALY,"SUNS_DONT_WORK_add_anomaly \r\n");
//			AL_add_anomaly(AL_GROUP_SUNS,AL_DONT_WORK_SUNS);
//			Auto_trans_set(&mode_trans_driver->aocs.enable);
			VectorCopy(preSUNVec, SUNVector); //1step前のサンベクトルを渡す

		}
	}
	suns_info.sun_anomaly_state = 1; //ACS13_SUNS_VOTING_METHODで0にされてたら1に戻す
}

static void ACS13_SUNS_GLOBAL_AOCS_set_SUN_TIME(int i, int suns_ret)
{
#ifdef TWO_U
	if (suns_info.AVE_UNI_FLAG == 0){ // 選択則
		GLOBAL_AOCS_set_SUN_TIME(suns_driver[i].sunsensor_time_before, suns_driver[i].sunsensor_time_now);
	}
	else{ //平均則
		GLOBAL_AOCS_set_SUN_TIME(suns_driver[3].sunsensor_time_before, suns_driver[3].sunsensor_time_now); //PY(PX~MZの順で取得する中央のセンサ)
	}
#endif/*for 2U*/
#ifdef ONE_U
	if (suns_info.AVE_UNI_FLAG == 0){ // 選択則
		GLOBAL_AOCS_set_SUN_TIME(suns_driver[i].sunsensor_time_before, suns_driver[i].sunsensor_time_now);
	}
	else{ //平均則
		GLOBAL_AOCS_set_SUN_TIME(suns_driver[1].sunsensor_time_before, suns_driver[1].sunsensor_time_now); //PY(MX,PY,MZの順で取得する中央のセンサ)
	}
#endif/*for 1U*/
#ifdef HILS
	GLOBAL_AOCS_set_SUN_TIME(suns_driver[0].sunsensor_time_before_hils, suns_driver[0].sunsensor_time_now_hils);
#endif/*for HILS*/
}

//debug　本当は#include <vector.h>使いたい
// ベクトルvの値をansにコピー
void VectorCopy(const float* v, float* ans)
{
  ans[0] = v[0];
  ans[1] = v[1];
  ans[2] = v[2];
}

// ベクトルの内積v1・v2を返す
float VectorInnerProduct(const float* v1, const float* v2)
{
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

/**
 * @brief set counter to counter_feton
 * @note
 */
void ACS13_SUNS_Set_Duty_Counter(void)
{
//モード遷移した時にstatic変数の値がそのまま保持されてしまうので初期化
//サンセンサのFETがONになるときにこの関数を通る　dutyで回ってるときはcounter == counter_fetonなので特に何も起こらない
	counter = suns_info.counter_feton;
	print(SUNS, "counter:%u\r\n", counter);

	hispin_counter = 0;
	suns_info.hi_spin_flag = 0;
}

/**
 * @brief　サンセンサ値から計算された太陽ベクトルが有効でないときに1サイクル前の太陽ベクトルをプロパゲートして太陽ベクトルを計算する
 * @brief　ref:C:\Users\tamaoki\OneDrive\NUcube1+\4_Software(ソフトウェア)\1_System\2_サブシステム設計\5_ACS系(センサ・アクチュエータ)\サンセンサ値が有効でないときの太陽方向計算.pptx
 * @param[in] in_p_Pre_sun_vector　1サイクル前の機体座標系太陽ベクトル[-]
 * @param[in] in_p_Pre_gyro_vector　1サイクル前の機体座標系角速度ベクトル[rad/s]
 * @param[out] out_p_Propagated_sun_vector　プロパゲートした機体座標系太陽ベクトル[-]
 * @return なし
 * 作成者：玉置　2023/12/01
 */
static void ACS13_sun_vector_propagate(const float in_p_Pre_sun_vector[3], const float in_p_Pre_gyro_vector[3], float out_p_Propagated_sun_vector[3])
{
  int get_time_diff = OBCT_diff_in_step(&g_acs.p_GLOBAL_AOCS->SunPreTime, &g_acs.p_GLOBAL_AOCS->SunTime); //1サイクル前の太陽ベクトルを計算した時刻と今回太陽ベクトルを計算した時刻の差[ms]
  float dt = get_time_diff * 0.001f; // ms->s
  float p_W_x_S[3] = {0.0f};    //１サイクル前の角速度と太陽ベクトルの外積を格納する配列
  float p_W_x_S_dt[3] = {0.0f}; //外積にdtをかけた値を格納する配列

  // ω_pre × S_pre
  Outer_product(in_p_Pre_gyro_vector, in_p_Pre_sun_vector, p_W_x_S);

  // (ω_pre × S_pre) * dt
  CoeffVec(p_W_x_S, 3, dt, p_W_x_S_dt);

  // S_propagated = S_pre - (ω_pre × S_pre) * dt
  sub(in_p_Pre_sun_vector, 3, 1, p_W_x_S_dt, 3, 1, out_p_Propagated_sun_vector);

  // 正規化
  Normalization(out_p_Propagated_sun_vector, 3, out_p_Propagated_sun_vector);
}
