/*
 * ODS_cmd_Propagater.h

 *
 *  Created on: 2022/07/04
 *      Author: Noro
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_ORBPROP_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_ORBPROP_H_


#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_user/Drivers/CDH/RTC/RTC.h>

#include "../../../../Drivers/Aocs/AOCSVector.h"
#include "../../../../Drivers/Aocs/Integrater.h"
#include "../../../../Drivers/Aocs/OrbitDynamics.h"
#include "../../../../Drivers/Aocs/Element2Cartesian.h"

typedef struct{
    IntegraterOptions options;
    ORBENV_STRUCT orbenv;
    cycle_t last_recv_time;//サイクル数での最終受信時刻も保持
    RTC_Info last_recv_rtc;//高精度な最終受信時刻が軌道計算には必要⇒msまで持ってるRTC_Infoで値保持
    AOCSVector current_y;//認識値（r,vの順番で格納）
    ORBENV_SET_STRUCT orbprop_set;
}ORBPROP_STRUCT;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo ORBPROP_param_update(void);
AppInfo ORBPROP_update(void);
AppInfo ORBPROP_Opponent_update(void);
ORBPROP_STRUCT* ODS_ORBPROP_Get_OrbpropPointer(void);
#ifdef __cplusplus
}
#endif

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_ORBPROP_H_ */
