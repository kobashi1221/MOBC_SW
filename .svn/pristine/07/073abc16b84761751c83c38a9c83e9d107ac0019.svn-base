/*
 * Global_AOCS_Param_Set.cpp
 *
 *  Created on: 2022/06/19
 *      Author: 稲守研
 */

#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Param_Set.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>
#include <src_user/Library/General/DBG_printf.h>

static void AOCS_Sensor_time_Set_update_(void);
static void ACS_Param_Set_update_(void);
static void ADS_Param_Set_update_(void);
static void OCS_Param_Set_update_(void);
static void ODS_Param_Set_update_(void);
static void ACS_Param_Reset_(void);
static void OCS_Param_Reset_(void);

AppInfo AOCS_Sensor_time_Set_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, AOCS_Sensor_time_Set_update_);
}

AppInfo ACS_Param_Set_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_Param_Set_update_);
}

AppInfo ADS_Param_Set_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ADS_Param_Set_update_);
}

AppInfo OCS_Param_Set_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, OCS_Param_Set_update_);
}

AppInfo ODS_Param_Set_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ODS_Param_Set_update_);
}

AppInfo ACS_Param_Reset(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_Param_Reset_);
}

AppInfo OCS_Param_Reset(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, OCS_Param_Reset_);
}

static void AOCS_Sensor_time_Set_update_(void){
	step_t Sensor_time;
	Sensor_time = TMGR_get_master_step();
	GLOBAL_AOCS_set_SensorTime(Sensor_time);
}

