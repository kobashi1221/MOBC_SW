/**
  *****************************************************************************************
  * @file    : MRAM.h
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/09/30
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/09/30  1.0   N.KISHI         First revision
  * 2020/10/19  1.1   N.KISHI         1stEM CDH1 board test passed
  *
  *****************************************************************************************
  */

#ifndef MRAM_H_
#define MRAM_H_

#include <stdint.h> //for uint8_t
#include <stdlib.h>
#include <src_user/IfWrapper/HAL.h>

typedef enum{
	WREN = 0x06,
	WRDI = 0x04,
	RDSR = 0x05,
	WRSR = 0x01,
	READ = 0x03,
	WRITE = 0x02,
	SLEEP = 0xb9,
	WAKE = 0xab
}MRAM_CMD;



#ifdef __cplusplus
 extern "C" {
#endif

 HAL_StatusTypeDef MRAM_Enable(void);
 HAL_StatusTypeDef MRAM_Disable(void);
 void MRAM_Init(void* spi_port, void* cs_port, uint16_t cs_pin);
 HAL_StatusTypeDef MRAM_GeneralRead(uint8_t* ReadDataBuf,
 									size_t DataSize,
 										uint32_t ReadAddress);
 HAL_StatusTypeDef MRAM_GeneralWrite(uint8_t* WriteDataBuf,
 									size_t DataSize,
 										uint32_t WriteAddress);

#ifdef __cplusplus
}
#endif

#endif // MRAM_H_
