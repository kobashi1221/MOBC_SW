/**
  *****************************************************************************************
  * @file    : CDH1_debug_SRAM.c
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/09/29
  * @brief   : SRAM IS61WV5128BLL
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/09/29  1.0   N.KISHI         First revision
  *
  *****************************************************************************************
  */

#include "MEM_cmd_SRAM.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <string.h> //for memset

#include <src_core/Library/endian.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_core/System/TimeManager/time_manager.h>
#include <src_user/Applications/UserDefined/tlm_mem_dump.h>
#include <src_core/Library/endian.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/DriverInstances/command_response_handler.h>

#define SRAM_ADDRESS  0x60000000UL
#define SRAM_END_ADDRESS 0x60080000
#define SRAM_SIZE 0x80000UL // 512KB(= 4Mb)
#define SRAM_HALF_SIZE SRAM_SIZE/2


static void CDH1_debug_sram_init(void);
static void CDH1_debug_sram_exe(void);
//static void CDH1_SEE_sram_init(void);
//static void CDH1_SEE_sram_exe(void);

/**
 * @brief
 *
 * @note 
 * @return
 */
AppInfo CDH1_debug_sram_update(void)
{
    return AI_create_app_info("DBG_SRAM", CDH1_debug_sram_init, CDH1_debug_sram_exe);
}

//AppInfo CDH1_SEE_SRAM_UPDATE(VOID)
//{
//    RETURN CREATE_APP_INFO("SEE_SRAM", CDH1_SEE_SRAM_INIT, CDH1_SEE_SRAM_EXE);
//}

/**
 *
 *
 *
 */
static void CDH1_debug_sram_init(void)
{
    int ret = 0;

//    printf("CDH1 SRAM Debug App initialize.\n");




    /* ----------*/
//	uint32_t *adr = (uint32_t*) SRAM_ADDRESS;
//	for (uint32_t i = 0; i < (uint32_t)SRAM_SIZE / 4; i++){
//		memset(adr + i, i, 1);
//	}
    /* ----------*/

    //set bytes in sram address
	// 0x00000000 - 6003ffff : 0x00
	// 0x60040000 - 6007ffff : 0xff

//    step_t before = TMGR_get_master_step();
	uint8_t *adr = (uint8_t*) SRAM_ADDRESS;

	memset(adr, 0x00, SRAM_SIZE);

	adr += SRAM_SIZE - 5;
//	uint8_t a = 0x00;
//	for(int i = 0; i < 10; i++){
//		memcpy(adr, &a, 1);
//		a++;
//		adr++;
//	}

//	adr = (uint8_t*) SRAM_ADDRESS;
//	for(int i = 0; i < 10; i++){
//		printf("%02x ", *adr);
//		adr++;
//	}
//	memset(adr, 0x00, SRAM_HALF_SIZE);
	memset(adr + SRAM_HALF_SIZE, 0xff, SRAM_HALF_SIZE);
//	uint8_t a = 0x00;
//	for(int i = 0; i < SRAM_SIZE; i++){
//		memcpy(adr, &a, 1);
//		a++;
//		adr++;
//		HAL_IWDG_Refresh_C2A(&hiwdg);
//	}

//	step_t after = TMGR_get_master_step();
//	printf("sram init time = %d\r\n", after - before);
//	adr = (uint8_t*) SRAM_ADDRESS;

	//for debug
//	uint8_t *adr = (uint8_t*) SRAM_ADDRESS;
//	for (int i = 0; i < 10; i++){
//		memset(adr + i, (uint8_t)i, 1);
//	}

//	for(int i = 0; i < SRAM_SIZE + 1; i++){
//		printf("%02x ", *adr);
//		adr++;
//		if((i % 500) == 0) {
//			HAL_IWDG_Refresh_C2A(&hiwdg);
//		}
//	}
//	printf("\r\n");

}


/**
 * @brief
 *
 *
 *
 *
 */
static void CDH1_debug_sram_exe(void)
{
    printf("sram debug execution\r\n");

    char msg[64];
    uint32_t limit = SRAM_HALF_SIZE / 4;

    /* --------------------Check Read Data and Written Data-------------------- */
    uint16_t numError = 0x00;


    //data: 0000000000
    for (int i = 0; i < 50; i++){
    	uint8_t* adr = (uint8_t*) SRAM_ADDRESS + i;
    	if(*adr != (uint8_t)0x00) numError++;
    	printf("%d ", *adr); //display data
    }
    printf("\r\n");


    //data: 255255255255255255255255255255
    for (int i = 0; i < 50; i++){
        uint8_t* adr = (uint8_t*) (SRAM_ADDRESS + SRAM_HALF_SIZE) + i;
        if(*adr != (uint8_t)0xff) numError++;
        printf("%d ", *adr); //display data
    }
    printf("\r\n");


    if(numError == 0) {
		printf("SRAM SUCCESS\r\n");
    } else {
        printf("num error is %d \r\n", numError); //display data
    	printf("SRAM SAVE FAILED\r\n");
    }


    /* ----- Wanted to do this START-----*/
//    for (int i = 0; i < 10; i++){
//        uint32_t* adr = (uint32_t*) SRAM_ADDRESS + i;
//        printf("%lu ", *adr);
//    }
//    printf("\r\n");
//
//    for (int i = 0; i < 10; i++){
//        uint32_t* adr = (uint32_t*) (SRAM_ADDRESS + SRAM_HALF_SIZE) + i;
//        printf("%lu ", *adr);
//    }
//    printf("\r\n");
    /* ----- Wanted to do this END -----*/
}

