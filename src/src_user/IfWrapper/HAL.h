/* HAL.h */

#ifndef HAL_H_
#define HAL_H_

#include "stdint.h"            // for int32_t, uint32_t

//extern UART_HandleTypeDef UART_HandleTypeDef_C2A;
//extern void UART_HandleTypeDef_C2A;

#ifndef HAL_SILS
#include "main.h"
#endif


#ifndef HAL_SILS
extern UART_HandleTypeDef *sp_huart;


#endif

#ifdef HAL_SILS

#include <stdio.h>

typedef enum
{
	GPIO_PIN_RESET = 0,
	GPIO_PIN_SET
}GPIO_PinState;

typedef enum
{
	HAL_OK = 0x00U,
	HAL_ERROR = 0x01U,
	HAL_BUSY = 0x02U,
	HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;

typedef int GPIO_TypeDef;
typedef int I2C_HandleTypeDef;
typedef int SPI_HandleTypeDef;
typedef int UART_HandleTypeDef;

//dummy adc buffer
extern uint16_t adc1_buf[60];

extern int huart4;
extern int huart5;
extern int huart7;
extern int huart8;
extern int huart1;
extern int huart2;
extern int huart3;
extern int huart6;
extern int hi2c1;
extern int hi2c2;
extern int hi2c3;
extern int hspi1;
extern int hspi2;
extern int hspi3;
extern int hspi4;
extern int hspi5;
extern int hspi6;
extern int* sp_huart;
extern int hrtc;

#endif

#ifdef __cplusplus
 extern "C" {
#endif


/* UART */
HAL_StatusTypeDef HAL_UART_Init_C2A(void *huart);
HAL_StatusTypeDef HAL_UART_DeInit_C2A(void *UartHandle);

int HAL_UART_Receive_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size,
		uint32_t Timeout);

int HAL_UART_Receive_IT_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size);

int HAL_UART_Transmit_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size,
		uint32_t Timeout);

int HAL_UART_Transmit_DMA_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size);

int HAL_UART_Receive_DMA_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size);

int HAL_UART_Transmit_IT_C2A(void *huart,
		uint8_t *pData,
		uint16_t Size);

int HAL_UART_Get_State_C2A(void *huart);


/* I2C */

int HAL_I2C_Get_State_C2A(void *hi2c);

HAL_StatusTypeDef HAL_I2C_Mem_Read_C2A(void *hi2c,
		uint16_t DevAddress,
		uint16_t MemAddress,
		uint16_t MemAddSize,
		uint8_t *pData,
		uint16_t Size,
		uint32_t Timeout);

HAL_StatusTypeDef HAL_I2C_Mem_Write_C2A(void *hi2c,
		uint16_t DevAddress,
		uint16_t MemAddress,
		uint16_t MemAddSize,
		uint8_t *pData,
		uint16_t Size,
		uint32_t Timeout);

int HAL_I2C_Master_Transmit_C2A(void *hi2c,
									uint16_t DevAddress,
										uint8_t *pData,
											uint16_t Size,
												uint32_t Timeout);

int HAL_I2C_Master_Receive_C2A(void *hi2c,
								uint16_t DevAddress,
									uint8_t *pData,
										uint16_t Size,
											uint32_t Timeout);

int HAL_I2C_GetState_C2A(void *hi2c);
int HAL_I2C_DeInit_C2A(void *hi2c);
int HAL_I2C_Init_C2A(void *hi2c);

HAL_StatusTypeDef HAL_SPI_DeInit_C2A(void *hspi);
HAL_StatusTypeDef HAL_SPI_Init_C2A(void *hspi);

int HAL_SPI_Get_State_C2A(void *hspi);

int HAL_SPI_GetState_C2A(void *hspi);

HAL_StatusTypeDef HAL_SPI_TransmitReceive_C2A(void* hspi,
									uint8_t *pTxData,
										uint8_t *pRxData,
											uint16_t Size,
												uint32_t Timeout);

HAL_StatusTypeDef HAL_SPI_Receive_C2A(void* hspi,
							uint8_t *pData,
								uint16_t Size,
									uint32_t Timeout);

HAL_StatusTypeDef HAL_SPI_Transmit_C2A(void* hspi,
							uint8_t *pData,
								uint16_t Size,
									uint32_t Timeout);


HAL_StatusTypeDef HAL_SPI_TransmitReceive_C2A(void* hspi,
									uint8_t *pTxData,
										uint8_t *pRxData,
											uint16_t Size,
												uint32_t Timeout);



void HAL_GPIO_WritePin_C2A(void* GPIOx,
							uint16_t GPIO_Pin,
								int PinState);

int HAL_GPIO_ReadPin_C2A(void* GPIOx, uint16_t GPIO_Pin);

void HAL_GPIO_TogglePin_C2A(void* GPIOx, uint16_t GPIO_Pin);

void HAL_GPIO_Init_C2A(void* GPIOx, void* GPIO_Pin);
//Flash_functions
int HAL_FLASH_Unlock_C2A(void);

int HAL_FLASH_Lock_C2A(void);

int HAL_FLASHEx_Erase_C2A(void *pEraseInit, uint32_t *SectorError);

int HAL_FLASH_Program_C2A(uint32_t TypeProgram, uint32_t Address, uint64_t Data);

HAL_StatusTypeDef HAL_IWDG_Refresh_C2A(void *hiwdg);

void HAL_Delay_C2A(uint32_t Delay);

uint32_t HAL_GetTick_C2A(void);

#ifdef ONE_U
HAL_StatusTypeDef HAL_DCMI_Start_DMA_C2A(void* hdcmi, uint32_t DCMI_Mode, uint32_t pData, uint32_t Length);

void __HAL_DCMI_ENABLE_IT_C2A(void* hdcmi, uint32_t DCMI_FLAME);
#endif
#ifdef __cplusplus
}
#endif

#ifdef HAL_SILS
 #define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
 #define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
 #define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
 #define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
 #define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
 #define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
 #define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
 #define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
 #define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
 #define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
 #define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
 #define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
 #define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
 #define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
 #define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
 #define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
 #define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */
 #define GPIO_PIN_MASK              0x0000FFFFU /* PIN mask for assert test */

 #define GPIOA NULL
 #define GPIOB NULL
 #define GPIOC NULL
 #define GPIOD NULL
 #define GPIOE NULL
 #define GPIOF NULL
 #define GPIOG NULL
 #define GPIOH NULL
 #define GPIOI NULL
 #define GPIOJ NULL
 #define GPIOK NULL

