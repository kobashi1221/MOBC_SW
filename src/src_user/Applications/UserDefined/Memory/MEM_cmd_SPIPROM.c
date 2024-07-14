/**
  *****************************************************************************************
  * @file    : MEM_cmd_SPIPROM.c
  * @author  : Yuta Sakagcuhi
  * @version : 1.0
  * @date    : 2021/10/29
  *****************************************************************************************
  */

#include "MEM_cmd_SPIPROM.h"
#include "../../../IfWrapper/HAL.h"

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <string.h> //for memset
#include <src_core/Library/endian.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Drivers/Cdh/SPIPROM/SPI_ROM.h>
#include <src_user/Applications/middleware/MemoryMW.h>
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include <src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.h>
#include <src_user/Applications/UserDefined/tlm_mem_dump.h>
#include <src_user/Applications/DriverInstances/command_response_handler.h>

#include "../../../TlmCmd/CCSDS/TCPacket.h"
#include "../../../../src_core/TlmCmd/packet_list.h"
#include "../../../TlmCmd/telemetry_definitions.h"

static PROM_STRUCT prom_struct;
const PROM_STRUCT*  p_prom_struct;

static Queue prom_queue;
const Queue* p_prom_queue = &prom_queue;

static Buffer receive_buf[PROM_MAX_PACKET_NUM];

static void CDH1_spiprom_power_on_init(void);
static void CDH1_spiprom_exe(void);

static void CDH1_spiprom_check_ready_operation(void);

static void CDH1_spiprom_load_param(void);
static void CDH1_spiprom_initialize_param(void);

static void CDH1_spiprom_queue_save_exe(void);

static void CDH1_spiprom_condition_save_exe(void);

static void CDH1_spiprom_operation_exe(void);

static const int spiprom_save_limit = 10;
static int prom_save_count = 0;
static int operation;
static int all_reset_counter;

TCP_APID RP_TLM_APID_PROM = AFSK_GS;
TLM_CODE RP_TLM_CODE_PROM = TF_MAX_TLMS;
uint32_t RP_TLM_SECTOR = 0;
uint8_t RP_TLM_FIRST_PACKET = 0;
uint8_t RP_TLM_NUM_PACKET = 0;

#define MAX_IN_SECTOR (8)
static TCP read_data[MAX_IN_SECTOR];

static uint32_t ConditionSector;
const uint32_t ConditionSector_ = 0x00000314;

static uint32_t Storage_Sector_Area[78];
const uint32_t Storage_Sector_Area_[78] =
									   {0x00000316, 0x00000322,  //MOBC
										0x00000323, 0x0000032F,  //TL
										0x00000330, 0x0000033C,  //BL
										0x0000033D, 0x00000349,  //CA
										0x0000034A, 0x00000356,  //TF
										0x00000357, 0x00000363,  //MM
										0x00000364, 0x00000370,  //AM
										0x00000371, 0x0000037D,  //APP_TIME
										0x0000037E, 0x00000396,  //AL
										0x00000397, 0x000003AF,  //AH
										0x000003B0, 0x000003C8,  //AH_RES
										0x000003C9, 0x000003D5,  //RXPIC_AH1
										0x000003D6, 0x000003E2,  //RXPIC_AH2
										0x000003E3, 0x000003EF,  //TXPIC_AH1
										0x000003F0, 0x000003FC,  //TXPIC_AH2
										0x000003FD, 0x00000477,  //SUBCAM
										0x00000478, 0x00000484,  //SD
										0x00000485, 0x00000491,  //PROM
										0x00000492, 0x0000049E,  //TI
										0x0000049F, 0x000004DD,  //HK
										0x000004DE, 0x000004E5,  //SHK
										0x000004E6, 0x000004F4,  //PWR_MOBC
										0x000004F5, 0x0000050D,  //THERMAL
										0x0000050E, 0x000005CB,  //SENSOR
										0x000005CC, 0x00000609,  //SEPARATION
										0x0000060A, 0x000006C1,  //ADS
										0x000006C2, 0x000006FF,  //AKF1
										0x00000700, 0x0000073D,  //AKF2
										0x0000073E, 0x000007F9,  //ACS
										0x000007FA, 0x000008B2,  //ODS
										0x000008B3, 0x000008F0,  //OKF
										0x000008F1, 0x000009AC,  //OCS
										0x000009AD, 0x000009C5,  //RXPIC_SI
										0x000009C6, 0x000009DF,  //TXPIC_SI
										0x000009E0, 0x000009F9,  //INTERSAT
										0x000009FA, 0x00000A74,	 //SPIN
										0x00000A75, 0x00000AB2,	 //AOCS_80
										0x00000AB3, 0x00000AF0,	 //AOCS_160
										0x00000AF1, 0x00000B2E,	 //AOCS_240
										};

