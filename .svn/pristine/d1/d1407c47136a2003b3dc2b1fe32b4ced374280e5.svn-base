#ifdef ONE_U
#include "ACS3_cmd_SUBCAM.h"
#include <stdio.h> //for printf
#include <string.h>
#include <src_user/IfWrapper/HAL.h>
#include <src_core/Library/endian.h>
#include <src_user/Library/General/DBG_printf.h>
#include "src_user/Library/Delay.h"
#include "src_user/Drivers/Sensor/TOA2BOA.h"
#include <src_user/Settings/System/flag_setting.h>


//static ACS3_SUBCAM_STRUCT subcamera;

uint8_t AH_flag = 0;
uint8_t init_flag = 0;
extern COMM_ANOMALY_STRUCT comm_anomaly_struct;
uint8_t* SRAM_Addr = (uint8_t*)0x60000000;
static uint32_t* scaled_data = (uint32_t*)(THUMBNAIL_ADDR);

static void ACS3_SUBCAM_init(void);
static void ACS3_SUBCAM_exe(void);

static void ACS3_SUBCAM_initialize_param(void);
static void ACS3_SUBCAM_load_param(void);

static void ACS3_SUBCAM_move_data_init(void);
static void ACS3_SUBCAM_move_data_exe(void);

// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS BEGIN *** //
uint8_t ov2640_regs_def[35] =
{
		0x82, //CLKRC_DOUBLE,
		0x02, //COM2_OUT_DRIVE_3x,
		0x78, //REG04_SET(REG04_VFLIP_IMG | REG04_VREF_EN | REG04_HREF_EN),
		0xE5, //COM8_SET(COM8_BNDF_EN | COM8_AGC_EN | COM8_AEC_EN),
		0x48, //COM9_AGC_SET(COM9_AGC_GAIN_8x),
		0xa0, //ARCOM2
		0x40, //AEW
		0x38, //AEB
		0x82, //VV_AGC_TH_SET(0x08, 0x02),
		0x22, //FLL
		0x3A, //COM3_BAND_SET(COM3_BAND_AUTO),
		0x55, //REG5D
		0x7d, //REG5E
		0x7d, //REG5F
		0x55, //REG60
		0x70, //HISTO_LOW
		0x80, //HISTO_HIGH
		0x04, //COM7_RES_UXGA | COM7_ZOOM_EN,
		0x00, //COM25
		0xbb, //BD50
		0x9c, //BD60
		0xc0, //MC_BIST_RESET | MC_BIST_BOOT_ROM_SEL, //MC_BIST
		0x14, //RESET_JPEG | RESET_DVP, //RESET
		0xd0, //CTRL3_BPC_EN | CTRL3_WPC_EN | 0x10, //CTRL3
		0x82, //R_DVP_SP_AUTO_MODE | 0x2, //R_DVP_SP
		0x00, //BPADDR
		0x00, //BPDATA
		0x03, //BPADDR
		0x48, //BPDATA
		0x48, //BPDATA
		0x08, //BPADDR
		0x20, //BPDATA
		0x10, //BPDATA
		0x0e, //BPDATA
		0x0c  //CTRL0_YUV422 | CTRL0_YUV_EN | CTRL0_RGB_EN, //CTRL0
};
//uint8_t ov2640_regs_def[35]; //load cmd

uint8_t ov2640_regs_rgb565[4] =
{
		0x00, //R_BYPASS_DSP_EN,
		0x08, //rgb
		0x00, //RESET
		0x00  //R_BYPASS_DSP_EN,
};
//uint8_t ov2640_regs_rgb565[4];

//	 ex.) FRAMESIZE_QQVGA = 9 (160*120)
uint8_t ov2640_regs_size[1] = {27};
//uint8_t ov2640_regs_size[1];

uint8_t ov2640_test[1] = {0x00};
//uint8_t ov2640_test[1];

int32_t ov2640_regs[6] = {0, 2, 2, -2, 0, 1000}; //which_param, satu(-2~2), contr(-2~2), bri(-2~2), exp_flag, exp_time
//int32_t ov2640_regs[6];