#ifdef TWO_U
 #define SCK_SPI3V3_MOBCtoSUNS_Pin GPIO_PIN_2
 #define SCK_SPI3V3_MOBCtoSUNS_GPIO_Port GPIOE
 #define CS_SPI3V3_MOBCtoSUNSPZ_Pin GPIO_PIN_3
 #define CS_SPI3V3_MOBCtoSUNSPZ_GPIO_Port GPIOE
 #define CS_SPI3V3_MOBCtoSUNSMZ_Pin GPIO_PIN_4
 #define CS_SPI3V3_MOBCtoSUNSMZ_GPIO_Port GPIOE
 #define MISO_SPI3V3_SUNStoMOBC_Pin GPIO_PIN_5
 #define MISO_SPI3V3_SUNStoMOBC_GPIO_Port GPIOE
 #define MOSI_SPI3V3_MOBCtoSUNS_Pin GPIO_PIN_6
 #define MOSI_SPI3V3_MOBCtoSUNS_GPIO_Port GPIOE
 #define RESET3V3_MOBCtoLORA_Pin GPIO_PIN_8
 #define RESET3V3_MOBCtoLORA_GPIO_Port GPIOI
 #define RESET3V3_MOBCtoGYRO_Pin GPIO_PIN_13
 #define RESET3V3_MOBCtoGYRO_GPIO_Port GPIOC
 #define OSC32_IN_Pin GPIO_PIN_14
 #define OSC32_IN_GPIO_Port GPIOC
 #define OSC32_OUT_Pin GPIO_PIN_15
 #define OSC32_OUT_GPIO_Port GPIOC
 #define DIO0_3V3_LORAtoMOBC_Pin GPIO_PIN_9
 #define DIO0_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define DIO1_3V3_LORAtoMOBC_Pin GPIO_PIN_10
 #define DIO1_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define DIO2_3V3_LORAtoMOBC_Pin GPIO_PIN_11
 #define DIO2_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define A0_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_0
 #define A0_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A1_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_1
 #define A1_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A2_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_2
 #define A2_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define DIO3_3V3_LORAtoMOBC_Pin GPIO_PIN_12
 #define DIO3_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define DIO4_3V3_LORAtoMOBC_Pin GPIO_PIN_13
 #define DIO4_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define DIO5_3V3_LORAtoMOBC_Pin GPIO_PIN_14
 #define DIO5_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define A3_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_3
 #define A3_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A4_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_4
 #define A4_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A5_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_5
 #define A5_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define RX_UART3V3_SILS_Pin GPIO_PIN_6
 #define RX_UART3V3_SILS_GPIO_Port GPIOF
 #define TX_UART3V3_SILS_Pin GPIO_PIN_7
 #define TX_UART3V3_SILS_GPIO_Port GPIOF
 #define MISO_SPI3V3_MRAM_LORAtoMOBC_Pin GPIO_PIN_8
 #define MISO_SPI3V3_MRAM_LORAtoMOBC_GPIO_Port GPIOF
 #define MOSI_SPI3V3_LORAtoMOBC_Pin GPIO_PIN_9
 #define MOSI_SPI3V3_LORAtoMOBC_GPIO_Port GPIOF
 #define CS_SPI3V3_MOBCtoMRAM_Pin GPIO_PIN_10
 #define CS_SPI3V3_MOBCtoMRAM_GPIO_Port GPIOF
 #define OSC_IN_Pin GPIO_PIN_0
 #define OSC_IN_GPIO_Port GPIOH
 #define OSC_OUT_Pin GPIO_PIN_1
 #define OSC_OUT_GPIO_Port GPIOH
 #define MOD_SPI2_CS_Pin GPIO_PIN_0
 #define MOD_SPI2_CS_GPIO_Port GPIOC
 #define MOD_SPI2_MOSI_Pin GPIO_PIN_1
 #define MOD_SPI2_MOSI_GPIO_Port GPIOC
 #define MOD_SPI2_MISO_Pin GPIO_PIN_2
 #define MOD_SPI2_MISO_GPIO_Port GPIOC
 #define ANA_CURS_MOBCtoMOBC_Pin GPIO_PIN_3
 #define ANA_CURS_MOBCtoMOBC_GPIO_Port GPIOC
 #define TX_UART3V3_MOBCtoGPS_M__AN_CMX_Pin GPIO_PIN_0
 #define TX_UART3V3_MOBCtoGPS_M__AN_CMX_GPIO_Port GPIOA
 #define RX_UART3V3_GPStoMOBC_Pin GPIO_PIN_1
 #define RX_UART3V3_GPStoMOBC_GPIO_Port GPIOA
 #define TX_UART3V3_MOBCtoCOBC__AN_CMZ3_Pin GPIO_PIN_2
 #define TX_UART3V3_MOBCtoCOBC__AN_CMZ3_GPIO_Port GPIOA
 #define TX_SOFTUART_MOBCtoTXPIC_M_Pin GPIO_PIN_2
 #define TX_SOFTUART_MOBCtoTXPIC_M_GPIO_Port GPIOH
 #define RX_SOFTUART_TXPICtoMOBC_M_Pin GPIO_PIN_3
 #define RX_SOFTUART_TXPICtoMOBC_M_GPIO_Port GPIOH
 #define SCL_I2C3V3_MOBCtoADCIOEX_Pin GPIO_PIN_4
 #define SCL_I2C3V3_MOBCtoADCIOEX_GPIO_Port GPIOH
 #define SDA_I2C3V3_MOBCtoADCIOEX_Pin GPIO_PIN_5
 #define SDA_I2C3V3_MOBCtoADCIOEX_GPIO_Port GPIOH
 #define RX_UART3V3_COBCtoMOBC__AN_CMZ4_Pin GPIO_PIN_3
 #define RX_UART3V3_COBCtoMOBC__AN_CMZ4_GPIO_Port GPIOA
 #define CS_SPI3V3_MOBCtoCOBC__AN_CMZ1_Pin GPIO_PIN_4
 #define CS_SPI3V3_MOBCtoCOBC__AN_CMZ1_GPIO_Port GPIOA
 #define SCK_SPI3V3_MOBCtoCOBC__AN_CMZ2_Pin GPIO_PIN_5
 #define SCK_SPI3V3_MOBCtoCOBC__AN_CMZ2_GPIO_Port GPIOA
 #define MISO_SPI3V3_COBCtoMOBC__PX_D_SC_Pin GPIO_PIN_6
 #define MISO_SPI3V3_COBCtoMOBC__PX_D_SC_GPIO_Port GPIOA
 #define MOSI_SPI3V3_MOBCtoCOBC__AN_GX_Pin GPIO_PIN_7
 #define MOSI_SPI3V3_MOBCtoCOBC__AN_GX_GPIO_Port GPIOA
 #define ANA_VOLS_GPStoMOBC_Pin GPIO_PIN_4
 #define ANA_VOLS_GPStoMOBC_GPIO_Port GPIOC
 #define ANA_VOLS_SUNStoMOBC_Pin GPIO_PIN_5
 #define ANA_VOLS_SUNStoMOBC_GPIO_Port GPIOC
 #define ANA_VOLS_LORAtoMOBC_Pin GPIO_PIN_0
 #define ANA_VOLS_LORAtoMOBC_GPIO_Port GPIOB
 #define ANA_CURS_LORAtoMOBC_Pin GPIO_PIN_1
 #define ANA_CURS_LORAtoMOBC_GPIO_Port GPIOB
 #define BOOT1_3V3_EXTtoMOBC_Pin GPIO_PIN_2
 #define BOOT1_3V3_EXTtoMOBC_GPIO_Port GPIOB
 #define CTL_OD5V_MOBCtoMUX_ANAS_Pin GPIO_PIN_15
 #define CTL_OD5V_MOBCtoMUX_ANAS_GPIO_Port GPIOI
 #define CTL_OD5V_MOBCtoMUX_TEMS_Pin GPIO_PIN_0
 #define CTL_OD5V_MOBCtoMUX_TEMS_GPIO_Port GPIOJ
 #define A0_OD5V_MOBCtoMUX_Pin GPIO_PIN_1
 #define A0_OD5V_MOBCtoMUX_GPIO_Port GPIOJ
 #define A1_OD5V_MOBCtoMUX_Pin GPIO_PIN_2
 #define A1_OD5V_MOBCtoMUX_GPIO_Port GPIOJ
 #define A2_OD5V_MOBCtoMUX_Pin GPIO_PIN_3
 #define A2_OD5V_MOBCtoMUX_GPIO_Port GPIOJ
 #define A3_OD5V_MOBCtoMUX_Pin GPIO_PIN_4
 #define A3_OD5V_MOBCtoMUX_GPIO_Port GPIOJ
 #define A4_OD5V_MOBCtoMUX_Pin GPIO_PIN_11
 #define A4_OD5V_MOBCtoMUX_GPIO_Port GPIOF
 #define A6_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_12
 #define A6_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A7_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_13
 #define A7_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A8_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_14
 #define A8_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A9_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_15
 #define A9_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A10_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_0
 #define A10_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define A11_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_1
 #define A11_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define D4_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_7
 #define D4_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOE
 #define D5_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_8
 #define D5_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOE
 #define D6_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_9
 #define D6_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOE
 #define D7_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_10
 #define D7_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOE
 #define GPIO3V3_6_MOBCtoDelta_Pin GPIO_PIN_11
 #define GPIO3V3_6_MOBCtoDelta_GPIO_Port GPIOE
 #define GPIO3V3_7_MOBCtoDelta_Pin GPIO_PIN_12
 #define GPIO3V3_7_MOBCtoDelta_GPIO_Port GPIOE
 #define GPIO3V3_8_MOBCtoDelta_Pin GPIO_PIN_13
 #define GPIO3V3_8_MOBCtoDelta_GPIO_Port GPIOE
 #define GPIO3V3_9_MOBCtoDelta_Pin GPIO_PIN_14
 #define GPIO3V3_9_MOBCtoDelta_GPIO_Port GPIOE
 #define GPIO3V3_10_MOBCtoDelta_Pin GPIO_PIN_15
 #define GPIO3V3_10_MOBCtoDelta_GPIO_Port GPIOE
 #define TX_UART3V3_MOBCtoTXPIC_M_Pin GPIO_PIN_10
 #define TX_UART3V3_MOBCtoTXPIC_M_GPIO_Port GPIOB
 #define RX_UART3V3_TXPICtoMOBC_M_Pin GPIO_PIN_11
 #define RX_UART3V3_TXPICtoMOBC_M_GPIO_Port GPIOB
 #define EN3V3_MOBCtoREG_CDH3V3_Pin GPIO_PIN_5
 #define EN3V3_MOBCtoREG_CDH3V3_GPIO_Port GPIOJ
 #define SCK_SPI3V3_MOBCtoMRAM_LORA_Pin GPIO_PIN_6
 #define SCK_SPI3V3_MOBCtoMRAM_LORA_GPIO_Port GPIOH
 #define D9_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_7
 #define D9_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOH
 #define RESET3V3_MOBCtoCOBC__HS_D_SC_Pin GPIO_PIN_8
 #define RESET3V3_MOBCtoCOBC__HS_D_SC_GPIO_Port GPIOH
 #define A0_3V3_MOBCtoADC_ACS1__D0_D_SC_Pin GPIO_PIN_9
 #define A0_3V3_MOBCtoADC_ACS1__D0_D_SC_GPIO_Port GPIOH
 #define AB_3V3_MOBCtoADC_ACS1__D1_D_SC_Pin GPIO_PIN_10
 #define AB_3V3_MOBCtoADC_ACS1__D1_D_SC_GPIO_Port GPIOH
 #define BYTESWAP3V3_MOBCtoADC_ACS1__D2_D_SC_Pin GPIO_PIN_11
 #define BYTESWAP3V3_MOBCtoADC_ACS1__D2_D_SC_GPIO_Port GPIOH
 #define RESET3V3_MOBCtoADC_ACS1__D3_D_SC_Pin GPIO_PIN_12
 #define RESET3V3_MOBCtoADC_ACS1__D3_D_SC_GPIO_Port GPIOH
 #define CNVST3V3_MOBCtoADC_ACS1_Pin GPIO_PIN_12
 #define CNVST3V3_MOBCtoADC_ACS1_GPIO_Port GPIOB
 #define MOD_SPI2_SCK_Pin GPIO_PIN_13
 #define MOD_SPI2_SCK_GPIO_Port GPIOB
 #define D0_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_14
 #define D0_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOB
 #define D1_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_15
 #define D1_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOB
 #define D2_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_8
 #define D2_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOD
 #define D3_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_9
 #define D3_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOD
 #define D4_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_10
 #define D4_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOD
 #define A16_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_11
 #define A16_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define A17_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_12
 #define A17_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define A18_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_13
 #define A18_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define D0_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_14
 #define D0_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define D1_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_15
 #define D1_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define A12_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_2
 #define A12_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define A13_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_3
 #define A13_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define A14_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_4
 #define A14_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define A15_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_5
 #define A15_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define D5_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_6
 #define D5_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOG
 #define D6_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_7
 #define D6_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOG
 #define D7_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_8
 #define D7_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOG
 #define TX_SOFTUART_MOBCtoRXPIC_M_Pin GPIO_PIN_6
 #define TX_SOFTUART_MOBCtoRXPIC_M_GPIO_Port GPIOC
 #define RX_SOFTUART_RXPICtoMOBC_M_Pin GPIO_PIN_7
 #define RX_SOFTUART_RXPICtoMOBC_M_GPIO_Port GPIOC
 #define D8_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_8
 #define D8_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOC
 #define SDA_I2C3V3_MOBCto9AXDAC_M__SD_I_9_D_SC_Pin GPIO_PIN_9
 #define SDA_I2C3V3_MOBCto9AXDAC_M__SD_I_9_D_SC_GPIO_Port GPIOC
 #define SCL_I2C3V3_MOBCto9AXDAC_M__SC_I_9_D_SC_Pin GPIO_PIN_8
 #define SCL_I2C3V3_MOBCto9AXDAC_M__SC_I_9_D_SC_GPIO_Port GPIOA
 #define TX_UART3V3_MOBCtoRXPIC_M_Pin GPIO_PIN_9
 #define TX_UART3V3_MOBCtoRXPIC_M_GPIO_Port GPIOA
 #define RX_UART3V3_RXPICtoMOBC_M_Pin GPIO_PIN_10
 #define RX_UART3V3_RXPICtoMOBC_M_GPIO_Port GPIOA
 #define D10_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_11
 #define D10_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOA
 #define D11_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_12
 #define D11_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOA
 #define TMS3V3_EXTtoMOBC_Pin GPIO_PIN_13
 #define TMS3V3_EXTtoMOBC_GPIO_Port GPIOA
 #define D12_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_13
 #define D12_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOH
 #define D13_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_14
 #define D13_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOH
 #define D14_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_15
 #define D14_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOH
 #define D15_3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_0
 #define D15_3V1_ADC_ACS1toMOBC_GPIO_Port GPIOI
 #define BUSY3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_1
 #define BUSY3V1_ADC_ACS1toMOBC_GPIO_Port GPIOI
 #define EOC3V1_ADC_ACS1toMOBC_Pin GPIO_PIN_2
 #define EOC3V1_ADC_ACS1toMOBC_GPIO_Port GPIOI
 #define SEL1_3V3_MOBCtoGYR_Pin GPIO_PIN_3
 #define SEL1_3V3_MOBCtoGYR_GPIO_Port GPIOI
 #define TCK3V3_EXTtoMOBC_Pin GPIO_PIN_14
 #define TCK3V3_EXTtoMOBC_GPIO_Port GPIOA
 #define CS_SPI3V3_MOBCtoSROM_Pin GPIO_PIN_15
 #define CS_SPI3V3_MOBCtoSROM_GPIO_Port GPIOA
 #define SCK_SPI3V3_MOBCtoSROM_Pin GPIO_PIN_10
 #define SCK_SPI3V3_MOBCtoSROM_GPIO_Port GPIOC
 #define MOD_GPIO_MOBC_6_Pin GPIO_PIN_11
 #define MOD_GPIO_MOBC_6_GPIO_Port GPIOC
 #define TX_UART3V3_MOBC_2UtoMOBC_1U_Pin GPIO_PIN_12
 #define TX_UART3V3_MOBC_2UtoMOBC_1U_GPIO_Port GPIOC
 #define D2_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_0
 #define D2_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define D3_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_1
 #define D3_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define RX_UART3V3_MOBC_1UtoMOBC_2U_Pin GPIO_PIN_2
 #define RX_UART3V3_MOBC_1UtoMOBC_2U_GPIO_Port GPIOD
 #define GATE3V3_MOBCtoFET_ADCIOEX_PWR2_Pin GPIO_PIN_3
 #define GATE3V3_MOBCtoFET_ADCIOEX_PWR2_GPIO_Port GPIOD
 #define NOE_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_4
 #define NOE_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define NWE_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_5
 #define NWE_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define MOSI_SPI3V3_MOBCtoSROM_Pin GPIO_PIN_6
 #define MOSI_SPI3V3_MOBCtoSROM_GPIO_Port GPIOD
 #define NE1_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_7
 #define NE1_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define MOD_GPIO_MOBC_1_Pin GPIO_PIN_12
 #define MOD_GPIO_MOBC_1_GPIO_Port GPIOJ
 #define MOD_GPIO_MOBC_2_Pin GPIO_PIN_13
 #define MOD_GPIO_MOBC_2_GPIO_Port GPIOJ
 #define PPS3V3_GPStoMOBC_Pin GPIO_PIN_14
 #define PPS3V3_GPStoMOBC_GPIO_Port GPIOJ
 #define RESET3V3_MOBCtoGPS_Pin GPIO_PIN_15
 #define RESET3V3_MOBCtoGPS_GPIO_Port GPIOJ
 #define GATE3V3_MOBCtoFET_GPS_Pin GPIO_PIN_9
 #define GATE3V3_MOBCtoFET_GPS_GPIO_Port GPIOG
 #define GATE3V3_MOBCtoFET_SUNS_Pin GPIO_PIN_10
 #define GATE3V3_MOBCtoFET_SUNS_GPIO_Port GPIOG
 #define CS_SPI3V3_MOBCtouSD_Pin GPIO_PIN_11
 #define CS_SPI3V3_MOBCtouSD_GPIO_Port GPIOG
 #define MISO_SPI3V3_uSDtoMOBC_Pin GPIO_PIN_12
 #define MISO_SPI3V3_uSDtoMOBC_GPIO_Port GPIOG
 #define SCK_SPI3V3_MOBCtouSD_Pin GPIO_PIN_13
 #define SCK_SPI3V3_MOBCtouSD_GPIO_Port GPIOG
 #define MOSI_SPI3V3_MOBCtouSD_Pin GPIO_PIN_14
 #define MOSI_SPI3V3_MOBCtouSD_GPIO_Port GPIOG
 #define GATE3V3_MOBCtoFET_MTQA_Pin GPIO_PIN_3
 #define GATE3V3_MOBCtoFET_MTQA_GPIO_Port GPIOK
 #define GATE3V3_MOBCtoFET_MTQO_Pin GPIO_PIN_4
 #define GATE3V3_MOBCtoFET_MTQO_GPIO_Port GPIOK
 #define GATE3V3_MOBCtoFET_SEPW1_Pin GPIO_PIN_5
 #define GATE3V3_MOBCtoFET_SEPW1_GPIO_Port GPIOK
 #define GATE3V3_MOBCtoFET_SEPW2_Pin GPIO_PIN_6
 #define GATE3V3_MOBCtoFET_SEPW2_GPIO_Port GPIOK
 #define BOOT1_3V3_MOBCtoCOBC_Pin GPIO_PIN_7
 #define BOOT1_3V3_MOBCtoCOBC_GPIO_Port GPIOK
 #define SEL0_3V3_MOBCtoGYRO_Pin GPIO_PIN_15
 #define SEL0_3V3_MOBCtoGYRO_GPIO_Port GPIOG
 #define CS_SPI3V3_MOBCtoLORA_Pin GPIO_PIN_3
 #define CS_SPI3V3_MOBCtoLORA_GPIO_Port GPIOB
 #define MISO_SPI3V3_SROMtoMOBC_Pin GPIO_PIN_4
 #define MISO_SPI3V3_SROMtoMOBC_GPIO_Port GPIOB
 #define GATE3V3_MOBCtoFET_LORA_Pin GPIO_PIN_5
 #define GATE3V3_MOBCtoFET_LORA_GPIO_Port GPIOB
 #define SCL_I2C3V3_MOBCtoEROM_Pin GPIO_PIN_6
 #define SCL_I2C3V3_MOBCtoEROM_GPIO_Port GPIOB
 #define SDA_I2C3V3_MOBCtoEROM_Pin GPIO_PIN_7
 #define SDA_I2C3V3_MOBCtoEROM_GPIO_Port GPIOB
 #define GPIO3V3_4_MOBCtoDelta_Pin GPIO_PIN_8
 #define GPIO3V3_4_MOBCtoDelta_GPIO_Port GPIOB
 #define GPIO3V3_5_MOBCtoDelta_Pin GPIO_PIN_9
 #define GPIO3V3_5_MOBCtoDelta_GPIO_Port GPIOB
 #define RX_UART3V3_DBG_Pin GPIO_PIN_0
 #define RX_UART3V3_DBG_GPIO_Port GPIOE
 #define TX_UART3V3_DBG_Pin GPIO_PIN_1
 #define TX_UART3V3_DBG_GPIO_Port GPIOE
 #define CS_SPI3V3_MOBCtoSUNSPX_M__D5_D_SC_Pin GPIO_PIN_4
 #define CS_SPI3V3_MOBCtoSUNSPX_M__D5_D_SC_GPIO_Port GPIOI
 #define CS_SPI3V3_MOBCtoSUNSMX_M__VS_D_SC_Pin GPIO_PIN_5
 #define CS_SPI3V3_MOBCtoSUNSMX_M__VS_D_SC_GPIO_Port GPIOI
 #define CS_SPI3V3_MOBCtoSUNSPY_Pin GPIO_PIN_6
 #define CS_SPI3V3_MOBCtoSUNSPY_GPIO_Port GPIOI
 #define CS_SPI3V3_MOBCtoSUNSMY_Pin GPIO_PIN_7
 #define CS_SPI3V3_MOBCtoSUNSMY_GPIO_Port GPIOI
