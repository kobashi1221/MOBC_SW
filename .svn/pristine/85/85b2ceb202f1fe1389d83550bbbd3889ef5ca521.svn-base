#ifndef _IS25LP_H
#define _IS25LP_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "src_user/Library/Delay.h"
//#include "stm32f4xx_hal.h" //kishi comment out
//#include "main.h"

typedef enum {
	IS25LP32 = 1, IS25LP64, IS25LP128

} IS25LP_ID_t;

typedef struct {
	IS25LP_ID_t ID;
	uint8_t UniqID[8];

	uint16_t PageSize;
	uint32_t PageCount;
	uint32_t SectorSize;
	uint32_t BlockSize;
	uint32_t SectorCount;
	uint32_t BlockCount;

	uint32_t CapacityInKiloByte;

	uint8_t StatusRegister1;
	uint8_t StatusRegister2;
	uint8_t StatusRegister3;

	uint8_t Lock;

} IS25LP_t;

extern IS25LP_t is25lp;
#ifdef __cplusplus
extern "C" {
#endif


//############################################################################
// in Page,Sector and block read/write functions, can put 0 to read maximum bytes 
//############################################################################
bool IS25LP_Init(void);

void IS25LP_Delay(uint16_t ms);

uint8_t IS25LP_ReadStatusRegister(uint8_t SelectStatusRegister_1_2_3);
void IS25LP_WriteStatusRegister(uint8_t SelectStatusRegister_1_2_3, uint8_t Data);

void IS25LP_EraseChip(void);
void IS25LP_EraseSector(uint32_t SectorAddr);
void IS25LP_Erase32KBlock(uint32_t BlockAddr);
void IS25LP_Erase64KBlock(uint32_t BlockAddr);

uint32_t IS25LP_PageToSector(uint32_t PageAddress);
uint32_t IS25LP_PageToBlock(uint32_t PageAddress);
uint32_t IS25LP_SectorToBlock(uint32_t SectorAddress);
uint32_t IS25LP_SectorToPage(uint32_t SectorAddress);
uint32_t IS25LP_BlockToPage(uint32_t BlockAddress);

bool IS25LP_IsEmptyBytes(uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_SectorSize);
bool IS25LP_IsEmptyPage(uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_PageSize);
bool IS25LP_IsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_SectorSize);
bool IS25LP_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_BlockSize);
bool IS25LP_WaitForWriteEnd(void);

void IS25LP_WriteByte(uint8_t pBuffer, uint32_t Bytes_Address);
void IS25LP_WritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte,
		uint32_t NumByteToWrite_up_to_PageSize);
void IS25LP_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte,
		uint32_t NumByteToWrite_up_to_SectorSize);
void IS25LP_WriteBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte,
		uint32_t NumByteToWrite_up_to_BlockSize);

void IS25LP_ReadByte(uint8_t *pBuffer, uint32_t Bytes_Address);
bool IS25LP_ReadBytes(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
bool IS25LP_ReadPage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte,
		uint32_t NumByteToRead_up_to_PageSize);
void IS25LP_ReadSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte,
		uint32_t NumByteToRead_up_to_SectorSize);
void IS25LP_ReadBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte,
		uint32_t NumByteToRead_up_to_BlockSize);

void IS25LP_Reset();

//for C2A driver function
bool IS25LP_EraseSector_C2A(uint32_t SectorAddr);

//############################################################################
#ifdef __cplusplus
}
#endif

#endif

