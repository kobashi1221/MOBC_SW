#pragma once
#ifndef DBG_PACKET_HANDLER_H_
#define DBG_PACKET_HANDLER_H_

#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include "../../TlmCmd/user_packet_handler.h"
#include "../../Drivers/Dbg/DBG.h"


typedef struct{
 DBG_STRUCT dbg1;
 DBG_ACK tcp_ack;
 cycle_t tcp_last_recv_time;
 PH_ACK cmd_ack;
}DPH;

extern const DPH* dph;

#ifdef __cplusplus
 extern "C" {
#endif

 	 AppInfo DBG_cmd_packet_handler(void);
#ifdef __cplusplus
}
#endif

#endif // DBG_PACKET_HANDLER_H_
