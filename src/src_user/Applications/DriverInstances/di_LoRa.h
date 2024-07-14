/*
 * di_LoRa.h
 *
 *  Created on: 2021/11/16
 *      Author: sakaguchi
 */

#ifndef C2A_APPLICATIONS_DRIVERINSTANCES_DI_LORA_H_
#define C2A_APPLICATIONS_DRIVERINSTANCES_DI_LORA_H_

#include "../../Drivers/CDH/LoRa/LoRa.h"
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_core/TlmCmd/command_dispatcher.h>
#include <src_user/Drivers/Com/Circular_buffer.h>
#include "src_user/Library/Delay.h"

#define LORA_MAX_RECEIVE_PACKET_NUM 4
extern Queue lora_queue;

#ifdef __cplusplus
 extern "C" {
#endif

extern LoRa_STRUCT* const lora_driver;
extern LoRa_Register* const lora_register;
extern const CommandDispatcher* const lora_cdis;

AppInfo COM_LoRa_receive_update(void);
AppInfo lora_update(void);
AppInfo lora_cmd_dispathcer(void);

void COM_LoRa_init(void);
CCP_CmdRet lora_dispatch_command(const CommonCmdPacket* packet);

uint8_t lora_length_checker(uint8_t* buffer, size_t size);
_ACK lora_TCPacket_Fixedheader_Checker(void);

_ACK lora_check_life_command(void);

#ifdef __cplusplus
}
#endif

#endif /* C2A_APPLICATIONS_DRIVERINSTANCES_COMMUNICATION_COM_CMD_LORA_H_ */
