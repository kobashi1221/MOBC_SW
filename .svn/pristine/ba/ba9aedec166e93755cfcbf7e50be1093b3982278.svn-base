/*
 * Flach.c
 *
 *  Created on: 2022/02/26
 *      Author: SAT37
 */

#include "MEM_cmd_FLASH.h"

#include "../../Middleware/MemoryMW.h"
#include <src_core/Library/endian.h>
//#include <src_user/Settings/AnomalyLogger/anomaly_group.h>
#include <src_user/Drivers/Cdh/Flash/flash.h>
#include <src_user/Drivers/Cdh/SD/SD.h>
#include <src_user/IfWrapper/HAL.h>
#include <src_user/Library/General/DBG_printf.h>
#include "../../../TlmCmd/telemetry_definitions.h"
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include <src_user/Applications/UserDefined/Cdh/ResetUtility.h>
#include <src_core/TlmCmd/telemetry_frame.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>

//メモリーパトロールのアドレス
static uint32_t FlashPatrolAddr = 0x08000000;
static uint16_t Flash_ReadSize;
const uint16_t Flash_ReadSize_ = 1024;

uint32_t boot_mis_address;
uint8_t boot_error_num;

static void     BD_init_(void);
static void     BD_app_(void);
static void     BD_exec_copy_sd_to_sd_(void);
static void     BD_exec_copy_flash_to_flash_(void);
static void     BD_exec_copy_sd_to_flash_(void);
static void     BD_exec_copy_flash_to_sd_(void);
static void     BD_exec_erase_flash_(void);
static void     BD_exec_calc_flash_crc_(void);
static void     BD_exec_calc_sd_crc_(void);
static void     BD_exec_copy_sd_to_ram_(void);
static void     BD_exec_calc_ram_crc_(void);

static BD_STATE BD_set_copy_sd_to_sd_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_block_adr,
									  uint32_t sd_src_adr,
									  uint32_t sd_dst_block_adr,
									  uint32_t sd_dst_adr,
                                      uint32_t len);
static BD_STATE BD_set_copy_flash_to_flash_(BigDataJobInfo* p_job_info,
											  uint32_t sd_src_adr,
											  uint32_t sd_dst_adr,
											  uint32_t len);
static BD_STATE BD_set_copy_sd_to_flash_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_block_adr,
									  uint32_t sd_src_adr,
									  uint32_t sd_dst_adr,
                                      uint32_t len);
static BD_STATE BD_set_copy_flash_to_sd_(BigDataJobInfo* p_job_info,
										  uint32_t sd_src_adr,
										  uint32_t sd_dst_block_adr,
										  uint32_t sd_dst_adr,
										  uint32_t len);
static BD_STATE BD_set_erase_flash_(BigDataJobInfo* p_job_info,
									  uint32_t Sector,
									  uint32_t NbSector);
static BD_STATE BD_set_calc_flash_crc_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_adr,
                                      uint32_t len);
static BD_STATE BD_set_calc_sd_crc_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_block_adr,
									  uint32_t sd_src_adr,
                                      uint32_t len);
static BD_STATE BD_set_copy_sd_to_ram_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_block_adr,
									  uint32_t sd_src_adr,
									  uint32_t sd_dst_adr,
                                      uint32_t len);
static BD_STATE BD_set_calc_ram_crc_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_adr,
                                      uint32_t len);
static BD_STATE BD_exec_job_(BigDataJobInfo* p_job_info);
static BD_STATE BD_abort_job_(BigDataJobInfo* p_job_info);
static BD_STATE BD_get_job_state_(BigDataJobInfo* p_job_info);
static void BD_init_job_info_(
                              BigDataJobInfo*      p_job_info,
                              BD_MODE              mode,
							  uint32_t             sd_src_block_adr,
							  uint32_t             sd_src_adr,
							  uint32_t             sd_dst_block_adr,
							  uint32_t             sd_dst_adr,
							  uint32_t             erase_block,
					          uint32_t             erase_block_num,
                              uint32_t             len);
static uint8_t BD_increment_id_counter_(void);

static void Generate_crc_calc_tlm(void);

static BigDataJobInfo job_info_via_cmd_;     // 初期に何もポインタがさしてないとテレメなどでやばいので
                                                  // また，コマンドからの実行でも使う
static BigDataInfo bdi_;
const BigDataInfo* bdi;

AppInfo BD_create_app(void)
{
  return AI_create_app_info("big_data", BD_init_, BD_app_);
}


static void BD_init_(void)
{
  job_info_via_cmd_.job_id             = 0;
  job_info_via_cmd_.state              = BD_STATE_COMPLETED;
  job_info_via_cmd_.mode               = (BD_MODE)0;
  job_info_via_cmd_.process_counter    = 0;
  job_info_via_cmd_.err_info.err_state = (BD_PROCESS_ERR)0;
  job_info_via_cmd_.err_info.err_adr   = 0;
  job_info_via_cmd_.err_info.err_block = 0;
  job_info_via_cmd_.sd_src_block_adr   = 0;
  job_info_via_cmd_.sd_dst_block_adr   = 0;
  job_info_via_cmd_.src_adr            = 0;
  job_info_via_cmd_.dst_adr            = 0;
  job_info_via_cmd_.len                = 0;
  job_info_via_cmd_.erase_block        = 0;
  job_info_via_cmd_.erase_block_num    = 0;
  job_info_via_cmd_.flash_crc          = 0;
  job_info_via_cmd_.sd_crc             = 0;
  job_info_via_cmd_.ram_crc            = 0;

  bdi = &bdi_;
  bdi_.p_job_info = &job_info_via_cmd_;
  bdi_.id_counter = 0;
}


