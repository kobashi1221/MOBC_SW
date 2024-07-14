
#if (_IS25LP_DEBUG==1)
#include <stdio.h>
#endif


#include "IS25LP.h"
#include "IS25LPConfig.h"
//#include <src_core/System/TimeManager/time_manager.h>//for delay()

#define NO_ROM_DEBUG false
#define IS25LP_DUMMY_BYTE         0xA5
//ダミーは適当に設定

IS25LP_t is25lp;
void IS25LP_Delay(uint16_t ms){
	c2a_delay_ms(ms);
}
//###################################################################################################################
uint8_t IS25LP_Spi(uint8_t Data) {
	uint8_t ret;
	uint8_t hal;
	int timeout = 0x4000;
	do{
		hal  = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
		--timeout;
		if(timeout == 0){
			break;
		}
	}while(hal != HAL_SPI_STATE_READY);
	HAL_SPI_TransmitReceive_DMA_C2A(&_IS25LP_SPI, &Data, &ret, 1);
	timeout = 0x4000;
	do{
		hal  = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
		--timeout;
		if(timeout == 0){
			break;
		}
	}while(hal != HAL_SPI_STATE_READY);
	return ret;
}
//###################################################################################################################
uint32_t IS25LP_ReadID(void) {
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x9F);
	Temp0 = IS25LP_Spi(IS25LP_DUMMY_BYTE);
	Temp1 = IS25LP_Spi(IS25LP_DUMMY_BYTE);
	Temp2 = IS25LP_Spi(IS25LP_DUMMY_BYTE);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}
//###################################################################################################################
void IS25LP_ReadUniqID(void) {
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x4B);
	for (uint8_t i = 0; i < 4; i++)
		IS25LP_Spi(IS25LP_DUMMY_BYTE);
	for (uint8_t i = 0; i < 8; i++)
		is25lp.UniqID[i] = IS25LP_Spi(IS25LP_DUMMY_BYTE);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
}
//###################################################################################################################
void IS25LP_WriteEnable(void) { //WRITE ENABLE OPERATION (WREN, 06h)
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x06);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
}
//###################################################################################################################
void IS25LP_WriteDisable(void) { // WRITE DISABLE OPERATION (WRDI, 04h)
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x04);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
}
//###################################################################################################################
uint8_t IS25LP_ReadStatusRegister(uint8_t SelectStatusRegister_1_2_3) {
	uint8_t status = 0;
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	if (SelectStatusRegister_1_2_3 == 1) {
		IS25LP_Spi(0x05);
		status = IS25LP_Spi(IS25LP_DUMMY_BYTE);
		is25lp.StatusRegister1 = status;
//	} else if (SelectStatusRegister_1_2_3 == 2) {
//		IS25LP_Spi(0x35);
//		status = IS25LP_Spi(IS25LP_DUMMY_BYTE);
//		w25qxx.StatusRegister2 = status;
//	} else {
//		IS25LP_Spi(0x15);
//		status = IS25LP_Spi(IS25LP_DUMMY_BYTE);
//		w25qxx.StatusRegister3 = status;
	}
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	return status;
}
//###################################################################################################################
void IS25LP_WriteStatusRegister(uint8_t SelectStatusRegister_1_2_3, uint8_t Data) {
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	if (SelectStatusRegister_1_2_3 == 1) {
		IS25LP_Spi(0x01);
		is25lp.StatusRegister1 = Data;
//	} else if (SelectStatusRegister_1_2_3 == 2) {
//		IS25LP_Spi(0x31);
//		w25qxx.StatusRegister2 = Data;
//	} else {
//		IS25LP_Spi(0x11);
//		w25qxx.StatusRegister3 = Data;
	}
	IS25LP_Spi(Data);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
}
//###################################################################################################################
bool IS25LP_WaitForWriteEnd(void) {
	int timeout = 0x4000;
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x05);
	do {
		is25lp.StatusRegister1 = IS25LP_Spi(IS25LP_DUMMY_BYTE);
		--timeout;
		if(timeout == 0){
			break;
		}
	} while ((is25lp.StatusRegister1 & 0x01) == 0x01);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	if(timeout == 0){
		return false;
	}
	return true;
}
//###################################################################################################################
bool IS25LP_Init(void) {
	is25lp.Lock = 1;
	while (HAL_GetTick_C2A() < 100)
		IS25LP_Delay(1);
	uint32_t id;
	printf("spiprom_init\r\n");

	id = IS25LP_ReadID();
	switch (id & 0x0000FFFF) {
	case 0x6018:	//	w25q64
		is25lp.ID = IS25LP128;
		is25lp.BlockCount = 256;
		break;
	case 0x6017:
		is25lp.ID = IS25LP64;
		is25lp.BlockCount = 128;
		break;
	case 0x6016:
		is25lp.ID = IS25LP32;
		is25lp.BlockCount = 64;
		break;
	default:
		if (NO_ROM_DEBUG != 1) {
			is25lp.Lock = 0;
			return false;
		}
	}
	is25lp.PageSize = 256;//一度に書き込み・読み込みできるbyte数
	is25lp.SectorSize = 0x1000; //4k Bytes
	is25lp.SectorCount = is25lp.BlockCount * 16;//number of sector
	is25lp.PageCount = (is25lp.SectorCount * is25lp.SectorSize) / is25lp.PageSize;//いくつのページを使うか
	is25lp.BlockSize = is25lp.SectorSize * 16;//一つのブロックにいくつのsectorがあるか
	is25lp.CapacityInKiloByte = (is25lp.SectorCount * is25lp.SectorSize) / 1024;//いくつのキロbyte単位が出来るか
	IS25LP_ReadUniqID();
	IS25LP_ReadStatusRegister(1);
	IS25LP_ReadStatusRegister(2);
	IS25LP_ReadStatusRegister(3);
#if (_IS25LP_DEBUG==1)
	printf("w25qxx Page Size: %d Bytes\r\n",w25qxx.PageSize);
	printf("w25qxx Page Count: %d\r\n",w25qxx.PageCount);
	printf("w25qxx Sector Size: %d Bytes\r\n",w25qxx.SectorSize);
	printf("w25qxx Sector Count: %d\r\n",w25qxx.SectorCount);
	printf("w25qxx Block Size: %d Bytes\r\n",w25qxx.BlockSize);
	printf("w25qxx Block Count: %d\r\n",w25qxx.BlockCount);
	printf("w25qxx Capacity: %d KiloBytes\r\n",w25qxx.CapacityInKiloByte);
	printf("w25qxx Init Done\r\n");
#endif
	is25lp.Lock = 0;
	return true;
}
//###################################################################################################################
void IS25LP_EraseChip(void) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	// set WEL -> erase all -> WEL automatically reset
	IS25LP_WriteEnable();
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0xC7);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	IS25LP_Delay(10);
	IS25LP_WaitForWriteEnd();
	IS25LP_Delay(10);
	is25lp.Lock = 0;
}
//###################################################################################################################
void IS25LP_EraseSector(uint32_t SectorAddr) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	IS25LP_WaitForWriteEnd();
	SectorAddr = SectorAddr * is25lp.SectorSize;
	IS25LP_WriteEnable();
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x20);
	IS25LP_Spi((SectorAddr & 0xFF0000) >> 16);
	IS25LP_Spi((SectorAddr & 0xFF00) >> 8);
	IS25LP_Spi(SectorAddr & 0xFF);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	IS25LP_WaitForWriteEnd();
