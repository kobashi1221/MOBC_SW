#pragma section REPRO
/**
 * @file
 * @brief モード定義とモード遷移定義
 */
#include "mode_definitions.h"

#include <src_core/System/ModeManager/mode_manager.h>

#include "../../TlmCmd/block_command_definitions.h"
#include <src_user/Settings/Modes/Phase/phase_setting.h>


// モードごとのTLを指定
void MD_load_mode_list(void)
{
  MM_set_mode_list(MD_MODEID_START_UP,                       BC_TL_STARTUP);
  MM_set_mode_list(MD_MODEID_STANDBY,                        BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_MISSION_STANDBY,                BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_DAMPING,                        BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_SUN_POINTING,                   BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_EARTH_CENTER_POINTING,          BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_GROUND_TRACKING,                BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_INERTIAL_POINTING,              BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_VELOCITY_DIRECTION_POINTING,    BC_TL_STANDBY);

  MM_set_mode_list(MD_MODEID_HIGH_RATE_SPIN,                 BC_TL_MISSION1);
  MM_set_mode_list(MD_MODEID_OTHER_SATELLITE_POINTING,       BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_RELATIVE_ORBITAL_CONTROL,       BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_CHECK_OUT,                      BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_RESERVED1,                      BC_TL_STANDBY);
  MM_set_mode_list(MD_MODEID_RESERVED2,                      BC_TL_MISSION1);
  MM_set_mode_list(MD_MODEID_RESERVED3,                      BC_TL_MISSION2);
  MM_set_mode_list(MD_MODEID_RESERVED4,                      BC_TL_MISSION3);
  MM_set_mode_list(MD_MODEID_DBG,                            BC_TL_STANDBY);
}

void MD_load_transition_table(void)
{
  printf("PHASE No.%d \r\n",p_phase_driver->current_phase);
  if(p_phase_driver->current_phase == CRITICAL){
		//to StartUP
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_START_UP, BC_SL_STANDBY_TO_STARTUP);

		//to STANDBY
	    MM_set_transition_table(MD_MODEID_START_UP, 					MD_MODEID_STANDBY, 	BC_SL_STARTUP_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_STANDBY, 	BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_STANDBY, 	BC_SL_TO_STANDBY);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_DBG, 							MD_MODEID_STANDBY, 	BC_SL_TO_STANDBY);

		//to Checkout
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);


/////////////////////////////////// 以下, 冗長MODE　/////////////////////////////////////////
		//to reserved1
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);

		//to reserved2
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);

		//to reserved3
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);

		//to reserved4
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);

		//to DBG
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_DBG, 		  BC_SL_TO_DBG_MODE);
	    MM_set_transition_table(MD_MODEID_DBG, 							MD_MODEID_DBG,  	  BC_SL_TO_DBG_MODE);
