/**
  *****************************************************************************************
  * @file    : SUBCAM.cpp
  * @author  : Ji Hyun Park,  Tamaoki Naoya
  * @version : 1.1
  * @date    : 2021/12/05
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2021/10/23  1.0   Naoya TAMAOKI              First revision
  * 2021/12/05  1.1   Naoya TAMAOKI
  *****************************************************************************************
  */

#ifdef ONE_U
#include "SUBCAM.h"
#include <src_user/Library/Delay.h> //c2a_delay_ms()のあるライブラリを呼び出す
#include <stdio.h>
#include <string.h>
#include <src_user/Drivers/Cdh/SD/stm32_adafruit_sd.h>
#include <src_user/IfWrapper/HAL.h>
#include "main.h"
#include <src_user/Library/General/DBG_printf.h>
#include <src_core/Library/endian.h>
#include <src_user/Drivers/Cdh/WDT/wdt_manager.h>
#include <src_core/TlmCmd/telemetry_frame.h>

static HAL_StatusTypeDef ret_I2C = HAL_ERROR;

#define DevAddress_Write 0x0061
#define DevAddress_Read  0x0060
#define MemAddres_Size 0x0002
#define OMV_XCLK_FREQUENCY (12000000)


static void *p_subcam_i2c_handle_;
static uint32_t timeout;

static uint8_t is_picture;
static uint8_t error_dcmi = 1;

static size_t SD_size = 512;
static uint8_t send_content[512] = {0};
static uint8_t info_buf[512*2] = {0};
static uint8_t stamp_buf[512] = {0};
static uint32_t SCAM_addr_now = 0;
static uint8_t ret_w = 0;
static uint8_t ret_r = 0;

static uint32_t exposure;

uint32_t s_dstAddress = (uint32_t)(SRAM_BANK_ADDR + WRITE_READ_ADDR);
uint8_t* p_dstAddress = (uint8_t*)(s_dstAddress);
static OV2640_STRUCT SCAM_driver_;
OV2640_STRUCT* const SCAM_driver = &SCAM_driver_;

//for MRAM param
extern uint8_t  ov2640_regs_def[33];
extern uint8_t  ov2640_regs_rgb565[4];
extern uint8_t  ov2640_SD_block[1];
extern uint16_t ov2640_scaled_size[2];
extern uint8_t  ov2640_regs_size[1];
extern uint8_t  ov2640_color_thred[2];
extern uint8_t  ov2640_scaled_method[1];

void SUBCAM_Init(){
	p_subcam_i2c_handle_ = &hi2c3;
	timeout = 0xff;
}

/**
 * @brief  read register by using I2C
 * @param[in]   reg_addr : address of OV2640
 * @param[out]  pdata : register value of OV2640
 * @return  HAL_StatusTypeDef ret1
 */
HAL_StatusTypeDef subcam_read_reg(uint8_t reg_addr, uint8_t* pdata){
    HAL_StatusTypeDef ret1 = HAL_ERROR;
    HAL_StatusTypeDef ret2 = HAL_ERROR;
	for(int i=0; i<3; i++) {
		if (ret1 != HAL_OK){
			ret2 = (HAL_StatusTypeDef)HAL_I2C_Master_Transmit_C2A(p_subcam_i2c_handle_, DevAddress_Read, &reg_addr, 1, timeout);
	//        usleep(10);
			ret1 = (HAL_StatusTypeDef)(HAL_I2C_Master_Receive_C2A(p_subcam_i2c_handle_, DevAddress_Read, pdata, 1, timeout) | ret2);

		}
	}
	if(ret1 != HAL_OK || ret2 != HAL_OK){
		printf("ret1 = %d\n", ret1);
		printf("ret2 = %d\n", ret2);
	}
	return ret1;
}

/**
 * @brief  write register by using I2C
 * @param[in]  reg_addr : address of OV2640
 * @param[in]  reg_data : register value of OV2640
 * @return  HAL_StatusTypeDef ret
 */
int cnt=0;
uint8_t subcam_write_reg(uint8_t reg_addr, uint8_t reg_data){
	HAL_StatusTypeDef ret = HAL_ERROR;
	for(int i=0; i<10; i++) {
		if (ret != HAL_OK){
			ret = (HAL_StatusTypeDef)HAL_I2C_Mem_Write_C2A(p_subcam_i2c_handle_, DevAddress_Read, reg_addr, 1, &reg_data, 1, timeout);
		}
	}

	if(cnt%100 == 0){
		WDT_clear_count();
	}

	cnt++;
	if(ret != HAL_OK){
      printf("write_reg err(I2C)\r\n");
      printf("count = %d, reg_addr = 0x%02x, ret = %d\n", cnt, reg_addr, ret);
	}
	HAL_Delay_C2A(10);

	return ret;
}

/**
 * @brief  set GPIO(power), RESET pin, and STANDBY pin of SCAM
 *     and confirm communication with SCAM by I2C
 * @return  none
 */
uint8_t subcam_init(){
	uint8_t regData = 0;
	HAL_GPIO_WritePin_C2A(GPIOE, RESET3V3_SCAM_Pin, GPIO_PIN_RESET);
	HAL_Delay_C2A(10);
	HAL_GPIO_WritePin_C2A(GPIOE, RESET3V3_SCAM_Pin, GPIO_PIN_SET);
	HAL_Delay_C2A(10);
	HAL_GPIO_WritePin_C2A(GPIOE, PWDN3V3_SCAM1U_Pin, GPIO_PIN_RESET);
	HAL_Delay_C2A(10);

	ret_I2C = subcam_read_reg(0x0a, (uint8_t*) &regData);
	printf("pid is 0x%02x\n", regData); //pid value should be 0x26
	ANOMALY_I2C_Set_Status(I2C_FAULT_SCAM, ret_I2C);
	if (regData != 0x26 || ret_I2C != HAL_OK){
		printf("raed_reg err(I2C)\r\n");
    }
	else{
		printf("I2C OK\r\n");
		subcam_read_reg(0x0b, (uint8_t*) &regData);
		printf("ver is 0x%02x\n", regData); //ver value should be 0x42
	}

	uint8_t ret = ret_I2C;

	// Stop DCMI clear buffer
	HAL_DCMI_Stop(&hdcmi);
	HAL_Delay_C2A(10); //if error->30ms

	return ret;
}

/**
 * @brief  set RESET pin, and STANDBY pin of SCAM
 *     and move to "Standby" mode of SCAM
 * @return  none
 */