static void ACS_Param_Set_update_(void){
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};
	float B_fld[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float Magvec[3] = {g_ads.p_ADS->MagVec[0], g_ads.p_ADS->MagVec[1], g_ads.p_ADS->MagVec[2]};
	float sunvec[3] = {g_ads.p_ADS->SunVec[0], g_ads.p_ADS->SunVec[1], g_ads.p_ADS->SunVec[2]};
	float mag_moment[3];// = {g_ads.p_ADS->Mag_moment[0], g_ads.p_ADS->Mag_moment[1], g_ads.p_ADS->Mag_moment[2]};
	float mag_moment_cn[3]= {g_acs.p_ACS->Mag_moment_Cn[0], g_acs.p_ACS->Mag_moment_Cn[1], g_acs.p_ACS->Mag_moment_Cn[2]};
	float mag_moment_co[3] = {g_acs.p_ACS->Mag_moment_Co[0], g_acs.p_ACS->Mag_moment_Co[1], g_acs.p_ACS->Mag_moment_Co[2]};
	float before_mag_moment[3];
	float quaternion[4] = {g_ads.p_ADS->Quaternion[0], g_ads.p_ADS->Quaternion[1], g_ads.p_ADS->Quaternion[2], g_ads.p_ADS->Quaternion[3]};
	float DCM[3][3];
	float Velocity[3] = {g_ads.p_ADS->Velocity[0], g_ads.p_ADS->Velocity[1], g_ads.p_ADS->Velocity[2]};
	float Position[3] = {g_ads.p_ADS->Position[0], g_ads.p_ADS->Position[1], g_ads.p_ADS->Position[2]};
	float BallCoef = g_ods.p_ODS->Ballistic_Coefficient;
	step_t ACS_time;
	Normalization(B_fld, 3, Magvec);
	copy(&g_ads.p_ADS->DCM_i2b[0][0], 3, 3, *DCM);

	// 補償則MagMomentの和を取る
	for (int i = 0; i < 3; i++){
		if (ACS_Mode(COMPENSATION_EARTH_OMEGA)){
			mag_moment_co[i] += g_acs.p_ACS->Mag_moment_Co_EarthAng[i];
		}
		if (ACS_Mode(COMPENXATION_COUPLING)){
			mag_moment_co[i] += g_acs.p_ACS->Mag_moment_Co_Coupling[i];
		}
		if (ACS_Mode(COMPENSATION_ANISOTROPY)){
			mag_moment_co[i] += g_acs.p_ACS->Mag_moment_Co_Anisotropy[i];
		}
		if (ACS_Mode(COMPENSATION_RMM)){
			mag_moment_co[i] += g_acs.p_ACS->Mag_moment_Co_RMM[i];
		}
		if (ACS_Mode(COMPENSATION_EDDY)){
			mag_moment_co[i] += g_acs.p_ACS->Mag_moment_Co_Eddy[i];
		}
		if (ACS_Mode(CONST_MAGMOMENT)){
			mag_moment_co[i] += g_acs.p_ACS->Mag_moment_Co_ConstMag[i];
		}
	}

	// 制御則MagMomentと補償則MagMomentの和を取る
	add(mag_moment_cn, 3, 1, mag_moment_co, 3, 1, mag_moment);

	// ACS_STRUCTへ格納
	GLOBAL_ACS_set_GYRO(omega);
	GLOBAL_ACS_set_MAG(B_fld);
	GLOBAL_ACS_set_MAGVec(Magvec);
	GLOBAL_ACS_set_SUNVec(sunvec);
	GLOBAL_ACS_set_Mag_moment(mag_moment);
	GLOBAL_ACS_set_Mag_moment_Co(mag_moment_co);
	GLOBAL_ACS_set_Quaternion(quaternion);
	GLOBAL_ACS_set_DCM(*DCM);
	GLOBAL_ACS_set_Velocity(Velocity);
	GLOBAL_ACS_set_Position(Position);
	GLOBAL_ACS_set_BallCoef(BallCoef);

	// 姿勢制御の場合はMagMomentをGLOBAL_AOCSにセット
	if (OCS_Mode(OCS_FLAG) == 0){

		// MagMomentに制限をかける
		if (g_acs.p_GLOBAL_AOCS->Maglimit_flag == 0){
			// 各軸ごとに制限
			for (int i = 0; i < 3; i++){
				if(mag_moment[i] > g_acs.p_GLOBAL_AOCS->MagLimit[i]){
					mag_moment[i] = g_acs.p_GLOBAL_AOCS->MagLimit[i];
				}else if(mag_moment[i] < -g_acs.p_GLOBAL_AOCS->MagLimit[i]){
					mag_moment[i] = -g_acs.p_GLOBAL_AOCS->MagLimit[i];
				}
			}
		}else{
			// 方向変化しない
			for (int i = 0; i < 3; i++){
				if(fabs(mag_moment[i]) > g_acs.p_GLOBAL_AOCS->MagLimit[i]){
					CoeffVec(mag_moment, 3, g_acs.p_GLOBAL_AOCS->MagLimit[i] / fabs(mag_moment[i]), mag_moment);
				}
			}
		}

		GLOBAL_AOCS_set_Mag_moment(mag_moment);

#ifdef HILS
		// Send torque to S2E	uint8_t mtq_dummyt_u8[24];
		uint8_t dummy_cmd[30] = { 'M', 'T', 'Q' };
		float mtq_control[3] = {mag_moment[0], mag_moment[1], mag_moment[2]};
		memcpy(dummy_cmd + 3, &AOCS_hils->utc_second, 4);
		memcpy(dummy_cmd + 7, mtq_control, 12);
		print(SILS, "%f\r\n", AOCS_hils->utc_second);
		HAL_UART_Transmit_C2A(&huart7,(uint8_t*)&dummy_cmd,19, 0xff);
#endif

	}
	ACS_time = TMGR_get_master_step();
	GLOBAL_ACS_set_ACScaltime(ACS_time);
}

