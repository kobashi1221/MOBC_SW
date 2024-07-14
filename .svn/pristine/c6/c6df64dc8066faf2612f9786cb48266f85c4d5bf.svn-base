/*
 * PLASMA.cpp
 *
 *  Created on: 2021/07/03
 *      Author: masud
 */

#ifdef TWO_U

#include <src_user/Drivers/Sensor/ADC.h>
#include <src_user/Drivers/Sensor/PLASMA.h>

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>


//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------


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
int PLASMA_CLASS::PLASMA_SENSOR_AD7654_Get(int state)
{
	this->MUX_ANASEN(state);
	this->Select_ANASEN_Channel();
	this->ADC_GPIO_BUSY();
//	this->ADC_GPIO_All_Read(this->ADC_intData, this->ADC_Data);
	this->ADC_GPIO_All_Read();
	if(state == 19){
		this->PLASMA1 = this->ADC_Data;
		this->ADC_PLASMA1 = this->ADC_intData;
	}
	else if(state == 20){
		this->PLASMA2 = this->ADC_Data;
		this->ADC_PLASMA2 = this->ADC_intData;
	}
	else if(state == 21){
		this->PLASMA3 = this->ADC_Data;
		this->ADC_PLASMA3 = this->ADC_intData;
	}
	else if(state == 22){
		this->PLASMA4 = this->ADC_Data;
		this->ADC_PLASMA4 = this->ADC_intData;
	}
	else if(state == 23){
		this->PLASMA5 = this->ADC_Data;
		this->ADC_PLASMA5 =this->ADC_intData;
	}
	else if(state == 24){
		this->PLASMA6 = this->ADC_Data;
		this->ADC_PLASMA6 = this->ADC_intData;
	}
	return 0;
}
//-----------------------------------------
// センサ依存部関数　宣言
//-----------------------------------------



//-----------------------------------------
// センサ依存部関数（データ取得関数）宣言
//-----------------------------------------


//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

/**
 * @brief  サンセンサの初期化、CSピンをHighに設定し、データに0.0を格納。
 */
int PLASMA_CLASS::PLASMA_SENSOR_init()
{
	return 0;
}

int PLASMA_CLASS::PLASMA_SENSOR_get_PLASMA1()
{
	int ret = this->PLASMA_SENSOR_AD7654_Get(19);

	 if(ret != 0){
		this->PLASMA1 = 0.0f;
		this->ADC_PLASMA1 = 0;
	 };

	return 0;
}

int PLASMA_CLASS::PLASMA_SENSOR_get_PLASMA2()
{
	int ret = this->PLASMA_SENSOR_AD7654_Get(20);

	 if(ret != 0){
		this->PLASMA2 = 0.0f;
		this->ADC_PLASMA2 = 0;
	 };

	return 0;
}

int PLASMA_CLASS::PLASMA_SENSOR_get_PLASMA3()
{
	int ret = this->PLASMA_SENSOR_AD7654_Get(21);

	 if(ret != 0){
		this->PLASMA3 = 0.0f;
		this->ADC_PLASMA3 = 0;
	 };

	return 0;
}

int PLASMA_CLASS::PLASMA_SENSOR_get_PLASMA4()
{
	int ret = this->PLASMA_SENSOR_AD7654_Get(22);

	 if(ret != 0){
		this->PLASMA4 = 0.0f;
		this->ADC_PLASMA4 = 0;
	 };

	return 0;
}

int PLASMA_CLASS::PLASMA_SENSOR_get_PLASMA5()
{
	int ret = this->PLASMA_SENSOR_AD7654_Get(23);

	 if(ret != 0){
		this->PLASMA5 = 0.0f;
		this->ADC_PLASMA5 = 0;
	 };

	return 0;
}

int PLASMA_CLASS::PLASMA_SENSOR_get_PLASMA6()
{
	int ret = this->PLASMA_SENSOR_AD7654_Get(24);

	 if(ret != 0){
		this->PLASMA6 = 0.0f;
		this->ADC_PLASMA6 = 0;
	 };

	return 0;
}

int PLASMA_CLASS::PLASMA_SENSOR_get_all()
{
	this->PLASMA_SENSOR_get_PLASMA1();
	this->PLASMA_SENSOR_get_PLASMA2();
	this->PLASMA_SENSOR_get_PLASMA3();
	this->PLASMA_SENSOR_get_PLASMA4();
	this->PLASMA_SENSOR_get_PLASMA5();
	this->PLASMA_SENSOR_get_PLASMA6();

	return 0;
}

#endif


