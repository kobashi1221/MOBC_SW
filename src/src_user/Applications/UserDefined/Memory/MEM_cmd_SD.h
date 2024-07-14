/**
  *****************************************************************************************
  * @file    : CDH1_debug_SD.h
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/10/17
  * @brief   : SD Card Application for CDH1 Board Debug
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/10/17  1.0   N.KISHI         First revision
  * 2020/11/25  2.0   K.KIMURA        Add SD card function
  *
  *****************************************************************************************
  */

#ifndef CDH1_DEBUG_SD_H_
#define CDH1_DEBUG_SD_H_

#include <stdint.h> //for uint8_t
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_user/Settings/TlmCmd/telemetry_frame_params.h>
#include <src_user/Drivers/Cdh/SD/SD.h>
#include <src_user/Drivers/Cdh/SD/stm32_adafruit_sd.h>
#include <src_user/TlmCmd/telemetry_definitions.h>

#define NOT_GENERATE (0)
#define EVERY (1)
#define LOW_RATE_1 (2)
#define LOW_RATE_2 (3)

#define SD_PARAM_OFFSET  0x00000000
#define FLIGHT_PROGRAM_OFFSET  0x00000B72
#define Redundancy_OFFSET  0x00262800

#define SD_TLM_PAGE_SIZE (20)
#define SD_TLM_PAGE_MAX  (5)

extern const uint8_t* p_tlm_flag;

typedef enum{
	TLM_WAIT,
	TLM_PROCEED,
	TLM_PAUSE,
	TLM_ID_ERROR,
	TLM_BLOCK_ERROR,
	TLM_DUMP_NUM_ERROR,
	TLM_START_COUNT_ERROR,
	TLM_INVALID_PACKET,
}TLM_DL_ack;

typedef enum{
	PROGRAM_DATA_MATCH,
	PROGRAM_DATA_MISMATCH,
}PROGRAM_DATA_AL_ACK;

typedef struct{
	uint16_t RP_TLM_APID;//どの通信機
	uint8_t RP_TLM_ROUTE;//どのルート
	TLM_CODE RP_TLM_CODE;//どのTLM
	uint32_t RP_TLM_BLOCK;//どのDATA
	uint32_t RP_TLM_packet_num;//何パケット
	uint8_t pos_in_block;//SD_BLOCKのどの位置
	uint8_t width;//取り出してくる間隔
	uint8_t dump_num;//1パケットあたりにダウンリンクする数
	TLM_DL_ack tlm_dl_ack;
	uint32_t process_count;
}TLM_DL_PARAM;

typedef struct{
	uint16_t RP_TLM_APID;
	uint8_t RP_TLM_ROUTE;
	TLM_CODE RP_TLM_CODE;
	uint32_t RP_TLM_BLOCK;
	uint32_t RP_TLM_packet_num;
	uint8_t pos_in_block;
	uint8_t width;
	uint16_t packet_len;
	uint8_t max_in_blick;
	TLM_DL_ack tlm_dl_ack;
	uint8_t los_count[243];
}TLM_DL_LOS_PARAM;

typedef struct{
	SD_Info_Irregular sdi_irregular[Tlm_CODE_HOGEHOGE74];
	SD_Info_Regular sdi_regular[TF_MAX_TLMS - Tlm_CODE_HOGEHOGE74];
	uint32_t SDPatrolBlock;
	uint32_t irregular_condition_block;
	uint32_t regular_condition_block;
	TLM_DL_PARAM tlm_dl_param;
}SD_STRUCT;

extern const SD_STRUCT* p_sd_struct;
extern const uint8_t* SD_tlm_page_no;
extern uint32_t Storage_Area[TF_MAX_TLMS*2];
extern uint32_t program_mis_address;

AppInfo CDH1_sd_update(void);
AppInfo CDH1_sd_param_update(void);
AppInfo CDH1_sd_memory_patrol(void);
AppInfo CDH1_sd_program_data(void);

CCP_CmdRet Cmd_CDH1_SD_BLOCK_READ(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH1_SD_CONDITION_SAVE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH1_SD_reset_all(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH1_SD_CHANGE_RPTLM_param(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH1_SD_Generate_RPTLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TLM_CHANGE_FREQ(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH1_SD_RPTLM_Pause(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_CDH1_SD_RPTLM_Resume(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_MEM_SD_writeParamData(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_SD_ReadParamData(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_MEM_SD_writeProgramData(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_SD_ReadProgramData(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_SD_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_MEM_SD_set_patrol_block(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_SD_command_read(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_SD_write(const CommonCmdPacket* packet);

#ifdef ONE_U
CCP_CmdRet Cmd_SD_ReadForDebug(const CommonCmdPacket* packet);
#endif /* ONE_U */

#endif /* CDH1_DEBUG_SD_H_ */
