/*
 * AOCS_cmd_EKF.h
 *
 *  ver.1.0 2022/07/16 sakaguchi
 *
 */

#ifndef C2A_APPLICATIONS_USERDEFINED_AOCS_CMD_SEPARATION_H_
#define C2A_APPLICATIONS_USERDEFINED_AOCS_CMD_SEPARATION_H_


#ifdef __cplusplus
 extern "C" {
#endif

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Settings/Modes/mode_definitions.h>

typedef enum{
	SEP_1_2_3_4,
	SEP_1_2_4_3,
	SEP_1_3_2_4,
	SEP_1_3_4_2,
	SEP_1_4_2_3,
	SEP_1_4_3_2,
	SEP_2_1_3_4,
	SEP_2_1_4_3,
	SEP_2_3_1_4,
	SEP_2_3_4_1,
	SEP_2_4_1_3,
	SEP_2_4_3_1,
	SEP_3_1_2_4,
	SEP_3_1_4_2,
	SEP_3_2_1_4,
	SEP_3_2_4_1,
	SEP_3_4_1_2,
	SEP_3_4_2_1,
	SEP_4_1_2_3,
	SEP_4_1_3_2,
	SEP_4_2_1_3,
	SEP_4_2_3_1,
	SEP_4_3_1_2,
	SEP_4_3_2_1,
}SEP_ORDER;

typedef enum{
	NOT_SEPARATED,
	SEPARATED
}SEP_NUM;

typedef enum{
	NOGO,
	GO
}SEP_GONOGO;

typedef struct{
	struct{
		uint8_t separate_checker_flag;
		uint8_t rotation_speed_checker;
		uint8_t rotation_direction_b_checker;
		uint8_t attitude_accuracy_checker;
		uint8_t rotation_direction_i_checker;
		uint8_t separate_direction_checker;
		uint8_t angular_velocity_checker;
		uint8_t rotation_direction_v_checker;
		uint8_t rotation_phase_checker;
	}sep_condition_check;
	struct{
		// Separation time after starting propagation
		int16_t sep_time;

		//冗長用(二つ以上, 0xffならseparated_flagが立つ)
		uint8_t sep_redundancy1;
		uint8_t sep_redundancy2;
		uint8_t sep_redundancy3;

		//フラグの本体
		//これがONなら分離した判定
		uint8_t separated_flag;
	}sep_critcal;
	struct{
		int16_t judgement_max_time;
		uint16_t error_check_time;
		uint16_t error_check_diff_time;
		uint8_t separation_go_from_ground_station_flag;
		uint8_t separation_local_order;
		uint8_t separation_interval1;
		uint8_t separation_interval2;
		uint8_t separation_interval3;
		uint32_t modeBC_transition_to;
		uint16_t param_load_delay;
		uint8_t judging_flag;
	}sep_param;
}SEP_STRUCT;

extern SEP_STRUCT* const p_sep_driver;


AppInfo Separation_param_update(void);
AppInfo Separation_flag_param_update(void);

void separation_judgement(int16_t flag);
bool separation_error_checker(ObcTime* before, ObcTime* after);

MD_MODEID TL_trans_vote(uint32_t* mode);
void Judge_sep_flag();
uint8_t Separated();
uint8_t vote_sep_flag(uint8_t* flag);
SEP_GONOGO vote_sep_gonogo(uint8_t* flag);
void Sep_flag_set(uint8_t* flag);
void Sep_flag_reset(uint8_t *flag);

#ifdef __cplusplus
}
#endif



#endif /* C2A_APPLICATIONS_USERDEFINED_AOCS_CMD_SEPARATION_H_ */
