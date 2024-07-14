/*
 * ADS_cmd_BBdot.h
 *
 *  Created on: 2022/06/13
 *      Author: Inakawa
 */

#ifndef C2A_APPLICATIONS_DRIVERINSTANCES_ADS_ADS_CMD_BBDOT_H_
#define C2A_APPLICATIONS_DRIVERINSTANCES_ADS_ADS_CMD_BBDOT_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

//磁場変化ベクトルの履歴最大要素数
#define HISTMAX (10)

typedef enum
{
	diff,
	propagate
} BBDOT_MODE;

typedef struct
{
	float DCM[3][3];
	float quaternion[4];
	float b_obs_prev[3];
	float b_est_prev[3];
	float omega_prev[3];
	int initflg;
	BBDOT_MODE BBdot_mode;  //0:diff 1:propagate

	float bdot_obs_vec_hist[HISTMAX][3];
	int hist_num;

	ObcTime PreTime;

	float Time_threshold;

} BBDOT_STRUCT;

extern const BBDOT_STRUCT* BBDOT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo BBdot_param_update(void);
AppInfo BBdot_update(void);

CCP_EXEC_STS Cmd_ADS_BBdot_HILS(const CommonCmdPacket* packet);

#ifdef __cplusplus
}
#endif



#endif /* C2A_APPLICATIONS_DRIVERINSTANCES_ADS_ADS_CMD_BBDOT_H_ */
