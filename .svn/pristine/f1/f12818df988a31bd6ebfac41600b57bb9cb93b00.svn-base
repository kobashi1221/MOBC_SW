/**
  *****************************************************************************************
  * @file    : COM_cmd_INTERSAT.c
  * @author  : Sakaguchi
  * @version : 1.0
  * @date    : 2021/11/26
  * @brief   :　他機とのUART通信用
  *****************************************************************************************
  */

#include "COM_cmd_INTERSAT.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <string.h>//for memcpy
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_core/TlmCmd/telemetry_frame.h>
#include <src_user/Applications/DriverInstances/di_LoRa.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.h>
#include <src_user/Applications/UserDefined/Cdh/ResetUtility.h>


const uint8_t intersat_tlm_flag_ = SEND_TLM;
const uint16_t divide_count = 10;
const uint16_t lora_divide_count = 600;

static void COM_intersat_tlm_init(void);
static void COM_intersat_tlm_exe(void);

static void COM_intersat_param_init(void);
static void COM_intersat_param_exe(void);

AppInfo COM_intersat_param_update(void){
  return AI_create_app_info("COBC_PARAM", COM_intersat_param_init, COM_intersat_param_exe);
}

static void COM_intersat_param_init(void)
{
  read_param(&intersat_driver->intersat_tlm_flag, &intersat_tlm_flag_, 1, (uint32_t)0x0000361C);
  read_param(&intersat_driver->divide_count, &divide_count, 2, (uint32_t)0x0000361D);
  read_param(&intersat_driver->lora_divide_count, &lora_divide_count, 2, (uint32_t)0x0000361F);
}

static void COM_intersat_param_exe(void)
{
  write_param(&intersat_driver->intersat_tlm_flag, &intersat_tlm_flag_, 1, (uint32_t)0x0000361C);
  write_param(&intersat_driver->divide_count, &divide_count, 2, (uint32_t)0x0000361D);
  write_param(&intersat_driver->lora_divide_count, &lora_divide_count, 2, (uint32_t)0x0000361F);
}


AppInfo COM_intersat_tlm_update(void){
  return AI_create_app_info("COBC_TLM_UPDATE", COM_intersat_tlm_init, COM_intersat_tlm_exe);
}


CCP_EXEC_STS Cmd_COM_debug_Intersat_Send(const CommonCmdPacket* packet){
  uint8_t data[3] = {0x11, 0x22, 0x22};
  INTERSAT_Send(intersat_driver,data,3);

  if (intersat_driver->uart_data[0] != 3){
    printf("INTERSAT RECEIVE ERR\r\n");
  }
  return CCP_EXEC_SUCCESS;
}

static void COM_intersat_tlm_init(void){

}

