#pragma section REPRO
/**
 * @file
 * @brief  packet_handlerのコマンドルーター，コマンドアナライザのユーザー定義部分
 */
#include "user_packet_handler.h"
#include <src_core/TlmCmd/packet_list_util.h>
#include "../Applications/DriverInstances/di_rxpic.h"
#include "../Applications/DriverInstances/di_txpic.h"
#include "../Applications/DriverInstances/di_rxpic.h"
#include "../Applications/DriverInstances/di_cobc.h"
#include "../Applications/DriverInstances/di_sobc.h"
#include "../Applications/DriverInstances/di_lora.h"
#include "../Applications/DriverInstances/di_intersat.h"
#include "../Applications/DriverInstances/command_response_handler.h"
//#include "../Applications/DriverInstances/di_aobc.h"
//#include "../Applications/DriverInstances/di_tobc.h"

#define PH_RXPIC_CMD_LIST_MAX  (4)          //!< RXPIC CMD QUEUEサイズ
#define PH_TXPIC_CMD_LIST_MAX  (4)          //!< TXPIC CMD QUEUEサイズ
#define PH_COBC_CMD_LIST_MAX  (6)           //!< COBC CMD QUEUEサイズ
#define PH_SOBC_CMD_LIST_MAX  (4)           //!< SOBC CMD QUEUEサイズ
#define PH_LORA_CMD_LIST_MAX  (4)           //!< LORA CMD QUEUEサイズ
#define PH_INTERSAT_CMD_LIST_MAX  (4)       //!< INTERSAT CMD QUEUEサイズ
#define PH_SD_TLM_LIST_MAX (8)		    	//!< SD TLM QUEUEサイズ
#define PH_TXPIC_TLM_LIST_MAX  (16)

static PH_ACK PH_add_rxpic_cmd_(const CommonTlmCmdPacket* packet);
static PH_ACK PH_add_txpic_cmd_(const CommonTlmCmdPacket* packet);
static PH_ACK PH_add_cobc_cmd_(const CommonTlmCmdPacket* packet);
static PH_ACK PH_add_lora_cmd_(const CommonTlmCmdPacket* packet);
static PH_ACK PH_add_sobc_cmd_(const CommonTlmCmdPacket* packet);
static PH_ACK PH_add_intersat_cmd_(const CommonTlmCmdPacket* packet);

static PH_ACK PH_add_tl_cmd_user(int line_no,
                            const CommonTlmCmdPacket* packet,
                            size_t now);

PacketList PH_rxpic_cmd_list;
PacketList PH_txpic_cmd_list;
PacketList PH_cobc_cmd_list;
PacketList PH_sobc_cmd_list;
PacketList PH_lora_cmd_list;
PacketList PH_intersat_cmd_list;
PacketList PH_sd_tlm_list;
PacketList PH_txpic_tlm_list;


static PL_Node PH_rxpic_cmd_stock_[PH_RXPIC_CMD_LIST_MAX];
static PL_Node PH_txpic_cmd_stock_[PH_TXPIC_CMD_LIST_MAX];
static PL_Node PH_cobc_cmd_stock_[PH_COBC_CMD_LIST_MAX];
static PL_Node PH_sobc_cmd_stock_[PH_SOBC_CMD_LIST_MAX];
static PL_Node PH_lora_cmd_stock_[PH_LORA_CMD_LIST_MAX];
static PL_Node PH_intersat_cmd_stock_[PH_INTERSAT_CMD_LIST_MAX];
static PL_Node PH_sd_tlm_stock_[PH_SD_TLM_LIST_MAX];
static PL_Node PH_txpic_tlm_stock_[PH_TXPIC_TLM_LIST_MAX];
static CommonCmdPacket PH_rxpic_ccp_stock_[PH_RXPIC_CMD_LIST_MAX];
static CommonCmdPacket PH_txpic_ccp_stock_[PH_TXPIC_CMD_LIST_MAX];
static CommonCmdPacket PH_cobc_ccp_stock_[PH_COBC_CMD_LIST_MAX];
static CommonCmdPacket PH_sobc_ccp_stock_[PH_SOBC_CMD_LIST_MAX];
static CommonCmdPacket PH_lora_ccp_stock_[PH_LORA_CMD_LIST_MAX];
static CommonCmdPacket PH_intersat_ccp_stock_[PH_INTERSAT_CMD_LIST_MAX];
static CommonTlmCmdPacket PH_sd_ctcp_stock_[PH_SD_TLM_LIST_MAX];
static CommonTlmCmdPacket PH_txpic_ctcp_stock_[PH_TXPIC_TLM_LIST_MAX];

