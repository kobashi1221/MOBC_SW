#ifndef _IS25LPCONFIG_H
#define _IS25LPCONFIG_H

#include <src_user/IfWrapper/HAL.h>
//#include "../../../Drivers/NU1_global_val_define.h"
//extern SPI_HandleTypeDef hspi5;


#define		_IS25LP_SPI			hspi3
#define		_IS25LP_CS_GPIO		CS_SPI3V3_MOBCtoSROM_GPIO_Port//CS_PORT_SPIPROM // //ピンアサインに応じて変更
#define		_IS25LP_CS_PIN		CS_SPI3V3_MOBCtoSROM_Pin//CS_PIN_SPIPROM // //ピンアサインに応じて変更

extern SPI_HandleTypeDef _IS25LP_SPI;

#endif
