/*
 * OCS_cmd_Deceleration.cpp
 *
 *  Created on: 2022/04/20
 *      Author: sunaken
 */

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/AOCS/OCS/OCS_cmd_CrossProduct.h>
#include <stdlib.h>
#include <string.h>

#include "../../Global_Sensor_Value.h"
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/CrossProduct.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>


static CROSS_PRODUCT_CLASS Cross_class;
static OCS_CROSS_STRUCT OCS_Cross_;

static void OCS_CrossProduct_load_param(void);
static void OCS_CrossProduct_initialize_param(void);
static void OCS_CrossProduct_Ave_Deceleration_update_(void);
static void OCS_CrossProduct_Min_Deceleration_update_(void);
static void OCS_CrossProduct_Max_Deceleration_update_(void);
static void OCS_CrossProduct_Keep_Deceleration_update_(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo OCS_CrossProduct_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_CrossProduct", OCS_CrossProduct_load_param, OCS_CrossProduct_initialize_param);
}
AppInfo OCS_CrossProduct_Ave_Deceleration_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, OCS_CrossProduct_Ave_Deceleration_update_);
}
AppInfo OCS_CrossProduct_Min_Deceleration_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, OCS_CrossProduct_Min_Deceleration_update_);
}
AppInfo OCS_CrossProduct_Max_Deceleration_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, OCS_CrossProduct_Max_Deceleration_update_);
}
AppInfo OCS_CrossProduct_Keep_Deceleration_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, OCS_CrossProduct_Keep_Deceleration_update_);
}
/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void OCS_CrossProduct_load_param(void)
{
    printf("OCS Cross driver initialize.\n");
    read_param(Cross_class.Kpave, CROSS_PRODUCT_CLASS::Kp_ave, (uint16_t)sizeof(Cross_class.Kpave), (uint32_t)0x0000A048);
    read_param(Cross_class.Kdave, CROSS_PRODUCT_CLASS::Kd_ave, (uint16_t)sizeof(Cross_class.Kdave), (uint32_t)0x0000A054);
    read_param(Cross_class.Kpmin, CROSS_PRODUCT_CLASS::Kp_min, (uint16_t)sizeof(Cross_class.Kpmin), (uint32_t)0x0000A060);
    read_param(Cross_class.Kdmin, CROSS_PRODUCT_CLASS::Kd_min, (uint16_t)sizeof(Cross_class.Kdmin), (uint32_t)0x0000A06C);
    read_param(Cross_class.Kpmax, CROSS_PRODUCT_CLASS::Kp_max, (uint16_t)sizeof(Cross_class.Kpmax), (uint32_t)0x0000A078);
    read_param(Cross_class.Kdmax, CROSS_PRODUCT_CLASS::Kd_max, (uint16_t)sizeof(Cross_class.Kdmax), (uint32_t)0x0000A084);
    read_param(Cross_class.Kpkeep, CROSS_PRODUCT_CLASS::Kp_keep, (uint16_t)sizeof(Cross_class.Kpkeep), (uint32_t)0x0000A090);
    read_param(Cross_class.Kdkeep, CROSS_PRODUCT_CLASS::Kd_keep, (uint16_t)sizeof(Cross_class.Kdkeep), (uint32_t)0x0000A09C);
    read_param(Cross_class.ControlDirection_Ave, CROSS_PRODUCT_CLASS::ControlDirection_ave, (size_t)sizeof(Cross_class.ControlDirection_Ave), (uint32_t)0x0000A0A8);
    read_param(Cross_class.ControlDirection_Min, CROSS_PRODUCT_CLASS::ControlDirection_min, (size_t)sizeof(Cross_class.ControlDirection_Min), (uint32_t)0x0000A0B4);
    read_param(Cross_class.ControlDirection_Max, CROSS_PRODUCT_CLASS::ControlDirection_max, (size_t)sizeof(Cross_class.ControlDirection_Max), (uint32_t)0x0000A0C0);
    read_param(Cross_class.ControlDirection_Keep, CROSS_PRODUCT_CLASS::ControlDirection_keep, (size_t)sizeof(Cross_class.ControlDirection_Keep), (uint32_t)0x0000A0CC);
    read_param(Cross_class.TargetOmeO, CROSS_PRODUCT_CLASS::TargetOmeO_, (size_t)sizeof(Cross_class.TargetOmeO), (uint32_t)0x0000A0D8);
    read_param(&Cross_class.eclipse_mode_ave, &CROSS_PRODUCT_CLASS::eclipse_mode_ave_, (size_t)sizeof(Cross_class.eclipse_mode_ave), (uint32_t)0x00007284);
    read_param(&Cross_class.eclipse_mode_min, &CROSS_PRODUCT_CLASS::eclipse_mode_min_, (size_t)sizeof(Cross_class.eclipse_mode_min), (uint32_t)0x00007285);
    read_param(&Cross_class.eclipse_mode_max, &CROSS_PRODUCT_CLASS::eclipse_mode_max_, (size_t)sizeof(Cross_class.eclipse_mode_max), (uint32_t)0x00007286);
    read_param(&Cross_class.eclipse_mode_keep, &CROSS_PRODUCT_CLASS::eclipse_mode_keep_, (size_t)sizeof(Cross_class.eclipse_mode_keep), (uint32_t)0x00007287);
}

