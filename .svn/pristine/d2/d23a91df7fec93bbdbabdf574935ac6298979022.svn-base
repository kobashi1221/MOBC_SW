/*
 * SPI_ROM.c
 *
 *  Created on: 2019/05/30
 *      Author: K.Okada
 */

#include "../../CDH/SPIPROM/SPI_ROM.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <src_core/Library/endian.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_user/Library/General/DBG_printf.h>

bool initialized;

static SPIPROM_STRUCT spiprom_driver_;
SPIPROM_STRUCT* const spiprom_driver = &spiprom_driver_;

uint8_t  SPIROM_INIT() {
	spiprom_driver->num_erasesector = 0;
	spiprom_driver->erase_flag = 0;
	spiprom_driver->operation_address= 0;
	spiprom_driver->condition_copy_flag = 0;
	spiprom_driver->pre_redundancy_step = 0;
	spiprom_driver->redundancy_step = 0;
	spiprom_driver->update_step = 0;
	spiprom_driver->operation_flag = NOMINAL;
	return init();
}

uint8_t init() {
	if (initialized)
		return true;
	initialized = IS25LP_Init();
	if (!initialized) {
		IS25LP_Reset();
		initialized = IS25LP_Init();
	}
	return initialized;
}


//EX: address = 2000, size = 12000
//               0      4096    8192   12288   16384   20480
// ROM   address |-------|-------|-------|-------|-------|
// write address     |------------------------|
// 1st   sector      |---|
// middle sectors        |---------------|
// last sector                           |----|
/**
 * @brief  read old data -> join with the new data -> write to the ROM
 * @param  pBuffer 	buffer for write data
 * @param  address 	0x000000 - 0xffffff
 * @param  size		write size
 * @return uint8_t
 **/
SPIPROM_ACK writeData(uint8_t *pBuffer, uint32_t address, uint32_t size) {
	if (address + size > is25lp.SectorCount * is25lp.SectorSize)
		return SPIPROM_FAILED;

	uint32_t firstSector = address / is25lp.SectorSize;
	//揮発性宣言
	volatile uint32_t firstSize = (firstSector + 1) * is25lp.SectorSize - address;
	if (address + size < (firstSector + 1) * is25lp.SectorSize) {
		firstSize = size;
	}

	//write 1st sector
	writeMinSector(pBuffer, address, firstSize);
	pBuffer += firstSize;

	//1sectorのみの書き込み
	if (firstSize == size)
		return true;
	//write middle sectors
	int loopCount = (size - firstSize) / is25lp.SectorSize;
	uint32_t iSector = firstSector + 1;
	for (int i = 0; i < loopCount; i++) {
		writeOneSector(pBuffer, iSector);
		pBuffer += is25lp.SectorSize;
		iSector++;
	}

	//write last sector
	uint32_t lastWriteAddress = iSector * is25lp.SectorSize;
	if (lastWriteAddress >= address + size)
		return true;
	writeMinSector(pBuffer, lastWriteAddress, (address + size - 1) - lastWriteAddress);
	return SPIPROM_SUCCESS;
}

/**
 * @brief  read from the ROM
 * @param  pBuffer	buffer for read data
 * @param  address	start address 0x000000 - 0xffffff
 * @param  size		~65535 or less
 * @return SPIPROM_ACK
 **/
SPIPROM_ACK readData(uint8_t* buffer, uint32_t address, uint16_t size) {
	if (address + size > is25lp.SectorCount * is25lp.SectorSize)
		return SPIPROM_FAILED;

	IS25LP_ReadBytes(buffer, address, size);
	return SPIPROM_SUCCESS;
}

/**
 * @brief  read from the ROM
 * @param  pBuffer	buffer for read data
 * @param  address	start sector address 0x000000 - 0x00ffff
 * @return SPIPROM_ACK
 **/
