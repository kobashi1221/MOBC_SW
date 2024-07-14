/*
 * 9AX_LSM9DS1.h
 *
 *  Created on: 2023/05/15
 *      Author: daiki
 */

#ifndef AX_LSM9DS1_H_
#define AX_LSM9DS1_H_

#include "stdio.h"
#include "stdint.h"
#include "main.h"

typedef enum{
	G_XL_WHO_AM_I = 0x0F,
	GYRO_CTRL_REG1 = 0x10,
	GYRO_CTRL_REG2 = 0x11,
	GYRO_CTRL_REG3 = 0x12,
	GYRO_OUT_X_L = 0x18,
	GYRO_OUT_X_H = 0x19,
	GYRO_OUT_Y_L = 0x1A,
	GYRO_OUT_Y_H = 0x1B,
	GYRO_OUT_Z_L = 0x1C,
	GYRO_OUT_Z_H = 0x1D,
	ACC_CTRL_REG6 = 0x20,
	ACC_OUT_X_L = 0x28,
}ACCGYRO_REG;

typedef enum{
	MAG_WHO_AM_I = 0x0F,
	MAG_CTRL_REG1 = 0x20,
	MAG_CTRL_REG2 = 0x21,
	MAG_CTRL_REG3 = 0x22,
	MAG_CTRL_REG4 = 0x23,
	MAG_OUT_X_L = 0x28,
	MAG_OUT_Z_L = 0x2C,
}MAG_REG;

typedef enum{
	XL_G_POWER_DOWN_MODE,
	XL_G_ACC_ONLY_MODE,
	XL_G_ACC_GYRO_MODE,
}ACCGYRO_MODE;

typedef enum{
	ACC_ODR_POWER_DOWN,
	ACC_ODR_10Hz,
	ACC_ODR_50Hz,
	ACC_ODR_119Hz,
	ACC_ODR_238Hz,
	ACC_ODR_476Hz,
	ACC_ODR_952Hz,
}ACC_ODR;

typedef enum{
	ACC_SCALE_2g,
	ACC_SCALE_16g,
	ACC_SCALE_4g,
	ACC_SCALE_8g,
}ACC_SCALE;

typedef enum{
	ACC_BW_408Hz,
	ACC_BW_211Hz,
	ACC_BW_105Hz,
	ACC_BW_50Hz,
}ACC_BANDWIDTH;

typedef enum{
	GYRO_ODR_POWER_DOWN,
	GYRO_ODR_14_9Hz,
	GYRO_ODR_59_5Hz,
	GYRO_ODR_119Hz,
	GYRO_ODR_238Hz,
	GYRO_ODR_476Hz,
	GYRO_ODR_952Hz,
}GYRO_ODR;

typedef enum{
	GYRO_SCALE_245dps,
	GYRO_SCALE_500dps,
	GYRO_SCALE_NOT_AVAILABLE,
	GYRO_SCALE_2000dps,
}GYRO_SCALE;

typedef enum{
	CONTINUOUS_CONVERSION,
	SINGLE_CONVERSION,
	POWER_DOWN1,
	POWER_DOWN2,
}MAG_SYSTEM_MODE;

typedef enum{
	LOW_POWER_MODE,
	MEDIUM_PERFORM_MODE,
	HIGH_PERFORM_MODE,
	ULTRA_HIGH_PERFORM_MODE,
}MAG_MODE;

typedef enum{
	MAG_ODR_0_625Hz,
	MAG_ODR_1_25Hz,
	MAG_ODR_2_5Hz,
	MAG_ODR_5Hz,
	MAG_ODR_10Hz,
	MAG_ODR_20Hz,
	MAG_ODR_40Hz,
	MAG_ODR_80Hz,
}MAG_ODR;

typedef enum{
	MAG_SCALE_4GAUSS,
	MAG_SCALE_8GAUSS,
	MAG_SCALE_12GAUSS,
	MAG_SCALE_16GAUSS,
}MAG_SCALE;

typedef struct{
	void* i2c_driver;
	//ACC
	ACC_ODR acc_odr;
	ACC_SCALE acc_scale;
	ACC_BANDWIDTH acc_bw;
	uint8_t acc_bw_select;
	float acc_scale_value;
	//GYRO
	GYRO_ODR gyro_odr;
	GYRO_SCALE gyro_scale;
	uint8_t gyro_bw;
	float gyro_scale_value;
	uint8_t gyro_low_power;
	//MAG
	MAG_SYSTEM_MODE mag_system_mode;
	MAG_MODE mag_mode;
	MAG_ODR mag_odr;
	MAG_SCALE mag_scale;
	float mag_scale_value;
}LSM9DS1;

uint8_t lsm9ds1_power_on_init(LSM9DS1* lsm9ds1);
uint8_t lsm9ds1_get_acc_all(LSM9DS1* lsm9ds1, int16_t* acc);
uint8_t lsm9ds1_get_gyro_all(LSM9DS1* lsm9ds1, int16_t* gyro);
uint8_t lsm9ds1_get_mag_all(LSM9DS1* lsm9ds1, int16_t* mag);

#endif /* 9AX_LSM9DS1_H_ */
