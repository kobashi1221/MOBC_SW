/*
 * LoRa_Struct.h
 *
 *  Created on: Jul 5, 2021
 *      Author: K.Okada
 *
 *  update : 2023/11/14
 *      Author: sakaguchi
 */

#ifndef SRC_LORA_LORA_STRUCT_H_
#define SRC_LORA_LORA_STRUCT_H_

#include "../../../IfWrapper/HAL.h"
#include <src_user/TlmCmd/CCSDS/TCPacket.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_user/Drivers/Com/INTER_SAT/Inter_Sat_telemetry_data_definitions.h>
#include <src_user/Applications/DriverInstances/di_struct.h>
#include <src_user/Drivers/Com/GS/command_response_struct.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct {
  // C2A情報
  struct {
    PH_ACK ph_ack;//受信したコマンドをPHに渡したときの返り値
  } c2a;//C2A情報

  // 通信情報
  struct {
  } comm;//通信情報
} LoRa_Info;

typedef struct {
  struct {
    uint32_t frequency;
    uint32_t pa_info;
    uint32_t modem_config;
    uint16_t timeout;
    uint16_t premble;
    uint16_t  hop_info;
    uint32_t agc_step;
    uint8_t pll;
    uint16_t valid_header;
    uint16_t valid_packet;
    uint16_t modem_sta;
    uint16_t pkt_value;
    uint32_t receive_info;
    uint32_t freq_error;
    uint8_t lora_temp;
  }reg_info;
  struct {
    uint8_t w_err_num;
    uint8_t w_err_count;
    uint8_t w_err_addr1;
    uint8_t w_value1;
    uint8_t w_err_addr2;
    uint8_t w_value2;
    uint8_t w_err_addr3;
    uint8_t w_value3;
    uint8_t w_err_addr4;
    uint8_t w_value4;
    uint8_t w_err_addr5;
    uint8_t w_value5;
    uint8_t w_err_addr6;
    uint8_t w_value6;
    uint8_t w_err_addr7;
    uint8_t w_value7;
    uint8_t w_err_addr8;
    uint8_t w_value8;
    uint8_t w_err_addr9;
    uint8_t w_value9;
    uint8_t w_err_addr10;
    uint8_t w_value10;
    uint8_t w_err_addr11;
    uint8_t w_value11;
    uint8_t w_err_addr12;
    uint8_t w_value12;
    uint8_t w_err_addr13;
    uint8_t w_value13;
    uint8_t w_err_addr14;
    uint8_t w_value14;
    uint8_t w_err_addr15;
    uint8_t w_value15;
    uint8_t w_err_addr16;
    uint8_t w_value16;
  }err_info;
  struct {
    INTERSAT_TlmData inter_sat;
  }lora_intersat_info;
}LORA_TLM_STRUCT;

typedef struct {
  uint8_t frequencyMsb;
  uint8_t frequencyMid;
  uint8_t frequencyLsb;
  uint8_t PaConfig;
  uint8_t PaRamp;
  uint8_t OCP;
  uint8_t LNA;
  uint8_t ModemConfig1;
  uint8_t ModemConfig2;
  uint8_t symbTimeout;
  uint8_t PreambleMsb;
  uint8_t PreambleLsb;
  uint8_t payload;
  uint8_t HopPeriod;
  uint8_t ModemConfig3;
  uint8_t AgcRefLevel;
  uint8_t AgcStep1Lf;
  uint8_t AgcStep2Lf;
  uint8_t AgcStep3Lf;
  uint8_t PllLf;
}LoRa_Register;

typedef struct {
  uint8_t lora_key_mode;
  uint8_t fixed_key;
  uint8_t variable_key;
  uint8_t tx_sync_word;//1U, 2U判別用
  uint8_t rx_sync_word;//1U, 2U判別用
}LoRa_CIPHER_STRUCT;

typedef struct {
  _ACK ack;
  TCP tcp;
  uint8_t receive_enable;
  uint8_t wait_counter;
  uint8_t done_counter;
  uint8_t lora_rx_mode;
  uint8_t tx_done_counter;
  uint8_t lora_reg_param[20];

  void *spi_handler;
  void *cs_GPIO;
  uint16_t cs_Pin;
  void *reset_GPIO;
  uint16_t reset_Pin;
  void *dio0_GPIO;
  uint16_t dio0_pin;
  void *dio1_GPIO;
  uint16_t dio1_pin;
  void *dio2_GPIO;
  uint16_t dio2_pin;
  void *dio3_GPIO;
  uint16_t dio3_pin;
  void *dio4_GPIO;
  uint16_t dio4_pin;
  void *dio5_GPIO;
  uint16_t dio5_pin;
  int TIMEOUT;
  cycle_t last_recv_time;

  LoRa_Info Info;
  LoRa_Register reg;
  LoRa_CIPHER_STRUCT cipher;
  LORA_TLM_STRUCT lora_tlm;
} LoRa_STRUCT;

#ifdef __cplusplus
 }
#endif

#endif /* SRC_LORA_LORA_STRUCT_H_ */
