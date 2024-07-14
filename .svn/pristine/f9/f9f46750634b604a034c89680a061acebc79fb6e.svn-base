/*
 * AOCS_cmd_Photo_COBC.h
 *
 *  ver.1.0 2022/08/12 Kanie
 *
 */


#include "AOCS_cmd_Photo_COBC.h"
#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Library/Delay.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Drivers/Power/PWR_Operation.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Applications/DriverInstances/di_cobc.h>
#include <string.h> //memcpy用

static void photo_cobc_load_param(void);
static void photo_cobc_initialize_param(void);

static void photo_cobc_init(void);
static void photo_cobc_exe(void);

void COBC_Photo_flag_send(void);
uint8_t vote_photo_flag(uint8_t* flag);

////////////////////MRAMパラメータの初期値/////////////////////////////

#ifdef TWO_U

// 時間条件をenableするフラグ
const uint8_t time_checker_ = 0xff;

// 光軸方向条件をenableするフラグ
const uint8_t direction_err_checker_ = 0xff;

// 角速度条件をenableするフラグ
const uint8_t omega_checker_ = 0xff;

// SCAM撮像からMCAM撮像までの時間上限[s]
const uint16_t time_limit_ = 600;

// 光軸方向
const float optical_axis_dir_[3] = {0.0f, 0.0f, 1.0f};

// 光軸方向と目標方向の許容角度誤差[deg]
const float photo_dir_err_ = 10.0f;

// 角速度の閾値[rad/s]
const float photo_omega_X_min_ = -0.05f;
const float photo_omega_X_max_ = 0.05f;
const float photo_omega_Y_min_ = -0.05f;
const float photo_omega_Y_max_ = 0.05f;
const float photo_omega_Z_min_ = -0.1f;
const float photo_omega_Z_max_ = 0.1f;

/////////////////////////////////////////////////////////////////

static PHOTO_STRUCT photo_driver;
PHOTO_STRUCT* const p_photo_driver = &photo_driver;

AppInfo photo_cobc_param_update(void)
{
	return AI_create_app_info("PHOTO_COBC_PARAM_UPDATE", photo_cobc_load_param, photo_cobc_initialize_param);
}

AppInfo photo_cobc_update(void)
{
    return AI_create_app_info("PHOTO_COBC_UPDATE", photo_cobc_init, photo_cobc_exe);
}

AppInfo COBC_Photo_Flag_Generator(void){
    return AI_create_app_info("COBC_PHOTO_FLAG_GENERATE", NULL, COBC_Photo_flag_send);
}

static void photo_cobc_load_param(void)
{
	printf("Photo COBC load param\r\n");
	read_param(&p_photo_driver->time_checker, &time_checker_, (uint16_t)sizeof(p_photo_driver->time_checker), (uint32_t)0x0000728F);
	read_param(&p_photo_driver->direction_err_checker, &direction_err_checker_, (uint16_t)sizeof(p_photo_driver->direction_err_checker), (uint32_t)0x00007290);
	read_param(&p_photo_driver->omega_checker, &omega_checker_, (uint16_t)sizeof(p_photo_driver->omega_checker), (uint32_t)0x00007291);
	read_param(&p_photo_driver->time_limit, &time_limit_, (uint16_t)sizeof(p_photo_driver->time_limit), (uint32_t)0x00007292);
	read_param(p_photo_driver->optical_axis_dir, optical_axis_dir_, (uint16_t)sizeof(p_photo_driver->optical_axis_dir), (uint32_t)0x00007294);
	read_param(&p_photo_driver->photo_dir_err, &photo_dir_err_, (uint16_t)sizeof(p_photo_driver->photo_dir_err), (uint32_t)0x000072A0);
	read_param(&p_photo_driver->photo_omega_X_min, &photo_omega_X_min_, (uint16_t)sizeof(p_photo_driver->photo_omega_X_min), (uint32_t)0x000072A4);
	read_param(&p_photo_driver->photo_omega_X_max, &photo_omega_X_max_, (uint16_t)sizeof(p_photo_driver->photo_omega_X_max), (uint32_t)0x000072A8);
	read_param(&p_photo_driver->photo_omega_Y_min, &photo_omega_Y_min_, (uint16_t)sizeof(p_photo_driver->photo_omega_Y_min), (uint32_t)0x000072AC);
	read_param(&p_photo_driver->photo_omega_Y_max, &photo_omega_Y_max_, (uint16_t)sizeof(p_photo_driver->photo_omega_Y_max), (uint32_t)0x000072B0);
	read_param(&p_photo_driver->photo_omega_Z_min, &photo_omega_Z_min_, (uint16_t)sizeof(p_photo_driver->photo_omega_Z_min), (uint32_t)0x000072B4);
	read_param(&p_photo_driver->photo_omega_Z_max, &photo_omega_Z_max_, (uint16_t)sizeof(p_photo_driver->photo_omega_Z_max), (uint32_t)0x000072B8);
}

