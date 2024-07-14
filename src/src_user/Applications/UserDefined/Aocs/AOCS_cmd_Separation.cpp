/*
 * AOCS_cmd_EKF.h
 *
 *  ver.1.0 2022/07/16 sakaguchi
 *	分離判断
 */
#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Library/General/DBG_printf.h>
#include "src_user/TlmCmd/block_command_definitions.h"
#include <src_user/Library/Delay.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Drivers/Power/PWR_Operation.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/System/TimeManager/obc_time.h>

static SEP_STRUCT sep_driver;
SEP_STRUCT* const p_sep_driver = &sep_driver;

static void separation_load_param(void);
static void separation_initialize_param(void);

static void separation_flag_load_param(void);
static void separation_flag_initialize_param(void);

const int16_t judgement_max_time_ = 200;
const uint16_t error_check_time_ = 10;
const uint16_t error_check_diff_time_ = 15;
const uint8_t separation_go_from_ground_station_flag_ = 0x00;
const uint8_t separation_local_order_ = 0;
const uint8_t separation_interval1_ = 0;
const uint8_t separation_interval2_ = 0;
const uint8_t separation_interval3_ = 0;
const uint8_t modeBC_transition_to_ = 5;

const uint8_t separation_redundancy1_ = 0x00;
const uint8_t separation_redundancy2_ = 0x00;
const uint8_t separation_redundancy3_ = 0x00;

const uint8_t separate_checker_flag_ = 0x00;
const uint8_t rotation_speed_checker_ = 0xff;
const uint8_t rotation_direction_b_checker_ = 0xff;
const uint8_t attitude_accuracy_checker_ = 0xff;
const uint8_t rotation_direction_i_checker_ = 0xff;
const uint8_t separate_direction_checker_ = 0xff;
const uint8_t angular_velocity_checker_ = 0xff;
const uint8_t rotation_direction_v_checker_ = 0xff;
const uint8_t rotation_phase_checker_ = 0xff;

const uint16_t param_load_delay_ = 5;

void separation_GO(void);

AppInfo Separation_param_update(void)
{
	return AI_create_app_info("SEP_PARAM_UPDATE", separation_load_param, separation_initialize_param);
}

//これはBC_PARAM_MEMORY_INIT_AOCS及びBC_PARAM_MEMORY_LOAD_AOCSには加えない
AppInfo Separation_flag_param_update(void)
{
	return AI_create_app_info("SEPFLAG_PARAM_UPDATE", separation_flag_load_param, separation_flag_initialize_param);
}

static void separation_load_param(void)
{
	read_param(&p_sep_driver->sep_param.judgement_max_time, &judgement_max_time_, 2, (uint32_t)0x000000EE);
	read_param(&p_sep_driver->sep_param.error_check_time, &error_check_time_, 2, (uint32_t)0x000000F0);
	read_param(&p_sep_driver->sep_param.error_check_diff_time, &error_check_diff_time_, 2, (uint32_t)0x000000F2);
	read_param(&p_sep_driver->sep_param.separation_go_from_ground_station_flag, &separation_go_from_ground_station_flag_, 1, (uint32_t)0x000000F4);
	read_param(&p_sep_driver->sep_param.separation_local_order, &separation_local_order_, 1, (uint32_t)0x000000F5);
	read_param(&p_sep_driver->sep_param.separation_interval1, &separation_interval1_, 1, (uint32_t)0x000000F6);
	read_param(&p_sep_driver->sep_param.separation_interval2, &separation_interval2_, 1, (uint32_t)0x000000F7);
	read_param(&p_sep_driver->sep_param.separation_interval3, &separation_interval3_, 1, (uint32_t)0x000000F8);
	read_param(&p_sep_driver->sep_param.modeBC_transition_to, &modeBC_transition_to_, 1, (uint32_t)0x000000FC);
	read_param(&p_sep_driver->sep_condition_check.separate_checker_flag, &separate_checker_flag_, 1, (uint32_t)0x000000FD);
	read_param(&p_sep_driver->sep_condition_check.rotation_speed_checker, &rotation_speed_checker_, 1, (uint32_t)0x000000FE);
	read_param(&p_sep_driver->sep_condition_check.rotation_direction_b_checker, &rotation_direction_b_checker_, 1, (uint32_t)0x000000FF);
	read_param(&p_sep_driver->sep_condition_check.attitude_accuracy_checker, &attitude_accuracy_checker_, 1, (uint32_t)0x00000100);
	read_param(&p_sep_driver->sep_condition_check.rotation_direction_i_checker, &rotation_direction_i_checker_, 1, (uint32_t)0x00000101);
	read_param(&p_sep_driver->sep_condition_check.separate_direction_checker, &separate_direction_checker_, 1, (uint32_t)0x00000102);
	read_param(&p_sep_driver->sep_condition_check.angular_velocity_checker, &angular_velocity_checker_, 1, (uint32_t)0x00000103);
	read_param(&p_sep_driver->sep_condition_check.rotation_direction_v_checker, &rotation_direction_v_checker_, 1, (uint32_t)0x00000104);
	read_param(&p_sep_driver->sep_condition_check.rotation_phase_checker, &rotation_phase_checker_, 1, (uint32_t)0x00000105);
	read_param(&p_sep_driver->sep_param.param_load_delay, &param_load_delay_, 2, (uint32_t)0x00000109);
}