SPIPROM_ACK readPageData(uint8_t* buffer, uint32_t page_address) {
	if (page_address * is25lp.PageSize  + is25lp.PageSize > is25lp.SectorCount * is25lp.SectorSize)
		return SPIPROM_FAILED;

	uint32_t address = page_address * is25lp.PageSize;

	IS25LP_ReadBytes(buffer, address, is25lp.PageSize);
	return SPIPROM_SUCCESS;
}

/**
 * @brief  read from the ROM
 * @param  pBuffer	buffer for read data
 * @param  address	start address 0x000000 - 0x000ffff
 * @return SPIPROM_ACK
 **/
SPIPROM_ACK readSectorData(uint8_t* buffer, uint32_t sector_address) {
	if (sector_address * is25lp.SectorSize  + is25lp.SectorSize > is25lp.SectorCount * is25lp.SectorSize)
		return SPIPROM_FAILED;

	uint32_t address = sector_address * is25lp.SectorSize;

	IS25LP_ReadBytes(buffer, address, is25lp.SectorSize);
	return SPIPROM_SUCCESS;
}

/**
 * @brief 書き込み用汎用関数　上書きする場合はセクター消去のフローに回す セクター跨ぎは禁止
 * @param uint8_t* pBuffer 書き込む先頭ポインタ
 * @param uint32_t addr 書き込み先アドレス
 * @param uint32_t size サイズ(size <= 4096)
 * @return SPIPROM_ACK
 */
SPIPROM_ACK spiprom_write_data(uint8_t *pBuffer,uint32_t address, uint32_t size){
	if (address + size > is25lp.SectorSize * is25lp.SectorCount){
		return SPIPROM_FAILED;
	}
	if (address % is25lp.SectorSize + size > is25lp.SectorSize){
		return SPIPROM_FAILED;
	}

	uint32_t iSector = address / is25lp.SectorSize;
	count_erasesector(iSector, 1, address, size);//0か1

	if(spiprom_driver->num_erasesector == 0){//セクターを消す必要がないならそのまま書き込み
    	print(SPIPROM,"direct write \r\n");
    	writeData(pBuffer, address, size);
		check_flag(&spiprom_driver->erase_flag, spiprom_driver->num_erasesector);
	}
	else if(spiprom_driver->num_erasesector == 1){//消す必要があるなら消してセクターごと書き換える
		int offset = address - iSector * is25lp.SectorSize;
		uint32_t read1stIndex = (uint32_t) (address / is25lp.SectorSize) * is25lp.SectorSize;
		prom_flag_set(&spiprom_driver->erase_flag);

		//read -> erase -> update the sector -> write new one
		IS25LP_ReadBytes(spiprom_driver->buffer, read1stIndex, is25lp.SectorSize);
		memcpy(&(spiprom_driver->buffer[offset]), pBuffer, size);

/////////////////////// for debug ////////////////////////
//		uint32_t read_Address = address / is25lp.PageSize;
//		uint8_t buffer[256];
//
//		readPageData(buffer, read_Address);
//
//		for(int i = 0; i<256; i++){
//			printf("%02x", buffer[i]);
//		}
//		printf("\r\n");
//////////////////////////////////////////////////////////
		eraseSector(iSector);
		spiprom_driver_.operation_address = address;
		spiprom_driver_.datasize = size;
		write_operation_flag(OVER_WRITE);

	}
	else{//2つ以上のセクターを跨ぐ時はブロック
		spiprom_driver->num_erasesector = 0;
	}
	return SPIPROM_SUCCESS;
}

/**
 * @brief 三重冗長用データ書き込み関数
 * 		 　 上書き時は部分的上書きでなく, セクターごと上書き
 * @param uint8_t* pBuffer 書き込む先頭ポインタ
 * @param uint32_t addr 書き込み先アドレス
 * @param uint32_t size サイズ(size <= 4096)
 * @return SPIPROM_ACK
 */
