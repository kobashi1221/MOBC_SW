/*
 * MemoryMW.c
 *
 *  Created on: 2021/12/02
 *      Author: SAT37
 */

#include "MemoryMW.h"

#include <stdio.h>
#include <string.h>
#include <src_user/Drivers/Cdh/MRAM/MRAM.h>
#include <src_user/Drivers/Cdh/EEPROM/EEPROM.h>
#include <src_user/Drivers/Cdh/SD/SD.h>
#include <src_user/Applications/UserDefined/Memory/MEM_cmd_SD.h>
#include <src_user/Applications/UserDefined/Memory/MEM_cmd_SPIPROM.h>
#include <src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.h>
#include <src_user/Applications/UserDefined/Cdh/ResetUtility.h>
#include <src_user/Library/General/DBG_printf.h>
#include "../UserDefined/GlobalMemoryParam.h"
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>

static uint8_t memory_mode = 0;
const uint8_t* p_memory_mode;
//パラメータROMの選択変数
//0b0001: read_paramで使用するROMの選択[1: MRAM, 0: EEPROM]
//0b0010: write_paramでのEEPROMの有効化[1: ENABLE, 0: DISABLE]
//0b0100: write_paramでのMRAMの有効化[1: ENABLE, 0: DISABLE]
//0b1000: PARAM_ROMを使用せずFLASHのconst値を使用するかの選択[1: ENABLE, 0: DISABLE]

static MISMATCH_Info mem_mis_info_;
const MISMATCH_Info* mem_mis_info = &mem_mis_info_;

typedef enum{
	FLAG_PARAMROM_TWO_MISMATCH = 0,
	FLAG_PARAMROM_THREE_MISMATCH = 1,
	FLAG_PARAMROM_HAL_ERROR    = 2,
	FLAG_PARAMROM_HAL_BUSY     = 3,
	FLAG_PARAMROM_HAL_TIMEOUT  = 4,
}PARAMROM_AH_FLAG;

/*	brief:	アノマリーフラグのゲッター
 * 	flag:	アノマリーフラグ
 * 	num:	読み取り対象ビットの場所(下位ビットから何bitか)
 */
static uint8_t get_ah_flag(uint8_t* flag, PARAMROM_AH_FLAG num){
	return (*flag >> num) & 0x01;
}

/*	brief:	アノマリーフラグのセッター
 * 	flag:	アノマリーフラグ
 * 	num:	読み取り対象ビットの場所(下位ビットから何bitか)
 */
static void set_ah_flag(uint8_t* flag, PARAMROM_AH_FLAG num){
	uint8_t temp = (0x01 << num);
	*flag |= temp;
	return;
}

/*	brief:	アノマリーフラグのセッター
 * 	flag:	アノマリーフラグ
 * 	num:	読み取り対象ビットの場所(下位ビットから何bitか)
 */
uint8_t get_mode_flag(uint8_t* flag, PARAMROM_MODE_FLAG num){
	return (*flag >> num) & 0x01;
}

/*	brief:		通信ポートの設定・memory_modeの初期化
 * 	spi_port:	SPIポート
 * 	cs_port:	CSポート
 * 	cs_pin:		CSピン
 * 	i2c_port:	I2Cポート
 */
void ParamMemory_Init(void* spi_port, void* cs_port, uint16_t cs_pin, void* i2c_port){
	MRAM_Init(spi_port, cs_port, cs_pin);
	EEPROM_Init(i2c_port);
	memory_mode = pic_data->Memory;
	p_memory_mode = &memory_mode;
//	memory_mode = 0b00000110;
	printf("Memory, Program(Data): %02x %02x %02x\r\n",pic_data->Memory, pic_data->Program, pic_data->Repro);
	printf("Memory, Program(ADDR): %08x %08x\r\n",&pic_data->Memory, &pic_data->Program);
}


/* @brief:三重冗長(多数決)
 * ReadData: 読み込んだデータ
 * Size :    データ長
 */
PARAM_MEM_AL_ACK Redundancy(uint8_t* ReadData_Redundancy0, uint8_t* ReadData_Redundancy1, uint8_t* ReadData_Redundancy2, uint16_t Size){

	uint8_t ret = 0;	//00000000
	for(int i = 0; i < Size; i++){
		if((ReadData_Redundancy0[i] != ReadData_Redundancy1[i])){
			if(ReadData_Redundancy0[i] != ReadData_Redundancy2[i]){
				if(ReadData_Redundancy1[i] != ReadData_Redundancy2[i]){		//すべて異なる場合
					set_ah_flag(&ret, FLAG_PARAMROM_THREE_MISMATCH);         //どこか一つでも違うと終わる	//1
				}
				else{
					ReadData_Redundancy0[i] = ReadData_Redundancy2[i]; 	//2		//0が書き換わっている
					set_ah_flag(&ret, FLAG_PARAMROM_TWO_MISMATCH);
				}
			}
			else{
				ReadData_Redundancy1[i] = ReadData_Redundancy2[i];		//3		//1が書き換わっている
				set_ah_flag(&ret, FLAG_PARAMROM_TWO_MISMATCH);
			}
		}
		else if(ReadData_Redundancy0[i] != ReadData_Redundancy2[i]){
			ReadData_Redundancy2[i] = ReadData_Redundancy0[i];					//2が書き換わっている
			set_ah_flag(&ret, FLAG_PARAMROM_TWO_MISMATCH);				//5
		}
	}

	if(get_ah_flag(&ret, FLAG_PARAMROM_THREE_MISMATCH))		return MEMORY_PARAM_THREE_MISMATCH;
	if(get_ah_flag(&ret, FLAG_PARAMROM_TWO_MISMATCH))		return MEMORY_PARAM_TWO_MISMATCH;

	return MEMORY_PARAM_NO_MISMATCH;
}

