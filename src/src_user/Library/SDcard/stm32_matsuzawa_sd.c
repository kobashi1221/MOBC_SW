/**
  *****************************************************************************************
  * @file    : stm32_matsuzawa_sd.c
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/12/03
  * @brief   : created by S.MATSUZAWA in 2018
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *  How to use
  *  1.　add stm32_matsuzawa_sd.c and　stm32_adafruit_sd.c
  *  2. include stm32_matsuzawa_sd.h and stm32_adafruit_sd.h in Src/user_diskio.c
  *  3. add the following
  *   	  	  FRESULT res;
  *   	  	  res = f_mount(&USERFatFS, (TCHAR const*)USERPath, 0);
  *    into USER CODE BEGIN Init in fatfs.c
  *  4. change SD_CS_PIN SD_CS_GPIO_PORT　to actual GPIO Port of Chip Select pin
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/12/03  1.0   N.KISHI         transplant to C2A
  *
  *****************************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm32_matsuzawa_sd.h"



/**
  * @brief LINK SD Card
  */
#define SD_DUMMY_BYTE            0xFF
#define SD_NO_RESPONSE_EXPECTED  0x80

/**
  * @brief  SD Control Lines management
  */
#define SD_CS_LOW()       HAL_GPIO_WritePin_C2A(CS_SPI3V3_MOBCtouSD_GPIO_Port, CS_SPI3V3_MOBCtouSD_Pin, GPIO_PIN_RESET)
#define SD_CS_HIGH()      HAL_GPIO_WritePin_C2A(CS_SPI3V3_MOBCtouSD_GPIO_Port, CS_SPI3V3_MOBCtouSD_Pin, GPIO_PIN_SET)

/* SD IO functions */
void SD_IO_Init(void);
void SD_IO_CSState(uint8_t state);
void SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);
uint8_t SD_IO_WriteByte(uint8_t Data);
void SD_IO_WritData(const uint8_t *DataIn, uint16_t DataLength);

const char WR_uart[3] = "WR:";
const char R_uart[3]  = "R :";
const char W_uart[3]  = "W :";
const char crlf_uart[2] = "\r\n";

/********************************* LINK SD ************************************/
/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for 
  *         data transfer).
  */
void SD_IO_Init(void)
{
  uint8_t counter;

  /* SD chip select high */
  SD_CS_HIGH();
  
  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (counter = 0; counter <= 9; counter++)
  {
    /* Send dummy byte 0xFF */
    SD_IO_WriteByte(SD_DUMMY_BYTE);
  }
}

/**
  * @brief  Set the SD_CS pin.
  * @param  val: pin value.
  */
void SD_IO_CSState(uint8_t val)
{
  if(val == 1) 
  {
    SD_CS_HIGH();
  }
  else
  {
    SD_CS_LOW();
  }
}

/**
  * @brief  Write a byte on the SD.
  * @param  DataIn: byte to send.
  * @param  DataOut: byte to read
  * @param  DataLength: length of data
  */
void SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;
	
	//HAL_UART_Transmit_C2A(&huart2,(uint8_t *) WR_uart,3,0xFFFF);
	//HAL_UART_Transmit_C2A(&huart2,(uint8_t *) DataOut,DataLength,0xFFFF);
	//HAL_UART_Transmit_C2A(&huart2,(uint8_t *) crlf_uart,2,0xFFFF);

  status = HAL_SPI_TransmitReceive_C2A(&hspi6, (uint8_t*) DataIn, DataOut, DataLength, 0xFFFF);
	
	//HAL_UART_Transmit_C2A(&huart2,(uint8_t *) WR_uart,3,0xFFFF);
	//HAL_UART_Transmit_C2A(&huart1,(uint8_t *) DataIn,DataLength,0xFFFF);
	//HAL_UART_Transmit_C2A(&huart2,(uint8_t *) crlf_uart,2,0xFFFF);
	//HAL_UART_Transmit_C2A(&huart2,(uint8_t *) WR_uart,3,0xFFFF);
	//HAL_UART_Transmit_C2A(&huart2,(uint8_t *) DataOut,DataLength,0xFFFF);
	//HAL_UART_Transmit_C2A(&huart2,(uint8_t *) crlf_uart,2,0xFFFF);
	//HAL_UART_Transmit_C2A(&huart2,(uint8_t *) crlf_uart,2,0xFFFF);
	
    
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    //SPIx_Error();
  }
}

/**
  * @brief  Writes a byte on the SD.
  * @param  Data: byte to send.
  */

uint8_t SD_IO_WriteByte(uint8_t Data)
{
  uint8_t tmp;
  HAL_StatusTypeDef status = HAL_OK;
/*
	HAL_UART_Transmit(&huart1,(uint8_t *) R_uart,3,0xFFFF);
	HAL_UART_Transmit(&huart1,&tmp,1,0xFFFF);
	HAL_UART_Transmit(&huart1,(uint8_t *) crlf_uart,2,0xFFFF);
*/
  status = HAL_SPI_TransmitReceive_C2A(&hspi6, &Data, &tmp, 1, 0xFFFF);

 /*
	HAL_UART_Transmit(&huart1,(uint8_t *) W_uart,3,0xFFFF);
	HAL_UART_Transmit(&huart1,&Data,1,0xFFFF);
	HAL_UART_Transmit(&huart1,(uint8_t *) crlf_uart,2,0xFFFF);
	HAL_UART_Transmit(&huart1,(uint8_t *) R_uart,3,0xFFFF);
	HAL_UART_Transmit(&huart1,&tmp,1,0xFFFF);
	HAL_UART_Transmit(&huart1,(uint8_t *) crlf_uart,2,0xFFFF);
	HAL_UART_Transmit(&huart1,(uint8_t *) crlf_uart,2,0xFFFF);
*/
    
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    //SPIx_Error();
  }
  return tmp;
}

void SD_IO_WritData(const uint8_t *DataIn, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_SPI_Transmit_C2A(&hspi6, (uint8_t*) DataIn, DataLength, 0xFFFF);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    //SPIx_Error();
  }
}




