/*
 * PSD.cpp
 *
 *  Created on: 2023/09/07
 *      Author: Ji Hyun Park
 */

#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Library/Delay.h>
#include <src_user/Library/General/DBG_printf.h>

#include <stdio.h> //printf
#include <string.h> //memcpy
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Drivers/Sensor/PSD.h>
#include <stdlib.h> //abs
#include <time.h>

// Delay関数定義
//#define SUNS_Delay SUNS_Delay
//#define SPI_DMA_ENABLED 1

int PSD_init(PSD_DRIVER_STRUCT* psd_driver, void* spi_port, void* gpio_port, uint16_t gpio_pin)
{
  psd_driver->spi_port = spi_port;
  psd_driver->gpio_port = gpio_port;
  psd_driver->gpio_pin = gpio_pin;

  HAL_GPIO_WritePin_C2A(psd_driver->gpio_port, psd_driver->gpio_pin, GPIO_PIN_SET);

  return 0;

}

int function_EnCrystal(PSD_DRIVER_STRUCT* psd_driver){
//    printf("Enable Crystal Task Initiated.\r\n");

//    uint8_t device = 0x01;
//    uint8_t address = 0x0E;
//    uint8_t rw = 0x00; //write
//    uint8_t control = (device << 6) | (address << 1) | rw;
//
//    uint8_t spi2datRx[4] = {0};
//    uint8_t spi2datTx[4] = {0};
//    spi2datTx[0] = control;
//
////    printf("spi2datTx:%u %u %u %u\r\n", spi2datTx[0], spi2datTx[1], spi2datTx[2], spi2datTx[3]);
//    for(uint8_t i = 1 ; i < 4; i ++) spi2datTx[i] = 0;

      uint8_t device = 0x01;
      uint8_t address = 0x0A;
      uint8_t rw = 0x00; //write
      uint8_t control = (device << 6) | (address << 1) | rw;


      uint8_t spi2datRx[43] = {0};
      uint8_t spi2datTx[43] = {0};
      spi2datTx[0] = control;
      for(uint8_t i = 1 ; i < 7; i ++) spi2datTx[i] = 0;
      spi2datTx[7] = 0xa9;
      spi2datTx[8] = 0;
      spi2datTx[9] = 0;
      spi2datTx[10] = 0x3b;
      spi2datTx[11] = 0xe0;
      spi2datTx[12] = 0x50;
      for(uint8_t i = 13 ; i < 43; i ++) spi2datTx[i] = 0;


    HAL_GPIO_WritePin_C2A(psd_driver->gpio_port, psd_driver->gpio_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive_C2A(psd_driver->spi_port, spi2datTx, spi2datRx, 43, 1000);
    HAL_GPIO_WritePin_C2A(psd_driver->gpio_port, psd_driver->gpio_pin, GPIO_PIN_SET);

//    printf("spi2datRx:%u %u %u %u\r\n", spi2datRx[0], spi2datRx[1], spi2datRx[2], spi2datRx[3]);
//    printf("Enable Crystal Task Completed.\r\n");
    return 0;
}


//  function_EnCrystalに複合させた
int function_SetRate(PSD_DRIVER_STRUCT* psd_driver){
//    printf("Set Rate Task Initiated.\r\n");

    uint8_t device = 0x01;
    uint8_t address = 0x0D;
    uint8_t rw = 0x00; //write
    uint8_t control = (device << 6) | (address << 1) | rw;

    uint8_t spi2datRx[4] = {0};
    uint8_t spi2datTx[4] = {0};

    for(uint8_t i = 1 ; i < 4; i ++) spi2datTx[i] = 0;
    spi2datTx[0] = control;
    spi2datTx[1] = 0b00111011;
    spi2datTx[2] = 0b11100000;
    spi2datTx[3] = 0b01010000;

//    printf("spi2datTx:%u %u %u %u\r\n", spi2datTx[0], spi2datTx[1], spi2datTx[2], spi2datTx[3]);

    HAL_GPIO_WritePin_C2A(psd_driver->gpio_port, psd_driver->gpio_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive_C2A(psd_driver->spi_port, spi2datTx, spi2datRx, 4, 1000);
    HAL_GPIO_WritePin_C2A(psd_driver->gpio_port, psd_driver->gpio_pin, GPIO_PIN_SET);

//    printf("spi2datRx:%u %u %u %u\r\n", spi2datRx[0], spi2datRx[1], spi2datRx[2], spi2datRx[3]);

//    printf("Set Rate Task Completed.\r\n");
    return 0;
}

int function_GetMeas(PSD_DRIVER_STRUCT* psd_driver){
//    printf("Get Measurement Task Initiated.\r\n");

    uint8_t device = 0x01;
    uint8_t address = 0x00;//読みたいアドレスの先頭
    uint8_t address_DR = 0x0C;//STATUSCOMアドレスの先頭
    uint8_t rw = 0x01; //read

    uint8_t control = (device << 6) | (address << 1) | rw;
    uint8_t control_DR = (device << 6) | (address_DR << 1) | rw;

    uint8_t spi2datRx[13] = {0};
    uint8_t spi2datTx[13] = {0};
    uint8_t spi2datRx_DR[4] = {0};
    uint8_t spi2datTx_DR[4] = {0};
    spi2datTx[0] = control;//1byte目のコマンド
    spi2datTx_DR[0] = control_DR;//1byte目のコマンド
    //ADC Datasheet
    //ADC側の決まり

    HAL_GPIO_WritePin_C2A(psd_driver->gpio_port, psd_driver->gpio_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive_C2A(psd_driver->spi_port, spi2datTx, spi2datRx, 13, 1000);//13Byte分取ってくる
    HAL_GPIO_WritePin_C2A(psd_driver->gpio_port, psd_driver->gpio_pin, GPIO_PIN_SET);

    HAL_Delay_C2A(5);
    HAL_GPIO_WritePin_C2A(psd_driver->gpio_port, psd_driver->gpio_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive_C2A(psd_driver->spi_port, spi2datTx_DR, spi2datRx_DR, 4, 1000);//STATUSCOM 4Byte分取ってくる
    HAL_GPIO_WritePin_C2A(psd_driver->gpio_port, psd_driver->gpio_pin, GPIO_PIN_SET);

//    printf("spi2datRxHEXDR:%x %x %x %x\r\n", spi2datRx_DR[0], spi2datRx_DR[1], spi2datRx_DR[2], spi2datRx_DR[3]);

//    printf("spi2datRxHEX:%x\r\n", spi2datRx[0]);
//    printf("spi2datRxHEXSens0:%x %x %x\r\n",spi2datRx[1], spi2datRx[2], spi2datRx[3]);
//    printf("spi2datRxHEXSens1:%x %x %x\r\n",spi2datRx[4], spi2datRx[5], spi2datRx[6]);
//    printf("spi2datRxHEXSens2:%x %x %x\r\n",spi2datRx[7], spi2datRx[8], spi2datRx[9]);
//    printf("spi2datRxHEXSens3:%x %x %x\r\n",spi2datRx[10], spi2datRx[11], spi2datRx[12]);
//    printf("spi2datRxDEC:%u %u %u %u %u %u %u\r\n", spi2datRx[0], spi2datRx[1], spi2datRx[2], spi2datRx[3], spi2datRx[4], spi2datRx[5], spi2datRx[6]);
//    printf("spi2datRxDEC:%u %u %u %u %u %u %u\r\n", spi2datRx[7], spi2datRx[8], spi2datRx[9], spi2datRx[10], spi2datRx[11], spi2datRx[12], spi2datRx[13]);
//  spi2datRx[0]がMOBCコマンド送っているから必ず0
//  spi2datRx[13]は存在しないはず

    uint8_t signval = 0;
    long anaread[4] = {0};
    float convdat[4] = {0};

    //printf("\r\n");
//    if(spi2datRx_DR[1] == 0xa9&&spi2datRx_DR[3] == 0x00){//DR check 根本解決ではなく対処療法です
      for(uint8_t i = 0 ; i < 4; i ++) {
          signval = (spi2datRx[i*3+1] >> 7) & 0x01;//先頭の符号ビット データシート5.5節参照

  //      変換値がおかしくなるらしいからコメントアウト20240529
  //        anaread[i] = ((spi2datRx[i*3+1] & 0x7F) << 16) |
  //                ((spi2datRx[i*3+2] ) << 8) |
  //                (spi2datRx[i*3+3] );

          anaread[i] = (((long)spi2datRx[i*3+1]<<16) | ((long)spi2datRx[i*3+2]<<8) | (spi2datRx[i*3+3])) & 0x007FFFFF;//先頭びっとに関わらず7FFFFF
          if(signval == 1) anaread[i] -= 8388608;//先頭ビットが負ならこれで直す
//          printf("anaread %d :　%x\r\n",i,anaread[i]);//takahashi added 20240529

          convdat[i] = ((float)(anaread[i])) / 8388608.0 / 1.5 * 1200.0; //mV

          //convdat[i] -= 4; //remove bias, change depending on config.
  //        if (convdat[i] < 0) convdat[i] = 0;//負の値も0になる
          print(SUNS, "Sens %d: %f\r\n",i,convdat[i]);
      }
//    }
    //printf("\r\n");

//    printf("Get Measurement Task Completed.\r\n");
    return 0;
}

