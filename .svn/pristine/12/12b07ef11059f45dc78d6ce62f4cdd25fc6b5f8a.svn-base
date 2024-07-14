// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "LoRa.h"
#include "LoRa_Struct.h"
#include <src_core/System/TimeManager/time_manager.h>
#include <string.h>
#include <stdio.h>
#include <src_user/Library/General/DBG_printf.h>

#include <stdbool.h>

SPI_HandleTypeDef *_spi;
long _frequency;
int _packetIndex;
int _implicitHeaderMode;
void (*_onReceive)(int);
void (*_onTxDone)();
uint32_t loratimeout;
uint8_t lora_txFifoSize;

int LoRa_begin(LoRa_STRUCT* mylora) {
  HAL_GPIO_WritePin_C2A(mylora->cs_GPIO,mylora->cs_Pin,GPIO_PIN_SET);
  lora_txFifoSize = 0;

  HAL_GPIO_WritePin_C2A(mylora->reset_GPIO,mylora->reset_Pin, GPIO_PIN_RESET);
  c2a_delay_ms(1);
  HAL_GPIO_WritePin_C2A(mylora->reset_GPIO,mylora->reset_Pin, GPIO_PIN_SET);
  c2a_delay_ms(1);
  // check version
  uint8_t version = LoRa_readRegister(mylora, REG_VERSION);//REG_VERSION
  if (version != 0x12) {
    print(LORA,"version = 0x%02x",version);
    return 0;
  }
  return 1;
}

void LoRa_Transmit(const LoRa_STRUCT* mylora) {
  // put in TX mode
  LoRa_writeRegister(mylora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);
}