//SDの生成フラグ(tlm_flag)とは別にSPIPROMに保存する周期を決める
static uint8_t acquisition_cycle[39];
const uint8_t acquisition_cycle_[39] =
								{INSTANCE, INSTANCE, INSTANCE, INSTANCE, INSTANCE,	//MOBC, TL, BL, CA, TF
								 INSTANCE, INSTANCE, INSTANCE, INSTANCE, INSTANCE,	//MM, AM, APP_TI, AL
								 INSTANCE, INSTANCE, INSTANCE, INSTANCE, INSTANCE,	//AH_RES, RX_AH1, RX_AH2, TX_AH1, TX_AH2
								 INSTANCE, INSTANCE, INSTANCE, CYCLE_10, INSTANCE,	//SCAM, SD, PROM, TI, HK
								 CYCLE_20, CYCLE_10, CYCLE_10, CYCLE_10, CYCLE_10,	//SHK, P_MOBC, THERMAL, SENSOR, SEP
								 CYCLE_40, CYCLE_10, CYCLE_20, CYCLE_40, CYCLE_40,	//ADS, AKF1, AKF2, ACS, ODS
								 CYCLE_10, CYCLE_40, CYCLE_20, CYCLE_20, CYCLE_20,	//OKF, OCS, RX_SI, TX_SI, ISAT
								 CYCLE_10, CYCLE_40, CYCLE_40, CYCLE_40				//SPIN, AOCS80, AOCS160, AOCS240
};

static uint8_t p_rom_enable;
const uint8_t p_rom_enable_ = P_ROM_ENABLE;


/**
 * @brief アプリケーション作成関数
 *
 * @note 
 * @return アプリケーション構造体
 */

AppInfo CDH1_spiprom_param_update(void)
{
	return AI_create_app_info("SPIPROM_PARAM_UPDATE", CDH1_spiprom_load_param, CDH1_spiprom_initialize_param);
}

AppInfo CDH1_spiprom_update(void)
{
	return AI_create_app_info("SPIPROM_UPDATE", CDH1_spiprom_power_on_init, CDH1_spiprom_exe);
}

AppInfo CDH1_spiprom_queue_save_update(void)
{
	return AI_create_app_info("SPIPROM_QUEUE_SAVE", NULL, CDH1_spiprom_queue_save_exe);
}

AppInfo CDH1_spiprom_condition_save(void)
{
	return AI_create_app_info("SPIPROM_CONDITION_SAVE", NULL, CDH1_spiprom_condition_save_exe);
}

AppInfo CDH1_spiprom_operation_update(void)
{
	return AI_create_app_info("SPIPROM_CONDITION_RESET", NULL, CDH1_spiprom_operation_exe);
}

AppInfo CDH1_spiprom_check_ready(void)
{
    return AI_create_app_info("SPIPROM_CHECK_OPERATION", NULL, CDH1_spiprom_check_ready_operation);
}

/**
 * @brief spipromのパラメータロード関数
 */
static void CDH1_spiprom_load_param(void)
{
	read_param(&ConditionSector, &ConditionSector_, 4, (uint32_t)0x00003D00);
	read_param(&Storage_Sector_Area, &Storage_Sector_Area_, sizeof(Storage_Sector_Area), (uint32_t)0x00003D04);
	read_param(&acquisition_cycle, &acquisition_cycle_, sizeof(acquisition_cycle), (uint32_t)0x00003E3C);
	read_param(&p_rom_enable, &p_rom_enable_, sizeof(p_rom_enable), (uint32_t)0x00003E63);
}

/**
 * @brief spipromのパラメータ初期化関数
 */
static void CDH1_spiprom_initialize_param(void)
{
	write_param(&ConditionSector, &ConditionSector_, 4, (uint32_t)0x00003D00);
	write_param(&Storage_Sector_Area, &Storage_Sector_Area_, sizeof(Storage_Sector_Area), (uint32_t)0x00003D04);
	write_param(&acquisition_cycle, &acquisition_cycle_, sizeof(acquisition_cycle), (uint32_t)0x00003E3C);
	write_param(&p_rom_enable, &p_rom_enable_, sizeof(p_rom_enable), (uint32_t)0x00003E63);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void CDH1_spiprom_power_on_init(void)
{
	p_prom_struct = &prom_struct;

	int ret = 0;
	int counter = 0;
	operation = 0;
	prom_struct.PROMPatrolPage = 0x00000020;


	queue_init(&prom_queue, PROM_MAX_PACKET_NUM, receive_buf);

	initialized = false;
	ret = SPIROM_INIT();
	if(ret != true){
		printf("SPIPROM init Failed! \r\n");
	}

	uint32_t address = ConditionSector * is25lp.SectorSize;
	uint32_t next_address = (ConditionSector + 1) * is25lp.SectorSize;
	uint32_t codition_addr = ConditionSector * is25lp.SectorSize + START_POSITION;

	for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
		prom_struct.spiprom_info[i].SECTOR_START		 = Storage_Sector_Area[2 * i];
		prom_struct.spiprom_info[i].SECTOR_END		   	 = Storage_Sector_Area[2 * i +1];
	}
	for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
		prom_struct.spiprom_info[i].SECTOR_START		 = Storage_Sector_Area[2 * (i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI)];
		prom_struct.spiprom_info[i].SECTOR_END		   	 = Storage_Sector_Area[2 * (i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) + 1];
	}

	for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
		prom_struct.spiprom_info[i].ACQUISITION_CYCLE		 = acquisition_cycle[i];
	}
	for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
		prom_struct.spiprom_info[i].ACQUISITION_CYCLE		 = acquisition_cycle[i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI];
	}

	for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
		prom_struct.spiprom_info[i].COUNT		 = 0;
	}
	for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
		prom_struct.spiprom_info[i].COUNT		 = 0;
	}

	for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
		prom_struct.spiprom_info[i].SECOTR_POSITION		 = 0;
	}
	for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
		prom_struct.spiprom_info[i].SECOTR_POSITION		 = 0;
	}

	uint8_t count_data[TOLERANCE_NUM_DATA];
	readData(count_data, address, sizeof(count_data));
	for(int i = 0; i < sizeof(count_data); i++){
		if(count_data[i] == 0){
			++counter;
		}
	}

	if(counter == 0){//保存する前にデータが消された場合もう一つのセクターから読み出し
		readData(count_data, next_address, sizeof(count_data));
		for(int i = 0; i < sizeof(count_data); i++){
			if(count_data[i] == 0){
				++counter;
			}
		}
	}
	prom_struct.condition_couter = counter;

	uint8_t receive_data[156];
	ret = IS25LP_ReadBytes(receive_data, codition_addr + counter * sizeof(receive_data),
			sizeof(receive_data));
	if(ret != true){
		ANOMALY_SPI_Set_Status(SPI_FAULT_SPIPROM, ret);
		printf("read err\r\n");
	}

	for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
	    ENDIAN_memcpy(&(prom_struct.spiprom_info[i].SECTOR_OLD), 		&(receive_data[i * 4]), 2);
	    ENDIAN_memcpy(&(prom_struct.spiprom_info[i].SECTOR_NOW), 		&(receive_data[i * 4 + 2]), 2);
	}
	for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
	    ENDIAN_memcpy(&(prom_struct.spiprom_info[i].SECTOR_OLD), 		&(receive_data[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4]), 2);
	    ENDIAN_memcpy(&(prom_struct.spiprom_info[i].SECTOR_NOW), 		&(receive_data[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 2]), 2);
	}

	//初期化時は次のセクターから書き込みを行う→ここでセクターアドレスの設定をし直す
	//
	for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
		prom_struct.spiprom_info[i].SECTOR_OLD 			+= 1;
		prom_struct.spiprom_info[i].SECTOR_NOW			+= 1;
		spiprom_sector_analyze(&(prom_struct.spiprom_info[i]));
	}
	for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
		prom_struct.spiprom_info[i].SECTOR_OLD 			+= 1;
		prom_struct.spiprom_info[i].SECTOR_NOW			+= 1;
		spiprom_sector_analyze(&(prom_struct.spiprom_info[i]));
	}

	write_operation_flag(CONDITION_UPDATE);
	printf("SPIPROM init end\r\n");
}