/* @brief			すべての型に対応した汎用Write関数
 * WriteData: 		書き込む配列データ
 * Size:			MRAM/EEPROMに書き込むデータの大きさ[Bytes]
 * WriteAddress: 	書き込み先先頭アドレス(0x0000~0xAAAA)
 * type: 			型		例."int"
 */
uint8_t write_param(void* WriteData, const void* WriteData_, size_t Size, uint32_t WriteAddress){
  uint8_t mram_flag, eeprom_flag;
  uint8_t ret = 0;
  uint8_t ret1, ret2;
  if(get_mode_flag(&memory_mode, FLAG_PARAMROM_WRITE_EEPROM)) eeprom_flag = 1;
  if(get_mode_flag(&memory_mode, FLAG_PARAMROM_WRITE_MRAM)) mram_flag = 1;

  if(mram_flag){//mram_flagが立っていたらMRAMに書き込む
//  printf("mram write\r\n");
    ret1 = MRAM_write_param(WriteData, WriteData_, Size, WriteAddress);
  }
  if(eeprom_flag){//eeprom_flagが立っていたらEEPROMに書き込む
//  printf("eeprom write\r\n");
    ret2 = EEPROM_write_param(WriteData, WriteData_, Size, WriteAddress);
  }
  ret = ret1 << 4 | ret2;
  return ret;  //通信結果を返す
}


/*	@brief:すべての型に対応した汎用read関数
 *	ReadDataBuf: 			同期用変数・配列
 *	DataSize: 				同期用変数・配列の要素数
 *	ReadAddress:			読み出し先頭アドレス(0x0000~0xAAAA)
 *	type:					型
 */
uint8_t read_param(void* ReadDataBuf, const void* ReadDataBuf_, size_t Size, uint32_t ReadAddress){
  uint8_t mode_flag = get_mode_flag(&memory_mode, FLAG_PARAMROM_READ);
  uint8_t flash_flag = get_mode_flag(&memory_mode, FLAG_PARAMROM_READ_FLASH);
  uint8_t ret = 0;
  uint32_t temp_adr = ReadAddress;
  uint8_t* temp_p = (uint8_t*)ReadDataBuf;

  if(flash_flag){
    if(ReadDataBuf_ != NULL){
      memcpy(ReadDataBuf, ReadDataBuf_, Size);
    }
    return 0;
  }
  else{
    if(mode_flag){ //フラグが1の時はMRAMを使用する
      ret = MRAM_read_param(ReadDataBuf, ReadDataBuf_, Size, ReadAddress);
    }
    else{ //フラグが0の時はEEPROMを使用する
      ret = EEPROM_read_param(ReadDataBuf, ReadDataBuf_, Size, ReadAddress);
    }
    if(ret == 0){
      if(DBG_Mode_Analyze(MRAM_EEPROM)){
        for(int i = 0; i < Size; i++){
	  print(MRAM_EEPROM, "%d %08x %02x\r\n", mode_flag, temp_adr, *temp_p);
	  temp_adr++;
	  temp_p++;
	  if(i % 100 == 0){
	    HAL_IWDG_Refresh_C2A(&hiwdg);
	    WDT_clear_count();
	  }
        }
      }
    }
    return ret;
  }
}



/* @brief			すべての型に対応した汎用Write関数
 * WriteData: 		書き込む配列データ
 * Size:			MRAMに書き込むデータの大きさ[Bytes]
 * WriteAddress: 	書き込み先先頭アドレス
 * type: 			型		例."int"
 */
uint8_t MRAM_write_param(void* WriteData,
						const void* WriteData_,
							size_t Size,
								uint32_t WriteAddress){
	uint8_t ret_HAL = 0;								//	通信結果確認用変数

	if(WriteAddress >= MRAM_REDUNDANCY_SIZE || (MRAM_REDUNDANCY_SIZE - WriteAddress) < Size){	//0x0000AAAA - WriteAddress + 1
//		AL_add_anomaly(AL_GROUP_MRAM_PARAM, MEMORY_PARAM_ILLEGAL_ADDRESS);
		printf("MRAM ADDRESS ANOMALY\r\n");
		return 4;
	}

	if(WriteData_ != NULL)memcpy(WriteData, WriteData_, Size);	//FlashからRAMにデータをコピー

	size_t PayloadSize = Size;				//	Payload長[Byte]

	//	データをBLOCKSIZEで分割	//
	uint16_t Block_rest = PayloadSize%BLOCKSIZE;				//Blockで分割した余りデータのサイズ[Bytes]
	uint16_t Block_num = (PayloadSize-Block_rest)/BLOCKSIZE;	//Block数
	uint8_t* Payload = (uint8_t*)WriteData;

	//MRAM特有のenable
	if((ret_HAL = MRAM_Enable()) != 0){
		ANOMALY_SPI_Set_Status(SPI_FAULT_MRAM_LORA, ret_HAL);
		printf("MRAM COM ANOMALY\r\n");
		return ret_HAL;
	}

	//	三十冗長用アドレス　セット	//
	uint32_t Address[3] = {0,0,0};								//	三十冗長用アドレス　配列
	for(int i = 0; i < 3; i++){
		Address[i] = WriteAddress + MRAM_REDUNDANCY_SIZE*i;					//通常の使用可能領域は0x000000~0x00AAAA;(残りは冗長用)
	}
	//	三十冗長	//
	for(int cnt = 0; cnt < 3; cnt++){
		//	分割送信		//
		for(int i = 0; i < Block_num; i++){
			ret_HAL = MRAM_GeneralWrite(Payload + i*BLOCKSIZE, BLOCKSIZE, (Address[cnt]+BLOCKSIZE*i));
			if(ret_HAL != HAL_OK){
				ANOMALY_SPI_Set_Status(SPI_FAULT_MRAM_LORA, ret_HAL);
				printf("MRAM COM ANOMALY\r\n");
				return ret_HAL;
			}
		}
		if(Block_rest!=0){
			ret_HAL = MRAM_GeneralWrite(Payload + Block_num*BLOCKSIZE, Block_rest, (Address[cnt]+BLOCKSIZE*Block_num));
		}
		if(ret_HAL != HAL_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_MRAM_LORA, ret_HAL);
			printf("MRAM COM ANOMALY\r\n");
			return ret_HAL;
		}
	}

	if((ret_HAL =MRAM_Disable()) != HAL_OK){
		ANOMALY_SPI_Set_Status(SPI_FAULT_MRAM_LORA, ret_HAL);
		printf("MRAM COM ANOMALY\r\n");
		return ret_HAL;
	}

	return ret_HAL;												//通信結果を返す
}