uint8_t subcam_reset() {
  HAL_GPIO_WritePin_C2A(GPIOE, RESET3V3_SCAM_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin_C2A(GPIOE, PWDN3V3_SCAM1U_Pin, GPIO_PIN_SET);
  return 0;
}

/**
 * @brief  set frame size
 * @param[in]  param : enum value of size (MRAM parameters)
 * @return  none
 */
void subcam_set_Framesize(uint8_t param){
	uint8_t ret = 0;
	if (ret_I2C == HAL_OK){
		uint16_t w = resolution[param][0];
		uint16_t h = resolution[param][1];
		uint16_t sensor_w = 0;
		uint16_t sensor_h = 0;

		subcam_write_reg(BANK_SEL,  BANK_SEL_SENSOR);
		subcam_write_reg(COM7,  COM7_RES_UXGA);
		subcam_write_reg(COM1,   0x0F | 0x80);
		subcam_write_reg(HSTART,   0x11);
		subcam_write_reg(HSTOP,   0x75);
		subcam_write_reg(VSTART,  0x01); // 0x01 fixes issue with garbage pixels in the image...
		subcam_write_reg(VSTOP,  0x97);
		subcam_write_reg(REG32,  0x36);
		subcam_write_reg(BANK_SEL,  BANK_SEL_DSP);
		subcam_write_reg(0xe0,  RESET_DVP); //{RESET, RESET_DVP}
//	    write_reg(ov2640_regs0[13],  (uint8_t)(SIZEL_HSIZE8_11_SET(UXGA_WIDTH) | SIZEL_HSIZE8_SET(UXGA_WIDTH) | SIZEL_VSIZE8_SET(UXGA_HEIGHT))); //??
		subcam_write_reg(HSIZE8,  0xc8);
		subcam_write_reg(VSIZE8,  0x96);
		subcam_write_reg(CTRL2,  0x3d); // {CTRL2, CTRL2_DCW_EN | CTRL2_SDE_EN | CTRL2_UV_AVG_EN | CTRL2_CMX_EN | CTRL2_UV_ADJ_EN}
        sensor_w = UXGA_WIDTH;
        sensor_h = UXGA_HEIGHT;
//    }

		 uint64_t tmp_div = IM_MIN(sensor_w/w, sensor_h/h);
		 uint16_t log_div = IM_MIN(IM_LOG2(tmp_div)-1, 3);
		 uint16_t div = 1 << log_div;
		 uint16_t max_x = w * div;
		 uint16_t max_y = h * div;
		 uint16_t off_x = (sensor_w - max_x) / 2;
		 uint16_t off_y = (sensor_h - max_y) / 2;

		 subcam_write_reg(CTRLI, (uint8_t)(CTRLI_LP_DP | CTRLI_V_DIV_SET(log_div) | CTRLI_H_DIV_SET(log_div)));
		 subcam_write_reg(HSIZE, HSIZE_SET(max_x));
		 subcam_write_reg(VSIZE, VSIZE_SET(max_y));
		 subcam_write_reg(XOFFL, XOFFL_SET(off_x));
		 subcam_write_reg(YOFFL, YOFFL_SET(off_y));
		 subcam_write_reg(VHYX, VHYX_HSIZE_SET(max_x)|VHYX_VSIZE_SET(max_y)|VHYX_XOFF_SET(off_x)|VHYX_YOFF_SET(off_y));
		 subcam_write_reg(0x57, TEST_HSIZE_SET(max_x)); //0x57:TEST
		 subcam_write_reg(ZMOW, ZMOW_OUTW_SET(w));
		 subcam_write_reg(ZMOH, ZMOH_OUTH_SET(h));
		 subcam_write_reg(ZMHH, ZMHH_OUTW_SET(w) | ZMHH_OUTH_SET(h));
		 subcam_write_reg(R_DVP_SP, div); //R_DVP_SP
		 subcam_write_reg(0xe0, 0x00); //RESET

//		 uint8_t regData;
//		 subcam_read_reg(CTRLI, (uint8_t*) &regData);
//		 printf("CTRLI : 0x%02x\n", regData);
//		 subcam_read_reg(HSIZE, (uint8_t*) &regData);
//		 printf("HSIZE : 0x%02x\n", regData);
//		 subcam_read_reg(VSIZE, (uint8_t*) &regData);
//		 printf("VSIZE : 0x%02x\n", regData);
//		 subcam_read_reg(XOFFL, (uint8_t*) &regData);
//		 printf("XOFFL : 0x%02x\n", regData);
//		 subcam_read_reg(YOFFL, (uint8_t*) &regData);
//		 printf("YOFFL : 0x%02x\n", regData);
//		 subcam_read_reg(VHYX, (uint8_t*) &regData);
//		 printf("VHYX : 0x%02x\n", regData);
//		 subcam_read_reg(ZMOW, (uint8_t*) &regData);
//		 printf("ZMOW : 0x%02x\n", regData);
//		 subcam_read_reg(ZMOH, (uint8_t*) &regData);
//		 printf("ZMOH : 0x%02x\n", regData);
//		 subcam_read_reg(ZMHH, (uint8_t*) &regData);
//		 printf("ZMHH : 0x%02x\n", regData);
//		 subcam_read_reg(R_DVP_SP, (uint8_t*) &regData);
//		 printf("R_DVP_SP : 0x%02x\n", regData);
	}
}

/**
 * @brief  set saturation(彩度)
 * @param[in]  level : MRAM parameters (level of saturation(1:-2, 2:-1, 3:0, 4:+1, 5:+2))
 * @return  none
 */
void subcam_set_Saturation(int level){
	if (ret_I2C == HAL_OK){
		level += (NUM_SATURATION_LEVELS / 2) + 1;
		if (level <= 0 || level > NUM_SATURATION_LEVELS) {
			return;
		}

		subcam_write_reg(BANK_SEL, BANK_SEL_DSP);
		for(uint32_t i=0; i<sizeof(saturation_regs[0])/sizeof(saturation_regs[0][0]); i++){
			subcam_write_reg(saturation_regs[0][i], saturation_regs[level][i]);
		}
	}
}

/**
 * @brief  set brightness(明度)
 * @param[in]  brightness : MRAM parameters (level of brightness(1:-2, 2:-1, 3:0, 4:+1, 5:+2))
 * @return  none
 */
void subcam_set_Brightness(int brightness){
	if (ret_I2C == HAL_OK){
		brightness += (NUM_BRIGHTNESS_LEVELS / 2)+1;
		if (brightness <= 0 || brightness > NUM_BRIGHTNESS_LEVELS) {
			return;
		}

		subcam_write_reg(BANK_SEL, BANK_SEL_DSP);
		for(uint32_t i=0; i<sizeof(brightness_regs[0])/sizeof(brightness_regs[0][0]); i++){
			subcam_write_reg(brightness_regs[0][i], brightness_regs[brightness][i]);
		}
	}
}

/**
 * @brief  set contrast
 * @param[in]  contrast : MRAM parameters (level of contrast(1:-2, 2:-1, 3:0, 4:+1, 5:+2))
 * @return  none
 */
void subcam_set_Contrast(int contrast){
	if (ret_I2C == HAL_OK){
		contrast += (NUM_CONTRAST_LEVELS / 2)+1;
		if (contrast <= 0 || contrast > NUM_CONTRAST_LEVELS) {
			return;
		}

		subcam_write_reg(BANK_SEL, BANK_SEL_DSP);
		for(uint32_t i=0; i<sizeof(contrast_regs[0])/sizeof(contrast_regs[0][0]); i++){
			subcam_write_reg(contrast_regs[0][i], contrast_regs[contrast][i]);
		}
	}
}

/**
 * @brief  set colorbar
 * @param[in] enable : MRAM parameters (enable=1 : colorbar(use for only debug), enable=0 : not colorbar(usual))
 * @return  none
 */
int subcam_set_colorbar(bool enable){
	if (ret_I2C == HAL_OK){
		uint8_t tmp;
		subcam_write_reg(BANK_SEL, BANK_SEL_SENSOR);
		subcam_read_reg(COM7, &tmp);
		if( enable ){
			subcam_write_reg(COM7, (tmp | COM7_COLOR_BAR));
		}else{
			subcam_write_reg(COM7, (tmp & (~COM7_COLOR_BAR)));
		}
	}
	return 0;
}

/**
 * @brief  set exposure time(露光時間)
 * @param[in]  enable : MRAM parameters (enable=1 : auto, enable=0 :manual))
 * @param[in]  exp_us : MRAM parameters (exposure time [us])
 * @return  none
 */
//AEC:Auto Exposure Control
void subcam_set_AutoExposure(bool enable, int exp_us){
	if (ret_I2C == HAL_OK){
		uint8_t tmp;
		uint8_t tmp1;
		uint8_t tmp2;
		uint8_t tmp3;
		subcam_write_reg(BANK_SEL, BANK_SEL_SENSOR);
		subcam_read_reg(COM8, &tmp);
		subcam_write_reg(COM8, COM8_SET_AEC(tmp, (enable != 0)));

		if ((enable==0) && (exp_us >= 0)) {
			subcam_read_reg(COM7, &tmp);
			int t_line = 0;

			if (COM7_GET_RES(tmp) == COM7_RES_UXGA) t_line = 1600 + 322;
			if (COM7_GET_RES(tmp) == COM7_RES_SVGA) t_line = 800 + 390;
			if (COM7_GET_RES(tmp) == COM7_RES_CIF) t_line = 400 + 195;

			subcam_read_reg(CLKRC, &tmp);
			int pll_mult = ((tmp & CLKRC_DOUBLE) ? 2 : 1) * 3;
			int clk_rc = (tmp & CLKRC_DIVIDER_MASK) + 2;

			subcam_write_reg(BANK_SEL, BANK_SEL_DSP);
			subcam_read_reg(IMAGE_MODE, &tmp);
			int t_pclk = 2;

	//        if (IMAGE_MODE_GET_FMT(tmp) == IMAGE_MODE_YUV422) t_pclk = 2;
	//        if (IMAGE_MODE_GET_FMT(tmp) == IMAGE_MODE_RAW10) t_pclk = 1;
	//        if (IMAGE_MODE_GET_FMT(tmp) == IMAGE_MODE_RGB565) t_pclk = 2;

			exposure = IM_MAX(IM_MIN(((exp_us*(((OMV_XCLK_FREQUENCY/clk_rc)*pll_mult)/1000000))/t_pclk)/t_line, 0xFFFF), 0x0000);
			printf("exp_time(set) %d ", exposure);

			subcam_write_reg(BANK_SEL, BANK_SEL_SENSOR);

			subcam_read_reg(REG04, &tmp1);
			subcam_write_reg(REG04, ((tmp1 & 0xFC) | ((exposure >> 0) & 0x3)));
			subcam_read_reg(AEC, &tmp2);
			subcam_write_reg(AEC, ((tmp2 & 0x00) | ((exposure >> 2) & 0xFF)));
			subcam_read_reg(REG45, &tmp3);
			subcam_write_reg(REG45, ((tmp3 & 0xC0) | ((exposure >> 10) & 0x3F)));
			double  T_EX = exposure * 1922 * 27.78 / (1.0e6); //UXGA mode
			printf("T_EX = %lg[ms]\n", T_EX);
		}
	}
}

void subcam_set_Exposure(bool enable, int exp_us){
	if (ret_I2C == HAL_OK){
    uint8_t tmp;
    exposure = exp_us;

    subcam_write_reg(BANK_SEL, BANK_SEL_SENSOR);
    subcam_read_reg(COM8, &tmp);
    subcam_write_reg(COM8, COM8_SET_AEC(tmp, (enable != 0)));

    subcam_write_reg(BANK_SEL, BANK_SEL_SENSOR);
	subcam_read_reg(REG04, &tmp);
	subcam_write_reg(REG04, ((tmp & 0xFC) | ((exposure >> 0) & 0x3)));
	subcam_read_reg(AEC, &tmp);
	subcam_write_reg(AEC, ((tmp & 0x00) | ((exposure >> 2) & 0xFF)));
	subcam_read_reg(REG45, &tmp);
	subcam_write_reg(REG45, ((tmp & 0xC0) | ((exposure >> 10) & 0x3F)));
    printf("exp_time(manual) %d ", exposure);
    double  T_EX = exposure * 1922 * 27.78 / (1.0e6); //UXGA mode
    printf("T_EX = %lg[ms]\n", T_EX);
	}
}

/**
 * @brief  set basic registers by using I2C (原則、値は変えない?)
 * @return  none
 */
void subcam_set_reg() {
	if (ret_I2C == HAL_OK){
//		int ret;
		int m_fmt = 0;

		// Software reset: reset all registers to default values
		// (But, these settings cause I2C write error.)
//		subcam_write_reg(BANK_SEL, BANK_SEL_SENSOR);
//		subcam_write_reg(COM7, 0x80);
//		HAL_Delay_C2A(10);

//	        delay(100);
		if (m_fmt == 1){
	   /* 	ret = write_regs(default_regs);
			ret = write_regs(yuv422_regs);
			ret = write_regs(jpeg_regs);
			ret = subcam_write_reg(ov2640_regs0[79], ov2640_regs0[81]);
			subcam_write_reg(0x15, 0x00);
			write_regs(svga_regs);
			//subcam_write_reg(0xff, 0x00);
			//subcam_write_reg(0x44, 0x32);    */
		}
		else{
//	        	write_regs(default_regs);
			// From Linux Driver.

			//CLK_reg at first
			subcam_write_reg(BANK_SEL, 	BANK_SEL_SENSOR);
			subcam_write_reg(0x3c,     	0x32);
//			subcam_write_reg(CLKRC, 	ov2640_regs_def[0]); //{CLKRC, CLKRC_DOUBLE}
			subcam_write_reg(CLKRC, 	0x06); //When value is large, communication speed is low. 6 is best?

			subcam_write_reg(BANK_SEL,	BANK_SEL_DSP);
			subcam_write_reg(0x2c,     	0xff);
			subcam_write_reg(0x2e,     	0xdf); //(cause error)

			//CLK_reg again
			subcam_write_reg(BANK_SEL, 	BANK_SEL_SENSOR);
			subcam_write_reg(0x3c,     	0x32);
//			subcam_write_reg(CLKRC, 	ov2640_regs_def[0]); //{CLKRC, CLKRC_DOUBLE}
			subcam_write_reg(CLKRC, 	0x06);//When value is large, communication speed is low. 6 is best?

			subcam_write_reg(COM2, 		ov2640_regs_def[1]); //{COM2, COM2_OUT_DRIVE_3x}
	//    subcam_write_reg(REG04,	REG04_SET(REG04_HFLIP_IMG | REG04_VFLIP_IMG | REG04_VREF_EN | REG04_HREF_EN));
			subcam_write_reg(REG04, 	ov2640_regs_def[2]); //{REG04, REG04_SET(REG04_VFLIP_IMG | REG04_VREF_EN | REG04_HREF_EN)}
			subcam_write_reg(COM8, 		ov2640_regs_def[3]); //{COM8, COM8_SET(COM8_BNDF_EN | COM8_AGC_EN | COM8_AEC_EN)}
			subcam_write_reg(COM9, 		ov2640_regs_def[4]); //{COM9, COM9_AGC_SET(COM9_AGC_GAIN_8x)}
			subcam_write_reg(0x2c,     	0x0c);
			subcam_write_reg(0x33,     	0x78);
			subcam_write_reg(0x3a,     	0x33);
			subcam_write_reg(0x3b,     	0xfb);
			subcam_write_reg(0x3e,     	0x00);
			subcam_write_reg(0x43,    	0x11);
			subcam_write_reg(0x16,    	0x10);
			subcam_write_reg(0x39,     	0x02);
			subcam_write_reg(0x35,     	0x88);
			subcam_write_reg(0x22,     	0x0a);
			subcam_write_reg(0x37,     	0x40);
			subcam_write_reg(0x23,     	0x00);
			subcam_write_reg(ARCOM2,	ov2640_regs_def[5]);
			subcam_write_reg(0x06,    	0x02);
			subcam_write_reg(0x06,     	0x88);
			subcam_write_reg(0x07,     	0xc0);
			subcam_write_reg(0x0d,     	0xb7);
			subcam_write_reg(0x0e,     	0x01);
			subcam_write_reg(0x4c,     	0x00);
			subcam_write_reg(0x4a,     	0x81);
			subcam_write_reg(0x21,     	0x99);
			subcam_write_reg(AEW, 			ov2640_regs_def[6]);
			subcam_write_reg(AEB, 			ov2640_regs_def[7]);
			subcam_write_reg(VV, 			ov2640_regs_def[8]);  //{VV, VV_AGC_TH_SET(0x08, 0x02)},
			subcam_write_reg(0x5c,     	0x00);
			subcam_write_reg(0x63,     	0x00);
			subcam_write_reg(FLL, 			ov2640_regs_def[9]);
			subcam_write_reg(COM3, 		ov2640_regs_def[10]); //{COM3, COM3_BAND_SET(COM3_BAND_AUTO)},
			subcam_write_reg(REG5D, 		ov2640_regs_def[11]);
			subcam_write_reg(REG5E, 		ov2640_regs_def[12]);
			subcam_write_reg(REG5F, 		ov2640_regs_def[13]);
			subcam_write_reg(REG60, 	  	ov2640_regs_def[14]);
			subcam_write_reg(HISTO_LOW,  	ov2640_regs_def[15]);
			subcam_write_reg(HISTO_HIGH, 	ov2640_regs_def[16]);
			subcam_write_reg(0x7c,     	0x05);
			subcam_write_reg(0x20,     	0x80);
			subcam_write_reg(0x28,     	0x30);
			subcam_write_reg(0x6c,     	0x00);
			subcam_write_reg(0x6d,     	0x80);
			subcam_write_reg(0x6e,     	0x00);
			subcam_write_reg(0x70,     	0x02);
			subcam_write_reg(0x71,     	0x94);
			subcam_write_reg(0x73,     	0xc1);
			subcam_write_reg(0x3d,     	0x34);
//			subcam_write_reg(COM7, 		ov2640_regs_def[17]); //{COM7, COM7_RES_UXGA | COM7_ZOOM_EN},
            subcam_write_reg(COM7,      0x44);
			subcam_write_reg(0x5a,     	0x57);
			subcam_write_reg(COM25, 		ov2640_regs_def[18]);
			subcam_write_reg(BD50, 		ov2640_regs_def[19]);
			subcam_write_reg(BD60, 		ov2640_regs_def[20]);
			subcam_write_reg(BANK_SEL, 	BANK_SEL_DSP);
			subcam_write_reg(0xe5,     	0x7f);
			subcam_write_reg(MC_BIST, 		ov2640_regs_def[21]); //{MC_BIST, MC_BIST_RESET | MC_BIST_BOOT_ROM_SEL},
			subcam_write_reg(0x41,     	0x24);
			subcam_write_reg(0xe0, 		ov2640_regs_def[22]); //{RESET, RESET_JPEG | RESET_DVP},
			subcam_write_reg(0x76,     	0xff);
			subcam_write_reg(0x33,     	0xa0);
			subcam_write_reg(0x42,     	0x20);
			subcam_write_reg(0x43,     	0x18);
			subcam_write_reg(0x4c,     	0x00);
			subcam_write_reg(CTRL3, 		ov2640_regs_def[23]); //{CTRL3, CTRL3_BPC_EN | CTRL3_WPC_EN | 0x10},
			subcam_write_reg(0x88,     	0x3f);
			subcam_write_reg(0xd7,     	0x03);
			subcam_write_reg(0xd9,     	0x10);
			subcam_write_reg(R_DVP_SP, 	ov2640_regs_def[24]); //{R_DVP_SP, R_DVP_SP_AUTO_MODE | 0x2},
			subcam_write_reg(0xc8,     	0x08);
			subcam_write_reg(0xc9,     	0x80);
			subcam_write_reg(BPADDR, 		ov2640_regs_def[25]);
			subcam_write_reg(BPDATA, 		ov2640_regs_def[26]);
			subcam_write_reg(BPADDR, 		ov2640_regs_def[27]);
			subcam_write_reg(BPDATA, 		ov2640_regs_def[28]);
			subcam_write_reg(BPDATA, 		ov2640_regs_def[29]);
			subcam_write_reg(BPADDR, 		ov2640_regs_def[30]);
			subcam_write_reg(BPDATA, 		ov2640_regs_def[31]);
			subcam_write_reg(BPDATA, 		ov2640_regs_def[32]);
			subcam_write_reg(BPDATA, 		ov2640_regs_def[33]);
			subcam_write_reg(0x90,     	0x00);
			subcam_write_reg(0x91,         0x0e);
			subcam_write_reg(0x91,         0x1a);
			subcam_write_reg(0x91,         0x31);
			subcam_write_reg(0x91,         0x5a);
			subcam_write_reg(0x91,         0x69);
			subcam_write_reg(0x91,         0x75);
			subcam_write_reg(0x91,         0x7e);
			subcam_write_reg(0x91,         0x88);
			subcam_write_reg(0x91,         0x8f);
			subcam_write_reg(0x91,         0x96);
			subcam_write_reg(0x91,         0xa3);
			subcam_write_reg(0x91,         0xaf);
			subcam_write_reg(0x91,         0xc4);
			subcam_write_reg(0x91,         0xd7);
			subcam_write_reg(0x91,         0xe8);
			subcam_write_reg(0x91,         0x20);
			subcam_write_reg(0x92,         0x00);
			subcam_write_reg(0x93,         0x06);
			subcam_write_reg(0x93,         0xe3);
			subcam_write_reg(0x93,         0x03);
			subcam_write_reg(0x93,         0x03);
			subcam_write_reg(0x93,         0x00);
			subcam_write_reg(0x93,         0x02);
			subcam_write_reg(0x93,         0x00);
			subcam_write_reg(0x93,         0x00);
			subcam_write_reg(0x93,         0x00);
			subcam_write_reg(0x93,         0x00);
			subcam_write_reg(0x93,         0x00);
			subcam_write_reg(0x93,         0x00);
			subcam_write_reg(0x93,         0x00);
			subcam_write_reg(0x96,         0x00);
			subcam_write_reg(0x97,         0x08);
			subcam_write_reg(0x97,         0x19);
			subcam_write_reg(0x97,         0x02);
			subcam_write_reg(0x97,         0x0c);
			subcam_write_reg(0x97,         0x24);
			subcam_write_reg(0x97,         0x30);
			subcam_write_reg(0x97,         0x28);
			subcam_write_reg(0x97,         0x26);
			subcam_write_reg(0x97,         0x02);
			subcam_write_reg(0x97,         0x98);
			subcam_write_reg(0x97,         0x80);
			subcam_write_reg(0x97,         0x00);
			subcam_write_reg(0x97,         0x00);
			subcam_write_reg(0xa4,         0x00);
			subcam_write_reg(0xa8,         0x00);
			subcam_write_reg(0xc5,         0x11);
			subcam_write_reg(0xc6,         0x51);
			subcam_write_reg(0xbf,         0x80);
			subcam_write_reg(0xc7,         0x10);  /* simple AWB */
			subcam_write_reg(0xb6,         0x66);
			subcam_write_reg(0xb8,         0xA5);
			subcam_write_reg(0xb7,         0x64);
			subcam_write_reg(0xb9,         0x7C);
			subcam_write_reg(0xb3,         0xaf);
			subcam_write_reg(0xb4,         0x97);
			subcam_write_reg(0xb5,         0xFF);
			subcam_write_reg(0xb0,         0xC5);
			subcam_write_reg(0xb1,         0x94);
			subcam_write_reg(0xb2,         0x0f);
			subcam_write_reg(0xc4,         0x5c);
			subcam_write_reg(0xa6,         0x00);
			subcam_write_reg(0xa7,         0x20);
			subcam_write_reg(0xa7,         0xd8);
			subcam_write_reg(0xa7,         0x1b);
			subcam_write_reg(0xa7,         0x31);
			subcam_write_reg(0xa7,         0x00);
			subcam_write_reg(0xa7,         0x18);
			subcam_write_reg(0xa7,         0x20);
			subcam_write_reg(0xa7,         0xd8);
			subcam_write_reg(0xa7,         0x19);
			subcam_write_reg(0xa7,         0x31);
			subcam_write_reg(0xa7,         0x00);
			subcam_write_reg(0xa7,         0x18);
			subcam_write_reg(0xa7,         0x20);
			subcam_write_reg(0xa7,         0xd8);
			subcam_write_reg(0xa7,         0x19);
			subcam_write_reg(0xa7,         0x31);
			subcam_write_reg(0xa7,         0x00);
			subcam_write_reg(0xa7,         0x18);
			subcam_write_reg(0x7f,         0x00);
			subcam_write_reg(0xe5,         0x1f);
			subcam_write_reg(0xe1,         0x77);
			subcam_write_reg(0xdd,         0x7f);
			subcam_write_reg(CTRL0, 		ov2640_regs_def[34]); // {CTRL0, CTRL0_YUV422 | CTRL0_YUV_EN | CTRL0_RGB_EN}
			// OpenMV Custom.
			//    subcam_write_reg(BANK_SEL,      BANK_SEL_SENSOR);
			//    subcam_write_reg(0x0f,          0x4b);
			//    subcam_write_reg(COM1,          0x8f);
			// End.

			/**
			 * @brief  set format(RGB, RAW, JPEG,,,)
			 * @param[in]  format_reg_param : MRAM parameters ("format_reg_param")
			 * @return  none
			 */
//	        	write_regs(rgb565_regs);
			subcam_write_reg(BANK_SEL,   BANK_SEL_DSP);
			subcam_write_reg(R_BYPASS,   ov2640_regs_rgb565[0]);//{R_BYPASS, R_BYPASS_DSP_EN},
		//	subcam_write_reg(IMAGE_MODE, ov2640_regs0[57]);//{IMAGE_MODE, IMAGE_MODE_RGB565},
			subcam_write_reg(IMAGE_MODE, ov2640_regs_rgb565[1]); //0x08
			subcam_write_reg(0xd7,       0x03);
			subcam_write_reg(0xe0, 	  ov2640_regs_rgb565[2]);//{RESET, 0x00},
			subcam_write_reg(R_BYPASS,   ov2640_regs_rgb565[3]);//{R_BYPASS, R_BYPASS_DSP_EN},
		}
	printf("set_reg fin\r\n");
	}
}

/**
 * @brief  capture and save SD
 * @param[in]  param : enum value of size
 * @return  none
 */
void subcam_bmp_capture(uint8_t param){
	uint16_t width = resolution[param][0];
	uint16_t height = resolution[param][1];
	SCAM_driver->ImSize = (width * height)*2;
	SCAM_driver->SCAM_start_block_addr = Storage_Area[Tlm_CODE_SUBCAM*2]; //plan is 0x0000C100
	SCAM_driver->SCAMstamp1_start_block_addr = Storage_Area[Tlm_CODE_1U_IMAGE_TLM_1*2];
	SCAM_driver->SCAMstamp2_start_block_addr = Storage_Area[Tlm_CODE_1U_IMAGE_TLM_2*2];
	SCAM_driver->SCAMstamp3_start_block_addr = Storage_Area[Tlm_CODE_1U_IMAGE_TLM_3*2];
	SCAM_driver->SCAMbmp_start_block_addr = SCAM_driver->SCAM_start_block_addr + 20;
	SCAM_driver->SCAM_divide = 512 * ov2640_SD_block[0];
	error_dcmi = 1;

	__HAL_DCMI_ENABLE_IT_C2A(&hdcmi, DCMI_IT_FRAME);
	HAL_DCMI_Start_DMA_C2A(&hdcmi, DCMI_MODE_CONTINUOUS, s_dstAddress, SCAM_driver->ImSize/4);
    printf("Now DCMI callbacking...\r\n");
	while(!is_picture){
		WDT_clear_count();
	}
	HAL_DCMI_Stop(&hdcmi);
	is_picture = 0;

	if(error_dcmi == 0){
	//read Image ID and CAM_addr of now picture
		ret_r = sd_read_address(SCAM_driver->SCAM_start_block_addr, 0x00000000, send_content, SD_size);
		if(ret_r != 0){
			printf("read err(ret_r)\r\n");
		}
		SCAM_driver->Image_ID = (send_content[12] << 8) | send_content[13];
		SCAM_driver->SCAM_pic_num = send_content[16];
		SCAM_driver->SCAM_addr = (send_content[17] << 24) | (send_content[18] << 16)
				| (send_content[19] << 8) | send_content[20];
		SCAM_driver->Image_ID++;
		if (SCAM_driver->SCAM_pic_num >= 128){
			SCAM_driver->SCAM_addr = 0x00000000;
			SCAM_driver->SCAM_pic_num = 0x00;
		}
//		printf("SCAM_addr = %08x\n", SCAM_driver->SCAM_addr);
		if (SCAM_driver->SCAM_addr > 512*1024*SCAM_driver->SCAM_pic_num){
			printf("SD over err\n");
			SCAM_driver->SCAM_addr = 512*1024*SCAM_driver->SCAM_pic_num;
		}
		SCAM_addr_now = SCAM_driver->SCAM_addr;

	//write ImageTLM
		//write stamp1
		ret_r = sd_read_address(SCAM_driver->SCAMstamp1_start_block_addr + SCAM_driver->SCAM_pic_num,
				0x00000000, stamp_buf, SD_size);
		if(ret_r != 0){
			printf("read err(ret_r)\r\n");
		}
		//MOBC
		TF_copy_u16(stamp_buf + 24,	0x0000); //Temperature
		TF_copy_u16(stamp_buf + 26,	0x0000);         //Pt.temperature
		TF_copy_u32(stamp_buf + 28,	(uint32_t)(TMGR_get_master_total_cycle())); //MOBC_time
		TF_copy_u32(stamp_buf + 32,	(uint32_t)(p_reset_struct->reset_count[8]));
		//TI
		TF_copy_u32(stamp_buf + 36,	rxpic_driver->tlm_data.rxpic_to_mobc.RXPIC_Time);
		TF_copy_u32(stamp_buf + 40,	txpic_driver->tlm_data.txpic_to_mobc.TXPIC_Time);
		TF_copy_u32(stamp_buf + 44,	(uint32_t)(TMGR_get_master_total_cycle()));
		TF_copy_u32(stamp_buf + 48,	p_rtc_struct->unix_time);
		//ADS(T:1s)
		TF_copy_float(stamp_buf + 52, (float)(g_ads.p_ADS->Omega[0]));
		TF_copy_float(stamp_buf + 56, (float)(g_ads.p_ADS->Omega[1]));
		TF_copy_float(stamp_buf + 60, (float)(g_ads.p_ADS->Omega[2]));
		TF_copy_float(stamp_buf + 64, (float)(g_ads.p_ADS->Quaternion[0]));
		TF_copy_float(stamp_buf + 68, (float)(g_ads.p_ADS->Quaternion[1]));
		TF_copy_float(stamp_buf + 72, (float)(g_ads.p_ADS->Quaternion[2]));
		TF_copy_float(stamp_buf + 76, (float)(g_ads.p_ADS->Quaternion[3]));
		TF_copy_float(stamp_buf + 80, (float)g_ads.p_GST->gst);
		//ODS(T:1s)
		TF_copy_float(stamp_buf + 84, (float)g_ods.p_ODS->Position[0]); //ECI?
		TF_copy_float(stamp_buf + 88, (float)g_ods.p_ODS->Position[1]);
		TF_copy_float(stamp_buf + 92, (float)g_ods.p_ODS->Position[2]);
		TF_copy_float(stamp_buf + 96, (float)g_ods.p_ODS->Altitude);
		ret_r = sd_write_address(SCAM_driver->SCAMstamp1_start_block_addr + SCAM_driver->SCAM_pic_num,
				0x00000000, stamp_buf, SD_size);
		if(ret_r != 0){
			printf("read err(ret_r)\r\n");
		}

		//write stamp2
		ret_r = sd_read_address(SCAM_driver->SCAMstamp2_start_block_addr + SCAM_driver->SCAM_pic_num,
				0x00000000, stamp_buf, SD_size);
		if(ret_r != 0){
			printf("read err(ret_r)\r\n");
		}
		//ADS(T:3s)
		TF_copy_u32(stamp_buf, (uint32_t)g_acs.p_GLOBAL_AOCS->AOCS_MODE.ADS_MODE_Number); //+252
	  	TF_copy_u16(stamp_buf +4, (uint16_t)(g_acs.p_GLOBAL_AOCS->Sensor_time));
		TF_copy_float(stamp_buf +6, (float)(g_ads.p_ADS->Omega[0]));
		TF_copy_float(stamp_buf +10,(float)(g_ads.p_ADS->Omega[1]));
		TF_copy_float(stamp_buf +14,(float)(g_ads.p_ADS->Omega[2]));
		TF_copy_float(stamp_buf +18,(float)(g_ads.p_ADS->MagField[0]));
		TF_copy_float(stamp_buf +22,(float)(g_ads.p_ADS->MagField[1]));
		TF_copy_float(stamp_buf +26,(float)(g_ads.p_ADS->MagField[2]));
		TF_copy_float(stamp_buf +30,(float)(g_ads.p_ADS->SunVec[0]));
		TF_copy_float(stamp_buf +34,(float)(g_ads.p_ADS->SunVec[1]));
		TF_copy_float(stamp_buf +38,(float)(g_ads.p_ADS->SunVec[2]));
		TF_copy_float(stamp_buf +42,(float)g_ads.p_IGRF->IGRF_B[0]);
		TF_copy_float(stamp_buf +46,(float)g_ads.p_IGRF->IGRF_B[1]);
		TF_copy_float(stamp_buf +50,(float)g_ads.p_IGRF->IGRF_B[2]);
		TF_copy_float(stamp_buf +54,(float)g_ods.p_ODS->Latitude);
		TF_copy_float(stamp_buf +58,(float)g_ods.p_ODS->Longitude);
		TF_copy_float(stamp_buf +62,(float)g_ods.p_ODS->Altitude);
		TF_copy_double(stamp_buf+66,(double)g_ads.p_JULIANDATE->juliandate);
		TF_copy_float(stamp_buf +74,(float)g_ads.p_GST->gst);
		TF_copy_float(stamp_buf +78,(float)g_ads.p_SUNVECTOR->sunvector[0]);
		TF_copy_float(stamp_buf +82,(float)g_ads.p_SUNVECTOR->sunvector[1]);
		TF_copy_float(stamp_buf +86,(float)g_ads.p_SUNVECTOR->sunvector[2]);
		TF_copy_float(stamp_buf +90,(float)(g_ads.p_ADS->Quaternion[0]));
		TF_copy_float(stamp_buf +94,(float)(g_ads.p_ADS->Quaternion[1]));
		TF_copy_float(stamp_buf +98,(float)(g_ads.p_ADS->Quaternion[2]));
		TF_copy_float(stamp_buf +102,(float)(g_ads.p_ADS->Quaternion[3]));
		TF_copy_u8(stamp_buf +106, (uint8_t)g_acs.p_GLOBAL_AOCS->Eclipse_flag);
		//ODS
		TF_copy_u32(stamp_buf +107, (uint32_t)g_acs.p_GLOBAL_AOCS->AOCS_MODE.ODS_MODE_Number);
		TF_copy_u16(stamp_buf +111, (uint16_t)(g_acs.p_GLOBAL_AOCS->Sensor_time));
		TF_copy_float(stamp_buf +113, (float)g_sensor.p_gps->ECI[0]);
		TF_copy_float(stamp_buf +117, (float)g_sensor.p_gps->ECI[1]);
		TF_copy_float(stamp_buf +121, (float)g_sensor.p_gps->ECI[2]);
		TF_copy_float(stamp_buf +125, (float)g_sensor.p_gps->Vel_ECI[0]);
		TF_copy_float(stamp_buf +129, (float)g_sensor.p_gps->Vel_ECI[1]);
		TF_copy_float(stamp_buf +133, (float)g_sensor.p_gps->Vel_ECI[2]);
		TF_copy_float(stamp_buf +137, (float)g_ods.p_ODS->Position[0]);
		TF_copy_float(stamp_buf +141, (float)g_ods.p_ODS->Position[1]);
		TF_copy_float(stamp_buf +145, (float)g_ods.p_ODS->Position[2]);
		TF_copy_float(stamp_buf +149, (float)g_ods.p_ODS->Velocity[0]);
		TF_copy_float(stamp_buf +153, (float)g_ods.p_ODS->Velocity[1]);
		TF_copy_float(stamp_buf +157, (float)g_ods.p_ODS->Velocity[2]);
		TF_copy_float(stamp_buf +161, (float)g_ods.p_ODS->RelativePosition[0]);
		TF_copy_float(stamp_buf +165, (float)g_ods.p_ODS->RelativePosition[1]);
		TF_copy_float(stamp_buf +169, (float)g_ods.p_ODS->RelativePosition[2]);
		TF_copy_float(stamp_buf +173, (float)g_ods.p_ODS->RelativeVelocity[0]);
		TF_copy_float(stamp_buf +177, (float)g_ods.p_ODS->RelativeVelocity[1]);
		TF_copy_float(stamp_buf +181, (float)g_ods.p_ODS->RelativeVelocity[2]);
		TF_copy_float(stamp_buf +185, (float)g_ods.p_ODS->Ballistic_Coefficient);
		TF_copy_u8(stamp_buf +189, (uint8_t)g_sensor.p_gps->GPS_Status);
		TF_copy_double(stamp_buf +190, (double)g_ads.p_JULIANDATE->juliandate);
		TF_copy_float(stamp_buf +198, (float)g_sensor.p_gps->Altitude); //additional
		ret_r = sd_write_address(SCAM_driver->SCAMstamp2_start_block_addr + SCAM_driver->SCAM_pic_num,
				0x00000000, stamp_buf, SD_size);
		if(ret_r != 0){
			printf("read err(ret_r)\r\n");
		}

		//write stamp3
		ret_r = sd_read_address(SCAM_driver->SCAMstamp3_start_block_addr + SCAM_driver->SCAM_pic_num,
				0x00000000, stamp_buf, SD_size);
		if(ret_r != 0){
			printf("read err(ret_r)\r\n");
		}
		//ACS
	  	TF_copy_u32(&stamp_buf[0], (uint32_t)g_acs.p_GLOBAL_AOCS->AOCS_MODE.ACS_MODE_Number);
	  	TF_copy_u16(&stamp_buf[4], (uint16_t)g_acs.p_ACS->ACS_time);
	  	TF_copy_float(&stamp_buf[6], (float)(g_acs.p_ACS->Omega[0]));
	  	TF_copy_float(&stamp_buf[10], (float)(g_acs.p_ACS->Omega[1]));
	  	TF_copy_float(&stamp_buf[14], (float)(g_acs.p_ACS->Omega[2]));
	    TF_copy_float(&stamp_buf[18], (float)(g_acs.p_ACS->MagField[0]));
	    TF_copy_float(&stamp_buf[22], (float)(g_acs.p_ACS->MagField[1]));
	    TF_copy_float(&stamp_buf[26], (float)(g_acs.p_ACS->MagField[2]));
	    TF_copy_float(&stamp_buf[30], (float)(g_acs.p_ACS->SunVec[0]));
	    TF_copy_float(&stamp_buf[34], (float)(g_acs.p_ACS->SunVec[1]));
	    TF_copy_float(&stamp_buf[38], (float)(g_acs.p_ACS->SunVec[2]));
	    TF_copy_float(&stamp_buf[42], (float)(g_acs.p_ACS->TagetVec[0]));
	    TF_copy_float(&stamp_buf[46], (float)(g_acs.p_ACS->TagetVec[1]));
	    TF_copy_float(&stamp_buf[50], (float)(g_acs.p_ACS->TagetVec[2]));
	    TF_copy_float(&stamp_buf[54], (float)(g_acs.p_ACS->Quaternion[0]));
	    TF_copy_float(&stamp_buf[58], (float)(g_acs.p_ACS->Quaternion[1]));
	    TF_copy_float(&stamp_buf[62], (float)(g_acs.p_ACS->Quaternion[2]));
	    TF_copy_float(&stamp_buf[66], (float)(g_acs.p_ACS->Quaternion[3]));
	    TF_copy_u16(&stamp_buf[70], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQX));
	    TF_copy_u16(&stamp_buf[72], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQY));
	    TF_copy_u16(&stamp_buf[74], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQZ1));
	    TF_copy_float(&stamp_buf[76], (float)g_acs.p_ACS->Mag_moment_Cn[0]);
	    TF_copy_float(&stamp_buf[80], (float)g_acs.p_ACS->Mag_moment_Cn[1]);
	    TF_copy_float(&stamp_buf[84], (float)g_acs.p_ACS->Mag_moment_Cn[2]);
	    TF_copy_float(&stamp_buf[88], (float)g_acs.p_ACS->Mag_moment_Co[0]);
	    TF_copy_float(&stamp_buf[92], (float)g_acs.p_ACS->Mag_moment_Co[1]);
	    TF_copy_float(&stamp_buf[96], (float)g_acs.p_ACS->Mag_moment_Co[2]);
	  	TF_copy_u8(&stamp_buf[100], (uint8_t)g_acs.p_GLOBAL_AOCS->Eclipse_flag);
	  	//OCS
		TF_copy_u32(&stamp_buf[101], (uint32_t)g_acs.p_GLOBAL_AOCS->AOCS_MODE.OCS_MODE_Number);
		TF_copy_u16(&stamp_buf[105], (uint16_t)g_ocs.p_OCS->OCS_time);
		TF_copy_float(&stamp_buf[107], (float)g_ocs.p_OCS->Position[0]);
		TF_copy_float(&stamp_buf[111], (float)g_ocs.p_OCS->Position[1]);
		TF_copy_float(&stamp_buf[115], (float)g_ocs.p_OCS->Position[2]);
		TF_copy_float(&stamp_buf[119], (float)g_ocs.p_OCS->Velocity[0]);
		TF_copy_float(&stamp_buf[123], (float)g_ocs.p_OCS->Velocity[1]);
		TF_copy_float(&stamp_buf[127], (float)g_ocs.p_OCS->Velocity[2]);
		TF_copy_float(&stamp_buf[131], (float)g_ocs.p_OCS->RelativePosition[0]);
		TF_copy_float(&stamp_buf[135], (float)g_ocs.p_OCS->RelativePosition[1]);
		TF_copy_float(&stamp_buf[139], (float)g_ocs.p_OCS->RelativePosition[2]);
		TF_copy_float(&stamp_buf[143], (float)g_ocs.p_OCS->RelativeVelocity[0]);
		TF_copy_float(&stamp_buf[147], (float)g_ocs.p_OCS->RelativeVelocity[1]);
		TF_copy_float(&stamp_buf[151], (float)g_ocs.p_OCS->RelativeVelocity[2]);
		TF_copy_float(&stamp_buf[155], (float)g_ocs.p_OCS->Ballistic_Coefficient);
		TF_copy_float(&stamp_buf[159], (float)g_ocs.p_OCS->Ballistic_Coefficient_order);
		TF_copy_float(&stamp_buf[163], (float)g_ocs.p_OCS->Ballistic_Coefficient_opponent);
		TF_copy_float(&stamp_buf[167], (float)g_ocs.p_OCS->Ballistic_Coefficient_opponent_order);
	  	TF_copy_float(&stamp_buf[171], (float)(g_acs.p_ACS->Omega[0]));
	  	TF_copy_float(&stamp_buf[175], (float)(g_acs.p_ACS->Omega[1]));
	  	TF_copy_float(&stamp_buf[179], (float)(g_acs.p_ACS->Omega[2]));
	    TF_copy_float(&stamp_buf[183], (float)(g_acs.p_ACS->MagField[0]));
	    TF_copy_float(&stamp_buf[187], (float)(g_acs.p_ACS->MagField[1]));
	    TF_copy_float(&stamp_buf[191], (float)(g_acs.p_ACS->MagField[2]));
	    TF_copy_float(&stamp_buf[195], (float)(g_acs.p_ACS->Quaternion[0]));
	    TF_copy_float(&stamp_buf[199], (float)(g_acs.p_ACS->Quaternion[1]));
	    TF_copy_float(&stamp_buf[203], (float)(g_acs.p_ACS->Quaternion[2]));
	    TF_copy_float(&stamp_buf[207], (float)(g_acs.p_ACS->Quaternion[3]));
	    TF_copy_u16(&stamp_buf[211], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQX));
	    TF_copy_u16(&stamp_buf[213], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQY));
	    TF_copy_u16(&stamp_buf[215], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQZ1));
	    TF_copy_float(&stamp_buf[217], (float)g_acs.p_ACS->Mag_moment_Cn[0]);
	    TF_copy_float(&stamp_buf[221], (float)g_acs.p_ACS->Mag_moment_Cn[1]);
	    TF_copy_float(&stamp_buf[225], (float)g_acs.p_ACS->Mag_moment_Cn[2]);
	    TF_copy_float(&stamp_buf[229], (float)g_acs.p_ACS->Mag_moment_Co[0]);
	    TF_copy_float(&stamp_buf[233], (float)g_acs.p_ACS->Mag_moment_Co[1]);
	    TF_copy_float(&stamp_buf[237], (float)g_acs.p_ACS->Mag_moment_Co[2]);
	  	TF_copy_u8(&stamp_buf[241], (uint8_t)g_acs.p_GLOBAL_AOCS->Eclipse_flag);
		ret_r = sd_write_address(SCAM_driver->SCAMstamp3_start_block_addr + SCAM_driver->SCAM_pic_num,
				0x00000000, stamp_buf, SD_size);
		if(ret_r != 0){
			printf("read err(ret_r)\r\n");
		}

	//save now addr & size in SD
		ret_r = sd_read_address(SCAM_driver->SCAM_start_block_addr + 1,
				0x00000000, info_buf, SD_size*2);
		if(ret_r != 0){
			printf("read err(ret_r)(init)\r\n");
		}
		info_buf[4*(SCAM_driver->SCAM_pic_num)] = SCAM_driver->SCAM_addr >> 24;
		info_buf[4*(SCAM_driver->SCAM_pic_num) + 1] = SCAM_driver->SCAM_addr >> 16;
		info_buf[4*(SCAM_driver->SCAM_pic_num) + 2] = SCAM_driver->SCAM_addr >> 8;
		info_buf[4*(SCAM_driver->SCAM_pic_num) + 3] = SCAM_driver->SCAM_addr;
		info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num)] = SCAM_driver->ImSize >> 24;
		info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 1] = SCAM_driver->ImSize >> 16;
		info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 2] = SCAM_driver->ImSize >> 8;
		info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 3] = SCAM_driver->ImSize;

	//calculate ImageTLM
		static uint32_t blk_cnt = 0;
		static uint32_t wht_cnt = 0;
		float SCAM_bright = 0.0f;
		uint8_t SCAM_bright_ave = 0;

		for (int k=0; k<SCAM_driver->ImSize/2; k++){
			uint8_t first = *(p_dstAddress + 2*k);
			uint8_t second = *(p_dstAddress + 2*k + 1);
			float red = ((second & 0b11111000)>>3)*255;
			float green = (((second & 0b00000111)<<2) | ((first & 0b11000000)>>6))*255;
			float blue = (first & 0b00011111)*255;
			float sum = red + blue + green;
			if (sum<(255*ov2640_color_thred[0]*3)){
				blk_cnt++;
			}
			else if (sum>((31 - ov2640_color_thred[1])*255*3)){
				wht_cnt++;
			}
			SCAM_bright = SCAM_bright + sum;
			WDT_clear_count();

		}
		uint32_t a = width*height*3*31;
		SCAM_bright_ave = SCAM_bright / a;

	//read exposure
        uint8_t tmp1;
        uint8_t tmp2;
        uint8_t tmp3;
        subcam_read_reg(REG04, &tmp1);
        subcam_read_reg(AEC, &tmp2);
        subcam_read_reg(REG45, &tmp3);
        exposure = ((tmp3 & 0x3F) << 10 ) | (tmp2 << 2) | (tmp1 & 0x03);
        printf("exp_time(auto) %d ", exposure);
        double  T_EX = exposure * 1922 * 27.78 / (1.0e6); //UXGA mode
        printf("T_EX = %lg[ms]\n", T_EX);

    //for debug (display the image data)
    // ** printf dcmi data **************************************
        for (int k=0; k<SCAM_driver->ImSize; k++){
          print(SUBCAM, "%02x", *(p_dstAddress+k));
          if (k%5000 == 0){
            WDT_clear_count();
          }
        }
        print(SUBCAM, "\r\n");
        print(SUBCAM, "------------------------\r\n");
    // **********************************************************

	//save SD
        uint32_t block_num = (SCAM_driver->ImSize) / SCAM_driver->SCAM_divide;
        uint32_t rest = (SCAM_driver->ImSize) % SCAM_driver->SCAM_divide;
        for(uint32_t i=0; i < block_num; i++){
            ret_w = sd_write_address(SCAM_driver->SCAMbmp_start_block_addr, SCAM_driver->SCAM_addr, p_dstAddress + SCAM_driver->SCAM_divide*i, SCAM_driver->SCAM_divide);
            if(ret_w != 0){
                printf("write err\r\n");
            }
            WDT_clear_count();
            SCAM_driver->SCAM_addr += SCAM_driver->SCAM_divide;
        }

        ret_w = sd_write_address(SCAM_driver->SCAMbmp_start_block_addr, SCAM_driver->SCAM_addr, p_dstAddress + SCAM_driver->SCAM_divide*block_num, rest);
        if(ret_w != 0){
            printf("write err\r\n");
        }
        if(rest != 0){
            SCAM_driver->SCAM_addr += SCAM_driver->SCAM_divide;
        }

    //send "send_content[0]~[13]" to MOBC
        //now ID & num / next addr
        send_content[12] = SCAM_driver->Image_ID >> 8;
        send_content[13] = SCAM_driver->Image_ID;
    //	send_content[14] = SCAM_driver->SCAM_pic_num >> 8;
        send_content[15] = SCAM_driver->SCAM_pic_num;
        send_content[17] = SCAM_driver->SCAM_addr >> 24;
        send_content[18] = SCAM_driver->SCAM_addr >> 16;
        send_content[19] = SCAM_driver->SCAM_addr >> 8;
        send_content[20] = SCAM_driver->SCAM_addr;
        ret_w = sd_write_address(SCAM_driver->SCAM_start_block_addr,
                0x00000000, send_content, SD_size);
        if (ret_w != 0){
            printf("write err(ret_w)\r\n");
        }
        ret_w = sd_write_address(SCAM_driver->SCAM_start_block_addr + 1,
                0x00000000, info_buf, SD_size*2);
        if (ret_w != 0){
            printf("write err(ret_w)\r\n");
        }

    //write ImageTLM
        //Camera
        ret_r = sd_read_address(SCAM_driver->SCAMstamp1_start_block_addr + SCAM_driver->SCAM_pic_num,
                0x00000000, stamp_buf, SD_size);
        if(ret_r != 0){
            printf("read err(ret_r)\r\n");
        }
        TF_copy_u8 (stamp_buf,		5); // 5->1U SCAM
        TF_copy_u8 (stamp_buf + 1,	(uint8_t)(SCAM_driver->SCAM_pic_num));
        TF_copy_u16(stamp_buf + 2,  SCAM_driver->Image_ID);
        TF_copy_u8 (stamp_buf + 4,	ov2640_regs_size[0]);
        TF_copy_u32(stamp_buf + 5,	0x00000000); //jpg_size
        TF_copy_u32(stamp_buf + 9,	blk_cnt);
        TF_copy_u32(stamp_buf + 13,	wht_cnt);
        TF_copy_u8 (stamp_buf + 17,	SCAM_bright_ave);
        TF_copy_u8 (stamp_buf + 18,	0x00); // saturation
        TF_copy_u32(stamp_buf + 19,	0x00000000); //exposure time
        TF_copy_u8 (stamp_buf + 23,	1); //success flag
        ret_w = sd_write_address(SCAM_driver->SCAMstamp1_start_block_addr + SCAM_driver->SCAM_pic_num,
                0x00000000, stamp_buf, SD_size);
        if (ret_w != 0){
            printf("write err(ret_w)\r\n");
        }
    //for debug
    //	uint8_t save[SD_size];
    //	ret_r = sd_read_address(SCAM_driver->SCAMstamp_start_block_addr + SCAM_driver->SCAM_pic_num,
    //				0x00000000, save, SD_size);
    //	if(ret_r != 0){
    //		printf("read err(ret_r)(init)\r\n");
    //	}
    //	for (int i=355; i<357; i++){
    //		printf("save[%d] = 0x%x\n", i, save[i]);
    //	}

        // ** read SD (debug)**************************************************
  //		uint8_t read_data[SCAM_driver->SCAM_divide];
  //		for(int i = 0; i < block_num; i++){
  //			ret_w = sd_read_address(SCAM_driver->SCAMbmp_start_block_addr, SCAM_addr_now + SCAM_driver->SCAM_divide*i, read_data, SCAM_driver->SCAM_divide);
  //			if(ret_w != 0){
  //				printf("write err\r\n");
  //			}
  //			for(int j = 0; j < SCAM_driver->SCAM_divide; j++){
  //				print(SUBCAM, "%02x", read_data[j]);
  //				printf("%02x", read_data[j]);
  //				if (j%3000 == 0){
  //					WDT_clear_count();
  //				}
  //			}
  //		}
  //
  //		ret_w = sd_read_address(SCAM_driver->SCAMbmp_start_block_addr, SCAM_addr_now + SCAM_driver->SCAM_divide*block_num, read_data, rest);
  //		if(ret_w != 0){
  //			printf("write err\r\n");
  //		}
  //		for(int j = 0; j < rest; j++){
  //			print(SUBCAM, "%02x", read_data[j]);
  //			printf("%02x", read_data[j]);
  //			if (j%3000 == 0){
  //				WDT_clear_count();
  //			}
  //		}
        // ******************************************************************

  //		printf("\r\n");
        printf("SCAM_bright_ave = %d, ", SCAM_bright_ave);
        printf("black_cnt = %d, ", blk_cnt);
        printf("white_cnt = %d\r\n", wht_cnt);
  //		printf("Image_ID = 0x%04x, ", SCAM_driver->Image_ID);
  //		printf("SCAM_pic_num = 0x%02x, ", SCAM_driver->SCAM_pic_num);
  //		printf("SCAM_addr = 0x%08x\r\n", SCAM_addr_now);
        printf("SCAM Image Capture Done(%d x %d)\n", width, height);
	}
	else{

	}
}

