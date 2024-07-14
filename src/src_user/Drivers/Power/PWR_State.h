/*
 * PWR_State.h
 *
 *  Created on: 2021/12/04
 *      Author: SAT63
 */

#ifndef SRC_USER_DRIVERS_PWR_PWR_STATE_H_
#define SRC_USER_DRIVERS_PWR_PWR_STATE_H_

#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>
#include<src_user/Applications/UserDefined/IF_Instances.h>
#include <stdint.h> //for uint8_t
#include <stdbool.h> //for boolean



extern uint8_t PWR_GPIO_State[PWR_State_MAX];
extern uint8_t PWR_Sepcoil_Guard[SEPCOIL_MAX];

#ifdef __cplusplus
 extern "C" {
#endif


void PWR_State_SET(PWR_STATE type,uint8_t state);
uint8_t PWR_State_GET(PWR_STATE type); // for getting power state
uint8_t PWR_VOTE(PWR_STATE num);

void PWR_Sepcoil_Guard_SET(uint8_t type, uint8_t state);
uint8_t PWR_Sepcoil_Guard_GET(uint8_t type);

#ifdef __cplusplus
}
#endif

#endif /* SRC_USER_DRIVERS_PWR_PWR_STATE_H_ */
