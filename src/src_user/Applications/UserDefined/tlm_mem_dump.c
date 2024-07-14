
#pragma section REPRO
/**
 * @file tlm_mem_dump.c
 * @brief  一般テレメでのmem dump
 *
 *         通常はMemoryDumpやFlashUtilityを使うべきだが，
 *         現在，WINGSでのdump系テレメが非対応なため，一般テレメでdumpするコマンドを提供する
 */
#include "tlm_mem_dump.h"

#include <string.h> // for memcpy
#include <src_core/Library/endian.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

static void APP_TMD_init_(void);
static int  APP_TMD_is_vaild_adr_in_flash_(uint32_t adr);

static int APP_TMD_is_vaild_adr_in_eeprom_(uint32_t adr);
static int APP_TMD_is_vaild_adr_in_mram_(uint32_t adr);
static int APP_TMD_is_vaild_adr_in_sd_(uint32_t block_adr);
static int APP_TMD_is_vaild_adr_in_spiprom_(uint32_t adr);
static int APP_TMD_is_vaild_adr_in_eeprom3_(uint32_t adr);
static int APP_TMD_is_vaild_adr_in_mram3_(uint32_t adr);
static int APP_TMD_is_vaild_adr_in_sd3_(uint32_t adr);
static int APP_TMD_is_vaild_adr_in_spiprom3_(uint32_t adr);

static TlmMemDump tlm_mem_dump_;
const TlmMemDump* const tlm_mem_dump = &tlm_mem_dump_;


AppInfo APP_TMD_create_app(void)
{
  return AI_create_app_info("tlm_mem_dump", APP_TMD_init_, NULL);
}


static void APP_TMD_init_(void)
{
  tlm_mem_dump_.ram.dump_time = TMGR_get_master_clock();
  tlm_mem_dump_.ram.begin_adr = RAM_BEGIN_ADR;
  tlm_mem_dump_.ram.is_all_adr_range_valid = 1;
  tlm_mem_dump_.ram.dumped_size = 0;
  tlm_mem_dump_.ram.dump_err = 0;

  tlm_mem_dump_.flash.dump_time = TMGR_get_master_clock();
  tlm_mem_dump_.flash.begin_adr = FLASH_BEGIN_ADR;
  tlm_mem_dump_.flash.is_all_adr_range_valid = 1;
  tlm_mem_dump_.flash.dumped_size = 0;
  tlm_mem_dump_.flash.dump_err = 0;

  tlm_mem_dump_.eeprom.dump_time = TMGR_get_master_clock();
  tlm_mem_dump_.eeprom.begin_adr = 0;
  tlm_mem_dump_.eeprom.is_all_adr_range_valid = 1;
  tlm_mem_dump_.eeprom.dumped_size = 0;
  tlm_mem_dump_.eeprom.dump_err = 0;

  tlm_mem_dump_.mram.dump_time = TMGR_get_master_clock();
  tlm_mem_dump_.mram.begin_adr = 0;
  tlm_mem_dump_.mram.is_all_adr_range_valid = 1;
  tlm_mem_dump_.mram.dumped_size = 0;
  tlm_mem_dump_.mram.dump_err = 0;

  tlm_mem_dump_.sd.dump_time = TMGR_get_master_clock();
  tlm_mem_dump_.sd.begin_block_adr = 0;
  tlm_mem_dump_.sd.part = 0;
  tlm_mem_dump_.sd.is_all_adr_range_valid = 1;
  tlm_mem_dump_.sd.dumped_size = 0;
  tlm_mem_dump_.sd.dump_err = 0;

  tlm_mem_dump_.spiprom.dump_time = TMGR_get_master_clock();
  tlm_mem_dump_.spiprom.begin_adr = 0;
  tlm_mem_dump_.spiprom.is_all_adr_range_valid = 1;
  tlm_mem_dump_.spiprom.dumped_size = 0;
  tlm_mem_dump_.spiprom.dump_err = 0;

  tlm_mem_dump_.eeprom3.dump_time = TMGR_get_master_clock();
  tlm_mem_dump_.eeprom3.begin_adr = 0;
  tlm_mem_dump_.eeprom3.is_all_adr_range_valid = 1;
  tlm_mem_dump_.eeprom3.dumped_size = 0;
  tlm_mem_dump_.eeprom3.dump_err = 0;

  tlm_mem_dump_.mram3.dump_time = TMGR_get_master_clock();
  tlm_mem_dump_.mram3.begin_adr = 0;
  tlm_mem_dump_.mram3.is_all_adr_range_valid = 1;
  tlm_mem_dump_.mram3.dumped_size = 0;
  tlm_mem_dump_.mram3.dump_err = 0;

  tlm_mem_dump_.sd3.dump_time = TMGR_get_master_clock();
  tlm_mem_dump_.sd3.begin_adr = 0;
  tlm_mem_dump_.sd3.is_all_adr_range_valid = 1;
  tlm_mem_dump_.sd3.dumped_size = 0;
  tlm_mem_dump_.sd3.dump_err = 0;

  tlm_mem_dump_.spiprom3.dump_time = TMGR_get_master_clock();
  tlm_mem_dump_.spiprom3.begin_adr = 0;
  tlm_mem_dump_.spiprom3.is_all_adr_range_valid = 1;
  tlm_mem_dump_.spiprom3.dumped_size = 0;
  tlm_mem_dump_.spiprom3.dump_err = 0;
}