/*	@brief:すべての型に対応した汎用read関数
 *	ReadDataBuf: 			同期用変数・配列
 *	DataSize: 				同期用変数・配列の要素数
 *	ReadAddress:			読み出し先頭アドレス
 *	type:					型
 */
uint8_t MRAM_read_param(void* ReadDataBuf, const void* ReadDataBuf_, size_t Size, uint32_t ReadAddress){

  uint8_t ret_HAL = 0;	// 通信結果確認用変数
  PARAM_MEM_AL_ACK ret_Redundancy = 0;
  uint8_t err_flag = 0;
  if(ReadAddress >= MRAM_REDUNDANCY_SIZE || (MRAM_REDUNDANCY_SIZE - ReadAddress) < Size){// 0x0000AAAA - WriteAddress + 1
//  AL_add_anomaly(AL_GROUP_MRAM_PARAM, MEMORY_PARAM_ILLEGAL_ADDRESS);
    printf("MRAM ADDRESS ANOMALY\r\n");
    return 4;
  }

  if(ReadDataBuf_ != NULL){
      memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
  }

  size_t ReadBytes = Size;// 型サイズの確認
  //	データをBLOCKSIZEで分割	//
  uint16_t Block_rest = ReadBytes%BLOCKSIZE;// Blockで分割したときの余り[Bytes]
  uint16_t Block_num = (ReadBytes - Block_rest)/BLOCKSIZE;// Block数
  uint8_t ReadData_Redundancy[3][BLOCKSIZE]; // Blockの長さ分だけ確保
  uint8_t* ReadBuf = (uint8_t*)ReadDataBuf;

  //MRAM特有のenable
  if((ret_HAL =MRAM_Enable()) != HAL_OK){
    ANOMALY_SPI_Set_Status(SPI_FAULT_MRAM_LORA, ret_HAL);
    if(ReadDataBuf_ != NULL){
      memcpy(ReadDataBuf, ReadDataBuf_, Size);// FlashからRAMにデータをコピー
    }
    print(ANOMALY, "MRAM COM ANOMALY\r\n");
    return ret_HAL;
  }

  //	三十冗長用アドレス　セット	//
  uint32_t Address[3] = {0,0,0};// 三十冗長用アドレス　配列
  for(int i = 0; i < 3; i++){
    Address[i] = ReadAddress + MRAM_REDUNDANCY_SIZE*i; // 通常の使用可能領域は0x000000~0x00AAAA;(残りは冗長用)
  }

  //BLOCKSIZEで分割して多数決
  ret_HAL = 0;
  for(int i = 0; i < Block_num; i++){	//ブロック分
    //三十冗長  //
    for(int cnt = 0; cnt < 3; cnt++){
      ret_HAL = MRAM_GeneralRead((uint8_t*)&ReadData_Redundancy[cnt][0], BLOCKSIZE, Address[cnt]+BLOCKSIZE*i);
	if(ret_HAL != HAL_OK){
	  ANOMALY_SPI_Set_Status(SPI_FAULT_MRAM_LORA, ret_HAL);
	  if(ReadDataBuf_ != NULL){
	      memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
	  }
	  print(ANOMALY, "MRAM COM ANOMALY\r\n");
	  return ret_HAL;
	}
    }

    ret_Redundancy = Redundancy(&ReadData_Redundancy[0][0], &ReadData_Redundancy[1][0], &ReadData_Redundancy[2][0], BLOCKSIZE);

    if(ret_Redundancy == MEMORY_PARAM_TWO_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH);
    if(ret_Redundancy == MEMORY_PARAM_THREE_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH);
    memcpy(ReadBuf + BLOCKSIZE * i, ReadData_Redundancy[0], BLOCKSIZE);
  }

  //あまり分の読み出し
  if(Block_rest!=0){
    for(int cnt = 0; cnt < 3; cnt++){
      ret_HAL = MRAM_GeneralRead((uint8_t*)&ReadData_Redundancy[cnt][0], Block_rest, Address[cnt]+BLOCKSIZE*Block_num);
      if(ret_HAL != HAL_OK){
	ANOMALY_SPI_Set_Status(SPI_FAULT_MRAM_LORA, ret_HAL);
	if(ReadDataBuf_ != NULL)	memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
	print(ANOMALY, "MRAM COM ANOMALY\r\n");
	return ret_HAL;
      }
    }
  }

  ret_Redundancy = Redundancy(&ReadData_Redundancy[0][0], &ReadData_Redundancy[1][0], &ReadData_Redundancy[2][0], Block_rest);

  if(ret_Redundancy == MEMORY_PARAM_TWO_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH);
  if(ret_Redundancy == MEMORY_PARAM_THREE_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH);
  memcpy(ReadBuf + BLOCKSIZE * Block_num, ReadData_Redundancy[0], Block_rest);

  if((ret_HAL =MRAM_Disable()) != HAL_OK){								//MRAM特有のdisable
    ANOMALY_SPI_Set_Status(SPI_FAULT_MRAM_LORA, ret_HAL);
    if(ReadDataBuf_ != NULL)	memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
    printf("MRAM COM ANOMALY\r\n");
    return ret_HAL;
  }

  if(get_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH)){
//  AL_add_anomaly(AL_GROUP_MRAM_PARAM, MEMORY_PARAM_TWO_MISMATCH);
    MRAM_write_param(ReadDataBuf, NULL, Size, ReadAddress);						//ROMを修正
    printf("MRAM_TWO_MISMATCH\r\n");
    printf("Address is %08x \r\n",ReadAddress);
    ret_HAL = 5;
  }

  if(get_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH)){
//  AL_add_anomaly(AL_GROUP_MRAM_PARAM, MEMORY_PARAM_THREE_MISMATCH);
    if(ReadDataBuf_ != NULL) memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
    mem_mis_info_.mis_mram.address = ReadAddress;
    mem_mis_info_.mis_mram.size = Size;
    ret_HAL = 6;
    generate_memory_repro_tlm();
  }
  return ret_HAL;
}

