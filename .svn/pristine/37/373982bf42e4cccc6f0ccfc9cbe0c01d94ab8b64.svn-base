/*
 * BC_LOAD_all_param_init.c
 *
 *  Created on: 2021/12/06
 *      Author: SAT37
 */

#include "nbc_header.h"
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/Applications/app_registry.h>

void BC_load_param_load_cdh_com(void){
	BCL_tool_prepare_param_uint32(AR_ANOMALY_SETTING_PARAM_APP);
	BCL_tool_register_cmd    ( 1, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ANOMALY_MODE_AUTO_TRANS_PRAM_APP);
	BCL_tool_register_cmd    ( 2, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_MEM_SD_PARAM_APP);
	BCL_tool_register_cmd    ( 3, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_MEM_MRAM_PARAM_APP);
	BCL_tool_register_cmd    ( 4, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_MEM_EEPROM_PARAM_APP);
	BCL_tool_register_cmd    ( 5, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_MEM_SPIPROM_PARAM_APP);
	BCL_tool_register_cmd    ( 6, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_CDH_ALLREPRO_PARAM_APP);
	BCL_tool_register_cmd    ( 7, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_MESSAGE_PARAM_APP);
	BCL_tool_register_cmd    ( 8, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_LARGE_PACKET_PARAM_APP);
	BCL_tool_register_cmd    ( 9, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_COMMABD_RESPONSE_PARAM_APP);
	BCL_tool_register_cmd    ( 10, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_LORA_PARAM_APP);
	BCL_tool_register_cmd    ( 11, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_COBC_COMM_PARAM_APP);
	BCL_tool_register_cmd    ( 12, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_PWR_ANOMALY_DETECT_APP);
	BCL_tool_register_cmd    ( 13, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_DI_INTERSAT_PARAM_APP);
	BCL_tool_register_cmd    ( 14, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_MEM_FLASH_PARAM_APP);
	BCL_tool_register_cmd    ( 15, Cmd_CODE_AM_INITIALIZE_APP);

    BCL_tool_prepare_param_uint32(AP_CDH_ECLIPSE_PARAM_APP);
    BCL_tool_register_cmd    ( 16, Cmd_CODE_AM_INITIALIZE_APP);

#ifdef TWO_U
	BCL_tool_prepare_param_uint32(AR_COM_COBC_COMM_PARAM_APP);
	BCL_tool_register_cmd    ( 22, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_COBC_BOOT_PARAM_APP);
	BCL_tool_register_cmd    ( 23, Cmd_CODE_AM_INITIALIZE_APP);
#endif

	BCL_tool_prepare_param_uint8(0);
	BCL_tool_prepare_param_uint16(BC_PARAM_MEMORY_LOAD_AOCS);
	BCL_tool_register_cmd    ( 24, Cmd_CODE_TLCD_DEPLOY_BLOCK);
}

