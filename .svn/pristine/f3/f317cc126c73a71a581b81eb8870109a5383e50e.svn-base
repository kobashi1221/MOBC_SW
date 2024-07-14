/*
 * PWR_State.c
 *
 *  Created on: 2021/12/04
 *      Author: SAT63
 */

#include <stdio.h>
#include "PWR_State.h"
#include "src_user/Library/General/DBG_printf.h"
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>

//#include "MCP3421.h"

//上流のアプリで上位の構造体のメンバーと繋いでいる
uint8_t PWR_GPIO_State[PWR_State_MAX];

//SEPCOIL分離保護用
uint8_t PWR_Sepcoil_Guard[SEPCOIL_MAX];

void PWR_State_SET(PWR_STATE type,uint8_t state){
	state==0 ? PWR_GPIO_State[type]=0x00: PWR_GPIO_State[type]=0xff;
}

uint8_t PWR_State_GET(PWR_STATE type){
	return PWR_VOTE(type);
}

uint8_t PWR_VOTE(PWR_STATE num){
	uint8_t ret=0;
	for(int i=0;i<8;i++){
		ret+= ((PWR_GPIO_State[num] >> i) & 0x01);
	}
	if (ret>=4) {
		PWR_GPIO_State[num] = 0xff;
		return 1;
	}
	else {
		PWR_GPIO_State[num] = 0x00;
		return 0;
	}
}


void PWR_Sepcoil_Guard_SET(uint8_t num, uint8_t state){
	state==0 ? PWR_Sepcoil_Guard[num]=0x00: PWR_Sepcoil_Guard[num]=0xff;
}

uint8_t PWR_Sepcoil_Guard_GET(uint8_t num){
	uint8_t ret=0;
	for(int i=0;i<8;i++){
		ret+= ((PWR_Sepcoil_Guard[num] >> i) & 0x01);
	}
	if (ret>=4) {
		PWR_Sepcoil_Guard[num] = 0xff;
		return 1;
	}
	else {
		PWR_Sepcoil_Guard[num] = 0x00;
		return 0;
	}
}