//	IS25LP_Delay(1);
	is25lp.Lock = 0;
}
//###################################################################################################################
void IS25LP_Erase32KBlock(uint32_t BlockAddr) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	IS25LP_WaitForWriteEnd();
	BlockAddr = BlockAddr * is25lp.SectorSize * 16;
	IS25LP_WriteEnable();
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x52);
	IS25LP_Spi((BlockAddr & 0xFF0000) >> 16);
	IS25LP_Spi((BlockAddr & 0xFF00) >> 8);
	IS25LP_Spi(BlockAddr & 0xFF);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	IS25LP_WaitForWriteEnd();
	IS25LP_Delay(1);
	is25lp.Lock = 0;
}
//###################################################################################################################
void IS25LP_Erase64KBlock(uint32_t BlockAddr) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	IS25LP_WaitForWriteEnd();
	BlockAddr = BlockAddr * is25lp.SectorSize * 16;
	IS25LP_WriteEnable();
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0xD8);
	IS25LP_Spi((BlockAddr & 0xFF0000) >> 16);
	IS25LP_Spi((BlockAddr & 0xFF00) >> 8);
	IS25LP_Spi(BlockAddr & 0xFF);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	IS25LP_WaitForWriteEnd();
	IS25LP_Delay(1);
	is25lp.Lock = 0;
}
//###################################################################################################################
uint32_t IS25LP_PageToSector(uint32_t PageAddress) {
	return ((PageAddress * is25lp.PageSize) / is25lp.SectorSize);
}
//###################################################################################################################
uint32_t IS25LP_PageToBlock(uint32_t PageAddress) {
	return ((PageAddress * is25lp.PageSize) / is25lp.BlockSize);
}
//###################################################################################################################
uint32_t IS25LP_SectorToBlock(uint32_t SectorAddress) {
	return ((SectorAddress * is25lp.SectorSize) / is25lp.BlockSize);
}
//###################################################################################################################
uint32_t IS25LP_SectorToPage(uint32_t SectorAddress) {
	return (SectorAddress * is25lp.SectorSize) / is25lp.PageSize;
}
//###################################################################################################################
uint32_t IS25LP_BlockToPage(uint32_t BlockAddress) {
	return (BlockAddress * is25lp.BlockSize) / is25lp.PageSize;
}
//###################################################################################################################
bool IS25LP_IsEmptyBytes(uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_SectorSize){
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	if ((NumByteToCheck_up_to_SectorSize > is25lp.SectorSize) || (NumByteToCheck_up_to_SectorSize == 0))
		NumByteToCheck_up_to_SectorSize = is25lp.SectorSize;
	uint8_t pBuffer[32];
	uint32_t WorkAddress;
	uint32_t i;
	int timeout = 0x4000;
	int hal;
	if(NumByteToCheck_up_to_SectorSize < sizeof(pBuffer)){
		for (i = OffsetInByte; i < OffsetInByte + NumByteToCheck_up_to_SectorSize; i++) {
			HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
			WorkAddress = (i + Sector_Address * is25lp.SectorSize);
			IS25LP_Spi(0x0B);
			IS25LP_Spi((WorkAddress & 0xFF0000) >> 16);
			IS25LP_Spi((WorkAddress & 0xFF00) >> 8);
			IS25LP_Spi(WorkAddress & 0xFF);
			IS25LP_Spi(0);
			do{
				hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
				 --timeout;
				if(timeout == 0){
					break;
				}
			}while(hal != HAL_SPI_STATE_READY);
			if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, 1) != 0) {
				volatile int tt = 0;
				tt++;
				printf("spiprom read err \r\n");
				is25lp.Lock = 0;
				return false;
			}
			timeout = 0x4000;
			do{
				hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
				 --timeout;
				if(timeout == 0){
					break;
				}
			}while(hal != HAL_SPI_STATE_READY);
			HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
			if (pBuffer[0] != 0xFF)
				goto NOT_EMPTY;
		}
	}
	else {
		for(i = OffsetInByte; i < (OffsetInByte + NumByteToCheck_up_to_SectorSize + 1) - sizeof(pBuffer); i += sizeof(pBuffer)){
			HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
			WorkAddress = (i + Sector_Address * is25lp.SectorSize);
			IS25LP_Spi(0x0B);
			IS25LP_Spi((WorkAddress & 0xFF0000) >> 16);
			IS25LP_Spi((WorkAddress & 0xFF00) >> 8);
			IS25LP_Spi(WorkAddress & 0xFF);
			IS25LP_Spi(0);
			do{
				hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
				 --timeout;
				if(timeout == 0){
					break;
				}
			}while(hal != HAL_SPI_STATE_READY);
			if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, sizeof(pBuffer)) != 0) {
				volatile int tt = 0;
				tt++;
				printf("spiprom read err \r\n");
				is25lp.Lock = 0;
				return false;
			}
			timeout = 0x4000;
			do{
				hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
				 --timeout;
				if(timeout == 0){
					break;
				}
			}while(hal != HAL_SPI_STATE_READY);
			HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
			for (uint8_t x = 0; x < sizeof(pBuffer); x++) {
				if (pBuffer[x] != 0xFF)
					goto NOT_EMPTY;
			}
		}
		if ((NumByteToCheck_up_to_SectorSize) % sizeof(pBuffer) != 0) {
			for (; i < NumByteToCheck_up_to_SectorSize; i++) {
				HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
				WorkAddress = (i + Sector_Address * is25lp.SectorSize);
				IS25LP_Spi(0x0B);
				IS25LP_Spi((WorkAddress & 0xFF0000) >> 16);
				IS25LP_Spi((WorkAddress & 0xFF00) >> 8);
				IS25LP_Spi(WorkAddress & 0xFF);
				IS25LP_Spi(0);
				do{
					hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
					 --timeout;
					if(timeout == 0){
						break;
					}
				}while(hal != HAL_SPI_STATE_READY);
				if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, 1) != 0) {
					volatile int tt = 0;
					tt++;
					printf("spiprom read err \r\n");
					is25lp.Lock = 0;
					return false;
				}
				timeout = 0x4000;
				do{
					hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
					 --timeout;
					if(timeout == 0){
						break;
					}
				}while(hal != HAL_SPI_STATE_READY);
				HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
				if (pBuffer[0] != 0xFF)
					goto NOT_EMPTY;
			}
		}
	}
	is25lp.Lock = 0;
	return true;
	NOT_EMPTY:

	is25lp.Lock = 0;
	return false;
}

