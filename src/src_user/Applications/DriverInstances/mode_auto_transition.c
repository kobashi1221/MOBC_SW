#include "mode_auto_transition.h"

#include <string.h> //for memcpy
#include <src_core/System/ModeManager/mode_manager.h>
#include <src_core/TlmCmd/block_command_executor.h>
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/TlmCmd/block_command_table.h>
#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/Settings/Modes/mode_definitions.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Settings/Modes/Phase/phase_setting.h>
#include "block_command_handler.h"
#include <src_user/Library/General/DBG_printf.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

static void CDH_mode_param_load (void);
static void CDH_mode_param_init (void);

static void CDH_mode_auto_transition_init(void);

static MODE_AUTO_TRANSITION mode_trans_driver_;
MODE_AUTO_TRANSITION* const mode_trans_driver = &mode_trans_driver_;

const uint8_t auto_mode_transition_enable_ = 0xff;

//modeBC
//8bitごとに区切る
//|Standby | Mission_standby | Damping | sun_pointing|
const uint32_t phase0_transition_to_ = 0xff000000;////default is standby

const uint32_t phase1_transition_to_ = 0x00ff0000;////default is standby

const uint32_t phase2_transition_to_ = 0x00ff0000;////default is standby

AppInfo CDH_mode_param_update(void)
{
    return AI_create_app_info("MODE_auto_transition_param", CDH_mode_param_load, CDH_mode_param_init);
}

AppInfo CDH_mode_auto_transition(void)
{
    return AI_create_app_info("MODE_auto_transition_param", CDH_mode_auto_transition_init, NULL);
}

static void CDH_mode_param_load (void){
	read_param(&mode_trans_driver->enable, &auto_mode_transition_enable_, 1,(uint32_t)0x000000C4);
	read_param(&mode_trans_driver->phase0_transition_to, &phase0_transition_to_, 4,(uint32_t)0x0000010b);
	read_param(&mode_trans_driver->phase1_transition_to, &phase1_transition_to_, 4,(uint32_t)0x0000010F);
	read_param(&mode_trans_driver->phase2_transition_to, &phase2_transition_to_, 4,(uint32_t)0x00000113);
}


static void CDH_mode_param_init (void){
	write_param(&mode_trans_driver->enable, &auto_mode_transition_enable_, 1,(uint32_t)0x000000C4);
	write_param(&mode_trans_driver->phase0_transition_to, &phase0_transition_to_, 4,(uint32_t)0x0000010b);
	write_param(&mode_trans_driver->phase1_transition_to, &phase1_transition_to_, 4,(uint32_t)0x0000010F);
	write_param(&mode_trans_driver->phase2_transition_to, &phase2_transition_to_, 4,(uint32_t)0x00000113);
}

static void CDH_mode_auto_transition_init(void){
}

