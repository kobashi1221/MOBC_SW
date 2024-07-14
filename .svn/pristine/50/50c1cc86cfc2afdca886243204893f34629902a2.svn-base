/*
 * ODS_cmd_ORBEKF.h
 *
 *  Created on: Aug 25, 2023
 *      Author: nanos
 */

#ifndef SRC_SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_ORBEKF_H_
#define SRC_SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_ORBEKF_H_

#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_user/Drivers/CDH/RTC/RTC.h>

#include "../../../../Drivers/Aocs/OrbitDynamics.h"
#include "../../../../Drivers/Aocs/OD_EKF.h"
#include "../../../../Drivers/Aocs/AOCSVector.h"


typedef struct{
    cycle_t last_recv_time;//サイクル数での最終受信時刻も保持
    RTC_Info last_recv_rtc;//高精度な最終受信時刻が軌道計算には必要⇒msまで持ってるRTC_Infoで値保持
    EKF_Param ekf;
    EKF_fun fun;
    ORBENV_STRUCT* orbenv;//ORBPROPの実体とつなぐ
}ORBEKF_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

AppInfo ORBEKF_param_update(void);
AppInfo ORBEKF_update(void);

#ifdef __cplusplus
}
#endif



#endif /* SRC_SRC_USER_APPLICATIONS_USERDEFINED_AOCS_ODS_ODS_CMD_ORBEKF_H_ */
