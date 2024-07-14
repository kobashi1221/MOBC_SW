/*---------------------------------------------------------------------------------
 LoRa_func.c
 - LoRa.cのラッパ C2A用に使用しやすくしたもの
 - LoRa.cは下記のライブラリを改変したもの
 https://github.com/sandeepmistry/arduino-LoRa
 ---------------------------------------------------------------------------------*/

#include "LoRa_func.h"

#include <string.h>
#include <stdio.h>
#include "../../../Applications/DriverInstances/di_struct.h"
#include <src_user/Applications/DriverInstances/di_rxpic.h>
#include <src_user/Library/General/DBG_printf.h>


//LoRaの通信初期化関数
_ACK LORA_COM_init(LoRa_STRUCT* lora_driver) {
  lora_driver->cs_GPIO = CS_SPI3V3_MOBCtoLORA_GPIO_Port;
  lora_driver->cs_Pin = CS_SPI3V3_MOBCtoLORA_Pin;
  lora_driver->reset_GPIO = RESET3V3_MOBCtoLORA_GPIO_Port;
  lora_driver->reset_Pin = RESET3V3_MOBCtoLORA_Pin;

  lora_driver->dio0_GPIO = DIO0_3V3_LORAtoMOBC_GPIO_Port;
  lora_driver->dio0_pin = DIO0_3V3_LORAtoMOBC_Pin;
  lora_driver->dio1_GPIO = DIO1_3V3_LORAtoMOBC_GPIO_Port;
  lora_driver->dio1_pin = DIO1_3V3_LORAtoMOBC_Pin;
  lora_driver->dio2_GPIO = DIO2_3V3_LORAtoMOBC_GPIO_Port;
  lora_driver->dio2_pin = DIO2_3V3_LORAtoMOBC_Pin;
  lora_driver->dio3_GPIO = DIO3_3V3_LORAtoMOBC_GPIO_Port;
  lora_driver->dio3_pin = DIO3_3V3_LORAtoMOBC_Pin;
  lora_driver->dio4_GPIO = DIO4_3V3_LORAtoMOBC_GPIO_Port;
  lora_driver->dio4_pin = DIO4_3V3_LORAtoMOBC_Pin;
  lora_driver->dio5_GPIO = DIO5_3V3_LORAtoMOBC_GPIO_Port;
  lora_driver->dio5_pin = DIO5_3V3_LORAtoMOBC_Pin;

  lora_driver->spi_handler = &hspi5;
  lora_driver->TIMEOUT = 0xff;

  return _SUCCESS;
}

//データ送信用のための準備関数
_ACK LORA_Send_Prepare(LoRa_STRUCT* mylora) {
  if (LoRa_isTransmitting(mylora)) { // 現在送信中か
    return _FAILED;
  }

  LoRa_sleep(mylora);//fifo reset
  LoRa_idle(mylora);//stand-by

  LoRa_explicitHeaderMode(mylora); // explicit modeに設定
  LoRa_writeRegister(mylora, REG_PAYLOAD_LENGTH, 0); // explicit mode の時は常にゼロ

  //送信用バッファを256byte分確保し, FIFOのポインタを先頭に設定
  LoRa_writeRegister(mylora, REG_FIFO_RX_BASE_ADDR, 255);
  LoRa_writeRegister(mylora, REG_FIFO_TX_BASE_ADDR, 0);
  LoRa_writeRegister(mylora, REG_FIFO_ADDR_PTR, 0);

  //同期用変数を設定
  LoRa_writeRegister(mylora, REG_SYNC_WORD, mylora->cipher.tx_sync_word);

  //受信フラグをdisable
  mylora->receive_enable = 0;

  return _SUCCESS;
}


//データ送信用関数
//FIFOへデータを書き込み, LORA変調により送信する
_ACK LORA_Send(LoRa_STRUCT* mylora, uint8_t *payload, uint8_t datasize) {
  uint8_t send_packet[256] = {0};
  if(LORA_Send_Prepare(mylora) != _SUCCESS){
    return _FAILED;
  }

  //暗号化し, FIFOにデータ書き込み、送信
  lora_encryption(mylora, &payload[0], &send_packet[0], datasize);
  LoRa_write(mylora, &send_packet[0], datasize);
  LoRa_Transmit(mylora);

  //送信にかかる秒数, TX -> CADモードに移行するまでの時間に相当
  mylora->tx_done_counter = mylora->done_counter;

  return _SUCCESS;
}