uint8_t ov2640_SD_block[1] = {5};
//uint8_t ov2640_SD_block[1];

uint8_t ov2640_jpg_quality[1] = {70};
//uint8_t ov2640_jpg_quality[1];

uint16_t ov2640_scaled_size[2] = {64, 48};
//uint16_t ov2640_scaled_size[2];

uint8_t ov2640_color_thred[2] = {7, 7};
//uint8_t ov2640_color_thred[2];

uint8_t ov2640_scaled_method[1] = {0}; //0:linear, 1:area average
//uint8_t ov2640_scaled_method[1];

float ov2640_TOA2BOA_arg[3] = {M_PI/2, 20.0f, 1.0f}; //Sz, N, mode
//static float ov2640_TOA2BOA_arg[3];

// *** DEFINITION OF MRAM_EEPROM INITIAL PARAMATERS END *** //

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo ACS3_SUBCAM_update(void)
{
    //create_app_info()を呼び出す．
    //
    return AI_create_app_info("sample", ACS3_SUBCAM_init, ACS3_SUBCAM_exe);
}

AppInfo ACS3_SUBCAM_param_update(void)
{
    return AI_create_app_info("sample_param_init", ACS3_SUBCAM_load_param, ACS3_SUBCAM_initialize_param);
}

AppInfo ACS3_SUBCAM_move_data(void)
{
    return AI_create_app_info("sample_param_init", ACS3_SUBCAM_move_data_init, ACS3_SUBCAM_move_data_exe);
}


static void ACS3_SUBCAM_load_param(void){	//アプリのinit関数
	//read (ToDo:What is MRAMaddr?)
//		printf("scam_param_init\r\n");
	//	MRAM_read_param(ov2640_regs_def,       ov2640_regs_def_,      sizeof(ov2640_regs_def_),      0x00006000);
	//	MRAM_read_param(ov2640_regs_rgb565,     ov2640_regs_rgb565_,    sizeof(ov2640_regs_rgb565_),    0x0000601A);
	//	MRAM_read_param(ov2640_regs_size,       ov2640_regs_size_,      sizeof(ov2640_regs_size_),      0x00006022);
	//	MRAM_read_param(ov2640_test,  ov2640_test_, sizeof(ov2640_test_), 0x00006034);
	//	MRAM_read_param(ov2640_regs,    ov2640_regs_,   sizeof(ov2640_regs_),   0x00006036);
	//	MRAM_read_param(ov2640_SD_block,    ov2640_SD_block_,   sizeof(ov2640_SD_block_),   0x00006080);
	//	MRAM_read_param(ov2640_jpg_quality,   ov2640_jpg_quality_,  sizeof(ov2640_jpg_quality_),  0x00006081);
	//	MRAM_read_param(ov2640_scaled_size,       ov2640_scaled_size_,      sizeof(ov2640_regs_2),      0x00006099);
	//	MRAM_read_param(ov2640_color_thred,     ov2640_color_thred_,    sizeof(ov2640_color_thred_),    0x000060FD);
	//	MRAM_read_param(ov2640_scaled_method,	     ov2640_scaled_method_,  	  sizeof(ov2640_scaled_method_),	  0x0000611F);
	//	MRAM_read_param(ov2640_TOA2BOA_arg  ,ov2640_TOA2BOA_arg_, sizeof(ov2640_TOA2BOA_arg_), 0x00006153);

}

