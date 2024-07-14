/**
  *****************************************************************************************
  * @file    : ADC.cpp
  * @author  : Satoshi NAKAYAMA
  * @version : 1.0
  * @date    : 2020/10/15
  * @brief   : ACS1 ADCとのインターフェースモジュール群
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/10/15  1.0   S.NAKAYAMA      First revision
  *
  *****************************************************************************************
  */
#ifdef TWO_U
#include <src_user/Drivers/SENSOR/ADC.h>
#include <src_user/Library/Delay.h>
#include <src_user/Applications/UserDefined/Sensor/ACS1_cmd_ADC.h>

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>
//#include <main.h>

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
  * @name   :ADC_GPIO_BUSY
  * @brief  :Read ADC's BUSY Pin
  * @param  :
  * @retval :void
  */

void ADC_Delay(uint16_t ms){
	//todo implement ADC_Delay
}


void ADC_CLASS::ADC_GPIO_BUSY()
{

	GPIO_Class CNV(this->CNVST_Port, this->CNVST_Pin);
	CNV.WriteL();

	bool Busy;

	// BUSY がHighになってからCNVをHighに戻しておく
	GPIO_Class BUSY(this->BUSY_Port, this->BUSY_Pin);
	for(int i = 0; i < 1000; i++){
		Busy = BUSY.ReadPin();
		if (Busy) break;
	}
//	printf("Busy?: %d, ", BUSY.ReadPin());
	CNV.WriteH();


//	GPIO_Class CNV(this->CNVST_Port, this->CNVST_Pin);
//
//	//check that CNVST is high
//	CNV.WriteH();
//	bool Busy;
//
//	//check if busy pin is low before initiating
//	GPIO_Class BUSY(this->BUSY_Port, this->BUSY_Pin);
//	for(uint16_t i = 0; i < 50000; i++){
//		Busy = BUSY.ReadPin();
//		if (!Busy) break;
//	}
//
//	//initiate
//	CNV.WriteL();
//	for(int i = 0; i < 1000; i++){
//		//do nothing at least 5 ns
//	}
//
//	// BUSY がHighになってからCNVをHighに戻しておく
//	CNV.WriteH();
//
//	//wait for conversion to finish
//	for(uint16_t i = 0; i < 50000; i++){
//		Busy = BUSY.ReadPin();
//		if (!Busy) break;  //check if busy pin is low
//	}
//	//continue

}

void ADC_CLASS::Select_ANASEN_Channel()
{
	GPIO_Class ADC_A0_ANASEN(this->ADC_A0_Port, this->ADC_A0_Pin);
	ADC_A0_ANASEN.WriteL();
//	c2a_delay_ms(10);
	GPIO_Class AB_ANASEN(this->AB_Port, this->AB_Pin);
	AB_ANASEN.WriteH();
//	c2a_delay_ms(10);
}

void ADC_CLASS::Select_PtTems_Channel()
{
	GPIO_Class ADC_A0_Pt(this->ADC_A0_Port, this->ADC_A0_Pin);
	ADC_A0_Pt.WriteL();
//	c2a_delay_ms(10);
	GPIO_Class AB_Pt(this->AB_Port, this->AB_Pin);
	AB_Pt.WriteL();
//	c2a_delay_ms(10);
}

void ADC_CLASS::MUX_ANASEN(int c_acs1)
{
	int buff_Channel;

	buff_Channel = c_acs1;
//	printf("buff_Channel:%d\r\n", buff_Channel);

//	c2a_delay_ms(3);

	//指定したMUXのCTLピンをLowにする
	//GPIO_Class gpio_class(CTL_OD5V_MOBCtoMUX_ANAS_GPIO_Port, CTL_OD5V_MOBCtoMUX_ANAS_Pin);
	GPIO_Class gpio_CTL(this->CTL_ANASEN_Port, this->CTL_ANASEN_Pin);
	gpio_CTL.WriteL();

	c2a_delay_ms(1); // tamura tested OK

	//指定したチャンネルに合わせてA0~A4ピンを選択する
	buff_Channel = buff_Channel-1;

	GPIO_Class gpio_A0(this->A0_Port, this->A0_Pin);
	GPIO_Class gpio_A1(this->A1_Port, this->A1_Pin);
	GPIO_Class gpio_A2(this->A2_Port, this->A2_Pin);
	GPIO_Class gpio_A3(this->A3_Port, this->A3_Pin);
	GPIO_Class gpio_A4(this->A4_Port, this->A4_Pin);

	if (buff_Channel%2) {gpio_A0.WriteH();} else {gpio_A0.WriteL();}
	if ((buff_Channel/2)%2) {gpio_A1.WriteH();} else {gpio_A1.WriteL();}
	if ((buff_Channel/4)%2) {gpio_A2.WriteH();} else {gpio_A2.WriteL();}
	if ((buff_Channel/8)%2) {gpio_A3.WriteH();} else {gpio_A3.WriteL();}
	if ((buff_Channel/16)%2) {gpio_A4.WriteH();} else {gpio_A4.WriteL();}

	c2a_delay_ms(1); // tamura tested OK

	//CTLピンをHIGHに戻してチャンネルを固定する
	gpio_CTL.WriteH();

//	c2a_delay_ms(3);

}

