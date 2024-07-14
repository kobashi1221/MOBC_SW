/*
 * AOCS_tlm_param.c
 *
 *  Created on: 2022/08/03
 *      Author: daiki
 */

#include "AOCS_tlm_param.h"
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/TlmCmd/telemetry_source.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/UserDefined/Global_Sensor_Value.h>

static AOCS_TLM aocs_tlm[3];
const AOCS_TLM aocs_tlm_[3] = {{0x01801B3C, 0x00000000, 0x00000000, 0x00000000},
							   {0x02002443, 0x00145400, 0x00000000, 0x00000000},
							   {0x087EC080, 0x00000000, 0x00000020, 0x00000000}};

static void AOCS_tlm_param_load(void);
static void AOCS_tlm_param_init(void);

AppInfo AOCS_tlm_param_update(void)
{
    return AI_create_app_info("AOCS_TLM", AOCS_tlm_param_load, AOCS_tlm_param_init);
}

static void AOCS_tlm_param_load(void){
	read_param(aocs_tlm, aocs_tlm_, (size_t)(sizeof(aocs_tlm_)),(uint32_t)0x00002000);
}

static void AOCS_tlm_param_init(void){
	write_param(aocs_tlm, aocs_tlm_, (size_t)(sizeof(aocs_tlm_)),(uint32_t)0x00002000);
}

static uint8_t AOCS_tlm_Analyze(int pos, AOCS_TLM_FLAG flag){
	uint8_t ret = 0;
	uint8_t byte = flag / 32;
	uint8_t rest = flag % 32;
	ret = (aocs_tlm[pos].tlm_flag[byte] >> rest)&0x01;

	return ret; //1: enabled, 0: disabled
}

