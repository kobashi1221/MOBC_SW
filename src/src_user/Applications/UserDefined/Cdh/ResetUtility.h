/**
 * @file ResetUtility.h
 * @brief  RQUのDV1など，クリティカル運用時のOBC再起動のためのUtil
 * @author 鈴本 遼
 * @date   2018/06/25
 */
#ifndef RESET_UTILITY_H_
#define RESET_UTILITY_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Drivers/CDH/reset_counter.h>

typedef struct
{
	uint32_t reset_count[TOTAL_RESET + 1];
}
Reset_struct;

extern const Reset_struct* p_reset_struct;


AppInfo RESET_create_app(void);
CCP_CmdRet Cmd_RESET_SET_COUNT(const CommonCmdPacket* packet);


#endif // RESET_UTILITY_H_
