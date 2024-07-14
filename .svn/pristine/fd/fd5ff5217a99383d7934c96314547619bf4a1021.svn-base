/*
 * 9AX_LSM9DS1.c
 *
 *  Created on: 2023/05/15
 *      Author: daiki
 */

#include "9AX.h"
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/IfWrapper/HAL.h>

#define ACC_GYRO_ID 0b1101000
#define MAG_ID 0b00111101

#define PI 3.14159f

//I2C address
//SA0=LOW(both accgyro and mag)
const uint8_t accgyro_address = 0b11010100;
const uint8_t mag_address = 0b00111000;

uint8_t accgyro_read_reg(LSM9DS1* lsm9ds1, uint8_t reg_address, uint8_t* data, uint8_t len);
uint8_t accgyro_write_reg(LSM9DS1* lsm9ds1, uint8_t reg_address, uint8_t* data, uint8_t len);
uint8_t mag_read_reg(LSM9DS1* lsm9ds1, uint8_t reg_address, uint8_t* data, uint8_t len);
uint8_t mag_write_reg(LSM9DS1* lsm9ds1, uint8_t reg_address, uint8_t* data, uint8_t len);
uint8_t lsm9ds1_id_check(LSM9DS1* lsm9ds1);
uint8_t lsm9ds1_ctrl_acc(LSM9DS1* lsm9ds1);
uint8_t lsm9ds1_ctrl_gyro(LSM9DS1* lsm9ds1);
uint8_t lsm9ds1_ctrl_mag(LSM9DS1* lsm9ds1);

uint8_t lsm9ds1_power_on_init(LSM9DS1* lsm9ds1){
	uint8_t ret = 0;

	ret = lsm9ds1_id_check(lsm9ds1);
	if(ret != 0)print(_9AX, "ID fail\r\n");

	lsm9ds1_ctrl_acc(lsm9ds1);
	lsm9ds1_ctrl_gyro(lsm9ds1);
	lsm9ds1_ctrl_mag(lsm9ds1);

	return ret;
}

uint8_t lsm9ds1_id_check(LSM9DS1* lsm9ds1){
	uint8_t ret = 0;
	uint8_t who;
	ret = accgyro_read_reg(lsm9ds1, (uint8_t)G_XL_WHO_AM_I, &who, 1);
	if(who != ACC_GYRO_ID)return 255;

	ret = mag_read_reg(lsm9ds1, (uint8_t)MAG_WHO_AM_I, &who, 1);
	if(who != MAG_ID)return 255;

	return ret;
}

uint8_t lsm9ds1_ctrl_acc(LSM9DS1* lsm9ds1){
	uint8_t ret = 0;
	uint8_t ctrl_reg6 = 0;
	ctrl_reg6 |= (lsm9ds1->acc_odr << 5)       & 0b11100000;
	ctrl_reg6 |= (lsm9ds1->acc_scale << 3)     & 0b00011000;
	ctrl_reg6 |= (lsm9ds1->acc_bw_select << 2) & 0b00000100;
	if(lsm9ds1->acc_bw_select == 1)ctrl_reg6 |= lsm9ds1->acc_bw & 0b00000011;

	switch(lsm9ds1->acc_scale){
		case ACC_SCALE_2g:
			lsm9ds1->acc_scale_value = 4.0f;
			break;
		case ACC_SCALE_16g:
			lsm9ds1->acc_scale_value = 32.0f;
			break;
		case ACC_SCALE_4g:
			lsm9ds1->acc_scale_value = 8.0f;
			break;
		case ACC_SCALE_8g:
			lsm9ds1->acc_scale_value = 16.0f;
			break;
		default:
			break;
	}

	ret = accgyro_write_reg(lsm9ds1, (uint8_t)ACC_CTRL_REG6, &ctrl_reg6, 1);
	return ret;
}

uint8_t lsm9ds1_ctrl_gyro(LSM9DS1* lsm9ds1){
	uint8_t ret = 0;
	uint8_t ctrl_reg1 = 0;
	ctrl_reg1 |= (lsm9ds1->gyro_odr << 5)   & 0b11100000;
	ctrl_reg1 |= (lsm9ds1->gyro_scale << 3) & 0b00011000;
	ctrl_reg1 |= lsm9ds1->gyro_bw           & 0b00000011;

	switch(lsm9ds1->gyro_scale){
		case GYRO_SCALE_245dps:
			lsm9ds1->gyro_scale_value = 490.0f;
			break;
		case GYRO_SCALE_500dps:
			lsm9ds1->gyro_scale_value = 1000.0f;
			break;
		case GYRO_SCALE_2000dps:
			lsm9ds1->gyro_scale_value = 4000.0f;
			break;
		default:
			break;
	}

	ret = accgyro_write_reg(lsm9ds1, (uint8_t)GYRO_CTRL_REG1, &ctrl_reg1, 1);

	uint8_t ctrl_reg3 = 0;
	ctrl_reg3 |= (lsm9ds1->gyro_low_power << 7) & 0b10000000;
	ret = accgyro_write_reg(lsm9ds1, (uint8_t)GYRO_CTRL_REG3, &ctrl_reg3, 1);

	return ret;
}

