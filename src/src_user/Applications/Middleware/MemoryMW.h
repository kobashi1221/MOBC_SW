/*
 * MemoryMW.h
 *
 *  Created on: 2021/12/02
 *      Author: SAT37
 */

#ifndef SRC_USER_APPLICATIONS_MIDDLEWARE_MEMORYMW_H_
#define SRC_USER_APPLICATIONS_MIDDLEWARE_MEMORYMW_H_
#include <stdio.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>


#define BLOCKSIZE (128)

typedef enum{
	FLAG_PARAMROM_READ,
	FLAG_PARAMROM_WRITE_EEPROM,
	FLAG_PARAMROM_WRITE_MRAM,
	FLAG_PARAMROM_READ_FLASH,
}PARAMROM_MODE_FLAG;

extern const uint8_t* p_memory_mode;

#ifdef __cplusplus
 extern "C" {
#endif

void ParamMemory_Init(void* spi_port, void* cs_port, uint16_t cs_pin, void* i2c_port);
uint8_t get_mode_flag(uint8_t* flag, PARAMROM_MODE_FLAG num);
void generate_memory_repro_tlm(void);


uint8_t write_param(void* WriteData,
						const void* WriteData_,
							size_t Size,
								uint32_t WriteAddress);

uint8_t read_param(void* ReadDataBuf,
						const void* ReadDataBuf_,
							size_t Size,
								uint32_t ReadAddress);

uint8_t MRAM_write_param(void* WriteData,
						const void* WriteData_,
							size_t Size,
								uint32_t WriteAddress);

uint8_t MRAM_read_param(void* ReadDataBuf,
						const void* ReadDataBuf_,
							size_t Size,
								uint32_t ReadAddress);

uint8_t EEPROM_write_param(void* WriteData,
						const void* WriteData_,
							size_t Size,
								uint32_t WriteAddress);

uint8_t EEPROM_read_param(void* ReadDataBuf,
						const void* ReadDataBuf_,
							size_t Size,
								uint32_t ReadAddress);

CCP_CmdRet Cmd_MEM_mram_eeprom_mode_change(const CommonCmdPacket* packet);

uint8_t SD_write_param(void* WriteData,
						const void* WriteData_,
							size_t Size,
								uint32_t WriteAddress);

uint8_t SD_read_param(void* ReadDataBuf,
						const void* ReadDataBuf_,
							size_t Size,
								uint32_t ReadAddress);
uint8_t SD_write_program(void* WriteData,
						const void* WriteData_,
							size_t Size,
								uint32_t WriteAddress);

uint8_t SD_read_program(void* ReadDataBuf,
						const void* ReadDataBuf_,
							size_t Size,
								uint32_t ReadAddress);

uint8_t SPIPROM_write_param(void* WriteData,
								const void* WriteData_,
									size_t Size,
										uint32_t WriteAddress);

uint8_t SPIPROM_read_param(void* ReadDataBuf,
								const void* ReadDataBuf_,
									size_t Size,
										uint32_t ReadAddress);

#ifdef __cplusplus
 }
#endif


#endif /* SRC_USER_APPLICATIONS_MIDDLEWARE_MEMORYMW_H_ */
