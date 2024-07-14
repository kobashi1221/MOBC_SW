/*
 * ACS_cmd_CrossProduct.cpp
 *
 *  Created on: 2021/03/27
 *      Author: masuda
 *   Reference:
 *
 */


#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_CrossProduct.h>
#include <stdlib.h>
#include <string.h>

#include "../../Global_Sensor_Value.h"
#include <src_user/Applications/UserDefined/AOCS/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/CrossProduct.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Drivers/Com/COBC/cobc_telemetry_data_definitions.h>
#include <src_user/Drivers/Com/COBC/COBC.h>
#include <src_user/Applications/DriverInstances/di_cobc.h>
#include <src_user/Drivers/Aocs/EarthVector.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.h>

static CROSS_PRODUCT_CLASS Cross_class;
static CROSS_STRUCT Cross_;
static EARTHVECTOR_CLASS EARTHVECTOR_class;

static void CrossProduct_load_param(void);
static void CrossProduct_initialize_param(void);

static void ACS_CrossProduct_AngularMomentum_update_(void);
static void ACS_CrossProduct_Quaternion_update_(void);
static void ACS_CrossProduct_Sun_Pointing_update_(void);
static void ACS_CrossProduct_Earth_Center_Pointing_update_(void);
static void ACS_CrossProduct_Earth_Point_Pointing_update_(void);
static void ACS_CrossProduct_Inertial_Pointing_update_(void);
static void ACS_CrossProduct_Velocity_Pointing_update_(void);
static void ACS_CrossProduct_Trajectry_Pointing_update_(void);
static void ACS_CrossProduct_QFB_update_(void);