bool IS25LP_IsEmptyPage(uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_PageSize) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	if (((NumByteToCheck_up_to_PageSize + OffsetInByte) > is25lp.PageSize) || (NumByteToCheck_up_to_PageSize == 0))
		NumByteToCheck_up_to_PageSize = is25lp.PageSize - OffsetInByte;

	uint8_t pBuffer[32];
	uint32_t WorkAddress;
	uint32_t i;
	int timeout = 0x4000;
	int hal;
	for (i = OffsetInByte; i < is25lp.PageSize; i += sizeof(pBuffer)) {
		HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
		WorkAddress = (i + Page_Address * is25lp.PageSize);
		IS25LP_Spi(0x0B);
		IS25LP_Spi((WorkAddress & 0xFF0000) >> 16);
		IS25LP_Spi((WorkAddress & 0xFF00) >> 8);
		IS25LP_Spi(WorkAddress & 0xFF);
		IS25LP_Spi(0);
		do{
			hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
			 --timeout;
			if(timeout == 0){
				break;
			}
		}while(hal != HAL_SPI_STATE_READY);
		if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, sizeof(pBuffer)) != 0) {
			volatile int tt = 0;
			tt++;
			printf("spiprom read err \r\n");
			is25lp.Lock = 0;
			return false;
		}
		timeout = 0x4000;
		do{
			hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
			 --timeout;
			if(timeout == 0){
				break;
			}
		}while(hal != HAL_SPI_STATE_READY);
		HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
		for (uint8_t x = 0; x < sizeof(pBuffer); x++) {
			if (pBuffer[x] != 0xFF)
				goto NOT_EMPTY;
		}
	}
	if ((is25lp.PageSize + OffsetInByte) % sizeof(pBuffer) != 0) {
		i -= sizeof(pBuffer);
		for (; i < is25lp.PageSize; i++) {
			HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
			WorkAddress = (i + Page_Address * is25lp.PageSize);
			IS25LP_Spi(0x0B);
			IS25LP_Spi((WorkAddress & 0xFF0000) >> 16);
			IS25LP_Spi((WorkAddress & 0xFF00) >> 8);
			IS25LP_Spi(WorkAddress & 0xFF);
			IS25LP_Spi(0);
			do{
				hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
				 --timeout;
				if(timeout == 0){
					break;
				}
			}while(hal != HAL_SPI_STATE_READY);
			if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, 1) != 0) {
				volatile int tt = 0;
				tt++;
				printf("spiprom read err \r\n");
				is25lp.Lock = 0;
				return false;
			}
			timeout = 0x4000;
			do{
				hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
				 --timeout;
				if(timeout == 0){
					break;
				}
			}while(hal != HAL_SPI_STATE_READY);
			HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
			if (pBuffer[0] != 0xFF)
				goto NOT_EMPTY;
		}
	}

	is25lp.Lock = 0;
	return true;

	NOT_EMPTY:
	is25lp.Lock = 0;
	return false;
}
//###################################################################################################################
bool IS25LP_IsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_SectorSize) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	if ((NumByteToCheck_up_to_SectorSize > is25lp.SectorSize) || (NumByteToCheck_up_to_SectorSize == 0))
		NumByteToCheck_up_to_SectorSize = is25lp.SectorSize;
	uint8_t pBuffer[32];
	uint32_t WorkAddress;
	uint32_t i;
	int timeout = 0x4000;
	int hal;
	for (i = OffsetInByte; i < is25lp.SectorSize; i += sizeof(pBuffer)) {
		HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
		WorkAddress = (i + Sector_Address * is25lp.SectorSize);
		IS25LP_Spi(0x0B);
		IS25LP_Spi((WorkAddress & 0xFF0000) >> 16);
		IS25LP_Spi((WorkAddress & 0xFF00) >> 8);
		IS25LP_Spi(WorkAddress & 0xFF);
		IS25LP_Spi(0);
		do{
			hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
			 --timeout;
			if(timeout == 0){
				break;
			}
		}while(hal != HAL_SPI_STATE_READY);
		if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, sizeof(pBuffer)) != 0) {
			volatile int tt = 0;
			tt++;
			printf("spiprom read err \r\n");
			is25lp.Lock = 0;
			return false;
		}
		timeout = 0x4000;
		do{
			hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
			 --timeout;
			if(timeout == 0){
				break;
			}
		}while(hal != HAL_SPI_STATE_READY);
		HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
		for (uint8_t x = 0; x < sizeof(pBuffer); x++) {
			if (pBuffer[x] != 0xFF)
				goto NOT_EMPTY;
		}
	}
	if ((is25lp.SectorSize + OffsetInByte) % sizeof(pBuffer) != 0) {
		i -= sizeof(pBuffer);
		for (; i < is25lp.SectorSize; i++) {
			HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
			WorkAddress = (i + Sector_Address * is25lp.SectorSize);
			IS25LP_Spi(0x0B);
			IS25LP_Spi((WorkAddress & 0xFF0000) >> 16);
			IS25LP_Spi((WorkAddress & 0xFF00) >> 8);
			IS25LP_Spi(WorkAddress & 0xFF);
			IS25LP_Spi(0);
			do{
				hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
				 --timeout;
				if(timeout == 0){
					break;
				}
			}while(hal != HAL_SPI_STATE_READY);
			if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, 1) != 0) {
				volatile int tt = 0;
				tt++;
				printf("spiprom read err \r\n");
				is25lp.Lock = 0;
				return false;
			}
			timeout = 0x4000;
			do{
				hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
				 --timeout;
				if(timeout == 0){
					break;
				}
			}while(hal != HAL_SPI_STATE_READY);
			HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
			if (pBuffer[0] != 0xFF)
				goto NOT_EMPTY;
		}
	}

	is25lp.Lock = 0;
	return true;
	NOT_EMPTY:

	is25lp.Lock = 0;
	return false;
}
//###################################################################################################################
bool IS25LP_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_BlockSize) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	if ((NumByteToCheck_up_to_BlockSize > is25lp.BlockSize) || (NumByteToCheck_up_to_BlockSize == 0))
		NumByteToCheck_up_to_BlockSize = is25lp.BlockSize;