/**
 * @brief テレメトリ保存
 *        フラグが立っているときテレメトリリストに入っていたら保存する フラグが立っていなくてもqueueに入れる
 *        毎周期取得するものは何周期かに一度保存する(保存しないときはスルー)
 */
static void CDH1_spiprom_exe(void)
{
	SPIPROM_SAVE_Info* p_prom;
	uint8_t res;
	int num_packet =PL_count_active_nodes(&PH_st_tlm_list);

	if(PL_is_empty(&PH_st_tlm_list)){
		print(SPIPROM,"PL is empty\r\n");
		return;
	}

	if(p_rom_enable == P_ROM_DISALE){
		return;
	}

	for(int i = 0; i<num_packet; i++){
		const TCP* tlm_pckt = (TCP*)PL_get_head(&PH_st_tlm_list)->packet;		//リストのheadにあるpacketをコピー
		uint8_t pckt_id = TCP_TLM_get_packet_id(tlm_pckt);
		if(pckt_id > TF_MAX_TLMS){				//範囲外のIDは何もしない
			printf("wrong ID\r\n");
			uint16_t Len = TCP_get_packet_len(tlm_pckt) + 6;
			for(int j=0; j<Len; j++){
				printf("%02x ", tlm_pckt->packet[j]);
			}
			printf("\r\n");
		}

		else{
			print(SPIPROM,"ID %d\r\n", TCP_TLM_get_packet_id(tlm_pckt));
			uint8_t legal_addr = 0;
			//PROMが保存するものかをチェック
			for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
				if(i == pckt_id){
					++legal_addr;
				}
			}
			for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
				if(i == pckt_id){
					++legal_addr;
				}
			}
			p_prom = &(prom_struct.spiprom_info[TCP_TLM_get_packet_id(tlm_pckt)]);
			if(p_prom->COUNT == p_prom->ACQUISITION_CYCLE && legal_addr != 0x00){//保存周期の時だけQueueに入れるか直接保存
				uint16_t len = TCP_get_packet_len(tlm_pckt) + TCP_PRM_HDR_LEN;
				if(len < 20){//長さが20以下のものは保存しない(バグ処理)
					return;
				}
				else{
					uint8_t SECTOR_COUNT_MAX = is25lp.SectorSize / len;
					enque_flag_check(spiprom_driver, &operation);//保存周期であるが何らかで直接保存ができないとき
					if(prom_save_count > spiprom_save_limit ||
							operation == SPIPROM_SUCCESS){//一周期に保存できるpacket数に達するか上書きしてる時
						enqueue(&(prom_queue.list), &tlm_pckt->packet[0]);//queueに入れ別の周期に保存
					}
					else{
						tlm_flag_check(spiprom_driver, &operation);
						if(operation == SPIPROM_SUCCESS){
							uint32_t write_address = p_prom->SECTOR_NOW * is25lp.SectorSize + p_prom->SECOTR_POSITION * len;
							++prom_save_count;
							++p_prom->SECOTR_POSITION;
							if(p_prom->SECOTR_POSITION >= SECTOR_COUNT_MAX){//1セクターで登録できるMAX
								p_prom->SECOTR_POSITION = 0;
								p_prom->SECTOR_NOW++;
								write_operation_flag(CONDITION_REWRITE);
								spiprom_sector_analyze(p_prom);
								spiprom_driver->condition_flag = 1;
							}
							uint8_t buffer[512];
							memcpy(buffer, tlm_pckt, len);
							res = spiprom_write_data(buffer, write_address, len);
							if(res != SPIPROM_SUCCESS){//何らかの理由で書き込みができなかったとき
								memset(spiprom_driver->buffer, 0, sizeof(spiprom_driver->buffer));
								for(int i = 0;i < START_POSITION; i++){
									spiprom_driver->buffer[i] = 0xff;
								}

								for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
									TF_copy_u16(&spiprom_driver->buffer[i * 4 + 32], (uint16_t)prom_struct.spiprom_info[i].SECTOR_OLD);
									TF_copy_u16(&spiprom_driver->buffer[i * 4 + 34], (uint16_t)prom_struct.spiprom_info[i].SECTOR_NOW);
								}
								for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
									TF_copy_u16(&spiprom_driver->buffer[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 32], (uint16_t)prom_struct.spiprom_info[i].SECTOR_OLD);
									TF_copy_u16(&spiprom_driver->buffer[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 34], (uint16_t)prom_struct.spiprom_info[i].SECTOR_NOW);
								}
								flag_set(&spiprom_driver->erase_flag);
								write_operation_flag(CONDITION_RESET);
								IS25LP_EraseSector_C2A(ConditionSector);
								printf("write err\r\n");
							}
							if(p_prom->SECTOR_NOW == p_prom->SECTOR_OLD){//リング状態
								p_prom->SECTOR_OLD++;
								spiprom_sector_analyze(p_prom);
							}
						}
					}
				}
				p_prom->COUNT = 0;
			}
			else if(legal_addr == 0){//何もしない

			}
			else{
				++p_prom->COUNT;
			}
		}
		//	一度消して一番後ろに入れ直す
		PL_drop_executed(&PH_st_tlm_list);
		PL_push_back(&PH_st_tlm_list, tlm_pckt);
	}
}

