/*
 * PWR_cmd_AnomalyHandler.c
 *
 *  Created on: 2021/12/03
 *      Author: SAT63
 *      revise by Noro
 */

#include "PWR_cmd_AnomalyHandle.h"
#include <src_user/Library/General/DBG_printf.h>
//#include <src_core/Applications/anomaly_handler.h>
#include <src_user/Applications/UserDefined/Com/COM_cmd_TXPIC.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Drivers/Power/PWR_Operation.h>
#include <src_user/Drivers/Cdh/SD/SD.h>
#include <src_user/Drivers/Cdh/SD/stm32_adafruit_sd.h>
#include "src_user/Library/Delay.h"
#include <src_user/Applications/DriverInstances/di_rxpic.h>  // for MOBCの遺言
#include <src_user/Drivers/Cdh/WDT/wdt_manager.h>


PWR_ANOMALY_STRUCT pwr_anomaly;

static void PWR_ANOMALY_Handle_param_init(void);
static void PWR_ANOMALY_Handle_param_exec(void);

// to do アノマリ対応回数，再起動回数の決定
// 当てはまらないコンポーネントもあるが(FETにOVは無し)，やりやすさのため全てに値を入れている Otsuki
const uint8_t pwr_anomaly_handle_count_oc_[MAX_CURRENT_SENSOR] = {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100};
const uint8_t pwr_anomaly_restart_count_oc_[MAX_CURRENT_SENSOR] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const uint8_t pwr_anomaly_handle_count_uc_[MAX_CURRENT_SENSOR] = {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100};
const uint8_t pwr_anomaly_handle_count_ov_[MAX_VOLTAGE_SENSOR] = {100,100,100,100,100,100,100,100,100,100,100,100,100,100};
const uint8_t pwr_anomaly_restart_count_ov_[MAX_VOLTAGE_SENSOR] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const uint8_t pwr_anomaly_handle_count_uv_[MAX_VOLTAGE_SENSOR] = {100,100,100,100,100,100,100,100,100,100,100,100,100,100};
const uint8_t pwr_anomaly_handle_count_psi_on_[MAX_VOLTAGE_SENSOR] = {100,100,100,100,100,100,100,100,100,100,100,100,100,100};
const uint8_t pwr_anomaly_handle_count_psi_off_[MAX_VOLTAGE_SENSOR] = {100,100,100,100,100,100,100,100,100,100,100,100,100,100};
const uint8_t pwr_anomaly_restart_count_psi_off_[MAX_VOLTAGE_SENSOR] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1};

const uint8_t pwr_anomaly_shut_down_limit_ = 10;

// アノマリカウント格納SDアドレス Otsuki
	// anomaly handle count -> ahc
	// anomaly restart count -> arc
const uint32_t sd_addr_ahc_oc = 0x00110700;
const uint32_t sd_addr_arc_oc = 0x0011070F;
const uint32_t sd_addr_ahc_uc = 0x0011071E;
const uint32_t sd_addr_ahc_ov = 0x0011072D;
const uint32_t sd_addr_arc_ov = 0x0011073B;
const uint32_t sd_addr_ahc_uv = 0x00110749;
const uint32_t sd_addr_ahc_psi_on = 0x00110757;
const uint32_t sd_addr_ahc_psi_off = 0x00110765;
const uint32_t sd_addr_arc_psi_off = 0x00110773;
const uint32_t sd_addr_shut_down_limit = 0x00110781;

AppInfo PWR_ANOMALY_Handle_update(void){
	return AI_create_app_info("pwr_anomaly_handle",PWR_ANOMALY_Handle_param_exec,PWR_ANOMALY_Handle_param_init);
}

