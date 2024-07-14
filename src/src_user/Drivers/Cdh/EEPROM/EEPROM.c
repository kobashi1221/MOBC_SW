/*
 * EEPROM.cpp
 *
 *  Created on: 2021/12/02
 *      Author: daiki
 */

#include "EEPROM.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Library/Delay.h>

#define DevAddress_Write0 0x00A0
#define DevAddress_Write1 0x00A2
#define DevAddress_Read0  0x00A1
#define DevAddress_Read1  0x00A3
#define MemAddres_Size 0x0002

#define BLOCK_LEN 256

static void *p_eeprom_i2c_handle_;


void EEPROM_Init(void* i2c_port){
	p_eeprom_i2c_handle_ = i2c_port;
}


/*
 * @brief ブロック書き込み
 * WriteDataBuf 書き込みデータ
 * BlockAddr    ブロックの番号(0~511)
 */
HAL_StatusTypeDef EEPROM_BLOCK_WRITE(uint8_t* WriteDataBuf, uint16_t BlockAddr){
	HAL_StatusTypeDef ret_HAL;

	if(BlockAddr < 256){		//ページ0でのブロック書き込み
		ret_HAL = HAL_I2C_Mem_Write_C2A(p_eeprom_i2c_handle_, DevAddress_Write0,
												(uint16_t)(BlockAddr * BLOCK_LEN), MemAddres_Size,
												WriteDataBuf, BLOCK_LEN, 0xff);
	}

	else{						//ページ1でのブロック書き込み
		BlockAddr = BlockAddr - 256;
		ret_HAL = HAL_I2C_Mem_Write_C2A(p_eeprom_i2c_handle_, DevAddress_Write1,
												(uint16_t)(BlockAddr * BLOCK_LEN), MemAddres_Size,
												WriteDataBuf, BLOCK_LEN, 0xff);
	}
	return ret_HAL;
}


/*
 * @brief ブロック内での書き込み
 * WriteDataBuf 書き込みデータ
 * DataSize     データ長(1~256)
 * WriteAddress データアドレス(0x00000000~0x0001FFFF)
 */
HAL_StatusTypeDef EEPROM_WRITE(uint8_t* WriteDataBuf, uint16_t DataSize, uint32_t WriteAddress){
	HAL_StatusTypeDef ret_HAL;

	uint32_t BlockNow = WriteAddress / BLOCK_LEN;
	uint16_t rest_len = (BlockNow + 1) * BLOCK_LEN - WriteAddress;

	if(rest_len < DataSize){		//アドレス内のブロックでは書き込めないとき
		ret_HAL = HAL_ERROR;
		return ret_HAL;
	}


	if(WriteAddress < 0x00010000){			//ページ0の時
		ret_HAL = HAL_I2C_Mem_Write_C2A(p_eeprom_i2c_handle_, DevAddress_Write0,
												(uint16_t)WriteAddress, MemAddres_Size,
												WriteDataBuf, DataSize, 0xff);
	}
	else{	//ページ1の時
		WriteAddress = WriteAddress - 0x00010000;
		ret_HAL = HAL_I2C_Mem_Write_C2A(p_eeprom_i2c_handle_, DevAddress_Write1,
												(uint16_t)WriteAddress, MemAddres_Size,
												WriteDataBuf, DataSize, 0xff);
	}
	return ret_HAL;
}


/*	@brief: eeprom用の汎用write関数
 * 	WriteDataBuf: 書き込むPayloadの配列
 * 	DataSize: Payload長[Bytes]
 * 	WriteAddress: 書き込み先先頭アドレス
 */
