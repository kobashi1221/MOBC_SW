/**
  *****************************************************************************************
  * @file    : PWR_cmd_GPIO_input_MOBC.cpp
  * @author  : KANIE Hayate
  * @version : 1.0
  * @date    : 2023/06/27
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2023/06/27  1.0   H.KANIE        First revision
  *
  *****************************************************************************************
  */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <src_user/IfWrapper/HAL.h>
#include <src_user/IfWrapper/IF_class.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_GPIO_input_MOBC.h>
#include "../Global_Sensor_Value.h"
#include "src_user/Library/Delay.h"
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include<src_user/Library/General/DBG_printf.h>
#include "PWR_cmd_AnomalyDetect.h"
#include "PWR_cmd_AnomalyHandle.h"
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_user/Library/Delay.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

static PWR_GPIO_INPUT_MOBC_STRUCT pwr_gpio_input_mobc;

static void PWR_GPIO_input_MOBC_init(void);
static void PWR_GPIO_input_MOBC_exe(void);
static void GPIO_input_MOBC_initialize_param(void);
static void GPIO_input_MOBC_load_param(void);

/**
 * @brief �A�v���P�[�V�����쐬�֐�
 */
AppInfo PWR_GPIO_input_MOBC_update(void)
{
    return AI_create_app_info("DBG_GPIO_input_MOBC", PWR_GPIO_input_MOBC_init, PWR_GPIO_input_MOBC_exe);
}

/**
 * @brief �A�v���P�[�V�����������֐�
 *
 * �\���̓��̏��������K�v�ȕϐ�������������D
 */
static void PWR_GPIO_input_MOBC_init(void)
{
    printf("PWR GPIO input_MOBC Debug initialize.\n");
    g_sensor.p_pwr_gpio_input_mobc = &pwr_gpio_input_mobc;
}

/**
 * @brief �A�v���P�[�V�������s�֐�
 */
static void PWR_GPIO_input_MOBC_exe(void)
{
    printf("sample driver execution.\n");
}


CCP_CmdRet Cmd_PWR_GPIO_input_MOBC_Read(const CommonCmdPacket* packet)
{
	int ret = 0;
	size_t len=CCP_get_param_len(packet);
	const uint8_t *head_param = CCP_get_param_head(packet);
	uint8_t param[1];

	memcpy(param,head_param,len);

	uint8_t state = 0;  // PWR state
	uint8_t gpio_state = Get_GPIO_state(param[0]);

	switch(param[0]){
#ifdef ONE_U
	case ORDER_VOLS_PLASMA:
		state = PWR_State_GET(PWR_State_MTQO_PLASMA);
		pwr_gpio_input_mobc.GPIO_input_PLASMA = gpio_state;
		gpio_state == 0xff ? print(PWR_STATE_MOBC, "GPIO_input_PLASMA : HIGH\r\n") : print(PWR_STATE_MOBC, "GPIO_input_PLASMA : LOW\r\n");

		// �d���A�m�}���H

		break;
#endif
	case ORDER_VOLS_SEPD_MSW:
//		state = ;	// ������Ԃ������p�����[�^������
		pwr_gpio_input_mobc.GPIO_input_SEPD_MSW = gpio_state;
		if(gpio_state == 0xff){
		  print(PWR_STATE_MOBC, "GPIO_input_SEPD_MSW : HIGH(No Separated)\r\n");
		}else{
		  print(PWR_STATE_MOBC, "GPIO_input_SEPD_MSW : LOW(Separated)\r\n");
		}

		// �A�m�}���H

		break;
	case ORDER_VOLS_SEPD_MILLMAX:
//		state = ;	// ������Ԃ������p�����[�^������
		pwr_gpio_input_mobc.GPIO_input_SEPD_MILLMAX = gpio_state;
        if(gpio_state == 0xff){
          print(PWR_STATE_MOBC, "GPIO_input_SEPD_MILLMAX : HIGH(Separated)\r\n");
        }else{
          print(PWR_STATE_MOBC, "GPIO_input_SEPD_MILLMAX : LOW(No Separated)\r\n");
        }

		// �A�m�}���H

		break;
	}

    if (ret != 0)
    {
        printf("Read GPIO_input_MOBC Failed! %d \r\n", ret);
        return CCP_make_cmd_ret_without_err_code((CCP_EXEC_STS)ret);
    }

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);

}


uint8_t Get_GPIO_state(uint8_t order)
{
	uint8_t ret = 0;
	uint8_t state = 0;
	GPIO_Class gpio_state;

	switch(order){
#ifdef ONE_U
	case ORDER_VOLS_PLASMA:
		gpio_state.SetPortAndPin(PSTATE_PLASMAtoMOBC_GPIO_Port, PSTATE_PLASMAtoMOBC_Pin);
		state = gpio_state.ReadPin();
		break;
#endif
	case ORDER_VOLS_SEPD_MSW:
		gpio_state.SetPortAndPin(GPIO3V3_SEPD_MSWtoMOBC_GPIO_Port, GPIO3V3_SEPD_MSWtoMOBC_Pin);
		state = gpio_state.ReadPin();
		break;
	case ORDER_VOLS_SEPD_MILLMAX:
		gpio_state.SetPortAndPin(GPIO3V3_SEPD_MILLMAXtoMOBC_GPIO_Port, GPIO3V3_SEPD_MILLMAXtoMOBC_Pin);
		state = gpio_state.ReadPin();
		break;
	}

	if (state == 1){
		ret = 0xff;
	}
	else{
		ret = 0x00;
	}

	return ret;
}
