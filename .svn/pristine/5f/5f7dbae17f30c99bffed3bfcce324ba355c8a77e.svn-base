/*
 * BC_load_obc_ph.h
 *
 *  Created on: 2021/05/29
 *      Author: kahak
 *  Edited: 2021/09/07
 *  	park
 */

#include "nbc_header.h"
#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>

void BC_load_tlm_cmd_ph(){
	//DBG用
	BCL_tool_register_app    ( 1, AR_DBG_PACKET_HANDLER_APP);

	//LARGE_PACKET_ANALYZE
	BCL_tool_register_app    ( 2, AR_COM_LARGE_PACKET_ANALYZE);

	//RXPIC
	BCL_tool_register_app    ( 4, AR_COM_DI_RXPIC_APP);
	//TXPIC
	BCL_tool_register_app    ( 5, AR_COM_DI_TXPIC_APP);
	//INTERSAT
	BCL_tool_register_app    ( 6, AR_COM_DI_INTERSAT_APP);
	//SOBC
	BCL_tool_register_app    ( 7, AR_COM_DI_SOBC_APP);

#ifdef TWO_U
	//COBC
	BCL_tool_register_app    ( 8, AR_COM_DI_COBC_APP);
#endif

}