static void COM_intersat_tlm_exe(void){
  TF_TLM_FUNC_ACK ret;
  uint8_t state = 0;
  state = PWR_State_GET(PWR_State_LORA);

  if(Separated() == NOT_SEPARATED){ //分離していないとき
    if(intersat_driver->divide_count <= 0){
      intersat_driver->divide_count = 1;
    }
    if(intersat_driver->intersat_tlm_flag == SEND_TLM){
      if((TMGR_get_master_total_cycle()%intersat_driver->divide_count)==0){
        TCP tcp_;
#ifdef ONE_U
        uint16_t apid = 0x0106;
#endif
#ifdef TWO_U
        uint16_t apid = 0x0116;
#endif
        uint8_t route = TCP_ROUTE_MILLMAX;
        uint8_t category = 0x40;
        uint8_t id = Tlm_CODE_INTERSAT;
        uint16_t len;

        // ADU生成
        // ADU分割が発生しない場合に限定したコードになっている。
        // TLM定義シート上で定義するADUはADU長をADU分割が発生しない長さに制限する。
        ret = TF_generate_contents((int)id,
                        tcp_.packet,
                        &len,
                        TCP_MAX_LEN);

        // 範囲外のTLM IDを除外
        if (ret == TF_TLM_FUNC_ACK_NOT_DEFINED) return;
        if (len < 0) return;     // TODO: lenがマイナスの値たちをどうするか？

        // TCPacketヘッダ設定
        TCP_TLM_setup_primary_hdr(&tcp_, apid, len);
        TCP_set_route(&tcp_, route);
        TCP_set_all_issuer_id(&tcp_, MY_SAT_ID, TCP_EXE_ID_MOBC);
        TCP_TLM_set_startup_count(&tcp_, (uint8_t)p_reset_struct->reset_count[TOTAL_RESET]);
        TCP_TLM_set_ti(&tcp_, (uint32_t)(TMGR_get_master_total_cycle()));
        TCP_TLM_set_category(&tcp_, category);
        TCP_TLM_set_packet_id(&tcp_, id);

        PH_analyze_packet(&tcp_);
      }
    }
    else if((intersat_driver->intersat_tlm_flag) == LIFE_COUNT && (TMGR_get_master_total_cycle()%10)==0){
      TCP tcp;
      uint32_t ap_id = AR_NOP;
      uint8_t param[4];
      uint8_t len = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN + sizeof(param);
      param[0] = (uint8_t)(ap_id << 24);
      param[1] = (uint8_t)(ap_id << 16);
      param[2] = (uint8_t)(ap_id << 8);
      param[3] = (uint8_t)(ap_id);

#ifdef TWO_U
      CCP_form_rtc_to_other_obc(&tcp, TCP_APID_MOBC_1U, (CMD_CODE)Cmd_CODE_AM_EXECUTE_APP,&param[0],sizeof(param));
#endif
#ifdef ONE_U
      CCP_form_rtc_to_other_obc(&tcp, TCP_APID_MOBC_2U, (CMD_CODE)Cmd_CODE_AM_EXECUTE_APP,&param[0],sizeof(param));
#endif
      INTERSAT_Send(intersat_driver,&tcp.packet[0],len);
    }
  }
  else if(Separated() == SEPARATED && state == 0x01){// 分離状態かつLoRa ON
    if(intersat_driver->lora_divide_count <= 0){
      intersat_driver->lora_divide_count = 600;
    }

    if(intersat_driver->intersat_tlm_flag == SEND_TLM){
      if((TMGR_get_master_total_cycle()%intersat_driver->lora_divide_count)==0){
        TCP tcp_;
#ifdef ONE_U
        uint16_t apid = 0x0106;
#endif
#ifdef TWO_U
        uint16_t apid = 0x0116;
#endif
        uint8_t route = TCP_ROUTE_LORA;
        uint8_t category = 0x40;
        uint8_t id = Tlm_CODE_INTERSAT;
        uint16_t len;

        // ADU生成
        // ADU分割が発生しない場合に限定したコードになっている。
        // TLM定義シート上で定義するADUはADU長をADU分割が発生しない長さに制限する。
        ret = TF_generate_contents((int)id,
                        tcp_.packet,
                        &len,
                        TCP_MAX_LEN);

        // 範囲外のTLM IDを除外
        if (ret == TF_TLM_FUNC_ACK_NOT_DEFINED) return;
        if (len < 0) return;     // TODO: lenがマイナスの値たちをどうするか？

        // TCPacketヘッダ設定
        TCP_TLM_setup_primary_hdr(&tcp_, apid, len);
        TCP_set_route(&tcp_, route);
        TCP_set_all_issuer_id(&tcp_, MY_SAT_ID, TCP_EXE_ID_MOBC);
        TCP_TLM_set_startup_count(&tcp_, (uint8_t)p_reset_struct->reset_count[TOTAL_RESET]);
        TCP_TLM_set_ti(&tcp_, (uint32_t)(TMGR_get_master_total_cycle()));
        TCP_TLM_set_category(&tcp_, category);
        TCP_TLM_set_packet_id(&tcp_, id);

        PH_analyze_packet(&tcp_);
      }
    }
    else if((intersat_driver->intersat_tlm_flag) == LIFE_COUNT && (TMGR_get_master_total_cycle()%intersat_driver->lora_divide_count)==0){
      TCP tcp;
      uint32_t ap_id = AR_NOP;
      uint8_t param[4];
      uint8_t len = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN + sizeof(param);
      param[0] = (uint8_t)(ap_id << 24);
      param[1] = (uint8_t)(ap_id << 16);
      param[2] = (uint8_t)(ap_id << 8);
      param[3] = (uint8_t)(ap_id);

#ifdef TWO_U
      CCP_form_rtc_to_other_obc(&tcp, TCP_APID_MOBC_1U, (CMD_CODE)Cmd_CODE_AM_EXECUTE_APP,&param[0],sizeof(param));
#endif
#ifdef ONE_U
      CCP_form_rtc_to_other_obc(&tcp, TCP_APID_MOBC_2U, (CMD_CODE)Cmd_CODE_AM_EXECUTE_APP,&param[0],sizeof(param));
#endif
      LORA_Send(lora_driver,&tcp.packet[0],len);
    }
  }
}
