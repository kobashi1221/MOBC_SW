/*
 * PSD.h
 *
 *  Created on: 2023/09/07
 *      Author: Ji Hyun Park
 */

#ifndef CSUN_SENSOR_H_
#define CSUN_SENSOR_H_

#include <src_core/System/TImeManager/time_manager.h>//for delay()
//#include <src_user/IfWrapper/IF_class.h>
#include <src_user/IfWrapper/HAL.h>
#include <src_user/Applications/UserDefined/Sensor/ACS123_cmd_AnomalyDetect.h>   // for Error list


typedef struct{
    // CSピンの情報
    void* gpio_port;
    uint16_t gpio_pin;

    // SPI handler
    void* spi_port;

    float SUN_Vec[3];

    SUN_SENSOR_StateTypeDef state;

}PSD_DRIVER_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

int PSD_init(PSD_DRIVER_STRUCT* psd_driver, void* spi_port, void* gpio_port, uint16_t gpio_pin);
int function_EnCrystal(PSD_DRIVER_STRUCT* psd_driver);
int function_SetRate(PSD_DRIVER_STRUCT* psd_driver);
//int function_GetMeasDR(PSD_DRIVER_STRUCT* psd_driver);//for debug 20240530
int function_GetMeas(PSD_DRIVER_STRUCT* psd_driver);//for debug 20240530

#ifdef __cplusplus
 }
#endif

#endif /* CSUN_SENSOR_H_ */
