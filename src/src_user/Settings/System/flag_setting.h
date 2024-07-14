#ifndef FLAG_SETTING_H_
#define FLAG_SETTING_H_
#include <stdint.h> //for uint8_t

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
  FLAG_OFF,
  FLAG_ON
} FLAG_NUM;

//VotingのCheck関数　中で0xffなら1, 0x00なら0を返して，反転を修正する機能もついてる
uint8_t  vote_flag(uint8_t* flag);
void flag_set(uint8_t* flag);
void flag_reset(uint8_t* flag);

#ifdef __cplusplus
 }
#endif

#endif
