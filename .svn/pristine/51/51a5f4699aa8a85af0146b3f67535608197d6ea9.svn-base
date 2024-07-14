/*
 * PWR_cmd_AnomalyDetect.h
 *
 *  Created on: 2021/06/27
 *      Author: sakaguchi
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_ACS123_CMD_ANOMALYDETECT_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_ACS123_CMD_ANOMALYDETECT_H_

#include <stdint.h> //for uint8_t
#include <src_core/System/ApplicationManager/app_info.h>


typedef enum{
	AL_OT_QAX_GYRO,
	MAX_OT_SENSOR,
	AL_CONSTANT_QAX_MAGS,
} AL_QAX_ACK;

typedef enum{
	AL_OT_GYRO,
}AL_GYRO_ACK;

typedef enum{
	AL_CONSTANT_MAGS,
}AL_MAGS_ACK;

typedef enum{
	AL_DONT_WORK_SUNS,
}AL_SUNS_ACK;

typedef enum{
	AL_OT_MTQO,
	AL_OT_MTQA
}AL_MTQ_ACK;

typedef enum{
	AL_OT_PT_TEMS,
}AL_PT_TEMS_ACK;

// Error list for SUNS driver.h
typedef enum{
	NO_ERROR = 0x00U,            		// No error
	CHECKSUM_ERROR = 0x01U,      		// Checksum error
	SPI_ERROR = 0x02U,           		// Error while SPI communication
	ALBEDO_EARTH_ERROR = 0x03U,  		// Sun sensor does not see the Sun
	ALBEDO_EARTH_SUN_ERROR = 0x04U,  	// Sun sensor does not see the Sun and the Earth*/
	OTHER_ERROR = 0x05U  				// Internal manufacturer information for the Sun sensor
}SUN_SENSOR_StateTypeDef;


#ifdef __cplusplus
 extern "C" {
#endif



#ifdef __cplusplus
 }
#endif

#endif