void PH_user_init(void)
{
  PL_initialize_with_ccp(PH_rxpic_cmd_stock_, PH_rxpic_ccp_stock_, PH_RXPIC_CMD_LIST_MAX, &PH_rxpic_cmd_list);
  PL_initialize_with_ccp(PH_txpic_cmd_stock_, PH_txpic_ccp_stock_, PH_TXPIC_CMD_LIST_MAX, &PH_txpic_cmd_list);
  PL_initialize_with_ccp(PH_cobc_cmd_stock_, PH_cobc_ccp_stock_, PH_COBC_CMD_LIST_MAX, &PH_cobc_cmd_list);
  PL_initialize_with_ccp(PH_sobc_cmd_stock_, PH_sobc_ccp_stock_, PH_SOBC_CMD_LIST_MAX, &PH_sobc_cmd_list);
  PL_initialize_with_ccp(PH_lora_cmd_stock_, PH_lora_ccp_stock_, PH_LORA_CMD_LIST_MAX, &PH_lora_cmd_list);
  PL_initialize_with_ccp(PH_intersat_cmd_stock_, PH_intersat_ccp_stock_, PH_INTERSAT_CMD_LIST_MAX, &PH_intersat_cmd_list);
  PL_initialize_with_ctcp(PH_sd_tlm_stock_, PH_sd_ctcp_stock_, PH_SD_TLM_LIST_MAX, &PH_sd_tlm_list);
  PL_initialize_with_ctcp(PH_txpic_tlm_stock_, PH_txpic_ctcp_stock_, PH_TXPIC_TLM_LIST_MAX, &PH_txpic_tlm_list);

}