/* @brief			すべての型に対応したEEPROMの汎用Write関数
 * WriteData: 		書き込む配列データ
 * Size:			EEPROMに書き込むデータの大きさ[Bytes]
 * WriteAddress: 	書き込み先先頭アドレス(0x0000~0xAAAA)
 * type: 			型		例."int"
 */
uint8_t EEPROM_write_param(void* WriteData,
						const void* WriteData_,
							size_t Size,
								uint32_t WriteAddress){
	uint8_t ret_HAL = 0;								//	通信結果確認用変数

	if(WriteAddress >= EEPROM_REDUNDANCY_SIZE || (EEPROM_REDUNDANCY_SIZE - WriteAddress) < Size){	//0x0000AAAA - WriteAddress + 1
//		AL_add_anomaly(AL_GROUP_EEPROM_PARAM, MEMORY_PARAM_ILLEGAL_ADDRESS);
		printf("PARAMROM ADDRESS ANOMALY\r\n");
		return 4;
	}

	if(WriteData_ != NULL)memcpy(WriteData, WriteData_, Size);	//FlashからRAMにデータをコピー

	size_t PayloadSize = Size;				//	Payload長[Byte]

	//	データをBLOCKSIZEで分割	//
	uint16_t Block_rest = PayloadSize%BLOCKSIZE;				//Blockで分割した余りデータのサイズ[Bytes]
	uint16_t Block_num = (PayloadSize-Block_rest)/BLOCKSIZE;	//Block数
	uint8_t* Payload = (uint8_t*)WriteData;


	//	三十冗長用アドレス　セット	//
	uint32_t Address[3] = {0,0,0};								//	三十冗長用アドレス　配列
	for(int i = 0; i < 3; i++){
		Address[i] = WriteAddress + EEPROM_REDUNDANCY_SIZE*i;					//通常の使用可能領域は0x000000~0x00AAAA;(残りは冗長用)
	}
	//	三十冗長	//
	for(int cnt = 0; cnt < 3; cnt++){
		//	分割送信		//
		for(int i = 0; i < Block_num; i++){
			ret_HAL = EEPROM_GeneralWrite(Payload + i*BLOCKSIZE, BLOCKSIZE, (Address[cnt]+BLOCKSIZE*i));
			if(ret_HAL != HAL_OK){
				ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, ret_HAL);
				printf("PARAMROM COM ANOMALY\r\n");
				return ret_HAL;
			}
		}
	if(Block_rest!=0){
		ret_HAL = EEPROM_GeneralWrite(Payload + Block_num*BLOCKSIZE, Block_rest, (Address[cnt]+BLOCKSIZE*Block_num));
	}
		if(ret_HAL != HAL_OK){
			ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, ret_HAL);
			printf("EEPROM COM ANOMALY\r\n");
			return ret_HAL;
		}
	}

	return ret_HAL;												//通信結果を返す
}


/*	@brief:すべての型に対応したEEPROMの汎用read関数
 *	ReadDataBuf: 			同期用変数・配列
 *	DataSize: 				同期用変数・配列の要素数
 *	ReadAddress:			読み出し先頭アドレス(0x0000~0xAAAA)
 *	type:					型
 */
