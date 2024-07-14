/**
  *****************************************************************************************
  * @file    : ACS2_debug_MTQ.cpp
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
  * 2021/06/14  2.0   MASUDA
  *****************************************************************************************
  */

#include "ACS23_cmd_MTQ.h"

#include <src_user/IfWrapper/HAL.h>
#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Sensor/MTQ.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

static ACS23_DEBUG_MTQ_STRUCT acs23_debug_mtq;
const ACS23_DEBUG_MTQ_STRUCT* p_acs23_debug_mtq;
static MTQ_CLASS MTQ_Class;

static void ACS23_mtq_init(void);
static void MTQ_initialize_param(void);
static void MTQ_load_param(void);
static void ACS23_mtqa4_update_(void);
static void ACS23_mtqo4_update_(void);
static void ACS23_debug_MTQA_Function_core(uint8_t param[3]);
static void ACS23_debug_MTQO_Function_core(uint8_t param);

/**
 * @brief App making function
 *
 * @note
 * @return structure of App
 */
AppInfo ACS23_debug_mtq_update(void)
{
    return AI_create_app_info("DBG_mtq", ACS23_mtq_init, NULL);
}

AppInfo ACS23_MTQ_param_update(void)
{
    return AI_create_app_info("sample_param_init", MTQ_load_param, MTQ_initialize_param);
}

AppInfo ACS23_mtqa4_update(void)
{
    return AI_create_app_info("DBG_mtq", NULL, ACS23_mtqa4_update_);
}

#ifdef TWO_U
AppInfo ACS23_mtqo4_update(void)
{
    return AI_create_app_info("DBG_mtq", NULL, ACS23_mtqo4_update_);
}
#endif


/**
 * @brief App initialize function.
 *
 *
 */
static void ACS23_mtq_init(void)
{
    p_acs23_debug_mtq = &acs23_debug_mtq;

    //(void)packet;
	HAL_I2C_DeInit_C2A(&hi2c3);
	HAL_I2C_Init_C2A(&hi2c3);

//    printf("ACS2 mtq Debug function_core Command execute.\r\n");
	MTQ_Class.MTQA_init();

	MTQ_Class.MTQO_init();
}

