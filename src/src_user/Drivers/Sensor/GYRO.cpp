/*
 * GYRO.cpp
 *
 *  Created on: 2021/05/27
 *      Author: masud
 */

#include <stdio.h>
#include <math.h>
#include <src_user/Drivers/Sensor/ADC.h>
#include <src_user/Drivers/Sensor/GYRO.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //for uint8_t
#include "src_user/Applications/UserDefined/Global_Sensor_Value.h"
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
//#include <src_core/System/TimeManager/obc_time.h>

//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------
//======MRAM格納パラメータの実体宣言部======//
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS BEGIN *** //
constexpr uint8_t GYRO_CLASS::state_[3];
constexpr float GYRO_CLASS::k_freq_[3]; //frequency of the rate of rotation
constexpr float GYRO_CLASS::k_R_[3]; //roll^off resistor inside the ASIC(nominally 48korm)
constexpr float GYRO_CLASS::k_C_[3]; //the value of capacitor C2
constexpr float GYRO_CLASS::k_gyro_scale_[3]; //Scale_Factor(deg/s)
constexpr float GYRO_CLASS::k_gyro_vols_bias_[3]; //Bias(Volts)
constexpr float GYRO_CLASS::k_InputVol_[3]; //supply Voltage (3.3V)
constexpr float GYRO_CLASS::k_K_[3]; // scale factor dependent on rate range
constexpr float GYRO_CLASS::ACS1_GYRO_DCM_[3][3]; //plus or minus sign x,y,z
constexpr float GYRO_CLASS::ACS3_GYRO_DCM_[3][3]; //plus or minus sign x,y,z
constexpr float GYRO_CLASS::GYRO_THRESHOLD_[3];   // angular velocity threshold
constexpr float GYRO_CLASS::GYRO_HIGHSPIN_THRESHOLD_[3];  // angular velocity threshold in high-spin mode
constexpr float GYRO_CLASS::mems_gyro_sf_t0_[3]; // standard temperature of GYRO ScaleFactor temperature compensation coefficient formula
constexpr float GYRO_CLASS::mems_gyro_sf_t1_[3]; // first term of GYRO ScaleFactor temperature compensation coefficient formula
constexpr float GYRO_CLASS::mems_gyro_sf_t2_[3]; // const term of GYRO ScaleFactor temperature compensation coefficient formula
constexpr float GYRO_CLASS::mems_gyro_os_t0_[3]; // standard temperature of GYRO Offset temperature compensation coefficient formula
constexpr float GYRO_CLASS::mems_gyro_os_t1_[3]; // first term of GYRO Offset temperature compensation coefficient formula
constexpr float GYRO_CLASS::mems_gyro_os_t2_[3]; // const term of GYRO Offset temperature compensation coefficient formula
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


#ifdef TWO_U
/**
  * @name   :GYRO_SENSOR_CRM100200_Get
  * @brief  :ADCでのアナログ値　デジタル値を求める
  * @param  :
  * @retval :
  */
int GYRO_CLASS::GYRO_SENSOR_AD7654_Get(int state)
{
	this->MUX_ANASEN(state);
	this->Select_ANASEN_Channel();
	this->ADC_GPIO_BUSY();
//	this->ADC_GPIO_All_Read(this->ADC_intData, this->ADC_Data);
	this->ADC_GPIO_All_Read();
//	printf("GYRO_ADC is %d\r\n", ADC_CLASS::ADC_intData);


	if(state == 1){
		this->ADC_omegax = this->ADC_intData;
		this->omega[0] = this->Convert_Voltage2GYRO_MAGNARO(0, this->ADC_Data, ADS_Mode(MEMS_GYRO_TEMP), g_sensor.p_adc->GYROX_TEMP);
	}

	else if(state == 2){
		this->ADC_omegay = this->ADC_intData;
		this->omega[1] = this->Convert_Voltage2GYRO_MAGNARO(1, this->ADC_Data, ADS_Mode(MEMS_GYRO_TEMP), g_sensor.p_adc->GYROY_TEMP);
	}

	else if(state == 3){
		this->ADC_omegaz = this->ADC_intData;
		this->omega[2] = this->Convert_Voltage2GYRO_MAGNARO(2, this->ADC_Data, ADS_Mode(MEMS_GYRO_TEMP), g_sensor.p_adc->GYROZ_TEMP);
	}

	GYRO_Vector_DCM_Sensor2Body_Calc(this->omega, this->ACS1_GYRO_DCM, this->OMEGA);

	return 0;
}
#endif/*for 2U*/



//-----------------------------------------
// センサ依存部関数（データ取得関数）宣言
//-----------------------------------------
/**
  * @name   :Convert_Voltage2GYRO
  * @brief  :出力電圧からGYROへ変換
  * @param  :
  * @retval :float
  */
