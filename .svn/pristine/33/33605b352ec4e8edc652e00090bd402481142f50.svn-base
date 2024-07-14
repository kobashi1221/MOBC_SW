/*
 * large_packet_driver.h
 *
 *  Created on: 2021/12/20
 *      Author: sakaguchi
 */

#ifndef C2A_DREIVERS_LARGE_PACKET_DRIVER_H_
#define C2A_DREIVERS_LARGE_PACKET_DRIVER_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/TlmCmd/CCSDS/TCPacket.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_user/Applications/DriverInstances/tlm_packet_handler.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define ID_SIZE (2)

typedef enum{
	LARGE_PACKET_FLAG_OFF,
	LARGE_PACKET_FLAG_ON
}LARGE_PACEKT_FLAG;

typedef struct
{
	TCP_VER ver;
	TCP_TYPE type;
	TCP_2ND_HDR_FLAG hdr_flag;
	TCP_PROJECT_ID project_id;
	TCP_ROUTE route;
	TCP_SAT_ID sat_id;
	TCP_EXE_ID exe_id;
	TCP_SEQ_FLAG seq_flag;
}LARGE_PACKET_COMMON;

typedef struct
{
	uint8_t category;
	uint8_t packet_identifer;
	TCP_SAT_ID tlm_issuer_sat_id;
	TCP_EXE_ID tlm_issuer_id;
}LARGE_PACKET_TLM;

typedef struct
{
	uint8_t cmd_type;
	TCP_CMD_EXEC_TYPE cmd_exe_type;
	cycle_t time;
}LARGE_PACKET_CMD;

typedef struct
{
	uint8_t function_flag;
	uint8_t large_packet_flag;
	uint8_t re_send_flag;
	uint8_t resend_last_one;
	uint8_t analyze_cycle;
	uint8_t send_cycle;
	uint16_t per_packet_size;
	int packet_counter;
	int pre_packet_counter;
	int header_length;
	int send_counter;
	uint16_t seq_count;
	size_t packet_size;
}LARGE_PACKET_ANALYZE;

typedef struct
{
	uint8_t* large_packet;
	TCP tcp;
	LARGE_PACKET_COMMON common;
	LARGE_PACKET_TLM tlm;
	LARGE_PACKET_CMD cmd;
	LARGE_PACKET_ANALYZE analyze;
}LARGE_PACKET_STRUCT;

extern LARGE_PACKET_STRUCT* const large_driver;

AppInfo COM_large_packet_Analyzer(void);

//実際にpacketを作る関数群
void large_packet_init(void);
void large_packet_exe(void);
void default_tlm_analyze(void);
void picture_tlm_analyze(void);
void large_packet_TLM_analyze(uint8_t* data, size_t size, TLM_CODE tlm);
void set_packet_header(uint8_t* packet, size_t size);

//init関数群
void analyzer_init(void);
void header_init(void);
void header_common_init(void);
void header_tlm_init(void);
void header_cmd_init(void);

//flagセット関数
void large_packet_set(uint8_t* flag);
void large_packet_reset(uint8_t* flag);


#ifdef __cplusplus
 }
#endif

#endif /* C2A_DREIVERS_LARGE_PACKET_TRANSMIT_H_ */
