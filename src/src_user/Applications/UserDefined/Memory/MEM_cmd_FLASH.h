/*
 * Flash.h
 *
 *  Created on: 2022/02/26
 *      Author: SAT37
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_MEMORY_MEM_CMD_FLASH_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_MEMORY_MEM_CMD_FLASH_H_

#include <stdint.h> //for uint8_t

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


// uint8_tで！！
typedef enum
{
  BD_STATE_WAITING_EXEC,
  BD_STATE_PROCESSING,
  BD_STATE_COMPLETED,
  BD_STATE_ABORTED_BY_CMD,
  BD_STATE_ABORTED_BY_ERR,
  BD_STATE_ERR_BUSY,
  BD_STATE_ERR_ILLEGAL_PARAMS,
  BD_STATE_ERR_MISMATCH_JOB,
  BD_STATE_ERR_CANNOT_EXEC,
  BD_STATE_ERR_UNKNOWN,
} BD_STATE;
/*
for tlm
0=WAITING_EXEC,1=PROCESSING,2=COMPLETED,3=ABORTED_BY_CMD,4=ABORTED_BY_ERR,5=ERR_BUSY,6=ERR_ILL_PARAMS,7=ERR_MISMATCH,8=ERR_CANNOT_EXEC,9=ERR_UNKNOWN,*=N/A
*/

// uint8_tで！！
typedef enum
{
  BD_PROCESS_ERR_AT_SD_COPY,
  BD_PROCESS_ERR_AT_FLASH_COPY,
  BD_PROCESS_ERR_AT_SD_TO_FLASH,
  BD_PROCESS_ERR_AT_FLASH_TO_SD,
  BD_PROCESS_ERR_AT_ERASE,
  BD_PROCESS_ERR_AT_SD_CRC,
  BD_PROCESS_ERR_AT_SD_TO_RAM
//  BD_PROCESS_ERR_DATA_MISMATCH,
} BD_PROCESS_ERR;
/*
for tlm
0=ERR_AT_WRITE,1=ERR_AT_READ,2=ERR_AT_ERASE,3=ERR_DATA_MISMATCH*=N/A
*/

// uint8_tで！！
typedef enum
{
	BD_MODE_SD_TO_SD,
	BD_MODE_FLASH_TO_FLASH,
	BD_MODE_SD_TO_FLASH,
	BD_MODE_FLASH_TO_SD,
	BD_MODE_ELASE_FLASH,
	BD_MODE_FLASH_CRC,
	BD_MODE_SD_CRC,
	BD_MODE_SD_TO_RAM,
	BD_MODE_RAM_CRC
} BD_MODE;
/*
for tlm
0=SRAM_TO_FLASH,1=FLASH_TO_SRAM,2=FLASH_TO_FLASH,3=ELASE_FLASH,4=VALID_DATA,*=N/A
*/

typedef struct
{
  BD_PROCESS_ERR  err_state;
  uint32_t        err_adr;
  uint32_t        err_block;
} BigDataErrInfo;

typedef struct
{
  uint8_t			job_id;
  BD_STATE			state;
  BD_MODE         	mode;
  uint32_t         	process_counter;
  BigDataErrInfo	err_info;
  uint32_t			sd_src_block_adr;
  uint32_t			sd_dst_block_adr;
  uint32_t			src_adr;
  uint32_t			dst_adr;
  uint32_t 			len;
  uint32_t 			rest_len;
  uint32_t 			end_process_counter;
  uint32_t       	erase_block;
  uint32_t       	erase_block_num;
  uint16_t          sd_crc;
  uint16_t          flash_crc;
  uint16_t          ram_crc;
  uint16_t          save_crc;

} BigDataJobInfo;

typedef struct
{
  BigDataJobInfo*      p_job_info;
  uint8_t              id_counter;
} BigDataInfo;

extern const BigDataInfo* bdi;
extern uint32_t boot_mis_address;
extern uint8_t boot_error_num;

AppInfo BD_create_app(void);
AppInfo CDH1_flash_param_update(void);

CCP_CmdRet Cmd_BD_SET_COPY_SD2SD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_SET_COPY_FLASH2FLASH(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_SET_COPY_SD2FLASH(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_SET_COPY_FLASH2SD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_SET_ERASE_FLASH(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_SET_FLASH_CRC(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_SET_SD_CRC(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_SET_COPY_SD2RAM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_SET_RAM_CRC(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_EXEC_JOB(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_ABORT_JOB(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_EXEC_JOB_ANYWAY(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BD_ABORT_JOB_ANYWAY(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_MEM_flash_unset_eraseflag(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_FlashBoot_patrol(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MEM_flash_set_patrol_addr(const CommonCmdPacket* packet);

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_MEMORY_MEM_CMD_FLASH_H_ */
