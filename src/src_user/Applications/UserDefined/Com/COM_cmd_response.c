/**
  *****************************************************************************************
  * @file    : COM_cmd_response.c
  * @author  : Yuta Sakaguchi
  * @version : 1.0
  * @date    : 2022/06/06
  * @brief   :　コマンド応答
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2022/06/06  1.0   Y.Sakaguchi    First revision
  *
  *****************************************************************************************
  */
#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <string.h>//for memcpy

#include <src_core/Applications/gs_command_dispatcher.h>
#include <src_core/Applications/realtime_command_dispatcher.h>
#include <src_core/Applications/timeline_command_dispatcher.h>
#include <src_user/Drivers/CDH/WDT/wdt_manager.h>
#include <src_user/Applications/DriverInstances/command_response_handler.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/Com/COM_cmd_Response.h>
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/Memory/MEM_cmd_SD.h>
#include <src_user/Applications/UserDefined/Memory/MEM_cmd_SPIPROM.h>

#define flag_on 1

static uint16_t code_id;
uint8_t tlm_count;
uint8_t tlm_id = Tlm_CODE_TI;

static void command_response_init(void);
static void command_response_exe(void);

static void command_response_load_param(void);
static void command_response_initialize_param(void);

static uint16_t command_response_rtc_CMDcode;//to pic
const uint16_t command_response_rtc_CMDcode_  = 0xC112;

static uint16_t command_response_tlc_CMDcode;//to pic
const uint16_t command_response_tlc_CMDcode_  = 0xC113;

//static uint8_t command_response_route[RES_MAX];
//const uint8_t command_response_route_[RES_MAX] = {
//												(uint8_t)TCP_ROUTE_DIRECT,//from GS
//												(uint8_t)TCP_ROUTE_DIRECT,//from Millmax
//												(uint8_t)TCP_ROUTE_DIRECT,//from LoRa
//};
//
//static uint8_t command_response_exe_id[RES_MAX];
//const uint8_t command_response_exe_id_[RES_MAX] = {
//												(uint8_t)TCP_EXE_ID_TXPIC,//from GS
//												(uint8_t)TCP_EXE_ID_TXPIC,//from Millmax
//												(uint8_t)TCP_EXE_ID_TXPIC,//from LoRa
//};

const uint8_t tlc_goal_response_ = TO_COBC | TO_MILLMAX | TO_LORA;

#ifdef ONE_U
//static uint16_t command_response_tlm_apid[RES_MAX];
//const uint16_t command_response_tlm_apid_[RES_MAX] = {0x0132, 0x0132, 0x0132};
#endif

#ifdef TWO_U
//static uint16_t command_response_tlm_apid[RES_MAX];
//const uint16_t command_response_tlm_apid_[RES_MAX] = {0x0122, 0x0122, 0x0122};
#endif

//static uint8_t command_response_tlm_category;
//const uint8_t command_response_tlm_category_ = 0x20;

//static uint8_t command_response_tlm_id;
//const uint8_t command_response_tlm_id_ = Tlm_CODE_HK;

//static uint8_t command_response_num;
//const uint8_t command_response_num_ = 0x01;

//static uint8_t command_response_enable;
//const uint8_t command_response_enable_ =  0x01;

static uint8_t reanalize_pakcet_flag;
const uint8_t reanalize_pakcet_flag_ = 0x01;

//const uint8_t command_response_sat_id_[RES_MAX] = {(TCP_SAT_ID)MY_SAT_ID, (TCP_SAT_ID)MY_SAT_ID, (TCP_SAT_ID)MY_SAT_ID};
//static uint8_t command_response_sat_id[RES_MAX];

static uint8_t tlm_time_counter;
const uint8_t tlm_time_counter_ = 3;//コマンド応答から12秒後にhkダウンリンク

static uint8_t who_tlm;
const uint8_t who_tlm_ = SD_TLM_INFO;