static void MTQ_initialize_param(void)
{
	printf("MTQ initialize param.\n");
	write_param(MTQ_Class.target_mag_moment_mtq2U, MTQ_CLASS::target_mag_moment_mtq2U_, (uint16_t)(sizeof(MTQ_Class.target_mag_moment_mtq2U)), (uint32_t)0x00004400);
	write_param(MTQ_Class.target_mag_moment_mtq1U, MTQ_CLASS::target_mag_moment_mtq1U_, (uint16_t)(sizeof(MTQ_Class.target_mag_moment_mtq1U)), (uint32_t)0x00004410);
	write_param(MTQ_Class.k_PID_mtqx2U, MTQ_CLASS::k_PID_mtqx2U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqx2U)), (uint32_t)0x0000441C); //配列は&なし
	write_param(MTQ_Class.k_PID_mtqy2U, MTQ_CLASS::k_PID_mtqy2U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqy2U)), (uint32_t)0x00004428);
	write_param(MTQ_Class.k_PID_mtqz2U, MTQ_CLASS::k_PID_mtqz2U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqz2U)), (uint32_t)0x00004434);
	write_param(MTQ_Class.k_PID_mtqo2U, MTQ_CLASS::k_PID_mtqo2U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqo2U)), (uint32_t)0x00004440);
	write_param(MTQ_Class.k_PID_mtqx1U, MTQ_CLASS::k_PID_mtqx1U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqx1U)), (uint32_t)0x0000444C);
	write_param(MTQ_Class.k_PID_mtqy1U, MTQ_CLASS::k_PID_mtqy1U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqy1U)), (uint32_t)0x00004458);
	write_param(MTQ_Class.k_PID_mtqz1U, MTQ_CLASS::k_PID_mtqz1U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqz1U)), (uint32_t)0x00004464);
	write_param(MTQ_Class.MTQA_DCM, MTQ_CLASS::MTQA_DCM_, (uint16_t)(sizeof(MTQ_Class.MTQA_DCM)), (uint32_t)0x00004470);
	write_param(MTQ_Class.MTQO_DCM, MTQ_CLASS::MTQO_DCM_, (uint16_t)(sizeof(MTQ_Class.MTQO_DCM)), (uint32_t)0x00004494);
	write_param(&MTQ_Class.nS_mtqxy2U, &MTQ_CLASS::nS_mtqxy2U_, (uint16_t)(sizeof(MTQ_Class.nS_mtqxy2U)), (uint32_t)0x000044B8);
	write_param(&MTQ_Class.nS_mtqxy1U, &MTQ_CLASS::nS_mtqxy1U_, (uint16_t)(sizeof(MTQ_Class.nS_mtqxy1U)), (uint32_t)0x000044BC);
	write_param(&MTQ_Class.nS_mtqz2U, &MTQ_CLASS::nS_mtqz2U_, (uint16_t)(sizeof(MTQ_Class.nS_mtqz2U)), (uint32_t)0x000044C0);
	write_param(&MTQ_Class.nS_mtqz1U, &MTQ_CLASS::nS_mtqz1U_, (uint16_t)(sizeof(MTQ_Class.nS_mtqz1U)), (uint32_t)0x000044C4);
	write_param(&MTQ_Class.nS_mtqo2U, &MTQ_CLASS::nS_mtqo2U_, (uint16_t)(sizeof(MTQ_Class.nS_mtqo2U)), (uint32_t)0x000044C8);
	write_param(MTQ_Class.kt_mtqx2U, MTQ_CLASS::kt_mtqx2U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqx2U)), (uint32_t)0x000044CC);
	write_param(MTQ_Class.kt_mtqy2U, MTQ_CLASS::kt_mtqy2U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqy2U)), (uint32_t)0x000044DC);
	write_param(MTQ_Class.kt_mtqz2U, MTQ_CLASS::kt_mtqz2U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqz2U)), (uint32_t)0x000044EC);
	write_param(MTQ_Class.kt_mtqx1U, MTQ_CLASS::kt_mtqx1U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqx1U)), (uint32_t)0x000044FC);
	write_param(MTQ_Class.kt_mtqy1U, MTQ_CLASS::kt_mtqy1U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqy1U)), (uint32_t)0x0000450C);
	write_param(MTQ_Class.kt_mtqz1U, MTQ_CLASS::kt_mtqz1U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqz1U)), (uint32_t)0x0000451C);
	write_param(&MTQ_Class.R_mtqx2U, &MTQ_CLASS::R_mtqx2U_, (uint16_t)(sizeof(MTQ_Class.R_mtqx2U)), (uint32_t)0x0000452C);
	write_param(&MTQ_Class.R_mtqy2U, &MTQ_CLASS::R_mtqy2U_, (uint16_t)(sizeof(MTQ_Class.R_mtqy2U)), (uint32_t)0x00004530);
	write_param(&MTQ_Class.R_mtqz2U, &MTQ_CLASS::R_mtqz2U_, (uint16_t)(sizeof(MTQ_Class.R_mtqz2U)), (uint32_t)0x00004534);
	write_param(&MTQ_Class.R_mtqx1U, &MTQ_CLASS::R_mtqx1U_, (uint16_t)(sizeof(MTQ_Class.R_mtqx1U)), (uint32_t)0x00004538);
	write_param(&MTQ_Class.R_mtqy1U, &MTQ_CLASS::R_mtqy1U_, (uint16_t)(sizeof(MTQ_Class.R_mtqy1U)), (uint32_t)0x0000453C);
	write_param(&MTQ_Class.R_mtqz1U, &MTQ_CLASS::R_mtqz1U_, (uint16_t)(sizeof(MTQ_Class.R_mtqz1U)), (uint32_t)0x00004540);
	write_param(MTQ_Class.current_limit_mtq2U, MTQ_CLASS::current_limit_mtq2U_, (uint16_t)(sizeof(MTQ_Class.current_limit_mtq2U)), (uint32_t)0x00004544);
	write_param(MTQ_Class.current_limit_mtq1U, MTQ_CLASS::current_limit_mtq1U_, (uint16_t)(sizeof(MTQ_Class.current_limit_mtq1U)), (uint32_t)0x00004554);
	write_param(MTQ_Class.MTQ_LOWER_THRESHOLD, MTQ_CLASS::MTQ_LOWER_THRESHOLD_, (uint16_t)(sizeof(MTQ_Class.MTQ_LOWER_THRESHOLD)), (uint32_t)0x00004564);
	write_param(MTQ_Class.MTQ_UPPER_THRESHOLD, MTQ_CLASS::MTQ_UPPER_THRESHOLD_, (uint16_t)(sizeof(MTQ_Class.MTQ_UPPER_THRESHOLD)), (uint32_t)0x0000456C);
	write_param(MTQ_Class.hispin_contorol_flag_mtq2U, MTQ_CLASS::hispin_contorol_flag_mtq2U_, (uint16_t)(sizeof(MTQ_Class.hispin_contorol_flag_mtq2U)), (uint32_t)0x00004574);
	write_param(MTQ_Class.hispin_contorol_flag_mtq1U, MTQ_CLASS::hispin_contorol_flag_mtq1U_, (uint16_t)(sizeof(MTQ_Class.hispin_contorol_flag_mtq1U)), (uint32_t)0x00004577);
}

