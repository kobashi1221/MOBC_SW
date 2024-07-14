#pragma section REPRO
#include "tlm_packet_handler.h"

#include "../../Settings/port_config.h"
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_user/Library/General/DBG_printf.h>
#include <string.h>
#include <src_core/TlmCmd/packet_list_util.h>


#define TLM_HANDLE_NUM_MAX 8 // アプリ実行1回あたりに処理してよい最大のVCDU数．明星OBCのFIFOサイズに合わせている

static TPH tph_;
const TPH* tph=&tph_;

static void TLM_mst_packet_handler_init_(void);
static void TLM_mst_packet_handler_(void);
static void TLM_rpt_packet_handler_init_(void);
static void TLM_rpt_packet_handler_(void);
static void TLM_txpic_packet_handler_init_(void);
static void TLM_txpic_packet_handler_(void);
static void TLM_txpic_largepacket_handler_init_(void);
static void TLM_txpic_largepacket_handler_(void);


AppInfo TLM_mst_packet_handler(void)
{
  return AI_create_app_info("TLM_MST",
                         TLM_mst_packet_handler_init_,
                         TLM_mst_packet_handler_);
}

static void TLM_mst_packet_handler_init_(void)
{
//  tph_.ms_t2m = T2M_initialize(&PH_ms_tlm_list,
//                               &gph_.ms_vcdu.m_pdu);
}

static void TLM_mst_packet_handler_(void)
{
  int i;

  for (i = 0; i < TLM_HANDLE_NUM_MAX; i++) // while (ack == T2M_SUCCESS) とかにしようと思ったけど無限ループ怖い
  {
	  if(PL_is_empty(&PH_rt_tlm_list)){
		  return;
	  }
	// 送信するパケットを取得
	TCP* packet = (TCP*)PL_get_head(&PH_rt_tlm_list)->packet;
    // 送信したパケットを消去
    // 以後エラーが出ても，そのパケットは再送しないので，取り出したここで消してしまう．
    PL_drop_executed(&PH_rt_tlm_list);

//    if (TCP_get_apid(&packet) != TCP_APID_PROCYON_MIS_TLM)
//    {
//      // FIXME: アノマリいれる？ 最後のエラーは以下で保存されるので不要な気もする．
//      //        AOBCのアノマリ基準は？
//
//      // 最後に起きたエラーを保存する
//      mobc_driver_.info.c2a.send_tlm_err_code = DRIVER_CMD_ILLEGAL_PARAMETER;
//      continue;
//    }
    int len =TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN;
    int ret;
    TCP_SAT_ID sat_id=TCP_get_apid_sat_id(packet);
    if((sat_id&0x01)==MY_SAT_ID){//自機および自機からGS宛て
    	switch(TCP_get_apid_executer_id(packet)){
    		case TCP_EXE_ID_RXPIC:
    			ret = RXPIC_Send(rxpic_driver,packet->packet,len);
    			break;
    		case TCP_EXE_ID_GMSK19k2:
    		case TCP_EXE_ID_GMSK9k6:
    		case TCP_EXE_ID_AFSK:
    			//TXPICへは一周期に1packetの送信
//    			if((TMGR_get_master_total_cycle()%4)==0){
//    			for(int i=0;i<len;i++) printf("%02x ",packet->packet[i]);
//    			printf("\r\n");
    			ret = PH_add_txpic_tlm_(packet);
//    			ret = TXPIC_Send(txpic_driver,packet->packet,len);
//    			}
    			break;
    		case TCP_EXE_ID_TXPIC:
    			ret = PH_add_txpic_tlm_(packet);
//    			ret = TXPIC_Send(txpic_driver,packet->packet,len);
    			break;
#ifdef TWO_U
    		case TCP_EXE_ID_COBC:
    			ret = COBC_Send(cobc_driver,packet->packet,len);
    			break;
#endif
            case TCP_EXE_ID_SOBC:
                ret = SOBC_SendReceive(sobc_driver,sobc_driver->spi_send_data, len);
                break;
    		default://何処にも配送せず捨てる
    			break;
    	}
    }

    else{//自機宛でない (他衛星宛てのコマンド)
    	switch(TCP_get_route(packet)){
  	  		case TCP_ROUTE_AFSK:
    			ret = PH_add_txpic_tlm_(packet);
//    			ret = TXPIC_Send(txpic_driver,packet->packet,len);
    			break;
  	  		case TCP_ROUTE_MILLMAX:
//    			if((TMGR_get_master_total_cycle()%4)==0){
//    			for(int i=0;i<len;i++) printf("%02x ",packet->packet[i]);
//    			printf("\r\n");
    			ret = INTERSAT_Send(intersat_driver,packet->packet,len);
//    			}
    			break;
  	  	  case TCP_ROUTE_LORA:
  	  		    print(LORA,"lora_route\r\n");
  	  		    ret = LORA_Send(lora_driver,packet->packet,len);
				break;
  	  	  default ://何処にも配送せず捨てる
  	  		    break;
  	  }
    }
  }
}

