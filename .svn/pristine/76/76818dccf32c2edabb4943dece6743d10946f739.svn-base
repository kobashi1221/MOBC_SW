/*
 * ODS_cmd_SGP4.h

 *
 *  Created on: 2021/10/28
 *      Author: masud
 */

/*
 * ODS_cmd_sgp4.h
 *
 *  Created on: 2021/09/26
 *      Author: Inakawa san
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_SGP4_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_SGP4_H_


#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd



typedef struct
{
	//----グローバル格納値---------
	float sgp4_pos[3];
	float sgp4_vel[3];

	float sgp4_Velocity_Vector_i[3]; // Inertia
	float sgp4_Velocity_Vector_b[3]; // Body

} SGP4_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

AppInfo SGP4_param_update(void);
AppInfo SGP4_update(void);
AppInfo SGP4_Opponent_update(void);

#ifdef __cplusplus
}
#endif


#endif /* SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_SGP4_H_ */