SPIPROM_ACK spiprom_write_paramdata(uint8_t *pBuffer,uint32_t address, uint32_t size){
	if (address + size > is25lp.SectorSize * is25lp.SectorCount){
		return SPIPROM_FAILED;
	}

	if (address % is25lp.SectorSize + size > is25lp.SectorSize){
		return SPIPROM_FAILED;
	}
	uint32_t iSector = address / is25lp.SectorSize;
	count_erasesector(iSector, 1, address, size);//0か1

	if(spiprom_driver->num_erasesector == 0){//セクターを消す必要がないならそのまま書き込み
    	print(SPIPROM,"direct write \r\n");
    	writeData(pBuffer, address, size);
		check_flag(&spiprom_driver->erase_flag, spiprom_driver->num_erasesector);
		++spiprom_driver->pre_redundancy_step;
		++spiprom_driver->redundancy_step;
		spiprom_driver->redundacy_address += PROM_Redundancy_OFFSET;
	}
	else if(spiprom_driver->num_erasesector == 1){//消す必要があるなら消してセクターごと書き換える
		int offset = address - iSector * is25lp.SectorSize;
		uint32_t read1stIndex = (uint32_t) (address / is25lp.SectorSize) * is25lp.SectorSize;
		prom_flag_set(&spiprom_driver->erase_flag);

		//read -> erase -> update the sector -> write new data
		IS25LP_ReadBytes(spiprom_driver->buffer, read1stIndex, is25lp.SectorSize);
		memcpy(&(spiprom_driver->buffer[offset]), pBuffer, size);

		eraseSector(iSector);
		write_operation_flag(PARAM_WRITE);
		++spiprom_driver->pre_redundancy_step;
	}
	else{//2つ以上のセクターを跨ぐ時はブロック
		spiprom_driver->num_erasesector = 0;
	}

	return SPIPROM_SUCCESS;
}
/**
 * @brief 上書き用の関数　指定したアドレスにバッファにある値を書き込む
 * @param uint32_t addr 書き込み先アドレス
 */
SPIPROM_ACK spiprom_overwrite_data(uint32_t addr){
	if (addr > is25lp.SectorCount * is25lp.SectorSize){
		return SPIPROM_FAILED;
	}
	int flag;
	flag = (int)get_operation_flag(spiprom_driver);
	size_t before_1 = TMGR_get_master_step();
	uint32_t Sector = addr / is25lp.SectorSize;
	//この関数にたどり着くころにはセクター内は消去できているはず
	if(flag == OVER_WRITE){//テレメトリを上書きする場合はデータサイズが必要
		/////////////////////// for debug ////////////////////////
//		uint32_t read_Address = addr / is25lp.PageSize;
//		uint8_t buffer[256];
//
//		readPageData(buffer, read_Address);
//
//		for(int i = 0; i<256; i++){
//			printf("%02x", buffer[i]);
//		}
//		printf("\r\n");
		//////////////////////////////////////////////////////////
    	writeData(&(spiprom_driver_.buffer[0]), addr, spiprom_driver_.datasize);
		if(spiprom_driver->condition_flag == 1){//overwriteしてフラグが書きかわったとき(冗長)
			prom_flag_reset(&spiprom_driver->condition_flag);
			write_operation_flag(CONDITION_REWRITE);
		}
		else{
			write_operation_flag(NOMINAL);
		}
		/////////////////////// for debug ////////////////////////
//		readPageData(buffer, read_Address);
//
//		for(int i = 0; i<256; i++){
//			printf("%02x", buffer[i]);
//		}
//		printf("\r\n");
		//////////////////////////////////////////////////////////
		print(SPIPROM, "tlm overwrite\r\n");
	}
	else if(flag == PARAM_WRITE){
		writeOneSector(&(spiprom_driver_.buffer[0]), Sector);
		print(SPIPROM, "param overwrite\r\n");
		++spiprom_driver->redundancy_step;
		spiprom_driver->redundacy_address += PROM_Redundancy_OFFSET;
	}
	else{
		writeOneSector(&(spiprom_driver_.buffer[0]), Sector);
		print(SPIPROM, "other data overwrite\r\n");
	}
	step_t after_1 = TMGR_get_master_step();
	print(SPIPROM,"check overwrite = %d \r\n", after_1 - before_1);
	prom_flag_reset(&spiprom_driver->erase_flag);
	return SPIPROM_SUCCESS;
}

