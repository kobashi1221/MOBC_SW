/*
 * wdt_manager.c
 *
 *  Created on: 2022/05/08
 *      Author: SAT37
 */

#include "stdint.h"
#include "wdt_manager.h"
#include <src_core/System/TaskManager/task_dispatcher.h>
#include <src_core/TlmCmd/block_command_executor.h>
#include <src_user/IfWrapper/HAL.h>

static WDTime wdt;
const WDTime* p_wdt = &wdt;

void WDTMNG_count_up(void){
	WDT_count_up(p_wdt);
}

void WDT_count_up(WDTime* time){
	time->count++;

	if(time->count == 3){
		printf("wdt reset\r\n");
		printf("cmd_id = %d\r\n", CCP_get_id(PL_get_head(task_dispathcer->tskd.pl)->packet));
		if(CCP_get_id(PL_get_head(task_dispathcer->tskd.pl)->packet) == Cmd_CODE_BCE_COMBINE_BLOCK){
		  uint16_t bc_id;
		  ENDIAN_memcpy(&bc_id, CCP_get_param_head(PL_get_head(task_dispathcer->tskd.pl)->packet), 2);
		  printf("bc_id = %d\r\n", bc_id);
		}
		while(1){}
	}
}

void WDT_clear_count(void){
	wdt.count = 0;
	HAL_IWDG_Refresh_C2A(&hiwdg);
}