static int APP_TMD_is_vaild_adr_in_flash_(uint32_t adr)
{
  if (adr >= FLASH_BEGIN_ADR && adr < FLASH_END_ADR) return 1;   // flashアドレス範囲
  return 0;                         // flashアドレス範囲外
}


int APP_TMD_is_vaild_adr_in_ram_(uint32_t adr)
{
  // FIXME:
  // 非常時にP,Cセクションなどもdumpできるように，RAM以外のアドレス領域もOKとした
  // この値は仮の値

  if(adr >= RAM_BEGIN_ADR && adr < RAM_END_ADR) return 1;
  if(adr >= SRAM_BEGIN_ADR && adr < SRAM_END_ADR) return 1;
  if(adr >= CCMRAM_BEGIN_ADR && adr < CCMARAM_END_ADR) return 1;

  return 0;
}

static int APP_TMD_is_vaild_adr_in_eeprom_(uint32_t adr)
{
  if (adr < EEPRAM_SIZE) return 1;   // flashアドレス範囲
  return 0;                         // flashアドレス範囲外
}

static int APP_TMD_is_vaild_adr_in_mram_(uint32_t adr)
{
  if (adr < MRAM_SIZE) return 1;   // flashアドレス範囲
  return 0;                         // flashアドレス範囲外
}

static int APP_TMD_is_vaild_adr_in_sd_(uint32_t block_adr)
{
  if (block_adr < SD_BLOCK_ADR_SIZE) return 1;   // flashアドレス範囲
  return 0;                         // flashアドレス範囲外
}

static int APP_TMD_is_vaild_adr_in_spiprom_(uint32_t adr)
{
  if (adr < SPIPROM_SIZE) return 1;   // flashアドレス範囲
  return 0;                         // flashアドレス範囲外
}

static int APP_TMD_is_vaild_adr_in_eeprom3_(uint32_t adr)
{
  if (adr < EEPROM_REDUNDANCY_SIZE) return 1;   // flashアドレス範囲
  return 0;                         // flashアドレス範囲外
}

static int APP_TMD_is_vaild_adr_in_mram3_(uint32_t adr)
{
  if (adr < MRAM_REDUNDANCY_SIZE) return 1;   // flashアドレス範囲
  return 0;                         // flashアドレス範囲外
}

static int APP_TMD_is_vaild_adr_in_sd3_(uint32_t adr)
{
  if (adr < SD_REDUNDANCY_SIZE) return 1;   // flashアドレス範囲
  return 0;                         // flashアドレス範囲外
}

static int APP_TMD_is_vaild_adr_in_spiprom3_(uint32_t adr)
{
  if (adr < SPIPROM_REDUNDANCY_SIZE) return 1;   // flashアドレス範囲
  return 0;                         // flashアドレス範囲外
}

int APP_TMD_RAM_SET_BEGIN_ADR(uint32_t adr){

	  if ( !(APP_TMD_is_vaild_adr_in_ram_(adr)) ){
		  return 1;
	  }

	  tlm_mem_dump_.ram.begin_adr = adr;
	  return 0;
}

