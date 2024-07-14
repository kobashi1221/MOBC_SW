/**
  *****************************************************************************************
  * @file    : CDH1_debug_MRAM.cpp
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/09/30
  * @brief   : MRAM Application for CDH1 Board Debug
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  * Memory Size 0x000000 - 0x07FFFF (524,288 Byte = 512KB)
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/09/30  1.0   N.KISHI         First revision
  *
  *****************************************************************************************
  */

#include "MEM_cmd_MRAM.h"

#include <src_user/IfWrapper/HAL.h>
#include <stdio.h> //for printf
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Library/Delay.h>
#include <src_user/Applications/DriverInstances/command_response_handler.h>
#include <src_core/Library/endian.h>
#include <src_core/System/TimeManager/time_manager.h>
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include "../GlobalMemoryParam.h"
#include <src_user/Library/General/DBG_printf.h>

//メモリーパトロールのアドレス
static uint32_t MRAMPatrolAddr = 0;
static uint8_t MRAM_ReadSize;
const uint8_t MRAM_ReadSize_ = 10;

static void mram_load_param(void);
static void mram_initialize_param(void);
static void mram_memory_patrol(void);

AppInfo MRAM_param_update(void)
{
    return AI_create_app_info("sample_param_init", mram_load_param, mram_initialize_param);
}

AppInfo MRAM_memory_patorol(void)
{
    return AI_create_app_info("sample_param_init", NULL, mram_memory_patrol);
}


static void mram_load_param(void){
	read_param(&MRAM_ReadSize, &MRAM_ReadSize_, 1, 0x00003800);
}

static void mram_initialize_param(void){
	write_param(&MRAM_ReadSize, &MRAM_ReadSize_, 1, 0x00003800);	//write_load
}

static void mram_memory_patrol(void){

	if(MRAM_ReadSize == 0) return;

	//ダミーバッファを用意
	uint8_t dummy[MRAM_ReadSize];
	//パトロールするアドレス範囲を算出
	uint32_t _eaddr = MRAMPatrolAddr + MRAM_ReadSize;

	//最大使用領域を超える場合
	if(_eaddr >= 0xAAAA){
		MRAM_read_param(dummy,NULL, MRAM_ReadSize - (_eaddr - 0xAAA9), MRAMPatrolAddr);

		MRAM_read_param(dummy,NULL, (_eaddr - 0xAAA9), 0x00000000);
		MRAMPatrolAddr = (_eaddr - 0xAAA9);
	}
	//最大使用領域を超えない場合
	else{
		MRAM_read_param(dummy,NULL, MRAM_ReadSize, MRAMPatrolAddr);
		MRAMPatrolAddr += MRAM_ReadSize;
		if(MRAMPatrolAddr >= 0xAAAA) MRAMPatrolAddr = 0;
	}
}

/**
 * @brief	write command from MRAM.
 * packet[15]:      mem-dump tlm D\L flag
 * packet[16~19]: 	write address
 * packet[20~]: 	data to write
 *
 */
