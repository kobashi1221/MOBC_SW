#pragma section REPRO
/**
 * @file
 * @brief  コマンド定義
 * @note   このコードは自動生成されています！
 */
#include <src_core/TlmCmd/command_analyze.h>
#include "command_definitions.h"
#include "command_source.h"

void CA_load_cmd_table(CA_CmdInfo cmd_table[CA_MAX_CMDS])
{
  cmd_table[Cmd_CODE_NOP].cmd_func = Cmd_NOP;
  cmd_table[Cmd_CODE_TMGR_SET_TIME].cmd_func = Cmd_TMGR_SET_TIME;
  cmd_table[Cmd_CODE_TMGR_UPDATE_UNIXTIME].cmd_func = Cmd_TMGR_UPDATE_UNIXTIME;
  cmd_table[Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH].cmd_func = Cmd_TMGR_SET_UTL_UNIXTIME_EPOCH;
  cmd_table[Cmd_CODE_TMGR_SET_CYCLE_CORRECTION].cmd_func = Cmd_TMGR_SET_CYCLE_CORRECTION;
  cmd_table[Cmd_CODE_TMGR_RESET_CYCLE_CORRECTION].cmd_func = Cmd_TMGR_RESET_CYCLE_CORRECTION;
  cmd_table[Cmd_CODE_TMGR_CLEAR_UNIXTIME_INFO].cmd_func = Cmd_TMGR_CLEAR_UNIXTIME_INFO;
  cmd_table[Cmd_CODE_AM_REGISTER_APP].cmd_func = Cmd_AM_REGISTER_APP;
  cmd_table[Cmd_CODE_AM_INITIALIZE_APP].cmd_func = Cmd_AM_INITIALIZE_APP;
  cmd_table[Cmd_CODE_AM_EXECUTE_APP].cmd_func = Cmd_AM_EXECUTE_APP;
  cmd_table[Cmd_CODE_AM_SET_PAGE_FOR_TLM].cmd_func = Cmd_AM_SET_PAGE_FOR_TLM;
  cmd_table[Cmd_CODE_AM_CLEAR_APP_INFO].cmd_func = Cmd_AM_CLEAR_APP_INFO;
  cmd_table[Cmd_CODE_MM_SET_MODE_LIST].cmd_func = Cmd_MM_SET_MODE_LIST;
  cmd_table[Cmd_CODE_MM_SET_TRANSITION_TABLE].cmd_func = Cmd_MM_SET_TRANSITION_TABLE;
  cmd_table[Cmd_CODE_MM_START_TRANSITION].cmd_func = Cmd_MM_START_TRANSITION;
  cmd_table[Cmd_CODE_MM_FINISH_TRANSITION].cmd_func = Cmd_MM_FINISH_TRANSITION;
  cmd_table[Cmd_CODE_MM_UPDATE_TRANSITION_TABLE_FOR_TLM].cmd_func = Cmd_MM_UPDATE_TRANSITION_TABLE_FOR_TLM;
  cmd_table[Cmd_CODE_TDSP_SET_TASK_LIST].cmd_func = Cmd_TDSP_SET_TASK_LIST;
  cmd_table[Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE].cmd_func = Cmd_TLCD_CLEAR_ALL_TIMELINE;
  cmd_table[Cmd_CODE_TLCD_CLEAR_TIMELINE_AT].cmd_func = Cmd_TLCD_CLEAR_TIMELINE_AT;
  cmd_table[Cmd_CODE_TLCD_DEPLOY_BLOCK].cmd_func = Cmd_TLCD_DEPLOY_BLOCK;
  cmd_table[Cmd_CODE_TLCD_CLEAR_ERR_LOG].cmd_func = Cmd_TLCD_CLEAR_ERR_LOG;
  cmd_table[Cmd_CODE_TLCD_SET_SOE_FLAG].cmd_func = Cmd_TLCD_SET_SOE_FLAG;
  cmd_table[Cmd_CODE_TLCD_SET_LOUT_FLAG].cmd_func = Cmd_TLCD_SET_LOUT_FLAG;
  cmd_table[Cmd_CODE_TLCD_SET_ID_FOR_TLM].cmd_func = Cmd_TLCD_SET_ID_FOR_TLM;
  cmd_table[Cmd_CODE_TLCD_SET_PAGE_FOR_TLM].cmd_func = Cmd_TLCD_SET_PAGE_FOR_TLM;
  cmd_table[Cmd_CODE_GENERATE_TLM].cmd_func = Cmd_GENERATE_TLM;
  cmd_table[Cmd_CODE_TG_GENERATE_TLM].cmd_func = Cmd_TG_GENERATE_TLM;
  cmd_table[Cmd_CODE_TG_GENERATE_HP_TLM].cmd_func = Cmd_TG_GENERATE_HP_TLM;
  cmd_table[Cmd_CODE_TG_GENERATE_RT_TLM].cmd_func = Cmd_TG_GENERATE_RT_TLM;
  cmd_table[Cmd_CODE_TG_GENERATE_ST_TLM].cmd_func = Cmd_TG_GENERATE_ST_TLM;
  cmd_table[Cmd_CODE_TG_FORWARD_TLM].cmd_func = Cmd_TG_FORWARD_TLM;
  cmd_table[Cmd_CODE_TG_FORWARD_AS_HP_TLM].cmd_func = Cmd_TG_FORWARD_AS_HP_TLM;
  cmd_table[Cmd_CODE_TG_FORWARD_AS_RT_TLM].cmd_func = Cmd_TG_FORWARD_AS_RT_TLM;
  cmd_table[Cmd_CODE_TG_FORWARD_AS_ST_TLM].cmd_func = Cmd_TG_FORWARD_AS_ST_TLM;
  cmd_table[Cmd_CODE_BCT_CLEAR_BLOCK].cmd_func = Cmd_BCT_CLEAR_BLOCK;
  cmd_table[Cmd_CODE_BCT_SET_BLOCK_POSITION].cmd_func = Cmd_BCT_SET_BLOCK_POSITION;
  cmd_table[Cmd_CODE_BCT_COPY_BCT].cmd_func = Cmd_BCT_COPY_BCT;
  cmd_table[Cmd_CODE_BCT_OVERWRITE_CMD].cmd_func = Cmd_BCT_OVERWRITE_CMD;
  cmd_table[Cmd_CODE_BCT_FILL_NOP].cmd_func = Cmd_BCT_FILL_NOP;
  cmd_table[Cmd_CODE_BCE_ACTIVATE_BLOCK].cmd_func = Cmd_BCE_ACTIVATE_BLOCK;
  cmd_table[Cmd_CODE_BCE_ACTIVATE_BLOCK_BY_ID].cmd_func = Cmd_BCE_ACTIVATE_BLOCK_BY_ID;
  cmd_table[Cmd_CODE_BCE_INACTIVATE_BLOCK_BY_ID].cmd_func = Cmd_BCE_INACTIVATE_BLOCK_BY_ID;
  cmd_table[Cmd_CODE_BCE_ROTATE_BLOCK].cmd_func = Cmd_BCE_ROTATE_BLOCK;
  cmd_table[Cmd_CODE_BCE_COMBINE_BLOCK].cmd_func = Cmd_BCE_COMBINE_BLOCK;
  cmd_table[Cmd_CODE_BCE_TIMELIMIT_COMBINE_BLOCK].cmd_func = Cmd_BCE_TIMELIMIT_COMBINE_BLOCK;
  cmd_table[Cmd_CODE_BCE_RESET_ROTATOR_INFO].cmd_func = Cmd_BCE_RESET_ROTATOR_INFO;
  cmd_table[Cmd_CODE_BCE_RESET_COMBINER_INFO].cmd_func = Cmd_BCE_RESET_COMBINER_INFO;
  cmd_table[Cmd_CODE_BCE_SET_ROTATE_INTERVAL].cmd_func = Cmd_BCE_SET_ROTATE_INTERVAL;
  cmd_table[Cmd_CODE_GSCD_CLEAR_ERR_LOG].cmd_func = Cmd_GSCD_CLEAR_ERR_LOG;
  cmd_table[Cmd_CODE_RTCD_CLEAR_ALL_REALTIME].cmd_func = Cmd_RTCD_CLEAR_ALL_REALTIME;
  cmd_table[Cmd_CODE_RTCD_CLEAR_ERR_LOG].cmd_func = Cmd_RTCD_CLEAR_ERR_LOG;
  cmd_table[Cmd_CODE_MEM_SET_REGION].cmd_func = Cmd_MEM_SET_REGION;
  cmd_table[Cmd_CODE_MEM_LOAD].cmd_func = Cmd_MEM_LOAD;
  cmd_table[Cmd_CODE_MEM_SET_DESTINATION].cmd_func = Cmd_MEM_SET_DESTINATION;
  cmd_table[Cmd_CODE_MEM_COPY_REGION_SEQ].cmd_func = Cmd_MEM_COPY_REGION_SEQ;
  cmd_table[Cmd_CODE_EL_INIT].cmd_func = Cmd_EL_INIT;
  cmd_table[Cmd_CODE_EL_CLEAR_LOG_ALL].cmd_func = Cmd_EL_CLEAR_LOG_ALL;
  cmd_table[Cmd_CODE_EL_CLEAR_LOG_BY_ERR_LEVEL].cmd_func = Cmd_EL_CLEAR_LOG_BY_ERR_LEVEL;
  cmd_table[Cmd_CODE_EL_CLEAR_STATISTICS].cmd_func = Cmd_EL_CLEAR_STATISTICS;
  cmd_table[Cmd_CODE_EL_CLEAR_TLOG].cmd_func = Cmd_EL_CLEAR_TLOG;
  cmd_table[Cmd_CODE_EL_CLEAR_CLOG].cmd_func = Cmd_EL_CLEAR_CLOG;
  cmd_table[Cmd_CODE_EL_RECORD_EVENT].cmd_func = Cmd_EL_RECORD_EVENT;
  cmd_table[Cmd_CODE_EL_TLOG_SET_PAGE_FOR_TLM].cmd_func = Cmd_EL_TLOG_SET_PAGE_FOR_TLM;
  cmd_table[Cmd_CODE_EL_CLOG_SET_PAGE_FOR_TLM].cmd_func = Cmd_EL_CLOG_SET_PAGE_FOR_TLM;
  cmd_table[Cmd_CODE_EL_INIT_LOGGING_SETTINGS].cmd_func = Cmd_EL_INIT_LOGGING_SETTINGS;
  cmd_table[Cmd_CODE_EL_ENABLE_LOGGING].cmd_func = Cmd_EL_ENABLE_LOGGING;
  cmd_table[Cmd_CODE_EL_DISABLE_LOGGING].cmd_func = Cmd_EL_DISABLE_LOGGING;
  cmd_table[Cmd_CODE_EL_ENABLE_LOGGING_ALL].cmd_func = Cmd_EL_ENABLE_LOGGING_ALL;
  cmd_table[Cmd_CODE_EL_DISABLE_LOGGING_ALL].cmd_func = Cmd_EL_DISABLE_LOGGING_ALL;
  cmd_table[Cmd_CODE_EL_ENABLE_TLOG_OVERWRITE].cmd_func = Cmd_EL_ENABLE_TLOG_OVERWRITE;
  cmd_table[Cmd_CODE_EL_DISABLE_TLOG_OVERWRITE].cmd_func = Cmd_EL_DISABLE_TLOG_OVERWRITE;
  cmd_table[Cmd_CODE_EL_ENABLE_TLOG_OVERWRITE_ALL].cmd_func = Cmd_EL_ENABLE_TLOG_OVERWRITE_ALL;
  cmd_table[Cmd_CODE_EL_DISABLE_TLOG_OVERWRITE_ALL].cmd_func = Cmd_EL_DISABLE_TLOG_OVERWRITE_ALL;
  cmd_table[Cmd_CODE_EH_INIT].cmd_func = Cmd_EH_INIT;
  cmd_table[Cmd_CODE_EH_CLEAR_ALL_RULE].cmd_func = Cmd_EH_CLEAR_ALL_RULE;
  cmd_table[Cmd_CODE_EH_LOAD_DEFAULT_RULE].cmd_func = Cmd_EH_LOAD_DEFAULT_RULE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_EVENT_PARAM].cmd_func = Cmd_EH_SET_REGISTER_RULE_EVENT_PARAM;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_CONDITION_PARAM].cmd_func = Cmd_EH_SET_REGISTER_RULE_CONDITION_PARAM;
  cmd_table[Cmd_CODE_EH_REGISTER_RULE].cmd_func = Cmd_EH_REGISTER_RULE;
  cmd_table[Cmd_CODE_EH_DELETE_RULE].cmd_func = Cmd_EH_DELETE_RULE;
  cmd_table[Cmd_CODE_EH_INIT_RULE].cmd_func = Cmd_EH_INIT_RULE;
  cmd_table[Cmd_CODE_EH_INIT_RULE_FOR_MULTI_LEVEL].cmd_func = Cmd_EH_INIT_RULE_FOR_MULTI_LEVEL;
  cmd_table[Cmd_CODE_EH_ACTIVATE_RULE].cmd_func = Cmd_EH_ACTIVATE_RULE;
  cmd_table[Cmd_CODE_EH_INACTIVATE_RULE].cmd_func = Cmd_EH_INACTIVATE_RULE;
  cmd_table[Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL].cmd_func = Cmd_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL;
  cmd_table[Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL].cmd_func = Cmd_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL;
  cmd_table[Cmd_CODE_EH_SET_RULE_COUNTER].cmd_func = Cmd_EH_SET_RULE_COUNTER;
  cmd_table[Cmd_CODE_EH_CLEAR_RULE_COUNTER].cmd_func = Cmd_EH_CLEAR_RULE_COUNTER;
  cmd_table[Cmd_CODE_EH_CLEAR_RULE_COUNTER_BY_EVENT].cmd_func = Cmd_EH_CLEAR_RULE_COUNTER_BY_EVENT;
  cmd_table[Cmd_CODE_EH_CLEAR_LOG].cmd_func = Cmd_EH_CLEAR_LOG;
  cmd_table[Cmd_CODE_EH_SET_MAX_RESPONSE_NUM].cmd_func = Cmd_EH_SET_MAX_RESPONSE_NUM;
  cmd_table[Cmd_CODE_EH_SET_MAX_CHECK_EVENT_NUM].cmd_func = Cmd_EH_SET_MAX_CHECK_EVENT_NUM;
  cmd_table[Cmd_CODE_EH_SET_MAX_MULTI_LEVEL_NUM].cmd_func = Cmd_EH_SET_MAX_MULTI_LEVEL_NUM;
  cmd_table[Cmd_CODE_EH_SET_PAGE_OF_RULE_TABLE_FOR_TLM].cmd_func = Cmd_EH_SET_PAGE_OF_RULE_TABLE_FOR_TLM;
  cmd_table[Cmd_CODE_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM].cmd_func = Cmd_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM;
  cmd_table[Cmd_CODE_EH_SET_PAGE_OF_LOG_TABLE_FOR_TLM].cmd_func = Cmd_EH_SET_PAGE_OF_LOG_TABLE_FOR_TLM;
  cmd_table[Cmd_CODE_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM].cmd_func = Cmd_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM;
  cmd_table[Cmd_CODE_EH_MATCH_EVENT_COUNTER_TO_EL].cmd_func = Cmd_EH_MATCH_EVENT_COUNTER_TO_EL;
  cmd_table[Cmd_CODE_EH_INIT_RULE_BY_EVENT_GROUP].cmd_func = Cmd_EH_INIT_RULE_BY_EVENT_GROUP;
  cmd_table[Cmd_CODE_EH_INIT_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL].cmd_func = Cmd_EH_INIT_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL;
  cmd_table[Cmd_CODE_EH_ACTIVATE_RULE_BY_EVENT_GROUP].cmd_func = Cmd_EH_ACTIVATE_RULE_BY_EVENT_GROUP;
  cmd_table[Cmd_CODE_EH_INACTIVATE_RULE_BY_EVENT_GROUP].cmd_func = Cmd_EH_INACTIVATE_RULE_BY_EVENT_GROUP;
  cmd_table[Cmd_CODE_EH_ACTIVATE_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL].cmd_func = Cmd_EH_ACTIVATE_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL;
  cmd_table[Cmd_CODE_EH_INACTIVATE_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL].cmd_func = Cmd_EH_INACTIVATE_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL;
  cmd_table[Cmd_CODE_EVENT_UTIL_ENABLE_EH_EXEC].cmd_func = Cmd_EVENT_UTIL_ENABLE_EH_EXEC;
  cmd_table[Cmd_CODE_EVENT_UTIL_DISABLE_EH_EXEC].cmd_func = Cmd_EVENT_UTIL_DISABLE_EH_EXEC;
  cmd_table[Cmd_CODE_EVENT_UTIL_EXEC_EH].cmd_func = Cmd_EVENT_UTIL_EXEC_EH;
  cmd_table[Cmd_CODE_TF_INIT].cmd_func = Cmd_TF_INIT;
  cmd_table[Cmd_CODE_CA_INIT].cmd_func = Cmd_CA_INIT;
  cmd_table[Cmd_CODE_TF_REGISTER_TLM].cmd_func = Cmd_TF_REGISTER_TLM;
  cmd_table[Cmd_CODE_CA_REGISTER_CMD].cmd_func = Cmd_CA_REGISTER_CMD;
  cmd_table[Cmd_CODE_TF_SET_PAGE_FOR_TLM].cmd_func = Cmd_TF_SET_PAGE_FOR_TLM;
  cmd_table[Cmd_CODE_CA_SET_PAGE_FOR_TLM].cmd_func = Cmd_CA_SET_PAGE_FOR_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_INIT].cmd_func = Cmd_TLM_MGR_INIT;
  cmd_table[Cmd_CODE_TLM_MGR_INIT_MASTER_AND_DEPLOY_BC].cmd_func = Cmd_TLM_MGR_INIT_MASTER_AND_DEPLOY_BC;
  cmd_table[Cmd_CODE_TLM_MGR_CLEAR_HK_TLM].cmd_func = Cmd_TLM_MGR_CLEAR_HK_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_CLEAR_HIGH_FREQ_TLM].cmd_func = Cmd_TLM_MGR_CLEAR_HIGH_FREQ_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_CLEAR_LOW_FREQ_TLM].cmd_func = Cmd_TLM_MGR_CLEAR_LOW_FREQ_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_CLEAR_USER_TLM].cmd_func = Cmd_TLM_MGR_CLEAR_USER_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_START_TLM].cmd_func = Cmd_TLM_MGR_START_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_STOP_TLM].cmd_func = Cmd_TLM_MGR_STOP_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_GENERATE_RT_TLM].cmd_func = Cmd_TLM_MGR_REGISTER_GENERATE_RT_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_GENERATE_ST_TLM].cmd_func = Cmd_TLM_MGR_REGISTER_GENERATE_ST_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_FORWARD_AS_RT_TLM].cmd_func = Cmd_TLM_MGR_REGISTER_FORWARD_AS_RT_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_FORWARD_AS_ST_TLM].cmd_func = Cmd_TLM_MGR_REGISTER_FORWARD_AS_ST_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_REPLAY_TLM].cmd_func = Cmd_TLM_MGR_REGISTER_REPLAY_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_GENERATE_RT_TLM].cmd_func = Cmd_TLM_MGR_DELETE_GENERATE_RT_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_GENERATE_ST_TLM].cmd_func = Cmd_TLM_MGR_DELETE_GENERATE_ST_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_FORWARD_AS_RT_TLM].cmd_func = Cmd_TLM_MGR_DELETE_FORWARD_AS_RT_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_FORWARD_AS_ST_TLM].cmd_func = Cmd_TLM_MGR_DELETE_FORWARD_AS_ST_TLM;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_REPLAY_TLM].cmd_func = Cmd_TLM_MGR_DELETE_REPLAY_TLM;
  cmd_table[Cmd_CODE_DCU_ABORT_CMD].cmd_func = Cmd_DCU_ABORT_CMD;
  cmd_table[Cmd_CODE_DCU_DOWN_ABORT_FLAG].cmd_func = Cmd_DCU_DOWN_ABORT_FLAG;
  cmd_table[Cmd_CODE_DCU_CLEAR_LOG].cmd_func = Cmd_DCU_CLEAR_LOG;
  cmd_table[Cmd_CODE_TEST_CCP_REGISTER_TLC_ASAP].cmd_func = Cmd_TEST_CCP_REGISTER_TLC_ASAP;
  cmd_table[Cmd_CODE_TEST_CCP_GET_RAW_PARAM_INFO].cmd_func = Cmd_TEST_CCP_GET_RAW_PARAM_INFO;

  /* - BCUS - */
  cmd_table[Cmd_CODE_BCUS_CLEAR_USER_SETTINGS].cmd_func = Cmd_BCUS_CLEAR_USER_SETTINGS;
  cmd_table[Cmd_CODE_BCUS_SWAP_ADDRESS].cmd_func = Cmd_BCUS_SWAP_ADDRESS;
  cmd_table[Cmd_CODE_BCUS_SWAP_ADDRESS].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCUS_SWAP_ADDRESS].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCUS_SWAP_CONTENTS].cmd_func = Cmd_BCUS_SWAP_CONTENTS;
  cmd_table[Cmd_CODE_BCUS_SWAP_CONTENTS].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCUS_SWAP_CONTENTS].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCUS_SWAP_RAM].cmd_func = Cmd_BCUS_SWAP_RAM;
  cmd_table[Cmd_CODE_BCUS_SWAP_RAM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCUS_SWAP_RAM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCUS_SWAP_MODE].cmd_func = Cmd_BCUS_SWAP_MODE;
  cmd_table[Cmd_CODE_BCUS_SWAP_MODE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCUS_SWAP_MODE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;

    /* ----- MAGNARO ----- */
    /* ----- COMMON MOBC & COBC ----- */
    /* - RTC - */
  cmd_table[Cmd_CODE_CDH_RTC_SET].cmd_func = Cmd_RTC_SET;
  cmd_table[Cmd_CODE_CDH_RTC_SET].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_CDH_RTC_SET].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_RTC_SET].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_RTC_SET].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_RTC_SET].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_RTC_SET].param_size_infos[2].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;

