
#pragma section REPRO
/**
 * @file ResetUtility.c
 * @brief  RQUのDV1など，クリティカル運用時のOBC再起動のためのUtil
 * @author 鈴本 遼
 * @date   2018/06/25
 */
#include "ResetUtility.h"

#include <src_core/TlmCmd/block_command_table.h>
#include <src_core/Applications/timeline_command_dispatcher.h>
#include "../../../TlmCmd/block_command_definitions.h"
#include "../../../Settings/initial_settings.h"
#include "main.h"

static Reset_struct reset_struct;
const Reset_struct* p_reset_struct;

static void RESET_init_(void);

AppInfo RESET_create_app(void)//名大
{
  return AI_create_app_info("reset", RESET_init_, NULL);
}


static void RESET_init_(void)//名大
{
	p_reset_struct = &reset_struct;
	read_param(reset_struct.reset_count, NULL, sizeof(reset_struct.reset_count), (uint32_t)0x000000A0);

	reset_struct.reset_count[reset_cause]++;
	reset_struct.reset_count[TOTAL_RESET]++;

	write_param(reset_struct.reset_count, NULL, sizeof(reset_struct.reset_count), (uint32_t)0x000000A0);
	printf("previous reset is %d\r\n", reset_cause);
	printf("reset count = %08x, %08x, %08x, %08x, %08x, %08x, %08x, %08x, %08x\r\n", reset_struct.reset_count[0], reset_struct.reset_count[1],
			reset_struct.reset_count[2], reset_struct.reset_count[3], reset_struct.reset_count[4], reset_struct.reset_count[5], reset_struct.reset_count[6], reset_struct.reset_count[7], reset_struct.reset_count[8]);
//	printf("reset_count = %08x\r\n", Total_reset_count);
	return;
}

CCP_CmdRet Cmd_RESET_SET_COUNT(const CommonCmdPacket* packet)//名大
{
	uint8_t* param;
	param = CCP_get_param_head(packet);
	uint8_t pos;
	memcpy(&pos, param, 1);
	ENDIAN_memcpy(&(reset_struct.reset_count[pos]), param + 1, 4);

	write_param(&(reset_struct.reset_count[pos]), NULL, 4, (uint32_t)0x000000A0 + 4 * pos);

	printf("reset count = %08x, %08x, %08x, %08x, %08x, %08x, %08x, %08x, %08x\r\n", reset_struct.reset_count[0], reset_struct.reset_count[1],
				reset_struct.reset_count[2], reset_struct.reset_count[3], reset_struct.reset_count[4], reset_struct.reset_count[5], reset_struct.reset_count[6], reset_struct.reset_count[7], reset_struct.reset_count[8]);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