/**
 * @brief  convert from RGB565 to thumbnail
 * @param[in]  param : enum value of size
 * @return  none
 */
void RGB24_to_thumbnail(uint8_t param){

    uint16_t width = resolution[param][0];
    uint16_t height = resolution[param][1];
	uint16_t scaled_width = ov2640_scaled_size[0];
	uint16_t scaled_height = ov2640_scaled_size[1];

	SCAM_driver->scaledSize = (scaled_width)*(scaled_height)*2;
	SCAM_driver->SCAMscaled_start_block_addr = SCAM_driver->SCAMbmp_start_block_addr + 131072; //scaled
	SCAM_driver->SCAM_divide = 512 * ov2640_SD_block[0];
	SCAM_driver->SCAMscaled_counter = int(SCAM_driver->scaledSize/ SCAM_driver->SCAM_divide);

    uint32_t* p_scaled_data = (uint32_t*)(SRAM_BANK_ADDR + THUMBNAIL_ADDR);
    uint8_t* scaled_data = (uint8_t*)(p_scaled_data);

    if(ov2640_scaled_method[0] == 0){
	 /* 線形補間法 (linear interpolated method) */
     /* Reference : https://daeudaeu.com/linear/#i-10 */
		int m0, m1, n0, n1;
		double originalm, originaln;
		double dm, dn;
		double scaleW = double(scaled_width)/double(width);
		double scaleH = double(scaled_height)/double(height);

		for(int n = 0; n < scaled_height; n++){
			for(int m = 0; m < scaled_width; m++){

				uint8_t red = 0;
				uint8_t green = 0;
				uint8_t blue = 0;

                /* 縮小前画像の座標を、縮小後画像の座標から計算 */
				originalm = (double)m / (double)scaleW;
				originaln = (double)n / (double)scaleH;

				/* grids of surrounded 4 points */
				m0 = (int)originalm;
				m1 = m0 + 1;
				n0 = (int)originaln;
				n1 = n0 + 1;

				/* if (m1>width) or (n1>height) */
				if(m1 == width) m1 = width - 1;
				if(n1 == height) n1 = height - 1;

				/* distance between origin_grid and m0(n0) */
				dm = originalm - m0;
				dn = originaln - n0;

				uint8_t first_m0n0  = *(p_dstAddress + 2 * (m0 + n0 * width));
				uint8_t second_m0n0 = *(p_dstAddress + 2 * (m0 + n0 * width) + 1);
				uint8_t first_m0n1  = *(p_dstAddress + 2 * (m0 + n1 * width));
				uint8_t second_m0n1 = *(p_dstAddress + 2 * (m0 + n1 * width) + 1);
				uint8_t first_m1n0  = *(p_dstAddress + 2 * (m1 + n0 * width));
				uint8_t second_m1n0 = *(p_dstAddress + 2 * (m1 + n0 * width) + 1);
				uint8_t first_m1n1  = *(p_dstAddress + 2 * (m1 + n1 * width));
				uint8_t second_m1n1 = *(p_dstAddress + 2 * (m1 + n1 * width) + 1);

				/* 周囲４点の画素の色の輝度値より線形補間 */
				uint8_t red_m0n0 = first_m0n0 & 0b11111000;
				uint8_t red_m0n1 = first_m0n1 & 0b11111000;
				uint8_t red_m1n0 = first_m1n0 & 0b11111000;
				uint8_t red_m1n1 = first_m1n1 & 0b11111000;
				red = red_m1n1*dm*dn + red_m1n0*dm*(1-dn) + red_m0n1*(1-dm)*dn + red_m0n0*(1-dm)*(1-dn);
				//				printf("red = %x\r\n", red);

				uint8_t green_m0n0 = ((uint8_t)(first_m0n0 << 5)) | (uint8_t)((second_m0n0 & 0b11100000) >> 3);
				uint8_t green_m0n1 = ((uint8_t)(first_m0n1 << 5)) | (uint8_t)((second_m0n1 & 0b11100000) >> 3);
				uint8_t green_m1n0 = ((uint8_t)(first_m1n0 << 5)) | (uint8_t)((second_m1n0 & 0b11100000) >> 3);
				uint8_t green_m1n1 = ((uint8_t)(first_m1n1 << 5)) | (uint8_t)((second_m1n1 & 0b11100000) >> 3);
				green = green_m1n1*dm*dn + green_m1n0*dm*(1-dn) + green_m0n1*(1-dm)*dn + green_m0n0*(1-dm)*(1-dn);
			//				printf("green = %x\r\n", green);

				uint8_t blue_m0n0 = (uint8_t)(second_m0n0<<3);
				uint8_t blue_m0n1 = (uint8_t)(second_m0n1<<3);
				uint8_t blue_m1n0 = (uint8_t)(second_m1n0<<3);
				uint8_t blue_m1n1 = (uint8_t)(second_m1n1<<3);
				blue = blue_m1n1*dm*dn + blue_m1n0*dm*(1-dn) + blue_m0n1*(1-m)*dn + blue_m0n0*(1-dm)*(1-dn);
			//				printf("blue = %x\r\n", blue);

	//		uint8_t a= (red&0b11111000);
	//		uint8_t b= (green>>5);
	//		uint8_t c= ((green<<3)&0b11100000);
	//		uint8_t d= (blue>>3);
	//		printf("a=%x\r\n", a);
	//		printf("b=%x\r\n", b);
	//		printf("c=%x\r\n", c);
	//		printf("d=%x\r\n", d);

				uint8_t first = (uint8_t)(red&0b11111000) | (uint8_t)(green>>5);
				uint8_t second = (uint8_t)((green<<3)&0b11100000) | (uint8_t)(blue>>3);
				*(scaled_data + 2*(m + n*scaled_width)) = first;
				*(scaled_data + 2*(m + n*scaled_width) + 1) = second;
		//		printf("test = %x\r\n", *(scaled_data + (m + n*scaled_width)));
		//		printf("test2 = %x\r\n", (uint16_t) ((first<<8) | second));
			}
		}
    }
    else if(ov2640_scaled_method[0] == 1){
      /* Area average method (整数倍のみ)*/
      /* Reference : https://www.rainorshine.asia/2017/10/12/post2766.html
       * http://touminote.blog.shinobi.jp/%E9%9B%91%E8%A8%98/%E7%94%BB%E5%83%8F%E3%83%AA%E3%82%B5%E3%82%A4%E3%82%BA%E3%80%8C%E9%9D%A2%E7%A9%8D%E5%B9%B3%E5%9D%87%E6%B3%95%E3%80%8D */
		for (int m=0; m < scaled_height; m++){
			for (int k=0; k < (scaled_width); k++){
				uint32_t sum_r2 = 0;
				uint32_t sum_g2 = 0;
				uint32_t sum_b2 = 0;
				for (int i=0; i < (height/scaled_height); i++){
					uint32_t sum_r = 0;
					uint32_t sum_g = 0;
					uint32_t sum_b = 0;
					for (int j=0; j < (width/scaled_width); j++){
						uint8_t first  = *(p_dstAddress + 2*(j + (width)*i + (width/scaled_width)*k + width*(height/scaled_height)*m));
						uint8_t second = *(p_dstAddress + 2*(j + (width)*i + (width/scaled_width)*k + width*(height/scaled_height)*m) + 1);
						uint8_t red = first & 0b11111000;
						uint8_t green = (first << 5) | ((second & 0b11100000) >> 3);
						uint8_t blue = second << 3;
						sum_r = sum_r + red;
						sum_g = sum_g + green;
						sum_b = sum_b + blue;
					}
					sum_r2 = sum_r2 + sum_r;
					sum_g2 = sum_g2 + sum_g;
					sum_b2 = sum_b2 + sum_b;
				}
				uint8_t ave_r = sum_r2 / ((width/scaled_width)*(height/scaled_height));
				uint8_t ave_g = sum_g2 / ((width/scaled_width)*(height/scaled_height));
				uint8_t ave_b = sum_b2 / ((width/scaled_width)*(height/scaled_height));
				uint8_t ave_first = (ave_r & 0b11111000) | ((ave_g & 0b11100000) >> 5);
				uint8_t ave_second = ((ave_g & 0b00011100) << 3) | (ave_b >> 3);
				*(scaled_data + 2*(k + scaled_width*m)) = ave_first;
				*(scaled_data + 2*(k + scaled_width*m) + 1) = ave_second;
			}
		}
    }


	printf("Scaled_picture(%d x %d)\n", scaled_width, scaled_height);
//	for (int i=0; i < (scaled_width*scaled_height)*2; i++){
//		print(SUBCAM, "%02x", *(scaled_data + i));
//		printf("%02x", *(scaled_data + i));
//		if (i%3000 == 0){
//			WDT_clear_count();
//		}
//	}
//	printf("\r\n");

//read Image ID and CAM_addr of now picture
	ret_r = sd_read_address(SCAM_driver->SCAM_start_block_addr,
			0x00000000, send_content, SD_size);
	if(ret_r != 0){
		printf("read err(ret_r)\r\n");
	}
	ret_r = sd_read_address(SCAM_driver->SCAM_start_block_addr + 3,
			0x00000000, info_buf, SD_size*2);
	if(ret_r != 0){
		printf("read err(ret_r)\r\n");
	}

	SCAM_driver->SCAMscaled_addr = (send_content[21] << 24) | (send_content[22] << 16)
			| (send_content[23] << 8) | send_content[24];

	if (SCAM_driver->SCAM_pic_num == 0){
		SCAM_driver->SCAMscaled_addr = 0x00000000;
	}
	if (SCAM_driver->SCAMscaled_addr > 160*128*2*SCAM_driver->SCAM_pic_num){
		printf("SD over err\n");
		SCAM_driver->SCAMscaled_addr = 160*128*2*SCAM_driver->SCAM_pic_num;
	}
	uint32_t SCAMscaled_addr_now = SCAM_driver->SCAMscaled_addr;
//	printf("SCAMscaled_addr_now = %08x\n", SCAMscaled_addr_now);

	info_buf[4*(SCAM_driver->SCAM_pic_num)] = SCAM_driver->SCAMscaled_addr >> 24;
	info_buf[4*(SCAM_driver->SCAM_pic_num) + 1] = SCAM_driver->SCAMscaled_addr >> 16;
	info_buf[4*(SCAM_driver->SCAM_pic_num) + 2] = SCAM_driver->SCAMscaled_addr >> 8;
	info_buf[4*(SCAM_driver->SCAM_pic_num) + 3] = SCAM_driver->SCAMscaled_addr;

	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num)] = SCAM_driver->scaledSize >> 24;
	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 1] = SCAM_driver->scaledSize >> 16;
	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 2] = SCAM_driver->scaledSize >> 8;
	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 3] = SCAM_driver->scaledSize;

