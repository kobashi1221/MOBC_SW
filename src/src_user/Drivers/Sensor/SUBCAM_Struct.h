/*
 * SCAMStruct.h
 *
 *  Created on: 2023/06/12
 *      Author: Osaki
 */

#ifndef SRC_USER_DRIVERS_SENSOR_SUBCAM_STRUCT_H_
#define SRC_USER_DRIVERS_SENSOR_SUBCAM_STRUCT_H_

#pragma once
#include "main.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	NOT_OPERATED,
	DATA_ANALYZE,
	SD_TO_SRAM,
}OV2640_ENUM;

typedef struct{
	int SCAM_counter;
	int SCAM_divide;
	uint32_t SCAM_start_block_addr;
	uint32_t SCAMbmp_start_block_addr;
	uint32_t SCAM_addr;
	int ImSize;
	int SCAM_pic_num;
	uint16_t Image_ID;

	uint32_t SCAMstamp1_start_block_addr;
	uint32_t SCAMstamp1_addr;
	uint32_t SCAMstamp2_start_block_addr;
	uint32_t SCAMstamp2_addr;
	uint32_t SCAMstamp3_start_block_addr;
	uint32_t SCAMstamp3_addr;

	int SCAMscaled_counter;
	uint32_t SCAMscaled_start_block_addr;
	uint32_t SCAMscaled_addr;
	int scaledSize;

	int SCAMraw_counter;
	uint32_t SCAMraw_start_block_addr;
	uint32_t SCAMraw_addr;
	int rawSize;

	int SCAM_BOA_counter;
	uint32_t SCAM_BOA_start_block_addr;
	uint32_t SCAM_BOA_addr;
	int BOA_Size;

	struct{
		uint8_t mode_flag;
		uint32_t ImSize;//画像サイズ
		uint32_t start_block_addr;//先頭ブロックアドレス
		uint32_t addr;//ブロックアドレスのどのアドレスか
		uint16_t picture_id;
		int count;//残りのSRAMに移す回数
		int now_save_count;
		uint16_t sram_input_num;
		uint8_t move_data_flag;
	}large_packet;
}OV2640_STRUCT;

extern OV2640_STRUCT* const SCAM_driver;

#ifdef __cplusplus
};
#endif

#endif /* SRC_USER_DRIVERS_SENSOR_SUBCAM_STRUCT_H_ */
