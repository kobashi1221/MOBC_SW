/*
 * CDH_cmd_AllRepro.h
 *
 *  Created on: 2021/11/16
 *      Author: kahaku
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_CDH_ALL_REPRO_CDH_CMD_ALLREPRO_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_CDH_ALL_REPRO_CDH_CMD_ALLREPRO_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

typedef struct{
	uint16_t last_rec_seq;		//最終受信シーケンスカウント　いらない？
	uint8_t pos;				//プログラム番号
	uint32_t sd_write_addr;
	uint8_t* p_flash;			//FLASHの現在のアドレス
	uint32_t prog_size;			//program size
}ALLREPRO_STRUCT;

CCP_CmdRet Cmd_CDH_debug_Write_Program_on_FLASH(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH_AllRepro_Program_to_SD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH_AllRepro_set_number(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH_AllRepro_reset_flag(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH_ALLREPRO_SEND_ReceiveFlag(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_CDH_AllRepro_JUMP_NEW(const CommonCmdPacket* packet);

AppInfo CDH_AllRepro_update(void);
AppInfo allrepro_param_update(void);
#endif /* SRC_USER_APPLICATIONS_USERDEFINED_CDH_ALL_REPRO_CDH_CMD_ALLREPRO_H_ */