// SDに書き込み（打ち上げ時の状態）
void PWR_ANOMALY_Handle_param_init(void) {

	printf("PWR Anomaly Handle initialize(write) param\r\n");

	for(int i=0;i<MAX_CURRENT_SENSOR;++i){
		SD_write_param(&pwr_anomaly.curs[i].oc.anomaly.count,&pwr_anomaly_handle_count_oc_[i], (size_t)(sizeof(pwr_anomaly_handle_count_oc_[i])),(uint32_t)sd_addr_ahc_oc+i);
		SD_write_param(&pwr_anomaly.curs[i].oc.restart.count,&pwr_anomaly_restart_count_oc_[i], (size_t)(sizeof(pwr_anomaly_restart_count_oc_[i])),(uint32_t)sd_addr_arc_oc+i);
		SD_write_param(&pwr_anomaly.curs[i].uc.count,&pwr_anomaly_handle_count_uc_[i], (size_t)(sizeof(pwr_anomaly_handle_count_uc_[i])),(uint32_t)sd_addr_ahc_uc+i);
	}
	for(int i=0;i<MAX_VOLTAGE_SENSOR;++i) {
		SD_write_param(&pwr_anomaly.vols[i].ov.anomaly.count,&pwr_anomaly_handle_count_ov_[i], (size_t)(sizeof(pwr_anomaly_handle_count_ov_[i])),(uint32_t)sd_addr_ahc_ov+i);
		SD_write_param(&pwr_anomaly.vols[i].ov.restart.count,&pwr_anomaly_restart_count_ov_[i], (size_t)(sizeof(pwr_anomaly_restart_count_ov_[i])),(uint32_t)sd_addr_arc_ov+i);
		SD_write_param(&pwr_anomaly.vols[i].uv.count,&pwr_anomaly_handle_count_uv_[i], (size_t)(sizeof(pwr_anomaly_handle_count_uv_[i])),(uint32_t)sd_addr_ahc_uv+i);
		SD_write_param(&pwr_anomaly.vols[i].psi_on.count,&pwr_anomaly_handle_count_psi_on_[i], (size_t)(sizeof(pwr_anomaly_handle_count_psi_on_[i])),(uint32_t)sd_addr_ahc_psi_on+i);
		SD_write_param(&pwr_anomaly.vols[i].psi_off.anomaly.count,&pwr_anomaly_handle_count_psi_off_[i], (size_t)(sizeof(pwr_anomaly_handle_count_psi_off_[i])),(uint32_t)sd_addr_ahc_psi_off+i);
		SD_write_param(&pwr_anomaly.vols[i].psi_off.restart.count,&pwr_anomaly_restart_count_psi_off_[i], (size_t)(sizeof(pwr_anomaly_restart_count_psi_off_[i])),(uint32_t)sd_addr_arc_psi_off+i);
		WDT_clear_count();
	}
	SD_write_param(&pwr_anomaly.shut_down_limit, &pwr_anomaly_shut_down_limit_, (size_t)(sizeof(pwr_anomaly_shut_down_limit_)), (uint32_t)sd_addr_shut_down_limit);
}
// SD読み込み
void PWR_ANOMALY_Handle_param_exec(void) {

	printf("PWR_ANOMALY_Handle load(read) param\r\n");

	for(int i=0;i<MAX_CURRENT_SENSOR;++i){
		SD_read_param(&pwr_anomaly.curs[i].oc.anomaly.count,&pwr_anomaly_handle_count_oc_[i], (size_t)(sizeof(pwr_anomaly_handle_count_oc_[i])),(uint32_t)sd_addr_ahc_oc+i);
		SD_read_param(&pwr_anomaly.curs[i].oc.restart.count,&pwr_anomaly_restart_count_oc_[i], (size_t)(sizeof(pwr_anomaly_restart_count_oc_[i])),(uint32_t)sd_addr_arc_oc+i);
		SD_read_param(&pwr_anomaly.curs[i].uc.count,&pwr_anomaly_handle_count_uc_[i], (size_t)(sizeof(pwr_anomaly_handle_count_uc_[i])),(uint32_t)sd_addr_ahc_uc+i);
	}
	for(int i=0;i<MAX_VOLTAGE_SENSOR;++i) {
		SD_read_param(&pwr_anomaly.vols[i].ov.anomaly.count,&pwr_anomaly_handle_count_ov_[i], (size_t)(sizeof(pwr_anomaly_handle_count_ov_[i])),(uint32_t)sd_addr_ahc_ov+i);
		SD_read_param(&pwr_anomaly.vols[i].ov.restart.count,&pwr_anomaly_restart_count_ov_[i], (size_t)(sizeof(pwr_anomaly_restart_count_ov_[i])),(uint32_t)sd_addr_arc_ov+i);
		SD_read_param(&pwr_anomaly.vols[i].uv.count,&pwr_anomaly_handle_count_uv_[i], (size_t)(sizeof(pwr_anomaly_handle_count_uv_[i])),(uint32_t)sd_addr_ahc_uv+i);
		SD_read_param(&pwr_anomaly.vols[i].psi_on.count,&pwr_anomaly_handle_count_psi_on_[i], (size_t)(sizeof(pwr_anomaly_handle_count_psi_on_[i])),(uint32_t)sd_addr_ahc_psi_on+i);
		SD_read_param(&pwr_anomaly.vols[i].psi_off.anomaly.count,&pwr_anomaly_handle_count_psi_off_[i], (size_t)(sizeof(pwr_anomaly_handle_count_psi_off_[i])),(uint32_t)sd_addr_ahc_psi_off+i);
		SD_read_param(&pwr_anomaly.vols[i].psi_off.restart.count,&pwr_anomaly_restart_count_psi_off_[i], (size_t)(sizeof(pwr_anomaly_restart_count_psi_off_[i])),(uint32_t)sd_addr_arc_psi_off+i);
	}
	SD_read_param(&pwr_anomaly.shut_down_limit, &pwr_anomaly_shut_down_limit_, (size_t)(sizeof(pwr_anomaly_shut_down_limit_)), (uint32_t)sd_addr_shut_down_limit);
}