/* - Reset - */
  cmd_table[Cmd_CODE_CDH_SET_RESET_COUNT].cmd_func = Cmd_RESET_SET_COUNT;
  cmd_table[Cmd_CODE_CDH_SET_RESET_COUNT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SET_RESET_COUNT].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;

	/* - DBG - */
  cmd_table[Cmd_CODE_GENERATE_HK_DEBUG].cmd_func= Cmd_GENERATE_HK_DEBUG;
  cmd_table[Cmd_CODE_GENERATE_TLM_USER].cmd_func= Cmd_GENERATE_TLM_user;
  cmd_table[Cmd_CODE_GENERATE_TLM_USER].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_GENERATE_TLM_USER].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_GENERATE_TLM_USER].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_GENERATE_TLM_USER].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_GENERATE_TLM_USER].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;

/* - DBG print - */
  cmd_table[Cmd_CODE_DEBUG_PRINTF].cmd_func= Cmd_DBG_SELECT_printf;
  cmd_table[Cmd_CODE_DEBUG_PRINTF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_SET_PRINTF].cmd_func= Cmd_DBG_Set_Print;
  cmd_table[Cmd_CODE_SET_PRINTF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_RESET_PRINTF].cmd_func= Cmd_DBG_Reset_Print;
  cmd_table[Cmd_CODE_RESET_PRINTF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_DEBUG_NOP].cmd_func= Cmd_debug_nop;

