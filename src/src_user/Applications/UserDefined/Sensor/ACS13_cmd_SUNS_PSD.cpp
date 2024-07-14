/*
 * ACS13_cmd_SUNS_PSD.cpp
 *
 *  Created on: 2023/09/07
 *      Author: Ji Hyun Park
 *   Reference:
 */

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS_PSD.h>
#include <src_user/Drivers/SENSOR/PSD.h>
//#include <src_user/Drivers/Power/PWR_State.h>
#include <stdio.h> //printf

static PSD_DRIVER_STRUCT psd_driver[5];
const PSD_DRIVER_STRUCT* psd_driver_ = psd_driver;

#ifdef TWO_U

    // CSピンのリスト ポート
//    GPIO_TypeDef *PSD_CS_PORTS[] = { CS_SPI3V3_MOBCtoSUNSMX_GPIO_Port };
    GPIO_TypeDef *PSD_CS_PORTS[] = { CS_SPI3V3_MOBCtoSUNSMY_GPIO_Port, CS_SPI3V3_MOBCtoSUNSPZ_GPIO_Port }; // for thermal test　20240418

    // CSピンのリスト ピン
//    uint16_t PSD_CS_PINS[] = { CS_SPI3V3_MOBCtoSUNSMX_Pin };
    uint16_t PSD_CS_PINS[] = { CS_SPI3V3_MOBCtoSUNSMY_Pin,  CS_SPI3V3_MOBCtoSUNSPZ_Pin}; // for thermal test　20240418

#endif /*for 2U*/

#ifdef ONE_U

    // CSピンのリスト ポート
    GPIO_TypeDef *PSD_CS_PORTS[] = { CS_SPI3V3_MOBCtoSUNSMX_GPIO_Port };
    // CSピンのリスト ピン
    uint16_t PSD_CS_PINS[] = { CS_SPI3V3_MOBCtoSUNSMX_Pin };

#endif /*for 1U*/

static void ACS13_PSD_init_(void);
static void ACS13_PSD_update_(void);

/**
 * @brief アプリケーション作成関数
 * @param  void
 * @return アプリケーション構造体
 */
AppInfo ACS13_PSD_create_app(void)
{
  return AI_create_app_info("sample_code", ACS13_PSD_init_, ACS13_PSD_update_);
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void ACS13_PSD_init_(void){
  // ▼変数宣言
//  (void)data;         // Command Parameterを使用しない
//  (void)datalength;

  // ▼処理
#ifdef TWO_U
    int suns_num = 2;
#endif
#ifdef ONE_U
    int suns_num = 1;
#endif
    // 全てのサンセンサのインスタンスを生成
    for (int i = 0; i <  suns_num; i++) {
      PSD_init(&psd_driver[i], (void*)&hspi4, (void*)PSD_CS_PORTS[i], PSD_CS_PINS[i]);

      // FETついていないから絶対SPIエラーになる？
      function_EnCrystal(&psd_driver[i]);

//      function_SetRate(&psd_driver[i]);//EnCrystalの方にまとめたのでコメントアウト


    }
    printf("PSD initialized\r\n");
}

/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void ACS13_PSD_update_(void)
{
#ifdef TWO_U
    int suns_num = 2;

#endif
#ifdef ONE_U
    int suns_num = 1;
#endif

    // PSDのFETがONのとき
//    if (PWR_State_GET(PWR_State_SUNS)){
//      printf("PSD FET ON\r\n");

        for (int i = 0; i < suns_num; i++) {
//
          function_EnCrystal(&psd_driver[i]);
//
//          function_SetRate(&psd_driver[i]);//レジスタ書き換わらない限りずっとやる必要ない

          function_GetMeas(&psd_driver[i]);

        }
//    }
//    printf("PSD executed\r\n");

} // end of sun sensor main code
