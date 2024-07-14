
#include "nbc_header.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>

#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>

void BC_load_pt_sensor_rotate(void){
	//８本Ｐｔセンサがあるが，変化は低速であるため＋MUX切り替えに時間がかかるためloop取得
	for(int i=1; i<=16; i++){ //後ほどMTQZ1~Z4の温度も取得するので12→16に変更する
	BCL_tool_prepare_param_uint8(i);
	BCL_tool_register_cmd(2*i, Cmd_CODE_ACS_DEBUG_PT_TEMS);
	}
}

void BC_load_mtq_curvols_rotate(void){
	// MUX切り替えに時間がかかるため。loop取得
	for(int i=1; i<=2; i++){
		if(i<=2){
			BCL_tool_prepare_param_uint8(i);
			BCL_tool_register_cmd(2*i, Cmd_CODE_PWR_ACS1_VOLS_READ);
		}
//		else{ // 毎週期取得するのでコメントオフ
//			BCL_tool_prepare_param_uint8(i-2);
//			BCL_tool_register_cmd(2*i, Cmd_CODE_PWR_ACS1_CURS_READ);
//		}
	}
}

void BC_load_sensor_handler(void){
#ifdef HILS
  BCL_tool_register_app    (1, AR_AOCS_HILS_APP);
#endif

#ifdef TWO_U
    BCL_tool_register_rotate (1, BC_MTQ_CURVOLS_ROTATE);
    BCL_tool_prepare_param_uint8(0);
    BCL_tool_register_cmd    (2, Cmd_CODE_PWR_ACS1_CURS_READ);
#endif

	BCL_tool_register_cmd    (2, Cmd_CODE_ACS_MTQ_ZERO);

	BCL_tool_register_app    (3, AR_AOCS_SENSOR_TIME_APP);

#ifdef TWO_U
	BCL_tool_register_rotate (4, BC_Pt_ROTATE);
#endif

	BCL_tool_register_app    (5, AR_COM_SOBC_REGULAR_TLM); // kobayashi added (SOBCからの生存信号には姿勢系で使うセンサの値も含むためここに追加)

	BCL_tool_register_app    (6, AR_ACS_SUNS_APP);

	BCL_tool_register_app    (7, AR_ACS_9AX_APP);

#ifdef TWO_U
	BCL_tool_prepare_param_uint8(0);
	BCL_tool_register_cmd    (8, Cmd_CODE_ACS_DEBUG_MAGS);
#endif

	BCL_tool_register_cmd    (9, Cmd_CODE_ACS_MTQ_RETURN);

	BCL_tool_prepare_param_uint8(0);
	BCL_tool_register_cmd    (10, Cmd_CODE_ACS_DEBUG_GYRO);

 BCL_tool_register_app    (11, AR_ADS_GST_APP);        // GPSより先に計算させたい

 	BCL_tool_register_app    (12, AR_ACS_GPS_APP);

 	BCL_tool_register_app    (13, AR_ACS_PSD_APP);

}

void BC_load_orbit_determination(void){
	BCL_tool_register_app    (1, AR_GLOBAL_ODS_PARAM_SET_APP);

	if (ODS_Mode(SGPFOUR)){
		BCL_tool_register_app    (10, AR_ODS_SGP4_APP);
	}
	if (ODS_Mode(SGPFOUROPPONENT)){
		BCL_tool_register_app    (15, AR_ODS_SGP4_OPPONENT_APP);
	}
	if (ODS_Mode(ORBPROP)){
		BCL_tool_register_app    (20, AR_ODS_ORBPROP_APP);
	}
    if (ODS_Mode(EKF_GPS)){
        BCL_tool_register_app    (30, AR_ODS_ORBEKF_APP);
    }
	if (ODS_Mode(PLASMA)){
		BCL_tool_prepare_param_uint8(0);
		BCL_tool_register_cmd    (40, Cmd_CODE_ACS_DEBUG_PLASMA);
	}

}

void BC_load_attitude_determination(void){

	BCL_tool_register_app    (1, AR_GLOBAL_ADS_PARAM_SET_APP);

	if (ADS_Mode(IGRF_CALC)){
		BCL_tool_register_app    (5, AR_ADS_IGRF_APP);
	}

	if (ADS_Mode(SUNVEC_INERTIA)){
		BCL_tool_register_app    (10, AR_ADS_SUNVECTOR_APP);
	}

	if (ADS_Mode(TRIAD_CALC)){
		BCL_tool_register_app    (15, AR_ADS_TRIAD_APP);
	}

	if (ADS_Mode(QMETHOD_CALC)){
		BCL_tool_register_app    (20, AR_ADS_QMETHOD_APP);
	}

	if (ADS_Mode(QUEST_CALC)){
		BCL_tool_register_app    (25, AR_ADS_QUEST_APP);
	}

	if (ADS_Mode(EKF_GYRO) || ADS_Mode(EKF_GYROBIAS) ||
		ADS_Mode(EKF_MAGSUNGYRO) || ADS_Mode(EKF_EDDY) ||
		ADS_Mode(EKF_RESIDUAL_MAG) || ADS_Mode(EKF_ANISOTROPY_MAG)){
		BCL_tool_register_app    (30, AR_AOCS_EKF_APP);
	}

	if (ADS_Mode(BBDOT_CALC)){
		BCL_tool_register_app    (35, AR_ADS_BBDOT_APP);
	}

	if (ADS_Mode(EARTHVEC)){
		BCL_tool_register_app    (40, AR_ACS_EARTHVECTOR_APP);
	}

	if (ODS_Mode(BALLCOEF)){
		BCL_tool_register_app    (45, AR_ODS_BALLCOEF_APP);
	}

}

