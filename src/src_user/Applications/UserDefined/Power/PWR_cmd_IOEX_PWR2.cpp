/**
  *****************************************************************************************
  * @file    : PWR_IOEX_ADC_MUX.c
  * @author  : Hiroki OTSUKI
  * @version : 2.0
  * @date    : 2021/07/06
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  * This is driver instance of the driver of IOEXPANDER made by Mr.Okada.
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/09/25  1.0   N.KISHI         First revision
  * 2021/7/6    2.0   H.OTSUKI        Unified ON/OFF commands
  *
  *****************************************************************************************
  */

#include <src_user/Applications/UserDefined/Power/PWR_cmd_IOEX_PWR2.h>
#include <stdio.h>
#include <stdint.h> //for uint8_t
#include <string.h>
#include <src_user/Drivers/Power/MCP23017.h>

//#include <src_user/Drivers/PWR/MCP23017.h>
//#include <src_user/Drivers/PWR/MCP3421.h>
#include <src_user/IfWrapper/IF_class.h>
#include "../Global_Sensor_Value.h"
#include "src_user/Library/Delay.h"
#include "../IF_Instances.h"				//	gpios_out,gpios_inを使用するため
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Drivers/Power/PWR_Operation.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

#define HIGH 0x01
#define LOW 0x00

#define MCP23017_ADDR 0x40
#define MCP23017_PWR2_ADDR 0x48

//#define MCP3421_ADDR 0xD0
//#define MCP3421_PWR2_ADDR 0xD4
//#define TEST1 GATE3V3_FET_GPS_GPIO_Port // 後で変更する

//いずれここでない場所にまとめる
//extern I2C_HandleTypeDef hi2c1; //IOEXPANDERのI2Cに変更する

extern PWR_STATE_STRUCT pwr_state;

//pwrっぽい名前に変える？
//MCP23017 mcp23017_; //new演算子で作成した方がいい？とりあえずグローバルオブジェクトとして定義する
MCP23017 mcp23017_(MCP23017_PWR2_ADDR, &hi2c2); //コンストラクタここで実行するversion
const MCP23017* mcp23017;
//MCP3421 adc1(MCP3421_PWR2_ADDR, &hi2c2); //ADC_PWR2 Object
//MCP3421 adc2(MCP3421_ADDR, hi2c2); //ADC_CDH1 Object

extern GPIO_Class gpios_out[GPIO_OUT_MAX];		//CとC++のコンフリクトを避けるためここでexternをしている

static void pwr_IOEXPANDER_init(void); //アプリ初期化関数のプロトタイプ宣言

/**
 * @brief アプリケーション作成関数
 *
 * @note
 * @return アプリケーション構造体
 */
AppInfo PWR_IOEXPANDER_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("IOEXPANDER", pwr_IOEXPANDER_init, NULL);
}

