/**
 * @file Com_Fault_Detectin_Driver.h.h
 * @Auther Noro
 */
#ifndef COM_FAULT_DETECTION_DRIVER_H_
#define COM_FAULT_DETECTION_DRIVER_H_

#include "../../IfWrapper/HAL.h"
#include <src_core/System/TimeManager/time_manager.h>
#include <src_user/Settings/System/flag_setting.h>

#define NORESP_THRES_LV2_MAX 2
#define NORESP_THRES_LV3_MAX 2

typedef struct{
	uint8_t anomaly_enable;
	uint8_t anomaly_detect_lv1;
	uint8_t anomaly_detect_lv2;
	uint8_t anomaly_detect_lv3;
	uint32_t thres_lv2[NORESP_THRES_LV2_MAX];//ポートで閾値が変わるのでここにおく
	uint32_t thres_lv3[NORESP_THRES_LV3_MAX];
	uint16_t thres_lv1_counter;
	uint16_t thres_lv2_counter;
	uint16_t thres_lv3_counter;
	uint16_t count_lv1;
	uint16_t count_lv2;
	uint16_t count_lv3;
	HAL_StatusTypeDef ack;
	ObcTime last_rcv_time;
	void* handler;
	int who_am_i;
}ANOMALY_DRIVER_STRUCT;

#ifdef __cplusplus
extern "C" {
#endif

void ANOMALY_Port_init_(int num, ANOMALY_DRIVER_STRUCT *str, void* handler,uint8_t enable);
void ANOMALY_Default_Settings(ANOMALY_DRIVER_STRUCT *str);
void ANOMALY_HAL_ACK_Detect(uint16_t *count, ANOMALY_DRIVER_STRUCT *str);

#ifdef __cplusplus
}
#endif

#endif // COM_FAULT_DETECTION_DRIVER_H_
