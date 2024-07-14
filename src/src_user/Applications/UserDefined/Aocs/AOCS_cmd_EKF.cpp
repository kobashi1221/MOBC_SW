/*
 * AOCS_cmd_EKF.h
 *
 *  ver.1.0 2022/07/03 tamura
 *	軌道用ではないがいずれ拡張
 */

#include "AOCS_cmd_EKF.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Drivers/Aocs/EKF.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <stdlib.h>
#include <string.h>
#include "../Global_Sensor_Value.h"
#include <src_user/Applications/DriverInstances/block_command_handler.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>

static EKF_CLASS EKF_class[2];

static EKF_STRUCT EKF_;
const uint8_t ekf_order_[2] = {EKF_GYRO,0}; // 直列EKF
const uint8_t number_of_serials_ = 0;
const uint8_t eclipse_switch_flag_ = 1;

static void EKF_initialize_param(void);
static void EKF_load_param(void);


static void EKF_init(void);
static void EKF_exe(void);

uint8_t check_EKForder_change();
void set_obs_value(int i, ADS_MODE ekf_id);
void set_dimension(ADS_MODE ekf_id, uint8_t& ref_dim, uint8_t& obs_dim, uint8_t& P_dim, uint8_t& Q_dim, uint8_t& R_dim);
void pass_EKFclass_for_serial(uint8_t i, ADS_MODE ekf_id);
void copy_EKFclass_to_struct(uint8_t i, ADS_MODE ekf_id, uint8_t Pdim);
void set_global_to_EKFvalue(uint8_t i, ADS_MODE ekf_id);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */

AppInfo EKF_param_update(void)
{
    return AI_create_app_info("EKF_PARAM_UPDATE", EKF_load_param, EKF_initialize_param);
}

AppInfo EKF_update(void)
{

    return AI_create_app_info("EKF_UPDATE", EKF_init, EKF_exe);
}

