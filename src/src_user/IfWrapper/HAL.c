/* HAL.c */

#pragma section REPRO
#include "HAL.h"

#ifndef HAL_SILS

UART_HandleTypeDef *sp_huart;


int HAL_UART_Receive_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size,
		uint32_t Timeout){
	int ret;
	ret = HAL_UART_Receive(huart, pData, Size, Timeout);
	return ret;
}

int HAL_UART_Receive_IT_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size){
	int ret;
	ret = HAL_UART_Receive_IT(huart, pData, Size);
	return ret;
}

HAL_StatusTypeDef HAL_UART_Init_C2A(void *huart){
	return HAL_UART_Init(huart);
}

HAL_StatusTypeDef HAL_UART_DeInit_C2A(void *UartHandle){
	return HAL_UART_DeInit(UartHandle);
}

int HAL_UART_Transmit_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size,
		uint32_t Timeout){
	int ret;
	ret = HAL_UART_Transmit(huart, pData, Size, Timeout);
	return ret;
}

int HAL_UART_Transmit_DMA_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size){
	int ret;
	ret = HAL_UART_Transmit_DMA(huart, pData, Size);
	return ret;
}

int HAL_UART_Receive_DMA_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size){
	int ret;
	ret = HAL_UART_Receive_DMA(huart, pData, Size);
	return ret;
}

int HAL_UART_Get_State_C2A(void *huart){
	UART_HandleTypeDef* uart_=(UART_HandleTypeDef*)huart;
	return uart_->gState;
}

int HAL_UART_Transmit_IT_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size){
	int ret;
	ret = HAL_UART_Transmit_IT(huart, pData, Size);
	return ret;
}

int HAL_UARTEx_ReceiveToIdle_DMA_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size){
	int ret;
	ret = HAL_UARTEx_ReceiveToIdle_DMA(huart, pData, Size);
	return ret;
}

int HAL_I2C_Get_State_C2A(void *hi2c){
	I2C_HandleTypeDef* i2c_=(I2C_HandleTypeDef*)hi2c;
	return i2c_->State;
}

HAL_StatusTypeDef HAL_I2C_Mem_Read_C2A(void *hi2c,
		uint16_t DevAddress,
		uint16_t MemAddress,
		uint16_t MemAddSize,
		uint8_t *pData,
		uint16_t Size,
		uint32_t Timeout){
	HAL_StatusTypeDef ret;
	ret = HAL_I2C_Mem_Read(hi2c,
			DevAddress,
			MemAddress,
			MemAddSize,
			pData,
			Size,
			Timeout);
	return ret;
}

HAL_StatusTypeDef HAL_I2C_Mem_Write_C2A(void *hi2c,
		uint16_t DevAddress,
		uint16_t MemAddress,
		uint16_t MemAddSize,
		uint8_t *pData,
		uint16_t Size,
		uint32_t Timeout){
	HAL_StatusTypeDef ret;
	ret = HAL_I2C_Mem_Write(hi2c,
			DevAddress,
			MemAddress,
			MemAddSize,
			pData,
			Size,
			Timeout);
	return ret;
}

int HAL_I2C_Master_Transmit_C2A(void *hi2c,
									uint16_t DevAddress,
										uint8_t *pData,
											uint16_t Size,
												uint32_t Timeout)
{
	int ret =HAL_I2C_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout);
	return ret;
}

int HAL_I2C_Master_Receive_C2A(void *hi2c,
								uint16_t DevAddress,
									uint8_t *pData,
										uint16_t Size,
											uint32_t Timeout)
{
	int ret = HAL_I2C_Master_Receive(hi2c, DevAddress, pData, Size, Timeout);
	return ret;
}

int HAL_I2C_GetState_C2A(void *hi2c){
	int ret;
	ret = HAL_I2C_GetState(hi2c);
	return ret;
}
int HAL_I2C_DeInit_C2A(void *hi2c){
	int ret;
	ret = HAL_I2C_DeInit(hi2c);
	return ret;
}
int HAL_I2C_Init_C2A(void *hi2c){
	int ret;
	ret = HAL_I2C_Init(hi2c);
	return ret;
}

HAL_StatusTypeDef HAL_SPI_DeInit_C2A(void *hspi){
	return HAL_SPI_DeInit(hspi);
}

HAL_StatusTypeDef HAL_SPI_Init_C2A(void *hspi){
	return HAL_SPI_Init(hspi);
}

int HAL_SPI_Get_State_C2A(void *hspi){
	SPI_HandleTypeDef* spi_=(SPI_HandleTypeDef*)hspi;
	return spi_->State;
}

int HAL_SPI_GetState_C2A(void *hspi)
{
	SPI_HandleTypeDef *spi_=(SPI_HandleTypeDef*)hspi;
  return spi_->State;
}