/* - BCT - */
  cmd_table[Cmd_CODE_BCT_CHANGE_ALT_BC].cmd_func = Cmd_BCT_CHANGE_ALT_BC;//BCT書き換え用のコマンド
  cmd_table[Cmd_CODE_BCT_CHANGE_ALT_BC].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_CHANGE_ALT_BC].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_SET_PAGE_FOR_TLM].cmd_func = Cmd_BCT_SET_PAGE_FOR_TLM;
  cmd_table[Cmd_CODE_BCT_SET_PAGE_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD_PACKET].cmd_func = Cmd_BCT_SD_SET_CMD_Packet;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD_PACKET].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD_PACKET].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD_PACKET].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD].cmd_func = Cmd_BCT_SD_SET_CMD;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD_NUM].cmd_func = Cmd_BCT_SD_SET_CMD_NUM;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD_NUM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_SD_SET_CMD_NUM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BCT_SAVE_ROM].cmd_func = Cmd_SAVE_BCT;
  cmd_table[Cmd_CODE_BCT_SAVE_ROM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_SAVE_ROM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_BCT_LOAD_ROM].cmd_func = Cmd_LOAD_BCT;
  cmd_table[Cmd_CODE_BCT_LOAD_ROM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_LOAD_ROM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_BCT_SD_COMBINE].cmd_func = Cmd_SD_combine_block_cmd;
  cmd_table[Cmd_CODE_BCT_SD_COMBINE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_SD_DEPLOY].cmd_func = Cmd_SD_deploy_block_cmd;
  cmd_table[Cmd_CODE_BCT_SD_DEPLOY].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BCT_SD_DEPLOY].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;

  /* --- Repro --- */
  cmd_table[Cmd_CODE_CDH_RESET_SEQ_CNT].cmd_func = Cmd_CDH_AllRepro_reset_flag;
  cmd_table[Cmd_CODE_CDH_SEND_RECEIVEFLAG].cmd_func = Cmd_CDH_ALLREPRO_SEND_ReceiveFlag;
  cmd_table[Cmd_CODE_CDH_SEND_RECEIVEFLAG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_JUMP_NEW].cmd_func = Cmd_CDH_AllRepro_JUMP_NEW;

/* - ALL Repro - */
  cmd_table[Cmd_CODE_CDH_PROGRAM_TO_SD].cmd_func = Cmd_CDH_AllRepro_Program_to_SD;
  cmd_table[Cmd_CODE_CDH_PROGRAM_TO_SD].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_CDH_PROGRAM_TO_SD].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_CDH_PROGRAM_INIT].cmd_func = Cmd_CDH_AllRepro_set_number;
  cmd_table[Cmd_CODE_CDH_PROGRAM_INIT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_DEBUG_FLASH_WRITE_PACKET].cmd_func = Cmd_CDH_debug_Write_Program_on_FLASH;
  cmd_table[Cmd_CODE_CDH_DEBUG_FLASH_WRITE_PACKET].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;

/* - PATCH Repro - */
  cmd_table[Cmd_CODE_CDH_PATCH_SEQ_DATA].cmd_func = Cmd_CDH_patch_seq_data;
  cmd_table[Cmd_CODE_CDH_PATCH_SEQ_DATA].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_CDH_PATCH_SEQ_DATA].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_PATCH_SEQ_DATA].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_PATCH_SEQ_DATA].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_CDH_PATCH_SCATTER_DATA].cmd_func = Cmd_CDH_patch_scatter_data;
  cmd_table[Cmd_CODE_CDH_PATCH_SCATTER_DATA].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_CDH_PATCH_SCATTER_DATA].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_PATCH_SCATTER_DATA].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_CDH_PATCH_SCATTER_DATA].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_RAW;

/* - Function Uplink - */
  cmd_table[Cmd_CODE_CDH_SET_CODE_TO_SD].cmd_func = Cmd_CDH_set_code_to_SD;
  cmd_table[Cmd_CODE_CDH_SET_CODE_TO_SD].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SET_CODE_TO_SD].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_RAW;

  /* --- MEM --- */