static void EKF_initialize_param(void)
{
	printf("EKF initialize param.\n");
	write_param(EKF_.ekf_order, ekf_order_, (uint16_t)(sizeof(ekf_order_)), (uint32_t)0x00006000);
	write_param(&EKF_.number_of_serials, &number_of_serials_, (uint16_t)(sizeof(number_of_serials_)), (uint32_t)0x00006002);
	write_param(&EKF_.eclipse_switch_flag, &eclipse_switch_flag_, (uint16_t)(sizeof(eclipse_switch_flag_)), (uint32_t)0x000063D1);

	write_param(&EKF_CLASS::taub, &EKF_CLASS::taub_, (uint16_t)(sizeof(EKF_CLASS::taub_)), (uint32_t)0x000062F7);
	write_param(&EKF_CLASS::rmm_thr, &EKF_CLASS::rmm_thr_, (uint16_t)(sizeof(EKF_CLASS::rmm_thr_)), (uint32_t)0x000062FB);
	write_param(&EKF_CLASS::taurmm, &EKF_CLASS::taurmm_, (uint16_t)(sizeof(EKF_CLASS::taurmm_)), (uint32_t)0x000062FF);
	write_param(&EKF_CLASS::omega_abs_thr, &EKF_CLASS::omega_abs_thr_, (uint16_t)(sizeof(EKF_CLASS::omega_abs_thr_)), (uint32_t)0x0000632B);
	write_param(&EKF_CLASS::direction_err_b, &EKF_CLASS::direction_err_b_, (uint16_t)(sizeof(EKF_CLASS::direction_err_b_)), (uint32_t)0x0000632F);
	write_param(EKF_CLASS::err_cov_thr, EKF_CLASS::err_cov_thr_, (uint16_t)(sizeof(EKF_CLASS::err_cov_thr_)), (uint32_t)0x0000637F);
	write_param(&EKF_CLASS::direction_err_eci, &EKF_CLASS::direction_err_eci_, (uint16_t)(sizeof(EKF_CLASS::direction_err_eci_)), (uint32_t)0x00006337);
	write_param(&EKF_CLASS::sep_direction_err, &EKF_CLASS::sep_direction_err_, (uint16_t)(sizeof(EKF_CLASS::sep_direction_err_)), (uint32_t)0x0000633B);
	write_param(EKF_CLASS::separate_direction, EKF_CLASS::separate_direction_, (uint16_t)(sizeof(EKF_CLASS::separate_direction_)), (uint32_t)0x0000633F);
	write_param(EKF_CLASS::pos_1U_b, EKF_CLASS::pos_1U_b_, (uint16_t)(sizeof(EKF_CLASS::pos_1U_b)), (uint32_t)0x0000635B);
	write_param(&EKF_CLASS::omega_X_min, &EKF_CLASS::omega_X_min_, (uint16_t)(sizeof(EKF_CLASS::omega_X_min)), (uint32_t)0x00006367);
	write_param(&EKF_CLASS::omega_X_max, &EKF_CLASS::omega_X_max_, (uint16_t)(sizeof(EKF_CLASS::omega_X_max)), (uint32_t)0x0000636B);
	write_param(&EKF_CLASS::omega_Y_min, &EKF_CLASS::omega_Y_min_, (uint16_t)(sizeof(EKF_CLASS::omega_Y_min)), (uint32_t)0x0000636F);
	write_param(&EKF_CLASS::omega_Y_max, &EKF_CLASS::omega_Y_max_, (uint16_t)(sizeof(EKF_CLASS::omega_Y_max)), (uint32_t)0x00006373);
	write_param(&EKF_CLASS::omega_Z_min, &EKF_CLASS::omega_Z_min_, (uint16_t)(sizeof(EKF_CLASS::omega_Z_min)), (uint32_t)0x00006377);
	write_param(&EKF_CLASS::omega_Z_max, &EKF_CLASS::omega_Z_max_, (uint16_t)(sizeof(EKF_CLASS::omega_Z_max)), (uint32_t)0x0000637B);
	write_param(&EKF_CLASS::angle_velocity2omega, &EKF_CLASS::angle_velocity2omega_, (uint16_t)(sizeof(EKF_CLASS::angle_velocity2omega)), (uint32_t)0x00006397);
	write_param(&EKF_CLASS::angle_velocity2omega_err, &EKF_CLASS::angle_velocity2omega_err_, (uint16_t)(sizeof(EKF_CLASS::angle_velocity2omega_err)), (uint32_t)0x0000639B);
	write_param(&EKF_CLASS::angle_normal2vXomega, &EKF_CLASS::angle_normal2vXomega_, (uint16_t)(sizeof(EKF_CLASS::angle_normal2vXomega)), (uint32_t)0x0000639F);
	write_param(&EKF_CLASS::angle_normal2vXomega_err, &EKF_CLASS::angle_normal2vXomega_err_, (uint16_t)(sizeof(EKF_CLASS::angle_normal2vXomega_err)), (uint32_t)0x000063A3);
	write_param(&EKF_CLASS::sep_phase, &EKF_CLASS::sep_phase_, (uint16_t)(sizeof(EKF_CLASS::sep_phase)), (uint32_t)0x000063A7);
	write_param(&EKF_CLASS::sep_phase_err, &EKF_CLASS::sep_phase_err_, (uint16_t)(sizeof(EKF_CLASS::sep_phase_err)), (uint32_t)0x000063AB);
	write_param(&EKF_CLASS::width, &EKF_CLASS::width_, (uint16_t)(sizeof(EKF_CLASS::width)), (uint32_t)0x000063AF);
	write_param(&EKF_CLASS::mtq_rms_coeff_x, &EKF_CLASS::mtq_rms_coeff_x_, (uint16_t)(sizeof(EKF_CLASS::mtq_rms_coeff_x_)), (uint32_t)0x000063B0);
	write_param(&EKF_CLASS::mtq_rms_coeff_y, &EKF_CLASS::mtq_rms_coeff_y_, (uint16_t)(sizeof(EKF_CLASS::mtq_rms_coeff_y_)), (uint32_t)0x000063B4);
	write_param(&EKF_CLASS::mtq_rms_coeff_z, &EKF_CLASS::mtq_rms_coeff_z_, (uint16_t)(sizeof(EKF_CLASS::mtq_rms_coeff_z_)), (uint32_t)0x000063B8);
	write_param(&EKF_CLASS::select_ekf, &EKF_CLASS::select_ekf_, (uint16_t)(sizeof(EKF_CLASS::select_ekf)), (uint32_t)0x000063BC);
	write_param(&EKF_CLASS::K_type, &EKF_CLASS::K_type_, (uint16_t)(sizeof(EKF_CLASS::K_type_)), (uint32_t)0x000063D0);

	write_param(EKF_class[0].ini_X_ref, EKF_CLASS::ini_X_ref_0_, (uint16_t)(sizeof(EKF_CLASS::ini_X_ref_0_)), (uint32_t)0x0000600B);
	write_param(EKF_class[0].ini_P_diag, EKF_CLASS::ini_P_diag_0_, (uint16_t)(sizeof(EKF_CLASS::ini_P_diag_0_)), (uint32_t)0x0000604B);
	write_param(EKF_class[0].std_Q, EKF_CLASS::std_Q_0_, (uint16_t)(sizeof(EKF_CLASS::std_Q_0_)), (uint32_t)0x00006087);
	write_param(EKF_class[0].std_R, EKF_CLASS::std_R_0_, (uint16_t)(sizeof(EKF_CLASS::std_R_0_)), (uint32_t)0x000060C3);
	write_param(&EKF_class[0].adp_noise_flag, &EKF_CLASS::adp_noise_flag_0_, (uint16_t)(sizeof(EKF_CLASS::adp_noise_flag_0_)), (uint32_t)0x000060FF);
	write_param(&EKF_class[0].alpha, &EKF_CLASS::alpha_0_, (uint16_t)(sizeof(EKF_CLASS::alpha_0_)), (uint32_t)0x00006100);
	write_param(EKF_class[0].const_K_diag, EKF_CLASS::const_K_diag_0_, (uint16_t)(sizeof(EKF_CLASS::const_K_diag_0_)), (uint32_t)0x00006104);
	write_param(&EKF_class[0].P_constflag, &EKF_CLASS::P_constflag_0_, (uint16_t)(sizeof(EKF_CLASS::P_constflag_0_)), (uint32_t)0x00006140);
	write_param(EKF_class[0].const_P_diag, EKF_CLASS::const_P_diag_0_, (uint16_t)(sizeof(EKF_CLASS::const_P_diag_0_)), (uint32_t)0x00006141);
	write_param(&EKF_class[0].conv_thr, &EKF_CLASS::conv_thr_0_, (uint16_t)(sizeof(EKF_CLASS::conv_thr_0_)), (uint32_t)0x0000617D);

	write_param(EKF_class[1].ini_X_ref, EKF_CLASS::ini_X_ref_1_, (uint16_t)(sizeof(EKF_CLASS::ini_X_ref_1_)), (uint32_t)0x00006181);
	write_param(EKF_class[1].ini_P_diag, EKF_CLASS::ini_P_diag_1_, (uint16_t)(sizeof(EKF_CLASS::ini_P_diag_1_)), (uint32_t)0x000061C1);
	write_param(EKF_class[1].std_Q, EKF_CLASS::std_Q_1_, (uint16_t)(sizeof(EKF_CLASS::std_Q_1_)), (uint32_t)0x000061FD);
	write_param(EKF_class[1].std_R, EKF_CLASS::std_R_1_, (uint16_t)(sizeof(EKF_CLASS::std_R_1_)), (uint32_t)0x00006239);
	write_param(&EKF_class[1].adp_noise_flag, &EKF_CLASS::adp_noise_flag_1_, (uint16_t)(sizeof(EKF_CLASS::adp_noise_flag_1_)), (uint32_t)0x00006275);
	write_param(&EKF_class[1].alpha, &EKF_CLASS::alpha_1_, (uint16_t)(sizeof(EKF_CLASS::alpha_1_)), (uint32_t)0x00006276);
	write_param(EKF_class[1].const_K_diag, EKF_CLASS::const_K_diag_1_, (uint16_t)(sizeof(EKF_CLASS::const_K_diag_1_)), (uint32_t)0x0000627A);
	write_param(&EKF_class[1].P_constflag, &EKF_CLASS::P_constflag_1_, (uint16_t)(sizeof(EKF_CLASS::P_constflag_1_)), (uint32_t)0x000062B6);
	write_param(EKF_class[1].const_P_diag, EKF_CLASS::const_P_diag_1_, (uint16_t)(sizeof(EKF_CLASS::const_P_diag_1_)), (uint32_t)0x000062B7);
	write_param(&EKF_class[1].conv_thr, &EKF_CLASS::conv_thr_1_, (uint16_t)(sizeof(EKF_CLASS::conv_thr_1_)), (uint32_t)0x000062F3);

	write_param(&EKF_class[0].dt, &EKF_CLASS::dt_0_, (uint16_t)(sizeof(EKF_CLASS::dt_0_)), (uint32_t)0x0000634D);
	write_param(&EKF_class[0].dt_hispin, &EKF_CLASS::dt_hispin_0_, (uint16_t)(sizeof(EKF_CLASS::dt_hispin_0_)), (uint32_t)0x00006351);
	write_param(&EKF_class[1].dt, &EKF_CLASS::dt_1_, (uint16_t)(sizeof(EKF_CLASS::dt_1_)), (uint32_t)0x00006355);
	write_param(&EKF_class[1].dt_hispin, &EKF_CLASS::dt_hispin_1_, (uint16_t)(sizeof(EKF_CLASS::dt_hispin_1_)), (uint32_t)0x00006359);

	write_param(&EKF_CLASS::max_prop, &EKF_CLASS::max_prop_, (uint16_t)(sizeof(EKF_CLASS::max_prop_)), (uint32_t)0x000063BD);
	write_param(&EKF_CLASS::observe_time, &EKF_CLASS::observe_time_, (uint16_t)(sizeof(EKF_CLASS::observe_time_)), (uint32_t)0x000063BF);
	write_param(&EKF_CLASS::max_prop_hispin, &EKF_CLASS::max_prop_hispin_, (uint16_t)(sizeof(EKF_CLASS::max_prop_hispin_)), (uint32_t)0x000063C1);
	write_param(&EKF_CLASS::observe_time_hispin, &EKF_CLASS::observe_time_hispin_, (uint16_t)(sizeof(EKF_CLASS::observe_time_hispin_)), (uint32_t)0x000063C3);
	write_param(&EKF_CLASS::sep_time_under_limit, &EKF_CLASS::sep_time_under_limit_, (uint16_t)(sizeof(EKF_CLASS::sep_time_under_limit_)), (uint32_t)0x000063C5);
	write_param(&EKF_CLASS::reltime_type, &EKF_CLASS::reltime_type_, (uint16_t)(sizeof(EKF_CLASS::reltime_type)), (uint32_t)0x000063C7);
	write_param(EKF_CLASS::sensor_reltime_to_obs, EKF_CLASS::sensor_reltime_to_obs_, (uint16_t)(sizeof(EKF_CLASS::sensor_reltime_to_obs_)), (uint32_t)0x000063C8);

}