uint8_t vote_flag(uint8_t* flag);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo CrossProduct_param_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", CrossProduct_load_param, CrossProduct_initialize_param);
}
AppInfo ACS_CrossProduct_AngularMomentum_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_CrossProduct_AngularMomentum_update_);
}
AppInfo ACS_CrossProduct_Quaternion_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_CrossProduct_Quaternion_update_);
}
AppInfo ACS_CrossProduct_Sun_Pointing_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_CrossProduct_Sun_Pointing_update_);
}
AppInfo ACS_CrossProduct_Earth_Center_Pointing_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_CrossProduct_Earth_Center_Pointing_update_);
}
AppInfo ACS_CrossProduct_Earth_Point_Pointing_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_CrossProduct_Earth_Point_Pointing_update_);
}
AppInfo ACS_CrossProduct_Inertial_Pointing_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_CrossProduct_Inertial_Pointing_update_);
}
AppInfo ACS_CrossProduct_Velocity_Pointing_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_CrossProduct_Velocity_Pointing_update_);
}
AppInfo ACS_CrossProduct_Trajectry_Pointing_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_CrossProduct_Trajectry_Pointing_update_);
}
AppInfo ACS_CrossProduct_QFB_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_CrossProduct_QFB_update_);
}
/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void CrossProduct_load_param(void)
{
    printf("Cross load_param.\n");
    g_acs.p_CROSS = &Cross_;
    read_param(Cross_class.TargetQuaternion, CROSS_PRODUCT_CLASS::TargetQuaternion_, (size_t)sizeof(Cross_class.TargetQuaternion), (uint32_t)0x00007000);
    read_param(Cross_class.TargetH, CROSS_PRODUCT_CLASS::TargetH_, (size_t)sizeof(Cross_class.TargetH), (uint32_t)0x00007010);
    read_param(Cross_class.ControlDirection, CROSS_PRODUCT_CLASS::ControlDirection_, (size_t)sizeof(Cross_class.ControlDirection), (uint32_t)0x0000701C);
    read_param(Cross_class.Kc, CROSS_PRODUCT_CLASS::Kc_, (uint16_t)sizeof(Cross_class.Kc), (uint32_t)0x00007028);
    read_param(Cross_class.Kpps, CROSS_PRODUCT_CLASS::Kpp_s, (uint16_t)sizeof(Cross_class.Kpps), (uint32_t)0x00007034);
    read_param(Cross_class.Kpds, CROSS_PRODUCT_CLASS::Kpd_s, (uint16_t)sizeof(Cross_class.Kpds), (uint32_t)0x00007040);
    read_param(Cross_class.Kqp, CROSS_PRODUCT_CLASS::Kqp_, (uint16_t)sizeof(Cross_class.Kqp), (uint32_t)0x0000704C);
    read_param(Cross_class.Kqd, CROSS_PRODUCT_CLASS::Kqd_, (uint16_t)sizeof(Cross_class.Kqd), (uint32_t)0x00007058);
    read_param(Cross_class.Kppec, CROSS_PRODUCT_CLASS::Kpp_ec, (uint16_t)sizeof(Cross_class.Kppec), (uint32_t)0x000070D0);
    read_param(Cross_class.Kpdec, CROSS_PRODUCT_CLASS::Kpd_ec, (uint16_t)sizeof(Cross_class.Kpdec), (uint32_t)0x000070DC);
    read_param(Cross_class.Kppep, CROSS_PRODUCT_CLASS::Kpp_ep, (uint16_t)sizeof(Cross_class.Kppep), (uint32_t)0x000070E8);
    read_param(Cross_class.Kpdep, CROSS_PRODUCT_CLASS::Kpd_ep, (uint16_t)sizeof(Cross_class.Kpdep), (uint32_t)0x000070F4);
    read_param(Cross_class.Kppi, CROSS_PRODUCT_CLASS::Kpp_i, (uint16_t)sizeof(Cross_class.Kppi), (uint32_t)0x00007100);
    read_param(Cross_class.Kpdi, CROSS_PRODUCT_CLASS::Kpd_i, (uint16_t)sizeof(Cross_class.Kpdi), (uint32_t)0x0000710C);
    read_param(Cross_class.Kppv, CROSS_PRODUCT_CLASS::Kpp_v, (uint16_t)sizeof(Cross_class.Kppv), (uint32_t)0x00007118);
    read_param(Cross_class.Kpdv, CROSS_PRODUCT_CLASS::Kpd_v, (uint16_t)sizeof(Cross_class.Kpdv), (uint32_t)0x00007124);
    read_param(Cross_class.Kppm, CROSS_PRODUCT_CLASS::Kpp_m, (uint16_t)sizeof(Cross_class.Kppm), (uint32_t)0x00007130);
    read_param(Cross_class.Kpdm, CROSS_PRODUCT_CLASS::Kpd_m, (uint16_t)sizeof(Cross_class.Kpdm), (uint32_t)0x0000713C);
    read_param(Cross_class.ControlDirection_Sun, CROSS_PRODUCT_CLASS::ControlDirection_s, (size_t)sizeof(Cross_class.ControlDirection_Sun), (uint32_t)0x00007148);
    read_param(Cross_class.ControlDirection_EarthC, CROSS_PRODUCT_CLASS::ControlDirection_ec, (size_t)sizeof(Cross_class.ControlDirection_EarthC), (uint32_t)0x00007154);
    read_param(Cross_class.ControlDirection_EarthP, CROSS_PRODUCT_CLASS::ControlDirection_ep, (size_t)sizeof(Cross_class.ControlDirection_EarthP), (uint32_t)0x00007160);
    read_param(Cross_class.ControlDirection_Ine, CROSS_PRODUCT_CLASS::ControlDirection_i, (size_t)sizeof(Cross_class.ControlDirection_Ine), (uint32_t)0x0000716C);
    read_param(Cross_class.ControlDirection_Vel, CROSS_PRODUCT_CLASS::ControlDirection_v, (size_t)sizeof(Cross_class.ControlDirection_Vel), (uint32_t)0x00007178);
    read_param(Cross_class.ControlDirection_M, CROSS_PRODUCT_CLASS::ControlDirection_m, (size_t)sizeof(Cross_class.ControlDirection_M), (uint32_t)0x00007184);
    read_param(Cross_class.Kpdsc, CROSS_PRODUCT_CLASS::Kpd_sc, (uint16_t)sizeof(Cross_class.Kpdsc), (uint32_t)0x00007190);
    read_param(Cross_class.Kppsc, CROSS_PRODUCT_CLASS::Kpp_sc, (uint16_t)sizeof(Cross_class.Kppsc), (uint32_t)0x000071A8);
    read_param(Cross_class.ControlDirection_Spin, CROSS_PRODUCT_CLASS::ControlDirection_sc, (size_t)sizeof(Cross_class.ControlDirection_Spin), (uint32_t)0x000071B4);
    read_param(Cross_class.TargetDirection_Spin, CROSS_PRODUCT_CLASS::TargetDirection_sc, (size_t)sizeof(Cross_class.TargetDirection_Spin), (uint32_t)0x000071C0);
    read_param(Cross_class.TargetOmeA, CROSS_PRODUCT_CLASS::TargetOmeA_, (size_t)sizeof(Cross_class.TargetOmeA), (uint32_t)0x000071CC);
    read_param(Cross_class.TargetPosition, CROSS_PRODUCT_CLASS::TargetPosition_, (size_t)sizeof(Cross_class.TargetPosition), (uint32_t)0x0000726c);
    read_param(&Cross_class.eclipse_mode_s, &CROSS_PRODUCT_CLASS::eclipse_mode_s_, (size_t)sizeof(Cross_class.eclipse_mode_s), (uint32_t)0x0000727C);
    read_param(&Cross_class.eclipse_mode_ec, &CROSS_PRODUCT_CLASS::eclipse_mode_ec_, (size_t)sizeof(Cross_class.eclipse_mode_ec), (uint32_t)0x0000727D);
    read_param(&Cross_class.eclipse_mode_ep, &CROSS_PRODUCT_CLASS::eclipse_mode_ep_, (size_t)sizeof(Cross_class.eclipse_mode_ep), (uint32_t)0x0000727E);
    read_param(&Cross_class.eclipse_mode_i, &CROSS_PRODUCT_CLASS::eclipse_mode_i_, (size_t)sizeof(Cross_class.eclipse_mode_i), (uint32_t)0x0000727F);
    read_param(&Cross_class.eclipse_mode_v, &CROSS_PRODUCT_CLASS::eclipse_mode_v_, (size_t)sizeof(Cross_class.eclipse_mode_v), (uint32_t)0x00007280);
    read_param(&Cross_class.eclipse_mode_m, &CROSS_PRODUCT_CLASS::eclipse_mode_m_, (size_t)sizeof(Cross_class.eclipse_mode_m), (uint32_t)0x00007281);
    read_param(&Cross_class.eclipse_mode_q, &CROSS_PRODUCT_CLASS::eclipse_mode_q_, (size_t)sizeof(Cross_class.eclipse_mode_q), (uint32_t)0x00007282);
    read_param(&Cross_class.eclipse_mode_sc, &CROSS_PRODUCT_CLASS::eclipse_mode_sc_, (size_t)sizeof(Cross_class.eclipse_mode_sc), (uint32_t)0x00007283);
    read_param(&Cross_class.eclipse_mode_c, &CROSS_PRODUCT_CLASS::eclipse_mode_c_, (size_t)sizeof(Cross_class.eclipse_mode_c), (uint32_t)0x00007288);
    read_param(&Cross_class.TargetOmega_spinup, &CROSS_PRODUCT_CLASS::TargetOmega_spinup_, (size_t)sizeof(Cross_class.TargetOmega_spinup), (uint32_t)0x000072BC);
    read_param(&Cross_class.TargetOmega_spindown, &CROSS_PRODUCT_CLASS::TargetOmega_spindown_, (size_t)sizeof(Cross_class.TargetOmega_spindown), (uint32_t)0x000072C0);
    read_param(&Cross_class.TargetOmega_damping, &CROSS_PRODUCT_CLASS::TargetOmega_damping_, (size_t)sizeof(Cross_class.TargetOmega_damping), (uint32_t)0x000072C4);
    read_param(&Cross_class.timeout_damping, &CROSS_PRODUCT_CLASS::timeout_damping_, (size_t)sizeof(Cross_class.timeout_damping), (uint32_t)0x000072C8);
    read_param(&Cross_class.timeout_spindown, &CROSS_PRODUCT_CLASS::timeout_spindown_, (size_t)sizeof(Cross_class.timeout_spindown), (uint32_t)0x000072C9);
    read_param(&Cross_class.control_dir_cor, &CROSS_PRODUCT_CLASS::control_dir_cor_, (size_t)sizeof(Cross_class.control_dir_cor), (uint32_t)0x000072CA);
    read_param(&Cross_class.control_norm_dir_cor, &CROSS_PRODUCT_CLASS::control_norm_dir_cor_, (size_t)sizeof(Cross_class.control_norm_dir_cor), (uint32_t)0x000072CE);
    read_param(&Cross_class.Cd_Td_thr, &CROSS_PRODUCT_CLASS::Cd_Td_thr_, (size_t)sizeof(Cross_class.Cd_Td_thr), (uint32_t)0x000072D2);
    read_param(&Cross_class.B_Cd_thr, &CROSS_PRODUCT_CLASS::B_Cd_thr_, (size_t)sizeof(Cross_class.B_Cd_thr), (uint32_t)0x000072D6);

//    printf("Kpp %f %f %f\r\n", Cross_class.Kpp[0], Cross_class.Kpp[1], Cross_class.Kpp[2]);
//    printf("Kpd %f %f %f\r\n", Cross_class.Kpd[0], Cross_class.Kpd[1], Cross_class.Kpd[2]);
//    printf("Kqd %f %f %f\r\n", Cross_class.Kqd[0], Cross_class.Kqd[1], Cross_class.Kqd[2]);
//    printf("Kqp %f %f %f\r\n", Cross_class.Kqp[0], Cross_class.Kqp[1], Cross_class.Kqp[2]);
}

