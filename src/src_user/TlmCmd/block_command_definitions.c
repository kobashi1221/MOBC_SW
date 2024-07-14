#pragma section REPRO
#include "block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/TlmCmd/block_command_table.h>
#include <string.h> // for memcpy
#include "command_definitions.h"

#include "../Settings/Modes/Transitions/sl_initial.h"
#include "../Settings/Modes/Transitions/sl_nop.h"
#include "../Settings/Modes/TaskLists/tl_initial.h"
#include "../Settings/Modes/TaskLists/Elements/tl_elem_tlm_cmd_hirate.h"
#include "../Settings/Modes/TaskLists/Elements/tl_elem_debug_display.h"
#include "../Settings/Modes/TaskLists/Elements/tl_elem_drivers_update.h"
#include "../Settings/Modes/TaskLists/Elements/tl_elem_gs_related_process.h"
#include "./NormalBlockCommandDefinition/nbc_header.h"

/**
 * @brief
 * 各ブロックコマンドIDに中身の初期値をロードしていく
 */
void BC_load_defaults(void)
{
  // Block Cmds for Mode Transition
  BCL_load_bc(BC_SL_STARTUP_TO_STANDBY,                 BCL_load_startup_to_standby);
  BCL_load_bc(BC_SL_STANDBY_TO_STARTUP,    				BCL_load_standby_to_startup);
  BCL_load_bc(BC_SL_TO_STANDBY,    						BCL_load_to_standby);
  BCL_load_bc(BC_SL_TO_MISSION_STANDBY,    				BCL_load_to_mission_standby);
  BCL_load_bc(BC_SL_TO_DAMPING,    						BCL_load_to_damping);
  BCL_load_bc(BC_SL_TO_SUN_POINTING,    				BCL_load_to_sun_pointing);
  BCL_load_bc(BC_SL_TO_EARTH_CENTER_POINTING,    		BCL_load_to_earth_center_pointing);
  BCL_load_bc(BC_SL_TO_GROUND_TRACKING,                 BCL_load_to_ground_tracking);
  BCL_load_bc(BC_SL_TO_INERTIAL_POINTING,               BCL_load_to_inertial_pointing);
  BCL_load_bc(BC_SL_TO_VELOCITY_DIRECTION_POINTING,     BCL_load_to_velocity_direction_pointing);
  BCL_load_bc(BC_SL_TO_HIGH_RATE_SPIN,                  BCL_load_to_hispin);
  BCL_load_bc(BC_SL_TO_OTHER_SATELLITE_POINTING,        BCL_load_to_other_satellite_pointing);
  BCL_load_bc(BC_SL_TO_RELATIVE_ORBITAL_CONTROL,        BCL_load_to_relative_orbital_ontrol);
  BCL_load_bc(BC_SL_TO_CHECK_OUT,    					BCL_load_to_checkout);
  BCL_load_bc(BC_SL_TO_RESERVED1,    					BCL_load_to_reserved1);
  BCL_load_bc(BC_SL_TO_RESERVED2,    					BCL_load_to_reserved2);
  BCL_load_bc(BC_SL_TO_RESERVED3,    					BCL_load_to_reserved3);
  BCL_load_bc(BC_SL_TO_RESERVED4,    					BCL_load_to_reserved4);
  BCL_load_bc(BC_SL_TO_DBG_MODE,    					BCL_load_to_debug);

  // Block Cmds for TaskList
  // こいつは TDSP_initialize() で展開される
  BCL_load_bc(BC_TL_STARTUP,  BCL_load_startup_mode);
  BCL_load_bc(BC_TL_STANDBY,  BCL_load_standby_mode);
  BCL_load_bc(BC_TL_MISSION1, BCL_load_mission1_mode);
  BCL_load_bc(BC_TL_MISSION2, BCL_load_mission2_mode);
  BCL_load_bc(BC_TL_MISSION3, BCL_load_mission3_mode);

  // Block Cmds for Combination
  BCL_load_bc(BC_TLM_CMD_PACKET_HANDLER,  BC_load_tlm_cmd_ph);
  BCL_load_bc(BC_AC_TLM_CMD_DISPATCH,     BCL_load_tlm_cmd_dispatch);

  // Block Cmds for phase
  // Normal Block Cmds
  BCL_load_bc(BC_TLM_GENERATE_AND_SAVE,   BC_load_tlm_generate_and_save);
  BCL_load_bc(BC_READ_SEPCOIL,   		  BC_load_read_sepcoil);
  BCL_load_bc(BC_SEPARATION_ON,      	  BC_load_separation_on);
  BCL_load_bc(BC_SEPARATION_OFF,     	  BC_load_separation_off);
  BCL_load_bc(BC_PWR_BUILT_IN_ADC,     	  BC_load_built_in_ADC);
  BCL_load_bc(BC_OBC_COMM, 			      BC_load_obc_comm);
  BCL_load_bc(BC_PWR_I2C_ADC,     	  	  BC_load_i2c_ADC);
  BCL_load_bc(BC_TLM_EVERY, 			  BC_load_tlm_every);
  BCL_load_bc(BC_TLM_LOW_RATE_1, 		  BC_load_tlm_low_rate_1);
  BCL_load_bc(BC_TLM_LOW_RATE_2, 	      BC_load_tlm_low_rate_2);
  BCL_load_bc(BC_TLM_C2A, 			      BC_load_tlm_C2A);
  // Block Cmds for anomaly
  BCL_load_bc(BC_AH_PWR,     	  	      BC_load_AH_PWR);
  BCL_load_bc(BC_AH_ComPort_RESET,  	  BC_load_AH_ComPort_RESET);
  BCL_load_bc(BC_TLM_ROUTING,		      BC_load_tlm_routing);
  BCL_load_bc(BC_ANOMALY_DETECTION_HANDLER,BC_load_anomaly_detection_handler);
  BCL_load_bc(BC_AH_MODE_AUTO_TRANSITION, BC_load_AH_MODE_Auto_Trans);
  BCL_load_bc(BC_AH_SENSOR,		  		  BC_load_AH_SENSOR);


  BCL_load_bc(BC_PARAM_MEMORY_INIT_CDH,   BC_load_param_init_cdh_com);
  BCL_load_bc(BC_PARAM_MEMORY_INIT_AOCS,  BC_load_param_init_sensor_aocs);
  BCL_load_bc(BC_PARAM_MEMORY_INIT_AOCS2, BC_load_param_init_sensor_aocs2);
  BCL_load_bc(BC_PARAM_MEMORY_LOAD_CDH,   BC_load_param_load_cdh_com);
  BCL_load_bc(BC_PARAM_MEMORY_LOAD_AOCS,  BC_load_param_load_sensor_aocs);
  BCL_load_bc(BC_PARAM_MEMORY_LOAD_AOCS2, BC_load_param_load_sensor_aocs2);
  BCL_load_bc(BC_AFTER_SEP_PARAM_INSTANCE,BC_load_after_sep_param_instance);
  BCL_load_bc(BC_AFTER_SEP_PARAM_DELAY,	  BC_load_after_sep_param_delay);
  BCL_load_bc(BC_AH_AL_FULL,			  BC_load_ah_al_full);
  BCL_load_bc(BC_AH_AL,			  		  BC_load_ah_al);
  BCL_load_bc(BC_AH_AH,			  		  BC_load_ah_ah);
  BCL_load_bc(BC_AH_AH_RES,		  		  BC_load_ah_ah_res);

  //BC VARIABLES1~40までの初期設定
  BCL_load_bc(BC_AOCS_SENSOR_HANDLE,      	BC_load_sensor_handler);
  BCL_load_bc(BC_ADS,	 	 			  	BC_load_attitude_determination);
  BCL_load_bc(BC_ODS,		 			  	BC_load_orbit_determination);
  BCL_load_bc(BC_ACS,		 			  	BC_load_attitude_control);
  BCL_load_bc(BC_OCS,		 			  	BC_load_orbit_control);
  BCL_load_bc(BC_ACTUATOR, 			      	BC_load_actuator);
  BCL_load_bc(BC_Pt_ROTATE, 			  	BC_load_pt_sensor_rotate);
  BCL_load_bc(BC_MTQ_CURVOLS_ROTATE,      	BC_load_mtq_curvols_rotate);
  BCL_load_bc(BC_COBC_LARGE_PACKET_SET,   	BCL_load_cobc_large_packet_set);
  BCL_load_bc(BC_COBC_LARGE_PACKET_RECEIVE, BCL_load_cobc_large_packet_receive);
  BCL_load_bc(BC_SPIN_CONTROL_TL, 		  	BC_load_spin_control_TL);
  BCL_load_bc(BC_MEMORY_PATROL, 		  	BC_load_memory_patrol);
  BCL_load_bc(BC_AOCS, 		              	BC_load_aocs);
  BCL_load_bc(BC_HI_SPIN_TL, 		      	BCL_load_hispin_mode);
  BCL_load_bc(BC_HI_SPIN_SL, 		      	BCL_load_to_hispin);
  BCL_load_bc(BC_TLM_HIGH_SPIN, 		  	BCL_tlm_hispin_mode);
}

#pragma section