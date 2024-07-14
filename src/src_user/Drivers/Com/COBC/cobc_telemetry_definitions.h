/*
 * cobc_telemetry_definitions.h
 *
 *  Created on: 2021/10/16
 *      Author: daiki
 *
 *  Updated on: 2021/12/22
 *      Author: Sakaguchi
 */

#ifndef COBC_TELEMETRY_DEFINITIONS_H_
#define COBC_TELEMETRY_DEFINITIONS_H_

typedef enum
{
  COBC_Tlm_CODE_COBC_TO_MOBC=0x00,//Full BitVer
  COBC_MCAM_TLM = 0xC0,//Full BitVer
  COBC_SCAM_TLM = 0xC1,//Full BitVer
  COBC_CAMINFO_BY_UART = 0xCA,
  COBC_CAMDATA_BY_UART = 0xCB,
  COBC_TLM_CODE_MAX
} COBC_TLM_CODE;

#endif /* SRC_USER_DRIVERS_COM_COBC_COBC_TELEMETRY_DEFINITIONS_H_ */
