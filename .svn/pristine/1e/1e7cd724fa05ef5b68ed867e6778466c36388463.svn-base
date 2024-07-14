/*
 * downlink_printf.h
 *
 *  Created on: 2022/02/22
 *      Author: daiki
 */

#ifndef SRC_USER_DRIVERS_DOWNLINK_PRINTF_H_
#define SRC_USER_DRIVERS_DOWNLINK_PRINTF_H_

#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_user/TlmCmd/telemetry_definitions.h>

PH_ACK DL_printf(const TCP* packet, uint16_t apid, uint8_t* data, uint16_t len, TLM_CODE tlm_code);
PH_ACK SAVE_printf_for_log(const TCP* packet, uint16_t apid, uint8_t* data, uint16_t len);

#endif /* SRC_USER_DRIVERS_DOWNLINK_PRINTF_H_ */
