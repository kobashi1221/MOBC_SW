
#ifndef SRC_USER_APPLICATIONS_USERDEFINED_MISSION_GLOBAL_MISSION_PARAMETER_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_MISSION_GLOBAL_MISSION_PARAMETER_H_

#include <stdint.h> //for uint8_t

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd


typedef struct{
    uint32_t PWR_MISSION_MODE_Number;
    uint32_t MISSION_MODE_Number;
}MISSION_MODE_STRUCT;

typedef struct{
  MISSION_MODE_STRUCT MISSION_MODE;
}GLOBAL_MISSION_PARAMETER_STRUCT;

typedef enum{
  DEBUG1,
}PWR_MISSION_MODE;

typedef enum{
  DEBUG2,
  DEBUG3
}MISSION_MODE;


void PWR_Mission_Mode_Set(uint32_t PWR_Mission_Mode_Num);
void Mission_Mode_Set(uint32_t Mission_Mode_Num);


#endif
