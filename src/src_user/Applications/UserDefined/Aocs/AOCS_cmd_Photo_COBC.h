/*
 * AOCS_cmd_Photo_COBC.h
 *
 *  ver.1.0 2022/08/12 Tamura
 *
 */

#ifndef C2A_APPLICATIONS_USERDEFINED_AOCS_CMD_PHOTO_COBC_H_
#define C2A_APPLICATIONS_USERDEFINED_AOCS_CMD_PHOTO_COBC_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_core/TlmCmd/packet_handler.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Settings/Modes/mode_definitions.h>

#ifdef TWO_U

 typedef struct{
	 uint8_t time_checker;
	 uint8_t direction_err_checker;
	 uint8_t omega_checker;
	 uint16_t counter;
	 uint8_t condition[3];
	 uint8_t photo_OK_flag;
	 uint16_t time_limit;	// SCAM撮像からMCAM撮像までの時間上限[s]
	 float optical_axis_dir[3];	// 光軸方向
	 float photo_dir_err;	// 光軸方向と目標方向の許容角度誤差[deg]
	 float photo_omega_X_min;
	 float photo_omega_X_max;
	 float photo_omega_Y_min;
	 float photo_omega_Y_max;
	 float photo_omega_Z_min;
	 float photo_omega_Z_max;
 }PHOTO_STRUCT;

 extern PHOTO_STRUCT* const p_photo_driver;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo photo_cobc_param_update(void);
AppInfo photo_cobc_update(void);
AppInfo COBC_Photo_Flag_Generator(void);

void COBC_Photo_flag_send(void);

#ifdef __cplusplus
}
#endif

#endif/*for 2U*/

#endif /* C2A_APPLICATIONS_USERDEFINED_AOCS_CMD_PHOTO_COBC_H_ */