void BC_load_attitude_control(void){

	BCL_tool_register_app    (86, AR_GLOBAL_ACS_PARAM_RESET_APP);

	if (ACS_Mode(BDOT_CALC)){
		BCL_tool_register_app    (88, AR_ACS_BDOT_APP);
	}

	if (ACS_Mode(EARTH_CENTER_POINTING)){
		  BCL_tool_prepare_param_uint8(2);
		  
		  BCL_tool_register_app    (90, AR_ACS_CROSSPRODUCT_EARTH_CENTER_POINTING_APP);
	}

	if (ACS_Mode(EARTH_CENTER_QUA)){
		  BCL_tool_prepare_param_uint8(2);
		  BCL_tool_register_app    (92, AR_ACS_CROSSPRODUCT_QFB);
	}

	if (ACS_Mode(EARTH_POINT_POINTING)){
		  BCL_tool_prepare_param_uint8(3);
		
		  BCL_tool_register_app    (94, AR_ACS_CROSSPRODUCT_EARTH_POINT_POINTING_APP);
	}

	if (ACS_Mode(EARTH_POINT_QUA)){
		  BCL_tool_prepare_param_uint8(3);

		  BCL_tool_register_app    (96, AR_ACS_CROSSPRODUCT_QFB);
	}

	if (ACS_Mode(VELOCITY_DIRECTION_POINTING)){
		  BCL_tool_prepare_param_uint8(3);
		 
		  BCL_tool_register_app    (98, AR_ACS_CROSSPRODUCT_VELOCITY_POINTING_APP);
	}

	if (ACS_Mode(VELOCITH_DIRECTION_QUA)){
		  BCL_tool_prepare_param_uint8(3);

		  BCL_tool_register_app    (100, AR_ACS_CROSSPRODUCT_QFB);
	}

	if (ACS_Mode(INERTIA_POINTING)){
		  BCL_tool_prepare_param_uint8(1);

		  BCL_tool_register_app    (102, AR_ACS_CROSSPRODUCT_INERTIAL_POINTING_APP);
	}

	if (ACS_Mode(INERTIA_QUA)){
		  BCL_tool_prepare_param_uint8(1);
		  BCL_tool_register_app    (104, AR_ACS_CROSSPRODUCT_QFB);
	}

	if (ACS_Mode(ATTITUDE_TRAJECTORY_POINTING)){
		  BCL_tool_prepare_param_uint8(1);

		  BCL_tool_register_app    (106, AR_ACS_CROSSPRODUCT_TRAJECTRY_POINTING_APP);
	}

	if (ACS_Mode(ATTITUDE_TRAJECTORY_QUA)){
		  BCL_tool_prepare_param_uint8(1);

		  BCL_tool_register_app    (108, AR_ACS_CROSSPRODUCT_QFB);
	}

	if (ACS_Mode(OMEGA_CONTROL)){

//		  BCL_tool_register_app    (110, AR_ACS_CROSSPRODUCT_ANGULARMOMENTUM_APP);
		BCL_tool_register_cmd    (110, Cmd_CODE_ACS_CrossProduct_SPIN_HILS);
	}

	if (ACS_Mode(SUN_POINTING)){
		  BCL_tool_prepare_param_uint8(1);
		  BCL_tool_register_app    (112, AR_ACS_CROSSPRODUCT_SUN_POINTING_APP);
	}

	if (ACS_Mode(COMPENSATION_EARTH_OMEGA)){
		  BCL_tool_register_app    (114, AR_ACS_COMPENSATION_EARTHANGULARVELOCITY_APP);
	}

	if (ACS_Mode(COMPENXATION_COUPLING)){
		  BCL_tool_register_app    (114, AR_ACS_COMPENSATION_COUPLING_APP);
	}

	if (ACS_Mode(COMPENSATION_RMM)){
		  BCL_tool_register_app    (114, AR_ACS_COMPENSATION_RMM_APP);
	}

	if (ACS_Mode(COMPENSATION_ANISOTROPY)){
		  BCL_tool_prepare_param_uint8(1);
		  BCL_tool_register_cmd    (114, Cmd_CODE_ACS_COMPENSATION_SHAPEANISORTROPY);
	}

	if (ACS_Mode(COMPENSATION_EDDY)){
		  BCL_tool_register_app    (114, AR_ACS_COMPENSATION_EDDYCURRENT_APP);
	}

	if (ACS_Mode(CONST_MAGMOMENT)){
		  BCL_tool_register_app    (114, AR_ACS_COMPENSATION_CONSTANTMAGNETIC_APP);
	}

	if (ACS_Mode(BDOT_DELTA_CALC)){
		BCL_tool_register_app    (116, AR_ACS_BDOT_DELTA_APP);
	}

	if (ACS_Mode(PHOTO_COBC)){
		BCL_tool_register_app    (118, AR_AOCS_PHOTO_COBC_APP);
	}

	BCL_tool_register_app    (120, AR_GLOBAL_ACS_PARAM_SET_APP);

}
void BC_load_orbit_control(void){

	BCL_tool_register_app    (1, AR_GLOBAL_OCS_PARAM_RESET_APP);

	if (OCS_Mode(AVE_DECELERATE)){

		  BCL_tool_register_app    (5, AR_OCS_CROSSPRODUCT_AVE_DECELERATION_APP);
	}

	if (OCS_Mode(MIN_DECELERATE)){

		  BCL_tool_register_app    (15, AR_OCS_CROSSPRODUCT_MIN_DECELERATION_APP);
	}

	if (OCS_Mode(MAX_DECELERATE)){

		  BCL_tool_register_app    (25, AR_OCS_CROSSPRODUCT_MAX_DECELERATION_APP);
	}

	if (OCS_Mode(KEEP_DECELERATE)){

		  BCL_tool_register_app    (35, AR_OCS_CROSSPRODUCT_KEEP_DECELERATION_APP);
	}

	BCL_tool_register_app    (40, AR_GLOBAL_OCS_PARAM_SET_APP);

}
void BC_load_actuator(void){

	BCL_tool_register_app    (5, AR_ACS_MTQA4_APP);

#ifdef TWO_U
	BCL_tool_register_app    (10, AR_ACS_MTQO4_APP);

#endif

//	BCL_tool_prepare_param_uint8(60);
//	BCL_tool_prepare_param_uint8(60);
//	BCL_tool_prepare_param_uint8(60);
//	BCL_tool_register_cmd    (5, Cmd_CODE_ACS_DEBUG_MTQA);
//
//	BCL_tool_prepare_param_uint8(60);
//	BCL_tool_prepare_param_uint8(60);
//	BCL_tool_prepare_param_uint8(60);
//	BCL_tool_register_cmd    (10, Cmd_CODE_ACS_DEBUG_MTQO);
}

