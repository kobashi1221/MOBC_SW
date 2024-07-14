/*
 * PlatinumSensor.cpp
 *
 *  Created on: 2021/05/27
 *      Author: masud
 */
#ifdef TWO_U
#include "ADC.h"
#include "PlatinumSensor.h"

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Library/Delay.h>


void PS_Delay(uint16_t ms){
	//todo implement MTQ_Delay
}

//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------
//======MRAM格納パラメータの実体宣言部======//
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS BEGIN *** //
constexpr float PLATINUM_CLASS::k_AppliedCurrent_; //frequency of the rate of rotation
constexpr float PLATINUM_CLASS::PT_TEMP_THRESHOLD_;  // PT Temperature Threshold
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS END*** //

float k_A  = 3.9083f*0.001f;
float k_B  = -5.775f*0.0000001f;

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
int PLATINUM_CLASS::PLATINUM_SENSOR_AD7654_Get(int state)
{
////	printf("state:%d\r\n", state);
//	this->MUX_PtTEMS(state);
//	c2a_delay_ms(20);
	this->Select_PtTems_Channel();
	this->ADC_GPIO_BUSY();
//	for(int i = 0; i <= 5; i++){
//		this->ADC_GPIO_All_Read(this->ADC_intData, this->ADC_Data);
		this->ADC_GPIO_All_Read();
//	}
//	printf("Pt_ADC is %f\r\n", this->ADC_Data);
	if(state == 1){
		this->ADC_PtMOBC = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtMOBC = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtMOBC = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 2){
		this->ADC_PtADC = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtADC = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtADC = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 3){
		this->ADC_PtMAGS = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtMAGS = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtMAGS = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 4){
		this->ADC_PtGPS = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtGPS = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtGPS = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 5){
		this->ADC_Pt9AX = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->Pt9AX = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->Pt9AX = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 6){
		this->ADC_PtLENSPX = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtLENSPX = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtLENSPX = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 7){
		this->ADC_PtLENSPY = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtLENSPY = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtLENSPY = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 8){
		this->ADC_PtIMAGE = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtIMAGE = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtIMAGE = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 9){
		this->ADC_PtLENSMX = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtLENSMX = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtLENSMX = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 10){
		this->ADC_PtGYROX = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtGYROX = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtGYROX = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 11){
		this->ADC_PtGYROY = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtGYROY = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtGYROY = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 12){
		this->ADC_PtGYROZ = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtGYROZ = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtGYROZ = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 13){
		this->ADC_PtMTQZ1 = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtMTQZ1 = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtMTQZ1 = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 14){
		this->ADC_PtMTQZ2 = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtMTQZ2 = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtMTQZ2 = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 15){
		this->ADC_PtMTQZ3 = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtMTQZ3 = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtMTQZ3 = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}
	else if(state == 16){
		this->ADC_PtMTQZ4 = ADC_CLASS::ADC_intData;
		if(this->ADC_Data >= 3.0f){
			this->PtMTQZ4 = this->Calculate_Pt_Plus(this->ADC_Data);
		}
		else{
			this->PtMTQZ4 = this->Calculate_Pt_Minus(this->ADC_Data);
		}
	}

	if (state == 16){
		this->MUX_PtTEMS(1); //多分1で合ってると思うけどMAGNAROでは0になってた
	}
	else {
		this->MUX_PtTEMS(state + 1);
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
  * @name   :Calculate_Pt_Plus Temperature
  * @brief  :Calculate Voltage to Pt
  * @param  :
  * @retval :float
  */
float PLATINUM_CLASS::Calculate_Pt_Plus(float g_b) {
    float g_tp = ((- k_A + sqrt(k_A*k_A - 4.0f*k_B*(1.0f - (g_b/(k_AppliedCurrent*10.0f)))))*0.5f)/k_B; //tamaoki edit
//    printf("TEMP %f\r\n", g_tp);
    return g_tp;
}

/**
  * @name   :Calculate_Pt_Minus_Temperature
  * @brief  :Calculate Voltage to Pt
  * @param  :
  * @retval :float
  */
float PLATINUM_CLASS::Calculate_Pt_Minus(float g_c) {
    float g_tm = 84.632f*g_c - 253.96f;
    return g_tm;
}


//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------

/**
 * @brief  サンセンサの初期化、CSピンをHighに設定し、データに0.0を格納。
 */
int PLATINUM_CLASS::PLATINUM_SENSOR_init()
{
	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_MOBC()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(1);

	 if(ret != 0){
		this->PtMOBC = 0.0f;
		this->ADC_PtMOBC = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_ADC()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(2);

	 if(ret != 0){
		this->PtADC = 0.0f;
		this->ADC_PtADC = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_MAGS()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(3);

	 if(ret != 0){
		this->PtMAGS = 0.0f;
		this->ADC_PtMAGS = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_GPS()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(4);

	 if(ret != 0){
		this->PtGPS = 0.0f;
		this->ADC_PtGPS = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_9AX()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(5);

	 if(ret != 0){
		this->Pt9AX = 0.0f;
		this->ADC_Pt9AX = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_LENSPX()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(6);

	 if(ret != 0){
		this->PtLENSPX = 0.0f;
		this->ADC_PtLENSPX = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_LENSPY()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(7);

	 if(ret != 0){
		this->PtLENSPY = 0.0f;
		this->ADC_PtLENSPY = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_IMAGE()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(8);

	 if(ret != 0){
		this->PtIMAGE = 0.0f;
		this->ADC_PtIMAGE = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_LENSMX()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(9);

	 if(ret != 0){
		this->PtLENSMX = 0.0f;
		this->ADC_PtLENSMX = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_GYROX()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(10);

	 if(ret != 0){
		this->PtGYROX = 0.0f;
		this->ADC_PtGYROX = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_GYROY()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(11);

	 if(ret != 0){
		this->PtGYROX = 0.0f;
		this->ADC_PtGYROX = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_GYROZ()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(12);

	 if(ret != 0){
		this->PtGYROX = 0.0f;
		this->ADC_PtGYROX = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_MTQZ1()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(13);

	 if(ret != 0){
		this->PtMTQZ1 = 0.0f;
		this->ADC_PtMTQZ1 = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_MTQZ2()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(14);

	 if(ret != 0){
		this->PtMTQZ2 = 0.0f;
		this->ADC_PtMTQZ2 = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_MTQZ3()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(15);

	 if(ret != 0){
		this->PtMTQZ3 = 0.0f;
		this->ADC_PtMTQZ3 = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_MTQZ4()
{
	int ret = this->PLATINUM_SENSOR_AD7654_Get(16);

	 if(ret != 0){
		this->PtMTQZ4 = 0.0f;
		this->ADC_PtMTQZ4 = 0;
	 };

	return 0;
}

int PLATINUM_CLASS::PLATINUM_SENSOR_get_all()
{
	this->PLATINUM_SENSOR_get_MOBC();
	this->PLATINUM_SENSOR_get_ADC();
	this->PLATINUM_SENSOR_get_MAGS();
	this->PLATINUM_SENSOR_get_GPS();
	this->PLATINUM_SENSOR_get_9AX();
	this->PLATINUM_SENSOR_get_LENSPX();
	this->PLATINUM_SENSOR_get_LENSPY();
	this->PLATINUM_SENSOR_get_IMAGE();
	this->PLATINUM_SENSOR_get_LENSMX();
	this->PLATINUM_SENSOR_get_GYROX();
	this->PLATINUM_SENSOR_get_GYROY();
	this->PLATINUM_SENSOR_get_GYROZ();
	this->PLATINUM_SENSOR_get_MTQZ1();
	this->PLATINUM_SENSOR_get_MTQZ2();
	this->PLATINUM_SENSOR_get_MTQZ3();
	this->PLATINUM_SENSOR_get_MTQZ4();

	return 0;
}

#endif /**/
