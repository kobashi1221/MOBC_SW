/*
 * COM_cmd_LoRa.c
 *
 *  Created on: 2021/07/14
 *      Author: kahaku
 *
 *  update : 2023/11/14
 *  	Author: sakaguchi
 */

#include "COM_cmd_LoRa.h"

#include <string.h> //for memcpy
#include <stdlib.h> //for malloc
#include <src_user/Drivers/Cdh/LoRa/LoRa_func.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/middleware/MemoryMW.h>
#include <src_user/Library/General/DBG_printf.h>

static void COM_lora_load_param(void);
static void COM_lora_initialize_param(void);

const uint8_t lora_key_mode_ = PLAINTEXT;
const uint8_t fixed_key_ = 0xd3;//PICに合わせる
const uint8_t variable_key_ = 0x92;//PICに合わせる
#ifdef ONE_U
const uint8_t tx_sync_word_ = 0x34;
const uint8_t rx_sync_word_ = 0x73;
#endif
#ifdef TWO_U
const uint8_t tx_sync_word_ = 0x73;
const uint8_t rx_sync_word_ = 0x34;
#endif

const uint8_t wait_counter_ = 1;
const uint8_t done_counter_ = 0x03;
const uint8_t lora_rx_mode_ = RX_CAD;

const uint8_t lora_reg_param_[20] =
{
  0xE4,	    //frequencyMsb 0x00003601
  0xC0,	    //frequencyMid 0x00003602
  0x00,	    //frequencyLsb 0x00003603
  0x8a, 	//PaConfig 0x00003604
  0x09,	    //PaRamp 0x00003605
  0x34,	    //OCP 0x00003606
  0x23, 	//LNA 0x00003607
  0x72, 	//ModemConfig1 0x00003608
  0x80, 	//ModemConfig2 0x00003609
  0xff, 	//symbTimeout 0x0000360a
  0x05,	    //PreambleMsb 0x0000360b
  0x50,	    //PreambleLsb 0x0000360c
  0x01,	    //payload length for implicit mode 0x0000360d
  0x0a,	    //HopPeriod 0x0000360e
  0x04,	    //ModemConfig3 0x0000360f
  0x1c,	    //AgcRefLevel 0x00003610
  0x0e,	    //AgcStep1Lf 0x00003611
  0x5B,	    //AgcStep2Lf 0x00003612
  0xCC,	    //AgcStep3Lf 0x00003613
  0xD0,	    //PllLf 0x00003614
};

AppInfo CDH1_lora_param_update(void) {
  return AI_create_app_info("LORA_PARAM_UPDATE", COM_lora_load_param, COM_lora_initialize_param);
}

static void COM_lora_load_param(void) {
  read_param(&lora_driver->cipher.lora_key_mode, &lora_key_mode_, 1, (uint32_t)0x00003600);
  read_param(&lora_driver->cipher.fixed_key, &fixed_key_, 1, (uint32_t)0x00003601);
  read_param(&lora_driver->cipher.variable_key, &variable_key_, 1, (uint32_t)0x00003602);
  read_param(&lora_driver->cipher.tx_sync_word, &tx_sync_word_, 1, (uint32_t)0x00003603);
  read_param(&lora_driver->cipher.rx_sync_word, &rx_sync_word_, 1, (uint32_t)0x00003604);
  read_param(&lora_driver->wait_counter, &wait_counter_, 1, (uint32_t)0x00003605);
  read_param(&lora_driver->done_counter, &done_counter_, 1, (uint32_t)0x00003606);
  read_param(&lora_driver->lora_rx_mode, &lora_rx_mode_, 1, (uint32_t)0x00003607);
  read_param(&lora_driver->lora_reg_param, &lora_reg_param_, 20, (uint32_t)0x00003608);
}