void AOCS_tlm_pack(int pos, int tlm_len, unsigned char* contents){
	int i = 0;

	TF_copy_u32(&contents[i], (uint32_t)(aocs_tlm[pos].tlm_flag[0]));
	i += 4;
	TF_copy_u32(&contents[i], (uint32_t)(aocs_tlm[pos].tlm_flag[1]));
	i += 4;
	TF_copy_u32(&contents[i], (uint32_t)(aocs_tlm[pos].tlm_flag[2]));
	i += 4;
	TF_copy_u32(&contents[i], (uint32_t)(aocs_tlm[pos].tlm_flag[3]));
	i += 4;

	if(AOCS_tlm_Analyze(pos, Sensor_time)){
		if(tlm_len < i + 2) return;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_acs.p_GLOBAL_AOCS->Sensor_time));
	  	i += 2;
	}

	if(AOCS_tlm_Analyze(pos, nineax_time)){
		if(tlm_len < i + 2) return;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_acs.p_GLOBAL_AOCS->NineAxTime.step));
	  	i += 2;
	}

	if(AOCS_tlm_Analyze(pos, nineax_gyro_raw_data)){
		if(tlm_len < i + 6) return;

	  	TF_copy_i16(&contents[i], (int16_t)(g_sensor.p_imu->GYRO_HK[0]));
	  	i += 2;
	  	TF_copy_i16(&contents[i], (int16_t)(g_sensor.p_imu->GYRO_HK[1]));
	  	i += 2;
	  	TF_copy_i16(&contents[i], (int16_t)(g_sensor.p_imu->GYRO_HK[2]));
	  	i += 2;
	}

	if(AOCS_tlm_Analyze(pos, nineax_mag_raw_data)){
		if(tlm_len < i + 6) return;

	  	TF_copy_i16(&contents[i], (int16_t)(g_sensor.p_imu->MAG_HK[0]));
	  	i += 2;
	  	TF_copy_i16(&contents[i], (int16_t)(g_sensor.p_imu->MAG_HK[1]));
	  	i += 2;
	  	TF_copy_i16(&contents[i], (int16_t)(g_sensor.p_imu->MAG_HK[2]));
	  	i += 2;
	}

	if(AOCS_tlm_Analyze(pos, nineax_acc_raw_data)){
		if(tlm_len < i + 6) return;

	  	TF_copy_i16(&contents[i], (int16_t)(g_sensor.p_imu->ACC_HK[0]));
	  	i += 2;
	  	TF_copy_i16(&contents[i], (int16_t)(g_sensor.p_imu->ACC_HK[1]));
	  	i += 2;
	  	TF_copy_i16(&contents[i], (int16_t)(g_sensor.p_imu->ACC_HK[2]));
	  	i += 2;
	}

	if(AOCS_tlm_Analyze(pos, nineax_temp)){
		if(tlm_len < i + 2) return;

		TF_copy_u16(&contents[i], (uint16_t)g_sensor.p_adc->ADC_NINEAX_TEMP);
		i += 2;
	}

	if(AOCS_tlm_Analyze(pos, mems_gyro_time)){
		if(tlm_len < i + 2) return;

		TF_copy_u16(&contents[i], (uint16_t)g_acs.p_GLOBAL_AOCS->MemsGyroTime.step);
		i += 2;
	}

	if(AOCS_tlm_Analyze(pos, mems_gyro_range)){
		if(tlm_len < i + 3) return;
		TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_adc->state[0]));
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_adc->state[1]));
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_adc->state[2]));
		i += 1;
	}

	if(AOCS_tlm_Analyze(pos, mems_gyro_raw_data)){
		if(tlm_len < i + 6) return;

	#ifdef TWO_U
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_OMEGAX));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_OMEGAY));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_OMEGAZ));
	  	i += 2;
	#endif
	#ifdef ONE_U
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_gyro->ADC_OMEGAX));
	  	i += 2;
		TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_gyro->ADC_OMEGAY));
		i += 2;
		TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_gyro->ADC_OMEGAZ));
		i += 2;
	#endif
	}

	if(AOCS_tlm_Analyze(pos, mems_gyro_temp)){
		if(tlm_len < i + 6) return;

	  	TF_copy_u16(&contents[i], (uint16_t)g_sensor.p_adc->ADC_GYROX_TEMP);
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)g_sensor.p_adc->ADC_GYROY_TEMP);
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)g_sensor.p_adc->ADC_GYROZ_TEMP);
	  	i += 2;
	}

	if(AOCS_tlm_Analyze(pos, mems_mag_time)){
		if(tlm_len < i + 2) return;

		TF_copy_u16(&contents[i], (uint16_t)g_acs.p_GLOBAL_AOCS->MemsMagTime.step);
		i += 2;
	}

	if(AOCS_tlm_Analyze(pos, mems_mag_raw_data)){
		if(tlm_len < i + 12) return;
	#ifdef TWO_U
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_MAGSPX));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_MAGSMX));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_MAGSPY));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_MAGSMY));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_MAGSPZ));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_MAGSMZ));
	  	i += 2;
	#endif/*for 2U*/
	}

	if(AOCS_tlm_Analyze(pos, mems_mag_temp)){
		if(tlm_len < i + 2) return;

		TF_copy_u16(&contents[i], (uint16_t)g_sensor.p_adc->ADC_MAGS_TEMP);
		i += 2;
	}

	if(AOCS_tlm_Analyze(pos, suns_time)){
		if(tlm_len < i + 12) return;

		TF_copy_u16(&contents[i], (uint16_t)g_acs.p_GLOBAL_AOCS->SunsTime[0].step);
		i += 2;
		TF_copy_u16(&contents[i], (uint16_t)g_acs.p_GLOBAL_AOCS->SunsTime[1].step);
		i += 2;
		TF_copy_u16(&contents[i], (uint16_t)g_acs.p_GLOBAL_AOCS->SunsTime[2].step);
		i += 2;
		TF_copy_u16(&contents[i], (uint16_t)g_acs.p_GLOBAL_AOCS->SunsTime[3].step);
		i += 2;
		TF_copy_u16(&contents[i], (uint16_t)g_acs.p_GLOBAL_AOCS->SunsTime[4].step);
		i += 2;
		TF_copy_u16(&contents[i], (uint16_t)g_acs.p_GLOBAL_AOCS->SunsTime[5].step);
		i += 2;
	}

	if(AOCS_tlm_Analyze(pos, suns_error_num)){
		if(tlm_len < i + 6) return;
	    TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_suns[0].state));
	    i += 1;
	    TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_suns[1].state));
	    i += 1;
	    TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_suns[2].state));
	    i += 1;
	    TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_suns[3].state));
	    i += 1;
	    TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_suns[4].state));
	    i += 1;
	    TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_suns[5].state));
	    i += 1;
	}

	if(AOCS_tlm_Analyze(pos, suns_unfiltered)){
		if(tlm_len < i + 80) return;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[0].UNFILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[0].UNFILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[0].UNFILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[0].UNFILTERED_PHOTOCELL[3]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[1].UNFILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[1].UNFILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[1].UNFILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[1].UNFILTERED_PHOTOCELL[3]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[2].UNFILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[2].UNFILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[2].UNFILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[2].UNFILTERED_PHOTOCELL[3]);
	    i += 4;
	#ifdef TWO_U
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[3].UNFILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[3].UNFILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[3].UNFILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[3].UNFILTERED_PHOTOCELL[3]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[4].UNFILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[4].UNFILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[4].UNFILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[4].UNFILTERED_PHOTOCELL[3]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[5].UNFILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[5].UNFILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[5].UNFILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[5].UNFILTERED_PHOTOCELL[3]);
	    i += 4;
	#endif/*for 2U*/
    #ifdef ONE_U
	    i += 48;
    #endif/*for 1U*/
	}

	if(AOCS_tlm_Analyze(pos, suns_filtered)){
		if(tlm_len < i + 80) return;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[0].FILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[0].FILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[0].FILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[0].FILTERED_PHOTOCELL[3]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[1].FILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[1].FILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[1].FILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[1].FILTERED_PHOTOCELL[3]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[2].FILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[2].FILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[2].FILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[2].FILTERED_PHOTOCELL[3]);
	    i += 4;
	#ifdef TWO_U
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[3].FILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[3].FILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[3].FILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[3].FILTERED_PHOTOCELL[3]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[4].FILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[4].FILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[4].FILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[4].FILTERED_PHOTOCELL[3]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[5].FILTERED_PHOTOCELL[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[5].FILTERED_PHOTOCELL[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[5].FILTERED_PHOTOCELL[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[5].FILTERED_PHOTOCELL[3]);
	    i += 4;
	#endif/*for 2U*/
	#ifdef ONE_U
		i += 48;
	#endif/*for 1U*/
	}

	if(AOCS_tlm_Analyze(pos, suns_angle)){
		if(tlm_len < i + 24) return;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[0].ANGULAR_POSITION[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[0].ANGULAR_POSITION[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[1].ANGULAR_POSITION[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[1].ANGULAR_POSITION[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[2].ANGULAR_POSITION[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[2].ANGULAR_POSITION[1]);
	    i += 4;
	#ifdef TWO_U
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[3].ANGULAR_POSITION[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[3].ANGULAR_POSITION[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[4].ANGULAR_POSITION[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[4].ANGULAR_POSITION[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[5].ANGULAR_POSITION[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_sensor.p_suns[5].ANGULAR_POSITION[1]);
	    i += 4;
	#endif/*for 2U*/
	#ifdef ONE_U
		i += 24;
	#endif/*for 1U*/
	}


	if(AOCS_tlm_Analyze(pos, csuns_value)){
		if(tlm_len < i + 6) return;
	#ifdef TWO_U
		i += 6;
	#endif/*for 1U*/
	#ifdef ONE_U
// NUcube1+ではチップサンセンサをMOBCで使用しない
//	    TF_copy_float(&contents[i], (uint16_t)g_sensor.p_csuns[0].Value);
//	    i += 2;
//	    TF_copy_float(&contents[i], (uint16_t)g_sensor.p_csuns[1].Value);
//	    i += 2;
//	    TF_copy_float(&contents[i], (uint16_t)g_sensor.p_csuns[2].Value);
//	    i += 2;
	    i += 6;
	#endif/*for 1U*/
	}

	if(AOCS_tlm_Analyze(pos, csuns_peak)){
		if(tlm_len < i + 12) return;
	#ifdef TWO_U
		i += 12;
	#endif/*for 1U*/
	#ifdef ONE_U
// NUcube1+ではチップサンセンサをMOBCで使用しない
//	    TF_copy_float(&contents[i], (float)g_sensor.p_csuns[0].Peak);
//	    i += 4;
//	    TF_copy_float(&contents[i], (float)g_sensor.p_csuns[1].Peak);
//	    i += 4;
//	    TF_copy_float(&contents[i], (float)g_sensor.p_csuns[2].Peak);
//	    i += 4;
	    i += 12;
	#endif/*for 1U*/
	}

	if(AOCS_tlm_Analyze(pos, gps_utc)){
		if(tlm_len < i + 7) return;

	    TF_copy_u16(&contents[i], (uint16_t)g_sensor.p_gps->GPS_UTC_YEAR);
	    i += 2;
		TF_copy_u8(&contents[i], (uint8_t)g_sensor.p_gps->GPS_UTC_MONTH);
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)g_sensor.p_gps->GPS_UTC_DAY);
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)g_sensor.p_gps->GPS_UTC_HOUR);
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)g_sensor.p_gps->GPS_UTC_MIN);
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)g_sensor.p_gps->GPS_UTC_SEC);
		i += 1;
	}

	if(AOCS_tlm_Analyze(pos, gps_pos)){
		if(tlm_len < i + 12) return;

		TF_copy_float(&contents[i], (float)g_sensor.p_gps->ECI[0]);
		i += 4;
		TF_copy_float(&contents[i], (float)g_sensor.p_gps->ECI[1]);
		i += 4;
		TF_copy_float(&contents[i], (float)g_sensor.p_gps->ECI[2]);
		i += 4;
	}

	if(AOCS_tlm_Analyze(pos, gps_vel)){
		if(tlm_len < i + 12) return;

		TF_copy_float(&contents[i], (float)g_sensor.p_gps->Vel_ECI[0]);
		i += 4;
		TF_copy_float(&contents[i], (float)g_sensor.p_gps->Vel_ECI[1]);
		i += 4;
		TF_copy_float(&contents[i], (float)g_sensor.p_gps->Vel_ECI[2]);
		i += 4;
	}

	if(AOCS_tlm_Analyze(pos, gps_temp)){
		if(tlm_len < i + 2) return;

		TF_copy_u16(&contents[i], (uint16_t)g_sensor.p_adc->ADC_GPS_TEMP);
		i += 2;
	}

	if(AOCS_tlm_Analyze(pos, plasma)){
		if(tlm_len < i + 12) return;

	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_VOLS_PLASMA1));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_VOLS_PLASMA2));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_VOLS_PLASMA3));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_VOLS_PLASMA4));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_VOLS_PLASMA5));
	  	i += 2;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_VOLS_PLASMA6));
	  	i += 2;
	}

	if(AOCS_tlm_Analyze(pos, panel_temp)){
		if(tlm_len < i + 12) return;

		TF_copy_u16(&contents[i], (uint16_t)(txpic_driver->tlm_data.txpic_to_mobc.temp.strpx));
		i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(txpic_driver->tlm_data.txpic_to_mobc.temp.strmx));
		i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(txpic_driver->tlm_data.txpic_to_mobc.temp.strpy));
		i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(txpic_driver->tlm_data.txpic_to_mobc.temp.strmy));
		i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(txpic_driver->tlm_data.txpic_to_mobc.temp.strpz));
		i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(txpic_driver->tlm_data.txpic_to_mobc.temp.strmz));
		i += 2;
	}

	if(AOCS_tlm_Analyze(pos, vol_sepcoil)){
		if(tlm_len < i + 4) return;
		TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_pwr_adc_pwr2->ADC_V_SEPC1>>4));
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_pwr_adc_pwr2->ADC_V_SEPC2>>4));
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_pwr_adc_pwr2->ADC_V_SEPC3>>4));
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)(g_sensor.p_pwr_adc_pwr2->ADC_V_SEPC4>>4));
		i += 1;
	}

	if(AOCS_tlm_Analyze(pos, adc_curs_mtq)){
		if(tlm_len < i + 12) return;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQX));
	    i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQY));
	    i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQZ1));
	    i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQZ2));
	    i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQZ3));
	    i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQZ4));
	    i += 2;
	}

	if(AOCS_tlm_Analyze(pos, ads_mode)){
		if(tlm_len < i + 4) return;

		TF_copy_u32(&contents[i], (uint32_t)g_acs.p_GLOBAL_AOCS->AOCS_MODE.ADS_MODE_Number);
		i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_Mag)){
		if(tlm_len < i + 12) return;

	    TF_copy_float(&contents[i], (float)(g_ads.p_ADS->MagField[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_ADS->MagField[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_ADS->MagField[2]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_Gyro)){
		if(tlm_len < i + 12) return;

	  	TF_copy_float(&contents[i], (float)(g_ads.p_ADS->Omega[0]));
	  	i += 4;
	  	TF_copy_float(&contents[i], (float)(g_ads.p_ADS->Omega[1]));
	  	i += 4;
	  	TF_copy_float(&contents[i], (float)(g_ads.p_ADS->Omega[2]));
	  	i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_Sunvec)){
		if(tlm_len < i + 12) return;

	    TF_copy_float(&contents[i], (float)(g_ads.p_ADS->SunVec[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_ADS->SunVec[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_ADS->SunVec[2]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_Julian)){
		if(tlm_len < i + 8) return;

	    TF_copy_double(&contents[i], (double)g_ads.p_JULIANDATE->juliandate);
	    i += 8;
	}

	if(AOCS_tlm_Analyze(pos, ads_GST)){
		if(tlm_len < i + 4) return;

		TF_copy_float(&contents[i], (float)g_ads.p_GST->gst);
		i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_IGRF)){
		if(tlm_len < i + 12) return;

	    TF_copy_float(&contents[i], (float)g_ads.p_IGRF->IGRF_B[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ads.p_IGRF->IGRF_B[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ads.p_IGRF->IGRF_B[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_Isunvec)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ads.p_SUNVECTOR->sunvector[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ads.p_SUNVECTOR->sunvector[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ads.p_SUNVECTOR->sunvector[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_Bsunvec)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ads.p_SUNVECTOR->SunVector_Body[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ads.p_SUNVECTOR->SunVector_Body[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ads.p_SUNVECTOR->SunVector_Body[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_Quaternion)){
		if(tlm_len < i + 16) return;

	    TF_copy_float(&contents[i], (float)(g_ads.p_ADS->Quaternion[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_ADS->Quaternion[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_ADS->Quaternion[2]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_ADS->Quaternion[3]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_X_observed_1)){
		if(tlm_len < i + 28) return;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[2]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[3]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[4]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[5]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[6]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_X_observed_2)){
		if(tlm_len < i + 36) return;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[7]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[8]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[9]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[10]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[11]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[12]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[13]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[14]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->Obs[15]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_X_ref_a_priori_1)){
		if(tlm_len < i + 28) return;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[2]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[3]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[4]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[5]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[6]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_X_ref_a_priori_2)){
		if(tlm_len < i + 36) return;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[7]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[8]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[9]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[10]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[11]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[12]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[13]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[14]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_pre[15]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_X_ref_a_poseriori_1)){
		if(tlm_len < i + 28) return;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[2]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[3]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[4]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[5]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[6]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_X_ref_a_poseriori_2)){
		if(tlm_len < i + 36) return;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[7]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[8]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[9]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[10]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[11]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[12]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[13]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[14]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->X_ref_post[15]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_P_a_priori_1)){
		if(tlm_len < i + 24) return;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[2]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[3]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[4]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[5]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_P_a_priori_2)){
		if(tlm_len < i + 36) return;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[6]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[7]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[8]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[9]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[10]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[11]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[12]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[13]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_pre_diag[14]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_P_a_posteriori_1)){
		if(tlm_len < i + 24) return;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[2]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[3]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[4]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[5]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_P_a_posteriori_2)){
		if(tlm_len < i + 36) return;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[6]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[7]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[8]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[9]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[10]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[11]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[12]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[13]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_ads.p_EKF->P_post_diag[14]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_valid)){
		if(tlm_len < i + 3) return;

		TF_copy_u8(&contents[i], (uint8_t)g_ads.p_EKF->ekf_valid);
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)g_ads.p_EKF->dq_valid);
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)g_ads.p_EKF->divg_valid);
		i += 1;
	}

	if(AOCS_tlm_Analyze(pos, ads_EKF_sep_time)){
		if(tlm_len < i + 2) return;

		TF_copy_i16(&contents[i], (int16_t)g_ads.p_EKF->sep_time);
		i += 2;
	}

	if(AOCS_tlm_Analyze(pos, Separate_judge)){
		if(tlm_len < i + 8) return;

	}

	if(AOCS_tlm_Analyze(pos, acs_mode)){
		if(tlm_len < i + 4) return;

		TF_copy_u32(&contents[i], (uint32_t)g_acs.p_GLOBAL_AOCS->AOCS_MODE.ACS_MODE_Number);
		i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_time)){
		if(tlm_len < i + 2) return;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_acs.p_ACS->ACS_time));
	  	i += 2;
	}

	if(AOCS_tlm_Analyze(pos, acs_Mag)){
		if(tlm_len < i + 12) return;

	    TF_copy_float(&contents[i], (float)(g_acs.p_ACS->MagField[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_acs.p_ACS->MagField[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_acs.p_ACS->MagField[2]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_Gyro)){
		if(tlm_len < i + 12) return;

	  	TF_copy_float(&contents[i], (float)(g_acs.p_ACS->Omega[0]));
	  	i += 4;
	  	TF_copy_float(&contents[i], (float)(g_acs.p_ACS->Omega[1]));
	  	i += 4;
	  	TF_copy_float(&contents[i], (float)(g_acs.p_ACS->Omega[2]));
	  	i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_SunVec)){
		if(tlm_len < i + 12) return;

	    TF_copy_float(&contents[i], (float)(g_acs.p_ACS->SunVec[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_acs.p_ACS->SunVec[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_acs.p_ACS->SunVec[2]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_Target_Direction)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)(g_acs.p_ACS->TagetVec[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_acs.p_ACS->TagetVec[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_acs.p_ACS->TagetVec[2]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_Earth_center_vec)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)(g_acs.p_EARTHVECTOR->EarthCenterVector_Body[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_acs.p_EARTHVECTOR->EarthCenterVector_Body[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_acs.p_EARTHVECTOR->EarthCenterVector_Body[2]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_Earth_point_vec)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)(g_acs.p_EARTHVECTOR->EarthPointVector_Body[0]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_acs.p_EARTHVECTOR->EarthPointVector_Body[1]));
	    i += 4;
	    TF_copy_float(&contents[i], (float)(g_acs.p_EARTHVECTOR->EarthPointVector_Body[2]));
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_Inertial_vec)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->InertialTagetVec[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->InertialTagetVec[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->InertialTagetVec[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_Velocity_vec)){
		if(tlm_len < i + 12) return;

	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Velocity_b[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Velocity_b[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Velocity_b[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_COBC_vec)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->COBC_TargetDirection[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->COBC_TargetDirection[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->COBC_TargetDirection[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_spin_dorection)){
		if(tlm_len < i + 12) return;

	}

	if(AOCS_tlm_Analyze(pos, acs_Quaternion)){
		if(tlm_len < i + 16) return;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Quaternion[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Quaternion[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Quaternion[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Quaternion[3]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_MTQ_output)){
		if(tlm_len < i + 6) return;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQX));
	    i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQY));
	    i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQZ1));
	    i += 2;
	}

	if(AOCS_tlm_Analyze(pos, acs_MTQ_Cn_req)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Mag_moment_Cn[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Mag_moment_Cn[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Mag_moment_Cn[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, acs_MTQ_Co_req)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Mag_moment_Co[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Mag_moment_Co[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_acs.p_ACS->Mag_moment_Co[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ods_mode)){
		if(tlm_len < i + 4) return;

		TF_copy_u32(&contents[i], (uint32_t)g_acs.p_GLOBAL_AOCS->AOCS_MODE.ODS_MODE_Number);
		i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ods_Pos)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->Position[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->Position[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->Position[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ods_Vel)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->Velocity[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->Velocity[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->Velocity[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ods_relative_pos)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->RelativePosition[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->RelativePosition[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->RelativePosition[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ods_relative_vel)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->RelativeVelocity[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->RelativeVelocity[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->RelativeVelocity[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ods_BC)){
		if(tlm_len < i + 4) return;
	    TF_copy_float(&contents[i], (float)g_ods.p_ODS->Ballistic_Coefficient);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ods_GPS_Status)){
		if(tlm_len < i + 1) return;
		TF_copy_u8(&contents[i], (uint8_t)g_sensor.p_gps->GPS_Status);
		i += 1;
	}

	if(AOCS_tlm_Analyze(pos, ods_DCM_eci2lvlh)){
		if(tlm_len < i + 36) return;

	}

	if(AOCS_tlm_Analyze(pos, ods_EKF_X_observed)){
		if(tlm_len < i + 24) return;
	}

	if(AOCS_tlm_Analyze(pos, ods_EKF_X_ref_a_priori)){
		if(tlm_len < i + 24) return;
	}

	if(AOCS_tlm_Analyze(pos, ods_EKF_X_ref_a_poseriori)){
		if(tlm_len < i + 24) return;
	}

	if(AOCS_tlm_Analyze(pos, ods_EKF_P_a_priori)){
		if(tlm_len < i + 24) return;
	}

	if(AOCS_tlm_Analyze(pos, ods_EKF_P_a_posteriori)){
		if(tlm_len < i + 24) return;
	}

	if(AOCS_tlm_Analyze(pos, ods_EKF_valid)){
		if(tlm_len < i + 3) return;
		TF_copy_u8(&contents[i], (uint8_t)g_ads.p_EKF->ekf_valid);
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)g_ads.p_EKF->dq_valid);
		i += 1;
		TF_copy_u8(&contents[i], (uint8_t)g_ads.p_EKF->divg_valid);
		i += 1;
	}

	if(AOCS_tlm_Analyze(pos, ods_EKF_sep_time)){
		if(tlm_len < i + 2) return;
		TF_copy_i16(&contents[i], (int16_t)g_ads.p_EKF->sep_time);
		i += 2;
	}

	if(AOCS_tlm_Analyze(pos, ods_RSSI)){
		if(tlm_len < i + 4) return;
	  	TF_copy_u32(&contents[i], (uint32_t)(lora_driver->lora_tlm.reg_info.receive_info));
	  	i += 4;
		TF_copy_u16(&contents[i],(uint16_t)(rxpic_driver->tlm_data.rxpic_to_mobc.rssi));
		i += 2;
	}

	if(AOCS_tlm_Analyze(pos, ocs_mode)){
		if(tlm_len < i + 4) return;
		TF_copy_u32(&contents[i], (uint32_t)g_acs.p_GLOBAL_AOCS->AOCS_MODE.OCS_MODE_Number);
		i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ocs_time)){
		if(tlm_len < i + 2) return;
	  	TF_copy_u16(&contents[i], (uint16_t)(g_ocs.p_OCS->OCS_time));
	  	i += 2;
	}

	if(AOCS_tlm_Analyze(pos, ocs_Pos)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Position[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Position[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Position[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ocs_Vel)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Velocity[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Velocity[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Velocity[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ocs_relative_pos)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->RelativePosition[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->RelativePosition[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->RelativePosition[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ocs_relative_vel)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->RelativeVelocity[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->RelativeVelocity[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->RelativeVelocity[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ocs_BC)){
		if(tlm_len < i + 16) return;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Ballistic_Coefficient);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Ballistic_Coefficient_order);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Ballistic_Coefficient_opponent);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Ballistic_Coefficient_opponent_order);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ocs_BC_req)){
		if(tlm_len < i + 8) return;

	}

	if(AOCS_tlm_Analyze(pos, ocs_Mag)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->MagField[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->MagField[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->MagField[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ocs_Gyro)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Omega[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Omega[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Omega[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ocs_Quaternion)){
		if(tlm_len < i + 16) return;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Quaternion[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Quaternion[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Quaternion[2]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Quaternion[3]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ocs_MTQ_output)){
		if(tlm_len < i + 6) return;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQX));
	    i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQY));
	    i += 2;
	    TF_copy_u16(&contents[i], (uint16_t)(g_sensor.p_adc->ADC_CURS_MTQZ1));
	    i += 2;
	}

	if(AOCS_tlm_Analyze(pos, ocs_MTQ_Cn_req)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Mag_moment_Cn[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Mag_moment_Cn[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Mag_moment_Cn[2]);
	    i += 4;
	}

	if(AOCS_tlm_Analyze(pos, ocs_MTQ_Co_req)){
		if(tlm_len < i + 12) return;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Mag_moment_Co[0]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Mag_moment_Co[1]);
	    i += 4;
	    TF_copy_float(&contents[i], (float)g_ocs.p_OCS->Mag_moment_Co[2]);
	    i += 4;
	}

}