/////////////////////////////////////////////////////////////////////////////////////////
	}

	else if(p_phase_driver->current_phase == INITIAL_OPERATION){
		//to StartUP
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_START_UP, BC_SL_STANDBY_TO_STARTUP);

		//to STANDBY
	    MM_set_transition_table(MD_MODEID_START_UP, 					MD_MODEID_STANDBY, 	BC_SL_STARTUP_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_STANDBY, 	BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_STANDBY, 	BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_STANDBY, 	BC_SL_TO_STANDBY);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_DBG, 							MD_MODEID_STANDBY, 	BC_SL_TO_STANDBY);

	    //to MISSION STANDBY
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_DAMPING, 						MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_SUN_POINTING, 				MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_EARTH_CENTER_POINTING, 		MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_GROUND_TRACKING, 			    MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_INERTIAL_POINTING, 	        MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
        MM_set_transition_table(MD_MODEID_VELOCITY_DIRECTION_POINTING,  MD_MODEID_MISSION_STANDBY,  BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN, 				MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_MISSION_STANDBY,  BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_MISSION_STANDBY,  BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_MISSION_STANDBY,  BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_MISSION_STANDBY,  BC_SL_TO_MISSION_STANDBY);

	    //to Damping
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_DAMPING, 			BC_SL_TO_DAMPING);
	    MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN, 				MD_MODEID_DAMPING, 			BC_SL_TO_DAMPING);
        MM_set_transition_table(MD_MODEID_SUN_POINTING,                 MD_MODEID_DAMPING,          BC_SL_TO_DAMPING);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_DAMPING, 			BC_SL_TO_DAMPING);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_DAMPING,  		BC_SL_TO_DAMPING);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_DAMPING,  		BC_SL_TO_DAMPING);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_DAMPING,  		BC_SL_TO_DAMPING);

	    //to sun pointing
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);
        MM_set_transition_table(MD_MODEID_DAMPING,                      MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);

		//to earth center pointing
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_EARTH_CENTER_POINTING,  BC_SL_TO_EARTH_CENTER_POINTING);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_EARTH_CENTER_POINTING,  BC_SL_TO_EARTH_CENTER_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_EARTH_CENTER_POINTING,  BC_SL_TO_EARTH_CENTER_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_EARTH_CENTER_POINTING,  BC_SL_TO_EARTH_CENTER_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_EARTH_CENTER_POINTING,  BC_SL_TO_EARTH_CENTER_POINTING);

		//to ground tracking
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_GROUND_TRACKING,  BC_SL_TO_GROUND_TRACKING);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_GROUND_TRACKING,  BC_SL_TO_GROUND_TRACKING);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_GROUND_TRACKING,  BC_SL_TO_GROUND_TRACKING);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_GROUND_TRACKING,  BC_SL_TO_GROUND_TRACKING);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_GROUND_TRACKING,  BC_SL_TO_GROUND_TRACKING);

		//to inertial pointing
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_INERTIAL_POINTING,  BC_SL_TO_INERTIAL_POINTING);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_INERTIAL_POINTING,  BC_SL_TO_INERTIAL_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_INERTIAL_POINTING,  BC_SL_TO_INERTIAL_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_INERTIAL_POINTING,  BC_SL_TO_INERTIAL_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_INERTIAL_POINTING,  BC_SL_TO_INERTIAL_POINTING);

        //to velocity direction pointing
        MM_set_transition_table(MD_MODEID_MISSION_STANDBY,              MD_MODEID_VELOCITY_DIRECTION_POINTING,  BC_SL_TO_VELOCITY_DIRECTION_POINTING);

        MM_set_transition_table(MD_MODEID_RESERVED1,                    MD_MODEID_VELOCITY_DIRECTION_POINTING,  BC_SL_TO_VELOCITY_DIRECTION_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED2,                    MD_MODEID_VELOCITY_DIRECTION_POINTING,  BC_SL_TO_VELOCITY_DIRECTION_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED3,                    MD_MODEID_VELOCITY_DIRECTION_POINTING,  BC_SL_TO_VELOCITY_DIRECTION_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED4,                    MD_MODEID_VELOCITY_DIRECTION_POINTING,  BC_SL_TO_VELOCITY_DIRECTION_POINTING);

		//to high spin
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_HIGH_RATE_SPIN,  BC_SL_TO_HIGH_RATE_SPIN);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_HIGH_RATE_SPIN,  BC_SL_TO_HIGH_RATE_SPIN);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_HIGH_RATE_SPIN,  BC_SL_TO_HIGH_RATE_SPIN);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_HIGH_RATE_SPIN,  BC_SL_TO_HIGH_RATE_SPIN);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_HIGH_RATE_SPIN,  BC_SL_TO_HIGH_RATE_SPIN);

		//to check out
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_CHECK_OUT,  BC_SL_TO_CHECK_OUT);

/////////////////////////////////// 以下, 冗長MODE　/////////////////////////////////////////
		//to reserved1
		MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_DAMPING, 						MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_SUN_POINTING, 				MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_EARTH_CENTER_POINTING, 	    MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_GROUND_TRACKING, 			    MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_INERTIAL_POINTING, 	        MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_VELOCITY_DIRECTION_POINTING,  MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN, 				MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
		MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
		MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
		MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
		MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);

		//to reserved2
		MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_DAMPING, 						MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_SUN_POINTING, 				MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_EARTH_CENTER_POINTING, 		MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_GROUND_TRACKING, 			    MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_INERTIAL_POINTING, 	        MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_VELOCITY_DIRECTION_POINTING,  MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN, 				MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
		MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
		MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
		MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
		MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);

		//to reserved3
		MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_DAMPING, 						MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_SUN_POINTING, 				MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_EARTH_CENTER_POINTING, 		MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_GROUND_TRACKING, 			    MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_INERTIAL_POINTING, 	        MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_VELOCITY_DIRECTION_POINTING,  MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN, 				MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
		MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
		MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
		MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
		MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);

		//to reserved4
		MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_DAMPING, 						MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_SUN_POINTING, 				MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_EARTH_CENTER_POINTING, 		MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_GROUND_TRACKING, 			    MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_INERTIAL_POINTING, 	        MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_VELOCITY_DIRECTION_POINTING,  MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN, 				MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
	    MM_set_transition_table(MD_MODEID_CHECK_OUT, 					MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
		MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
		MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
		MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
		MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);

		//to DBG
		MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_DBG, 		  BC_SL_TO_DBG_MODE);
		MM_set_transition_table(MD_MODEID_DBG, 							MD_MODEID_DBG,  	  BC_SL_TO_DBG_MODE);
