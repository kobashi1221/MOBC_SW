/*
 * AOCS_cmd_EKF.h
 *
 *  ver.1.0 2022/07/03 tamura
 *
 */

#ifndef C2A_APPLICATIONS_USERDEFINED_AOCS_CMD_EKF_H_
#define C2A_APPLICATIONS_USERDEFINED_AOCS_CMD_EKF_H_


#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <stdint.h> //for uint8_t

typedef struct
{
	uint8_t ekf_order[2]; // EKFの順番
	uint8_t previous_ekf_order[2]; // 前回までのEKFの順番
	uint8_t number_of_serials; // 直列のうち構造体に格納されるEKF指定

	float Obs[16]; // 観測値

	//-----事前推定値 a priori estimate------------
	float X_ref_pre[16];
	float P_pre_diag[15];
	//-----------------------

	//----事後推定値  a posteriori estimate-----------
	float X_ref_post[16];
	float P_post_diag[15];
	//-----------------------

	float quaternion[4];
	float omega[3];
	float gyro_bias[3];
	float mags_bias[3];
	float rmm[3];

	uint8_t ekf_valid; // カルマンフィルタ有効フラグ
	uint8_t dq_valid; // 微小クォータニオン有効フラグ
	uint8_t divg_valid; // カルマンフィルタの発散確認フラグ

	float y[15]; // EKFのイノベーション過程

	int16_t sep_time; // プロパゲート分離タイミング [ms]　負の値なら分離条件未達成

	uint8_t eclipse_switch_flag; // 蝕前後の観測値切り替えフラグ


} EKF_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

AppInfo EKF_update(void);
AppInfo EKF_param_update(void);


#ifdef __cplusplus
}
#endif



#endif /* C2A_APPLICATIONS_USERDEFINED_AOCS_CMD_EKF_H_ */