/**
 * @brief SPIPROMの指定したアドレスから指定したアドレスに値をコピー(セクター跨ぎは禁止)
 * @param uint32_t addr_dst コピー先の先頭アドレス
 * @param uint32_t addr_src コピー元の先頭アドレス
 * @param uint16_t size 何byteコピーするか(size <= 256)
 *
 */
SPIPROM_ACK spiprom_copy_data(uint32_t addr_dst,uint32_t addr_src, uint32_t size){
	if (addr_dst > is25lp.SectorCount * is25lp.SectorSize || addr_src > is25lp.SectorCount * is25lp.SectorSize){
		return SPIPROM_FAILED;
	}

	uint8_t ret;
	uint32_t iSector = addr_dst / is25lp.SectorSize;
	uint8_t receive_data[0x1000];

	ret = IS25LP_ReadBytes(receive_data, addr_src, size);

	count_erasesector(iSector, 1, addr_dst, size);//0か1

	if(spiprom_driver->num_erasesector == 0){//セクターを消す必要がないならそのまま書き込み
    	print(SPIPROM,"direct write \r\n");
    	writeData(receive_data, addr_dst, size);
	}
	else{//消す必要があるなら消してセクターごと書き換える
		int offset = addr_dst - iSector * is25lp.SectorSize;
		uint32_t read1stIndex = (uint32_t) (addr_dst / is25lp.SectorSize) * is25lp.SectorSize;

		//read -> erase -> update the sector -> write new one
		IS25LP_ReadBytes(spiprom_driver->buffer, read1stIndex, is25lp.SectorSize);
		memcpy(&(spiprom_driver->buffer[offset]), receive_data, size);

		prom_flag_set(&spiprom_driver->erase_flag);
		eraseSector(iSector);

		spiprom_driver->operation_address = addr_src;
		write_operation_flag(COPY_WRITE);
	}
	if(ret != true){
		return SPIPROM_FAILED;
	}
	return SPIPROM_SUCCESS;
}

/**
 * @brief　TLMを書き込むセクターアドレスが範囲内にあるか, なければ初期位置に戻す
 *
 */
void spiprom_sector_analyze(SPIPROM_SAVE_Info* p_prom_struct){
	if((uint32_t)p_prom_struct->SECTOR_NOW < (uint32_t)p_prom_struct->SECTOR_START ||
			(uint32_t)p_prom_struct->SECTOR_NOW > (uint32_t)p_prom_struct->SECTOR_END){

		//現在のセクターアドレスが範囲内にないときは初期位置に戻す
		p_prom_struct->SECTOR_NOW = p_prom_struct->SECTOR_START;
	}
	if((uint32_t)p_prom_struct->SECTOR_OLD < (uint32_t)p_prom_struct->SECTOR_START ||
			(uint32_t)p_prom_struct->SECTOR_OLD > (uint32_t)p_prom_struct->SECTOR_END){

		//現在のセクターアドレスが範囲内にないときは初期位置に戻す
		p_prom_struct->SECTOR_OLD = p_prom_struct->SECTOR_START;
	}
}

SPIPROM_ACK spiprom_write_buffer(uint32_t sector_address){
	if (sector_address > is25lp.SectorCount){
		SPIROM_INIT();
		return SPIPROM_FAILED;
	}
	writeOneSector(spiprom_driver->buffer, sector_address);
	prom_flag_reset((int*)spiprom_driver->erase_flag);
	write_operation_flag(NOMINAL);
	return SPIPROM_SUCCESS;
}

