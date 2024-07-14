/*
 * flash.c
 *
 *  Created on: 2021/10/04
 *      Author: kahaku
 */
#include "flash.h"

#include <src_user/IfWrapper/HAL.h>
#include <src_user/TlmCmd/CCSDS/TCPacket.h>

//RXPICの受信可能な最大packet長
#define RXPIC_MAX_BUFFER (256)
//通常時packetの含むプログラムサイズ
#define DEFAULT_SIZE (RXPIC_MAX_BUFFER - TCP_PRM_HDR_LEN - TCP_CMD_2ND_HDR_LEN - TCP_CMD_USER_HDR_LEN)

static uint32_t Erase_Flag = 0;	//下位ビットからeraseされているかどうかを表示
void Set_Erase_Flag(uint32_t sector);
void Unset_Erase_Flag(uint32_t sector);
uint8_t Get_Erase_Flag(uint32_t sector);

const SECTOR_INFO_STRUCT Sector[MAX_SECTOR_SIZE + 1] = {
		0x08000000,	//SECTOR_0
		0x08004000, //SECTOR_1
		0x08008000, //SECTOR_2
		0x0800C000, //SECTOR_3
		0x08010000, //SECTOR_4
		0x08020000, //SECTOR_5
		0x08040000, //SECTOR_6
		0x08060000, //SECTOR_7
		0x08080000, //SECTOR_8
		0x080A0000, //SECTOR_9
		0x080C0000, //SECTOR_10
		0x080E0000, //SECTOR_11
		0x08100000, //SECTOR_12
		0x08104000, //SECTOR_13
		0x08108000, //SECTOR_14
		0x0810C000, //SECTOR_15
		0x08110000, //SECTOR_16
		0x08120000, //SECTOR_17
		0x08140000, //SECTOR_18
		0x08160000, //SECTOR_19
		0x08180000, //SECTOR_20
		0x081A0000, //SECTOR_21
		0x081C0000, //SECTOR_22
		0x081E0000, //SECTOR_23
		0x08200000	//end of main memory
};

/*	brief: セクターごとにFlashを削除
 * 	Sector: 削除する先頭セクター
 * 	NbSector: 先頭セクターから何セクター分削除するか
 */
uint8_t Flash_Sector_Erase(uint8_t Sector, uint8_t NbSectors){
	uint8_t res = 0;
	uint8_t timeout;
	uint32_t SectorError = 0;

	FLASH_EraseInitTypeDef p_EraseInit;
	//SectorとNbSectorはMRAMで管理する．
	p_EraseInit.Sector = Sector;
	p_EraseInit.NbSectors = NbSectors;
	//MAGNAROでは固定とする．
	p_EraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
	p_EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	HAL_FLASH_Unlock_C2A();
	timeout = 3;
	do{
		res = HAL_FLASHEx_Erase_C2A(&p_EraseInit, &SectorError);
	}while(timeout-- != 0 && res != 0);
	HAL_FLASH_Lock_C2A();


	if(SectorError == 0xFFFFFFFF){					//問題なく消せた
		for(uint32_t i = 0; i < NbSectors; i++){
			Set_Erase_Flag(Sector + (uint32_t)i);
		}
	}else{
		printf("Sector%d erase error\r\n",SectorError);		//SectorErrorの場所でエラー発生
		if(SectorError > Sector){
			for(uint32_t i = 0; i < (SectorError - Sector); i++){
				Set_Erase_Flag(Sector + (uint32_t)i);
			}
		}
	}

	return res;
}


/*	brief: プログラムの書き込み先セクターを算出する
 * 	block: プログラム保存領域の何ブロック目のデータか(0起算)
 * 	Program: 0 or 1 (0: FLIGHT_SW, 1: NEW_SW)
 */
uint8_t Calc_Sector_From_block(uint16_t block, uint8_t Program){
	int WriteBytes = block*DEFAULT_SIZE*2;	//これまでに書き込んだプログラムサイズを計算
	int Offset = 0x8100000; //NEW_SWの先頭アドレス　基本はこっちを使用する
	if(Program == 0){
		Offset = 0x8010000;	//FLIGHT_SWの先頭アドレス
	}

	uint8_t Number = 0;
	int Top = WriteBytes+Offset;	//書き込まれるべきアドレス
	while(WriteBytes < Sector[Number].Origin){
		Number++;
	}

	return Number;
}

void Set_Erase_Flag(uint32_t sector){
	Erase_Flag |= (1U << sector);
}

void Unset_Erase_Flag(uint32_t sector){
	Erase_Flag &= ~(1U << sector);
}

uint8_t Get_Erase_Flag(uint32_t sector){
	return (uint8_t)(Erase_Flag>>sector)&1U;
}

uint8_t flash_copy_flash_to_flash_(uint32_t from, uint32_t to, uint32_t len){
	uint8_t res;
	uint8_t timeout;

	uint8_t* original = from;
	uint8_t* copy = to;

	HAL_FLASH_Unlock_C2A();
	for(uint32_t i = 0; i < len; i++){
		timeout = 3;
		do{
			res = HAL_FLASH_Program_C2A(FLASH_TYPEPROGRAM_BYTE, (uint32_t)copy, (uint64_t)(*original));
			timeout--;
		}while((res != 0)&&timeout != 0);
		if(res != 0)	printf("a\r\n");
		if(timeout == 0)	printf("b\r\n");
		if(*copy != *original) printf("c\r\n");

		original++;
		copy++;
	}
	HAL_FLASH_Lock_C2A();

	return res;
}



/* @brief:三重冗長(多数決)
 * ReadData: 読み込んだデータ
 * Size :    データ長
 */
FLASH_AL_ACK Flash_Redundancy_check(uint8_t* data0, uint8_t* data1, uint8_t* data2){

	FLASH_AL_ACK ret = BOOT_FLASH_NO_MISMATCH;	//00000000

	if(*data0 != *data1){
		if(*data0 != *data2){
			if(*data1 != *data2){		//すべて異なる場合
				ret = BOOT_FLASH_THREE_MISMATCH;
			}
			else{
				//0が書き換わっている
				ret = BOOT_FLASH_MISMATCH0;
			}
		}
		else{
			//1が書き換わっている
			ret = BOOT_FLASH_MISMATCH1;
		}
	}
	else if(*data1 != *data2){
		//2が書き換わっている
		ret = BOOT_FLASH_MISMATCH2;
	}

	return ret;
}



/* @brief:flash_write
 * ReadData: 読み込んだデータ
 * Size :    データ長
 */
uint8_t flash_write(uint8_t* flash_addr, uint8_t* data, uint16_t size){

	uint8_t ret, timeout;
	HAL_FLASH_Unlock_C2A();
	for(int i = 0; i < size; i++){
		timeout = 3;
		do{
			ret = HAL_FLASH_Program_C2A(FLASH_TYPEPROGRAM_BYTE, (uint32_t)flash_addr, (uint64_t)data[i]);
			timeout--;
		}while((ret != 0)&&*(flash_addr) != data[i]&&timeout != 0);
		if(ret != 0)	printf("a\r\n");
		if(timeout == 0)	printf("b\r\n");
//		if(*(flash_addr) != data[i])	printf("c\r\n");

		flash_addr++;
	}
	HAL_FLASH_Lock_C2A();

	return ret;
}


