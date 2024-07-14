/*
 * NU1_global_interrupt.c
 *
 *  Created on: 2020/11/24
 *      Author: Noro
 */
#include "global_interrupt.h"
#include "../Library/Delay.h"
//めちゃめちゃ循環参照っぽいが.cファイルで読んでるので大丈夫
#include "../Applications/UserDefined/Cdh/Communication_Fault_Detection.h"
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include <main.h>
#include <src_user/Applications/UserDefined/Cdh/RTC_handler.h>
#include <src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.h>
#include <src_user/Drivers/Power/PWR_State.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_hils.h>

uint8_t rxpic_anomaly_detect_counter = 10;//MOBCがついた瞬間はPICからデータを貰っても反応できない　⇒　検知までdelayを設ける

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){
	int ret;
	int timeout=3;//順番的にCOBCの前に何か入れないとifdefでエラー吐く
#ifdef HILS
	static int t = 0;
	static int start_byte = 67; // 67 is C in ASCII code
	static int receive_size = 102; //bytes
#endif
	if(UartHandle->Instance==UART8){//DBG
		HAL_UART_Receive_IT_C2A(UartHandle,(uint8_t*)&DBG_rx_buf,1);
		DBG_Callback(DBG_rx_buf);
	}
	else if(UartHandle->Instance==UART4){//GPS
		do{
			ret=HAL_UART_Receive_IT_C2A(UartHandle,(uint8_t*)&(gps_queue.dri.byte_buf),1);
		}while(ret!=HAL_OK || --timeout==0);
		GPS_Callback_(&gps_queue);
		if(timeout==0) {
			ANOMALY_UART_Set_Anomaly(UART_FAULT_GPS);
//			AL_add_anomaly(AL_GROUP_UART, UART_FAULT_GPS_INTERRUPT);
		}
	}
#ifdef TWO_U
	else if(UartHandle->Instance==USART2){//COBC
//		do{
		ret=HAL_UART_Receive_DMA_C2A(UartHandle,(uint8_t*)&(cobc_queue.dri.byte_buf),1);
//		}while(ret!=HAL_OK || --timeout==0);
		Callback_(&cobc_queue);
		if(timeout==0) {
			ANOMALY_UART_Set_Anomaly(UART_FAULT_COBC);
//			AL_add_anomaly(AL_GROUP_UART, UART_FAULT_COBC_INTERRUPT);
			printf("COBC Receive at least fault\r\n");
		}
//		else if(timeout!=2) printf("COBC Receive at least fault\r\n");
	}
#endif
	else if(UartHandle->Instance==UART5){//ISAT
		do{
			ret=HAL_UART_Receive_DMA_C2A(UartHandle,(uint8_t*)&(intersat_queue.dri.byte_buf),1);
		}while(ret!=HAL_OK || --timeout==0);
		Callback_(&intersat_queue);
		if(timeout==0) {
			ANOMALY_UART_Set_Anomaly(UART_FAULT_MILLMAX);
//			AL_add_anomaly(AL_GROUP_UART, UART_FAULT_MILLMAX_INTERRUPT);
		}
		else if(timeout!=2) printf("intersat Receive at least fault\r\n");
	}
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *UartHandle, uint16_t Size){
	int ret;
	if(UartHandle->Instance==USART1){//RXPIC
		Callback_Packet(&rxpic_queue,Size);
		ret=HAL_UARTEx_ReceiveToIdle_DMA_C2A(UartHandle, &rxpic_queue.dri.receive_buf[0], TCP_MAX_LEN);
		if(ret!=HAL_OK){
			ANOMALY_UART_Set_Anomaly(UART_FAULT_RXPIC);
//			AL_add_anomaly(AL_GROUP_UART, UART_FAULT_RXPIC_INTERRUPT);
			printf("RXPIC Interrupt Anomaly!\r\n");
		}
	}
//#ifdef TWO_U
//	else if(UartHandle->Instance==USART2){//COBC
//		Callback_Packet(&cobc_queue,Size);
//		ret=HAL_UARTEx_ReceiveToIdle_DMA(UartHandle, &cobc_queue.dri.receive_buf[0], TCP_MAX_LEN);
//		printf("r is %d \r\n",ret);//		if(ret!=HAL_OK){
//			ANOMALY_UART_Set_Anomaly(UART_FAULT_COBC);
//			AL_add_anomaly(AL_GROUP_UART, UART_FAULT_COBC_INTERRUPT);
//			printf("ret is %d \r\n", ret);
//			printf("COBC Interrupt Anomaly!\r\n");
//		}
//	}
//#endif
	else if(UartHandle->Instance==USART3){//TXPIC
		Callback_Packet(&txpic_queue,Size);
		ret=HAL_UARTEx_ReceiveToIdle_DMA_C2A(UartHandle, &txpic_queue.dri.receive_buf[0], TCP_MAX_LEN);
		if(ret!=HAL_OK){
			ANOMALY_UART_Set_Anomaly(UART_FAULT_TXPIC);
//			AL_add_anomaly(AL_GROUP_UART, UART_FAULT_TXPIC_INTERRUPT);
			printf("TXPIC Interrupt Anomaly!\r\n");
		}
	}
//	else if(UartHandle->Instance==UART5){//1U-2U MOBC
//		Callback_Packet(&intersat_queue,Size);
//		ret=HAL_UARTEx_ReceiveToIdle_DMA_C2A(UartHandle, &intersat_queue.dri.receive_buf[0], TCP_MAX_LEN);
//		if(ret!=HAL_OK){
//			ANOMALY_UART_Set_Anomaly(UART_FAULT_MILLMAX);
//			AL_add_anomaly(AL_GROUP_UART, UART_FAULT_MILLMAX_INTERRUPT);
//			printf("ret is %d \r\n",ret);
//			printf("MILLMAX Interrupt Anomaly!\r\n");
//		}
//	}
#ifdef HILS
	else if(UartHandle->Instance==UART7){//HILS
//		ObcTime at_now=TMGR_get_master_clock();
//		while(HAL_UART_Get_State_C2A(UartHandle) != 32){
//			ObcTime now=TMGR_get_master_clock();
//			if(OBCT_diff_in_msec(&at_now ,&now)>((unsigned int)(0xff))){
//				break;
//			}
//		}
		ret=HAL_UARTEx_ReceiveToIdle_DMA_C2A(UartHandle,(uint8_t *)hils_receive_buf, 175);
        if(ret!=HAL_OK){
            //printf("HILS Interrupt Anomaly!\r\n");
        }
		hils_received = 1;
//		printf("buf is\r\n");
//		for(int i =0;i < TCP_MAX_LEN; i++){
//			printf("%02x", buf[i]);
//		}
//		printf("\r\n");
////		printf("now buf is %d \r\n", );
//		printf("size is %d \r\n", Size);
//		printf("ret is %d \r\n", ret);
	}
#endif
}