// LoRa受信後の処理関数(LoRa FIFO -> MOBC)
// max : 受信する最大バイト数を設定しておく
// size : 受信したバイト数が格納される
void LORA_Recieve(const LoRa_STRUCT* mylora, uint8_t *payload, uint16_t max) {
  Receive(mylora, payload, max, 100); // 受信したデータをLoRaモジュールのFIFOからMOBCのバッファへ移す
  LoRa_receive(mylora);
}

//データが受信済みであれば、payloadのポインタを渡す
void Receive(const LoRa_STRUCT* mylora, uint8_t *payload, uint16_t max, uint16_t LoRa_timeout) {
  uint16_t packetSize;
  uint32_t waitCount = 0;
  uint16_t index = 0;
  while (1) {
    packetSize = LoRa_parsePacket(mylora);
    //パケットサイズが0でなければ
    if (packetSize) {
      //packet長さの分インクリメントし, readする
      while (LoRa_available(mylora)) {
        payload[index] = (uint8_t) LoRa_read(mylora);
        index++;
        if (index == max)
          break;
      }
      break;
    }
    waitCount++;
    if (waitCount > loratimeout)
      break;
  }
  print(LORA,"packet is \r\n");
  for(int i = 0;i<index;i++){
    print(LORA, "%02x ",payload[i]);
  }
  print(LORA,"\r\n");
  print(LORA,"Receive_finish\r\n");
}

//受信したパケットの長さを読み取る関数
//ヘッダの5,6byte目にパケットの長さがある = bufferの132と133を読む
int LoRa_Read_length(const LoRa_STRUCT* mylora) {
  uint8_t length[2];
  int len;

  //FIFOAddrPtrを移動させる
  LoRa_writeRegister(mylora, 0x0d, 0x84);
  for(int i = 0; i < 2; ++i){
    length[i] = (uint8_t) LoRa_read(mylora);
  }

  //FIFOAddrPtrを戻す
  LoRa_writeRegister(mylora, 0x0d, 0x80);
  //データ数入れる
  len = (length[0] << 8) + (length[1]);

  return len;
}

//FIFOを空っぽにして受信モードに移行する関数
void LORA_Receive_Prepare(LoRa_STRUCT* mylora) {
  LoRa_sleep(mylora);//fifo reset
  LoRa_idle(mylora);//stand-by

  //受信用byteを256byte分確保し, FIFOのポインタを先頭に移す
  LoRa_writeRegister(mylora, REG_FIFO_RX_BASE_ADDR, 0);
  LoRa_writeRegister(mylora, REG_FIFO_TX_BASE_ADDR, 255);
  LoRa_writeRegister(mylora, REG_FIFO_ADDR_PTR, 0);

  //受信同期用変数
  LoRa_writeRegister(mylora, REG_SYNC_WORD, mylora->cipher.rx_sync_word);

  //受信フラグをenable
  mylora->receive_enable = 1;

  //受信モードの選択
  if(mylora->lora_rx_mode == RX_CAD) {
    LoRa_mode_cad(mylora);
  }
  else {
    LoRa_mode_continuous(mylora);
  }
}

