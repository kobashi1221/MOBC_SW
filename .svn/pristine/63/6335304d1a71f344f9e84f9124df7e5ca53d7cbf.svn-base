/*
  *****************************************************************************************
  * @file    : MTQ.cpp
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

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Library/Delay.h>
#include <src_user/Drivers/Sensor/MTQ.h>
#include <src_user/Drivers/Sensor/ADC.h>
#include <src_user/Drivers/Sensor/CURS.h>
#include <src_user/Applications/UserDefined/Sensor/ACS1_cmd_ADC.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_ADC_MOBC.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/DriverInstances/di_txpic.h>
#include <src_user/Drivers/Com/TXPIC/TXPIC.h>
#include <src_user/Drivers/Com/TXPIC/txpic_telemetry_data_definitions.h>
#include <src_user/Applications/UserDefined/Sensor/ACS123_cmd_AnomalyDetect.h>    // for Anomaly_Local
#include <src_user/Applications/DriverInstances/block_command_handler.h>    //for current mode id
#include <src_user/Drivers/Cdh/CDH_Mode_Transition_driver.h>                //for Auto_trans_set
#include <src_user/Drivers/Power/PWR_Operation.h>
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>

#ifdef TWO_U
static ADC_CLASS adc_class;
static CURS_CLASS cur_class;
#endif //for 2U

I2C_Class ACS_MTQ(&hi2c3, 0xA8, 0xA9, 0x0F);

//======MRAM格納パラメータの実体宣言部======//
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS BEGIN *** //
constexpr float MTQ_CLASS::target_mag_moment_mtq2U_[4]; //target constant current for MTQX 2U
constexpr float MTQ_CLASS::target_mag_moment_mtq1U_[3]; //target constant current for MTQX 1U
constexpr float MTQ_CLASS::k_PID_mtqx2U_[3];      //PID gain for MTQX 2U
constexpr float MTQ_CLASS::k_PID_mtqy2U_[3];      //PID gain for MTQY 2U
constexpr float MTQ_CLASS::k_PID_mtqz2U_[3];      //PID gain for MTQZ 2U
constexpr float MTQ_CLASS::k_PID_mtqo2U_[3];      //PID gain for MTQO 2U
constexpr float MTQ_CLASS::k_PID_mtqx1U_[3];      //PID gain for MTQX 2U
constexpr float MTQ_CLASS::k_PID_mtqy1U_[3];      //PID gain for MTQX 2U
constexpr float MTQ_CLASS::k_PID_mtqz1U_[3];      //PID gain for MTQX 2U
constexpr float MTQ_CLASS::MTQA_DCM_[3][3];      //plus or minus sign x,y,z
constexpr float MTQ_CLASS::MTQO_DCM_[3][3];      //plus or minus sign x,y,z
constexpr float MTQ_CLASS::nS_mtqxy2U_;
constexpr float MTQ_CLASS::nS_mtqxy1U_;
constexpr float MTQ_CLASS::nS_mtqz2U_;
constexpr float MTQ_CLASS::nS_mtqz1U_;
constexpr float MTQ_CLASS::nS_mtqo2U_;
constexpr float MTQ_CLASS::kt_mtqx2U_[4];
constexpr float MTQ_CLASS::kt_mtqy2U_[4];
constexpr float MTQ_CLASS::kt_mtqz2U_[4];
constexpr float MTQ_CLASS::kt_mtqx1U_[4];
constexpr float MTQ_CLASS::kt_mtqy1U_[4];
constexpr float MTQ_CLASS::kt_mtqz1U_[4];
constexpr float MTQ_CLASS::R_mtqx2U_;
constexpr float MTQ_CLASS::R_mtqy2U_;
constexpr float MTQ_CLASS::R_mtqz2U_;
constexpr float MTQ_CLASS::R_mtqx1U_;
constexpr float MTQ_CLASS::R_mtqy1U_;
constexpr float MTQ_CLASS::R_mtqz1U_;
constexpr float MTQ_CLASS::current_limit_mtq2U_[4];
constexpr float MTQ_CLASS::current_limit_mtq1U_[3];
constexpr uint16_t MTQ_CLASS::MTQ_LOWER_THRESHOLD_[4];
constexpr uint16_t MTQ_CLASS::MTQ_UPPER_THRESHOLD_[4];
constexpr uint8_t MTQ_CLASS::hispin_contorol_flag_mtq2U_[3];
constexpr uint8_t MTQ_CLASS::hispin_contorol_flag_mtq1U_[3];
// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS END*** //

/**
 * MTQA初期化関数
 * MTQX,Y,ZのDACをゼロ点出力の状態にする
 */
void MTQ_CLASS::MTQA_init()
{
  //print(MTQ, "ACS2 DAC ALL set to 0 current.\r\n");
  uint16_t _cmd_val_int_ = (uint16_t)(2.5/5*16384);
  uint8_t data_ptr[2];
  data_ptr[0] = 0b11000000 | (_cmd_val_int_ >> 8);
  data_ptr[1] = _cmd_val_int_ & 0xFF;
  for (uint8_t buff_ch = 0; buff_ch < 3; buff_ch++)
  {
	  ACS_MTQ.Write(buff_ch, 1, data_ptr, 2);
  }
}

/**
 * MTQO初期化関数
 * MTQOのDACをゼロ点出力の状態にする
 */
void MTQ_CLASS::MTQO_init()
{
  //print(MTQ,"ACS2 DAC ALL set to 0 current.\r\n");
  uint16_t _cmd_val_int_ = (uint16_t)(2.5/5*16384);
  uint8_t data_ptr[2];
  data_ptr[0] = 0b11000000 | (_cmd_val_int_ >> 8);
  data_ptr[1] = _cmd_val_int_ & 0xFF;
  for (uint8_t buff_ch = 3; buff_ch < 4; buff_ch++)
  {
		ACS_MTQ.Write(buff_ch, 1, data_ptr, 2);
  }
}

/**
 * MTQX,Y,Z電圧制御（デバッグ用）
 */