#if (_IS25LP_DEBUG==1)
	printf("w25qxx CheckBlock:%d, Offset:%d, Bytes:%d begin...\r\n",Block_Address,OffsetInByte,NumByteToCheck_up_to_BlockSize);
	IS25LP_Delay(100);
	uint32_t StartTime=HAL_GetTick_C2A();
#endif
	uint8_t pBuffer[32];
	uint32_t WorkAddress;
	uint32_t i;
	int timeout = 0x4000;
	int hal;
	for (i = OffsetInByte; i < is25lp.BlockSize; i += sizeof(pBuffer)) {
		HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
		WorkAddress = (i + Block_Address * is25lp.BlockSize);
		IS25LP_Spi(0x0B);
		IS25LP_Spi((WorkAddress & 0xFF0000) >> 16);
		IS25LP_Spi((WorkAddress & 0xFF00) >> 8);
		IS25LP_Spi(WorkAddress & 0xFF);
		IS25LP_Spi(0);
		do{
			hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
			 --timeout;
			if(timeout == 0){
				break;
			}
		}while(hal != HAL_SPI_STATE_READY);
		if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, sizeof(pBuffer)) != 0) {
			volatile int tt = 0;
			tt++;
			printf("spiprom read err \r\n");
			is25lp.Lock = 0;
			return false;
		}
		timeout = 0x4000;
		do{
			hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
			 --timeout;
			if(timeout == 0){
				break;
			}
		}while(hal != HAL_SPI_STATE_READY);
		HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
		for (uint8_t x = 0; x < sizeof(pBuffer); x++) {
			if (pBuffer[x] != 0xFF)
				goto NOT_EMPTY;
		}
	}
	if ((is25lp.BlockSize + OffsetInByte) % sizeof(pBuffer) != 0) {
		i -= sizeof(pBuffer);
		for (; i < is25lp.BlockSize; i++) {
			HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
			WorkAddress = (i + Block_Address * is25lp.BlockSize);
			IS25LP_Spi(0x0B);
			IS25LP_Spi((WorkAddress & 0xFF0000) >> 16);
			IS25LP_Spi((WorkAddress & 0xFF00) >> 8);
			IS25LP_Spi(WorkAddress & 0xFF);
			IS25LP_Spi(0);
			do{
				hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
				 --timeout;
				if(timeout == 0){
					break;
				}
			}while(hal != HAL_SPI_STATE_READY);
			if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, 1) != 0) {
				volatile int tt = 0;
				tt++;
				printf("spiprom read err \r\n");
				is25lp.Lock = 0;
				return false;
			}
			timeout = 0x4000;
			do{
				hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
				 --timeout;
				if(timeout == 0){
					break;
				}
			}while(hal != HAL_SPI_STATE_READY);
			HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
			if (pBuffer[0] != 0xFF)
				goto NOT_EMPTY;
		}
	}

	is25lp.Lock = 0;
	return true;
	NOT_EMPTY:

	is25lp.Lock = 0;
	return false;
}
//###################################################################################################################
void IS25LP_WriteByte(uint8_t pBuffer, uint32_t WriteAddr_inBytes) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;

	IS25LP_WaitForWriteEnd();
	IS25LP_WriteEnable();
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x02);
	IS25LP_Spi((WriteAddr_inBytes & 0xFF0000) >> 16);
	IS25LP_Spi((WriteAddr_inBytes & 0xFF00) >> 8);
	IS25LP_Spi(WriteAddr_inBytes & 0xFF);
	IS25LP_Spi(pBuffer);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	IS25LP_WaitForWriteEnd();

	is25lp.Lock = 0;
}
//###################################################################################################################
void IS25LP_WritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte,
		uint32_t NumByteToWrite_up_to_PageSize) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	if (((NumByteToWrite_up_to_PageSize + OffsetInByte) > is25lp.PageSize) || (NumByteToWrite_up_to_PageSize == 0))
		NumByteToWrite_up_to_PageSize = is25lp.PageSize - OffsetInByte;
	if ((OffsetInByte + NumByteToWrite_up_to_PageSize) > is25lp.PageSize)
		NumByteToWrite_up_to_PageSize = is25lp.PageSize - OffsetInByte;

	IS25LP_WaitForWriteEnd();
	IS25LP_WriteEnable();
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x02);
	Page_Address = (Page_Address * is25lp.PageSize) + OffsetInByte;
	IS25LP_Spi((Page_Address & 0xFF0000) >> 16);
	IS25LP_Spi((Page_Address & 0xFF00) >> 8);
	IS25LP_Spi(Page_Address & 0xFF);
	HAL_SPI_Transmit_C2A(&_IS25LP_SPI, pBuffer, NumByteToWrite_up_to_PageSize, 100);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	IS25LP_WaitForWriteEnd();