// 定期的に実行され，バックグラウンドプロセスを分割処理する
static void BD_app_(void)
{
	print(Flash, "Big Data Info\r\n");
	print(Flash, "mode = %d\r\n", bdi_.p_job_info->mode);
	print(Flash, "state = %d\r\n", bdi_.p_job_info->state);
	print(Flash, "proces cnt = %d\r\n", bdi_.p_job_info->process_counter);
	print(Flash, "end cnt = %d\r\n", bdi_.p_job_info->end_process_counter);
	print(Flash, "job_id = %d\r\n", bdi_.p_job_info->job_id);
	print(Flash, "flash crc = %d\r\n", bdi_.p_job_info->flash_crc);
	print(Flash, "sd crc = %d\r\n", bdi_.p_job_info->sd_crc);
	print(Flash, "ram crc = %d\r\n", bdi_.p_job_info->ram_crc);

  // 処理する？
  if (bdi_.p_job_info->state != BD_STATE_PROCESSING) return;

  switch (bdi_.p_job_info->mode)
  {
  case BD_MODE_SD_TO_SD:
    BD_exec_copy_sd_to_sd_();
    break;
  case BD_MODE_FLASH_TO_FLASH:
    BD_exec_copy_flash_to_flash_();
    break;
  case BD_MODE_SD_TO_FLASH:
    BD_exec_copy_sd_to_flash_();
    break;
  case BD_MODE_FLASH_TO_SD:
    BD_exec_copy_flash_to_sd_();
    break;
  case BD_MODE_ELASE_FLASH:
    BD_exec_erase_flash_();
    break;
  case BD_MODE_FLASH_CRC:
    BD_exec_calc_flash_crc_();
    break;
  case BD_MODE_SD_CRC:
    BD_exec_calc_sd_crc_();
    break;
  case BD_MODE_SD_TO_RAM:
    BD_exec_copy_sd_to_ram_();
    break;
  case BD_MODE_RAM_CRC:
	BD_exec_calc_ram_crc_();
    break;
  default:
    // 何もしない
    break;
  }
}

static void BD_exec_copy_sd_to_sd_(void){

	static uint8_t is_len_blocksize     = 0;	  //lenがblocksizeで割り切れるか

	int ret;
    uint32_t src_block_adr;
    uint32_t dst_block_adr;
    uint32_t src_adr;
    uint32_t dst_adr;
    uint32_t len;

	// 初回の初期化
	if (bdi_.p_job_info->process_counter == 0)
	{
		is_len_blocksize = (bdi_.p_job_info->len % SD_BLOCKSIZE == 0) ? 1 : 0;

		if(is_len_blocksize)
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE;
			bdi->p_job_info->rest_len = 0;
		}
		else
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE + 1;
			bdi->p_job_info->rest_len = bdi_.p_job_info->len % SD_BLOCKSIZE;
		}
	}

	if((bdi->p_job_info->process_counter + 1) == bdi->p_job_info->end_process_counter)
	{
	    // 最終回
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_block_adr = bdi_.p_job_info->sd_src_block_adr;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    dst_block_adr = bdi_.p_job_info->sd_dst_block_adr;
	    dst_adr       = bdi_.p_job_info->dst_adr + offset;

	    if(bdi->p_job_info->rest_len == 0)
	    {
	    	len = SD_BLOCKSIZE;
	    }
	    else
	    {
	    	len = bdi->p_job_info->rest_len;
	    }
	}
	else
	{
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_block_adr = bdi_.p_job_info->sd_src_block_adr;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    dst_block_adr = bdi_.p_job_info->sd_dst_block_adr;
	    dst_adr       = bdi_.p_job_info->dst_adr + offset;
	    len = SD_BLOCKSIZE;
	}

	ret = sd_copy_sd_to_sd_(dst_block_adr, dst_adr,
							src_block_adr, src_adr, len);

	// エラー処理
	if (ret != BSP_SD_OK)
	{
		bdi_.p_job_info->state = BD_STATE_ABORTED_BY_ERR;
		bdi_.p_job_info->err_info.err_state = BD_PROCESS_ERR_AT_SD_COPY;
		bdi_.p_job_info->err_info.err_block = src_block_adr;
		bdi_.p_job_info->err_info.err_adr   = src_adr;
	}

	// 終了判定
	bdi_.p_job_info->process_counter++;
	if (bdi_.p_job_info->process_counter >= bdi->p_job_info->end_process_counter)
	{
		bdi_.p_job_info->state = BD_STATE_COMPLETED;
	}
}

static void BD_exec_copy_flash_to_flash_(void){

	static uint8_t is_len_blocksize     = 0;	  //lenがblocksizeで割り切れるか

	int ret;
    uint32_t src_adr;
    uint32_t dst_adr;
    uint32_t len;

	// 初回の初期化
	if (bdi_.p_job_info->process_counter == 0)
	{
		is_len_blocksize = (bdi_.p_job_info->len % SD_BLOCKSIZE == 0) ? 1 : 0;

		if(is_len_blocksize)
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE;
			bdi->p_job_info->rest_len = 0;
		}
		else
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE + 1;
			bdi->p_job_info->rest_len = bdi_.p_job_info->len % SD_BLOCKSIZE;
		}
	}

	if((bdi->p_job_info->process_counter + 1) == bdi->p_job_info->end_process_counter)
	{
	    // 最終回
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    dst_adr       = bdi_.p_job_info->dst_adr + offset;

	    if(bdi->p_job_info->rest_len == 0)
	    {
	    	len = SD_BLOCKSIZE;
	    }
	    else
	    {
	    	len = bdi->p_job_info->rest_len;
	    }
	}
	else
	{
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    dst_adr       = bdi_.p_job_info->dst_adr + offset;
	    len = SD_BLOCKSIZE;
	}

	ret = flash_copy_flash_to_flash_(src_adr, dst_adr, len);

	// エラー処理
	if (ret != HAL_OK)
	{
		bdi_.p_job_info->state = BD_STATE_ABORTED_BY_ERR;
		bdi_.p_job_info->err_info.err_state = BD_PROCESS_ERR_AT_FLASH_COPY;
		bdi_.p_job_info->err_info.err_adr   = src_adr;
	}

	// 終了判定
	bdi_.p_job_info->process_counter++;
	if (bdi_.p_job_info->process_counter >= bdi->p_job_info->end_process_counter)
	{
		bdi_.p_job_info->state = BD_STATE_COMPLETED;
	}
}