uint8_t EEPROM_read_param(void* ReadDataBuf,
						const void* ReadDataBuf_,
							size_t Size,
								uint32_t ReadAddress){

	uint8_t ret_HAL = 0;											//	通信結果確認用変数
	PARAM_MEM_AL_ACK ret_Redundancy = 0;
	uint8_t err_flag = 0;
	if(ReadAddress >= EEPROM_REDUNDANCY_SIZE || (EEPROM_REDUNDANCY_SIZE - ReadAddress) < Size){	//0x0000AAAA - WriteAddress + 1
//		AL_add_anomaly(AL_GROUP_EEPROM_PARAM, MEMORY_PARAM_ILLEGAL_ADDRESS);
		printf("EEPROM  ADDRESS ANOMALY\r\n");
		return 4;
	}

	if(ReadDataBuf_ != NULL) memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー

	size_t ReadBytes = Size;						//	型サイズの確認
	//	データをBLOCKSIZEで分割	//
	uint16_t Block_rest = ReadBytes%BLOCKSIZE;					    //	Blockで分割したときの余り[Bytes]
	uint16_t Block_num = (ReadBytes - Block_rest)/BLOCKSIZE;		//	Block数
	uint8_t ReadData_Redundancy[3][BLOCKSIZE];                      //  Blockの長さ分だけ確保
	uint8_t* ReadBuf = (uint8_t*)ReadDataBuf;


	//	三十冗長用アドレス　セット	//
	uint32_t Address[3] = {0,0,0};    							    //	三十冗長用アドレス　配列
	for(int i = 0; i < 3; i++){
		Address[i] = ReadAddress + EEPROM_REDUNDANCY_SIZE*i;					    //通常の使用可能領域は0x000000~0x00AAAA;(残りは冗長用)
	}


	//BLOCKSIZEで分割して多数決
	ret_HAL = 0;
	for(int i = 0; i < Block_num; i++){	//ブロック分
		//三十冗長  //
		for(int cnt = 0; cnt < 3; cnt++){
			ret_HAL = EEPROM_GeneralRead((uint8_t*)&ReadData_Redundancy[cnt][0], BLOCKSIZE, Address[cnt]+BLOCKSIZE*i);
			if(ret_HAL != HAL_OK){
				ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, ret_HAL);
				if(ReadDataBuf_ != NULL)	memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
				printf("PARAMROM COM ANOMALY\r\n");
				return ret_HAL;
			}
		}
		ret_Redundancy = Redundancy(&ReadData_Redundancy[0][0], &ReadData_Redundancy[1][0], &ReadData_Redundancy[2][0], BLOCKSIZE);

		if(ret_Redundancy == MEMORY_PARAM_TWO_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH);
		if(ret_Redundancy == MEMORY_PARAM_THREE_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH);
		memcpy(ReadBuf + BLOCKSIZE * i, ReadData_Redundancy[0], BLOCKSIZE);
	}

	//あまり分の読み出し
	if(Block_rest!=0){
		for(int cnt = 0; cnt < 3; cnt++){
			ret_HAL = EEPROM_GeneralRead((uint8_t*)&ReadData_Redundancy[cnt][0], Block_rest, Address[cnt]+BLOCKSIZE*Block_num);
			if(ret_HAL != HAL_OK){
				ANOMALY_I2C_Set_Status(I2C_FAULT_EEPROM_9AXDAC, ret_HAL);
				if(ReadDataBuf_ != NULL)	memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
				printf("PARAMROM COM ANOMALY\r\n");
				return ret_HAL;
			}
		}
	}
	ret_Redundancy = Redundancy(&ReadData_Redundancy[0][0], &ReadData_Redundancy[1][0], &ReadData_Redundancy[2][0], Block_rest);

	if(ret_Redundancy == MEMORY_PARAM_TWO_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH);
	if(ret_Redundancy == MEMORY_PARAM_THREE_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH);
	memcpy(ReadBuf + BLOCKSIZE * Block_num, ReadData_Redundancy[0], Block_rest);

	if(get_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH)){
//		AL_add_anomaly(AL_GROUP_EEPROM_PARAM, MEMORY_PARAM_TWO_MISMATCH);
		EEPROM_write_param(ReadDataBuf, NULL, Size, ReadAddress);						//ROMを修正
		printf("EEPROM TWO MISMATCH\r\n");
		ret_HAL = 5;
	}

	if(get_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH)){
//		AL_add_anomaly(AL_GROUP_EEPROM_PARAM, MEMORY_PARAM_THREE_MISMATCH);
		if(ReadDataBuf_ != NULL) memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
		mem_mis_info_.mis_eeprom.address = ReadAddress;
		mem_mis_info_.mis_eeprom.size = Size;
		ret_HAL = 6;
		generate_memory_repro_tlm();
	}

	return ret_HAL;
}

/* @brief:	パラメーターROMの選択
 * packet[15]： memory_mode
 *
 */
CCP_CmdRet Cmd_MEM_mram_eeprom_mode_change(const CommonCmdPacket* packet){
	memory_mode = *CCP_get_param_head(packet);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/* @brief			汎用Write関数
 * WriteData: 		書き込む配列データ
 * Size:			SDに書き込むデータの大きさ[Bytes]
 * OffsetBlock      三十冗長にする先頭のブロックアドレス
 * WriteAddress: 	書き込み先先頭アドレス
 * type: 			型		例."int"
 */
uint8_t SD_write_param(void* WriteData,
						const void* WriteData_,
							size_t Size,
								uint32_t WriteAddress){
	uint8_t ret_HAL = 0;								//	通信結果確認用変数

	if(WriteAddress >= FLIGHT_PROGRAM_OFFSET * SD_BLOCKSIZE || WriteAddress  < SD_PARAM_OFFSET * SD_BLOCKSIZE|| (FLIGHT_PROGRAM_OFFSET * SD_BLOCKSIZE - WriteAddress) < Size){
//		AL_add_anomaly(AL_GROUP_SD_PARAM, MEMORY_PARAM_ILLEGAL_ADDRESS);
		printf("SD ADDRESS ANOMALY\r\n");
		ret_HAL = 4;
		return ret_HAL;
	}

	if(WriteData_ != NULL)memcpy(WriteData, WriteData_, Size);	//FlashからRAMにデータをコピー

	uint8_t* Payload = (uint8_t*)WriteData;

	//	三十冗長用アドレス　セット	//
	uint32_t Address[3] = {0,0,0};								//	三十冗長用アドレス　配列
	for(int i = 0; i < 3; i++){
		Address[i] = WriteAddress + Redundancy_OFFSET * i;					//通常の使用可能領域は0x000000~0x002AAAAA;(残りは冗長用)
	}
	//	三十冗長	//
	for(int cnt = 0; cnt < 3; cnt++){
		ret_HAL = sd_write_address(SD_PARAM_OFFSET, Address[cnt], Payload, Size);
		if(ret_HAL != HAL_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SD, ret_HAL);
			printf("SD COM ANOMALY\r\n");
			return ret_HAL;
		}
	}
	return ret_HAL;												//通信結果を返す
}


