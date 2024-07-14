/*
 * PlatinumSensor.h
 *
 *  Created on: 2021/05/27
 *      Author: masud
 */

#ifndef C2A_DRIVERS_ACS_PLATINUMSENSOR_H_
#define C2A_DRIVERS_ACS_PLATINUMSENSOR_H_
#ifdef TWO_U
//#include <main.h>
#include <src_core/System/TImeManager/time_manager.h>//for delay()
#include <src_user/IfWrapper/IF_class.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------


//-----------------------------------------
// センサクラス宣言
//-----------------------------------------
class PLATINUM_CLASS :  public ADC_CLASS
{
	private:

		//この構成では、privateな変数をおくか、コードファイル内のstaticな変数でおくか、あるいは#defineするかは任意性がある
		//センサ依存関数で個体によらないものもコード内staticで宣言できるが、関数によって呼び出し方が異なるのは不便であるので、クラスで宣言する

		//-----------------------------------------
		// センサ依存部関数（マイコン依存関数のラッパー）宣言
		//-----------------------------------------

		int PLATINUM_SENSOR_AD7654_Get(int state);

		//-----------------------------------------
		// センサ依存部関数（非マイコン依存かつデータ取得でない関数）宣言
		//-----------------------------------------
		float Calculate_Pt_Plus(float g_b);
		float Calculate_Pt_Minus(float g_c);

		//-----------------------------------------
		// センサ依存部関数（データ取得関数）宣言
		//-----------------------------------------


	public:

		//-----------------------------------------
		// マイコン依存変数宣言
		//-----------------------------------------


		//-----------------------------------------
		// 衛星ソフトウェア依存変数宣言
		//-----------------------------------------

		float k_AppliedCurrent;
		static constexpr float k_AppliedCurrent_ = 0.31f; //Applied Current of Platinum sensor [mA]

		// 温度データ[℃]
		float PtMOBC;
//		float PtTX;
//		float PtTXPIC;
//		float PtRX;
//		float PtRXPIC;
		float PtADC;
		float PtMAGS;
		float PtGPS;
		float Pt9AX;
//		float PtSEPCOILPY;
		float PtLENSPX;
		float PtLENSPY;
		float PtIMAGE;
		float PtLENSMX;
//		float PtSEPCOILMY;
		float PtGYROX;
		float PtGYROY;
		float PtGYROZ;
		float PtMTQZ1;
		float PtMTQZ2;
		float PtMTQZ3;
		float PtMTQZ4;

		//出力電圧デジタル値
		uint16_t ADC_PtMOBC;
//		uint16_t ADC_PtTX;
//		uint16_t ADC_PtTXPIC;
//		uint16_t ADC_PtRX;
//		uint16_t ADC_PtRXPIC;
		uint16_t ADC_PtADC;
		uint16_t ADC_PtMAGS;
		uint16_t ADC_PtGPS;
		uint16_t ADC_Pt9AX;
//		uint16_t ADC_PtSEPCOILPY; //旧PtSUNSPX
		uint16_t ADC_PtLENSPX;    //旧PtSUNSMX
		uint16_t ADC_PtLENSPY;    //旧PtSUNSPY
		uint16_t ADC_PtIMAGE;     //旧PtSUNSMY
		uint16_t ADC_PtLENSMX;    //旧PtSUNSPZ
//		uint16_t ADC_PtSEPCOILMY; //旧PtSUNSMZ
		uint16_t ADC_PtGYROX;
		uint16_t ADC_PtGYROY;
		uint16_t ADC_PtGYROZ;
		uint16_t ADC_PtMTQZ1;
		uint16_t ADC_PtMTQZ2;
		uint16_t ADC_PtMTQZ3;
		uint16_t ADC_PtMTQZ4;

		//  PT Temperature Threshold for Anomaly
		float PT_TEMP_THRESHOLD;
		static constexpr float PT_TEMP_THRESHOLD_ = 50.0f;

		// 上位クラス ADC_CLASSで宣言してあるのでいらない 田村
//		float ADC_data;
//		uint16_t ADC_intData;
		//-----------------------------------------
		// 衛星ソフトウェア依存関数宣言
		//-----------------------------------------
		int PLATINUM_SENSOR_init();
		int PLATINUM_SENSOR_get_all();
		int PLATINUM_SENSOR_get_MOBC();
		int PLATINUM_SENSOR_get_ADC();
		int PLATINUM_SENSOR_get_MAGS();
		int PLATINUM_SENSOR_get_GPS();
		int PLATINUM_SENSOR_get_9AX();
		int PLATINUM_SENSOR_get_LENSPX();
		int PLATINUM_SENSOR_get_LENSPY();
		int PLATINUM_SENSOR_get_IMAGE();
		int PLATINUM_SENSOR_get_LENSMX();
		int PLATINUM_SENSOR_get_GYROX();
		int PLATINUM_SENSOR_get_GYROY();
		int PLATINUM_SENSOR_get_GYROZ();
		int PLATINUM_SENSOR_get_MTQZ1();
		int PLATINUM_SENSOR_get_MTQZ2();
		int PLATINUM_SENSOR_get_MTQZ3();
		int PLATINUM_SENSOR_get_MTQZ4();
};

void PS_Delay(uint16_t ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*for 2U*/
#endif /* C2A_DRIVERS_ACS_PLATINUMSENSOR_H_ */
