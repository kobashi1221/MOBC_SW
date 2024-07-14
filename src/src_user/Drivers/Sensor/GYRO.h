/*
 * GYRO.h
 *
 *  Created on: 2021/05/27
 *      Author: masuda
 */

#ifndef C2A_DRIVERS_ACS_GYRO_H_
#define C2A_DRIVERS_ACS_GYRO_H_


//#include <main.h>
#include <src_core/System/TImeManager/time_manager.h>//for delay()
#include <src_user/Applications/UserDefined/Power/PWR_cmd_IOEX_PWR2.h>
#include <src_user/IfWrapper/IF_class.h>
#include <stdint.h> //for uint8_t
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------


//-----------------------------------------
// センサクラス宣言
//-----------------------------------------
#ifdef TWO_U
class GYRO_CLASS :  public ADC_CLASS
#endif /*for 2U*/
#ifdef ONE_U
class GYRO_CLASS
#endif /*for 2U*/
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
		float Calculate_GYRO_TempCorrect(int i, float g_a);
		void Temperature_Compensation_SF(int i, float g_t);
		void Temperature_Compensation_OFFSET(int i, float g_t);

		//-----------------------------------------
		// センサ依存部関数（データ取得関数）宣言
		//-----------------------------------------
		int GYRO_SENSOR_AD7654_Get(int state);


	public:

		//-----------------------------------------
		// マイコン依存変数宣言
		//-----------------------------------------
		GPIO_TypeDef* GYRO_RESET_Port = RESET3V3_MOBCtoGYRO_GPIO_Port;
		uint16_t GYRO_RESET_Pin = RESET3V3_MOBCtoGYRO_Pin;

		GPIO_TypeDef* GYROX_SEL0_Port = SEL0_3V3_MOBCtoGYROX_GPIO_Port;
		uint16_t GYROX_SEL0_Pin = SEL0_3V3_MOBCtoGYROX_Pin;

		GPIO_TypeDef* GYROX_SEL1_Port = SEL1_3V3_MOBCtoGYROX_GPIO_Port;
		uint16_t GYROX_SEL1_Pin = SEL1_3V3_MOBCtoGYROX_Pin;

		GPIO_TypeDef* GYROY_SEL0_Port = SEL0_3V3_MOBCtoGYROY_GPIO_Port;
		uint16_t GYROY_SEL0_Pin = SEL0_3V3_MOBCtoGYROY_Pin;

		GPIO_TypeDef* GYROY_SEL1_Port = SEL1_3V3_MOBCtoGYROY_GPIO_Port;
		uint16_t GYROY_SEL1_Pin = SEL1_3V3_MOBCtoGYROY_Pin;

		GPIO_TypeDef* GYROZ_SEL0_Port = SEL0_3V3_MOBCtoGYROZ_GPIO_Port;
		uint16_t GYROZ_SEL0_Pin = SEL0_3V3_MOBCtoGYROZ_Pin;

		GPIO_TypeDef* GYROZ_SEL1_Port = SEL1_3V3_MOBCtoGYROZ_GPIO_Port;
		uint16_t GYROZ_SEL1_Pin = SEL1_3V3_MOBCtoGYROZ_Pin;

		//-----------------------------------------
		// センサ依存部関数（データ取得関数）宣言
		//-----------------------------------------
		float Convert_Voltage2GYRO(int i, float g_a);


		//-----------------------------------------
		// 衛星ソフトウェア依存変数宣言
		//-----------------------------------------
		uint8_t state[3]; //±75deg/s→0, ±150deg/s→1, ±300deg/s→2, ±900deg/s→3
		static constexpr uint8_t state_[3] = {0, 0, 0};

		float k_freq[3]; //frequency of the rate of rotation
		static constexpr float k_freq_[3] = {0.0f, 0.0f, 0.0f}; //frequency of the rate of rotation

		float k_R[3]; //roll^off resistor inside the ASIC(nominally 48korm)
		static constexpr float k_R_[3] = {48.0f*1000.0f, 48.0f*1000.0f, 48.0f*1000.0f}; //roll^off resistor inside the ASIC(nominally 48korm)

		float k_C[3]; //the value of capacitor C2
		static constexpr float k_C_[3] = {47.0f*0.000000001f, 47.0f*0.000000001f, 47.0f*0.000000001f}; //the value of capacitor C2

		float k_gyro_scale[3]; //Scale_Factor(deg/s)
		static constexpr float k_gyro_scale_[3] = {1.0f, 1.0f, 1.0f}; //Scale_Factor(deg/s)

#ifdef TWO_U
		float k_gyro_vols_scale[3] = {1.429f, 1.429f, 1.429f}; //Scale_Factor(Vols)

        float k_gyro_vols_bias[3]; //Bias(Volts)
        static constexpr float k_gyro_vols_bias_[3] = {0.042099f + 0.975f, 0.039152f + 0.975, 0.035185f + 0.975}; //Bias(Volts) ±6mV //offset
#endif /*for 2U*/
#ifdef ONE_U
        float k_gyro_vols_scale[3] = {1.0f, 1.0f, 1.0f}; //Scale_Factor(Vols)

        float k_gyro_vols_bias[3]; //Bias(Volts)
        static constexpr float k_gyro_vols_bias_[3] = {-0.00281f, -0.00171f, -0.00141f}; //Bias(Volts) ±6mV //offset
