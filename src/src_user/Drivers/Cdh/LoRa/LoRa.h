// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef LORA_H
#define LORA_H

#include <src_user/IfWrapper/HAL.h>

#include <stdint.h>
#include <stdbool.h>
#include "../../../IfWrapper/HAL.h"
#include "LoRa_Struct.h"
#include <src_core/TlmCmd/packet_handler.h>
#include <src_user/Library/Delay.h>
#include <src_core/Drivers/Super/driver_super.h>

#ifdef __cplusplus
 extern "C" {
#endif
#define LoRa_MAX_RECEIVE_PACKET_NUM   4

 // registers
#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01
#define REG_FRF_MSB              0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08
#define REG_PA_CONFIG            0x09
#define REG_PA_RAMP				 0x0a
#define REG_OCP                  0x0b
#define REG_LNA                  0x0c
#define REG_FIFO_ADDR_PTR        0x0d
#define REG_FIFO_TX_BASE_ADDR    0x0e
#define REG_FIFO_RX_BASE_ADDR    0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS_MASK       0x11
#define REG_IRQ_FLAGS            0x12
#define REG_RX_NB_BYTES          0x13
#define REG_VALID_HEADER_MSB	 0x14
#define REG_VALID_HEADER_LSB	 0x15
#define REG_VALID_PACKET_MSB	 0x16
#define REG_VALID_PACKET_LSB	 0x17
#define REG_MODEM_STATUS		 0x18
#define REG_PKT_SNR_VALUE        0x19
#define REG_PKT_RSSI_VALUE       0x1a
#define REG_RSSI_VALUE           0x1b
#define REG_HOP_CHANNEL		 	 0x1c
#define REG_MODEM_CONFIG_1       0x1d
#define REG_MODEM_CONFIG_2       0x1e
#define REG_TimeoutLsb         	 0x1f
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_HOP_PERIOD		  	 0x24
#define REG_MODEM_CONFIG_3       0x26
#define REG_FREQ_ERROR_MSB       0x28
#define REG_FREQ_ERROR_MID       0x29
#define REG_FREQ_ERROR_LSB       0x2a
#define REG_RSSI_WIDEBAND        0x2c
#define REG_DETECTION_OPTIMIZE   0x31
#define REG_INVERTIQ             0x33
#define REG_DETECTION_THRESHOLD  0x37
#define REG_SYNC_WORD            0x39
#define REG_INVERTIQ2            0x3b
#define REG_TEMP				 0x3C
#define REG_DIO_MAPPING_1        0x40
#define REG_VERSION              0x42
#define REG_PA_DAC               0x4d
#define REG_AGC_REF				 0x61
#define REG_Thresh1				 0x62
#define REG_Thresh2				 0x63
#define REG_Thresh3				 0x64
#define REG_PLL					 0x70

 // modes
#define MODE_LONG_RANGE_MODE     0x80
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03
#define MODE_RX_CONTINUOUS       0x05
#define MODE_RX_SINGLE           0x06
#define MODE_CAD                 0x07

 // PA config
 #define PA_BOOST                 0x80

 // IRQ masks
 #define IRQ_NONE_MASK				0x00
 #define IRQ_FHSS_CHANNEL 			0x02
 #define IRQ_TX_DONE_MASK           0x08
 #define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
 #define IRQ_RX_DONE_MASK           0x40

 #define RF_MID_BAND_THRESHOLD    525E6
 #define RSSI_OFFSET_HF_PORT      157
 #define RSSI_OFFSET_LF_PORT      164

 #define MAX_PKT_LENGTH           256

 #if (ESP8266 || ESP32)
     #define ISR_PREFIX ICACHE_RAM_ATTR
 #else
 #define ISR_PREFIX
 #endif

#if defined(ARDUINO_SAMD_MKRWAN1300)
#define LORA_DEFAULT_SPI           SPI1
#define LORA_DEFAULT_SPI_FREQUENCY 200000
#define LORA_DEFAULT_SS_PIN        LORA_IRQ_DUMB
#define LORA_DEFAULT_RESET_PIN     -1
#define LORA_DEFAULT_DIO0_PIN      -1
#elif defined(ARDUINO_SAMD_MKRWAN1310)
#define LORA_DEFAULT_SPI           SPI1
#define LORA_DEFAULT_SPI_FREQUENCY 200000
#define LORA_DEFAULT_SS_PIN        LORA_IRQ_DUMB
#define LORA_DEFAULT_RESET_PIN     -1
#define LORA_DEFAULT_DIO0_PIN      LORA_IRQ
#else
#define LORA_DEFAULT_SPI           SPI
#define LORA_DEFAULT_SPI_FREQUENCY 8E6 
#define LORA_DEFAULT_SS_PIN        10
#define LORA_DEFAULT_RESET_PIN     9
#define LORA_DEFAULT_DIO0_PIN      2
#endif

#define PA_OUTPUT_RFO_PIN          0
#define PA_OUTPUT_PA_BOOST_PIN     1


//モジュール依存の関数(git_hubから引用)
int LoRa_begin(LoRa_STRUCT* mylora);
size_t LoRa_write(const LoRa_STRUCT* mylora, const uint8_t *buffer, size_t size);
void LoRa_Transmit(const LoRa_STRUCT* mylora);
int LoRa_parsePacket(const LoRa_STRUCT* mylora);
float LoRa_packetSnr(const LoRa_STRUCT* mylora);
int LoRa_rssi(const LoRa_STRUCT* mylora);
int LoRa_available(const LoRa_STRUCT* mylora);
int LoRa_read(const LoRa_STRUCT* mylora);
int LoRa_peek(const LoRa_STRUCT* mylora);
void LoRa_receive(const LoRa_STRUCT* mylora);
void LoRa_idle(const LoRa_STRUCT* mylora);
void LoRa_sleep(const LoRa_STRUCT* mylora);
void LoRa_setTxPower(const LoRa_STRUCT* mylora, int level);
void LoRa_setOCP(const LoRa_STRUCT* mylora, uint8_t mA);
void LoRa_setFrequency(const LoRa_STRUCT* mylora, long frequency);
void LoRa_setSpreadingFactor(const LoRa_STRUCT* mylora, int sf);
void LoRa_setSignalBandwidth(const LoRa_STRUCT* mylora, long sbw);
void LoRa_setPreambleLength(const LoRa_STRUCT* mylora, long length);
void LoRa_disableCrc(const LoRa_STRUCT* mylora);
void LoRa_setHopPeriod(const LoRa_STRUCT* mylora, uint8_t period);
void LoRa_setTxFifoSize(const LoRa_STRUCT* mylora, uint8_t txSize );
void LoRa_setReadPtr(const LoRa_STRUCT* mylora, uint8_t RxPtr);
void LoRa_explicitHeaderMode(const LoRa_STRUCT* mylora);
void LoRa_implicitHeaderMode(const LoRa_STRUCT* mylora);
_Bool LoRa_isTransmitting(const LoRa_STRUCT* mylora);
int LoRa_getSpreadingFactor(const LoRa_STRUCT* mylora);
long LoRa_getSignalBandwidth(const LoRa_STRUCT* mylora);

//末端ドライバ関数
_ACK LoRa_writeRegister(const LoRa_STRUCT* mylora,uint8_t address, uint8_t value);
uint8_t LoRa_readRegister(const LoRa_STRUCT* mylora,uint8_t address);
uint8_t LoRa_spi_txrx(const LoRa_STRUCT* mylora,uint8_t address, uint8_t value);

extern uint32_t loratimeout;

#ifdef __cplusplus
 }
#endif

/*extern "C" {

//----------------------------------------------------------
// ここからCのコード
//----------------------------------------------------------

#ifdef __cplusplus
}*/
#endif

//#endif
