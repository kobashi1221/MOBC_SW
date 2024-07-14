/**
 * @file
 * @brief  BCTのCoreTlmパラメタのオーバーライド用ヘッダー
 */
#ifndef BLOCK_COMMAND_TABLE_PARAMS_H_
#define BLOCK_COMMAND_TABLE_PARAMS_H_

#undef BCT_CMD_MAX_LENGTH
#undef BCT_MAX_CMD_NUM
#undef BCT_MAX_BLOCKS

#undef SIZE_OF_BCT_ID_T

#define BCT_CMD_MAX_LENGTH   (36)
#define BCT_MAX_CMD_NUM      (24)
#define BCT_MAX_BLOCKS       (100)

#define SIZE_OF_BCT_ID_T     (2)

#define BCT_TLM_PAGE_SIZE   (12)
#define BCT_TLM_PAGE_MAX    (2)

#endif