void BC_load_aocs(void){
	BCL_tool_register_combine( 1, BC_AOCS_SENSOR_HANDLE);
	BCL_tool_register_combine( 2, BC_ODS);
	BCL_tool_register_combine( 3, BC_ADS);
	BCL_tool_register_combine( 4, BC_ACS);
	BCL_tool_register_combine( 5, BC_OCS);
	BCL_tool_register_combine( 6, BC_ACTUATOR);
}


void BC_load_spin_control_TL(void){
	//sensor
#ifdef TWO_U

//	BCL_tool_register_rotate (1, BC_Pt_ROTATE);
//	BCL_tool_register_rotate (5, BC_MTQ_CURVOLS_ROTATE);
	BCL_tool_prepare_param_uint8(0);
	BCL_tool_register_cmd    (15, Cmd_CODE_ACS_DEBUG_MAGS);

#endif

	BCL_tool_prepare_param_uint8(0);
	BCL_tool_register_cmd    (20, Cmd_CODE_ACS_DEBUG_GYRO);

	BCL_tool_register_app    (25, AR_ACS_GPS_APP);

	BCL_tool_register_app    (30, AR_ACS_SUNS_APP);

	BCL_tool_register_app    (35, AR_ACS_9AX_APP);

	//ODS
	BCL_tool_register_app    (40, AR_ADS_GST_APP);

	if (ODS_Mode(SGPFOUR)){
		BCL_tool_register_app    (45, AR_ODS_SGP4_APP);
	}

	//ADS
		BCL_tool_register_app    (50, AR_ADS_IGRF_APP);

		BCL_tool_register_app    (55, AR_ADS_SUNVECTOR_APP);

//	if (ADS_Mode(TRIAD_CALC)){
		BCL_tool_register_app    (65, AR_ADS_TRIAD_APP);
//	}

	if (ADS_Mode(QMETHOD_CALC)){
		BCL_tool_register_app    (65, AR_ADS_QMETHOD_APP);
	}

	//ACS
	if (ACS_Mode(BDOT_CALC)){
		BCL_tool_register_app    (75, AR_ACS_BDOT_APP);
	}

//		  BCL_tool_register_app    (110, AR_ACS_CROSSPRODUCT_ANGULARMOMENTUM_APP);
		BCL_tool_register_cmd    (75, Cmd_CODE_ACS_CrossProduct_SPIN_HILS);

}
