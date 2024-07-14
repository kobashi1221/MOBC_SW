#pragma section REPRO
/**
 * @file
 * @brief  CTP 関連基本関数の定義
 * @note   CCP:  CommonCmdPacket
 */
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_core/TlmCmd/Ccsds/cmd_space_packet.h>
#include <string.h>
#include <src_user/TlmCmd/Ccsds/TCPacket.h>


int CCP_is_valid_packet(const CommonCmdPacket* packet)
{
  if (packet == NULL) return 0;
  if ( CCP_get_packet_len(packet) > CCP_MAX_LEN ) return 0;

  return 1;
}

cycle_t CCP_get_ti(const CommonCmdPacket* packet)
{
  return (cycle_t)TCP_CMD_get_ti(packet);
}

void CCP_set_ti(CommonCmdPacket* packet, cycle_t ti)
{
  TCP_CMD_set_ti(packet, (uint32_t)ti);
}

APID CCP_get_apid(const CommonCmdPacket* packet)
{
  return TCP_get_apid_(packet);
}

void CCP_set_apid(CommonCmdPacket* packet, APID apid)
{
  TCP_set_apid(packet, apid);
}

CCP_EXEC_TYPE CCP_get_exec_type(const CommonCmdPacket* packet)
{
  return TCP_CMD_get_exec_type(packet);
}

void CCP_set_exec_type(CommonCmdPacket* packet, CCP_EXEC_TYPE type)
{
  // CCP_EXEC_TYPEのほうがとりうる数値の幅が広いので，アサーション
  if ((int)type >= (int)TCP_CMD_EXEC_TYPE_UNKNOWN)
  {
    TCP_CMD_set_exec_type(packet, TCP_CMD_EXEC_TYPE_UNKNOWN);
  }
  else
  {
    TCP_CMD_set_exec_type(packet, (TCP_CMD_EXEC_TYPE)type);
  }
}

//CCP_DEST_TYPE CCP_get_dest_type(const CommonCmdPacket* packet)
//{
//  return CSP_get_dest_type(packet);
//}
//
//void CCP_set_dest_type(CommonCmdPacket* packet, CCP_DEST_TYPE type)
//{
//  CSP_set_dest_type(packet, type);
//}

CMD_CODE CCP_get_id(const CommonCmdPacket* packet)
{
  return TCP_CMD_get_channel_id(packet);
}

void CCP_set_id(CommonCmdPacket* packet, CMD_CODE id)
{
  TCP_CMD_set_channel_id(packet, (uint16_t)id);
}

uint16_t CCP_get_packet_len(const CommonCmdPacket* packet)
{
  return (uint16_t)(TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN);
}

uint16_t CCP_get_param_len(const CommonCmdPacket* packet)
{
  return (uint16_t)(TCP_get_packet_len(packet) - TCP_CMD_2ND_HDR_LEN - TCP_CMD_USER_HDR_LEN);
}

uint16_t CCP_get_max_param_len(void)
{
  return (uint16_t)(TCP_MAX_LEN - TCP_PRM_HDR_LEN - TCP_CMD_2ND_HDR_LEN - TCP_CMD_USER_HDR_LEN);
}

const uint8_t* CCP_get_param_head(const CommonCmdPacket* packet)
{
  return TCP_CMD_get_param_head(packet);
}

void CCP_set_param(CommonCmdPacket* packet, const uint8_t* param, uint16_t len)
{
  TCP_CMD_set_param(packet, param, len);
}

void CCP_set_common_hdr(CommonCmdPacket* packet)
{
  TCP_CMD_set_common_hdr(packet);
}

void CCP_copy_packet(CommonCmdPacket* dest, const CommonCmdPacket* src)
{
  TCP_copy_packet(dest, src);
}

#pragma section
