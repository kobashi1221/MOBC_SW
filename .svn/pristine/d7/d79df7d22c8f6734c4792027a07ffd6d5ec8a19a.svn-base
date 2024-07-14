/*
 * VOLS.cpp
 *
 *  Created on: 2021/05/29
 *      Author: masud
 */
#ifdef TWO_U

#include <src_user/Drivers/Sensor/ADC.h>
#include <src_user/Drivers/Sensor/VOLS.h>

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Library/Delay.h>

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
int VOLS_CLASS::VOLS_SENSOR_AD7654_Get(int state)
{

	this->MUX_ANASEN(state); // MUX チャンネル設定
	this->Select_ANASEN_Channel(); // ADC チャンネル設定
	this->ADC_GPIO_BUSY(); // ADC Conversion、BUSY が落ちるとデータレディ
	this->ADC_GPIO_All_Read(); // ADC データピン読み出し
	if(state == 17){ // MAGNARO-ll:17, MAGNARO:18
		// ハードウェアの不具合？で 0 の値のときに原因不明のピークが一定間隔で立つので
		// 回避するために複数回取得して最小値を利用
		uint16_t check_i = this->ADC_intData;
		float check_f = this->ADC_Data;

//		printf("before for check_f %f\r\n", check_f);
//		printf("before for check_i %d\r\n", check_i);

		// 5 サイクル 0.1ms くらい
		for(int i=0; i<5; i++){
			this->ADC_GPIO_BUSY(); // ADC Conversion、BUSY が落ちるとデータレディ
			this->ADC_GPIO_All_Read(); // ADC データピン読み出し
			check_i = check_i < this->ADC_intData ? check_i : this->ADC_intData;
			check_f = check_f < this->ADC_Data ? check_f : this->ADC_Data;
//			printf("check_f %f\r\n", check_f);
//			printf("check_i %d\r\n", check_i);
		}

		this->ADC_volmtqa = check_i;
		this->volmtqa = check_f;

//		printf("volmtqa %d\r\n", this->volmtqa);
//		printf("ADC_volmtqa %d\r\n", this->ADC_volmtqa);
	}
	else if(state == 18){ // MAGNARO-ll:18, MAGNARO:19

		// ハードウェアの不具合？で 0 の値のときに原因不明のピークが一定間隔で立つので
		// 回避するために複数回取得して最小値を利用
		uint16_t check_i = this->ADC_intData;
		float check_f = this->ADC_Data;

//		printf("before for check_f %f\r\n", check_f);
//		printf("before for check_i %d\r\n", check_i);

		for(int i=0; i<5; i++){
			this->ADC_GPIO_BUSY(); // ADC Conversion、BUSY が落ちるとデータレディ
			this->ADC_GPIO_All_Read(); // ADC データピン読み出し
			check_i = check_i < this->ADC_intData ? check_i : this->ADC_intData;
			check_f = check_f < this->ADC_Data ? check_f : this->ADC_Data;
//			check_i += this->ADC_intData;
//			check_f += this->ADC_Data;
//			printf("check_i %d\r\n", check_i);
//			printf("check_f %f\r\n", check_f);
		}

//		this->ADC_volmtqo = (uint16_t)check_i/i;
//		this->volmtqo = (float)check_f/ i;
		this->ADC_volmtqo = check_i;
		this->volmtqo = check_f;

//		printf("volmtqo %d\r\n", this->volmtqo);
//		printf("ADC_volmtqo %d\r\n", this->ADC_volmtqo);
//		printf("VOLS.cpp this->ADC_intData is %d\r\n", this->ADC_intData);
//		printf("VOLS.cpp ADC_CLASS::ADC_intData is %d\r\n", ADC_CLASS::ADC_intData);
//		printf("VOLS.cpp VOLS_CLASS::ADC_intData is %d\r\n", VOLS_CLASS::ADC_intData);
//		printf("VOLS.cpp this->VOLS_CLASS::ADC_intData is %d\r\n", this->ADC_CLASS::ADC_intData);
	}

	return 0;
//	c2a_delay_ms(50);
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
int VOLS_CLASS::VOLS_SENSOR_init()
{
	return 0;
}

int VOLS_CLASS::VOLS_SENSOR_get_MTQA()
{
	int ret = this->VOLS_SENSOR_AD7654_Get(17); // MAGNARO-ll:17, MAGNARO:18

	 if(ret != 0){
		this->volmtqa = 0.0f;
		this->ADC_volmtqa = 0;
	 };

	return 0;
}

int VOLS_CLASS::VOLS_SENSOR_get_MTQO()
{
	int ret = this->VOLS_SENSOR_AD7654_Get(18); // MAGNARO-ll:18, MAGNARO:19

	 if(ret != 0){
		this->volmtqo = 0.0f;
		this->ADC_volmtqo = 0;
	 };

	return 0;
}

int VOLS_CLASS::VOLS_SENSOR_get_all()
{
	this->VOLS_SENSOR_get_MTQA();
	this->VOLS_SENSOR_get_MTQO();

	return 0;
}

#endif