uint8_t lsm9ds1_ctrl_mag(LSM9DS1* lsm9ds1){
	uint8_t ret = 0;

	uint8_t ctrl_reg[4] = {0x00};

	//REG1
	ctrl_reg[0] |= (lsm9ds1->mag_mode << 5)  & 0b01100000;
	ctrl_reg[0] |= (lsm9ds1->mag_odr << 2)   & 0b00011100;

	//REG2
	ctrl_reg[1] |= (lsm9ds1->mag_scale << 5)  & 0b01100000;

	//REG3
	ctrl_reg[2] |= lsm9ds1->mag_system_mode & 0b00000011;

	//REG4
	ctrl_reg[3] |= (lsm9ds1->mag_mode << 2)  & 0b00001100;

	switch(lsm9ds1->mag_scale){
		case MAG_SCALE_4GAUSS:
			lsm9ds1->mag_scale_value = 8.0f;
			break;
		case MAG_SCALE_8GAUSS:
			lsm9ds1->mag_scale_value = 16.0f;
			break;
		case MAG_SCALE_12GAUSS:
			lsm9ds1->mag_scale_value = 24.0f;
			break;
		case MAG_SCALE_16GAUSS:
			lsm9ds1->mag_scale_value = 32.0f;
			break;
		default:
			break;
	}

	ret = mag_write_reg(lsm9ds1, (uint8_t)MAG_CTRL_REG1, ctrl_reg, 4);

	return ret;
}


uint8_t lsm9ds1_get_acc_all(LSM9DS1* lsm9ds1, int16_t* acc){
	uint8_t ret = 0;
	uint8_t acc_out[6];
	ret = accgyro_read_reg(lsm9ds1, (uint8_t)ACC_OUT_X_L, acc_out, 6);
	if(ret != 0)print(_9AX, "fail\r\n");
	acc[0] = (uint16_t)(acc_out[1] << 8) | acc_out[0];
	acc[1] = (uint16_t)(acc_out[3] << 8) | acc_out[2];
	acc[2] = (uint16_t)(acc_out[5] << 8) | acc_out[4];

	return ret;
}

uint8_t lsm9ds1_get_gyro_all(LSM9DS1* lsm9ds1, int16_t* gyro){
	uint8_t ret = 0;
	uint8_t gyro_out[6];
	ret = accgyro_read_reg(lsm9ds1, (uint8_t)GYRO_OUT_X_L, gyro_out, 6);
	if(ret != 0)print(_9AX, "fail\r\n");
	gyro[0] = (uint16_t)(gyro_out[1] << 8) | gyro_out[0];
	gyro[1] = (uint16_t)(gyro_out[3] << 8) | gyro_out[2];
	gyro[2] = (uint16_t)(gyro_out[5] << 8) | gyro_out[4];

	return ret;
}

uint8_t lsm9ds1_get_mag_all(LSM9DS1* lsm9ds1, int16_t* mag){
	uint8_t ret = 0;
	uint8_t mag_out[6];
	ret = mag_read_reg(lsm9ds1, (uint8_t)MAG_OUT_X_L, mag_out, 6);
	if(ret != 0)print(_9AX, "fail\r\n");
	mag[0] = (uint16_t)(mag_out[1] << 8) | mag_out[0];
	mag[1] = (uint16_t)(mag_out[3] << 8) | mag_out[2];
	mag[2] = (uint16_t)(mag_out[5] << 8) | mag_out[4];

	return ret;
}


uint8_t accgyro_read_reg(LSM9DS1* lsm9ds1, uint8_t reg_address, uint8_t* data, uint8_t len){
	uint8_t ret = 0;
	ret = HAL_I2C_Mem_Read_C2A((I2C_HandleTypeDef*)lsm9ds1->i2c_driver, accgyro_address, reg_address, 1, data, len, 0xff);
	if(ret != 0)print(_9AX, "fail\r\n");
	return ret;
}

uint8_t accgyro_write_reg(LSM9DS1* lsm9ds1, uint8_t reg_address, uint8_t* data, uint8_t len){
	uint8_t ret = 0;
	ret = HAL_I2C_Mem_Write_C2A((I2C_HandleTypeDef*)lsm9ds1->i2c_driver, accgyro_address, reg_address, 1, data, len, 0xff);
	if(ret != 0)print(_9AX, "fail\r\n");
	return ret;
}

uint8_t mag_read_reg(LSM9DS1* lsm9ds1, uint8_t reg_address, uint8_t* data, uint8_t len){
	uint8_t ret = 0;
	ret = HAL_I2C_Mem_Read_C2A((I2C_HandleTypeDef*)lsm9ds1->i2c_driver, mag_address, reg_address, 1, data, len, 0xff);
	if(ret != 0)print(_9AX, "fail\r\n");
	return ret;
}

uint8_t mag_write_reg(LSM9DS1* lsm9ds1, uint8_t reg_address, uint8_t* data, uint8_t len){
	uint8_t ret = 0;
	ret = HAL_I2C_Mem_Write_C2A((I2C_HandleTypeDef*)lsm9ds1->i2c_driver, mag_address, reg_address, 1, data, len, 0xff);
	if(ret != 0)print(_9AX, "fail\r\n");
	return ret;
}