/**
 * @brief IOEXPANDER,ADCの電源ON,初期化を実行
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void pwr_IOEXPANDER_init(void)
{
    printf("PWR IOEXPANDER Debug App initialize.\n"); //別に無くてもいい
    PWR_Operate_ON_OFF(PWR_State_IOEX_PWR2,1);
//    gpios_out[GATE3V3_MOBCtoFET_ADCIOEX_PWR2].Write(1);  // IOEXPANDERの電源をON(MOBCが実行)
    c2a_delay_ms(50);
    mcp23017_.init();
    c2a_delay_ms(10);
    PWR_Operate_IOEX_init();    // この処理をしないとIOEX初期化後にトリップ付きFETが全てEnableされてしまう　2023/08/09 Kanie

#ifdef TWO_U
//	PWR_Operate_ON_OFF(PWR_State_MIS1,1);
//    c2a_delay_ms(10); //10msで十分 2021/11/12 Otsuki
//    PWR_Operate_ON_OFF(PWR_State_MIS1,0);
#endif
//    mcp23017_.digitalWrite(IOEX_PORT_MIS1,0); // FET_MIS1
//    c2a_delay_ms(10); //10msで十分 2021/11/12 Otsuki
//    mcp23017_.digitalWrite(IOEX_PORT_MIS1,1); // FET_MIS1
}


CCP_CmdRet Cmd_PWR_IOEX_PWR2_Init(const CommonCmdPacket* packet)
{
    (void)packet;
    mcp23017_.init();
    printf("IOEX_INIT\r\n");
    c2a_delay_ms(10);
    PWR_Operate_IOEX_init();    // この処理をしないとIOEX初期化後にトリップ付きFETが全てEnableされてしまう　2023/08/09 Kanie
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_SUNS_MXPZの操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SUNS_MXPZ_ON_OFF(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
//    printf("PWR FET_SUNS_MXPZ Debug. FET execute.\r\n");

    PWR_Operate_ON_OFF(PWR_State_SUNS_MXPZ,data);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_SUNS_PXPYMYの操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SUNS_PXPYMY_ON_OFF(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
//    printf("PWR FET_SUNS_PXPYMY Debug. FET execute.\r\n");

    PWR_Operate_ON_OFF(PWR_State_SUNS_PXPYMY,data);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_GYROの操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_GYRO_ON_OFF(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
//    printf("PWR FET_GYRO Debug. FET execute.\r\n");

    PWR_Operate_ON_OFF(PWR_State_GYRO,data);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_SEMP(PANEL)の操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SEMP_ON_OFF(const CommonCmdPacket* packet)	// ONResetもいれるべき？
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
//    printf("PWR FET_SEMP Debug. FET execute.\r\n");

    PWR_Operate_ON_OFF(PWR_State_SEMP,data);	// ONResetもいれるべき？

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_SEPCOIL1の操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SEPCOIL1_ON_OFF(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
//    printf("PWR FET_SEPCOIL1 Debug. FET execute.\r\n");

    PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,data);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_SEPCOIL2の操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SEPCOIL2_ON_OFF(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
//    printf("PWR FET_SEPCOIL2 Debug. FET execute.\r\n");
    PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,data);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_SEPCOIL3の操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SEPCOIL3_ON_OFF(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
    PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,data);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_SEPCOIL4の操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SEPCOIL4_ON_OFF(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
    PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,data);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief SEPCOIL1 GUARDフラグ 操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SEPCOIL1_GUARD_ON_OFF(const CommonCmdPacket* packet)
{
    size_t len=CCP_get_param_len(packet);
    const uint8_t *param = CCP_get_param_head(packet);
    uint8_t data;
    memcpy(&data,param,len);

    print(PWR_OPERATION,"PWR SEPCOIL1_GUARD Flag: %d\r\n", data);
    PWR_Sepcoil_Guard_SET(SEPCOIL_1,data);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief SEPCOIL2 GUARDフラグ 操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SEPCOIL2_GUARD_ON_OFF(const CommonCmdPacket* packet)
{
    size_t len=CCP_get_param_len(packet);
    const uint8_t *param = CCP_get_param_head(packet);
    uint8_t data;
    memcpy(&data,param,len);

    print(PWR_OPERATION,"PWR SEPCOIL2_GUARD Flag: %d\r\n", data);
    PWR_Sepcoil_Guard_SET(SEPCOIL_2,data);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief SEPCOIL3 GUARDフラグ 操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SEPCOIL3_GUARD_ON_OFF(const CommonCmdPacket* packet)
{
    size_t len=CCP_get_param_len(packet);
    const uint8_t *param = CCP_get_param_head(packet);
    uint8_t data;
    memcpy(&data,param,len);

    print(PWR_OPERATION,"PWR SEPCOIL3_GUARD Flag: %d\r\n", data);
    PWR_Sepcoil_Guard_SET(SEPCOIL_3,data);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief SEPCOIL4 GUARDフラグ 操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_SEPCOIL4_GUARD_ON_OFF(const CommonCmdPacket* packet)
{
    size_t len=CCP_get_param_len(packet);
    const uint8_t *param = CCP_get_param_head(packet);
    uint8_t data;
    memcpy(&data,param,len);

    print(PWR_OPERATION,"PWR SEPCOIL4_GUARD Flag: %d\r\n", data);
    PWR_Sepcoil_Guard_SET(SEPCOIL_4,data);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief FET_MIS1の操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_MIS1_ON_OFF_ONReset(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
    printf("PWR FET_MIS1 Debug. FET execute.\r\n");
    PWR_Operate_ON_OFF(PWR_State_MIS1,data);
    if(data==2){
    	PWR_Operate_ONInit(PWR_State_MIS1);
    }

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#ifdef TWO_U
/**
 * @brief FET_MAGSの操作コマンド 実行関数
 *
 */
CCP_CmdRet Cmd_PWR_MAGS_ON_OFF(const CommonCmdPacket* packet)
{
	size_t len=CCP_get_param_len(packet);
	const uint8_t *param = CCP_get_param_head(packet);
	uint8_t data;
	memcpy(&data,param,len);
//    printf("PWR FET_MAGS Debug. FET execute.\r\n");

    PWR_Operate_ON_OFF(PWR_State_MAGS,data);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#endif TWO_U