/**
 * @brief アプリケーション実行関数
 *
 */
static void CrossProduct_initialize_param(void)
{
    printf("CrossProduct initialize_param.\n");
    write_param(Cross_class.TargetQuaternion, CROSS_PRODUCT_CLASS::TargetQuaternion_, (uint16_t)sizeof(Cross_class.TargetQuaternion), (uint32_t)0x00007000);
    write_param(Cross_class.TargetH, CROSS_PRODUCT_CLASS::TargetH_, (uint16_t)sizeof(Cross_class.TargetH), (uint32_t)0x00007010);
    write_param(Cross_class.ControlDirection, CROSS_PRODUCT_CLASS::ControlDirection_, (uint16_t)sizeof(Cross_class.ControlDirection), (uint32_t)0x0000701C);
    write_param(Cross_class.Kc, CROSS_PRODUCT_CLASS::Kc_, (uint16_t)sizeof(Cross_class.Kc), (uint32_t)0x00007028);
    write_param(Cross_class.Kpps, CROSS_PRODUCT_CLASS::Kpp_s, (uint16_t)sizeof(Cross_class.Kpps), (uint32_t)0x00007034);
    write_param(Cross_class.Kpds, CROSS_PRODUCT_CLASS::Kpd_s, (uint16_t)sizeof(Cross_class.Kpds), (uint32_t)0x00007040);
    write_param(Cross_class.Kqp, CROSS_PRODUCT_CLASS::Kqp_, (uint16_t)sizeof(Cross_class.Kqp), (uint32_t)0x0000704C);
    write_param(Cross_class.Kqd, CROSS_PRODUCT_CLASS::Kqd_, (uint16_t)sizeof(Cross_class.Kqd), (uint32_t)0x00007058);
    write_param(Cross_class.Kppec, CROSS_PRODUCT_CLASS::Kpp_ec, (uint16_t)sizeof(Cross_class.Kppec), (uint32_t)0x000070D0);
    write_param(Cross_class.Kpdec, CROSS_PRODUCT_CLASS::Kpd_ec, (uint16_t)sizeof(Cross_class.Kpdec), (uint32_t)0x000070DC);
    write_param(Cross_class.Kppep, CROSS_PRODUCT_CLASS::Kpp_ep, (uint16_t)sizeof(Cross_class.Kppep), (uint32_t)0x000070E8);
    write_param(Cross_class.Kpdep, CROSS_PRODUCT_CLASS::Kpd_ep, (uint16_t)sizeof(Cross_class.Kpdep), (uint32_t)0x000070F4);
    write_param(Cross_class.Kppi, CROSS_PRODUCT_CLASS::Kpp_i, (uint16_t)sizeof(Cross_class.Kppi), (uint32_t)0x00007100);
    write_param(Cross_class.Kpdi, CROSS_PRODUCT_CLASS::Kpd_i, (uint16_t)sizeof(Cross_class.Kpdi), (uint32_t)0x0000710C);
    write_param(Cross_class.Kppv, CROSS_PRODUCT_CLASS::Kpp_v, (uint16_t)sizeof(Cross_class.Kppv), (uint32_t)0x00007118);
    write_param(Cross_class.Kpdv, CROSS_PRODUCT_CLASS::Kpd_v, (uint16_t)sizeof(Cross_class.Kpdv), (uint32_t)0x00007124);
    write_param(Cross_class.Kppm, CROSS_PRODUCT_CLASS::Kpp_m, (uint16_t)sizeof(Cross_class.Kppm), (uint32_t)0x00007130);
    write_param(Cross_class.Kpdm, CROSS_PRODUCT_CLASS::Kpd_m, (uint16_t)sizeof(Cross_class.Kpdm), (uint32_t)0x0000713C);
    write_param(Cross_class.ControlDirection_Sun, CROSS_PRODUCT_CLASS::ControlDirection_s, (size_t)sizeof(Cross_class.ControlDirection_Sun), (uint32_t)0x00007148);
    write_param(Cross_class.ControlDirection_EarthC, CROSS_PRODUCT_CLASS::ControlDirection_ec, (size_t)sizeof(Cross_class.ControlDirection_EarthC), (uint32_t)0x00007154);
    write_param(Cross_class.ControlDirection_EarthP, CROSS_PRODUCT_CLASS::ControlDirection_ep, (size_t)sizeof(Cross_class.ControlDirection_EarthP), (uint32_t)0x00007160);
    write_param(Cross_class.ControlDirection_Ine, CROSS_PRODUCT_CLASS::ControlDirection_i, (size_t)sizeof(Cross_class.ControlDirection_Ine), (uint32_t)0x0000716C);
    write_param(Cross_class.ControlDirection_Vel, CROSS_PRODUCT_CLASS::ControlDirection_v, (size_t)sizeof(Cross_class.ControlDirection_Vel), (uint32_t)0x00007178);
    write_param(Cross_class.ControlDirection_M, CROSS_PRODUCT_CLASS::ControlDirection_m, (size_t)sizeof(Cross_class.ControlDirection_M), (uint32_t)0x00007184);
    write_param(Cross_class.Kpdsc, CROSS_PRODUCT_CLASS::Kpd_sc, (uint16_t)sizeof(Cross_class.Kpdsc), (uint32_t)0x00007190);
    write_param(Cross_class.Kppsc, CROSS_PRODUCT_CLASS::Kpp_sc, (uint16_t)sizeof(Cross_class.Kppsc), (uint32_t)0x000071A8);
    write_param(Cross_class.ControlDirection_Spin, CROSS_PRODUCT_CLASS::ControlDirection_sc, (size_t)sizeof(Cross_class.ControlDirection_Spin), (uint32_t)0x000071B4);
    write_param(Cross_class.TargetDirection_Spin, CROSS_PRODUCT_CLASS::TargetDirection_sc, (size_t)sizeof(Cross_class.TargetDirection_Spin), (uint32_t)0x000071C0);
    write_param(Cross_class.TargetOmeA, CROSS_PRODUCT_CLASS::TargetOmeA_, (size_t)sizeof(Cross_class.TargetOmeA), (uint32_t)0x000071CC);
    write_param(Cross_class.TargetPosition, CROSS_PRODUCT_CLASS::TargetPosition_, (size_t)sizeof(Cross_class.TargetPosition), (uint32_t)0x0000726c);
    write_param(&Cross_class.eclipse_mode_s, &CROSS_PRODUCT_CLASS::eclipse_mode_s_, (size_t)sizeof(Cross_class.eclipse_mode_s), (uint32_t)0x0000727C);
    write_param(&Cross_class.eclipse_mode_ec, &CROSS_PRODUCT_CLASS::eclipse_mode_ec_, (size_t)sizeof(Cross_class.eclipse_mode_ec), (uint32_t)0x0000727D);
    write_param(&Cross_class.eclipse_mode_ep, &CROSS_PRODUCT_CLASS::eclipse_mode_ep_, (size_t)sizeof(Cross_class.eclipse_mode_ep), (uint32_t)0x0000727E);
    write_param(&Cross_class.eclipse_mode_i, &CROSS_PRODUCT_CLASS::eclipse_mode_i_, (size_t)sizeof(Cross_class.eclipse_mode_i), (uint32_t)0x0000727F);
    write_param(&Cross_class.eclipse_mode_v, &CROSS_PRODUCT_CLASS::eclipse_mode_v_, (size_t)sizeof(Cross_class.eclipse_mode_v), (uint32_t)0x00007280);
    write_param(&Cross_class.eclipse_mode_m, &CROSS_PRODUCT_CLASS::eclipse_mode_m_, (size_t)sizeof(Cross_class.eclipse_mode_m), (uint32_t)0x00007281);
    write_param(&Cross_class.eclipse_mode_q, &CROSS_PRODUCT_CLASS::eclipse_mode_q_, (size_t)sizeof(Cross_class.eclipse_mode_q), (uint32_t)0x00007282);
    write_param(&Cross_class.eclipse_mode_sc, &CROSS_PRODUCT_CLASS::eclipse_mode_sc_, (size_t)sizeof(Cross_class.eclipse_mode_sc), (uint32_t)0x00007283);
    write_param(&Cross_class.eclipse_mode_c, &CROSS_PRODUCT_CLASS::eclipse_mode_c_, (size_t)sizeof(Cross_class.eclipse_mode_c), (uint32_t)0x00007288);
    write_param(&Cross_class.TargetOmega_spinup, &CROSS_PRODUCT_CLASS::TargetOmega_spinup_, (size_t)sizeof(Cross_class.TargetOmega_spinup), (uint32_t)0x000072BC);
    write_param(&Cross_class.TargetOmega_spindown, &CROSS_PRODUCT_CLASS::TargetOmega_spindown_, (size_t)sizeof(Cross_class.TargetOmega_spindown), (uint32_t)0x000072C0);
    write_param(&Cross_class.TargetOmega_damping, &CROSS_PRODUCT_CLASS::TargetOmega_damping_, (size_t)sizeof(Cross_class.TargetOmega_damping), (uint32_t)0x000072C4);
    write_param(&Cross_class.timeout_damping, &CROSS_PRODUCT_CLASS::timeout_damping_, (size_t)sizeof(Cross_class.timeout_damping), (uint32_t)0x000072C8);
    write_param(&Cross_class.timeout_spindown, &CROSS_PRODUCT_CLASS::timeout_spindown_, (size_t)sizeof(Cross_class.timeout_spindown), (uint32_t)0x000072C9);
    write_param(&Cross_class.control_dir_cor, &CROSS_PRODUCT_CLASS::control_dir_cor_, (size_t)sizeof(Cross_class.control_dir_cor), (uint32_t)0x000072CA);
    write_param(&Cross_class.control_norm_dir_cor, &CROSS_PRODUCT_CLASS::control_norm_dir_cor_, (size_t)sizeof(Cross_class.control_norm_dir_cor), (uint32_t)0x000072CE);
    write_param(&Cross_class.Cd_Td_thr, &CROSS_PRODUCT_CLASS::Cd_Td_thr_, (size_t)sizeof(Cross_class.Cd_Td_thr), (uint32_t)0x000072D2);
    write_param(&Cross_class.B_Cd_thr, &CROSS_PRODUCT_CLASS::B_Cd_thr_, (size_t)sizeof(Cross_class.B_Cd_thr), (uint32_t)0x000072D6);
}

