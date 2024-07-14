/*
 * AOCS_tlm_param.h
 *
 *  Created on: 2022/08/03
 *      Author: daiki
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_AOCS_AOCS_TLM_PARAM_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_AOCS_AOCS_TLM_PARAM_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

typedef struct{
	uint32_t tlm_flag[4];

	//tlm_flag[0] MSB 31  ~ 0  LSB
	//tlm_flag[1] MSB 63  ~ 32 LSB
	//tlm_flag[2] MSB 95  ~ 64 LSB
	//tlm_flag[3] MSB 127 ~ 96 LSB
}AOCS_TLM;

typedef enum{
	Sensor_time,
	nineax_time,
	nineax_gyro_raw_data,
	nineax_mag_raw_data,
	nineax_acc_raw_data,
	nineax_temp,
	mems_gyro_time,
	mems_gyro_range,
	mems_gyro_raw_data,
	mems_gyro_temp,
	mems_mag_time,
	mems_mag_raw_data,
	mems_mag_temp,
	suns_time,
	suns_error_num,
	suns_unfiltered,
	suns_filtered,
	suns_angle,
	csuns_value,
	csuns_peak,
	gps_utc,
	gps_pos,
	gps_vel,
	gps_temp,
	plasma,
	panel_temp,
	vol_sepcoil,
	adc_curs_mtq,
	reserve1,
	reserve2,
	reserve3,
	reserve4,

	ads_mode,
	ads_Mag,
	ads_Gyro,
	ads_Sunvec,
	ads_Julian,
	ads_GST,
	ads_IGRF,
	ads_Isunvec,
	ads_Bsunvec,
	ads_Quaternion,
	ads_EKF_X_observed_1,
	ads_EKF_X_observed_2,
	ads_EKF_X_ref_a_priori_1,
	ads_EKF_X_ref_a_priori_2,
	ads_EKF_X_ref_a_poseriori_1,
	ads_EKF_X_ref_a_poseriori_2,
	ads_EKF_P_a_priori_1,
	ads_EKF_P_a_priori_2,
	ads_EKF_P_a_posteriori_1,
	ads_EKF_P_a_posteriori_2,
	ads_EKF_valid,
	ads_EKF_sep_time,
	Separate_judge,
	reserve5,

	acs_mode,
	acs_time,
	acs_Mag,
	acs_Gyro,
	acs_SunVec,
	acs_Target_Direction,
	acs_Earth_center_vec,
	acs_Earth_point_vec,
	acs_Inertial_vec,
	acs_Velocity_vec,
	acs_COBC_vec,
	acs_spin_dorection,
	acs_Quaternion,
	acs_MTQ_output,
	acs_MTQ_Cn_req,
	acs_MTQ_Co_req,

	ods_mode,
	ods_Pos,
	ods_Vel,
	ods_relative_pos,
	ods_relative_vel,
	ods_BC,
	ods_GPS_Status,
	ods_DCM_eci2lvlh,
	ods_EKF_X_observed,
	ods_EKF_X_ref_a_priori,
	ods_EKF_X_ref_a_poseriori,
	ods_EKF_P_a_priori,
	ods_EKF_P_a_posteriori,
	ods_EKF_valid,
	ods_EKF_sep_time,
	ods_RSSI,

	ocs_mode,
	ocs_time,
	ocs_Pos,
	ocs_Vel,
	ocs_relative_pos,
	ocs_relative_vel,
	ocs_BC,
	ocs_BC_req,
	ocs_Mag,
	ocs_Gyro,
	ocs_Quaternion,
	ocs_MTQ_output,
	ocs_MTQ_Cn_req,
	ocs_MTQ_Co_req,

}AOCS_TLM_FLAG;


AppInfo AOCS_tlm_param_update(void);

void AOCS_tlm_pack(int i, int tlm_len, unsigned char* contents);

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_AOCS_AOCS_TLM_PARAM_H_ */