#if (_IS25LP_DEBUG==1)
	StartTime = HAL_GetTick_C2A()-StartTime;
	for(uint32_t i=0;i<NumByteToWrite_up_to_PageSize; i++)
	{
		if((i%8==0)&&(i>2))
		{
			printf("\r\n");
			IS25LP_Delay(10);
		}
		printf("0x%02X,",pBuffer[i]);
	}
	printf("\r\n");
	printf("w25qxx WritePage done after %d ms\r\n",StartTime);
	IS25LP_Delay(100);
#endif
	is25lp.Lock = 0;
}
//###################################################################################################################
void IS25LP_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte,
		uint32_t NumByteToWrite_up_to_SectorSize) {
	if ((NumByteToWrite_up_to_SectorSize > is25lp.SectorSize) || (NumByteToWrite_up_to_SectorSize == 0))
		NumByteToWrite_up_to_SectorSize = is25lp.SectorSize;
#if (_IS25LP_DEBUG==1)
	printf("+++w25qxx WriteSector:%d, Offset:%d ,Write %d Bytes, begin...\r\n",Sector_Address,OffsetInByte,NumByteToWrite_up_to_SectorSize);
	IS25LP_Delay(100);
#endif
	if (OffsetInByte >= is25lp.SectorSize) {
#if (_IS25LP_DEBUG==1)
		printf("---w25qxx WriteSector Faild!\r\n");
		IS25LP_Delay(100);
#endif
		return;
	}
	uint32_t StartPage;
	uint32_t NowPage;
	int32_t BytesToWrite;
	uint32_t LocalOffset;
	if ((OffsetInByte + NumByteToWrite_up_to_SectorSize) > is25lp.SectorSize)
		BytesToWrite = is25lp.SectorSize - OffsetInByte;
	else
		BytesToWrite = NumByteToWrite_up_to_SectorSize;
	StartPage = IS25LP_SectorToPage(Sector_Address) + (OffsetInByte / is25lp.PageSize);
	NowPage = StartPage;
	LocalOffset = OffsetInByte % is25lp.PageSize;
	do {
		IS25LP_WritePage(pBuffer, NowPage, LocalOffset, BytesToWrite);
		BytesToWrite -= is25lp.PageSize - LocalOffset;
		if(NowPage == StartPage){
			pBuffer += is25lp.PageSize - LocalOffset;
		}
		else{
			pBuffer += is25lp.PageSize;
		}
		NowPage++;
		LocalOffset = 0;
	} while (BytesToWrite > 0);
#if (_IS25LP_DEBUG==1)
	printf("---w25qxx WriteSector Done\r\n");
	IS25LP_Delay(100);
#endif
}
//###################################################################################################################
void IS25LP_WriteBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte,
		uint32_t NumByteToWrite_up_to_BlockSize) {
	if ((NumByteToWrite_up_to_BlockSize > is25lp.BlockSize) || (NumByteToWrite_up_to_BlockSize == 0))
		NumByteToWrite_up_to_BlockSize = is25lp.BlockSize;
#if (_IS25LP_DEBUG==1)
	printf("+++w25qxx WriteBlock:%d, Offset:%d ,Write %d Bytes, begin...\r\n",Block_Address,OffsetInByte,NumByteToWrite_up_to_BlockSize);
	IS25LP_Delay(100);
#endif
	if (OffsetInByte >= is25lp.BlockSize) {
#if (_IS25LP_DEBUG==1)
		printf("---w25qxx WriteBlock Faild!\r\n");
		IS25LP_Delay(100);
#endif
		return;
	}
	uint32_t StartPage;
	uint32_t NowPage;
	int32_t BytesToWrite;
	uint32_t LocalOffset;
	if ((OffsetInByte + NumByteToWrite_up_to_BlockSize) > is25lp.BlockSize)
		BytesToWrite = is25lp.BlockSize - OffsetInByte;
	else
		BytesToWrite = NumByteToWrite_up_to_BlockSize;
	StartPage = IS25LP_BlockToPage(Block_Address) + (OffsetInByte / is25lp.PageSize);
	NowPage = StartPage;
	LocalOffset = OffsetInByte % is25lp.PageSize;
	do {
		IS25LP_WritePage(pBuffer, NowPage, LocalOffset, BytesToWrite);
		BytesToWrite -= is25lp.PageSize - LocalOffset;
		if(NowPage == StartPage){
			pBuffer += BytesToWrite;
		}
		else{
			pBuffer += is25lp.PageSize;
		}
		NowPage++;
		LocalOffset = 0;
	} while (BytesToWrite > 0);
#if (_IS25LP_DEBUG==1)
	printf("---w25qxx WriteBlock Done\r\n");
	IS25LP_Delay(100);
#endif
}
//###################################################################################################################
void IS25LP_ReadByte(uint8_t *pBuffer, uint32_t Bytes_Address) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
#if (_IS25LP_DEBUG==1)
	uint32_t StartTime=HAL_GetTick_C2A();
	printf("w25qxx ReadByte at address %d begin...\r\n",Bytes_Address);