void MTQ_CLASS::MTQA_FunctionCore4debug(const float Vol[3])
{

	float absrng[3];

	for(int i = 0; i < 3; i ++){
		absrng[i] = Vol[i]; // -1 ~ 1
	}

	for(int buff_ch = 0; buff_ch < 3; buff_ch++){
		float _cmd_val_ = absrng[buff_ch] * 2.5 + 2.5; // 0 ~ 5[V]
		uint16_t _cmd_val_int_ = (uint16_t)(_cmd_val_/5*16384);

		if(_cmd_val_int_ > 16319) _cmd_val_int_ = 16383;
		else if(_cmd_val_int_ < 0) _cmd_val_int_ = 0;

		MTQ_Read_Current(buff_ch); //電流値計測

		uint8_t data_ptr[2];
		data_ptr[0] = 0b11000000 | (_cmd_val_int_ >> 8);
		data_ptr[1] = _cmd_val_int_ & 0xFF;

		ACS_MTQ.Write(buff_ch, 1, data_ptr, 2);
	}
}

/**
 * MTQO電圧制御（デバッグ用）
 */
void MTQ_CLASS::MTQO_FunctionCore4debug(float Vol)
{

	float absrng = Vol; // -1 ~ 1

	float _cmd_val_ = absrng * 2.5 + 2.5; // 0 ~ 5[V]
	uint16_t _cmd_val_int_ = (uint16_t)(_cmd_val_/5*16384);
	if(_cmd_val_int_ > 16319) _cmd_val_int_ = 16383;
	else if(_cmd_val_int_ < 0) _cmd_val_int_ = 0;
	uint8_t data_ptr[2];
	data_ptr[0] = 0b11000000 | (_cmd_val_int_ >> 8);
	data_ptr[1] = _cmd_val_int_ & 0xFF;

	for (uint8_t buff_ch = 3; buff_ch < 4; buff_ch++) //buff_ch 1->MTQX 2->MYQY 3->MTQZ1 4->MTQZ2,Z3,Z4
	{
		MTQ_Read_Current(buff_ch); //電流値計測
		ACS_MTQ.Write(buff_ch, 1, data_ptr, 2);
	}
}

/**
 * MTQX,Y,Z制御則
 * 一定磁気モーメント出力制御
 * AOCSで計算された磁気モーメント制御
 */
void MTQ_CLASS::MTQA_FunctionCore(const float Mag_moment[3])
{
	//MTQX
	if(ACS_Mode(CONST_MAGMOMENT) && ACS_Mode(OUTPUT_MTQA_X)){
#ifdef TWO_U
		//initialize parameter of CURS_SENSOR_AD7654
	    cur_class.CURS_SENSOR_init();
#endif //for 2U
	    MTQAX_Const_Mag_Moment_Control();
	}
	//通常は制御則から算出された磁気モーメントにより動作
	else if (!ACS_Mode(CONST_MAGMOMENT) && ACS_Mode(OUTPUT_MTQA_X)){
		MTQA_X(Mag_moment[0]);
	}
	else {
		curs_cmd_val_int_[0] = 8192;
	}

	// MTQY
	if(ACS_Mode(CONST_MAGMOMENT) && ACS_Mode(OUTPUT_MTQA_Y)){
		MTQAY_Const_Mag_Moment_Control();
	}
	else if (!ACS_Mode(CONST_MAGMOMENT) && ACS_Mode(OUTPUT_MTQA_Y)){
		MTQA_Y(Mag_moment[1]);
	}
	else {
		curs_cmd_val_int_[1] = 8192;
	}

	// MTQZ
	if(ACS_Mode(CONST_MAGMOMENT) && ACS_Mode(OUTPUT_MTQA_Z)){
		MTQAZ_Const_Mag_Moment_Control();
	}
	else if (!ACS_Mode(CONST_MAGMOMENT) && ACS_Mode(OUTPUT_MTQA_Z)){
		MTQA_Z(Mag_moment[2]);
	}
	else {
		curs_cmd_val_int_[2] = 8192;
	}
}

/**
 * MTQO制御則
 * 一定磁気モーメント出力制御
 * AOCSで計算された磁気モーメント制御
 */
void MTQ_CLASS::MTQO_FunctionCore(float Mag_moment)
{
	// MTQO
	// OUTPUT_MTQA_XのようなAOCSフラグはない　FET_MTQOのオンオフで操作する
	if(ACS_Mode(OUTPUT_CONST_MTQO)){
		MTQO_Const_Mag_Moment_Control();
	}
	else {
		MTQO_Z(Mag_moment);
	}

}

/**
 * MTQX,Y,Z制御モード
 * 高速スピンモード：目標磁気モーメントを目標電流値に変換し，V=IRよりDAC出力電圧を計算する
 * 通常電流制御モード：目標磁気モーメントを目標電流値に変換し，目標電流値に追従させるPID制御を行う
 */
void MTQ_CLASS::MTQA_X(float Mag_moment)
{

    if(user_mode->current_mode_id == MD_AOCSFLAG_HIGH_RATE_SPIN){ //high spin rate mode

    	MTQ_Calculate_HighSpin_Control(0, Mag_moment);
    }
    else { // normal current control mode

//    	print(MTQ, "Mag_momentX:%f\r\n", Mag_moment);
#ifdef TWO_U
    	float Current_MTQX = Mag_moment / nS_mtqxy2U;
#endif // for 2U
#ifdef ONE_U
    	float Current_MTQX = Mag_moment / nS_mtqxy1U;
#endif // for 1U

    	MTQ_Target_Current_Setting(0, Current_MTQX);

    	MTQ_Read_Current(0);

    	MTQ_Calculate_PID_Control(0, target_curs_mtqx_int, curs_mtqx_int);
    }
}

void MTQ_CLASS::MTQA_Y(float Mag_moment)
{

    if(user_mode->current_mode_id == MD_AOCSFLAG_HIGH_RATE_SPIN){ //high spin rate mode

    	MTQ_Calculate_HighSpin_Control(1, Mag_moment);
    }
    else { // normal current control mode

//    	print(MTQ, "Mag_momentY:%f\r\n", Mag_moment);
#ifdef TWO_U
    	float Current_MTQY = Mag_moment / nS_mtqxy2U;
#endif // for 2U
#ifdef ONE_U
    	float Current_MTQY = Mag_moment / nS_mtqxy1U;
#endif // for 1U

    	MTQ_Target_Current_Setting(1, Current_MTQY);

    	MTQ_Read_Current(1);

    	MTQ_Calculate_PID_Control(1, target_curs_mtqy_int, curs_mtqy_int);
    }
}