#endif //TWO_U

#ifdef ONE_U
 #define SCK_SPI3V3_SUNS_Pin GPIO_PIN_2
 #define SCK_SPI3V3_SUNS_GPIO_Port GPIOE
 #define CS_SPI3V3_SUNSPZ_Pin GPIO_PIN_3
 #define CS_SPI3V3_SUNSPZ_GPIO_Port GPIOE
 #define CS_SPI3V3_SUNSMZ_Pin GPIO_PIN_4
 #define CS_SPI3V3_SUNSMZ_GPIO_Port GPIOE
 #define MISO_SPI3V3_SUNS_Pin GPIO_PIN_5
 #define MISO_SPI3V3_SUNS_GPIO_Port GPIOE
 #define MOSI_SPI3V3_SUNS_Pin GPIO_PIN_6
 #define MOSI_SPI3V3_SUNS_GPIO_Port GPIOE
 #define RESET3V3_MOBCtoLORA_Pin GPIO_PIN_8
 #define RESET3V3_MOBCtoLORA_GPIO_Port GPIOI
 #define RESET3V3_MOBCtoGYRO_Pin GPIO_PIN_13
 #define RESET3V3_MOBCtoGYRO_GPIO_Port GPIOC
 #define OSC32_IN_Pin GPIO_PIN_14
 #define OSC32_IN_GPIO_Port GPIOC
 #define OSC32_OUT_Pin GPIO_PIN_15
 #define OSC32_OUT_GPIO_Port GPIOC
 #define DIO0_3V3_LORAtoMOBC_Pin GPIO_PIN_9
 #define DIO0_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define DIO1_3V3_LORAtoMOBC_Pin GPIO_PIN_10
 #define DIO1_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define DIO2_3V3_LORAtoMOBC_Pin GPIO_PIN_11
 #define DIO2_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define A0_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_0
 #define A0_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A1_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_1
 #define A1_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A2_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_2
 #define A2_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define DIO3_3V3_LORAtoMOBC_Pin GPIO_PIN_12
 #define DIO3_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define DIO4_3V3_LORAtoMOBC_Pin GPIO_PIN_13
 #define DIO4_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define DIO5_3V3_LORAtoMOBC_Pin GPIO_PIN_14
 #define DIO5_3V3_LORAtoMOBC_GPIO_Port GPIOI
 #define A3_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_3
 #define A3_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A4_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_4
 #define A4_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A5_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_5
 #define A5_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define RX_UART3V3_SILS_Pin GPIO_PIN_6
 #define RX_UART3V3_SILS_GPIO_Port GPIOF
 #define TX_UART3V3_SILS_Pin GPIO_PIN_7
 #define TX_UART3V3_SILS_GPIO_Port GPIOF
 #define MISO_SPI3V3_MRAM_LORAtoMOBC_Pin GPIO_PIN_8
 #define MISO_SPI3V3_MRAM_LORAtoMOBC_GPIO_Port GPIOF
 #define MOSI_SPI3V3_MOBCtoMRAM_LORA_Pin GPIO_PIN_9
 #define MOSI_SPI3V3_MOBCtoMRAM_LORA_GPIO_Port GPIOF
 #define CS_SPI3V3_MOBCtoMRAM_Pin GPIO_PIN_10
 #define CS_SPI3V3_MOBCtoMRAM_GPIO_Port GPIOF
 #define OSC_IN_Pin GPIO_PIN_0
 #define OSC_IN_GPIO_Port GPIOH
 #define OSC_OUT_Pin GPIO_PIN_1
 #define OSC_OUT_GPIO_Port GPIOH
 #define ANA_VOLS_MTQA_1UtoMOBC_Pin GPIO_PIN_0
 #define ANA_VOLS_MTQA_1UtoMOBC_GPIO_Port GPIOC
 #define ANA_GYROY_MOBC_Pin GPIO_PIN_1
 #define ANA_GYROY_MOBC_GPIO_Port GPIOC
 #define ANA_GYROZ_MOBC_Pin GPIO_PIN_2
 #define ANA_GYROZ_MOBC_GPIO_Port GPIOC
 #define ANA_CURS_MOBCtoMOBC_Pin GPIO_PIN_3
 #define ANA_CURS_MOBCtoMOBC_GPIO_Port GPIOC
 #define ANA_CURS_MTQX_MOBC_Pin GPIO_PIN_0
 #define ANA_CURS_MTQX_MOBC_GPIO_Port GPIOA
 #define ANA_CURS_MTQY_MOBC_Pin GPIO_PIN_1
 #define ANA_CURS_MTQY_MOBC_GPIO_Port GPIOA
 #define ANA_CURS_MTQZ1_MOBC_Pin GPIO_PIN_2
 #define ANA_CURS_MTQZ1_MOBC_GPIO_Port GPIOA
 #define TX_SOFTUART_MOBCtoTXPIC_M_Pin GPIO_PIN_2
 #define TX_SOFTUART_MOBCtoTXPIC_M_GPIO_Port GPIOH
 #define RX_SOFTUART_TXPICtoMOBC_M_Pin GPIO_PIN_3
 #define RX_SOFTUART_TXPICtoMOBC_M_GPIO_Port GPIOH
 #define SCL_I2C3V3_MOBCtoADCIOEX_Pin GPIO_PIN_4
 #define SCL_I2C3V3_MOBCtoADCIOEX_GPIO_Port GPIOH
 #define SDA_I2C3V3_MOBCtoADCIOEX_Pin GPIO_PIN_5
 #define SDA_I2C3V3_MOBCtoADCIOEX_GPIO_Port GPIOH
 #define ANA_CURS_MTQZ2_MOBC_Pin GPIO_PIN_3
 #define ANA_CURS_MTQZ2_MOBC_GPIO_Port GPIOA
 #define ANA_CURS_MTQZ3_MOBC_Pin GPIO_PIN_4
 #define ANA_CURS_MTQZ3_MOBC_GPIO_Port GPIOA
 #define ANA_CURS_MTQZ4_MOBC_Pin GPIO_PIN_5
 #define ANA_CURS_MTQZ4_MOBC_GPIO_Port GPIOA
 #define ANA_GYROXtoMOBC_Pin GPIO_PIN_7
 #define ANA_GYROXtoMOBC_GPIO_Port GPIOA
 #define ANA_VOLS_GPStoMOBC_Pin GPIO_PIN_4
 #define ANA_VOLS_GPStoMOBC_GPIO_Port GPIOC
 #define ANA_VOLS_SUNStoMOBC_Pin GPIO_PIN_5
 #define ANA_VOLS_SUNStoMOBC_GPIO_Port GPIOC
 #define ANA_VOLS_LORAtoMOBC_Pin GPIO_PIN_0
 #define ANA_VOLS_LORAtoMOBC_GPIO_Port GPIOB
 #define ANA_CURS_LORAtoMOBC_Pin GPIO_PIN_1
 #define ANA_CURS_LORAtoMOBC_GPIO_Port GPIOB
 #define A6_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_12
 #define A6_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A7_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_13
 #define A7_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A8_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_14
 #define A8_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A9_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_15
 #define A9_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOF
 #define A10_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_0
 #define A10_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define A11_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_1
 #define A11_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define D4_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_7
 #define D4_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOE
 #define D5_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_8
 #define D5_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOE
 #define D6_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_9
 #define D6_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOE
 #define D7_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_10
 #define D7_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOE
 #define GPIO3V3_6_MOBCtoDelta_Pin GPIO_PIN_11
 #define GPIO3V3_6_MOBCtoDelta_GPIO_Port GPIOE
 #define STANDBY3V3_SCAM1U_Pin GPIO_PIN_12
 #define STANDBY3V3_SCAM1U_GPIO_Port GPIOE
 #define RESET3V3_SCAM1U_Pin GPIO_PIN_13
 #define RESET3V3_SCAM1U_GPIO_Port GPIOE
 #define CS_SPI3V3_SUNSMX_Pin GPIO_PIN_14
 #define CS_SPI3V3_SUNSMX_GPIO_Port GPIOE
 #define CS_SPI3V3_SUNSPX_Pin GPIO_PIN_15
 #define CS_SPI3V3_SUNSPX_GPIO_Port GPIOE
 #define TX_UART3V3_MOBCtoTXPIC_M_Pin GPIO_PIN_10
 #define TX_UART3V3_MOBCtoTXPIC_M_GPIO_Port GPIOB
 #define RX_UART3V3_TXPICtoMOBC_M_Pin GPIO_PIN_11
 #define RX_UART3V3_TXPICtoMOBC_M_GPIO_Port GPIOB
 #define EN3V3_MOBCtoREG_CDH3V3_Pin GPIO_PIN_5
 #define EN3V3_MOBCtoREG_CDH3V3_GPIO_Port GPIOJ
 #define SCK_SPI3V3_MOBCtoMRAM_LORA_Pin GPIO_PIN_6
 #define SCK_SPI3V3_MOBCtoMRAM_LORA_GPIO_Port GPIOH
 #define HSYNC_DCMI3V3_SCAM1U_Pin GPIO_PIN_8
 #define HSYNC_DCMI3V3_SCAM1U_GPIO_Port GPIOH
 #define D0_DCMI3V3_SCAM1U_Pin GPIO_PIN_9
 #define D0_DCMI3V3_SCAM1U_GPIO_Port GPIOH
 #define D1_DCMI3V3_SCAM2U_Pin GPIO_PIN_10
 #define D1_DCMI3V3_SCAM2U_GPIO_Port GPIOH
 #define D2_DCMI3V3_SCAM3U_Pin GPIO_PIN_11
 #define D2_DCMI3V3_SCAM3U_GPIO_Port GPIOH
 #define D3_DCMI3V3_SCAM3U_Pin GPIO_PIN_12
 #define D3_DCMI3V3_SCAM3U_GPIO_Port GPIOH
 #define A16_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_11
 #define A16_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define A17_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_12
 #define A17_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define A18_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_13
 #define A18_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define D0_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_14
 #define D0_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define D1_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_15
 #define D1_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define A12_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_2
 #define A12_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define A13_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_3
 #define A13_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define A14_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_4
 #define A14_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define A15_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_5
 #define A15_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOG
 #define TX_SOFTUART_MOBCtoRXPIC_M_Pin GPIO_PIN_6
 #define TX_SOFTUART_MOBCtoRXPIC_M_GPIO_Port GPIOC
 #define RX_SOFTUART_RXPICtoMOBC_M_Pin GPIO_PIN_7
 #define RX_SOFTUART_RXPICtoMOBC_M_GPIO_Port GPIOC
 #define SDA_I2C3V3_MOBCto9AXDACSCAM1U_M__SD_I_9_D_SC_Pin GPIO_PIN_9
 #define SDA_I2C3V3_MOBCto9AXDACSCAM1U_M__SD_I_9_D_SC_GPIO_Port GPIOC
 #define SCL_I2C3V3_MOBCto9AXDACSCAM1U_M__SC_I_9_D_SC_Pin GPIO_PIN_8
 #define SCL_I2C3V3_MOBCto9AXDACSCAM1U_M__SC_I_9_D_SC_GPIO_Port GPIOA
 #define TX_UART3V3_MOBCtoRXPIC_M_Pin GPIO_PIN_9
 #define TX_UART3V3_MOBCtoRXPIC_M_GPIO_Port GPIOA
 #define RX_UART3V3_RXPICtoMOBC_M_Pin GPIO_PIN_10
 #define RX_UART3V3_RXPICtoMOBC_M_GPIO_Port GPIOA
 #define TMS3V3_EXTtoMOBC_Pin GPIO_PIN_13
 #define TMS3V3_EXTtoMOBC_GPIO_Port GPIOA
 #define SEL1_3V3_MOBCtoGYR_Pin GPIO_PIN_3
 #define SEL1_3V3_MOBCtoGYR_GPIO_Port GPIOI
 #define TCK3V3_EXTtoMOBC_Pin GPIO_PIN_14
 #define TCK3V3_EXTtoMOBC_GPIO_Port GPIOA
 #define CS_SPI3V3_MOBCtoSROM_Pin GPIO_PIN_15
 #define CS_SPI3V3_MOBCtoSROM_GPIO_Port GPIOA
 #define SCK_SPI3V3_MOBCtoSROM_Pin GPIO_PIN_10
 #define SCK_SPI3V3_MOBCtoSROM_GPIO_Port GPIOC
 #define D4_DCMI3V3_SCAM1U_Pin GPIO_PIN_11
 #define D4_DCMI3V3_SCAM1U_GPIO_Port GPIOC
 #define RX_UART3V3_MOBC_2UtoMOBC_1U_Pin GPIO_PIN_12
 #define RX_UART3V3_MOBC_2UtoMOBC_1U_GPIO_Port GPIOC
 #define D2_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_0
 #define D2_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define D3_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_1
 #define D3_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define TX_UART3V3_MOBC_1UtoMOBC_2U_Pin GPIO_PIN_2
 #define TX_UART3V3_MOBC_1UtoMOBC_2U_GPIO_Port GPIOD
 #define GATE3V3_MOBCtoFET_ADCIOEX_PWR2_Pin GPIO_PIN_3
 #define GATE3V3_MOBCtoFET_ADCIOEX_PWR2_GPIO_Port GPIOD
 #define NOE_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_4
 #define NOE_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define NWE_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_5
 #define NWE_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define MOSI_SPI3V3_MOBCtoSROM_Pin GPIO_PIN_6
 #define MOSI_SPI3V3_MOBCtoSROM_GPIO_Port GPIOD
 #define NE1_FMC3V3_MOBCtoSRAM_Pin GPIO_PIN_7
 #define NE1_FMC3V3_MOBCtoSRAM_GPIO_Port GPIOD
 #define RX_SOFTUART3V3_GPS_Pin GPIO_PIN_12
 #define RX_SOFTUART3V3_GPS_GPIO_Port GPIOJ
 #define TX_SOFTUART3V3_GPS_Pin GPIO_PIN_13
 #define TX_SOFTUART3V3_GPS_GPIO_Port GPIOJ
 #define PPS3V3_GPStoMOBC_Pin GPIO_PIN_14
 #define PPS3V3_GPStoMOBC_GPIO_Port GPIOJ
 #define RESET3V3_MOBCtoGPS_Pin GPIO_PIN_15
 #define RESET3V3_MOBCtoGPS_GPIO_Port GPIOJ
 #define GATE3V3_MOBCtoFET_GPS_Pin GPIO_PIN_9
 #define GATE3V3_MOBCtoFET_GPS_GPIO_Port GPIOG
 #define GATE3V3_MOBCtoFET_SUNS_Pin GPIO_PIN_10
 #define GATE3V3_MOBCtoFET_SUNS_GPIO_Port GPIOG
 #define CS_SPI3V3_MOBCtouSD_Pin GPIO_PIN_11
 #define CS_SPI3V3_MOBCtouSD_GPIO_Port GPIOG
 #define MISO_SPI3V3_uSDtoMOBC_Pin GPIO_PIN_12
 #define MISO_SPI3V3_uSDtoMOBC_GPIO_Port GPIOG
 #define SCK_SPI3V3_MOBCtouSD_Pin GPIO_PIN_13
 #define SCK_SPI3V3_MOBCtouSD_GPIO_Port GPIOG
 #define MOSI_SPI3V3_MOBCtouSD_Pin GPIO_PIN_14
 #define MOSI_SPI3V3_MOBCtouSD_GPIO_Port GPIOG
 #define GATE3V3_MOBCtoFET_MTQA_Pin GPIO_PIN_3
 #define GATE3V3_MOBCtoFET_MTQA_GPIO_Port GPIOK
 #define SEL0_3V3_MOBCtoGYRO_Pin GPIO_PIN_15
 #define SEL0_3V3_MOBCtoGYRO_GPIO_Port GPIOG
 #define CS_SPI3V3_MOBCtoLORA_Pin GPIO_PIN_3
 #define CS_SPI3V3_MOBCtoLORA_GPIO_Port GPIOB
 #define MISO_SPI3V3_SROMtoMOBC_Pin GPIO_PIN_4
 #define MISO_SPI3V3_SROMtoMOBC_GPIO_Port GPIOB
 #define GATE3V3_MOBCtoFET_LORA_Pin GPIO_PIN_5
 #define GATE3V3_MOBCtoFET_LORA_GPIO_Port GPIOB
 #define SCL_I2C3V3_MOBCtoEROM_Pin GPIO_PIN_6
 #define SCL_I2C3V3_MOBCtoEROM_GPIO_Port GPIOB
 #define SDA_I2C3V3_MOBCtoEROM_Pin GPIO_PIN_7
 #define SDA_I2C3V3_MOBCtoEROM_GPIO_Port GPIOB
 #define D5_DCMI3V3_SCAM1U_Pin GPIO_PIN_8
 #define D5_DCMI3V3_SCAM1U_GPIO_Port GPIOB
 #define D6_DCMI3V3_SCAM1U_Pin GPIO_PIN_9
 #define D6_DCMI3V3_SCAM1U_GPIO_Port GPIOB
 #define RX_UART3V3_DBG_Pin GPIO_PIN_0
 #define RX_UART3V3_DBG_GPIO_Port GPIOE
 #define TX_UART3V3_DBG_Pin GPIO_PIN_1
 #define TX_UART3V3_DBG_GPIO_Port GPIOE
 #define D5_DCMI3V3_SCAM1UI4_Pin GPIO_PIN_4
 #define D5_DCMI3V3_SCAM1UI4_GPIO_Port GPIOI
 #define VSYNC_DCMI3V3_SCAM1U_Pin GPIO_PIN_5
 #define VSYNC_DCMI3V3_SCAM1U_GPIO_Port GPIOI
 #define CS_SPI3V3_SUNSPY_Pin GPIO_PIN_6
 #define CS_SPI3V3_SUNSPY_GPIO_Port GPIOI
 #define CS_SPI3V3_SUNSMY_Pin GPIO_PIN_7
 #define CS_SPI3V3_SUNSMY_GPIO_Port GPIOI

 #endif

 typedef enum
 {
   HAL_OK       = 0x00U,
   HAL_ERROR    = 0x01U,
   HAL_BUSY     = 0x02U,
   HAL_TIMEOUT  = 0x03U
 } HAL_StatusTypeDef;

 typedef enum
 {
   HAL_UART_STATE_RESET             = 0x00U,    /*!< Peripheral is not yet Initialized
                                                    Value is allowed for gState and RxState */
   HAL_UART_STATE_READY             = 0x20U,    /*!< Peripheral Initialized and ready for use
                                                    Value is allowed for gState and RxState */
   HAL_UART_STATE_BUSY              = 0x24U,    /*!< an internal process is ongoing
                                                    Value is allowed for gState only */
   HAL_UART_STATE_BUSY_TX           = 0x21U,    /*!< Data Transmission process is ongoing
                                                    Value is allowed for gState only */
   HAL_UART_STATE_BUSY_RX           = 0x22U,    /*!< Data Reception process is ongoing
                                                    Value is allowed for RxState only */
   HAL_UART_STATE_BUSY_TX_RX        = 0x23U,    /*!< Data Transmission and Reception process is ongoing
                                                    Not to be used for neither gState nor RxState.
                                                    Value is result of combination (Or) between gState and RxState values */
   HAL_UART_STATE_TIMEOUT           = 0xA0U,    /*!< Timeout state
                                                    Value is allowed for gState only */
   HAL_UART_STATE_ERROR             = 0xE0U     /*!< Error
                                                    Value is allowed for gState only */
 } HAL_UART_StateTypeDef;

 typedef enum
 {
   HAL_I2C_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
   HAL_I2C_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use  */
   HAL_I2C_STATE_BUSY              = 0x24U,   /*!< An internal process is ongoing            */
   HAL_I2C_STATE_BUSY_TX           = 0x21U,   /*!< Data Transmission process is ongoing      */
   HAL_I2C_STATE_BUSY_RX           = 0x22U,   /*!< Data Reception process is ongoing         */
   HAL_I2C_STATE_LISTEN            = 0x28U,   /*!< Address Listen Mode is ongoing            */
   HAL_I2C_STATE_BUSY_TX_LISTEN    = 0x29U,   /*!< Address Listen Mode and Data Transmission
                                                  process is ongoing                         */
   HAL_I2C_STATE_BUSY_RX_LISTEN    = 0x2AU,   /*!< Address Listen Mode and Data Reception
                                                  process is ongoing                         */
   HAL_I2C_STATE_ABORT             = 0x60U,   /*!< Abort user request ongoing                */
   HAL_I2C_STATE_TIMEOUT           = 0xA0U,   /*!< Timeout state                             */
   HAL_I2C_STATE_ERROR             = 0xE0U    /*!< Error                                     */

 } HAL_I2C_StateTypeDef;

 typedef enum
 {
   HAL_SPI_STATE_RESET      = 0x00U,    /*!< Peripheral not Initialized                         */
   HAL_SPI_STATE_READY      = 0x01U,    /*!< Peripheral Initialized and ready for use           */
   HAL_SPI_STATE_BUSY       = 0x02U,    /*!< an internal process is ongoing                     */
   HAL_SPI_STATE_BUSY_TX    = 0x03U,    /*!< Data Transmission process is ongoing               */
   HAL_SPI_STATE_BUSY_RX    = 0x04U,    /*!< Data Reception process is ongoing                  */
   HAL_SPI_STATE_BUSY_TX_RX = 0x05U,    /*!< Data Transmission and Reception process is ongoing */
   HAL_SPI_STATE_ERROR      = 0x06U,    /*!< SPI error state                                    */
   HAL_SPI_STATE_ABORT      = 0x07U     /*!< SPI abort is ongoing                               */
 } HAL_SPI_StateTypeDef;

#endif //HAL_SILS

#endif