static void ADS_Param_Set_update_(void){
	float omega[3] = {g_ods.p_ODS->Omega[0], g_ods.p_ODS->Omega[1], g_ods.p_ODS->Omega[2]};
	float B_fld[3] = {g_ods.p_ODS->MagField[0], g_ods.p_ODS->MagField[1], g_ods.p_ODS->MagField[2]};
	float Magvec[3]= {g_ods.p_ODS->MagVec[0], g_ods.p_ODS->MagVec[1], g_ods.p_ODS->MagVec[2]};
	float sunvec[3] = {g_ods.p_ODS->SunVec[0], g_ods.p_ODS->SunVec[1], g_ods.p_ODS->SunVec[2]};
	float mag_moment[3] = {g_ods.p_ODS->Mag_moment[0], g_ods.p_ODS->Mag_moment[1], g_ods.p_ODS->Mag_moment[2]};
//	float mag_moment_cn[3]= {g_acs.p_COMP->Mag_moment[0], g_acs.p_COMP->Mag_moment[1], g_acs.p_COMP->Mag_moment[2]};
//	float mag_moment_co[3]= {g_acs.p_COMP->Mag_moment[0], g_acs.p_COMP->Mag_moment[1], g_acs.p_COMP->Mag_moment[2]};
	float quaternion[4] = {g_ads.p_ADS->Quaternion[0], g_ads.p_ADS->Quaternion[1], g_ads.p_ADS->Quaternion[2], g_ads.p_ADS->Quaternion[3]};
	float DCM[3][3];
	float Velocity[3] = {g_ods.p_ODS->Velocity[0], g_ods.p_ODS->Velocity[1], g_ods.p_ODS->Velocity[2]};
	float Position[3] = {g_ods.p_ODS->Position[0], g_ods.p_ODS->Position[1], g_ods.p_ODS->Position[2]};
	Normalization(B_fld, 3, Magvec);
	copy(&g_ads.p_ADS->DCM_i2b[0][0], 3, 3, *DCM);

	GLOBAL_ADS_set_GYRO(omega);
	GLOBAL_ADS_set_MAG(B_fld);
	GLOBAL_ADS_set_MAGVec(Magvec);
	GLOBAL_ADS_set_SUNVec(sunvec);
	GLOBAL_ADS_set_Mag_moment(mag_moment);
	GLOBAL_ADS_set_Quaternion(quaternion);
	GLOBAL_ADS_set_DCM(*DCM);
	GLOBAL_ADS_set_Velocity(Velocity);
	GLOBAL_ADS_set_Position(Position);
}

