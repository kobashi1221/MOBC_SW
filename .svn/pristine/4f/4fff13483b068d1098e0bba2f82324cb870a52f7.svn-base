/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <src_user/Drivers/CDH/reset_counter.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi4;
extern SPI_HandleTypeDef hspi5;
extern SPI_HandleTypeDef hspi6;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern RTC_HandleTypeDef hrtc;

extern uint16_t adc1_buf[12]; //park
extern DCMI_HandleTypeDef hdcmi;
extern IWDG_HandleTypeDef hiwdg; //K.K.
extern reset_cause_t reset_cause;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void User_Error_Handler(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SCK_SPI3V3_MOBCtoSUNS_SOBC_Pin GPIO_PIN_2
#define SCK_SPI3V3_MOBCtoSUNS_SOBC_GPIO_Port GPIOE
#define GPIO_3V3_MOBCtoSOBC_Pin GPIO_PIN_3
#define GPIO_3V3_MOBCtoSOBC_GPIO_Port GPIOE
#define GPIO3V3_10_MOBCtoDelta_Pin GPIO_PIN_4
#define GPIO3V3_10_MOBCtoDelta_GPIO_Port GPIOE
#define MISO_SPI3V3_SUNS_SOBCtoMOBC_Pin GPIO_PIN_5
#define MISO_SPI3V3_SUNS_SOBCtoMOBC_GPIO_Port GPIOE
#define MOSI_SPI3V3_MOBCtoSUNS_SOBC_Pin GPIO_PIN_6
#define MOSI_SPI3V3_MOBCtoSUNS_SOBC_GPIO_Port GPIOE
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
#define TX_UART3V3_MOBCtoGPS_Pin GPIO_PIN_0
#define TX_UART3V3_MOBCtoGPS_GPIO_Port GPIOA
#define RX_UART3V3_GPStoMOBC_Pin GPIO_PIN_1
#define RX_UART3V3_GPStoMOBC_GPIO_Port GPIOA
#define ANA_CURS_MTQZ34_MOBC_Pin GPIO_PIN_2
#define ANA_CURS_MTQZ34_MOBC_GPIO_Port GPIOA
#define TX_SOFTUART_MOBCtoTXPIC_M_Pin GPIO_PIN_2
#define TX_SOFTUART_MOBCtoTXPIC_M_GPIO_Port GPIOH
#define RX_SOFTUART_TXPICtoMOBC_M_Pin GPIO_PIN_3
#define RX_SOFTUART_TXPICtoMOBC_M_GPIO_Port GPIOH
#define SCL_I2C3V3_MOBCtoADCIOEX_Pin GPIO_PIN_4
#define SCL_I2C3V3_MOBCtoADCIOEX_GPIO_Port GPIOH
#define SDA_I2C3V3_MOBCtoADCIOEX_Pin GPIO_PIN_5
#define SDA_I2C3V3_MOBCtoADCIOEX_GPIO_Port GPIOH
#define ANA_CURS_MTQX_MOBC_Pin GPIO_PIN_3
#define ANA_CURS_MTQX_MOBC_GPIO_Port GPIOA
#define ANA_CURS_MTQY_MOBC_Pin GPIO_PIN_4
#define ANA_CURS_MTQY_MOBC_GPIO_Port GPIOA
#define ANA_CURS_MTQZ12_MOBC_Pin GPIO_PIN_5
#define ANA_CURS_MTQZ12_MOBC_GPIO_Port GPIOA
#define ANA_GYROXtoMOBC_Pin GPIO_PIN_7
#define ANA_GYROXtoMOBC_GPIO_Port GPIOA
#define ANA_VOLS_GPStoMOBC_Pin GPIO_PIN_4
#define ANA_VOLS_GPStoMOBC_GPIO_Port GPIOC
#define SEL0_3V3_MOBCtoGYROY_Pin GPIO_PIN_5
#define SEL0_3V3_MOBCtoGYROY_GPIO_Port GPIOC
#define ANA_VOLS_LORAtoMOBC_Pin GPIO_PIN_0
#define ANA_VOLS_LORAtoMOBC_GPIO_Port GPIOB
#define ANA_CURS_LORAtoMOBC_Pin GPIO_PIN_1
#define ANA_CURS_LORAtoMOBC_GPIO_Port GPIOB
#define PSTATE_PLASMAtoMOBC_Pin GPIO_PIN_0
#define PSTATE_PLASMAtoMOBC_GPIO_Port GPIOJ
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
#define CS_SPI3V3_MOBCtoSOBC_Pin GPIO_PIN_11
#define CS_SPI3V3_MOBCtoSOBC_GPIO_Port GPIOE
#define RESET3V3_SCAM_Pin GPIO_PIN_12
#define RESET3V3_SCAM_GPIO_Port GPIOE
#define PWDN3V3_SCAM1U_Pin GPIO_PIN_13
#define PWDN3V3_SCAM1U_GPIO_Port GPIOE
#define CS_SPI3V3_SUNSPY_Pin GPIO_PIN_14
#define CS_SPI3V3_SUNSPY_GPIO_Port GPIOE
#define RESET3V3_MOBCtoSOBC_Pin GPIO_PIN_15
#define RESET3V3_MOBCtoSOBC_GPIO_Port GPIOE
#define TX_UART3V3_MOBCtoTXPIC_M_Pin GPIO_PIN_10
#define TX_UART3V3_MOBCtoTXPIC_M_GPIO_Port GPIOB
#define RX_UART3V3_TXPICtoMOBC_M_Pin GPIO_PIN_11
#define RX_UART3V3_TXPICtoMOBC_M_GPIO_Port GPIOB
#define EN3V3_MOBCtoREG_CDH3V3_Pin GPIO_PIN_5
#define EN3V3_MOBCtoREG_CDH3V3_GPIO_Port GPIOJ
#define SCK_SPI3V3_MOBCtoMRAM_LORA_Pin GPIO_PIN_6
#define SCK_SPI3V3_MOBCtoMRAM_LORA_GPIO_Port GPIOH
#define HREF_DCMI3V3_SCAM1U_Pin GPIO_PIN_8
#define HREF_DCMI3V3_SCAM1U_GPIO_Port GPIOH
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
#define SDA_I2C3V3_MOBCto9AXDACSCAM1U_Pin GPIO_PIN_9
#define SDA_I2C3V3_MOBCto9AXDACSCAM1U_GPIO_Port GPIOC
#define SCL_I2C3V3_MOBCto9AXDACSCAM1U_Pin GPIO_PIN_8
#define SCL_I2C3V3_MOBCto9AXDACSCAM1U_GPIO_Port GPIOA
#define TX_UART3V3_MOBCtoRXPIC_M_Pin GPIO_PIN_9
#define TX_UART3V3_MOBCtoRXPIC_M_GPIO_Port GPIOA
#define RX_UART3V3_RXPICtoMOBC_M_Pin GPIO_PIN_10
#define RX_UART3V3_RXPICtoMOBC_M_GPIO_Port GPIOA
#define TMS3V3_EXTtoMOBC_Pin GPIO_PIN_13
#define TMS3V3_EXTtoMOBC_GPIO_Port GPIOA
#define SEL1_3V3_MOBCtoGYROX_Pin GPIO_PIN_3
#define SEL1_3V3_MOBCtoGYROX_GPIO_Port GPIOI
#define TCK3V3_EXTtoMOBC_Pin GPIO_PIN_14
#define TCK3V3_EXTtoMOBC_GPIO_Port GPIOA
#define CS_SPI3V3_MOBCtoSROM_Pin GPIO_PIN_15
#define CS_SPI3V3_MOBCtoSROM_GPIO_Port GPIOA
#define SCK_SPI3V3_MOBCtoSROM_Pin GPIO_PIN_10
#define SCK_SPI3V3_MOBCtoSROM_GPIO_Port GPIOC
#define D4_DCMI3V3_SCAM1U_Pin GPIO_PIN_11
#define D4_DCMI3V3_SCAM1U_GPIO_Port GPIOC
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
#define GPIO3V3_SEPD_MSWtoMOBC_Pin GPIO_PIN_12
#define GPIO3V3_SEPD_MSWtoMOBC_GPIO_Port GPIOJ
#define GPIO3V3_SEPD_MSWtoMOBC_EXTI_IRQn EXTI15_10_IRQn
#define GPIO3V3_SEPD_MILLMAXtoMOBC_Pin GPIO_PIN_13
#define GPIO3V3_SEPD_MILLMAXtoMOBC_GPIO_Port GPIOJ
#define GPIO3V3_SEPD_MILLMAXtoMOBC_EXTI_IRQn EXTI15_10_IRQn
#define PPS3V3_GPStoMOBC_Pin GPIO_PIN_14
#define PPS3V3_GPStoMOBC_GPIO_Port GPIOJ
#define PPS3V3_GPStoMOBC_EXTI_IRQn EXTI15_10_IRQn
#define RESET3V3_MOBCtoGPS_Pin GPIO_PIN_15
#define RESET3V3_MOBCtoGPS_GPIO_Port GPIOJ
#define GATE3V3_MOBCtoFET_GPS_Pin GPIO_PIN_9
#define GATE3V3_MOBCtoFET_GPS_GPIO_Port GPIOG
#define SEL1_3V3_MOBCtoGYROY_Pin GPIO_PIN_10
#define SEL1_3V3_MOBCtoGYROY_GPIO_Port GPIOG
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
#define GATE3V3_MOBCtoFET_PLASMA_Pin GPIO_PIN_4
#define GATE3V3_MOBCtoFET_PLASMA_GPIO_Port GPIOK
#define SEL0_3V3_MOBCtoGYROZ_Pin GPIO_PIN_5
#define SEL0_3V3_MOBCtoGYROZ_GPIO_Port GPIOK
#define SEL1_3V3_MOBCtoGYROZ_Pin GPIO_PIN_6
#define SEL1_3V3_MOBCtoGYROZ_GPIO_Port GPIOK
#define SEL0_3V3_MOBCtoGYROX_Pin GPIO_PIN_15
#define SEL0_3V3_MOBCtoGYROX_GPIO_Port GPIOG
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
#define D6_DCMI3V3_SCAM1U_Pin GPIO_PIN_8
#define D6_DCMI3V3_SCAM1U_GPIO_Port GPIOB
#define D7_DCMI3V3_SCAM1U_Pin GPIO_PIN_9
#define D7_DCMI3V3_SCAM1U_GPIO_Port GPIOB
#define RX_UART3V3_DBG_Pin GPIO_PIN_0
#define RX_UART3V3_DBG_GPIO_Port GPIOE
#define TX_UART3V3_DBG_Pin GPIO_PIN_1
#define TX_UART3V3_DBG_GPIO_Port GPIOE
#define D5_DCMI3V3_SCAM1U_Pin GPIO_PIN_4
#define D5_DCMI3V3_SCAM1U_GPIO_Port GPIOI
#define VSYNC_DCMI3V3_SCAM1U_Pin GPIO_PIN_5
#define VSYNC_DCMI3V3_SCAM1U_GPIO_Port GPIOI
#define CS_SPI3V3_MOBCtoSUNSMX_Pin GPIO_PIN_6
#define CS_SPI3V3_MOBCtoSUNSMX_GPIO_Port GPIOI
#define GPIO3V3_SOBCtoMOBC_Pin GPIO_PIN_7
#define GPIO3V3_SOBCtoMOBC_GPIO_Port GPIOI
#define GPIO3V3_SOBCtoMOBC_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
