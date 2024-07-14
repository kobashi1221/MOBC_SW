#include <stdio.h>
#include <math.h>
#include <string.h>
#include <src_user/IfWrapper/HAL.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_ADC_MOBC.h>
#include "../Global_Sensor_Value.h"
#include "src_user/Library/Delay.h"
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include<src_user/Library/General/DBG_printf.h>
#include "PWR_cmd_ADC_PWR2.h"
#include "PWR_cmd_AnomalyDetect.h"
#include "PWR_cmd_AnomalyHandle.h"
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_user/Library/Delay.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

static PWR_ADC_MOBC_STRUCT pwr_adc_mobc; // デバッグ用構造体
//static PWR_FETREG_STRUCT pwr_fetreg; // デバッグ用構造体
//extern PWR_ANOMALY_STRUCT pwr_anomaly; // アノマリ用構造体
//const PWR_ADC_MOBC_STRUCT* g_sensor.p_pwr_adc_mobc; //デバッグ用構造体ポインタ格納用

static void PWR_ADC_MOBC_init(void);
static void PWR_ADC_MOBC_exe(void);
static void ADC_MOBC_initialize_param(void);
static void ADC_MOBC_load_param(void);


/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo PWR_ADC_MOBC_update(void)
{
    return AI_create_app_info("DBG_ADC_MOBC", PWR_ADC_MOBC_init, PWR_ADC_MOBC_exe);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void PWR_ADC_MOBC_init(void)
{
    printf("PWR ADC_MOBC Debug initialize.\n");
    g_sensor.p_pwr_adc_mobc = &pwr_adc_mobc;
}

/**
 * @brief アプリケーション実行関数
 *
 */
static void PWR_ADC_MOBC_exe(void)
{
    printf("sample driver execution.\n");
}

CCP_CmdRet Cmd_PWR_ADC_MOBC_Read(const CommonCmdPacket* packet)
{
	int ret = 0;
	size_t len=CCP_get_param_len(packet);
	const uint8_t *head_param = CCP_get_param_head(packet);
	uint8_t param[1];

	memcpy(param,head_param,len);

	uint16_t rawData = Get_ADC_Value(param[0]);
	uint8_t state = 0;  // PWR state

	// c2a_delay_ms(10); // DMA DMAContinuousRequests で値自体は常に取得している 1.6mA

	switch(param[0]){
	case ORDER_CURS_MOBC:
		pwr_adc_mobc.ADC_I_MOBC = rawData;
//		printf("ADC_I_MOBC : %d\r\n",rawData);
		AD_CURS(AL_CURS_MOBC,rawData,&g_pwr_thres->AT_CURS_MOBC);
		break;
	case ORDER_CURS_LORA:
		pwr_adc_mobc.ADC_I_LORA = rawData;
		if(g_pwr_awc==0){
			AD_CURS(AL_CURS_LORA,rawData,&g_pwr_thres->AT_CURS_LORA);
		}
		break;
	case ORDER_VOLS_GPS:
		pwr_adc_mobc.ADC_V_GPS = rawData;
		state = PWR_State_GET(PWR_State_GPS);
//		printf("ADC_V_GPS : %d, state : %d\r\n",rawData,state);
		if(g_pwr_awc==0){
			AD_VOLS_FET(AL_VOLS_GPS,rawData,g_pwr_thres->AT_VOLS_GPS,state);
		}
		break;
	case ORDER_VOLS_LORA:
		pwr_adc_mobc.ADC_V_LORA = rawData;
//		printf("ADC_VOLS_LORA : %d\r\n",rawData);
		state = PWR_State_GET(PWR_State_LORA);
//		printf("PWR_state_LORA : %d\r\n",state);
		if(g_pwr_awc==0){
			AD_VOLS_FET(AL_VOLS_LORA,rawData,g_pwr_thres->AT_VOLS_LORA,state);
		}
		break;
#ifdef ONE_U
	case ORDER_VOLS_MTQA: // only 1U
		pwr_adc_mobc.ADC_V_MTQA = rawData;
		print(PWR_ADC_MOBC, "VOLS_MTQA, %d, %f\r\n", pwr_adc_mobc.ADC_V_MTQA, (float)pwr_adc_mobc.ADC_V_MTQA/4096.0f*3.3f*2.0f);
		state = PWR_State_GET(PWR_State_MTQA);
		if(g_pwr_awc==0){
			AD_VOLS_FET(AL_VOLS_MTQA_MOBC,rawData,g_pwr_thres->AT_VOLS_MTQ_MOBC,state);
		}
		break;
	case ORDER_CURS_MTQZ3Z4:  // only 1U
		pwr_adc_mobc.ADC_I_MTQZ3Z4 = rawData;
		state = PWR_State_GET(PWR_State_MTQA);
		if(g_pwr_awc==0){
			AD_CURS_MTQ(AL_CURS_MTQZ3Z4_MOBC,rawData,g_pwr_thres->AT_CURS_MTQ_MOBC[3],state);
		}
		print(PWR_ADC_MOBC, "ADC_CURS_MTQZ3Z4 : %d\r\n",rawData);
//		print(PWR_ADC_MOBC, "ADC_CURS_MTQZ3Z4 OC_THRES_POSITIVE : %d\r\n",g_pwr_thres->AT_CURS_MTQ_MOBC[3][0]);
//		print(PWR_ADC_MOBC, "ADC_CURS_MTQZ3Z4 OC_THRES_NEGATIVE : %d\r\n",g_pwr_thres->AT_CURS_MTQ_MOBC[3][1]);
		break;
	case ORDER_CURS_MTQX:  // only 1U
		pwr_adc_mobc.ADC_I_MTQX = rawData;
		state = PWR_State_GET(PWR_State_MTQA);
		if(g_pwr_awc==0){
			AD_CURS_MTQ(AL_CURS_MTQX_MOBC,rawData,g_pwr_thres->AT_CURS_MTQ_MOBC[0],state);
		}
		print(PWR_ADC_MOBC, "ADC_CURS_MTQX : %d\r\n",rawData);
//		print(PWR_ADC_MOBC, "ADC_CURS_MTQX OC_THRES_POSITIVE : %d\r\n",g_pwr_thres->AT_CURS_MTQ_MOBC[0][0]);
//		print(PWR_ADC_MOBC, "ADC_CURS_MTQX OC_THRES_NEGATIVE : %d\r\n",g_pwr_thres->AT_CURS_MTQ_MOBC[0][1]);
		break;
	case ORDER_CURS_MTQY:  // only 1U
		pwr_adc_mobc.ADC_I_MTQY = rawData;
		state = PWR_State_GET(PWR_State_MTQA);
		if(g_pwr_awc==0){
			AD_CURS_MTQ(AL_CURS_MTQY_MOBC,rawData,g_pwr_thres->AT_CURS_MTQ_MOBC[1],state);
		}
		print(PWR_ADC_MOBC, "ADC_CURS_MTQY : %d\r\n",rawData);
//		print(PWR_ADC_MOBC, "ADC_CURS_MTQY OC_THRES_POSITIVE : %d\r\n",g_pwr_thres->AT_CURS_MTQ_MOBC[1][0]);
//		print(PWR_ADC_MOBC, "ADC_CURS_MTQY OC_THRES_NEGATIVE : %d\r\n",g_pwr_thres->AT_CURS_MTQ_MOBC[1][1]);
		break;
	case ORDER_CURS_MTQZ1Z2:  // only 1U
		pwr_adc_mobc.ADC_I_MTQZ1Z2 = rawData;
		state = PWR_State_GET(PWR_State_MTQA);
		if(g_pwr_awc==0){
			AD_CURS_MTQ(AL_CURS_MTQZ1Z2_MOBC,rawData,g_pwr_thres->AT_CURS_MTQ_MOBC[2],state);
		}
		print(PWR_ADC_MOBC, "ADC_CURS_MTQZ1Z2 : %d\r\n",rawData);
//		print(PWR_ADC_MOBC, "ADC_CURS_MTQZ1Z1 OC_THRES_POSITIVE : %d\r\n",g_pwr_thres->AT_CURS_MTQ_MOBC[2][0]);
//		print(PWR_ADC_MOBC, "ADC_CURS_MTQZ1Z2 OC_THRES_NEGATIVE : %d\r\n",g_pwr_thres->AT_CURS_MTQ_MOBC[2][1]);

		break;
#endif
	}

    if (ret != 0)
    {
        printf("Read ADC_MOBC Failed! %d \r\n", ret); //printfの内容を変更する
        return CCP_make_cmd_ret_without_err_code((CCP_EXEC_STS)ret);
    }

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);

}

//uint16_t Get_ADC_Value(PWR_ADC_DMA_ORDER order)
uint16_t Get_ADC_Value(uint8_t order)
{
//	enumをprintするために用意
//	const char* DMA_order[]=
//	{
//		"CURS_MOBC",
//		"CURS_LORA",
//		"VOLS_GPS",
//		"VOLS_SUNS",
//		"VOLS_LORA",
//		"VOLS_MTQ",
//		"CURS_MTQZ3Z4",
//		"CURS_MTQX",
//		"CURS_MTQY",
//		"CURS_MTQZ1Z2",
//	};
//
//	PWR_ADC_DMA_ORDER buf;
//	buf = order;

	uint16_t ADC_Value = adc1_buf[order];

	return ADC_Value;
}