//Todo Ukita UARTのエラー割込み//ほんとはCommunication　Fault Detectionのアノマリ検知に入れる方が良い気がする．．．　Noro
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Prevent unused argument(s) compilation warning */
//#define HAL_UART_ERROR_NONE              0x00000000U   /*!< No error            */
//#define HAL_UART_ERROR_PE                0x00000001U   /*!< Parity error        */
//#define HAL_UART_ERROR_NE                0x00000002U   /*!< Noise error         */
//#define HAL_UART_ERROR_FE                0x00000004U   /*!< Frame error         */
//#define HAL_UART_ERROR_ORE               0x00000008U   /*!< Overrun error       */
//#define HAL_UART_ERROR_DMA               0x00000010U   /*!< DMA transfer error  */
	if(UartHandle->Instance==USART1){
		if(rxpic_anomaly_detect_counter == 0){
			printf("RXPIC error callback\r\n");
			printf("Error code is %d\r\n",(int)UartHandle->ErrorCode);
			HAL_UART_DeInit_C2A((void*)UartHandle);
			HAL_UART_Init_C2A((void*)UartHandle);
			queue_count_reset(&rxpic_queue);
			HAL_UARTEx_ReceiveToIdle_DMA_C2A(UartHandle, &rxpic_queue.dri.receive_buf[0], TCP_MAX_LEN);
		}
		else if(rxpic_anomaly_detect_counter > 0){
			--rxpic_anomaly_detect_counter;
		}
	}
	else if(UartHandle->Instance==USART3){
		printf("TXPIC error callback\r\n");
		printf("Error code is %d\r\n",(int)UartHandle->ErrorCode);
		HAL_UART_DeInit_C2A((void*)UartHandle);
		HAL_UART_Init_C2A((void*)UartHandle);
		queue_count_reset(&txpic_queue);
		HAL_UARTEx_ReceiveToIdle_DMA_C2A(UartHandle, &txpic_queue.dri.receive_buf[0], TCP_MAX_LEN);
	}