#endif
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x0B);
	IS25LP_Spi((Bytes_Address & 0xFF0000) >> 16);
	IS25LP_Spi((Bytes_Address & 0xFF00) >> 8);
	IS25LP_Spi(Bytes_Address & 0xFF);
	IS25LP_Spi(0);
	*pBuffer = IS25LP_Spi(IS25LP_DUMMY_BYTE);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
#if (_IS25LP_DEBUG==1)
	printf("w25qxx ReadByte 0x%02X done after %d ms\r\n",*pBuffer,HAL_GetTick_C2A()-StartTime);
#endif
	is25lp.Lock = 0;
}
//###################################################################################################################
bool IS25LP_ReadBytes(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	int hal;
	int timeout = 0x4000;
#if (_IS25LP_DEBUG==1)
	uint32_t StartTime=HAL_GetTick_C2A();
	printf("w25qxx ReadBytes at Address:%d, %d Bytes  begin...\r\n",ReadAddr,NumByteToRead);
#endif
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x0B);
	IS25LP_Spi((ReadAddr & 0xFF0000) >> 16);
	IS25LP_Spi((ReadAddr & 0xFF00) >> 8);
	IS25LP_Spi(ReadAddr & 0xFF);
	IS25LP_Spi(0);
	do{
		hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
		--timeout;
		if(timeout == 0){
			break;
		}
	}while(hal != HAL_SPI_STATE_READY);
	if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, NumByteToRead) != 0) {
		volatile int tt = 0;
		tt++;
		printf("spiprom read err \r\n");
		is25lp.Lock = 0;
		return false;
	}
	timeout = 0x4000;
	do{
		hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
		--timeout;
		if(timeout == 0){
			break;
		}
	}while(hal != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);