/**
 * @brief アプリケーション実行関数
 *
 */
static void OCS_CrossProduct_initialize_param(void)
{
    printf("OCS CrossProduct initialize_param.\n");
    write_param(Cross_class.Kpave, CROSS_PRODUCT_CLASS::Kp_ave, (uint16_t)sizeof(Cross_class.Kpave), (uint32_t)0x0000A048);
    write_param(Cross_class.Kdave, CROSS_PRODUCT_CLASS::Kd_ave, (uint16_t)sizeof(Cross_class.Kdave), (uint32_t)0x0000A054);
    write_param(Cross_class.Kpmin, CROSS_PRODUCT_CLASS::Kp_min, (uint16_t)sizeof(Cross_class.Kpmin), (uint32_t)0x0000A060);
    write_param(Cross_class.Kdmin, CROSS_PRODUCT_CLASS::Kd_min, (uint16_t)sizeof(Cross_class.Kdmin), (uint32_t)0x0000A06C);
    write_param(Cross_class.Kpmax, CROSS_PRODUCT_CLASS::Kp_max, (uint16_t)sizeof(Cross_class.Kpmax), (uint32_t)0x0000A078);
    write_param(Cross_class.Kdmax, CROSS_PRODUCT_CLASS::Kd_max, (uint16_t)sizeof(Cross_class.Kdmax), (uint32_t)0x0000A084);
    write_param(Cross_class.Kpkeep, CROSS_PRODUCT_CLASS::Kp_keep, (uint16_t)sizeof(Cross_class.Kpkeep), (uint32_t)0x0000A090);
    write_param(Cross_class.Kdkeep, CROSS_PRODUCT_CLASS::Kd_keep, (uint16_t)sizeof(Cross_class.Kdkeep), (uint32_t)0x0000A09C);
    write_param(Cross_class.ControlDirection_Ave, CROSS_PRODUCT_CLASS::ControlDirection_ave, (size_t)sizeof(Cross_class.ControlDirection_Ave), (uint32_t)0x0000A0A8);
    write_param(Cross_class.ControlDirection_Min, CROSS_PRODUCT_CLASS::ControlDirection_min, (size_t)sizeof(Cross_class.ControlDirection_Min), (uint32_t)0x0000A0B4);
    write_param(Cross_class.ControlDirection_Max, CROSS_PRODUCT_CLASS::ControlDirection_max, (size_t)sizeof(Cross_class.ControlDirection_Max), (uint32_t)0x0000A0C0);
    write_param(Cross_class.ControlDirection_Keep, CROSS_PRODUCT_CLASS::ControlDirection_keep, (size_t)sizeof(Cross_class.ControlDirection_Keep), (uint32_t)0x0000A0CC);
    write_param(Cross_class.TargetOmeO, CROSS_PRODUCT_CLASS::TargetOmeO_, (size_t)sizeof(Cross_class.TargetOmeO), (uint32_t)0x0000A0D8);
    write_param(&Cross_class.eclipse_mode_ave, &CROSS_PRODUCT_CLASS::eclipse_mode_ave_, (size_t)sizeof(Cross_class.eclipse_mode_ave), (uint32_t)0x00007284);
    write_param(&Cross_class.eclipse_mode_min, &CROSS_PRODUCT_CLASS::eclipse_mode_min_, (size_t)sizeof(Cross_class.eclipse_mode_min), (uint32_t)0x00007285);
    write_param(&Cross_class.eclipse_mode_max, &CROSS_PRODUCT_CLASS::eclipse_mode_max_, (size_t)sizeof(Cross_class.eclipse_mode_max), (uint32_t)0x00007286);
    write_param(&Cross_class.eclipse_mode_keep, &CROSS_PRODUCT_CLASS::eclipse_mode_keep_, (size_t)sizeof(Cross_class.eclipse_mode_keep), (uint32_t)0x00007287);
}

