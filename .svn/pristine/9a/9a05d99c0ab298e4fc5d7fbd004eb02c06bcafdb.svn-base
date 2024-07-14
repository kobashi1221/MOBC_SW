/**
  *****************************************************************************************
  * @file    : CDH1_debug_SD.cpp
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/10/17
  * @brief   : SD Card Application for CDH1 Board Debug
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/10/17  1.0   N.KISHI         First revision
  * 2020/11/25  2.0   K.KIMURA        Add SD card function
  *
  *****************************************************************************************
  */

#include "MEM_cmd_SD.h"
#include  "../../../IfWrapper/HAL.h"

#include <stdio.h> //for printf
#include <src_core/Library/endian.h>
#include <src_user/Applications/DriverInstances/di_txpic.h>
#include <src_user/Applications/DriverInstances/block_command_handler.h>
#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.h>
#include <src_user/Applications/DriverInstances/di_LoRa.h>
#include <src_user/Applications/DriverInstances/tlm_packet_handler.h>
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/tlm_mem_dump.h>
#include <src_user/Applications/UserDefined/Cdh/ResetUtility.h>
#include <src_user/Applications/DriverInstances/command_response_handler.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>

#include "../../../TlmCmd/CCSDS/TCPacket.h"
#include <src_core/TlmCmd/packet_list.h>
#include "../../../TlmCmd/telemetry_definitions.h"
#include <src_core/TlmCmd/packet_handler.h>
#include <src_user/Settings/TlmCmd/telemetry_frame_params.h>

//#include <src_core/System/AnomalyLogger/anomaly_logger.h>

static SD_STRUCT sd_struct;

const SD_STRUCT* p_sd_struct;
static uint8_t SD__tlm_page_no_;
const uint8_t* SD_tlm_page_no;

static void CDH1_sd_power_on_init(void);
static void CDH1_save_tlm_sd_exe(void);

static void CDH1_sd_load_param(void);
static void CDH1_sd_initialize_param(void);

static void CDH1_sd_memory_patrol_(void);

void CDH1_sd_load_program_data(void);

void sd_save_irregular_packet(SD_SAVE_Info* save_info, const TCP* stpkt);
void sd_save_regular_packet(SD_Info_Regular* sdi, const TCP* stpkt);
void RP_TLM_set_count(TCP* tcp, uint32_t count);
TLM_DL_ack SD_analyze_pzcket(const TCP* packet);

static TCP read_data;

static uint8_t tlm_flag[TF_MAX_TLMS - Tlm_CODE_HOGEHOGE74];
const  uint8_t tlm_flag_[TF_MAX_TLMS - Tlm_CODE_HOGEHOGE74] = {
		NOT_GENERATE, NOT_GENERATE, NOT_GENERATE, NOT_GENERATE, EVERY,
		LOW_RATE_2,   LOW_RATE_2,   LOW_RATE_2,   LOW_RATE_2,   EVERY,
		EVERY,        EVERY,        EVERY,        EVERY,        EVERY,
		EVERY,        EVERY,        EVERY,        LOW_RATE_2,   LOW_RATE_2,
		NOT_GENERATE, NOT_GENERATE, EVERY,        EVERY,        EVERY,
		NOT_GENERATE
};
const uint8_t* p_tlm_flag = &tlm_flag;

const uint32_t irregular_condition_block_ = 0x0000393C;
const uint32_t regular_condition_block_ = 0x00003990;

uint32_t Storage_Area[TF_MAX_TLMS*2];
const uint32_t Storage_Area_[TF_MAX_TLMS*2] = {
		0x00003A00, 0x000041FF,		0x00004300, 0x00004AFF,		0x00004C00, 0x000053FF,		0x00005500, 0x00005CFF,
		0x00005E00, 0x000065FF,		0x00006700, 0x00006EFF,		0x00007000, 0x000077FF,		0x00007900, 0x000080FF,
		0x00008200, 0x000089FF,		0x00008B00, 0x000092FF,		0x00009400, 0x00009BFF,		0x00009D00, 0x0000A4FF,
		0x0000A600, 0x0000ADFF,		0x0000AF00, 0x0000B6FF,		0x0000B800, 0x0000BFFF,		0x0000C100, 0x000318FF,
		0x00031A00, 0x000321FF,		0x00032300, 0x00032AFF,		0x00032C00, 0x000333FF,		0x00033500, 0x00033CFF,
		0x00033E00, 0x000345FF,		0x00034700, 0x00034EFF,		0x00035000, 0x000357FF,		0x00035900, 0x000360FF,
		0x00036200, 0x000369FF,		0x00036B00, 0x000372FF,		0x00037400, 0x00037BFF,		0x00037D00, 0x000384FF,
		0x00038600, 0x00038DFF,		0x00038F00, 0x000396FF,		0x00039800, 0x00039FFF,		0x0003A100, 0x0003A8FF,
		0x0003AA00, 0x0003AAFF,		0x0003AC00, 0x0003ACFF,		0x0003AE00, 0x0003AEFF,		0x0003B000, 0x0003B7FF,
		0x0003B900, 0x0003D67F,		0x0003D780, 0x0003F4FF,		0x0003F600, 0x0004137F,		0x00041480, 0x000431FF,
		0x00043300, 0x00043AFF,		0x00043C00, 0x000443FF,		0x00044500, 0x00044CFF,		0x00044E00, 0x000455FF,
		0x00045700, 0x00045EFF,		0x00046000, 0x000467FF,		0x00046900, 0x000470FF,		0x00047200, 0x000479FF,
		0x00047B00, 0x000482FF,		0x00048400, 0x00048BFF,		0x00048D00, 0x000494FF,		0x00049600, 0x00049DFF,
		0x00049F00, 0x0004A6FF,		0x0004A800, 0x0004AFFF,		0x0004B100, 0x0004B8FF,		0x0004BA00, 0x0004C1FF,
		0x0004C300, 0x0004CAFF,		0x0004CC00, 0x0004D3FF,		0x0004D500, 0x0004DCFF,		0x0004DE00, 0x0004E5FF,
		0x0004E700, 0x0004EEFF,		0x0004F000, 0x0004F7FF,		0x0004F900, 0x000500FF,		0x00050200, 0x000509FF,
		0x00050B00, 0x000512FF,		0x00051400, 0x00051BFF,		0x00051D00, 0x000524FF,		0x00052600, 0x00052DFF,
		0x00052F00, 0x000536FF,		0x00053800, 0x00053FFF,		0x00054100, 0x000548FF,		0x00054A00, 0x000551FF,
		0x00055300, 0x00055AFF,		0x00055C00, 0x000563FF,		0x00056500, 0x001504FF,		0x00150600, 0x0024A5FF,
		0x0024A700, 0x003446FF,		0x00344800, 0x0043E7FF,		0x0043E900, 0x004898FF,		0x00489A00, 0x004D49FF,
		0x004D4B00, 0x0051FAFF,		0x0051FC00, 0x0056ABFF,		0x0056AD00, 0x005B5CFF,		0x005B5E00, 0x006AFDFF,
		0x006AFF00, 0x008AFEFF,		0x008B0000, 0x009A9FFF,		0x009AA100, 0x00AA40FF,		0x00AA4200, 0x00B9E1FF,
		0x00B9E300, 0x00C982FF,		0x00C98400, 0x00D923FF,		0x00D92500, 0x00E8C4FF,		0x00E8C600, 0x00F865FF,
		0x00F86700, 0x00FD16FF,		0x00FD1800, 0x0101C7FF,		0x0101C900, 0x011168FF,		0x01116A00, 0x013169FF,
		0x01316B00, 0x01410AFF,		0x01410C00, 0x01587BFF,		0x01587D00, 0x01787CFF,		0x01787E00, 0x01881DFF,
};