//save SD
	uint8_t Saved_Data[SCAM_driver->SCAM_divide];
	for (int k = 0; k < SCAM_driver->SCAMscaled_counter + 1; k++){
		for (int j=0; j<SCAM_driver->SCAM_divide; j++){
			Saved_Data[j] = *(scaled_data + SCAM_driver->SCAM_divide*k + j);
		}
		ret_w = sd_write_address(SCAM_driver->SCAMscaled_start_block_addr,
				SCAM_driver->SCAMscaled_addr, Saved_Data, SCAM_driver->SCAM_divide);
		if(ret_w != 0){
			printf("write err(ret_w)\r\n");
		}
		SCAM_driver->SCAMscaled_addr += SCAM_driver->SCAM_divide;
	}

//addr of next picture
	send_content[21] = SCAM_driver->SCAMscaled_addr >> 24;
	send_content[22] = SCAM_driver->SCAMscaled_addr >> 16;
	send_content[23] = SCAM_driver->SCAMscaled_addr >> 8;
	send_content[24] = SCAM_driver->SCAMscaled_addr;
	ret_w = sd_write_address(SCAM_driver->SCAM_start_block_addr,
			0x00000000, send_content, SD_size);
	if(ret_r != 0){
		printf("write err(ret_w)\r\n");
	}
	ret_w = sd_write_address(SCAM_driver->SCAM_start_block_addr + 3,
			0x00000000, info_buf, SD_size*2);
	if (ret_w != 0){
		printf("write err(ret_w)\r\n");
	}