static uint8_t tlm_info_flag[TF_MAX_TLMS];
const uint8_t tlm_info_flag_[TF_MAX_TLMS] = {
		TLM_ENABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,   	//Tlm_CODE_MOBC, Tlm_CODE_TL, Tlm_CODE_BL, Tlm_CODE_CA, Tlm_CODE_TF
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_MM, Tlm_CODE_AM, Tlm_CODE_APP_TIME, 	Tlm_CODE_AL, Tlm_CODE_AH
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_AH_RES, Tlm_CODE_RXPIC_AH1, Tlm_CODE_RXPIC_AH2, Tlm_CODE_TXPIC_AH1, Tlm_CODE_TXPIC_AH2
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_SUBCAM, Tlm_CODE_SD, Tlm_CODE_PROM, Tlm_CODE_REPRO_FLAG, Tlm_CODE_TLM_MEMDUMP_RAM
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_TLM_MEMDUMP_FLASH, Tlm_CODE_TLM_MEMDUMP_EEPROM, Tlm_CODE_TLM_MEMDUMP_MRAM, Tlm_CODE_TLM_MEMDUMP_SD, Tlm_CODE_TLM_MEMDUMP_SPIPROM
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_TLM_MEMDUMP_EEPROM3, Tlm_CODE_TLM_MEMDUMP_MRAM3, Tlm_CODE_TLM_MEMDUMP_SD3, Tlm_CODE_TLM_MEMDUMP_SPIPROM3, Tlm_CODE_MEM
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_COM_DEBUG, Tlm_CODE_MESSAGE, Tlm_CODE_HOGEHOGE32, Tlm_CODE_HOGEHOGE33, Tlm_CODE_HOGEHOGE34
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_HOGEHOGE35, Tlm_CODE_HOGEHOGE36, Tlm_CODE_HOGEHOGE37, Tlm_CODE_HOGEHOGE38, Tlm_CODE_FLASH_BAD_BLOCK
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_FLASH_ADR_TABLE, Tlm_CODE_LOG, Tlm_CODE_HOGEHOGE42,Tlm_CODE_HOGEHOGE43, Tlm_CODE_HOGEHOGE44
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_HOGEHOGE45, Tlm_CODE_HOGEHOGE46, Tlm_CODE_HOGEHOGE47, Tlm_CODE_HOGEHOGE48, Tlm_CODE_HOGEHOGE4
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_HOGEHOGE50, Tlm_CODE_HOGEHOGE51, Tlm_CODE_HOGEHOGE52, Tlm_CODE_HOGEHOGE53, Tlm_CODE_HOGEHOGE54
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_HOGEHOGE55, Tlm_CODE_HOGEHOGE56, Tlm_CODE_HOGEHOGE57, Tlm_CODE_HOGEHOGE58, Tlm_CODE_HOGEHOGE59
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_HOGEHOGE60, Tlm_CODE_HOGEHOGE61, Tlm_CODE_HOGEHOGE62, Tlm_CODE_HOGEHOGE63, Tlm_CODE_HOGEHOGE64
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_HOGEHOGE65, Tlm_CODE_HOGEHOGE66, Tlm_CODE_HOGEHOGE67, Tlm_CODE_HOGEHOGE68, Tlm_CODE_HOGEHOGE69
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_DISABLE,	//Tlm_CODE_HOGEHOGE70, Tlm_CODE_HOGEHOGE71, Tlm_CODE_HOGEHOGE72, Tlm_CODE_HOGEHOGE73, Tlm_CODE_HOGEHOGE74
		TLM_DISABLE, TLM_DISABLE, TLM_DISABLE, TLM_ENABLE, TLM_ENABLE,		//Tlm_CODE_HOGEHOGE75, Tlm_CODE_HOGEHOGE76, Tlm_CODE_HOGEHOGE77, Tlm_CODE_TI, Tlm_CODE_HK
		TLM_ENABLE, TLM_ENABLE, TLM_ENABLE, TLM_ENABLE, TLM_ENABLE,			//Tlm_CODE_SHK, Tlm_CODE_PWR_MOBC, Tlm_CODE_THERMAL, Tlm_CODE_SENSOR ,Tlm_CODE_SEPARATION
		TLM_ENABLE, TLM_ENABLE, TLM_ENABLE, TLM_ENABLE, TLM_ENABLE,			//Tlm_CODE_ADS, Tlm_CODE_AKF1, Tlm_CODE_AKF2, Tlm_CODE_ACS, Tlm_CODE_ODS
		TLM_ENABLE, TLM_ENABLE, TLM_ENABLE, TLM_ENABLE, TLM_DISABLE,		//Tlm_CODE_OKF, Tlm_CODE_OCS, Tlm_CODE_RXPIC_TO_MOBC, Tlm_CODE_TXPIC_TO_MOBC, Tlm_CODE_INTERSAT
		TLM_DISABLE, TLM_ENABLE, TLM_ENABLE, TLM_ENABLE, TLM_DISABLE,		//Tlm_CODE_SPIN, Tlm_CODE_AOCS80, Tlm_CODE_AOCS160, Tlm_CODE_AOCS240, Tlm_CODE_HOGEHOGE99
};