#if (_IS25LP_DEBUG==1)
	StartTime = HAL_GetTick_C2A()-StartTime;
	for(uint32_t i=0;i<NumByteToRead; i++)
	{
		if((i%8==0)&&(i>2))
		{
			printf("\r\n");
			IS25LP_Delay(10);
		}
		printf("0x%02X,",pBuffer[i]);
	}
	printf("\r\n");
	printf("w25qxx ReadBytes done after %d ms\r\n",StartTime);
	IS25LP_Delay(100);
#endif
	IS25LP_Delay(1);
	is25lp.Lock = 0;
	return true;
}
//###################################################################################################################
bool IS25LP_ReadPage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte,
		uint32_t NumByteToRead_up_to_PageSize) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	int hal;
	int timeout = 0x4000;
	if ((NumByteToRead_up_to_PageSize > is25lp.PageSize) || (NumByteToRead_up_to_PageSize == 0))
		NumByteToRead_up_to_PageSize = is25lp.PageSize;
	if ((OffsetInByte + NumByteToRead_up_to_PageSize) > is25lp.PageSize)
		NumByteToRead_up_to_PageSize = is25lp.PageSize - OffsetInByte;
#if (_IS25LP_DEBUG==1)
	printf("w25qxx ReadPage:%d, Offset:%d ,Read %d Bytes, begin...\r\n",Page_Address,OffsetInByte,NumByteToRead_up_to_PageSize);
	IS25LP_Delay(100);
	uint32_t StartTime=HAL_GetTick_C2A();