//flag
	ret_r = sd_read_address(SCAM_driver->SCAMstamp1_start_block_addr + SCAM_driver->SCAM_pic_num,
			0x00000000, stamp_buf, SD_size);
	if (ret_r != 0){
		printf("read err(ret_r)\r\n");
	}
	stamp_buf[23] = 2; //success flag
	ret_w = sd_write_address(SCAM_driver->SCAMstamp1_start_block_addr + SCAM_driver->SCAM_pic_num,
			0x00000000, stamp_buf, SD_size);
	if (ret_w != 0){
		printf("write err(ret_w)\r\n");
	}
////////////////////////////////// debug read /////////////////////////////////////////////////////////
//	for (uint32_t k=0; k < SCAM_driver->SCAMscaled_counter + 1; k++){
//		if (k < SCAM_driver->SCAMscaled_counter){
//			ret_r = sd_read_address(SCAM_driver->SCAMscaled_start_block_addr, SCAMscaled_addr_now + SCAM_driver->SCAM_divide*k,
//					Saved_Data, SCAM_driver->SCAM_divide);
//			if(ret_r != 0){
//				printf("read err(ret_r)\r\n");
//			}
//			for(int i=0; i<SCAM_driver->SCAM_divide; i++){
//				print(SUBCAM, "%02x", Saved_Data[i]);
//				printf("%02x", Saved_Data[i]);
//				if (i%3000 == 0){
//					WDT_clear_count();
//				}
//			}
//		}
//		else if((k == SCAM_driver->SCAMscaled_counter)&&(SCAM_driver->scaledSize % SCAM_driver->SCAM_divide != 0)){
//			ret_r = sd_read_address(SCAM_driver->SCAMscaled_start_block_addr, SCAMscaled_addr_now + SCAM_driver->SCAM_divide*k,
//					Saved_Data, SCAM_driver->SCAM_divide);
//			if(ret_r != 0){
//				printf("read err(ret_r)\r\n");
//			}
//			for(int i=0; i<(SCAM_driver->scaledSize) % SCAM_driver->SCAM_divide; i++){
//				print(SUBCAM, "%02x", Saved_Data[i]);
//				printf("%02x", Saved_Data[i]);
//				if (i%3000 == 0){
//					WDT_clear_count();
//				}
//			}
//		}
//	}
//	printf("\n");
////////////////////////////////////////////////////////////////////////////////////////////////////////