static void BD_exec_copy_sd_to_flash_(void){

	static uint8_t is_len_blocksize     = 0;	  //lenがblocksizeで割り切れるか

	int ret;
    uint32_t src_block_adr;
    uint32_t src_adr;
    uint32_t dst_adr;
    uint32_t len;

	// 初回の初期化
	if (bdi_.p_job_info->process_counter == 0)
	{
		is_len_blocksize = (bdi_.p_job_info->len % SD_BLOCKSIZE == 0) ? 1 : 0;

		if(is_len_blocksize)
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE;
			bdi->p_job_info->rest_len = 0;
		}
		else
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE + 1;
			bdi->p_job_info->rest_len = bdi_.p_job_info->len % SD_BLOCKSIZE;
		}
	}

	if((bdi->p_job_info->process_counter + 1) == bdi->p_job_info->end_process_counter)
	{
	    // 最終回
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_block_adr = bdi_.p_job_info->sd_src_block_adr;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    dst_adr       = bdi_.p_job_info->dst_adr + offset;

	    if(bdi->p_job_info->rest_len == 0)
	    {
	    	len = SD_BLOCKSIZE;
	    }
	    else
	    {
	    	len = bdi->p_job_info->rest_len;
	    }
	}
	else
	{
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_block_adr = bdi_.p_job_info->sd_src_block_adr;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    dst_adr       = bdi_.p_job_info->dst_adr + offset;
	    len = SD_BLOCKSIZE;
	}

	ret = flash_copy_sd_to_flash_(dst_adr, src_block_adr, src_adr, len);

	// エラー処理
	if (ret != BSP_SD_OK)
	{
		bdi_.p_job_info->state = BD_STATE_ABORTED_BY_ERR;
		bdi_.p_job_info->err_info.err_state = BD_PROCESS_ERR_AT_SD_TO_FLASH;
		bdi_.p_job_info->err_info.err_block = src_block_adr;
		bdi_.p_job_info->err_info.err_adr   = src_adr;
	}

	// 終了判定
	bdi_.p_job_info->process_counter++;
	if (bdi_.p_job_info->process_counter >= bdi->p_job_info->end_process_counter)
	{
		bdi_.p_job_info->state = BD_STATE_COMPLETED;
	}
}

static void BD_exec_copy_flash_to_sd_(void){

	static uint8_t is_len_blocksize     = 0;	  //lenがblocksizeで割り切れるか

	int ret;
    uint32_t dst_block_adr;
    uint32_t src_adr;
    uint32_t dst_adr;
    uint32_t len;

	// 初回の初期化
	if (bdi_.p_job_info->process_counter == 0)
	{
		is_len_blocksize = (bdi_.p_job_info->len % SD_BLOCKSIZE == 0) ? 1 : 0;

		if(is_len_blocksize)
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE;
			bdi->p_job_info->rest_len = 0;
		}
		else
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE + 1;
			bdi->p_job_info->rest_len = bdi_.p_job_info->len % SD_BLOCKSIZE;
		}
	}

	if((bdi->p_job_info->process_counter + 1) == bdi->p_job_info->end_process_counter)
	{
	    // 最終回
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    dst_block_adr = bdi_.p_job_info->sd_dst_block_adr;
	    dst_adr       = bdi_.p_job_info->dst_adr + offset;

	    if(bdi->p_job_info->rest_len == 0)
	    {
	    	len = SD_BLOCKSIZE;
	    }
	    else
	    {
	    	len = bdi->p_job_info->rest_len;
	    }
	}
	else
	{
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    dst_block_adr = bdi_.p_job_info->sd_dst_block_adr;
	    dst_adr       = bdi_.p_job_info->dst_adr + offset;
	    len = SD_BLOCKSIZE;
	}

	ret = sd_copy_flash_to_sd_(dst_block_adr, dst_adr,
							      src_adr, len);

	// エラー処理
	if (ret != BSP_SD_OK)
	{
		bdi_.p_job_info->state = BD_STATE_ABORTED_BY_ERR;
		bdi_.p_job_info->err_info.err_state = BD_PROCESS_ERR_AT_FLASH_TO_SD;
		bdi_.p_job_info->err_info.err_adr   = src_adr;
	}

	// 終了判定
	bdi_.p_job_info->process_counter++;
	if (bdi_.p_job_info->process_counter >= bdi->p_job_info->end_process_counter)
	{
		bdi_.p_job_info->state = BD_STATE_COMPLETED;
	}
}