void MTQ_CLASS::MTQA_Z(float Mag_moment)
{

    if(user_mode->current_mode_id == MD_AOCSFLAG_HIGH_RATE_SPIN){ //high spin rate mode

    	MTQ_Calculate_HighSpin_Control(2, Mag_moment);
    }
    else { // normal current control mode
//    	print(MTQ, "Mag_momentZ:%f\r\n", Mag_moment);
#ifdef TWO_U
    	float Current_MTQZ = Mag_moment / nS_mtqz2U;
#endif // for 2U
#ifdef ONE_U
    	float Current_MTQZ = Mag_moment / nS_mtqz1U;
#endif // for 1U

    	MTQ_Target_Current_Setting(2, Current_MTQZ);

    	MTQ_Read_Current(2);

    	MTQ_Calculate_PID_Control(2, target_curs_mtqz_int, curs_mtqz_int);
    }
}

/**
 * MTQO制御モード
 * 通常電流制御モード：目標磁気モーメントを目標電流値に変換し，目標電流値に追従させるPID制御を行う
 */
void MTQ_CLASS::MTQO_Z(float Mag_moment)
{
	float Current_MTQO = Mag_moment / nS_mtqo2U;  //Test value

	MTQ_Target_Current_Setting(3, Current_MTQO);

	MTQ_Read_Current(3);

	MTQ_Calculate_PID_Control(3, target_curs_mtqo_int, curs_mtqo_int);
}


/**
 * 高速スピンモード：目標磁気モーメントを目標電流値に変換し，V=IRよりDAC出力電圧を計算する
 */
void MTQ_CLASS::MTQ_Calculate_HighSpin_Control(int i, float Mag_moment)
{
#ifdef TWO_U
	if (i == 0){ // 2U MTQX
    	if (hispin_contorol_flag_mtq2U[i] == 0) { // constant resistance
        	float Current_MTQX = Mag_moment / nS_mtqxy2U;
        	float Volt_MTQX = Current_MTQX * R_mtqx2U;  //V = I*R
        	curs_cmd_val_int_[0] = (uint16_t)((Volt_MTQX + 2.5f)/5*16384);
    	}
    	else { // compensate temperature
        	float Current_MTQX = Mag_moment / nS_mtqxy2U;
        	MTQ_Target_Current_Setting(0, Current_MTQX);
    		float t = 0.1f * txpic_driver->tlm_data.txpic_to_mobc.temp.strmx - 50.0f; // 0.1 = 4.096 / 2^12 * 100
    		curs_cmd_val_int_[0] = (uint16_t)((kt_mtqx2U[0] * t + kt_mtqx2U[1]) * target_curs_mtqx + (kt_mtqx2U[2] * t + kt_mtqx2U[3]));
    	}
 	}
	else if (i == 1){ // 2U MTQY
    	if (hispin_contorol_flag_mtq2U[i] == 0) { // constant resistance
        	float Current_MTQY = Mag_moment / nS_mtqxy2U;
        	float Volt_MTQY = Current_MTQY * R_mtqy2U;  //V = I*R
        	curs_cmd_val_int_[1] = (uint16_t)((Volt_MTQY + 2.5f)/5*16384);
    	}
    	else { // compensate temperature
    		float Current_MTQY = Mag_moment / nS_mtqxy2U;
    		MTQ_Target_Current_Setting(1, Current_MTQY);
    	    float t = 0.1f * txpic_driver->tlm_data.txpic_to_mobc.temp.strpy - 50.0f; // 0.1 = 4.096 / 2^12 * 100
    	    curs_cmd_val_int_[1] = (uint16_t)((kt_mtqy2U[0] * t + kt_mtqy2U[1]) * target_curs_mtqy + (kt_mtqy2U[2] * t + kt_mtqy2U[3]));
    	}
	}
	else if (i == 2){ // 2U MTQZ
    	if (hispin_contorol_flag_mtq2U[i] == 0) { // constant resistance
    		float Current_MTQZ;
    		if(fabsf(Mag_moment) <= 0.08){
    			Current_MTQZ = fabsf(Mag_moment) / nS_mtqz2U;
    		}
    		else{
    			Current_MTQZ = 0.5 * (18.929 + sqrtf(358.307041 + 152.568 * (fabsf(Mag_moment) - 0.2121312))) / 38.142;
    		}
    		float Volt_MTQZ = Current_MTQZ * R_mtqz2U;  //V = I*R
    		//極性で変更
    		if(Mag_moment >= 0.0f){
    			curs_cmd_val_int_[2] = (uint16_t)((Volt_MTQZ + 2.5f)/5*16384);
    		}
    		else{
    			curs_cmd_val_int_[2] = (uint16_t)((-Volt_MTQZ + 2.5f)/5*16384);
    		}
    	}
    	else { // compensate temperature
    		float Current_MTQZ = Mag_moment / nS_mtqz2U;
    	   	MTQ_Target_Current_Setting(2, Current_MTQZ);
    		float t = 0.1f * txpic_driver->tlm_data.txpic_to_mobc.temp.strpz - 50.0f; // 0.1 = 4.096 / 2^12 * 100
    		curs_cmd_val_int_[2] = (uint16_t)((kt_mtqz2U[0] * t + kt_mtqz2U[1]) * target_curs_mtqz + (kt_mtqz2U[2] * t + kt_mtqz2U[3]));
    	}
	}
#endif

#ifdef ONE_U
		if (i == 0){ // 1U MTQX
	    	if (hispin_contorol_flag_mtq1U[i] == 0) { // constant resistance
	    		float Current_MTQX = Mag_moment / nS_mtqxy1U;
	        	float Volt_MTQX = Current_MTQX * R_mtqx1U;  //V = I*R
	        	curs_cmd_val_int_[0] = (uint16_t)((Volt_MTQX + 2.5f)/5*16384);
	    	}
	    	else { // compensate temperature
	        	float Current_MTQX = Mag_moment / nS_mtqxy1U;
	        	MTQ_Target_Current_Setting(0, Current_MTQX);
	    		float t = 0.1f * txpic_driver->tlm_data.txpic_to_mobc.temp.strmx - 50.0f; // 0.1 = 4.096 / 2^12 * 100
	    		curs_cmd_val_int_[0] = (uint16_t)((kt_mtqx1U[0] * t + kt_mtqx1U[1]) * target_curs_mtqx + (kt_mtqx1U[2] * t + kt_mtqx1U[3]));
	    	}
		}
		else if (i == 1){ // 1U MTQY
	    	if (hispin_contorol_flag_mtq1U[i] == 0) { // constant resistance
	    		float Current_MTQY = Mag_moment / nS_mtqxy1U;
	        	float Volt_MTQY = Current_MTQY * R_mtqy1U;  //V = I*R
	        	curs_cmd_val_int_[1] = (uint16_t)((Volt_MTQY + 2.5f)/5*16384);
	    	}
	    	else { // compensate temperature
	        	float Current_MTQY = Mag_moment / nS_mtqxy1U;
	        	MTQ_Target_Current_Setting(1, Current_MTQY);
	    		float t = 0.1f * txpic_driver->tlm_data.txpic_to_mobc.temp.strpy - 50.0f; // 0.1 = 4.096 / 2^12 * 100
	    		curs_cmd_val_int_[1] = (uint16_t)((kt_mtqy1U[0] * t + kt_mtqy1U[1]) * target_curs_mtqy + (kt_mtqy1U[2] * t + kt_mtqy1U[3]));
	    	}
		}
		else if (i == 2){ // 1U MTQZ
	    	if (hispin_contorol_flag_mtq1U[i] == 0) { // constant resistance
	    		float Current_MTQZ = Mag_moment / nS_mtqz1U;
	        	float Volt_MTQZ = Current_MTQZ * R_mtqz1U;  //V = I*R
	        	curs_cmd_val_int_[2] = (uint16_t)((Volt_MTQZ + 2.5f)/5*16384);
	    	}
	    	else { // compensate temperature
	        	float Current_MTQZ = Mag_moment / nS_mtqz1U;
	        	MTQ_Target_Current_Setting(2, Current_MTQZ);
	    		float t = 0.1f * txpic_driver->tlm_data.txpic_to_mobc.temp.strpz - 50.0f; // 0.1 = 4.096 / 2^12 * 100
	    		curs_cmd_val_int_[2] = (uint16_t)((kt_mtqz1U[0] * t + kt_mtqz1U[1]) * target_curs_mtqz + (kt_mtqz1U[2] * t + kt_mtqz1U[3]));
	    	}
		}
#endif

}