//from scaled(RGB) to RAW
//	WDT_clear_count();
//	uint32_t* p_RAW_data = (uint32_t*)(SRAM_BANK_ADDR);
//	uint8_t* RAW_data = (uint8_t*)(p_RAW_data);
//	int line=0;
//	int k=0;
//	for (int i=0; i<((scaled_width)*(scaled_height));i++){
//		if (i%(scaled_width) == 0){
//			line++;
//		}
//		if (line%2 == 1){
//			uint8_t first_r  = *(scaled_data + 2*i);
//			uint8_t first_g  = *(scaled_data + 2*i);
//			uint8_t second_g = *(scaled_data + 2*i + 1);
//			uint8_t red = first_r & 0b11111000; //red
//			uint8_t green_r = (uint8_t)((first_g << 5) | ((second_g & 0b11100000) >> 3)); //green_r
//			*(RAW_data + 2*k) = red;
//			*(RAW_data + 2*k + 1) = green_r;
//			k++;
//		}
//		else if (line%2 == 0){
//			uint8_t first_g  = *(scaled_data + 2*i);
//			uint8_t second_g = *(scaled_data + 2*i + 1);
//			uint8_t second_b = *(scaled_data + 2*i + 1);
//			uint8_t green_b = (uint8_t)((first_g << 5) | ((second_g & 0b11100000) >> 3)); //green_b
//			uint8_t blue = (second_b & 0b00011111) << 3; //blue
//			*(RAW_data + 2*k) = green_b;
//			*(RAW_data + 2*k + 1) = blue;
//			k++;
//		}
//	}
//	printf("RAW picture\r\n");
////	for (int i=0; i < (scaled_width)*(scaled_height)/4; i++){
////		print(SUBCAM, "%02x", *(RAW_data + i));
////	}
////	printf("\r\n");
//
////save SD (raw)
//	ret_r = sd_read_address(SCAM_driver->SCAM_start_block_addr,
//			0x00000000, send_content, SD_size);
//	if(ret_r != 0){
//		printf("read err(ret_r)\r\n");
//	}
//	ret_r = sd_read_address(SCAM_driver->SCAM_start_block_addr + 7,
//			0x00000000, info_buf, SD_size*2);
//	if(ret_r != 0){
//		printf("read err(ret_r)\r\n");
//	}
//
//	SCAM_driver->SCAMraw_addr = (send_content[25] << 24) | (send_content[26] << 16)
//			| (send_content[27] << 8) | send_content[28];
//	if (SCAM_driver->SCAM_pic_num == 0){
//		SCAM_driver->SCAMraw_addr = 0x00000000;
//	}
//	uint32_t SCAMraw_addr_now = SCAM_driver->SCAMraw_addr;
//	printf("SCAMraw_addr_now = %x\n", SCAMraw_addr_now);
//
//	info_buf[4*(SCAM_driver->SCAM_pic_num)] = SCAM_driver->SCAMraw_addr >> 24;
//	info_buf[4*(SCAM_driver->SCAM_pic_num) + 1] = SCAM_driver->SCAMraw_addr >> 16;
//	info_buf[4*(SCAM_driver->SCAM_pic_num) + 2] = SCAM_driver->SCAMraw_addr >> 8;
//	info_buf[4*(SCAM_driver->SCAM_pic_num) + 3] = SCAM_driver->SCAMraw_addr;
//	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num)] = SCAM_driver->rawSize >> 24;
//	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 1] = SCAM_driver->rawSize >> 16;
//	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 2] = SCAM_driver->rawSize >> 8;
//	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 3] = SCAM_driver->rawSize;
//
//	for (int k = 0; k < SCAM_driver->SCAMraw_counter + 1; k++){
//		for (int j=0; j<SCAM_driver->SCAM_divide/2; j++){
//			Saved_Data[2*j    ] = (uint8_t)(*(RAW_data + SCAM_driver->SCAM_divide*k/2 + j) >> 8);
//			Saved_Data[2*j + 1] = (uint8_t)(*(RAW_data + SCAM_driver->SCAM_divide*k/2 + j) & 0b0000000011111111);
//		}
//		ret_w = sd_write_address(SCAM_driver->SCAMraw_start_block_addr,
//				SCAM_driver->SCAMraw_addr, Saved_Data, SCAM_driver->SCAM_divide);
//		if(ret_w != 0){
//			printf("write err(ret_w)\r\n");
//		}
//		SCAM_driver->SCAMraw_addr = SCAM_driver->SCAMraw_addr + SCAM_driver->SCAM_divide;
//	}
//
//	//addr when next capture
//	send_content[25] = SCAM_driver->SCAMraw_addr >> 24;
//	send_content[26] = SCAM_driver->SCAMraw_addr >> 16;
//	send_content[27] = SCAM_driver->SCAMraw_addr >> 8;
//	send_content[28] = SCAM_driver->SCAMraw_addr;
//	ret_w = sd_write_address(SCAM_driver->SCAM_start_block_addr,
//			0x00000000, send_content, SD_size);
//	if (ret_w != 0){
//		printf("write err(ret_w)\r\n");
//	}
//	ret_w = sd_write_address(SCAM_driver->SCAM_start_block_addr + 7,
//			0x00000000, info_buf, SD_size*2);
//	if (ret_w != 0){
//		printf("write err(ret_w)\r\n");
//	}
//
////flag
//	ret_r = sd_read_address(SCAM_driver->SCAMstamp_start_block_addr + SCAM_driver->SCAM_pic_num,
//			0x00000000, stamp_buf, SD_size);
//	if (ret_r != 0){
//		printf("read err(ret_r)\r\n");
//	}
//	stamp_buf[23] = 3; //success flag
//	ret_w = sd_write_address(SCAM_driver->SCAMstamp_start_block_addr + SCAM_driver->SCAM_pic_num,
//			0x00000000, stamp_buf, SD_size);
//	if (ret_w != 0){
//		printf("write err(ret_w)\r\n");
//	}

