/*
 * CURS.cpp
 *
 *  Created on: 2021/05/29
 *      Author: masud
 */
#ifdef TWO_U
#include <src_user/Drivers/Sensor/ADC.h>
#include <src_user/Drivers/Sensor/CURS.h>

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
  * @name   :GYRO_SENSOR_CRM100200_Get
  * @brief  :ADCでのアナログ値　デジタル値を求める
  * @param  :
  * @retval :
  */
int CURS_CLASS::CURS_SENSOR_AD7654_Get(int state)
{
	this->MUX_ANASEN(state);
	this->Select_ANASEN_Channel();
	this->ADC_GPIO_BUSY();
//	this->ADC_GPIO_All_Read(this->ADC_intData, this->ADC_Data);
	this->ADC_GPIO_All_Read();
	if(state == 10){
		this->ADC_curmtqz4 = this->ADC_intData;
		this->curmtqz4 = this->ADC_Data;
	}
	else if(state == 11){
		this->ADC_curmtqz3 = this->ADC_intData;
		this->curmtqz3 = this->ADC_Data;
	}
	else if(state == 12){
		this->ADC_curmtqz2 = this->ADC_intData;
		this->curmtqz2 = this->ADC_Data;
	}
	else if(state == 13){
		this->ADC_curmtqz1 = this->ADC_intData;
		this->curmtqz1 = this->ADC_Data;
	}
	else if(state == 14){
		this->ADC_curmtqy = this->ADC_intData;
		this->curmtqy = this->ADC_Data;
	}
	else if(state == 15){
		this->ADC_curmtqx = this->ADC_intData;
		this->curmtqx = this->ADC_Data;
	}
	else if(state == 16){

		// ハードウェアの不具合？で 0 の値のときに原因不明のピークが一定間隔で立つので
		// 回避するために複数回取得して最小値を利用
		float check_f = this->ADC_Data;
		uint16_t check_i = this->ADC_intData;

		for(int i=0; i<5; i++){
			this->ADC_GPIO_BUSY(); // ADC Conversion、BUSY が落ちるとデータレディ
			this->ADC_GPIO_All_Read(); // ADC データピン読み出し
			check_f = check_f < this->ADC_Data ? check_f : this->ADC_Data;
			check_i = check_i < this->ADC_intData ? check_i : this->ADC_intData;
		}

		this->ADC_curmtq2u = check_i;
		this->curmtq2u = check_f;
	}
    else if(state == 25){
        this->ADC_curtx = this->ADC_intData;
        this->curtx = this->ADC_Data;
    }

	return 0;
}
//-----------------------------------------
// センサ依存部関数　宣言
//-----------------------------------------



//-----------------------------------------
// センサ依存部関数（データ取得関数）宣言
//-----------------------------------------
/**
  * @name   :Calculate_Current
  * @brief  :Calculate Voltage to Current
  * @param  :
  * @retval :float
  */
float CURS_CLASS::Calculate_CURRENT(float g_a) {
    float g_r = (g_a - 2.5f) * 0.1f; //[A]
    return g_r;
}

//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

/**
 * @brief  サンセンサの初期化、CSピンをHighに設定し、データに0.0を格納。
 */
int CURS_CLASS::CURS_SENSOR_init()
{
	return 0;
}

int CURS_CLASS::CURS_SENSOR_get_MTQZ4()
{
	int ret = this->CURS_SENSOR_AD7654_Get(10);

	 if(ret != 0){
		this->curmtqz4 = 0.0f;
		this->ADC_curmtqz4 = 0;
	 };

	return 0;
}

int CURS_CLASS::CURS_SENSOR_get_MTQZ3()
{
	int ret = this->CURS_SENSOR_AD7654_Get(11);

	 if(ret != 0){
		this->curmtqz3 = 0.0f;
		this->ADC_curmtqz3 = 0;
	 };

	return 0;
}

int CURS_CLASS::CURS_SENSOR_get_MTQZ2()
{
	int ret = this->CURS_SENSOR_AD7654_Get(12);

	 if(ret != 0){
		this->curmtqz2 = 0.0f;
		this->ADC_curmtqz2 = 0;
	 };

	return 0;
}

int CURS_CLASS::CURS_SENSOR_get_MTQZ1()
{
	int ret = this->CURS_SENSOR_AD7654_Get(13);

	 if(ret != 0){
		this->curmtqz1 = 0.0f;
		this->ADC_curmtqz1 = 0;
	 };

	return 0;
}

int CURS_CLASS::CURS_SENSOR_get_MTQY()
{
	int ret = this->CURS_SENSOR_AD7654_Get(14);

	 if(ret != 0){
		this->curmtqy = 0.0f;
		this->ADC_curmtqy = 0;
	 };

	return 0;
}

int CURS_CLASS::CURS_SENSOR_get_MTQX()
{
	int ret = this->CURS_SENSOR_AD7654_Get(15);

	 if(ret != 0){
		this->curmtqx = 0.0f;
		this->ADC_curmtqx = 0;
	 };

	return 0;
}

int CURS_CLASS::CURS_SENSOR_get_MTQ2U()
{
	int ret = this->CURS_SENSOR_AD7654_Get(16);

	 if(ret != 0){
		this->curmtq2u = 0.0f;
		this->ADC_curmtq2u = 0;
	 };

	return 0;
}

int CURS_CLASS::CURS_SENSOR_get_TX()
{
    int ret = this->CURS_SENSOR_AD7654_Get(25);

     if(ret != 0){
        this->curmtq2u = 0.0f;
        this->ADC_curmtq2u = 0;
     };

    return 0;
}

int CURS_CLASS::CURS_SENSOR_get_all()
{
	this->CURS_SENSOR_get_MTQZ4();
	this->CURS_SENSOR_get_MTQZ3();
	this->CURS_SENSOR_get_MTQZ2();
	this->CURS_SENSOR_get_MTQZ1();
	this->CURS_SENSOR_get_MTQY();
	this->CURS_SENSOR_get_MTQX();
	this->CURS_SENSOR_get_MTQ2U();
	this->CURS_SENSOR_get_TX();

	return 0;
}

#endif/*for 2U*/

