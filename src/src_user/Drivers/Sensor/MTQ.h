/*
  *****************************************************************************************
  * @file    : MTQ.h
  * @author  : Hoang Xuan Truong An
  * @version : 1.0
  * @date    : 2020/10/20
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/10/20  1.0   An              First revision
  * 2021/06/16  2.0   Hiroaki MASUDA
  *****************************************************************************************
  */

#ifndef ACS2_mtq_H_
#define ACS2_mtq_H_

#include <stdint.h> //for uint8_t
#include <src_user/IfWrapper/HAL.h>
#include <src_user/IfWrapper/IF_class.h>

#define MTQ_I2C_TIMEOUT 1000

//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------

extern uint16_t curs_cmd_val_int_[4];
extern uint16_t curs_ctrl_int_[4];
extern uint16_t curs_cmd_val_int_x;
extern uint16_t curs_cmd_val_int_y;
extern uint16_t curs_cmd_val_int_z;
extern uint16_t curs_cmd_val_int_o;

//-----------------------------------------
// センサクラス宣言
//-----------------------------------------
class MTQ_CLASS : I2C_Class
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

	    float target_mag_moment_mtq2U[4]; //2U MTQX,Y,Z target constant current
	    static constexpr float target_mag_moment_mtq2U_[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	    float target_mag_moment_mtq1U[3]; //1U MTQX target constant current
	    static constexpr float target_mag_moment_mtq1U_[3] = {0.0f, 0.0f, 0.0f};

	    float k_PID_mtqx2U[3];                  // k_P,   k_I,   k_D
	    static constexpr float k_PID_mtqx2U_[3] = {0.25f, 0.05f, 0.0f};//{0.25f, 0.05f, 0.0f}; {1.5f, 0.05f, 0.1f}

	    float k_PID_mtqy2U[3];                  // k_P,   k_I,   k_D
	    static constexpr float k_PID_mtqy2U_[3] = {0.3f, 0.1f, 0.0f};//{0.3f, 0.1f, 0.0f}; {1.5f, 0.05f, 0.1f}

	    float k_PID_mtqz2U[3];                  // k_P,   k_I,   k_D
	    static constexpr float k_PID_mtqz2U_[3] = {0.1f, 0.1f, 0.0f};//{0.25f, 0.1f, 0.0f}; {0.5f, 0.1f, 0.1f}

	    float k_PID_mtqo2U[3];                  // k_P,   k_I,   k_D
	    static constexpr float k_PID_mtqo2U_[3] = {0.1f, 0.1f, 0.0f};//{0.1f, 0.1f, 0.0f}; {0.5f, 0.1f, 0.1f}

	    float k_PID_mtqx1U[3];                  // k_P,   k_I,   k_D
	    static constexpr float k_PID_mtqx1U_[3] = {2.2f, 1.0f, 0.0f};

	    float k_PID_mtqy1U[3];                  // k_P,   k_I,   k_D
	    static constexpr float k_PID_mtqy1U_[3] = {4.0f, 1.0f, 0.0f};

	    float k_PID_mtqz1U[3];                  // k_P,   k_I,   k_D
	    static constexpr float k_PID_mtqz1U_[3] = {2.0f, 1.0f, 0.0f};

		float MTQA_DCM[3][3];    // plus or minus//x,      y,       z,
		static constexpr float MTQA_DCM_[3][3] = {1, 0, 0, 0, 1, 0, 0, 0, 1};

		float MTQO_DCM[3][3];    // plus or minus//x,      y,       z,
		static constexpr float MTQO_DCM_[3][3] = {0, 0, 0, 0, 0, 0, 0, 0, 1};

		// nS
		float nS_mtqxy2U; // 1.449 = 250 * 0.042 * 0.138 = nS equation(2)
		static constexpr float nS_mtqxy2U_ = 1.2f * 0.001f; // 1.449f * 0.001f;(MAGNARO)

		float nS_mtqxy1U; // 0.4935 = 250 * 0.042 * 0.047 = nS equation(2)
		static constexpr float nS_mtqxy1U_ = 1.2f * 0.001f; // 0.4935f * 0.001f;(MAGNARO)

		float nS_mtqz2U;
		static constexpr float nS_mtqz2U_ = 13.226f * 0.001f;

		float nS_mtqz1U;
		static constexpr float nS_mtqz1U_ = 2.102f * 0.001f * 4; // 4本あるので1本あたりはMTQZの1/4

		float nS_mtqo2U;
		static constexpr float nS_mtqo2U_ = 13.226f * 0.001f * 3; // 3本あるので1本あたりはMTQZの1/3    //1.5898f test value

        // const for 2U MTQX
		float kt_mtqx2U[4];
		static constexpr float kt_mtqx2U_[4] = {0.1473f, 33.796f, 0.0645f, 8183.9f};

        // const for 2U MTQY
		float kt_mtqy2U[4];
		static constexpr float kt_mtqy2U_[4] = {0.1367f, 37.905f, 0.0779f, 8191.0f};

        // const for 2U MTQZ
		float kt_mtqz2U[4];
		static constexpr float kt_mtqz2U_[4] = {0.0669f, 15.577f, 0.1297f, 8178.9f};

        // const for 1U MTQX
		float kt_mtqx1U[4];
		static constexpr float kt_mtqx1U_[4] = {0.0784f, 18.752f, -0.1798f, 8178.5f};

        // const for 1U MTQY
		float kt_mtqy1U[4];
		static constexpr float kt_mtqy1U_[4] = {0.0734f, 19.261f, -0.1338f, 8174.7f};

        // const for 1U MTQZ
		float kt_mtqz1U[4];
		static constexpr float kt_mtqz1U_[4] = {0.0613f, 14.842f, -0.008f, 8189.9f};

		float R_mtqx2U;
		static constexpr float R_mtqx2U_ = 22.3f;

		float R_mtqy2U;
	    static constexpr float R_mtqy2U_ = 22.2f;

	    float R_mtqz2U;
	    static constexpr float R_mtqz2U_ = 10.1f;

	    float R_mtqx1U;
	    static constexpr float R_mtqx1U_ = 12.4f;

	    float R_mtqy1U;
	    static constexpr float R_mtqy1U_ = 12.5f;

	    float R_mtqz1U;
	    static constexpr float R_mtqz1U_ = 9.0f;

	    float current_limit_mtq2U[4];
//	    static constexpr float current_limit_mtq2U_[4] = {100.0f, 100.0f, 200.0f, 200.0f}; // for MAGNARO
	    static constexpr float current_limit_mtq2U_[4] = {500.0f, 500.0f, 250.0f, 750.0f};

	    float current_limit_mtq1U[3];
//	    static constexpr float current_limit_mtq1U_[3] = {150.0f, 150.0f, 200.0f}; // for MAGNARO
	    static constexpr float current_limit_mtq1U_[3] = {500.0f, 500.0f, 250.0f};

		uint16_t MTQ_LOWER_THRESHOLD[4];             // MTQ lower threshold for Anomaly
		static constexpr uint16_t MTQ_LOWER_THRESHOLD_[4] = {0, 0, 0, 0};

		uint16_t MTQ_UPPER_THRESHOLD[4];              // MTQ upper threshold for Anomaly
		static constexpr uint16_t MTQ_UPPER_THRESHOLD_[4] = {16383, 16383, 16383, 16383};

		uint8_t hispin_contorol_flag_mtq2U[3];
		static constexpr uint8_t hispin_contorol_flag_mtq2U_[3] = {0, 0, 0};

		uint8_t hispin_contorol_flag_mtq1U[3];
		static constexpr uint8_t hispin_contorol_flag_mtq1U_[3] = {0, 0, 0};

		//-----------------------------------------
		// 衛星ソフトウェア依存変数宣言
		//-----------------------------------------

	    // prameter of float type of target current
	    float target_curs_mtqx;
	    float target_curs_mtqy;
	    float target_curs_mtqz;
	    float target_curs_mtqo;

	    // prameter of int16_t type of target current
	    uint16_t target_curs_mtqx_int;
	    uint16_t target_curs_mtqy_int;
	    uint16_t target_curs_mtqz_int;
	    uint16_t target_curs_mtqo_int;

	    uint16_t curs_mtqx_int;
	    uint16_t curs_mtqy_int;
	    uint16_t curs_mtqz_int;
	    uint16_t curs_mtqo_int;

	    uint16_t curs_cmd_val_int_[4] = {8192, 8192, 8192, 8192};

	    // prameter of PID gain
	    float k_P[4];
	    float k_I[4];
	    float k_D[4];

	    float dt = 0.001f;       // period of for initial timeline loop
	    int32_t P[4];            // P control amount
	    int32_t I[4];            // I control amount
	    int32_t D[4];            // D control amount
	    int32_t preP[4];         // 1 step previous P control amount

	    uint8_t mtqa_anomaly = 0;		//maqa_OT_anomaly_flag
	    uint8_t mtqo_anomaly = 0;		//maqo_OT_anomaly_flag


		//コンストラクを利用せず初期化する場合や、
		//geter seterを利用せず、const pointerをグローバルで共有する場合、
		//データ格納部はpublicである必要がある

		//-----------------------------------------
		// 衛星ソフトウェア依存関数宣言
		//-----------------------------------------
		void MTQA_init();
		void MTQO_init();
		void MTQA_X(float Mag_moment);
		void MTQA_Y(float Mag_moment);
		void MTQA_Z(float Mag_moment);
		void MTQO_Z(float Mag_moment);
		void MTQA_FunctionCore(const float Mag_moment[3]);
		void MTQO_FunctionCore(float Mag_moment);
		void MTQA_FunctionCore4debug(const float Vol[3]);
		void MTQO_FunctionCore4debug(float Vol);
		void MTQ_Target_Current_Setting(int i, float Target_current);
		void MTQ_Read_Current(int i);
		void MTQ_PID_Param_Setting();
		void MTQ_Calculate_HighSpin_Control(int i, float Mag_moment);
		void MTQ_Calculate_PID_Control(int i, float target_curs_mtq_int, float curs_mtq_int);
		void MTQA_DCM_Calc(uint16_t cmd_val_[3]);
		void MTQO_DCM_Calc(uint16_t cmd_val_[3]);
		void MTQ_Output(int bfc, uint16_t mtq_output);
		void MTQAX_Const_Mag_Moment_Control(void);
		void MTQAY_Const_Mag_Moment_Control(void);
		void MTQAZ_Const_Mag_Moment_Control(void);
		void MTQO_Const_Mag_Moment_Control(void);


		uint8_t mtq_flag_vote(uint8_t* flag);
		void mtq_trans_set(uint8_t* flag);
		void mtqtrans_reset(uint8_t* flag);
};

#endif