#endif
	Page_Address = Page_Address * is25lp.PageSize + OffsetInByte;
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x0B);
	IS25LP_Spi((Page_Address & 0xFF0000) >> 16);
	IS25LP_Spi((Page_Address & 0xFF00) >> 8);
	IS25LP_Spi(Page_Address & 0xFF);
	IS25LP_Spi(0);
	do{
		hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
		--timeout;
		if(timeout == 0){
			break;
		}
	}while(hal != HAL_SPI_STATE_READY);
	if (HAL_SPI_Receive_DMA_C2A(&_IS25LP_SPI, pBuffer, NumByteToRead_up_to_PageSize) != 0) {
		volatile int tt = 0;
		tt++;
		printf("spiprom read err \r\n");
		is25lp.Lock = 0;
		return false;
	}
	timeout = 0x4000;
	do{
		hal = HAL_SPI_GetState_C2A(&_IS25LP_SPI);
		--timeout;
		if(timeout == 0){
			break;
		}
	}while(hal != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
#if (_IS25LP_DEBUG==1)
	StartTime = HAL_GetTick_C2A()-StartTime;
	for(uint32_t i=0;i<NumByteToRead_up_to_PageSize; i++)
	{
		if((i%8==0)&&(i>2))
		{
			printf("\r\n");
			IS25LP_Delay(10);
		}
		printf("0x%02X,",pBuffer[i]);
	}
	printf("\r\n");
	printf("w25qxx ReadPage done after %d ms\r\n",StartTime);
	IS25LP_Delay(100);
#endif
	IS25LP_Delay(1);
	is25lp.Lock = 0;
	return true;
}
//###################################################################################################################
void IS25LP_ReadSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte,
		uint32_t NumByteToRead_up_to_SectorSize) {
	if ((NumByteToRead_up_to_SectorSize > is25lp.SectorSize) || (NumByteToRead_up_to_SectorSize == 0))
		NumByteToRead_up_to_SectorSize = is25lp.SectorSize;
#if (_IS25LP_DEBUG==1)
	printf("+++w25qxx ReadSector:%d, Offset:%d ,Read %d Bytes, begin...\r\n",Sector_Address,OffsetInByte,NumByteToRead_up_to_SectorSize);
	IS25LP_Delay(100);
#endif
	if (OffsetInByte >= is25lp.SectorSize) {
#if (_IS25LP_DEBUG==1)
		printf("---w25qxx ReadSector Faild!\r\n");
		IS25LP_Delay(100);
#endif
		return;
	}
	uint32_t StartPage;
	int32_t BytesToRead;
	uint32_t LocalOffset;
	if ((OffsetInByte + NumByteToRead_up_to_SectorSize) > is25lp.SectorSize)
		BytesToRead = is25lp.SectorSize - OffsetInByte;
	else
		BytesToRead = NumByteToRead_up_to_SectorSize;
	StartPage = IS25LP_SectorToPage(Sector_Address) + (OffsetInByte / is25lp.PageSize);
	LocalOffset = OffsetInByte % is25lp.PageSize;
	do {
		IS25LP_ReadPage(pBuffer, StartPage, LocalOffset, BytesToRead);
		StartPage++;
		BytesToRead -= is25lp.PageSize - LocalOffset;
		pBuffer += is25lp.PageSize;
		LocalOffset = 0;
	} while (BytesToRead > 0);
#if (_IS25LP_DEBUG==1)
	printf("---w25qxx ReadSector Done\r\n");
	IS25LP_Delay(100);
#endif
}
//###################################################################################################################
void IS25LP_ReadBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte,
		uint32_t NumByteToRead_up_to_BlockSize) {
	if ((NumByteToRead_up_to_BlockSize > is25lp.BlockSize) || (NumByteToRead_up_to_BlockSize == 0))
		NumByteToRead_up_to_BlockSize = is25lp.BlockSize;
#if (_IS25LP_DEBUG==1)
	printf("+++w25qxx ReadBlock:%d, Offset:%d ,Read %d Bytes, begin...\r\n",Block_Address,OffsetInByte,NumByteToRead_up_to_BlockSize);
	IS25LP_Delay(100);
#endif
	if (OffsetInByte >= is25lp.BlockSize) {
#if (_IS25LP_DEBUG==1)
		printf("w25qxx ReadBlock Faild!\r\n");
		IS25LP_Delay(100);
#endif
		return;
	}
	uint32_t StartPage;
	int32_t BytesToRead;
	uint32_t LocalOffset;
	if ((OffsetInByte + NumByteToRead_up_to_BlockSize) > is25lp.BlockSize)
		BytesToRead = is25lp.BlockSize - OffsetInByte;
	else
		BytesToRead = NumByteToRead_up_to_BlockSize;
	StartPage = IS25LP_BlockToPage(Block_Address) + (OffsetInByte / is25lp.PageSize);
	LocalOffset = OffsetInByte % is25lp.PageSize;
	do {
		IS25LP_ReadPage(pBuffer, StartPage, LocalOffset, BytesToRead);
		StartPage++;
		BytesToRead -= is25lp.PageSize - LocalOffset;
		pBuffer += is25lp.PageSize;
		LocalOffset = 0;
	} while (BytesToRead > 0);
#if (_IS25LP_DEBUG==1)
	printf("---w25qxx ReadBlock Done\r\n");
	IS25LP_Delay(100);
#endif
}
//###################################################################################################################

void IS25LP_Reset(){
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x66);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	IS25LP_Delay(1);

	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x99);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
	IS25LP_Delay(1);
}

//###################################################################################################################
bool IS25LP_EraseSector_C2A(uint32_t SectorAddr) {
	while (is25lp.Lock == 1)
		IS25LP_Delay(1);
	is25lp.Lock = 1;
	int ret;
	ret = IS25LP_WaitForWriteEnd();
	if (ret != true){
		return false;
	}
	SectorAddr = SectorAddr * is25lp.SectorSize;
	IS25LP_WriteEnable();
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
	IS25LP_Spi(0x20);
	IS25LP_Spi((SectorAddr & 0xFF0000) >> 16);
	IS25LP_Spi((SectorAddr & 0xFF00) >> 8);
	IS25LP_Spi(SectorAddr & 0xFF);
	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
//	IS25LP_WaitForWriteEnd();ここの処理をC2Aのループで行うことにする
//	IS25LP_Delay(1);
	is25lp.Lock = 0;
	return true;
}

//void IS25LP_PWriteByte(uint8_t pBuffer, uint32_t WriteAddr_inBytes) {
//	while (is25lp.Lock == 1)
//		IS25LP_Delay(1);
//	is25lp.Lock = 1;
//
//	IS25LP_WaitForWriteEnd();
//	IS25LP_WriteEnable();
//	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_RESET);
//	IS25LP_Spi(0x02);
//	IS25LP_Spi((WriteAddr_inBytes & 0xFF0000) >> 16);
//	IS25LP_Spi((WriteAddr_inBytes & 0xFF00) >> 8);
//	IS25LP_Spi(WriteAddr_inBytes & 0xFF);
//	IS25LP_Spi(pBuffer);
//	HAL_GPIO_WritePin_C2A(_IS25LP_CS_GPIO, _IS25LP_CS_PIN, GPIO_PIN_SET);
//	IS25LP_WaitForWriteEnd();
//
//	is25lp.Lock = 0;
//}