CCP_CmdRet Cmd_MEM_mram_writeData(const CommonCmdPacket* packet)
{
	size_t param_len;
	print(MRAM_EEPROM, "mram write command\r\n");

    if ((param_len = CCP_get_param_len(packet)) < 6)
    {
        // パラメータはブロック番号1Byte。
        return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
    }
    uint8_t DataSize = param_len - 5;
    uint8_t writedata[DataSize];
    uint8_t* param_head = (uint8_t*)CCP_get_param_head(packet);
    uint8_t tlm_dl_flag = 0;
    uint32_t Address = 0;

    tlm_dl_flag = param_head[0];
    ENDIAN_memcpy(&Address, &param_head[1], 4);
    memcpy(writedata, &param_head[5], DataSize);

	write_param(writedata, NULL, DataSize, Address);

	print(MRAM_EEPROM, "writeaddress: %08x", Address);
	print(MRAM_EEPROM, "writedata: ");
	for(int i = 0; i<DataSize; ++i){
        print(MRAM_EEPROM, "%02x ",writedata[i]);
    }
	print(MRAM_EEPROM,"\r\n");

	uint8_t eeprom_flag, mram_flag;
	if(get_mode_flag(p_memory_mode, FLAG_PARAMROM_WRITE_EEPROM))	eeprom_flag = 1;
	if(get_mode_flag(p_memory_mode, FLAG_PARAMROM_WRITE_MRAM))	mram_flag = 1;

	if(eeprom_flag){
		APP_TMD_EEPROM3_SET_BEGIN_ADR(Address);
		TMD_EEPROM3_DUMP();
		if(tlm_dl_flag){
			uint8_t param[6];
			TF_copy_u16(&param[0], (uint16_t)(p_gs_response_struct->res_apid));
			TF_copy_u8(&param[2], (uint8_t)(p_gs_response_struct->res_route));
			TF_copy_u8(&param[3], 0x40);
			TF_copy_u8(&param[4], (uint8_t)Tlm_CODE_TLM_MEMDUMP_EEPROM3);
			TF_copy_u8(&param[5], (uint8_t)(1));
			CCP_form_rtc(&p_gs_response_struct->tcp,(CMD_CODE)Cmd_CODE_GENERATE_TLM_USER, &param[0], 6);
			TCP_set_apid_(&p_gs_response_struct->tcp, MY_SAT_ID, TCP_EXE_ID_MOBC);
			PH_analyze_packet(&p_gs_response_struct->tcp);
		}
	}
	if(mram_flag){
		APP_TMD_MRAM3_SET_BEGIN_ADR(Address);
		TMD_MRAM3_DUMP();
		if(tlm_dl_flag){
			uint8_t param[6];
			TF_copy_u16(&param[0], (uint16_t)(p_gs_response_struct->res_apid));
			TF_copy_u8(&param[2], (uint8_t)(p_gs_response_struct->res_route));
			TF_copy_u8(&param[3], 0x40);
			TF_copy_u8(&param[4], (uint8_t)Tlm_CODE_TLM_MEMDUMP_MRAM3);
			TF_copy_u8(&param[5], (uint8_t)(1));
			CCP_form_rtc(&p_gs_response_struct->tcp,(CMD_CODE)Cmd_CODE_GENERATE_TLM_USER, &param[0], 6);
			TCP_set_apid_(&p_gs_response_struct->tcp, MY_SAT_ID, TCP_EXE_ID_MOBC);
			PH_analyze_packet(&p_gs_response_struct->tcp);
		}
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*
 * @brief	read command from MRAM. 0x01 0x0c 0x00 0xaa 0xaa 0x0a
 * param[0]~param[3]:	readaddress (BIG Endian)
 * param[4]: 			datasize
 *
 */
CCP_CmdRet Cmd_MEM_mram_ReadData(const CommonCmdPacket* packet)
{
	int ret;

	size_t param_len;

	print(MRAM_EEPROM,"mram read command\r\n");
	 if ((param_len = CCP_get_param_len(packet)) != 5) //その場しのぎだがpacketlengthを足した
	{
		// パラメータはブロック番号1Byte。
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
	}

	 uint8_t DataSize = CCP_get_param_head(packet)[4];
	 uint8_t readbuf[DataSize];
	 uint32_t Address;
	 ENDIAN_memcpy(&Address, CCP_get_param_head(packet), 4);
	 read_param(readbuf, NULL, DataSize, Address);
	 print(MRAM_EEPROM, "ReadData: ");
	 for(int i=0; i<DataSize; ++i){
		print(MRAM_EEPROM, "%02x ",readbuf[i]);
	 }
	 print(MRAM_EEPROM, "\r\n");

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}



/*	@brief: パトロールアドレスの変更コマンド
 * 	packet[15~18]: アドレス（ビッグエンディアン）
 */
CCP_CmdRet Cmd_MEM_mram_set_patrol_addr(const CommonCmdPacket* packet){
	uint32_t temp;
	//パラメータを受信
	ENDIAN_memcpy(&temp, CCP_get_param_head(packet), 4);
	//不正なアドレスを除去
	if(temp > 0xAAAA){
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	//正常なアドレスであれば変更を適用する
	MRAMPatrolAddr = temp;

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