int TMD_RAM_DUMP(void){
	  uint32_t i;
	  size_t dump_size = 0;

	  tlm_mem_dump_.ram.is_all_adr_range_valid = 1;

	  // 有効なアドレス範囲を探す
	  for (i = 0; i < APP_TMD_DUMP_SIZE; ++i)
	  {
	    uint32_t adr = tlm_mem_dump_.ram.begin_adr + i;

	    if ( !(APP_TMD_is_vaild_adr_in_ram_(adr)) )
	    {
	      tlm_mem_dump_.ram.is_all_adr_range_valid = 0;
	      break;
	    }

	    dump_size++;
	  }

	  if(dump_size != 0){
		  memcpy(tlm_mem_dump_.ram.dump, (unsigned char*)(tlm_mem_dump_.ram.begin_adr), dump_size);
		  tlm_mem_dump_.ram.dumped_size = (uint16_t)dump_size;
		  tlm_mem_dump_.ram.dump_err = 0;
		  tlm_mem_dump_.ram.dump_time = TMGR_get_master_clock();
	  }

	  return 0;
}

int APP_TMD_FLASH_SET_BEGIN_ADR(uint32_t adr){

	  if ( !(APP_TMD_is_vaild_adr_in_flash_(adr)) ) return 1;

	  tlm_mem_dump_.flash.begin_adr = adr;
	  return 0;
}

int TMD_FLASH_DUMP(void){
	  uint32_t i;
	  size_t dump_size = 0;

	  tlm_mem_dump_.flash.is_all_adr_range_valid = 1;

	  // 有効なアドレス範囲を探す
	  for (i = 0; i < APP_TMD_DUMP_SIZE; ++i)
	  {
	    uint32_t adr = tlm_mem_dump_.flash.begin_adr + i;

	    if ( !(APP_TMD_is_vaild_adr_in_flash_(adr)) )
	    {
	      tlm_mem_dump_.flash.is_all_adr_range_valid = 0;
	      break;
	    }

	    dump_size++;
	  }

	  if(dump_size != 0){
		  memcpy(tlm_mem_dump_.flash.dump, (unsigned char*)(tlm_mem_dump_.flash.begin_adr), dump_size);
		  tlm_mem_dump_.flash.dumped_size = (uint16_t)dump_size;
		  tlm_mem_dump_.flash.dump_err = 0;
		  tlm_mem_dump_.flash.dump_time = TMGR_get_master_clock();
	  }

	  return 0;
}

int APP_TMD_EEPROM_SET_BEGIN_ADR(uint32_t adr){

	  if ( !(APP_TMD_is_vaild_adr_in_eeprom_(adr)) ) return 1;

	  tlm_mem_dump_.eeprom.begin_adr = adr;
	  return 0;
}

int TMD_EEPROM_DUMP(void){
	  uint32_t i;
	  size_t dump_size = 0;
	  uint8_t ret = 0;

	  tlm_mem_dump_.eeprom.is_all_adr_range_valid = 1;

	  // 有効なアドレス範囲を探す
	  for (i = 0; i < APP_TMD_DUMP_SIZE; ++i)
	  {
	    uint32_t adr = tlm_mem_dump_.eeprom.begin_adr + i;

	    if ( !(APP_TMD_is_vaild_adr_in_eeprom_(adr)) )
	    {
	      tlm_mem_dump_.eeprom.is_all_adr_range_valid = 0;
	      break;
	    }

	    dump_size++;
	  }

	  if(dump_size != 0){
		  ret = EEPROM_GeneralRead(tlm_mem_dump_.eeprom.dump, dump_size, tlm_mem_dump_.eeprom.begin_adr);
		  tlm_mem_dump_.eeprom.dumped_size = (uint16_t)dump_size;
		  tlm_mem_dump_.eeprom.dump_err = ret;
		  tlm_mem_dump_.eeprom.dump_time = TMGR_get_master_clock();
	  }

	  return 0;
}

int APP_TMD_MRAM_SET_BEGIN_ADR(uint32_t adr){

	  if ( !(APP_TMD_is_vaild_adr_in_mram_(adr)) ) return 1;

	  tlm_mem_dump_.mram.begin_adr = adr;
	  return 0;
}