/**
 * 目標電流値の設定
 * target_curs_mtqx     : float型の目標電流値[mA]
 * target_curs_mtqx_int : uint16_t型の目標電流値[-]
 */
void MTQ_CLASS::MTQ_Target_Current_Setting(int i, float Target_current)
{

#ifdef TWO_U

	if (i == 0){
		target_curs_mtqx = Target_current;
		print(MTQ, "target_curs_mtqx:%f\r\n", target_curs_mtqx);
		// 2U MTQX Target Current Range　-100 ~ 100 mA -> Target Mag Moment Range TBD ~ TBD Am^2
		if (target_curs_mtqx > current_limit_mtq2U[i]) target_curs_mtqx = current_limit_mtq2U[i];
		else if (target_curs_mtqx < -current_limit_mtq2U[i]) target_curs_mtqx = -current_limit_mtq2U[i];
		//change MTQ target current (mA -> uint16_t)
//		target_curs_mtqx_int = (uint16_t)((target_curs_mtqx * 0.01f + 2.5f) / 5.0f * 65535);
		target_curs_mtqx_int = (uint16_t)((target_curs_mtqx * 0.0036f + 2.5f) / 5.0f * 65535);
	}
	else if (i == 1){
		target_curs_mtqy = Target_current;
		print(MTQ, "target_curs_mtqy:%f\r\n", target_curs_mtqy);
		// 2U MTQY Target Current Range　-100 ~ 100 mA -> Target Mag Moment Range TBD ~ TBD Am^2
		if (target_curs_mtqy > current_limit_mtq2U[i]) target_curs_mtqy = current_limit_mtq2U[i];
		else if (target_curs_mtqy < -current_limit_mtq2U[i]) target_curs_mtqy = -current_limit_mtq2U[i];
		//change MTQ target current (mA -> uint16_t)
//		target_curs_mtqy_int = (uint16_t)((target_curs_mtqy * 0.01f + 2.5f) / 5.0f * 65535);
		target_curs_mtqy_int = (uint16_t)((target_curs_mtqy * 0.0036f + 2.5f) / 5.0f * 65535);
	}
	else if (i == 2){
		target_curs_mtqz = Target_current;
		print(MTQ, "target_curs_mtqz:%f\r\n", target_curs_mtqz);
		// 2U MTQZ Target Current Range　-200 ~ 200 mA -> Target Mag Moment Range TBD ~ TBD Am^2
		if (target_curs_mtqz > current_limit_mtq2U[i]) target_curs_mtqz = current_limit_mtq2U[i];
		else if (target_curs_mtqz < -current_limit_mtq2U[i]) target_curs_mtqz = -current_limit_mtq2U[i];
		//change MTQ target current (mA -> uint16_t)
//		target_curs_mtqz_int = (uint16_t)((target_curs_mtqz * 0.01f + 2.5f) / 5.0f * 65535);
		target_curs_mtqz_int = (uint16_t)((target_curs_mtqz * 0.0091f + 2.5f) / 5.0f * 65535);
	}
	else if (i == 3){
		target_curs_mtqo = Target_current;
		// 2U MTQO Target Current Range　-200 ~ 200 mA -> Target Mag Moment Range TBD ~ TBD Am^2
		if (target_curs_mtqo > current_limit_mtq2U[i]) target_curs_mtqo = current_limit_mtq2U[i];
		else if (target_curs_mtqo < -current_limit_mtq2U[i]) target_curs_mtqo = -current_limit_mtq2U[i];
		//change MTQ target current (mA -> uint16_t)
//		target_curs_mtqo_int = (uint16_t)((target_curs_mtqo * 0.01f + 2.5f) / 5.0f * 65535);
		target_curs_mtqo_int = (uint16_t)((target_curs_mtqo * 0.0091f + 2.5f) / 5.0f * 65535);
	}

#endif //for 2U

#ifdef ONE_U
	if (i == 0){
		target_curs_mtqx = Target_current;
		// 1U MTQX Target Current Range　-150 ~ 150 mA -> Target Mag Moment Range TBD ~ TBD Am^2
		if (target_curs_mtqx > current_limit_mtq1U[i]) target_curs_mtqx = current_limit_mtq1U[i];
		else if (target_curs_mtqx < -current_limit_mtq1U[i]) target_curs_mtqx = -current_limit_mtq1U[i];
		//change MTQ target current (mA -> uint16_t)
//		target_curs_mtqx_int = (uint16_t)((target_curs_mtqx + 746.576) / 0.2441);
		target_curs_mtqx_int = (uint16_t)((target_curs_mtqx * 0.0036 + 2.5) * 4096 / (3.3 * 2));
	}
	else if (i == 1){
		target_curs_mtqy = Target_current;
		// 1U MTQY Target Current Range　-150 ~ 150 mA -> Target Mag Moment Range TBD ~ TBD Am^2
		if (target_curs_mtqy > current_limit_mtq1U[i]) target_curs_mtqy = current_limit_mtq1U[i];
		else if (target_curs_mtqy < -current_limit_mtq1U[i]) target_curs_mtqy = -current_limit_mtq1U[i];
		//change MTQ target current (mA -> uint16_t)
//		target_curs_mtqy_int = (uint16_t)((target_curs_mtqy + 746.576) / 0.2441);
		target_curs_mtqy_int = (uint16_t)((target_curs_mtqy * 0.0036 + 2.5) * 4096 / (3.3 * 2));
	}
	else if (i == 2){
		target_curs_mtqz = Target_current;
		// 1U MTQZ Target Current Range　-200 ~ 200 mA -> Target Mag Moment Range TBD ~ TBD Am^2
		if (target_curs_mtqz > current_limit_mtq1U[i]) target_curs_mtqz = current_limit_mtq1U[i];
		else if (target_curs_mtqz < -current_limit_mtq1U[i]) target_curs_mtqz = -current_limit_mtq1U[i];
		//change MTQ target current (mA -> uint16_t)
//		target_curs_mtqz_int = (uint16_t)((target_curs_mtqz + 746.576) / 0.2441);
		target_curs_mtqz_int = (uint16_t)((target_curs_mtqz * 0.0091 + 2.5) * 4096 / (3.3 * 2));
	}
#endif //for 1U
}

