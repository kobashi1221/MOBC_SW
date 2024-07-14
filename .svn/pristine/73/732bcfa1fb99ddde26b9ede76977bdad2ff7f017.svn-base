/*
 * Gloval_AOCS_Parameter.h
 *
 *  Created on: 2021/10/13
 *      Author: masud
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_AOCS_GLOBAL_AOCS_PARAMETER_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_AOCS_GLOBAL_AOCS_PARAMETER_H_


#include <stdint.h> //for uint8_t

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
//class GLOBAL_AOCS_CLASS
//{
//public:
//	float MagLmit[3];
//	#ifdef TWO_U
//		static constexpr float maglimit_[3] = {0.113f, 0.113f, 1.130f};
//	#endif
//	#ifdef ONE_U
//		static constexpr float maglimit_[3] = {0.0113f, 0.0113f, 0.113f};
//	#endif
//};

typedef enum{
    CALCULATION_SUCCESS,
    CALCULATION_FAILED
}AOCS_STATUS_STRUCT;

typedef struct{
	uint32_t ACS_MODE_Number;
	uint32_t ADS_MODE_Number;
	uint32_t OCS_MODE_Number;
	uint32_t ODS_MODE_Number;
	uint32_t AOCS_DRIVER_MODE_Number;
}AOCS_MODE_STRUCT;

typedef struct
{
	float Omega[3];         //gyro[rad/s]
	ObcTime OmeTime;        //AOCS使用角速度取得時刻
	ObcTime OmePreTime;     //1ステップ前のAOCS使用角速度取得時刻

	float MagField[3];      //magsenser[T]
	float MagVec[3];        //magsenser norm
	ObcTime MagTime;        //AOCS使用磁束密度取得時刻
	ObcTime MagPreTime;     //1ステップ前のAOCS使用磁束密度取得時刻
	float pre_MagField[3];  //previous magsenser[T]
	float pre_MagVec[3];    //previous magsenser norm

	float SunVec[3];        //sunsenser norm
	ObcTime SunTime;        //AOCS使用太陽ベクトル取得時刻
	ObcTime SunPreTime;     //1ステップ前のAOCS使用太陽ベクトル取得時刻

	ObcTime MemsGyroTime;   //精ジャイロセンサ取得時刻
	ObcTime MemsMagTime;    //精磁気センサ取得時刻
	ObcTime NineAxTime;     //9軸センサ取得時刻
	ObcTime SunsTime[6];       //サンセンサ取得時刻

	float Mag_moment[3];    //[A/m^2]

	float MagLimit[3];      //[A/m^2]

	float Quaternion[4];

	float DCM_i2b[3][3];

	float Position[3];      //ECI [m]
	float Velocity[3];      //ECI [m/s]
	float velocity_norm;     //[km/s]
	float Position_ECEF[3]; //ECEF
	float Velocity_ECEF[3]; //ECEF
	float Latitude;         //[deg]
	float Longitude;        //[deg]
	float Altitude;         //[m]

	float Inertia_tensor[3][3];
	float Inertia_tensor_copy[3][3];
	float Inertia_tensor_inv[3][3];

	float RM_moment[3];

	uint8_t Eclipse_flag;  //0x00: 蝕でない, 0xFF: 蝕である
			       //サンセンサ取得後に判断 コメント記入: 浮田 2023/06/19

	uint8_t Maglimit_flag;

	uint8_t GPS_Status;

	step_t Sensor_time;

	AOCS_MODE_STRUCT AOCS_MODE;

} GLOBAL_AOCS_PARAMETER_STRUCT;

typedef struct
{
	float Omega[3];    //gyro[rad/s]
	float MagField[3]; //magsenser[T]
	float MagVec[3];   //magsenser norm
	float SunVec[3];   //sunsenser norm
	float pre_MagField[3];      //previous magsenser[T]
	float pre_MagVec[3];        //previous magsenser norm

	float Before_Mag_moment[3];    //必要磁気モーメント
	float Mag_moment[3];    //指令磁気モーメント
	float Mag_moment_Cn[3]; //制御則用(Bdot, CrossProduct)
	float Mag_moment_Co[3]; //補償則用(Compensation)

	float MagLimit[3];

	float Quaternion[4];

	float DCM_i2b[3][3];

	float Position[3];      //ECI [m]
	float Velocity[3];      //ECI [m/s]
	float Position_ECEF[3]; //ECEF
	float Velocity_ECEF[3]; //ECEF
	float Latitude;         //[deg]
	float Longitude;        //[deg]
	float Altitude;         //[m]

	float Inertia_tensor[3][3];
	float Inertia_tensor_copy[3][3];
	float Inertia_tensor_inv[3][3];

	float ADS_TIME;

	float RM_moment[3];

	uint8_t ads_gyro_sensor_choice;       // アノマリ対応時,9AX or GYRO どちらのフラグを上げるか決めるMRAMパラメータ
} GLOBAL_ADS_PARAMETER;

typedef struct
{
	float Omega[3];    //gyro[rad/s]
	float MagField[3]; //magsenser[T]
	float MagVec[3];   //magsenser norm
	float SunVec[3];   //sunsenser norm
	float pre_MagField[3];      //previous magsenser[T]
	float pre_MagVec[3];        //previous magsenser norm

	float Before_Mag_moment[3];    //必要磁気モーメント
	float Mag_moment[3];    //指令磁気モーメント
	float Mag_moment_Cn[3]; //制御則用(Bdot, CrossProduct)
	float Mag_moment_Co[3]; //補償則用(Compensation)
	float Mag_moment_Co_EarthAng[3];
	float Mag_moment_Co_Coupling[3];
	float Mag_moment_Co_Anisotropy[3];
	float Mag_moment_Co_RMM[3];
	float Mag_moment_Co_Eddy[3];
	float Mag_moment_Co_ConstMag[3];

	float MagLimit[3];

	float TagetVec[3];//期待座標系使用目標ベクトル
	float InertialTagetVec[3];//慣性座標系目標ベクトル（地球、太陽等）
	float COBC_TargetDirection[3];   //COBC目標ベクトル（地球、太陽等）
	float Velocity_b[3];

	float Quaternion[4];
	float TargetQuaternion_COBC[4];
	float TargetQuaternion_MOBC[4];

	float DCM_i2b[3][3];

	float Position[3];      //ECI [m]
	float Velocity[3];      //ECI [m/s]
	float Position_ECEF[3]; //ECEF
	float Velocity_ECEF[3]; //ECEF
	float Latitude;         //[deg]
	float Longitude;        //[deg]
	float Altitude;         //[m]

	float Inertia_tensor[3][3];
	float Inertia_tensor_copy[3][3];
	float Inertia_tensor_inv[3][3];

	float Ballistic_Coefficient;

	step_t ACS_time;

	float RM_moment[3];

} GLOBAL_ACS_PARAMETER;

typedef struct
{
	float Omega[3];    //gyro[rad/s]
	float MagField[3]; //magsenser[T]
	float MagVec[3];   //magsenser norm
	float SunVec[3];   //sunsenser norm
	float pre_MagField[3];      //previous magsenser[T]
	float pre_MagVec[3];        //previous magsenser norm

	float Before_Mag_moment[3];    //必要磁気モーメント
	float Mag_moment[3];    //指令磁気モーメント
	float Mag_moment_Cn[3]; //制御則用(Bdot, CrossProduct)
	float Mag_moment_Co[3]; //補償則用(Compensation)

	float MagLimit[3];      //最大磁気モーメント

	float Quaternion[4];

	float DCM_i2b[3][3];

	float Position[3];      //ECI [m]
	float Velocity[3];      //ECI [m/s]
	float Position_ECEF[3]; //ECEF
	float Velocity_ECEF[3]; //ECEF
	float Latitude;         //[deg]
	float Longitude;        //[deg]
	float Altitude;         //[m]

	float RelativePosition[3];
	float RelativeVelocity[3];

	float Inertia_tensor[3][3];
	float Inertia_tensor_copy[3][3];
	float Inertia_tensor_inv[3][3];

	float Ballistic_Coefficient;

	float ODS_TIME;

	float RM_moment[3];

} GLOBAL_ODS_PARAMETER;

typedef struct
{
	float Omega[3];    //gyro[rad/s]
	float MagField[3]; //magsenser[T]
	float MagVec[3];   //magsenser norm
	float SunVec[3];   //sunsenser norm
	float pre_MagField[3];      //previous magsenser[T]
	float pre_MagVec[3];        //previous magsenser norm

	float Before_Mag_moment[3];    //必要磁気モーメント
	float Mag_moment[3];    //指令磁気モーメント
	float Mag_moment_Cn[3]; //制御則用(Bdot, CrossProduct)
	float Mag_moment_Co[3]; //補償則用(Compensation)

	float MagLimit[3];

	float Quaternion[4];

	float DCM_i2b[3][3];

	float Position[3];      //ECI [m]
	float Velocity[3];      //ECI [m/s]
	float Position_ECEF[3]; //ECEF
	float Velocity_ECEF[3]; //ECEF
	float Latitude;         //[deg]
	float Longitude;        //[deg]
	float Altitude;         //[m]

	float RelativePosition[3];
	float RelativeVelocity[3];

	float Inertia_tensor[3][3];
	float Inertia_tensor_copy[3][3];
	float Inertia_tensor_inv[3][3];

	float Ballistic_Coefficient;
	float Ballistic_Coefficient_order;
	float Ballistic_Coefficient_opponent;
	float Ballistic_Coefficient_opponent_order;

	float RM_moment[3];

	step_t OCS_time;
} GLOBAL_OCS_PARAMETER;


typedef enum{
	NINEAX_MAG,
	NINEAX_MAG_TEMP,
	NINEAX_GYRO,
	NINEAX_GYRO_TEMP,
	NINEAX_ACC,
	NINEAX_ACC_TEMP,
	SUN_UNI,
	SUN_AVE,
	MEMS_GYRO,
	MEMS_GYRO_TEMP,
	PRECIZE_MAG,
	PRECIZE_MAG_TEMP,
	BBDOT_CALC,
	IGRF_CALC,
	SUNVEC_INERTIA,
	EARTHVEC,
	TRIAD_CALC,
	QMETHOD_CALC,
	QUEST_CALC,
	EKF_GYRO,
	EKF_GYRO_COV,
	EKF_GYROBIAS,
	EKF_GYROBIAS_COV,
	EKF_MAGSUNGYRO,
	EKF_MAGSUNGYRO_COV,
	EKF_EDDY,
	EKF_EDDY_COV,
	EKF_RESIDUAL_MAG,
	EKF_RESIDUAL_MAG_COV,
	EKF_ANISOTROPY_MAG,
	EKF_ANISOTROPY_MAG_COV,
}ADS_MODE;

typedef enum{
	BDOT_CALC,
	LINE_MAG,
	BDOT_DELTA_CALC,
	EARTH_CENTER_POINTING,
	EARTH_CENTER_QUA,
	EARTH_POINT_POINTING,
	EARTH_POINT_QUA,
	VELOCITY_DIRECTION_POINTING,
	VELOCITH_DIRECTION_QUA,
	INERTIA_POINTING,
	INERTIA_QUA,
	ATTITUDE_TRAJECTORY_POINTING,
	ATTITUDE_TRAJECTORY_QUA,
	OMEGA_CONTROL,
	SUN_POINTING,
	SUN_POINTING_ECLIPSE,
	COMPENSATION_EARTH_OMEGA,
	COMPENXATION_COUPLING,
	COMPENSATION_RMM,
	COMPENSATION_ANISOTROPY,
	COMPENSATION_EDDY,
	CONST_MAGMOMENT,
	PHOTO_COBC,
	OUTPUT_MTQA_X,
	MTQA_X_TEMP,
	OUTPUT_MTQA_Y,
	MTQA_Y_TEMP,
	OUTPUT_MTQA_Z,
	MTQA_Z_TEMP,
	OUTPUT_CONST_MTQO,
	MTQO_TEMP,
	TBD3,
}ACS_MODE;

typedef enum{
	AVE_DECELERATE,
	MIN_DECELERATE,
	MAX_DECELERATE,
	KEEP_DECELERATE,
	SET_VELOCITYVEC_ANG,
	KEEP_MAG_DIRECTION,
	SET_MAG_DIRECTION_ANG,
	SET_PROFILE_VELOCITY_SUN_MAG,
	OCS_TBDONE,
	BANG_BANG,
	LQR_NORO,
	MPC_NAGAI,
	COMPENSATION_AERO,
	OCS_TBDTWO,
	ESCAPE_COLLISION,
	OCS_FLAG,
}OCS_MODE;

typedef enum{
	GPS_TWO_ANTENNA,
	GPS_FRONT_ANTENNA,
	GPS_RESERVE_ANTENNA,
	ECLIPS_SUNBAT,
	ECLIPS_SUNS,
	SGPFOUR,
	RSSI_AMATURE,
	RSSI_LORA,
	SGPFOUROPPONENT,
	RELATIVE_PROPAGATION,
	ORBPROP,
	EKF_GPS,
	EKF_GPS_COV,
	EKF_RADIO_WAVE,
	EKF_RADIO_WAVE_COV,
	ANOTHER_SAT_DIRECYION,
	EVALUATION_RELATIVE_ORBIT_CONDITION,
	EKF_ECLIPS_MAG,
	EKF_ECLIPS_MAG_COV,
	COLLISION_PROPABILITY,
	PLASMA,
	BALLCOEF,
}ODS_MODE;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo Global_AOCS_update(void);
//AppInfo Global_AOCS_Param_update(void);
void GLOBAL_AOCS_set_GYROX(float omegax);
void GLOBAL_AOCS_set_GYROY(float omegay);
void GLOBAL_AOCS_set_GYROZ(float omegaz);
void GLOBAL_AOCS_set_GYRO_TIME(ObcTime OmePreTime, ObcTime OmeTime);
void GLOBAL_AOCS_set_MAGX(float magx);
void GLOBAL_AOCS_set_MAGY(float magy);
void GLOBAL_AOCS_set_MAGZ(float magz);
void GLOBAL_AOCS_set_MAGVec(float* magvec);
void GLOBAL_AOCS_set_MAG_TIME(ObcTime MagPreTime, ObcTime MagTime);
void GLOBAL_AOCS_set_SUNVec(float*sunvec);
void GLOBAL_AOCS_set_SUN_TIME(ObcTime SunPreTime, ObcTime SunTime);
void GLOBAL_AOCS_set_MEMS_GYRO_GET_TIME(ObcTime MemsGyroTime);
void GLOBAL_AOCS_set_MEMS_MAG_GET_TIME(ObcTime MemsMagTime);
void GLOBAL_AOCS_set_NineAx_GET_TIME(ObcTime NineAxTime);
void GLOBAL_AOCS_set_SUNS_GET_TIME(int i, ObcTime SunsTime);
void GLOBAL_AOCS_set_Mag_moment(float* Mag_moment);
void GLOBAL_AOCS_set_Quaternion(float* Quaternion);
void GLOBAL_AOCS_set_DCM(float* DCM);
void GLOBAL_AOCS_set_Velocity(float* Velocity);
void GLOBAL_AOCS_set_velocity_norm(float velocity_norm);
void GLOBAL_AOCS_set_Position(float* Position);
void GLOBAL_AOCS_set_Eclipse();
void GLOBAL_AOCS_reset_Eclipse();
uint8_t vote_eclipse_flag(const uint8_t* flag);
void GLOBAL_AOCS_set_GPS_Status(uint8_t status);
void GLOBAL_AOCS_set_Latitude(float Latitude);
void GLOBAL_AOCS_set_Longitude(float Longitude);
void GLOBAL_AOCS_set_Altitude(float Altitude);
void GLOBAL_AOCS_set_Position_ECEF(float* Position);
void GLOBAL_AOCS_set_Velocity_ECEF(float* Velocity);
void GLOBAL_AOCS_set_PreMAG(float* mag);
void GLOBAL_AOCS_set_PreMAGVec(float* magvec);
void GLOBAL_AOCS_set_SensorTime(step_t time);

void GLOBAL_ACS_set_GYRO(float* omega);
void GLOBAL_ACS_set_MAG(float* mag);
void GLOBAL_ACS_set_MAGVec(float* magvec);
void GLOBAL_ACS_set_SUNVec(float*sunvec);
void GLOBAL_ACS_set_Mag_moment(float* Mag_moment);
void GLOBAL_ACS_set_Mag_moment_Cn(float* Mag_moment_Cn);
void GLOBAL_ACS_set_Mag_moment_Co(float* Mag_moment_Co);
void GLOBAL_ACS_set_Mag_moment_Co_EarthAng(float* Mag_moment_Co);
void GLOBAL_ACS_set_Mag_moment_Co_Coupling(float* Mag_moment_Co);
void GLOBAL_ACS_set_Mag_moment_Co_Anisotropy(float* Mag_moment_Co);
void GLOBAL_ACS_set_Mag_moment_Co_RMM(float* Mag_moment_Co);
void GLOBAL_ACS_set_Mag_moment_Co_Eddy(float* Mag_moment_Co);
void GLOBAL_ACS_set_Mag_moment_Co_ConstMag(float* Mag_moment_Co);
void GLOBAL_ACS_set_Quaternion(float* Quaternion);
void GLOBAL_ACS_set_DCM(float* DCM);
void GLOBAL_ACS_set_Velocity(float* Velocity);
void GLOBAL_ACS_set_Position(float* Position);
void GLOBAL_ACS_set_Before_Mag_moment(float* Mag_moment);
void GLOBAL_ACS_set_BallCoef(float BC);
void GLOBAL_ACS_set_InertialVec(float* InertialVec);
void GLOBAL_ACS_set_COBCVec(float* COBCVec);
void GLOBAL_ACS_set_TargetVec(const float* TargetVec);
void GLOBAL_ACS_set_Velocity_b(const float* VelVec);
void GLOBAL_ACS_set_ACScaltime(step_t time);

void GLOBAL_ADS_set_GYRO(float* omega);
void GLOBAL_ADS_set_MAG(float* mag);
void GLOBAL_ADS_set_MAGVec(float* magvec);
void GLOBAL_ADS_set_SUNVec(float*sunvec);
void GLOBAL_ADS_set_Mag_moment(float* Mag_moment);
void GLOBAL_ADS_set_Mag_moment_Cn(float* Mag_moment_Cn);
void GLOBAL_ADS_set_Mag_moment_Co(float* Mag_moment_Co);
void GLOBAL_ADS_set_Quaternion(float* Quaternion);
void GLOBAL_ADS_set_DCM(float* DCM);
void GLOBAL_ADS_set_Velocity(float* Velocity);
void GLOBAL_ADS_set_Position(float* Position);

void GLOBAL_OCS_set_GYRO(float* omega);
void GLOBAL_OCS_set_MAG(float* mag);
void GLOBAL_OCS_set_MAGVec(float* magvec);
void GLOBAL_OCS_set_SUNVec(float*sunvec);
void GLOBAL_OCS_set_Mag_moment(float* Mag_moment);
void GLOBAL_OCS_set_Mag_moment_Cn(float* Mag_moment_Cn);
void GLOBAL_OCS_set_Mag_moment_Co(float* Mag_moment_Co);
void GLOBAL_OCS_set_Quaternion(float* Quaternion);
void GLOBAL_OCS_set_DCM(float* DCM);
void GLOBAL_OCS_set_Velocity(float* Velocity);
void GLOBAL_OCS_set_Position(float* Position);
void GLOBAL_OCS_set_Before_Mag_moment(float* Mag_moment);
void GLOBAL_OCS_set_BallCoef(float BC);
void GLOBAL_OCS_set_OCScaltime(step_t time);

void GLOBAL_ODS_set_GYRO(float* omega);
void GLOBAL_ODS_set_MAG(float* mag);
void GLOBAL_ODS_set_MAGVec(float* magvec);
void GLOBAL_ODS_set_SUNVec(float*sunvec);
void GLOBAL_ODS_set_Mag_moment(float* Mag_moment);
void GLOBAL_ODS_set_Mag_moment_Cn(float* Mag_moment_Cn);
void GLOBAL_ODS_set_Mag_moment_Co(float* Mag_moment_Co);
void GLOBAL_ODS_set_Quaternion(float* Quaternion);
void GLOBAL_ODS_set_DCM(float* DCM);
void GLOBAL_ODS_set_Velocity(float* Velocity);
void GLOBAL_ODS_set_Position(float* Position);
void GLOBAL_ODS_set_Latitude(float Latitude);
void GLOBAL_ODS_set_Longitude(float Longitude);
void GLOBAL_ODS_set_Altitude(float Altitude);
void GLOBAL_ODS_set_Position_ECEF(float* Position_ECEF);
void GLOBAL_ODS_set_Velocity_ECEF(float* Velocity_ECEF);
void GLOBAL_ODS_set_BallCoef(float BC);

int ADS_Mode(ADS_MODE ADS_MODE);
int ACS_Mode(ACS_MODE ACS_MODE);
int ODS_Mode(ODS_MODE ODS_MODE);
int OCS_Mode(OCS_MODE OCS_MODE);

void ADS_Mode_Set(uint32_t ADS_Mode_Num);
void ACS_Mode_Set(uint32_t ACS_Mode_Num);
void ODS_Mode_Set(uint32_t ODS_Mode_Num);
void OCS_Mode_Set(uint32_t OCS_Mode_Num);
void AOCS_DRIVER_Mode_Set(uint32_t AOCS_DRIVER_MODE_Num);

uint32_t ADS_Mode_Get(void);
uint32_t ACS_Mode_Get(void);
uint32_t ODS_Mode_Get(void);
uint32_t OCS_Mode_Get(void);
uint32_t AOCS_DRIVER_Mode_Get(void);


AOCS_STATUS_STRUCT GLOBAL_AOCS_set_Status(float *value, int n);
#ifdef __cplusplus
 }
#endif


#endif /* SRC_USER_APPLICATIONS_USERDEFINED_AOCS_GLOBAL_AOCS_PARAMETER_H_ */