int TMD_MRAM_DUMP(void){
	  uint32_t i;
	  size_t dump_size = 0;
	  uint8_t ret = 0;

	  tlm_mem_dump_.mram.is_all_adr_range_valid = 1;

	  // 有効なアドレス範囲を探す
	  for (i = 0; i < APP_TMD_DUMP_SIZE; ++i)
	  {
	    uint32_t adr = tlm_mem_dump_.mram.begin_adr + i;

	    if ( !(APP_TMD_is_vaild_adr_in_mram_(adr)) )
	    {
	      tlm_mem_dump_.mram.is_all_adr_range_valid = 0;
	      break;
	    }

	    dump_size++;
	  }

	  if(dump_size != 0){
		  ret = MRAM_GeneralRead(tlm_mem_dump_.mram.dump, dump_size, tlm_mem_dump_.mram.begin_adr);
		  tlm_mem_dump_.mram.dumped_size = (uint16_t)dump_size;
		  tlm_mem_dump_.mram.dump_err = ret;
		  tlm_mem_dump_.mram.dump_time = TMGR_get_master_clock();
	  }

	  return 0;
}

int APP_TMD_SD_SET_BEGIN_BLOCK_ADR(uint32_t adr){

	  if ( !(APP_TMD_is_vaild_adr_in_sd_(adr)) ) return 1;

	  tlm_mem_dump_.sd.begin_block_adr = adr;
	  return 0;
}

int TMD_SD_DUMP(void){
	  uint8_t ret = 0;

	  tlm_mem_dump_.sd.is_all_adr_range_valid = 1;

	  uint32_t adr = tlm_mem_dump_.sd.begin_block_adr;

	  if(APP_TMD_is_vaild_adr_in_sd_(adr) == 0 || tlm_mem_dump_.sd.part >= 4){
		  tlm_mem_dump_.sd.dumped_size = 0;
		  tlm_mem_dump_.sd.dump_err = 0xff;
		  tlm_mem_dump_.sd.dump_time = TMGR_get_master_clock();
		  return 0;
	  }

	  else{
		  ret = sd_specify_read(tlm_mem_dump_.sd.begin_block_adr,
								  tlm_mem_dump_.sd.part * APP_TMD_SD_DUMP_SIZE,
								  tlm_mem_dump_.sd.dump,
								  APP_TMD_SD_DUMP_SIZE);
		  tlm_mem_dump_.sd.dumped_size = (uint16_t)APP_TMD_SD_DUMP_SIZE;
		  tlm_mem_dump_.sd.dump_err = ret;
		  tlm_mem_dump_.sd.dump_time = TMGR_get_master_clock();
	  }

	  return 0;
}

int APP_TMD_SPIPROM_SET_BEGIN_ADR(uint32_t adr){

	  if ( !(APP_TMD_is_vaild_adr_in_spiprom_(adr)) ) return 1;

	  tlm_mem_dump_.spiprom.begin_adr = adr;
	  return 0;
}

int TMD_SPIPROM_DUMP(void){
	  uint32_t i;
	  size_t dump_size = 0;
	  uint8_t ret = 0;

	  tlm_mem_dump_.spiprom.is_all_adr_range_valid = 1;

	  // 有効なアドレス範囲を探す
	  for (i = 0; i < APP_TMD_DUMP_SIZE; ++i)
	  {
	    uint32_t adr = tlm_mem_dump_.spiprom.begin_adr + i;

	    if ( !(APP_TMD_is_vaild_adr_in_spiprom_(adr)) )
	    {
	      tlm_mem_dump_.spiprom.is_all_adr_range_valid = 0;
	      break;
	    }

	    dump_size++;
	  }

	  if(dump_size != 0){
		  ret = readData(tlm_mem_dump_.spiprom.dump, tlm_mem_dump_.spiprom.begin_adr, dump_size);
		  tlm_mem_dump_.spiprom.dumped_size = (uint16_t)dump_size;
		  tlm_mem_dump_.spiprom.dump_err = ret;
		  tlm_mem_dump_.spiprom.dump_time = TMGR_get_master_clock();
	  }

	  return 0;
}

int APP_TMD_EEPROM3_SET_BEGIN_ADR(uint32_t adr){

	  if ( !(APP_TMD_is_vaild_adr_in_eeprom3_(adr)) ) return 1;

	  tlm_mem_dump_.eeprom3.begin_adr = adr;
	  return 0;
}