_Bool LoRa_isTransmitting(const LoRa_STRUCT* mylora) {
  if ((LoRa_readRegister(mylora, REG_OP_MODE) & MODE_TX) == MODE_TX) {
    print(LORA,"isTransmitting\r\n");
    return true;
  }

  if (LoRa_readRegister(mylora, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) {
    // clear IRQ's
    LoRa_writeRegister(mylora, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
  }

  print(LORA,"notisTransmitting\r\n");
  return false;
}

int LoRa_parsePacket(const LoRa_STRUCT* mylora) {
  int size = 0;//explicit header mode
  int packetLength = 0;
  lora_txFifoSize = 0;
  int irqFlags = LoRa_readRegister(mylora, REG_IRQ_FLAGS);

  if (size > 0) {
    LoRa_implicitHeaderMode(mylora);
    LoRa_writeRegister(mylora, REG_PAYLOAD_LENGTH, size & 0xff);
  }
  else {
    LoRa_explicitHeaderMode(mylora);
  }

  // clear IRQ's
  LoRa_writeRegister(mylora, REG_IRQ_FLAGS, irqFlags);


  if ((irqFlags & IRQ_RX_DONE_MASK) && (irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0) {
    //データ受信した場合
    _packetIndex = 0;

    if (_implicitHeaderMode) {
      packetLength = LoRa_readRegister(mylora, REG_PAYLOAD_LENGTH);
    }
    else {//explicit mode
      packetLength = LoRa_readRegister(mylora, REG_RX_NB_BYTES);
    }

    // set FIFO address to current RX address
    LoRa_writeRegister(mylora, REG_FIFO_ADDR_PTR, LoRa_readRegister(mylora, REG_FIFO_RX_CURRENT_ADDR));

    // put in standby mode
    LoRa_idle(mylora);
  }
  else if (LoRa_readRegister(mylora, REG_OP_MODE) != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE)) {
    //データ受信してない場合
    // reset FIFO address
    LoRa_writeRegister(mylora, REG_FIFO_ADDR_PTR, lora_txFifoSize); //RXのbase address(TXとの境界)に設定

    // put in continuous RX mode
    LoRa_writeRegister(mylora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
  }
  return packetLength;
}

int LoRa_packetRssi(const LoRa_STRUCT* mylora) {
  return (LoRa_readRegister(mylora, REG_PKT_RSSI_VALUE) - (_frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));
}

float LoRa_packetSnr(const LoRa_STRUCT* mylora) {
  return ((int8_t) LoRa_readRegister(mylora, REG_PKT_SNR_VALUE)) * 0.25;
}

int LoRa_rssi(const LoRa_STRUCT* mylora) {
  return (LoRa_readRegister(mylora, REG_RSSI_VALUE) - (_frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));
}

size_t LoRa_write(const LoRa_STRUCT* mylora, const uint8_t *buffer, size_t size) {
  int currentLength = LoRa_readRegister(mylora, REG_PAYLOAD_LENGTH);

  // check size
  if ((currentLength + size) > MAX_PKT_LENGTH) {
    size = MAX_PKT_LENGTH - currentLength;

    // write data
    for (size_t i = 0; i < size; i++) {
      LoRa_writeRegister(mylora, REG_FIFO, buffer[i]);
    }

    // update length
    LoRa_writeRegister(mylora, REG_PAYLOAD_LENGTH,  size);
  }

  if ((currentLength + size) <= MAX_PKT_LENGTH) {
    // write data
    for (size_t i = 0; i < size; i++) {
      LoRa_writeRegister(mylora, REG_FIFO, buffer[i]);
    }
    LoRa_writeRegister(mylora, REG_FIFO_ADDR_PTR, 0);

//    for (size_t i = 0; i < size; i++) {
//      uint8_t ret = LoRa_readRegister(mylora, REG_FIFO);
//    }

    // update length
    LoRa_writeRegister(mylora, REG_PAYLOAD_LENGTH, size);
  }
  return size;
}

int LoRa_available(const LoRa_STRUCT* mylora) {
  return (LoRa_readRegister(mylora, REG_RX_NB_BYTES) - _packetIndex);
}

int LoRa_read(const LoRa_STRUCT* mylora) {
  if (!LoRa_available(mylora)) {
    return -1;
  }

  _packetIndex++;

  return LoRa_readRegister(mylora, REG_FIFO);
}

int LoRa_peek(const LoRa_STRUCT* mylora) {
  if (!LoRa_available(mylora)) {
    return -1;
  }

  // store current FIFO address
  int currentAddress = LoRa_readRegister(mylora, REG_FIFO_ADDR_PTR);

  // read
  uint8_t b = LoRa_readRegister(mylora, REG_FIFO);

  // restore FIFO address
  LoRa_writeRegister(mylora, REG_FIFO_ADDR_PTR, currentAddress);
  return b;
}

void LoRa_receive(const LoRa_STRUCT* mylora) {
  LoRa_writeRegister(mylora, REG_DIO_MAPPING_1, 0x00); // DIO0 => RXDONE

  LoRa_explicitHeaderMode(mylora);
  LoRa_writeRegister(mylora, REG_OP_MODE,  MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
}

void LoRa_idle(const LoRa_STRUCT* mylora) {
  LoRa_writeRegister(mylora, REG_OP_MODE,  MODE_LONG_RANGE_MODE | MODE_STDBY);
}

void LoRa_sleep(const LoRa_STRUCT* mylora) {
  LoRa_writeRegister(mylora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

void LoRa_setTxPower(const LoRa_STRUCT* mylora, int level) {
  int outputPin = PA_OUTPUT_PA_BOOST_PIN;

  if (PA_OUTPUT_RFO_PIN == outputPin) {
    // RFO
    if (level < 0) {
      level = 0;
    }
    else if (level > 14) {
      level = 14;
    }
    LoRa_writeRegister(mylora, REG_PA_CONFIG, 0x70 | level);
  }
  else {
    // PA BOOST
    if (level > 17) {
      if (level > 20) {
        level = 20;
      }

      // subtract 3 from level, so 18 - 20 maps to 15 - 17
      level -= 3;

      // High Power +20 dBm Operation (Semtech SX1276/77/78/79 5.4.3.)
      LoRa_writeRegister(mylora, REG_PA_DAC, 0x87);
      LoRa_setOCP(mylora, 200);
    }
    else {
      if (level < 2) {
          level = 2;
      }
      //Default value PA_HF/LF or +17dBm
      LoRa_writeRegister(mylora, REG_PA_DAC, 0x84);
      LoRa_setOCP(mylora, 150);
    }
    LoRa_writeRegister(mylora, REG_PA_CONFIG, PA_BOOST | (level - 2));
  }
}

void LoRa_setOCP(const LoRa_STRUCT* mylora, uint8_t mA) {
  uint8_t ocpTrim = 27;

  if (mA <= 120) {
    ocpTrim = (mA - 45) / 5;
  }
  else if (mA <= 240) {
    ocpTrim = (mA + 30) / 10;
  }
  LoRa_writeRegister(mylora, REG_OCP, 0x20 | (0x1F & ocpTrim));
}

void LoRa_setFrequency(const LoRa_STRUCT* mylora, long frequency) {
  _frequency = frequency;
  uint64_t frf = ((uint64_t) frequency << 19) / 32000000;

  LoRa_writeRegister(mylora, REG_FRF_MSB, (uint8_t) (frf >> 16));
  LoRa_writeRegister(mylora, REG_FRF_MID, (uint8_t) (frf >> 8));
  LoRa_writeRegister(mylora, REG_FRF_LSB, (uint8_t) (frf >> 0));
}

int LoRa_getSpreadingFactor(const LoRa_STRUCT* mylora) {
  return LoRa_readRegister(mylora, REG_MODEM_CONFIG_2) >> 4;
}

void LoRa_setSpreadingFactor(const LoRa_STRUCT* mylora, int sf) {
  if (sf < 6) {
    sf = 6;
  }
  else if (sf > 12) {
    sf = 12;
  }

  if (sf == 6) {
    LoRa_writeRegister(mylora, REG_DETECTION_OPTIMIZE, 0xc5);
    LoRa_writeRegister(mylora, REG_DETECTION_THRESHOLD, 0x0c);
  }
  else {
    LoRa_writeRegister(mylora, REG_DETECTION_OPTIMIZE, 0xc3);
    LoRa_writeRegister(mylora, REG_DETECTION_THRESHOLD, 0x0a);
  }

  LoRa_writeRegister(mylora, REG_MODEM_CONFIG_2, (LoRa_readRegister(mylora, REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
}

long LoRa_getSignalBandwidth(const LoRa_STRUCT* mylora) {
  uint8_t bw = (LoRa_readRegister(mylora, REG_MODEM_CONFIG_1) >> 4);

  switch (bw) {
  case 0:
    return 7.8E3;
  case 1:
    return 10.4E3;
  case 2:
    return 15.6E3;
  case 3:
    return 20.8E3;
  case 4:
    return 31.25E3;
  case 5:
    return 41.7E3;
  case 6:
    return 62.5E3;
  case 7:
    return 125E3;
  case 8:
    return 250E3;
  case 9:
    return 500E3;
  }

  return -1;
}

void LoRa_setSignalBandwidth(const LoRa_STRUCT* mylora, long sbw) {
  int bw;

  if (sbw <= 7.8E3) {
    bw = 0;
  }
  else if (sbw <= 10.4E3) {
    bw = 1;
  }
  else if (sbw <= 15.6E3) {
    bw = 2;
  }
  else if (sbw <= 20.8E3) {
    bw = 3;
  }
  else if (sbw <= 31.25E3) {
    bw = 4;
  }
  else if (sbw <= 41.7E3) {
    bw = 5;
  }
  else if (sbw <= 62.5E3) {
    bw = 6;
  }
  else if (sbw <= 125E3) {
    bw = 7;
  }
  else if (sbw <= 250E3) {
    bw = 8;
  }
  else /*if (sbw <= 250E3)*/{
    bw = 9;
  }
  LoRa_writeRegister(mylora, REG_MODEM_CONFIG_1, (LoRa_readRegister(mylora, REG_MODEM_CONFIG_1) & 0x0f) | (bw << 4));
}

void LoRa_setPreambleLength(const LoRa_STRUCT* mylora, long length) {
  LoRa_writeRegister(mylora, REG_PREAMBLE_MSB, (uint8_t) (length >> 8));
  LoRa_writeRegister(mylora, REG_PREAMBLE_LSB, (uint8_t) (length >> 0));
}

void LoRa_disableCrc(const LoRa_STRUCT* mylora) {
  LoRa_writeRegister(mylora, REG_MODEM_CONFIG_2, LoRa_readRegister(mylora, REG_MODEM_CONFIG_2) & 0xfb);
}

void LoRa_explicitHeaderMode(const LoRa_STRUCT* mylora) {
  _implicitHeaderMode = 0;
  LoRa_writeRegister(mylora, REG_MODEM_CONFIG_1, LoRa_readRegister(mylora, REG_MODEM_CONFIG_1) & 0xfe);
}

void LoRa_implicitHeaderMode(const LoRa_STRUCT* mylora) {
  _implicitHeaderMode = 1;
  LoRa_writeRegister(mylora, REG_MODEM_CONFIG_1, LoRa_readRegister(mylora, REG_MODEM_CONFIG_1) | 0x01);
}

void LoRa_setHopPeriod(const LoRa_STRUCT* mylora, uint8_t period) {
  LoRa_writeRegister(mylora, REG_HOP_PERIOD, period);
}

void LoRa_setTxFifoSize(const LoRa_STRUCT* mylora, uint8_t txSize) {
  lora_txFifoSize = txSize;
  LoRa_writeRegister(mylora, REG_FIFO_RX_BASE_ADDR, lora_txFifoSize); //RXアドレスを書き換え
}

void LoRa_setReadPtr(const LoRa_STRUCT* mylora, uint8_t RxPtr) {
  LoRa_writeRegister(mylora, REG_FIFO_ADDR_PTR, RxPtr); //読みだす位置の変更
}

_ACK LoRa_writeRegister(const LoRa_STRUCT* mylora, uint8_t address, uint8_t value) {
  return (_ACK) LoRa_spi_txrx(mylora, address | 0x80, value);
}

uint8_t LoRa_readRegister(const LoRa_STRUCT* mylora,uint8_t address) {
  return LoRa_spi_txrx(mylora, address & 0x7f, 0x00);
}

uint8_t LoRa_spi_txrx(const LoRa_STRUCT* mylora,uint8_t address, uint8_t value) {
  uint8_t response;
  if(HAL_GPIO_ReadPin_C2A(GATE3V3_MOBCtoFET_LORA_GPIO_Port,GATE3V3_MOBCtoFET_LORA_Pin) == GPIO_PIN_RESET) { //電源OFFなら通信をしない

    HAL_GPIO_WritePin_C2A(CS_SPI3V3_MOBCtoLORA_GPIO_Port,CS_SPI3V3_MOBCtoLORA_Pin,GPIO_PIN_RESET);
    HAL_SPI_Transmit_C2A(mylora->spi_handler, &address, 1, 100);
    HAL_SPI_TransmitReceive_C2A(mylora->spi_handler, &value, &response, 1, 100);
    HAL_GPIO_WritePin_C2A(CS_SPI3V3_MOBCtoLORA_GPIO_Port,CS_SPI3V3_MOBCtoLORA_Pin,GPIO_PIN_SET);
  }
  return response;
}
