
/**
  *****************************************************************************************
  * @file    : PWR_IOEX_ADC_MUX.h
  * @author  : Hiroki OTSUKI
  * @version : 1.0
  * @date    : 2020/09/28
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
  * 2020/09/28  1.0   H.KISHI         First revision
  *
  *****************************************************************************************
  */
#ifndef PWR_ADC_MOBC_H_
#define PWR_ADC_MOBC_H_

#include <stdint.h>

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd

//#include "PWR_cmd_FETREG.h"

typedef struct
{
	//referred by GenerateTelemetry.c
	// 2U and 1U
    uint16_t ADC_I_MOBC; // ADC出力値
	uint16_t ADC_V_GPS;
	uint16_t ADC_V_SUNS;
	uint16_t ADC_V_LORA; // ADC出力値
	uint16_t ADC_I_LORA; // ADC出力値
	//Only 1U
	uint16_t ADC_V_MTQA; // ADC出力値
	uint16_t ADC_I_MTQX; // ADC出力値
	uint16_t ADC_I_MTQY; // ADC出力値
	uint16_t ADC_I_MTQZ1Z2; // ADC出力値
	uint16_t ADC_I_MTQZ3Z4; // ADC出力値

	// analog voltage (using for ACS3)


} PWR_ADC_MOBC_STRUCT;

typedef enum
{
	// DMAのrank順に記載        //1U   2U
	ORDER_CURS_MOBC=0,   //PC3  PC3
	ORDER_CURS_LORA,     //PB1  PB1
	ORDER_VOLS_GPS,      //PC4  PC4
	ORDER_VOLS_LORA,     //PB0  PB0
	ORDER_VOLS_MTQA,     //PC0
	ORDER_CURS_MTQZ3Z4,  //PA2
	ORDER_CURS_MTQX,     //PA3
	ORDER_CURS_MTQY,     //PA4
	ORDER_CURS_MTQZ1Z2,  //PA5
    ORDER_ANA_GYROX,     //PA7
    ORDER_ANA_GYROY,     //PC1
	ORDER_ANA_GYROZ,     //PC2
} PWR_ADC_DMA_ORDER;

// MRAMに保存するアノマリ閾値の構造体
struct AT_VOLS{
	uint16_t AT_OV;
	uint16_t AT_LV;
	uint16_t AT_PSI;
	// threshold calculation memo
	//OV_3.3[V] : (3.5/2)[V]*(4096/3.3)~2170)
	//UV_3.3[V] : (3.0/2)[V]*(4096/3.3)~1860)
	//OV_5[V] : (5.2/2)[V]*(4096/3.3)~3230)
	//UV_5[V] : (4.8/2)[V]*(4096/3.3)~2980)
	//PSI[V] : (0.5/2)[V]**(4096/3.3)~310)
};


#ifdef __cplusplus
 extern "C" {
#endif


AppInfo PWR_ADC_MOBC_update(void);

CCP_CmdRet Cmd_PWR_ADC_MOBC_Read(const CommonCmdPacket* packet);

//uint16_t Get_ADC_Value(PWR_ADC_DMA_ORDER order);
uint16_t Get_ADC_Value(uint8_t order);

//
//extern uint8_t GPIO_State_GET(const uint8_t* GPIO_State,MOBC_GPIO_STATE type);



#ifdef __cplusplus
}
#endif

#endif /* PWR_ADC_MOBC_H_ */