/**
 * 電流値の読み取り
 * curs_mtqx_int : uint16_t型の電流値[-]
 */
void MTQ_CLASS::MTQ_Read_Current(int i)
{
#ifdef TWO_U
	if (i == 0){
		//read MTQX current
		cur_class.CURS_SENSOR_get_MTQX();
		print(MTQ, "CURS_MTQX, %u, %f\r\n", cur_class.ADC_curmtqx, (float)(cur_class.ADC_curmtqx * 0.021193f - 694.444f));// (5 / 65536 * ADC値 - 2.5) / 100 / 0.036 * 1000 [mA]
		curs_mtqx_int = cur_class.ADC_curmtqx;
	//	print(MTQ, "curs_mtqx_int is %u \r\n", cur_class.ADC_curmtqx);
	}
	else if (i == 1){
		//read MTQY current
		cur_class.CURS_SENSOR_get_MTQY();
		print(MTQ, "CURS_MTQY, %u, %f\r\n", cur_class.ADC_curmtqy, (float)(cur_class.ADC_curmtqy * 0.021193f - 694.444f));// (5 / 65536 * ADC値 - 2.5) / 100 / 0.036 * 1000 [mA]
		curs_mtqy_int = cur_class.ADC_curmtqy;
	//	print(MTQ, "curs_mtqy_int is %u \r\n", cur_class.ADC_curmtqy);
	}
	else if (i == 2){
		//read MTQZ current
		cur_class.CURS_SENSOR_get_MTQZ1();
		print(MTQ, "CURS_MTQZ1, %u, %f\r\n", cur_class.ADC_curmtqz1, (float)(cur_class.ADC_curmtqz1 * 0.00838395f - 274.725f));// (5 / 65536 * ADC値 - 2.5) / 100 / 0.091 * 1000 [mA]
		curs_mtqz_int = cur_class.ADC_curmtqz1;
	//  print(MTQ, "curs_mtqz_int is %u \r\n", cur_class.ADC_curmtqz1);
	}
	else if (i == 3){
		//read MTQO current
		cur_class.CURS_SENSOR_get_MTQZ2();
		print(MTQ, "CURS_MTQZ2, %u, %f\r\n", cur_class.ADC_curmtqz2, (float)(cur_class.ADC_curmtqz2 * 0.00838395f - 274.725f));// (5 / 65536 * ADC値 - 2.5) / 100 / 0.091 * 1000 [mA]
		cur_class.CURS_SENSOR_get_MTQZ3();
		print(MTQ, "CURS_MTQZ3, %u, %f\r\n", cur_class.ADC_curmtqz3, (float)(cur_class.ADC_curmtqz3 * 0.00838395f - 274.725f));// (5 / 65536 * ADC値 - 2.5) / 100 / 0.091 * 1000 [mA]
		cur_class.CURS_SENSOR_get_MTQZ4();
		print(MTQ, "CURS_MTQZ4, %u, %f\r\n", cur_class.ADC_curmtqz4, (float)(cur_class.ADC_curmtqz4 * 0.00838395f - 274.725f));// (5 / 65536 * ADC値 - 2.5) / 100 / 0.091 * 1000 [mA]
		curs_mtqo_int = cur_class.ADC_curmtqz3;
	//	print(MTQ, "curs_mtqo_int is %u \r\n", cur_class.ADC_curmtqz3);
	}
#endif // for 2U

#ifdef ONE_U
	if (i == 0){
		curs_mtqx_int = Get_ADC_Value(ORDER_CURS_MTQX);//pwr_adc_mobc.ADC_I_MTQX;
	//	print(MTQ, "ADC_CURS_MTQX is %02x \r\n", curs_mtqx_int>>4);
		print(MTQ, "CURS_MTQX, %u, %f\r\n", curs_mtqx_int, (float)(curs_mtqx_int * 0.44759f - 694.444f)); // (3.3 / 4096 * 2 * ADC値 - 2.5) / 100 / 0.036 * 1000 [mA]
	//	printf("MTQ_CURS MTQX : %02x \r\n", curs_mtqx_int>>4);
	}
	else if (i == 1){
		curs_mtqy_int = Get_ADC_Value(ORDER_CURS_MTQY);//pwr_adc_mobc.ADC_I_MTQY;
	//	print(MTQ, "ADC_CURS_MTQY is %02x \r\n", curs_mtqy_int>>4);
		print(MTQ, "CURS_MTQY, %u, %f\r\n", curs_mtqy_int, (float)(curs_mtqy_int * 0.44759f - 694.444f));  // (3.3 / 4096 * 2 * ADC値 - 2.5) / 100 / 0.036 * 1000 [mA]
	//	printf("MTQ_CURS MTQY : %02x \r\n", curs_mtqy_int>>4);
	}
	else if (i == 2){
		curs_mtqz_int = Get_ADC_Value(ORDER_CURS_MTQZ1Z2);
	//	print(MTQ, "ADC_CURS_MTQZ is %02x \r\n", curs_mtqz_int>>4);
		print(MTQ, "CURS_MTQZ1Z2, %u, %f\r\n", curs_mtqz_int, (float)(curs_mtqz_int * 0.17707f - 274.725f));  // (3.3 / 4096 * 2 * ADC値 - 2.5) / 100 / 0.091 * 1000 [mA]
	//	printf("MTQ_CURS MTQZ : %02x \r\n", curs_mtqz_int>>4);

		curs_mtqz_int = Get_ADC_Value(ORDER_CURS_MTQZ3Z4);
	//	print(MTQ, "ADC_CURS_MTQZ is %02x \r\n", curs_mtqz_int>>4);
		print(MTQ, "CURS_MTQZ3Z4, %u, %f\r\n", curs_mtqz_int, (float)(curs_mtqz_int * 0.17707f - 274.725f));  // (3.3 / 4096 * 2 * ADC値 - 2.5) / 100 / 0.091 * 1000 [mA]
	//	printf("MTQ_CURS MTQZ : %02x \r\n", curs_mtqz_int>>4);
	}
	else if (i == 3){
		curs_mtqz_int = Get_ADC_Value(ORDER_CURS_MTQZ1Z2);
	//	print(MTQ, "ADC_CURS_MTQZ is %02x \r\n", curs_mtqz_int>>4);
		print(MTQ, "CURS_MTQZ1Z2, %u, %f\r\n", curs_mtqz_int, (float)(curs_mtqz_int * 0.17707f - 274.725f));  // (3.3 / 4096 * 2 * ADC値 - 2.5) / 100 / 0.091 * 1000 [mA]
	//	printf("MTQ_CURS MTQZ : %02x \r\n", curs_mtqz_int>>4);

		curs_mtqz_int = Get_ADC_Value(ORDER_CURS_MTQZ3Z4);
	//	print(MTQ, "ADC_CURS_MTQZ is %02x \r\n", curs_mtqz_int>>4);
		print(MTQ, "CURS_MTQZ3Z4, %u, %f\r\n", curs_mtqz_int, (float)(curs_mtqz_int * 0.17707f - 274.725f));  // (3.3 / 4096 * 2 * ADC値 - 2.5) / 100 / 0.091 * 1000 [mA]
	//	printf("MTQ_CURS MTQZ : %02x \r\n", curs_mtqz_int>>4);
	}
#endif // for 1U
}

