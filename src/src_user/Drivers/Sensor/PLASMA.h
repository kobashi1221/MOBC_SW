/*
 * PLASMA.h
 *
 *  Created on: 2021/07/03
 *      Author: masud
 */

#ifndef C2A_DRIVERS_ACS_PLASMA_H_
#define C2A_DRIVERS_ACS_PLASMA_H_

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
class PLASMA_CLASS :  public ADC_CLASS
{
	private:

		//この構成では、privateな変数をおくか、コードファイル内のstaticな変数でおくか、あるいは#defineするかは任意性がある
		//センサ依存関数で個体によらないものもコード内staticで宣言できるが、関数によって呼び出し方が異なるのは不便であるので、クラスで宣言する

		//-----------------------------------------
		// センサ依存部関数（マイコン依存関数のラッパー）宣言
		//-----------------------------------------
		int PLASMA_SENSOR_AD7654_Get(int state);

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


		//-----------------------------------------
		// 衛星ソフトウェア依存変数宣言
		//-----------------------------------------

		//コンストラクを利用せず初期化する場合や、
		//geter seterを利用せず、const pointerをグローバルで共有する場合、
		//データ格納部はpublicである必要がある
		//GYRO Data
		float PLASMA1;
		float PLASMA2;
		float PLASMA3;
		float PLASMA4;
		float PLASMA5;
		float PLASMA6;
		uint16_t ADC_PLASMA1;
		uint16_t ADC_PLASMA2;
		uint16_t ADC_PLASMA3;
		uint16_t ADC_PLASMA4;
		uint16_t ADC_PLASMA5;
		uint16_t ADC_PLASMA6;

		// 上位クラス ADC_CLASSで宣言してあるのでいらない 田村
//		float ADC_data;
//		uint16_t ADC_intData;
		//-----------------------------------------
		// 衛星ソフトウェア依存関数宣言
		//-----------------------------------------
		int PLASMA_SENSOR_init();
		int PLASMA_SENSOR_get_all();
		int PLASMA_SENSOR_get_PLASMA1();
		int PLASMA_SENSOR_get_PLASMA2();
		int PLASMA_SENSOR_get_PLASMA3();
		int PLASMA_SENSOR_get_PLASMA4();
		int PLASMA_SENSOR_get_PLASMA5();
		int PLASMA_SENSOR_get_PLASMA6();
};

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /*for 2U*/



#endif /* C2A_DRIVERS_ACS_PLASMA_H_ */
