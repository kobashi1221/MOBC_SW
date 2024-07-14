#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <string.h>//for memcpy

#include <src_user/Applications/middleware/MemoryMW.h>
#include <src_user/Applications/UserDefined/Com/COM_cmd_Relay.h>
uint8_t* message_data_from_sat = (uint8_t*)0x6007F800;
uint8_t* message_data_from_GS = (uint8_t*)0x60082A00;

const uint16_t txpic_message_cmd = 0xC116;

static void CDH1_relay_load_param(void);
static void CDH1_relay_initialize_param(void);

static uint8_t route_change[2];
const uint8_t route_change_[2] =
							{MESSAGE_FLAG_OFF,//interval_to_sat
							 MESSAGE_FLAG_OFF //interval_to_GS
};

static uint8_t packet_route[2];
const uint8_t packet_route_[2] =
						{0b00000001,//AFSK to sat
						 0b00000001 //AFSK to GS
};

static uint32_t interval_OBC_time[2];
const uint32_t interval_OBC_time_[2] =
									{0x00000010,//interval_to_sat
									 0x00000010 //interval_to_GS
};

static uint8_t Auto_deliverly[2];
const uint8_t Auto_deliverly_[2] =
								{MESSAGE_FLAG_ON,//to_sat
								 MESSAGE_FLAG_OFF//to_GS
};

static uint8_t message_size;
const uint8_t message_size_ = 128;

static MESSAGE_STRUCT message_struct;
const MESSAGE_STRUCT* p_message_struct = &message_struct;

AppInfo COM_message_param_update(void){
	return AI_create_app_info("SPIPROM_PARAM_UPDATE", CDH1_relay_load_param, CDH1_relay_initialize_param);
}

static void CDH1_relay_load_param(void)
{
	read_param(&route_change, &route_change_, 2, (uint32_t)0x00003000);
	read_param(&packet_route, &packet_route_, 2, (uint32_t)0x00003002);
	read_param(&interval_OBC_time, &interval_OBC_time_, 8, (uint32_t)0x00003004);
	read_param(&Auto_deliverly, &Auto_deliverly_, 2, (uint32_t)0x0000300C);
	read_param(&message_size, &message_size_, 1, (uint32_t)0x0000300E);

//	for(int i=0; i<100 ;i++){
//		message_struct.from_gs_info[i].length = 0x00;
//		message_struct.to_gs_info[i].length = 0x00;
//	}

}

static void CDH1_relay_initialize_param(void)
{
	write_param(&route_change, &route_change_, 2, (uint32_t)0x00003000);
	write_param(&packet_route, &packet_route_, 2, (uint32_t)0x00003002);
	write_param(&interval_OBC_time, &interval_OBC_time_, 8, (uint32_t)0x00003004);
	write_param(&Auto_deliverly, &Auto_deliverly_, 2, (uint32_t)0x0000300C);
	write_param(&message_size, &message_size_, 1, (uint32_t)0x0000300E);
}

