/*
 * Global_PWR_Parameter.cpp
 *
 *  Created on: 2021/12/10
 *      Author: SAT63
 */


#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>
#include <src_user/Drivers/Power/PWR_Operation.h>
#include "Global_Sensor_Value.h"
#include <src_user/Library/General/DBG_printf.h>

PWR_STATE_STRUCT pwr_state;

static void pwr_global_param_init(void);
void PWR_REFLECT_MODE_(uint8_t num);

AppInfo PWR_GLOBAL_Param_update(void)
{
    //create_app_info()を呼び出す．
    return AI_create_app_info("Global_PWR_Param", pwr_global_param_init, NULL);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void pwr_global_param_init(void)
{
	print(PWR_MODE_FLAG,"PWR Global Param App initialize.\n");
	g_sensor.p_pwr_state = &pwr_state;
	pwr_state.State=PWR_GPIO_State;
	for(int i=0; i<PWR_State_MAX;++i){
		pwr_state.State[i]=0;
	}
	g_sensor.p_pwr_spc_guard = PWR_Sepcoil_Guard;
	for(int i=0; i<SEPCOIL_MAX; i++){
		PWR_Sepcoil_Guard[i]=0;
	}
}

void PWR_Mode_Set(uint32_t PWR_Mode_Num){
	pwr_state.PWR_MODE_Number = PWR_Mode_Num;
	for(int i=0; i<PWR_State_MAX;++i) PWR_REFLECT_MODE_(i);
}

void PWR_REFLECT_MODE_(uint8_t num){
	uint8_t bit=(uint8_t)((pwr_state.PWR_MODE_Number>>num)&0x01);
	bit == 1 ? pwr_state.State[num]=0xff : pwr_state.State[num]=0x00;
	print(PWR_MODE_FLAG,"PWR_Mode_Set : %d\r\n",bit);
}

uint8_t PWR_GET_MODE(uint8_t num){
	uint8_t bit=(uint8_t)((pwr_state.PWR_MODE_Number>>num)&0x01);
	print(PWR_MODE_FLAG,"PWR_COMPO_MODE, %d: %d\r\n",num,bit);
//	printf("PWR_COMPO: %d, bit: %d\r\n",num,bit);
	return bit;
}

void PWR_Mode_Trans(void){
//	printf("PWR_Mode_Num : %d\r\n",PWR_Mode_Num);
//  MOBC起動の際に電源が落ちるため，コメントアウト 2021/12/13 Otsuki

	// MRAM書き込みミスで起動時を読みだしてREG_CDH3V3をOFFにするようなモードに入るループを避ける 田村
	PWR_Operate_ON_OFF(PWR_State_REG_CDH3V3,1);

	if(PWR_VOTE(PWR_State_IOEX_PWR2)) PWR_Operate_ONInit(PWR_State_IOEX_PWR2);
	else PWR_Operate_ON_OFF(PWR_State_IOEX_PWR2,0);

	PWR_Operate_ON_OFF(PWR_State_GPS,PWR_VOTE(PWR_State_GPS));
	PWR_Operate_ON_OFF(PWR_State_SUNS,PWR_VOTE(PWR_State_SUNS));

	if(PWR_VOTE(PWR_State_LORA)) PWR_Operate_ONInit(PWR_State_LORA);
	else PWR_Operate_ON_OFF(PWR_State_LORA,0);

	if(PWR_VOTE(PWR_State_MTQA)) PWR_Operate_ONInit(PWR_State_MTQA);
	else PWR_Operate_ON_OFF(PWR_State_MTQA,0);

	if(PWR_VOTE(PWR_State_MTQO_PLASMA)) PWR_Operate_ONInit(PWR_State_MTQO_PLASMA);
	else PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);

    PWR_Operate_ON_OFF(PWR_State_SUNS_MXPZ,PWR_VOTE(PWR_State_SUNS_MXPZ));

    PWR_Operate_ON_OFF(PWR_State_SUNS_PXPYMY,PWR_VOTE(PWR_State_SUNS_PXPYMY));

    PWR_Operate_ON_OFF(PWR_State_GYRO,PWR_VOTE(PWR_State_GYRO));

    PWR_Operate_ON_OFF(PWR_State_SEMP,PWR_VOTE(PWR_State_SEMP));

#ifdef TWO_U
	if(PWR_VOTE(PWR_State_MIS1)) PWR_Operate_ONInit(PWR_State_MIS1);
	else PWR_Operate_ON_OFF(PWR_State_MIS1,0);

    PWR_Operate_ON_OFF(PWR_State_MAGS,PWR_VOTE(PWR_State_MAGS));

#endif
}
