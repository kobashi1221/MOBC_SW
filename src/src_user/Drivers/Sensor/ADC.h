/**
  *****************************************************************************************
  * @file    : ADC.h
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

#ifndef ADC_H_
#define ADC_H_
#ifdef TWO_U

#include <stdint.h> //for uint8_t
#include <src_user/IfWrapper/HAL.h>
#include <src_user/IfWrapper/IF_class.h>

//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------


//-----------------------------------------
// センサクラス宣言
//-----------------------------------------
class ADC_CLASS : public GPIO_Class
{
	private:

		//この構成では、privateな変数をおくか、コードファイル内のstaticな変数でおくか、あるいは#defineするかは任意性がある
		//センサ依存関数で個体によらないものもコード内staticで宣言できるが、関数によって呼び出し方が異なるのは不便であるので、クラスで宣言する

		//-----------------------------------------
		// センサ依存部関数（マイコン依存関数のラッパー）宣言
		//-----------------------------------------

		//-----------------------------------------
		// センサ依存部関数（非マイコン依存かつデータ取得でない関数）宣言
		//-----------------------------------------


		//-----------------------------------------
		// センサ依存部関数（データ取得関数）宣言
		//-----------------------------------------


	public:

		//-----------------------------------------
		// マイコン依存変数宣言
		//-----------------------------------------
		//GPIO Pin
		GPIO_TypeDef* ADC_GPIO_Port;
		uint16_t ADC_GPIO_Pin;

		//MUX Pins
		GPIO_TypeDef* CTL_ANASEN_Port = CTL_OD3V3_MOBCtoMUX_ANAS_GPIO_Port;
		uint16_t CTL_ANASEN_Pin = CTL_OD3V3_MOBCtoMUX_ANAS_Pin;
		GPIO_TypeDef* CTL_TEMS_Port = CTL_OD3V3_MOBCtoMUX_TEMS_GPIO_Port;
		uint16_t CTL_TEMS_Pin = CTL_OD3V3_MOBCtoMUX_TEMS_Pin;
		GPIO_TypeDef* A0_Port = A0_OD3V3_MOBCtoMUX_GPIO_Port;
		uint16_t A0_Pin = A0_OD3V3_MOBCtoMUX_Pin;
		GPIO_TypeDef* A1_Port = A1_OD3V3_MOBCtoMUX_GPIO_Port;
		uint16_t A1_Pin = A1_OD3V3_MOBCtoMUX_Pin;
		GPIO_TypeDef* A2_Port = A2_OD3V3_MOBCtoMUX_GPIO_Port;
		uint16_t A2_Pin = A2_OD3V3_MOBCtoMUX_Pin;
		GPIO_TypeDef* A3_Port = A3_OD3V3_MOBCtoMUX_GPIO_Port;
		uint16_t A3_Pin = A3_OD3V3_MOBCtoMUX_Pin;
		GPIO_TypeDef* A4_Port = A4_OD3V3_MOBCtoMUX_GPIO_Port;
		uint16_t A4_Pin = A4_OD3V3_MOBCtoMUX_Pin;

		//ADC GPIO Pins
		GPIO_TypeDef* ADC_A0_Port = A0_3V3_MOBCtoADC_ACS1_GPIO_Port;
		uint16_t ADC_A0_Pin = A0_3V3_MOBCtoADC_ACS1_Pin;
		GPIO_TypeDef* AB_Port = AB_3V3_MOBCtoADC_ACS1_GPIO_Port;
		uint16_t AB_Pin = AB_3V3_MOBCtoADC_ACS1_Pin;
		GPIO_TypeDef* CNVST_Port = CNVST3V3_MOBCtoADC_ACS1_GPIO_Port;
		uint16_t CNVST_Pin = CNVST3V3_MOBCtoADC_ACS1_Pin;
		GPIO_TypeDef* BUSY_Port = BUSY3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t BUSY_Pin = BUSY3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* BYTESWAP_Port = BYTESWAP3V3_MOBCtoADC_ACS1_GPIO_Port;
		uint16_t BYTESWAP_Pin = BYTESWAP3V3_MOBCtoADC_ACS1_Pin;
		GPIO_TypeDef* D0_Port = D0_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D0_Pin = D0_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D1_Port = D1_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D1_Pin = D1_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D2_Port = D2_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D2_Pin = D2_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D3_Port = D3_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D3_Pin = D3_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D4_Port = D4_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D4_Pin = D4_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D5_Port = D5_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D5_Pin = D5_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D6_Port = D6_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D6_Pin = D6_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D7_Port = D7_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D7_Pin = D7_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D8_Port = D8_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D8_Pin = D8_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D9_Port = D9_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D9_Pin = D9_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D10_Port = D10_3V1_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D10_Pin = D10_3V1_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D11_Port = D11_3V1_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D11_Pin = D11_3V1_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D12_Port = D12_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D12_Pin = D12_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D13_Port = D13_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D13_Pin = D13_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D14_Port = D14_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D14_Pin = D14_3V3_ADC_ACS1toMOBC_Pin;
		GPIO_TypeDef* D15_Port = D15_3V3_ADC_ACS1toMOBC_GPIO_Port;
		uint16_t D15_Pin = D15_3V3_ADC_ACS1toMOBC_Pin;

		//-----------------------------------------
		// 衛星ソフトウェア依存変数宣言
		//-----------------------------------------

		//コンストラクを利用せず初期化する場合や、
		//geter seterを利用せず、const pointerをグローバルで共有する場合、
		//データ格納部はpublicである必要がある

		uint16_t ADC_intData;
		float ADC_Data;


		//-----------------------------------------
		// 衛星ソフトウェア依存関数宣言
		//-----------------------------------------
		void ADC_GPIO_BUSY();
		void Select_ANASEN_Channel();
		void Select_PtTems_Channel();
		void MUX_ANASEN(int c_acs1);
		void MUX_PtTEMS(int c_acs1);
//		int ADC_GPIO_All_Read(uint16_t ADC_intData, float ADC_Data);
//		int ADC_GPIO_All_Read(ADC_CLASS* sub_instance);
		int ADC_GPIO_All_Read();
};


void ADC_Delay(uint16_t ms);


#endif/* for 2U*/
#endif /* ADC_H_ */