/* - SD - */
  cmd_table[Cmd_CODE_CDH_SD_READ_BLOCK].cmd_func = Cmd_CDH1_SD_BLOCK_READ;
  cmd_table[Cmd_CODE_CDH_SD_READ_BLOCK].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SD_CONDITION_SAVE].cmd_func = Cmd_CDH1_SD_CONDITION_SAVE;
  cmd_table[Cmd_CODE_CDH_SD_RESET].cmd_func = Cmd_CDH1_SD_reset_all;
  cmd_table[Cmd_CODE_CDH_SD_CHANGE_RPTLM_PARAM].cmd_func = Cmd_CDH1_SD_CHANGE_RPTLM_param;
  cmd_table[Cmd_CODE_CDH_SD_CHANGE_RPTLM_PARAM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SD_CHANGE_RPTLM_PARAM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_CDH_SD_CHANGE_RPTLM_PARAM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SD_CHANGE_RPTLM_PARAM].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SD_CHANGE_RPTLM_PARAM].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SD_CHANGE_RPTLM_PARAM].param_size_infos[2].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SD_CHANGE_RPTLM_PARAM].param_size_infos[3].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SD_CHANGE_RPTLM_PARAM].param_size_infos[3].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SD_GENERATE_RPTLM].cmd_func = Cmd_CDH1_SD_Generate_RPTLM;
  cmd_table[Cmd_CODE_SD_SET_PAGE_FOE_TLM].cmd_func = Cmd_SD_SET_PAGE_FOR_TLM;
  cmd_table[Cmd_CODE_SD_SET_PAGE_FOE_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_SD_PARAM_WRITE].cmd_func = Cmd_MEM_SD_writeParamData;
  cmd_table[Cmd_CODE_MEM_SD_PARAM_WRITE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_SD_PARAM_WRITE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SD_PARAM_WRITE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_MEM_SD_PARAM_READ].cmd_func = Cmd_MEM_SD_ReadParamData;
  cmd_table[Cmd_CODE_MEM_SD_PARAM_READ].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SD_PARAM_READ].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SD_PROGRAM_WRITE].cmd_func = Cmd_MEM_SD_writeProgramData;
  cmd_table[Cmd_CODE_MEM_SD_PROGRAM_WRITE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_SD_PROGRAM_WRITE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SD_PROGRAM_WRITE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_MEM_SD_PROGRAM_READ].cmd_func = Cmd_MEM_SD_ReadProgramData;
  cmd_table[Cmd_CODE_MEM_SD_PROGRAM_READ].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SD_PROGRAM_READ].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SD_SET_PATROLBLOCK].cmd_func = Cmd_MEM_SD_set_patrol_block;
  cmd_table[Cmd_CODE_MEM_SD_SET_PATROLBLOCK].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SD_DUMP].cmd_func = Cmd_APP_TMD_SD_SET_BEGIN_BLOCK_ADR;
  cmd_table[Cmd_CODE_MEM_SD_DUMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SD_DUMP].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_SD3_DUMP].cmd_func = Cmd_APP_TMD_SD3_SET_BEGIN_ADR;
  cmd_table[Cmd_CODE_MEM_SD3_DUMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_TLM_CHANGE_FREQ].cmd_func = Cmd_TLM_CHANGE_FREQ;
  cmd_table[Cmd_CODE_SD_RPTLM_PAUSE].cmd_func = Cmd_CDH1_SD_RPTLM_Pause;
  cmd_table[Cmd_CODE_SD_RPTLM_RESUME].cmd_func = Cmd_CDH1_SD_RPTLM_Resume;
  cmd_table[Cmd_CODE_MEM_SD_READ_COMMAND].cmd_func = Cmd_MEM_SD_command_read;
  cmd_table[Cmd_CODE_MEM_SD_READ_COMMAND].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_MEM_SD_WRITE].cmd_func = Cmd_MEM_SD_write;
  cmd_table[Cmd_CODE_MEM_SD_WRITE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SD_WRITE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SD_WRITE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
#ifdef ONE_U
  cmd_table[Cmd_CODE_MEM_SD_READIMAGE].cmd_func = Cmd_SD_ReadForDebug;
  cmd_table[Cmd_CODE_MEM_SD_READIMAGE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_SD_READIMAGE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
#endif

/* - FLASH - */
  cmd_table[Cmd_CODE_MEM_FLASH_UNSET_ERASEFLAG].cmd_func = Cmd_MEM_flash_unset_eraseflag;
  cmd_table[Cmd_CODE_MEM_FLASH_UNSET_ERASEFLAG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_FLASH_UNSET_ERASEFLAG].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_FLASH_MEMORY_PATROL].cmd_func = Cmd_MEM_FlashBoot_patrol;
  cmd_table[Cmd_CODE_MEM_FLASH_SET_PATROLADDR].cmd_func = Cmd_MEM_flash_set_patrol_addr;
  cmd_table[Cmd_CODE_MEM_FLASH_SET_PATROLADDR].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2SD].cmd_func = Cmd_BD_SET_COPY_SD2SD;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2SD].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2SD].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2SD].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2SD].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2SD].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_FLASH2FLASH].cmd_func = Cmd_BD_SET_COPY_FLASH2FLASH;
  cmd_table[Cmd_CODE_MEM_SET_COPY_FLASH2FLASH].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_FLASH2FLASH].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_FLASH2FLASH].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2FLASH].cmd_func = Cmd_BD_SET_COPY_SD2FLASH;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2FLASH].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2FLASH].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2FLASH].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2FLASH].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_FLASH2SD].cmd_func = Cmd_BD_SET_COPY_FLASH2SD;
  cmd_table[Cmd_CODE_MEM_SET_COPY_FLASH2SD].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_FLASH2SD].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_FLASH2SD].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_FLASH2SD].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_ERASE_FLASH].cmd_func = Cmd_BD_SET_ERASE_FLASH;
  cmd_table[Cmd_CODE_MEM_SET_ERASE_FLASH].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_ERASE_FLASH].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_FLASH_CRC].cmd_func = Cmd_BD_SET_FLASH_CRC;
  cmd_table[Cmd_CODE_MEM_SET_FLASH_CRC].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_FLASH_CRC].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_SD_CRC].cmd_func = Cmd_BD_SET_SD_CRC;
  cmd_table[Cmd_CODE_MEM_SET_SD_CRC].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_SD_CRC].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_SD_CRC].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2RAM].cmd_func = Cmd_BD_SET_COPY_SD2RAM;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2RAM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2RAM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2RAM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_COPY_SD2RAM].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_RAM_CRC].cmd_func = Cmd_BD_SET_RAM_CRC;
  cmd_table[Cmd_CODE_MEM_SET_RAM_CRC].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_RAM_CRC].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_BD_EXEC_JOB].cmd_func = Cmd_BD_EXEC_JOB;
  cmd_table[Cmd_CODE_BD_EXEC_JOB].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BD_ABORT_JOB].cmd_func = Cmd_BD_ABORT_JOB;
  cmd_table[Cmd_CODE_BD_ABORT_JOB].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BD_EXEC_JOB_ANYWAY].cmd_func = Cmd_BD_EXEC_JOB_ANYWAY;
  cmd_table[Cmd_CODE_BD_ABORT_JOB_ANYWAY].cmd_func = Cmd_BD_ABORT_JOB_ANYWAY;
  cmd_table[Cmd_CODE_FLASH_DUMP].cmd_func = Cmd_APP_TMD_FLASH_SET_BEGIN_ADR;
  cmd_table[Cmd_CODE_FLASH_DUMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;

  /* - RAM - */
  cmd_table[Cmd_CODE_CDH_RAM_DUMP].cmd_func = Cmd_APP_TMD_RAM_SET_BEGIN_ADR;
  cmd_table[Cmd_CODE_CDH_RAM_DUMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SRAM_WRITE].cmd_func = Cmd_MEM_SRAM_write_data;
  cmd_table[Cmd_CODE_CDH_SRAM_WRITE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SRAM_WRITE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SRAM_WRITE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;

  /* - MRAM&EEPROM - */
  cmd_table[Cmd_CODE_MEM_MRAM_EEPROM_WRITE].cmd_func = Cmd_MEM_mram_writeData;
  cmd_table[Cmd_CODE_MEM_MRAM_EEPROM_WRITE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_MRAM_EEPROM_WRITE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_MRAM_EEPROM_WRITE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_MEM_DEBUG_MRAM_READ].cmd_func = Cmd_MEM_mram_ReadData;
  cmd_table[Cmd_CODE_MEM_DEBUG_MRAM_READ].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_DEBUG_MRAM_READ].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_DEBUG_ME_MODE_CHANGE].cmd_func = Cmd_MEM_mram_eeprom_mode_change;
  cmd_table[Cmd_CODE_MEM_DEBUG_ME_MODE_CHANGE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_MRAM_SET_PATROLADDR].cmd_func = Cmd_MEM_mram_set_patrol_addr;
  cmd_table[Cmd_CODE_MEM_MRAM_SET_PATROLADDR].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_EEPROM_SET_PATROLADDR].cmd_func = Cmd_MEM_eeprom_set_patrol_addr;
  cmd_table[Cmd_CODE_MEM_EEPROM_SET_PATROLADDR].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_EEPROM_RESET].cmd_func = Cmd_MEM_eeprom_reset;
  cmd_table[Cmd_CODE_MEM_EEPROM_WRITE].cmd_func = Cmd_MEM_eeprom_writeData;
  cmd_table[Cmd_CODE_MEM_EEPROM_WRITE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_EEPROM_WRITE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_MEM_EEPROM_READ].cmd_func = Cmd_MEM_eeprom_ReadData;
  cmd_table[Cmd_CODE_MEM_EEPROM_READ].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_EEPROM_READ].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MEM_EEPROM_DUMP].cmd_func = Cmd_APP_TMD_EEPROM_SET_BEGIN_ADR;
  cmd_table[Cmd_CODE_MEM_EEPROM_DUMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_MRAM_DUMP].cmd_func = Cmd_APP_TMD_MRAM_SET_BEGIN_ADR;
  cmd_table[Cmd_CODE_MEM_MRAM_DUMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_EEPROM3_DUMP].cmd_func = Cmd_APP_TMD_EEPROM3_SET_BEGIN_ADR;
  cmd_table[Cmd_CODE_MEM_EEPROM3_DUMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_MRAM3_DUMP].cmd_func = Cmd_APP_TMD_MRAM3_SET_BEGIN_ADR;
  cmd_table[Cmd_CODE_MEM_MRAM3_DUMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;

  /* - SPIPROM - */
  cmd_table[Cmd_CODE_CDH_SPIPROM_WRITE].cmd_func = Cmd_MEM_spiprom_write_data;
  cmd_table[Cmd_CODE_CDH_SPIPROM_WRITE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_WRITE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_WRITE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_CDH_SPIPROM_ERASE_SECTORS].cmd_func = Cmd_MEM_spiprom_erase_Sectors;
  cmd_table[Cmd_CODE_CDH_SPIPROM_ERASE_SECTORS].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_ERASE_SECTORS].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_READ_PAGE].cmd_func = Cmd_MEM_spiprom_read_page;
  cmd_table[Cmd_CODE_CDH_SPIPROM_READ_PAGE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_WRITE_BUF_DATA].cmd_func = Cmd_MEM_spiprom_write_buffer_data;
  cmd_table[Cmd_CODE_CDH_SPIPROM_WRITE_BUF_DATA].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_ALL_RESET].cmd_func = Cmd_MEM_spiprom_ALL_Reset;
  cmd_table[Cmd_CODE_CDH_SPIPROM_ALL_RESET].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_RPTLM_GENERATE].cmd_func = Cmd_MEM_spiprom_Generate_RPTLM;
  cmd_table[Cmd_CODE_CDH_SPIPROM_RPTLM_GENERATE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_RPTLM_GENERATE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_RPTLM_GENERATE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_RPTLM_GENERATE].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_RPTLM_GENERATE].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_DUMP].cmd_func = Cmd_APP_TMD_SPIPROM_SET_BEGIN_ADR;
  cmd_table[Cmd_CODE_CDH_SPIPROM_DUMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM3_DUMP].cmd_func = Cmd_APP_TMD_SPIPROM3_SET_BEGIN_ADR;
  cmd_table[Cmd_CODE_CDH_SPIPROM3_DUMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CDH_SPIPROM_MEMORY_PATROL].cmd_func = Cmd_MEM_SPIPROM_memory_patrol;
  cmd_table[Cmd_CODE_MEM_SD_SET_PATROLPAGE].cmd_func = Cmd_MEM_SPIPROM_set_patrol_block;
  cmd_table[Cmd_CODE_MEM_SD_SET_PATROLPAGE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;


/* ----- MOBC ORIGINAL ----- */
/* - MODE - */
  cmd_table[Cmd_CODE_SAME_TL_MODE_TRANS].cmd_func = Cmd_SAME_TL_MODE_TRANS;
  cmd_table[Cmd_CODE_SAME_TL_MODE_TRANS].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_CHANGE_MODE_FLAG].cmd_func = Cmd_CAHNGE_MODEFLAG_SEPARATELY;
  cmd_table[Cmd_CODE_CHANGE_MODE_FLAG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_MODE_FLAG].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_MODE_FLAG].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_MODE_FLAG].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_MODE_FLAG].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_ACS_FLAG].cmd_func = Cmd_CAHNGE_ACS_MODEFLAG;
  cmd_table[Cmd_CODE_CHANGE_ACS_FLAG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_ACS_FLAG].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_ADS_FLAG].cmd_func = Cmd_CAHNGE_ADS_MODEFLAG;
  cmd_table[Cmd_CODE_CHANGE_ADS_FLAG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_ADS_FLAG].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_OCS_FLAG].cmd_func = Cmd_CAHNGE_OCS_MODEFLAG;
  cmd_table[Cmd_CODE_CHANGE_OCS_FLAG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_OCS_FLAG].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_ODS_FLAG].cmd_func = Cmd_CAHNGE_ODS_MODEFLAG;
  cmd_table[Cmd_CODE_CHANGE_ODS_FLAG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CHANGE_ODS_FLAG].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MODE_AUTO_TRANS].cmd_func = Cmd_MODE_AUTO_Transition;
  cmd_table[Cmd_CODE_AUTO_TRANS_FLAG_SETTING].cmd_func = Cmd_MODE_auto_flag_setting;
  cmd_table[Cmd_CODE_AUTO_TRANS_FLAG_SETTING].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PHASE_TRANS_TEMP].cmd_func = Cmd_chage_phase_temp;
  cmd_table[Cmd_CODE_PHASE_TRANS_TEMP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PHASE_TRANS].cmd_func = Cmd_mode_transition_tabel_load;
  cmd_table[Cmd_CODE_DEBUG_STTING_ECLIPSE_WEIGHT].cmd_func = Cmd_Debug_setting_eclipse_weight;
  cmd_table[Cmd_CODE_DEBUG_STTING_ECLIPSE_WEIGHT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_DEBUG_STTING_ECLIPSE_WEIGHT].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_DEBUG_STTING_ECLIPSE_WEIGHT].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;

	/* --- COM --- */
