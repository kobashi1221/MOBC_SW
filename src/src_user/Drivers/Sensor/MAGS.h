/*
 * MAGS.h
 *
 *  Created on: 2021/05/27
 *      Author: masud
 */

#ifndef C2A_DRIVERS_ACS_MAGS_H_
#define C2A_DRIVERS_ACS_MAGS_H_

#ifdef TWO_U
//#include <main.h>
#include <src_core/System/TImeManager/time_manager.h>//for delay()
#include <src_user/IfWrapper/IF_class.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <src_user/Drivers/Sensor/ADC.h>

//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------


//-----------------------------------------
// センサクラス宣言
//-----------------------------------------
class MAGS_CLASS : public ADC_CLASS
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

		void Temperature_Compensation_SF(int i, float g_t);
		void Temperature_Compensation_OFFSET(int i, float g_t);

		//-----------------------------------------
		// センサ依存部関数（データ取得関数）宣言
		//-----------------------------------------
		int MAGS_SENSOR_AD7654_Get(int state);
		float Convert_Voltage2MAG(int i, float g_a);
		float Convert_Voltage2MAG_TempCorrect(int i, float g_a);
		void Calculate_MAG_value();

	public:

		//-----------------------------------------
		// マイコン依存変数宣言
		//-----------------------------------------

		float k_mag_scale[3]; //scaling 30mV/uT
		static constexpr float k_mag_scale_[3] = {30.0f/1.13484f, 30.0f, 30.0f/1.7f};

		float k_mag_offset[3]; //offset +-100nT [in zero field]
		static constexpr float k_mag_offset_[3] = { -(5.0f*12744.667f/65536.0f)/30000.0f, -(5.0f*2469.08333f/65536.0f)/30000.0f, (5.0f*972.91667f/65536.0f)/30000.0f};  //[T]

		float MAGS_DCM[3][3]; // plus or minus  //x, y, z
		static constexpr float MAGS_DCM_[3][3] = {-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f};

		// MEMS_MAGS Temperature Compensation Scale Factor Coefficient
		float mems_mag_sf_t0[3];
		static constexpr float mems_mag_sf_t0_[3] = {25.0f, 25.0f, 25.0f};

		float mems_mag_sf_t1[3];
		static constexpr float mems_mag_sf_t1_[3] = {0.0f, 0.0f, 0.0f};

		float mems_mag_sf_t2[3];
		static constexpr float mems_mag_sf_t2_[3] = {0.0f, 0.0f, 0.0f};

		// MEMS_MAGS Temperature Compensation Offset Coefficient
		float mems_mag_os_t0[3];
		static constexpr float mems_mag_os_t0_[3] = {25.0f, 25.0f, 25.0f};

		float mems_mag_os_t1[3];
		static constexpr float mems_mag_os_t1_[3] = {0.0f, 0.0f, 0.0f};

		float mems_mag_os_t2[3];
		static constexpr float mems_mag_os_t2_[3] = {0.0f, 0.0f, 0.0f};

		//-----------------------------------------
		// 衛星ソフトウェア依存変数宣言
		//-----------------------------------------

		//コンストラクを利用せず初期化する場合や、
		//geter seterを利用せず、const pointerをグローバルで共有する場合、
		//データ格納部はpublicである必要がある


		float magpx;     //X軸P面の出力電圧デジタル値[V]
		float magmx;     //X軸M面の出力電圧デジタル値[V]
		float magpy;   	 //Y軸P面の出力電圧デジタル値[V]
		float magmy;     //Y軸M面の出力電圧デジタル値[V]
		float magpz;     //Z軸P面の出力電圧デジタル値[V]
		float magmz;     //Z軸M面の出力電圧デジタル値[V]
		float magx;      //X軸の出力電圧デジタル値[V]
		float magy;      //Y軸の出力電圧デジタル値[V]
		float magz;      //Z軸の出力電圧デジタル値[V]
		int ADC_magpx;   //X軸P面の出力電圧アナログ値
		int ADC_magmx;   //X軸M面の出力電圧アナログ値
		int ADC_magpy;   //Y軸P面の出力電圧アナログ値
		int ADC_magmy;   //Y軸M面の出力電圧アナログ値
		int ADC_magpz;   //Z軸P面の出力電圧アナログ値
		int ADC_magmz;   //Z軸M面の出力電圧アナログ値

		float Mag[3];
		float MAG[3];
//		float Magx;   //X軸磁束密度[T]
//		float Magy;   //Y軸磁束密度[T]
//		float Magz;   //Z軸磁束密度[T]
		int16_t ADC_magx;    //X軸出力電圧アナログ値(HK用)
		int16_t ADC_magy;    //Y軸出力電圧アナログ値(HK用)
		int16_t ADC_magz;    //Z軸出力電圧アナログ値(HK用)

		float MAGVec[3];

		float mag_scale_temp[3] = {0.0f};   //MEMS_MAGSスケールファクタ温度補正
		float mag_offset_temp[3] = {0.0f};  //MEMS_MAGSオフセット温度補正


		//  MAGS Continuous Time Threshold for Anomaly
		uint8_t MAGS_Continuous_Time_THRESHOLD;
		static constexpr uint8_t MAGS_Continuous_Time_THRESHOLD_ = 20*3;    // 20sec * 3 components

		ObcTime magsensor_time_before;
		ObcTime magsensor_time_now;
#ifdef HILS
		ObcTime magsensor_time_before_hils;
		ObcTime magsensor_time_now_hils;
#endif

		// 上位クラス ADC_CLASSで宣言してあるのでいらない 田村
//		float ADC_data;
//		uint16_t ADC_intData;
		//-----------------------------------------
		// 衛星ソフトウェア依存関数宣言
		//-----------------------------------------
		int MAGS_SENSOR_init();
		int MAGS_SENSOR_get_all();
		int MAGS_SENSOR_get_px();
		int MAGS_SENSOR_get_mx();
		int MAGS_SENSOR_get_py();
		int MAGS_SENSOR_get_my();
		int MAGS_SENSOR_get_pz();
		int MAGS_SENSOR_get_mz();
		void MAGS_Vector_Calc(float magx, float magy, float magz, float MAGSensorVec[3]);
		void MAG_SENSOR_get_time();
		void MAG_SENSOR_get_time_HILS();
		void MAG_Vector_DCM_Sensor2Body_Calc();
		float Convert_Voltage2MAG_MAGNARO(int i, float g_a, int flag);
};

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*for 2U*/
#endif /* C2A_DRIVERS_ACS_MAGS_H_ */