//////////////////////////////////////以下SPIPROM用のローカル関数
//-----------------------------------------------------
// write data within one sector.
// "size" must be less than sector size (4096 == 4kbyte)
//-----------------------------------------------------
void writeMinSector(uint8_t *pBuffer, uint32_t address, uint32_t size) {
	int sectorIndex = address / is25lp.SectorSize;
	int offset = address - sectorIndex * is25lp.SectorSize;
//	uint32_t read1stIndex = (uint32_t) (address / is25lp.SectorSize) * is25lp.SectorSize;

//	past 前に消して保存するならここでわざわざ保存しなくてもいいはず
//	IS25LP_ReadBytes(spiprom_driver_.buffer, read1stIndex, is25lp.SectorSize);
//	memcpy(&(spiprom_driver_.buffer[offset]), pBuffer, size);
//	IS25LP_WriteSector(spiprom_driver_.buffer, sectorIndex, 0, is25lp.SectorSize);
	IS25LP_WriteSector(pBuffer, sectorIndex, offset, size);
}

//-----------------------------------------------------
// write data to a sector.
// the size of pBuffer is always 4096
//-----------------------------------------------------
void writeOneSector(uint8_t *pBuffer, uint32_t sector) {
	IS25LP_WriteSector(pBuffer, sector, 0, is25lp.SectorSize);
}


void eraseSector(uint32_t first_erasesector){
	IS25LP_EraseSector_C2A(first_erasesector);
	--spiprom_driver->num_erasesector;
	prom_flag_set(&spiprom_driver->erase_flag);

	print(SPIPROM,"erase sector num is %d \r\n", spiprom_driver->num_erasesector);
}

void count_erasesector(uint32_t firstsector, uint32_t numsector, uint32_t write_address, uint32_t write_size){
	int first_sector_offset = write_address - firstsector * is25lp.SectorSize;
	uint32_t last_sector_size = write_address + write_size - ((firstsector + numsector - 1) * is25lp.SectorSize);
	for (int isector = firstsector ; isector < numsector + firstsector; isector++){
		if(isector == firstsector ){
			if (!IS25LP_IsEmptyBytes(isector, first_sector_offset, write_size)){
				++spiprom_driver->num_erasesector;
				print(SPIPROM,"num_erasesector is %x \r\n", spiprom_driver->num_erasesector);
			}
		}
		else if(isector > firstsector && isector < numsector - 1){
			if (!IS25LP_IsEmptyBytes(isector, 0, is25lp.SectorSize)){
				++spiprom_driver->num_erasesector;
				print(SPIPROM,"num_erasesector is %x \r\n", spiprom_driver->num_erasesector);
			}
		}
		else{
			if (!IS25LP_IsEmptyBytes(isector, 0, last_sector_size)){
				++spiprom_driver->num_erasesector;
				print(SPIPROM,"num_erasesector is %x \r\n", spiprom_driver->num_erasesector);
			}
		}
	}
}

void choose_erase_or_direct(uint32_t isector, uint32_t write_address, uint32_t write_size){
	int sector_offset = write_address - isector * is25lp.SectorSize;
	if (!IS25LP_IsEmptyBytes(isector, sector_offset, write_size)){
		prom_flag_set(&spiprom_driver->erase_flag);
		print(SPIPROM,"num_erasesector is %x \r\n", spiprom_driver->num_erasesector);
	}
}

////////////////////////// 各フラグのチェック関数 ///////////////////////////
SPIPROM_ACK condition_reset_flag_check(const SPIPROM_STRUCT* spiprom, int* ret){
	int flag, res;
	flag = (int)get_operation_flag(spiprom);
	if(spiprom_driver->erase_flag == FLAG_OFF){
		switch(flag){
			case CONDITION_RESET:
				res = 0;
				break;
			default:
				res = 1;
				break;
		}
		if(res == 0){
			return *ret = SPIPROM_SUCCESS;
		}
		else{
			return *ret = SPIPROM_FAILED;
		}
	}
	else{
		return *ret = SPIPROM_FAILED;
	}
}

