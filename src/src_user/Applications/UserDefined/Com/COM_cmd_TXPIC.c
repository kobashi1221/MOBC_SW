/**
  *****************************************************************************************
  * @file    : COM1_debug_TXPIC.c
  * @author  : Takumi NORO
  * @version : 1.0
  * @date    : 2020/08/20
  * @brief   :　TXPICとのUART通信用
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/06/23  1.0   T.NORO         First revision
  *
  *****************************************************************************************
  */

#include "COM_cmd_TXPIC.h"

#include <string.h> //memcpy用
#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <src_user/Applications/UserDefined/Sensor/ACS3_cmd_SUBCAM.h>
#include <src_user/Drivers/large_packet_driver.h>
#include <src_user/Applications/UserDefined/Com/COM_cmd_COBC.h>

static TCP tcp;

void COM_txpic_clear_count_(void);

uint8_t* SRAM_address = (uint8_t*)0x60000000;

AppInfo COM_txpic_clear_count(void)
{
  return AI_create_app_info("TXPIC_CLEAR_COUNT", NULL, COM_txpic_clear_count_);
}

void COM_txpic_clear_count_(void){
	if((TMGR_get_master_total_cycle()%3)==0){
		uint8_t length = TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN;
#ifdef TWO_U
		CCP_form_rtc_to_other_obc(&tcp, TCP_APID_TXPIC_2U, (CMD_CODE)0xc140,NULL,0);
#endif
#ifdef ONE_U
		CCP_form_rtc_to_other_obc(&tcp, TCP_APID_TXPIC_1U, (CMD_CODE)0xc140,NULL,0);
#endif
		TCP_set_route(&tcp, TCP_ROUTE_DIRECT);
		//生存人号をanalyzeにすると処理できるC2A一周で処理できるpacket(max16)に制限が出るため直接送る
		TXPIC_Send(txpic_driver,(uint8_t *)tcp.packet,length);
	}
}

CCP_CmdRet Cmd_COM_scam_downlink(const CommonCmdPacket* packet){
#ifdef ONE_U
	const uint8_t* param = CCP_get_param_head(packet);
	TCP tcp;

	//ダウンリンク開始
	uint8_t dummy[2];
	dummy[0] = param[0];
	dummy[1] = param[1];//ダウンリンク中のMODE移動先
	const uint8_t* data = &dummy[0];
	CCP_form_rtc(&tcp,Cmd_CODE_SAME_TL_MODE_TRANS, &data[0], 2);
	PH_analyze_packet(&tcp);
	memcpy(&txpic_driver->picture_id, &SCAM_driver->large_packet.picture_id, 2);

	large_packet_TLM_analyze(&SRAM_address[0], SCAM_driver->large_packet.ImSize, Tlm_CODE_SUBCAM);

	SCAM_driver->large_packet.move_data_flag = NOT_OPERATED;
	SCAM_default_setting();
#endif
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//引数に取りこぼしたシーケンスフラグをが入る
CCP_CmdRet Cmd_COM_picture_data_resend(const CommonCmdPacket* packet){
	const uint8_t* param = CCP_get_param_head(packet);
	uint8_t temp[2];
	size_t len = CCP_get_param_len(packet);
	size_t seq_packet_num = 0x0001;//少なくとも一つはあるはず
	TCP tcp;
	if(large_driver->analyze.function_flag == LARGE_PACKET_FLAG_OFF
			&& large_driver->analyze.large_packet_flag== LARGE_PACKET_FLAG_OFF){
		large_packet_set(&large_driver->analyze.re_send_flag);
		memcpy(&temp[0], &param[0], 2);
		ENDIAN_memcpy(&large_driver->analyze.seq_count, &temp[0], 2);
		printf("seq_count is %d \r\n", large_driver->analyze.seq_count);
		for(int i = 0; i < len / 2; i++){//2byteなので
			uint16_t now_seq_count, next_seq_count;
			memcpy(&temp[0], &param[i * 2], 2);
			ENDIAN_memcpy(&now_seq_count, &temp[0], 2);
			memcpy(&temp[0], &param[i * 2 + 2], 2);
			ENDIAN_memcpy(&next_seq_count, &temp[0], 2);
			//連続データなら
			if(next_seq_count == now_seq_count + 1){
				++seq_packet_num;
				printf("seq_count is %d \r\n", next_seq_count);
			}
			else{
				break;
			}
		}
		size_t next_len = len - (seq_packet_num * 2);
		uint32_t analyze_per_size = large_driver->analyze.per_packet_size - (TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN + ID_SIZE);
#ifdef TWO_U
		if(cobc_driver->large_packet.info == MCAM_INFO){
			large_packet_TLM_analyze(cobc_driver->large_packet.sram_bank_addr +
					(large_driver->analyze.per_packet_size - large_driver->analyze.header_length - ID_SIZE) * large_driver->analyze.seq_count ,
					analyze_per_size * seq_packet_num, COBC_MCAM_TLM);
		}
		else if(cobc_driver->large_packet.info == SCAM_INFO){
			large_packet_TLM_analyze(cobc_driver->large_packet.sram_bank_addr +
					(large_driver->analyze.per_packet_size - large_driver->analyze.header_length - ID_SIZE) * large_driver->analyze.seq_count ,
					analyze_per_size * seq_packet_num, COBC_SCAM_TLM);
		}
#endif
#ifdef ONE_U
		large_packet_TLM_analyze(&SRAM_address[0] +
				(large_driver->analyze.per_packet_size - large_driver->analyze.header_length - ID_SIZE) * large_driver->analyze.seq_count ,
				analyze_per_size * seq_packet_num, Tlm_CODE_SUBCAM);

#endif
		if(next_len > 0){
			CCP_form_rtc(&tcp, (CMD_CODE)Cmd_CODE_COM_LARGE_PACKET_RESEND, &param[seq_packet_num * 2],next_len);
			PH_analyze_packet(&tcp);
		}
		else{
			large_driver->analyze.re_send_flag = 2;//他にフラグを作るのももったいない
		}
	}
	else{
		CCP_form_rtc(&tcp, (CMD_CODE)Cmd_CODE_COM_LARGE_PACKET_RESEND, &param[0], len);
		PH_analyze_packet(&tcp);
	}
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/************************************** END OF FILE **************************************/