/*	@brief:すべての型に対応した汎用read関数
 *	ReadDataBuf: 			同期用変数・配列
 *	DataSize: 				同期用変数・配列の要素数
 *	ReadAddress:			読み出し先頭アドレス
 *	type:					型
 */
uint8_t SD_read_param(void* ReadDataBuf,
						const void* ReadDataBuf_,
							size_t Size,
								uint32_t ReadAddress){

	uint8_t ret_HAL = 0;											//	通信結果確認用変数
	PARAM_MEM_AL_ACK ret_Redundancy = 0;
	uint8_t err_flag = 0;
	if(ReadAddress >= (uint32_t)(FLIGHT_PROGRAM_OFFSET * SD_BLOCKSIZE) || ReadAddress  < SD_PARAM_OFFSET * SD_BLOCKSIZE|| (FLIGHT_PROGRAM_OFFSET * SD_BLOCKSIZE - ReadAddress) < Size){	//0x002AAAAA - WriteAddress + 1
//		AL_add_anomaly(AL_GROUP_SD_PARAM, MEMORY_PARAM_ILLEGAL_ADDRESS);
		printf("SD ADDRESS ANOMALY(%08x)(%08x)\r\n", Size, ReadAddress);
		ret_HAL = 4;
		return ret_HAL;
	}

	if(ReadDataBuf_ != NULL)
		memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー

	uint8_t ReadData_Redundancy[3][Size];
	uint8_t* ReadBuf = (uint8_t*)ReadDataBuf;

	//	三十冗長用アドレス　セット	//
	uint32_t Address[3] = {0,0,0};    							    //	三十冗長用アドレス　配列
	for(int i = 0; i < 3; i++){
		Address[i] = ReadAddress + Redundancy_OFFSET * i;					//通常の使用可能領域は0x00000000~0x002AAAAA;(残りは冗長用)
	}

	//三十冗長  //
	for(int cnt = 0; cnt < 3; cnt++){
		ret_HAL = sd_read_address(SD_PARAM_OFFSET, Address[cnt], (uint8_t*)&ReadData_Redundancy[cnt][0], Size);
		if(ret_HAL != HAL_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SD, ret_HAL);
			printf("SD COM ANOMALY\r\n");
			return ret_HAL;
		}
	}

	ret_Redundancy = Redundancy(&ReadData_Redundancy[0][0], &ReadData_Redundancy[1][0], &ReadData_Redundancy[2][0], Size);
	if(ret_Redundancy == MEMORY_PARAM_TWO_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH);
	if(ret_Redundancy == MEMORY_PARAM_THREE_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH);
	memcpy(ReadBuf, ReadData_Redundancy[0], Size);

	if(get_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH)){
//		AL_add_anomaly(AL_GROUP_SD_PARAM, MEMORY_PARAM_TWO_MISMATCH);
		SD_write_param(ReadDataBuf, NULL, Size, ReadAddress);						//ROMを修正
		ret_HAL = 5;
	}

	if(get_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH)){
//		AL_add_anomaly(AL_GROUP_SD_PARAM, MEMORY_PARAM_THREE_MISMATCH);
		if(ReadDataBuf_ != NULL)	memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
		mem_mis_info_.mis_sd.address = ReadAddress;
		mem_mis_info_.mis_sd.size = Size;
		ret_HAL = 6;
		generate_memory_repro_tlm();
	}

	return ret_HAL;
}


/* @brief			汎用Write関数
 * WriteData: 		書き込む配列データ
 * Size:			SDに書き込むデータの大きさ[Bytes]
 * OffsetBlock      三十冗長にする先頭のブロックアドレス
 * WriteAddress: 	書き込み先先頭アドレス
 * type: 			型		例."int"
 */
