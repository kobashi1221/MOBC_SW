#include <src_user/Settings/System/flag_setting.h>

//アノマリ検知のEnableByteはめちゃめちゃ大事なので，0とffでbitの多数決を行う
//ついでに多いほうに寄せる処理も行っておく
uint8_t vote_flag(uint8_t* flag){
  uint8_t ret=0;
  for(int i=0;i<8;i++){
    ret+= ((*flag >> i) & 0x01);
  }
  if (ret>=4) {
    *flag = 0xff;
    return FLAG_ON;
  }
  else {
    *flag = 0x00;
    return FLAG_OFF;
  }
}

//フラグセット関数
void flag_set(uint8_t* flag) {
  *flag = 0xff;
}

//フラグリセット関数
void flag_reset(uint8_t *flag) {
  *flag = 0x00;
}
