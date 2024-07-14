/*
 * AOCS_hils.h
 *
 *  Created on: 2023/07/07
 *      Author: sunaken
 */

#ifndef SRC_SRC_USER_APPLICATIONS_USERDEFINED_AOCS_AOCS_HILS_H_
#define SRC_SRC_USER_APPLICATIONS_USERDEFINED_AOCS_AOCS_HILS_H_

#include <src_core/System/ApplicationManager/app_info.h>

#ifdef HILS
typedef struct
{
  float gyro[3];
  float mags[3];
  float suns1[3];
  float suns2[3];
  float suns3[3];
  float suns4[3];
  float suns5[3];
  float suns6[3];
  float ecef_velocity[3]; // [m/s]
  float llh[3];
  uint16_t utc_year;
  uint8_t utc_month;
  uint8_t utc_day;
  uint8_t utc_hour;
  uint8_t utc_minute;
  float utc_second;
  uint8_t gps_visible;
}AOCSHils;

extern uint8_t hils_receive_buf[256];
extern uint8_t hils_received;
extern uint8_t hils_UTC_flag;
extern const AOCSHils* const AOCS_hils;
#endif

AppInfo AOCS_HILS_create_app(void);

#endif /* SRC_SRC_USER_APPLICATIONS_USERDEFINED_AOCS_AOCS_HILS_H_ */
