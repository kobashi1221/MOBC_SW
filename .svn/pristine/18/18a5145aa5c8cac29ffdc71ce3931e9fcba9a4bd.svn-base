/**
  *****************************************************************************************
  * @file    : PWR_cmd_GPIO_input_MOBC.h
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

#ifndef PWR_GPIO_input_MOBC_H_
#define PWR_GPIO_input_MOBC_H_

#include <stdint.h>

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


typedef struct
{
	uint8_t GPIO_input_PLASMA;
	uint8_t GPIO_input_SEPD_MSW;
	uint8_t GPIO_input_SEPD_MILLMAX;

} PWR_GPIO_INPUT_MOBC_STRUCT;

typedef enum
{
	ORDER_VOLS_PLASMA,
	ORDER_VOLS_SEPD_MSW,
	ORDER_VOLS_SEPD_MILLMAX

} PWR_GPIO_INPUT_ORDER;


#ifdef __cplusplus
 extern "C" {
#endif


AppInfo PWR_GPIO_input_MOBC_update(void);

CCP_CmdRet Cmd_PWR_GPIO_input_MOBC_Read(const CommonCmdPacket* packet);

uint8_t Get_GPIO_state(uint8_t);

#ifdef __cplusplus
}
#endif

#endif /* PWR_ADC_MOBC_H_ */
