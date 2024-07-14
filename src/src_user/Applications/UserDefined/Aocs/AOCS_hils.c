/*
 * AOCS_hils.c
 *
 *  Created on: 2023/07/07
 *      Author: sunaken
 */

#include "AOCS_hils.h"
#include <src_user/TlmCmd/Ccsds/TCPacket.h>
#include <src_user/IfWrapper/HAL.h>
#include <src_user/Library/General/DBG_printf.h>

#ifdef HILS
static AOCSHils AOCS_hils_;
const AOCSHils *const AOCS_hils = &AOCS_hils_;
uint8_t hils_receive_buf[256];
uint8_t hils_received = 0;
uint8_t hils_UTC_flag = 0;
float before_second = 61.0f;
#endif

static void AOCS_HILS_init_(void);
static void AOCS_HILS_update_(void);


AppInfo AOCS_HILS_create_app(void)
{
  return AI_create_app_info("AOCS_hils", AOCS_HILS_init_, AOCS_HILS_update_);
}


static void AOCS_HILS_init_(void)
{
#ifdef HILS
  HAL_UARTEx_ReceiveToIdle_DMA_C2A(&huart7, hils_receive_buf, TCP_MAX_LEN);
#endif
}


static void AOCS_HILS_update_(void)
{
#ifdef HILS
  before_second = AOCS_hils_.utc_second;

  if (hils_received)
  {
    hils_received = 0;
    if (hils_receive_buf[0] == 'C' && hils_receive_buf[1] == 'M' && hils_receive_buf[2] == 'D' && hils_receive_buf[3] == '3' && hils_receive_buf[135] == 'G' && hils_receive_buf[136] == 'G')
    {
//      print(SILS, "buf0: %c\n", (char)hils_receive_buf[0]);
//      print(SILS, "buf1: %c\n", (char)hils_receive_buf[1]);
//      print(SILS, "buf2: %c\n", (char)hils_receive_buf[2]);
//      print(SILS, "buf3: %c\n", (char)hils_receive_buf[3]);
      memcpy(AOCS_hils_.gyro, hils_receive_buf + 4, 12);
      memcpy(AOCS_hils_.mags, hils_receive_buf + 16, 12);
      memcpy(AOCS_hils_.suns1, hils_receive_buf + 28, 12);
      memcpy(AOCS_hils_.suns2, hils_receive_buf + 40, 12);
      memcpy(AOCS_hils_.suns3, hils_receive_buf + 52, 12);
      memcpy(AOCS_hils_.suns4, hils_receive_buf + 64, 12);
      memcpy(AOCS_hils_.suns5, hils_receive_buf + 76, 12);
      memcpy(AOCS_hils_.suns6, hils_receive_buf + 88, 12);
      memcpy(AOCS_hils_.ecef_velocity, hils_receive_buf + 100, 12);
      memcpy(AOCS_hils_.llh, hils_receive_buf + 112, 12);
      memcpy(&AOCS_hils_.utc_year, hils_receive_buf + 124, 2);
      memcpy(&AOCS_hils_.utc_month, hils_receive_buf + 126, 1);
      memcpy(&AOCS_hils_.utc_day, hils_receive_buf + 127, 1);
      memcpy(&AOCS_hils_.utc_hour, hils_receive_buf + 128, 1);
      memcpy(&AOCS_hils_.utc_minute, hils_receive_buf + 129, 1);
      memcpy(&AOCS_hils_.utc_second, hils_receive_buf + 130, 4);
      memcpy(&AOCS_hils_.gps_visible, hils_receive_buf + 134, 1);
      hils_UTC_flag = 1;
    }
  }
  if(before_second == AOCS_hils_.utc_second){
    HAL_UARTEx_ReceiveToIdle_DMA_C2A(&huart7, hils_receive_buf, TCP_MAX_LEN);
    print(SILS, "reset hils uart\r\n");
  }
#endif
}