int TMD_EEPROM3_DUMP(void){
	  uint32_t i;
	  size_t dump_size = 0;
	  uint8_t ret = 0;

	  tlm_mem_dump_.eeprom3.is_all_adr_range_valid = 1;

	  // 有効なアドレス範囲を探す
	  for (i = 0; i < APP_TMD_DUMP3_SIZE; ++i)
	  {
	    uint32_t adr = tlm_mem_dump_.eeprom3.begin_adr + i;

	    if ( !(APP_TMD_is_vaild_adr_in_eeprom3_(adr)) )
	    {
	      tlm_mem_dump_.eeprom3.is_all_adr_range_valid = 0;
	      break;
	    }

	    dump_size++;
	  }

	  if(dump_size != 0){
		  ret += EEPROM_GeneralRead(&tlm_mem_dump_.eeprom3.dump[0][0], dump_size, tlm_mem_dump_.eeprom3.begin_adr);
		  ret += EEPROM_GeneralRead(&tlm_mem_dump_.eeprom3.dump[1][0], dump_size, tlm_mem_dump_.eeprom3.begin_adr + EEPROM_REDUNDANCY_SIZE * 1);
		  ret += EEPROM_GeneralRead(&tlm_mem_dump_.eeprom3.dump[2][0], dump_size, tlm_mem_dump_.eeprom3.begin_adr + EEPROM_REDUNDANCY_SIZE * 2);
		  tlm_mem_dump_.eeprom3.dumped_size = (uint16_t)dump_size;
		  tlm_mem_dump_.eeprom3.dump_err = ret;
		  tlm_mem_dump_.eeprom3.dump_time = TMGR_get_master_clock();
	  }

	  return 0;
}

int APP_TMD_MRAM3_SET_BEGIN_ADR(uint32_t adr){

	  if ( !(APP_TMD_is_vaild_adr_in_mram3_(adr)) ) return 1;

	  tlm_mem_dump_.mram3.begin_adr = adr;
	  return 0;
}

int TMD_MRAM3_DUMP(void){
	  uint32_t i;
	  size_t dump_size = 0;
	  uint8_t ret = 0;

	  tlm_mem_dump_.mram3.is_all_adr_range_valid = 1;

	  // 有効なアドレス範囲を探す
	  for (i = 0; i < APP_TMD_DUMP3_SIZE; ++i)
	  {
	    uint32_t adr = tlm_mem_dump_.mram3.begin_adr + i;

	    if ( !(APP_TMD_is_vaild_adr_in_mram3_(adr)) )
	    {
	      tlm_mem_dump_.mram3.is_all_adr_range_valid = 0;
	      break;
	    }

	    dump_size++;
	  }

	  if(dump_size != 0){
		  ret += MRAM_GeneralRead(&tlm_mem_dump_.mram3.dump[0][0], dump_size, tlm_mem_dump_.mram3.begin_adr);
		  ret += MRAM_GeneralRead(&tlm_mem_dump_.mram3.dump[1][0], dump_size, tlm_mem_dump_.mram3.begin_adr + MRAM_REDUNDANCY_SIZE * 1);
		  ret += MRAM_GeneralRead(&tlm_mem_dump_.mram3.dump[2][0], dump_size, tlm_mem_dump_.mram3.begin_adr + MRAM_REDUNDANCY_SIZE * 2);
		  tlm_mem_dump_.mram3.dumped_size = (uint16_t)dump_size;
		  tlm_mem_dump_.mram3.dump_err = ret;
		  tlm_mem_dump_.mram3.dump_time = TMGR_get_master_clock();
	  }

	  return 0;
}

int APP_TMD_SD3_SET_BEGIN_ADR(uint32_t adr){

	  if ( !(APP_TMD_is_vaild_adr_in_sd3_(adr)) ) return 1;

	  tlm_mem_dump_.sd3.begin_adr = adr;
	  return 0;
}