/////////////////////////////////////////////////////////////////////////////////////////
	}

	else if(p_phase_driver->current_phase == NORMAL_OPERATION){
		//to StartUP
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_START_UP, BC_SL_STANDBY_TO_STARTUP);

		//to STANDBY
	    MM_set_transition_table(MD_MODEID_START_UP, 					MD_MODEID_STANDBY, 	BC_SL_STARTUP_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_STANDBY, 	BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_STANDBY, 	BC_SL_TO_STANDBY);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_STANDBY,  BC_SL_TO_STANDBY);
	    MM_set_transition_table(MD_MODEID_DBG, 							MD_MODEID_STANDBY, 	BC_SL_TO_STANDBY);

	    //to MISSION STANDBY
	    MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_DAMPING, 						MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_SUN_POINTING, 				MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_EARTH_CENTER_POINTING, 		MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_GROUND_TRACKING, 			    MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_INERTIAL_POINTING, 	        MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
        MM_set_transition_table(MD_MODEID_VELOCITY_DIRECTION_POINTING,  MD_MODEID_MISSION_STANDBY,  BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN, 				MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_OTHER_SATELLITE_POINTING,     MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_RELATIVE_ORBITAL_CONTROL,     MD_MODEID_MISSION_STANDBY, 	BC_SL_TO_MISSION_STANDBY);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_MISSION_STANDBY,  BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_MISSION_STANDBY,  BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_MISSION_STANDBY,  BC_SL_TO_MISSION_STANDBY);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_MISSION_STANDBY,  BC_SL_TO_MISSION_STANDBY);

	    //to Damping
        MM_set_transition_table(MD_MODEID_MISSION_STANDBY,              MD_MODEID_DAMPING,          BC_SL_TO_DAMPING);
        MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN,               MD_MODEID_DAMPING,          BC_SL_TO_DAMPING);
        MM_set_transition_table(MD_MODEID_SUN_POINTING,                 MD_MODEID_DAMPING,          BC_SL_TO_DAMPING);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_DAMPING,  		BC_SL_TO_DAMPING);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_DAMPING,  		BC_SL_TO_DAMPING);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_DAMPING,  		BC_SL_TO_DAMPING);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_DAMPING,  		BC_SL_TO_DAMPING);

	    //to sun pointing
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);
        MM_set_transition_table(MD_MODEID_DAMPING,                      MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_SUN_POINTING,  BC_SL_TO_SUN_POINTING);

        //to earth center pointing
        MM_set_transition_table(MD_MODEID_MISSION_STANDBY,              MD_MODEID_EARTH_CENTER_POINTING,  BC_SL_TO_EARTH_CENTER_POINTING);

        MM_set_transition_table(MD_MODEID_RESERVED1,                    MD_MODEID_EARTH_CENTER_POINTING,  BC_SL_TO_EARTH_CENTER_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED2,                    MD_MODEID_EARTH_CENTER_POINTING,  BC_SL_TO_EARTH_CENTER_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED3,                    MD_MODEID_EARTH_CENTER_POINTING,  BC_SL_TO_EARTH_CENTER_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED4,                    MD_MODEID_EARTH_CENTER_POINTING,  BC_SL_TO_EARTH_CENTER_POINTING);

        //to ground tracking
        MM_set_transition_table(MD_MODEID_MISSION_STANDBY,              MD_MODEID_GROUND_TRACKING,  BC_SL_TO_GROUND_TRACKING);

        MM_set_transition_table(MD_MODEID_RESERVED1,                    MD_MODEID_GROUND_TRACKING,  BC_SL_TO_GROUND_TRACKING);
        MM_set_transition_table(MD_MODEID_RESERVED2,                    MD_MODEID_GROUND_TRACKING,  BC_SL_TO_GROUND_TRACKING);
        MM_set_transition_table(MD_MODEID_RESERVED3,                    MD_MODEID_GROUND_TRACKING,  BC_SL_TO_GROUND_TRACKING);
        MM_set_transition_table(MD_MODEID_RESERVED4,                    MD_MODEID_GROUND_TRACKING,  BC_SL_TO_GROUND_TRACKING);

        //to inertial pointing
        MM_set_transition_table(MD_MODEID_MISSION_STANDBY,              MD_MODEID_INERTIAL_POINTING,  BC_SL_TO_INERTIAL_POINTING);

        MM_set_transition_table(MD_MODEID_RESERVED1,                    MD_MODEID_INERTIAL_POINTING,  BC_SL_TO_INERTIAL_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED2,                    MD_MODEID_INERTIAL_POINTING,  BC_SL_TO_INERTIAL_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED3,                    MD_MODEID_INERTIAL_POINTING,  BC_SL_TO_INERTIAL_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED4,                    MD_MODEID_INERTIAL_POINTING,  BC_SL_TO_INERTIAL_POINTING);

        //to velocity direction pointing
        MM_set_transition_table(MD_MODEID_MISSION_STANDBY,              MD_MODEID_VELOCITY_DIRECTION_POINTING,  BC_SL_TO_VELOCITY_DIRECTION_POINTING);

        MM_set_transition_table(MD_MODEID_RESERVED1,                    MD_MODEID_VELOCITY_DIRECTION_POINTING,  BC_SL_TO_VELOCITY_DIRECTION_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED2,                    MD_MODEID_VELOCITY_DIRECTION_POINTING,  BC_SL_TO_VELOCITY_DIRECTION_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED3,                    MD_MODEID_VELOCITY_DIRECTION_POINTING,  BC_SL_TO_VELOCITY_DIRECTION_POINTING);
        MM_set_transition_table(MD_MODEID_RESERVED4,                    MD_MODEID_VELOCITY_DIRECTION_POINTING,  BC_SL_TO_VELOCITY_DIRECTION_POINTING);

		//to high spin
	    MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_HIGH_RATE_SPIN,  BC_SL_TO_HIGH_RATE_SPIN);

	    MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_HIGH_RATE_SPIN,  BC_SL_TO_HIGH_RATE_SPIN);
	    MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_HIGH_RATE_SPIN,  BC_SL_TO_HIGH_RATE_SPIN);
	    MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_HIGH_RATE_SPIN,  BC_SL_TO_HIGH_RATE_SPIN);
	    MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_HIGH_RATE_SPIN,  BC_SL_TO_HIGH_RATE_SPIN);

		//to other satellite pointing
		MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_OTHER_SATELLITE_POINTING,  BC_SL_TO_OTHER_SATELLITE_POINTING);

		MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_OTHER_SATELLITE_POINTING,  BC_SL_TO_OTHER_SATELLITE_POINTING);
		MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_OTHER_SATELLITE_POINTING,  BC_SL_TO_OTHER_SATELLITE_POINTING);
		MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_OTHER_SATELLITE_POINTING,  BC_SL_TO_OTHER_SATELLITE_POINTING);
		MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_OTHER_SATELLITE_POINTING,  BC_SL_TO_OTHER_SATELLITE_POINTING);

		//to relative orbital control
		MM_set_transition_table(MD_MODEID_MISSION_STANDBY, 				MD_MODEID_RELATIVE_ORBITAL_CONTROL,  BC_SL_TO_RELATIVE_ORBITAL_CONTROL);

		MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_RELATIVE_ORBITAL_CONTROL,  BC_SL_TO_RELATIVE_ORBITAL_CONTROL);
		MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_RELATIVE_ORBITAL_CONTROL,  BC_SL_TO_RELATIVE_ORBITAL_CONTROL);
		MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_RELATIVE_ORBITAL_CONTROL,  BC_SL_TO_RELATIVE_ORBITAL_CONTROL);
		MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_RELATIVE_ORBITAL_CONTROL,  BC_SL_TO_RELATIVE_ORBITAL_CONTROL);

