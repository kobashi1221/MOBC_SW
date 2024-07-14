/*
 * SD.c
 *
 *  Created on: 2021/11/16
 *      Author: daiki
 */

#include <stdio.h>
#include <inttypes.h>
#include <src_core/System/TimeManager/time_manager.h>
#include "CRC_calc.h"
#include "SD.h"

/**
  * @brief  ブロックアドレスが正しいか調べる
  * @param
  *
  * @return
  */
uint8_t sd_block_analyze(SD_SAVE_Info* p_sd_struct){

	uint8_t res = 0;

	if((uint32_t)p_sd_struct->BLOCK_NOW < (uint32_t)p_sd_struct->BLOCK_START ||
			(uint32_t)p_sd_struct->BLOCK_NOW > (uint32_t)p_sd_struct->BLOCK_END){

		//現在のブロックアドレスが範囲内にない時はstart位置に戻す
		p_sd_struct->BLOCK_NOW = p_sd_struct->BLOCK_START;
		res++;
	}

	if((uint32_t)p_sd_struct->BLOCK_OLD < (uint32_t)p_sd_struct->BLOCK_START ||
			(uint32_t)p_sd_struct->BLOCK_OLD > (uint32_t)p_sd_struct->BLOCK_END){

		//現在のブロックアドレスが範囲内にない時はstart位置に戻す
		p_sd_struct->BLOCK_OLD = p_sd_struct->BLOCK_START;
		res++;
	}
	if(p_sd_struct->BLOCK_OLD == p_sd_struct->BLOCK_NOW + 1) p_sd_struct->ring_now = 1;
	else p_sd_struct->ring_now = 0;
	return res;
}


/**
  * @brief  指定したアドレスに書き込み
  * @param  uint32_t ブロックアドレス 4Byte
  * @param  uint16_t ブロックの中でのアドレス(0~512)
  * @param  保存したいデータの先頭アドレス
  * @param  uint16_t データ長
  */
uint8_t sd_specify_write(uint32_t block_addr, uint16_t addr, uint8_t* p_data, uint16_t len){
	uint8_t res = 0;
	uint8_t receive_data[SD_BLOCKSIZE] = {0};
	uint16_t block_num;
	uint16_t surplus_len;
	uint16_t temp;

	if(addr == 0 && len >= SD_BLOCKSIZE){		//はじめに読み込む必要がない場合
		res = BSP_SD_WriteBlocks(p_data, block_addr, 1, 0xFFFF);
		if(res != BSP_SD_OK){
			return res;
		}
		temp = SD_BLOCKSIZE;
		block_num = (len - SD_BLOCKSIZE) / SD_BLOCKSIZE;
		surplus_len = (len - SD_BLOCKSIZE) % SD_BLOCKSIZE;
	}

	else{										//はじめに読み込む必要がある場合
		res = BSP_SD_ReadBlocks(&receive_data, block_addr, 1, 0xFFFF);
		if(res != BSP_SD_OK){
			return res;
		}

		temp = SD_BLOCKSIZE - addr;

		if(temp >= len){					//指定したブロックで書き込みが終了するとき
			memcpy(&(receive_data[addr]), p_data, len);
			res = BSP_SD_WriteBlocks(&receive_data, block_addr, 1, 0xFFFF);
			return res;
		}
		else{
			memcpy(&(receive_data[addr]), p_data, temp);
			res = BSP_SD_WriteBlocks(&receive_data, block_addr, 1, 0xFFFF);
			if(res != BSP_SD_OK){
				return res;
			}

			block_num = (len - temp) / SD_BLOCKSIZE;
			surplus_len = (len - temp) % SD_BLOCKSIZE;
		}
	}

	if(block_num > 0){							//指定したブロックを超える場合
		res = BSP_SD_WriteBlocks(p_data + temp, block_addr + 1, block_num, 0xFFFF);
		if(res != BSP_SD_OK){
			return res;
		}
	}

	if(surplus_len != 0){
		res = BSP_SD_ReadBlocks(&receive_data, block_addr + block_num + 1, 1, 0xFFFF);
		if(res != BSP_SD_OK){
			return res;
		}

		memcpy(&(receive_data[0]), p_data + temp + block_num * SD_BLOCKSIZE, surplus_len);
		res = BSP_SD_WriteBlocks(&receive_data, block_addr + block_num + 1, 1, 0xFFFF);
		if(res != BSP_SD_OK){
			return res;
		}
	}

	return res;
}


/**
  * @brief  指定したアドレスに書き込み
  * @param  uint32_t セクションの先頭ブロックアドレス 4Byte
  * @param  uint32_t セクション内でのアドレス
  * @param  保存したいデータの先頭アドレス
  * @param  uint16_t データ長
  */
uint8_t sd_write_address(uint32_t block_addr, uint32_t addr, uint8_t* p_writedata, uint16_t size){
	uint32_t internal_block = addr/SD_BLOCKSIZE;
	uint32_t internal_rest = addr%SD_BLOCKSIZE;
	uint8_t res = sd_specify_write(block_addr + internal_block, internal_rest, p_writedata, size);
	return res;
}


/**
  * @brief  指定したアドレスから読み込み
  * @param  uint32_t ブロックアドレス 4Byte
  * @param  uint16_t ブロックの中でのアドレス(0~512)
  * @param  読み出し後のデータの先頭アドレス
  * @param  uint16_t データ長
  */
