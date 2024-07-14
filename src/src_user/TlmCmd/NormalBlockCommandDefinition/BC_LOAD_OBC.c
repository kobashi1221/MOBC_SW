#pragma section REPRO
#include "nbc_header.h"

#include "src_user/Applications/app_registry.h"
#include "src_user/TlmCmd/block_command_definitions.h"

#include <src_core/TlmCmd/block_command_loader.h>

void BC_load_obc_comm(void){
	BCL_tool_register_app    (1, AR_COM_COMMABD_RESPONSE);

	BCL_tool_register_app    (3, AR_COM_COBC_DISPATCH_COMMAND);

	BCL_tool_register_app    (5, AR_COM_TXPIC_DISPATCH_COMMAND);

	BCL_tool_register_app    (7, AR_COM_RXPIC_DISPATCH_COMMAND);

	BCL_tool_register_app    (9, AR_COM_INTERSAT_DISPATCH_COMMAND);

	BCL_tool_register_app    (10, AR_COM_SOBC_DISPATCH_COMMAND);

	BCL_tool_register_app    (11, AR_COM_LORA_DISPATCH_COMMAND);

    BCL_tool_register_cmd    (12, Cmd_CODE_PWR_LORA_AUTO_ON);

	BCL_tool_register_app    (13, AR_COM_LORA_RECEIVE_APP);

	BCL_tool_register_app    (15, AR_COM_DI_LORA_APP);

	BCL_tool_register_app    (17, AR_COM_TXPIC_CLAER_COUNT);

	BCL_tool_register_app    (19, AR_COM_RXPIC_CLAER_COUNT);

	BCL_tool_register_app    (21, AR_COM_INTERSAT_TLM_UPDATE);

//	BCL_tool_register_app    (23, AR_COM_SOBC_REGULAR_TLM);//kobayashi added 2023/11/28

#ifdef TWO_U
//	BCL_tool_register_cmd    (23, Cmd_CODE_COM_COBC_SEND_AOCS_PARAM);//COBC dispatchと離さないと通らないことあり

	BCL_tool_register_app    (24, AR_COM_COBC_REGULAR_TLM);
#endif

}

#pragma section