#ifdef TWO_U
	else if(UartHandle->Instance==USART2){
		printf("COBC error callback\r\n");
		printf("Error code is %d\r\n",(int)UartHandle->ErrorCode);
		HAL_UART_DeInit_C2A((void*)UartHandle);
		HAL_UART_Init_C2A((void*)UartHandle);
		queue_count_reset(&cobc_queue);
		HAL_UART_Receive_DMA_C2A(UartHandle,(uint8_t*)&(cobc_queue.dri.byte_buf),1);
	}
#endif
	else if(UartHandle->Instance==UART5){
		printf("Intersat error callback\r\n");
		printf("Error code is %d\r\n",(int)UartHandle->ErrorCode);
		HAL_UART_DeInit_C2A((void*)UartHandle);
		HAL_UART_Init_C2A((void*)UartHandle);
		queue_count_reset(&intersat_queue);
		HAL_UART_Receive_DMA_C2A(UartHandle,(uint8_t*)&(intersat_queue.dri.byte_buf),1);
	}
	else if(UartHandle->Instance==UART4){//GPS printf(GPS error)
		printf("GPS port error\r\n");
	}
	else if(UartHandle->Instance==UART8){//DBG
		printf("DBG port error\r\n");
	}
}
//HILS UAR
//#ifdef HILS
//
//static int t = 0;
//static int start_byte = 67; // 67 is C in ASCII code
//static int receive_size = 102; //bytes
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){
//		if(UartHandle->Instance==UART7){//SILS/HILS
//
//			buf[t] = uart7buf[0];
//
//			if(buf[0] != start_byte){
//				t = 0;
//			}else{
//				t++;
//			}
//			if(t == receive_size){
//				t = 0;
//				flag = 1;
//			}
//
//			HAL_UART_Receive_IT_C2A(&huart7,(uint8_t*)&uart7buf[0],1);
//
//		}
//		else if(UartHandle->Instance==UART8){//DBG
//			HAL_UART_Receive_IT_C2A(&huart8,(uint8_t*)&DBG_rx_buf,1);
//			DBG_Callback(DBG_rx_buf);
//		}
//}
//#endif

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if(GPIO_Pin == OSC32_IN_Pin){
    if(PWR_State_GET(PWR_State_GPS) && p_gps_sensor->GPS_Status == 0x04 && p_gps_sensor->RTC_UTC_Permission == 0xAA){
      p_rtc_struct->sTime.Hours   = p_gps_sensor->GPS_UTC_HOUR;
      p_rtc_struct->sTime.Minutes = p_gps_sensor->GPS_UTC_MIN;
      p_rtc_struct->sTime.Seconds = p_gps_sensor->GPS_UTC_SEC + 1;
      p_rtc_struct->sDate.Month   = p_gps_sensor->GPS_UTC_MONTH;
      p_rtc_struct->sDate.Date    = p_gps_sensor->GPS_UTC_DAY;
      p_rtc_struct->year          = p_gps_sensor->GPS_UTC_YEAR;

      if(p_rtc_struct->sTime.Seconds == 60) p_rtc_struct->sTime.Seconds = 0;

      Set_RTC_Time_Date(p_rtc_struct);
      print(GPS, "---------- PPS received ----------\r\n");
    }
  }
  else if(GPIO_Pin == GPIO3V3_SOBCtoMOBC_Pin){
//    for(int i=0; i < 256 ; i ++){
//    sobc_driver->spi_send_data[i] = i+1;
//    }
//
//    SOBC_SendReceive(sobc_driver, sobc_driver->spi_send_data, 256);
    printf("SOBC_GPIO_DEBUG\r\n");
  }
}
