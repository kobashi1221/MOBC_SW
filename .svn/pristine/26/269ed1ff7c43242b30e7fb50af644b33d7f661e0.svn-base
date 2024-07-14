/*
 * ACS23_cmd_PLASMA.c
 *
 *  Created on: 2023/06/14
 *      Author: daiki
 */

#include "ACS23_cmd_PLASMA.h"
#include "ACS23_cmd_DAC.h"
#include <src_user/Library/General/DBG_printf.h>
#include "src_core/Library/endian.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>

static PLASMA_STRUCT plasma_struct_;
const PLASMA_STRUCT* plasma_struct;


AppInfo ACS23_PLASMA_update(void)
{
	//create_app_info()を呼び出す．
	return AI_create_app_info("DAC", ACS23_PLASMA_init, NULL);
}

void ACS23_PLASMA_init(void){
	uint8_t ret = 0;
	plasma_struct = &plasma_struct_;

#ifdef TWO_U
	ret = dac_change_voltage(dac_struct, 4, 8192);
	ret = dac_change_voltage(dac_struct, 5, 8192);
	ret = dac_change_voltage(dac_struct, 6, 8192);
	ret = dac_change_voltage(dac_struct, 7, 8192);
	ret = dac_change_voltage(dac_struct, 8, 8192);
#endif
#ifdef ONE_U
    ret = dac_change_voltage(dac_struct, 3, 8192);
    ret = dac_change_voltage(dac_struct, 4, 8192);
    ret = dac_change_voltage(dac_struct, 5, 8192);
    ret = dac_change_voltage(dac_struct, 6, 8192);
#endif
	if(ret != 0)print(_PLASMA, "err\r\n");
}

/**
 * @brief FET_MTQO_PLASMAをOFFにするときPLASMAの出力を0Vにする
 * @brief ゼロ点出力(2.5V)だとP5V-MTQO/P5V-PLASMAがOFF時に0Vに落ちないため
 * @param  void
 * @return void
 */
void ACS23_PLASMA_off(void){
    uint8_t ret = 0;
    plasma_struct = &plasma_struct_;

#ifdef TWO_U
    ret = dac_change_voltage(dac_struct, 4, 0);
    ret = dac_change_voltage(dac_struct, 5, 0);
    ret = dac_change_voltage(dac_struct, 6, 0);
    ret = dac_change_voltage(dac_struct, 7, 0);
    ret = dac_change_voltage(dac_struct, 8, 0);
#endif
#ifdef ONE_U
    ret = dac_change_voltage(dac_struct, 3, 0);
    ret = dac_change_voltage(dac_struct, 4, 0);
    ret = dac_change_voltage(dac_struct, 5, 0);
    ret = dac_change_voltage(dac_struct, 6, 0);
#endif
    if(ret != 0)print(_PLASMA, "err\r\n");
}

CCP_CmdRet Cmd_PLASMA_change_voltage(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);
	uint16_t value[5] = {0};
	uint8_t ret = 0;

	ENDIAN_memcpy(&value[0], param,     2);
	ENDIAN_memcpy(&value[1], param + 2, 2);
	ENDIAN_memcpy(&value[2], param + 4, 2);
	ENDIAN_memcpy(&value[3], param + 6, 2);
	ENDIAN_memcpy(&value[4], param + 8, 2);

	for(int i = 0; i < 5; i++){
		if(value[i] >= 16384){
			return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
		}
	}

#ifdef TWO_U
	ret = dac_change_voltage(dac_struct, 4, value[0]);
	ret = dac_change_voltage(dac_struct, 5, value[1]);
	ret = dac_change_voltage(dac_struct, 6, value[2]);
	ret = dac_change_voltage(dac_struct, 7, value[3]);
	ret = dac_change_voltage(dac_struct, 8, value[4]);
#endif
#ifdef ONE_U
    ret = dac_change_voltage(dac_struct, 3, value[0]);
    ret = dac_change_voltage(dac_struct, 4, value[1]);
    ret = dac_change_voltage(dac_struct, 5, value[2]);
    ret = dac_change_voltage(dac_struct, 6, value[3]);
#endif
	if(ret != 0)print(_PLASMA, "err\r\n");

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}



