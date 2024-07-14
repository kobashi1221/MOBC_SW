/*
 * SD.h
 *
 *  Created on: 2021/11/16
 *      Author: daiki
 */

#ifndef SRC_USER_DRIVERS_CDH_SD_SD_H_
#define SRC_USER_DRIVERS_CDH_SD_SD_H_

#include "stm32_adafruit_sd.h"

#define SD_BLOCKSIZE (512)

typedef struct{
	uint32_t BLOCK_START;
	uint32_t BLOCK_END;
	uint32_t BLOCK_NOW;
	uint32_t BLOCK_OLD;
	uint8_t POSITION_COUNTER;
	uint8_t ring_permission;
	uint8_t ring_now;
}SD_SAVE_Info;

typedef struct{
	SD_SAVE_Info save_info;
}SD_Info_Irregular;

typedef struct{
	uint8_t save_data_buffer[512];
	SD_SAVE_Info save_info;
}SD_Info_Regular;

#ifdef __cplusplus
 extern "C" {
#endif
uint8_t sd_block_analyze(SD_SAVE_Info* p_sd_struct);
uint8_t sd_specify_write(uint32_t block_addr, uint16_t addr, uint8_t* p_data, uint16_t len);
uint8_t sd_write_address(uint32_t block_addr, uint32_t addr, uint8_t* p_writedata, uint16_t size);
uint8_t sd_specify_read(uint32_t block_addr, uint16_t addr, uint8_t* p_data, uint16_t len);
uint8_t sd_read_address(uint32_t block_addr, uint32_t addr, uint8_t* p_writedata, uint16_t size);
uint8_t sd_crc(uint32_t block_addr, uint32_t addr, uint32_t len, uint16_t *result, uint16_t *save_result);
uint8_t sd_copy_sd_to_sd_(uint32_t block_addr_dst, uint32_t addr_dst,
							uint32_t block_addr_src, uint32_t addr_src,
								uint32_t len);
uint8_t flash_copy_sd_to_flash_(uint32_t dst_adr,
								uint32_t src_block_adr, uint32_t src_adr,
								uint32_t len);
uint8_t sd_copy_flash_to_sd_(uint32_t dst_block_adr, uint32_t dst_adr,
							     uint32_t src_adr, uint32_t len);
uint8_t sd_copy_sd_to_ram_(uint32_t dst_adr,
								uint32_t src_block_adr, uint32_t src_adr,
								uint32_t len);

#ifdef __cplusplus
 }
#endif

#endif /* SRC_USER_DRIVERS_CDH_SD_SD_H_ */