/**
 * @brief コマンド実行関数
 *
 * 初期化のいらない処理内容を記述．
 */


//---------------------------------------クロスプロダクトによる角運動量管理--------------------------------------------------
static void ACS_CrossProduct_AngularMomentum_update_(void){
	///Input: 地磁気，クォータニオン，目標角運動量ベクトル
	///Output: 磁気モーメント


//		float B_fld_[3] = {g_acs.p_GLOBAL_AOCS->MagField[0], g_acs.p_GLOBAL_AOCS->MagField[1], g_acs.p_GLOBAL_AOCS->MagField[2]};
//		float omega[3] = {g_acs.p_GLOBAL_AOCS->Omega[0], g_acs.p_GLOBAL_AOCS->Omega[1], g_acs.p_GLOBAL_AOCS->Omega[2]};
	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_c == 2){
	    Cross_class.Angcal(B_fld_, omega, Cross_class.TargetH);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

		copy(Cross_class.OUT_Moment_b_, 3, 1, Cross_.Mag_moment);

//		GLOBAL_AOCS_set_Mag_moment(Cross_.Mag_moment);
		GLOBAL_ACS_set_Mag_moment_Cn(Cross_.Mag_moment);

#ifdef HILS
//		// Send torque to S2E
//		uint8_t mtq_dummyt_u8[24];
//		uint8_t dummy_cmd[30] = { 'M', 'T', 'Q',  '0' };
//		float mtq_control[3] = { Cross_.Mag_moment[0],Cross_.Mag_moment[1],Cross_.Mag_moment[2] };
//		memcpy(dummy_cmd + 4, mtq_control, 12);
//		HAL_UART_Transmit(&huart7,(uint8_t*)&dummy_cmd,16, 0xff);
//		print(CPRODUCT,"CrossProduct_AngularMomentum\r\n");
		print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
		print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
//		print(CPRODUCT,"Mag moment %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);
		print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);