/**
 * @brief 過電流時の操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
CCP_CmdRet Cmd_PWR_AH_OC(const CommonCmdPacket* packet)
{
	for (uint8_t i=0;i<MAX_CURRENT_SENSOR;++i){
		PWR_ANOMALY_DRIVER_STRUCT *handle = &pwr_anomaly.curs[i].oc.anomaly;
		PWR_RESTART_STRUCT *restart = &pwr_anomaly.curs[i].oc.restart;
		if (handle->flag == 1){ // 	アノマリ対応フラグが１なら対応する
			handle->count -= 1;   // 残アノマリ対応回数を1減らす
			SD_write_param(&handle->count,&handle->count, (size_t)(sizeof(handle->count)),(uint32_t)(sd_addr_ahc_oc+i));
			if (restart->count == 0 && pwr_anomaly.shut_down_limit > 0){
				if(i==AL_CURS_MOBC || i==AL_CURS_REG_CDH3V3){
					// MOBC自爆前にSDのシャットダウン上限を更新
					pwr_anomaly.shut_down_limit--;
					SD_write_param(&pwr_anomaly.shut_down_limit, &pwr_anomaly.shut_down_limit, (size_t)(sizeof(pwr_anomaly.shut_down_limit)), (uint32_t)sd_addr_shut_down_limit);
					printf("ANOMALY OC. shut_down_limit %u\r\n", pwr_anomaly.shut_down_limit);

					// MOBC自爆直前情報をPICに送信
					// MOBCが落ちたOBC時刻(4byte)＋アノマリグループ･ローカル(各1byte)＋PWRの残り対応回数(1byte)＋ModeID(1byte)+リセット回数(2byte) 計10byte　を遺言として送信
					rxpic_unexpected_shutdown_send_data_generator();

					c2a_delay_ms(0);
				}
				PWR_AH_CURS_OFF(i);
			}
			else if(restart->count > 0){
				restart->count -= 1; // 残再起動回数を1減らす
				// SDカードに書き込み
				SD_write_param(&restart->count,&restart->count, (size_t)(sizeof(restart->count)),(uint32_t)(sd_addr_arc_oc+i));
				PWR_AH_CURS_Restart(i);
				print(PWR_ANOMALY, "OC, CURS_ID : %d, Restart Count : %d\r\n",i,restart->count);
			}

			if(pwr_anomaly.shut_down_limit > 0){
				handle->flag =0;
				print(PWR_ANOMALY, "PWR Anomaly handle completed, Over Current, CURS_ID : %d\r\n",i);
				print(PWR_ANOMALY, "Anomaly Handle Count is now: %d\r\n",handle->count);
			}
			break;
		}
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief 不足電流時の操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
CCP_CmdRet Cmd_PWR_AH_UC(const CommonCmdPacket* packet)
{
	for (uint8_t i=0;i<MAX_CURRENT_SENSOR;++i){
		PWR_ANOMALY_DRIVER_STRUCT *handle=&pwr_anomaly.curs[i].uc;
		if (handle->flag == 1){
			handle->count -= 1;   // 残アノマリ対応回数を1減らす
			SD_write_param(&handle->count,&handle->count, (size_t)(sizeof(handle->count)),(uint32_t)(sd_addr_ahc_uc+i));
			PWR_AH_CURS_Restart(i);

			handle->flag =0;
			print(PWR_ANOMALY, "PWR Anomaly handle completed, Under Current, CURS_ID : %d\r\n",i);
			print(PWR_ANOMALY, "Anomaly Handle Count is now: %d\r\n",handle->count);
			break;
		}
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief 過電圧時の操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
CCP_CmdRet Cmd_PWR_AH_OV(const CommonCmdPacket* packet)
{
	for (uint8_t i=0;i<MAX_VOLTAGE_SENSOR;++i){
		PWR_ANOMALY_DRIVER_STRUCT *handle=&pwr_anomaly.vols[i].ov.anomaly;
		PWR_RESTART_STRUCT *restart=&pwr_anomaly.vols[i].ov.restart;
		if (handle->flag == 1){
			handle->count -= 1;   // 残アノマリ対応回数を1減らす
			SD_write_param(&handle->count,&handle->count, (size_t)(sizeof(handle->count)),(uint32_t)(sd_addr_ahc_ov+i));
			if (restart->count == 0 && pwr_anomaly.shut_down_limit > 0){
				if(i==AL_VOLS_REG_CDH3V3 || i==AL_VOLS_REG_ACS2_5V || i==AL_VOLS_REG_ACS1_5V){
					// MOBC自爆前にSDのシャットダウン上限を更新
					pwr_anomaly.shut_down_limit--;
					SD_write_param(&pwr_anomaly.shut_down_limit, &pwr_anomaly.shut_down_limit, (size_t)(sizeof(pwr_anomaly.shut_down_limit)), (uint32_t)sd_addr_shut_down_limit);
					printf("ANOMALY OV. shut_down_limit %u\r\n", pwr_anomaly.shut_down_limit);

					// MOBC自爆直前情報をPICに送信
					// MOBCが落ちたOBC時刻(4byte)＋アノマリグループ･ローカル(各1byte)＋PWRの残り対応回数(1byte)＋ModeID(1byte)+リセット回数(2byte) 計10byte　を遺言として送信
					rxpic_unexpected_shutdown_send_data_generator();

					c2a_delay_ms(0);
				}
				PWR_AH_VOLS_OFF(i);
			}
			else if(restart->count > 0){
				restart->count -= 1; // 残再起動回数を1減らす
				SD_write_param(&restart->count,&restart->count, (size_t)(sizeof(restart->count)),(uint32_t)(sd_addr_arc_ov+i));
				PWR_AH_VOLS_Restart(i);
				print(PWR_ANOMALY, "OV, VOLS_ID : %d, Restart Count : %d\r\n",i,restart->count);
			}

			if(pwr_anomaly.shut_down_limit > 0){
				handle->flag =0;
				print(PWR_ANOMALY, "PWR Anomaly handle completed, Over Voltage, VOLS_ID : %d\r\n",i);
				print(PWR_ANOMALY, "Anomaly Handle Count is now: %d\r\n",handle->count);
			}
			break;
		}
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief 不足電圧時の操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
CCP_CmdRet Cmd_PWR_AH_UV(const CommonCmdPacket* packet)
{

	for (uint8_t i=0;i<MAX_VOLTAGE_SENSOR;++i){
		PWR_ANOMALY_DRIVER_STRUCT *handle=&pwr_anomaly.vols[i].uv;
		if (handle->flag == 1){
			handle->count -= 1;   // 残アノマリ対応回数を1減らす
			SD_write_param(&handle->count,&handle->count, (size_t)(sizeof(handle->count)),(uint32_t)(sd_addr_ahc_uv+i));
			PWR_AH_VOLS_Restart(i);
			handle->flag =0;
			print(PWR_ANOMALY, "PWR Anomaly handle completed, Under Voltage, VOLS_ID : %d\r\n",i);
			print(PWR_ANOMALY, "Anomaly Handle Count is now: %d\r\n",handle->count);
			break;
		}
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}



/**
 * @brief 意図しない電源ON時の操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
CCP_CmdRet Cmd_PWR_AH_PSI_ON(const CommonCmdPacket* packet)
{
	for (uint8_t i=0;i<MAX_VOLTAGE_SENSOR;++i){
		PWR_ANOMALY_DRIVER_STRUCT *handle=&pwr_anomaly.vols[i].psi_on;
		if (handle->flag == 1 && pwr_anomaly.shut_down_limit > 0){
			SD_write_param(&handle->count,&handle->count, (size_t)(sizeof(handle->count)),(uint32_t)(sd_addr_ahc_psi_on+i));
			handle->count -= 1;   // 残アノマリ対応回数を1減らす
			if(i==AL_VOLS_REG_CDH3V3 || i==AL_VOLS_REG_ACS2_5V || i==AL_VOLS_REG_ACS1_5V){
				// MOBC自爆前にSDのシャットダウン上限を更新
				pwr_anomaly.shut_down_limit--;
				SD_write_param(&pwr_anomaly.shut_down_limit, &pwr_anomaly.shut_down_limit, (size_t)(sizeof(pwr_anomaly.shut_down_limit)), (uint32_t)sd_addr_shut_down_limit);
				printf("ANOMALY OC. shut_down_limit %u\r\n", pwr_anomaly.shut_down_limit);

				// MOBC自爆直前情報をPICに送信
				// MOBCが落ちたOBC時刻(4byte)＋アノマリグループ･ローカル(各1byte)＋PWRの残り対応回数(1byte)＋ModeID(1byte)+リセット回数(2byte) 計10byte　を遺言として送信
				rxpic_unexpected_shutdown_send_data_generator();

				c2a_delay_ms(0);
			}
			PWR_AH_VOLS_OFF(i);

			handle->flag =0;
			print(PWR_ANOMALY, "PWR Anomaly handle completed,  Power State Inconsistency_ON, VOLS_ID : %d\r\n",i);
			print(PWR_ANOMALY, "Anomaly Handle Count is now: %d\r\n",handle->count);
			break;
		}
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief 意図しない電源OFF時の操作コマンド 実行関数
 *
 * 初期化のいらない処理内容を記述．
 */