/* - RXPIC - */
  cmd_table[Cmd_CODE_COM_RXPIC_RESET_SEQ].cmd_func = Cmd_RXPIC_seq_reset;
  cmd_table[Cmd_CODE_COM_MESSAGE_FROM_GS].cmd_func = COM_message_from_GS;
  cmd_table[Cmd_CODE_COM_MESSAGE_FROM_GS].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_MESSAGE_FROM_GS].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_MESSAGE_FROM_GS].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_MESSAGE_FROM_SAT].cmd_func = COM_message_from_SAT;
  cmd_table[Cmd_CODE_COM_MESSAGE_FROM_SAT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_MESSAGE_FROM_SAT].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_MESSAGE_FROM_SAT].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;

/* - TXPIC - */
  cmd_table[Cmd_CODE_COM_MESSAGE_TO_GS].cmd_func = COM_message_to_GS;
  cmd_table[Cmd_CODE_COM_MESSAGE_TO_GS].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_MESSAGE_TO_GS].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_MESSAGE_TO_GS].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_MESSAGE_TO_SAT].cmd_func = COM_message_to_SAT;
  cmd_table[Cmd_CODE_COM_MESSAGE_TO_SAT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_MESSAGE_TO_SAT].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_MESSAGE_TO_SAT].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_CHANGE_COMMON].cmd_func = Cmd_change_large_packet_common_header;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_CHANGE_COMMON].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_CHANGE_COMMON].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_CHANGE_COMMON].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_CHANGE_CMD].cmd_func = Cmd_change_large_packet_cmd_header;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_CHANGE_CMD].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_CHANGE_TLM].cmd_func = Cmd_change_large_packet_tlm_header;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_CHANGE_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_CHANGE_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_CHANGE_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_ENABLE].cmd_func = Cmd_large_packet_enable;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_ENABLE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_SRAM_ANALYZE].cmd_func = Cmd_large_packet_SRAM_analyze;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_SRAM_ANALYZE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_SRAM_ANALYZE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_SRAM_ANALYZE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_SCAM_PACKET_DOWNLINK].cmd_func = Cmd_COM_scam_downlink;
  cmd_table[Cmd_CODE_COM_SCAM_PACKET_DOWNLINK].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_SCAM_PACKET_DOWNLINK].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_RESEND].cmd_func = Cmd_COM_picture_data_resend;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_RESEND].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_COM_LARGE_PACKET_RESEND].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_DEV_REGISTER_MSG].cmd_func = Cmd_CDH_develop_register_msg;
  cmd_table[Cmd_CODE_COM_DEV_REGISTER_MSG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_COM_DEV_REGISTER_MSG].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_DEV_SEND_TXPIC].cmd_func = Cmd_CDH_develop_msg_send_txpic;
  cmd_table[Cmd_CODE_COM_DEV_SEND_TXPIC].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_COM_DEV_SEND_TXPIC].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;