//
#endif
}
//-----------------------------------------------------------------------------------------------------------------------






//-----------------------------------クロスプロダクトによるポインティング制御-------------------------------------------------------------

/**
 * @brief 太陽ポインティング制御則により要求磁気モーメントを計算する
 * @param なし
 * @return なし
 *
 * 作成者: 増田 2021/03/27
 * コメント記入者: 浮田 2023/06/22
 */
static void ACS_CrossProduct_Sun_Pointing_update_(void)
{
  print(CPRODUCT,"SUN Pointing\r\n");

  // 機体座標系の地磁気ベクトル [T]
  float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
  // 機体座標系の角速度ベクトル [rad/s]
  float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};

#ifdef HILS
//	B_fld_[0] = magsbuf[0]*0.000000001f;
//	B_fld_[1] = magsbuf[1]*0.000000001f;
//	B_fld_[2] = magsbuf[2]*0.000000001f;
//	omega[0] = gyrobuf[0];
//	omega[1] = gyrobuf[1];
//	omega[2] = gyrobuf[2];
//	float sunvec[3] = {suns1buf[0], suns1buf[1], suns1buf[2]};
#endif

  if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_s == 2){
      // 蝕でない状態またはeclipse_modeが2のとき
      Cross_class.Poincal(B_fld_, omega, g_ads.p_ADS->SunVec, Cross_class.ControlDirection_Sun, Cross_class.TargetOmeA, Cross_class.Kpps, Cross_class.Kpds);
  }
  else if (Cross_class.eclipse_mode_s == 1){
      //  蝕状態　かつ　eclipse_modeが1のとき
      Cross_class.Poincal_eclipse(B_fld_, omega, g_ads.p_ADS->SunVec, Cross_class.ControlDirection_Sun, Cross_class.TargetOmeA, Cross_class.Kpps, Cross_class.Kpds);
  }
  else{
      // 蝕状態　かつ　eclipse_modeが1,2以外のとき
      // 要求磁気モーメントを0とする
      Cross_class.OUT_Moment_b_[0] = 0.0f;
      Cross_class.OUT_Moment_b_[1] = 0.0f;
      Cross_class.OUT_Moment_b_[2] = 0.0f;
  }

//	Cross_class.Poincal(B_fld_, omega, suns7buf, Cross_class.ControlDirection_Sun, Cross_class.TargetOmeA, Cross_class.Kpps, Cross_class.Kpds);

//	void CROSS_PRODUCT_CLASS::cross_product_Pointing(float in_B_fld_[3], float in_Omega[3], const float in_TD[3], float in_CD[3], float in_TOme[3], float Kpp[3], float Kpd[3], float out_Moment_b_[3])

//#ifdef HILS
//	Cross_class.Poincal(B_fld_, omega, g_acs.p_GLOBAL_AOCS->SunVec, Cross_class.ControlDirection_Sun, Cross_class.TargetOmeA, Cross_class.Kpps, Cross_class.Kpds);
//#endif

        // 計算されたOUT_Moment_b_をMag_momentに代入
	copy(Cross_class.OUT_Moment_b_, 3, 1, Cross_.Mag_moment);

	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);

//	GLOBAL_AOCS_set_Mag_moment(Cross_.Mag_moment);
	// 最終的な要求磁気トルクをGLOBAL_ACS_PARAMETERに登録
	GLOBAL_ACS_set_Mag_moment_Cn(Cross_.Mag_moment);

#ifdef HILS

	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);

#endif

}


static void ACS_CrossProduct_Earth_Center_Pointing_update_(void)
{

///地球中心方向指向
///Input: 地磁気，クォータニオン，目標ベクトル
///Output: 磁気モーメント

	print(CPRODUCT,"Earth Center Pointing\r\n");

	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};