static void EKF_load_param(void)
{
	printf("EKF load param\r\n");
	g_ads.p_EKF = &EKF_;
	read_param(EKF_.ekf_order, ekf_order_, (uint16_t)(sizeof(ekf_order_)), (uint32_t)0x00006000);
	read_param(&EKF_.number_of_serials, &number_of_serials_, (uint16_t)(sizeof(number_of_serials_)), (uint32_t)0x00006002);
	read_param(&EKF_.eclipse_switch_flag, &eclipse_switch_flag_, (uint16_t)(sizeof(eclipse_switch_flag_)), (uint32_t)0x000063D1);

	read_param(&EKF_CLASS::taub, &EKF_CLASS::taub_, (uint16_t)(sizeof(EKF_CLASS::taub_)), (uint32_t)0x000062F7);
	read_param(&EKF_CLASS::rmm_thr, &EKF_CLASS::rmm_thr_, (uint16_t)(sizeof(EKF_CLASS::rmm_thr_)), (uint32_t)0x000062FB);
	read_param(&EKF_CLASS::taurmm, &EKF_CLASS::taurmm_, (uint16_t)(sizeof(EKF_CLASS::taurmm_)), (uint32_t)0x000062FF);
	read_param(&EKF_CLASS::omega_abs_thr, &EKF_CLASS::omega_abs_thr_, (uint16_t)(sizeof(EKF_CLASS::omega_abs_thr_)), (uint32_t)0x0000632B);
	read_param(&EKF_CLASS::direction_err_b, &EKF_CLASS::direction_err_b_, (uint16_t)(sizeof(EKF_CLASS::direction_err_b_)), (uint32_t)0x0000632F);
	read_param(EKF_CLASS::err_cov_thr, EKF_CLASS::err_cov_thr_, (uint16_t)(sizeof(EKF_CLASS::err_cov_thr_)), (uint32_t)0x0000637F);
	read_param(&EKF_CLASS::direction_err_eci, &EKF_CLASS::direction_err_eci_, (uint16_t)(sizeof(EKF_CLASS::direction_err_eci_)), (uint32_t)0x00006337);
	read_param(&EKF_CLASS::sep_direction_err, &EKF_CLASS::sep_direction_err_, (uint16_t)(sizeof(EKF_CLASS::sep_direction_err_)), (uint32_t)0x0000633B);
	read_param(EKF_CLASS::separate_direction, EKF_CLASS::separate_direction_, (uint16_t)(sizeof(EKF_CLASS::separate_direction_)), (uint32_t)0x0000633F);
	read_param(EKF_CLASS::pos_1U_b, EKF_CLASS::pos_1U_b_, (uint16_t)(sizeof(EKF_CLASS::pos_1U_b)), (uint32_t)0x0000635B);
	read_param(&EKF_CLASS::omega_X_min, &EKF_CLASS::omega_X_min_, (uint16_t)(sizeof(EKF_CLASS::omega_X_min)), (uint32_t)0x00006367);
	read_param(&EKF_CLASS::omega_X_max, &EKF_CLASS::omega_X_max_, (uint16_t)(sizeof(EKF_CLASS::omega_X_max)), (uint32_t)0x0000636B);
	read_param(&EKF_CLASS::omega_Y_min, &EKF_CLASS::omega_Y_min_, (uint16_t)(sizeof(EKF_CLASS::omega_Y_min)), (uint32_t)0x0000636F);
	read_param(&EKF_CLASS::omega_Y_max, &EKF_CLASS::omega_Y_max_, (uint16_t)(sizeof(EKF_CLASS::omega_Y_max)), (uint32_t)0x00006373);
	read_param(&EKF_CLASS::omega_Z_min, &EKF_CLASS::omega_Z_min_, (uint16_t)(sizeof(EKF_CLASS::omega_Z_min)), (uint32_t)0x00006377);
	read_param(&EKF_CLASS::omega_Z_max, &EKF_CLASS::omega_Z_max_, (uint16_t)(sizeof(EKF_CLASS::omega_Z_max)), (uint32_t)0x0000637B);
	read_param(&EKF_CLASS::angle_velocity2omega, &EKF_CLASS::angle_velocity2omega_, (uint16_t)(sizeof(EKF_CLASS::angle_velocity2omega)), (uint32_t)0x00006397);
	read_param(&EKF_CLASS::angle_velocity2omega_err, &EKF_CLASS::angle_velocity2omega_err_, (uint16_t)(sizeof(EKF_CLASS::angle_velocity2omega_err)), (uint32_t)0x0000639B);
	read_param(&EKF_CLASS::angle_normal2vXomega, &EKF_CLASS::angle_normal2vXomega_, (uint16_t)(sizeof(EKF_CLASS::angle_normal2vXomega)), (uint32_t)0x0000639F);
	read_param(&EKF_CLASS::angle_normal2vXomega_err, &EKF_CLASS::angle_normal2vXomega_err_, (uint16_t)(sizeof(EKF_CLASS::angle_normal2vXomega_err)), (uint32_t)0x000063A3);
	read_param(&EKF_CLASS::sep_phase, &EKF_CLASS::sep_phase_, (uint16_t)(sizeof(EKF_CLASS::sep_phase)), (uint32_t)0x000063A7);
	read_param(&EKF_CLASS::sep_phase_err, &EKF_CLASS::sep_phase_err_, (uint16_t)(sizeof(EKF_CLASS::sep_phase_err)), (uint32_t)0x000063AB);
	read_param(&EKF_CLASS::width, &EKF_CLASS::width_, (uint16_t)(sizeof(EKF_CLASS::width)), (uint32_t)0x000063AF);
	read_param(&EKF_CLASS::mtq_rms_coeff_x, &EKF_CLASS::mtq_rms_coeff_x_, (uint16_t)(sizeof(EKF_CLASS::mtq_rms_coeff_x_)), (uint32_t)0x000063B0);
	read_param(&EKF_CLASS::mtq_rms_coeff_y, &EKF_CLASS::mtq_rms_coeff_y_, (uint16_t)(sizeof(EKF_CLASS::mtq_rms_coeff_y_)), (uint32_t)0x000063B4);
	read_param(&EKF_CLASS::mtq_rms_coeff_z, &EKF_CLASS::mtq_rms_coeff_z_, (uint16_t)(sizeof(EKF_CLASS::mtq_rms_coeff_z_)), (uint32_t)0x000063B8);
	read_param(&EKF_CLASS::select_ekf, &EKF_CLASS::select_ekf_, (uint16_t)(sizeof(EKF_CLASS::select_ekf)), (uint32_t)0x000063BC);
	read_param(&EKF_CLASS::K_type, &EKF_CLASS::K_type_, (uint16_t)(sizeof(EKF_CLASS::K_type_)), (uint32_t)0x000063D0);

	read_param(EKF_class[0].ini_X_ref, EKF_CLASS::ini_X_ref_0_, (uint16_t)(sizeof(EKF_CLASS::ini_X_ref_0_)), (uint32_t)0x0000600B);
	read_param(EKF_class[0].ini_P_diag, EKF_CLASS::ini_P_diag_0_, (uint16_t)(sizeof(EKF_CLASS::ini_P_diag_0_)), (uint32_t)0x0000604B);
	read_param(EKF_class[0].std_Q, EKF_CLASS::std_Q_0_, (uint16_t)(sizeof(EKF_CLASS::std_Q_0_)), (uint32_t)0x00006087);
	read_param(EKF_class[0].std_R, EKF_CLASS::std_R_0_, (uint16_t)(sizeof(EKF_CLASS::std_R_0_)), (uint32_t)0x000060C3);
	read_param(&EKF_class[0].adp_noise_flag, &EKF_CLASS::adp_noise_flag_0_, (uint16_t)(sizeof(EKF_CLASS::adp_noise_flag_0_)), (uint32_t)0x000060FF);
	read_param(&EKF_class[0].alpha, &EKF_CLASS::alpha_0_, (uint16_t)(sizeof(EKF_CLASS::alpha_0_)), (uint32_t)0x00006100);
	read_param(EKF_class[0].const_K_diag, EKF_CLASS::const_K_diag_0_, (uint16_t)(sizeof(EKF_CLASS::const_K_diag_0_)), (uint32_t)0x00006104);
	read_param(&EKF_class[0].P_constflag, &EKF_CLASS::P_constflag_0_, (uint16_t)(sizeof(EKF_CLASS::P_constflag_0_)), (uint32_t)0x00006140);
	read_param(EKF_class[0].const_P_diag, EKF_CLASS::const_P_diag_0_, (uint16_t)(sizeof(EKF_CLASS::const_P_diag_0_)), (uint32_t)0x00006141);
	read_param(&EKF_class[0].conv_thr, &EKF_CLASS::conv_thr_0_, (uint16_t)(sizeof(EKF_CLASS::conv_thr_0_)), (uint32_t)0x0000617D);

	read_param(EKF_class[1].ini_X_ref, EKF_CLASS::ini_X_ref_1_, (uint16_t)(sizeof(EKF_CLASS::ini_X_ref_1_)), (uint32_t)0x00006181);
	read_param(EKF_class[1].ini_P_diag, EKF_CLASS::ini_P_diag_1_, (uint16_t)(sizeof(EKF_CLASS::ini_P_diag_1_)), (uint32_t)0x000061C1);
	read_param(EKF_class[1].std_Q, EKF_CLASS::std_Q_1_, (uint16_t)(sizeof(EKF_CLASS::std_Q_1_)), (uint32_t)0x000061FD);
	read_param(EKF_class[1].std_R, EKF_CLASS::std_R_1_, (uint16_t)(sizeof(EKF_CLASS::std_R_1_)), (uint32_t)0x00006239);
	read_param(&EKF_class[1].adp_noise_flag, &EKF_CLASS::adp_noise_flag_1_, (uint16_t)(sizeof(EKF_CLASS::adp_noise_flag_1_)), (uint32_t)0x00006275);
	read_param(&EKF_class[1].alpha, &EKF_CLASS::alpha_1_, (uint16_t)(sizeof(EKF_CLASS::alpha_1_)), (uint32_t)0x00006276);
	read_param(EKF_class[1].const_K_diag, EKF_CLASS::const_K_diag_1_, (uint16_t)(sizeof(EKF_CLASS::const_K_diag_1_)), (uint32_t)0x0000627A);
	read_param(&EKF_class[1].P_constflag, &EKF_CLASS::P_constflag_1_, (uint16_t)(sizeof(EKF_CLASS::P_constflag_1_)), (uint32_t)0x000062B6);
	read_param(EKF_class[1].const_P_diag, EKF_CLASS::const_P_diag_1_, (uint16_t)(sizeof(EKF_CLASS::const_P_diag_1_)), (uint32_t)0x000062B7);
	read_param(&EKF_class[1].conv_thr, &EKF_CLASS::conv_thr_1_, (uint16_t)(sizeof(EKF_CLASS::conv_thr_1_)), (uint32_t)0x000062F3);

	read_param(&EKF_class[0].dt, &EKF_CLASS::dt_0_, (uint16_t)(sizeof(EKF_CLASS::dt_0_)), (uint32_t)0x0000634D);
	read_param(&EKF_class[0].dt_hispin, &EKF_CLASS::dt_hispin_0_, (uint16_t)(sizeof(EKF_CLASS::dt_hispin_0_)), (uint32_t)0x00006351);
	read_param(&EKF_class[1].dt, &EKF_CLASS::dt_1_, (uint16_t)(sizeof(EKF_CLASS::dt_1_)), (uint32_t)0x00006355);
	read_param(&EKF_class[1].dt_hispin, &EKF_CLASS::dt_hispin_1_, (uint16_t)(sizeof(EKF_CLASS::dt_hispin_1_)), (uint32_t)0x00006359);

	read_param(&EKF_CLASS::max_prop, &EKF_CLASS::max_prop_, (uint16_t)(sizeof(EKF_CLASS::max_prop_)), (uint32_t)0x000063BD);
	read_param(&EKF_CLASS::observe_time, &EKF_CLASS::observe_time_, (uint16_t)(sizeof(EKF_CLASS::observe_time_)), (uint32_t)0x000063BF);
	read_param(&EKF_CLASS::max_prop_hispin, &EKF_CLASS::max_prop_hispin_, (uint16_t)(sizeof(EKF_CLASS::max_prop_hispin_)), (uint32_t)0x000063C1);
	read_param(&EKF_CLASS::observe_time_hispin, &EKF_CLASS::observe_time_hispin_, (uint16_t)(sizeof(EKF_CLASS::observe_time_hispin_)), (uint32_t)0x000063C3);
	read_param(&EKF_CLASS::sep_time_under_limit, &EKF_CLASS::sep_time_under_limit_, (uint16_t)(sizeof(EKF_CLASS::sep_time_under_limit_)), (uint32_t)0x000063C5);
	read_param(&EKF_CLASS::reltime_type, &EKF_CLASS::reltime_type_, (uint16_t)(sizeof(EKF_CLASS::reltime_type)), (uint32_t)0x000063C7);
	read_param(EKF_CLASS::sensor_reltime_to_obs, EKF_CLASS::sensor_reltime_to_obs_, (uint16_t)(sizeof(EKF_CLASS::sensor_reltime_to_obs_)), (uint32_t)0x000063C8);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void EKF_init(void)
{
    printf("EKF driver initialize.\n");

    uint8_t refdim, obsdim, Pdim, Qdim, Rdim;
    set_dimension(EKF_GYRO, refdim, obsdim, Pdim, Qdim, Rdim);
    EKF_class[0].reset(refdim, Pdim, Qdim, Rdim);

    EKF_class[0].ekf_num = 0x00;
    EKF_class[1].ekf_num = 0xff;

    printf("EKF_CLASS size : %x\r\n", sizeof(EKF_class[0]));
}

/**
 * @brief アプリケーション実行関数
 *
 */
static void EKF_exe(void)
{

	// 直列に実行するEKFのIDを配列順番にいれておく
	// 1種類のEKFのみフラグがある場合は自動的にそれを先頭に入れる
	if (ADS_Mode(EKF_GYRO) && ADS_Mode(EKF_GYROBIAS) + ADS_Mode(EKF_MAGSUNGYRO) + ADS_Mode(EKF_EDDY) + ADS_Mode(EKF_RESIDUAL_MAG) + ADS_Mode(EKF_ANISOTROPY_MAG) == 0)
		{ EKF_.ekf_order[0] = EKF_GYRO; }
	else if (ADS_Mode(EKF_GYROBIAS) && ADS_Mode(EKF_GYRO) + ADS_Mode(EKF_MAGSUNGYRO) + ADS_Mode(EKF_EDDY) + ADS_Mode(EKF_RESIDUAL_MAG) + ADS_Mode(EKF_ANISOTROPY_MAG) == 0)
		{ EKF_.ekf_order[0] = EKF_GYROBIAS; }
	else if (ADS_Mode(EKF_MAGSUNGYRO) && ADS_Mode(EKF_GYRO) + ADS_Mode(EKF_GYROBIAS) + ADS_Mode(EKF_EDDY) + ADS_Mode(EKF_RESIDUAL_MAG) + ADS_Mode(EKF_ANISOTROPY_MAG) == 0)
		{ EKF_.ekf_order[0] = EKF_MAGSUNGYRO; }
	else if (ADS_Mode(EKF_EDDY) && ADS_Mode(EKF_GYRO) + ADS_Mode(EKF_GYROBIAS) + ADS_Mode(EKF_MAGSUNGYRO) + ADS_Mode(EKF_RESIDUAL_MAG) + ADS_Mode(EKF_ANISOTROPY_MAG) == 0)
		{ EKF_.ekf_order[0] = EKF_EDDY; }
	else if (ADS_Mode(EKF_RESIDUAL_MAG) && ADS_Mode(EKF_GYRO) + ADS_Mode(EKF_GYROBIAS) + ADS_Mode(EKF_MAGSUNGYRO) + ADS_Mode(EKF_EDDY) + ADS_Mode(EKF_ANISOTROPY_MAG) == 0)
		{ EKF_.ekf_order[0] = EKF_RESIDUAL_MAG; }
	else if (ADS_Mode(EKF_ANISOTROPY_MAG) && ADS_Mode(EKF_GYRO) + ADS_Mode(EKF_GYROBIAS) + ADS_Mode(EKF_MAGSUNGYRO) + ADS_Mode(EKF_EDDY) + ADS_Mode(EKF_RESIDUAL_MAG) == 0)
		{ EKF_.ekf_order[0] = EKF_ANISOTROPY_MAG; }


	//　AOCSフラグモードのEKF内容変更をチェック
	if(check_EKForder_change()){
		printf("EKF_class reset for ekf_order change.\n");
		ADS_MODE ekf_id;
		uint8_t refdim, obsdim, Pdim, Qdim, Rdim;

		for(int i=0; i<2; i++){
			ekf_id = (ADS_MODE)EKF_.ekf_order[i];
			EKF_.previous_ekf_order[i] = EKF_.ekf_order[i];

			set_dimension(ekf_id, refdim, obsdim, Pdim, Qdim, Rdim);
			EKF_class[i].reset(refdim, Pdim, Qdim, Rdim);
		}

	}

	for (int i=0; i<2; i++){

		ADS_MODE ekf_id = (ADS_MODE)EKF_.ekf_order[i];

		if(ekf_id != EKF_GYRO && ekf_id != EKF_GYROBIAS && ekf_id != EKF_MAGSUNGYRO &&
		   ekf_id != EKF_EDDY && ekf_id != EKF_RESIDUAL_MAG && ekf_id != EKF_ANISOTROPY_MAG) break;

		else{

			//　次元設定
			uint8_t refdim, obsdim, Pdim, Qdim, Rdim;
			set_dimension(ekf_id, refdim, obsdim, Pdim, Qdim, Rdim);

			// 観測値を構造体に設定
			set_obs_value(i, ekf_id);

			if (user_mode->current_mode_id == MD_MODEID_HIGH_RATE_SPIN){ // high-rate spin

//				int before, after;
//				before = TMGR_get_master_step();
//				printf("start time %d\r\n", before);
				p_sep_driver->sep_critcal.sep_time = EKF_class[i].cal_hispin(ekf_id, EKF_.Obs, refdim, obsdim, Pdim, Qdim, Rdim);
//				after = TMGR_get_master_step();
//				printf("cal_separation time %d\r\n", after - before);
//				printf("sep_time %d\r\n",p_sep_driver->sep_critcal.sep_time);

				// 直列EKF時のEKF間の値の受け渡し
				pass_EKFclass_for_serial(i, ekf_id);

				if(EKF_class[i].ekf_valid && i == EKF_.number_of_serials){
					copy_EKFclass_to_struct(i, ekf_id, Pdim);
					set_global_to_EKFvalue(i, ekf_id);

					if(vote_sep_gonogo(&p_sep_driver->sep_param.separation_go_from_ground_station_flag) == GO){
						separation_judgement(p_sep_driver->sep_critcal.sep_time);
					}
				}


			}

			else { // normal mode
//　　			print(EKF,"EKF_.Obs %f %f %f %f %f %f %f \r\n", EKF_.Obs[0], EKF_.Obs[1], EKF_.Obs[2], EKF_.Obs[3], EKF_.Obs[4], EKF_.Obs[5], EKF_.Obs[6]);
				int before, after;
				before = TMGR_get_master_step();
//				printf("start time %d\r\n", before);
				EKF_class[i].cal(ekf_id, EKF_.Obs, refdim, obsdim, Pdim, Qdim, Rdim);
				after = TMGR_get_master_step();
//				printf("cal time %d\r\n", after - before);

				// 直列EKF時のEKF間の値の受け渡し
				pass_EKFclass_for_serial(i, ekf_id);

				if(EKF_class[i].ekf_valid && i == EKF_.number_of_serials){

					copy_EKFclass_to_struct(i, ekf_id, Pdim);
					set_global_to_EKFvalue(i, ekf_id);

				}
			}
		}
	}
}

/**
 * @brief EKFのオーダーの変更をチェック
 */
uint8_t check_EKForder_change(void){

	if(EKF_.previous_ekf_order[0] != EKF_.ekf_order[0]) return 1;
	if(EKF_.previous_ekf_order[1] != EKF_.ekf_order[1]) return 1;

	return 0;
}

/**
 * @brief EKFの観測値として使用する値の受け渡し
 */
void set_obs_value(int i, ADS_MODE ekf_id){

	//---Quaternion---//
	// フラグが使用される複数立っている場合、このif文の上位にある決定測が使用される
	if(ADS_Mode(TRIAD_CALC)){
		for(int i=0; i<4; i++){
			EKF_.Obs[i] = g_ads.p_TRIAD->quaternion[i];
		}
	}
	else if(ADS_Mode(QUEST_CALC)){
		for(int i=0; i<4; i++){
			EKF_.Obs[i] = g_ads.p_QUEST->quaternion[i];
		}
	}
	else if(ADS_Mode(QMETHOD_CALC)){
		for(int i=0; i<4; i++){
			EKF_.Obs[i] = g_ads.p_QMETHOD->quaternion[i];
		}
	}
	else if(ADS_Mode(BBDOT_CALC)){
		for(int i=0; i<4; i++){
			EKF_.Obs[i] = g_ads.p_BBDOT->quaternion[i];
		}
	}

#ifndef HILS
	//　蝕の時はBBdotで上書き
	if(EKF_.eclipse_switch_flag && ADS_Mode(BBDOT_CALC) && vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag)){
//		printf("eclipse\r\n");
		for(int i=0; i<4; i++){
			EKF_.Obs[i] = g_ads.p_BBDOT->quaternion[i];
		}
	}
#endif
	//----------------//


	//---- 個々のEKFで観測値としてしようする値 ----//
	if(ekf_id == EKF_GYRO) {
		for(int i=0; i<3; i++){
			EKF_.Obs[i+4] =  g_ads.p_ADS->Omega[i];
			EKF_.Obs[i+7] =  g_ads.p_EKF->gyro_bias[i];
		}
	}

	if(ekf_id == EKF_GYROBIAS) {
		for(int i=0; i<3; i++){
			EKF_.Obs[i+4] =  g_ads.p_ADS->Omega[i];
		}
	}


	if(ekf_id == EKF_MAGSUNGYRO) {
		for(int i=0; i<3; i++){
			EKF_.Obs[i+4] =  g_ads.p_ADS->Omega[i];
			EKF_.Obs[i+7] =  g_ads.p_ADS->MagVec[i];
			EKF_.Obs[i+10] =  g_ads.p_ADS->SunVec[i];
			EKF_.Obs[i+13] =  g_ads.p_EKF->gyro_bias[i];
		}
	}

	if(ekf_id == EKF_EDDY) {
		for(int i=0; i<3; i++){

		}
	}

	if(ekf_id == EKF_RESIDUAL_MAG) {
		for(int i=0; i<3; i++){
			EKF_.Obs[i] =  g_ads.p_ADS->Omega[i];
			EKF_.Obs[i+3] =  g_ads.p_ADS->MagField[i];
		}
	}

	if(ekf_id == EKF_ANISOTROPY_MAG) {
		for(int i=0; i<3; i++){

		}
	}
	//----------------//
}

