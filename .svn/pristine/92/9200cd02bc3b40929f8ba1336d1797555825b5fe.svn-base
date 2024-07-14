/*
 * UNIX.c
 *
 *  Created on: 2022/07/06
 *      Author: SAT37
 */

#include "UNIX.h"
#include "stdint.h"

/*その月までの日数の和（累積和）*/
static const int monthDays[13]={
	0,31,59,90,120,151,181,212,243,273,304,334,365
};

/*1日の秒数(=24*60*60)*/
#define SECONDS_PER_DAY 86400

/*1年からyear年まで（両端を含む）の日数を求める*/
static long getDays(int year) {
	long result;
	if(year<0)return -getDays(-year)-366+365;
	else if(year==0)return 0;
	result=year*365;/*1年は基本365日*/
	result+=year/4;/*4で割り切れたら閏年*/
	result-=year/100;/*100で割り切れたら閏年ではない*/
	result+=year/400;/*400で割り切れたら閏年*/
	return result;
}

/*1年からyear年までの閏年の個数を求める*/
static int getLeapYearNum(int year) {
	int result;
	if(year<0)return -getLeapYearNum(-year)-1;
	else if(year==0)return 0;
	result=year/4;/*4で割り切れたら閏年*/
	result-=year/100;/*100で割り切れたら閏年ではない*/
	result+=year/400;/*400で割り切れたら閏年*/
	return result;
}

/*fromYear+1年からtoYear年までの閏年の個数を求める*/
static int getLeapYearNum2(int fromYear,int toYear) {
	return getLeapYearNum(toYear)-getLeapYearNum(fromYear);
}

/*その年が閏年であるかを求める*/
static int isLeapYear(int year) {
	return (year%400==0 || (year%4==0 && year%100!=0));
}

/*1970年1月1日　00:00:00からの経過秒数を日付にする*/
void ut2date(RTC_TimeTypeDef* p_time, RTC_DateTypeDef* p_date, long unix) {
	long days;
	int yearNum;
	int amariDays;
	int daySeconds;
	int hoseiYear;
	int is2_29 = 0;
//	if(!p_time && !p_date)return;
	if(unix >= 0)
	{
		days = unix / SECONDS_PER_DAY;
		daySeconds = (int)(unix % SECONDS_PER_DAY);
		yearNum = 1970 + (int)(days / 365);
		amariDays = (int)(days % 365);
	}
	else
	{
		days=(-unix)/SECONDS_PER_DAY;
		daySeconds=(int)(SECONDS_PER_DAY-((-unix)%SECONDS_PER_DAY));
		if(daySeconds==SECONDS_PER_DAY) {
			daySeconds=0;
		}
		else days++;

		yearNum=1970-(int)(days/365)-1;
		amariDays=365-(int)(days%365);
	}
	amariDays -= getLeapYearNum2(1969, yearNum-1);
	while(1) {
		if((amariDays >=0 && amariDays < 365) || (amariDays == 365 && isLeapYear(yearNum)))break;

		if(amariDays < 0) {
			hoseiYear = (-amariDays)/365+1;
			amariDays += hoseiYear*365;
			amariDays += getLeapYearNum(yearNum - 1) - getLeapYearNum(yearNum - hoseiYear - 1);
			yearNum -= hoseiYear;
		}
		else if(amariDays >= 365) {
			hoseiYear = amariDays / 365;
			amariDays = amariDays % 365;
			amariDays -= getLeapYearNum(yearNum + hoseiYear - 1) - getLeapYearNum(yearNum - 1);
			yearNum += hoseiYear;
		}
		else break;
	}

	if(isLeapYear(yearNum) && amariDays >= 59) {
		if(amariDays == 59)is2_29 = 1;
		amariDays--;
	}

	uint8_t j = 0;
	for(uint8_t i=1 ; i<=12 ; i++) {
		j++;
		if(amariDays >= monthDays[i-1] && amariDays < monthDays[i])break;
	}
	amariDays -= monthDays[j-1];
	if(is2_29)amariDays++;

	p_date->Year = (uint8_t)(yearNum - 2000);
	p_date->Month = j;
	p_date->Date = (uint8_t)(amariDays + 1);
	p_time->Hours = (uint8_t)(daySeconds / 3600);
	p_time->Minutes = (uint8_t)((daySeconds / 60) % 60);
	p_time->Seconds = (uint8_t)(daySeconds % 60);

//	printf("j = %d\r\n", j);
//	printf("%d, %d, %d",yearNum, amariDays, daySeconds);
//	printf("RTC:%02d/%02d/%02d %02d:%02d:%02d\r\n", p_date->Year, p_date->Month, p_date->Date, p_time->Hours, p_time->Minutes, p_time->Seconds);
}



//https://unoh.github.io/2010/11/12/unix_time.html
//グレゴリオ暦換算紀元元年1月1日から1970年1月1日0時0分0秒までの経過日数
#define EPOC_TIME (719163)

/*1970年1月1日　00:00:00からの経過秒数を返す*/
uint32_t date2unix(RTC_TimeTypeDef* p_time, RTC_DateTypeDef* p_date){
	uint32_t unix_time;

	uint32_t year = ((uint32_t)p_date->Year) + 2000;
	uint32_t month = (uint32_t)p_date->Month;
	uint32_t date = (uint32_t)p_date->Date;
	uint32_t hour = (uint32_t)p_time->Hours;
	uint32_t minute = (uint32_t)p_time->Minutes;
	uint32_t second = (uint32_t)p_time->Seconds;

	//フェアフィールド(Fairfield)の公式
	//経過日数 365*（ｙ-1）+[ｙ/4]-[ｙ/100]+[ｙ/400]+31+28+1+[306*(ｍ+1)/10]-122+(d-1)
	if(month == 1 || month == 2){
		month += 12;
		year -= 1;
	}

	// Fairfieldの公式で算出した719161日からの経過秒数の算出
	unix_time = (uint32_t)((365 * (year - 1) + (uint32_t)(year / 4) - (uint32_t)(year / 100) + (uint32_t)(year / 400) + (uint32_t)(306 * (month + 1) / 10) + date - 63 - EPOC_TIME) * 86400)
					+ (hour * 3600) + (minute * 60) + second;

	return unix_time;
}