/**
 * PID制御のゲイン設定
 */
void MTQ_CLASS::MTQ_PID_Param_Setting()
{
#ifdef TWO_U
	k_P[0] = k_PID_mtqx2U[0]; k_I[0] = k_PID_mtqx2U[1]; k_D[0] = k_PID_mtqx2U[2]; //PID gain for MTQX 2U
	k_P[1] = k_PID_mtqy2U[0]; k_I[1] = k_PID_mtqy2U[1]; k_D[1] = k_PID_mtqy2U[2]; //PID gain for MTQY 2U
	k_P[2] = k_PID_mtqz2U[0]; k_I[2] = k_PID_mtqz2U[1]; k_D[2] = k_PID_mtqz2U[2]; //PID gain for MTQZ 2U
	k_P[3] = k_PID_mtqo2U[0]; k_I[3] = k_PID_mtqo2U[1]; k_D[3] = k_PID_mtqo2U[2]; //PID gain for MTQO 2U
#endif //for 2U

#ifdef ONE_U
	k_P[0] = k_PID_mtqx1U[0]; k_I[0] = k_PID_mtqx1U[1]; k_D[0] = k_PID_mtqx1U[2]; //PID gain for MTQX 1U
	k_P[1] = k_PID_mtqy1U[0]; k_I[1] = k_PID_mtqy1U[1]; k_D[1] = k_PID_mtqy1U[2]; //PID gain for MTQY 1U
	k_P[2] = k_PID_mtqz1U[0]; k_I[2] = k_PID_mtqz1U[1]; k_D[2] = k_PID_mtqz1U[2]; //PID gain for MTQZ 1U
#endif //for 1U
}

/**
 * PID制御則
 * 目標電流値と現在の電流値の偏差をフィードバック
 */
void MTQ_CLASS::MTQ_Calculate_PID_Control(int i, float target_curs_mtq_int, float curs_mtq_int)
{
	if (abs(target_curs_mtq_int - curs_mtq_int) > 5){

      P[i] = target_curs_mtq_int - curs_mtq_int;
      I[i] += P[i] * dt;
      D[i] = (P[i] - preP[i]) / dt;

      if (i < 2 ){ // MTQX,の場合　反転増幅回路のため符号を反転させる
        curs_cmd_val_int_[i] -= k_P[i] * P[i] + k_I[i] * I[i] + k_D[i] * D[i];
      }
      else { // MTQZ,Oの場合
        curs_cmd_val_int_[i] += k_P[i] * P[i] + k_I[i] * I[i] + k_D[i] * D[i];
      }

      preP[i] = P[i];
    }
	else{
		print(MTQ, "Current control is completed about MTQ[%d] \r\n", i);
	}
}

