#include "CDH_Mode_Transition_driver.h"

//フラグを多数決して, 最も多いところに遷移させる
MD_AOCSMODEID aoce_trans_vote(uint32_t* aocs){
	uint8_t ret_standby=0;
	uint8_t ret_dumping=0;
	uint8_t ret_sun_pointing=0;
	for(int i = 0;i<8;i++){
		ret_standby+=((*aocs >> i) &0x01);
	}
	for(int i = 8;i<16;i++){
		ret_dumping+=((*aocs >> i) &0x01);
	}
	for(int i = 16;i<24;i++){
		ret_sun_pointing+=((*aocs >> i) &0x01);
	}

	if(ret_standby > ret_dumping && ret_standby > ret_sun_pointing){
		return MD_AOCSFLAG_STANDBY;
	}
	else if(ret_dumping > ret_standby && ret_dumping > ret_sun_pointing){
		return MD_AOCSFLAG_DAMPING;
	}
	else if(ret_sun_pointing > ret_standby && ret_sun_pointing > ret_dumping){
		return MD_AOCSFLAG_SUN_POINTING;
	}
	else{//どれにも属さないときはダンピング
		return MD_AOCSFLAG_DAMPING;
	}
}

//フラグを多数決して, 最も多いところに遷移させる
MD_MODEID modeBC_trans_vote(uint32_t* mode){
	uint8_t ret_standby=0;
	uint8_t ret_mission1=0;
	uint8_t ret_mission2=0;
	uint8_t ret_mission3=0;
	for(int i = 0;i<8;i++){
		ret_standby+=((*mode >> i) &0x01);
	}
	for(int i = 8;i<16;i++){
		ret_mission1+=((*mode >> i) &0x01);
	}
	for(int i = 16;i<24;i++){
		ret_mission2+=((*mode >> i) &0x01);
	}
	for(int i = 24;i<32;i++){
		ret_mission3+=((*mode >> i) &0x01);
	}

	if(ret_standby > ret_mission1 && ret_standby > ret_mission2 && ret_standby > ret_mission3){
		return MD_MODEID_SUN_POINTING;
	}
	else if(ret_mission1 > ret_standby && ret_mission1 > ret_mission2 && ret_mission1 > ret_mission3){
		return MD_MODEID_DAMPING;
	}
	else if(ret_mission2 > ret_standby && ret_mission2 > ret_mission1 && ret_mission2 > ret_mission3){
		return MD_MODEID_MISSION_STANDBY;
	}
	else if(ret_mission3 > ret_standby && ret_mission3 > ret_mission1 && ret_mission3 > ret_mission2){
		return MD_MODEID_STANDBY;
	}
	else{//どれにも属さないときはStandby
		return MD_MODEID_STANDBY;
	}
}

MD_AOCSMODEID AOCS_mode_change_checker(MD_AOCSMODEID before_mode,MD_AOCSMODEID after_mode){
	if(before_mode == MD_AOCSFLAG_HIGH_RATE_SPIN && after_mode == MD_AOCSFLAG_STANDBY){
		return MD_AOCSFLAG_DAMPING;
	}
	//追記していく
	else{//問題がなければそのまま返す
		return after_mode;
	}
}

//自動モード遷移には誤コマンド防止のためフラグを付ける
//勝手にbit反転しても衛星が死ぬ可能性がある
//多数決を行い, 多いほうを採用する
uint8_t auto_flag_vote(uint8_t* flag){
	uint8_t ret=0;
	for(int i=0;i<8;i++){
		ret+= ((*flag >> i) & 0x01);
	}
	if (ret>=4) {
		*flag = 0xff;
		return 1;
	}
	else {
		*flag = 0x00;
		return 0;
	}
}

void Auto_trans_set(uint8_t* flag){
	*flag = 0xff;
}

void Auto_trans_reset(uint8_t *flag){
	*flag = 0x00;
}