_ACK check_recieve_enable(LoRa_STRUCT* mylora) {
//	int ret1,ret2;
//	ret1=LoRa_readRegister(mylora, REG_IRQ_FLAGS);
//	ret2=LoRa_readRegister(mylora, REG_IRQ_FLAGS_MASK);
//
//	printf("ret is %02x %02x \r\n",ret1, ret2);

  if((LoRa_readRegister(mylora, REG_MODEM_CONFIG_2) & 0b00001000) == 0b00001000) { // TX continuousなら受信モードへはいかずに送信を続ける
    return _FAILED;
  }

  if((LoRa_readRegister(mylora, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0) {
    if(mylora->tx_done_counter > 0){
      --mylora->tx_done_counter;
      return _FAILED;
    }
    LoRa_writeRegister(mylora, REG_IRQ_FLAGS_MASK, (IRQ_TX_DONE_MASK | IRQ_FHSS_CHANNEL));
    return _SUCCESS;
  }
  else if((LoRa_readRegister(mylora, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) != 0) {//TX DONE
    if(mylora->tx_done_counter > 0){
      --mylora->tx_done_counter;
      return _FAILED;
    }
    LoRa_writeRegister(mylora, REG_IRQ_FLAGS_MASK, (IRQ_TX_DONE_MASK | IRQ_FHSS_CHANNEL));
    return _SUCCESS;
  }
  return _FAILED;
}

///////////////////////// 以下, レジスタ設定関数 ///////////////////////////////
void LoRa_mode_cad(LoRa_STRUCT* mylora) { // CAD_MODE
  LoRa_writeRegister(mylora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_CAD);
  int irqFlags = LoRa_readRegister(mylora, REG_IRQ_FLAGS);
  LoRa_writeRegister(mylora, REG_IRQ_FLAGS, irqFlags);
}

void LoRa_mode_continuous(LoRa_STRUCT* mylora) { // RX_CONTINUOUS MODE
  LoRa_writeRegister(mylora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
}

uint8_t LoRa_Rx_Size(LoRa_STRUCT* mylora) { // 受信byte数を返す
  return LoRa_readRegister(mylora, REG_RX_NB_BYTES);
}

//MRAMに登録されたパラメータをレジスタへ書き込み
void LoRa_load_default_setting(LoRa_STRUCT* mylora) {
  memcpy(&mylora->reg.frequencyMsb, 	&mylora->lora_reg_param[0], 	1);
  memcpy(&mylora->reg.frequencyMid, 	&mylora->lora_reg_param[1], 	1);
  memcpy(&mylora->reg.frequencyLsb, 	&mylora->lora_reg_param[2], 	1);
  memcpy(&mylora->reg.PaConfig, 		&mylora->lora_reg_param[3], 	1);
  memcpy(&mylora->reg.PaRamp, 		    &mylora->lora_reg_param[4], 	1);
  memcpy(&mylora->reg.OCP,			    &mylora->lora_reg_param[5], 	1);
  memcpy(&mylora->reg.LNA, 			    &mylora->lora_reg_param[6],	    1);
  memcpy(&mylora->reg.ModemConfig1, 	&mylora->lora_reg_param[7], 	1);
  memcpy(&mylora->reg.ModemConfig2, 	&mylora->lora_reg_param[8], 	1);
  memcpy(&mylora->reg.symbTimeout, 	    &mylora->lora_reg_param[9], 	1);
  memcpy(&mylora->reg.PreambleMsb, 	    &mylora->lora_reg_param[10],    1);
  memcpy(&mylora->reg.PreambleLsb, 	    &mylora->lora_reg_param[11],    1);
  memcpy(&mylora->reg.payload,		    &mylora->lora_reg_param[12],    1);
  memcpy(&mylora->reg.HopPeriod, 		&mylora->lora_reg_param[13],    1);
  memcpy(&mylora->reg.ModemConfig3, 	&mylora->lora_reg_param[14],    1);
  memcpy(&mylora->reg.AgcRefLevel, 	    &mylora->lora_reg_param[15],    1);
  memcpy(&mylora->reg.AgcStep1Lf, 	    &mylora->lora_reg_param[16],    1);
  memcpy(&mylora->reg.AgcStep2Lf, 	    &mylora->lora_reg_param[17],    1);
  memcpy(&mylora->reg.AgcStep3Lf,		&mylora->lora_reg_param[18],    1);
  memcpy(&mylora->reg.PllLf, 			&mylora->lora_reg_param[19],	1);

  LoRa_sleep(mylora);
  LoRa_writeRegister(mylora, REG_FRF_MSB, mylora->reg.frequencyMsb);
  LoRa_writeRegister(mylora, REG_FRF_MID, mylora->reg.frequencyMid);
  LoRa_writeRegister(mylora, REG_FRF_LSB, mylora->reg.frequencyLsb);
  LoRa_writeRegister(mylora, REG_PA_CONFIG, mylora->reg.PaConfig);
  LoRa_writeRegister(mylora, REG_PA_RAMP, mylora->reg.PaRamp);
  LoRa_writeRegister(mylora, REG_OCP, mylora->reg.OCP);
  LoRa_writeRegister(mylora, REG_LNA, mylora->reg.LNA);
  LoRa_writeRegister(mylora, REG_MODEM_CONFIG_1, mylora->reg.ModemConfig1);
  LoRa_writeRegister(mylora, REG_MODEM_CONFIG_2, mylora->reg.ModemConfig2);
  LoRa_writeRegister(mylora, REG_TimeoutLsb, mylora->reg.symbTimeout);
  LoRa_writeRegister(mylora, REG_PREAMBLE_MSB, mylora->reg.PreambleMsb);
  LoRa_writeRegister(mylora, REG_PREAMBLE_LSB, mylora->reg.PreambleLsb);
  LoRa_writeRegister(mylora, REG_PAYLOAD_LENGTH, mylora->reg.payload);
  LoRa_writeRegister(mylora, REG_HOP_PERIOD, mylora->reg.HopPeriod);
  LoRa_writeRegister(mylora, REG_MODEM_CONFIG_3, mylora->reg.ModemConfig3);
  LoRa_writeRegister(mylora, REG_SYNC_WORD, mylora->cipher.rx_sync_word);
  LoRa_idle(mylora);
  lora_tlm_update(mylora);
}

//LoRaのレジスタをリセット
void lora_setting_reset(LoRa_STRUCT* mylora) {
  HAL_GPIO_WritePin_C2A(mylora->reset_GPIO, mylora->reset_Pin, GPIO_PIN_SET);
  c2a_delay_ms(5);
  HAL_GPIO_WritePin_C2A(mylora->reset_GPIO, mylora->reset_Pin, GPIO_PIN_RESET);
}

///////////////////////// 以下, LORAテレメトリ用の関数 ///////////////////////////////
//LoRa TLMをバッファに詰める関数(LORAの情報は常に取得はしていないためコマンドで取得)
void lora_tlm_update(LoRa_STRUCT* mylora) {
  mylora->lora_tlm.reg_info.frequency = mylora->reg.frequencyMsb << 16 |
          mylora->reg.frequencyMid << 8 | mylora->reg.frequencyLsb;
  mylora->lora_tlm.reg_info.pa_info = mylora->reg.PaConfig << 24 | mylora->reg.PaRamp << 16 |
          mylora->reg.OCP << 8 | mylora->reg.LNA;
  mylora->lora_tlm.reg_info.modem_config = mylora->reg.ModemConfig1 << 24 |
          mylora->reg.ModemConfig2 << 16 | mylora->reg.ModemConfig3 << 8 | LoRa_readRegister(mylora, REG_MODEM_STATUS);
  mylora->lora_tlm.reg_info.timeout = (mylora->reg.ModemConfig2 & 0b11111100) << 8 |
          mylora->reg.symbTimeout;
  mylora->lora_tlm.reg_info.premble = mylora->reg.PreambleMsb << 8 | mylora->reg.PreambleLsb;
  mylora->lora_tlm.reg_info.hop_info = mylora->reg.HopPeriod << 8 | LoRa_readRegister(mylora, REG_HOP_CHANNEL);
  mylora->lora_tlm.reg_info.agc_step = mylora->reg.AgcRefLevel << 24 |
          mylora->reg.AgcStep1Lf << 16| mylora->reg.AgcStep2Lf << 8 | mylora->reg.AgcStep3Lf;
  mylora->lora_tlm.reg_info.pll = mylora->reg.PllLf;
  mylora->lora_tlm.reg_info.valid_header = LoRa_readRegister(mylora, REG_VALID_HEADER_MSB) << 8 |
      LoRa_readRegister(mylora, REG_VALID_HEADER_LSB);
  mylora->lora_tlm.reg_info.valid_packet = LoRa_readRegister(mylora, REG_VALID_PACKET_MSB) << 8 |
      LoRa_readRegister(mylora, REG_VALID_PACKET_LSB);
  mylora->lora_tlm.reg_info.receive_info = LoRa_readRegister(mylora, REG_RSSI_WIDEBAND) << 24 |
      LoRa_readRegister(mylora, REG_RSSI_WIDEBAND) << 16 | LoRa_readRegister(mylora, REG_RSSI_WIDEBAND) << 8 | LoRa_readRegister(mylora, REG_RSSI_WIDEBAND);
  mylora->lora_tlm.reg_info.freq_error = LoRa_readRegister(mylora, REG_FREQ_ERROR_MSB) << 16 |
      LoRa_readRegister(mylora, REG_FREQ_ERROR_MID) << 8 | LoRa_readRegister(mylora, REG_FREQ_ERROR_LSB);

  lora_get_temprature(mylora);
}

//LoRaが持つ温度センサから温度取得する関数
//温度取得後, すぐにTLM用のバッファに詰める
void lora_get_temprature(LoRa_STRUCT* mylora) {
  uint8_t now_mode = LoRa_readRegister(mylora,REG_OP_MODE);
  LoRa_writeRegister(mylora, REG_OP_MODE, now_mode | 0b01000000);
  mylora->lora_tlm.reg_info.lora_temp = LoRa_readRegister(mylora,REG_TEMP);

  print(LORA,"temp is: %02x", mylora->lora_tlm.reg_info.lora_temp);
  print(LORA,"\r\n");
  LoRa_writeRegister(mylora, REG_OP_MODE, now_mode);
}

///////////////////////// 以下, 暗号化関数 ///////////////////////////////
//暗号化関数
void lora_encryption(LoRa_STRUCT* mylora, uint8_t* in_buf, uint8_t* out_buf, size_t size) {
  if(mylora->cipher.lora_key_mode == PLAINTEXT) { //平文の場合
    for(int j = 0; j < size - 1; j++) {
      out_buf[j] = in_buf[j];
    }
  }
  else if(mylora->cipher.lora_key_mode == FIXATION) { //固定鍵の場合
    uint8_t key = mylora->cipher.fixed_key;
    size_t i = size -1;//bufの中身はゼロ起算なので

    //Nbyte目
    out_buf[i] = in_buf[i] ^ key;

    for(int j = i; j > 0; j--) {
      out_buf[j - 1] = in_buf[j - 1] ^ out_buf[j];
    }

    out_buf[0] = out_buf[0] ^ key;
  }
  else if(mylora->cipher.lora_key_mode == VARIABLE) { //可変鍵の場合
    uint8_t key = variable_key_maker(mylora);
    size_t i = size - 1;//bufの中身はゼロ起算なので

    //Nbyte目
    out_buf[i] = in_buf[i] ^ key;

    for(int j = i; j > 0; j--) {
      out_buf[j - 1] = in_buf[j - 1] ^ in_buf[j];
    }

    out_buf[0] = out_buf[0] ^ key;
  }
  else {
    for(int j = 0; j < size - 1; j++) { //設定を間違えどれにも当てはまらない場合, 平文として送信
      out_buf[j] = in_buf[j];
    }
  }
}

//複合化関数
_ACK lora_decryption(LoRa_STRUCT* mylora, TCP* in_buf, TCP* out_buf, size_t size) {
  uint8_t ret;

  //受信したデータに対して, 平文固定鍵可変鍵, 全て確認(可変鍵から順に確認していく)
  //可変鍵で受信できなかった場合, 固定鍵や平文で送信されるが, 受信側はどの形体で送信されたかは判別できないため
  ret = lora_check_variable_key(mylora, in_buf, out_buf, size);
  if(ret == _SUCCESS) {
    return ret;
  }

  ret = lora_check_fixation_key(mylora, in_buf, out_buf, size);
  if(ret == _SUCCESS) {
    return ret;
  }

  ret = lora_check_plaintext(mylora, in_buf, out_buf, size);
  if(ret == _SUCCESS) {
    return ret;
  }
  //ここまできたら復号化失敗
  return ret;
}

//可変鍵の変換し, 確認する関数
_ACK lora_check_variable_key(LoRa_STRUCT* mylora, TCP* in_buf, TCP* out_buf, size_t size) {
  uint8_t key = variable_key_maker(mylora);
  uint8_t ret;
  size_t i = size - 1;//bufの中身はゼロ起算なので

  //Nbyte目
  out_buf->packet[i] = in_buf->packet[i] ^ key;

  for(int j = i; j > 0; j--) {
    out_buf->packet[j - 1] = in_buf->packet[j - 1] ^ in_buf->packet[j];
  }

  out_buf->packet[0] = out_buf->packet[0] ^ key;

  switch(TCP_get_type(out_buf)) {
    case TCP_TYPE_TLM:
      ret = lora_check_tlm(out_buf, size);
      return ret;
    case TCP_TYPE_CMD:
      ret = lora_check_command(out_buf, size);
      return ret;
    default:
      return _UNKNOWN;
  }
}

//固定鍵の変換し, 確認する関数
_ACK lora_check_fixation_key(LoRa_STRUCT* mylora, TCP* in_buf, TCP* out_buf, size_t size) {
  uint8_t key = mylora->cipher.fixed_key;
  uint8_t ret;
  size_t i = size -1;//bufの中身はゼロ起算なので

  //Nbyte目
  out_buf->packet[i] = in_buf->packet[i] ^ key;

  for(int j = i; j > 0; j--) {
    out_buf->packet[j - 1] = in_buf->packet[j - 1] ^ in_buf->packet[j];
  }

  out_buf->packet[0] = out_buf->packet[0] ^ key;

  switch(TCP_get_type(out_buf)) {
    case TCP_TYPE_TLM:
      ret = lora_check_tlm(out_buf, size);
      return ret;
    case TCP_TYPE_CMD:
      ret = lora_check_command(out_buf, size);
      return ret;
    default:
      return _UNKNOWN;
  }
}

//平文かを確認する関数
_ACK lora_check_plaintext(LoRa_STRUCT* mylora, TCP* in_buf, TCP* out_buf, size_t size) {
  uint8_t ret;
  memcpy(&out_buf->packet[0], &in_buf->packet[0], size);

  switch(TCP_get_type(out_buf)) {
    case TCP_TYPE_TLM:
      ret = lora_check_tlm(out_buf, size);
      return ret;
    case TCP_TYPE_CMD:
      ret = lora_check_command(out_buf, size);
      return ret;
    default:
      return _UNKNOWN;
  }
}

//鍵変換数内で自分宛てのCMDかどうかを確認する関数
_ACK lora_check_command(TCP* packet, size_t size) {
  switch (TCP_get_apid_project_id(packet)) { //project ID check
    case MAGNAROII:
      return _SUCCESS;
    default:
      return _UNKNOWN;
  }
}

//鍵変換数内で自分宛てのTLMかどうかを確認する関数
_ACK lora_check_tlm(TCP* packet, size_t size) {
  switch (TCP_get_apid_project_id(packet)) {// project ID check
    case MAGNAROII:
      break;
    default:
      return _UNKNOWN;
  }

  switch (TCP_TLM_get_category(packet)) {// CMD type check
  case 0x20://HK
  case 0x40://MS
  case 0x80://ST
  case 0x00://RT
    return _SUCCESS;
  default:
    return _UNKNOWN;
  }
}

//固定鍵生成関数(固定鍵の作成関数はPICと同じ)
uint8_t variable_key_maker(LoRa_STRUCT* mylora) {
  RXPIC_TlmData* rxpic_tlm = &(rxpic_driver->tlm_data);
  uint8_t key;
  uint32_t temp_key1 = 0;
  uint32_t temp_key2 = 0;
  uint32_t mask = 0x00ffff00;
  temp_key1 = rxpic_tlm->rxpic_to_mobc.RXPIC_Time & mask;
  temp_key2 = rxpic_tlm->rxpic_to_mobc.RXPIC_Time + temp_key1;

  key = (uint8_t)(temp_key2 << 16);
  key = key & mylora->cipher.variable_key;

  return key;
}

