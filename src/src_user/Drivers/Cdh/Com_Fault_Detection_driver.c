#include "Com_Fault_Detection_driver.h"


void ANOMALY_Port_init_(int num, ANOMALY_DRIVER_STRUCT *str, void* handler, uint8_t enable){
  str->who_am_i=num;
  enable == 1 ? flag_set(&str->anomaly_enable): flag_reset(&str->anomaly_enable);
  ANOMALY_Default_Settings(str);
  str->handler=handler;
  OBCT_clear(&(str->last_rcv_time));
}

void ANOMALY_Default_Settings(ANOMALY_DRIVER_STRUCT *str){
  //アノマリEnableはBit反転したら終わるので，0xffでVotingする．
  str->anomaly_detect_lv1=0;
  str->anomaly_detect_lv2=0;
  str->anomaly_detect_lv3=0;
  str->thres_lv1_counter=0;
  str->thres_lv2_counter=0;
  str->thres_lv3_counter=0;
  str->thres_lv2[1]=0xffffff00;
  str->thres_lv3[1]=0xffffff00;
  str->ack=HAL_OK;
  str->last_rcv_time=TMGR_get_master_clock();
}

void ANOMALY_HAL_ACK_Detect(uint16_t *count, ANOMALY_DRIVER_STRUCT *str){
  if(vote_flag(&(str->anomaly_enable))){
    if((str->ack != HAL_OK) && (str->ack != HAL_BUSY)){
      flag_set(&str->anomaly_detect_lv1);
      (*count)++;
      if(*count>0xff00) *count=0;
    }
  }
}