//0: リングバッファOK　1: stop
static uint8_t ring_permission[TF_MAX_TLMS];
const uint8_t ring_permission_[TF_MAX_TLMS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//Flag for comparing program data in Flash with that in SD
static uint8_t compare_program_data_flag;
const uint8_t compare_program_data_flag_ = 0;
// parameters for CDH1_sd_load_program_data
static uint32_t num = 0x00000B72;                   //Start block address of program data in SD
const uint32_t start_program_block = 0x00000B72;
uint8_t *flash_pointer = (uint8_t*)0x8010000;         //Address of flight program in Flash
uint32_t program_mis_address;

static uint32_t max_patrol_block;
const uint32_t max_patrol_block_ = 0x00001080;
const uint32_t limit_max_block = 0x00001313;

const uint32_t COMMAND_SAVE_SECTION = 0x018FAB00;

uint32_t save_count = 0;
/**
 * @brief アプリケーション作成関数
 *
 * @note
 * @return アプリケーション構造体
 */
AppInfo CDH1_sd_update(void)
{
    return AI_create_app_info("DBG_SD", CDH1_sd_power_on_init, CDH1_save_tlm_sd_exe);
}

AppInfo CDH1_sd_param_update(void)
{
	return AI_create_app_info("SD_param", CDH1_sd_load_param, CDH1_sd_initialize_param);
}

AppInfo CDH1_sd_memory_patrol(void)
{
	return AI_create_app_info("SD_param", NULL, CDH1_sd_memory_patrol_);
}

AppInfo CDH1_sd_program_data(void){
	return AI_create_app_info("SD_param", NULL, CDH1_sd_load_program_data);
}

static void CDH1_sd_memory_patrol_(void){
	uint8_t dummy[512];

	if(sd_struct.SDPatrolBlock < 0x00000B72){
		SD_read_param(dummy, NULL, SD_BLOCKSIZE, (uint32_t)(sd_struct.SDPatrolBlock * SD_BLOCKSIZE));
	}
	else if(sd_struct.SDPatrolBlock >= 0x00000B72 && sd_struct.SDPatrolBlock < 0x00001314){
		SD_read_program(dummy, NULL, SD_BLOCKSIZE, (uint32_t)(sd_struct.SDPatrolBlock * SD_BLOCKSIZE));
	}

	sd_struct.SDPatrolBlock++;
	if(sd_struct.SDPatrolBlock > max_patrol_block) sd_struct.SDPatrolBlock = 0;//0x00001313
	if(sd_struct.SDPatrolBlock > limit_max_block) sd_struct.SDPatrolBlock = 0;
}


static void CDH1_sd_load_param(void){

	read_param(&tlm_flag, &tlm_flag_, sizeof(tlm_flag), (uint32_t)0x00003840);
	read_param(&sd_struct.irregular_condition_block, &irregular_condition_block_, 4, (uint32_t)0x00003870);
	read_param(&sd_struct.regular_condition_block, &regular_condition_block_, 4, (uint32_t)0x00003874);
	read_param(&Storage_Area, &Storage_Area_, sizeof(Storage_Area), (uint32_t)0x00003878);
	read_param(&ring_permission, &ring_permission_, sizeof(ring_permission), (uint32_t)0x00003B98);
	read_param(&compare_program_data_flag, &compare_program_data_flag_, sizeof(compare_program_data_flag), (uint32_t)0x00003BFC);
	read_param(&max_patrol_block, &max_patrol_block_, sizeof(max_patrol_block_), (uint32_t)0x00003BFD);
}

static void CDH1_sd_initialize_param(void){
	write_param(&tlm_flag, &tlm_flag_, sizeof(tlm_flag), (uint32_t)0x00003840);
	write_param(&sd_struct.irregular_condition_block, &irregular_condition_block_, 4, (uint32_t)0x00003870);
	write_param(&sd_struct.regular_condition_block, &regular_condition_block_, 4, (uint32_t)0x00003874);
	write_param(&Storage_Area, &Storage_Area_, sizeof(Storage_Area), (uint32_t)0x00003878);
	write_param(&ring_permission, &ring_permission_, sizeof(ring_permission), (uint32_t)0x00003B98);
	write_param(&compare_program_data_flag, &compare_program_data_flag_, sizeof(compare_program_data_flag), (uint32_t)0x00003BFC);
	write_param(&max_patrol_block, &max_patrol_block_, sizeof(max_patrol_block_), (uint32_t)0x00003BFD);
}

void CDH1_sd_load_program_data(void){
	uint8_t sd_data[512];

     //start flag
	if(compare_program_data_flag != 0xff) return;

		//Load program data from SD
		BSP_SD_ReadBlocks((uint32_t*)sd_data ,num ,1 ,0xFFFF);
		uint8_t* tmp = flash_pointer;
		for(int i=0;i<512;i++){
			if(sd_data[i] != *tmp){
				print(SD, "(%08x)%02x, %02x\r\n", tmp, sd_data[i], *tmp);
//				AL_add_anomaly(AL_GROUP_PROGRAM_DATA,PROGRAM_DATA_MISMATCH);
				program_mis_address = (uint32_t)tmp;
				generate_memory_repro_tlm();
				break;
			}
			tmp++;
		}
		//To next block
		num++;
		flash_pointer += SD_BLOCKSIZE;
		//point of going back
		if(num > max_patrol_block_){
			num = start_program_block;
			flash_pointer = (uint8_t*)0x08010000;
		}
		if(num > limit_max_block){
			num = start_program_block;
			flash_pointer = (uint8_t*)0x08010000;
		}
}


/**
  * @brief  SDの初期化と保存状態の読み込み
  *         保存状態がおかしい時はstartアドレスにする
  *
  */
static void CDH1_sd_power_on_init(void){

	p_sd_struct = &sd_struct;
	SD_tlm_page_no = &SD__tlm_page_no_;
	uint8_t analyze_cnt = 0;

	sd_struct.SDPatrolBlock = 0;
	sd_struct.tlm_dl_param.RP_TLM_BLOCK = 0;
	sd_struct.tlm_dl_param.RP_TLM_CODE = TF_MAX_TLMS;
	sd_struct.tlm_dl_param.RP_TLM_packet_num = 0;
	sd_struct.tlm_dl_param.RP_TLM_APID = AFSK_GS;
	sd_struct.tlm_dl_param.RP_TLM_ROUTE = TCP_ROUTE_DIRECT;
	sd_struct.tlm_dl_param.width = 1;
	sd_struct.tlm_dl_param.tlm_dl_ack = TLM_WAIT;
	sd_struct.tlm_dl_param.process_count = 0;

	sd_struct.SDPatrolBlock = 0x00000B70;

	for(int i = 0; i < Tlm_CODE_HOGEHOGE74; i++){
		sd_struct.sdi_irregular[i].save_info.POSITION_COUNTER = 0;
		sd_struct.sdi_irregular[i].save_info.BLOCK_START = Storage_Area[2*i];
		sd_struct.sdi_irregular[i].save_info.BLOCK_END = Storage_Area[2*i + 1];
		sd_struct.sdi_irregular[i].save_info.ring_permission = ring_permission[i];
	}
	for(int i = 0; i < (TF_MAX_TLMS - Tlm_CODE_HOGEHOGE74); i++){
		sd_struct.sdi_regular[i].save_info.POSITION_COUNTER = 0;
		sd_struct.sdi_regular[i].save_info.BLOCK_START = Storage_Area[2 * (i + Tlm_CODE_HOGEHOGE74)];
		sd_struct.sdi_regular[i].save_info.BLOCK_END = Storage_Area[2 * (i + Tlm_CODE_HOGEHOGE74) + 1];
		sd_struct.sdi_regular[i].save_info.ring_permission = ring_permission[i + Tlm_CODE_HOGEHOGE74];
	}

	uint8_t res = BSP_SD_Init();
	if(res != BSP_SD_OK){
		printf("SD INIT FAILED\r\n");
	}

	uint8_t cond1[9 * Tlm_CODE_HOGEHOGE74];
	res = sd_specify_read(sd_struct.irregular_condition_block, 0, cond1, sizeof(cond1));
	if(res != BSP_SD_OK){
		ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
		printf("read err\r\n");
	}

	for(int i = 0; i < Tlm_CODE_HOGEHOGE74; i++){
		memcpy(&(sd_struct.sdi_irregular[i].save_info.BLOCK_OLD), 	     &(cond1[9*i    ]), 4);
		memcpy(&(sd_struct.sdi_irregular[i].save_info.BLOCK_NOW), 		 &(cond1[9*i + 4]), 4);
		memcpy(&(sd_struct.sdi_irregular[i].save_info.POSITION_COUNTER), &(cond1[9*i + 8]), 1);
		res = sd_block_analyze(&(sd_struct.sdi_irregular[i].save_info));
		if(res != 0){
			analyze_cnt++;
			sd_struct.sdi_irregular[i].save_info.POSITION_COUNTER = 0;
			memcpy(&(cond1[9*i    ]), &(sd_struct.sdi_irregular[i].save_info.BLOCK_START), 	    4);
			memcpy(&(cond1[9*i + 4]), &(sd_struct.sdi_irregular[i].save_info.BLOCK_START), 		4);
			memcpy(&(cond1[9*i + 8]), &(sd_struct.sdi_irregular[i].save_info.POSITION_COUNTER), 1);
		}
	}
	if(analyze_cnt != 0){
		analyze_cnt = 0;
		res = sd_specify_write(sd_struct.irregular_condition_block, 0, cond1, sizeof(cond1));
		if(res != BSP_SD_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
			printf("write err\r\n");
		}
	}

	uint8_t cond2[8 * (TF_MAX_TLMS - Tlm_CODE_HOGEHOGE74)];
	res = sd_specify_read(sd_struct.regular_condition_block, 0, cond2, sizeof(cond2));
	if(res != BSP_SD_OK){
		ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
		printf("read err\r\n");
	}

	for(int i = 0; i < (TF_MAX_TLMS - Tlm_CODE_HOGEHOGE74); i++){
		memcpy(&(sd_struct.sdi_regular[i].save_info.BLOCK_OLD), &(cond2[8*i    ]), 4);
		memcpy(&(sd_struct.sdi_regular[i].save_info.BLOCK_NOW), &(cond2[8*i + 4]), 4);
		res = sd_block_analyze(&(sd_struct.sdi_regular[i].save_info));
		if(res != 0){
			analyze_cnt++;
			memcpy(&(cond2[9*i    ]), &(sd_struct.sdi_regular[i].save_info.BLOCK_START), 4);
			memcpy(&(cond2[9*i + 4]), &(sd_struct.sdi_regular[i].save_info.BLOCK_START), 4);
		}
	}
	if(analyze_cnt != 0){
		analyze_cnt = 0;
		res = sd_specify_write(sd_struct.regular_condition_block, 0, cond2, sizeof(cond2));
		if(res != BSP_SD_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
			printf("write err\r\n");
		}
	}

	printf("SD init end\r\n");
}


/**
  * @brief  テレメトリ保存
  *         packet長を512の約数にして512Byteたまったら保存する
  *
  */
static void CDH1_save_tlm_sd_exe(void){

	save_count = 0;
	for(uint8_t i=0; i<PH_ST_TLM_LIST_MAX; i++){//LIST_MAX分繰り返す

		if(PL_is_empty(&PH_st_tlm_list)){//空の場合終わり
//			printf("\r\nsave_count = %d\r\n\r\n", save_count);
			return;
		}

		const TCP* stpkt = (TCP*)PL_get_head(&PH_st_tlm_list)->packet;		//リストのheadにあるpacketをコピー

		int len =TCP_get_packet_len(stpkt) + TCP_PRM_HDR_LEN;
		TCP_SAT_ID sat_id=TCP_get_apid_sat_id(stpkt);
		if((sat_id&0x01) == MY_SAT_ID){//自機宛
			uint8_t tlm_id = TCP_TLM_get_packet_id(stpkt);
//			printf("%d, ", tlm_id);
			if(tlm_id < Tlm_CODE_HOGEHOGE74){
				save_count++;
				sd_save_irregular_packet(&(sd_struct.sdi_irregular[tlm_id].save_info), stpkt);
			}
			if(tlm_id >= Tlm_CODE_HOGEHOGE74 && tlm_id < TF_MAX_TLMS){
				sd_save_regular_packet(&(sd_struct.sdi_regular[tlm_id - Tlm_CODE_HOGEHOGE74]), stpkt);
			}
		}
		else{//自機宛でない (他衛星宛てのコマンド)
	    	switch(TCP_get_route(stpkt)){
	  	  		case TCP_ROUTE_AFSK:
	    			PH_add_txpic_tlm_(stpkt);
	    			break;
	  	  		case TCP_ROUTE_MILLMAX:
	    			INTERSAT_Send(intersat_driver,stpkt->packet,len);
	    			break;
	  	  	  case TCP_ROUTE_LORA:
	  	  		    print(LORA,"lora_route\r\n");
	  	  		    LORA_Send(lora_driver,stpkt->packet,len);
					break;
	  	  	  default :
	  	  		    break;
	  	  }
		}

		PL_drop_executed(&PH_st_tlm_list);
	}
//	printf("\r\nsave_count(max) = %d\r\n\r\n", save_count);
	return;
}


/**
  * @brief  各テレメトリの現在のブロックアドレスを保存する
  * @param  無し
  */
CCP_CmdRet Cmd_CDH1_SD_CONDITION_SAVE(const CommonCmdPacket* packet){
	uint8_t res = 0;

	uint8_t cond1[9 * Tlm_CODE_HOGEHOGE74];

	for(int i = 0; i < Tlm_CODE_HOGEHOGE74; i++){
		memcpy(&(cond1[9*i    ]), &(sd_struct.sdi_irregular[i].save_info.BLOCK_OLD), 	    4);
		memcpy(&(cond1[9*i + 4]), &(sd_struct.sdi_irregular[i].save_info.BLOCK_NOW), 		4);
		memcpy(&(cond1[9*i + 8]), &(sd_struct.sdi_irregular[i].save_info.POSITION_COUNTER), 4);
	}
	res = sd_specify_write(sd_struct.irregular_condition_block, 0, cond1, sizeof(cond1));
	if(res != BSP_SD_OK){
		ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
		printf("write err\r\n");
	}

	uint8_t cond2[8 * (TF_MAX_TLMS - Tlm_CODE_HOGEHOGE74)];

	for(int i = 0; i < (TF_MAX_TLMS - Tlm_CODE_HOGEHOGE74); i++){
		memcpy(&(cond2[8*i    ]), &(sd_struct.sdi_regular[i].save_info.BLOCK_OLD), 4);
		memcpy(&(cond2[8*i + 4]), &(sd_struct.sdi_regular[i].save_info.BLOCK_NOW), 4);
	}
	res = sd_specify_write(sd_struct.regular_condition_block, 0, cond2, sizeof(cond2));
	if(res != BSP_SD_OK){
		ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
		printf("write err\r\n");
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief replay_tlm生成のパラメータ変更
 * @param TLM_CODE
 * @param TCP_ROUTE
 * @param どのブロックから
 * @param 何ブロック分
 */
CCP_CmdRet Cmd_CDH1_SD_CHANGE_RPTLM_param(const CommonCmdPacket* packet){

	const uint8_t* param = CCP_get_param_head(packet);

	memcpy(&sd_struct.tlm_dl_param.RP_TLM_CODE, param, 1);
	ENDIAN_memcpy(&sd_struct.tlm_dl_param.RP_TLM_APID, param + 1, 2);
	memcpy(&sd_struct.tlm_dl_param.RP_TLM_ROUTE, param + 3, 1);
	ENDIAN_memcpy(&sd_struct.tlm_dl_param.RP_TLM_BLOCK, param + 4, 4);
	memcpy(&sd_struct.tlm_dl_param.pos_in_block, param + 8, 1);
	ENDIAN_memcpy(&sd_struct.tlm_dl_param.RP_TLM_packet_num, param + 9, 4);
	memcpy(&sd_struct.tlm_dl_param.width, param + 13, 1);
	memcpy(&sd_struct.tlm_dl_param.dump_num, param + 14, 1);

	if(sd_struct.tlm_dl_param.RP_TLM_CODE >= TF_MAX_TLMS){	//TLM_CODEが正しくないとき
		sd_struct.tlm_dl_param.RP_TLM_CODE = TF_MAX_TLMS;
		sd_struct.tlm_dl_param.RP_TLM_APID = AFSK_GS;
		sd_struct.tlm_dl_param.RP_TLM_ROUTE = TCP_ROUTE_DIRECT;
		sd_struct.tlm_dl_param.RP_TLM_packet_num = 0;
		sd_struct.tlm_dl_param.tlm_dl_ack = TLM_ID_ERROR;
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	if(sd_struct.tlm_dl_param.dump_num > 8){
		sd_struct.tlm_dl_param.tlm_dl_ack = TLM_DUMP_NUM_ERROR;
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

	sd_struct.tlm_dl_param.tlm_dl_ack = TLM_PROCEED;
	sd_struct.tlm_dl_param.process_count = 0;

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief 保存したテレメトリを取り出してreplay_tlmにする
 */
CCP_CmdRet Cmd_CDH1_SD_Generate_RPTLM(const CommonCmdPacket* packet){
	uint8_t res = 0;
	uint16_t len;
	uint8_t data[512];
	SD_SAVE_Info* p_sd;

	//エラーor待ち状態
	if(sd_struct.tlm_dl_param.tlm_dl_ack != TLM_PROCEED){
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}

	print(SD, "TLM_ID = %d\r\n", sd_struct.tlm_dl_param.RP_TLM_CODE);
	print(SD, "RP_TLM_APID = %d\r\n", sd_struct.tlm_dl_param.RP_TLM_APID);
	print(SD, "RP_TLM_ROUTE = %d\r\n", sd_struct.tlm_dl_param.RP_TLM_ROUTE);
	print(SD, "RP_TLM_BLOCK = %08x\r\n", sd_struct.tlm_dl_param.RP_TLM_BLOCK);
	print(SD, "RP_TLM_process_cnt = %d\r\n", sd_struct.tlm_dl_param.process_count);

	//終了
	if(sd_struct.tlm_dl_param.RP_TLM_packet_num == sd_struct.tlm_dl_param.process_count){
		sd_struct.tlm_dl_param.tlm_dl_ack = TLM_WAIT;
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}

	//エラー
	if(sd_struct.tlm_dl_param.RP_TLM_CODE >= TF_MAX_TLMS){
		sd_struct.tlm_dl_param.tlm_dl_ack = TLM_ID_ERROR;
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}

	if(sd_struct.tlm_dl_param.RP_TLM_CODE < Tlm_CODE_HOGEHOGE74){
		p_sd = &(sd_struct.sdi_irregular[sd_struct.tlm_dl_param.RP_TLM_CODE].save_info);
	}
	else{
		p_sd = &(sd_struct.sdi_regular[sd_struct.tlm_dl_param.RP_TLM_CODE - Tlm_CODE_HOGEHOGE74].save_info);
	}

	//エラー
	if(sd_struct.tlm_dl_param.RP_TLM_BLOCK < p_sd->BLOCK_START || sd_struct.tlm_dl_param.RP_TLM_BLOCK > p_sd->BLOCK_END){
		printf("RP_TLM_BLOCK error\r\n");
		sd_struct.tlm_dl_param.tlm_dl_ack = TLM_BLOCK_ERROR;
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}

	res = BSP_SD_ReadBlocks((uint32_t*)data, sd_struct.tlm_dl_param.RP_TLM_BLOCK, 1, 0xFF);
	if(res != BSP_SD_OK){
		ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
		printf("read err\r\n");
	}

	ENDIAN_memcpy(&len, &(data[4]), 2);
	len += 7;									//packet_len + 6 + 1
	if(len < 15){
		sd_struct.tlm_dl_param.tlm_dl_ack = TLM_INVALID_PACKET;
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}

	uint8_t COUNT_MAX = 512 / len;
	uint8_t start = sd_struct.tlm_dl_param.pos_in_block;

	if(sd_struct.tlm_dl_param.pos_in_block >= COUNT_MAX){
		sd_struct.tlm_dl_param.tlm_dl_ack = TLM_START_COUNT_ERROR;
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}

	//現在のブロックアドレスにダウンリンクすべきパケットは何個あるか
	uint8_t rest_packet_num = COUNT_MAX - start - 1;//自身は含まず
	uint8_t rest_dl_packet_num = rest_packet_num / sd_struct.tlm_dl_param.width + 1;
	uint8_t sign = 0;

	uint16_t rest_txpic_list = PL_count_inactive_nodes(&PH_txpic_tlm_list);

	for(uint8_t i = 0; i < rest_dl_packet_num; i++){
		if(rest_txpic_list <= sd_struct.tlm_dl_param.dump_num || PL_count_inactive_nodes(&PH_rp_tlm_list) <= sd_struct.tlm_dl_param.dump_num){
			sd_struct.tlm_dl_param.pos_in_block += sd_struct.tlm_dl_param.width * i;
			sign = 1;
			break;
		}
		memcpy(read_data.packet, &data[len * start], len);
		sd_struct.tlm_dl_param.tlm_dl_ack = SD_analyze_pzcket(&read_data);
		if(sd_struct.tlm_dl_param.tlm_dl_ack != TLM_PROCEED){
		    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
		}
		TCP_set_apid(&read_data, sd_struct.tlm_dl_param.RP_TLM_APID);
		TCP_set_route(&read_data, sd_struct.tlm_dl_param.RP_TLM_ROUTE);//sakaguchi 追加
		TCP_TLM_set_category(&read_data, 0x00);	//Replay Tlmに変更
		RP_TLM_set_count(&read_data, sd_struct.tlm_dl_param.process_count);
		for(int j= 0; j < sd_struct.tlm_dl_param.dump_num; j++){
			PH_analyze_packet(&read_data);
		}
		sd_struct.tlm_dl_param.process_count++;
		start += sd_struct.tlm_dl_param.width;
		rest_txpic_list -= sd_struct.tlm_dl_param.dump_num;
		if(sd_struct.tlm_dl_param.RP_TLM_packet_num == sd_struct.tlm_dl_param.process_count){
		    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
		}
	}

	if(sign == 1){
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}
	else{//次のブロックとポジションを探す
		sd_struct.tlm_dl_param.pos_in_block = start - sd_struct.tlm_dl_param.width;
		rest_packet_num = COUNT_MAX - sd_struct.tlm_dl_param.pos_in_block - 1;
		uint32_t next_block = (sd_struct.tlm_dl_param.width - rest_packet_num) / COUNT_MAX;
		uint32_t pos = (sd_struct.tlm_dl_param.width - rest_packet_num) % COUNT_MAX;
		if(pos == 0){
			sd_struct.tlm_dl_param.RP_TLM_BLOCK += next_block;
			sd_struct.tlm_dl_param.pos_in_block = COUNT_MAX - 1;
		}
		else{
			sd_struct.tlm_dl_param.RP_TLM_BLOCK += (next_block + 1);
			sd_struct.tlm_dl_param.pos_in_block = pos - 1;
		}
		uint32_t temp = sd_struct.tlm_dl_param.RP_TLM_BLOCK - p_sd->BLOCK_END - 1;
		if(sd_struct.tlm_dl_param.RP_TLM_BLOCK > p_sd->BLOCK_END) sd_struct.tlm_dl_param.RP_TLM_BLOCK = p_sd->BLOCK_START + temp;
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_CDH1_SD_RPTLM_Pause(const CommonCmdPacket* packet){

	sd_struct.tlm_dl_param.tlm_dl_ack = TLM_PAUSE;

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_CDH1_SD_RPTLM_Resume(const CommonCmdPacket* packet){

	if(sd_struct.tlm_dl_param.tlm_dl_ack != TLM_PAUSE)return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

	if(sd_struct.tlm_dl_param.RP_TLM_CODE >= TF_MAX_TLMS){	//TLM_CODEが正しくないとき
		sd_struct.tlm_dl_param.RP_TLM_CODE = TF_MAX_TLMS;
		sd_struct.tlm_dl_param.RP_TLM_APID = AFSK_GS;
		sd_struct.tlm_dl_param.RP_TLM_ROUTE = TCP_ROUTE_DIRECT;
		sd_struct.tlm_dl_param.RP_TLM_packet_num = 0;
		sd_struct.tlm_dl_param.tlm_dl_ack = TLM_ID_ERROR;
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
	}
	if(sd_struct.tlm_dl_param.dump_num > 8){
		sd_struct.tlm_dl_param.tlm_dl_ack = TLM_DUMP_NUM_ERROR;
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
	}

	sd_struct.tlm_dl_param.tlm_dl_ack = TLM_PROCEED;

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}



CCP_CmdRet Cmd_TLM_CHANGE_FREQ(const CommonCmdPacket* packet){

	read_param(&tlm_flag, &tlm_flag_, sizeof(tlm_flag_), (uint32_t)0x00003840);

	//BCTableの該当する番号のところを丸ごと書き換え⇒IF文でタスクの登録がされるかどうかが決まる Noro
	overload_bctable(BC_TLM_EVERY,BC_ALT_TLM_EVERY);
	overload_bctable(BC_TLM_LOW_RATE_1,BC_ALT_TLM_LOW_RATE_1);
	overload_bctable(BC_TLM_LOW_RATE_2,BC_ALT_TLM_LOW_RATE_2);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
  * @brief  指定した1ブロックを読み込み
  * @param  uint32_t ブロックアドレス 4Byte
  */
CCP_CmdRet Cmd_CDH1_SD_BLOCK_READ(const CommonCmdPacket* packet){
	if(CCP_get_param_len(packet) == 4){
		uint8_t res = 0;

		const uint8_t* p_dataset = CCP_get_param_head(packet);
		uint32_t block_addr;
		ENDIAN_memcpy(&block_addr, p_dataset, 4);

		uint8_t receive_data[512];
		res = BSP_SD_ReadBlocks((uint32_t*)receive_data, block_addr, 1, 0xFFFF);
		if(res != BSP_SD_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
			printf("read err\r\n");
		}

		for(int j=0 ; j<512 ; j++){
			printf("%02x ", receive_data[j]);
		}
		printf("\r\n");
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}

	else {
		printf("wrong cmd length\r\n");
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
	}
}

/**
 * @brief SD保存状態をリセットする
 */
CCP_CmdRet Cmd_CDH1_SD_reset_all(const CommonCmdPacket* packet){
	uint8_t res = 0;

	uint8_t cond1[9 * Tlm_CODE_HOGEHOGE74];

	for(int i = 0; i < Tlm_CODE_HOGEHOGE74; i++){
		sd_struct.sdi_irregular[i].save_info.BLOCK_OLD = sd_struct.sdi_irregular[i].save_info.BLOCK_START;
		sd_struct.sdi_irregular[i].save_info.BLOCK_NOW = sd_struct.sdi_irregular[i].save_info.BLOCK_START;
		sd_struct.sdi_irregular[i].save_info.POSITION_COUNTER = 0;
		memcpy(&(cond1[9*i    ]), &(sd_struct.sdi_irregular[i].save_info.BLOCK_OLD), 	    4);
		memcpy(&(cond1[9*i + 4]), &(sd_struct.sdi_irregular[i].save_info.BLOCK_NOW), 		4);
		memcpy(&(cond1[9*i + 8]), &(sd_struct.sdi_irregular[i].save_info.POSITION_COUNTER), 4);
	}
	res = sd_specify_write(sd_struct.irregular_condition_block, 0, cond1, sizeof(cond1));
	if(res != BSP_SD_OK){
		ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
		printf("write err\r\n");
	}

	uint8_t cond2[8 * (TF_MAX_TLMS - Tlm_CODE_HOGEHOGE74)];

	for(int i = 0; i < (TF_MAX_TLMS - Tlm_CODE_HOGEHOGE74); i++){
		sd_struct.sdi_regular[i].save_info.BLOCK_OLD = sd_struct.sdi_regular[i].save_info.BLOCK_START;
		sd_struct.sdi_regular[i].save_info.BLOCK_NOW = sd_struct.sdi_regular[i].save_info.BLOCK_START;
		memcpy(&(cond2[8*i    ]), &(sd_struct.sdi_regular[i].save_info.BLOCK_OLD), 4);
		memcpy(&(cond2[8*i + 4]), &(sd_struct.sdi_regular[i].save_info.BLOCK_NOW), 4);
	}
	res = sd_specify_write(sd_struct.regular_condition_block, 0, cond2, sizeof(cond2));
	if(res != BSP_SD_OK){
		ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
		printf("write err\r\n");
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief	write command to SD.
 * packet[15]:      mem-dump tlm D\L flag
 * packet[16~19]: 	write address
 * packet[20~]: 	data to write
 *
 */
CCP_CmdRet Cmd_MEM_SD_writeParamData(const CommonCmdPacket* packet)
{
	size_t param_len;
	print(SD, "SD write command\r\n");

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

	SD_write_param(writedata, NULL, DataSize, Address);

//	mram_class.WriteGenerarl(writedata, DataSize,Address);	三十冗長試験用

	print(SD, "writeaddress: %08x", Address);
	print(SD, "writedata: ");
	for(int i = 0; i<DataSize; ++i){
        print(SD, "%02x ",writedata[i]);
    }
	print(SD,"\r\n");

	APP_TMD_SD3_SET_BEGIN_ADR(Address);
	TMD_SD3_DUMP();

	if(tlm_dl_flag){
		uint8_t param[6];
		TF_copy_u16(&param[0], (uint16_t)(p_gs_response_struct->res_apid));
		TF_copy_u8(&param[2], (uint8_t)(p_gs_response_struct->res_route));
		TF_copy_u8(&param[3], 0x40);
		TF_copy_u8(&param[4], (uint8_t)Tlm_CODE_TLM_MEMDUMP_SD3);
		TF_copy_u8(&param[5], (uint8_t)(1));
		CCP_form_rtc(&p_gs_response_struct->tcp,(CMD_CODE)Cmd_CODE_GENERATE_TLM_USER, &param[0], 6);
		TCP_set_apid_(&p_gs_response_struct->tcp, MY_SAT_ID, TCP_EXE_ID_MOBC);
		PH_analyze_packet(&p_gs_response_struct->tcp);
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*
 * @brief	read command from SD. 0x01 0x0c 0x00 0xaa 0xaa 0x0a
 * param[0]~param[3]:	readaddress (BIG Endian)
 * param[4]: 			datasize
 *
 */
CCP_CmdRet Cmd_MEM_SD_ReadParamData(const CommonCmdPacket* packet)
{

	size_t param_len;

	print(SD,"SD read command\r\n");
	 if ((param_len = CCP_get_param_len(packet)) != 8) //その場しのぎだがpacketlengthを足した
	{
		// パラメータはブロック番号1Byte。
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
	}

	 uint32_t DataSize;
	 uint32_t Address;
	 ENDIAN_memcpy(&Address, CCP_get_param_head(packet), 4);
	 ENDIAN_memcpy(&DataSize, CCP_get_param_head(packet) + 4, 4);

	 uint8_t readbuf[DataSize];
	 SD_read_param(readbuf, NULL, DataSize, Address);
	 print(SD, "ReadData: ");
	 for(int i=0; i<DataSize; ++i){
		print(SD, "%02x ",readbuf[i]);
	 }
	 print(SD, "\r\n");

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief	write command to SD.
 * packet[15]:      mem-dump tlm D\L flag
 * packet[16~19]: 	write address
 * packet[20~]: 	data to write
 *
 */
CCP_CmdRet Cmd_MEM_SD_writeProgramData(const CommonCmdPacket* packet)
{
	size_t param_len;
	print(SD, "SD write command\r\n");

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

	SD_write_program(writedata, NULL, DataSize, Address);

//	mram_class.WriteGenerarl(writedata, DataSize,Address);	三十冗長試験用

	print(SD, "writeaddress: %08x", Address);
	print(SD, "writedata: ");
	for(int i = 0; i<DataSize; ++i){
        print(SD, "%02x ",writedata[i]);
    }
	print(SD,"\r\n");

	APP_TMD_SD3_SET_BEGIN_ADR(Address);
	TMD_SD3_DUMP();


	if(tlm_dl_flag){
		uint8_t param[6];
		TF_copy_u16(&param[0], (uint16_t)(p_gs_response_struct->res_apid));
		TF_copy_u8(&param[2], (uint8_t)(p_gs_response_struct->res_route));
		TF_copy_u8(&param[3], 0x40);
		TF_copy_u8(&param[4], (uint8_t)Tlm_CODE_TLM_MEMDUMP_SD3);
		TF_copy_u8(&param[5], (uint8_t)(1));
		CCP_form_rtc(&p_gs_response_struct->tcp,(CMD_CODE)Cmd_CODE_GENERATE_TLM_USER, &param[0], 6);
		TCP_set_apid_(&p_gs_response_struct->tcp, MY_SAT_ID, TCP_EXE_ID_MOBC);
		PH_analyze_packet(&p_gs_response_struct->tcp);
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*
 * @brief	read command from SD. 0x01 0x0c 0x00 0xaa 0xaa 0x0a
 * param[0]~param[3]:	readaddress (BIG Endian)
 * param[4]: 			datasize
 *
 */
CCP_CmdRet Cmd_MEM_SD_ReadProgramData(const CommonCmdPacket* packet)
{
	size_t param_len;

	print(SD,"SD read command\r\n");
	 if ((param_len = CCP_get_param_len(packet)) != 8) //その場しのぎだがpacketlengthを足した
	{
		// パラメータはブロック番号1Byte。
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
	}

	 uint32_t DataSize;
	 uint32_t Address;
	 ENDIAN_memcpy(&Address, CCP_get_param_head(packet), 4);
	 ENDIAN_memcpy(&DataSize, CCP_get_param_head(packet) + 4, 4);

	 uint8_t readbuf[DataSize];
	 SD_read_program(readbuf, NULL, DataSize, Address);
	 print(SD, "ReadData: ");
	 for(int i=0; i<DataSize; ++i){
		print(SD, "%02x ",readbuf[i]);
	 }
	 print(SD, "\r\n");

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_SD_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet)
{
  uint8_t page;

  page = CCP_get_param_head(packet)[0];
  if (page >= SD_TLM_PAGE_MAX)
  {
    // ページ番号がコマンドテーブル範囲外
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  SD__tlm_page_no_ = page;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/*	@brief: パトロールアドレスの変更コマンド
 * 	packet[15~18]: ブロックアドレス（ビッグエンディアン）
 */
CCP_CmdRet Cmd_MEM_SD_set_patrol_block(const CommonCmdPacket* packet){
	uint32_t temp;
	//パラメータを受信
	ENDIAN_memcpy(&temp, CCP_get_param_head(packet), 4);
	//不正なアドレスを除去
	if(temp > 0x00000B71){			//0x00001313
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	//正常なアドレスであれば変更を適用する
	sd_struct.SDPatrolBlock = temp;

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_MEM_SD_command_read(const CommonCmdPacket* packet){
	uint16_t command_pos;
	uint8_t ret;
	TCP command;
	uint16_t cmd_len;

	ENDIAN_memcpy(&command_pos, CCP_get_param_head(packet), 2);

	ret = sd_read_address(COMMAND_SAVE_SECTION, command_pos * TCP_MAX_LEN, command.packet, TCP_MAX_LEN);

	if(ret != 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

	cmd_len = TCP_get_packet_len(&command);

	if(TCP_get_ver(&command) != TCP_VER_2) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
	if(TCP_get_type(&command) != TCP_TYPE_CMD) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
	if(TCP_get_apid_project_id(&command) != MAGNAROII) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
	if(TCP_get_2nd_hdr_flag(&command) != TCP_2ND_HDR_PRESENT) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
	if(cmd_len < TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
	if(cmd_len > TCP_MAX_LEN) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

	PH_analyze_packet(&command);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_MEM_SD_write(const CommonCmdPacket* packet){
	uint8_t ret = 0;
	uint32_t block;
	uint32_t address;
	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t data_len = CCP_get_param_len(packet) - 8;

	ENDIAN_memcpy(&block, param, 4);
	ENDIAN_memcpy(&address, param + 4, 4);

	if(block > SD_BLOCK_ADR_SIZE) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	ret = sd_write_address(block, address, param + 8, data_len);

	if(ret != 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

void sd_save_irregular_packet(SD_SAVE_Info* save_info, const TCP* stpkt){
	uint16_t len = TCP_get_packet_len(stpkt) + 6;
	uint8_t COUNT_MAX = 512 / len;
	uint8_t res;
	uint8_t data[512] = {0};

	if(save_info->ring_now == 1 && save_info->ring_permission == 1){
		return;
	}

	if(save_info->POSITION_COUNTER > COUNT_MAX){
		save_info->POSITION_COUNTER = 0;
		return;
	}
	else if(save_info->POSITION_COUNTER == 0){
		memcpy(data, stpkt->packet, len);
		res = BSP_SD_WriteBlocks((uint32_t*)data, save_info->BLOCK_NOW, 1, 0xFF);
		if(res != BSP_SD_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
			printf("write err\r\n");
		}
		save_info->POSITION_COUNTER++;
	}
	else{
		res = BSP_SD_ReadBlocks((uint32_t*)data, save_info->BLOCK_NOW, 1, 0xFF);
		memcpy(&data[len*save_info->POSITION_COUNTER], stpkt->packet, len);
		res = BSP_SD_WriteBlocks((uint32_t*)data, save_info->BLOCK_NOW, 1, 0xFF);
		if(res != BSP_SD_OK){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
			printf("write err\r\n");
		}
		save_info->POSITION_COUNTER++;
		if(save_info->POSITION_COUNTER == COUNT_MAX){
			save_info->POSITION_COUNTER = 0;
			save_info->BLOCK_NOW++;
			sd_block_analyze(save_info);
			if(save_info->BLOCK_NOW == save_info->BLOCK_OLD){//リング状態
				save_info->BLOCK_OLD++;
				sd_block_analyze(save_info);
			}
		}
	}
}

void sd_save_regular_packet(SD_Info_Regular* sdi, const TCP* stpkt){
	uint16_t len = TCP_get_packet_len(stpkt) + 6;
	uint8_t COUNT_MAX = 512 / len;
	uint8_t res;

	if(sdi->save_info.ring_now == 1 && sdi->save_info.ring_permission == 1){
		return;
	}

	if(sdi->save_info.POSITION_COUNTER >= COUNT_MAX){
		sdi->save_info.POSITION_COUNTER = 0;
		return;
	}
	else{
		memcpy(&(sdi->save_data_buffer[sdi->save_info.POSITION_COUNTER * len]), stpkt, len);
		sdi->save_info.POSITION_COUNTER++;
		if(sdi->save_info.POSITION_COUNTER == COUNT_MAX){
			res = BSP_SD_WriteBlocks((uint32_t*)sdi->save_data_buffer, sdi->save_info.BLOCK_NOW, 1, 0xFF);
			if(res != BSP_SD_OK){
				ANOMALY_SPI_Set_Status(SPI_FAULT_SD, res);
				printf("write err\r\n");
			}
			sdi->save_info.POSITION_COUNTER = 0;
			sdi->save_info.BLOCK_NOW++;
			sd_block_analyze(&(sdi->save_info));
			if(sdi->save_info.BLOCK_NOW == sdi->save_info.BLOCK_OLD){//リング状態
				sdi->save_info.BLOCK_OLD++;
				sd_block_analyze(&(sdi->save_info));
			}
			save_count++;
		}
	}
}

void RP_TLM_set_count(TCP* tcp, uint32_t count){
	if(count == 0){
		TCP_set_seq_flag(tcp, TCP_SEQ_FIRST);
	}
	else if(count == sd_struct.tlm_dl_param.RP_TLM_packet_num - 1){
		TCP_set_seq_flag(tcp, TCP_SEQ_LAST);
	}
	else{
		TCP_set_seq_flag(tcp, TCP_SEQ_CONT);
	}

	TCP_set_seq_cnt(tcp, count);
}

TLM_DL_ack SD_analyze_pzcket(const TCP* packet){
	uint16_t len = TCP_get_packet_len(packet);
	if(TCP_get_ver(packet) != TCP_VER_2) return TLM_INVALID_PACKET;
	if(TCP_get_type(packet) != TCP_TYPE_TLM) return TLM_INVALID_PACKET;
	if(TCP_get_apid_project_id(packet) != MAGNAROII) return TLM_INVALID_PACKET;
	if(TCP_get_2nd_hdr_flag(packet) != TCP_2ND_HDR_PRESENT) return TLM_INVALID_PACKET;
	if(TCP_TLM_get_packet_id(packet) != sd_struct.tlm_dl_param.RP_TLM_CODE) return TLM_INVALID_PACKET;
	if(len < TCP_TLM_2ND_HDR_LEN) return TLM_INVALID_PACKET;
	if(len > TCP_MAX_LEN) return TLM_INVALID_PACKET;

	return TLM_PROCEED;
}

#ifdef ONE_U
//for debug
//* *      Author: Osaki
// * @brief　 debug function to see CAM Data.
// * packet[15] : SCAM->1, SCAM(thumbnail)->5, SCAM(input)->9,
// * packet[16] : k枚目(0<=k<=127)
//*/
CCP_CmdRet Cmd_SD_ReadForDebug(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);

	uint8_t ret_r = 2;
	int divide = 512 * 20;
	uint8_t Saved_Data[divide];

	size_t info_size = 512;
	uint8_t info_buf[info_size*2];
	uint32_t ImageSize = 0;
	uint32_t addr = 0;
	uint32_t SCAM_addr = Storage_Area[Tlm_CODE_SUBCAM*2]; //plan is 0x0000C100;
	uint32_t SCAMbmp_addr = SCAM_addr + 20;
	uint32_t SCAMscaled_addr = SCAMbmp_addr + 131072;
	uint32_t SCAM_BOA_addr = SCAMscaled_addr + 10240; //0x0002E914

	printf("SD_ReadForDebug\r\n");

	if(param[0]==1){
		printf("for SCAM\r\n");
		ret_r = sd_read_address(SCAM_addr + 1, 0x00000000, info_buf, info_size*2);
		if(ret_r != 0){
			printf("read err(ret_r)\r\n");
		}
		ImageSize = (info_buf[info_size + 4*param[1]] << 24) | (info_buf[info_size + 1 + 4*param[1]] << 16)
					| (info_buf[info_size + 2 + 4*param[1]] << 8) | info_buf[info_size + 3 + 4*param[1]];
		addr = (info_buf[4*param[1]] << 24) | (info_buf[1 + 4*param[1]] << 16)
					| (info_buf[2 + 4*param[1]] << 8) | info_buf[3 + 4*param[1]];
//		ImageSize = 480*320*2;
//		addr = 0x008c0400;
		int CAM_counter = ImageSize / divide;

		for (uint32_t k=0; k < CAM_counter + 1; k++){
			if (k < CAM_counter){
				ret_r = sd_read_address(SCAMbmp_addr, addr + divide*k,
						Saved_Data, divide);
				if(ret_r != 0){
					printf("read err(ret_r)\r\n");
				}
				for(int i=0; i<divide; i++){
					printf("%02x", Saved_Data[i]);
					WDT_clear_count();
				}
			}
			else if((k == CAM_counter)&&(ImageSize % divide != 0)){
				ret_r = sd_read_address(SCAMbmp_addr, addr + divide * k,
						Saved_Data, divide);
				if(ret_r != 0){
					printf("read err(ret_r)\r\n");
				}
				for(int i=0; i<(ImageSize % divide); i++){
					printf("%02x", Saved_Data[i]);
					WDT_clear_count();
				}
			}
		}
	}

	else if(param[0]==5){
		printf("for SCAM(thumbnail)\r\n");
		ret_r = sd_read_address(SCAM_addr + 3, 0x00000000, info_buf, info_size*2);
		if(ret_r != 0){
			printf("read err(ret_r)\r\n");
		}
		ImageSize = (info_buf[info_size + 4*param[1]] << 24) | (info_buf[info_size + 1 + 4*param[1]] << 16)
					| (info_buf[info_size + 2 + 4*param[1]] << 8) | info_buf[info_size + 3 + 4*param[1]];
		addr = (info_buf[4*param[1]] << 24) | (info_buf[1 + 4*param[1]] << 16)
					| (info_buf[2 + 4*param[1]] << 8) | info_buf[3 + 4*param[1]];
//		ImageSize = 64*48*2;
//		addr = 0x0002f800;
		int CAM_counter = ImageSize / divide;

		for (uint32_t k=0; k < CAM_counter + 1; k++){
			if (k < CAM_counter){
				ret_r = sd_read_address(SCAMscaled_addr, addr + divide*k,
						Saved_Data, divide);
				if(ret_r != 0){
					printf("read err(ret_r)\r\n");
				}
				for(int i=0; i<divide; i++){
					printf("%02x", Saved_Data[i]);
					WDT_clear_count();
				}
			}
			else if((k == CAM_counter)&&(ImageSize % divide != 0)){
				ret_r = sd_read_address(SCAMscaled_addr, addr + divide*k,
						Saved_Data, divide);
				if(ret_r != 0){
					printf("read err(ret_r)\r\n");
				}
				for(int i=0; i<(ImageSize % divide); i++){
					printf("%02x", Saved_Data[i]);
					WDT_clear_count();
				}
			}
		}
	}

	else if(param[0]==9){
		printf("for SCAM(input)\r\n");
		ret_r = sd_read_address(SCAM_addr + 5, 0x00000000, info_buf, info_size*2);
		if(ret_r != 0){
			printf("read err(ret_r)\r\n");
		}
		ImageSize = (info_buf[info_size + 4*param[1]] << 24) | (info_buf[info_size + 1 + 4*param[1]] << 16)
					| (info_buf[info_size + 2 + 4*param[1]] << 8) | info_buf[info_size + 3 + 4*param[1]];
		addr = (info_buf[4*param[1]] << 24) | (info_buf[1 + 4*param[1]] << 16)
					| (info_buf[2 + 4*param[1]] << 8) | info_buf[3 + 4*param[1]];

		int CAM_counter = ImageSize / divide;

		for (uint32_t k=0; k < CAM_counter + 1; k++){
			if (k < CAM_counter){
				ret_r = sd_read_address(SCAM_BOA_addr, addr + divide*k,
						Saved_Data, divide);
				if(ret_r != 0){
					printf("read err(ret_r)\r\n");
				}
				for(int i=0; i<divide; i++){
					printf("%02x", Saved_Data[i]);
					if (i%3000 == 0){
						WDT_clear_count();
					}
				}
			}
			else if((k == CAM_counter)&&(ImageSize % divide != 0)){
				ret_r = sd_read_address(SCAM_BOA_addr, addr + divide * k,
						Saved_Data, divide);
				if(ret_r != 0){
					printf("read err(ret_r)\r\n");
				}
				for(int i=0; i<(ImageSize % divide); i++){
					printf("%02x", Saved_Data[i]);
					if (i%3000 == 0){
						WDT_clear_count();
					}
				}
			}
		}
	}

	printf("\r\n");
	printf("Imagesize = %d ", ImageSize);
	printf("addr = %08x\r\n", addr);
	printf("Finish\r\n");
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
#endif/* for 1U*/

