/*
 * UNIX.h
 *
 *  Created on: 2022/07/06
 *      Author: SAT37
 */

#ifndef SRC_USER_DRIVERS_CDH_RTC_UNIX_H_
#define SRC_USER_DRIVERS_CDH_RTC_UNIX_H_

#include <src_user/IfWrapper/HAL.h>

void ut2date(RTC_TimeTypeDef* p_time, RTC_DateTypeDef* p_date, long unix);
uint32_t date2unix(RTC_TimeTypeDef* p_time, RTC_DateTypeDef* p_date);

#endif /* SRC_USER_DRIVERS_CDH_RTC_UNIX_H_ */
