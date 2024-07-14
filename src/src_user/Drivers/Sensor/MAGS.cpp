/*
 * MAGS.cpp
 *
 *  Created on: 2021/05/27
 *      Author: masud
 */

#ifdef TWO_U
#include <src_user/Drivers/Sensor/MAGS.h>
#include "src_user/Applications/UserDefined/Global_Sensor_Value.h"
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Library/General/DBG_printf.h>


//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------
//======MRAM格納パラメータの実体宣言部======//
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS BEGIN *** //
constexpr float MAGS_CLASS::k_mag_scale_[3]; //MAG Scale Factor
constexpr float MAGS_CLASS::k_mag_offset_[3]; //MAG Offset
constexpr float MAGS_CLASS::MAGS_DCM_[3][3]; //plus or minus sign x,y,z
constexpr uint8_t MAGS_CLASS::MAGS_Continuous_Time_THRESHOLD_; // Continuous Time Threshold for Anomaly
constexpr float MAGS_CLASS::mems_mag_sf_t0_[3]; // standard temperature of MAG ScaleFactor temperature compensation coefficient formula
constexpr float MAGS_CLASS::mems_mag_sf_t1_[3]; // first term of MAG ScaleFactor temperature compensation coefficient formula
constexpr float MAGS_CLASS::mems_mag_sf_t2_[3]; // const term of MAG ScaleFactor temperature compensation coefficient formula
constexpr float MAGS_CLASS::mems_mag_os_t0_[3]; // standard temperature of MAG Offset temperature compensation coefficient formula
constexpr float MAGS_CLASS::mems_mag_os_t1_[3]; // first term of MAG Offset temperature compensation coefficient formula
constexpr float MAGS_CLASS::mems_mag_os_t2_[3]; // const term of MAG Offset temperature compensation coefficient formula
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS END*** //

//-----------------------------------------
// センサ依存変数宣言（センサ動作させるために必要だが、汎用的な関数をここに定義しておく）
//-----------------------------------------


//-----------------------------------------
// センサクラス定義
//-----------------------------------------

//-----------------------------------------
// センサ依存部関数（マイコン依存関数のラッパー）定義
//-----------------------------------------


/**
  * @name   :MAGS_SENSOR_Mag649_Get
  * @brief  :ADCでのアナログ値　デジタル値を求める
  * @param  :
  * @retval :
  */
int MAGS_CLASS::MAGS_SENSOR_AD7654_Get(int state)
{
	this->MUX_ANASEN(state);
	this->Select_ANASEN_Channel();
	this->ADC_GPIO_BUSY();
//	this->ADC_GPIO_All_Read(this->ADC_intData, this->ADC_Data);
	this->ADC_GPIO_All_Read();
	if(state == 4){
	this->magpx = this->ADC_Data;
	this->ADC_magpx = this->ADC_intData;
	// 試験手順書ID2「MAG-MUX-ADC入出力電圧確認」で使用
	//以下のprint文で「つなたーむ」に表示される電圧値とCCCVから試験用ハーネスに印加する電圧値が一致することを確認
	print(ACS_ADC, "ADC Voltage, %d, %f\r\n", ADC_magpx, magpx);
	}
	else if(state == 5){
	this->magmx = this->ADC_Data;
	this->ADC_magmx = this->ADC_intData;
	}
	else if(state == 6){
	this->magpy = this->ADC_Data;
	this->ADC_magpy = this->ADC_intData;
	}
	else if(state == 7){
	this->magmy = this->ADC_Data;
	this->ADC_magmy = this->ADC_intData;
	}
	else if(state == 8){
	this->magpz = this->ADC_Data;
	this->ADC_magpz = this->ADC_intData;
	}
	else if(state == 9){
	this->magmz = this->ADC_Data;
	this->ADC_magmz = this->ADC_intData;
	}
	return 0;
}


//-----------------------------------------
// センサ依存部関数（非マイコン依存かつデータ取得でない関数）宣言
//-----------------------------------------



/**
  * @name   :Temperature_Compensation_SF
  * @brief  :Temperature_Compensation of SF
  * @param  :
  * @retval :float
  */
