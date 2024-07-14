/*
 * EEPROM.h
 *
 *  Created on: 2021/12/02
 *      Author: daiki
 */

#ifndef SRC_USER_DRIVERS_CDH_EEPROM_EEPROM_H_
#define SRC_USER_DRIVERS_CDH_EEPROM_EEPROM_H_

#include <stdint.h> //for uint8_t
#include <stdlib.h>
#include <src_user/IfWrapper/HAL.h>

#ifdef __cplusplus
 extern "C" {
#endif

// *** don't modified these functions begin *** //
void EEPROM_Init(void* i2c_port);
HAL_StatusTypeDef EEPROM_GeneralWrite(uint8_t* WriteDataBuf,
											size_t DataSize,
												uint32_t WriteAddress);
HAL_StatusTypeDef EEPROM_GeneralRead(uint8_t* ReadDataBuf,
											size_t DataSize,
												uint32_t ReadAddress);
// *** don't modified these functions end *** //

#ifdef __cplusplus
 }
#endif

#endif /* SRC_USER_DRIVERS_CDH_EEPROM_EEPROM_H_ */