static void ACS3_SUBCAM_initialize_param(void){	//アプリのexe関数
	//write (ToDo:What is MRAMaddr?)
		printf("scam_param_exe\r\n");
	//	MRAM_write_param(ov2640_regs_0,       ov2640_regs_0_,      sizeof(ov2640_regs_0),      0x00006000);
	//	MRAM_write_param(ov2640_regs_CLK,     ov2640_regs_CLK_,    sizeof(ov2640_regs_CLK),    0x0000601A);
	//	MRAM_write_param(ov2640_regs_1,       ov2640_regs_1_,      sizeof(ov2640_regs_1),      0x00006022);
	//	MRAM_write_param(ov2640_regs_format,  ov2640_regs_format_, sizeof(ov2640_regs_format), 0x00006034);
	//	MRAM_write_param(ov2640_regs_lens,    ov2640_regs_lens_,   sizeof(ov2640_regs_lens),   0x00006036);
	//	MRAM_write_param(ov2640_regs_size,    ov2640_regs_size_,   sizeof(ov2640_regs_size),   0x00006080);
	//	MRAM_write_param(ov2640_regs_ganma,   ov2640_regs_ganma_,  sizeof(ov2640_regs_ganma),  0x00006081);
	//	MRAM_write_param(ov2640_regs_2,       ov2640_regs_2_,      sizeof(ov2640_regs_2),      0x00006099);
	//	MRAM_write_param(ov2640_regs_exp,     ov2640_regs_exp_,    sizeof(ov2640_regs_exp),    0x000060FD);
	//	MRAM_write_param(ov2640_context,	  ov2640_context_,     sizeof(ov2640_context),     0x0000611F);
	//	MRAM_write_param(ov2640_test,    	  ov2640_test_,   	   sizeof(ov2640_test),		   0x00006120);
	//	MRAM_write_param(ov2640_SD_block,     ov2640_SD_block_,    sizeof(ov2640_SD_block_),   0x00006121);
	//	MRAM_write_param(ov2640_init_flag,    ov2640_init_flag_,   sizeof(ov2640_init_flag_),  0x00006122);
	//	MRAM_write_param(ov2640_jpg_quality,  ov2640_jpg_quality_, sizeof(ov2640_jpg_quality_),0x00006123);
	//	MRAM_write_param(ov2640_scaled_size,  ov2640_scaled_size_, sizeof(ov2640_scaled_size_),0x00006124);
	//	MRAM_write_param(ov2640_regs_AWB,	  ov2640_regs_AWB_,	   sizeof(ov2640_regs_AWB_),   0x00006128);
	//	MRAM_write_param(ov2640_color_thred,  ov2640_color_thred_, sizeof(ov2640_color_thred), 0x00006150);
	//	MRAM_write_param(ov2640_scaled_method,ov2640_scaled_method_,sizeof(ov2640_scaled_method),0x00006152);
	//	MRAM_write_param(ov2640_TOA2BOA_arg  ,ov2640_TOA2BOA_arg_, sizeof(ov2640_TOA2BOA_arg), 0x00006153);

}

static void ACS3_SUBCAM_init(void)
{
//	printf("SCAM_init\r\n");
	SUBCAM_Init(); //変数の宣言
//	subcam_reset();
}

static void ACS3_SUBCAM_exe(void)
{

}