float GYRO_CLASS::Convert_Voltage2GYRO(int i, float g_a) {
	float g_r = (((k_gyro_vols_scale[i] * g_a - k_gyro_vols_bias[i]) - k_InputVol[i] * 0.5f)*(1.0f + 2.0f * M_PI * k_freq[i] * k_R[i] * k_C[i]) / k_K[i]) * k_gyro_scale[i] *M_PI/180.0f;
	return g_r;
}

//-----------------------------------------
// センサ依存部関数（非マイコン依存かつデータ取得でない関数）宣言
//-----------------------------------------
float GYRO_CLASS::Calculate_GYRO_TempCorrect(int i, float g_a) {
//
//  //0.3/100：SF誤差,　0.06/100：SF非線形誤差,　0.5/100*(g_t - 25.0)：SF温度特性,　1/6500*(g_t - 25.0)：SF正規化誤差
//  k_K_Vec[0] = (0.012f / 3.3f /3.0f)*(1.0f + 0.3/100 + 0.06/100 + (0.5/100 + 1/6500)*(g_t - 25.0)); //SFの補正(-)
//
//  //1.0f:Bias温度特性,　(3.0f/6500.0f)*(g_t - 25.0f):Bias正規化誤差,　0.14f：Bias再現性誤差
//	float k_Bias2 = ±1.0f ± (3.0f/6500.0f)*(g_t - 25.0f) ± 0.14f; //Bias2(deg/s)

    float g_r = (((k_gyro_vols_scale[i] * g_a - k_gyro_vols_bias[i]) - k_InputVol[i] * 0.5f)*(1.0f + 2.0f * M_PI * k_freq[i] * k_R[i] * k_C[i])/k_K[i] - gyro_offset_temp[i]) * k_gyro_scale[i] * gyro_scale_temp[i] *M_PI/180.0f;
    return g_r;
}

/**
  * @name   :Temperature_Compensation_SF
  * @brief  :Temperature_Compensation of SF
  * @param  :
  * @retval :float
  */
void GYRO_CLASS::Temperature_Compensation_SF(int i, float g_t){
	//g_t(global temperature) : g_sensor.p_adc->GYROX_TEMP or GYROY_TEMP or GYROZ_TEMP
	//±0.3/100：SF誤差,　±0.06/100：SF非線形誤差,　±0.5/100*(g_t - 25.0)：SF温度特性,　±1/6500*(g_t - 25.0)：SF正規化誤差
////gyro_scale_temp[i] = 1.0f + ±0.3f/100.0f + ±0.06f/100.0f + ±(0.5f/100.0f + 1.0f/6500.0f)*(g_t - 25.0f); //SFの補正(-)
	gyro_scale_temp[i] = 1.0f + mems_gyro_sf_t1[i]*(g_t - mems_gyro_sf_t0[i]) + mems_gyro_sf_t2[i];
}

/**
  * @name   :Temperature_Compensation_OFFSET
  * @brief  :Temperature_Compensation of OFFSET
  * @param  :
  * @retval :float
  */
void GYRO_CLASS::Temperature_Compensation_OFFSET(int i, float g_t){
	//g_t(global temperature) : g_sensor.p_adc->GYROX_TEMP or GYROY_TEMP or GYROZ_TEMP
	//±1.0f:Bias温度特性,　±(3.0f/6500.0f)*(g_t - 25.0f):Bias正規化誤差,　±0.14:Bias再現性誤差
////gyro_offset_temp[i] = ±1.0f + ±(3.0f/6500.0f)*(g_t - mems_gyro_os_t0[i]) + ±0.14f; //Bias2(deg/s)
	gyro_offset_temp[i] =  mems_gyro_os_t1[i]*(g_t - mems_gyro_os_t0[i]) + mems_gyro_os_t2[i]; //Bias2(deg/s)
}


//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

/**
  * @name   :GYRO_SENSOR_init
  * @brief  :GYROのレンジを初期設定する
  * @param  :
  * @retval :int
  */