AppInfo command_response_update(void)
{
	  return AI_create_app_info("response_update", command_response_init, command_response_exe);
}

AppInfo command_response_param_update(void)
{
	  return AI_create_app_info("response_param_update", command_response_load_param, command_response_initialize_param);
}

static void command_response_load_param(void)
{
	read_param(&command_response_rtc_CMDcode, &command_response_rtc_CMDcode_, 2, (uint32_t)0x00003016);
	read_param(&command_response_tlc_CMDcode, &command_response_tlc_CMDcode_, 2, (uint32_t)0x00003018);
//	read_param(&command_response_route[0], &command_response_route_[0], 3, (uint32_t)0x0000301A);
//	read_param(&command_response_exe_id[0], &command_response_exe_id_[0], 3, (uint32_t)0x0000301D);
	read_param(&p_gs_response_struct->tlc_goal_response, &tlc_goal_response_, 1, (uint32_t)0x00003020);
//	read_param(&command_response_tlm_apid[0], &command_response_tlm_apid_[0], 6, (uint32_t)0x00003021);
//	read_param(&command_response_tlm_category, &command_response_tlm_category_, 1, (uint32_t)0x00003027);
//	read_param(&command_response_tlm_id, &command_response_tlm_id_, 1, (uint32_t)0x00003028);
//	read_param(&command_response_num, &command_response_num_, 1, (uint32_t)0x00003029);
//	read_param(&command_response_enable, &command_response_enable_, 1, (uint32_t)0x0000302A);
	read_param(&reanalize_pakcet_flag, &reanalize_pakcet_flag_, 1, (uint32_t)0x0000302B);
//	read_param(&command_response_sat_id[0], &command_response_sat_id_[0], 3, (uint32_t)0x0000302C);
	read_param(&tlm_time_counter, &tlm_time_counter_, 1, (uint32_t)0x0000302F);
	read_param(&tlm_info_flag[0], &tlm_info_flag_[0], TF_MAX_TLMS, (uint32_t)0x00003030);
	read_param(&who_tlm, &who_tlm_, 1, (uint32_t)0x00003130);

	tlm_count = tlm_time_counter;
}

static void command_response_initialize_param(void)
{
	write_param(&command_response_rtc_CMDcode, &command_response_rtc_CMDcode_, 2, (uint32_t)0x00003016);
	write_param(&command_response_tlc_CMDcode, &command_response_tlc_CMDcode_, 2, (uint32_t)0x00003018);
//	write_param(&command_response_route[0], &command_response_route_[0], 3, (uint32_t)0x0000301A);
//	write_param(&command_response_exe_id[0], &command_response_exe_id_[0], 3, (uint32_t)0x0000301D);
	write_param(&p_gs_response_struct->tlc_goal_response, &tlc_goal_response_, 1, (uint32_t)0x00003020);
//	write_param(&command_response_tlm_apid[0], &command_response_tlm_apid_[0], 6, (uint32_t)0x00003021);
//	write_param(&command_response_tlm_category, &command_response_tlm_category_, 1, (uint32_t)0x00003027);
//	write_param(&command_response_tlm_id, &command_response_tlm_id_, 1, (uint32_t)0x00003028);
//	write_param(&command_response_num, &command_response_num_, 1, (uint32_t)0x00003029);
//	write_param(&command_response_enable, &command_response_enable_, 1, (uint32_t)0x0000302A);
	write_param(&reanalize_pakcet_flag, &reanalize_pakcet_flag_, 1, (uint32_t)0x0000302B);
//	write_param(&command_response_sat_id[0], &command_response_sat_id_[0], 3, (uint32_t)0x0000302C);
	write_param(&tlm_time_counter, &tlm_time_counter_, 1, (uint32_t)0x0000302F);
	WDT_clear_count();  // Time over アノマリ対策
	write_param(&tlm_info_flag[0], &tlm_info_flag_[0], TF_MAX_TLMS, (uint32_t)0x00003030);
	write_param(&who_tlm, &who_tlm_, 1, (uint32_t)0x00003130);
}