/**
 * MTQX,Y,ZのDCM計算
 */
void MTQ_CLASS::MTQA_DCM_Calc(uint16_t cmd_val_[4])
{
	uint16_t temp_[4] = {0};
	for (int i = 0; i < 3; i++) {
		temp_[i] = cmd_val_[i];
		cmd_val_[i] = 0;
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cmd_val_[i] += MTQA_DCM[i][j] * temp_[j];
		}

// MTQの制御量がDACの出力限界を超えるのはMAGNAROの設計思想（制御則で制限をかけずMTQの限界まで出力する）では正常なのでアノマリから外す 2022/08/20 tamaoki
//		 if(MTQA_anomaly_counter > 0){
//			 --MTQA_anomaly_counter;
//		 }
//		 else{
//			// Anomaly detection ：comparing with Threshold
//			if(cmd_val_[i] > MTQ_UPPER_THRESHOLD[i] || cmd_val_[i] < MTQ_LOWER_THRESHOLD [i]){
//				printf("cmd_val[%d] = %d  THRES[%d] = %d \r\n",i,cmd_val_[i],i,MTQ_UPPER_THRESHOLD[i]);
//				printf("cmd_val[%d] = %d  THRES[%d] = %d \r\n",i,cmd_val_[i],i,MTQ_LOWER_THRESHOLD[i]);
//				printf("MTQA_OT_add_anomaly \r\n");
//				mtq_trans_set(&mtqa_anomaly);
//				AL_add_anomaly(AL_GROUP_MTQ,AL_OT_MTQA);
//			}
//		}

		if(cmd_val_[i] > 16383) cmd_val_[i] = 16383;
		else if(cmd_val_[i] < 0) cmd_val_[i] = 0;

		MTQ_Output(i, cmd_val_[i]);
	}
}

/**
 * MTQOのDCM計算
 */
void MTQ_CLASS::MTQO_DCM_Calc(uint16_t cmd_val_[4])
{
	uint16_t temp_[4] = {0};
	for (int i = 0; i < 4; i++) {
		temp_[i] = cmd_val_[i];
	}
	cmd_val_[3] = 0;

//	curs_ctrl_int_[3] = 0; //initialize MTQO
	for (int j = 0; j < 2; j++) {
		cmd_val_[3] += MTQO_DCM[2][j] * temp_[j]; //MTQX,Y
	}
	cmd_val_[3] += MTQO_DCM[2][2] * temp_[3]; //MTQOf

//	 if(MTQO_anomaly_counter > 0){
//		 --MTQO_anomaly_counter;
//	 }
//	else{
//		// Anomaly detection ：comparing with Threshold
//		if(cmd_val_[3] > MTQ_UPPER_THRESHOLD[3] || cmd_val_[3] < MTQ_LOWER_THRESHOLD [3]){
//			print(ANOMALY, "cmd_val[3] = %d  THRES[3] = %d \r\n",cmd_val_[3],MTQ_UPPER_THRESHOLD[3]);
//			mtq_trans_set(&mtqo_anomaly);
//			print(ANOMALY,"MTQO_OT_add_anomaly \r\n");
//			AL_add_anomaly(AL_GROUP_MTQ,AL_OT_MTQO);
//		}
//	}


	if(cmd_val_[3] > 16383) cmd_val_[3] = 16383;
	else if(cmd_val_[3] < 0) cmd_val_[3] = 0;

	MTQ_Output(3, cmd_val_[3]);
}

/**
 * MTQ出力
 * curs_cmd_val_int_ | DAC出力(電圧値)[V]
 * --------------------------------------
 *          0        |       0.0
 *       8192        |       2.5
 *      16383        |       5.0
 */
void MTQ_CLASS::MTQ_Output(int bfc, uint16_t mtq_output)
{
	int buff_ch = bfc; // buff_ch = 0->MTQX 1->MTQY 2->MTQZ 3->MTQO
	uint16_t  curs_cmd_val_int_ = mtq_output;
	uint8_t data_ptr[2];
	// MTQ output
	if ( curs_cmd_val_int_ > 16383) curs_cmd_val_int_ = 16383;
	else if ( curs_cmd_val_int_ < 0) curs_cmd_val_int_ = 0;

	data_ptr[0] = 0b11000000 | (curs_cmd_val_int_ >> 8);
	data_ptr[1] = curs_cmd_val_int_ & 0xFF;
	ACS_MTQ.Write(buff_ch, 1, data_ptr, 2);
}

/**
 * MTQX一定磁気モーメント制御
 */
void MTQ_CLASS::MTQAX_Const_Mag_Moment_Control()
{
#ifdef TWO_U
	//read MTQX current
	cur_class.CURS_SENSOR_get_MTQX();
	print(MTQ, "const_curs_mtqx is %f [mA] \r\n", cur_class.curmtqx * 100.0f - 250.0f);
	curs_mtqx_int = cur_class.ADC_curmtqx;
//	print(MTQ, "curs_mtqx_int is %u \r\n", cur_class.ADC_curmtqx);
#endif // for 2U

#ifdef ONE_U
	curs_mtqx_int = Get_ADC_Value(7);//pwr_adc_mobc.ADC_I_MTQX;
//	print(MTQ, "ADC_CURS_MTQX is %02x \r\n", curs_mtqx_int>>4);
	print(MTQ, "curs_mtqx is %f [mA] \r\n", (float)(curs_mtqx_int * 0.2441 - 746.576));
//	printf("MTQ_CURS MTQX : %02x \r\n", curs_mtqx_int>>4);
#endif // for 1U

	// MTQ target current
#ifdef TWO_U
	float Current_MTQX = target_mag_moment_mtq2U[0] / nS_mtqxy2U;
	MTQ_Target_Current_Setting(0, Current_MTQX);
#endif // for 2U

#ifdef ONE_U
	float Current_MTQX = target_mag_moment_mtq1U[0] / nS_mtqxy1U;
	MTQ_Target_Current_Setting(0, Current_MTQX);
#endif // for 1U

	// conduct when the difference between MTQ target current and MTQ current is over 5
	MTQ_Calculate_PID_Control(0, target_curs_mtqx_int, curs_mtqx_int);
}

