#pragma section REPRO
#include "m_pdu.h"

#include "string.h" // for memcpy

static void M_PDU_clear_spare_(M_PDU* m_pdu);

void M_PDU_generate_byte_stream(const M_PDU* m_pdu,
                                uint8_t byte_stream[M_PDU_LEN])
{
  memcpy(byte_stream, m_pdu->header, M_PDU_HEADER_SIZE);
  byte_stream += M_PDU_HEADER_SIZE;

  memcpy(byte_stream, m_pdu->data, M_PDU_DATA_SIZE);
}

void M_PDU_setup_idle_m_pdu(M_PDU* m_pdu)
{
  static uint8_t temp_fill_vcdu_data[432] = {
    0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
    0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0xcc,
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00, 0xcc,
    0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00,
    0x00, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0xcc, 0xcc,
    0xcc, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0xcc,
    0x00, 0xcc, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0x00,
    0x00, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0x00,
    0x00, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0xcc,
    0xcc, 0x00, 0xcc, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0x00,
    0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0xcc,
    0xcc, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0xcc,
    0x00, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0x00,
    0x00, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0x00,
    0x00, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0xcc,
    0xcc, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0xcc, 0x00,
    0x00, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0x00,
    0x00, 0x00, 0xcc, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc,
    0x00, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0x00, 0xcc, 0xcc, 0xcc,
    0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  M_PDU_clear_spare_(m_pdu);
  M_PDU_set_1st_hdr_ptr(m_pdu, M_PDU_PTR_IDLE);
  memcpy(&m_pdu->data[0], &temp_fill_vcdu_data[0], 432);
}

static void M_PDU_clear_spare_(M_PDU* m_pdu)
{
  unsigned int pos = 0;
  uint8_t mask = 0xf8; // 11111000b

  m_pdu->header[pos] &= (uint8_t)(~mask);
}

uint16_t M_PDU_get_1st_hdr_ptr(const M_PDU* m_pdu)
{
  unsigned int pos = 0;
  uint8_t mask = 0x3f; // 00111111b

  uint16_t ptr = (uint8_t)(m_pdu->header[pos] & mask);
  ptr <<= 8;
  ptr += m_pdu->header[pos + 1];

  return ptr;
}

void M_PDU_set_1st_hdr_ptr(M_PDU* m_pdu,
                           uint16_t ptr)
{
  unsigned int pos = 0;
  uint8_t mask = 0x3f; // 00111111b

  m_pdu->header[pos] &= (uint8_t)(~mask);
  m_pdu->header[pos] |= (uint8_t)((ptr >> 8) & mask);
  m_pdu->header[pos + 1] = (uint8_t)(ptr & 0xff);
}

void M_PDU_set_data(M_PDU* m_pdu,
                    const uint8_t* data,
                    size_t offset,
                    size_t len)
{
  // 指定されたデータ長が上限値を超過する場合は上限値に制限
  if (offset + len > M_PDU_DATA_SIZE)
  {
    len = M_PDU_DATA_SIZE - offset;
  }

  // offset以降にデータを書き込む
  memcpy(m_pdu->data + offset, data, len);
}
#pragma section