//	// Calculate orbit angular velocity vector
//	float r[3] = {g_ads.p_ADS->Position[0], g_ads.p_ADS->Position[1], g_ads.p_ADS->Position[2]};
//	float v[3] = {g_ads.p_ADS->Velocity[0], g_ads.p_ADS->Velocity[1], g_ads.p_ADS->Velocity[2]};
//
//	float Buf[3] = {0.0f};
//	float ome_orbit[3] = {0.0f};
//	float ome_orbit_b[3] = {0.0f};
//	float r_norm = 0.0f;
//
//	Outer_product(r, v, Buf);
//	r_norm = Fabs(r,3);
//	float coeff = 1.0f / (r_norm * r_norm);
//	CoeffVec(Buf, 3, coeff, ome_orbit);
//
//	multiplication(*g_ads.p_ADS->DCM_i2b, 3, 3, ome_orbit, 3, 1, ome_orbit_b, 3, 1);

	// Calculate required torque
	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_ec == 2){
		Cross_class.Poincal(B_fld_, omega, g_acs.p_EARTHVECTOR->EarthCenterVector_Body, Cross_class.ControlDirection_EarthC, Cross_class.TargetOmeA, Cross_class.Kppec, Cross_class.Kpdec);
	}
	else if (Cross_class.eclipse_mode_ec == 1){
		Cross_class.Poincal_eclipse(B_fld_, omega, g_acs.p_EARTHVECTOR->EarthCenterVector_Body, Cross_class.ControlDirection_EarthC, Cross_class.TargetOmeA, Cross_class.Kppec, Cross_class.Kpdec);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

//	Cross_class.Poincal(B_fld_, omega, g_acs.p_EARTHVECTOR->EarthCenterVector_Body, Cross_class.ControlDirection_EarthC, ome_orbit_b, Cross_class.Kppec, Cross_class.Kpdec);

	copy(Cross_class.OUT_Moment_b_, 3, 1, Cross_.Mag_moment);

	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);

//	GLOBAL_AOCS_set_Mag_moment(Cross_.Mag_moment);
	GLOBAL_ACS_set_Mag_moment_Cn(Cross_.Mag_moment);
	GLOBAL_ACS_set_TargetVec(g_acs.p_EARTHVECTOR->EarthCenterVector_Body);

#ifdef HILS
//	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
//	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
//	print(CPRODUCT,"Mag moment %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);


//	print(CPRODUCT,"r  %f %f %f\r\n", r[0], r[1], r[2]);
//	print(CPRODUCT,"v %f %f %f\r\n", v[0], v[1], v[2]);
//	print(CPRODUCT,"Buf %f %f %f\r\n", Buf[0], Buf[1], Buf[2]);
//	print(CPRODUCT,"Omega orbit %f %f %f\r\n", ome_orbit[0], ome_orbit[1], ome_orbit[2]);
//	print(CPRODUCT,"Omega orbit in body frame %f %f %f\r\n", ome_orbit_b[0], ome_orbit_b[1], ome_orbit_b[2]);

//	printf("r %f %f %f ", r[0], r[1], r[2]);
//	printf("v %f %f %f ", v[0], v[1], v[2]);
//	printf("orb_ome %f %f %f ", ome_orbit[0], ome_orbit[1], ome_orbit[2]);
//	printf("b_fld %f %f %f ", B_fld_[0], B_fld_[1], B_fld_[2]);
//	float sun[3] = {g_acs.p_GLOBAL_AOCS->SunVec[0], g_acs.p_GLOBAL_AOCS->SunVec[1], g_acs.p_GLOBAL_AOCS->SunVec[2]};
//	printf("sun_b %f %f %f ", sun[0], sun[1], sun[2]);
//	printf("gyro %f %f %f ", omega[0], omega[1], omega[2]);
	float q[4] = {g_ads.p_ADS->Quaternion[0], g_ads.p_ADS->Quaternion[1], g_ads.p_ADS->Quaternion[2], g_ads.p_ADS->Quaternion[3]};
//	printf("qua %f %f %f %f ", q[0], q[1], q[2], q[3]);
//	printf("orb_ome_b %f %f %f ", ome_orbit_b[0], ome_orbit_b[1], ome_orbit_b[2]);
//	printf("Target %f %f %f ", g_acs.p_EARTHVECTOR->EarthCenterVector_Body[0], g_acs.p_EARTHVECTOR->EarthCenterVector_Body[1], g_acs.p_EARTHVECTOR->EarthCenterVector_Body[2]);
//	printf("Control %f %f %f ", Cross_class.ControlDirection_EarthC[0], Cross_class.ControlDirection_EarthC[1], Cross_class.ControlDirection_EarthC[2]);
//	printf("MagMoment %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);

#endif
}



static void ACS_CrossProduct_Earth_Point_Pointing_update_(void)
{

///地球一点方向指向
///Input: 地磁気，クォータニオン，目標ベクトル
///Output: 磁気モーメント

	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_ep == 2){
		Cross_class.Poincal(B_fld_, omega, g_acs.p_EARTHVECTOR->EarthPointVector_Body, Cross_class.ControlDirection_EarthP, Cross_class.TargetOmeA, Cross_class.Kppep, Cross_class.Kpdep);
	}
	else if (Cross_class.eclipse_mode_ep == 1){
		Cross_class.Poincal_eclipse(B_fld_, omega, g_acs.p_EARTHVECTOR->EarthPointVector_Body, Cross_class.ControlDirection_EarthP, Cross_class.TargetOmeA, Cross_class.Kppep, Cross_class.Kpdep);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(Cross_class.OUT_Moment_b_, 3, 1, Cross_.Mag_moment);

//	GLOBAL_AOCS_set_Mag_moment(Cross_.Mag_moment);
	GLOBAL_ACS_set_Mag_moment_Cn(Cross_.Mag_moment);
	GLOBAL_ACS_set_TargetVec(g_acs.p_EARTHVECTOR->EarthPointVector_Body);

#ifdef HILS

	print(CPRODUCT,"Earth Point Pointing\r\n");
	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);

#endif
}



static void ACS_CrossProduct_Inertial_Pointing_update_(void)
{
///慣性方向指向
///Input: 地磁気，クォータニオン，目標ベクトル
///Output: 磁気モーメント

	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};
	float InertialTarget[3];
	float InertialTarget_Norm[3];
	float Body_InertialTarget_Norm[3];
	sub(Cross_class.TargetPosition, 3, 1, g_ads.p_ADS->Position, 3, 1, InertialTarget);

	Normalization(InertialTarget, 3, InertialTarget_Norm);

	multiplication(*g_ads.p_ADS->DCM_i2b, 3, 3, InertialTarget_Norm, 3, 1, Body_InertialTarget_Norm, 3, 1);

	GLOBAL_ACS_set_InertialVec(Body_InertialTarget_Norm);

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_i == 2){
		Cross_class.Poincal(B_fld_, omega, Body_InertialTarget_Norm, Cross_class.ControlDirection_Ine, Cross_class.TargetOmeA,  Cross_class.Kppi, Cross_class.Kpdi);
	}
	else if (Cross_class.eclipse_mode_i == 1){
		Cross_class.Poincal_eclipse(B_fld_, omega, Body_InertialTarget_Norm, Cross_class.ControlDirection_Ine, Cross_class.TargetOmeA,  Cross_class.Kppi, Cross_class.Kpdi);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(Cross_class.OUT_Moment_b_, 3, 1, Cross_.Mag_moment);

//	GLOBAL_AOCS_set_Mag_moment(Cross_.Mag_moment);
	GLOBAL_ACS_set_Mag_moment_Cn(Cross_.Mag_moment);
	GLOBAL_ACS_set_TargetVec(Body_InertialTarget_Norm);

#ifdef HILS
	print(CPRODUCT,"Inertial Pointing\r\n");
	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);