uint8_t SD_write_program(void* WriteData,
						const void* WriteData_,
							size_t Size,
								uint32_t WriteAddress){
	uint8_t ret_HAL = 0;								//	通信結果確認用変数

	if(WriteAddress >= Redundancy_OFFSET || WriteAddress  < FLIGHT_PROGRAM_OFFSET * SD_BLOCKSIZE|| (Redundancy_OFFSET - WriteAddress) < Size){
//		AL_add_anomaly(AL_GROUP_SD_PARAM, MEMORY_PARAM_ILLEGAL_ADDRESS);
		printf("SD ADDRESS ANOMALY %08x\r\n", WriteAddress);
		ret_HAL = 4;
		return ret_HAL;
	}

	if(WriteData_ != NULL)memcpy(WriteData, WriteData_, Size);	//FlashからRAMにデータをコピー

	uint8_t* Payload = (uint8_t*)WriteData;

	//	三十冗長用アドレス　セット	//
	uint32_t Address[3] = {0,0,0};								//	三十冗長用アドレス　配列
	for(int i = 0; i < 3; i++){
		Address[i] = WriteAddress + Redundancy_OFFSET * i;					//通常の使用可能領域は0x000000~0x002AAAAA;(残りは冗長用)
	}
	//	三十冗長	//
	for(int cnt = 0; cnt < 3; cnt++){
		ret_HAL = sd_write_address(SD_PARAM_OFFSET, Address[cnt], Payload, Size);
		if(ret_HAL != HAL_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SD, ret_HAL);
			printf("SD COM ANOMALY\r\n");
			return ret_HAL;
		}
	}
	return ret_HAL;												//通信結果を返す
}




/*	@brief:すべての型に対応した汎用read関数
 *	ReadDataBuf: 			同期用変数・配列
 *	DataSize: 				同期用変数・配列の要素数
 *	ReadAddress:			読み出し先頭アドレス
 *	type:					型
 */
uint8_t SD_read_program(void* ReadDataBuf,
						const void* ReadDataBuf_,
							size_t Size,
								uint32_t ReadAddress){

	uint8_t ret_HAL = 0;											//	通信結果確認用変数
	PARAM_MEM_AL_ACK ret_Redundancy = 0;
	uint8_t err_flag = 0;
	if(ReadAddress >= Redundancy_OFFSET || ReadAddress  < FLIGHT_PROGRAM_OFFSET * SD_BLOCKSIZE|| (Redundancy_OFFSET - ReadAddress) < Size){	//0x002AAAAA - WriteAddress + 1
//		AL_add_anomaly(AL_GROUP_SD_PARAM, MEMORY_PARAM_ILLEGAL_ADDRESS);
		printf("SD ADDRESS ANOMALY(%08x)(%08x)\r\n", Size, ReadAddress);
		ret_HAL = 4;
		return ret_HAL;
	}

	if(ReadDataBuf_ != NULL)
		memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー

	uint8_t ReadData_Redundancy[3][Size];
	uint8_t* ReadBuf = (uint8_t*)ReadDataBuf;

	//	三十冗長用アドレス　セット	//
	uint32_t Address[3] = {0,0,0};    							    //	三十冗長用アドレス　配列
	for(int i = 0; i < 3; i++){
		Address[i] = ReadAddress + Redundancy_OFFSET * i;					//通常の使用可能領域は0x00000000~0x002AAAAA;(残りは冗長用)
	}

	//三十冗長  //
	for(int cnt = 0; cnt < 3; cnt++){
		ret_HAL = sd_read_address(SD_PARAM_OFFSET, Address[cnt], (uint8_t*)&ReadData_Redundancy[cnt][0], Size);
		if(ret_HAL != HAL_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SD, ret_HAL);
			printf("SD COM ANOMALY\r\n");
			return ret_HAL;
		}
	}

	ret_Redundancy = Redundancy(&ReadData_Redundancy[0][0], &ReadData_Redundancy[1][0], &ReadData_Redundancy[2][0], Size);
	if(ret_Redundancy == MEMORY_PARAM_TWO_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH);
	if(ret_Redundancy == MEMORY_PARAM_THREE_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH);
	memcpy(ReadBuf, ReadData_Redundancy[0], Size);

	if(get_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH)){
//		AL_add_anomaly(AL_GROUP_SD_PARAM, MEMORY_PARAM_TWO_MISMATCH);
		SD_write_program(ReadDataBuf, NULL, Size, ReadAddress);						//ROMを修正
		printf("SD_TWO_MISMATCH\r\n");
		ret_HAL = 5;
	}

	if(get_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH)){
//		AL_add_anomaly(AL_GROUP_SD_PARAM, MEMORY_PARAM_THREE_MISMATCH);
		if(ReadDataBuf_ != NULL)	memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
		mem_mis_info_.mis_sd.address = ReadAddress;
		mem_mis_info_.mis_sd.size = Size;
		ret_HAL = 6;
		generate_memory_repro_tlm();
	}

	return ret_HAL;
}


/* @brief			汎用Write関数
 * WriteData: 		書き込む配列データ(セクター跨ぎは禁止)
 * Size:			PROMに書き込むデータの大きさ[Bytes]
 * OffsetBlock      三十冗長にする先頭のブロックアドレス
 * WriteAddress: 	書き込み先先頭アドレス
 * type: 			型		例."int"
 */
uint8_t SPIPROM_write_param(void* WriteData,
								const void* WriteData_,
									size_t Size,
										uint32_t WriteAddress){
	uint8_t ret_HAL;

	if(WriteAddress >= PROM_FLIGHT_PROGRAM_OFFSET || WriteAddress  < PROM_PARAM_OFFSET||
			WriteAddress % is25lp.SectorSize + Size > is25lp.SectorSize || (PROM_FLIGHT_PROGRAM_OFFSET - WriteAddress) < Size){
//		AL_add_anomaly(AL_GROUP_SPIPROM_PARAM, MEMORY_PARAM_ILLEGAL_ADDRESS);
		printf("SPIPROM ADDRESS ANOMALY %08x\r\n", WriteAddress);
		ret_HAL = 4;
		return ret_HAL;
	}

	if(WriteData_ != NULL)memcpy(WriteData, WriteData_, Size);	//FlashからRAMにデータをコピー

	//	三十冗長	//
	//  PROMはセクターを一度消去するためフラグだけ立てる
	write_operation_flag(PARAM_WRITE);
	spiprom_driver->pre_redundancy_step = 0;
	spiprom_driver->redundancy_step = 0;
	spiprom_driver->datasize = Size;
	memset(spiprom_driver->buffer, 0, sizeof(spiprom_driver->buffer));
	memcpy(&(spiprom_driver->param_buffer[0]), (uint8_t*)WriteData, Size);
	spiprom_driver->redundacy_address = WriteAddress;

	return ret_HAL;												//通信結果を返す
}