CCP_CmdRet Cmd_MODE_AUTO_Transition(const CommonCmdPacket* packet){
	uint8_t transition_flag;
	TCP tcp;
	transition_flag = auto_flag_vote(&mode_trans_driver->enable);
	if(transition_flag == AUTO_TRANSITION_ON){
		MD_MODEID ret, mode_id;
		bct_id_t bc_id;

		switch(p_phase_driver->current_phase){
		case CRITICAL:
			switch(mode_manager->current_id){
			case MD_MODEID_CHECK_OUT:
				ret = modeBC_trans_vote(&mode_trans_driver->phase0_transition_to);//stand byをべた書きでもいいが…
				bc_id = mode_manager->transition_table[mode_manager->current_id][ret];
				if(bc_id == MM_NOT_DEFINED){//遷移先が遷移テーブルにないときは自動遷移しない
					return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
				}

				mode_id = modeBC_trans_vote(&mode_trans_driver->phase0_transition_to);
				CCP_form_rtc(&tcp,(CMD_CODE)Cmd_CODE_MM_START_TRANSITION,&mode_id,1);
				PH_analyze_packet(&tcp);
				break;
			default:
				//何もしない
				break;
			}
			break;
		case INITIAL_OPERATION:
			switch(mode_manager->current_id){
			case MD_MODEID_MISSION_STANDBY:
			case MD_MODEID_CHECK_OUT:
				bc_id = mode_manager->transition_table[mode_manager->current_id][MD_MODEID_STANDBY];
				if(bc_id == MM_NOT_DEFINED){//遷移先が遷移テーブルにないときは自動遷移しない
					return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
				}

				mode_id = MD_MODEID_STANDBY;
				CCP_form_rtc(&tcp,(CMD_CODE)Cmd_CODE_MM_START_TRANSITION,&mode_id,1);
				PH_analyze_packet(&tcp);
				break;
			case MD_MODEID_DAMPING:
			case MD_MODEID_SUN_POINTING:
			case MD_AOCSFLAG_EARTH_CENTER_POINTING:
			case MD_AOCSFLAG_GROUND_TRACKING:
			case MD_AOCSFLAG_INERTIAL_POINTING:
			case MD_AOCSFLAG_VELOCITY_DIRECTION_POINTING:
			case MD_MODEID_HIGH_RATE_SPIN:
				ret = modeBC_trans_vote(&mode_trans_driver->phase1_transition_to);
				bc_id = mode_manager->transition_table[mode_manager->current_id][ret];
				if(bc_id == MM_NOT_DEFINED){//遷移先が遷移テーブルにないときは自動遷移しない
					return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
				}

				mode_id = modeBC_trans_vote(&mode_trans_driver->phase1_transition_to);
				CCP_form_rtc(&tcp,(CMD_CODE)Cmd_CODE_MM_START_TRANSITION,&mode_id,1);
				PH_analyze_packet(&tcp);
				break;
			default:
				//何もしない
				break;
			}
			break;
		case NORMAL_OPERATION:
			switch(mode_manager->current_id){
			case MD_MODEID_MISSION_STANDBY:
				bc_id = mode_manager->transition_table[mode_manager->current_id][MD_MODEID_STANDBY];
				if(bc_id == MM_NOT_DEFINED){//遷移先が遷移テーブルにないときは自動遷移しない
					return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
				}

				MD_MODEID mode_id;
				mode_id = MD_MODEID_STANDBY;
				CCP_form_rtc(&tcp,(CMD_CODE)Cmd_CODE_MM_START_TRANSITION,&mode_id,1);
				PH_analyze_packet(&tcp);
				break;
			case MD_MODEID_DAMPING:
			case MD_MODEID_SUN_POINTING:
			case MD_MODEID_EARTH_CENTER_POINTING:
			case MD_MODEID_GROUND_TRACKING:
			case MD_MODEID_INERTIAL_POINTING:
			case MD_MODEID_VELOCITY_DIRECTION_POINTING:
			case MD_MODEID_HIGH_RATE_SPIN:
			case MD_AOCSFLAG_OTHER_SATELLITE_POINTING:
			case MD_AOCSFLAG_RELATIVE_ORBITAL_CONTROL:
				ret = modeBC_trans_vote(&mode_trans_driver->phase2_transition_to);
				bc_id = mode_manager->transition_table[mode_manager->current_id][ret];
				if(bc_id == MM_NOT_DEFINED){//遷移先が遷移テーブルにないときは自動遷移しない
					return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
				}
				mode_id = modeBC_trans_vote(&mode_trans_driver->phase2_transition_to);
				CCP_form_rtc(&tcp,(CMD_CODE)Cmd_CODE_MM_START_TRANSITION,&mode_id,1);
				PH_analyze_packet(&tcp);
				break;
			default:
				//何もしない
				break;
			}
			break;
		}
	}
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief   自動モード遷移をenable of disableにする
 * param[15]  0x01→enable, 0x00→disable
 *
 */
CCP_CmdRet Cmd_MODE_auto_flag_setting(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);

	if(param[0] == AUTO_TRANSITION_ON){
		Auto_trans_set(&mode_trans_driver->enable);
	}
	else{
		Auto_trans_reset(&mode_trans_driver->enable);
	}
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