AppInfo TLM_rpt_packet_handler(void)
{
  return AI_create_app_info("TLM_RPT",
                         TLM_rpt_packet_handler_init_,
                         TLM_rpt_packet_handler_);
}

static void TLM_rpt_packet_handler_init_(void)
{
//  gph_.rp_vcdu_counter = 0;
//  // TCPacket -> M_PDU生成処理のパラメータ初期化
//  gph_.rp_t2m = T2M_initialize(&PH_rp_tlm_list,
//                               &gph_.rp_vcdu.m_pdu);
}

static void TLM_rpt_packet_handler_(void)
{
	int i;
	for(i = 0; i < TLM_HANDLE_NUM_MAX; i++)// while (ack == T2M_SUCCESS) とかにしようと思ったけど無限ループ怖い
	{
		if(PL_is_empty(&PH_rp_tlm_list)){
			return;
		}
		// 送信するパケットを取得
		TCP* packet = (TCP*)PL_get_head(&PH_rp_tlm_list)->packet;
	    // 送信したパケットを消去
	    // 以後エラーが出ても，そのパケットは再送しないので，取り出したここで消してしまう．
	    PL_drop_executed(&PH_rp_tlm_list);

	//    if (TCP_get_apid(&packet) != TCP_APID_PROCYON_MIS_TLM)
	//    {
	//      // FIXME: アノマリいれる？ 最後のエラーは以下で保存されるので不要な気もする．
	//      //        AOBCのアノマリ基準は？
	//
	//      // 最後に起きたエラーを保存する
	//      mobc_driver_.info.c2a.send_tlm_err_code = DRIVER_CMD_ILLEGAL_PARAMETER;
	//      continue;
	//    }

	    int len =TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN;
	    int ret;
	    TCP_SAT_ID sat_id=TCP_get_apid_sat_id(packet);
	    if((sat_id&0x01)==MY_SAT_ID){//自機および自機からGS宛て
	    	switch(TCP_get_apid_executer_id(packet)){
	    		case TCP_EXE_ID_RXPIC:
	    			ret = RXPIC_Send(rxpic_driver,packet->packet,len);
	    			break;
	    		case TCP_EXE_ID_GMSK19k2:
	    		case TCP_EXE_ID_GMSK9k6:
	    		case TCP_EXE_ID_AFSK:
//	    			for(int i=0;i<len;i++) printf("%02x ",packet->packet[i]);
//	    			printf("\r\n");
	    			ret = PH_add_txpic_tlm_(packet);
	    			break;
	    		case TCP_EXE_ID_TXPIC:
	    			ret = PH_add_txpic_tlm_(packet);
	    			break;
#ifdef TWO_U
	    		case TCP_EXE_ID_COBC:
	    			ret = COBC_Send(cobc_driver,packet->packet,len);
	    			break;
#endif
                case TCP_EXE_ID_SOBC:
                  ret = SOBC_SendReceive(sobc_driver,sobc_driver->spi_send_data, len);
                    break;
	    		default:
	    			break;
	    	}
	    }
	    else{//自機宛でない (他衛星宛てのコマンド)
	    	switch(TCP_get_route(packet)){
	  	  		case TCP_ROUTE_AFSK:
	    			ret = PH_add_txpic_tlm_(packet);
	    			break;
	  	  		case TCP_ROUTE_MILLMAX:
	    			ret = INTERSAT_Send(intersat_driver,packet->packet,len);
	    			break;
	  	  	  case TCP_ROUTE_LORA:
					//LoRaでの送信は別の場所で行う
	  	  		    ret = LORA_Send(lora_driver,packet->packet,len);
					break;
	  	  	  default :
	  	  		    break;
	  	  }
	    }
	  }
}

AppInfo TLM_txpic_packet_handler(void)
{
  return AI_create_app_info("TLM_TXPIC",
                         TLM_txpic_packet_handler_init_,
                         TLM_txpic_packet_handler_);
}

//TXPICへの送信は1周期1packetに制限する
//TXPICに送信する場合は, 別でLISTを作り別で処理をする
static void TLM_txpic_packet_handler_init_(void)
{
//  gph_.rp_vcdu_counter = 0;
//  // TCPacket -> M_PDU生成処理のパラメータ初期化
//  gph_.rp_t2m = T2M_initialize(&PH_rp_tlm_list,
//                               &gph_.rp_vcdu.m_pdu);
}