static void BD_exec_erase_flash_(void){

	static uint8_t is_len_blocksize     = 0;	  //lenがblocksizeで割り切れるか

	int ret;
    uint8_t Sector;

    Sector = bdi->p_job_info->erase_block + bdi_.p_job_info->process_counter;

	ret = Flash_Sector_Erase(Sector, 1);

	// エラー処理
	if (ret != HAL_OK)
	{
		bdi_.p_job_info->state = BD_STATE_ABORTED_BY_ERR;
		bdi_.p_job_info->err_info.err_state = BD_PROCESS_ERR_AT_ERASE;
		bdi_.p_job_info->err_info.err_block = Sector;
	}

	// 終了判定
	bdi_.p_job_info->process_counter++;
	if (bdi_.p_job_info->process_counter >= bdi->p_job_info->erase_block_num)
	{
		bdi_.p_job_info->state = BD_STATE_COMPLETED;
	}
}

static void BD_exec_calc_flash_crc_(void){

	static uint8_t is_len_blocksize     = 0;	  //lenがblocksizeで割り切れるか

	int ret;
    uint32_t src_adr;
    uint32_t len;

	// 初回の初期化
	if (bdi_.p_job_info->process_counter == 0)
	{
		is_len_blocksize = (bdi_.p_job_info->len % SD_BLOCKSIZE == 0) ? 1 : 0;

		if(is_len_blocksize)
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE;
			bdi->p_job_info->rest_len = 0;
		}
		else
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE + 1;
			bdi->p_job_info->rest_len = bdi_.p_job_info->len % SD_BLOCKSIZE;
		}
		bdi->p_job_info->save_crc = 0x0000;
	}

	if((bdi->p_job_info->process_counter + 1) == bdi->p_job_info->end_process_counter)
	{
	    // 最終回
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_adr       = bdi_.p_job_info->src_adr + offset;

	    if(bdi->p_job_info->rest_len == 0)
	    {
	    	len = SD_BLOCKSIZE;
	    }
	    else
	    {
	    	len = bdi->p_job_info->rest_len;
	    }
	}
	else
	{
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    len = SD_BLOCKSIZE;
	}

	calc_crc16_update((uint8_t*)src_adr, len, &(job_info_via_cmd_.flash_crc));
	calc_crc16_update((uint8_t*)src_adr, len, &(job_info_via_cmd_.save_crc));
	if(bdi_.p_job_info->process_counter % 16 == 15){
		Generate_crc_calc_tlm();
		job_info_via_cmd_.save_crc = 0x0000;
	}

//	// エラー処理
//	if (ret != BSP_SD_OK)
//	{
//		bdi_.p_job_info->state = BD_STATE_ABORTED_BY_ERR;
//		bdi_.p_job_info->err_info.err_state = BD_PROCESS_ERR_AT_SD_COPY;
//		bdi_.p_job_info->err_info.err_adr   = src_adr;
//	}

	// 終了判定
	bdi_.p_job_info->process_counter++;
	if (bdi_.p_job_info->process_counter >= bdi->p_job_info->end_process_counter)
	{
		bdi_.p_job_info->state = BD_STATE_COMPLETED;
	}
}

static void BD_exec_calc_sd_crc_(void){

	static uint8_t is_len_blocksize     = 0;	  //lenがblocksizeで割り切れるか

	int ret;
    uint32_t src_block_adr;
    uint32_t src_adr;
    uint32_t len;

	// 初回の初期化
	if (bdi_.p_job_info->process_counter == 0)
	{
		is_len_blocksize = (bdi_.p_job_info->len % SD_BLOCKSIZE == 0) ? 1 : 0;

		if(is_len_blocksize)
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE;
			bdi->p_job_info->rest_len = 0;
		}
		else
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE + 1;
			bdi->p_job_info->rest_len = bdi_.p_job_info->len % SD_BLOCKSIZE;
		}
		job_info_via_cmd_.save_crc = 0x0000;
	}

	if((bdi->p_job_info->process_counter + 1) == bdi->p_job_info->end_process_counter)
	{
	    // 最終回
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_block_adr = bdi_.p_job_info->sd_src_block_adr;
	    src_adr       = bdi_.p_job_info->src_adr + offset;

	    if(bdi->p_job_info->rest_len == 0)
	    {
	    	len = SD_BLOCKSIZE;
	    }
	    else
	    {
	    	len = bdi->p_job_info->rest_len;
	    }
	}
	else
	{
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_block_adr = bdi_.p_job_info->sd_src_block_adr;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    len = SD_BLOCKSIZE;
	}

	ret = sd_crc(src_block_adr, src_adr, len, &(job_info_via_cmd_.sd_crc), &(job_info_via_cmd_.save_crc));
	if(bdi_.p_job_info->process_counter % 16 == 15){
		Generate_crc_calc_tlm();
		job_info_via_cmd_.save_crc = 0x0000;
	}

	// エラー処理
	if (ret != BSP_SD_OK)
	{
		bdi_.p_job_info->state = BD_STATE_ABORTED_BY_ERR;
		bdi_.p_job_info->err_info.err_state = BD_PROCESS_ERR_AT_SD_CRC;
		bdi_.p_job_info->err_info.err_block = src_block_adr;
		bdi_.p_job_info->err_info.err_adr   = src_adr;
	}

	// 終了判定
	bdi_.p_job_info->process_counter++;
	if (bdi_.p_job_info->process_counter >= bdi->p_job_info->end_process_counter)
	{
		bdi_.p_job_info->state = BD_STATE_COMPLETED;
	}
}


