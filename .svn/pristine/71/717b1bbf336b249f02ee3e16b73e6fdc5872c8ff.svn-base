/*
 * PWR_cmd_AnomalyHandle.h
 *
 *  Created on: 2021/12/03
 *      Author: Otsuki
 */

#ifndef SRC_USER_APPLICATIONS_USERDEFINED_PWR_PWR_CMD_ANOMALYHANDLE_H_
#define SRC_USER_APPLICATIONS_USERDEFINED_PWR_PWR_CMD_ANOMALYHANDLE_H_

#include <stdio.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.h>

typedef struct
 {
// 	uint16_t thres; // Threshold，基本的に変わらないのでROMに保存
 	uint8_t flag;  // handlingのフラグ
 	uint8_t count; // handlingカウント,積算値なのでSDに保存
 } PWR_ANOMALY_DRIVER_STRUCT;

 typedef struct
 {
 	// 再電源投入回数を制限するアノマリ（過電流，過電圧,PSI_OFF(HW過電流検知）に対して使用
 	uint8_t flag;		// 再電源投入フラグ
 	uint8_t count;      // 再電源投入カウント
 } PWR_RESTART_STRUCT;

 typedef struct
 {
 	PWR_ANOMALY_DRIVER_STRUCT anomaly;
 	PWR_RESTART_STRUCT restart;
 } PWR_ANOMALY_RESTART_STRUCT;

 typedef struct
 {
 	PWR_ANOMALY_RESTART_STRUCT oc;
 	PWR_ANOMALY_DRIVER_STRUCT uc;
 } PWR_ANOMALY_CURS_STRUCT;

 typedef struct
 {
 	PWR_ANOMALY_RESTART_STRUCT ov;
 	PWR_ANOMALY_DRIVER_STRUCT uv;
 	PWR_ANOMALY_DRIVER_STRUCT psi_on;
 	PWR_ANOMALY_RESTART_STRUCT psi_off;
 } PWR_ANOMALY_VOLS_STRUCT;

 typedef struct
 {
 	PWR_ANOMALY_CURS_STRUCT curs[MAX_CURRENT_SENSOR];
 	PWR_ANOMALY_VOLS_STRUCT vols[MAX_VOLTAGE_SENSOR];

 	// MOBCがアノマリ検知し自爆するループから逃れるための制限パラメータ
 	uint8_t shut_down_limit;

 } PWR_ANOMALY_STRUCT;



#ifdef __cplusplus
 extern "C" {
#endif

extern PWR_ANOMALY_STRUCT pwr_anomaly;


AppInfo PWR_ANOMALY_Handle_update(void);

// Anomaly handling
CCP_CmdRet Cmd_PWR_AH_OC(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_PWR_AH_UC(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_PWR_AH_OV(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_PWR_AH_UV(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_PWR_AH_PSI_ON(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_PWR_AH_PSI_OFF(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_PWR_AH_SYSTEM_ENABLE(const CommonCmdPacket* packet);

void PWR_AH_CURS_OFF(uint8_t curs);
void PWR_AH_CURS_Restart(uint8_t curs);
void PWR_AH_VOLS_OFF(uint8_t vols);
void PWR_AH_VOLS_Restart(uint8_t vols);
void PWR_AH_TXPIC_REG_CDH3V3_Restart(void);

#ifdef __cplusplus
 }
#endif

#endif /* SRC_USER_APPLICATIONS_USERDEFINED_PWR_PWR_CMD_ANOMALYHANDLE_H_ */