int GYRO_CLASS::GYRO_SENSOR_init(uint8_t state[3])
{
	GPIO_Class gyro_reset(this->GYRO_RESET_Port, this->GYRO_RESET_Pin);
	GPIO_Class gyrox_sel0(this->GYROX_SEL0_Port, this->GYROX_SEL0_Pin);
	GPIO_Class gyrox_sel1(this->GYROX_SEL1_Port, this->GYROX_SEL1_Pin);
	GPIO_Class gyroy_sel0(this->GYROY_SEL0_Port, this->GYROY_SEL0_Pin);
	GPIO_Class gyroy_sel1(this->GYROY_SEL1_Port, this->GYROY_SEL1_Pin);
	GPIO_Class gyroz_sel0(this->GYROZ_SEL0_Port, this->GYROZ_SEL0_Pin);
	GPIO_Class gyroz_sel1(this->GYROZ_SEL1_Port, this->GYROZ_SEL1_Pin);

	// GYROXのレンジ設定
	switch(state[0]){
	case MEMS_GYRO_RANGE_75:
		gyro_reset.WriteH();
		gyrox_sel0.WriteH();
		gyrox_sel1.WriteH();
		this->k_K[0] = 0.012f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_150:
		gyro_reset.WriteH();
		gyrox_sel0.WriteL();
		gyrox_sel1.WriteH();
		this->k_K[0] = 0.006f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_300:
		gyro_reset.WriteH();
		gyrox_sel0.WriteH();
		gyrox_sel1.WriteL();
		this->k_K[0] = 0.003f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_900:
		gyro_reset.WriteH();
		gyrox_sel0.WriteL();
		gyrox_sel1.WriteL();
		this->k_K[0] = 0.001f * 3.3f / 3.0f;
		break;
	}

	// GYROYのレンジ設定
	switch(state[1]){
	case MEMS_GYRO_RANGE_75:
		gyro_reset.WriteH();
		gyroy_sel0.WriteH();
		gyroy_sel1.WriteH();
		this->k_K[1] = 0.012f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_150:
		gyro_reset.WriteH();
		gyroy_sel0.WriteL();
		gyroy_sel1.WriteH();
		this->k_K[1] = 0.006f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_300:
		gyro_reset.WriteH();
		gyroy_sel0.WriteH();
		gyroy_sel1.WriteL();
		this->k_K[1] = 0.003f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_900:
		gyro_reset.WriteH();
		gyroy_sel0.WriteL();
		gyroy_sel1.WriteL();
		this->k_K[1] = 0.001f * 3.3f / 3.0f;
		break;
	}

	// GYROZのレンジ設定
	switch(state[2]){
	case MEMS_GYRO_RANGE_75:
		gyro_reset.WriteH();
		gyroz_sel0.WriteH();
		gyroz_sel1.WriteH();
		this->k_K[2] = 0.012f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_150:
		gyro_reset.WriteH();
		gyroz_sel0.WriteL();
		gyroz_sel1.WriteH();
		this->k_K[2] = 0.006f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_300:
		gyro_reset.WriteH();
		gyroz_sel0.WriteH();
		gyroz_sel1.WriteL();
		this->k_K[2] = 0.003f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_900:
		gyro_reset.WriteH();
		gyroz_sel0.WriteL();
		gyroz_sel1.WriteL();
		this->k_K[2] = 0.001f * 3.3f / 3.0f;
		break;
	}
	return 0;
}

/**
  * @name   :GYRO_SENSOR_CHANGE_RANGE
  * @brief  :GYROのレンジを変更する
  * @param  :
  * @retval :int
  */
int GYRO_CLASS::GYRO_SENSOR_CHANGE_RANGE(uint8_t state[3])
{
	GPIO_Class gyro_reset(this->GYRO_RESET_Port, this->GYRO_RESET_Pin);
	GPIO_Class gyrox_sel0(this->GYROX_SEL0_Port, this->GYROX_SEL0_Pin);
	GPIO_Class gyrox_sel1(this->GYROX_SEL1_Port, this->GYROX_SEL1_Pin);
	GPIO_Class gyroy_sel0(this->GYROY_SEL0_Port, this->GYROY_SEL0_Pin);
	GPIO_Class gyroy_sel1(this->GYROY_SEL1_Port, this->GYROY_SEL1_Pin);
	GPIO_Class gyroz_sel0(this->GYROZ_SEL0_Port, this->GYROZ_SEL0_Pin);
	GPIO_Class gyroz_sel1(this->GYROZ_SEL1_Port, this->GYROZ_SEL1_Pin);

    gyro_reset.WriteL();

	// GYROXのレンジ設定
	switch(state[0]){
	case MEMS_GYRO_RANGE_75:
		gyrox_sel0.WriteH();
		gyrox_sel1.WriteH();
		this->k_K[0] = 0.012f * 3.3f / 3.0f;
//			if (g_sensor.p_pwr_IOEXPANDER->V_REG_CDH3V3 == 0) {
//				this->k_InputVol = 3.3f;
//				this->k_K = 0.012f * 3.3f / 3.0f;
//			}
//			else {
//				this->k_InputVol = g_sensor.p_pwr_IOEXPANDER->V_REG_CDH3V3;
//				this->k_K = 0.012f * this->k_InputVol / 3.0f;
//			}
		break;
	case MEMS_GYRO_RANGE_150:
		gyrox_sel0.WriteL();
		gyrox_sel1.WriteH();
		this->k_K[0] = 0.006f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_300:
		gyrox_sel0.WriteH();
		gyrox_sel1.WriteL();
		this->k_K[0] = 0.003f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_900:
		gyrox_sel0.WriteL();
		gyrox_sel1.WriteL();
		this->k_K[0] = 0.001f * 3.3f / 3.0f;
		break;
	}

	// GYROYのレンジ設定
	switch(state[1]){
	case MEMS_GYRO_RANGE_75:
		gyroy_sel0.WriteH();
		gyroy_sel1.WriteH();
		this->k_K[1] = 0.012f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_150:
		gyroy_sel0.WriteL();
		gyroy_sel1.WriteH();
		this->k_K[1] = 0.006f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_300:
		gyroy_sel0.WriteH();
		gyroy_sel1.WriteL();
		this->k_K[1] = 0.003f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_900:
		gyroy_sel0.WriteL();
		gyroy_sel1.WriteL();
		this->k_K[1] = 0.001f * 3.3f / 3.0f;
		break;
	}

	// GYROZのレンジ設定
	switch(state[2]){
	case MEMS_GYRO_RANGE_75:
		gyroz_sel0.WriteH();
		gyroz_sel1.WriteH();
		this->k_K[2] = 0.012f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_150:
		gyroz_sel0.WriteL();
		gyroz_sel1.WriteH();
		this->k_K[2] = 0.006f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_300:
		gyroz_sel0.WriteH();
		gyroz_sel1.WriteL();
		this->k_K[2] = 0.003f * 3.3f / 3.0f;
		break;
	case MEMS_GYRO_RANGE_900:
		gyroz_sel0.WriteL();
		gyroz_sel1.WriteL();
		this->k_K[2] = 0.001f * 3.3f / 3.0f;
		break;
	}
	gyro_reset.WriteH();

	printf("GYRO_RANGE X:%d Y:%d Z:%d\r\n", state[0], state[1], state[2]);

	return 0;
}