static void separation_initialize_param(void)
{
	write_param(&p_sep_driver->sep_param.judgement_max_time, &judgement_max_time_, 2, (uint32_t)0x000000EE);
	write_param(&p_sep_driver->sep_param.error_check_time, &error_check_time_, 2, (uint32_t)0x000000F0);
	write_param(&p_sep_driver->sep_param.error_check_diff_time, &error_check_diff_time_, 2, (uint32_t)0x000000F2);
	write_param(&p_sep_driver->sep_param.separation_go_from_ground_station_flag, &separation_go_from_ground_station_flag_, 1, (uint32_t)0x000000F4);
	write_param(&p_sep_driver->sep_param.separation_local_order, &separation_local_order_, 1, (uint32_t)0x000000F5);
	write_param(&p_sep_driver->sep_param.separation_interval1, &separation_interval1_, 1, (uint32_t)0x000000F6);
	write_param(&p_sep_driver->sep_param.separation_interval2, &separation_interval2_, 1, (uint32_t)0x000000F7);
	write_param(&p_sep_driver->sep_param.separation_interval3, &separation_interval3_, 1, (uint32_t)0x000000F8);
	write_param(&p_sep_driver->sep_param.modeBC_transition_to, &modeBC_transition_to_, 1, (uint32_t)0x000000FC);
	write_param(&p_sep_driver->sep_condition_check.separate_checker_flag, &separate_checker_flag_, 1, (uint32_t)0x000000FD);
	write_param(&p_sep_driver->sep_condition_check.rotation_speed_checker, &rotation_speed_checker_, 1, (uint32_t)0x000000FE);
	write_param(&p_sep_driver->sep_condition_check.rotation_direction_b_checker, &rotation_direction_b_checker_, 1, (uint32_t)0x000000FF);
	write_param(&p_sep_driver->sep_condition_check.attitude_accuracy_checker, &attitude_accuracy_checker_, 1, (uint32_t)0x00000100);
	write_param(&p_sep_driver->sep_condition_check.rotation_direction_i_checker, &rotation_direction_i_checker_, 1, (uint32_t)0x00000101);
	write_param(&p_sep_driver->sep_condition_check.separate_direction_checker, &separate_direction_checker_, 1, (uint32_t)0x00000102);
	write_param(&p_sep_driver->sep_condition_check.angular_velocity_checker, &angular_velocity_checker_, 1, (uint32_t)0x00000103);
	write_param(&p_sep_driver->sep_condition_check.rotation_direction_v_checker, &rotation_direction_v_checker_, 1, (uint32_t)0x00000104);
	write_param(&p_sep_driver->sep_condition_check.rotation_phase_checker, &rotation_phase_checker_, 1, (uint32_t)0x00000105);
	write_param(&p_sep_driver->sep_param.param_load_delay, &param_load_delay_, 2, (uint32_t)0x00000109);
}

static void separation_flag_load_param(void){
	read_param(&p_sep_driver->sep_critcal.sep_redundancy1, &separation_interval1_, 1, (uint32_t)0x000000F9);
	read_param(&p_sep_driver->sep_critcal.sep_redundancy2, &separation_interval2_, 1, (uint32_t)0x000000FA);
	read_param(&p_sep_driver->sep_critcal.sep_redundancy3, &separation_interval3_, 1, (uint32_t)0x000000FB);

	Judge_sep_flag();
}