SPIPROM_ACK condition_rewrite_flag_check(const SPIPROM_STRUCT* spiprom, int* ret){
	int flag, res;
	flag = (int)get_operation_flag(spiprom);
	if(spiprom_driver->erase_flag == FLAG_OFF){
		switch(flag){
			case CONDITION_REWRITE:
				res = 0;
				break;
			default:
				res = 1;
				break;
		}
		if(res == 0){
			return *ret = SPIPROM_SUCCESS;
		}
		else{
			return *ret = SPIPROM_FAILED;
		}
	}
	else{
		return *ret = SPIPROM_FAILED;
	}
}

SPIPROM_ACK condition_update_flag_check(const SPIPROM_STRUCT* spiprom, int* ret){
	int flag, res;
	flag = (int)get_operation_flag(spiprom);
	if(spiprom_driver->erase_flag == FLAG_OFF){
		switch(flag){
			case CONDITION_UPDATE:
				res = 0;
				break;
			default:
				res = 1;
				break;
		}
		if(res == 0){
			return *ret = SPIPROM_SUCCESS;
		}
		else{
			return *ret = SPIPROM_FAILED;
		}
	}
	else{
		return *ret = SPIPROM_FAILED;
	}
}

SPIPROM_ACK tlm_flag_check(const SPIPROM_STRUCT* spiprom, int* ret){
	int flag ,res;
	flag = (int)get_operation_flag(spiprom);
	if(spiprom_driver->erase_flag == FLAG_OFF){
		switch(flag){
			case NOMINAL:
			case TLM_WRITE:
				res = 0;
				break;
			default:
				res = 1;
				break;
		}
		if(res == 0){
			return *ret = SPIPROM_SUCCESS;
		}
		else{
			return *ret = SPIPROM_FAILED;
		}
	}
	else{
		return *ret = SPIPROM_FAILED;
	}
}

SPIPROM_ACK over_write_flag_check(const SPIPROM_STRUCT* spiprom, int* ret){
	int flag ,res;
	flag = (int)get_operation_flag(spiprom);
	if(spiprom_driver->erase_flag == FLAG_OFF){
		switch(flag){
			case OVER_WRITE:
				res = 0;
				break;
			default:
				res = 1;
				break;
		}
		if(res == 0){
			return *ret = SPIPROM_SUCCESS;
		}
		else{
			return *ret = SPIPROM_FAILED;
		}
	}
	else{
		return *ret = SPIPROM_FAILED;
	}
}

SPIPROM_ACK param_write_flag_check(const SPIPROM_STRUCT* spiprom, int* ret){
	int flag ,res;
	flag = (int)get_operation_flag(spiprom);
	if(spiprom_driver->erase_flag == FLAG_OFF){
		switch(flag){
			case PARAM_WRITE:
				res = 0;
				break;
			default:
				res = 1;
				break;
		}
		if(res == 0){
			return *ret = SPIPROM_SUCCESS;
		}
		else{
			return *ret = SPIPROM_FAILED;
		}
	}
	else{
		return *ret = SPIPROM_FAILED;
	}
}

SPIPROM_ACK copy_flag_check(const SPIPROM_STRUCT* spiprom, int* ret){
	int flag, res;
	flag = (int)get_operation_flag(spiprom);
	if(spiprom_driver->erase_flag == FLAG_OFF){
		switch(flag){
			case COPY_WRITE:
				res = 0;
				break;
			default:
				res = 1;
				break;
		}
		if(res == 0){
			return *ret = SPIPROM_SUCCESS;
		}
		else{
			return *ret = SPIPROM_FAILED;
		}
	}
	else{
		return *ret = SPIPROM_FAILED;
	}
}