int TMD_SD3_DUMP(void){
	  uint32_t i;
	  size_t dump_size = 0;
	  uint8_t ret = 0;

	  tlm_mem_dump_.sd3.is_all_adr_range_valid = 1;

	  // 有効なアドレス範囲を探す
	  for (i = 0; i < APP_TMD_DUMP3_SIZE; ++i)
	  {
	    uint32_t adr = tlm_mem_dump_.sd3.begin_adr + i;

	    if ( !(APP_TMD_is_vaild_adr_in_sd3_(adr)) )
	    {
	      tlm_mem_dump_.sd3.is_all_adr_range_valid = 0;
	      break;
	    }

	    dump_size++;
	  }

	  if(dump_size != 0){
		  ret += sd_read_address(0, tlm_mem_dump_.sd3.begin_adr,                          &tlm_mem_dump_.sd3.dump[0][0], dump_size);
		  ret += sd_read_address(0, tlm_mem_dump_.sd3.begin_adr + SD_REDUNDANCY_SIZE * 1, &tlm_mem_dump_.sd3.dump[1][0], dump_size);
		  ret += sd_read_address(0, tlm_mem_dump_.sd3.begin_adr + SD_REDUNDANCY_SIZE * 2, &tlm_mem_dump_.sd3.dump[2][0], dump_size);
		  tlm_mem_dump_.sd3.dumped_size = (uint16_t)dump_size;
		  tlm_mem_dump_.sd3.dump_err = ret;
		  tlm_mem_dump_.sd3.dump_time = TMGR_get_master_clock();
	  }

	  return 0;
}

int APP_TMD_SPIPROM3_SET_BEGIN_ADR(uint32_t adr){

	  if ( !(APP_TMD_is_vaild_adr_in_spiprom3_(adr)) ) return 1;

	  tlm_mem_dump_.spiprom3.begin_adr = adr;
	  return 0;
}

int TMD_SPIPROM3_DUMP(void){
	  uint32_t i;
	  size_t dump_size = 0;
	  uint8_t ret = 0;

	  tlm_mem_dump_.spiprom3.is_all_adr_range_valid = 1;

	  // 有効なアドレス範囲を探す
	  for (i = 0; i < APP_TMD_DUMP3_SIZE; ++i)
	  {
	    uint32_t adr = tlm_mem_dump_.spiprom3.begin_adr + i;

	    if ( !(APP_TMD_is_vaild_adr_in_spiprom3_(adr)) )
	    {
	      tlm_mem_dump_.spiprom3.is_all_adr_range_valid = 0;
	      break;
	    }

	    dump_size++;
	  }

	  if(dump_size != 0){
		  ret += readData(&tlm_mem_dump_.spiprom3.dump[0][0], tlm_mem_dump_.spiprom3.begin_adr, dump_size);
		  ret += readData(&tlm_mem_dump_.spiprom3.dump[1][0], tlm_mem_dump_.spiprom3.begin_adr + SPIPROM_REDUNDANCY_SIZE * 1, dump_size);
		  ret += readData(&tlm_mem_dump_.spiprom3.dump[2][0], tlm_mem_dump_.spiprom3.begin_adr + SPIPROM_REDUNDANCY_SIZE * 2, dump_size);
		  tlm_mem_dump_.spiprom3.dumped_size = (uint16_t)dump_size;
		  tlm_mem_dump_.spiprom3.dump_err = ret;
		  tlm_mem_dump_.spiprom3.dump_time = TMGR_get_master_clock();
	  }

	  return 0;
}