static void separation_flag_initialize_param(void){
	write_param(&p_sep_driver->sep_critcal.sep_redundancy1, &separation_interval1_, 1, (uint32_t)0x000000F9);
	write_param(&p_sep_driver->sep_critcal.sep_redundancy2, &separation_interval2_, 1, (uint32_t)0x000000FA);
	write_param(&p_sep_driver->sep_critcal.sep_redundancy3, &separation_interval3_, 1, (uint32_t)0x000000FB);
}

void separation_judgement(int16_t flag){
	ObcTime at_now=TMGR_get_master_clock();
	if(flag < 0){
		p_sep_driver->sep_param.judging_flag = 0;
		return;
	}
	for(int i = 0; i < p_sep_driver->sep_param.judgement_max_time; i++){

		p_sep_driver->sep_param.judging_flag = 1;

		if(flag == i){
			separation_GO();
			Sep_flag_set(&p_sep_driver->sep_critcal.separated_flag);

			TCP rtc, tlc;
			// 高速スピンダウン遷移時 param load(instance)
			uint8_t bc_id_rtc[2];
			bc_id_rtc[0] = (uint8_t)(BC_AFTER_SEP_PARAM_INSTANCE >> 8);
			bc_id_rtc[1] = (uint8_t)(BC_AFTER_SEP_PARAM_INSTANCE);
			CCP_form_rtc(&rtc,(CMD_CODE)Cmd_CODE_BCE_COMBINE_BLOCK,&bc_id_rtc[0],2);
			PH_analyze_packet(&rtc);

			// 高速スピンダウン遷移時 param load(delay)
			uint8_t tlc_param[3];
			cycle_t now_cycle = TMGR_get_master_total_cycle();
			cycle_t load_cycle = now_cycle + p_sep_driver->sep_param.param_load_delay;
			tlc_param[0] = 0;
			tlc_param[1] = (uint8_t)(BC_AFTER_SEP_PARAM_DELAY >> 8);
			tlc_param[2] = (uint8_t)(BC_AFTER_SEP_PARAM_DELAY);
			CCP_form_tlc(&tlc, load_cycle, (CMD_CODE)Cmd_CODE_TLCD_DEPLOY_BLOCK,&tlc_param[0],3);
			PH_analyze_packet(&tlc);

			p_sep_driver->sep_param.separation_go_from_ground_station_flag = NOGO;
			break;
		}
		c2a_delay_ms(0);
		if((i % error_check_time_) ==0){//error_check_time秒ごとに時間のズレを確認
			ObcTime now=TMGR_get_master_clock();
			int ret = separation_error_checker(&at_now, &now);//TODO ADDアノマリ追加
			if(ret == true){
				//trueなら時間を更新して走査を続行
				at_now=TMGR_get_master_clock();
			}
			else{
				//boolなら走査を打ち切り
				break;
			}
		}
	}
}


//色々考えたがユーザー的には列挙したほうがわかりやすい
void separation_GO(void){//check ここのテレメトリは必要?
	switch(p_sep_driver->sep_param.separation_local_order){
	case SEP_1_2_3_4:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
	    break;
	case SEP_1_2_4_3:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
	    break;
	case SEP_1_3_2_4:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
	    break;
	case SEP_1_3_4_2:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
	    break;
	case SEP_1_4_3_2:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
	    break;
	case SEP_2_1_3_4:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
	    break;
	case SEP_2_1_4_3:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
	    break;
	case SEP_2_3_1_4:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
	    break;
	case SEP_2_3_4_1:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
	    break;
	case SEP_2_4_1_3:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
	    break;
	case SEP_2_4_3_1:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
	    break;
	case SEP_3_1_2_4:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
	    break;
	case SEP_3_1_4_2:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
	    break;
	case SEP_3_2_1_4:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
	    break;
	case SEP_3_2_4_1:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
	    break;
	case SEP_3_4_1_2:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
	    break;
	case SEP_3_4_2_1:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
	    break;
	case SEP_4_1_2_3:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
	    break;

	case SEP_4_1_3_2:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
	    break;
	case SEP_4_2_1_3:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
	    break;
	case SEP_4_2_3_1:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
	    break;
	case SEP_4_3_1_2:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
	    break;
	case SEP_4_3_2_1:
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval1);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval2);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,1);
		c2a_delay_ms(p_sep_driver->sep_param.separation_interval3);
	    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,1);
	    break;
	default:
		break;
	}
}