HAL_StatusTypeDef EEPROM_GeneralWrite(uint8_t* WriteDataBuf,
									size_t DataSize,
										uint32_t WriteAddress){
	HAL_StatusTypeDef ret_HAL = HAL_ERROR;											//通信結果確認用変数

	uint32_t BlockNow = WriteAddress / BLOCK_LEN;						//現在のブロックアドレス
	uint32_t rest_len = (BlockNow + 1) * BLOCK_LEN - WriteAddress;		//今のブロックで残り書き込める長さ

	if(rest_len >= DataSize){											//現在のブロックだけで書き込めるとき
		ret_HAL= EEPROM_WRITE(WriteDataBuf, (uint16_t)DataSize, WriteAddress);
	}

	else{																//現在のブロックだけでは書き込めないとき
		ret_HAL = EEPROM_WRITE(WriteDataBuf, (uint16_t)rest_len, WriteAddress);	//現在のブロックに書ける分だけ書く

		uint32_t Len = DataSize - rest_len;		//残り書き込み長さ
		uint16_t BlockNum = Len / BLOCK_LEN;	//書き込みブロック数
		uint16_t SurplusLen = Len % BLOCK_LEN;  //余りデータ長
		uint16_t offset = 0;
		uint16_t block_offset = 1;

		while(BlockNum--){
			ret_HAL = EEPROM_BLOCK_WRITE(WriteDataBuf + rest_len + offset, (uint16_t)(BlockNow + block_offset));
			offset += BLOCK_LEN;
			block_offset++;
			printf("irregular\r\n");			//MAGNAROでは128Byte書き込みがmax
		}
		c2a_delay_ms(5);

		if(SurplusLen != 0){
			ret_HAL = EEPROM_WRITE(WriteDataBuf + rest_len + offset, SurplusLen, WriteAddress + rest_len + offset);
		}
	}
	c2a_delay_ms(5);

	return ret_HAL;
}


/*	@brief: eeprom用の汎用read関数    readはブロック単位でなくても大丈夫そう?
 * 	ReadDataBuf: 読み出しデータを格納するバッファ
 * 	DataSize: データサイズ[Bytes]
 * 	ReadAddress: 読み出し先頭アドレス
 */
HAL_StatusTypeDef EEPROM_GeneralRead(uint8_t* ReadDataBuf,
									size_t DataSize,
										uint32_t ReadAddress){
	HAL_StatusTypeDef ret_HAL = HAL_ERROR;											//通信結果確認用変数

	if(ReadAddress < 0x00010000){	//スタートがpage0

		uint32_t len = 0x00010000 - ReadAddress;	//page0だけで読みめる長さ

		if(len >= DataSize)	//page0に収まるとき
		{
			ret_HAL = HAL_I2C_Mem_Read_C2A(p_eeprom_i2c_handle_, DevAddress_Read0,
													(uint16_t)ReadAddress, MemAddres_Size,
													ReadDataBuf, (uint16_t)DataSize, 0xff);
			if(ret_HAL != HAL_OK)	return ret_HAL;
//			c2a_delay_ms(10);
		}
		else	////page0に収まらないとき
		{
			ret_HAL = HAL_I2C_Mem_Read_C2A(p_eeprom_i2c_handle_, DevAddress_Read0,
													(uint16_t)ReadAddress, MemAddres_Size,
													ReadDataBuf, (uint16_t)len, 0xff);
			if(ret_HAL != HAL_OK)	return ret_HAL;
//			c2a_delay_ms(10);
			ret_HAL = HAL_I2C_Mem_Read_C2A(p_eeprom_i2c_handle_, DevAddress_Read1,
													0x0000, MemAddres_Size,
													ReadDataBuf + len, (uint16_t)(DataSize - len), 0xff);
			if(ret_HAL != HAL_OK)	return ret_HAL;
//			c2a_delay_ms(10);
		}
	}


	else if(ReadAddress >= 0x00010000 && ReadAddress < 0x00020000)	//スタートがpage1
	{
		ReadAddress = ReadAddress - 0x00010000;
		ret_HAL = HAL_I2C_Mem_Read_C2A(p_eeprom_i2c_handle_, DevAddress_Read1,
										(uint16_t)ReadAddress, MemAddres_Size,
										ReadDataBuf, (uint16_t)DataSize, 0xff);
//		c2a_delay_ms(10);
	}

	return ret_HAL;
}