/**
 * @brief EKFの次元設定
 * 使用するEKFの種類(ekf_id)ごとに必要最低限の次元数で計算
 */
void set_dimension(ADS_MODE ekf_id, uint8_t& ref_dim, uint8_t& obs_dim, uint8_t& P_dim, uint8_t& Q_dim, uint8_t& R_dim){
	switch(ekf_id){
	// ref_dim: レファレンス状態量, obs_dim:　使用するセンサ値（観測量＋状態方程式使用センサ値）, P_dim:　誤差共分散, 微小状態量, Q_dim: システムノイズ, R_dim: 観測ノイズ, 観測量
	 case EKF_GYRO:
		 //　直列EKFではGYROBIASを前に配置する前提
		 if(ADS_Mode(EKF_GYROBIAS)){
			 ref_dim = 7; obs_dim = 10, P_dim = 6; Q_dim = 6; R_dim = 6;
		 }
		 else{
			 ref_dim = 7; obs_dim = 7, P_dim = 6; Q_dim = 6; R_dim = 6;
		 }
		 break;
	 case EKF_GYROBIAS:
		 ref_dim = 7; obs_dim = 7, P_dim = 6; Q_dim = 6; R_dim = 6;
		 break;
	 case EKF_MAGSUNGYRO:
		 //　直列EKFではGYROBIASを前に配置する前提
		 if(ADS_Mode(EKF_GYROBIAS)){
			 ref_dim = 7; obs_dim = 16, P_dim = 6; Q_dim = 6; R_dim = 9;
		 }
		 else{
			 ref_dim = 7; obs_dim = 13, P_dim = 6; Q_dim = 6; R_dim = 9;
		 }
		 break;
	 case EKF_EDDY:
		 ref_dim = 13; obs_dim = 7, P_dim = 12; Q_dim = 12; R_dim = 12;
		 break;
	 case EKF_RESIDUAL_MAG:
		 ref_dim = 6; obs_dim = 6, P_dim = 6; Q_dim = 6; R_dim = 3;
		 break;
	 case EKF_ANISOTROPY_MAG:
		 ref_dim = 13; obs_dim = 7, P_dim = 12; Q_dim = 12; R_dim = 12;
		 break;
	 default:
		 ref_dim = 7; obs_dim = 7, P_dim = 6; Q_dim = 6; R_dim = 6;
		 break;
	}
}