uint8_t sd_specify_read(uint32_t block_addr, uint16_t addr, uint8_t* p_data, uint16_t len){
	uint8_t res = 0;
	uint8_t receive_data[SD_BLOCKSIZE] = {0};

	res = BSP_SD_ReadBlocks(receive_data, block_addr, 1, 0xFFFF);
	if(res != BSP_SD_OK){
		return res;
	}

	uint16_t temp = SD_BLOCKSIZE - addr;

	if(temp >= len){//指定したブロックで書き込みが終了するとき
		memcpy(p_data, &(receive_data[addr]), len);
	}

	else{//指定したブロックを超えるとき
		memcpy(p_data, &(receive_data[addr]), temp);

		uint16_t block_num = (len - temp) / SD_BLOCKSIZE;
		uint16_t surplus_len = (len - temp) % SD_BLOCKSIZE;
		if(block_num > 0){
			res = BSP_SD_ReadBlocks(p_data + temp, block_addr + 1, block_num, 0xFFFF);
			if(res != BSP_SD_OK){
				return res;
			}
		}

		if(surplus_len > 0){
			res = BSP_SD_ReadBlocks(receive_data, block_addr + block_num + 1, 1, 0xFFFF);
			if(res != BSP_SD_OK){
				return res;
			}
			memcpy(p_data + temp + block_num * SD_BLOCKSIZE, &(receive_data[0]), surplus_len);
		}
	}
	return res;
}


/**
  * @brief  指定したアドレスから読み込み
  * @param  uint32_t セクションの先頭ブロックアドレス 4Byte
  * @param  uint32_t 指定のセクション先頭からの相対アドレス
  * @param  読みとり後のデータの先頭アドレス
  * @param  uint16_t データ長
  */
uint8_t sd_read_address(uint32_t block_addr, uint32_t addr, uint8_t* p_writedata, uint16_t size){
	uint32_t internal_block = addr/SD_BLOCKSIZE;
	uint32_t internal_rest = addr%SD_BLOCKSIZE;
	uint8_t res = sd_specify_read(block_addr + internal_block, internal_rest, p_writedata, size);
	return res;
}

/**
  * @brief  SDの指定の領域のCRC計算を行う関数
  * 複数ブロック指定時はひとブロックずつ読み込みして追記的にCRC計算を行っていく
  * 20220116　inakawa作成
  * @param uint32_t block_addr　セクションの先頭ブロックアドレス 4Byte
  * @param uint32_t addr 指定のセクション先頭からの相対アドレス
  * @param uint32_t len CRC計算したいバイト長さ
  * @param uint16_t *result 計算結果
  * @return uint8_t res 0-2 adfruit.h 参照 3:mallocエラー
  */
uint8_t sd_crc(uint32_t block_addr, uint32_t addr, uint32_t len, uint16_t *result, uint16_t *save_result)
{
	uint32_t read_byte = 0;
	uint8_t receive_data[SD_BLOCKSIZE] = {0};
	uint8_t res = 0;

	while ((len-read_byte) > SD_BLOCKSIZE) {
		res = sd_read_address(block_addr, addr + read_byte, receive_data, SD_BLOCKSIZE);
		if(res != BSP_SD_OK){
			return res;
		}
		calc_crc16_update(&receive_data[0], SD_BLOCKSIZE, result);
		calc_crc16_update(&receive_data[0], SD_BLOCKSIZE, save_result);
		read_byte += SD_BLOCKSIZE;
	}

	res = sd_read_address(block_addr, addr + read_byte, receive_data, (len-read_byte));
	if(res != BSP_SD_OK){
		return res;
	}

	calc_crc16_update(&receive_data[0], (len-read_byte), result);
	calc_crc16_update(&receive_data[0], (len-read_byte), save_result);

	return res;
}

uint8_t sd_data[512];

uint8_t sd_copy_sd_to_sd_(uint32_t block_addr_dst, uint32_t addr_dst,
							uint32_t block_addr_src, uint32_t addr_src,
								uint32_t len){
	uint8_t ret;

	ret = sd_read_address(block_addr_src, addr_src, sd_data, len);

	ret = sd_write_address(block_addr_dst, addr_dst, sd_data, len);

	return ret;
}

uint8_t flash_copy_sd_to_flash_(uint32_t dst_adr,
								uint32_t src_block_adr, uint32_t src_adr,
								uint32_t len){
	uint8_t ret;

	step_t before = TMGR_get_master_step();
	ret = sd_read_address(src_block_adr, src_adr, sd_data, len);
	step_t after = TMGR_get_master_step();
	printf("sd time = %d\r\n", after- before);

	before = TMGR_get_master_step();
	ret = flash_write((uint8_t*)dst_adr, sd_data, (uint16_t)len);
	after = TMGR_get_master_step();
	printf("flash time = %d\r\n", after- before);

	return ret;
}

uint8_t sd_copy_flash_to_sd_(uint32_t dst_block_adr, uint32_t dst_adr,
							     uint32_t src_adr, uint32_t len){
	uint8_t ret;

	ret = sd_write_address(dst_block_adr, dst_adr, (uint8_t*)src_adr, len);

	return ret;
}

uint8_t sd_copy_sd_to_ram_(uint32_t dst_adr,
								uint32_t src_block_adr, uint32_t src_adr,
								uint32_t len){
	uint8_t ret;
	uint8_t* p_ram = (uint8_t*)dst_adr;

	ret = APP_TMD_is_vaild_adr_in_ram_(dst_adr);
	if(ret != 1) return 3;

	ret = sd_read_address(src_block_adr, src_adr, p_ram, len);

	return ret;
}