CCP_CmdRet COM_message_from_GS(const CommonCmdPacket* packet){
	printf("FROM_GS \r\n");
	const uint8_t* param = CCP_get_param_head(packet);
	int massage_len =TCP_get_packet_len(packet) - TCP_CMD_USER_HDR_LEN - TCP_CMD_2ND_HDR_LEN;
	TCP_ROUTE route;
	uint8_t message_num;
	uint8_t stored_data[MESSAGE_MAX_SIZE];
	TCP pckt;
	step_t now = (TMGR_get_master_total_cycle());
	step_t message_time = now << 8;
	ENDIAN_memcpy(&route, &param[0], 1);
	ENDIAN_memcpy(&message_num, &param[1], 1);

	printf("massage_len is %d \r\n", massage_len);
	message_struct.from_gs_info[message_num].length = massage_len + ADDIITIONAL_DATA;//satID + time

//message packet[0] = route
//message packet[1] =  message_num
//message packet[2]~[7] = sat_ID
//message packet[8] ~ [9] = time
//message packet[10] ~ =  message
	ENDIAN_memcpy(&stored_data[0], &route, 1);
	ENDIAN_memcpy(&stored_data[1], &message_num, 1);

//set satID
#ifdef ONE_U
	stored_data[2] = 0x50;//P
	stored_data[3] = 0x69;//i
	stored_data[4] = 0x73;//s
	stored_data[5] = 0x63;//c
	stored_data[6] = 0x69;//i
	stored_data[7] = 0x73;//s
#endif
#ifdef TWO_U
	stored_data[2] = 0x54;//T
	stored_data[3] = 0x69;//i
	stored_data[4] = 0x67;//g
	stored_data[5] = 0x72;//r
	stored_data[6] = 0x69;//i
	stored_data[7] = 0x73;//s
#endif

	memcpy(&stored_data[8], &message_time, 2);
	memcpy(&stored_data[10], &param[2], massage_len - MESSAGE_HEADER);//ヘッダを引いた
	memcpy(message_data_from_GS + (message_num * message_size), &stored_data[0], message_struct.from_gs_info[message_num].length);//保存

	if(Auto_deliverly[0] == MESSAGE_FLAG_ON){//自動配送ならTLCで相手衛星に送信
		CCP_form_tlc(&pckt, now + interval_OBC_time[0], Cmd_CODE_COM_MESSAGE_TO_SAT, &stored_data[0], 2);
		PH_analyze_packet(&pckt);
	}

	printf("packet is \r\n");
	for(int i = 0; i < massage_len + TCP_PRM_HDR_LEN; i ++){
		printf("%02x", packet->packet[i]);
	}
	printf("\r\n");

	printf("message_num is %d \r\n", message_num);
	printf("length is %d \r\n", message_struct.from_gs_info[message_num].length);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

/**
 * @brief   外部RAMヵら取り出して送る
 * param[15~18]  //何番目
 */
CCP_CmdRet COM_message_to_SAT(const CommonCmdPacket* packet){
	printf("TO_SAT \r\n");
	const uint8_t* param = CCP_get_param_head(packet);
	TCP_ROUTE route;
	TCP pckt;
	uint8_t message_num;
	uint8_t message[MESSAGE_MAX_SIZE];
	memcpy(&route, &param[0], 1);
	memcpy(&message_num, &param[1], 1);
	memcpy(&message[0], message_data_from_GS + (message_num * message_size), message_struct.from_gs_info[message_num].length);

	printf("message is \r\n");
	for(int i = 0; i < message_struct.from_gs_info[message_num].length; i ++){
		printf("%x \r\n", message[i]);
	}
	printf("\r\n");

	CCP_form_rtc(&pckt, Cmd_CODE_COM_MESSAGE_FROM_SAT, &message[0], message_struct.from_gs_info[message_num].length);

	if(route_change[0] == MESSAGE_FLAG_ON){//flagが立っている時だけルートを再設定
		TCP_set_route(&pckt, packet_route[0]);
	}
	else{
		switch(route){//ルートとsatidだけ再設定
			case TCP_ROUTE_DIRECT:
				TCP_set_route(&pckt, TCP_ROUTE_DIRECT);
				break;
			case TCP_ROUTE_AFSK:
				TCP_set_route(&pckt, TCP_ROUTE_AFSK);
				break;
			case TCP_ROUTE_MILLMAX:
				TCP_set_route(&pckt, TCP_ROUTE_MILLMAX);
				break;
			case TCP_ROUTE_LORA:
				TCP_set_route(&pckt, TCP_ROUTE_LORA);
				break;
			default:
				//何もしない
				break;
		}
	}
//パケット配送したいので, 実行先は相手方のMOBC
#ifdef TWO_U
	TCP_set_apid_(&pckt, TCP_SAT_ID_1U, TCP_EXE_ID_MOBC);
#endif
#ifdef ONE_U
	TCP_set_apid_(&pckt, TCP_SAT_ID_2U, TCP_EXE_ID_MOBC);
#endif
	PH_analyze_packet(&pckt);

	printf("pckt is \r\n");
	for(int i = 0; i < message_struct.from_gs_info[message_num].length + TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN; i ++){
		printf("%02x", pckt.packet[i]);
	}
	printf("\r\n");

	printf("message_num is %d \r\n", message_num);
	printf("length is %d \r\n", message_struct.from_gs_info[message_num].length);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet COM_message_from_SAT(const CommonCmdPacket* packet){
	printf("FROM_SAT \r\n");
	const uint8_t* param = CCP_get_param_head(packet);
	int massage_len = TCP_get_packet_len(packet) - TCP_CMD_USER_HDR_LEN - TCP_CMD_2ND_HDR_LEN;
	TCP_ROUTE route;
	uint8_t message_num;
	uint8_t stored_data[MESSAGE_MAX_SIZE];
	step_t now = (TMGR_get_master_total_cycle());
	step_t message_time = now << 8;
	TCP pckt;
	memcpy(&route, &param[0], 1);
	memcpy(&message_num, &param[1], 1);
	message_struct.to_gs_info[message_num].length = massage_len + ADDIITIONAL_DATA;//satID + time

//message packet[0]= route
//message packet[1]=  message_num
//message packet[2]~[7] = another_sat_ID
//message packet[8] ~ [9] = another_sat_time
//message pacekt[10] ~ [15] = sat_ID
//message packet[16] ~ [17] = sat_time
//message packet[18] ~  message
	memcpy(&stored_data[0], &route, 1);
	memcpy(&stored_data[1], &message_num, 1);
	memcpy(&stored_data[2], &param[2], ADDIITIONAL_DATA);
#ifdef ONE_U
	stored_data[10] = 0x50;
	stored_data[11] = 0x69;
	stored_data[12] = 0x73;
	stored_data[13] = 0x63;
	stored_data[14] = 0x69;
	stored_data[15] = 0x73;
#endif
#ifdef TWO_U
	stored_data[10] = 0x54;
	stored_data[11] = 0x69;
	stored_data[12] = 0x67;
	stored_data[13] = 0x72;
	stored_data[14] = 0x69;
	stored_data[15] = 0x73;
#endif
	memcpy(&stored_data[16], &message_time, 2);
	memcpy(&stored_data[18], &param[MESSAGE_HEADER + ADDIITIONAL_DATA], massage_len - (MESSAGE_HEADER + ADDIITIONAL_DATA));//諸々を引いた
	memcpy(message_data_from_sat + (message_num * message_size), &stored_data[0], message_struct.to_gs_info[message_num].length);//保存

	if(Auto_deliverly[1] == MESSAGE_FLAG_ON){
		CCP_form_tlc(&pckt, now + interval_OBC_time[1], Cmd_CODE_COM_MESSAGE_TO_GS, &stored_data[0], message_struct.to_gs_info[message_num].length);
		PH_analyze_packet(&pckt);
	}
	printf("message_num is %d \r\n", message_num);
	printf("length is %d \r\n", message_struct.to_gs_info[message_num].length);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//外部RAMヵら取り出して送る
CCP_CmdRet COM_message_to_GS(const CommonCmdPacket* packet){
	printf("TO_GS \r\n");
	const uint8_t* param = CCP_get_param_head(packet);
	TCP_ROUTE route;
	TCP pckt;
	uint8_t message_num;
	uint8_t message[MESSAGE_MAX_SIZE];
	memcpy(&route, &param[0], 1);
	memcpy(&message_num, &param[1], 1);
	memcpy(&message[0], message_data_from_sat + (message_num * message_size), message_struct.to_gs_info[message_num].length);
#ifdef TWO_U
	CCP_form_rtc_to_other_obc(&pckt, TCP_APID_TXPIC_2U, (CMD_CODE)txpic_message_cmd,&message[0], message_struct.to_gs_info[message_num].length);
#endif
#ifdef ONE_U
	CCP_form_rtc_to_other_obc(&pckt, TCP_APID_TXPIC_1U, (CMD_CODE)txpic_message_cmd,&message[0], message_struct.to_gs_info[message_num].length);
#endif

	if(route_change[1] == 1){//flagが立っている時だけルートを再設定
		TCP_set_route(&pckt, packet_route[1]);
	}
	else{
		switch(route){//ルートとsatidだけ再設定
			case TCP_ROUTE_DIRECT:
				TCP_set_route(&pckt, TCP_ROUTE_DIRECT);
				break;
			case TCP_ROUTE_AFSK:
				TCP_set_route(&pckt, TCP_ROUTE_AFSK);
				break;
			case TCP_ROUTE_MILLMAX:
				TCP_set_route(&pckt, TCP_ROUTE_MILLMAX);
				break;
			case TCP_ROUTE_LORA:
				TCP_set_route(&pckt, TCP_ROUTE_LORA);
				break;
			default:
				//何もしない
				break;
		}
	}
	PH_analyze_packet(&pckt);

	printf("message is \r\n");
	for(int i = 0; i < message_struct.to_gs_info[message_num].length + TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN; i ++){
		printf("%02x", pckt.packet[i]);
	}
	printf("\r\n");
	printf("message_num is %d \r\n", message_num);
	printf("length is %d \r\n", message_struct.to_gs_info[message_num].length);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


