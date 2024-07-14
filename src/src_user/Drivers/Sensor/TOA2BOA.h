/*
 * TOA2BOA.h
 *
 *  Created on: 2022. 7. 24.
 *      Author: g
 */
#ifdef ONE_U
#ifndef SRC_USER_DRIVERS_SENSOR_TOA2BOA_H_
#define SRC_USER_DRIVERS_SENSOR_TOA2BOA_H_

#include "math.h"
//#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app

#ifdef __cplusplus
extern "C" {
#endif

static void toa2boa_load_param(void);
static void toa2boa_initialize_param(void);
AppInfo toa2boa_param_update(void);

int TOA2BOA_DOS(uint32_t *startAddr, uint32_t size, float Sz, uint32_t N,
                uint8_t mode, uint8_t cam, uint8_t *BOA);

#ifdef __cplusplus
}
#endif

#endif /* SRC_USER_DRIVERS_SENSOR_TOA2BOA_H_ */
#endif/* for 1U*/
