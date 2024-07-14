/*
 * DAC.c
 *
 *  Created on: 2023/06/14
 *      Author: daiki
 */

#include "DAC.h"
#include <src_user/IfWrapper/HAL.h>

const uint8_t dac_dev_address = 0xA8;

uint8_t dac_change_voltage(const DAC_STRUCT* dac, uint8_t ch, uint16_t value){
	uint8_t ret = 0;
	uint8_t send_data[3] = {0};

	send_data[0] = ch & 0b00001111;
	send_data[1] = 0b11000000 | (value >> 8);
	send_data[2] = value & 0xFF;

	ret = HAL_I2C_Master_Transmit_C2A(dac->i2c_driver, dac_dev_address, send_data, 3, 0xff);
	return ret;
}