static void COM_lora_initialize_param(void) {
  write_param(&lora_driver->cipher.lora_key_mode, &lora_key_mode_, 1, (uint32_t)0x00003600);
  write_param(&lora_driver->cipher.fixed_key, &fixed_key_, 1, (uint32_t)0x00003601);
  write_param(&lora_driver->cipher.variable_key, &variable_key_, 1, (uint32_t)0x00003602);
  write_param(&lora_driver->cipher.tx_sync_word, &tx_sync_word_, 1, (uint32_t)0x00003603);
  write_param(&lora_driver->cipher.rx_sync_word, &rx_sync_word_, 1, (uint32_t)0x00003604);
  write_param(&lora_driver->wait_counter, &wait_counter_, 1, (uint32_t)0x00003605);
  write_param(&lora_driver->done_counter, &done_counter_, 1, (uint32_t)0x00003606);
  write_param(&lora_driver->lora_rx_mode, &lora_rx_mode_, 1, (uint32_t)0x00003607);
  write_param(&lora_driver->lora_reg_param, &lora_reg_param_, 20, (uint32_t)0x00003608);
}

/*
*
 * @brief　  LoRaの設定を変更するデバッグ用関数.
 * param[0]: 	Address Name (実行する commandID)
 * param[1]  : 与える値　(実行するコマンドに与える引数)
 *
*/
CCP_CmdRet Cmd_COM_LoRa_Debug_Set(const CommonCmdPacket* packet) {
  const uint8_t* param = CCP_get_param_head(packet);

  print(LORA,"value[0] is %02x \r\n", param[0]);
  print(LORA,"value[1] is %02x \r\n", param[1]);

  LoRa_writeRegister(lora_driver,param[0], param[1]);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*
 * @brief	LoRaの設定を確認するデバッグ用関数.
 * param[0]: 	Address Name (実行する commandID)
 * param[1]  : 与える値　(確認する値)
 *
*/
CCP_CmdRet Cmd_COM_LoRa_Debug_Check(const CommonCmdPacket* packet) {
  const uint8_t* param = CCP_get_param_head(packet);

  print(LORA,"value[0] is %02x \r\n", param[0]);

  uint8_t ret = LoRa_readRegister(lora_driver,param[0]);

  printf("read register is: %02x", ret);
  printf("\r\n");
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//MOBCからコマンドを受け取り次第、送り先で実行したいコマンドのIDと引数を送るコマンド
/**
 * @brief   Transmit Another MOBC a Command.
 * param[0~1]:   Channel ID (command ID)
 * param[2]  :   Parameters (実行するコマンドID)
 *
 */
CCP_CmdRet Cmd_COM_LoRa_Debug_Transmit(const CommonCmdPacket* packet) {
  DS_CMD_ERR_CODE ret;
  TCP* pckt = (TCP*)packet; // const_cast
  // ここでTCP_CMD_DEST_TYPEを宛先で受理できるように変更（なので const cast が発生している．．．）

  const int len =TCP_get_packet_len(pckt) + TCP_PRM_HDR_LEN;

  // LoRaでの送信を明示
  TCP_set_route(pckt,TCP_ROUTE_LORA);

  //chID分長さが変わるから
  TCP_set_packet_data_len(pckt, TCP_get_packet_len(pckt) - 2);

#ifdef ONE_U
  TCP_set_apid_sat_id(pckt, TCP_SAT_ID_2U);
#endif

#ifdef TWO_U
  TCP_set_apid_sat_id(pckt, TCP_SAT_ID_1U);
#endif

  //ここまででcmdとparam以外はmobc宛てに置き換わった
  uint8_t lora_packet[len - 3];

  //一旦, 他衛星宛てのpacketにコピー
  for(int i = 0;i < len - 3; i++){
    lora_packet[i] =  pckt->packet[i];
  }

  lora_packet[8] = pckt->packet[17];
  lora_packet[9] = pckt->packet[18];

  if(len >17){
    for(int i = 0;i < (len - 19); i++){
      lora_packet[i + 17] =  pckt->packet[i + 19];
    }
  }

  ret = (DS_CMD_ERR_CODE)LORA_Send(lora_driver, lora_packet, len - 2);
  ANOMALY_SPI_Set_Status(SPI_FAULT_MRAM_LORA, ret);
// *************************debug read register****************************************************//
  for(int i=0;i<len - 1;i++) print(LORA,"%02x ",pckt->packet[i]);
// ***********************************************************************************************//
  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

/*
 * @brief	LoRaからリアルタイムコマンドを送信するコマンド(送信側は地上局, 軌道上では使用しない).
 * param[0]: ルート
 * param[1]: sat ID
 * param[2]: exe ID
 * param[3] ~ [4]: ch id
 * param[5] ~ : param
 *
*/
//完全に新しいパケットを生成する
CCP_CmdRet Cmd_COM_user_RTCmd_send(const CommonCmdPacket* packet) {
  const uint8_t* param = CCP_get_param_head(packet);
  uint16_t len = CCP_get_param_len(packet);
  uint16_t next_param_len = len - 5;
  uint8_t length = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN + next_param_len;
  uint16_t ch_id;
  ENDIAN_memcpy(&ch_id, &param[3], 2);
  TCP tcp;
#ifdef TWO_U
  CCP_form_rtc_to_other_obc(&tcp, TCP_APID_MOBC_2U, (CMD_CODE)ch_id,&param[5], next_param_len);
#endif
#ifdef ONE_U
  CCP_form_rtc_to_other_obc(&tcp, TCP_APID_MOBC_1U, (CMD_CODE)ch_id,&param[5], next_param_len);
#endif
  TCP_set_route(&tcp, param[0]);//ROUTEを変更する
  TCP_set_apid_(&tcp, param[1], param[2]);//実行先を変更する
#ifdef TWO_U
  TCP_set_all_issuer_id(&tcp, TCP_SAT_ID_GS_2U, TCP_EXE_ID_LORA);
#endif
#ifdef ONE_U
  TCP_set_all_issuer_id(&tcp, TCP_SAT_ID_GS_1U, TCP_EXE_ID_LORA);
#endif
  TCP_CMD_set_exec_type(&tcp, TCP_CMD_EXEC_TYPE_GS);

  LORA_Send(lora_driver, &tcp.packet[0], length);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*
 * @brief	LoRaかタイムラインコマンドを送信するコマンド(送信側は地上局, 軌道上では使用しない).
 * param[0]: ルート
 * param[1]: sat ID
 * param[2]: exe ID
 * param[3] ~ [4]: ch id
 * param[5] ~ [8]: time(OBC時刻)
 * param[9] ~ : param
 *
*/
CCP_CmdRet Cmd_COM_user_TLCmd_send(const CommonCmdPacket* packet) {
  const uint8_t* param = CCP_get_param_head(packet);
  uint16_t len = CCP_get_param_len(packet);
  uint16_t next_param_len = len - 9;
  uint8_t length = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN + next_param_len;
  uint16_t ch_id;
  ENDIAN_memcpy(&ch_id, &param[3], 2);
  uint32_t exe_time;
  ENDIAN_memcpy(&exe_time, &param[5], 4);
  TCP tcp;
#ifdef TWO_U
  CCP_form_tlc_to_other_obc(&tcp, exe_time, TCP_APID_MOBC_2U, ch_id, &param[9], next_param_len);
#endif
#ifdef ONE_U
  CCP_form_tlc_to_other_obc(&tcp, exe_time, TCP_APID_MOBC_1U, ch_id, &param[9], next_param_len);
#endif
  TCP_set_route(&tcp, param[0]);//ROUTEを変更する
  TCP_set_apid_(&tcp, param[1], param[2]);//実行先を変更する
#ifdef TWO_U
  TCP_set_all_issuer_id(&tcp, TCP_SAT_ID_GS_2U, TCP_EXE_ID_LORA);
#endif
#ifdef ONE_U
  TCP_set_all_issuer_id(&tcp, TCP_SAT_ID_GS_1U, TCP_EXE_ID_LORA);
#endif
  TCP_CMD_set_exec_type(&tcp, TCP_CMD_EXEC_TYPE_TL);

  LORA_Send(lora_driver, &tcp.packet[0], length);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*
 * @brief   LoRaのレジスタへMRAMパラメータの値を書き込むコマンド.
 *
*/
CCP_CmdRet Cmd_COM_LoRa_default_setting(const CommonCmdPacket* packet) {
  (void) packet;
  LoRa_load_default_setting(lora_driver);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*
 * @brief   LoRaのテレメトリを生成するコマンド.
 *
*/
CCP_CmdRet Cmd_COM_LoRa_tlm_update(const CommonCmdPacket* packet){
  (void) packet;
  lora_tlm_update(lora_driver);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
