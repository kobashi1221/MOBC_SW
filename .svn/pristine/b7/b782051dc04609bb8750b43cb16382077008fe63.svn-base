/**
  *****************************************************************************************
  * @file    : ACS1_debug_ADC.h
  * @author  : Satoshi NAKAYAMA
  * @version : 1.0
  * @date    : 2020/10/15
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
  * 2020/10/15  1.0   S.NAKAYAMA      First revision
  *
  *****************************************************************************************
  */

#ifndef ACS1_DEBUG_ADC_H_
#define ACS1_DEBUG_ADC_H_

#include <stdint.h> //for uint8_t

//#include "main.h"
#include <src_user/IfWrapper/HAL.h>
#include <src_core/System/ApplicationManager/app_info.h> //necessary to add app
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>  //necessary to add cmd
#include <src_core/System/ApplicationManager/app_info.h>

typedef struct
{
	//GYRO Data
	float OMEGAX;
	float OMEGAY;
	float OMEGAZ;
	uint16_t ADC_OMEGAX;
	uint16_t ADC_OMEGAY;
	uint16_t ADC_OMEGAZ;
	uint8_t state[3];

	//MAGS Data
	float MAGSX;
	float MAGSY;
	float MAGSZ;
	int ADC_MAGSX;
	int ADC_MAGSY;
	int ADC_MAGSZ;

	float MAGSPX;
	float MAGSMX;
	float MAGSPY;
	float MAGSMY;
	float MAGSPZ;
	float MAGSMZ;
	uint16_t ADC_MAGSPX;
	uint16_t ADC_MAGSMX;
	uint16_t ADC_MAGSPY;
	uint16_t ADC_MAGSMY;
	uint16_t ADC_MAGSPZ;
	uint16_t ADC_MAGSMZ;


	float MAGVec[3];


	//CURS Data
	float CURS_MTQX;
	float CURS_MTQY;
	float CURS_MTQZ1;
	float CURS_MTQZ2;
	float CURS_MTQZ3;
	float CURS_MTQZ4;
	float CURS_MTQTWO_U;
	float CURS_TX;
	uint16_t ADC_CURS_MTQX;
	uint16_t ADC_CURS_MTQY;
	uint16_t ADC_CURS_MTQZ1;
	uint16_t ADC_CURS_MTQZ2;
	uint16_t ADC_CURS_MTQZ3;
	uint16_t ADC_CURS_MTQZ4;
	uint16_t ADC_CURS_MTQ2U;
	uint16_t ADC_CURS_TX;

	//VOLS Data
	float VOLS_MTQA;
	float VOLS_MTQO;
	uint16_t ADC_VOLS_MTQA;
	uint16_t ADC_VOLS_MTQO;

	//PLASMA Data
	float VOLS_PLASMA1;        //プラズマPX計測電圧アナログ値 [V]
	float VOLS_PLASMA2;        //プラズマMX計測電圧アナログ値 [V]
	float VOLS_PLASMA3;        //プラズマPY計測電圧アナログ値 [V]
	float VOLS_PLASMA4;        //プラズマMY計測電圧アナログ値 [V]
	float VOLS_PLASMA5;        //プラズマPZ計測電圧アナログ値 [V]
	float VOLS_PLASMA6;        //プラズマMZ計測電圧アナログ値 [V]
	uint16_t ADC_VOLS_PLASMA1; //プラズマPX計測電圧デジタル値 [-]
	uint16_t ADC_VOLS_PLASMA2; //プラズマMX計測電圧デジタル値 [-]
	uint16_t ADC_VOLS_PLASMA3; //プラズマPY計測電圧デジタル値 [-]
	uint16_t ADC_VOLS_PLASMA4; //プラズマMY計測電圧デジタル値 [-]
	uint16_t ADC_VOLS_PLASMA5; //プラズマPZ計測電圧デジタル値 [-]
	uint16_t ADC_VOLS_PLASMA6; //プラズマMZ計測電圧デジタル値 [-]

	//Platinum Sensor Data
	float MOBC_TEMP;
	float ADC_TEMP;
	float MAGS_TEMP;
	float GPS_TEMP;
	float NINEAX_TEMP;
	float LENSPX_TEMP;
	float LENSPY_TEMP;
	float IMAGE_TEMP;
	float LENSMX_TEMP;
	float GYROX_TEMP;
	float GYROY_TEMP;
	float GYROZ_TEMP;
	float MTQZ1_TEMP;
	float MTQZ2_TEMP;
	float MTQZ3_TEMP;
	float MTQZ4_TEMP;



	uint16_t ADC_MOBC_TEMP;
	uint16_t ADC_ADC_TEMP;
	uint16_t ADC_MAGS_TEMP;
	uint16_t ADC_GPS_TEMP;
	uint16_t ADC_NINEAX_TEMP;
	uint16_t ADC_LENSPX_TEMP;
	uint16_t ADC_LENSPY_TEMP;
	uint16_t ADC_IMAGE_TEMP;
	uint16_t ADC_LENSMX_TEMP;
	uint16_t ADC_GYROX_TEMP;
	uint16_t ADC_GYROY_TEMP;
	uint16_t ADC_GYROZ_TEMP;
	uint16_t ADC_MTQZ1_TEMP;
	uint16_t ADC_MTQZ2_TEMP;
	uint16_t ADC_MTQZ3_TEMP;
	uint16_t ADC_MTQZ4_TEMP;

	uint16_t ADC_intData;
	float ADC_Data;//ドライバ内ADC_GPIO_All_Read関数で計算結果として出てきた値を構造体のメンバーとして置いておくことでドライバーインスタンス内でこの値を使用できる


} ADC_STRUCT;


#ifdef __cplusplus
 extern "C" {
#endif

AppInfo ACS1_debug_adc_update(void);
AppInfo ACS1_param_update(void);

CCP_CmdRet Cmd_ACS1_debug_GYRO(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS1_Change_GYRO_Range(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS1_debug_PT_TEMS(const CommonCmdPacket* packet);     //masuda add
CCP_CmdRet Cmd_ACS1_debug_MAGS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS1_debug_CURS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS1_debug_VOLS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ACS1_debug_PLASMA(const CommonCmdPacket* packet);


#ifdef __cplusplus
}
#endif

#endif /* ACS1_DEBUG_ADC_H_ */