/**
 * MTQY一定磁気モーメント制御
 */
void MTQ_CLASS::MTQAY_Const_Mag_Moment_Control()
{
#ifdef TWO_U
	//read MTQX current
	cur_class.CURS_SENSOR_get_MTQY();
	print(MTQ, "curs_mtqy is %f [mA] \r\n", cur_class.curmtqy * 100.0f - 250.0f);
	curs_mtqy_int = cur_class.ADC_curmtqy;
//	print(MTQ, "curs_mtqy_int is %u \r\n", cur_class.ADC_curmtqy);
#endif // for 2U

#ifdef ONE_U
	curs_mtqy_int = Get_ADC_Value(8);//pwr_adc_mobc.ADC_I_MTQY;
//	print(MTQ, "ADC_CURS_MTQY is %02x \r\n", curs_mtqy_int>>4);
	print(MTQ, "curs_mtqy is %f [mA] \r\n", (float)(curs_mtqy_int * 0.2441 - 746.576));
//	printf("MTQ_CURS MTQY : %02x \r\n", curs_mtqy_int>>4);
#endif // for 1U

#ifdef TWO_U
	float Current_MTQY = target_mag_moment_mtq2U[1] / nS_mtqxy2U;
	MTQ_Target_Current_Setting(1, Current_MTQY);
#endif // for 2U

#ifdef ONE_U
	float Current_MTQY = target_mag_moment_mtq1U[1] / nS_mtqxy1U;
	MTQ_Target_Current_Setting(1, Current_MTQY);
#endif // for 1U
//	printf("target_curs_mtqy is %d [mA] \r\n", target_curs_mtqy);
//  print(MTQ, "target_curs_mtqy_int is %u \r\n", target_curs_mtqy_int);

	// conduct when the difference between MTQ target current and MTQ current is over 5
	MTQ_Calculate_PID_Control(1, target_curs_mtqy_int, curs_mtqy_int);
}

/**
 * MTQZ一定磁気モーメント制御
 */
void MTQ_CLASS::MTQAZ_Const_Mag_Moment_Control()
{
#ifdef TWO_U
	//read MTQX current
	cur_class.CURS_SENSOR_get_MTQZ1();
	print(MTQ, "curs_mtqz is %f [mA] \r\n", cur_class.curmtqz1 * 100.0f - 250.0f);
	curs_mtqz_int = cur_class.ADC_curmtqz1;
//    print(MTQ, "curs_mtqz_int is %u \r\n", cur_class.ADC_curmtqz1);
#endif // for 2U

#ifdef ONE_U
	curs_mtqz_int = Get_ADC_Value(9);//pwr_adc_mobc.ADC_I_MTQX;
//	print(MTQ, "ADC_CURS_MTQZ is %02x \r\n", curs_mtqz_int>>4);
//	print(MTQ, "pwr_adc_mobc.ADC_I_MTQX is %02x \r\n", pwr_adc_mobc.ADC_I_MTQX>>4);
	print(MTQ, "curs_mtqz is %f [mA] \r\n", (float)(curs_mtqz_int * 0.2441 - 746.576));
//	printf("MTQ_CURS MTQZ : %02x \r\n", curs_mtqz_int>>4);
#endif // for 1U

	// MTQ target current
#ifdef TWO_U
	float Current_MTQZ = target_mag_moment_mtq2U[2] / nS_mtqz2U;
	MTQ_Target_Current_Setting(2, Current_MTQZ);
#endif // for 2U

#ifdef ONE_U
	float Current_MTQZ = target_mag_moment_mtq1U[2] / nS_mtqz1U;
	MTQ_Target_Current_Setting(2, Current_MTQZ);
#endif // for 1U
//	printf("target_curs_mtqz is %d [mA] \r\n", target_curs_mtqz);
//  print(MTQ, "target_curs_mtqz_int is %u \r\n", target_curs_mtqz_int);

	// conduct when the difference between MTQ target current and MTQ current is over 5
	MTQ_Calculate_PID_Control(2, target_curs_mtqz_int, curs_mtqz_int);
}

/**
 * MTQO一定磁気モーメント制御
 */
#ifdef TWO_U
void MTQ_CLASS::MTQO_Const_Mag_Moment_Control()
{
#ifdef TWO_U
	//read MTQO current
	cur_class.CURS_SENSOR_get_MTQZ3();
	print(MTQ, "curs_mtqo is %f [mA] \r\n", cur_class.curmtqz3 * 100.0f - 250.0f);
	curs_mtqo_int = cur_class.ADC_curmtqz3;
//	print(MTQ, "curs_mtqo_int is %u \r\n", cur_class.ADC_curmtqz3);
#endif // for 2U

	float Current_MTQO = target_mag_moment_mtq2U[3] / nS_mtqo2U;
	MTQ_Target_Current_Setting(3, Current_MTQO);
//	printf("target_curs_mtqo is %d [mA] \r\n", target_curs_mtqo);
//  print(MTQ, "target_curs_mtqo_int is %u \r\n", target_curs_mtqo_int);

	// conduct when the difference between MTQ target current and MTQ current is over 5
	MTQ_Calculate_PID_Control(3, target_curs_mtqo_int, curs_mtqo_int);
}

#endif // for 2U

//アノマリフラグ用関数
//多数決を行い, 多いほうを採用する
uint8_t MTQ_CLASS::mtq_flag_vote(uint8_t* flag){
	uint8_t ret=0;
	for(int i=0;i<8;i++){
		ret+= ((*flag >> i) & 0x01);
	}
	if (ret>=4) {
		*flag = 0xff;
		return 1;
	}
	else {
		*flag = 0x00;
		return 0;
	}
}

void MTQ_CLASS::mtq_trans_set(uint8_t* flag){
	*flag = 0xff;
}

void MTQ_CLASS::mtqtrans_reset(uint8_t *flag){
	*flag = 0x00;
}