static void command_response_init(void){
	response_init();
}

static void command_response_exe(void){
	if(p_gs_response_struct->res_flag == GSC_RES){
		gsc_response_handler();
	}
	else if(p_gs_response_struct->res_flag  == TLC_RES){
		tlc_response_handler();
	}
}

void response_init(void){
	p_gs_response_struct->res_flag = NON_RES;
	p_gs_response_struct->res_packet_id = NON_RESPONSE;
	p_gs_response_struct->res_apid = AFSK_GS;
	p_gs_response_struct->res_route =TCP_ROUTE_DIRECT;
	tlm_count = tlm_time_counter;
	txpic_driver-> reanalyze_packet = reanalize_pakcet_flag;
}

void gsc_response_handler(void){
	uint8_t gsc_buffer[17];
	if(p_gs_response_struct->res_packet_id == ONLY_RESULT_RESPONSE){
//		TF_copy_u8(&gsc_buffer[0], (uint8_t)(p_gs_response_struct->res_apid));
//		TF_copy_u8(&gsc_buffer[1], (uint8_t)(p_gs_response_struct->res_route));
//		TF_copy_u32(&gsc_buffer[2], (uint32_t)(TMGR_get_master_total_cycle()));
//		TF_copy_u16(&gsc_buffer[6], (uint16_t)(gs_command_dispatcher->prev.code));
//		TF_copy_i8(&gsc_buffer[8], (int8_t)(gs_command_dispatcher->prev.cmd_ret.exec_sts));
//		TF_copy_u16(&gsc_buffer[9], (uint16_t)(p_gs_response_struct->res_seq_count));
//		TF_copy_u32(&gsc_buffer[11], (uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle));
//		TF_copy_u16(&gsc_buffer[15], (uint16_t)(gs_command_dispatcher->prev_err.code));
//		TF_copy_i8(&gsc_buffer[17], (int8_t)(gs_command_dispatcher->prev_err.cmd_ret.exec_sts));
//		TF_copy_u32(&gsc_buffer[18], (uint32_t)(gs_command_dispatcher->error_counter));

//		parse_tlm_info(&gsc_buffer[22]);
        memcpy(&gsc_buffer[0], p_gs_response_struct->packet_header, sizeof(p_gs_response_struct->packet_header));
        TF_copy_u32(&gsc_buffer[6], (uint16_t)(gs_command_dispatcher->prev.time.total_cycle));
        TF_copy_u16(&gsc_buffer[6], (uint16_t)(gs_command_dispatcher->prev.code));
        TF_copy_i8(&gsc_buffer[8], (int8_t)(gs_command_dispatcher->prev.cmd_ret.exec_sts));
        TF_copy_u32(&gsc_buffer[9], (uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle));
        TF_copy_u16(&gsc_buffer[13], (uint16_t)(gs_command_dispatcher->prev_err.code));
        TF_copy_i8(&gsc_buffer[15], (int8_t)(gs_command_dispatcher->prev_err.cmd_ret.exec_sts));
        TF_copy_u32(&gsc_buffer[16], (uint32_t)(gs_command_dispatcher->error_counter));
//#ifdef TWO_U
////		CCP_form_rtc_to_other_obc(&p_gs_response_struct->tcp, TCP_APID_TXPIC_2U, (CMD_CODE)command_response_rtc_CMDcode,&gsc_buffer[0],sizeof(gsc_buffer));
//#endif
//#ifdef ONE_U
////		CCP_form_rtc_to_other_obc(&p_gs_response_struct->tcp, TCP_APID_TXPIC_1U, (CMD_CODE)command_response_rtc_CMDcode,&gsc_buffer[0],sizeof(gsc_buffer));
//#endif
////		memcpy(&code_id, &gs_command_dispatcher->prev.code, 2);

//        CCP_form_rtc_(&p_gs_response_struct->tcp, (CMD_CODE)command_response_rtc_CMDcode, p_gs_response_struct->packet_header, sizeof(p_gs_response_struct->packet_header));
        CCP_set_common_hdr(&p_gs_response_struct->tcp);
        CCP_set_id(&p_gs_response_struct->tcp, (CMD_CODE)command_response_rtc_CMDcode);
        CCP_set_exec_type(&p_gs_response_struct->tcp, CCP_EXEC_TYPE_RT);
        CCP_set_ti(&p_gs_response_struct->tcp, 0);  // RTの場合、TIは0固定
        CCP_set_param(&p_gs_response_struct->tcp, gsc_buffer, sizeof(gsc_buffer));
		TCP_set_apid_sat_id(&p_gs_response_struct->tcp, p_gs_response_struct->res_sat_id);
		TCP_set_apid_executer_id(&p_gs_response_struct->tcp, TCP_EXE_ID_TXPIC);
		TCP_set_route(&p_gs_response_struct->tcp, p_gs_response_struct->res_route);

		print(TXPIC,"gsc_buffer is");
		for(int i = 0; i< 17 + sizeof(gsc_buffer);i++){
			print(TXPIC,"%02x", p_gs_response_struct->tcp.packet[i]);
		}
		print(TXPIC,"\r\n");

		PH_analyze_packet(&p_gs_response_struct->tcp);

		response_init();
	}
	else if(p_gs_response_struct->res_packet_id == NON_RESPONSE){
		response_init();
	}
	else{
		//電流値が反映されないことがあるからelifにはしない
		if(tlm_count == tlm_time_counter){
//			TF_copy_u8(&gsc_buffer[0], (uint8_t)(p_gs_response_struct->res_apid));
//			TF_copy_u8(&gsc_buffer[1], (uint8_t)(p_gs_response_struct->res_route));
//			TF_copy_u32(&gsc_buffer[2], (uint32_t)(TMGR_get_master_total_cycle()));
//			TF_copy_u16(&gsc_buffer[6], (uint16_t)(gs_command_dispatcher->prev.code));
//			TF_copy_i8(&gsc_buffer[8], (int8_t)(gs_command_dispatcher->prev.cmd_ret.exec_sts));
//			TF_copy_u16(&gsc_buffer[9], (uint16_t)(p_gs_response_struct->res_seq_count));
//			TF_copy_u32(&gsc_buffer[11], (uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle));
//			TF_copy_u16(&gsc_buffer[15], (uint16_t)(gs_command_dispatcher->prev_err.code));
//			TF_copy_i8(&gsc_buffer[17], (int8_t)(gs_command_dispatcher->prev_err.cmd_ret.exec_sts));
//			TF_copy_u32(&gsc_buffer[18], (uint32_t)(gs_command_dispatcher->error_counter));
//
//			parse_tlm_info(&gsc_buffer[22]);

	        memcpy(&gsc_buffer[0], p_gs_response_struct->packet_header, sizeof(p_gs_response_struct->packet_header));
	        TF_copy_u32(&gsc_buffer[6], (uint16_t)(gs_command_dispatcher->prev.time.total_cycle));
	        TF_copy_u16(&gsc_buffer[6], (uint16_t)(gs_command_dispatcher->prev.code));
	        TF_copy_i8(&gsc_buffer[8], (int8_t)(gs_command_dispatcher->prev.cmd_ret.exec_sts));
	        TF_copy_u32(&gsc_buffer[9], (uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle));
	        TF_copy_u16(&gsc_buffer[13], (uint16_t)(gs_command_dispatcher->prev_err.code));
	        TF_copy_i8(&gsc_buffer[15], (int8_t)(gs_command_dispatcher->prev_err.cmd_ret.exec_sts));
	        TF_copy_u32(&gsc_buffer[16], (uint32_t)(gs_command_dispatcher->error_counter));
////			CCP_form_rtc_to_other_obc(&p_gs_response_struct->tcp, TCP_APID_TXPIC_2U, (CMD_CODE)command_response_rtc_CMDcode,&gsc_buffer[0],sizeof(gsc_buffer));
//#endif
//#ifdef ONE_U
//			CCP_form_rtc_to_other_obc(&p_gs_response_struct->tcp, TCP_APID_TXPIC_1U, (CMD_CODE)command_response_rtc_CMDcode,&gsc_buffer[0],sizeof(gsc_buffer));
//#endif
//			memcpy(&code_id, &gs_command_dispatcher->prev.code, 2);

//            CCP_form_rtc(&p_gs_response_struct->tcp, command_response_rtc_CMDcode, p_gs_response_struct->packet_header, sizeof(p_gs_response_struct->packet_header));
//			TCP_set_apid_sat_id(&p_gs_response_struct->tcp, p_gs_response_struct->res_sat_id);
//			TCP_set_apid_executer_id(&p_gs_response_struct->tcp, TCP_EXE_ID_TXPIC);
//			TCP_set_route(&p_gs_response_struct->tcp, p_gs_response_struct->res_route);
	        CCP_set_common_hdr(&p_gs_response_struct->tcp);
	        CCP_set_id(&p_gs_response_struct->tcp, (CMD_CODE)command_response_rtc_CMDcode);
	        CCP_set_exec_type(&p_gs_response_struct->tcp, CCP_EXEC_TYPE_RT);
	        CCP_set_ti(&p_gs_response_struct->tcp, 0);  // RTの場合、TIは0固定
	        CCP_set_param(&p_gs_response_struct->tcp, gsc_buffer, sizeof(gsc_buffer));
	        TCP_set_apid_sat_id(&p_gs_response_struct->tcp, p_gs_response_struct->res_sat_id);
	        TCP_set_apid_executer_id(&p_gs_response_struct->tcp, TCP_EXE_ID_TXPIC);
	        TCP_set_route(&p_gs_response_struct->tcp, p_gs_response_struct->res_route);
//	        TCP_CMD_set_param(&p_gs_response_struct->tcp, p_gs_response_struct->packet_header, 6);

			print(TXPIC,"gsc_buffer is");
			for(int i = 0; i< 17 + sizeof(gsc_buffer);i++){
				print(TXPIC,"%02x", p_gs_response_struct->tcp.packet[i]);
			}
			print(TXPIC,"\r\n");

			PH_analyze_packet(&p_gs_response_struct->tcp);
			--tlm_count;
		}
		else if(tlm_count <= 0){
			tlm_response_handler();
			response_init();
		}
		else{
			--tlm_count;
		}
	}
}