HAL_StatusTypeDef HAL_SPI_Transmit_C2A(void* hspi,
							uint8_t *pData,
								uint16_t Size,
									uint32_t Timeout){
	HAL_StatusTypeDef ret ;
	SPI_HandleTypeDef* temp = (SPI_HandleTypeDef*)hspi;
	ret = HAL_SPI_Transmit(temp, pData, Size, Timeout);
	return ret;
}

HAL_StatusTypeDef HAL_SPI_Receive_C2A(void* hspi,
							uint8_t *pData,
								uint16_t Size,
									uint32_t Timeout){
	HAL_StatusTypeDef ret ;
	SPI_HandleTypeDef* temp = (SPI_HandleTypeDef*)hspi;
	ret = HAL_SPI_Receive(temp, pData, Size, Timeout);
	return ret;
}

HAL_StatusTypeDef HAL_SPI_TransmitReceive_C2A(void* hspi,
									uint8_t *pTxData,
										uint8_t *pRxData,
											uint16_t Size,
												uint32_t Timeout){
	HAL_StatusTypeDef ret ;
	SPI_HandleTypeDef* temp = (SPI_HandleTypeDef*)hspi;
	ret = HAL_SPI_TransmitReceive(temp, pTxData, pRxData, Size, Timeout);
	return ret;
}

HAL_StatusTypeDef HAL_SPI_Receive_DMA_C2A(void* hspi,uint8_t *pTxData,uint16_t Size){
	HAL_StatusTypeDef ret ;
	SPI_HandleTypeDef* temp = (SPI_HandleTypeDef*)hspi;
	ret = HAL_SPI_Receive_DMA(temp, pTxData, Size);
	return ret;
}

HAL_StatusTypeDef HAL_SPI_TransmitReceive_DMA_C2A(void* hspi,
									uint8_t *pTxData,
										uint8_t *pRxData,
											uint16_t Size){
	HAL_StatusTypeDef ret ;
	SPI_HandleTypeDef* temp = (SPI_HandleTypeDef*)hspi;
	ret = HAL_SPI_TransmitReceive_DMA(temp, pTxData, pRxData, Size);
	return ret;
}

void HAL_GPIO_WritePin_C2A(void* GPIOx,
							uint16_t GPIO_Pin,
								int PinState){
	GPIO_TypeDef* temp = (GPIO_TypeDef*)GPIOx;
	HAL_GPIO_WritePin(temp, GPIO_Pin, PinState);
}

int HAL_GPIO_ReadPin_C2A(void* GPIOx, uint16_t GPIO_Pin){
	int pinstate = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	return pinstate;
}

void HAL_GPIO_TogglePin_C2A(void* GPIOx, uint16_t GPIO_Pin){
	HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}

void HAL_GPIO_Init_C2A(void* GPIOx, void* GPIO_Pin){
	HAL_GPIO_Init(GPIOx, GPIO_Pin);
}


int HAL_FLASH_Unlock_C2A(void){
	int ret = HAL_FLASH_Unlock();
	return ret;
}

int HAL_FLASH_Lock_C2A(void){
	int ret = HAL_FLASH_Lock();
	return ret;
}
int HAL_FLASHEx_Erase_C2A(void *pEraseInit, uint32_t *SectorError){
	FLASH_EraseInitTypeDef *temp = (FLASH_EraseInitTypeDef *)pEraseInit;
	int ret = HAL_FLASHEx_Erase(temp, SectorError);
	return ret;
}

int HAL_FLASH_Program_C2A(uint32_t TypeProgram, uint32_t Address, uint64_t Data){
	int ret = HAL_FLASH_Program(TypeProgram, Address, Data);
	return ret;
}

HAL_StatusTypeDef HAL_IWDG_Refresh_C2A(void *hiwdg){
	IWDG_HandleTypeDef* temp = hiwdg;
	return HAL_IWDG_Refresh(temp);
}

void HAL_Delay_C2A(uint32_t Delay){
	HAL_Delay(Delay);
}

uint32_t HAL_GetTick_C2A(void){
	uint32_t ret;
	ret = HAL_GetTick();
	return ret;
}

#ifdef ONE_U
HAL_StatusTypeDef HAL_DCMI_Start_DMA_C2A(void* hdcmi, uint32_t DCMI_Mode, uint32_t pData, uint32_t Length){
	uint8_t ret_HAL = 0;
	DCMI_HandleTypeDef* temp = hdcmi;
	ret_HAL = HAL_DCMI_Start_DMA(temp, DCMI_Mode, (uint32_t)pData, ((uint32_t)Length));
	return ret_HAL;
}


void __HAL_DCMI_ENABLE_IT_C2A(void* hdcmi, uint32_t DCMI_FLAME){
	DCMI_HandleTypeDef* temp = hdcmi;
	__HAL_DCMI_ENABLE_IT(temp, DCMI_FLAME);
}
#endif

#endif

#pragma section
