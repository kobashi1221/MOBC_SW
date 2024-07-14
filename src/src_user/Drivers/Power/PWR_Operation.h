/*
 * GPIO_MOBC.h
 *
 *  Created on: 2021/12/03
 *      Author: Otsuki
 */

#ifndef SRC_USER_DRIVERS_PWR_PWR_OPERATION_H_
#define SRC_USER_DRIVERS_PWR_PWR_OPERATION_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>


#include<src_user/Applications/UserDefined/IF_Instances.h> // for GPIO_OUT_PINS
#include "PWR_State.h"


#ifdef __cplusplus
 extern "C" {
#endif



typedef enum{
	// HW固定
	IOEX_PORT_SUNS_MXPZ = 4,
	IOEX_PORT_SUNS_PXPYMY = 5,
	IOEX_PORT_MIS1 = 8,
	IOEX_PORT_SEPCOIL1 = 9,
	IOEX_PORT_SEPCOIL2 = 10,
	IOEX_PORT_SEPCOIL3 = 11,
	IOEX_PORT_SEPCOIL4 = 12,
	IOEX_PORT_GYRO = 13,
	IOEX_PORT_MAGS = 14,
	IOEX_PORT_SEMP = 15,
}IOEX_PORT;

//void PWR_MOBC_Operate_ON_OFF(GPIO_OUT_PINS pin,uint8_t data);
//void PWR_MOBC_Operate_ONInit(GPIO_OUT_PINS pin);
//void PWR_MOBC_Operate_Restart(GPIO_OUT_PINS pin);
////#ifdef TWO_U
//void PWR_IOEX_Operate_ON_OFF(IOEX_PORT port,uint8_t data);
//void PWR_IOEX_Operate_ONInit(IOEX_PORT port);
//void PWR_IOEX_Operate_Restart(IOEX_PORT pin);
////#endif

extern uint8_t MTQA_anomaly_counter;//MATQAのアノマリenableまでのカウンタ初期値
extern uint8_t MTQO_anomaly_counter;//MATQOのアノマリenableまでのカウンタ初期値

void PWR_Operate_ON_OFF(PWR_STATE num, uint8_t data);
void PWR_Operate_ONInit(PWR_STATE num);
void PWR_Operate_Restart(PWR_STATE num);
void PWR_Operate_IOEX_init();

#ifdef __cplusplus
}
#endif

#endif /* SRC_USER_DRIVERS_PWR_PWR_OPERATION_H_ */