/* ---------- For SEE Test ---------- */

//static void CDH1_SEE_sram_init(void)
//{
//    int ret = 0;
//
//    printf("CDH1 SRAM SEE App initialize.\n");
//
//    p_cdh1_debug_sram = &cdh1_debug_sram;
//
//	// 0x00000000 - 6003ffff : 0x00
//	// 0x60040000 - 6007ffff : 0xFF
//	uint8_t *adr = (uint8_t*) SRAM_ADDRESS;
//	memset(adr, 0x00, SRAM_HALF_SIZE);
//	memset(adr + SRAM_HALF_SIZE, 0xff, SRAM_HALF_SIZE);
//
//}
//
//static void CDH1_SEE_sram_exe(void)
//{
////    printf("sram SEE execution\r\n");
//
//    char msg[64];
//    uint32_t errorBitCount = 0;
//    static uint32_t TotalErrorBitCount = 0;
//    uint32_t limit = SRAM_HALF_SIZE / 4;
//
//
//    for (uint32_t i = 0; i < limit; i++) {
//    	uint32_t* adr = (uint32_t*) SRAM_ADDRESS + i;
//    	errorBitCount += popCount(*adr);
//    }
//
//    for (uint32_t i = 0; i < limit; i++) {
//    	uint32_t *adr = (uint32_t*) (SRAM_ADDRESS + SRAM_HALF_SIZE) + i;
//    	errorBitCount += (32 - popCount(*adr));
//    }
//
//    TotalErrorBitCount += errorBitCount;
//
//    printf("SRAM BIT ERROR = %lu\r\n", errorBitCount);
//    printf("SRAM TOTAL BIT ERROR = %lu\r\n", TotalErrorBitCount);
//
//    printf("%lu %lu\r\n", *(uint32_t *)(SRAM_ADDRESS), *(uint32_t *)(SRAM_ADDRESS+1));
//    printf("%lu %lu\r\n", *(uint32_t *)(SRAM_ADDRESS+SRAM_HALF_SIZE), *(uint32_t *)(SRAM_ADDRESS+SRAM_HALF_SIZE+1));
//
//}

/**
 * @brief  write data for SRAM
 * packet[15]:      mem-dump tlm D\L flag
 * packet[16~19]: 	write address
 * packet[20~]: 	data to write
 */
CCP_CmdRet Cmd_MEM_SRAM_write_data(const CommonCmdPacket* packet)
{
	size_t param_len;
	uint32_t ram_adr;

	if((param_len = CCP_get_param_len(packet)) < 6)
	{
		printf("illegal length \r\n");
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
	}

	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t data_len = param_len - 5;

	uint8_t tlm_dl_flag = param[0];
	ENDIAN_memcpy(&ram_adr, &param[1], 4);

	if(ram_adr < SRAM_ADDRESS || ram_adr >=SRAM_END_ADDRESS){
		printf("illegal addr \r\n");
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

	memcpy((uint8_t*)ram_adr, param + 5, data_len);

	APP_TMD_RAM_SET_BEGIN_ADR(ram_adr);
	TMD_RAM_DUMP();

	if(tlm_dl_flag){
		uint8_t param[6];
		TF_copy_u16(&param[0], (uint16_t)(p_gs_response_struct->res_apid));
		TF_copy_u8(&param[2], (uint8_t)(p_gs_response_struct->res_route));
		TF_copy_u8(&param[3], 0x40);
		TF_copy_u8(&param[4], (uint8_t)Tlm_CODE_TLM_MEMDUMP_RAM);
		TF_copy_u8(&param[5], (uint8_t)(1));
		CCP_form_rtc(&p_gs_response_struct->tcp,(CMD_CODE)Cmd_CODE_GENERATE_TLM_USER, &param[0], 6);
		TCP_set_apid_(&p_gs_response_struct->tcp, MY_SAT_ID, TCP_EXE_ID_MOBC);
		PH_analyze_packet(&p_gs_response_struct->tcp);
	}


	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);

/*東大のコード
	ret = APP_TMD_is_vaild_adr_in_ram_(ram_adr);

	if(ret == 1){
		memcpy((uint8_t*)ram_adr, param + 4, len);
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}
	else return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
*/
}