void BC_load_param_load_sensor_aocs(void){
	BCL_tool_prepare_param_uint32(AR_ACS_DEBUG_ADC_PARAM_APP);
	BCL_tool_register_cmd    ( 1, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_SUNS_PARAM_APP);
	BCL_tool_register_cmd    ( 2, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_9AX_PARAM_APP);
	BCL_tool_register_cmd    ( 3, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_GPS_PARAM_APP);
	BCL_tool_register_cmd    ( 4, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_MTQ_PARAM_APP);
	BCL_tool_register_cmd    ( 5, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_GLOBAL_AOCS_PARAM_APP);
	BCL_tool_register_cmd    ( 6, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_GLOBAL_AOCS_APP);
	BCL_tool_register_cmd    ( 7, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_BDOT_PARAM_APP);
	BCL_tool_register_cmd    ( 8, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_CROSSPRODUCT_PARAM_APP);
	BCL_tool_register_cmd    ( 9, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_EARTHVECTOR_PARAM_APP);
	BCL_tool_register_cmd    ( 10, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_COMPENSATION_PARAM_APP);
	BCL_tool_register_cmd    ( 11, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ADS_IGRF_PARAM_APP);
	BCL_tool_register_cmd    ( 12, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ADS_SUNVECTOR_PARAM_APP);
	BCL_tool_register_cmd    ( 13, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ADS_QMETHOD_PARAM_APP);
	BCL_tool_register_cmd    ( 14, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ADS_QUEST_PARAM_APP);
	BCL_tool_register_cmd    ( 15, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_OCS_CROSSPRODUCT_PARAM_APP);
	BCL_tool_register_cmd    ( 16, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ODS_SGP4_PARAM_APP);
	BCL_tool_register_cmd    ( 17, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ODS_ORBPROP_PARAM_APP);
	BCL_tool_register_cmd    ( 18, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_AOCS_EKF_PARAM_APP);
	BCL_tool_register_cmd    ( 19, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ODS_BALLCOEF_PARAM_APP);
	BCL_tool_register_cmd    ( 20, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_AOCS_SEPARATION_PARAM_APP);
	BCL_tool_register_cmd    ( 21, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_ADS_BBDOT_PARAM_APP);
	BCL_tool_register_cmd    ( 22, Cmd_CODE_AM_INITIALIZE_APP);

#ifdef ONE_U
	BCL_tool_prepare_param_uint32(AR_ACS_GYRO_PARAM_APP);
	BCL_tool_register_cmd    ( 23, Cmd_CODE_AM_INITIALIZE_APP);
	BCL_tool_prepare_param_uint32(AR_SUBCAM_PARAM_APP);
	BCL_tool_register_cmd    ( 25, Cmd_CODE_AM_INITIALIZE_APP);
#endif

#ifdef TWO_U
	BCL_tool_prepare_param_uint32(AR_AOCS_SEPARATION_PARAM_APP);
	BCL_tool_register_cmd    ( 23, Cmd_CODE_AM_INITIALIZE_APP);
#endif

	BCL_tool_prepare_param_uint8(0);
	BCL_tool_prepare_param_uint16(BC_PARAM_MEMORY_LOAD_AOCS2);
	BCL_tool_register_cmd    ( 24, Cmd_CODE_TLCD_DEPLOY_BLOCK);

}

void BC_load_param_load_sensor_aocs2(void){
	BCL_tool_prepare_param_uint32(AR_AOCS_TLM_PARAM_APP);
	BCL_tool_register_cmd    ( 1, Cmd_CODE_AM_INITIALIZE_APP);

	BCL_tool_prepare_param_uint32(AR_AOCS_PHOTO_COBC_PARAM_APP);
	BCL_tool_register_cmd    ( 2, Cmd_CODE_AM_INITIALIZE_APP);
}

void BC_load_param_init_cdh_com(void){
	BCL_tool_prepare_param_uint32(AR_ANOMALY_SETTING_PARAM_APP);
	BCL_tool_register_cmd    ( 1, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ANOMALY_MODE_AUTO_TRANS_PRAM_APP);
	BCL_tool_register_cmd    ( 2, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_MEM_SD_PARAM_APP);
	BCL_tool_register_cmd    ( 3, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_MEM_MRAM_PARAM_APP);
	BCL_tool_register_cmd    ( 4, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_MEM_EEPROM_PARAM_APP);
	BCL_tool_register_cmd    ( 5, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_MEM_SPIPROM_PARAM_APP);
	BCL_tool_register_cmd    ( 6, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_CDH_ALLREPRO_PARAM_APP);
	BCL_tool_register_cmd    ( 7, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_MESSAGE_PARAM_APP);
	BCL_tool_register_cmd    ( 8, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_LARGE_PACKET_PARAM_APP);
	BCL_tool_register_cmd    ( 9, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_COMMABD_RESPONSE_PARAM_APP);
	BCL_tool_register_cmd    ( 10, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_LORA_PARAM_APP);
	BCL_tool_register_cmd    ( 11, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_COBC_COMM_PARAM_APP);
	BCL_tool_register_cmd    ( 12, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_PWR_ANOMALY_DETECT_APP);
	BCL_tool_register_cmd    ( 13, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_DI_INTERSAT_PARAM_APP);
	BCL_tool_register_cmd    ( 14, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_MEM_FLASH_PARAM_APP);
	BCL_tool_register_cmd    ( 15, Cmd_CODE_AM_EXECUTE_APP);

#ifdef TWO_U
	BCL_tool_prepare_param_uint32(AR_COM_COBC_COMM_PARAM_APP);
	BCL_tool_register_cmd    ( 22, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_COM_COBC_BOOT_PARAM_APP);
	BCL_tool_register_cmd    ( 23, Cmd_CODE_AM_EXECUTE_APP);
#endif

	BCL_tool_prepare_param_uint8(0);
	BCL_tool_prepare_param_uint16(BC_PARAM_MEMORY_INIT_AOCS);
	BCL_tool_register_cmd    ( 24, Cmd_CODE_TLCD_DEPLOY_BLOCK);
}