PH_ACK PH_user_analyze_cmd(const CommonCmdPacket* packet){
  TCP_SAT_ID sat_id=TCP_get_apid_sat_id(packet);
  //コマンド応答処
  command_response_handler(packet);
  if((sat_id & 0x01) == MY_SAT_ID){ //自機および自機からGS宛て
    switch(TCP_get_apid_executer_id(packet)){
      case TCP_EXE_ID_RXPIC:
        return (PH_add_rxpic_cmd_( packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;
      case TCP_EXE_ID_GMSK19k2:
      case TCP_EXE_ID_GMSK9k6:
      case TCP_EXE_ID_AFSK:
      case TCP_EXE_ID_TXPIC:
        return (PH_add_txpic_cmd_(packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;
#ifdef TWO_U
      case TCP_EXE_ID_COBC:
	if(CCP_get_exec_type(packet) == CCP_EXEC_TYPE_TL_FROM_GS){
	  int ret;
	  ret = PH_add_tl_cmd_user(0, packet, (size_t)(TMGR_get_master_total_cycle()));
          return ret;
        }
	return (PH_add_cobc_cmd_(packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;
#endif
     case TCP_EXE_ID_SOBC:
        return (PH_add_sobc_cmd_( packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;

      default://自分宛
        return PH_ACK_UNKNOWN;
    }
  }
  else{//自機宛でない (他衛星宛てのコマンド)
    switch(TCP_get_route(packet)){
      case TCP_ROUTE_AFSK:
	return (PH_add_txpic_cmd_(packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;
      case TCP_ROUTE_MILLMAX:
	if(CCP_get_exec_type(packet) == CCP_EXEC_TYPE_TL_FROM_GS){
	  int ret;
	  ret = PH_add_tl_cmd_user(0, packet, (size_t)(TMGR_get_master_total_cycle()));
	  return ret;
	}
	return (PH_add_intersat_cmd_(packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;
      case TCP_ROUTE_LORA:
	if(CCP_get_exec_type(packet) == CCP_EXEC_TYPE_TL_FROM_GS){
	  int ret;
	  ret = PH_add_tl_cmd_user(0, packet, (size_t)(TMGR_get_master_total_cycle()));
	  return ret;
	}
	return (PH_add_lora_cmd_(packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;
      default :
	return PH_ACK_UNKNOWN;
    }
  }
}

CCP_CmdRet PH_user_cmd_router(const CommonCmdPacket* packet){
  TCP_SAT_ID sat_id=TCP_get_apid_sat_id(packet);
  if((sat_id & 0x01) == MY_SAT_ID){ //自機および自機からGS宛て
  	switch(TCP_get_apid_executer_id(packet)){
			case TCP_EXE_ID_RXPIC:
				return rxpic_dispatch_command(packet);
			case TCP_EXE_ID_GMSK19k2:
			case TCP_EXE_ID_GMSK9k6:
			case TCP_EXE_ID_AFSK:
			case TCP_EXE_ID_LORA:
			case TCP_EXE_ID_TXPIC:
				return txpic_dispatch_command(packet);
#ifdef TWO_U
			case TCP_EXE_ID_COBC:
				return cobc_dispatch_command(packet);
#endif
            case TCP_EXE_ID_SOBC:
                return sobc_dispatch_command(packet);
			default:
				return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ROUTING_FAILED);
  	}
  }
  else{//自機宛でない (他衛星宛てのコマンド)
  	switch(TCP_get_route(packet)){
	  		case TCP_ROUTE_AFSK:
	  			return txpic_dispatch_command(packet);
	  		case TCP_ROUTE_MILLMAX://
	  			return intersat_dispatch_command(packet);
	  		case TCP_ROUTE_LORA:
  			return lora_dispatch_command(packet);
	  		default :
	  			return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ROUTING_FAILED);
  	}
  }
}

TF_TLM_FUNC_ACK PH_user_telemetry_router(APID apid,
                                         TLM_CODE tlm_id,
                                         uint8_t* packet,
                                         uint16_t* len,
                                         uint16_t max_len){

}

static PH_ACK PH_add_tl_cmd_user(int line_no,
                            const CommonTlmCmdPacket* packet,
                            size_t now)
{//他OBC用のTLC追加関数

  PL_ACK ack = PL_insert_tl_cmd(&(PH_tl_cmd_list[line_no]), packet, now);


  switch (ack)
  {
  case PL_SUCCESS:
    return PH_ACK_TLC_SUCCESS;

  case PL_LIST_FULL:
    return PH_ACK_PL_LIST_FULL;

  case PL_TLC_PAST_TIME:
    return PH_ACK_TLC_PAST_TIME;

  case PL_TLC_ALREADY_EXISTS:
    return PH_ACK_TLC_ALREADY_EXISTS;

  default:
    return PH_ACK_UNKNOWN;
  }
}


static PH_ACK PH_add_rxpic_cmd_(const CommonCmdPacket* packet)
{
  PL_ACK ack = PL_push_back(&PH_rxpic_cmd_list, packet);

  if (ack != PL_SUCCESS) return PH_ACK_PL_LIST_FULL;
  return PH_ACK_SUCCESS;
}


static PH_ACK PH_add_txpic_cmd_(const CommonCmdPacket* packet)
{
  PL_ACK ack = PL_push_back(&PH_txpic_cmd_list, packet);

  if (ack != PL_SUCCESS) return PH_ACK_PL_LIST_FULL;
  return PH_ACK_SUCCESS;
}


static PH_ACK PH_add_cobc_cmd_(const CommonCmdPacket* packet)
{
  PL_ACK ack = PL_push_back(&PH_cobc_cmd_list, packet);

  if (ack != PL_SUCCESS) return PH_ACK_PL_LIST_FULL;
  return PH_ACK_SUCCESS;
}


static PH_ACK PH_add_sobc_cmd_(const CommonCmdPacket* packet)
{
  PL_ACK ack = PL_push_back(&PH_sobc_cmd_list, packet);

  if (ack != PL_SUCCESS) return PH_ACK_PL_LIST_FULL;
  return PH_ACK_SUCCESS;
}


static PH_ACK PH_add_lora_cmd_(const CommonCmdPacket* packet)
{
  PL_ACK ack = PL_push_back(&PH_lora_cmd_list, packet);

  if (ack != PL_SUCCESS) return PH_ACK_PL_LIST_FULL;
  return PH_ACK_SUCCESS;
}


static PH_ACK PH_add_intersat_cmd_(const CommonCmdPacket* packet)
{
  PL_ACK ack = PL_push_back(&PH_intersat_cmd_list, packet);

  if (ack != PL_SUCCESS) return PH_ACK_PL_LIST_FULL;
  return PH_ACK_SUCCESS;
}


//#define PH_AOBC_CMD_LIST_MAX   (16)          //!< AOBC CMD QUEUEサイズ
//#define PH_TOBC_CMD_LIST_MAX   (16)          //!< TOBC CMD QUEUEサイズ
//
//static PH_ACK PH_add_aobc_cmd_(const CommonCmdPacket* packet);
//static PH_ACK PH_add_tobc_cmd_(const CommonCmdPacket* packet);
//
//PacketList PH_aobc_cmd_list;
//PacketList PH_tobc_cmd_list;
//
//static PL_Node PH_aobc_cmd_stock_[PH_AOBC_CMD_LIST_MAX];
//static PL_Node PH_tobc_cmd_stock_[PH_TOBC_CMD_LIST_MAX];
//static CommonCmdPacket PH_aobc_ccp_stock_[PH_AOBC_CMD_LIST_MAX];
//static CommonCmdPacket PH_tobc_ccp_stock_[PH_TOBC_CMD_LIST_MAX];
//
//
//void PH_user_init(void)
//{
//  PL_initialize_with_ccp(PH_aobc_cmd_stock_, PH_aobc_ccp_stock_, PH_AOBC_CMD_LIST_MAX, &PH_aobc_cmd_list);
//  PL_initialize_with_ccp(PH_tobc_cmd_stock_, PH_tobc_ccp_stock_, PH_TOBC_CMD_LIST_MAX, &PH_tobc_cmd_list);
//}
//
//
//PH_ACK PH_user_analyze_cmd(const CommonCmdPacket* packet)
//{
//  CCP_DEST_TYPE dest_type;// = CCP_get_dest_type(packet);
//  APID apid = CCP_get_apid(packet);
//
//  if (dest_type == CCP_DEST_TYPE_TO_APID)
//  {
//    switch (apid)
//    {
//    case APID_AOBC_CMD:
//      return (PH_add_aobc_cmd_(packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;
//    case APID_TOBC_CMD:
//      return (PH_add_tobc_cmd_(packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;
//    default:
//      // APID_MOBC_CMD
//      // 不正な APID
//      // はここに
//      return PH_ACK_UNKNOWN;
//    }
//  }
//  else
//  {
//    switch (dest_type)
//    {
//    case CCP_DEST_TYPE_TO_AOBC:    // CCP_DEST_TYPE_TO_APID の追加に伴い deprecated
//      return (PH_add_aobc_cmd_(packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;
//    case CCP_DEST_TYPE_TO_TOBC:    // CCP_DEST_TYPE_TO_APID の追加に伴い deprecated
//      return (PH_add_tobc_cmd_(packet) == PH_ACK_SUCCESS) ? PH_ACK_FORWARDED : PH_ACK_PL_LIST_FULL;
//    default:
//      // CCP_DEST_TYPE_TO_ME
//      // CCP_DEST_TYPE_TO_MOBC などの自分宛．なお， CCP_DEST_TYPE_TO_APID の追加に伴い deprecated
//      // 宛先不明
//      // はここに
//      return PH_ACK_UNKNOWN;
//    }
//  }
//}
//
//
//CCP_CmdRet PH_user_cmd_router(const CommonCmdPacket* packet)
//{
//  APID apid = CCP_get_apid(packet);
//  switch (apid)
//  {
//  case APID_AOBC_CMD:
//    // AOBCに配送
//    return DI_AOBC_dispatch_command(packet);
//  case APID_TOBC_CMD:
//    // TOBCに配送
//    // return DI_TOBC_dispatch_command(packet);
//  default:
//    // 該当する配送先が定義されていない場合
//    return CCP_make_cmd_ret(CCP_EXEC_ROUTING_FAILED, apid);
//  }
//}
//
//
//TF_TLM_FUNC_ACK PH_user_telemetry_router(APID apid,
//                                         TLM_CODE tlm_id,
//                                         uint8_t* packet,
//                                         uint16_t* len,
//                                         uint16_t max_len)
//{
//  switch (apid)
//  {
//    case APID_AOBC_TLM:
//      return AOBC_pick_up_tlm_buffer(aobc_driver, (AOBC_TLM_CODE)tlm_id, packet, len, max_len);
//    default:
//      return TF_TLM_FUNC_ACK_NOT_DEFINED;
//  }
//}
//
//
//static PH_ACK PH_add_aobc_cmd_(const CommonCmdPacket* packet)
//{
//  PL_ACK ack = PL_push_back(&PH_aobc_cmd_list, packet);
//
//  if (ack != PL_SUCCESS) return PH_ACK_PL_LIST_FULL;
//  return PH_ACK_SUCCESS;
//}
//
//
//static PH_ACK PH_add_tobc_cmd_(const CommonCmdPacket* packet)
//{
//  (void)packet;
//#if 0
//  PL_ACK ack = PL_push_back(&PH_tobc_cmd_list, packet);
//
//  if (ack != PL_SUCCESS) return PH_ACK_PL_LIST_FULL;
//#endif
//  return PH_ACK_SUCCESS;
//}

#pragma section
