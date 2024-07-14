/**
 * @file RTC_handler.c
 * @brief
 */

#include "main.h"
#include "RTC_handler.h"
#include "../../../Drivers/Cdh/RTC/RTC.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>

static void RTC_init(void);
static void RTC_exe(void);

static RTC_Info rtc_struct;
RTC_Info* const p_rtc_struct = &rtc_struct;

AppInfo RTC_update(void)
{
	return AI_create_app_info("rtc", RTC_init, RTC_exe);
}

static void RTC_init(void)
{
	rtc_struct.year = 2022;
	rtc_struct.ms = 0.0f;

	rtc_struct.sTime.Hours = 0;
	rtc_struct.sTime.Minutes = 0;
	rtc_struct.sTime.Seconds = 0;
	rtc_struct.sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	rtc_struct.sTime.StoreOperation = RTC_STOREOPERATION_RESET;

	rtc_struct.sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	rtc_struct.sDate.Month = 1;
	rtc_struct.sDate.Date = 1;
	rtc_struct.sDate.Year = (uint8_t)(rtc_struct.year % 100);

	Set_RTC_Time_Date(p_rtc_struct);
//	printf("RTC %d/%d/%d,%d:%d:%d(%f)\r\n", rtc_struct.sDate.Year, rtc_struct.sDate.Month, rtc_struct.sDate.Date, rtc_struct.sTime.Hours, rtc_struct.sTime.Minutes, rtc_struct.sTime.Seconds, rtc_struct.ms);
//	printf("year = %d, tlm_time = %d\r\n", rtc_struct.year, rtc_struct.tlm_time);
//	printf("unix = %d\r\n", rtc_struct.unix_time);
	return;
}

static void RTC_exe(void)
{
	Get_RTC_Time_Date(p_rtc_struct);
//	printf("RTC %d/%d/%d,%d:%d:%d(%f)\r\n", rtc_struct.sDate.Year, rtc_struct.sDate.Month, rtc_struct.sDate.Date, rtc_struct.sTime.Hours, rtc_struct.sTime.Minutes, rtc_struct.sTime.Seconds, rtc_struct.ms);
//	printf("year = %d, tlm_time = %d\r\n", rtc_struct.year, rtc_struct.tlm_time);
//	printf("unix = %u, %lf\r\n", p_rtc_struct->unix_time, p_rtc_struct->d_unix);
//	printf("%d\r\n", sizeof(rtc_struct.d_unix));

	return;
}

CCP_CmdRet Cmd_RTC_SET(const CommonCmdPacket* packet){
	if(CCP_get_param_len(packet) != 7){
		printf("wrong cmd length\r\n");
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
	}

	uint8_t* param = CCP_get_param_head(packet);

	ENDIAN_memcpy(&rtc_struct.year,   param,     2);
	memcpy(&rtc_struct.sDate.Month,   param + 2, 1);
	memcpy(&rtc_struct.sDate.Date,    param + 3, 1);
	memcpy(&rtc_struct.sTime.Hours,   param + 4, 1);
	memcpy(&rtc_struct.sTime.Minutes, param + 5, 1);
	memcpy(&rtc_struct.sTime.Seconds, param + 6, 1);

	rtc_struct.ms = 0.0f;
	rtc_struct.sDate.Year = (uint8_t)(rtc_struct.year % 100);
	Set_RTC_Time_Date(p_rtc_struct);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//CCP_EXEC_STS Cmd_RTC_calib(const CTCP* packet){
//	Calib_RTC();
//	return CCP_EXEC_SUCCESS;
//}