#endif /*for 1U*/

		float k_InputVol[3]; //g_sensor.p_pwr_IOEXPANDER->V_REG_CDH3V3;
		static constexpr float k_InputVol_[3] = {3.3f, 3.3f, 3.3f}; //measured at cc2_07 and cc2_08

		float k_K[3]; // scale factor dependent on rate range //±75deg/s→0.012, ±150deg/s→0.006, ±300deg/s→0.0003, ±900deg/s→0.001
		static constexpr float k_K_[3] = {0.012f*3.3f/3.0f, 0.012f*3.3f/3.0f, 0.012f*3.3f/3.0f}; //毎回ドライバで値がだいにゅうされれうのでこのMRAMは使用されない

		float ACS1_GYRO_DCM[3][3]; // plus or minus  //x, y, z
		static constexpr float ACS1_GYRO_DCM_[3][3] = {1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f};

		float ACS3_GYRO_DCM[3][3]; // plus or minus  //x, y, z
		static constexpr float ACS3_GYRO_DCM_[3][3] = {-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

		float GYRO_THRESHOLD[3];  // angular velocity threshold
		static constexpr float GYRO_THRESHOLD_[3] = {1.0f, 1.0f, 1.0f};

		float GYRO_HIGHSPIN_THRESHOLD[3];  // angular velocity threshold in high spin mode
		static constexpr float GYRO_HIGHSPIN_THRESHOLD_[3] = {13.0f, 5.0f, 5.0f};

		// MEMS_GYRO Temperature Compensation Scale Factor Coefficient
		float mems_gyro_sf_t0[3];
		static constexpr float mems_gyro_sf_t0_[3] = {25.0f, 25.0f, 25.0f};

		float mems_gyro_sf_t1[3];
		static constexpr float mems_gyro_sf_t1_[3] = {0.0f, 0.0f, 0.0f};

		float mems_gyro_sf_t2[3];
		static constexpr float mems_gyro_sf_t2_[3] = {0.0f, 0.0f, 0.0f};

		// MEMS_GYRO Temperature Compensation Offset Coefficient
		float mems_gyro_os_t0[3];
		static constexpr float mems_gyro_os_t0_[3] = {25.0f, 25.0f, 25.0f};

		float mems_gyro_os_t1[3];
		static constexpr float mems_gyro_os_t1_[3] = {0.0f, 0.0f, 0.0f};

		float mems_gyro_os_t2[3];
		static constexpr float mems_gyro_os_t2_[3] = {0.0f, 0.0f, 0.0f};

		typedef enum {
			MEMS_GYRO_RANGE_75,
			MEMS_GYRO_RANGE_150,
			MEMS_GYRO_RANGE_300,
			MEMS_GYRO_RANGE_900
		} MEMS_GYRO_RANGE;

		//コンストラクを利用せず初期化する場合や、
		//geter seterを利用せず、const pointerをグローバルで共有する場合、
		//データ格納部はpublicである必要がある
		//GYRO Data
		float omega[3];
		float OMEGA[3];
//		float omegax;          //角速度x [rad/s]
//		float omegay;          //角速度y [rad/s]
//		float omegaz;          //角速度z [rad/s]
		uint16_t ADC_omegax;   //GYROX出力電圧デジタル値
		uint16_t ADC_omegay;   //GYROY出力電圧デジタル値
		uint16_t ADC_omegaz;   //GYROZ出力電圧デジタル値
		float gyro_scale_temp[3] = {0.0f};  //MEMS_GYROスケールファクタ温度補正
		float gyro_offset_temp[3] = {0.0f}; //MEMS_GYROオフセット温度補正
		ObcTime gyrosensor_time_before; //1step前のGYROセンサ取得時刻
		ObcTime gyrosensor_time_now;    //現在のGYROセンサ取得時刻
#ifdef HILS
		ObcTime gyrosensor_time_before_hils; //1step前のGYROセンサ取得時刻
		ObcTime gyrosensor_time_now_hils;    //現在のGYROセンサ取得時刻
#endif

		// 上位クラス ADC_CLASSで宣言してあるのでいらない 田村
//		float ADC_data;
//		uint16_t ADC_intData;
		//-----------------------------------------
		// 衛星ソフトウェア依存関数宣言
		//-----------------------------------------
		int GYRO_SENSOR_init(uint8_t state[3]);
		int GYRO_SENSOR_get_all();
		int GYRO_SENSOR_get_x();
		int GYRO_SENSOR_get_y();
		int GYRO_SENSOR_get_z();
		void GYRO_SENSOR_get_time();
		void GYRO_SENSOR_get_time_HILS();
		float Convert_Voltage2GYRO_MAGNARO(int i, float g_a, int flag, float g_t);
		void GYRO_Vector_DCM_Sensor2Body_Calc(float Omega[3], float DCM[3][3], float OMEGA[3]);//1Uと2Uで使うので引数が必要
		int GYRO_SENSOR_CHANGE_RANGE(uint8_t state[3]);
};

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* C2A_DRIVERS_ACS_GYRO_H_ */
