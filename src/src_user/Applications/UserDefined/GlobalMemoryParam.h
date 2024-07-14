/*
 * GlobalMemoryParam.h
 *
 *  Created on: 2021/09/14
 *      Author: kimura
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_GLOBALMEMORYPARAM_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_GLOBALMEMORYPARAM_H_

#undef FLASH_SIZE
#undef MRAM_SIZE

#include <src_user/Drivers/Cdh/MRAM/MRAM.h>

#define RAM_BEGIN_ADR     (0x20001000)
#define RAM_END_ADR       (0x20050000)//(exclusive)
#define FLASH_BEGIN_ADR   (0x08000000)
#define FLASH_END_ADR     (0x08200000)//(exclusive)
#define SRAM_BEGIN_ADR    (0x60000000)
#define SRAM_END_ADR      (0x60080000)//(exclusive)
#define CCMRAM_BEGIN_ADR  (0x10000000)
#define CCMARAM_END_ADR   (0x10010000)//(exclusive)

#define RAM_SIZE          (0x00050000)
#define FLASH_SIZE        (0x00200000)
#define SRAM_SIZE         (0x00080000)
#define CCMRAM_SIZE       (0x00010000)
#define MRAM_SIZE         (0x00080000)
#define EEPRAM_SIZE       (0x00020000)
#define SD_BLOCK_ADR_SIZE (0x01CF7FFF)
#define SPIPROM_SIZE      (0x01000000)

#define EEPROM_REDUNDANCY_SIZE (0xAAAA)
#define MRAM_REDUNDANCY_SIZE   (0xAAAA)
#define SD_REDUNDANCY_SIZE (0x262800)
#define SPIPROM_REDUNDANCY_SIZE (0x108000)

typedef enum{
	MEMORY_PARAM_NO_MISMATCH,
	MEMORY_PARAM_TWO_MISMATCH,
	MEMORY_PARAM_THREE_MISMATCH,
	MEMORY_PARAM_ILLEGAL_ADDRESS,
}PARAM_MEM_AL_ACK;


typedef struct{
	uint8_t Program;
	uint8_t Memory;
	uint8_t Repro;
}PIC_PARAM_STRUCT;

typedef struct{
	uint32_t address;
	uint32_t size;
}MISMATCH_ADDRESS;

typedef struct{
	MISMATCH_ADDRESS mis_eeprom;
	MISMATCH_ADDRESS mis_mram;
	MISMATCH_ADDRESS mis_sd;
	MISMATCH_ADDRESS mis_spiprom;
}MISMATCH_Info;

extern const MISMATCH_Info* mem_mis_info;
extern const PIC_PARAM_STRUCT* pic_data;


#endif /* SRC_USER_APPLICATIONS_USERDEFINED_GLOBALMEMORYPARAM_H_ */