/////////////////////////////////// 以下, 冗長MODE　/////////////////////////////////////////
		//to reserved1
        MM_set_transition_table(MD_MODEID_STANDBY,                      MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
        MM_set_transition_table(MD_MODEID_MISSION_STANDBY,              MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
        MM_set_transition_table(MD_MODEID_DAMPING,                      MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
        MM_set_transition_table(MD_MODEID_SUN_POINTING,                 MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
        MM_set_transition_table(MD_MODEID_EARTH_CENTER_POINTING,        MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
        MM_set_transition_table(MD_MODEID_GROUND_TRACKING,              MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
        MM_set_transition_table(MD_MODEID_INERTIAL_POINTING,            MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
        MM_set_transition_table(MD_MODEID_VELOCITY_DIRECTION_POINTING,  MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN, 				MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_OTHER_SATELLITE_POINTING,     MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
	    MM_set_transition_table(MD_MODEID_RELATIVE_ORBITAL_CONTROL,     MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
        MM_set_transition_table(MD_MODEID_CHECK_OUT,                    MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
		MM_set_transition_table(MD_MODEID_RESERVED1, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
		MM_set_transition_table(MD_MODEID_RESERVED2, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
		MM_set_transition_table(MD_MODEID_RESERVED3, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);
		MM_set_transition_table(MD_MODEID_RESERVED4, 					MD_MODEID_RESERVED1,  BC_SL_TO_RESERVED1);

		//to reserved2
        MM_set_transition_table(MD_MODEID_STANDBY,                      MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_MISSION_STANDBY,              MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_DAMPING,                      MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_SUN_POINTING,                 MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_EARTH_CENTER_POINTING,        MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_GROUND_TRACKING,              MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_INERTIAL_POINTING,            MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_VELOCITY_DIRECTION_POINTING,  MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN,               MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_OTHER_SATELLITE_POINTING,     MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_RELATIVE_ORBITAL_CONTROL,     MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_CHECK_OUT,                    MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_RESERVED1,                    MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_RESERVED2,                    MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_RESERVED3,                    MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);
        MM_set_transition_table(MD_MODEID_RESERVED4,                    MD_MODEID_RESERVED2,  BC_SL_TO_RESERVED2);

		//to reserved3
        MM_set_transition_table(MD_MODEID_STANDBY,                      MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_MISSION_STANDBY,              MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_DAMPING,                      MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_SUN_POINTING,                 MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_EARTH_CENTER_POINTING,        MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_GROUND_TRACKING,              MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_INERTIAL_POINTING,            MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_VELOCITY_DIRECTION_POINTING,  MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN,               MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_OTHER_SATELLITE_POINTING,     MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_RELATIVE_ORBITAL_CONTROL,     MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_CHECK_OUT,                    MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_RESERVED1,                    MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_RESERVED2,                    MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_RESERVED3,                    MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);
        MM_set_transition_table(MD_MODEID_RESERVED4,                    MD_MODEID_RESERVED3,  BC_SL_TO_RESERVED3);

		//to reserved4
        MM_set_transition_table(MD_MODEID_STANDBY,                      MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_MISSION_STANDBY,              MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_DAMPING,                      MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_SUN_POINTING,                 MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_EARTH_CENTER_POINTING,        MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_GROUND_TRACKING,              MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_INERTIAL_POINTING,            MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_VELOCITY_DIRECTION_POINTING,  MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_HIGH_RATE_SPIN,               MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_OTHER_SATELLITE_POINTING,     MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_RELATIVE_ORBITAL_CONTROL,     MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_CHECK_OUT,                    MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_RESERVED1,                    MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_RESERVED2,                    MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_RESERVED3,                    MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);
        MM_set_transition_table(MD_MODEID_RESERVED4,                    MD_MODEID_RESERVED4,  BC_SL_TO_RESERVED4);

		//to DBG
		MM_set_transition_table(MD_MODEID_STANDBY, 						MD_MODEID_DBG, 		  BC_SL_TO_DBG_MODE);
		MM_set_transition_table(MD_MODEID_DBG, 							MD_MODEID_DBG,  	  BC_SL_TO_DBG_MODE);
	}
}

#pragma section
