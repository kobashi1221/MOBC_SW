
#ifndef ACS3_DEBUG_GYRO_H_
#define ACS3_DEBUG_GYRO_H_
#ifdef ONE_U
#include <stdint.h> //for uint8_t
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include "ACS123_cmd_AnomalyDetect.h"

typedef struct
{
	uint8_t state[3];
	float omega[3];
	float OMEGA[3];
	uint16_t ADC_OMEGAX;
	uint16_t ADC_OMEGAY;
	uint16_t ADC_OMEGAZ;
} ACS3_DEBUG_GYRO_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

AppInfo ACS3_debug_gyro_update(void);
AppInfo ACS3_GYRO_param_update(void);

CCP_CmdRet Cmd_ACS3_debug_gyro_read(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS3_Change_GYRO_Range(const CommonCmdPacket* packet);


#ifdef __cplusplus
}
#endif
#endif /*for 1U*/
#endif /* ACS3_DEBUG_MTQ_H_ */
