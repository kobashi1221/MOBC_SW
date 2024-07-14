
/*
 * CDH_cmd_AllRepro.c
 *
 *  Created on: 2021/11/16
 *      Author: kahaku
 */
#include "CDH_cmd_AllRepro.h"

#include <src_user/IfWrapper/HAL.h>

#include <src_user/Drivers/CDH/Flash/flash.h>
#include <src_user/Drivers/CDH/SD/SD.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Library/General/DBG_printf.h>

#define PROGRAM_SIZE (0x780)
#define COUNT_SIZE (2)

static void allrepro_load_param(void);
static void allrepro_initialize_param(void);

static uint8_t* p_flash = 0x8100000;
const uint32_t p_flash_new = 0x8100000;

static uint32_t ProgramAreaStartBlockAddress = 0x01881F00;			//リプロ用領域の先頭ブロックアドレス
static ALLREPRO_STRUCT allrepro;

static uint8_t data_size;
const  uint8_t data_size_ = 237;


static void CDH_AllRepro_init(void);

extern uint32_t _sflash_new;

AppInfo allrepro_param_update(void)
{
  return AI_create_app_info("all_repro", allrepro_load_param, allrepro_initialize_param);
}

static void allrepro_load_param(void){
	read_param(&data_size, &data_size_, 1, (uint32_t)0x00001000);
}

static void allrepro_initialize_param(void){
	write_param(&data_size, &data_size_, 1, (uint32_t)0x00001000);
}

AppInfo CDH_AllRepro_update(void)
{
    return AI_create_app_info("AllRepro", CDH_AllRepro_init, NULL);
}

static void CDH_AllRepro_init(void){
	if(pic_data->Program == 0xda){
//		printf("Set Flight Program\r\n");
		allrepro.p_flash = 0x8010000;
	}else{
//		printf("Set New Program\r\n");
		allrepro.p_flash = 0x8100000;
	}
}


/*	brief: packetを直接Flashに書き込む(デバッグ用コマンド)
 *
 */
CCP_CmdRet Cmd_CDH_debug_Write_Program_on_FLASH(const CommonCmdPacket* packet){
	uint8_t param_len = CCP_get_param_len(packet);
	uint8_t* param_head = (uint8_t*)CCP_get_param_head(packet);
	uint16_t seq_cnt = TCP_get_seq_cnt(packet);
	printf("Repro: %d\r\n", seq_cnt);
	int ret = 0;
	uint8_t timeout;
	HAL_FLASH_Unlock_C2A();
	for(uint8_t i = 0; i < param_len; i++ ){
		timeout = 3;
		printf("%d ",i);
		do{
			ret = HAL_FLASH_Program_C2A(FLASH_TYPEPROGRAM_BYTE, (uint32_t)p_flash, (uint64_t)param_head[i]);
			HAL_IWDG_Refresh_C2A(&hiwdg);
		}while(ret != 0 || timeout-- != 0);
		p_flash++;
		if(timeout == 0){
			printf("add anomaly repro\r\n");
		}
	}
	HAL_FLASH_Lock_C2A();
	printf("Repro packet finish\r\n");
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


typedef void(*pFunction)(void);	//関数ポインタの定義
pFunction JumpToApplication;

void JumpToApp(uint32_t* addr)
{
    uint32_t JumpAddress = *(__IO uint32_t*)(addr + 1);	//(ポインタキャスト)アドレス
	pFunction test = JumpAddress;
    JumpToApplication = (pFunction)JumpAddress;					//関数ポインタの設定

    printf("JumpAddress: %08x\r\n",JumpAddress);
    printf("jump: %08x\r\n", (uint32_t)JumpToApplication);
    __disable_irq();
    __set_MSP(*(__IO uint32_t*)addr);						//スタックポインタの変更
    JumpToApplication();
}

CCP_CmdRet Cmd_CDH_AllRepro_JUMP_NEW(const CommonCmdPacket* packet){
	printf("change new program\r\n");
	JumpToApp(&_sflash_new);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_CDH_AllRepro_Program_to_SD(const CommonCmdPacket* packet){
	//受信データの抽出
	uint16_t param_len = CCP_get_param_len(packet);
	uint8_t* param_head = (uint8_t*)CCP_get_param_head(packet);
	uint8_t res = 0;

	//書き込み先のブロックアドレスを算出
	//ProgramNum: リプロ領域内でのプログラム番号
	//PROFRAM_SIZE: 一つのプログラムの最大サイズ(0xF0000: 約1MBytes)
	uint32_t block_addr = ProgramAreaStartBlockAddress + PROGRAM_SIZE*(uint32_t)allrepro.pos;

	uint16_t seq_cnt;
	ENDIAN_memcpy(&seq_cnt, param_head, 2);

    print(REPRO, "block addr: %08x\r\n", block_addr);
    print(REPRO, "Address: %08x\r\n", (uint32_t)(seq_cnt * data_size));
    print(REPRO, "seq_cnt: %04x\r\n", seq_cnt);

	//SDへ書き込み
	res = sd_write_address(block_addr, (uint32_t)(seq_cnt * data_size), (uint8_t*)(param_head + COUNT_SIZE), param_len - COUNT_SIZE);
	if(res != 0){
		printf("AllRepro SD Error\r\n");
	}

    if(res == 0){
      //receive_flag書き込み
        uint8_t receive_frag = 0xff;
        uint8_t res_flag = SD_write_param(&receive_frag, NULL, 1, (uint32_t)seq_cnt);
        if(res_flag != 0){
            printf("WRITE FLAG ERROR\r\n");
        }
    }

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*
 * @brief reproを始める前の初期化, ProgramNumの決定
 *        プログラムサイズと受信フラグを0にする
 * @param ProgramNum
 */
CCP_CmdRet Cmd_CDH_AllRepro_set_number(const CommonCmdPacket* packet){

	uint8_t* param = CCP_get_param_head(packet);

	allrepro.pos = param[0];
	allrepro.sd_write_addr = 0;
	allrepro.p_flash = 0x8100000;

	printf("program number %d\r\n", allrepro.pos);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_CDH_AllRepro_reset_flag(const CommonCmdPacket* packet){

	uint8_t res = 0;
	uint8_t reset_data[512] = {0};
	for(int i = 0; i < 9; i++){
		res = SD_write_param(reset_data, NULL, SD_BLOCKSIZE, 0x00000000 + SD_BLOCKSIZE * i);
		if(res != 0){
			printf("RECEIVE FLAG INIT FAILED\r\n");
		}
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*
 * @brief 受信フラグをGSに送る
 */
CCP_CmdRet Cmd_CDH_ALLREPRO_SEND_ReceiveFlag(const CommonCmdPacket* packet){
	uint8_t res = 0;

	uint8_t* p_data = 0x60000000;
	uint8_t* param = CCP_get_param_head(packet);
	uint32_t flag_num;

	ENDIAN_memcpy(&flag_num, param, 4);

	for(int i = 0; i < 9; i++){
		res = SD_read_param(p_data + SD_BLOCKSIZE * i, NULL, SD_BLOCKSIZE, 0x00000000 + SD_BLOCKSIZE * i);
		if(res != 0){
			printf("RECEIVE FLAG READ FAILED\r\n");
		}
	}

	large_packet_TLM_analyze(p_data, flag_num, Tlm_CODE_REPRO_FLAG);

	printf("flag is\r\n");
	for(int i = 0; i < flag_num; i++) printf("%02x ", p_data[i]);
	printf("\r\n");

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
