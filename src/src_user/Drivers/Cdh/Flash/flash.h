/*
 * flash.h
 *
 *  Created on: 2021/10/12
 *      Author: SAT37
 */

#ifndef FLASH_H_
#define FLASH_H_
#include <stdio.h>

typedef enum{
	SECTOR_0,
	SECTOR_1,
	SECTOR_2,
	SECTOR_3,
	SECTOR_4,
	SECTOR_5,
	SECTOR_6,
	SECTOR_7,
	SECTOR_8,
	SECTOR_9,
	SECTOR_10,
	SECTOR_11,
	SECTOR_12,
	SECTOR_13,
	SECTOR_14,
	SECTOR_15,
	SECTOR_16,
	SECTOR_17,
	SECTOR_18,
	SECTOR_19,
	SECTOR_20,
	SECTOR_21,
	SECTOR_22,
	SECTOR_23,
	MAX_SECTOR_SIZE
}Sector_NUMBER;

typedef struct{
	uint32_t Origin;
}SECTOR_INFO_STRUCT;

typedef enum{
	BOOT_FLASH_NO_MISMATCH,
	BOOT_FLASH_MISMATCH0,
	BOOT_FLASH_MISMATCH1,
	BOOT_FLASH_MISMATCH2,
	BOOT_FLASH_THREE_MISMATCH,
}FLASH_AL_ACK;


extern const SECTOR_INFO_STRUCT Sector[MAX_SECTOR_SIZE + 1];
uint8_t Flash_Sector_Erase(uint8_t Sector, uint8_t NbSectors);
uint8_t Calc_Sector_From_block(uint16_t block, uint8_t Program);
void Set_Erase_Flag(uint32_t sector);
void Unset_Erase_Flag(uint32_t sector);
uint8_t Get_Erase_Flag(uint32_t sector);
uint8_t flash_copy_flash_to_flash_(uint32_t from, uint32_t to, uint32_t len);
uint8_t Flash_Redundancy_check(uint8_t* data0, uint8_t* data1, uint8_t* data2);
uint8_t flash_write(uint8_t* flash_addr, uint8_t* data, uint16_t size);

#endif /* FLASH_H_ */