//	is_picture = 3;
}

/**
 * @brief  save BOA image in SD
 * @param[in]  *BOA_data : SRAM addr of BOA image
 * @param[in]  size : the number of pixels
 * @return  none
 */
void save_SD_BOA(uint32_t *BOA_data, uint32_t size){
//save SD
	SCAM_driver->BOA_Size = size * 2;
	SCAM_driver->SCAM_BOA_start_block_addr = 0x0002E914; //BOA
	SCAM_driver->SCAM_divide = 512 * ov2640_SD_block[0];
	SCAM_driver->SCAM_BOA_counter = int(SCAM_driver->BOA_Size/ SCAM_driver->SCAM_divide);

	ret_r = sd_read_address(SCAM_driver->SCAM_start_block_addr,
			0x00000000, send_content, SD_size);
	if(ret_r != 0){
		printf("read err(ret_r)\r\n");
	}
	ret_r = sd_read_address(SCAM_driver->SCAM_start_block_addr + 5,
			0x00000000, info_buf, SD_size*2);
	if(ret_r != 0){
		printf("read err(ret_r)\r\n");
	}

//read Image ID and CAM_addr of now picture
	SCAM_driver->SCAM_BOA_addr = (send_content[33] << 24) | (send_content[34] << 16)
			| (send_content[35] << 8) | send_content[36];
	if (SCAM_driver->SCAM_pic_num == 0){
		SCAM_driver->SCAM_BOA_addr = 0x00000000;
	}
	if (SCAM_driver->SCAM_BOA_addr > 160*128*3*SCAM_driver->SCAM_pic_num){
		printf("SD over err\n");
		SCAM_driver->SCAM_BOA_addr = 160*128*3*SCAM_driver->SCAM_pic_num;
	}
	uint32_t SCAM_BOA_addr_now = SCAM_driver->SCAM_BOA_addr;
//	printf("SCAM_BOA_addr_now = %08x\n", SCAM_BOA_addr_now);

//save now addr & size in SD
	info_buf[4*(SCAM_driver->SCAM_pic_num)] = SCAM_driver->SCAM_BOA_addr >> 24;
	info_buf[4*(SCAM_driver->SCAM_pic_num) + 1] = SCAM_driver->SCAM_BOA_addr >> 16;
	info_buf[4*(SCAM_driver->SCAM_pic_num) + 2] = SCAM_driver->SCAM_BOA_addr >> 8;
	info_buf[4*(SCAM_driver->SCAM_pic_num) + 3] = SCAM_driver->SCAM_BOA_addr;
	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num)] = SCAM_driver->BOA_Size >> 24;
	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 1] = SCAM_driver->BOA_Size >> 16;
	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 2] = SCAM_driver->BOA_Size >> 8;
	info_buf[SD_size + 4*(SCAM_driver->SCAM_pic_num) + 3] = SCAM_driver->BOA_Size;

//save SD
	uint8_t Saved_Data[SCAM_driver->SCAM_divide];
	uint8_t *p_BOA_data = (uint8_t*)BOA_data;
	for (int k = 0; k < SCAM_driver->SCAM_BOA_counter + 1; k++){
		for (int j=0; j<SCAM_driver->SCAM_divide; j++){
			Saved_Data[j] = *(p_BOA_data + SCAM_driver->SCAM_divide*k + j);
		}
		ret_w = sd_write_address(SCAM_driver->SCAM_BOA_start_block_addr,
				SCAM_driver->SCAM_BOA_addr, Saved_Data, SCAM_driver->SCAM_divide);
		if(ret_w != 0){
			printf("write err(ret_w)\r\n");
		}
		SCAM_driver->SCAM_BOA_addr += SCAM_driver->SCAM_divide;
	}

//addr of next picture
	send_content[33] = SCAM_driver->SCAM_BOA_addr >> 24;
	send_content[34] = SCAM_driver->SCAM_BOA_addr >> 16;
	send_content[35] = SCAM_driver->SCAM_BOA_addr >> 8;
	send_content[36] = SCAM_driver->SCAM_BOA_addr;
	ret_w = sd_write_address(SCAM_driver->SCAM_start_block_addr,
			0x00000000, send_content, SD_size);
	if (ret_w != 0){
		printf("write err(ret_w)\r\n");
	}
	ret_w = sd_write_address(SCAM_driver->SCAM_start_block_addr + 5,
			0x00000000, info_buf, SD_size*2);
	if (ret_w != 0){
		printf("write err(ret_w)\r\n");
	}

//flag
	ret_r = sd_read_address(SCAM_driver->SCAMstamp1_start_block_addr + SCAM_driver->SCAM_pic_num,
			0x00000000, stamp_buf, SD_size);
	if (ret_r != 0){
		printf("read err(ret_r)\r\n");
	}
	TF_copy_u8(stamp_buf + 23,	4); //success flag
	ret_w = sd_write_address(SCAM_driver->SCAMstamp1_start_block_addr + SCAM_driver->SCAM_pic_num,
			0x00000000, stamp_buf, SD_size);
	if (ret_w != 0){
		printf("write err(ret_w)\r\n");
	}
////////////////////////////////// debug read /////////////////////////////////////////////////////////
//	for (uint32_t k=0; k < SCAM_driver->SCAM_BOA_counter + 1; k++){
//		if (k < SCAM_driver->SCAM_BOA_counter){
//			ret_r = sd_read_address(SCAM_driver->SCAM_BOA_start_block_addr, SCAM_BOA_addr_now + SCAM_driver->SCAM_divide*k,
//					Saved_Data, SCAM_driver->SCAM_divide);
//			if(ret_r != 0){
//				printf("read err(ret_r)\r\n");
//			}
//			for(int i=0; i<SCAM_driver->SCAM_divide; i++){
//				printf("%02x", Saved_Data[i]);
//			}
//			HAL_IWDG_Refresh(&hiwdg);
//		}
//		else if((k == SCAM_driver->SCAM_BOA_counter)&&(SCAM_driver->BOA_Size % SCAM_driver->SCAM_divide != 0)){
//			ret_r = sd_read_address(SCAM_driver->SCAM_BOA_start_block_addr, SCAM_BOA_addr_now + SCAM_driver->SCAM_divide * k,
//					Saved_Data, SCAM_driver->SCAM_divide);
//			if(ret_r != 0){
//				printf("read err(ret_r)\r\n");
//			}
//			for(int i=0; i<(SCAM_driver->BOA_Size) % SCAM_driver->SCAM_divide; i++){
//				printf("%02x", Saved_Data[i]);
//			}
//		}
//	}
//	printf("\n");
////////////////////////////////////////////////////////////////////////////////////////////////////////

	//num
	ret_r = sd_read_address(SCAM_driver->SCAM_start_block_addr, //SCAM_start_block_addr
			0x00000000, send_content, SD_size);
	if (ret_r != 0){
		printf("read err(ret_r)\r\n");
	}
	printf("pic_num (decimal) = %d\n", SCAM_driver->SCAM_pic_num);
    printf("pic_num (hex) = %02x\n", SCAM_driver->SCAM_pic_num);
	SCAM_driver->SCAM_pic_num++;
	send_content[16] = SCAM_driver->SCAM_pic_num;