/**
 * @brief テレメトリ保存
 *        promのqueueに入っていたら保存する
 */
static void CDH1_spiprom_queue_save_exe(void)
{
	tlm_flag_check(spiprom_driver, &operation);
	if(operation == SPIPROM_SUCCESS){

		if(p_rom_enable == P_ROM_DISALE){
			return;
		}
		uint8_t ret = 0;
		SPIPROM_SAVE_Info* p_prom;
		TCP tcp;
		prom_save_count = 0x00;//countの初期化

		for(int i = 0; i< PROM_MAX_PACKET_NUM; i++){

			//packetが入ってないとき
			if(isEmpty(&(prom_queue.list))) return;
			//パケットを移してくる
			ret = (_ACK)dequeue(&(prom_queue.list), &tcp);
			print(SPIPROM,"ID %d\r\n", TCP_TLM_get_packet_id(&tcp));
			if(ret != _SUCCESS){
				//受信データがあるが何らかの異常が発生した場合は処理の打ち切り
				queue_reset(&(prom_queue.list));//queueの初期化
				return;
			}

			p_prom = &(prom_struct.spiprom_info[TCP_TLM_get_packet_id(&tcp)]);
			uint16_t len = TCP_get_packet_len(&tcp) + TCP_PRM_HDR_LEN;
			uint8_t SECTOR_COUNT_MAX = is25lp.SectorSize / len;

			if(prom_save_count > spiprom_save_limit ||
					spiprom_driver->erase_flag == FLAG_ON){//一周期に保存できるpacket数に達するか上書きしてる時
				enqueue(&(prom_queue.list), &tcp.packet[0]);//queueに入れ別の周期に保存
			}
			else{
				uint32_t write_address = p_prom->SECTOR_NOW * is25lp.SectorSize + p_prom->SECOTR_POSITION * len;
				++p_prom->SECOTR_POSITION;
				prom_save_count ++;
				if(p_prom->SECOTR_POSITION >= SECTOR_COUNT_MAX){//1セクターで登録できるMAX
					p_prom->SECOTR_POSITION = 0;
					p_prom->SECTOR_NOW++;
					write_operation_flag(CONDITION_REWRITE);
					spiprom_sector_analyze(p_prom);
					spiprom_driver->condition_flag = 1;
				}
				uint8_t buffer[512];
				memcpy(buffer, &tcp, len);
				ret = spiprom_write_data(buffer, write_address, len);
				if(ret != SPIPROM_SUCCESS){//何らかの理由で書き込みができなかったとき
					memset(spiprom_driver->buffer, 0, sizeof(spiprom_driver->buffer));
					for(int i = 0;i < START_POSITION; i++){
						spiprom_driver->buffer[i] = 0xff;
					}

					for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
						TF_copy_u16(&spiprom_driver->buffer[i * 4 + 32], (uint16_t)prom_struct.spiprom_info[i].SECTOR_OLD);
						TF_copy_u16(&spiprom_driver->buffer[i * 4 + 34], (uint16_t)prom_struct.spiprom_info[i].SECTOR_NOW);
					}
					for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
						TF_copy_u16(&spiprom_driver->buffer[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 32], (uint16_t)prom_struct.spiprom_info[i].SECTOR_OLD);
						TF_copy_u16(&spiprom_driver->buffer[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 34], (uint16_t)prom_struct.spiprom_info[i].SECTOR_NOW);
					}
					flag_set(&spiprom_driver->erase_flag);
					write_operation_flag(CONDITION_RESET);
					IS25LP_EraseSector_C2A(ConditionSector);
					printf("write err\r\n");
				}
				if(p_prom->SECTOR_NOW == p_prom->SECTOR_OLD){//リング状態
					p_prom->SECTOR_OLD++;
					spiprom_sector_analyze(p_prom);
				}
			}
		}
	}
}

