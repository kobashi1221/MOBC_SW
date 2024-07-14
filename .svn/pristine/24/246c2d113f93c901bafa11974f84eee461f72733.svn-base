#ifndef eclipse_STRUCT_H_
#define eclipse_STRUCT_H_
#include <stdint.h> //for uint8_t

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct {
  //各条件の重み
  uint8_t pwr_eclipse_weight;
  uint8_t aocs_eclipse_weight;
  uint8_t time_eclipse_weight;

  //蝕検知フラグ
  uint8_t pwr_eclipse_detection;
  uint8_t aocs_eclipse_detection;
  uint8_t time_eclipse_detection;

  //蝕フラグ
  uint8_t eclipse_flag;
  uint8_t eclipse_flag_tlm;
} eclipse_STRUCT;

#ifdef __cplusplus
 }
#endif

#endif
