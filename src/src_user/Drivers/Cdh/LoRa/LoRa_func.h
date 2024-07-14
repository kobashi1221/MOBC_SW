/*
 * LoRa_c.h
 *
 *  Created on: Jul 5, 2021
 *      Author: K.Okada
 *
 *  update : 2023/11/14
 *      Author: sakaguchi
 */

#ifndef SRC_LORA_LORA_FUNC_H_
#define SRC_LORA_LORA_FUNC_H_

#include "LoRa.h"
#include "LoRa_Struct.h"

#include <src_user/IfWrapper/HAL.h> //necessary to add HAL func
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
  PLAINTEXT,
  FIXATION,
  VARIABLE,
}LORA_CIHPER_MODE;

typedef enum{
  RX_CONTINUOUS,
  RX_CAD,
}LORA_RX_MODE;

//初期化関数
_ACK LORA_COM_init(LoRa_STRUCT* mylora);

//送信・受信関数
_ACK LORA_Send_Prepare(LoRa_STRUCT* mylora);
_ACK LORA_Send(LoRa_STRUCT* mylora, uint8_t *payload, uint8_t datasize);
void LORA_Receive_Prepare(LoRa_STRUCT* mylora);
void LORA_Recieve(const LoRa_STRUCT* mylora, uint8_t *payload, uint16_t maｘ);
void Receive(const LoRa_STRUCT* mylora, uint8_t *payload,uint16_t max, uint16_t _timeout);
_ACK check_recieve_enable(LoRa_STRUCT* mylora);

//レジスタ関数
void LoRa_mode_cad(LoRa_STRUCT* mylora);
void LoRa_mode_continuous(LoRa_STRUCT* mylora);
uint8_t LoRa_Rx_Size(LoRa_STRUCT* mylora);
void LoRa_load_default_setting(LoRa_STRUCT* mylora);
void lora_setting_reset(LoRa_STRUCT* mylora);

//テレメ用関数
void lora_tlm_update(LoRa_STRUCT* mylora);
void lora_get_temprature(LoRa_STRUCT* mylora);

//暗号化関数
void lora_encryption(LoRa_STRUCT* mylora, uint8_t* in_buf, uint8_t* out_buf, size_t size);
_ACK lora_decryption(LoRa_STRUCT* mylora, TCP* in_buf, TCP* out_buf, size_t size);
_ACK lora_check_variable_key(LoRa_STRUCT* mylora, TCP* in_buf, TCP* out_buf, size_t size);
_ACK lora_check_fixation_key(LoRa_STRUCT* mylora, TCP* in_buf, TCP* out_buf, size_t size);
_ACK lora_check_plaintext(LoRa_STRUCT* mylora, TCP* in_buf, TCP* out_buf, size_t size);
_ACK lora_check_command(TCP* packet, size_t size);
_ACK lora_check_tlm(TCP* packet, size_t size);
uint8_t variable_key_maker(LoRa_STRUCT* mylora);


#ifdef __cplusplus
}
#endif

#endif /* SRC_LORA_LORA_FUNC_H_ */
