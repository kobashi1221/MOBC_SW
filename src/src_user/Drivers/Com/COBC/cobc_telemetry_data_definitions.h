/*
 * cobc_telemetry_data_definitions.h
 *
 *  Created on: 2021/10/16
 *      Author: daiki
 *
 *  Updated on: 2021/12/22
 *      Author: Sakaguchi
 */

#ifndef COBC_TELEMETRY_DATA_DEFINITIONS_H_
#define COBC_TELEMETRY_DATA_DEFINITIONS_H_

typedef struct
{
  struct
  {//COBCからMOBCに送られるデータ
    struct{
      uint16_t cobc;
    }temp;

    struct{
      uint16_t curs_cobc;
      uint16_t curs_mcam;
      uint16_t curs_scam12;
      uint16_t curs_scam28;
      uint16_t vols_mcam;
      uint16_t vols_heat;
    }cur_vols;

    uint32_t COBC_Time;
    uint8_t cobc_reset_conut;
    uint8_t anomaly_count;
    uint32_t cobc_event_time;
    uint8_t cobc_event_group;
    uint16_t cobc_event_local;
    uint8_t cobc_event_err_level;
    uint8_t cobc_mode;

    uint8_t cnt_mcam;
    uint8_t cnt_scam;
    uint16_t cycle_cnt;
  } cobc_to_mobc;

  struct
  {//COBCからMOBCの送られるデータ(AOBCが利用)
    float target_vector[3];
    uint8_t AttitudeControl_flag;
  } cobc_aocs_tlm;
} COBC_TlmData;



#endif /* SRC_USER_DRIVERS_COM_COBC_COBC_TELEMETRY_DATA_DEFINITIONS_H_ */