void ADC_CLASS::MUX_PtTEMS(int c_acs1)
{
	int buff_Channel;

	buff_Channel = c_acs1;
//	printf("(Next state is %d)\r\n", buff_Channel);

	//指定したMUXのCTLピンをLowにする
	GPIO_Class gpio_CTL(this->CTL_TEMS_Port, this->CTL_TEMS_Pin);
	gpio_CTL.WriteL();

	//c2a_delay_ms(10); // tamaoki edit
	c2a_delay_ms(1); //tamura tested

	//指定したチャンネルに合わせてA0~A4ピンを選択する
	buff_Channel = buff_Channel-1;

	GPIO_Class gpio_A0(this->A0_Port, this->A0_Pin);
	GPIO_Class gpio_A1(this->A1_Port, this->A1_Pin);
	GPIO_Class gpio_A2(this->A2_Port, this->A2_Pin);
	GPIO_Class gpio_A3(this->A3_Port, this->A3_Pin);
	GPIO_Class gpio_A4(this->A4_Port, this->A4_Pin);

	if (buff_Channel%2) {gpio_A0.WriteH();} else {gpio_A0.WriteL();}
	if ((buff_Channel/2)%2) {gpio_A1.WriteH();} else {gpio_A1.WriteL();}
	if ((buff_Channel/4)%2) {gpio_A2.WriteH();} else {gpio_A2.WriteL();}
	if ((buff_Channel/8)%2) {gpio_A3.WriteH();} else {gpio_A3.WriteL();}
	if ((buff_Channel/16)%2) {gpio_A4.WriteH();} else {gpio_A4.WriteL();}

	//c2a_delay_ms(3); // tamaoki edit
	c2a_delay_ms(1); //tamura tested

	//CTLピンをHIGHに戻してチャンネルを固定する
	gpio_CTL.WriteH();

	//c2a_delay_ms(3); //tamaoki edit //c2a_delay_ms(1);
	c2a_delay_ms(8); // tamura tested

}
//-----------------------------------------
// センサ依存部関数　宣言
//-----------------------------------------



//-----------------------------------------
// センサ依存部関数（データ取得関数）宣言
//-----------------------------------------
/**
  * @name   :ADC_GPIO_ALL_Read
  * @brief  :Read all ADC's GPIO
  * @param  :
  * @retval :void
  */
//int ADC_CLASS::ADC_GPIO_All_Read(uint16_t ADC_intData, float ADC_Data)
//int ADC_CLASS::ADC_GPIO_All_Read(ADC_CLASS* sub_instance)
int ADC_CLASS::ADC_GPIO_All_Read(void)
{
	GPIO_Class gpio_BYTE(this->BYTESWAP_Port, this->BYTESWAP_Pin);
	gpio_BYTE.WriteL();

	bool ADC_getData[16];

	GPIO_Class gpio_D0(this->D0_Port, this->D0_Pin);
	ADC_getData[0] = gpio_D0.ReadPin();
	GPIO_Class gpio_D1(this->D1_Port, this->D1_Pin);
	ADC_getData[1] = gpio_D1.ReadPin();
	GPIO_Class gpio_D2(this->D2_Port, this->D2_Pin);
	ADC_getData[2] = gpio_D2.ReadPin();
	GPIO_Class gpio_D3(this->D3_Port, this->D3_Pin);
	ADC_getData[3] = gpio_D3.ReadPin();
	GPIO_Class gpio_D4(this->D4_Port, this->D4_Pin);
	ADC_getData[4] = gpio_D4.ReadPin();
	GPIO_Class gpio_D5(this->D5_Port, this->D5_Pin);
	ADC_getData[5] = gpio_D5.ReadPin();
	GPIO_Class gpio_D6(this->D6_Port, this->D6_Pin);
	ADC_getData[6] = gpio_D6.ReadPin();
	GPIO_Class gpio_D7(this->D7_Port, this->D7_Pin);
	ADC_getData[7] = gpio_D7.ReadPin();
	GPIO_Class gpio_D8(this->D8_Port, this->D8_Pin);
	ADC_getData[8] = gpio_D8.ReadPin();
	GPIO_Class gpio_D9(this->D9_Port, this->D9_Pin);
	ADC_getData[9] = gpio_D9.ReadPin();
	GPIO_Class gpio_D10(this->D10_Port, this->D10_Pin);
	ADC_getData[10] = gpio_D10.ReadPin();
	GPIO_Class gpio_D11(this->D11_Port, this->D11_Pin);
	ADC_getData[11] = gpio_D11.ReadPin();
	GPIO_Class gpio_D12(this->D12_Port, this->D12_Pin);
	ADC_getData[12] = gpio_D12.ReadPin();
	GPIO_Class gpio_D13(this->D13_Port, this->D13_Pin);
	ADC_getData[13] = gpio_D13.ReadPin();
	GPIO_Class gpio_D14(this->D14_Port, this->D14_Pin);
	ADC_getData[14] = gpio_D14.ReadPin();
	GPIO_Class gpio_D15(this->D15_Port, this->D15_Pin);
	ADC_getData[15] = gpio_D15.ReadPin();

	//D0~D15 change to Decimal number
	uint16_t intData = 0;
	int a = 1;
	for (int i = 0; i < 16; i ++){
		intData += a*ADC_getData[i];
		a =  a*2;
	}
	this->ADC_intData = intData;

	//Analog conversion
	this->ADC_Data = 5.0f * this->ADC_intData * 1.5258789062f * 0.00001f;

//    printf("ADC.cpp ADC_intData is %d\r\n", intData);
//    printf("ADC.cpp this->ADC_intData is %d\r\n", this->ADC_intData);


	return 0;
}

#endif
//-----------------------------------------
// 衛星ソフトウェア依存関数宣言
//-----------------------------------------