static void BD_exec_copy_sd_to_ram_(void){

	static uint8_t is_len_blocksize     = 0;	  //lenがblocksizeで割り切れるか

	int ret;
    uint32_t src_block_adr;
    uint32_t src_adr;
    uint32_t dst_adr;
    uint32_t len;

	// 初回の初期化
	if (bdi_.p_job_info->process_counter == 0)
	{
		is_len_blocksize = (bdi_.p_job_info->len % SD_BLOCKSIZE == 0) ? 1 : 0;

		if(is_len_blocksize)
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE;
			bdi->p_job_info->rest_len = 0;
		}
		else
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE + 1;
			bdi->p_job_info->rest_len = bdi_.p_job_info->len % SD_BLOCKSIZE;
		}
	}

	if((bdi->p_job_info->process_counter + 1) == bdi->p_job_info->end_process_counter)
	{
	    // 最終回
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_block_adr = bdi_.p_job_info->sd_src_block_adr;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    dst_adr       = bdi_.p_job_info->dst_adr + offset;

	    if(bdi->p_job_info->rest_len == 0)
	    {
	    	len = SD_BLOCKSIZE;
	    }
	    else
	    {
	    	len = bdi->p_job_info->rest_len;
	    }
	}
	else
	{
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_block_adr = bdi_.p_job_info->sd_src_block_adr;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    dst_adr       = bdi_.p_job_info->dst_adr + offset;
	    len = SD_BLOCKSIZE;
	}

	ret = sd_copy_sd_to_ram_(dst_adr, src_block_adr, src_adr, len);

	// エラー処理
	if (ret != BSP_SD_OK)
	{
		bdi_.p_job_info->state = BD_STATE_ABORTED_BY_ERR;
		bdi_.p_job_info->err_info.err_state = BD_PROCESS_ERR_AT_SD_TO_RAM;
		bdi_.p_job_info->err_info.err_block = src_block_adr;
		bdi_.p_job_info->err_info.err_adr   = src_adr;
	}

	// 終了判定
	bdi_.p_job_info->process_counter++;
	if (bdi_.p_job_info->process_counter >= bdi->p_job_info->end_process_counter)
	{
		bdi_.p_job_info->state = BD_STATE_COMPLETED;
	}
}

static void BD_exec_calc_ram_crc_(void){

	static uint8_t is_len_blocksize     = 0;	  //lenがblocksizeで割り切れるか

	int ret;
    uint32_t src_adr;
    uint32_t len;

	// 初回の初期化
	if (bdi_.p_job_info->process_counter == 0)
	{
		is_len_blocksize = (bdi_.p_job_info->len % SD_BLOCKSIZE == 0) ? 1 : 0;

		if(is_len_blocksize)
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE;
			bdi->p_job_info->rest_len = 0;
		}
		else
		{
			bdi->p_job_info->end_process_counter = bdi_.p_job_info->len / SD_BLOCKSIZE + 1;
			bdi->p_job_info->rest_len = bdi_.p_job_info->len % SD_BLOCKSIZE;
		}
		job_info_via_cmd_.save_crc = 0x0000;
	}

	if((bdi->p_job_info->process_counter + 1) == bdi->p_job_info->end_process_counter)
	{
	    // 最終回
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_adr       = bdi_.p_job_info->src_adr + offset;

	    if(bdi->p_job_info->rest_len == 0)
	    {
	    	len = SD_BLOCKSIZE;
	    }
	    else
	    {
	    	len = bdi->p_job_info->rest_len;
	    }
	}
	else
	{
	    uint32_t offset = SD_BLOCKSIZE * bdi_.p_job_info->process_counter;
	    src_adr       = bdi_.p_job_info->src_adr + offset;
	    len = SD_BLOCKSIZE;
	}

	calc_crc16_update((uint8_t*)src_adr, len, &(job_info_via_cmd_.ram_crc));
	calc_crc16_update((uint8_t*)src_adr, len, &(job_info_via_cmd_.save_crc));
	if(bdi_.p_job_info->process_counter % 16 == 15){
		Generate_crc_calc_tlm();
		job_info_via_cmd_.save_crc = 0x0000;
	}

//	// エラー処理
//	if (ret != BSP_SD_OK)
//	{
//		bdi_.p_job_info->state = BD_STATE_ABORTED_BY_ERR;
//		bdi_.p_job_info->err_info.err_state = BD_PROCESS_ERR_AT_SD_COPY;
//		bdi_.p_job_info->err_info.err_adr   = src_adr;
//	}

	// 終了判定
	bdi_.p_job_info->process_counter++;
	if (bdi_.p_job_info->process_counter >= bdi->p_job_info->end_process_counter)
	{
		bdi_.p_job_info->state = BD_STATE_COMPLETED;
	}
}

// ############# 処理系 #############
static BD_STATE BD_set_copy_sd_to_sd_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_block_adr,
									  uint32_t sd_src_adr,
									  uint32_t sd_dst_block_adr,
									  uint32_t sd_dst_adr,
                                      uint32_t len)
{
  if (bdi_.p_job_info->state == BD_STATE_PROCESSING) return BD_STATE_ERR_BUSY;
  if (len == 0) return BD_STATE_ERR_ILLEGAL_PARAMS;     // 処理部でめんどいのでここで弾く

  BD_init_job_info_(
                      p_job_info,
					  BD_MODE_SD_TO_SD,
					  sd_src_block_adr,
					  sd_src_adr,
					  sd_dst_block_adr,
					  sd_dst_adr,
                      0,
                      0,
                      len
                    );

  return p_job_info->state;
}

static BD_STATE BD_set_copy_flash_to_flash_(BigDataJobInfo* p_job_info,
											  uint32_t sd_src_adr,
											  uint32_t sd_dst_adr,
											  uint32_t len)
{
  if (bdi_.p_job_info->state == BD_STATE_PROCESSING) return BD_STATE_ERR_BUSY;
  if (len == 0) return BD_STATE_ERR_ILLEGAL_PARAMS;     // 処理部でめんどいのでここで弾く

  BD_init_job_info_(
                      p_job_info,
					  BD_MODE_FLASH_TO_FLASH,
					  0,
					  sd_src_adr,
					  0,
					  sd_dst_adr,
                      0,
                      0,
                      len
                    );

  return p_job_info->state;
}

