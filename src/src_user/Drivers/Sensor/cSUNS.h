///*
// * cSUNS.h
// *
// *  Created on: 2021/11/25
// *      Author: masud
// */
//
//#ifndef SRC_USER_DRIVERS_SENSOR_CSUNS_H_
//#define SRC_USER_DRIVERS_SENSOR_CSUNS_H_
//
//#ifdef ONE_U
//
//#include <src_core/System/TImeManager/time_manager.h>//for delay()
//#include <src_user/IfWrapper/IF_class.h>
//#include <src_user/Applications/UserDefined/Sensor/ACS123_cmd_AnomalyDetect.h>   // for Error list
//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */
//
//
////-----------------------------------------
//// センサ依存変数宣言
////-----------------------------------------
//
////他の関数でセンサ依存部変数を呼び出さない場合、.hで宣言する必要はなく、.cppで宣言すべき
//// エラー一覧
//
//
//#define SUN_SENSOR_SPI_TIMEOUT 1000
//
////-----------------------------------------
//// センサクラス宣言
////-----------------------------------------
//class CSUNS_CLASS : SPI_Class
//{
//	private:
//
//		//この構成では、privateな変数をおくか、コードファイル内のstaticな変数でおくか、あるいは#defineするかは任意性がある
//		//センサ依存関数で個体によらないものもコード内staticで宣言できるが、関数によって呼び出し方が異なるのは不便であるので、クラスで宣言する
//
//		//-----------------------------------------
//		// センサ依存部関数（マイコン依存関数のラッパー）宣言
//		//-----------------------------------------
//
//
//
//		//-----------------------------------------
//		// センサ依存部関数（非マイコン依存かつデータ取得でない関数）宣言
//		//-----------------------------------------
//
//
//		//-----------------------------------------
//		// センサ依存部関数（データ取得関数）宣言
//		//-----------------------------------------
//	    int CSS_S1600833_GET(float &output);
//
//
//	public:
//
//		//-----------------------------------------
//		// マイコン依存変数宣言
//		//-----------------------------------------
//
//		// CSピンの情報
//		GPIO_TypeDef *gpio_port;
//		uint16_t gpio_pin;
//
//		// SPI handler
//		SPI_HandleTypeDef *spi_handler;
//
//		//-----------------------------------------
//		// 衛星ソフトウェア依存変数宣言
//		//-----------------------------------------
//
//		//コンストラクを利用せず初期化する場合や、
//		//geter seterを利用せず、const pointerをグローバルで共有する場合、
//		//データ格納部はpublicである必要がある
//
//		uint16_t css_val_;          // チップサンセンサ生値
//
//		float theta;                // 太陽光入射角
//
//		float peak; // 太陽放射強度が入力された時の返り値（最大3000mV） TODO 1355W/m^2で垂直に光を当てた時の値を初期参照値とする
//		static constexpr float peak_ = 0.0f;
//
//		float CSS_threshold;
//		static constexpr float CSS_threshold_ = 0.17f; // アルベドを考慮した下限閾値
//
//		float CSUNSensorVec[3];     // 太陽方向ベクトル(PX, -MY, PZ)
//
//        ObcTime csunsensor_time_before;
//        ObcTime csunsensor_time_now;
//
//		// センサエラー番号の格納
//		SUN_SENSOR_StateTypeDef state;
//
//		//-----------------------------------------
//		// 衛星ソフトウェア依存関数宣言
//		//-----------------------------------------
//		int CSS_get_all();
//		int CSS_Vector_Calc(float thetaPX, float thetaMY, float thetaPZ);
//		int CSS_init();
//		void CSUN_SENSOR_get_time();
//
//};
//
//
//
//#ifdef __cplusplus
//}
//#endif /* __cplusplus */
//
//#endif
//
//#endif /* SRC_USER_DRIVERS_SENSOR_CSUNS_H_ */