static void CDH1_spiprom_condition_save_exe(void)
{
	condition_rewrite_flag_check(spiprom_driver, &operation);
	if(operation == SPIPROM_SUCCESS){
		uint8_t data[156] = {0};
		uint8_t res = 0;
		int counter = 0;
		uint8_t receive_buffer[TOLERANCE_NUM_DATA];

		uint32_t address = ConditionSector * is25lp.SectorSize;
		uint32_t next_address = (ConditionSector + 1) * is25lp.SectorSize;
		readData(receive_buffer, address, sizeof(receive_buffer));

		for(int i = 0; i < sizeof(receive_buffer); i++){
			if(receive_buffer[i] == 0){
				++counter;
			}
		}

		if(counter == 0){//保存する前にデータが消された場合もう一つのセクターから読み出し
			readData(receive_buffer, next_address, sizeof(receive_buffer));
			for(int i = 0; i < sizeof(receive_buffer); i++){
				if(receive_buffer[i] == 0){
					++counter;
				}
			}
		}
		prom_struct.condition_couter = counter;

		uint32_t codition_addr = ConditionSector * is25lp.SectorSize + START_POSITION;
		uint8_t write_data[1];
		write_data[0] = 0x00;
		spiprom_write_data(write_data, ConditionSector * is25lp.SectorSize + counter, 1);

		for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
			TF_copy_u16(&data[i * 4], (uint16_t)prom_struct.spiprom_info[i].SECTOR_OLD);
			TF_copy_u16(&data[i * 4 + 2], (uint16_t)prom_struct.spiprom_info[i].SECTOR_NOW);
		}
		for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
			TF_copy_u16(&data[(i + Tlm_CODE_PROM - Tlm_CODE_TI + 1) * 4], 	(uint16_t)prom_struct.spiprom_info[i].SECTOR_OLD);
			TF_copy_u16(&data[(i + Tlm_CODE_PROM - Tlm_CODE_TI + 1) * 4 + 2], (uint16_t)prom_struct.spiprom_info[i].SECTOR_NOW);
		}

		res = spiprom_write_data(&data[0], codition_addr + (counter + 1) * sizeof(data) ,sizeof(data));
		if(res != SPIPROM_SUCCESS){//何らかの理由で書き込みに失敗したとき
			memset(spiprom_driver->buffer, 0, sizeof(spiprom_driver->buffer));
			for(int i = 0;i < START_POSITION; i++){
				spiprom_driver->buffer[i] = 0xff;
			}

			for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
				TF_copy_u16(&spiprom_driver->buffer[i * 4 + 32], (uint16_t)prom_struct.spiprom_info[i].SECTOR_OLD);
				TF_copy_u16(&spiprom_driver->buffer[i * 4 + 34], (uint16_t)prom_struct.spiprom_info[i].SECTOR_NOW);
			}
			for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
				TF_copy_u16(&spiprom_driver->buffer[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 32], (uint16_t)prom_struct.spiprom_info[i].SECTOR_OLD);
				TF_copy_u16(&spiprom_driver->buffer[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 34], (uint16_t)prom_struct.spiprom_info[i].SECTOR_NOW);
			}
			flag_set(&spiprom_driver->erase_flag);
			write_operation_flag(CONDITION_RESET);
			IS25LP_EraseSector_C2A(ConditionSector);
			//ANOMALY_SPI_Set_Status(SPIPROM_FAILED, res);
			printf("write err\r\n");
		}
		write_operation_flag(NOMINAL);
		if(counter >= TOLERANCE_NUM_DATA - 2){
			counter = 0;
			write_operation_flag(CONDITION_UPDATE);
		}
	}
}

