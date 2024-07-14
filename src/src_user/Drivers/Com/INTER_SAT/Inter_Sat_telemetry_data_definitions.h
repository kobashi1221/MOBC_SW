/*
 * intersat_telemetry_data_definitions.h
 *
 *  Created on: 2021/11/28
 *      Author: sakaguchi
 */

#ifndef SRC_USER_DRIVERS_COM_COBC_INTERSAT_TELEMETRY_DATA_DEFINITIONS_H_
#define SRC_USER_DRIVERS_COM_COBC_INTERSAT_TELEMETRY_DATA_DEFINITIONS_H_

typedef struct
{
	struct
	{
		float ACCX;
		float ACCY;
		float ACCZ;

		float OMEGAX;
		float OMEGAY;
		float OMEGAZ;

		float MAGSX;
		float MAGSY;
		float MAGSZ;

		float SUNSX;
		float SUNSY;
		float SUNSZ;
  } sensor_info;
  struct{
		uint16_t rssi;
		uint32_t lora_rssi;
		uint32_t rxpic_time;
		uint32_t txpic_time;
		uint32_t mobc_time;
  }system;
  struct{
		double juliandate;
		uint16_t utc_year;
		uint8_t utc_month;
		uint8_t utc_day;
		uint8_t utc_hour;
		uint8_t utc_min;
		uint8_t utc_sec;

		float latitude;
		float Longitude;
		float Altitude;

		float PositionX;
		float PositionY;
		float PositionZ;

		float VelocityX;
		float VelocityY;
		float VelocityZ;

		float RelativePositionX;
		float RelativePositionY;
		float RelativePositionZ;

		float RelativeVelocityX;
		float RelativeVelocityY;
		float RelativeVelocityZ;

		float Ballistic_Coefficient;
  }orbit;
} INTERSAT_TlmData;



#endif /* SRC_USER_DRIVERS_COM_COBC_COBC_TELEMETRY_DATA_DEFINITIONS_H_ */
