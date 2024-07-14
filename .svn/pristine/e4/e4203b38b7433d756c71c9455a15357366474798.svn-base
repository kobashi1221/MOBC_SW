/*
 * Global_Mission_Parameter.c
 *
 *  Created on: 2024/06/21
 *      Author: kobayashi
 */

#include "Global_Mission_Parameter.h"
#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <stdlib.h>
#include <string.h>
#include <src_user/Drivers/Cdh/RTC/RTC.h>


static GLOBAL_MISSION_PARAMETER_STRUCT GMP;

void PWR_Mission_Mode_Set(uint32_t PWR_Mission_Mode_Num){
    GMP.MISSION_MODE.PWR_MISSION_MODE_Number = PWR_Mission_Mode_Num;
}

void Mission_Mode_Set(uint32_t Mission_Mode_Num){
  GMP.MISSION_MODE.MISSION_MODE_Number = Mission_Mode_Num;

}