/**
 * @brief 直列EKF時のEKF間の値の受け渡し
 */
void pass_EKFclass_for_serial(uint8_t i, ADS_MODE ekf_id){
	switch(ekf_id){
		 case EKF_GYRO:
			 if(!vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag)){
				 ExtractVec(EKF_class[i].X_ref_post,0,3,EKF_.quaternion);
				 ExtractVec(EKF_class[i].X_ref_post,4,6,EKF_.omega);
			 }
			 break;
		 case EKF_GYROBIAS:
			 if(!vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag)){
				 ExtractVec(EKF_class[i].X_ref_post,0,3,EKF_.quaternion);
				 ExtractVec(EKF_class[i].X_ref_post,4,6,EKF_.gyro_bias);
			 }
			 break;
		 case EKF_MAGSUNGYRO:
			 ExtractVec(EKF_class[i].X_ref_post,0,3,EKF_.quaternion);
			 ExtractVec(EKF_class[i].X_ref_post,4,6,EKF_.omega);
			 break;
		 case EKF_EDDY:

			 break;
		 case EKF_RESIDUAL_MAG:
			 ExtractVec(EKF_class[i].X_ref_post,0,1,EKF_.omega);
			 ExtractVec(EKF_class[i].X_ref_post,3,5,EKF_.rmm);
			 break;
		 case EKF_ANISOTROPY_MAG:

			 break;
		 default:
			 break;
	}
}

