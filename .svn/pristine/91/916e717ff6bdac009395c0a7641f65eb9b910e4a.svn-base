/*
* SUNS.h
*
*  Created on: 2021/05/09
*      Author: Hiroaki MASUDA
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
* 2021/05/09  1.0   H.MASUDA      First revision
*
 *****************************************************************************************
*/

#ifndef SUN_SENSOR_H_
#define SUN_SENSOR_H_

//#include <main.h>
#include <src_core/System/TImeManager/time_manager.h>//for delay()
#include <src_user/IfWrapper/IF_class.h>
#include <src_user/Applications/UserDefined/Sensor/ACS123_cmd_AnomalyDetect.h>   // for Error list

//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------

//他の関数でセンサ依存部変数を呼び出さない場合、.hで宣言する必要はなく、.cppで宣言すべき
// エラー一覧  <----　Anomaly追加の為、別のファイルに移動
//typedef enum{
//	NO_ERROR = 0x00U,            		// No error
//	CHECKSUM_ERROR = 0x01U,      		// Checksum error
//	SPI_ERROR = 0x02U,           		// Error while SPI communication
//	ALBEDO_EARTH_ERROR = 0x03U,  		// Sun sensor does not see the Sun
//	ALBEDO_EARTH_SUN_ERROR = 0x04U,  	// Sun sensor does not see the Sun and the Earth*/
//	OTHER_ERROR = 0x05U  				// Internal manufacturer information for the Sun sensor
//}SUN_SENSOR_StateTypeDef;

#define SUN_SENSOR_SPI_TIMEOUT 200

/* ここにあった Error list は ACS123_cmd_AnomalyDetect.h に移動しました */

typedef struct{
	// CSピンの情報
	void* gpio_port;
	uint16_t gpio_pin;

	// SPI handler
	void* spi_port;

	// サンセンサのデータを格納する変数
	float filtered_photocell_voltage[4];    // [V]
	float unfiltered_photocell_voltage[4];  // [V]
	float angular_position[2];              // [deg] //inakawa [x,y]? 内容まで記述した方が分かりやすいと思います

	float FILTERED_PHOTOCELL[4];
	float UNFILTERED_PHOTOCELL[4];
	float ANGULAR_POSITION[2];

	float SUN_Vec[3];
	float DCM_Euler[3][3] = {};

    ObcTime sunsensor_time_before;
    ObcTime sunsensor_time_now;

#ifdef HILS
	ObcTime sunsensor_time_before_hils;
	ObcTime sunsensor_time_now_hils;
#endif

	//param
	float SUNS_DCM[3][3];

	// センサエラー番号の格納
	SUN_SENSOR_StateTypeDef state;
}SunSensorDriver;

typedef struct{
	float k_error = 0.1f + 0.5f;  //[deg] 0.1f：precision, 0.5f：3siguma tamaoki add
	float ANGULAR_THRESHOLD;
	uint8_t counter_feton;
	uint8_t counter_getdata;
	uint8_t counter_fetoff;
	float ECLIPSE_THRESHOLD;
    uint8_t UNFLTERED_FILTERED_FLAG;  //太陽強度選択フラグ
    uint8_t AVE_UNI_FLAG;             //平均則と選択則のフラグ
    uint8_t UNI_METHOD_FLAG;          //太陽強度選択フラグ　のちにAOC_FLAGに変更
    uint8_t SUNS_USE_FLAG[5];         // 2U:PX, MX, PY, MY, PZ 0:no use 1:use
    float voting_sunvec_threshold;
    float anomaly_sunvec_threshold;
    uint8_t sun_anomaly_state = 1;
    uint8_t hi_spin_flag = 0;  //高速スピンモードにモード遷移した際，FETがONになった周期にデータを取得しようとしてSPIエラーが出る
                               //そのため最初の周期はデータを取得しないようにするためのフラグ
}SunSensorInfo;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int SUN_SENSOR_init(SunSensorDriver* sun_driver, void* spi_port, void* gpio_port, uint16_t gpio_pin);
int SUN_SENSOR_get_all(SunSensorDriver* sun_driver);
void SUN_Vector_DCM_Sensor2Body_Calc(SunSensorDriver* sun_driver, float SUNBodyVec[3]);
int SUN_SENSOR_get_only_angle(SunSensorDriver* sun_driver);
void SUNS_Vector_Calc(float alpha, float beta, float SUNSensorVec[3]);
void SUN_SENSOR_get_time_HILS(SunSensorDriver* sun_driver);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SUN_SENSOR_H_ */