/* - COBC - */
#ifdef TWO_U
  cmd_table[Cmd_CODE_COM_COBC_SEND_UART].cmd_func = cobc_dispatch_command;
  cmd_table[Cmd_CODE_COM_COBC_SEND_COMMAND].cmd_func = Cmd_cobc_Transmit_command;
  cmd_table[Cmd_CODE_COM_COBC_SEND_COMMAND].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_COBC_BOOT].cmd_func = Cmd_COBC_BOOT;
  cmd_table[Cmd_CODE_COM_COBC_SEND_INFO].cmd_func = Cmd_COBC_SEND_INFORMATION;
  cmd_table[Cmd_CODE_COM_COBC_SEND_INFO].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_COBC_CHOOSE_INFO].cmd_func = Cmd_COBC_Choose_Info;
  cmd_table[Cmd_CODE_COM_COBC_CHOOSE_INFO].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_COBC_SEND_AOCS_PARAM].cmd_func = Cmd_COBC_send_AOCS;
  cmd_table[Cmd_CODE_COM_COBC_RECEIVE_ENABLE].cmd_func = Cmd_COBC_large_packet_mode_enable;
  cmd_table[Cmd_CODE_COM_COBC_RECEIVE_ENABLE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_COBC_LARGE_PACKET].cmd_func = Cmd_COBC_large_packet_mode;
  cmd_table[Cmd_CODE_COM_COBC_LARGE_PACKET].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_COBC_ALL_RESET].cmd_func = Cmd_COBC_large_packet_all_reset;
  cmd_table[Cmd_CODE_COM_COBC_PICTURE_DOWNLINK].cmd_func = Cmd_COBC_large_packet_downlink;
  cmd_table[Cmd_CODE_COM_COBC_PICTURE_DOWNLINK].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_COBC_PICTURE_DOWNLINK].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_COBC_GPIO_IN].cmd_func = Cmd_COBC_GPIO_in; //added by Osaki (MAGNARO-II)
  cmd_table[Cmd_CODE_COM_COBC_GPIO_OUT].cmd_func = Cmd_COBC_GPIO_out; //added by Osaki (MAGNARO-II)
  cmd_table[Cmd_CODE_COM_COBC_GPIO_OUT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_COBC_GPIO_OUT].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_COBC_RESET].cmd_func = Cmd_COBC_Reset;//added by kobayashi


  #endif


/* - SOBC - */
  cmd_table[Cmd_CODE_COM_SOBC_DEBUG_SEND].cmd_func = Cmd_SOBC_DEBUG_SPI_Send;
  cmd_table[Cmd_CODE_COM_SOBC_RESET].cmd_func = Cmd_SOBC_Reset;
  cmd_table[Cmd_CODE_COM_SOBC_DEBUG_GPIO].cmd_func = Cmd_DEBUG_GPIO;
  cmd_table[Cmd_CODE_COM_SOBC_DEBUG_CMD_SEND].cmd_func = sobc_debug_command_send;
  cmd_table[Cmd_CODE_COM_SOBC_DEBUG_CMD_SEND].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
//  cmd_table[Cmd_CODE_COM_SOBC_DEBUG_NOP].cmd_func = Cmd_DEBUG_regular_nop;
//  cmd_table[Cmd_CODE_COM_SOBC_DEBUG_NOP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_SOBC_DEBUG_REGULAR].cmd_func = Cmd_DEBUG_regular_tlm_on;
  cmd_table[Cmd_CODE_COM_SOBC_DEBUG_REGULAR].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_SOBC_DEBUG_ANOMALY_COUNT].cmd_func = Cmd_DEBUG_anomaly_count;

/* - LORA - */
  cmd_table[Cmd_CODE_COM_LORA_SEND_COMMAND].cmd_func = Cmd_COM_LoRa_Debug_Transmit;
  cmd_table[Cmd_CODE_COM_LORA_SEND_COMMAND].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_LORA_SET].cmd_func = Cmd_COM_LoRa_Debug_Set;
  cmd_table[Cmd_CODE_COM_LORA_SET].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LORA_SET].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LORA_CHECK].cmd_func = Cmd_COM_LoRa_Debug_Check;
  cmd_table[Cmd_CODE_COM_LORA_CHECK].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LORA_DEFAULT_SETTING].cmd_func = Cmd_COM_LoRa_default_setting;
  cmd_table[Cmd_CODE_COM_LORA_USER_RTC_SEND].cmd_func = Cmd_COM_user_RTCmd_send;
  cmd_table[Cmd_CODE_COM_LORA_USER_RTC_SEND].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LORA_USER_RTC_SEND].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LORA_USER_RTC_SEND].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LORA_USER_RTC_SEND].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_COM_LORA_USER_RTC_SEND].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_LORA_USER_TLC_SEND].cmd_func = Cmd_COM_user_TLCmd_send;
  cmd_table[Cmd_CODE_COM_LORA_USER_TLC_SEND].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LORA_USER_TLC_SEND].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LORA_USER_TLC_SEND].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_LORA_USER_TLC_SEND].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_COM_LORA_USER_TLC_SEND].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_COM_LORA_USER_TLC_SEND].param_size_infos[2].packed_info.bit.second = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_COM_LORA_TLM_UPDATE].cmd_func = Cmd_COM_LoRa_tlm_update;

/* - INTERSAT - */
  cmd_table[Cmd_CODE_COM_INTERSAT_SEND_COMMAND].cmd_func = Cmd_intersat_Transmit_command;
  cmd_table[Cmd_CODE_COM_INTERSAT_SEND_COMMAND].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;

/* - COM Anomaly - */
  cmd_table[Cmd_CODE_AH_COMPORT_ANOMALY_ENABLE].cmd_func=Cmd_ComPort_ANOMALY_Set_ENABLE;
  cmd_table[Cmd_CODE_AH_COMPORT_ANOMALY_ENABLE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_AH_COMPORT_ANOMALY_ENABLE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_AH_COMPORT_ANOMALY_DISABLE].cmd_func=Cmd_ComPort_ANOMALY_Reset_ENABLE;
  cmd_table[Cmd_CODE_AH_COMPORT_ANOMALY_DISABLE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_AH_COMPORT_ANOMALY_DISABLE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_AH_COMPORT_ANOMALY_PORTRESET].cmd_func=Cmd_ComPort_ANOMALY_Port_Reset;
  cmd_table[Cmd_CODE_AH_COMPORT_ANOMALY_PORTRESET].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_AH_COMPORT_ANOMALY_PORTRESET].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_AH_UART_RESET_LV1].cmd_func=Cmd_OBC_UART_Reset_LV1;
  cmd_table[Cmd_CODE_AH_UART_RESET_LV2].cmd_func=Cmd_OBC_UART_Reset_LV2;
  cmd_table[Cmd_CODE_AH_UART_RESET_LV3].cmd_func=Cmd_OBC_UART_Reset_LV3;
  cmd_table[Cmd_CODE_AH_I2C_RESET].cmd_func=Cmd_OBC_I2C_Reset;
  cmd_table[Cmd_CODE_AH_SPI_RESET].cmd_func=Cmd_OBC_SPI_Reset;
  cmd_table[Cmd_CODE_AH_COM_IOEX_RESET].cmd_func=Cmd_IOEX_Reset;
#ifdef ONE_U
  cmd_table[Cmd_CODE_AH_I2C_OFF].cmd_func=Cmd_OBC_I2C_OFF;
#endif


/* ----- PWR ----- */
/* - GPIO MOBC - */
  cmd_table[Cmd_CODE_PWR_GPS_ON_OFF].cmd_func = Cmd_PWR_GPS_ON_OFF;
  cmd_table[Cmd_CODE_PWR_GPS_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_SUNS_ON_OFF].cmd_func = Cmd_PWR_SUNS_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SUNS_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_LORA_ON_OFF_ONINIT].cmd_func = Cmd_PWR_LORA_ON_OFF_ONInit;
  cmd_table[Cmd_CODE_PWR_LORA_ON_OFF_ONINIT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_REG_CDH3V3_ON_OFF].cmd_func = Cmd_PWR_REG_CDH3V3_ON_OFF;
  cmd_table[Cmd_CODE_PWR_REG_CDH3V3_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_IOEX_PWR2_ON_OFF_ONINIT].cmd_func = Cmd_PWR_IOEX_PWR2_ON_OFF_ONInit;
  cmd_table[Cmd_CODE_PWR_IOEX_PWR2_ON_OFF_ONINIT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_MTQA_ON_OFF_ONINIT].cmd_func = Cmd_PWR_MTQA_ON_OFF_ONInit;
  cmd_table[Cmd_CODE_PWR_MTQA_ON_OFF_ONINIT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_MTQO_PLASMA_ON_OFF_ONINIT].cmd_func = Cmd_PWR_MTQO_PLASMA_ON_OFF_ONInit;
  cmd_table[Cmd_CODE_PWR_MTQO_PLASMA_ON_OFF_ONINIT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_LORA_AUTO_ON].cmd_func = Cmd_PWR_LORA_AUTO_ONInit;