static void CDH1_spiprom_operation_exe(void)
{
	condition_reset_flag_check(spiprom_driver, &operation);
	if(operation == SPIPROM_SUCCESS){
		spiprom_write_data(&(spiprom_driver->buffer[0]), ConditionSector *is25lp.SectorSize, 188);//156+32
		CDH1_spiprom_power_on_init();
		write_operation_flag(NOMINAL);//仮に関数内でフラグが書き換わっても上書きする
		return;
	}

	condition_update_flag_check(spiprom_driver, &operation);
	if(operation == SPIPROM_SUCCESS){
		uint32_t address = ConditionSector * is25lp.SectorSize;
		uint32_t next_address = (ConditionSector + 1) * is25lp.SectorSize;
		if(spiprom_driver->update_step == 0){
			spiprom_copy_data(next_address, address, is25lp.SectorSize);
			IS25LP_EraseSector_C2A(ConditionSector);
			flag_set(&spiprom_driver->erase_flag);
			++spiprom_driver->update_step;
		}
		else if(spiprom_driver->update_step == 1){
			memset(spiprom_driver->buffer, 0, sizeof(spiprom_driver->buffer));
			for(int i = 0;i < START_POSITION; i++){
				spiprom_driver->buffer[i] = 0xff;
			}
			for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
				TF_copy_u16(&spiprom_driver->buffer[i * 4 + 32], (uint16_t)prom_struct.spiprom_info[i].SECTOR_OLD);
				TF_copy_u16(&spiprom_driver->buffer[i * 4 + 34], (uint16_t)prom_struct.spiprom_info[i].SECTOR_NOW);
			}
			for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
				TF_copy_u16(&spiprom_driver->buffer[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 32], (uint16_t)prom_struct.spiprom_info[i].SECTOR_OLD);
				TF_copy_u16(&spiprom_driver->buffer[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 34], (uint16_t)prom_struct.spiprom_info[i].SECTOR_NOW);
			}

			spiprom_write_data(&(spiprom_driver->buffer[0]), address, 188);
			IS25LP_EraseSector_C2A(ConditionSector + 1);
			flag_set(&spiprom_driver->erase_flag);
			spiprom_driver->update_step = 0;
			write_operation_flag(NOMINAL);
		}
		return;
	}

	copy_flag_check(spiprom_driver, &operation);
	if (operation == SPIPROM_SUCCESS){
		spiprom_write_buffer(spiprom_driver->operation_address);
		write_operation_flag(NOMINAL);
		return;
	}

	over_write_flag_check(spiprom_driver, &operation);
	if (operation == SPIPROM_SUCCESS){
		spiprom_overwrite_data(spiprom_driver->operation_address);
		write_operation_flag(NOMINAL);
		return;
	}

	param_write_flag_check(spiprom_driver, &operation);
	if (operation == SPIPROM_SUCCESS){
		if(spiprom_driver->redundancy_step == spiprom_driver->pre_redundancy_step){
			spiprom_write_paramdata(&(spiprom_driver->param_buffer[0]),spiprom_driver->redundacy_address, spiprom_driver->datasize);
		}
		else{
			spiprom_overwrite_data(spiprom_driver->redundacy_address);
		}
		if(spiprom_driver->redundancy_step == 3){
			spiprom_driver->redundancy_step = 0;
			spiprom_driver->pre_redundancy_step = 0;

			APP_TMD_SPIPROM3_SET_BEGIN_ADR(spiprom_driver->operation_address);
			TMD_SPIPROM3_DUMP();

			if(spiprom_driver->tlm_dl_flag){
				uint8_t param[6];
				TF_copy_u16(&param[0], (uint16_t)(p_gs_response_struct->res_apid));
				TF_copy_u8(&param[2], (uint8_t)(p_gs_response_struct->res_route));
				TF_copy_u8(&param[3], 0x40);
				TF_copy_u8(&param[4], (uint8_t)Tlm_CODE_TLM_MEMDUMP_SPIPROM3);
				TF_copy_u8(&param[5], (uint8_t)(1));
				CCP_form_rtc(&p_gs_response_struct->tcp,(CMD_CODE)Cmd_CODE_GENERATE_TLM_USER, &param[0], 6);
				TCP_set_apid_(&p_gs_response_struct->tcp, MY_SAT_ID, TCP_EXE_ID_MOBC);
				PH_analyze_packet(&p_gs_response_struct->tcp);
			}
			spiprom_driver->tlm_dl_flag = 0x00;
			write_operation_flag(NOMINAL);
		}
		return;
	}

	erace_sectors_flag_check(spiprom_driver, &operation);
	if (operation == SPIPROM_SUCCESS){
		--spiprom_driver->num_erasesector;
		IS25LP_EraseSector_C2A(spiprom_driver->operation_address);
		if(spiprom_driver->num_erasesector == 0){
			flag_reset(&spiprom_driver->erase_flag);
			write_operation_flag(NOMINAL);
		}
		else{
			++spiprom_driver->operation_address;
		}
		return;
	}

	all_reset_flag_check(spiprom_driver, &operation);
	if (operation == SPIPROM_SUCCESS){
		memset(spiprom_driver->buffer, 0, sizeof(spiprom_driver->buffer));
		for(int i = 0;i < START_POSITION; i++){
			spiprom_driver->buffer[i] = 0xff;
		}
		//初期位置に戻す
		for(int i = Tlm_CODE_MOBC; i <= Tlm_CODE_PROM; i++){
			TF_copy_u16(&spiprom_driver->buffer[i * 4 + 32], (uint32_t)prom_struct.spiprom_info[i].SECTOR_START);
			TF_copy_u16(&spiprom_driver->buffer[i * 4 + 34], (uint32_t)prom_struct.spiprom_info[i].SECTOR_START);
		}
		for(int i = Tlm_CODE_TI; i <= Tlm_CODE_AOCS240; i++){
			TF_copy_u16(&spiprom_driver->buffer[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 32], (uint16_t)prom_struct.spiprom_info[i].SECTOR_START);
			TF_copy_u16(&spiprom_driver->buffer[(i + Tlm_CODE_PROM + 1 - Tlm_CODE_TI) * 4 + 34], (uint16_t)prom_struct.spiprom_info[i].SECTOR_START);
		}
		write_operation_flag(CONDITION_RESET);
		return;
	}
}

/**
 * @brief write_enableかどうかを確認
 * @brief　all_resetコマンドが動かして100s以内か, SPIPROMのセクターを消去している最中なら書き込みを行わないようにする
 */
static void CDH1_spiprom_check_ready_operation(void)
{
	if(all_reset_counter !=0){
		--all_reset_counter;
		return;
	}
	if(IS25LP_WaitForWriteEnd()){
		flag_reset(&spiprom_driver->erase_flag);
	}
}

/**
 * @brief  write data for SPIPROM
 * packet[15]:      mem-dump tlm D\L flag
 * packet[16~19]: 	write address
 * packet[20~]: 	data to write
 */
