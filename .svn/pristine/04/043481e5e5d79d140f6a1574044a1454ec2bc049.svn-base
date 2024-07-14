/*
 * BC_LOAD_TlmGenerateAndSave.c
 *
 *  Created on: 2021/05/29
 *      Author: kahaku
 *  Edited on: 2021/09/07
 *  	park
 */

#include "nbc_header.h"

#include "src_core/TlmCmd/block_command_table.h"
#include "src_user/TlmCmd/block_command_definitions.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>
#include "src_user/Applications/UserDefined/Memory/MEM_cmd_SD.h"
#include <src_core/System/ModeManager/mode_manager.h>

void BC_load_tlm_generate_and_save(void){

	BCL_tool_register_combine(2, BC_TLM_EVERY);
	BCL_tool_register_rotate (3, BC_TLM_LOW_RATE_1);
	BCL_tool_register_rotate (4, BC_TLM_LOW_RATE_2);

	BCL_tool_register_app    (6, AR_MEM_SPIPROM_CHECK_ENABLE);
	BCL_tool_register_app    (7, AR_MEM_SPIPROM_OPERATION_APP);
	BCL_tool_register_app    (8, AR_MEM_SPIPROM_QUEUE_SAVE_APP);
	BCL_tool_register_app    (9, AR_MEM_SPIPROM_APP);
	BCL_tool_register_app    (10, AR_MEM_SPIPROM_CONDITION_SAVE_APP);

//	BCL_tool_register_rotate (4, BC_TLM_C2A);

	BCL_tool_register_app    (14, AR_MEM_SD_APP);

	//INTERSAT
	//Inter-SAT
//	BCL_tool_register_app    (20, AR_CDH1_INTER_SAT_PACKET_HANDLER_APP);

	BCL_tool_register_cmd    (15, Cmd_CODE_CDH_SD_CONDITION_SAVE);

	BCL_tool_register_cmd    (16, Cmd_CODE_CDH_SD_GENERATE_RPTLM);
}

void BC_load_tlm_routing(void){
	//	HK/MSパケット処理
	BCL_tool_register_app    ( 1, AR_COM_TLM_MST_PACKET_HANDLER_APP);
	BCL_tool_register_app    ( 2, AR_COM_TLM_RPT_PACKET_HANDLER_APP);
	BCL_tool_register_app    ( 3, AR_COM_TLM_TXPIC_PACKET_HANDLER_APP);
	BCL_tool_register_app    ( 4, AR_COM_TLM_TXPIC_LARGEPACKET_HANDLER_APP);

	BCL_tool_register_cmd    (5, Cmd_CODE_CDH_SD_GENERATE_RPTLM);
#ifdef TWO_U
	BCL_tool_register_app    ( 6, AR_COM_COBC_LARGE_PACKET_RECEIVE);
#endif

#ifdef ONE_U
	BCL_tool_register_app    ( 6, AR_SUBCAM_LARGE_PACEKT_APP);
#endif
}

void BC_load_tlm_every(void){
	if(*(p_tlm_flag + 4)     == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_TI);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 1, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 5)     == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_HK);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 2, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 6) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_SHK);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 3, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 7) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_PWR_MOBC);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 4, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 8) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_THERMAL);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 5, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 9) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_SENSOR);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 6, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 11) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_ADS);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 7, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 12) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_AKF1);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 8, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 13) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_AKF2);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 9, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 14) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_ACS);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 10, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 15) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_ODS);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 11, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 16) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_OKF);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 12, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 17) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_OCS);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 13, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 18) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_RXPIC_TO_MOBC);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 14, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 19) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_TXPIC_TO_MOBC);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 15, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 21) == EVERY){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_SPIN);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 16, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 22) == EVERY && mode_manager->current_id != MD_MODEID_HIGH_RATE_SPIN){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_AOCS80);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 17, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 23) == EVERY && mode_manager->current_id != MD_MODEID_HIGH_RATE_SPIN){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_AOCS160);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 18, Cmd_CODE_GENERATE_TLM_USER);
	}

	if(*(p_tlm_flag + 24) == EVERY && mode_manager->current_id != MD_MODEID_HIGH_RATE_SPIN){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_AOCS240);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 19, Cmd_CODE_GENERATE_TLM_USER);
	}

}