void MAGS_CLASS::Temperature_Compensation_SF(int i, float g_t) {
	//g_t(global temperature) : g_sensor.p_adc->MAGS_TEMP
	//±0.5f/100.0f：SF誤差, ±(0.01f/100.0f)* (k_T_ - 25.0f)：SF温度係数,　±0.0033f/100.0f：非線形誤差
//	mag_scale_temp[i] = 1.0f + ±0.5f/100.0f + ±(0.01f/100.0f)*(g_t - 25.0f) + ±0.0033f/100.0f;
	mag_scale_temp[i] = 1.0f + mems_mag_sf_t1_[i] * (g_t - mems_mag_sf_t0_[i]) + mems_mag_sf_t2_[i];
}


/**
  * @name   :Temperature_Compensation_OFFSET
  * @brief  :Temperature_Compensation of OFFSET
  * @param  :
  * @retval :float
  */
void MAGS_CLASS::Temperature_Compensation_OFFSET(int i, float g_t) {
	//g_t(global temperature) : g_sensor.p_adc->GYROX_TEMP or GYROY_TEMP or GYROZ_TEMP
	//±0.000001f：offset ±100nT [in zero field], ±0.000000001*(k_T_ - 25.0f)：オフセット温度特性, ±0.00000002：ノイズ標準偏差
//	mag_offset_temp[i] = 0.000000001*(g_t - 25.0f) + 0.000001f + 0.00000002f; (T)
	mag_offset_temp[i] = mems_mag_os_t1_[i] * (g_t - mems_mag_os_t0_[i]) + mems_mag_os_t2_[i];
}



//-----------------------------------------
// センサ依存部関数（データ取得関数）宣言
//-----------------------------------------
/**
  * @name   :Convert_Voltage2MAG
  * @brief  :Calculate Voltage to Magnetic field
  * @param  :
  * @retval :float
  */
float MAGS_CLASS::Convert_Voltage2MAG(int i, float g_a) {
    float g_mag = (g_a * 1000.0f / k_mag_scale[i]) * 0.001f * 0.001f + k_mag_offset[i]; //[T]
    return g_mag;
}

/**
  * @name   :Convert_Voltage2MAG_TempCorrect
  * @brief  :Calculate Voltage to Magnetic field considering temperature compensation
  * @param  :
  * @retval :float
  */
float MAGS_CLASS::Convert_Voltage2MAG_TempCorrect(int i, float g_a) {
    float g_mag = (g_a * 1000.0f / k_mag_scale[i]) * mag_scale_temp[i] * 0.001f * 0.001f + k_mag_offset[i] + mag_offset_temp[i]; //[T]
    return g_mag;
}

/**
  * @name   :Calculate_MAG_value
  * @brief  :Mag649は各軸P面からM面の値を引いた値がその軸の磁場となるのでその計算
  * @param  :
  * @retval :float
  */
void MAGS_CLASS::Calculate_MAG_value(){
	this->ADC_magx = this->ADC_magpx - this->ADC_magmx;
	this->ADC_magy = this->ADC_magpy - this->ADC_magmy;
	this->ADC_magz = this->ADC_magpz - this->ADC_magmz;

//	print(MAGS, "MAG,%d,%d,%d,%d,%d,%d\r\n", ADC_magpx, ADC_magmx, ADC_magpy, ADC_magmy, ADC_magpz, ADC_magmz);

	this->magx = this->magpx - this->magmx;
	this->magy = this->magpy - this->magmy;
	this->magz = this->magpz - this->magmz;

//	printf("MAGXYZ,%d,%d,%d\r\n", ADC_magx, ADC_magy, ADC_magz);

	this->Mag[0] = this->Convert_Voltage2MAG_MAGNARO(0, this->magx, ADS_Mode(PRECIZE_MAG_TEMP));
	this->Mag[1] = this->Convert_Voltage2MAG_MAGNARO(1, this->magy, ADS_Mode(PRECIZE_MAG_TEMP));
	this->Mag[2] = this->Convert_Voltage2MAG_MAGNARO(2, this->magz, ADS_Mode(PRECIZE_MAG_TEMP));
}

//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

/**
 * @brief
 */
int MAGS_CLASS::MAGS_SENSOR_init()
{
	return 0;
}



/**
  * @name   :Convert_Voltage2MAG_MAGNARO
  * @brief  :出力電圧から磁束密度へ変換 モードフラグにより温度補正の有無を選択
  * @param  :
  * @retval :float
  */
