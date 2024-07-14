#include <src_user/Settings/System/flag_setting.h>
#include "eclipse_setting_driver.h"

static eclipse_STRUCT eclipse_driver;
eclipse_STRUCT* const p_eclipse_driver = &eclipse_driver;

void eclipse_flag_set(uint8_t* flag) {
  flag_set(flag);

  judge_eclipse_flag(p_eclipse_driver);
  p_eclipse_driver->eclipse_flag_tlm = vote_flag(&p_eclipse_driver->eclipse_flag);
}

void eclipse_flag_reset(uint8_t* flag) {
  flag_reset(flag);

  judge_eclipse_flag(p_eclipse_driver);
  p_eclipse_driver->eclipse_flag_tlm = vote_flag(&p_eclipse_driver->eclipse_flag);
}


//蝕判断の関数
//過半数の計算は/2を計算する方法もあるが, uint型を使いたいこと(整数型の割り算は使いたくない), 計算リソースの観点から割り算は使用しない
void judge_eclipse_flag(eclipse_STRUCT* myeclipse) {
  //蝕フラグの重みの合計
  int8_t sum = myeclipse->pwr_eclipse_weight + myeclipse->aocs_eclipse_weight + myeclipse->time_eclipse_weight;

  //全条件の重みが0であれば蝕検知はしない
  if(sum == 0x00) {
    flag_reset(&myeclipse->eclipse_flag);
  }

  //各条件で判断をしていたら, 蝕フラグの重みの合計から各条件の重みを引く
  //過半数考慮のため各条件の重みに2をかける → a>=b/2 <-> b-2a>=0
  if(myeclipse->pwr_eclipse_detection == 0xff){
    sum -= 2 * myeclipse->pwr_eclipse_weight;
  }
  if(myeclipse->aocs_eclipse_detection == 0xff){
    sum -= 2 * myeclipse->aocs_eclipse_weight;
  }
  if(myeclipse->time_eclipse_detection == 0xff){
    sum -= 2 * myeclipse->time_eclipse_weight;
  }

  //蝕フラグ判断
  if(sum <= 0) {
    flag_set(&myeclipse->eclipse_flag);
  }
  else{
    flag_reset(&myeclipse->eclipse_flag);
  }
}

//flagの初期化関数
void eclipse_flag_init(eclipse_STRUCT* myeclipse) {
  flag_reset(&myeclipse->pwr_eclipse_detection);
  flag_reset(&myeclipse->aocs_eclipse_detection);
  flag_reset(&myeclipse->time_eclipse_detection);
  flag_reset(&myeclipse->eclipse_flag);
  flag_reset(&myeclipse->eclipse_flag_tlm);
}
