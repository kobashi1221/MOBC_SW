/**
 * @file tlm_mem_dump.h
 * @brief  一般テレメでのmem dump
 *
 *         通常はMemoryDumpやFlashUtilityを使うべきだが，
 *         現在，WINGSでのdump系テレメが非対応なため，一般テレメでdumpするコマンドを提供する
 */
#ifndef TLM_MEM_DUMP_H_
#define TLM_MEM_DUMP_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/System/TimeManager/obc_time.h>

#define APP_TMD_DUMP_SIZE   (229)        //!< 1 tlmでのdumpサイズ
#define APP_TMD_DUMP3_SIZE (76)
#define APP_TMD_SD_DUMP_SIZE (128)

typedef struct
{
  ObcTime  dump_time;
  uint32_t begin_adr;                   //!< 開始アドレス
  // uint32_t end_adr;                     //!< 終了アドレス (exclusive)    一旦シンプルに実装するため，自動終了などは考えない
  uint8_t  dump[APP_TMD_DUMP_SIZE];
  uint8_t  is_all_adr_range_valid;      //!< 今回ダンプした領域が，すべて有効なアドレス範囲か？
  uint16_t dumped_size;                 //!< 今回ダンプしたサイズ
  uint8_t  dump_err;                    //!< 0: 正常，1: 何かしらのエラー
} APP_TMD_DumpInfo;

typedef struct
{
	ObcTime  dump_time;
	uint32_t begin_adr;                   //!< 開始アドレス
	// uint32_t end_adr;                     //!< 終了アドレス (exclusive)    一旦シンプルに実装するため，自動終了などは考えない
	uint8_t  dump[3][APP_TMD_DUMP3_SIZE];
	uint8_t  is_all_adr_range_valid;      //!< 今回ダンプした領域が，すべて有効なアドレス範囲か？
	uint16_t dumped_size;                 //!< 今回ダンプしたサイズ
	uint8_t  dump_err;                    //!< 0: 正常，1: 何かしらのエラー
}APP_TMD_Dump3Info;

typedef struct
{
  ObcTime  dump_time;
  uint32_t begin_block_adr;
  uint8_t part;
  // uint32_t end_adr;                     //!< 終了アドレス (exclusive)    一旦シンプルに実装するため，自動終了などは考えない
  uint8_t  dump[APP_TMD_SD_DUMP_SIZE];
  uint8_t  is_all_adr_range_valid;      //!< 今回ダンプした領域が，すべて有効なアドレス範囲か？
  uint16_t dumped_size;                 //!< 今回ダンプしたサイズ
  uint8_t  dump_err;                    //!< 0: 正常，1: 何かしらのエラー
} APP_TMD_DumpSDInfo;

typedef struct
{
  APP_TMD_DumpInfo ram;
  APP_TMD_DumpInfo flash;
  APP_TMD_DumpInfo eeprom;
  APP_TMD_DumpInfo mram;
  APP_TMD_DumpSDInfo sd;
  APP_TMD_DumpInfo spiprom;
  APP_TMD_Dump3Info eeprom3;
  APP_TMD_Dump3Info mram3;
  APP_TMD_Dump3Info sd3;
  APP_TMD_Dump3Info spiprom3;
} TlmMemDump;

extern const TlmMemDump* const tlm_mem_dump;

int APP_TMD_is_vaild_adr_in_ram_(uint32_t adr);

int APP_TMD_RAM_SET_BEGIN_ADR(uint32_t adr);
int TMD_RAM_DUMP(void);
int APP_TMD_FLASH_SET_BEGIN_ADR(uint32_t adr);
int TMD_FLASH_DUMP(void);
int APP_TMD_EEPROM_SET_BEGIN_ADR(uint32_t adr);
int TMD_EEPROM_DUMP(void);
int APP_TMD_MRAM_SET_BEGIN_ADR(uint32_t adr);
int TMD_MRAM_DUMP(void);
int APP_TMD_SD_SET_BEGIN_BLOCK_ADR(uint32_t adr);
int TMD_SD_DUMP(void);
int APP_TMD_SPIPROM_SET_BEGIN_ADR(uint32_t adr);
int TMD_SPIPROM_DUMP(void);
int APP_TMD_EEPROM3_SET_BEGIN_ADR(uint32_t adr);
int TMD_EEPROM3_DUMP(void);
int APP_TMD_MRAM3_SET_BEGIN_ADR(uint32_t adr);
int TMD_MRAM3_DUMP(void);
int APP_TMD_SD3_SET_BEGIN_ADR(uint32_t adr);
int TMD_SD3_DUMP(void);
int APP_TMD_SPIPROM3_SET_BEGIN_ADR(uint32_t adr);
int TMD_SPIPROM3_DUMP(void);

AppInfo APP_TMD_create_app(void);

CCP_CmdRet Cmd_APP_TMD_RAM_SET_BEGIN_ADR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TMD_RAM_SET_NEXT_BEGIN_ADR(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_APP_TMD_FLASH_SET_BEGIN_ADR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TMD_FLASH_SET_NEXT_BEGIN_ADR(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_APP_TMD_EEPROM_SET_BEGIN_ADR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TMD_MRAM_SET_BEGIN_ADR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TMD_SD_SET_BEGIN_BLOCK_ADR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TMD_SPIPROM_SET_BEGIN_ADR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TMD_EEPROM3_SET_BEGIN_ADR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TMD_MRAM3_SET_BEGIN_ADR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TMD_SD3_SET_BEGIN_ADR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TMD_SPIPROM3_SET_BEGIN_ADR(const CommonCmdPacket* packet);

#endif // TLM_MEM_DUMP_H_