void tlc_response_handler(void){
	uint8_t tlc_buffer[26];
	if(p_gs_response_struct->res_packet_id == ONLY_RESULT_RESPONSE){
		TF_copy_u8(&tlc_buffer[0], (uint8_t)(p_gs_response_struct->res_apid));
		TF_copy_u8(&tlc_buffer[1], (uint8_t)(p_gs_response_struct->res_route));
		TF_copy_u32(&tlc_buffer[2], (uint32_t)(TMGR_get_master_total_cycle()));
		TF_copy_u16(&tlc_buffer[6], (uint16_t)(p_gs_response_struct->tlc_register_id));
		TF_copy_i8(&tlc_buffer[8], (int8_t)(p_gs_response_struct->res_seq_count));
		TF_copy_u32(&tlc_buffer[9], (uint32_t)(p_gs_response_struct->tlc_register_id));
		TF_copy_u16(&tlc_buffer[13], (uint16_t)(p_gs_response_struct->tlc_register_time));
		TF_copy_u16(&tlc_buffer[15], (uint16_t)CCP_get_id(PL_get_head(&(PH_tl_cmd_list[0]))->packet));
		TF_copy_u32(&tlc_buffer[17], (uint32_t)CCP_get_ti(PL_get_head(&(PH_tl_cmd_list[0]))->packet));
		parse_tlm_info(&tlc_buffer[21]);

		memcpy(&code_id, &timeline_command_dispatcher->dispatcher[0].prev.code, 2);
#ifdef TWO_U
//		CCP_form_rtc_to_other_obc(&p_gs_response_struct->tcp, TCP_APID_TXPIC_2U, (CMD_CODE)command_response_tlc_CMDcode,&tlc_buffer[0],sizeof(tlc_buffer));
#endif
#ifdef ONE_U
		CCP_form_rtc_to_other_obc(&p_gs_response_struct->tcp, TCP_APID_TXPIC_1U, (CMD_CODE)command_response_tlc_CMDcode,&tlc_buffer[0],sizeof(tlc_buffer));
#endif
        CCP_form_rtc(&p_gs_response_struct->tcp, command_response_rtc_CMDcode, p_gs_response_struct->packet_header, sizeof(p_gs_response_struct->packet_header));
		TCP_set_apid_sat_id(&p_gs_response_struct->tcp, p_gs_response_struct->res_sat_id);
		TCP_set_apid_executer_id(&p_gs_response_struct->tcp, TCP_EXE_ID_TXPIC);
		TCP_set_route(&p_gs_response_struct->tcp, p_gs_response_struct->res_route);

//        TCP_CMD_set_param(&p_gs_response_struct->tcp, p_gs_response_struct->packet_header, 6);

		PH_analyze_packet(&p_gs_response_struct->tcp);

		print(TXPIC,"tlc_response is");
		for(int i = 0; i< 17 + sizeof(tlc_buffer);i++){
			print(TXPIC,"%02x", p_gs_response_struct->tcp.packet[i]);
		}
		print(TXPIC,"\r\n");

		response_init();
	}
	else if(p_gs_response_struct->res_packet_id == NON_RESPONSE){
		response_init();
	}
	else{
		if(tlm_count == tlm_time_counter){
			TF_copy_u8(&tlc_buffer[0], (uint8_t)(p_gs_response_struct->res_apid));
			TF_copy_u8(&tlc_buffer[1], (uint8_t)(p_gs_response_struct->res_route));

			TF_copy_u32(&tlc_buffer[2], (uint32_t)(TMGR_get_master_total_cycle()));
			TF_copy_u16(&tlc_buffer[6], (uint16_t)(p_gs_response_struct->tlc_register_id));
			TF_copy_i8(&tlc_buffer[8], (int8_t)(p_gs_response_struct->res_seq_count));
			TF_copy_u32(&tlc_buffer[9], (uint32_t)(p_gs_response_struct->tlc_register_id));
			TF_copy_u16(&tlc_buffer[13], (uint16_t)(p_gs_response_struct->tlc_register_time));
			TF_copy_u16(&tlc_buffer[15], (uint16_t)CCP_get_id(PL_get_head(&(PH_tl_cmd_list[0]))->packet));
			TF_copy_u32(&tlc_buffer[17], (uint32_t)CCP_get_ti(PL_get_head(&(PH_tl_cmd_list[0]))->packet));
			parse_tlm_info(&tlc_buffer[21]);

			memcpy(&code_id, &timeline_command_dispatcher->dispatcher[0].prev.code, 2);
//#ifdef TWO_U
			CCP_form_rtc_to_other_obc(&p_gs_response_struct->tcp, TCP_APID_TXPIC_2U, (CMD_CODE)command_response_tlc_CMDcode,&tlc_buffer[0],sizeof(tlc_buffer));
//#endif
//#ifdef ONE_U
//			CCP_form_rtc_to_other_obc(&p_gs_response_struct->tcp, TCP_APID_TXPIC_1U, (CMD_CODE)command_response_tlc_CMDcode,&tlc_buffer[0],sizeof(tlc_buffer));
//#endif
			CCP_form_rtc(&p_gs_response_struct->tcp, command_response_rtc_CMDcode, p_gs_response_struct->packet_header, sizeof(p_gs_response_struct->packet_header));
			TCP_set_apid_sat_id(&p_gs_response_struct->tcp, p_gs_response_struct->res_sat_id);
			TCP_set_apid_executer_id(&p_gs_response_struct->tcp, TCP_EXE_ID_TXPIC);
			TCP_set_route(&p_gs_response_struct->tcp, p_gs_response_struct->res_route);

//			TCP_CMD_set_param(&p_gs_response_struct->tcp, p_gs_response_struct->packet_header, 6);

			PH_analyze_packet(&p_gs_response_struct->tcp);

			print(TXPIC,"tlc_response is");
			for(int i = 0; i< 17 + sizeof(tlc_buffer);i++){
				print(TXPIC,"%02x", p_gs_response_struct->tcp.packet[i]);
			}
			print(TXPIC,"\r\n");
			--tlm_count;
		}
		else if(tlm_count <= 0){
			tlm_response_handler();
			response_init();
		}
		else{
			--tlm_count;
		}
	}
}