static void TLM_txpic_packet_handler_(void)
{
	if(large_driver->analyze.large_packet_flag == LARGE_PACKET_FLAG_ON){
		return;
	}
	else if(large_driver->analyze.large_packet_flag == LARGE_PACKET_FLAG_OFF){
		for(int i = 0; i < large_driver->analyze.send_cycle; i++){
			if(PL_is_empty(&PH_txpic_tlm_list)){
				  return;
			}
			// 送信するパケットを取得
			TCP* packet = (TCP*)PL_get_head(&PH_txpic_tlm_list)->packet;
			// 送信したパケットを消去
			//　ただし, 大容量通信時はあとで入れなおすようにする
			PL_drop_executed(&PH_txpic_tlm_list);

			int len =TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN ;
			int ret;

			ret = TXPIC_Send(txpic_driver,packet->packet, len);
	//		for(int i=0;i<len;i++) printf("%02x ",packet->packet[i]);
	//		printf("\r\n");
			//新しくリストに入れる数は制限をかけるようにする
			if(ret == _TRANSMIT_FAILED){
				PL_push_front(&PH_txpic_tlm_list, packet);
			}
		}
	}
}

AppInfo TLM_txpic_largepacket_handler(void)
{
  return AI_create_app_info("TLM_TXPIC",
                         TLM_txpic_largepacket_handler_init_,
                         TLM_txpic_largepacket_handler_);
}

//TXPICへの送信は1周期1packetに制限する
//TXPICに送信する場合は, 別でLISTを作り別で処理をする
static void TLM_txpic_largepacket_handler_init_(void)
{
//  gph_.rp_vcdu_counter = 0;
//  // TCPacket -> M_PDU生成処理のパラメータ初期化
//  gph_.rp_t2m = T2M_initialize(&PH_rp_tlm_list,
//                               &gph_.rp_vcdu.m_pdu);
}

static void TLM_txpic_largepacket_handler_(void)
{
	if(large_driver->analyze.large_packet_flag == LARGE_PACKET_FLAG_OFF){
		return;
	}
	else if(large_driver->analyze.large_packet_flag == LARGE_PACKET_FLAG_ON){
		for(int i = 0; i < large_driver->analyze.send_cycle; i++){
			if(PL_is_empty(&PH_txpic_tlm_list)){
				  return;
			}
			// 送信するパケットを取得
			TCP* packet = (TCP*)PL_get_head(&PH_txpic_tlm_list)->packet;
			// 送信したパケットを消去
			//　ただし, 大容量通信時はあとで入れなおすようにする
			PL_drop_executed(&PH_txpic_tlm_list);

			int len =TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN ;
			int ret;

			ret = TXPIC_Send(txpic_driver,packet->packet, len);
	//		for(int i=0;i<len;i++) printf("%02x ",packet->packet[i]);
	//		printf("\r\n");
			//該当TLMを送った回数を数え, 全て送ったら大容量通信モードを終える
			if(ret == _SUCCESS && TCP_TLM_get_packet_id(packet) == large_driver->tlm.packet_identifer){
				print(LARGE_PACKET, "send_counter is %d \r\n",large_driver->analyze.send_counter);
				++large_driver->analyze.send_counter;
			}
			//大容量通信の場合送れなかったらリストに入れなおす
			//新しくリストに入れる数は制限をかけるようにする
			else if(ret == _TRANSMIT_FAILED){
				PL_push_front(&PH_txpic_tlm_list, packet);
			}
			if(large_driver->analyze.send_counter == large_driver->analyze.pre_packet_counter){//送り終わったらフラグを降ろす
				large_packet_reset(&large_driver->analyze.large_packet_flag);
				large_packet_reset(&large_driver->analyze.function_flag);
				large_packet_reset(&large_driver->analyze.re_send_flag);
				printf("send_counter is %d \r\n",large_driver->analyze.send_counter);
				printf("pre_packet_counter is %d \r\n",large_driver->analyze.pre_packet_counter);
				printf("large_packet_send_finish\r\n");
				if(large_driver->analyze.re_send_flag == 0){
					analyzer_init();
				}
				else if(large_driver->analyze.resend_last_one == LARGE_PACKET_FLAG_ON){
					analyzer_init();
				}
				return;
			}
		}
	}
}

PH_ACK PH_add_txpic_tlm_(const CommonCmdPacket* packet)
{
	PL_ACK ack = PL_push_back(&PH_txpic_tlm_list, packet);

	if (ack != PL_SUCCESS) return PH_ACK_PL_LIST_FULL;

	return PH_ACK_SUCCESS;
}

//#pragma section
