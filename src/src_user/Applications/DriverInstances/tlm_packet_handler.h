#ifndef TLM_PACKET_HANDLER_H_
#define TLM_PACKET_HANDLER_H_

//#include "../../Drivers/COM/RXPIC/rxpic.h"
//#include "../../Drivers/COM/TXPIC/txpic.h"
#include "di_rxpic.h"
#include "di_txpic.h"
#include "di_cobc.h"
#include "di_sobc.h"
#include "di_intersat.h"
#include "di_lora.h"
#include "../../Drivers/CDH/LoRa/LoRa_func.h"
#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Drivers/large_packet_driver.h>

/**
 * @brief  PHのTXPIC送信用初期化処理
 *
 *         PacketListの初期化を行う
 * @param  void
 * @return void
 */
void PH_tlm_init(void);

typedef struct
{
  cycle_t tcf_last_recv_time;
  PH_ACK cmd_ack;
} TPH;

extern const TPH* tph;

//HKとMSテレメの送信
AppInfo TLM_mst_packet_handler(void);

AppInfo TLM_rpt_packet_handler(void);

AppInfo TLM_txpic_packet_handler(void);

AppInfo TLM_txpic_largepacket_handler(void);

PH_ACK PH_add_txpic_tlm_(const CommonCmdPacket* packet);

// ST_TLMはDRが処理するのでない

#endif // GSTOS_PACKET_HANDLER_H_
