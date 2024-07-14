/*
 * sample_code.h
 *
 *  Created on: 2022/07/06
 *      Author: fujita
 */

#ifndef SAMPLE_CODE_H_
#define SAMPLE_CODE_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>


/**
 * @struct SAMPLE_CODE_HogeHoge
 * @brief  sample
 */
typedef struct
{
  uint8_t hoge; // hogeの説明　変数の説明は右側に記述する
} SAMPLE_CODE_HogeHoge;

/**
 * @enum SAMPLE_CODE_CASE
 * @brief  sample
 */
typedef enum {
	SAMPLE_CASE_HOGEHOGE,
	SAMPLE_CASE_HUGAHUGA,
	SAMPLE_CASE_HOGEHUGA
} SAMPLE_CODE_CASE;

/**
 * @struct SampleCode
 * @brief  sample
 */
typedef struct
{
  uint8_t test;                   // testの説明
  float test_test;                // test_testの説明
  uint8_t is_inited;              // is_initedの説明
  SAMPLE_CODE_HogeHoge hogehoge;  // 構造体は説明を省略可
} SAMPLE_CODE_STRUCT;

extern const SAMPLE_CODE_STRUCT* const sample_code;

AppInfo CDH_sample_code_create_app(void);

CCP_CmdRet Cmd_CDH_SAMPLE_CODE_TEST(const CommonCmdPacket* packet);

#endif // SAMPLE_CODE_H_