static void OCS_Param_Set_update_(void){
	float omega[3] = {g_acs.p_ACS->Omega[0], g_acs.p_ACS->Omega[1], g_acs.p_ACS->Omega[2]};
	float B_fld[3] = {g_acs.p_ACS->MagField[0], g_acs.p_ACS->MagField[1], g_acs.p_ACS->MagField[2]};
	float Magvec[3]= {g_acs.p_ACS->MagVec[0], g_acs.p_ACS->MagVec[1], g_acs.p_ACS->MagVec[2]};
	float sunvec[3] = {g_acs.p_ACS->SunVec[0], g_acs.p_ACS->SunVec[1], g_acs.p_ACS->SunVec[2]};
	float mag_moment[3];// = {g_acs.p_ACS->Mag_moment[0], g_acs.p_ACS->Mag_moment[1], g_acs.p_ACS->Mag_moment[2]};
	float mag_moment_cn[3] = {g_ocs.p_OCS->Mag_moment_Cn[0], g_ocs.p_OCS->Mag_moment_Cn[1], g_ocs.p_OCS->Mag_moment_Cn[2]};
	float mag_moment_co[3] = {g_acs.p_ACS->Mag_moment_Co[0], g_acs.p_ACS->Mag_moment_Co[1], g_acs.p_ACS->Mag_moment_Co[2]};
	float quaternion[4] = {g_acs.p_ACS->Quaternion[0], g_acs.p_ACS->Quaternion[1], g_acs.p_ACS->Quaternion[2], g_acs.p_ACS->Quaternion[3]};
	float DCM[3][3];
	float Velocity[3] = {g_acs.p_ACS->Velocity[0], g_acs.p_ACS->Velocity[1], g_acs.p_ACS->Velocity[2]};
	float Position[3] = {g_acs.p_ACS->Position[0], g_acs.p_ACS->Position[1], g_acs.p_ACS->Position[2]};
	float BallCoef = g_acs.p_ACS->Ballistic_Coefficient;
	step_t OCS_time;
//	Normalization(B_fld, 3, Magvec);
	copy(&g_acs.p_ACS->DCM_i2b[0][0], 3, 3, *DCM);

	// 制御則MagMomentとACS補償則のMagMomentの和を取る
	add(mag_moment_cn, 3, 1, mag_moment_co, 3, 1, mag_moment);

	// OCS_STRUCTへ格納
	GLOBAL_OCS_set_GYRO(omega);
	GLOBAL_OCS_set_MAG(B_fld);
	GLOBAL_OCS_set_MAGVec(Magvec);
	GLOBAL_OCS_set_SUNVec(sunvec);
	GLOBAL_OCS_set_Mag_moment(mag_moment);
	GLOBAL_OCS_set_Mag_moment_Co(mag_moment_co);
	GLOBAL_OCS_set_Quaternion(quaternion);
	GLOBAL_OCS_set_DCM(*DCM);
	GLOBAL_OCS_set_Velocity(Velocity);
	GLOBAL_OCS_set_Position(Position);
	GLOBAL_OCS_set_BallCoef(BallCoef);
	GLOBAL_AOCS_set_PreMAG(B_fld);
	GLOBAL_AOCS_set_PreMAGVec(Magvec);

	// 軌道制御の場合はMagMomentをGLOBAL_AOCSにセット
	if (OCS_Mode(OCS_FLAG) == 1){

		// MagMomentに制限をかける
		if (g_acs.p_GLOBAL_AOCS->Maglimit_flag == 0){
			// 各軸ごとに制限
			for (int i = 0; i < 3; i++){
				if(mag_moment[i] > g_acs.p_GLOBAL_AOCS->MagLimit[i]){
					mag_moment[i] = g_acs.p_GLOBAL_AOCS->MagLimit[i];
				}else if(mag_moment[i] < -g_acs.p_GLOBAL_AOCS->MagLimit[i]){
					mag_moment[i] = -g_acs.p_GLOBAL_AOCS->MagLimit[i];
				}
			}
		}else{
			// 方向変化しない
			for (int i = 0; i < 3; i++){
				if(fabs(mag_moment[i]) > g_acs.p_GLOBAL_AOCS->MagLimit[i]){
					CoeffVec(mag_moment, 3, g_acs.p_GLOBAL_AOCS->MagLimit[i] / fabs(mag_moment[i]), mag_moment);
				}
			}
		}

		GLOBAL_AOCS_set_Mag_moment(mag_moment);

#ifdef HILS
		// Send torque to S2E	uint8_t mtq_dummyt_u8[24];
		uint8_t dummy_cmd[30] = { 'M', 'T', 'Q' };
        float mtq_control[3] = {mag_moment[0], mag_moment[1], mag_moment[2]};
        memcpy(dummy_cmd + 3, &AOCS_hils->utc_second, 4);
        memcpy(dummy_cmd + 7, mtq_control, 12);
        HAL_UART_Transmit_C2A(&huart7,(uint8_t*)&dummy_cmd,19, 0xff);
#endif

	}
	OCS_time = TMGR_get_master_step();
	GLOBAL_OCS_set_OCScaltime(OCS_time);
}


