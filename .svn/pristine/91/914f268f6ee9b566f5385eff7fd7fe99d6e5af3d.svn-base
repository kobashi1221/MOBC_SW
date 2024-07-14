#ifndef SUBCAM_H_
#define SABCAM_H_
#ifdef ONE_U

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "SUBCAM_Param.h"
#include "SUBCAM_regs.h"
#include "SUBCAM_Struct.h"
#include "main.h"
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_user/Drivers/Cdh/SD/SD.h>
#include "src_user/Applications/Middleware/MemoryMW.h"
#include <src_user/Applications/UserDefined/Memory/MEM_cmd_SD.h>
#include <src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.h>
#include <src_user/Applications/UserDefined/Cdh/ResetUtility.h>
#include <src_user/Applications/UserDefined/GLOBAL_AOCS_Value.h>
#include <src_user/Drivers/Com/TXPIC/TXPIC.h>
#include <src_user/Drivers/Com/RXPIC/RXPIC.h>
#include <src_user/Applications/UserDefined/Cdh/RTC_handler.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>

//extern DCMI_HandleTypeDef hdcmi;

/* USER CODE BEGIN PD */
#define CAMERA_FRAME_BUFFER                  0x60000000UL
#define CAMERA_FRAME_ASCII                   0x60010000UL //Park-san add
//#define IMAGE_SIZE	0x1400
//at 160 * 128  with processed bayer + RGB?
//   actually 80 * 128 is coming in
// 80 * 128 * 2 / 4 =

//#define IMAGE_SIZE	0x5000
//at 320 * 256 with processed bayer + RGB?

#define IMAGE_SIZE	0x2800

//0x14000 * 4 bytes = 327680 byte DMA buffer
//actual size image 640 512  = 655360 byte

#define SRAM_BANK_ADDR		0x60000000
#define WRITE_READ_ADDR		((uint32_t)0x0000)  //((uint32_t)0x0800)
#define THUMBNAIL_ADDR		0x00077000
/* USER CODE END PD */

extern Global_ADS_STRUCT g_ads;
extern Global_ACS_STRUCT g_acs;
extern Global_ODS_STRUCT g_ods;
extern Global_OCS_STRUCT g_ocs;
extern RXPIC_STRUCT* const rxpic_driver;
extern TXPIC_STRUCT* const txpic_driver;
extern RTC_Info* const p_rtc_struct;
extern Global_SENSOR_STRUCT g_sensor;


#ifdef __cplusplus
extern "C"{
#endif

 void SUBCAM_Init();
uint8_t subcam_prepare();
uint8_t subcam_init();
uint8_t subcam_reset();
HAL_StatusTypeDef subcam_read_reg(uint8_t reg_addr, uint8_t* pdata);
uint8_t subcam_write_reg(uint8_t reg_addr, uint8_t reg_data);

void subcam_set_Framesize(uint8_t param);
void subcam_set_Saturation(int level);
void subcam_set_Brightness(int brightness);
void subcam_set_Contrast(int contrast);
int  subcam_set_colorbar(bool enable);
void subcam_set_AutoExposure(bool enable, int exp_us);
void subcam_set_Exposure(bool enable, int exp_us);
void subcam_set_reg();

void subcam_bmp_capture(uint8_t param);
uint8_t subcam_configuration();
uint8_t subcam_take_picture();
int outputDCMI();
void RGB24_to_thumbnail(uint8_t param);
void save_SD_BOA(uint32_t *BOA_data, uint32_t size);

void SCAM_default_setting(void);

#ifdef __cplusplus
}
#endif

#endif /* for 1U*/
#endif /* SABCAM_H_ */