static void photo_cobc_initialize_param(void)
{
	printf("Photo COBC initialize param\r\n");
	write_param(&p_photo_driver->time_checker, &time_checker_, (uint16_t)sizeof(p_photo_driver->time_checker), (uint32_t)0x0000728F);
	write_param(&p_photo_driver->direction_err_checker, &direction_err_checker_, (uint16_t)sizeof(p_photo_driver->direction_err_checker), (uint32_t)0x00007290);
	write_param(&p_photo_driver->omega_checker, &omega_checker_, (uint16_t)sizeof(p_photo_driver->omega_checker), (uint32_t)0x00007291);
	write_param(&p_photo_driver->time_limit, &time_limit_, (uint16_t)sizeof(p_photo_driver->time_limit), (uint32_t)0x00007292);
	write_param(p_photo_driver->optical_axis_dir, optical_axis_dir_, (uint16_t)sizeof(p_photo_driver->optical_axis_dir), (uint32_t)0x00007294);
	write_param(&p_photo_driver->photo_dir_err, &photo_dir_err_, (uint16_t)sizeof(p_photo_driver->photo_dir_err), (uint32_t)0x000072A0);
	write_param(&p_photo_driver->photo_omega_X_min, &photo_omega_X_min_, (uint16_t)sizeof(p_photo_driver->photo_omega_X_min), (uint32_t)0x000072A4);
	write_param(&p_photo_driver->photo_omega_X_max, &photo_omega_X_max_, (uint16_t)sizeof(p_photo_driver->photo_omega_X_max), (uint32_t)0x000072A8);
	write_param(&p_photo_driver->photo_omega_Y_min, &photo_omega_Y_min_, (uint16_t)sizeof(p_photo_driver->photo_omega_Y_min), (uint32_t)0x000072AC);
	write_param(&p_photo_driver->photo_omega_Y_max, &photo_omega_Y_max_, (uint16_t)sizeof(p_photo_driver->photo_omega_Y_max), (uint32_t)0x000072B0);
	write_param(&p_photo_driver->photo_omega_Z_min, &photo_omega_Z_min_, (uint16_t)sizeof(p_photo_driver->photo_omega_Z_min), (uint32_t)0x000072B4);
	write_param(&p_photo_driver->photo_omega_Z_max, &photo_omega_Z_max_, (uint16_t)sizeof(p_photo_driver->photo_omega_Z_max), (uint32_t)0x000072B8);
}

static void photo_cobc_init(void)
{
	p_photo_driver->counter = 0;
	p_photo_driver->photo_OK_flag = 0;
}

static void photo_cobc_exe(void)
{
#ifdef TWO_U
	if (cobc_driver->tlm_data.cobc_aocs_tlm.AttitudeControl_flag == 1){

		// フラグ初期化, 条件を満たしていなければフラグを落としていく
		p_photo_driver->condition[0] = 1;
		p_photo_driver->condition[1] = 1;
		p_photo_driver->condition[2] = 1;

		// カウントアップ
		p_photo_driver->counter += 1 ;

		// 時間の条件
		if (vote_photo_flag(&p_photo_driver->time_checker)){
			// 条件判断
			if (p_photo_driver->counter > p_photo_driver->time_limit){
				p_photo_driver->condition[0] = 0;
			}
		}

		// 光軸方向の条件
		if (vote_photo_flag(&p_photo_driver->direction_err_checker)){
			// 光軸方向
			float Od[3];
			Normalization(p_photo_driver->optical_axis_dir, 3, Od);

			// 目標方向
			float Td[3];
			Normalization(g_acs.p_ACS->TagetVec , 3, Td);

			// 角度誤差計算
			float err_deg = VecAngle(Od, Td, 3, 1);

			// 条件判断
			if (err_deg > p_photo_driver->photo_dir_err){
				p_photo_driver->condition[1] = 0;
			}
		}

		// 角速度の条件
		if (vote_photo_flag(&p_photo_driver->omega_checker)){
			// 角速度
			float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};

			// 条件判断
			if (omega[0] < p_photo_driver->photo_omega_X_min || omega[0] > p_photo_driver->photo_omega_X_max ||
				omega[1] < p_photo_driver->photo_omega_Y_min || omega[1] > p_photo_driver->photo_omega_Y_max ||
				omega[2] < p_photo_driver->photo_omega_Z_min || omega[2] > p_photo_driver->photo_omega_Z_max){
				p_photo_driver->condition[2] = 0;
			}
		}

		// すべての条件フラグが立っていれば条件を満たしているとする
		if (p_photo_driver->condition[0] * p_photo_driver->condition[1] * p_photo_driver->condition[2] == 1 && p_photo_driver->photo_OK_flag == 0){
			// 連続して撮像コマンドが送られないようにフラグを立てる
			p_photo_driver->photo_OK_flag = 1;

			// COBCに撮像コマンドを送る処理    要検討
			COBC_Photo_flag_send();
		}

	}
	else{
		p_photo_driver->counter = 0;
		p_photo_driver->photo_OK_flag = 0;
	}
#endif

}

void COBC_Photo_flag_send(void){
	static TCP tcp;

	uint8_t* param;
	param = (uint8_t*)&photo_driver.photo_OK_flag;

	uint8_t length = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN + sizeof(photo_driver.photo_OK_flag);

	CCP_form_rtc_to_other_obc(&tcp, (APID)TCP_APID_COBC_2U, (CMD_CODE)0x04D6, param, sizeof(photo_driver.photo_OK_flag));
	COBC_Send(cobc_driver, &tcp.packet[0], length);
}

//多数決システム
uint8_t vote_photo_flag(uint8_t* flag){
	uint8_t ret=0;
	for(int i=0;i<8;i++){
		ret+= ((*flag >> i) & 0x01);
	}
	if (ret>=4) {
		*flag = 0xff;
		return 1;
	}
	else {
		*flag = 0x00;
		return 0;
	}
}

#endif/*for 2U*/
