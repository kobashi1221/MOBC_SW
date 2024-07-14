///*
// * cSUNS.cpp
// *
// *  Created on: 2021/11/25
// *      Author: masud
// */
//#include <src_user/Drivers/Aocs/AOCScalc.h>
//#include <src_user/Drivers/SENSOR/cSUNS.h>
//#include <src_user/Library/Delay.h>
//#include "DBG_printf.h"
//
//#ifdef ONE_U
//
//// Delay関数定義
//#define SUNS_Delay SUNS_Delay
//#define SPI_DMA_ENABLED 1
//
//#include <string.h>
//#include <math.h>
//
//
////-----------------------------------------
//// センサ依存変数宣言
////-----------------------------------------
////#defineしても良いため大文字で記述しておく
//
//
////======MRAM格納パラメータの実体宣言部======//
//// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS BEGIN *** //
//constexpr float CSUNS_CLASS::peak_;      // max solar energy
//constexpr float CSUNS_CLASS::CSS_threshold_;
//// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS END*** //
//
////-----------------------------------------
//// センサ依存変数宣言（センサ動作させるために必要だが、汎用的な関数をここに定義しておく）
////-----------------------------------------
//
//
////-----------------------------------------
//// センサクラス定義
////-----------------------------------------
//
////-----------------------------------------
//// センサ依存部関数（マイコン依存関数のラッパー）定義
////-----------------------------------------
//
//
////-----------------------------------------
//// センサ依存部関数　宣言
////-----------------------------------------
//
//
//
////-----------------------------------------
//// センサ依存部関数（データ取得関数）宣言
////-----------------------------------------
//int CSUNS_CLASS::CSS_S1600833_GET(float &output){
//
//	SPI_Class cSUN(this->spi_handler, this->gpio_port, this->gpio_pin, SUN_SENSOR_SPI_TIMEOUT);
////		printf("Get CSS \r\n");
//
//	uint8_t spi4_buf_[2] = {0,0};
//	uint8_t spi4_dum_[2];
//
////	cSUN.WriteH();
////	HAL_SPI_Receive(&hspi4,spi4_buf_,2,1000);
////	cSUN.WriteL();
//
//	int ret = cSUN.Receive(spi4_buf_, 2);
//
//	if(ret != 0) {
//		return this->state = CHECKSUM_ERROR;
//	}
//
////	printf(" %u,%u\r\n",spi4_buf_[0],spi4_buf_[1]);
//	css_val_ = ((spi4_buf_[0] & 0x0F) << 8) | spi4_buf_[1];
////	print(SUNS,"css_val = %u \r\n", css_val_);
//
//	float css_convert_ = (float)css_val_ / 4096.0f * 3000.0f;
//
////	print(SUNS,"css_convert = %.2f \r\n", css_convert_);
//
//	if (css_convert_ >= peak) { // peakは最大3000mV
//		 peak = css_convert_;
//	}
//
////	print(SUNS,"peak = %.2f \r\n", peak);
//
//	//not to divide by zero
//	//make  acos(css_convert_ / peak )  90 deg    _taguchi
//    if(peak == 0){
//        peak += 1e5;
//    }
//	output = acos(css_convert_ / peak ) * 180.0f / M_PI;
//
//	print(SUNS,"output = %.2f [deg]\r\n", output);
//
//	return this->state = NO_ERROR;
//}
//
//
//
////-----------------------------------------
//// 衛星ソフトウェア依存関数宣言
////-----------------------------------------
//int CSUNS_CLASS::CSS_init(){
//	WriteH();
//
//	SPI_Class cSUN(this->spi_handler, this->gpio_port, this->gpio_pin, SUN_SENSOR_SPI_TIMEOUT);
//
//	uint8_t spi4_buf_[2] = {0,0};
//	cSUN.Receive(spi4_buf_, 2);
//
//	uint16_t css_val_ = ((spi4_buf_[0] & 0x0F) << 8) | spi4_buf_[1];
////	print(SUNS,"css_val = %u \r\n", css_val_);
//
//	float css_convert_ = (float)css_val_ / 4096.0f * 3000.0f;
//
//	if (css_convert_ >= peak) { // peakは最大3000mV
//		 peak = css_convert_;
//	}
//
//	return this->state = NO_ERROR;
//
//}
//
//int CSUNS_CLASS::CSS_get_all(){
//
//	if (this->CSS_S1600833_GET(this->theta)){
//		return this->state;
//	}
//	this->CSUN_SENSOR_get_time();
//
//	return NO_ERROR;
//}
//
///**
//  * @name   :CSUNS_Vector_Calc
//  * @brief  :θから正規化されたベクトルを求める
//  * @param  :
//  * @retval :
//  */
//int CSUNS_CLASS::CSS_Vector_Calc(float thetaPX, float thetaMY, float thetaPZ)
//{
//	float PX = cos(thetaPX * M_PI / 180.0f);
//	float MY = cos(thetaMY * M_PI / 180.0f);
//	float PZ = cos(thetaPZ * M_PI / 180.0f);
//	float norm = sqrt(PX * PX + MY * MY + PZ * PZ);
//
////	printf("PX, MY, PZ is %f, %f, %f \r\n", PX, MY, PZ);
//
//	// アルベドを考慮した閾値より絶対値が小さい時はPX,MY,PZ面には太陽光が当たっていないとする
//	if ( norm > CSS_threshold ) {
//		CSUNSensorVec[0] = PX / norm;
//		CSUNSensorVec[1] = - MY / norm;
//		CSUNSensorVec[2] = PZ / norm;
//
//		return 0;
//	}
//	else {
//		CSUNSensorVec[0] = 0.0f;
//		CSUNSensorVec[1] = 0.0f;
//		CSUNSensorVec[2] = 0.0f;
//
//		this->state = ALBEDO_EARTH_ERROR; //アルベドアノマリー
//
//		return 1;
//	}
//
//}
//
//void CSUNS_CLASS::CSUN_SENSOR_get_time()
//{
//    csunsensor_time_before = csunsensor_time_now;
//    csunsensor_time_now = TMGR_get_master_clock();
//}
//
//#endif