static void MTQ_load_param(void)
{
	printf("MTQ load param\r\n");
	read_param(MTQ_Class.target_mag_moment_mtq2U, MTQ_CLASS::target_mag_moment_mtq2U_, (uint16_t)(sizeof(MTQ_Class.target_mag_moment_mtq2U)), (uint32_t)0x00004400);
	read_param(MTQ_Class.target_mag_moment_mtq1U, MTQ_CLASS::target_mag_moment_mtq1U_, (uint16_t)(sizeof(MTQ_Class.target_mag_moment_mtq1U)), (uint32_t)0x00004410);
	read_param(MTQ_Class.k_PID_mtqx2U, MTQ_CLASS::k_PID_mtqx2U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqx2U)), (uint32_t)0x0000441C); //配列は&なし
	read_param(MTQ_Class.k_PID_mtqy2U, MTQ_CLASS::k_PID_mtqy2U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqy2U)), (uint32_t)0x00004428);
	read_param(MTQ_Class.k_PID_mtqz2U, MTQ_CLASS::k_PID_mtqz2U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqz2U)), (uint32_t)0x00004434);
	read_param(MTQ_Class.k_PID_mtqo2U, MTQ_CLASS::k_PID_mtqo2U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqo2U)), (uint32_t)0x00004440);
	read_param(MTQ_Class.k_PID_mtqx1U, MTQ_CLASS::k_PID_mtqx1U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqx1U)), (uint32_t)0x0000444C);
	read_param(MTQ_Class.k_PID_mtqy1U, MTQ_CLASS::k_PID_mtqy1U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqy1U)), (uint32_t)0x00004458);
	read_param(MTQ_Class.k_PID_mtqz1U, MTQ_CLASS::k_PID_mtqz1U_, (uint16_t)(sizeof(MTQ_Class.k_PID_mtqz1U)), (uint32_t)0x00004464);
	read_param(MTQ_Class.MTQA_DCM, MTQ_CLASS::MTQA_DCM_, (uint16_t)(sizeof(MTQ_Class.MTQA_DCM)), (uint32_t)0x00004470);
	read_param(MTQ_Class.MTQO_DCM, MTQ_CLASS::MTQO_DCM_, (uint16_t)(sizeof(MTQ_Class.MTQO_DCM)), (uint32_t)0x00004494);
	read_param(&MTQ_Class.nS_mtqxy2U, &MTQ_CLASS::nS_mtqxy2U_, (uint16_t)(sizeof(MTQ_Class.nS_mtqxy2U)), (uint32_t)0x000044B8);
	read_param(&MTQ_Class.nS_mtqxy1U, &MTQ_CLASS::nS_mtqxy1U_, (uint16_t)(sizeof(MTQ_Class.nS_mtqxy1U)), (uint32_t)0x000044BC);
	read_param(&MTQ_Class.nS_mtqz2U, &MTQ_CLASS::nS_mtqz2U_, (uint16_t)(sizeof(MTQ_Class.nS_mtqz2U)), (uint32_t)0x000044C0);
	read_param(&MTQ_Class.nS_mtqz1U, &MTQ_CLASS::nS_mtqz1U_, (uint16_t)(sizeof(MTQ_Class.nS_mtqz1U)), (uint32_t)0x000044C4);
	read_param(&MTQ_Class.nS_mtqo2U, &MTQ_CLASS::nS_mtqo2U_, (uint16_t)(sizeof(MTQ_Class.nS_mtqo2U)), (uint32_t)0x000044C8);
	read_param(MTQ_Class.kt_mtqx2U, MTQ_CLASS::kt_mtqx2U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqx2U)), (uint32_t)0x000044CC);
	read_param(MTQ_Class.kt_mtqy2U, MTQ_CLASS::kt_mtqy2U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqy2U)), (uint32_t)0x000044DC);
	read_param(MTQ_Class.kt_mtqz2U, MTQ_CLASS::kt_mtqz2U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqz2U)), (uint32_t)0x000044EC);
	read_param(MTQ_Class.kt_mtqx1U, MTQ_CLASS::kt_mtqx1U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqx1U)), (uint32_t)0x000044FC);
	read_param(MTQ_Class.kt_mtqy1U, MTQ_CLASS::kt_mtqy1U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqy1U)), (uint32_t)0x0000450C);
	read_param(MTQ_Class.kt_mtqz1U, MTQ_CLASS::kt_mtqz1U_, (uint16_t)(sizeof(MTQ_Class.kt_mtqz1U)), (uint32_t)0x0000451C);
	read_param(&MTQ_Class.R_mtqx2U, &MTQ_CLASS::R_mtqx2U_, (uint16_t)(sizeof(MTQ_Class.R_mtqx2U)), (uint32_t)0x0000452C);
	read_param(&MTQ_Class.R_mtqy2U, &MTQ_CLASS::R_mtqy2U_, (uint16_t)(sizeof(MTQ_Class.R_mtqy2U)), (uint32_t)0x00004530);
	read_param(&MTQ_Class.R_mtqz2U, &MTQ_CLASS::R_mtqz2U_, (uint16_t)(sizeof(MTQ_Class.R_mtqz2U)), (uint32_t)0x00004534);
	read_param(&MTQ_Class.R_mtqx1U, &MTQ_CLASS::R_mtqx1U_, (uint16_t)(sizeof(MTQ_Class.R_mtqx1U)), (uint32_t)0x00004538);
	read_param(&MTQ_Class.R_mtqy1U, &MTQ_CLASS::R_mtqy1U_, (uint16_t)(sizeof(MTQ_Class.R_mtqy1U)), (uint32_t)0x0000453C);
	read_param(&MTQ_Class.R_mtqz1U, &MTQ_CLASS::R_mtqz1U_, (uint16_t)(sizeof(MTQ_Class.R_mtqz1U)), (uint32_t)0x00004540);
	read_param(MTQ_Class.current_limit_mtq2U, MTQ_CLASS::current_limit_mtq2U_, (uint16_t)(sizeof(MTQ_Class.current_limit_mtq2U)), (uint32_t)0x00004544);
	read_param(MTQ_Class.current_limit_mtq1U, MTQ_CLASS::current_limit_mtq1U_, (uint16_t)(sizeof(MTQ_Class.current_limit_mtq1U)), (uint32_t)0x00004554);
	read_param(MTQ_Class.MTQ_LOWER_THRESHOLD, MTQ_CLASS::MTQ_LOWER_THRESHOLD_, (uint16_t)(sizeof(MTQ_Class.MTQ_LOWER_THRESHOLD)), (uint32_t)0x00004564);
	read_param(MTQ_Class.MTQ_UPPER_THRESHOLD, MTQ_CLASS::MTQ_UPPER_THRESHOLD_, (uint16_t)(sizeof(MTQ_Class.MTQ_UPPER_THRESHOLD)), (uint32_t)0x0000456C);
	read_param(MTQ_Class.hispin_contorol_flag_mtq2U, MTQ_CLASS::hispin_contorol_flag_mtq2U_, (uint16_t)(sizeof(MTQ_Class.hispin_contorol_flag_mtq2U)), (uint32_t)0x00004574);
	read_param(MTQ_Class.hispin_contorol_flag_mtq1U, MTQ_CLASS::hispin_contorol_flag_mtq1U_, (uint16_t)(sizeof(MTQ_Class.hispin_contorol_flag_mtq1U)), (uint32_t)0x00004577);
}



