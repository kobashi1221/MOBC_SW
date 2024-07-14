/**
  *****************************************************************************************
  * @file    : PWR_FETREG.c
  * @author  : Hiroki OTSUKI
  * @version : 1.0
  * @date    : 2020/07/20
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/07/20  1.0   H.OTSUKI        First revision
  *
  *****************************************************************************************
  */

#include <src_user/Applications/UserDefined/Power/PWR_cmd_FETREG.h>

#include <stdio.h>
#include <string.h>
#include <stdint.h> //for uint8_t

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../IF_Instances.h"                //  gpios_out,gpios_inを使用するため
#include "../Global_Sensor_Value.h"
#include "src_user/Library/Delay.h"
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Drivers/Power/PWR_Operation.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include "PWR_cmd_AnomalyDetect.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/DriverInstances/di_rxpic.h>

#define TEST1 GPIOD // 後で変更する

extern PWR_STATE_STRUCT pwr_state;

static void PWR_FETREG_init(void); //アプリ初期化関数のプロトタイプ宣言
static void PWR_FETREG_exe(void); //アプリ実行関数のプロトタイプ宣言

static void PWR_FETREG_load_param(void);
static void PWR_FETREG_init_param(void);

const uint32_t lora_auto_on_time_ = 0x00014820;
static uint32_t lora_auto_on_time;

/**
 * @brief アプリケーション作成関数
 *
 * @note
 * @return アプリケーション構造体
 */
AppInfo PWR_FETREG_update(void)
{
    //create_app_info()を呼び出す
    //
    return AI_create_app_info("DBG_FETREG", PWR_FETREG_init, PWR_FETREG_exe); //アプリを作成
}

AppInfo PWR_FETREG_param_update(void)
{
    return AI_create_app_info("FET_PARAM_UPDATE", PWR_FETREG_load_param, PWR_FETREG_init_param);
}

static void PWR_FETREG_load_param(void)
{
  printf("fetreg load param\r\n");
  read_param(&lora_auto_on_time, &lora_auto_on_time_, 4, (uint32_t)0x00004000);
}

static void PWR_FETREG_init_param(void)
{
  printf("fetreg init param\r\n");
  write_param(&lora_auto_on_time, &lora_auto_on_time_, 4, (uint32_t)0x00004000);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する
 */
static void PWR_FETREG_init(void)
{
    int ret;

    printf("PWR FETREG Debug App initialize.\n"); //別に無くてもいい

	//エラー処理. printfの中身を適当に変更してください.
    if (ret != 0)
    {
        printf("PWR FETREG init Failed! %d \r\n", ret);
    }
}

/**
 * @brief アプリケーション実行関数
 *
 * 中身は特に定義していない．
 * 処理内容は基本的にコマンド実行関数（Cmd_という名前の関数）で定義すればいいと思う．
 * ここに何を書くかは追々考える．
 */
static void PWR_FETREG_exe(void)
{
    printf("sample driver execution.\r\n");

}


/**
 * @brief FET_GPSの操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
CCP_CmdRet Cmd_PWR_GPS_ON_OFF(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
	PWR_Operate_ON_OFF(PWR_State_GPS,data);
//	printf("PWR_State_GPS after : %d\r\n",g_sensor.p_pwr_state->mobc.GPIO_State[0]);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_SUNSの操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
CCP_CmdRet Cmd_PWR_SUNS_ON_OFF(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);

	PWR_Operate_ON_OFF(PWR_State_SUNS,data);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_LORAの操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */

CCP_CmdRet Cmd_PWR_LORA_ON_OFF_ONInit(const CommonCmdPacket* packet)
{

	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);


	PWR_Operate_ON_OFF(PWR_State_LORA,data);
	if(data == 2){
		PWR_Operate_ONInit(PWR_State_LORA);
	}

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief REG_CDH3V3の操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */

CCP_CmdRet Cmd_PWR_REG_CDH3V3_ON_OFF(const CommonCmdPacket* packet)
{

	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);

	if(data == 0){
		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
#ifdef ONE_U
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
#endif
#ifdef TWO_U
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
	}
	PWR_Operate_ON_OFF(PWR_State_REG_CDH3V3,data);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief IOEX_PWR2の操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */

CCP_CmdRet Cmd_PWR_IOEX_PWR2_ON_OFF_ONInit(const CommonCmdPacket* packet)
{

	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);

	if(data == 0){
	  PWR_Operate_ON_OFF(PWR_State_IOEX_PWR2,0);
    }
	// IOEX ON時に必ず初期化するように修正．(初期化せずに，ただONするだけだとFETなどの操作ができない) 2023/08/11 Kanie
	else if(data == 1){
//    printf("IOEX_ADC_PWR2 ON and Initialize.\r\n");
	  PWR_Operate_ONInit(PWR_State_IOEX_PWR2);
	}
	else if(data == 2){
//	  printf("IOEX_ADC_PWR2 ON and Initialize.\r\n");
      PWR_Operate_ONInit(PWR_State_IOEX_PWR2);
	}

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_MTQAの操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */

CCP_CmdRet Cmd_PWR_MTQA_ON_OFF_ONInit(const CommonCmdPacket* packet)
{

	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);

	if(data == 0){
		// MTQAを消す際にMTQOも消す
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
	}
	else if (data == 1){
		// MTQはOnだけだと出力無制御で、何かしら一定値にしておく
		PWR_Operate_ONInit(PWR_State_MTQA);
//		PWR_Operate_ON_OFF(PWR_State_MTQA,1);

	}

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_MTQO_PLASMAの操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */

CCP_CmdRet Cmd_PWR_MTQO_PLASMA_ON_OFF_ONInit(const CommonCmdPacket* packet)
{

	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);

	if(data == 0){
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
	}
	else if(data == 1){
		// MTQOをつける際にMTQAもつける
		PWR_Operate_ONInit(PWR_State_MTQA);
		// MTQはONだけだと出力無制御で、何かしら一定値にしておく
		PWR_Operate_ONInit(PWR_State_MTQO_PLASMA);
	}

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief FET_LORAの自動ONコマンド
 * 　　　　　　AFSKを通しての地上局からの通信が一定時間, 来なかったときにLORAをONにする
 */
CCP_CmdRet Cmd_PWR_LORA_AUTO_ONInit(const CommonCmdPacket* packet) {
  (void)packet;

  //RXPICのOBC時刻はは250msで1count upするため÷4, UL_latest_timeは真ん中の2byteを取得するため×256÷4する必要がある
  uint32_t rxpic_obc_time = rxpic_driver->tlm_data.rxpic_to_mobc.RXPIC_Time / 4;
  uint32_t afsk_ul_latest_time = rxpic_driver->tlm_data.rxpic_to_mobc.RXfromGS_LatestTime * 64;
  uint32_t diff_time = rxpic_obc_time - afsk_ul_latest_time;

  if(rxpic_obc_time < afsk_ul_latest_time){//計算によってdiff_timeが負になる時、抜ける
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  }

  diff_time = rxpic_obc_time - afsk_ul_latest_time;
  //diff_timeが衛星パラメータで設定された時間を超えたらLoRaをONにする
  if((uint32_t)diff_time > lora_auto_on_time){
    uint8_t state = PWR_State_GET(PWR_State_LORA);
    if(state == 0x00) { // 既に電源ONの時にループを防ぐ
      PWR_Operate_ONInit(PWR_State_LORA);
    }
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