void BC_load_param_init_sensor_aocs(void){
	BCL_tool_prepare_param_uint32(AR_ACS_DEBUG_ADC_PARAM_APP);
	BCL_tool_register_cmd    ( 1, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_SUNS_PARAM_APP);
	BCL_tool_register_cmd    ( 2, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_9AX_PARAM_APP);
	BCL_tool_register_cmd    ( 3, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_GPS_PARAM_APP);
	BCL_tool_register_cmd    ( 4, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_MTQ_PARAM_APP);
	BCL_tool_register_cmd    ( 5, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_GLOBAL_AOCS_PARAM_APP);
	BCL_tool_register_cmd    ( 6, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_GLOBAL_AOCS_APP);
	BCL_tool_register_cmd    ( 7, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_BDOT_PARAM_APP);
	BCL_tool_register_cmd    ( 8, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_CROSSPRODUCT_PARAM_APP);
	BCL_tool_register_cmd    ( 9, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_EARTHVECTOR_PARAM_APP);
	BCL_tool_register_cmd    ( 10, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ACS_COMPENSATION_PARAM_APP);
	BCL_tool_register_cmd    ( 11, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ADS_IGRF_PARAM_APP);
	BCL_tool_register_cmd    ( 12, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ADS_SUNVECTOR_PARAM_APP);
	BCL_tool_register_cmd    ( 13, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ADS_QMETHOD_PARAM_APP);
	BCL_tool_register_cmd    ( 14, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ADS_QUEST_PARAM_APP);
	BCL_tool_register_cmd    ( 15, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_OCS_CROSSPRODUCT_PARAM_APP);
	BCL_tool_register_cmd    ( 16, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ODS_SGP4_PARAM_APP);
	BCL_tool_register_cmd    ( 17, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ODS_ORBPROP_PARAM_APP);
	BCL_tool_register_cmd    ( 18, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_AOCS_EKF_PARAM_APP);
	BCL_tool_register_cmd    ( 19, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ODS_BALLCOEF_PARAM_APP);
	BCL_tool_register_cmd    ( 20, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_AOCS_SEPARATION_PARAM_APP);
	BCL_tool_register_cmd    ( 21, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_ADS_BBDOT_PARAM_APP);
	BCL_tool_register_cmd    ( 22, Cmd_CODE_AM_EXECUTE_APP);

#ifdef ONE_U
	BCL_tool_prepare_param_uint32(AR_ACS_GYRO_PARAM_APP);
	BCL_tool_register_cmd    ( 23, Cmd_CODE_AM_EXECUTE_APP);
#endif
#ifdef TWO_U
	BCL_tool_prepare_param_uint32(AR_AOCS_SEPARATION_PARAM_APP);
	BCL_tool_register_cmd    ( 23, Cmd_CODE_AM_EXECUTE_APP);
#endif

	BCL_tool_prepare_param_uint8(0);
	BCL_tool_prepare_param_uint16(BC_PARAM_MEMORY_INIT_AOCS2);
	BCL_tool_register_cmd    ( 24, Cmd_CODE_TLCD_DEPLOY_BLOCK);
}

void BC_load_param_init_sensor_aocs2(void){
	BCL_tool_prepare_param_uint32(AR_AOCS_TLM_PARAM_APP);
	BCL_tool_register_cmd    ( 1, Cmd_CODE_AM_EXECUTE_APP);

	BCL_tool_prepare_param_uint32(AR_AOCS_PHOTO_COBC_PARAM_APP);
	BCL_tool_register_cmd    ( 2, Cmd_CODE_AM_EXECUTE_APP);

#ifdef ONE_U
    BCL_tool_prepare_param_uint32(AR_SUBCAM_PARAM_APP);
    BCL_tool_register_cmd    ( 3, Cmd_CODE_AM_EXECUTE_APP);

    BCL_tool_prepare_param_uint32(AR_SUBCAM_TOA2BOA_PARAM_APP);
    BCL_tool_register_cmd    ( 4, Cmd_CODE_AM_EXECUTE_APP);
#endif
}