void BC_load_tlm_low_rate_1(void){
	if(*(p_tlm_flag + 9) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_SENSOR);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 1, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 1, AR_NOP);

	if(*(p_tlm_flag + 11) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_ADS);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 2, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 2, AR_NOP);

	if(*(p_tlm_flag + 12) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_AKF1);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 3, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 3, AR_NOP);

	if(*(p_tlm_flag + 13) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_AKF2);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 4, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 4, AR_NOP);

	if(*(p_tlm_flag + 14) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_ACS);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 5, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 5, AR_NOP);

	if(*(p_tlm_flag + 15) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_ODS);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 6, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 6, AR_NOP);

	if(*(p_tlm_flag + 16) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_OKF);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 7, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 7, AR_NOP);

	if(*(p_tlm_flag + 17) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_OCS);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 8, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 8, AR_NOP);

	if(*(p_tlm_flag + 21) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_SPIN);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 9, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 9, AR_NOP);

	if(*(p_tlm_flag + 22) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_AOCS80);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 10, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 10, AR_NOP);

	if(*(p_tlm_flag + 23) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_AOCS160);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 11, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 11, AR_NOP);

	if(*(p_tlm_flag + 24) == LOW_RATE_1){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_AOCS240);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 12, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 12, AR_NOP);
}


void BC_load_tlm_low_rate_2(void){
	if(*(p_tlm_flag + 4)     == LOW_RATE_2){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_TI);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 1, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 1, AR_NOP);

	if(*(p_tlm_flag + 5)     == LOW_RATE_2){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_HK);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 2, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 2, AR_NOP);

	if(*(p_tlm_flag + 6) == LOW_RATE_2){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_SHK);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 3, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 3, AR_NOP);

	if(*(p_tlm_flag + 7) == LOW_RATE_2){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_PWR_MOBC);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 4, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 4, AR_NOP);

	if(*(p_tlm_flag + 8) == LOW_RATE_2){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_THERMAL);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 5, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 5, AR_NOP);

	if(*(p_tlm_flag + 18) == LOW_RATE_2){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_RXPIC_TO_MOBC);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 6, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 6, AR_NOP);

	if(*(p_tlm_flag + 19) == LOW_RATE_2){
		BCL_tool_prepare_param_uint16(AFSK_GS);
		BCL_tool_prepare_param_int8(0);
		BCL_tool_prepare_param_int8(0x80);
		BCL_tool_prepare_param_int8(Tlm_CODE_TXPIC_TO_MOBC);
		BCL_tool_prepare_param_int8(1);
		BCL_tool_register_cmd    ( 7, Cmd_CODE_GENERATE_TLM_USER);
	}
	else BCL_tool_register_app    ( 7, AR_NOP);

	BCL_tool_register_cmd    (8, Cmd_CODE_GENERATE_HK_DEBUG);
}


void BC_load_tlm_C2A(void){
	BCL_tool_prepare_param_uint16(AFSK_GS); //AFSK
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80); //category:ST
	BCL_tool_prepare_param_int8(Tlm_CODE_MOBC); //MOBC
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 1, Cmd_CODE_GENERATE_TLM_USER);

//	BCL_tool_prepare_param_uint8(0);
//	BCL_tool_register_cmd    ( 2, Cmd_CODE_TLCD_UPDATE_TIMELINE_TLM);

	BCL_tool_prepare_param_uint16(AFSK_GS); //AFSK
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80); //category:ST
	BCL_tool_prepare_param_int8(Tlm_CODE_TL); //TL
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 3, Cmd_CODE_GENERATE_TLM_USER);

	BCL_tool_prepare_param_uint16(AFSK_GS); //AFSK
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80); //category:ST
	BCL_tool_prepare_param_int8(Tlm_CODE_BL); //BL
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 4, Cmd_CODE_GENERATE_TLM_USER);

	BCL_tool_prepare_param_uint16(AFSK_GS); //AFSK
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80); //category:ST
	BCL_tool_prepare_param_int8(Tlm_CODE_CA); //CA
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 5, Cmd_CODE_GENERATE_TLM_USER);

	BCL_tool_prepare_param_uint16(AFSK_GS); //AFSK
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80); //category:ST
	BCL_tool_prepare_param_int8(Tlm_CODE_TF); //TF
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 6, Cmd_CODE_GENERATE_TLM_USER);

	BCL_tool_prepare_param_uint16(AFSK_GS); //AFSK
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80); //category:ST
	BCL_tool_prepare_param_int8(Tlm_CODE_MM); //MM
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 7, Cmd_CODE_GENERATE_TLM_USER);

	BCL_tool_prepare_param_uint16(AFSK_GS); //AFSK
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80); //category:ST
	BCL_tool_prepare_param_int8(Tlm_CODE_AM); //AM
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 8, Cmd_CODE_GENERATE_TLM_USER);

	BCL_tool_prepare_param_uint16(AFSK_GS); //AFSK
	BCL_tool_prepare_param_int8(0);
	BCL_tool_prepare_param_int8(0x80); //category:ST
	BCL_tool_prepare_param_int8(Tlm_CODE_APP_TIME); //APP_TIME
	BCL_tool_prepare_param_int8(1);
	BCL_tool_register_cmd    ( 9, Cmd_CODE_GENERATE_TLM_USER);
}
