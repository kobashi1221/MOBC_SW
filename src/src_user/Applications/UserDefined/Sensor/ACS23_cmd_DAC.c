/*
 * ACS23_cmd_DAC.c
 *
 *  Created on: 2023/06/14
 *      Author: daiki
 */

#include "ACS23_cmd_DAC.h"
#include <src_user/IfWrapper/HAL.h>

static DAC_STRUCT dac_struct_;
const DAC_STRUCT* dac_struct;

static void ACS23_DAC_init(void);

AppInfo ACS23_DAC_update(void)
{
	//create_app_info()を呼び出す．
	return AI_create_app_info("DAC", ACS23_DAC_init, NULL);
}

static void ACS23_DAC_init(void){
	dac_struct = &dac_struct_;

	dac_struct_.i2c_driver = &hi2c3;
}