/* - GPIO IOEX PWR2 - */
  cmd_table[Cmd_CODE_PWR_SUNS_MXPZ_ON_OFF].cmd_func = Cmd_PWR_SUNS_MXPZ_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SUNS_MXPZ_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_SUNS_PXPYMY_ON_OFF].cmd_func = Cmd_PWR_SUNS_PXPYMY_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SUNS_PXPYMY_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_GYRO_ON_OFF].cmd_func = Cmd_PWR_GYRO_ON_OFF;
  cmd_table[Cmd_CODE_PWR_GYRO_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_SEMP_ON_OFF].cmd_func = Cmd_PWR_SEMP_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SEMP_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_SEPCOIL1_ON_OFF].cmd_func = Cmd_PWR_SEPCOIL1_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SEPCOIL1_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_SEPCOIL2_ON_OFF].cmd_func = Cmd_PWR_SEPCOIL2_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SEPCOIL2_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_SEPCOIL3_ON_OFF].cmd_func = Cmd_PWR_SEPCOIL3_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SEPCOIL3_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_SEPCOIL4_ON_OFF].cmd_func = Cmd_PWR_SEPCOIL4_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SEPCOIL4_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_MIS1_ON_OFF_ONRESET].cmd_func = Cmd_PWR_MIS1_ON_OFF_ONReset;
  cmd_table[Cmd_CODE_PWR_MIS1_ON_OFF_ONRESET].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
#ifdef TWO_U
  cmd_table[Cmd_CODE_PWR_MAGS_ON_OFF].cmd_func = Cmd_PWR_MAGS_ON_OFF;
  cmd_table[Cmd_CODE_PWR_MAGS_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
#endif
  cmd_table[Cmd_CODE_PWR_IOEX_PWR2_INIT].cmd_func = Cmd_PWR_IOEX_PWR2_Init;

	/* - PWR Anomaly handling - */
  cmd_table[Cmd_CODE_PWR_AH_OC].cmd_func = Cmd_PWR_AH_OC;
  cmd_table[Cmd_CODE_PWR_AH_UC].cmd_func = Cmd_PWR_AH_UC;
  cmd_table[Cmd_CODE_PWR_AH_OV].cmd_func = Cmd_PWR_AH_OV;
  cmd_table[Cmd_CODE_PWR_AH_UV].cmd_func = Cmd_PWR_AH_UV;
  cmd_table[Cmd_CODE_PWR_AH_PSI_ON].cmd_func = Cmd_PWR_AH_PSI_ON;
  cmd_table[Cmd_CODE_PWR_AH_PSI_OFF].cmd_func = Cmd_PWR_AH_PSI_OFF;
  cmd_table[Cmd_CODE_PWR_AH_SYSTEM_ENABLE].cmd_func = Cmd_PWR_AH_SYSTEM_ENABLE;

/* - SEPCOIL Guard - */
  cmd_table[Cmd_CODE_PWR_SEPCOIL1_GUARD_ON_OFF].cmd_func = Cmd_PWR_SEPCOIL1_GUARD_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SEPCOIL1_GUARD_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_SEPCOIL2_GUARD_ON_OFF].cmd_func = Cmd_PWR_SEPCOIL2_GUARD_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SEPCOIL2_GUARD_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_SEPCOIL3_GUARD_ON_OFF].cmd_func = Cmd_PWR_SEPCOIL3_GUARD_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SEPCOIL3_GUARD_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_SEPCOIL4_GUARD_ON_OFF].cmd_func = Cmd_PWR_SEPCOIL4_GUARD_ON_OFF;
  cmd_table[Cmd_CODE_PWR_SEPCOIL4_GUARD_ON_OFF].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;

	/* - ADC_MOBC - */
  cmd_table[Cmd_CODE_PWR_ADC_MOBC_READ].cmd_func = Cmd_PWR_ADC_MOBC_Read;
  cmd_table[Cmd_CODE_PWR_ADC_MOBC_READ].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;

	/* - GPIO_input_MOBC - */
  cmd_table[Cmd_CODE_PWR_GPIO_INPUT_MOBC_READ].cmd_func = Cmd_PWR_GPIO_input_MOBC_Read;
  cmd_table[Cmd_CODE_PWR_GPIO_INPUT_MOBC_READ].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;

	/* - ADC_PWR2 - */
  cmd_table[Cmd_CODE_PWR_ADC_PWR2_INIT].cmd_func = Cmd_PWR_ADC_PWR2_Init;
  cmd_table[Cmd_CODE_PWR_ADC_PWR2_READ].cmd_func = Cmd_PWR_ADC_PWR2_Read;
  cmd_table[Cmd_CODE_PWR_ADC_PWR2_READ].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;

  	/* - ADC_ACS1 - */
#ifdef TWO_U
  cmd_table[Cmd_CODE_PWR_ACS1_CURS_READ].cmd_func = Cmd_ACS1_debug_CURS;
  cmd_table[Cmd_CODE_PWR_ACS1_CURS_READ].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_PWR_ACS1_VOLS_READ].cmd_func = Cmd_ACS1_debug_VOLS;
  cmd_table[Cmd_CODE_PWR_ACS1_VOLS_READ].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
#endif

	/* ----- ACOS Sensor ----- */
	/* - TEMP - */
#ifdef TWO_U
  cmd_table[Cmd_CODE_ACS_DEBUG_PT_TEMS].cmd_func = Cmd_ACS1_debug_PT_TEMS;    //masuda add
  cmd_table[Cmd_CODE_ACS_DEBUG_PT_TEMS].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
#endif

	/* - GYRO - */
#ifdef TWO_U
  cmd_table[Cmd_CODE_ACS_DEBUG_GYRO].cmd_func = Cmd_ACS1_debug_GYRO;
  cmd_table[Cmd_CODE_ACS_DEBUG_GYRO].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_CHANGE_GYRO_RANGE].cmd_func = Cmd_ACS1_Change_GYRO_Range;
  cmd_table[Cmd_CODE_ACS_CHANGE_GYRO_RANGE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_CHANGE_GYRO_RANGE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_CHANGE_GYRO_RANGE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
#endif

#ifdef ONE_U
  cmd_table[Cmd_CODE_ACS_DEBUG_GYRO].cmd_func = Cmd_ACS3_debug_gyro_read;
  cmd_table[Cmd_CODE_ACS_DEBUG_GYRO].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_CHANGE_GYRO_RANGE].cmd_func = Cmd_ACS3_Change_GYRO_Range;
  cmd_table[Cmd_CODE_ACS_CHANGE_GYRO_RANGE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_CHANGE_GYRO_RANGE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_CHANGE_GYRO_RANGE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
#endif

/* - MAGS - */
#ifdef TWO_U
  cmd_table[Cmd_CODE_ACS_DEBUG_MAGS].cmd_func = Cmd_ACS1_debug_MAGS;
  cmd_table[Cmd_CODE_ACS_DEBUG_MAGS].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
#endif
	/* - SUNS - */

	/* - 9AX - */
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_ACC_RANGE].cmd_func = Cmd_9AX_change_acc_config;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_ACC_RANGE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_ACC_RANGE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_ACC_RANGE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_ACC_RANGE].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_GYRO_RANGE].cmd_func = Cmd_9AX_change_gyro_config;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_GYRO_RANGE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_GYRO_RANGE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_GYRO_RANGE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_GYRO_RANGE].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_MAG_RANGE].cmd_func = Cmd_9AX_change_mag_config;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_MAG_RANGE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_MAG_RANGE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_MAG_RANGE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_9AX_CHANGE_MAG_RANGE].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;

  	/* - GPS - */
  cmd_table[Cmd_CODE_ACS_GPS_SET_RTC].cmd_func = Cmd_ACS13_GPS_SET_RTC_BY_UTC;

	/* - MTQ - */
  cmd_table[Cmd_CODE_ACS_MTQ_ZERO].cmd_func = Cmd_ACS23_MTQ_Output_Zero;
  cmd_table[Cmd_CODE_ACS_MTQ_RETURN].cmd_func = Cmd_ACS23_MTQ_Output_Return;
  cmd_table[Cmd_CODE_ACS_DEBUG_MTQA].cmd_func = Cmd_ACS23_debug_MTQA_Function_core;
  cmd_table[Cmd_CODE_ACS_DEBUG_MTQA].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_DEBUG_MTQA].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_DEBUG_MTQA].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_DEBUG_MTQA].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_AH_MTQA].cmd_func = Cmd_ACS23_AH_MTQA_OT;

#ifdef TWO_U
  cmd_table[Cmd_CODE_ACS_DEBUG_MTQO].cmd_func = Cmd_ACS23_debug_MTQO_Function_core;
  cmd_table[Cmd_CODE_ACS_DEBUG_MTQO].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_DEBUG_MTQO].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_ACS_AH_MTQO].cmd_func = Cmd_ACS23_AH_MTQO_OT;
#endif

	/* - PLASMA - */
#ifdef TWO_U
  cmd_table[Cmd_CODE_ACS_DEBUG_PLASMA].cmd_func = Cmd_ACS1_debug_PLASMA;
  cmd_table[Cmd_CODE_ACS_DEBUG_PLASMA].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
#endif
  cmd_table[Cmd_CODE_ACS_PLASMA_CHANGE_VOLTAGE].cmd_func = Cmd_PLASMA_change_voltage;
  cmd_table[Cmd_CODE_ACS_PLASMA_CHANGE_VOLTAGE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_ACS_PLASMA_CHANGE_VOLTAGE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_ACS_PLASMA_CHANGE_VOLTAGE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_ACS_PLASMA_CHANGE_VOLTAGE].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_ACS_PLASMA_CHANGE_VOLTAGE].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;

	/* - SCAM - */
