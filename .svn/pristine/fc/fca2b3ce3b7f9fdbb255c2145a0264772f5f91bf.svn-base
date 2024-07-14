/**
  *****************************************************************************************
  * @file    : COM1_debug_RXPIC.c
  * @author  : Takumi NORO
  * @version : 1.0
  * @date    : 2020/08/20
  * @brief   :　RXPICとのUART通信用
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/06/23  1.0   T.NORO         First revision
  *
  *****************************************************************************************
  */

#include "COM_cmd_RXPIC.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <string.h>//for memcpy

static TCP tcp;

static void COM_rxpic_clear_count_(void);
static void COM_rxpic_DBG_couter(void);

uint8_t dbg_command_counter;
ObcTime last_excute_time;

AppInfo COM_rxpic_clear_count(void)
{
  return AI_create_app_info("RXPIC_CLEAR_COUNT", NULL, COM_rxpic_clear_count_);
}

AppInfo COM_DBG_counter(void)
{
  return AI_create_app_info("RXPIC_CLEAR_COUNT", NULL, COM_rxpic_DBG_couter);
}

static void COM_rxpic_clear_count_(void){
	if((TMGR_get_master_total_cycle()%3)==0){
		uint8_t length = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN;
#ifdef TWO_U
		CCP_form_rtc_to_other_obc(&tcp, TCP_APID_RXPIC_2U, (CMD_CODE)0x3140,NULL,0);
#endif
#ifdef ONE_U
		CCP_form_rtc_to_other_obc(&tcp, TCP_APID_RXPIC_1U, (CMD_CODE)0x3140,NULL,0);
#endif
		TCP_set_route(&tcp, TCP_ROUTE_DIRECT);
		//生存人号をanalyzeにすると処理できるC2A一周で処理できるpacket(max16)に制限が出るため直接送る
		RXPIC_Send(rxpic_driver,(uint8_t *)&tcp.packet,length);
	}
}

static void COM_rxpic_DBG_couter(void){
	ObcTime now=TMGR_get_master_clock();
	int diff_time = (int) OBCT_diff_in_sec(&last_excute_time, &now);

	if(diff_time > 0x0f){
		dbg_command_counter = 0;
	}
	++dbg_command_counter;
	printf("dbg_command_count is %d \r\n", dbg_command_counter);
	last_excute_time = TMGR_get_master_clock();
}

/************************************** END OF FILE **************************************/
