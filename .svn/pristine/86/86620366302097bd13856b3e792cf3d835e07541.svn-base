#pragma section REPRO

#include "block_command_user_settings.h"
#include "src_core/TlmCmd/common_cmd_packet_util.h"

#define BCUS_SRAM_BCT_BLOCKS (BCT_MAX_BLOCKS - BCUS_CCMRAM_BCT_BLOCKS)
#define BCUS_CCMRAM_BCT_BEGIN_IDX BCUS_SRAM_BCT_BLOCKS

static BCT_Table  BCUS_bc_table_[BCUS_SRAM_BCT_BLOCKS];      //!< BCT の blocks の本体
static BCE_Params BCUS_bc_exe_params_[BCUS_SRAM_BCT_BLOCKS]; //!< BCE_Params の本体

#ifndef HAL_SILS
__attribute__((section(".ccmram")))
#endif
static BCT_Table  CCMRAM_BCUS_bc_table_[BCUS_CCMRAM_BCT_BLOCKS];      //!< BCT の blocks の本体(CCMRAM上にのる)

#ifndef HAL_SILS
__attribute__((section(".ccmram")))
#endif
static BCE_Params CCMRAM_BCUS_bc_exe_params_[BCUS_CCMRAM_BCT_BLOCKS]; //!< BCE_Params の本体(CCMRAM上にのる)

void BCUS_load_user_settings(BlockCommandTable* block_command_table, BlockCommandExecutor* block_command_executor)
{
  bct_id_t block;
  int sram_block_idx = 0;

  for (block = 0; block < BCT_MAX_BLOCKS; ++block)
  {
    BCT_Table* bct_adr;
    BCE_Params* bce_adr;
    if(block<BCUS_CCMRAM_BCT_BEGIN_IDX){
    	block_command_table->blocks[block] = &BCUS_bc_table_[block];
    	block_command_executor->bc_exe_params[block] = &BCUS_bc_exe_params_[block];
    }
    else{
        block_command_table->blocks[block] = &CCMRAM_BCUS_bc_table_[sram_block_idx];
        block_command_executor->bc_exe_params[block] = &CCMRAM_BCUS_bc_exe_params_[sram_block_idx];
        sram_block_idx++;
    }
  }
}

int BCUS_bc_should_clear(const bct_id_t block)
{
  (void)block;
  return 1;
}


