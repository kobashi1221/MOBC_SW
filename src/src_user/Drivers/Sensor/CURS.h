/*
 * CURS.h
 *
 *  Created on: 2021/05/29
 *      Author: masud
 */

#ifndef C2A_DRIVERS_ACS_CURS_H_
#define C2A_DRIVERS_ACS_CURS_H_
#ifdef TWO_U

//#include "main.h"
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
class CURS_CLASS :  public ADC_CLASS
{
	private:

		//この構成では、privateな変数をおくか、コードファイル内のstaticな変数でおくか、あるいは#defineするかは任意性がある
		//センサ依存関数で個体によらないものもコード内staticで宣言できるが、関数によって呼び出し方が異なるのは不便であるので、クラスで宣言する

		//-----------------------------------------
		// センサ依存部関数（マイコン依存関数のラッパー）宣言
		//-----------------------------------------

		int CURS_SENSOR_AD7654_Get(int state);

		//-----------------------------------------
		// センサ依存部関数（非マイコン依存かつデータ取得でない関数）宣言
		//-----------------------------------------

		//-----------------------------------------
		// センサ依存部関数（データ取得関数）宣言
		//-----------------------------------------
		float Calculate_CURRENT(float g_a);

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
		float curmtqz4;
		float curmtqz3;
		float curmtqz2;
		float curmtqz1;
		float curmtqy;
		float curmtqx;
		float curmtq2u;
		float curtx;
		uint16_t ADC_curmtqz4;
		uint16_t ADC_curmtqz3;
		uint16_t ADC_curmtqz2;
		uint16_t ADC_curmtqz1;
		uint16_t ADC_curmtqy;
		uint16_t ADC_curmtqx;
		uint16_t ADC_curmtq2u;
		uint16_t ADC_curtx;

		// 上位クラス ADC_CLASSで宣言してあるのでいらない 田村
//		float ADC_data;
//		uint16_t ADC_intData;
		//-----------------------------------------
		// 衛星ソフトウェア依存関数宣言
		//-----------------------------------------
		int CURS_SENSOR_init();
		int CURS_SENSOR_get_all();
		int CURS_SENSOR_get_MTQZ4();
		int CURS_SENSOR_get_MTQZ3();
		int CURS_SENSOR_get_MTQZ2();
		int CURS_SENSOR_get_MTQZ1();
		int CURS_SENSOR_get_MTQY();
		int CURS_SENSOR_get_MTQX();
		int CURS_SENSOR_get_MTQ2U();
		int CURS_SENSOR_get_TX();
};

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*for 2U*/
#endif /* C2A_DRIVERS_ACS_CURS_H_ */
