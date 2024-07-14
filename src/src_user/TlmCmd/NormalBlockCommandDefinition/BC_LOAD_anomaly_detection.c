/*
 * BC_load_anomaly_detection.h
 *
 *  Created on: 2021/12/07
 *      Author: Noro
 */

#include "nbc_header.h"

#include <src_core/TlmCmd/block_command_table.h>
#include "../block_command_definitions.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>

void BC_load_anomaly_detection_handler(){
	BCL_tool_register_app    ( 1, AR_ANOMALY_UART_DETECT_APP);

	BCL_tool_register_app    ( 2, AR_ANOMALY_I2C_DETECT_APP);

	BCL_tool_register_app    ( 3, AR_ANOMALY_SPI_DETECT_APP);

	BCL_tool_register_app    ( 4, AR_ANOMALY_TXPIC_SENSING_DETECT_APP);

	BCL_tool_register_app    ( 5, AR_EVENT_UTILITY);

}
