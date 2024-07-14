/**
 * @file Com_Fault_Detectin_Driver.h
 * @Auther Sakaguchi
 */
#ifndef CDH_MODE_TRANSITION_DRIVER_H_
#define CDH_MODE_TRANSITION_DRIVER_H_

#include "../../IfWrapper/HAL.h"
#include <src_core/System/TimeManager/time_manager.h>
#include "../../Settings/Modes/mode_definitions.h"
#include "../../Settings/Modes/Phase/phase_setting_driver.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define AUTO_TRANSITION_ON (1)
#define AUTO_TRANSITION_OFF (0)

typedef struct{
	uint32_t phase0_transition_to;
	uint32_t phase1_transition_to;
	uint32_t phase2_transition_to;
	uint8_t enable;
}MODE_AUTO_TRANSITION;

MD_AOCSMODEID aoce_trans_vote(uint32_t* aocs);
MD_MODEID modeBC_trans_vote(uint32_t* mode);
MD_AOCSMODEID AOCS_mode_change_checker(MD_AOCSMODEID before_mode,MD_AOCSMODEID after_mode);
uint8_t auto_flag_vote(uint8_t* flag);
void Auto_trans_set(uint8_t *flag);
void Auto_trans_reset(uint8_t *flag);

uint8_t mode_table_checker(PHASE_NUM phase);

#ifdef __cplusplus
 }
#endif

#endif