static BD_STATE BD_set_copy_sd_to_flash_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_block_adr,
									  uint32_t sd_src_adr,
									  uint32_t sd_dst_adr,
                                      uint32_t len)
{
  if (bdi_.p_job_info->state == BD_STATE_PROCESSING) return BD_STATE_ERR_BUSY;
  if (len == 0) return BD_STATE_ERR_ILLEGAL_PARAMS;     // 処理部でめんどいのでここで弾く

  BD_init_job_info_(
                      p_job_info,
					  BD_MODE_SD_TO_FLASH,
					  sd_src_block_adr,
					  sd_src_adr,
					  0,
					  sd_dst_adr,
                      0,
                      0,
                      len
                    );

  return p_job_info->state;
}

static BD_STATE BD_set_copy_flash_to_sd_(BigDataJobInfo* p_job_info,
										  uint32_t sd_src_adr,
										  uint32_t sd_dst_block_adr,
										  uint32_t sd_dst_adr,
										  uint32_t len)
{
  if (bdi_.p_job_info->state == BD_STATE_PROCESSING) return BD_STATE_ERR_BUSY;
  if (len == 0) return BD_STATE_ERR_ILLEGAL_PARAMS;     // 処理部でめんどいのでここで弾く

  BD_init_job_info_(
                      p_job_info,
					  BD_MODE_FLASH_TO_SD,
					  0,
					  sd_src_adr,
					  sd_dst_block_adr,
					  sd_dst_adr,
                      0,
                      0,
                      len
                    );

  return p_job_info->state;
}

static BD_STATE BD_set_erase_flash_(BigDataJobInfo* p_job_info,
									  uint32_t Sector,
									  uint32_t NbSector)
{
  if (bdi_.p_job_info->state == BD_STATE_PROCESSING) return BD_STATE_ERR_BUSY;
  if (NbSector == 0) return BD_STATE_ERR_ILLEGAL_PARAMS;     // 処理部でめんどいのでここで弾く

  BD_init_job_info_(
                      p_job_info,
					  BD_MODE_ELASE_FLASH,
					  0,
					  0,
					  0,
					  0,
					  Sector,
					  NbSector,
					  0
                    );

  return p_job_info->state;
}

static BD_STATE BD_set_calc_flash_crc_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_adr,
                                      uint32_t len)
{
  if (bdi_.p_job_info->state == BD_STATE_PROCESSING) return BD_STATE_ERR_BUSY;
  if (len == 0) return BD_STATE_ERR_ILLEGAL_PARAMS;     // 処理部でめんどいのでここで弾く

  BD_init_job_info_(
                      p_job_info,
					  BD_MODE_FLASH_CRC,
					  0,
					  sd_src_adr,
					  0,
					  0,
                      0,
                      0,
                      len
                    );

  return p_job_info->state;
}

static BD_STATE BD_set_calc_sd_crc_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_block_adr,
									  uint32_t sd_src_adr,
                                      uint32_t len)
{
  if (bdi_.p_job_info->state == BD_STATE_PROCESSING) return BD_STATE_ERR_BUSY;
  if (len == 0) return BD_STATE_ERR_ILLEGAL_PARAMS;     // 処理部でめんどいのでここで弾く

  BD_init_job_info_(
                      p_job_info,
					  BD_MODE_SD_CRC,
					  sd_src_block_adr,
					  sd_src_adr,
					  0,
					  0,
                      0,
                      0,
                      len
                    );

  return p_job_info->state;
}

static BD_STATE BD_set_copy_sd_to_ram_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_block_adr,
									  uint32_t sd_src_adr,
									  uint32_t sd_dst_adr,
                                      uint32_t len)
{
  if (bdi_.p_job_info->state == BD_STATE_PROCESSING) return BD_STATE_ERR_BUSY;
  if (len == 0) return BD_STATE_ERR_ILLEGAL_PARAMS;     // 処理部でめんどいのでここで弾く

  BD_init_job_info_(
                      p_job_info,
					  BD_MODE_SD_TO_RAM,
					  sd_src_block_adr,
					  sd_src_adr,
					  0,
					  sd_dst_adr,
                      0,
                      0,
                      len
                    );

  return p_job_info->state;
}

static BD_STATE BD_set_calc_ram_crc_(BigDataJobInfo* p_job_info,
									  uint32_t sd_src_adr,
                                      uint32_t len)
{
  if (bdi_.p_job_info->state == BD_STATE_PROCESSING) return BD_STATE_ERR_BUSY;
  if (len == 0) return BD_STATE_ERR_ILLEGAL_PARAMS;     // 処理部でめんどいのでここで弾く

  BD_init_job_info_(
                      p_job_info,
					  BD_MODE_RAM_CRC,
					  0,
					  sd_src_adr,
					  0,
					  0,
                      0,
                      0,
                      len
                    );

  return p_job_info->state;
}

static BD_STATE BD_exec_job_(BigDataJobInfo* p_job_info)
{
  // 実行しようとしているjobが登録jobと同じか，IDチェック
  if (bdi_.p_job_info->job_id != p_job_info->job_id) return BD_STATE_ERR_MISMATCH_JOB;
  // 実行待ちか？
  if (p_job_info->state != BD_STATE_WAITING_EXEC) return BD_STATE_ERR_CANNOT_EXEC;

  // 実行させる
  p_job_info->state = BD_STATE_PROCESSING;
  return p_job_info->state;
}