/**
 * @brief
 * MTQゼロ点コマンド
 */
CCP_CmdRet Cmd_ACS23_MTQ_Output_Zero(const CommonCmdPacket* packet)
{
	if(PWR_GET_MODE(PWR_State_MTQA)){

	    MTQ_Class.MTQA_init();

#ifdef TWO_U
	    if(PWR_GET_MODE(PWR_State_MTQO_PLASMA)){
	        MTQ_Class.MTQO_init();
	    }
#endif/*for 2U*/
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief
 * MTQゼロ点にした出力を元に戻すコマンド
 */
CCP_CmdRet Cmd_ACS23_MTQ_Output_Return(const CommonCmdPacket* packet)
{
	if(PWR_GET_MODE(PWR_State_MTQA)){
		//i : 0->MTQX 1->MTQY 2->MTQZ 3->MTQO
		for (int i = 0; i < 3; i++){
			MTQ_Class.MTQ_Output(i, MTQ_Class.curs_cmd_val_int_[i]);
		}

#ifdef TWO_U
		if(PWR_GET_MODE(PWR_State_MTQO_PLASMA)){
			MTQ_Class.MTQ_Output(3, MTQ_Class.curs_cmd_val_int_[3]);
		}
#endif/*for 2U*/

	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief
 * MTQA用デバッグコマンド
 * コマンド |DAC電圧[V] | MTQ電圧[V]
 * ------------------------------
 *  00  |    0.0   |  -0.625
 *  80  |    2.5   |   0.0
 *  ff  |    5.0   |   0.625
 */
CCP_CmdRet Cmd_ACS23_debug_MTQA_Function_core(const CommonCmdPacket* packet)
{
	acs23_debug_mtq.mtqa_debug_flag = CCP_get_param_head(packet)[0]; //0x00(0) or 0x01(1)

	acs23_debug_mtq.mtqa_debug_param[0] = CCP_get_param_head(packet)[1]; //0x00(0) ~ 0xff(255)
	acs23_debug_mtq.mtqa_debug_param[1] = CCP_get_param_head(packet)[2]; //0x00(0) ~ 0xff(255)
	acs23_debug_mtq.mtqa_debug_param[2] = CCP_get_param_head(packet)[3]; //0x00(0) ~ 0xff(255)

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief
 * MTQO用デバッグコマンド
 * コマンド |DAC電圧[V] | MTQ電圧[V]
 * ------------------------------
 *  00  |    0.0   |  -0.625
 *  80  |    2.5   |   0.0
 *  ff  |    5.0   |   0.625
 */
CCP_CmdRet Cmd_ACS23_debug_MTQO_Function_core(const CommonCmdPacket* packet)
{
	acs23_debug_mtq.mtqo_debug_flag = CCP_get_param_head(packet)[0]; //0x00(0) or 0x01(1)

	acs23_debug_mtq.mtqo_debug_param = CCP_get_param_head(packet)[1]; //0x00(0) ~ 0xff(255)

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief
 * MTQAデバッグ用関数
 */
static void ACS23_debug_MTQA_Function_core(uint8_t param[3])
{
	int Value1 = (param[0] - 128); //-128 ~ 127
	int Value2 = (param[1] - 128); //-128 ~ 127
	int Value3 = (param[2] - 128); //-128 ~ 127

	float Input[3];

	Input[0]  = float(Value1 / 128.0f); //-1 ~ 1
	Input[1]  = float(Value2 / 128.0f); //-1 ~ 1
	Input[2]  = float(Value3 / 128.0f); //-1 ~ 1

	MTQ_Class.MTQA_FunctionCore4debug(Input);
}

/**
 * @brief
 * MTQOデバッグ用関数
 */
static void ACS23_debug_MTQO_Function_core(uint8_t param)
{
	int Value = param - 128; //-128 ~ 127

	float Input;

	Input = float(Value / 128.0f); //-1 ~ 1

	//MTQ loop
	MTQ_Class.MTQO_FunctionCore4debug(Input);
}


/**
 * @brief
 * MTQAで常に周り続ける関数
 */
static void ACS23_mtqa4_update_(void){

	// set MTQ PID parameter
	MTQ_Class.MTQ_PID_Param_Setting();

	if (PWR_State_GET(PWR_State_MTQA)){

		if (acs23_debug_mtq.mtqa_debug_flag == 1) { // MTQAデバッグコマンドで回る

			ACS23_debug_MTQA_Function_core(acs23_debug_mtq.mtqa_debug_param);

		}
		else {// 通常のタスクリストで回る

			float Mag_moment[3]; // = {0.05f, 0.05f, 1.0f}

			for (int i = 0; i < 3; i ++){

				Mag_moment[i] = g_acs.p_GLOBAL_AOCS->Mag_moment[i];  //Example
			}

			MTQ_Class.MTQA_FunctionCore(Mag_moment);

			MTQ_Class.MTQA_DCM_Calc(MTQ_Class.curs_cmd_val_int_);
		}
	}
	else {
//		print(MTQ, "FET_MTQA is OFF\r\n");
	}
}

/**
 * @brief
 * MTQA OT AHコマンド
 */
CCP_CmdRet Cmd_ACS23_AH_MTQA_OT(const CommonCmdPacket* packet)
{
	if(MTQ_Class.mtq_flag_vote(&MTQ_Class.mtqa_anomaly) == 1){
		printf("MTQA_anomaly!!!!\r\n");
		uint32_t current_ACS_flag = ACS_Mode_Get();
		uint32_t temp = (0b0001 << OUTPUT_MTQA_X) + (0b0001 << OUTPUT_MTQA_Y) + (0b0001 << OUTPUT_MTQA_Z); // = (10)176160768

		ACS_Mode_Set(current_ACS_flag ^ temp);    // MTQAx~z=23,25,27 のフラグを下げる
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#ifdef TWO_U
/**
 * @brief
 * MTQOで常に周り続ける関数
 */
static void ACS23_mtqo4_update_(void){

	if (PWR_State_GET(PWR_State_MTQO_PLASMA)){

		if (acs23_debug_mtq.mtqo_debug_flag == 1) { // MTQAデバッグコマンドで回る

			ACS23_debug_MTQO_Function_core(acs23_debug_mtq.mtqo_debug_param);

		}
		else { // 通常のタスクリストで回る

			float Mag_moment; // = 1.0f;

			Mag_moment = g_acs.p_GLOBAL_AOCS->Mag_moment[2];  //Example

			MTQ_Class.MTQO_FunctionCore(Mag_moment);

			MTQ_Class.MTQO_DCM_Calc(MTQ_Class.curs_cmd_val_int_);
		}
	}
	else {
//		print(MTQ, "FET_MTQO is OFF\r\n");
	}
}

/**
 * @brief
 * MTQO OT AHコマンド
 */
CCP_CmdRet Cmd_ACS23_AH_MTQO_OT(const CommonCmdPacket* packet)
{
	if(MTQ_Class.mtq_flag_vote(&MTQ_Class.mtqo_anomaly) == 1){
		printf("MTQO_anomaly!!!!\r\n");
		uint32_t current_ACS_flag = ACS_Mode_Get();
		uint32_t temp = 0b0001 << OUTPUT_CONST_MTQO;

		ACS_Mode_Set(current_ACS_flag ^ temp);    // MTQO=29 のフラグを下げる
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


#endif/*for 2U*/