static void ODS_Param_Set_update_(void){
	float omega[3] = {g_acs.p_GLOBAL_AOCS->Omega[0], g_acs.p_GLOBAL_AOCS->Omega[1], g_acs.p_GLOBAL_AOCS->Omega[2]};
	float B_fld[3] = {g_acs.p_GLOBAL_AOCS->MagField[0], g_acs.p_GLOBAL_AOCS->MagField[1], g_acs.p_GLOBAL_AOCS->MagField[2]};
	float Magvec[3] = {g_acs.p_GLOBAL_AOCS->MagVec[0], g_acs.p_GLOBAL_AOCS->MagVec[1], g_acs.p_GLOBAL_AOCS->MagVec[2]};
	float sunvec[3] = {g_acs.p_GLOBAL_AOCS->SunVec[0], g_acs.p_GLOBAL_AOCS->SunVec[1], g_acs.p_GLOBAL_AOCS->SunVec[2]};
	float mag_moment[3] = {g_acs.p_GLOBAL_AOCS->Mag_moment[0], g_acs.p_GLOBAL_AOCS->Mag_moment[1], g_acs.p_GLOBAL_AOCS->Mag_moment[2]};
//	float mag_moment_cn[3]= {g_acs.p_COMP->Mag_moment[0], g_acs.p_COMP->Mag_moment[1], g_acs.p_COMP->Mag_moment[2]};
//	float mag_moment_co[3]= {g_acs.p_COMP->Mag_moment[0], g_acs.p_COMP->Mag_moment[1], g_acs.p_COMP->Mag_moment[2]};
	float quaternion[4] = {g_acs.p_GLOBAL_AOCS->Quaternion[0], g_acs.p_GLOBAL_AOCS->Quaternion[1], g_acs.p_GLOBAL_AOCS->Quaternion[2], g_acs.p_GLOBAL_AOCS->Quaternion[3]};
	float DCM[3][3];
	float Velocity[3] = {g_acs.p_GLOBAL_AOCS->Velocity[0], g_acs.p_GLOBAL_AOCS->Velocity[1], g_acs.p_GLOBAL_AOCS->Velocity[2]};
	float Position[3] = {g_acs.p_GLOBAL_AOCS->Position[0], g_acs.p_GLOBAL_AOCS->Position[1], g_acs.p_GLOBAL_AOCS->Position[2]};
	float Latitude = g_acs.p_GLOBAL_AOCS->Latitude;
	float Longitude = g_acs.p_GLOBAL_AOCS->Longitude;
	float Altitude = g_acs.p_GLOBAL_AOCS->Altitude;
	float ECEF_p[3] = {g_acs.p_GLOBAL_AOCS->Position_ECEF[0], g_acs.p_GLOBAL_AOCS->Position_ECEF[1], g_acs.p_GLOBAL_AOCS->Position_ECEF[2]};
//	Normalization(B_fld, 3, Magvec);
	copy(&g_acs.p_GLOBAL_AOCS->DCM_i2b[0][0], 3, 3, *DCM);


	GLOBAL_ODS_set_GYRO(omega);
	GLOBAL_ODS_set_MAG(B_fld);
	GLOBAL_ODS_set_MAGVec(Magvec);
	GLOBAL_ODS_set_SUNVec(sunvec);
	GLOBAL_ODS_set_Mag_moment(mag_moment);
	GLOBAL_ODS_set_Quaternion(quaternion);
	GLOBAL_ODS_set_DCM(*DCM);

	GLOBAL_ODS_set_Velocity(Velocity);
	GLOBAL_ODS_set_Position(Position);
	GLOBAL_ODS_set_Latitude(Latitude);
	GLOBAL_ODS_set_Longitude(Longitude);
	GLOBAL_ODS_set_Altitude(Altitude);

	GLOBAL_AOCS_set_Quaternion(quaternion);
	GLOBAL_AOCS_set_DCM(*DCM);
}

static void ACS_Param_Reset_(void){
	float zero_vec[3] = {0.0f};
	GLOBAL_ACS_set_Mag_moment(zero_vec);
	GLOBAL_ACS_set_Mag_moment_Cn(zero_vec);
	GLOBAL_ACS_set_Mag_moment_Co(zero_vec);
	GLOBAL_ACS_set_Mag_moment_Co_EarthAng(zero_vec);
	GLOBAL_ACS_set_Mag_moment_Co_Coupling(zero_vec);
	GLOBAL_ACS_set_Mag_moment_Co_Anisotropy(zero_vec);
	GLOBAL_ACS_set_Mag_moment_Co_RMM(zero_vec);
	GLOBAL_ACS_set_Mag_moment_Co_Eddy(zero_vec);
	GLOBAL_ACS_set_Mag_moment_Co_ConstMag(zero_vec);
}

static void OCS_Param_Reset_(void){
	float zero_vec[3] = {0.0f};
	GLOBAL_OCS_set_Mag_moment(zero_vec);
	GLOBAL_OCS_set_Mag_moment_Cn(zero_vec);
	GLOBAL_OCS_set_Mag_moment_Co(zero_vec);
}