/**
 * @brief クラス変数の値を構造体に格納
 */
void copy_EKFclass_to_struct(uint8_t i, ADS_MODE ekf_id, uint8_t Pdim){

	EKF_.divg_valid = EKF_class[i].divg_valid;
	EKF_.dq_valid = EKF_class[i].dq_valid;
	EKF_.ekf_valid = EKF_class[i].ekf_valid;
	EKF_.sep_time = EKF_class[i].sep_time;
	for(int j=0; j<Pdim; j++){
		EKF_.P_pre_diag[j] = EKF_class[i].P_PRE_previous[j*Pdim+j];
		EKF_.P_post_diag[j] = EKF_class[i].P_POST[j*Pdim+j];
	}
	for(int j=Pdim; j<15; j++){
		EKF_.P_pre_diag[j] = 0.0f;
		EKF_.P_post_diag[j] = 0.0f;
	}
	memcpy(EKF_.X_ref_pre, EKF_class[i].X_ref_pre_previous, sizeof(EKF_class[i].X_ref_pre_previous));
	memcpy(EKF_.X_ref_post, EKF_class[i].X_ref_post, sizeof(EKF_class[i].X_ref_post));
	memcpy(EKF_.y, EKF_class[i].y, sizeof(EKF_class[i].y));

}

