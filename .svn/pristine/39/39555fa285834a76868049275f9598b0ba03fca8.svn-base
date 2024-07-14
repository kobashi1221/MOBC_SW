/*
 * DAC.h
 *
 *  Created on: 2023/06/14
 *      Author: daiki
 */

#ifndef SRC_USER_DRIVERS_SENSOR_DAC_H_
#define SRC_USER_DRIVERS_SENSOR_DAC_H_

#include "stdint.h"

typedef struct{
	void* i2c_driver;

}DAC_STRUCT;

typedef enum{
	DAC_SPECIAL_FUNCTION_REGISTER,
	DAC_GAIN_REGISTER,
	DAC_OFFSET_REGISTER,
	DAC_INPUT_DATA_REGISTER
}DAC_REGISTER;

uint8_t dac_change_voltage(const DAC_STRUCT* dac, uint8_t ch, uint16_t value);

#endif /* SRC_USER_DRIVERS_SENSOR_DAC_H_ */