/**
 * @brief コマンド実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
//-----------------------------------クロスプロダクトによるポインティング制御-------------------------------------------------------------

static void OCS_CrossProduct_Ave_Deceleration_update_(void)
{

///Input: 地磁気，クォータニオン，目標ベクトル
///Output: 磁気モーメント

	float B_fld_[3] = {g_acs.p_ACS->MagField[0], g_acs.p_ACS->MagField[1], g_acs.p_ACS->MagField[2]};
	float omega[3] = {g_acs.p_ACS->Omega[0], g_acs.p_ACS->Omega[1], g_acs.p_ACS->Omega[2]};
	float Velocity_Vector[3] = {g_acs.p_ACS->Velocity[0], g_acs.p_ACS->Velocity[1], g_acs.p_ACS->Velocity[2]};
	float Velocity_Vector_b[3];

//	float maglimit[3];
#ifdef HILS
		B_fld_[0] = AOCS_hils->mags[0]*0.000000001;
		B_fld_[1] = AOCS_hils->mags[1]*0.000000001;
		B_fld_[2] = AOCS_hils->mags[2]*0.000000001;
		omega[0] = AOCS_hils->gyro[0];
		omega[1] = AOCS_hils->gyro[1];
		omega[2] = AOCS_hils->gyro[2];
#endif
	//平均減速
	multiplication(&g_acs.p_ACS->DCM_i2b[0][0], 3, 3, Velocity_Vector, 3, 1, Velocity_Vector_b, 3, 1);

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_ave == 2){
	 	Cross_class.Poincal(B_fld_, omega, Velocity_Vector_b, Cross_class.ControlDirection_Ave, Cross_class.TargetOmeO, Cross_class.Kpave, Cross_class.Kdave);
	}
	else if (Cross_class.eclipse_mode_ave == 1){
	 	Cross_class.Poincal_eclipse(B_fld_, omega, Velocity_Vector_b, Cross_class.ControlDirection_Ave, Cross_class.TargetOmeO, Cross_class.Kpave, Cross_class.Kdave);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(Cross_class.OUT_Moment_b_, 3, 1, OCS_Cross_.Mag_moment);

//	for(int i = 0; i < 3; i ++ ){
//		maglimit[i] = g_acs.p_GLOBAL_AOCS->MagLimit[i];
//	}
//#ifdef TWO_U
//	if(OCS_Cross_.Mag_moment[0] > maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = maglimit[0];
//	}else if(OCS_Cross_.Mag_moment[0] < -maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = -maglimit[0];
//	}
//	if(OCS_Cross_.Mag_moment[1] > maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = maglimit[1];
//	}else if(OCS_Cross_.Mag_moment[1] < -maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = -maglimit[1];
//	}
//	if(OCS_Cross_.Mag_moment[2] > maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = maglimit[2];
//	}else if(OCS_Cross_.Mag_moment[2] < -maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = -maglimit[2];
//	}
//#endif
//
//#ifdef ONE_U
//	if(OCS_Cross_.Mag_moment[0] > maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = maglimit[0];
//	}else if(OCS_Cross_.Mag_moment[0] < -maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = -maglimit[0];
//	}
//	if(OCS_Cross_.Mag_moment[1] > maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = maglimit[1];
//	}else if(OCS_Cross_.Mag_moment[1] < -maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = -maglimit[1];
//	}
//	if(OCS_Cross_.Mag_moment[2] > maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = maglimit[2];
//	}else if(OCS_Cross_.Mag_moment[2] < -maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = -maglimit[2];
//	}
//#endif

//	GLOBAL_AOCS_set_Mag_moment(OCS_Cross_.Mag_moment);
	GLOBAL_OCS_set_Mag_moment_Cn(OCS_Cross_.Mag_moment);

#ifdef HILS
//	// Send torque to S2E
//	uint8_t mtq_dummyt_u8[24];
//	uint8_t dummy_cmd[30] = { 'M', 'T', 'Q',  '0' };
//	float mtq_control[3] = { OCS_Cross_.Mag_moment[0],OCS_Cross_.Mag_moment[1],OCS_Cross_.Mag_moment[2] };
//	memcpy(dummy_cmd + 4, mtq_control, 12);
//	HAL_UART_Transmit(&huart7,(uint8_t*)&dummy_cmd,16, 0xff);
	print(CPRODUCT,"Ave_Deceleration\r\n");
	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", OCS_Cross_.Mag_moment[0], OCS_Cross_.Mag_moment[1], OCS_Cross_.Mag_moment[2]);

#endif
}

static void OCS_CrossProduct_Min_Deceleration_update_(void)
{

///Input: 地磁気，クォータニオン，目標ベクトル
///Output: 磁気モーメント

	float B_fld_[3] = {g_acs.p_ACS->MagField[0], g_acs.p_ACS->MagField[1], g_acs.p_ACS->MagField[2]};
	float omega[3] = {g_acs.p_ACS->Omega[0], g_acs.p_ACS->Omega[1], g_acs.p_ACS->Omega[2]};
	float Velocity_Vector[3] = {g_acs.p_ACS->Velocity[0], g_acs.p_ACS->Velocity[1], g_acs.p_ACS->Velocity[2]};
	float Velocity_Vector_b[3];
//	float maglimit[3];
#ifdef HILS
		B_fld_[0] = AOCS_hils->mags[0]*0.000000001;
		B_fld_[1] = AOCS_hils->mags[1]*0.000000001;
		B_fld_[2] = AOCS_hils->mags[2]*0.000000001;
		omega[0] = AOCS_hils->gyro[0];
		omega[1] = AOCS_hils->gyro[1];
		omega[2] = AOCS_hils->gyro[2];
#endif
	//最小減速
	multiplication(&g_acs.p_ACS->DCM_i2b[0][0], 3, 3, Velocity_Vector, 3, 1, Velocity_Vector_b, 3, 1);

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_min == 2){
		Cross_class.Poincal(B_fld_, omega, Velocity_Vector_b, Cross_class.ControlDirection_Min, Cross_class.TargetOmeO, Cross_class.Kpmin, Cross_class.Kdmin);
	}
	else if (Cross_class.eclipse_mode_min == 1){
		Cross_class.Poincal_eclipse(B_fld_, omega, Velocity_Vector_b, Cross_class.ControlDirection_Min, Cross_class.TargetOmeO, Cross_class.Kpmin, Cross_class.Kdmin);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(Cross_class.OUT_Moment_b_, 3, 1, OCS_Cross_.Mag_moment);;
//	for(int i = 0; i < 3; i ++ ){
//		maglimit[i] = g_acs.p_GLOBAL_AOCS->MagLimit[i];
//	}
//#ifdef TWO_U
//	if(OCS_Cross_.Mag_moment[0] > maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = maglimit[0];
//	}else if(OCS_Cross_.Mag_moment[0] < -maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = -maglimit[0];
//	}
//	if(OCS_Cross_.Mag_moment[1] > maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = maglimit[1];
//	}else if(OCS_Cross_.Mag_moment[1] < -maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = -maglimit[1];
//	}
//	if(OCS_Cross_.Mag_moment[2] > maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = maglimit[2];
//	}else if(OCS_Cross_.Mag_moment[2] < -maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = -maglimit[2];
//	}
//#endif
//
//#ifdef ONE_U
//	if(OCS_Cross_.Mag_moment[0] > maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = maglimit[0];
//	}else if(OCS_Cross_.Mag_moment[0] < -maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = -maglimit[0];
//	}
//	if(OCS_Cross_.Mag_moment[1] > maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = maglimit[1];
//	}else if(OCS_Cross_.Mag_moment[1] < -maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = -maglimit[1];
//	}
//	if(OCS_Cross_.Mag_moment[2] > maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = maglimit[2];
//	}else if(OCS_Cross_.Mag_moment[2] < -maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = -maglimit[2];
//	}
//#endif

//	GLOBAL_AOCS_set_Mag_moment(OCS_Cross_.Mag_moment);
	GLOBAL_OCS_set_Mag_moment_Cn(OCS_Cross_.Mag_moment);

#ifdef HILS
	// Send torque to S2E
//	uint8_t mtq_dummyt_u8[24];
//	uint8_t dummy_cmd[30] = { 'M', 'T', 'Q',  '0' };
//	float mtq_control[3] = { OCS_Cross_.Mag_moment[0],OCS_Cross_.Mag_moment[1],OCS_Cross_.Mag_moment[2] };
//	memcpy(dummy_cmd + 4, mtq_control, 12);
//	HAL_UART_Transmit(&huart7,(uint8_t*)&dummy_cmd,16, 0xff);
	print(CPRODUCT,"Min_Deceleration\r\n");
	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", OCS_Cross_.Mag_moment[0], OCS_Cross_.Mag_moment[1], OCS_Cross_.Mag_moment[2]);
#endif
}

static void OCS_CrossProduct_Max_Deceleration_update_(void)
{

///Input: 地磁気，クォータニオン，目標ベクトル
///Output: 磁気モーメント

	float B_fld_[3] = {g_acs.p_ACS->MagField[0], g_acs.p_ACS->MagField[1], g_acs.p_ACS->MagField[2]};
	float omega[3] = {g_acs.p_ACS->Omega[0], g_acs.p_ACS->Omega[1], g_acs.p_ACS->Omega[2]};
	float Velocity_Vector[3] = {g_acs.p_ACS->Velocity[0], g_acs.p_ACS->Velocity[1], g_acs.p_ACS->Velocity[2]};
	float Velocity_Vector_b[3];
//	float maglimit[3];
#ifdef HILS
		B_fld_[0] = AOCS_hils->mags[0]*0.000000001;
		B_fld_[1] = AOCS_hils->mags[1]*0.000000001;
		B_fld_[2] = AOCS_hils->mags[2]*0.000000001;
		omega[0] = AOCS_hils->gyro[0];
		omega[1] = AOCS_hils->gyro[1];
		omega[2] = AOCS_hils->gyro[2];
#endif
	//最大減速
	multiplication(&g_acs.p_ACS->DCM_i2b[0][0], 3, 3, Velocity_Vector, 3, 1, Velocity_Vector_b, 3, 1);

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_max == 2){
		Cross_class.Poincal(B_fld_, omega, Velocity_Vector_b, Cross_class.ControlDirection_Max, Cross_class.TargetOmeO, Cross_class.Kpmax, Cross_class.Kdmax);
	}
	else if (Cross_class.eclipse_mode_max == 1){
		Cross_class.Poincal_eclipse(B_fld_, omega, Velocity_Vector_b, Cross_class.ControlDirection_Max, Cross_class.TargetOmeO, Cross_class.Kpmax, Cross_class.Kdmax);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(Cross_class.OUT_Moment_b_, 3, 1, OCS_Cross_.Mag_moment);;
//	for(int i = 0; i < 3; i ++ ){
//		maglimit[i] = g_acs.p_GLOBAL_AOCS->MagLimit[i];
//	}
//#ifdef TWO_U
//	if(OCS_Cross_.Mag_moment[0] > maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = maglimit[0];
//	}else if(OCS_Cross_.Mag_moment[0] < -maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = -maglimit[0];
//	}
//	if(OCS_Cross_.Mag_moment[1] > maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = maglimit[1];
//	}else if(OCS_Cross_.Mag_moment[1] < -maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = -maglimit[1];
//	}
//	if(OCS_Cross_.Mag_moment[2] > maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = maglimit[2];
//	}else if(OCS_Cross_.Mag_moment[2] < -maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = -maglimit[2];
//	}
//#endif
//
//#ifdef ONE_U
//	if(OCS_Cross_.Mag_moment[0] > maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = maglimit[0];
//	}else if(OCS_Cross_.Mag_moment[0] < -maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = -maglimit[0];
//	}
//	if(OCS_Cross_.Mag_moment[1] > maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = maglimit[1];
//	}else if(OCS_Cross_.Mag_moment[1] < -maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = -maglimit[1];
//	}
//	if(OCS_Cross_.Mag_moment[2] > maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = maglimit[2];
//	}else if(OCS_Cross_.Mag_moment[2] < -maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = -maglimit[2];
//	}
//#endif

//	GLOBAL_AOCS_set_Mag_moment(OCS_Cross_.Mag_moment);
	GLOBAL_OCS_set_Mag_moment_Cn(OCS_Cross_.Mag_moment);

#ifdef HILS
	// Send torque to S2E
//	uint8_t mtq_dummyt_u8[24];
//	uint8_t dummy_cmd[30] = { 'M', 'T', 'Q',  '0' };
//	float mtq_control[3] = { OCS_Cross_.Mag_moment[0],OCS_Cross_.Mag_moment[1],OCS_Cross_.Mag_moment[2] };
//	memcpy(dummy_cmd + 4, mtq_control, 12);
//	HAL_UART_Transmit(&huart7,(uint8_t*)&dummy_cmd,16, 0xff);
	print(CPRODUCT,"Max_Deceleration\r\n");
	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", OCS_Cross_.Mag_moment[0], OCS_Cross_.Mag_moment[1], OCS_Cross_.Mag_moment[2]);
#endif
}

static void OCS_CrossProduct_Keep_Deceleration_update_(void)
{

///Input: 地磁気，クォータニオン，目標ベクトル
///Output: 磁気モーメント

	float B_fld_[3] = {g_acs.p_ACS->MagField[0], g_acs.p_ACS->MagField[1], g_acs.p_ACS->MagField[2]};
	float omega[3] = {g_acs.p_ACS->Omega[0], g_acs.p_ACS->Omega[1], g_acs.p_ACS->Omega[2]};
	float Velocity_Vector[3] = {g_acs.p_ACS->Velocity[0], g_acs.p_ACS->Velocity[1], g_acs.p_ACS->Velocity[2]};
	float Velocity_Vector_b[3];
//	float maglimit[3];
#ifdef HILS
		B_fld_[0] = AOCS_hils->mags[0]*0.000000001;
		B_fld_[1] = AOCS_hils->mags[1]*0.000000001;
		B_fld_[2] = AOCS_hils->mags[2]*0.000000001;
		omega[0] = AOCS_hils->gyro[0];
		omega[1] = AOCS_hils->gyro[1];
		omega[2] = AOCS_hils->gyro[2];
#endif
	multiplication(&g_acs.p_ACS->DCM_i2b[0][0], 3, 3, Velocity_Vector, 3, 1, Velocity_Vector_b, 3, 1);

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_keep == 2){
		Cross_class.Poincal(B_fld_, omega, Velocity_Vector_b, Cross_class.ControlDirection_Keep, Cross_class.TargetOmeO, Cross_class.Kpkeep, Cross_class.Kdkeep);
	}
	else if (Cross_class.eclipse_mode_keep == 1){
		Cross_class.Poincal_eclipse(B_fld_, omega, Velocity_Vector_b, Cross_class.ControlDirection_Keep, Cross_class.TargetOmeO, Cross_class.Kpkeep, Cross_class.Kdkeep);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(Cross_class.OUT_Moment_b_, 3, 1, OCS_Cross_.Mag_moment);
//	for(int i = 0; i < 3; i ++ ){
//		maglimit[i] = g_acs.p_GLOBAL_AOCS->MagLimit[i];
//	}

//#ifdef TWO_U
//	if(OCS_Cross_.Mag_moment[0] > maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = maglimit[0];
//	}else if(OCS_Cross_.Mag_moment[0] < -maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = -maglimit[0];
//	}
//	if(OCS_Cross_.Mag_moment[1] > maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = maglimit[1];
//	}else if(OCS_Cross_.Mag_moment[1] < -maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = -maglimit[1];
//	}
//	if(OCS_Cross_.Mag_moment[2] > maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = maglimit[2];
//	}else if(OCS_Cross_.Mag_moment[2] < -maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = -maglimit[2];
//	}
//#endif
//
//#ifdef ONE_U
//	if(OCS_Cross_.Mag_moment[0] > maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = maglimit[0];
//	}else if(OCS_Cross_.Mag_moment[0] < -maglimit[0]){
//		OCS_Cross_.Mag_moment[0] = -maglimit[0];
//	}
//	if(OCS_Cross_.Mag_moment[1] > maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = maglimit[1];
//	}else if(OCS_Cross_.Mag_moment[1] < -maglimit[1]){
//		OCS_Cross_.Mag_moment[1] = -maglimit[1];
//	}
//	if(OCS_Cross_.Mag_moment[2] > maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = maglimit[2];
//	}else if(OCS_Cross_.Mag_moment[2] < -maglimit[2]){
//		OCS_Cross_.Mag_moment[2] = -maglimit[2];
//	}
//#endif

//	GLOBAL_AOCS_set_Mag_moment(OCS_Cross_.Mag_moment);
	GLOBAL_OCS_set_Mag_moment_Cn(OCS_Cross_.Mag_moment);

#ifdef HILS
	// Send torque to S2E
//	uint8_t mtq_dummyt_u8[24];
//	uint8_t dummy_cmd[30] = { 'M', 'T', 'Q',  '0' };
//	float mtq_control[3] = { OCS_Cross_.Mag_moment[0],OCS_Cross_.Mag_moment[1],OCS_Cross_.Mag_moment[2] };
//	memcpy(dummy_cmd + 4, mtq_control, 12);
//	HAL_UART_Transmit(&huart7,(uint8_t*)&dummy_cmd,16, 0xff);
	print(CPRODUCT,"Keep_Deceleration\r\n");
	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", OCS_Cross_.Mag_moment[0], OCS_Cross_.Mag_moment[1], OCS_Cross_.Mag_moment[2]);
#endif
}
//-------------------------------------------------------------------------------------------------------------------