//for DBG (Now, we don't use.)
//uint8_t param = 9;
CCP_CmdRet Cmd_ACS3_SUBCAM(const CommonCmdPacket* packet)
{
	uint8_t ret;

	printf("Power Up SCAM module\n");
	subcam_init();
	HAL_Delay(100);

	//Set Format
	subcam_set_reg();
	HAL_Delay(100);

//	subcam_set_colorbar(false);
	HAL_Delay(100);

	//Set Framsize
	subcam_set_Framesize(ov2640_regs_size[0]);
	HAL_Delay(100);

	if (ov2640_test[0] == 0x00){
		subcam_set_colorbar(false);
	}
	else if (ov2640_test[0] == 0x01){
		subcam_set_colorbar(true);
	}

//	if(ov2640_regs_AWB[0] == 1){
//		scam.set_disAWB();
//	}


	subcam_bmp_capture(ov2640_regs_size[0]);

	HAL_Delay(10);

	subcam_reset();
	printf("[1U_SCAM]Task done\n");
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief  power on ->  set register(camera parameters) to SCAM
 * @return  none
 */
CCP_CmdRet Cmd_ACS3_SUBCAM_initialize(const CommonCmdPacket* packet){
//	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t ret = 1;

	// set RESET pin, and STANDBY pin of SCAM and confirm communication with SCAM by I2C
	printf("Power Up SCAM module\n");
	ret = subcam_init();

	if(ret == 0){
		//Set Format
		subcam_set_reg();

		//Set Frame size
		subcam_set_Framesize(ov2640_regs_size[0]);

		if (ov2640_test[0] == 0x00){
			subcam_set_colorbar(false);
		}
		else if (ov2640_test[0] == 0x01){
			subcam_set_colorbar(true);
		}

//		if(ov2640_regs_AWB[0] == 1){
	//		scam.set_disAWB();
//		}

		init_flag = 1;
		printf("subcam initialize finish\n");
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief  撮影コマンド(シャッターを切りたいときに送信)
 * take picture -> make thumbnail -> TOA2BOA -> Standby mode
 * @return  none
 */
CCP_CmdRet Cmd_ACS3_SUBCAM_take_pic(const CommonCmdPacket* packet)
{
	if (init_flag == 1){
		subcam_bmp_capture(ov2640_regs_size[0]);
		RGB24_to_thumbnail(ov2640_regs_size[0]);

		uint32_t size = ov2640_scaled_size[0]*ov2640_scaled_size[1];
		float Sz = ov2640_TOA2BOA_arg[0];
		uint32_t N = ov2640_TOA2BOA_arg[1];
		uint8_t mode = ov2640_TOA2BOA_arg[2];
		uint8_t cam = 2;
		uint8_t *BOA = 0;
		TOA2BOA_DOS(scaled_data, size, Sz, N, mode, cam, BOA);
		save_SD_BOA(scaled_data, size);

		subcam_reset(); //move to Standby mode of SCAM
		printf("[SCAM] TaskDone!\n");
		init_flag = 0;
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}
	else{
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}
}

CCP_CmdRet Cmd_ACS3_SUBCAM_read_reg(const CommonCmdPacket* packet)
{
	uint8_t ret = 0;
	uint16_t dummy = 0;

//	ret = subcam_read_reg(ROW_WIDTH, (uint8_t *)&dummy);
//	printf("ROW_WIDTH %04x\n", dummy);
//	ret = subcam_read_reg(COL_WIDTH, (uint8_t *)&dummy);
//	printf("COL_WIDTH %04x\n", dummy);
//	ret = subcam_read_reg(HORIZON_ZOOM, (uint8_t *)&dummy);
//	printf("HORIZON_ZOOM %04x\n", dummy);
//	ret = subcam_read_reg(VERTICL_ZOOM, (uint8_t *)&dummy);
//	printf("VERTICL_ZOOM %04x\n", dummy);
//	ret = subcam_read_reg(HORIZON_PAN, (uint8_t *)&dummy);
//	printf("HORIZON_PAN %04x\n", dummy);
//	ret = subcam_read_reg(VERTICL_PAN, (uint8_t *)&dummy);
//	printf("VERTICL_PAN %04x\n", dummy);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//* *      Author: Osaki
// * @brief k枚目の写真の先頭addrの情報を、send_content[8]~[11]に移す
// *        k枚目のImsizeを、send_content[0]~[3]に移す
// * packet[15] : SCAM->1, SCAM(thumbnail)->5, SCAM(input)->9
// * packet[16] : k枚目(0<=k<=127)
//*/
CCP_CmdRet Cmd_CAMaddr_Renew(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t ret_w = 2, ret_r = 2;
	size_t info_size = 512;
	uint8_t send_content[info_size];
	uint8_t info_buf[info_size*2];
	uint32_t SCAM_addr = Storage_Area[Tlm_CODE_SUBCAM*2]; //plan is 0x0000C100;
	uint32_t SCAMbmp_addr = SCAM_addr + 20;
	uint32_t SCAMscaled_addr = SCAMbmp_addr + 131072;
	uint32_t SCAM_BOA_addr = SCAMscaled_addr + 10240;

	printf("CAMaddr_Renew\r\n");

	if(param[0]==1){
		printf("for SCAM\r\n");
		ret_r = sd_read_address(SCAM_addr + 1, 0x00000000, info_buf, info_size*2);
		if(ret_r != 0){
			printf("read err(init)\r\n");
		}
		send_content[0]  = info_buf[info_size + 4*param[1]    ];
		send_content[1]  = info_buf[info_size + 4*param[1] + 1];
		send_content[2]  = info_buf[info_size + 4*param[1] + 2];
		send_content[3]  = info_buf[info_size + 4*param[1] + 3];
		send_content[4]  = SCAMbmp_addr >> 24;
		send_content[5]  = SCAMbmp_addr >> 16;
		send_content[6]  = SCAMbmp_addr >> 8;
		send_content[7]  = SCAMbmp_addr;
		send_content[8]  = info_buf[4*param[1]];
		send_content[9]  = info_buf[4*param[1] + 1];
		send_content[10] = info_buf[4*param[1] + 2];
		send_content[11] = info_buf[4*param[1] + 3];
		ret_w = sd_write_address(SCAM_addr,
				0x00000000, send_content, info_size);
		if(ret_w != 0){
			printf("write err(init)\r\n");
		}
	}

	else if(param[0]==5){
		printf("for SCAM(thumbnail)\r\n");
		ret_r = sd_read_address(SCAM_addr + 3, 0x00000000, info_buf, info_size*2);
		if(ret_r != 0){
			printf("read err(init)\r\n");
		}
		send_content[0]  = info_buf[info_size + 4*param[1]    ];
		send_content[1]  = info_buf[info_size + 4*param[1] + 1];
		send_content[2]  = info_buf[info_size + 4*param[1] + 2];
		send_content[3]  = info_buf[info_size + 4*param[1] + 3];
		send_content[4]  = SCAMscaled_addr >> 24;
		send_content[5]  = SCAMscaled_addr >> 16;
		send_content[6]  = SCAMscaled_addr >> 8;
		send_content[7]  = SCAMscaled_addr ;
		send_content[8]  = info_buf[4*param[1]];
		send_content[9]  = info_buf[4*param[1] + 1];
		send_content[10] = info_buf[4*param[1] + 2];
		send_content[11] = info_buf[4*param[1] + 3];
		ret_w = sd_write_address(SCAM_addr, //SCAM_addr!!!
				0x00000000, send_content, info_size);
		if(ret_w != 0){
			printf("write err(init)\r\n");
		}
	}

	else if(param[0]==9){
		printf("for SCAM(input)\r\n");
		ret_r = sd_read_address(SCAM_addr + 5, 0x00000000, info_buf, info_size*2);
		if(ret_r != 0){
			printf("read err(init)\r\n");
		}
		send_content[0]  = info_buf[info_size + 4*param[1]    ];
		send_content[1]  = info_buf[info_size + 4*param[1] + 1];
		send_content[2]  = info_buf[info_size + 4*param[1] + 2];
		send_content[3]  = info_buf[info_size + 4*param[1] + 3];
		send_content[4]  = SCAM_BOA_addr >> 24;
		send_content[5]  = SCAM_BOA_addr >> 16;
		send_content[6]  = SCAM_BOA_addr >> 8;
		send_content[7]  = SCAM_BOA_addr;
		send_content[8]  = info_buf[4*param[1]];
		send_content[9]  = info_buf[4*param[1] + 1];
		send_content[10] = info_buf[4*param[1] + 2];
		send_content[11] = info_buf[4*param[1] + 3];
		ret_w = sd_write_address(SCAM_addr,
				0x00000000, send_content, info_size);
		if(ret_w != 0){
			printf("write err(init)\r\n");
		}
	}

	//for debug
	ret_r = sd_read_address(SCAM_addr,
				0x00000000, send_content, info_size);
	if(ret_r != 0){
		printf("read err\r\n");
	}
	printf("SCAM\r\n");
	uint32_t size = (send_content[0] << 24) | (send_content[1] << 16) | (send_content[2] << 8) | send_content[3];
	printf("size = %d\n", size);
	uint32_t start_block_addr = (send_content[4] << 24) | (send_content[5] << 16) | (send_content[6] << 8) | send_content[7];
	printf("start_block_addr = %x ", start_block_addr);
	uint32_t addr_to_read =  (send_content[8] << 24) | (send_content[9] << 16) | (send_content[10] << 8) | send_content[11];
	printf("addr_to_read = %x ", addr_to_read);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/* Author: Osaki
 * @brief　 Cmd of initializing SD content (image ID, image address) before launch
            打ち上げ前に、撮影画像についてのSDの中身 (画像ID, 保存するaddress) を初期化するCmd
*/
CCP_CmdRet SDcontents_init(const CommonCmdPacket* packet){
	printf("SDcontents_init\n");
	static size_t SD_size = 512;
	static uint8_t send_content[512] = {0};
	static uint8_t info_buf[512*2] = {0};
	static uint8_t ret_w = 2;

//SCAM
	uint32_t SCAM_start_block_addr = Storage_Area[Tlm_CODE_SUBCAM*2]; //plan is 0x0000C100
	printf("SCAM_start_block_addr = %08x\n", SCAM_start_block_addr);
	//bmp
	send_content[12] = 0xff;
	send_content[13] = 0xff; //ImageID
//	send_content[14] = 0x00; //array_num (for MCAM sort)
//	send_content[15] = 0x00; //init (for MCAM sort)
	send_content[16] = 0x00; //pic_num(=TLM Status)
	ret_w = sd_write_address(SCAM_start_block_addr,
			0x00000000, send_content, SD_size);
	if (ret_w != 0){
		printf("write err\r\n");
	}
	ret_w = sd_write_address(SCAM_start_block_addr + 1,
			0x00000000, info_buf, SD_size*2);
	if (ret_w != 0){
		printf("write err\r\n");
	}

	// thumbnail
	ret_w = sd_write_address(SCAM_start_block_addr + 3,
			0x00000000, info_buf, SD_size*2);
	if (ret_w != 0){
		printf("write err\r\n");
	}

	//BOA(input)
	ret_w = sd_write_address(SCAM_start_block_addr + 5,
			0x00000000, info_buf, SD_size*2);
	if (ret_w != 0){
		printf("write err\r\n");
	}

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);

}

/* When anomaly, we use below commands. */
CCP_CmdRet Cmd_ACS3_SUBCAM_re_initialize(const CommonCmdPacket* packet){
//	for (int i=0; i<9; i++){
//		printf("SCAM_param = %02x\n", *(SCAM_param + i));
//	}
	ANOMALY_DRIVER_STRUCT *str = &comm_anomaly_struct.i2c.anomaly[I2C_FAULT_SCAM];
	uint8_t ret = 1;

	if(vote_flag(&str->anomaly_detect_lv1)){
	    flag_reset(&str->anomaly_detect_lv1);
		printf("Power Up SCAM module\n");
		ret = subcam_init();
	//	HAL_Delay(100);

		if(ret == 0){
			//Set Format
			subcam_set_reg();
	//		HAL_Delay(100);

		//	subcam_set_colorbar(false);
	//		HAL_Delay(100);

			//Set Framsize
			subcam_set_Framesize(ov2640_regs_size[0]);
//			subcam_set_Framesize(param);
	//		HAL_Delay(100);

			if (ov2640_test[0] == 0x00){
				subcam_set_colorbar(false);
			}
			else if (ov2640_test[0] == 0x01){
				subcam_set_colorbar(true);
			}

//			if(ov2640_regs_AWB[0] == 1){
		//		scam.set_disAWB();
//			}

			printf("subcam initialize finish\n");
		}
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

static void ACS3_SUBCAM_move_data_init(void){
	SCAM_driver->large_packet.mode_flag = NOT_OPERATED;
	SCAM_driver->large_packet.sram_input_num = 0;
	SCAM_driver->large_packet.ImSize = 0;
	SCAM_driver->large_packet.start_block_addr = 0x00000000;
	SCAM_default_setting();
}

static void ACS3_SUBCAM_move_data_exe(void){
	if(SCAM_driver->large_packet.mode_flag == NOT_OPERATED){
		return;
	}
	else if(SCAM_driver->large_packet.mode_flag == DATA_ANALYZE){
		SCAM_default_setting();
		uint8_t temp[ANALYZE_SIZE] = {0};
		c2a_delay_ms(2);
		sd_read_address(SCAM_driver->SCAM_start_block_addr, SCAM_driver->SCAM_addr, &temp[0], ANALYZE_SIZE);

		SCAM_driver->large_packet.ImSize = temp[0] << 24 | temp[1] << 16 |temp[2] << 8 |temp[3];
		SCAM_driver->large_packet.start_block_addr = (temp[4] << 24 | temp[5] << 16 | temp[6] << 8 |temp[7]);
		SCAM_driver->large_packet.addr = temp[8] << 24 | temp[9] << 16 | temp[10] << 8 | temp[11];
		SCAM_driver->large_packet.picture_id = temp[12] << 8| temp[13];

		SCAM_driver->large_packet.count = (SCAM_driver->large_packet.ImSize / ANALYZE_SIZE) + 1;

		printf("start_block_addr is %08x \r\n",SCAM_driver->large_packet.start_block_addr);
		printf("addr is %08x\r\n",SCAM_driver->large_packet.addr);
		printf("ImSize is %d \r\n",SCAM_driver->large_packet.ImSize);
		printf("count is %d \r\n",SCAM_driver->large_packet.count);

		for(int i = 0; i < 512; i++){
			print(SUBCAM, "%02x",temp[i]);
		}
		print(SUBCAM, "\r\n");

		SCAM_driver->large_packet.mode_flag = SD_TO_SRAM;
	}
	else if(SCAM_driver->large_packet.mode_flag == SD_TO_SRAM){
		printf("analyze_start \r\n");
		for(int i = 0; i < 4; i++){
			c2a_delay_ms(2);
			uint8_t ret_r;
			ret_r = sd_read_address(SCAM_driver->large_packet.start_block_addr,
					SCAM_driver->large_packet.addr + (SCAM_driver->large_packet.now_save_count*ANALYZE_SIZE),
					&SRAM_Addr[0] + (SCAM_driver->large_packet.now_save_count*ANALYZE_SIZE) , ANALYZE_SIZE);
			if(ret_r != 0){
				SCAM_default_setting();
				printf("read err\r\n");
				SCAM_driver->large_packet.mode_flag = NOT_OPERATED;
			}

			for(int i = 0; i < ANALYZE_SIZE; i++){
				print(SUBCAM,"%02x",SRAM_Addr[i] + (SCAM_driver->large_packet.now_save_count*ANALYZE_SIZE));
			}
			print(SUBCAM,"\r\n");

			++SCAM_driver->large_packet.now_save_count;
			printf("now_save_count is %d \r\n",SCAM_driver->large_packet.now_save_count);

			if(SCAM_driver->large_packet.now_save_count >= SCAM_driver->large_packet.count){
				printf("count is %d \r\n",SCAM_driver->large_packet.count);
				++SCAM_driver->large_packet.sram_input_num;
				SCAM_driver->large_packet.mode_flag = NOT_OPERATED;
				return;
			}
		}
	}
}

CCP_CmdRet Cmd_ACS3_SUBCAM_default_setting(const CommonCmdPacket* packet){
	SCAM_default_setting();
	SCAM_driver->large_packet.move_data_flag = NOT_OPERATED;
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_ACS3_SUBCAM_SD_to_SRAM(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);

	if(param[0] == 0){
		SCAM_driver->large_packet.mode_flag = NOT_OPERATED;
	}
	else if(param[0] == 1){
		SCAM_driver->large_packet.mode_flag = DATA_ANALYZE;
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}



#endif/* for 1U*/