CCP_CmdRet Cmd_APP_TMD_RAM_SET_BEGIN_ADR(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t adr;

  // パラメータを読み出し
  ENDIAN_memcpy(&adr, param, 4);

  uint8_t ret = APP_TMD_RAM_SET_BEGIN_ADR(adr);

  if (ret == 1) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  TMD_RAM_DUMP();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_TMD_RAM_SET_NEXT_BEGIN_ADR(const CommonCmdPacket* packet)
{
  uint32_t adr;
  (void)packet;

  adr = tlm_mem_dump_.ram.begin_adr + APP_TMD_DUMP_SIZE;

  if ( !(APP_TMD_is_vaild_adr_in_ram_(adr)) ) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  tlm_mem_dump_.ram.begin_adr = adr;

  TMD_RAM_DUMP();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_TMD_FLASH_SET_BEGIN_ADR(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t adr;

  // パラメータを読み出し
  ENDIAN_memcpy(&adr, param, 4);

  uint8_t ret = APP_TMD_FLASH_SET_BEGIN_ADR(adr);

  if (ret == 1) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  TMD_FLASH_DUMP();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_TMD_FLASH_SET_NEXT_BEGIN_ADR(const CommonCmdPacket* packet)
{
  uint32_t adr;
  (void)packet;

  adr = tlm_mem_dump_.flash.begin_adr + APP_TMD_DUMP_SIZE;

  if ( !(APP_TMD_is_vaild_adr_in_flash_(adr)) ) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  tlm_mem_dump_.flash.begin_adr = adr;

  TMD_FLASH_DUMP();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


//CCP_EXEC_STS Cmd_APP_TMD_FLASH_PI_DUMP(const CTCP* packet)
//{
//  CCP_EXEC_STS ret;
//
//  FLASH_on_physical_io_mode();
//  ret = Cmd_APP_TMD_FLASH_DUMP(packet);
//  FLASH_off_physical_io_mode();
//  return ret;
//}

CCP_CmdRet Cmd_APP_TMD_EEPROM_SET_BEGIN_ADR(const CommonCmdPacket* packet){
	  const uint8_t* param = CCP_get_param_head(packet);
	  uint32_t adr;

	  // パラメータを読み出し
	  ENDIAN_memcpy(&adr, param, 4);

	  uint8_t ret = APP_TMD_EEPROM_SET_BEGIN_ADR(adr);

	  if (ret == 1) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	  TMD_EEPROM_DUMP();

	  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_TMD_MRAM_SET_BEGIN_ADR(const CommonCmdPacket* packet){
	  const uint8_t* param = CCP_get_param_head(packet);
	  uint32_t adr;

	  // パラメータを読み出し
	  ENDIAN_memcpy(&adr, param, 4);

	  uint8_t ret = APP_TMD_MRAM_SET_BEGIN_ADR(adr);

	  if (ret == 1) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	  TMD_MRAM_DUMP();

	  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_TMD_SD_SET_BEGIN_BLOCK_ADR(const CommonCmdPacket* packet){
	  const uint8_t* param = CCP_get_param_head(packet);
	  uint32_t adr;
	  uint8_t pos;

	  // パラメータを読み出し
	  ENDIAN_memcpy(&adr, param, 4);
	  memcpy(&tlm_mem_dump_.sd.part, param + 4, 1);

	  uint8_t ret = APP_TMD_SD_SET_BEGIN_BLOCK_ADR(adr);

	  if (ret == 1) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	  if(tlm_mem_dump_.sd.part >= 4){
		  tlm_mem_dump_.sd.part = 0;
		  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	  }

	  TMD_SD_DUMP();

	  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_TMD_SPIPROM_SET_BEGIN_ADR(const CommonCmdPacket* packet){
	  const uint8_t* param = CCP_get_param_head(packet);
	  uint32_t adr;

	  // パラメータを読み出し
	  ENDIAN_memcpy(&adr, param, 4);

	  uint8_t ret = APP_TMD_SPIPROM_SET_BEGIN_ADR(adr);

	  if (ret == 1) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	  TMD_SPIPROM_DUMP();

	  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_TMD_EEPROM3_SET_BEGIN_ADR(const CommonCmdPacket* packet){
	  const uint8_t* param = CCP_get_param_head(packet);
	  uint32_t adr;

	  // パラメータを読み出し
	  ENDIAN_memcpy(&adr, param, 4);

	  uint8_t ret = APP_TMD_EEPROM3_SET_BEGIN_ADR(adr);

	  if (ret == 1) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	  TMD_EEPROM3_DUMP();

	  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_TMD_MRAM3_SET_BEGIN_ADR(const CommonCmdPacket* packet){
	  const uint8_t* param = CCP_get_param_head(packet);
	  uint32_t adr;

	  // パラメータを読み出し
	  ENDIAN_memcpy(&adr, param, 4);

	  uint8_t ret = APP_TMD_MRAM3_SET_BEGIN_ADR(adr);

	  if (ret == 1) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	  TMD_MRAM3_DUMP();

	  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_TMD_SD3_SET_BEGIN_ADR(const CommonCmdPacket* packet){
	  const uint8_t* param = CCP_get_param_head(packet);
	  uint32_t adr;

	  // パラメータを読み出し
	  ENDIAN_memcpy(&adr, param, 4);

	  uint8_t ret = APP_TMD_SD3_SET_BEGIN_ADR(adr);

	  if (ret == 1) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	  TMD_SD3_DUMP();

	  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_TMD_SPIPROM3_SET_BEGIN_ADR(const CommonCmdPacket* packet){
	  const uint8_t* param = CCP_get_param_head(packet);
	  uint32_t adr;

	  // パラメータを読み出し
	  ENDIAN_memcpy(&adr, param, 4);

	  uint8_t ret = APP_TMD_SPIPROM3_SET_BEGIN_ADR(adr);

	  if (ret == 1) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

	  TMD_SPIPROM3_DUMP();

	  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


#pragma section
