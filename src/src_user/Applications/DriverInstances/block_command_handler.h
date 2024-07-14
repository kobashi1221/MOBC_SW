/**
 * @file   block_command_handler.h
 * @brief  ブロックコマンドの書き換え関数群
 * @note
 * @Auther Takumi Noro
 */
#ifndef BLOCK_COMMAND_HANDLER_H_
#define BLOCK_COMMAND_HANDLER_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>
#include "../UserDefined/Aocs/Global_AOCS_Parameter.h"
#include "../UserDefined/Mission/Global_Mission_Parameter.h"
#include <src_core/TlmCmd/packet_list.h>
#include <src_core/TlmCmd/block_command_table.h>

typedef struct{
	void (*func)(void);
} ALT_BC_LIST;

typedef struct{
	uint32_t tlm_pwr_mode;
	uint32_t tlm_acs_mode;
	uint32_t tlm_ads_mode;
	uint32_t tlm_ocs_mode;
	uint32_t tlm_ods_mode;
	uint32_t tlm_aocs_driver_mode;
	uint32_t tlm_pwr_mission_mode;
	uint32_t tlm_mission_mode;
}TLM_MODE_FLAG;

typedef struct{
	PWR_STATE_STRUCT pwr_mode;
	AOCS_MODE_STRUCT aocs_mode;
	MISSION_MODE_STRUCT mission_mode;
}MODE_FLAG_STRUCT;

typedef struct{
	uint8_t current_mode_id;//自動遷移を防ぐものとして必要
	cycle_t aocs_trans_time;
}USER_MODE_STRUCT;

extern const int* BCT_page_no;
extern const TLM_MODE_FLAG* p_tlm_mode_flag;
extern USER_MODE_STRUCT* const user_mode;


AppInfo ALT_BCT_APP(void);
AppInfo MODE_DEFINITION_PARAM_APP(void);
AppInfo Global_AOCS_Param_update(void);
AppInfo BCT_Tlm_Page(void);
AppInfo Global_AOCS_flag_app(void);
AppInfo Global_Mission_flag_app(void);

CCP_CmdRet SD_combine_block_cmd(const CommonCmdPacket* packet);
CCP_CmdRet SD_deploy_block_cmd(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCT_CHANGE_ALT_BC(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_SAME_TL_MODE_TRANS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_SAVE_BCT(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_LOAD_BCT(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCT_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCT_SD_SET_CMD_Packet(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCT_SD_SET_CMD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCT_SD_SET_CMD_NUM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CAHNGE_MODEFLAG_SEPARATELY(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CAHNGE_ACS_MODEFLAG(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CAHNGE_ADS_MODEFLAG(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CAHNGE_OCS_MODEFLAG(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CAHNGE_ODS_MODEFLAG(const CommonCmdPacket* packet);
void BCT_SD_initialize(void);
PL_ACK PL_SD_deploy_block_cmd(PacketList* pl, const bct_id_t block, cycle_t start_at);
uint8_t overload_bctable(bct_id_t BC_ID, bct_id_t ALT_BC_ID);

#endif
