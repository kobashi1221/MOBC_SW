/**
 * @file RTC.c
 * @brief RTCのHALfanction
 */

#include "RTC.h"
#include <src_user/Library/General/DBG_printf.h>
#include "UNIX.h"


void Set_RTC_Time_Date(RTC_Info* rtc_info){//Sets RTC current time
#ifndef HAL_SILS

	rtc_info->sDate.Year = (uint8_t)(rtc_info->year % 100);

	rtc_info->sTime.SubSeconds = rtc_info->sTime.SecondFraction;

	if (HAL_RTC_SetTime(&hrtc, &rtc_info->sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
	printf("RTC SetTime err\r\n");
	}

	rtc_info->ms = (rtc_info->sTime.SecondFraction - rtc_info->sTime.SubSeconds)/(rtc_info->sTime.SecondFraction + 1.0f);

	if (HAL_RTC_SetDate(&hrtc, &rtc_info->sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
	  printf("RTC SetDate err\r\n");
	}

#endif
	return;
}

void Get_RTC_Time_Date(RTC_Info* rtc_info){//Gets RTC current time
#ifndef HAL_SILS

	HAL_RTC_GetTime(&hrtc, &rtc_info->sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &rtc_info->sDate, RTC_FORMAT_BIN);

	rtc_info->ms = (rtc_info->sTime.SecondFraction - rtc_info->sTime.SubSeconds)/(rtc_info->sTime.SecondFraction + 1.0f);
	rtc_info->unix_time = date2unix(&rtc_info->sTime, &rtc_info->sDate);
	rtc_info->d_unix = (double)rtc_info->unix_time + (double)rtc_info->ms;

	print(Debug_RTC, "%d/%d/%d %d:%d:%d\r\n", rtc_info->sDate.Year, rtc_info->sDate.Month, rtc_info->sDate.Date,
	    rtc_info->sTime.Hours, rtc_info->sTime.Minutes, rtc_info->sTime.Seconds);

	uint16_t temp = (uint8_t)(rtc_info->year / 100);
	rtc_info->year = temp * 100 + rtc_info->sDate.Year;

#endif
	return;
}

void Calib_RTC(void){
#ifndef HAL_SILS
	HAL_RTCEx_SetSmoothCalib(&hrtc, RTC_SMOOTHCALIB_PERIOD_32SEC, RTC_SMOOTHCALIB_PLUSPULSES_RESET, 0x000001FF);
#endif
}






