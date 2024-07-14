#ifndef PHASE_SETTING_DRIVER_H_
#define PHASE_SETTING_DRIVER_H_
#include <stdint.h> //for uint8_t

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
  CRITICAL,
  INITIAL_OPERATION,
  NORMAL_OPERATION,
  PHASE_MAX,
} PHASE_NUM;

typedef struct {
  uint8_t current_phase;

  uint8_t phase_redundancy1;
  uint8_t phase_redundancy2;
  uint8_t phase_redundancy3;
} PHASE_STRUCT;

void Judge_phase_flag(PHASE_STRUCT* phase_driver);

#ifdef __cplusplus
 }
#endif

#endif
