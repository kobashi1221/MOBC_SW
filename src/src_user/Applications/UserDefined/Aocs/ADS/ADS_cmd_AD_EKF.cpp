/*
 * ADS_cmd_AD_EKF.cpp
 *
 *  Created on: 2021/06/23
 *      Author: masud
 */

#include "ADS_cmd_AD_EKF.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Drivers/Aocs/AD_EKF.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <stdlib.h>
#include <string.h>
#include "../../Global_Sensor_Value.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>

static AD_EKF_CLASS AD_EKF_class;
//static AD_EKF_STRUCT AD_EKF_;
//const AD_EKF_STRUCT* AD_EKF;

static void AD_EKF_init(void);
static void AD_EKF_exe(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo AD_EKF_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("DBG_AD_EKF", AD_EKF_init, AD_EKF_exe);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void AD_EKF_init(void)
{
    printf("AD_EKF driver initialize.\n");
//	AD_EKF = &AD_EKF_;
//	AD_EKF_class.reset();
	AD_EKF_class.reset(g_ads.p_QUEST->quaternion,g_acs.p_GLOBAL_AOCS->Omega);

}

/**
 * @brief アプリケーション実行関数
 *
 */
static void AD_EKF_exe(void)
{

	float omega[3] = {g_acs.p_GLOBAL_AOCS->Omega[0], g_acs.p_GLOBAL_AOCS->Omega[1], g_acs.p_GLOBAL_AOCS->Omega[2]};

	AD_EKF_class.cal(g_ads.p_QUEST->quaternion, g_acs.p_GLOBAL_AOCS->Omega);



//	printf("quest %f %f %f %f \r\n", g_ads.p_QUEST->quaternion[0], g_ads.p_QUEST->quaternion[1], g_ads.p_QUEST->quaternion[2], g_ads.p_QUEST->quaternion[3]);

}

/**
 * @brief コマンド実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
CCP_CmdRet Cmd_ADS_AD_EKF(const CommonCmdPacket* packet)
{
//	printf("AD_EKF\r\n");
//
//	float omega[3] = {g_acs.p_GLOBAL_AOCS->Omega[0], g_acs.p_GLOBAL_AOCS->Omega[1], g_acs.p_GLOBAL_AOCS->Omega[2]};
//
//	AD_EKF_class.cal(g_ads.p_QUEST->quaternion, omega);
//
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