//	printf("send_content[16] = 0x%02x\n", send_content[16]);
	ret_w = sd_write_address(SCAM_driver->SCAM_start_block_addr, //SCAM_start_block_addr
			0x00000000, send_content, SD_size);
	if (ret_w != 0){
		printf("write err(ret_w)\r\n");
	}

}

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi){
	printf("DCMI FramEvent!\n");
	is_picture = 1;
	error_dcmi = 0;
}
//void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi){
//	printf("HAL_DCMI_VsyncEventCallback\n");
////	is_picture = 1;
//}
//void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi){
//	printf("HAL_DCMI_LineEventCallback\n");
////	is_picture = 1;
//}
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi){
	printf("DCMI Err!\n");
//	is_picture = 1;
//	error_dcmi = 1;
}

//Park-sensei made. This is used in MAGNARO at first.
// ************************************************************************

uint8_t subcam_configuration()
{
	uint8_t ret_HAL = 0;

	HAL_GPIO_WritePin_C2A(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin_C2A(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
	printf("Subcam enabled \r\n");

	HAL_Delay_C2A(100);
	uint8_t i2cdat_r[2];
	uint8_t i2cdat_w[2];
	ret_HAL = HAL_I2C_Mem_Read_C2A(&hi2c3, 0xBA, 0x00, 1, i2cdat_r, 2, 1000);
	if(ret_HAL != HAL_OK)printf("Read err\r\n");
	printf("ID read done \r\n");

	//Set column start
	if (1)
	{
		i2cdat_w[0] = 0x00;
		i2cdat_w[1] = 0x00;
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xF0, 1, i2cdat_w, 2, 1000); //select page 0
		if(ret_HAL != HAL_OK)printf("Set column err\r\n");
		//i2cdat_w[0] = 0b01000011;
		i2cdat_w[0] = 0x00;
		i2cdat_w[1] = 0x68;
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x02, 1, i2cdat_w, 2, 1000);
		if(ret_HAL != HAL_OK)printf("Set column err\r\n");
	}
	//Set RGB mode
	if (1)
	{
		i2cdat_w[0] = 0x00;
		i2cdat_w[1] = 0x01;
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xF0, 1, i2cdat_w, 2, 1000); //select page 1
		HAL_Delay(100);
		if(ret_HAL != HAL_OK)printf("Set RGB mode err\r\n");
		//i2cdat_w[0] = 0b01000011;
		i2cdat_w[0] = 0b00000011;
		i2cdat_w[1] = 0b00000000;
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x3A, 1, i2cdat_w, 2, 1000);
		HAL_Delay_C2A(100);
		if(ret_HAL != HAL_OK)printf("Set RGB mode err\r\n");
	}
	//Test mode -> monochrome
	if (0)
	{
		i2cdat_w[0] = 0x00;
		i2cdat_w[1] = 0x01;
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xF0, 1, i2cdat_w, 2, 1000); //select page 1
		//i2cdat_w[0] = 0b01000011;
		i2cdat_w[0] = 0x00;
		i2cdat_w[1] = 0x80;  //A0 -> mono , 80 -> default
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x08, 1, i2cdat_w, 2, 1000);
	}

	//Set Bayer output
	//Rx108[1] = 0
	//Rx108[0] = 1 -> RGGB
	if (0)
	{
		i2cdat_w[0] = 0x00;
		i2cdat_w[1] = 0x01;
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xF0, 1, i2cdat_w, 2, 1000); //select page 1
		i2cdat_w[0] = 0x02;
		i2cdat_w[1] = 0x00;
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x3A, 1, i2cdat_w, 2, 1000);

		i2cdat_w[0] = 0x00;
		i2cdat_w[1] = 0x01;
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xF0, 1, i2cdat_w, 2, 1000); //select page 1
		//i2cdat_w[0] = 0b01000011;
		i2cdat_w[0] = 0x00;
		i2cdat_w[1] = 0x81;
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x08, 1, i2cdat_w, 2, 1000);
	}
	//Set Image Size

	i2cdat_w[0] = 0x00;
	i2cdat_w[1] = 0x00;
	ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xF0, 1, i2cdat_w, 2, 1000); //select page 0
	if(ret_HAL != HAL_OK)printf("Set image size err\r\n");
	i2cdat_w[0] = 0x05;//0x05;
	i2cdat_w[1] = 0x00;//0x00;
	ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x04, 1, i2cdat_w, 2, 1000); //Horizontal A
	if(ret_HAL != HAL_OK)printf("Set image size err\r\n");
	i2cdat_w[0] = 0x04;//0x04;
	i2cdat_w[1] = 0x00;//0x00;
	ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x03, 1, i2cdat_w, 2, 1000); //Vertical A
	if(ret_HAL != HAL_OK)printf("Set image size err\r\n");

	//Set Image Size
	//0x280: 640, 0x200: 512
	//0x1E0: 480, 0x180: 384       memory -> 184320 Bytes  //Not working with small size
	//0x140: 320, 0x100: 256       memory -> 81920 Bytes
	//0xA0: 160, 0x80: 128
	if (1)
	{
		i2cdat_w[0] = 0x00;
		i2cdat_w[1] = 0x01;
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xF0, 1, i2cdat_w, 2, 1000); //select page 1
		if(ret_HAL != HAL_OK)printf("Set image size err\r\n");
		i2cdat_w[0] = 0x00; //0x00;
		i2cdat_w[1] = 0xA0; //0xA0; //160
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xA7, 1, i2cdat_w, 2, 1000); //Horizontal A
		if(ret_HAL != HAL_OK)printf("Set image size err\r\n");
		i2cdat_w[0] = 0x00; //0x00;
		i2cdat_w[1] = 0x80; //0x80; //128
		ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xAA, 1, i2cdat_w, 2, 1000); //Vertical A
		if(ret_HAL != HAL_OK)printf("Set image size err\r\n");
	}

	HAL_Delay_C2A(500);
	//printf("ID: \r\n");
	//HAL_UART_Transmit(&huart8,i2cdat_r,2,1000);


	//set context A
	i2cdat_w[0] = 0x00;
	i2cdat_w[1] = 0x00;
	ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xF0, 1, i2cdat_w, 2, 1000); //select page 0
	if(ret_HAL != HAL_OK)printf("Set contextA err\r\n");
	i2cdat_w[0] = 0x80;
	i2cdat_w[1] = 0x00; //row skip column skip
	ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x21, 1, i2cdat_w, 2, 1000);
	if(ret_HAL != HAL_OK)printf("Set contextA err\r\n");


	HAL_Delay_C2A(100);
	//Set PLL  :  f = fm(24) * M * (1/(2*(N+1)*(P+1)));
	i2cdat_w[0] = 0x00;
	i2cdat_w[1] = 0x00;
	ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0xF0, 1, i2cdat_w, 2, 1000); //select page 0
	if(ret_HAL != HAL_OK)printf("Set PLL err\r\n");
	i2cdat_w[0] = 0x10; // M  = 16
//	i2cdat_w[1] = 0x0B; //0x0B; // N =  11
	i2cdat_w[1] = 0x0B; //0x0B; // N =  11
	ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x66, 1, i2cdat_w, 2, 1000);
	if(ret_HAL != HAL_OK)printf("Set PLL err\r\n");
	HAL_Delay_C2A(100);
	i2cdat_w[0] = 0x05;
//	i2cdat_w[1] = 0x07; //0x07; // P = 7
	i2cdat_w[1] = 0x0F; // P = 15
	ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x67, 1, i2cdat_w, 2, 1000);
	if(ret_HAL != HAL_OK)printf("Set PLL err\r\n");
	HAL_Delay_C2A(100);
	i2cdat_w[0] = 0x80;
	i2cdat_w[1] = 0x00;
	ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x65, 1, i2cdat_w, 2, 1000);
	if(ret_HAL != HAL_OK)printf("Set PLL err\r\n");
	HAL_Delay_C2A(100);
	i2cdat_w[0] = 0x00;
	i2cdat_w[1] = 0x00;
	ret_HAL = HAL_I2C_Mem_Write_C2A(&hi2c3, 0xBA, 0x65, 1, i2cdat_w, 2, 1000);
	if(ret_HAL != HAL_OK)printf("Set PLL err\r\n");

	HAL_Delay_C2A(1000);
	HAL_IWDG_Refresh_C2A(&hiwdg);
	HAL_Delay_C2A(1000);
	HAL_IWDG_Refresh_C2A(&hiwdg);
	HAL_Delay_C2A(1000);
	HAL_IWDG_Refresh_C2A(&hiwdg);
	HAL_Delay_C2A(1000);
	HAL_IWDG_Refresh_C2A(&hiwdg);
	HAL_Delay_C2A(1000);
	HAL_IWDG_Refresh_C2A(&hiwdg);
	printf("Subcam registry configured\r\n");
	return ret_HAL;
}

uint8_t subcam_take_picture()
{
	uint8_t ret_HAL = 0;
	printf("DCMI_Start_DMA \r\n");
	ret_HAL = HAL_DCMI_Start_DMA_C2A(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)CAMERA_FRAME_BUFFER, ((uint32_t)IMAGE_SIZE));
//	HAL_Delay(1000);
//	HAL_IWDG_Refresh(&hiwdg);
//	HAL_Delay(1000);
//	HAL_IWDG_Refresh(&hiwdg);
//	HAL_Delay(1000);
//	HAL_IWDG_Refresh(&hiwdg);
//	HAL_Delay(1000);
//	HAL_IWDG_Refresh(&hiwdg);
//	HAL_Delay(1000);
//	HAL_IWDG_Refresh(&hiwdg);
	return ret_HAL;
}

//撮像したデータを送信する関数
int outputDCMI()
{
	printf("Converting to ASCII \r\n");

	for(uint16_t i = 0; i < IMAGE_SIZE*4 ; i++) {
		uint8_t * pTByte = (uint8_t *)(CAMERA_FRAME_BUFFER + i);
		uint8_t TByte = *pTByte;
		uint8_t TByte_ASCII[2];
		TByte_ASCII[0] = TByte >> 4;
		TByte_ASCII[1] = TByte & 0x0F;
		for(uint8_t j = 0; j < 2; j++) {
			if(TByte_ASCII[j] < 10){		// if it is 0~9 make ascii ‘0’~‘9’
				TByte_ASCII[j] += 0x30;
			}
			else{						// else 10~15 make ascii ‘A’~‘F’
				TByte_ASCII[j] += 0x37;
			}
		}
		uint8_t * pTByte_ASCII = (uint8_t *)(CAMERA_FRAME_ASCII + 2 * i);
		memcpy(pTByte_ASCII, TByte_ASCII, 2);
	}

    //ASCII変換したデータを送信
	HAL_UART_Transmit_C2A(&huart8,(uint8_t *) CAMERA_FRAME_ASCII,IMAGE_SIZE*2 ,0xFFFFFFFF);
	HAL_IWDG_Refresh_C2A(&hiwdg);
	HAL_UART_Transmit_C2A(&huart8,(uint8_t *) CAMERA_FRAME_ASCII + IMAGE_SIZE*2,IMAGE_SIZE*2 ,0xFFFFFFFF);
	HAL_IWDG_Refresh_C2A(&hiwdg);
	HAL_UART_Transmit_C2A(&huart8,(uint8_t *) CAMERA_FRAME_ASCII + IMAGE_SIZE*4,IMAGE_SIZE*2 ,0xFFFFFFFF);
	HAL_IWDG_Refresh_C2A(&hiwdg);
	HAL_UART_Transmit_C2A(&huart8,(uint8_t *) CAMERA_FRAME_ASCII + IMAGE_SIZE*6,IMAGE_SIZE*2 ,0xFFFFFFFF);
	HAL_IWDG_Refresh_C2A(&hiwdg);
	return 0;
}

void SCAM_default_setting(void){
	SCAM_driver->SCAM_start_block_addr = Storage_Area[Tlm_CODE_SUBCAM*2];
	SCAM_driver->SCAM_addr = 0x00000000;
	SCAM_driver->large_packet.addr = 0x00000000;
	SCAM_driver->large_packet.count = 0;
	SCAM_driver->large_packet.now_save_count = 0;
}

#endif/* for 1U*/