#endif
}

	

		
static void ACS_CrossProduct_Velocity_Pointing_update_(void)
{
///速度方向指向
///Input: 地磁気，クォータニオン，目標ベクトル
///Output: 磁気モーメント

	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};
	float vel[3] = {g_ads.p_ADS->Velocity[0], g_ads.p_ADS->Velocity[1], g_ads.p_ADS->Velocity[2]};
	float vel_vec[3];
	float vel_vec_b[3];
	Normalization(vel,3,vel_vec);

	multiplication(*g_ads.p_ADS->DCM_i2b, 3, 3, vel_vec, 3, 1, vel_vec_b, 3, 1);

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_v == 2){
		Cross_class.Poincal(B_fld_, omega, vel_vec_b, Cross_class.ControlDirection_Vel, Cross_class.TargetOmeA, Cross_class.Kppv, Cross_class.Kpdv);
	}
	else if (Cross_class.eclipse_mode_v == 1){
		Cross_class.Poincal_eclipse(B_fld_, omega, vel_vec_b, Cross_class.ControlDirection_Vel, Cross_class.TargetOmeA, Cross_class.Kppv, Cross_class.Kpdv);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

//    printf("sgp4_Velocity_Vector_b %f %f %f\n\r", g_ods.p_SGP4->sgp4_Velocity_Vector_b[0], g_ods.p_SGP4->sgp4_Velocity_Vector_b[1], g_ods.p_SGP4->sgp4_Velocity_Vector_b[2]);

	copy(Cross_class.OUT_Moment_b_, 3, 1, Cross_.Mag_moment);

	
//	GLOBAL_AOCS_set_Mag_moment(Cross_.Mag_moment);
	GLOBAL_ACS_set_Mag_moment_Cn(Cross_.Mag_moment);
	GLOBAL_ACS_set_TargetVec(vel_vec_b);
	GLOBAL_ACS_set_Velocity_b(vel_vec_b);


#ifdef HILS
	print(CPRODUCT,"vel_vec %f %f %f ", vel_vec[0], vel_vec[1], vel_vec[2]);
	print(CPRODUCT,"vel_vec_b %f %f %f ", vel_vec_b[0], vel_vec_b[1],vel_vec_b[2]);
	print(CPRODUCT,"Control_Direc %f %f %f ", Cross_class.ControlDirection_Vel[0], Cross_class.ControlDirection_Vel[1], Cross_class.ControlDirection_Vel[2]);
	print(CPRODUCT,"Req'd_Mag_mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);

	//	print(CPRODUCT,"VelocityPointing\r\n");
//	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
//	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
//	print(CPRODUCT,"Mag moment %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);
//	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);

#endif
}



static void ACS_CrossProduct_Trajectry_Pointing_update_(void)
{
#ifdef TWO_U
///地球中心方向指向
///Input: 地磁気，クォータニオン，目標ベクトル
///Output: 磁気モーメント

	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};

	
	float  COBCVec[3] = {0.0f, 1.0f, 0.0f};
	// param = COBC から送られてくる目標方向

	for(int i = 0; i < 3; i++){
		memcpy(&COBCVec[i], &(cobc_driver->tlm_data.cobc_aocs_tlm.target_vector[i]), 4);
//		printf("cobc_vec is %f \r\n", COBCVec[i]);
//		printf("target_vector is %f \r\n", cobc_driver->tlm_data.cobc_aocs_tlm.target_vector[i]);
	}

	EARTHVECTOR_class.EarthVector_calc(*g_ads.p_ADS->DCM_i2b, g_ads.p_GST->gst, g_ods.p_ODS->Position, COBCVec);

	GLOBAL_ACS_set_COBCVec(COBCVec);

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_m == 2){
		Cross_class.Poincal(B_fld_, omega, g_acs.p_EARTHVECTOR->EarthPointVector_Body, Cross_class.ControlDirection_M, Cross_class.TargetOmeA, Cross_class.Kppm, Cross_class.Kpdm);
	}
	else if (Cross_class.eclipse_mode_m == 1){
		Cross_class.Poincal_eclipse(B_fld_, omega, COBCVec, Cross_class.ControlDirection_M, Cross_class.TargetOmeA, Cross_class.Kppm, Cross_class.Kpdm);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

	//		GLOBAL_AOCS_set_Mag_moment(Cross_.Mag_moment);
			GLOBAL_ACS_set_Mag_moment_Cn(Cross_.Mag_moment);
			GLOBAL_ACS_set_TargetVec(COBCVec);

#ifdef HILS
	print(CPRODUCT,"Trajectry_Pointing　Pointing\r\n");
	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
//	print(CPRODUCT,"Mag moment %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);
	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);
#endif

#endif
}
//-------------------------------------------------------------------------------------------------------------------



//---------------------------------------クロスプロダクトによる目標クォータニオン制御--------------------------------------------------
//おそらくクォータニオン偏差を差分でとっているので厳密なクォータニオンフィードバックではない（制御はある程度できる？）