static BD_STATE BD_abort_job_(BigDataJobInfo* p_job_info)
{
  // 実行しようとしているjobが登録jobと同じか，IDチェック
  if (bdi_.p_job_info->job_id != p_job_info->job_id) return BD_STATE_ERR_MISMATCH_JOB;
  // 実行中か？
  if (p_job_info->state != BD_STATE_PROCESSING) return BD_STATE_ERR_CANNOT_EXEC;

  // 停止させる
  p_job_info->state = BD_STATE_ABORTED_BY_CMD;
  return p_job_info->state;
}

static BD_STATE BD_get_job_state_(BigDataJobInfo* p_job_info)
{
  return p_job_info->state;
}


static void BD_init_job_info_(
                              BigDataJobInfo*      p_job_info,
                              BD_MODE              mode,
							  uint32_t             sd_src_block_adr,
							  uint32_t             sd_src_adr,
							  uint32_t             sd_dst_block_adr,
							  uint32_t             sd_dst_adr,
							  uint32_t             erase_block,
					          uint32_t             erase_block_num,
                              uint32_t             len
                            )
{
  p_job_info->job_id             = BD_increment_id_counter_();
  p_job_info->state              = BD_STATE_WAITING_EXEC;
  p_job_info->mode               = mode;
  p_job_info->process_counter    = 0;
  p_job_info->sd_src_block_adr   = sd_src_block_adr;
  p_job_info->src_adr         = sd_src_adr;
  p_job_info->sd_dst_block_adr   = sd_dst_block_adr;
  p_job_info->dst_adr         = sd_dst_adr;
  p_job_info->erase_block        = erase_block;
  p_job_info->erase_block_num    = erase_block_num;
  p_job_info->len                = len;

  p_job_info->err_info.err_state = (BD_PROCESS_ERR)0;
  p_job_info->err_info.err_adr   = 0;
  p_job_info->err_info.err_block = 0;

  p_job_info->rest_len           = 0;
  p_job_info->end_process_counter= 0;
  p_job_info->sd_crc             = 0;
  p_job_info->flash_crc          = 0;

  bdi_.p_job_info = p_job_info;        // ポインタの差し替え！！
}

static uint8_t BD_increment_id_counter_(void)
{
  bdi_.id_counter = (uint8_t)((bdi_.id_counter + 1) % 256);
  return bdi_.id_counter;
}


