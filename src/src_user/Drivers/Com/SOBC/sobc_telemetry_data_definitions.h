/**
 * @file   rxpic_telemetry_data_definitions.h
 * @brief  バッファリングされているテレメをパースしてMOBC内でかんたんに利用できるようにするためのテレメデータ構造体定義
 * @note   このコードは自動生成されています！
 */
#ifndef SOBC_TELEMETRY_DATA_DEFINITIONS_H_
#define SOBC_TELEMETRY_DATA_DEFINITIONS_H_

typedef struct
{
  struct{
    uint32_t SOBC_Time;
    uint8_t anomaly_count;
    uint8_t latest_anomaly_ID;
    uint8_t reset_count;
  }system;

  struct{
    uint16_t curs_sobc;
  }curs;

  struct{
    uint16_t vols_sobc;
    uint16_t vols_mags;
    uint16_t vols_suns;
    uint16_t vols_tofs;
  }vols;

  struct{
    uint16_t temp_PX;
    uint16_t temp_PY;
  }temp;

} SOBC_TlmData;

#endif