static void ACS_CrossProduct_Quaternion_update_(void)
{

///Input: 地磁気，クォータニオン，目標クォータニオン
///Output: 磁気モーメント

//	float B_fld_[3] = {g_acs.p_GLOBAL_AOCS->MagField[0], g_acs.p_GLOBAL_AOCS->MagField[1], g_acs.p_GLOBAL_AOCS->MagField[2]};
//	float omega[3] = {g_acs.p_GLOBAL_AOCS->Omega[0], g_acs.p_GLOBAL_AOCS->Omega[1], g_acs.p_GLOBAL_AOCS->Omega[2]};
	float quaternion[4] = {g_ads.p_ADS->Quaternion[0], g_ads.p_ADS->Quaternion[1], g_ads.p_ADS->Quaternion[2], g_ads.p_ADS->Quaternion[3]};
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};
	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_q == 2){
	    Cross_class.Quacal(B_fld_, omega, quaternion, Cross_class.TargetQuaternion);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(Cross_class.OUT_Moment_b_, 3, 1, Cross_.Mag_moment);


	//		GLOBAL_AOCS_set_Mag_moment(Cross_.Mag_moment);
			GLOBAL_ACS_set_Mag_moment_Cn(Cross_.Mag_moment);

#ifdef HILS
	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
//	print(CPRODUCT,"Mag moment %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);
	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);
#endif


}
//---------------------------------------------------------------------------------------------------------------------------




//-----------------------------------クロスプロダクトによるクォータニオン積を用いたクォータニオンフィードバック制御-------------------------------------------------------------

static void ACS_CrossProduct_QFB_update_(void)
{


	
///Input: 地磁気，クォータニオン，目標ベクトル
///Output: 磁気モーメント

	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};
	float quaternion[4] = {g_ads.p_ADS->Quaternion[0], g_ads.p_ADS->Quaternion[1], g_ads.p_ADS->Quaternion[2], g_ads.p_ADS->Quaternion[3]};
	
	
//	float maglimit[3] = {0.0f};
#ifdef HILS

//	B_fld_[0] = magsbuf[0]*0.000000001f;
//	B_fld_[1] = magsbuf[1]*0.000000001f;
//	B_fld_[2] = magsbuf[2]*0.000000001f;
//	omega[0] = gyrobuf[0];
//	omega[1] = gyrobuf[1];
//	omega[2] = gyrobuf[2];
#endif
	//太陽方向指
		//-----適当
		float SunVector_Qua_Buf[3] = {g_ads.p_SUNVECTOR->SunVector_Body[0], g_ads.p_SUNVECTOR->SunVector_Body[1], g_ads.p_SUNVECTOR->SunVector_Body[2]};
		Rotate_one_axis(SunVector_Qua_Buf, 0, 0, M_PI/2, SunVector_Qua_Buf);
		float SunVector_Qua[4] = {SunVector_Qua_Buf[0], SunVector_Qua_Buf[1], SunVector_Qua_Buf[2], 0.0f};
		//----------

		if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_q == 2){
			Cross_class.QFBcal(B_fld_, omega, quaternion, SunVector_Qua);
		}
		else{
			Cross_class.OUT_Moment_b_[0] = 0.0f;
			Cross_class.OUT_Moment_b_[1] = 0.0f;
			Cross_class.OUT_Moment_b_[2] = 0.0f;
		}

	copy(Cross_class.OUT_Moment_b_, 3, 1, Cross_.Mag_moment);


	//		GLOBAL_AOCS_set_Mag_moment(Cross_.Mag_moment);
			GLOBAL_ACS_set_Mag_moment_Cn(Cross_.Mag_moment);

#ifdef HILS
	print(CPRODUCT,"QFB\r\n");
	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
//	print(CPRODUCT,"Mag moment %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);
	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);
#endif
}


CCP_CmdRet Cmd_ACS_CrossProduct_SpinControl_HILS(const CommonCmdPacket* packet)
{
	print(CPRODUCT,"CrossProduct_SpinControl_HILS\r\n");

	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};
	float Td_Norm[3] = {0.0f};
	float Td_b[3] = {0.0f};
	float Td_b_Norm[3] = {0.0f};
	float Target_omega = 0.0f;

	// 機体座標系での目標方向計算
	Normalization(Cross_class.TargetDirection_Spin, 3, Td_Norm);
	print(CPRODUCT,"Td_Norm %f %f %f\r\n", Td_Norm[0], Td_Norm[1], Td_Norm[2]);
	multiplication(*g_ads.p_ADS->DCM_i2b, 3, 3, Td_Norm, 3, 1, Td_b, 3, 1);
	Normalization(Td_b, 3, Td_b_Norm);

	// 目標角速度の大きさを指定
	if (vote_flag(&Cross_class.timeout_damping) || Separated()){
		Target_omega = Cross_class.TargetOmega_damping;
	}
	else if (vote_flag(&Cross_class.timeout_spindown)){
		Target_omega = Cross_class.TargetOmega_spindown;
	}
	else{
		Target_omega = Cross_class.TargetOmega_spinup;
	}

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || Cross_class.eclipse_mode_sc == 2){
		Cross_class.Spincal(B_fld_, omega, Td_b_Norm, Cross_class.ControlDirection_Spin, Target_omega);
	}
	else{
		Cross_class.OUT_Moment_b_[0] = 0.0f;
		Cross_class.OUT_Moment_b_[1] = 0.0f;
		Cross_class.OUT_Moment_b_[2] = 0.0f;
	}

	for(int i = 0; i < 3; i ++ ){
		Cross_.Mag_moment[i] = Cross_class.OUT_Moment_b_[i];
	}

//	print(CPRODUCT,"Before Mag moment %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);

#ifdef HILS
	print(CPRODUCT,"GYRO %f %f %f\r\n", omega[0], omega[1], omega[2]);
	print(CPRODUCT,"MAG %f %f %f\r\n", B_fld_[0], B_fld_[1], B_fld_[2]);
//	print(CPRODUCT,"Mag moment %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);
	print(CPRODUCT,"Req'd Mag mmt %f %f %f\r\n", Cross_.Mag_moment[0], Cross_.Mag_moment[1], Cross_.Mag_moment[2]);

#endif
	GLOBAL_ACS_set_Mag_moment_Cn(Cross_.Mag_moment);
	GLOBAL_ACS_set_TargetVec(Td_b_Norm);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//多数決システム
uint8_t vote_flag(uint8_t* flag){
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
