/*
 * SPI_ROM.h
 *
 *  Created on: 2019/05/30
 *      Author: K.Okada
 */

#ifndef SPI_ROM_SPI_ROM_H_
#define SPI_ROM_SPI_ROM_H_

#include <src_core/System/TImeManager/time_manager.h>//for delay()
#include <src_user/Settings/System/flag_setting.h>
#include "IS25LP.h"
#include <src_user/TlmCmd/CCSDS/TCPacket.h>
#include <src_user/Settings/TlmCmd/telemetry_frame_params.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PROM_PARAM_OFFSET 0x00000000
#define PROM_FLIGHT_PROGRAM_OFFSET 0x00013000
#define PROM_Redundancy_OFFSET 0x00108000

typedef enum {
	SPIPROM_SUCCESS,
	SPIPROM_FAILED,
	SPIPROM_UNKNOWN,
} SPIPROM_ACK;

typedef enum {
	NOMINAL,
	CONDITION_RESET,
	CONDITION_REWRITE,
	CONDITION_UPDATE,
	TLM_WRITE,
	PARAM_WRITE,
	OVER_WRITE,
	COPY_WRITE,
	ELASE_SECTOR,
	ALL_RESET,
} SPIPROM_OPERATION;

typedef struct {
	uint8_t buffer[0x1000];
	uint8_t param_buffer[0x0100];
	uint8_t tlm_dl_flag;
	uint16_t num_erasesector;
	uint32_t operation_address;//書き込む場合はaddress, 消す場合はSector Addressで指定
	uint32_t redundacy_address;
	uint32_t datasize;

	int update_step;
	int pre_redundancy_step;
	int redundancy_step;
	int erase_flag;
	int condition_flag;//operation_flagが書き変わったとき
	int condition_copy_flag;//conditionをコピーするときに既存の関数を使いたい
	SPIPROM_OPERATION operation_flag;
} SPIPROM_STRUCT;

typedef struct {
	uint32_t SECTOR_START;
	uint32_t SECTOR_END;
	uint32_t SECTOR_NOW;
	uint32_t SECTOR_OLD;
	uint32_t SECOTR_POSITION;
	uint8_t ACQUISITION_CYCLE;
	uint8_t COUNT;
}SPIPROM_SAVE_Info;

typedef struct{
	 SPIPROM_SAVE_Info spiprom_info[TF_MAX_TLMS];
	 uint8_t condition_couter;
	 uint32_t PROMPatrolPage;
}PROM_STRUCT;

extern SPIPROM_STRUCT* const spiprom_driver;
extern const PROM_STRUCT* p_prom_struct;

extern bool initialized;

#define SPIPROM_PAGESIZE (256)
#define OVER_THE_PAGE (2)
#define IN_THE_PAGE (1)
#define OVER_THE_SECTOR (2)
#define IN_THE_SECTOR (1)

uint8_t SPIROM_INIT();
uint8_t init();

SPIPROM_ACK spiprom_write_data(uint8_t *pBuffer,uint32_t address, uint32_t size);
SPIPROM_ACK spiprom_overwrite_data(uint32_t addr);
SPIPROM_ACK spiprom_write_paramdata(uint8_t *pBuffer,uint32_t address, uint32_t size);
SPIPROM_ACK spiprom_copy_data(uint32_t addr_dst,uint32_t addr_src, uint32_t size);
SPIPROM_ACK readData(uint8_t* buffer, uint32_t address, uint16_t size);
SPIPROM_ACK readPageData(uint8_t* buffer, uint32_t page_address);
SPIPROM_ACK readSectorData(uint8_t* buffer, uint32_t sector_address);
SPIPROM_ACK writeData(uint8_t *pBuffer, uint32_t address, uint32_t size);
SPIPROM_ACK spiprom_write_buffer(uint32_t sector_address);

void writeMinSector(uint8_t *pBuffer, uint32_t address, uint32_t size);
void writeOneSector(uint8_t *pBuffer, uint32_t sector);
void eraseSector(uint32_t first_erasesector);
void count_erasesector(uint32_t firstsector, uint32_t numsector, uint32_t write_address, uint32_t write_size);
void choose_erase_or_direct(uint32_t isector, uint32_t write_address, uint32_t write_size);
void erase_sectors(uint32_t Erase_SectorAddr);
void spiprom_sector_analyze(SPIPROM_SAVE_Info* p_prom_struct);

SPIPROM_ACK condition_reset_flag_check(const SPIPROM_STRUCT* spiprom, int* ret);
SPIPROM_ACK condition_rewrite_flag_check(const SPIPROM_STRUCT* spiprom, int* ret);
SPIPROM_ACK condition_update_flag_check(const SPIPROM_STRUCT* spiprom, int* ret);
SPIPROM_ACK tlm_flag_check(const SPIPROM_STRUCT* spiprom, int* ret);
SPIPROM_ACK over_write_flag_check(const SPIPROM_STRUCT* spiprom, int* ret);
SPIPROM_ACK param_write_flag_check(const SPIPROM_STRUCT* spiprom, int* ret);
SPIPROM_ACK copy_flag_check(const SPIPROM_STRUCT* spiprom, int* ret);
SPIPROM_ACK all_reset_flag_check(const SPIPROM_STRUCT* spiprom, int* ret);
SPIPROM_ACK erace_sectors_flag_check(const SPIPROM_STRUCT* spiprom, int* ret);
SPIPROM_ACK enque_flag_check(const SPIPROM_STRUCT* spiprom, int* ret);

int get_operation_flag(const SPIPROM_STRUCT* spiprom);
void write_operation_flag(SPIPROM_OPERATION ret);

void check_flag(int* flag, int check_content);
void prom_flag_set(int* flag);
void prom_flag_reset(int* flag);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SPI_ROM_SPI_ROM_H_ */