/*	@brief:すべての型に対応した汎用read関数
 *	ReadDataBuf: 			同期用変数・配列
 *	DataSize: 				同期用変数・配列の要素数
 *	ReadAddress:			読み出し先頭アドレス
 *	type:					型
 */
uint8_t SPIPROM_read_param(void* ReadDataBuf,
								const void* ReadDataBuf_,
									size_t Size,
										uint32_t ReadAddress){

	uint8_t ret_HAL = 0;											//	通信結果確認用変数
	PARAM_MEM_AL_ACK ret_Redundancy = 0;
	uint8_t err_flag = 0;
	if(ReadAddress >= PROM_FLIGHT_PROGRAM_OFFSET || ReadAddress  < PROM_PARAM_OFFSET||
			ReadAddress % is25lp.SectorSize + Size > is25lp.SectorSize || (PROM_FLIGHT_PROGRAM_OFFSET - ReadAddress) < Size){	//0x002AAAAA - WriteAddress + 1
//		AL_add_anomaly(AL_GROUP_SPIPROM_PARAM, MEMORY_PARAM_ILLEGAL_ADDRESS);
		printf("SPIPROM ADDRESS ANOMALY(%08x)(%08x)\r\n", Size, ReadAddress);
		ret_HAL = 4;
		return ret_HAL;
	}

	if(ReadDataBuf_ != NULL)
		memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー

	uint8_t ReadData_Redundancy[3][Size];
	uint8_t* ReadBuf = (uint8_t*)ReadDataBuf;

	//	三十冗長用アドレス　セット	//
	uint32_t Address[3] = {0,0,0};    							    //	三十冗長用アドレス　配列
	for(int i = 0; i < 3; i++){
		Address[i] = ReadAddress + PROM_Redundancy_OFFSET * i;					//通常の使用可能領域は0x00000000~0x002AAAAA;(残りは冗長用)
	}

	//三十冗長  //
	for(int cnt = 0; cnt < 3; cnt++){
		ret_HAL = readData((uint8_t*)&ReadData_Redundancy[cnt][0], Address[cnt], Size);
		if(ret_HAL != HAL_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SPIPROM, ret_HAL);
			printf("SPIPROM COM ANOMALY\r\n");
			return ret_HAL;
		}
	}

	ret_Redundancy = Redundancy(&ReadData_Redundancy[0][0], &ReadData_Redundancy[1][0], &ReadData_Redundancy[2][0], Size);
	if(ret_Redundancy == MEMORY_PARAM_TWO_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH);
	if(ret_Redundancy == MEMORY_PARAM_THREE_MISMATCH) set_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH);
	memcpy(ReadBuf, ReadData_Redundancy[0], Size);

	if(get_ah_flag(&err_flag, FLAG_PARAMROM_TWO_MISMATCH)){
//		AL_add_anomaly(AL_GROUP_SPIPROM_PARAM, MEMORY_PARAM_TWO_MISMATCH);
		SPIPROM_write_param(ReadDataBuf, NULL, Size, ReadAddress); 	//ROMを修正
		ret_HAL = 5;
	}

	if(get_ah_flag(&err_flag, FLAG_PARAMROM_THREE_MISMATCH)){
//		AL_add_anomaly(AL_GROUP_SPIPROM_PARAM, MEMORY_PARAM_THREE_MISMATCH);
		if(ReadDataBuf_ != NULL)	memcpy(ReadDataBuf, ReadDataBuf_, Size);	//FlashからRAMにデータをコピー
		mem_mis_info_.mis_spiprom.address = ReadAddress;
		mem_mis_info_.mis_spiprom.size = Size;
		printf("SPIPROM_THREE_MISMATCH\r\n");
		ret_HAL = 6;
		generate_memory_repro_tlm();
	}
	return ret_HAL;
}


void generate_memory_repro_tlm(void)
{
  static TCP tcp_;
  uint16_t len;
  TF_TLM_FUNC_ACK ack;

  ack = TF_generate_contents((int)Tlm_CODE_MEM_PATROL_REPRO,
			     tcp_.packet,
			     &len,
			     TCP_MAX_LEN);

  // TCPacketヘッダ設定
  TCP_TLM_setup_primary_hdr(&tcp_, AFSK_GS, len);
  TCP_set_route(&tcp_, TCP_ROUTE_DIRECT);
  TCP_set_all_issuer_id(&tcp_, MY_SAT_ID, TCP_EXE_ID_MOBC);
  TCP_TLM_set_startup_count(&tcp_, (uint8_t)p_reset_struct->reset_count[TOTAL_RESET]);
  TCP_TLM_set_ti(&tcp_, (uint32_t)(TMGR_get_master_total_cycle()));
  TCP_TLM_set_category(&tcp_, 0x80); // パラメータによる指定
  TCP_TLM_set_packet_id(&tcp_, Tlm_CODE_MEM_PATROL_REPRO);
  PH_analyze_packet(&tcp_);
}
