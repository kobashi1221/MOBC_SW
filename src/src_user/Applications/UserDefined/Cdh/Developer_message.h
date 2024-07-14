/*
 * Developer_message.h
 *
 *  Created on: 2022/08/16
 *      Author: daiki
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_CDH_DEVELOPER_MESSAGE_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_CDH_DEVELOPER_MESSAGE_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

typedef struct{
	uint8_t len;
	uint8_t msg[63];
}MSG_STRUCT;

CCP_CmdRet Cmd_CDH_develop_register_msg(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH_develop_msg_send_txpic(const CommonCmdPacket* packet);

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_CDH_DEVELOPER_MESSAGE_H_ */