CCP_CmdRet Cmd_MEM_spiprom_write_data(const CommonCmdPacket* packet){
	size_t param_len;
	uint8_t* param = (uint8_t*)CCP_get_param_head(packet);
	print(SPIPROM,"write data\r\n");

	if((param_len = CCP_get_param_len(packet)) < 6)
	{
		printf("illegal length \r\n");
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
	}
	uint8_t datasize = param_len - sizeof(is25lp.SectorSize) - sizeof(spiprom_driver->tlm_dl_flag );
	uint8_t writedata[datasize];
	uint32_t write_address = 0;

	spiprom_driver->tlm_dl_flag = param[0];
    ENDIAN_memcpy(&write_address, &param[1], sizeof(is25lp.SectorSize));
	memcpy(writedata, &param[5], datasize);

	if(spiprom_driver->erase_flag == 1){//他のデータを上書きしている間は書き込まない, データを構造体に入れる
		spiprom_driver->operation_address = write_address;
		spiprom_driver->datasize = datasize;
		memcpy(&spiprom_driver->buffer[0], &writedata, datasize);
		write_operation_flag(PARAM_WRITE);
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}
	flag_set(&spiprom_driver->erase_flag);
	SPIPROM_write_param(writedata, NULL, datasize, write_address);

//PROMは一度セクター消去(200ms)を挟むため一度に書き込みを行わない
//mem dumpは冗長部分の書き込みまで完了次第実行
//	APP_TMD_SPIPROM3_SET_BEGIN_ADR(write_address);
//	TMD_SPIPROM3_DUMP();
//
//	if(tlm_dl_flag){
//		uint8_t param[6];
//		TF_copy_u16(&param[0], (uint16_t)(p_gs_response_struct->res_apid));
//		TF_copy_u8(&param[2], (uint8_t)(p_gs_response_struct->res_route));
//		TF_copy_u8(&param[3], 0x40);
//		TF_copy_u8(&param[4], (uint8_t)Tlm_CODE_TLM_MEMDUMP_SPIPROM3);
//		TF_copy_u8(&param[5], (uint8_t)(1));
//		CCP_form_rtc(&p_gs_response_struct->tcp,(CMD_CODE)Cmd_CODE_GENERATE_TLM_USER, &param[0], 6);
//		TCP_set_apid_(&p_gs_response_struct->tcp, MY_SAT_ID, TCP_EXE_ID_MOBC);
//		PH_analyze_packet(&p_gs_response_struct->tcp);
//	}

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief  write data for SPIPROM
 * param[15~18]  write one sector address
 */
CCP_CmdRet Cmd_MEM_spiprom_write_buffer_data(const CommonCmdPacket* packet){
	size_t param_len;
	uint8_t* param = (uint8_t*)CCP_get_param_head(packet);
	print(SPIPROM,"write data\r\n");

	if((param_len = CCP_get_param_len(packet)) < 4)
	{
		printf("illegal length \r\n");
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
	}
	uint32_t write_address = 0;
	ENDIAN_memcpy(&write_address, param, sizeof(is25lp.SectorSize));
	spiprom_write_buffer(write_address);

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief   eraser sectors for SPIPROM
 *          ex)if you want to erase sectors which address from 0x00000010 to 0x00000030,
 *             you set user data "0x0000001020"
 * param[15~18]  erase first sector address
 * param[19~20]    num erase sector
 */
CCP_CmdRet Cmd_MEM_spiprom_erase_Sectors(const CommonCmdPacket* packet){
	uint8_t* param = (uint8_t*)CCP_get_param_head(packet);
	uint32_t erase_sectorAddress;
	spiprom_driver->num_erasesector = 0;

	ENDIAN_memcpy(&erase_sectorAddress, param, 4);
	ENDIAN_memcpy(&spiprom_driver->num_erasesector, param + 4, 2);

	if(erase_sectorAddress > is25lp.SectorSize){
		printf("illegal sector address \r\n");
	    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

	print(SPIPROM,"erease secoter address is %d \r\n", erase_sectorAddress);
	IS25LP_EraseSector_C2A(erase_sectorAddress);
	--spiprom_driver->num_erasesector;

	if(spiprom_driver->num_erasesector != 0){//消すsectorがまだあるならコマンドを再登録
		uint32_t next_address = erase_sectorAddress + 1;
		ENDIAN_memcpy(&spiprom_driver->operation_address, &next_address, sizeof(next_address));

		flag_set(&spiprom_driver->erase_flag);
		write_operation_flag(ELASE_SECTOR);
	}
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief   read data in one page
 * param[15~18]  sector address you want to read
 */
CCP_CmdRet Cmd_MEM_spiprom_read_page(const CommonCmdPacket* packet){
	uint8_t* param = (uint8_t*)CCP_get_param_head(packet);
	uint8_t ret;
	uint32_t read_pageAddress;
	uint8_t buffer[256];

	ENDIAN_memcpy(&read_pageAddress, param, 4);

	ret = readPageData(buffer, read_pageAddress);

	if(ret != SPIPROM_SUCCESS){
		ANOMALY_SPI_Set_Status(SPI_FAULT_SPIPROM, ret);
		printf("read err \r\n");
	}

	for(int i = 0; i<256; i++){
		printf("%02x", buffer[i]);
	}
	printf("\r\n");

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief   保存したテレメトリを取り出してreplay_tlmにする
 * param[15]     何のTLM(TLM_CODE)
 * param[16~17]	 apID
 * param[18~21]  どのセクターから
 * param[22]  (セクター内の)何番目のpacketから
 * param[23]　　いくつのpacket　(8packetまで)
 */
CCP_CmdRet Cmd_MEM_spiprom_Generate_RPTLM(const CommonCmdPacket* packet){
	uint32_t ret = 0;
	uint16_t len;
	uint8_t data[256];
	SPIPROM_SAVE_Info* p_prom;

	const uint8_t* param = CCP_get_param_head(packet);

	memcpy(&RP_TLM_CODE_PROM, param, 1);
	ENDIAN_memcpy(&RP_TLM_APID_PROM, param + 1, 2);
	ENDIAN_memcpy(&RP_TLM_SECTOR, param + 3, 4);
	memcpy(&RP_TLM_FIRST_PACKET, param + 7, 1);
	memcpy(&RP_TLM_NUM_PACKET, param + 8, 1);

	if(RP_TLM_CODE_PROM >= TF_MAX_TLMS || RP_TLM_NUM_PACKET == 0){
		RP_TLM_APID_PROM = AFSK_GS;
		RP_TLM_CODE_PROM = TF_MAX_TLMS;
		RP_TLM_SECTOR = 0;
		RP_TLM_FIRST_PACKET = 0;
		RP_TLM_NUM_PACKET = 0;
	    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

	p_prom = &(prom_struct.spiprom_info[RP_TLM_CODE_PROM]);
	if(RP_TLM_SECTOR < p_prom->SECTOR_START || RP_TLM_SECTOR > p_prom->SECTOR_END){
		printf("RP_TELM_SECTOR error\r\n");
		TCP_APID RP_TLM_APID_PROM = AFSK_GS;
		RP_TLM_CODE_PROM = TF_MAX_TLMS;
		RP_TLM_SECTOR = 0;
		RP_TLM_FIRST_PACKET = 0;
		RP_TLM_NUM_PACKET = 0;
	    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

	uint32_t first_page = RP_TLM_SECTOR * 16;

	ret = readPageData(data, first_page);
	if(ret != SPIPROM_SUCCESS){
		ANOMALY_SPI_Set_Status(SPI_FAULT_SPIPROM, ret);
		printf("read err");
	}
	ENDIAN_memcpy(&len, &(data[4]), 2);
	if(len < 10){//長さが10のTLM以下のものは送り出さない(バグ処理)
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
	}

	len += TCP_TLM_2ND_HDR_LEN;

	if(len > 256){
		printf("illegal_length\r\n");
		TCP_APID RP_TLM_APID_PROM = AFSK_GS;
		RP_TLM_CODE_PROM = TF_MAX_TLMS;
		RP_TLM_SECTOR = 0;
		RP_TLM_FIRST_PACKET = 0;
		RP_TLM_NUM_PACKET = 0;
	    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

	uint32_t packet_addr;
	for(int i = 0 ; i<RP_TLM_NUM_PACKET; i++){
		packet_addr = first_page * is25lp.PageSize + (i * RP_TLM_FIRST_PACKET)*len;
		ret = readData(data, packet_addr, len);
		if(ret != SPIPROM_SUCCESS){
			ANOMALY_SPI_Set_Status(SPI_FAULT_SPIPROM, ret);
			printf("read err");
		}
		memcpy(read_data[i].packet, &data, len);
		TCP_set_apid(&read_data[i], RP_TLM_APID_PROM);
		TCP_TLM_set_category(&read_data[i], 0x00);

		PH_analyze_packet(&read_data[i]);
	}
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/*	@brief: パトロールアドレスの変更コマンド
 * 	packet[15~18]: ブロックアドレス（ビッグエンディアン）
 */
CCP_CmdRet Cmd_MEM_SPIPROM_set_patrol_block(const CommonCmdPacket* packet){
	uint32_t temp;
	//パラメータを受信
	ENDIAN_memcpy(&temp, CCP_get_param_head(packet), 4);
	//不正なアドレスを除去
	if(temp > 0x00000130 && temp < 0x00000020){
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	//正常なアドレスであれば変更を適用する
	prom_struct.PROMPatrolPage = temp;

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief:	メモリーパトロールコマンド.
 */
CCP_CmdRet Cmd_MEM_SPIPROM_memory_patrol(const CommonCmdPacket* packet)
{
	tlm_flag_check(spiprom_driver, &operation);
	if(operation == SPIPROM_SUCCESS){
		uint8_t ret;
		uint8_t dummy[256];

		ret = SPIPROM_read_param(dummy, NULL, is25lp.PageSize, (uint32_t)(prom_struct.PROMPatrolPage * is25lp.PageSize));

		if(ret != HAL_OK){
			prom_struct.PROMPatrolPage = 0x00000020;
			return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
		}

		prom_struct.PROMPatrolPage++;
		if(prom_struct.PROMPatrolPage >= 0x00000130) prom_struct.PROMPatrolPage = 0x00000020;
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief   read data in one page
 * param[15~18]  password(ASCII で　SA を　16進数に変換)
 */
CCP_CmdRet Cmd_MEM_spiprom_ALL_Reset(const CommonCmdPacket* packet){
	uint8_t* param = (uint8_t*)CCP_get_param_head(packet);

	if(param[0] == 0x53 && param[1] == 0x41 && param[2] == 0x4b && param[3] == 0x41){
		IS25LP_EraseChip();
		flag_set(&spiprom_driver->erase_flag);
		write_operation_flag(ALL_RESET);
		all_reset_counter = 100;
	}
	else{
		printf("illegal password \r\n");
	    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