#ifdef ONE_U
  cmd_table[Cmd_CODE_ACS_SUBCAM].cmd_func = Cmd_ACS3_SUBCAM;
  cmd_table[Cmd_CODE_ACS_SUBCAM_INIT].cmd_func = Cmd_ACS3_SUBCAM_initialize;
  cmd_table[Cmd_CODE_ACS_SUBCAM_TAKE_PIC].cmd_func = Cmd_ACS3_SUBCAM_take_pic;
  cmd_table[Cmd_CODE_ACS_SUBCAM_READ_REG].cmd_func = Cmd_ACS3_SUBCAM_read_reg;
  cmd_table[Cmd_CODE_COM_IMAGETLM].cmd_func = Cmd_SendImageTLM_TXPIC;
  cmd_table[Cmd_CODE_COM_IMAGETLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_COM_IMAGETLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_MEM_SD_CAMADDR_RENEW].cmd_func = Cmd_CAMaddr_Renew;
  cmd_table[Cmd_MEM_SD_CAMADDR_RENEW].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_MEM_SD_CAMADDR_RENEW].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_MEM_SD_CONTENTS_INIT].cmd_func = SDcontents_init;
  cmd_table[Cmd_CODE_ACS_SUBCAM_RE_INIT].cmd_func = Cmd_ACS3_SUBCAM_re_initialize;
  cmd_table[Cmd_CODE_ACS_SUBCAM_DEFAULT_SETTING].cmd_func = Cmd_ACS3_SUBCAM_default_setting;
  cmd_table[Cmd_CODE_ACS_SUBCAM_SD_TO_SRAM].cmd_func = Cmd_ACS3_SUBCAM_SD_to_SRAM;
  cmd_table[Cmd_CODE_ACS_SUBCAM_SD_TO_SRAM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
#endif

	/* ----- AOCS ----- */
	/* - ADS - */
  cmd_table[Cmd_CODE_ADS_AD_EKF].cmd_func = Cmd_ADS_AD_EKF;

	/* - ACS - */
  cmd_table[Cmd_CODE_ACS_COMPENSATION_SHAPEANISORTROPY].cmd_func = Cmd_ACS_Compesation_ShapeAnisortropy;
  cmd_table[Cmd_CODE_ACS_COMPENSATION_SHAPEANISORTROPY].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_ACS_CrossProduct_SPIN_HILS].cmd_func = Cmd_ACS_CrossProduct_SpinControl_HILS;

/* - ODS -*/

/* - OCS -*/


  cmd_table[Cmd_CODE_TMGR_SET_TIME].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_TMGR_UPDATE_UNIXTIME].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_8BYTE;
  cmd_table[Cmd_CODE_TMGR_UPDATE_UNIXTIME].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_TMGR_UPDATE_UNIXTIME].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_8BYTE;
  cmd_table[Cmd_CODE_TMGR_SET_CYCLE_CORRECTION].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_8BYTE;
  cmd_table[Cmd_CODE_AM_REGISTER_APP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_AM_REGISTER_APP].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_AM_REGISTER_APP].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_AM_INITIALIZE_APP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_AM_EXECUTE_APP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_AM_SET_PAGE_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MM_SET_MODE_LIST].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MM_SET_MODE_LIST].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_MM_SET_TRANSITION_TABLE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MM_SET_TRANSITION_TABLE].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_MM_SET_TRANSITION_TABLE].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_MM_START_TRANSITION].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TDSP_SET_TASK_LIST].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLCD_CLEAR_TIMELINE_AT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLCD_CLEAR_TIMELINE_AT].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_TLCD_DEPLOY_BLOCK].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLCD_DEPLOY_BLOCK].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TLCD_CLEAR_ERR_LOG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLCD_SET_SOE_FLAG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLCD_SET_SOE_FLAG].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLCD_SET_LOUT_FLAG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLCD_SET_LOUT_FLAG].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLCD_SET_ID_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLCD_SET_PAGE_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_GENERATE_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_GENERATE_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_GENERATE_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_GENERATE_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_GENERATE_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_GENERATE_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_GENERATE_TLM].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_GENERATE_HP_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_GENERATE_RT_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_GENERATE_ST_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_GENERATE_ST_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_TLM].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_TLM].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_AS_HP_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_AS_HP_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_AS_RT_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_AS_RT_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_AS_ST_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_AS_ST_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TG_FORWARD_AS_ST_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BCT_CLEAR_BLOCK].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_SET_BLOCK_POSITION].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_SET_BLOCK_POSITION].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BCT_COPY_BCT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_COPY_BCT].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_OVERWRITE_CMD].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_OVERWRITE_CMD].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_BCT_OVERWRITE_CMD].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCT_OVERWRITE_CMD].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BCT_OVERWRITE_CMD].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_BCT_FILL_NOP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BCE_ACTIVATE_BLOCK_BY_ID].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCE_INACTIVATE_BLOCK_BY_ID].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCE_ROTATE_BLOCK].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCE_COMBINE_BLOCK].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCE_TIMELIMIT_COMBINE_BLOCK].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCE_TIMELIMIT_COMBINE_BLOCK].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_BCE_SET_ROTATE_INTERVAL].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_BCE_SET_ROTATE_INTERVAL].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_MEM_SET_REGION].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_SET_REGION].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_LOAD].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_LOAD].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_MEM_SET_DESTINATION].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_MEM_COPY_REGION_SEQ].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EL_CLEAR_LOG_BY_ERR_LEVEL].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EL_CLEAR_TLOG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EL_CLEAR_CLOG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EL_RECORD_EVENT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EL_RECORD_EVENT].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EL_RECORD_EVENT].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EL_RECORD_EVENT].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EL_TLOG_SET_PAGE_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EL_TLOG_SET_PAGE_FOR_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EL_CLOG_SET_PAGE_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EL_CLOG_SET_PAGE_FOR_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EL_ENABLE_LOGGING].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EL_DISABLE_LOGGING].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EL_ENABLE_TLOG_OVERWRITE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EL_DISABLE_TLOG_OVERWRITE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_EVENT_PARAM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_EVENT_PARAM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_EVENT_PARAM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_EVENT_PARAM].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_EVENT_PARAM].param_size_infos[2].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_EVENT_PARAM].param_size_infos[2].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_CONDITION_PARAM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_CONDITION_PARAM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_CONDITION_PARAM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EH_SET_REGISTER_RULE_CONDITION_PARAM].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_DELETE_RULE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_INIT_RULE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_INIT_RULE_FOR_MULTI_LEVEL].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_ACTIVATE_RULE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_INACTIVATE_RULE].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_SET_RULE_COUNTER].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_SET_RULE_COUNTER].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_CLEAR_RULE_COUNTER].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_CLEAR_RULE_COUNTER_BY_EVENT].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EH_CLEAR_RULE_COUNTER_BY_EVENT].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EH_CLEAR_RULE_COUNTER_BY_EVENT].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_SET_MAX_RESPONSE_NUM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_SET_MAX_CHECK_EVENT_NUM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_SET_MAX_MULTI_LEVEL_NUM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_SET_PAGE_OF_RULE_TABLE_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_SET_PAGE_OF_LOG_TABLE_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_EH_INIT_RULE_BY_EVENT_GROUP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EH_INIT_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EH_ACTIVATE_RULE_BY_EVENT_GROUP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EH_INACTIVATE_RULE_BY_EVENT_GROUP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EH_ACTIVATE_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_EH_INACTIVATE_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_TF_REGISTER_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TF_REGISTER_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CA_REGISTER_CMD].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_CA_REGISTER_CMD].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_CA_REGISTER_CMD].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;
  cmd_table[Cmd_CODE_TF_SET_PAGE_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_CA_SET_PAGE_FOR_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_GENERATE_RT_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_GENERATE_RT_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_GENERATE_ST_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_GENERATE_ST_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_GENERATE_ST_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_FORWARD_AS_RT_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_FORWARD_AS_RT_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_FORWARD_AS_RT_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_FORWARD_AS_ST_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_FORWARD_AS_ST_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_FORWARD_AS_ST_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_FORWARD_AS_ST_TLM].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_REPLAY_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_REGISTER_REPLAY_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_GENERATE_RT_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_GENERATE_RT_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_GENERATE_ST_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_GENERATE_ST_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_GENERATE_ST_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_FORWARD_AS_RT_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_FORWARD_AS_RT_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_FORWARD_AS_RT_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_FORWARD_AS_ST_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_FORWARD_AS_ST_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_FORWARD_AS_ST_TLM].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_FORWARD_AS_ST_TLM].param_size_infos[1].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_REPLAY_TLM].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TLM_MGR_DELETE_REPLAY_TLM].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_DCU_ABORT_CMD].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_DCU_DOWN_ABORT_FLAG].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TEST_CCP_REGISTER_TLC_ASAP].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_4BYTE;
  cmd_table[Cmd_CODE_TEST_CCP_GET_RAW_PARAM_INFO].param_size_infos[0].packed_info.bit.first = CA_PARAM_SIZE_TYPE_2BYTE;
  cmd_table[Cmd_CODE_TEST_CCP_GET_RAW_PARAM_INFO].param_size_infos[0].packed_info.bit.second = CA_PARAM_SIZE_TYPE_1BYTE;
  cmd_table[Cmd_CODE_TEST_CCP_GET_RAW_PARAM_INFO].param_size_infos[1].packed_info.bit.first = CA_PARAM_SIZE_TYPE_RAW;

}

#pragma section