float MAGS_CLASS::Convert_Voltage2MAG_MAGNARO(int i, float g_a, int flag){
	if (flag == 1){
		Temperature_Compensation_SF(i, g_sensor.p_adc->MAGS_TEMP);
		Temperature_Compensation_OFFSET(i, g_sensor.p_adc->MAGS_TEMP);
		float g_r = this->Convert_Voltage2MAG_TempCorrect(i, g_a);
		return g_r;
	}
	else {
		float g_r = this->Convert_Voltage2MAG(i, g_a);
		return g_r;
	}
}



int MAGS_CLASS::MAGS_SENSOR_get_px()
{
	int ret = this->MAGS_SENSOR_AD7654_Get(4);

	 if(ret != 0){
		this->magpx = 0.0f;
		this->ADC_magpx = 0;
	 };

	return 0;
}

int MAGS_CLASS::MAGS_SENSOR_get_mx()
{
	int ret = this->MAGS_SENSOR_AD7654_Get(5);

	 if(ret != 0){
		this->magmx = 0.0f;
		this->ADC_magmx = 0;
	 };

	return 0;
}

int MAGS_CLASS::MAGS_SENSOR_get_py()
{
	int ret = this->MAGS_SENSOR_AD7654_Get(6);

	 if(ret != 0){
		this->magpy = 0.0f;
		this->ADC_magpy = 0;
	 };

	return 0;
}

int MAGS_CLASS::MAGS_SENSOR_get_my()
{
	int ret = this->MAGS_SENSOR_AD7654_Get(7);

	 if(ret != 0){
		this->magmy = 0.0f;
		this->ADC_magmy = 0;
	 };

	return 0;
}

int MAGS_CLASS::MAGS_SENSOR_get_pz()
{
	int ret = this->MAGS_SENSOR_AD7654_Get(8);

	 if(ret != 0){
		this->magpz = 0.0f;
		this->ADC_magpz = 0;
	 };

	return 0;
}

int MAGS_CLASS::MAGS_SENSOR_get_mz()
{
	int ret = this->MAGS_SENSOR_AD7654_Get(9);

	 if(ret != 0){
		this->magmz = 0.0f;
		this->ADC_magmz = 0;
	 };

	return 0;
}


/**
  * @name   :MAGS_Vector_Calc
  * @brief  :正規化したセンサ磁場ベクトルを求める
  * @param  :
  * @retval :float
  */
void MAGS_CLASS::MAGS_Vector_Calc(float magx, float magy, float magz, float MAGSensorVec[3])
{
	float bunbo = sqrt(magx*magx + magy*magy +magz*magz);

	//not to divide by zero     _taguchi
	    if(bunbo == 0){
	        bunbo += 0.01;
	    }
	MAGSensorVec[0] = magx / bunbo;
	MAGSensorVec[1] = magy / bunbo;
	MAGSensorVec[2] = magz / bunbo;
}

void MAGS_CLASS::MAG_SENSOR_get_time()
{
    magsensor_time_before = magsensor_time_now;
    magsensor_time_now = TMGR_get_master_clock();
}

#ifdef HILS
void MAGS_CLASS::MAG_SENSOR_get_time_HILS()
{
    magsensor_time_before_hils = magsensor_time_now_hils;
    magsensor_time_now_hils = TMGR_get_master_clock();
}
#endif

int MAGS_CLASS::MAGS_SENSOR_get_all()
{
	this->MAGS_SENSOR_get_px();
	this->MAGS_SENSOR_get_mx();
	this->MAGS_SENSOR_get_py();
	this->MAGS_SENSOR_get_my();
	this->MAGS_SENSOR_get_pz();
	this->MAGS_SENSOR_get_mz();
	this->Calculate_MAG_value();
	this->MAG_Vector_DCM_Sensor2Body_Calc();
	this->MAGS_Vector_Calc(this->MAG[0], this->MAG[1], this->MAG[2], this->MAGVec);
	this->MAG_SENSOR_get_time();

	return 0;
}

void MAGS_CLASS::MAG_Vector_DCM_Sensor2Body_Calc()
{
	for (int i = 0; i < 3; i++)
	{
		MAG[i] = 0.0f;
		for (int j = 0; j < 3; j++) {
			MAG[i] += MAGS_DCM[i][j] * Mag[j];
		}
	}
}
#endif /*for 2U*/