/**
 * @brief EKFの出力をAOCS系グローバル変数に格納
 */
void set_global_to_EKFvalue(const uint8_t i, const ADS_MODE ekf_id){
	switch(ekf_id){
		// ref_dim: レファレンス状態量, obs_dim:　使用するセンサ値（観測量＋状態方程式使用センサ値）, P_dim:　誤差共分散, 微小状態量, Q_dim: システムノイズ, R_dim: 観測ノイズ, 観測量
		float qua[4], ome[3], gbias[3], rmm[3];
		case EKF_GYRO:
			if(!vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag)){
				ExtractVec(EKF_.X_ref_post,0,3,qua);
				ExtractVec(EKF_.X_ref_post,4,6,ome);
				GLOBAL_ADS_set_Quaternion(qua);
				GLOBAL_ADS_set_GYRO(ome);
			}
			break;
		case EKF_GYROBIAS:
			if(!vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag)){
				ExtractVec(EKF_.X_ref_post,0,3,qua);
				GLOBAL_ADS_set_Quaternion(qua);
			}
			break;
		case EKF_MAGSUNGYRO:
			ExtractVec(EKF_.X_ref_post,0,3,qua);
			ExtractVec(EKF_.X_ref_post,4,6,ome);
			GLOBAL_ADS_set_Quaternion(qua);
			GLOBAL_ADS_set_GYRO(ome);
			break;
		case EKF_EDDY:

			break;
		case EKF_RESIDUAL_MAG:
			ExtractVec(EKF_.X_ref_post,0,2,ome);
			GLOBAL_ADS_set_GYRO(ome);
			break;
		case EKF_ANISOTROPY_MAG:

			break;
		default:
			break;

	}
}