bool separation_error_checker(ObcTime* before, ObcTime* after){
	unsigned int diff_time = OBCT_diff_in_msec(before, after);
	if(diff_time < p_sep_driver->sep_param.error_check_diff_time){
		return true;
	}
	else{
		return false;
	}
}

//フラグを見て分離しているか分離していないかを判断する
//多数決システム
uint8_t Separated(){
	uint8_t ret=0;
	for(int i=0;i<8;i++){
		ret+= ((p_sep_driver->sep_critcal.separated_flag >> i) & 0x01);
	}
	if (ret>=4) {
		Sep_flag_set(&p_sep_driver->sep_critcal.separated_flag);
		return SEPARATED;
	}
	else {
		Sep_flag_reset(&p_sep_driver->sep_critcal.separated_flag);
		return NOT_SEPARATED;
	}
}

/////////////////////////////以下, 分離に関するローカル関数 /////////////////////////////

//フラグを多数決して, 最も多いところに遷移させる
MD_MODEID TL_trans_vote(uint32_t* mode){
	uint8_t ret_standby=0;
	uint8_t ret_mission1=0;
	uint8_t ret_mission2=0;
	uint8_t ret_mission3=0;
	for(int i = 0;i<8;i++){
		ret_standby+=((*mode >> i) &0x01);
	}
	for(int i = 8;i<16;i++){
		ret_mission1+=((*mode >> i) &0x01);
	}
	for(int i = 16;i<24;i++){
		ret_mission2+=((*mode >> i) &0x01);
	}
	for(int i = 24;i<32;i++){
		ret_mission3+=((*mode >> i) &0x01);
	}

	if(ret_standby > ret_mission1 && ret_standby > ret_mission2 && ret_standby > ret_mission3){
		return MD_MODEID_STANDBY;
	}
//	else if(ret_mission1 > ret_standby && ret_mission1 > ret_mission2 && ret_mission1 > ret_mission3){
//		return MD_MODEID_MISSION1;
//	}
//	else if(ret_mission2 > ret_standby && ret_mission2 > ret_mission1 && ret_mission2 > ret_mission3){
//		return MD_MODEID_MISSION2;
//	}
//	else if(ret_mission3 > ret_standby && ret_mission3 > ret_mission1 && ret_mission3 > ret_mission2){
//		return MD_MODEID_MISSION3;
//	}
	else{//どれにも属さないときはStandby
		return MD_MODEID_STANDBY;
	}
}

void Judge_sep_flag(void){
	int ret = 0;
	ret += vote_sep_flag(&p_sep_driver->sep_critcal.sep_redundancy1);
	ret += vote_sep_flag(&p_sep_driver->sep_critcal.sep_redundancy2);
	ret += vote_sep_flag(&p_sep_driver->sep_critcal.sep_redundancy3);

	if(ret >= 2){
		Sep_flag_set(&p_sep_driver->sep_critcal.separated_flag);
	}
	else{
		Sep_flag_reset(&p_sep_driver->sep_critcal.separated_flag);
	}
}

SEP_GONOGO vote_sep_gonogo(uint8_t* flag){
	uint8_t ret=0;
	for(int i=0;i<8;i++){
		ret+= ((*flag >> i) & 0x01);
	}
	if (ret>=4) {
		*flag = 0xff;
		return GO;
	}
	else {
		*flag = 0x00;
		return NOGO;
	}
}

//多数決システム
uint8_t vote_sep_flag(uint8_t* flag){
	uint8_t ret=0;
	for(int i=0;i<8;i++){
		ret+= ((*flag >> i) & 0x01);
	}
	if (ret>=4) {
		*flag = 0xff;
		return SEPARATED;
	}
	else {
		*flag = 0x00;
		return NOT_SEPARATED;
	}
}

void Sep_flag_set(uint8_t* flag){
	*flag = 0xff;
}

void Sep_flag_reset(uint8_t *flag){
	*flag = 0x00;
}