CCP_CmdRet Cmd_PWR_AH_PSI_OFF(const CommonCmdPacket* packet)
{

	for (uint8_t i=0;i<MAX_VOLTAGE_SENSOR;++i){
		PWR_ANOMALY_DRIVER_STRUCT *handle=&pwr_anomaly.vols[i].psi_off.anomaly;
		PWR_RESTART_STRUCT *restart=&pwr_anomaly.vols[i].psi_off.restart;
		if (handle->flag == 1){
			handle->count -= 1;   // 残アノマリ対応回数を1減らす
			SD_write_param(&handle->count,&handle->count, (size_t)(sizeof(handle->count)),(uint32_t)(sd_addr_ahc_psi_off+i));
			if (restart->count == 0 && pwr_anomaly.shut_down_limit > 0){
				if(i==AL_VOLS_REG_CDH3V3 || i==AL_VOLS_REG_ACS2_5V || i==AL_VOLS_REG_ACS1_5V){
					// MOBC自爆前にSDのシャットダウン上限を更新
					pwr_anomaly.shut_down_limit--;
					SD_write_param(&pwr_anomaly.shut_down_limit, &pwr_anomaly.shut_down_limit, (size_t)(sizeof(pwr_anomaly.shut_down_limit)), (uint32_t)sd_addr_shut_down_limit);
					printf("ANOMALY OC. shut_down_limit %u\r\n", pwr_anomaly.shut_down_limit);

					// MOBC自爆直前情報をPICに送信
					// MOBCが落ちたOBC時刻(4byte)＋アノマリグループ･ローカル(各1byte)＋PWRの残り対応回数(1byte)＋ModeID(1byte)+リセット回数(2byte) 計10byte　を遺言として送信
					rxpic_unexpected_shutdown_send_data_generator();

					c2a_delay_ms(0);
				}
				PWR_AH_VOLS_OFF(i);
			}
			else if(restart->count > 0){
				restart->count -= 1; // 残再起動回数を1減らす
				SD_write_param(&restart->count,&restart->count, (size_t)(sizeof(restart->count)),(uint32_t)(sd_addr_arc_psi_off+i));
				PWR_AH_VOLS_Restart(i);
				print(PWR_ANOMALY, "PSI_OFF, VOLS_ID : %d, Restart Count : %d\r\n",i,restart->count);
			}
			if(pwr_anomaly.shut_down_limit > 0){
				handle->flag =0;
				print(PWR_ANOMALY, "PWR Anomaly handle completed,  Power State Inconsistency_OFF, VOLS_ID : %d\r\n",i);
				print(PWR_ANOMALY, "Anomaly Handle Count is now: %d\r\n",handle->count);
			}
			break;
		}
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


// 電源OFF or 再起動
void PWR_AH_CURS_OFF(uint8_t curs){
	// 共通処理

	switch (curs){

	case AL_CURS_MOBC:
		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
#ifdef ONE_U
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
#ifdef TWO_U
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
		printf("AH MOBC OFF\r\n");
		PWR_Operate_ON_OFF(PWR_State_REG_CDH3V3,0);
		break;
	case AL_CURS_LORA:
		PWR_Operate_ON_OFF(PWR_State_LORA,0);
		break;
	case AL_CURS_REG_CDH3V3:
		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
#ifdef ONE_U
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
#ifdef TWO_U
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
		printf("AH MOBC OFF\r\n");
		PWR_Operate_ON_OFF(PWR_State_REG_CDH3V3,0);
		break;
#ifdef TWO_U
	case AL_CURS_SEPCOIL:
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		break;
	case AL_CURS_MTQ_ACS1:
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		break;
	case AL_CURS_MTQX_ACS1:
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
	    break;
	case AL_CURS_MTQY_ACS1:
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		break;
	case AL_CURS_MTQZ1_ACS1:
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		break;
	case AL_CURS_MTQZ2_ACS1:
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		break;
	case AL_CURS_MTQZ3_ACS1:
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		break;
	case AL_CURS_MTQZ4_ACS1:
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		break;
#endif
#ifdef ONE_U
	case AL_CURS_MTQX_MOBC:
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		break;
	case AL_CURS_MTQY_MOBC:
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		break;
	case AL_CURS_MTQZ1Z2_MOBC:
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		break;
	case AL_CURS_MTQZ3Z4_MOBC:
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		break;
#endif
	}
}

void PWR_AH_CURS_Restart(uint8_t curs){
	switch (curs){
	case AL_CURS_MOBC:

		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
#ifdef ONE_U
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
#ifdef TWO_U
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
		printf("AH MOBC Restart\r\n");
		PWR_AH_TXPIC_REG_CDH3V3_Restart();
		break;
	case AL_CURS_LORA:
		PWR_Operate_Restart(PWR_State_LORA);
//		PWR_AH_TXPIC_REG_CDH3V3_Restart(); // for test
		break;
	case AL_CURS_REG_CDH3V3:
		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
#ifdef ONE_U
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
#ifdef TWO_U
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
		printf("AH MOBC Restart\r\n");
		PWR_AH_TXPIC_REG_CDH3V3_Restart();
		break;
#ifdef TWO_U
	case AL_CURS_SEPCOIL:
		PWR_Operate_Restart(PWR_State_SEPCOIL1);
		PWR_Operate_Restart(PWR_State_SEPCOIL2);
		PWR_Operate_Restart(PWR_State_SEPCOIL3);
		PWR_Operate_Restart(PWR_State_SEPCOIL4);
		break;
	case AL_CURS_MTQ_ACS1:
		// MTQO OFF -> MTQA OFF -> MTQA ONInit
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		PWR_Operate_ONInit(PWR_State_MTQA);
		break;
	case AL_CURS_MTQX_ACS1:
		// MTQO OFF -> MTQA OFF -> MTQA ONInit
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		PWR_Operate_ONInit(PWR_State_MTQA);
	    break;
	case AL_CURS_MTQY_ACS1:
		// MTQO OFF -> MTQA OFF -> MTQA ONInit
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		PWR_Operate_ONInit(PWR_State_MTQA);
		break;
	case AL_CURS_MTQZ1_ACS1:
		// MTQO OFF -> MTQA OFF -> MTQA ONInit
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		PWR_Operate_ONInit(PWR_State_MTQA);
		break;
	case AL_CURS_MTQZ2_ACS1:
		PWR_Operate_Restart(PWR_State_MTQO_PLASMA);
		break;
	case AL_CURS_MTQZ3_ACS1:
		PWR_Operate_Restart(PWR_State_MTQO_PLASMA);
		break;
	case AL_CURS_MTQZ4_ACS1:
		PWR_Operate_Restart(PWR_State_MTQO_PLASMA);
		break;
#endif
#ifdef ONE_U
	case AL_CURS_MTQX_MOBC:
		print(PWR_ANOMALY, "Anomaly Handle: MTQA Restart\r\n");
		PWR_Operate_Restart(PWR_State_MTQA);
		break;
	case AL_CURS_MTQY_MOBC:
		print(PWR_ANOMALY, "Anomaly Handle: MTQA Restart\r\n");
		PWR_Operate_Restart(PWR_State_MTQA);
		break;
	case AL_CURS_MTQZ1Z2_MOBC:
		print(PWR_ANOMALY, "Anomaly Handle: MTQA Restart\r\n");
		PWR_Operate_Restart(PWR_State_MTQA);
		break;
	case AL_CURS_MTQZ3Z4_MOBC:
		print(PWR_ANOMALY, "Anomaly Handle: MTQA Restart\r\n");
		PWR_Operate_Restart(PWR_State_MTQA);
		break;
#endif
	}
}


// 電源ON/OFF
void PWR_AH_VOLS_OFF(uint8_t vols){
	switch (vols){
	case AL_VOLS_GPS:
		PWR_Operate_ON_OFF(PWR_State_GPS,0);
		break;
	case AL_VOLS_SUNS:
		PWR_Operate_ON_OFF(PWR_State_SUNS,0);
		break;
	case AL_VOLS_LORA:
		PWR_Operate_ON_OFF(PWR_State_LORA,0);
		break;
#ifdef ONE_U
	case AL_VOLS_MTQA_MOBC:
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		break;
#endif
	case AL_VOLS_REG_CDH3V3:
		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
#ifdef ONE_U
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
#ifdef TWO_U
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
		printf("AH MOBC OFF\r\n");
		PWR_Operate_ON_OFF(PWR_State_REG_CDH3V3,0);
		break;
	case AL_VOLS_REG_ACS2_5V:
		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
#ifdef ONE_U
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
#ifdef TWO_U
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
		printf("AH MOBC OFF\r\n");
		PWR_Operate_ON_OFF(PWR_State_REG_CDH3V3,0);
		break;
#ifdef TWO_U
		case AL_VOLS_REG_ACS1_5V:
		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		printf("AH MOBC OFF\r\n");
		PWR_Operate_ON_OFF(PWR_State_REG_CDH3V3,0);
		break;
	case AL_VOLS_MTQA_ACS1:
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		break;
	case AL_VOLS_MTQO_ACS1:
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		break;
	case AL_VOLS_MIS1:
		PWR_Operate_ON_OFF(PWR_State_MIS1,0);
		break;
	case AL_VOLS_SEPCOIL1:
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		break;
	case AL_VOLS_SEPCOIL2:
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		break;
	case AL_VOLS_SEPCOIL3:
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		break;
	case AL_VOLS_SEPCOIL4:
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		break;
#endif
	}
}

void PWR_AH_VOLS_Restart(uint8_t vols){
	switch (vols){
	case AL_VOLS_GPS:
		PWR_Operate_Restart(PWR_State_GPS);
		break;
	case AL_VOLS_SUNS:
		PWR_Operate_Restart(PWR_State_SUNS);
		break;
	case AL_VOLS_LORA:
		PWR_Operate_Restart(PWR_State_LORA);
		break;
#ifdef ONE_U
	case AL_VOLS_MTQA_MOBC:
		PWR_Operate_Restart(PWR_State_MTQA);
		break;
#endif
	case AL_VOLS_REG_CDH3V3:
		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
#ifdef ONE_U
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
#ifdef TWO_U
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
		printf("AH MOBC Restart\r\n");
		PWR_AH_TXPIC_REG_CDH3V3_Restart();
		break;
	case AL_VOLS_REG_ACS2_5V:
		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
#ifdef ONE_U
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
#ifdef TWO_U
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
#endif
		printf("AH MOBC Restart\r\n");
		PWR_AH_TXPIC_REG_CDH3V3_Restart();
		break;
#ifdef TWO_U
	case AL_VOLS_REG_ACS1_5V:
		// MOBC OFFするときREGCDH3V3配下以外のコンポを先にOFF
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL1,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL2,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL3,0);
		PWR_Operate_ON_OFF(PWR_State_SEPCOIL4,0);
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		printf("AH MOBC Restart\r\n");
		PWR_AH_TXPIC_REG_CDH3V3_Restart();
		break;
	case AL_VOLS_MTQA_ACS1:
		// MTQO OFF -> MTQA OFF -> MTQA ON -> MTQO ON
		PWR_Operate_ON_OFF(PWR_State_MTQO_PLASMA,0);
		PWR_Operate_ON_OFF(PWR_State_MTQA,0);
		PWR_Operate_ONInit(PWR_State_MTQA);
		break;
	case AL_VOLS_MTQO_ACS1:
		PWR_Operate_Restart(PWR_State_MTQO_PLASMA);
		break;
	case AL_VOLS_MIS1:
		PWR_Operate_Restart(PWR_State_MIS1);
		break;
	case AL_VOLS_SEPCOIL1:
		PWR_Operate_Restart(PWR_State_SEPCOIL1);
		break;
	case AL_VOLS_SEPCOIL2:
		PWR_Operate_Restart(PWR_State_SEPCOIL2);
		break;
	case AL_VOLS_SEPCOIL3:
		PWR_Operate_Restart(PWR_State_SEPCOIL3);
		break;
	case AL_VOLS_SEPCOIL4:
		PWR_Operate_Restart(PWR_State_SEPCOIL4);
		break;
#endif
	}
}

void PWR_AH_TXPIC_REG_CDH3V3_Restart(void){
	TCP tcp;
#ifdef TWO_U
	CCP_form_rtc_to_other_obc(&tcp, TCP_APID_TXPIC_2U, (CMD_CODE)0xC22b,0x00, 0x00);
#endif
#ifdef ONE_U
	CCP_form_rtc_to_other_obc(&tcp, TCP_APID_TXPIC_1U, (CMD_CODE)0xC22b,0x00, 0x00);
#endif

	TCP_set_apid_(&tcp, MY_SAT_ID, TCP_EXE_ID_TXPIC);//実行先だけ変更する

	TXPIC_Send(txpic_driver, tcp.packet, 17); // 引数なしだったら 17
}


//全電源系統のＡＨをＥＮＡＢＬＥにするコマンド 野呂
CCP_CmdRet Cmd_PWR_AH_SYSTEM_ENABLE(const CommonCmdPacket* packet){
//	for(uint8_t i=AH_RULE_OC_CURS_MOBC;i<=AH_RULE_PSI_OFF_VOLS_SEPCOIL4;++i) AH_activate_rule(i);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

