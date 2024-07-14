#pragma section REPRO
/**
 * @file
 * @brief  CTP 関連基本関数の定義
 * @note   CTP:  CommonTlmPacket
 */
#include <src_core/TlmCmd/common_tlm_packet.h>
#include <src_core/TlmCmd/Ccsds/tlm_space_packet.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <string.h>
#include <src_user/TlmCmd/Ccsds/TCPacket.h>


int CTP_is_valid_packet(const CommonTlmPacket* packet)
{
  if (packet == NULL) return 0;
  if ( CTP_get_packet_len(packet) > CTP_MAX_LEN ) return 0;

  return 1;
}

APID CTP_get_apid(const CommonTlmPacket* packet)
{
  return TCP_get_apid_(packet);
}

void CTP_set_apid(CommonTlmPacket* packet, APID apid)
{
  TCP_set_apid(packet, apid);
}

//double CTP_get_global_time(const CommonTlmPacket* packet)
//{
//  return TSP_get_global_time(packet);
//}

//void CTP_set_global_time(CommonTlmPacket* packet)
//{
//  // 何を設定するかはユーザー定義
//  // TMGR_get_curret_unixtime() で現在の unixtime を入れたり, gps 時刻 を入れたり, など
//  TSP_set_global_time(packet, 0.0);
//}

//uint32_t CTP_get_on_board_subnet_time(const CommonTlmPacket* packet)
//{
//  return TSP_get_on_board_subnet_time(packet);
//}

//void CTP_set_on_board_subnet_time(CommonTlmPacket* packet)
//{
//  // 何を設定するかはユーザー定義
//  // MOBC では主に TI を，2nd OBC では主に 0xffffffff を
//  TSP_set_on_board_subnet_time(packet, (uint32_t)TMGR_get_master_total_cycle());
//}

ctp_dest_flags_t CTP_get_dest_flags(const CommonTlmPacket* packet)
{
  uint8_t category = TCP_TLM_get_category(packet);
  uint8_t mask = 0xe0; // 11100000b

  category &= mask;
  category >>= 5;

  if (category == 0)
  {
    return CTP_DEST_FLAG_RP_TLM;
  }
  else
  {
    return (CTP_DEST_FLAG)category;
  }
}

void CTP_set_dest_flags(CommonTlmPacket* packet, ctp_dest_flags_t flags)
{
  uint8_t category = TCP_TLM_get_category(packet);
  uint8_t mask = 0xe0; // 11100000b

  category &= (uint8_t)(~mask);
  category |= (uint8_t)(flags << 5);

  TCP_TLM_set_category(packet, category);
}

TLM_CODE CTP_get_id(const CommonTlmPacket* packet)
{
  return TCP_TLM_get_packet_id(packet);
}

void CTP_set_id(CommonTlmPacket* packet, TLM_CODE id)
{
  TCP_TLM_set_packet_id(packet, id);
}

uint16_t CTP_get_packet_len(const CommonTlmPacket* packet)
{
  return (uint16_t)(TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN);
}

uint8_t* CTP_get_user_data_head(CommonTlmPacket* packet)
{
  return TCP_TLM_get_user_data_head(packet);
}

void CTP_copy_packet(CommonTlmPacket* dest, const CommonTlmPacket* src)
{
  TCP_copy_packet(dest, src);
}

#pragma section