SPIPROM_ACK erace_sectors_flag_check(const SPIPROM_STRUCT* spiprom, int* ret){
	int flag, res;
	flag = (int)get_operation_flag(spiprom);
	if(spiprom_driver->erase_flag == FLAG_OFF){
		switch(flag){
			case ELASE_SECTOR:
				res = 0;
				break;
			default:
				res = 1;
				break;
		}
		if(res == 0){
			return *ret = SPIPROM_SUCCESS;
		}
		else{
			return *ret = SPIPROM_FAILED;
		}
	}
	else{
		return *ret = SPIPROM_FAILED;
	}
}

SPIPROM_ACK all_reset_flag_check(const SPIPROM_STRUCT* spiprom, int* ret){
	int flag, res;
	flag = (int)get_operation_flag(spiprom);
	if(spiprom_driver->erase_flag == FLAG_OFF){
		switch(flag){
			case ALL_RESET:
				res = 0;
				break;
			default:
				res = 1;
				break;
		}
		if(res == 0){
			return *ret = SPIPROM_SUCCESS;
		}
		else{
			return *ret = SPIPROM_FAILED;
		}
	}
	else{
		return *ret = SPIPROM_FAILED;
	}
}

SPIPROM_ACK enque_flag_check(const SPIPROM_STRUCT* spiprom, int* ret){
	int flag, res;
	flag = (int)get_operation_flag(spiprom);
	if(spiprom_driver->erase_flag == FLAG_OFF){
		switch(flag){
			case CONDITION_RESET:
			case CONDITION_REWRITE:
			case CONDITION_UPDATE:
			case PARAM_WRITE:
			case OVER_WRITE:
			case COPY_WRITE:
			case ELASE_SECTOR:
				res = 0;
				break;
			default:
				res = 1;
				break;
		}
		if(res == 0){
			return *ret = SPIPROM_SUCCESS;
		}
		else{
			return *ret = SPIPROM_FAILED;
		}
	}
	else{//flagがONならenque
		return *ret = SPIPROM_SUCCESS;
	}
}

int get_operation_flag(const SPIPROM_STRUCT* spiprom){
	int ret ;
	return ret = spiprom->operation_flag;
}

void write_operation_flag(SPIPROM_OPERATION ret){
	switch(ret){
		case NOMINAL:
			spiprom_driver->operation_flag = NOMINAL;
			break;
		case CONDITION_RESET:
			spiprom_driver->operation_flag = CONDITION_RESET;
			break;
		case CONDITION_REWRITE:
			spiprom_driver->operation_flag = CONDITION_REWRITE;
			break;
		case CONDITION_UPDATE:
			spiprom_driver->operation_flag = CONDITION_UPDATE;
			break;
		case TLM_WRITE:
			spiprom_driver->operation_flag = TLM_WRITE;
			break;
		case PARAM_WRITE:
			spiprom_driver->operation_flag = PARAM_WRITE;
			break;
		case OVER_WRITE:
			spiprom_driver->operation_flag = OVER_WRITE;
			break;
		case COPY_WRITE:
			spiprom_driver->operation_flag = COPY_WRITE;
			break;
		case ELASE_SECTOR:
			spiprom_driver->operation_flag = ELASE_SECTOR;
			break;
		case ALL_RESET:
			spiprom_driver->operation_flag = ALL_RESET;
			break;
		default:
			break;
	}
}

//////////////////////// フラグチェック関数 ///////////////////////////
void check_flag(int* flag, int check_content){
	if(check_content != 0){
		prom_flag_set(flag);
	}
	else if(check_content == 0){
		prom_flag_reset(flag);
	}
}

void prom_flag_set(int* flag){
	*flag = FLAG_ON;
}

void prom_flag_reset(int* flag){
	*flag = FLAG_OFF;
}