/**
  * @name   :Convert_Voltage2GYRO_MAGNARO
  * @brief  :出力電圧からGYROへ変換 モードフラグにより温度補正の有無を選択
  * @param  :i = 0->x 1->y 2->z g_a = アナログ電圧値  flag->0 温度補正なし　flag->1 温度補正あり　g_t = global temperature
  * @retval :float
  */
float GYRO_CLASS::Convert_Voltage2GYRO_MAGNARO(int i, float g_a, int flag, float g_t) {
	if (flag == 1){
		Temperature_Compensation_SF(i, g_t);
		Temperature_Compensation_OFFSET(i, g_t);
		float g_r = this->Calculate_GYRO_TempCorrect(i, g_a);
		return g_r;
	}
	else {
//		printf("k_Bias:%f\r\n", k_Bias);
		float g_r = this->Convert_Voltage2GYRO(i, g_a);
		return g_r;
	}
}

void GYRO_CLASS::GYRO_Vector_DCM_Sensor2Body_Calc(float omega[3], float DCM[3][3], float OMEGA[3])
{

	for (int i = 0; i < 3; i++)
	{
		OMEGA[i] = 0.0f;
		for (int j = 0; j < 3; j++) {
			OMEGA[i] += DCM[i][j] * omega[j];
		}
	}
}

void GYRO_CLASS::GYRO_SENSOR_get_time()
{
    gyrosensor_time_before = gyrosensor_time_now;
    gyrosensor_time_now = TMGR_get_master_clock();
}

#ifdef HILS
void GYRO_CLASS::GYRO_SENSOR_get_time_HILS()
{
    gyrosensor_time_before_hils = gyrosensor_time_now_hils;
    gyrosensor_time_now_hils = TMGR_get_master_clock();
}
#endif

#ifdef TWO_U
int GYRO_CLASS::GYRO_SENSOR_get_x()
{
	int ret = this->GYRO_SENSOR_AD7654_Get(1);


	 if(ret != 0){
		this->omega[0] = 0.0f;
		this->ADC_omegax = 0;
	 };

	return 0;
}

int GYRO_CLASS::GYRO_SENSOR_get_y()
{
	int ret = this->GYRO_SENSOR_AD7654_Get(2);

	 if(ret != 0){
		this->omega[1] = 0.0f;
		this->ADC_omegay = 0;
	 };

	return 0;
}
int GYRO_CLASS::GYRO_SENSOR_get_z()
{
	int ret = this->GYRO_SENSOR_AD7654_Get(3);

	 if(ret != 0){
		this->omega[2] = 0.0f;
		this->ADC_omegaz = 0;
	 };

	return 0;
}

int GYRO_CLASS::GYRO_SENSOR_get_all()
{
	this->GYRO_SENSOR_get_x();
	this->GYRO_SENSOR_get_y();
	this->GYRO_SENSOR_get_z();
	this->GYRO_SENSOR_get_time();

	return 0;
}

#endif/*for 2U*/