// ############# コマンド系 #############
CCP_CmdRet Cmd_BD_SET_COPY_SD2SD(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t sd_src_block_adr, sd_src_adr, sd_dst_block_adr, sd_dst_adr, len;

  // パラメータを読み出し
  ENDIAN_memcpy(&sd_src_block_adr, param, 4);
  ENDIAN_memcpy(&sd_src_adr      , param + 4, 4);
  ENDIAN_memcpy(&sd_dst_block_adr, param + 8, 4);
  ENDIAN_memcpy(&sd_dst_adr,       param + 12, 4);
  ENDIAN_memcpy(&len,              param + 16, 4);

  BD_set_copy_sd_to_sd_(&job_info_via_cmd_,
  				        sd_src_block_adr, sd_src_adr,
  						sd_dst_block_adr, sd_dst_adr, len);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_SET_COPY_FLASH2FLASH(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t src_adr, dst_adr, len;

  // パラメータを読み出し
  ENDIAN_memcpy(&src_adr, param, 4);
  ENDIAN_memcpy(&dst_adr, param + 4, 4);
  ENDIAN_memcpy(&len, param + 8, 4);

  BD_set_copy_flash_to_flash_(&job_info_via_cmd_, src_adr, dst_adr, len);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_SET_COPY_SD2FLASH(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t sd_src_block_adr, sd_src_adr, sd_dst_adr, len;

  // パラメータを読み出し
  ENDIAN_memcpy(&sd_src_block_adr, param, 4);
  ENDIAN_memcpy(&sd_src_adr      , param + 4, 4);
  ENDIAN_memcpy(&sd_dst_adr,       param + 8, 4);
  ENDIAN_memcpy(&len,              param + 12, 4);

  BD_set_copy_sd_to_flash_(&job_info_via_cmd_, sd_src_block_adr, sd_src_adr, sd_dst_adr, len);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_SET_COPY_FLASH2SD(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t sd_src_adr, sd_dst_block_adr, sd_dst_adr, len;

  // パラメータを読み出し
  ENDIAN_memcpy(&sd_src_adr      , param, 4);
  ENDIAN_memcpy(&sd_dst_block_adr, param + 4, 4);
  ENDIAN_memcpy(&sd_dst_adr,       param + 8, 4);
  ENDIAN_memcpy(&len,              param + 12, 4);

  BD_set_copy_flash_to_sd_(&job_info_via_cmd_, sd_src_adr, sd_dst_block_adr, sd_dst_adr, len);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_SET_ERASE_FLASH(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t Sector, NbSector;

  // パラメータを読み出し
  ENDIAN_memcpy(&Sector,   param, 4);
  ENDIAN_memcpy(&NbSector, param + 4, 4);

  BD_set_erase_flash_(&job_info_via_cmd_, Sector, NbSector);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_SET_FLASH_CRC(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t sd_src_adr, len;

  // パラメータを読み出し
  ENDIAN_memcpy(&sd_src_adr      , param, 4);
  ENDIAN_memcpy(&len,              param + 4, 4);

  BD_set_calc_flash_crc_(&job_info_via_cmd_, sd_src_adr, len);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_SET_SD_CRC(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t sd_src_block_adr, sd_src_adr, len;

  // パラメータを読み出し
  ENDIAN_memcpy(&sd_src_block_adr, param, 4);
  ENDIAN_memcpy(&sd_src_adr      , param + 4, 4);
  ENDIAN_memcpy(&len,              param + 8, 4);

  BD_set_calc_sd_crc_(&job_info_via_cmd_, sd_src_block_adr, sd_src_adr, len);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_SET_COPY_SD2RAM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t sd_src_block_adr, sd_src_adr, sd_dst_adr, len;

  // パラメータを読み出し
  ENDIAN_memcpy(&sd_src_block_adr, param, 4);
  ENDIAN_memcpy(&sd_src_adr      , param + 4, 4);
  ENDIAN_memcpy(&sd_dst_adr,       param + 8, 4);
  ENDIAN_memcpy(&len,              param + 12, 4);

  BD_set_copy_sd_to_ram_(&job_info_via_cmd_, sd_src_block_adr, sd_src_adr, sd_dst_adr, len);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_SET_RAM_CRC(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t sd_src_adr, len;

  // パラメータを読み出し
  ENDIAN_memcpy(&sd_src_adr      , param, 4);
  ENDIAN_memcpy(&len,              param + 4, 4);

  BD_set_calc_ram_crc_(&job_info_via_cmd_, sd_src_adr, len);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_EXEC_JOB(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t id;

  id = param[0];
  if (bdi_.p_job_info->job_id != id ) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  BD_exec_job_(bdi_.p_job_info);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_ABORT_JOB(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t id;

  id = param[0];
  if (bdi_.p_job_info->job_id != id ) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  BD_abort_job_(bdi_.p_job_info);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_EXEC_JOB_ANYWAY(const CommonCmdPacket* packet)
{
  (void)packet;

  BD_exec_job_(bdi_.p_job_info);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BD_ABORT_JOB_ANYWAY(const CommonCmdPacket* packet)
{
  (void)packet;

  // FLASH_BD_abort_job_(&job_info_via_cmd_);
  BD_abort_job_(bdi_.p_job_info);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/*
 * @brief Erase_Flagをunsetする
 * param1 sector
 * param2 sectorの個数
 */
CCP_CmdRet Cmd_MEM_flash_unset_eraseflag(const CommonCmdPacket* packet){
	uint8_t ret;
	uint8_t sector;
	uint8_t NbSector;

	memcpy(&sector, CCP_get_param_head(packet), 1);
	memcpy(&NbSector, CCP_get_param_head(packet) + 1, 1);

	uint8_t flag;

	for(int i = 0; i < NbSector; i++){
		flag = Get_Erase_Flag(sector);
		printf("flag = %d\r\n", flag);
		Unset_Erase_Flag(sector + i);
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief:	ブートローダのフラッシュメモリパトロールコマンド.
 *          全部一気に確認すると12msecかかる
 */
CCP_CmdRet Cmd_MEM_FlashBoot_patrol(const CommonCmdPacket* packet){
	FLASH_AL_ACK ret;

	for(uint16_t i=0; i<Flash_ReadSize; i++){
		ret = Flash_Redundancy_check(FlashPatrolAddr, (FlashPatrolAddr + 0x00004000), (FlashPatrolAddr + 0x00008000));

		if(ret != BOOT_FLASH_NO_MISMATCH) {
//			AL_add_anomaly(AL_GROUP_BOOT_FLASH, ret);
			boot_mis_address = FlashPatrolAddr;
			boot_error_num = ret;
			generate_memory_repro_tlm();
//			AL_disable_logging(AL_GROUP_BOOT_FLASH);
			return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
		}

		FlashPatrolAddr++;

		if(FlashPatrolAddr >= 0x08004000){
			FlashPatrolAddr = 0x08000000;
		}
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

static void CDH1_flash_param_load(void);
static void CDH1_flash_param_init(void);

AppInfo CDH1_flash_param_update(void)
{
    return AI_create_app_info("DBG_SD", CDH1_flash_param_load, CDH1_flash_param_init);
}

static void CDH1_flash_param_load(void){
	read_param(&Flash_ReadSize, &Flash_ReadSize_, sizeof(Flash_ReadSize), (uint32_t)0x00003E00);
}

static void CDH1_flash_param_init(void){
	write_param(&Flash_ReadSize, &Flash_ReadSize_, sizeof(Flash_ReadSize), (uint32_t)0x00003E00);
}

/*	@brief: パトロールアドレスの変更コマンド
 * 	packet[15~18]: アドレス（ビッグエンディアン）
 */
CCP_CmdRet Cmd_MEM_flash_set_patrol_addr(const CommonCmdPacket* packet){
	uint32_t temp;
	//パラメータを受信
	ENDIAN_memcpy(&temp, CCP_get_param_head(packet), 4);
	//不正なアドレスを除去
	if(temp < 0x08000000 || temp >= 0x08004000){
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	//正常なアドレスであれば変更を適用する
	FlashPatrolAddr = temp;

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

static void Generate_crc_calc_tlm(void){
	TCP packet;
	uint16_t len = 0;
	TF_generate_contents((int)Tlm_CODE_MEM,
				  packet.packet,
				  &len,
				  TCP_MAX_LEN);

	TCP_TLM_setup_primary_hdr(&packet, (TCP_APID)AFSK_GS, len);
	TCP_set_route(&packet, TCP_ROUTE_DIRECT);
	TCP_set_all_issuer_id(&packet, MY_SAT_ID, TCP_EXE_ID_MOBC);
	TCP_TLM_set_startup_count(&packet, (uint8_t)p_reset_struct->reset_count[TOTAL_RESET]);
	TCP_TLM_set_ti(&packet, (uint32_t)(TMGR_get_master_total_cycle()));
	TCP_TLM_set_category(&packet, 0x80);
	TCP_TLM_set_packet_id(&packet, Tlm_CODE_MEM);

	PH_analyze_packet(&packet);
}