void tlm_response_handler(void){
	uint8_t param[6];
	uint8_t tlm_category;
	if(p_gs_response_struct->res_packet_id == Tlm_CODE_HK || p_gs_response_struct->res_packet_id == Tlm_CODE_SHK){
		tlm_category = 0x20;
	}
	else{
		tlm_category = 0x40;
	}
	TF_copy_u16(&param[0], (uint16_t)(p_gs_response_struct->res_apid));
	TF_copy_u8(&param[2], (uint8_t)(p_gs_response_struct->res_route));
	TF_copy_u8(&param[3], (uint8_t)(tlm_category));
	TF_copy_u8(&param[4], (uint8_t)(p_gs_response_struct->res_packet_id));
	TF_copy_u8(&param[5], (uint8_t)(1));
	CCP_form_rtc(&p_gs_response_struct->tcp,(CMD_CODE)Cmd_CODE_GENERATE_TLM_USER, &param[0], 6);
	TCP_set_apid_(&p_gs_response_struct->tcp, MY_SAT_ID, TCP_EXE_ID_MOBC);
	PH_analyze_packet(&p_gs_response_struct->tcp);
}

void parse_tlm_info(uint8_t* in_buf){
	if(who_tlm == SD_TLM_INFO){
		for(int now_tlm_id = tlm_id; now_tlm_id < TF_MAX_TLMS; now_tlm_id++){
			if(tlm_info_flag[now_tlm_id] == TLM_ENABLE){
				if(tlm_id < Tlm_CODE_HOGEHOGE74){
					TF_copy_u8(&in_buf[0], (uint8_t)(now_tlm_id));
					TF_copy_u32(&in_buf[1], (uint32_t)(p_sd_struct->sdi_irregular[now_tlm_id].save_info.BLOCK_NOW));
				}
				else if(tlm_id >= Tlm_CODE_HOGEHOGE74 && tlm_id < TF_MAX_TLMS){
					const SD_Info_Regular* p_sd = &(p_sd_struct->sdi_regular[now_tlm_id - Tlm_CODE_HOGEHOGE74]);
					TF_copy_u8(&in_buf[0], (uint8_t)(now_tlm_id));
					TF_copy_u32(&in_buf[1], (uint32_t)(p_sd->save_info.BLOCK_NOW));
				}
				++tlm_id;
				if(tlm_id == TLM_CODE_MAX){
					tlm_id = 0;
				}
				break;
			}
		}
	}
	else if(who_tlm == PROM_TLM_INFO){
		for(int now_tlm_id = tlm_id; now_tlm_id < TF_MAX_TLMS; now_tlm_id++){
			if(tlm_info_flag[now_tlm_id] == TLM_ENABLE){
				  if(tlm_id <= Tlm_CODE_PROM){
					  TF_copy_u8(&in_buf[0], (uint8_t)(now_tlm_id));
					  TF_copy_u32(&in_buf[1], (uint32_t)(p_prom_struct->spiprom_info[now_tlm_id].SECTOR_NOW));
				  }
				  else if(tlm_id >= Tlm_CODE_TI && tlm_id <= Tlm_CODE_AOCS240){
					  TF_copy_u8(&in_buf[0], (uint8_t)(now_tlm_id));
					  TF_copy_u32(&in_buf[1],
							  (uint32_t)(p_prom_struct->spiprom_info[now_tlm_id].SECTOR_NOW));
				  }
				  else{
					  for(int i = 0; i < 5; i++){
						  in_buf[i]=0;
					  }
				  }
			}
		}
	}
}