CCP_CmdRet Cmd_BCUS_CLEAR_USER_SETTINGS(const CommonCmdPacket* packet)
{
//  (void)packet;
//  BCUS_load_user_settings((BlockCommandTable*)block_command_table, (BlockCommandExecutor*)block_command_executor); // const_cast

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BCUS_SWAP_ADDRESS(const CommonCmdPacket* packet)
{
//  const uint8_t* param = CCP_get_param_head(packet);
//  bct_id_t block_a, block_b;
//  BCT_Table* tmp_table_adr;
//  BCE_Params* tmp_params_adr;
//  BCT_ACK ack;
//
//  if (CCP_get_param_len(packet) != 2 * SIZE_OF_BCT_ID_T) return CCP_EXEC_ILLEGAL_LENGTH;
//
//  ENDIAN_memcpy(&block_a, param, SIZE_OF_BCT_ID_T);
//  ENDIAN_memcpy(&block_b, param + SIZE_OF_BCT_ID_T, SIZE_OF_BCT_ID_T);
//
//  ack = BCT_swap_address(block_a, block_b);
//  if (ack != BCT_SUCCESS) return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
//  tmp_table_adr = block_command_table->blocks[block_a];
//  MRAM_set_value(&mram->cdh.bct_address[block_a], &tmp_table_adr, sizeof(uint32_t), 1);
//  tmp_table_adr = block_command_table->blocks[block_b];
//  MRAM_set_value(&mram->cdh.bct_address[block_b], &tmp_table_adr, sizeof(uint32_t), 1);
//
//  ack = BCE_swap_address(block_a, block_b);
//  if (ack != BCT_SUCCESS) return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
//  tmp_params_adr = block_command_executor->bc_exe_params[block_a];
//  MRAM_set_value(&mram->cdh.bc_exe_params[block_a], &tmp_params_adr, sizeof(uint32_t), 1);
//  tmp_params_adr = block_command_executor->bc_exe_params[block_b];
//  MRAM_set_value(&mram->cdh.bc_exe_params[block_b], &tmp_params_adr, sizeof(uint32_t), 1);
//
//  return BCT_convert_bct_ack_to_ctcp_exec_sts(ack);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BCUS_SWAP_CONTENTS(const CommonCmdPacket* packet)
{
//  const uint8_t* param = CCP_get_param_head(packet);
//  bct_id_t block_a, block_b;
//  BCT_ACK ack;
//
//  if (CCP_get_param_len(packet) != 2 * SIZE_OF_BCT_ID_T) return CCP_EXEC_ILLEGAL_LENGTH;
//
//  ENDIAN_memcpy(&block_a, param, SIZE_OF_BCT_ID_T);
//  ENDIAN_memcpy(&block_b, param + SIZE_OF_BCT_ID_T, SIZE_OF_BCT_ID_T);
//
//  ack = BCT_swap_contents(block_a, block_b);
//  if (ack != BCT_SUCCESS) return BCT_convert_bct_ack_to_ctcp_exec_sts(ack);
//  ack = BCE_swap_contents(block_a, block_b);
//
//  return BCT_convert_bct_ack_to_ctcp_exec_sts(ack);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BCUS_SWAP_RAM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  bct_id_t block_a, block_b;
//  BCT_Table* tmp_table_adr;
//  BCE_Params* tmp_params_adr;
  BCT_ACK ack;

  if (CCP_get_param_len(packet) != 2 * SIZE_OF_BCT_ID_T) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);

  ENDIAN_memcpy(&block_a, param, SIZE_OF_BCT_ID_T);
  ENDIAN_memcpy(&block_b, param + SIZE_OF_BCT_ID_T, SIZE_OF_BCT_ID_T);

  ack = BCE_inactivate_block_by_id(block_a);
  if (ack != BCT_SUCCESS) return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
  ack = BCE_inactivate_block_by_id(block_b);
  if (ack != BCT_SUCCESS) return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);

  ack = BCT_swap_address(block_a, block_b);
  if (ack != BCT_SUCCESS){
    // 予期しない動作をしないようにinactiveに
    BCE_inactivate_block_by_id(block_a);
    BCE_inactivate_block_by_id(block_b);
    return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
  }
//  tmp_table_adr = block_command_table->blocks[block_a];
//  MRAM_set_value(&mram->cdh.bct_address[block_a], &tmp_table_adr, sizeof(uint32_t), 1);
//  tmp_table_adr = block_command_table->blocks[block_b];
//  MRAM_set_value(&mram->cdh.bct_address[block_b], &tmp_table_adr, sizeof(uint32_t), 1);

  ack = BCE_swap_address(block_a, block_b);
  if (ack != BCT_SUCCESS){
    // 予期しない動作をしないようにinactiveに
    BCE_inactivate_block_by_id(block_a);
    BCE_inactivate_block_by_id(block_b);
    return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
  }
//  tmp_params_adr = block_command_executor->bc_exe_params[block_a];
//  MRAM_set_value(&mram->cdh.bc_exe_params[block_a], &tmp_params_adr, sizeof(uint32_t), 1);
//  tmp_params_adr = block_command_executor->bc_exe_params[block_b];
//  MRAM_set_value(&mram->cdh.bc_exe_params[block_b], &tmp_params_adr, sizeof(uint32_t), 1);

  WDT_clear_wdt(); // 引っかかったので

  ack = BCT_swap_contents(block_a, block_b);
  if (ack != BCT_SUCCESS) return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
  ack = BCE_swap_contents(block_a, block_b);

  BCE_activate_block_by_id(block_a);
  BCE_activate_block_by_id(block_b);

  return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
//  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//モードに関するBCだけ移し替える
//アクティブ, 非アクティブ化を同時に行う
CCP_CmdRet Cmd_BCUS_SWAP_MODE(const CommonCmdPacket* packet){
//	const uint8_t* param = CCP_get_param_head(packet);
//	bct_id_t block_a, block_b;
//	BCT_ACK ack;
//
//	if (CCP_get_param_len(packet) != 2 * SIZE_OF_BCT_ID_T) return BCT_convert_bct_ack_to_ccp_cmd_ret(CCP_EXEC_ILLEGAL_LENGTH);
//
//	ENDIAN_memcpy(&block_a, param, SIZE_OF_BCT_ID_T);
//	ENDIAN_memcpy(&block_b, param + SIZE_OF_BCT_ID_T, SIZE_OF_BCT_ID_T);
//
////	if(block_a > BC_TL_MISSION3) return BCT_convert_bct_ack_to_ccp_cmd_ret(CCP_EXEC_ILLEGAL_PARAMETER);
//
//	ack = BCE_inactivate_block_by_id(block_a);
//	if (ack != BCT_SUCCESS) return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
//	ack = BCE_inactivate_block_by_id(block_b);
//	if (ack != BCT_SUCCESS)
//	{//アクティブに戻してから終了
//		BCE_activate_block_by_id(block_a);
//		BCE_activate_block_by_id(block_b);
//		return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
//	}
//
//	ack = BCT_swap_contents(block_a, block_b);
//	if (ack != BCT_SUCCESS)
//	{//予期しない動作をしないようにinactiveに
//		BCE_inactivate_block_by_id(block_a);
//		BCE_inactivate_block_by_id(block_b);
//		return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
//	}
//	ack = BCE_swap_contents(block_a, block_b);
//	if (ack != BCT_SUCCESS)
//	{//予期しない動作をしないようにinactiveに
//		BCE_inactivate_block_by_id(block_a);
//		BCE_inactivate_block_by_id(block_b);
//		return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
//	}
//
//	BCE_activate_block_by_id(block_a);
//	BCE_activate_block_by_id(block_b);
//
//	return BCT_convert_bct_ack_to_ccp_cmd_ret(ack);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
