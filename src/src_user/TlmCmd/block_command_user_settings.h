#ifndef BLOCK_COMMAND_USER_SETTINGS_H_
#define BLOCK_COMMAND_USER_SETTINGS_H_

#include <src_core/TlmCmd/block_command_table.h>
#include <src_core/TlmCmd/block_command_executor.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>

#define BCUS_MRAM_BCT_BLOCKS (10) // MRAMに確保する
#define BCUS_CCMRAM_BCT_BLOCKS (70) // MRAMに確保する

/**
 * @brief BCT, BCE に関してのメモリ確保や getter, setter を user settings として上書きする
 * @param[in] block_command_table: 初期化する BlockCommandTable*
 * @param[in] block_command_executor: 初期化する BlockCommandExecutor*
 * @note SRAM, 単一ではなく MRAM上, 三重冗長で置くなどの際に上書きで使用
 */
void BCUS_load_user_settings(BlockCommandTable* block_command_table, BlockCommandExecutor* block_command_executor);

/**
 * @brief その block をクリアすべきかどうか判別する
 * @param[in] block: BC の id
 * @return 0: No, 1: Yes
 * @note 不揮発であれば clear しない. 揮発であれば clear する.
 */
int BCUS_bc_should_clear(const bct_id_t block);

CCP_CmdRet Cmd_BCUS_CLEAR_USER_SETTINGS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCUS_SWAP_ADDRESS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCUS_SWAP_CONTENTS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCUS_SWAP_RAM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCUS_SWAP_MODE(const CommonCmdPacket* packet);

#endif
