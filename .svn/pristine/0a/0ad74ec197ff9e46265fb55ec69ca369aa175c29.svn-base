/*
 * GPS.cpp
 *
 *  Created on: 2020/11/27
 *      Author: masudahiroaki
 */

#include "GPS.h"
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <string.h>
#include <src_user/Library/General/DBG_printf.h>
#include "math.h"
#include <src_user/Drivers/Aocs/sgdp4h.h>


GPS_Queue gps_queue;

static GPS_Buffer receive_buf[GPS_MAX_RECEIVE_PACKET_NUM];

// taguchi added
void UTC_analyze(GPS_STRUCT* mygps, uint8_t pos);
void Latitude_analyze(GPS_STRUCT* mygps, uint8_t Lati_start_pos);
void Longitude_analyze(GPS_STRUCT* mygps, uint8_t Long_start_pos);
uint8_t Checksum_analyze(GPS_STRUCT* mygps);
uint8_t ascii_to_dec(uint8_t data);
void Speed_analyze(GPS_STRUCT* mygps, uint8_t Speed_start_pos);
void NS_analyze(GPS_STRUCT* mygps, uint8_t Lati_start_pos);
void WE_analyze(GPS_STRUCT* mygps, uint8_t Long_start_pos);
void Valid_analyze(GPS_STRUCT* mygps, uint8_t Valid_start_pos);
void kNot_analyze(GPS_STRUCT* mygps, uint8_t Speed_knt_start_pos);
void Course_analyze(GPS_STRUCT* mygps, uint8_t Course_start_pos);

/*
  * @name   :GPS_UART_init
  * @brief  :GPS用ハンドラー等の設定
  * @param  myrxpicuart:RXPIC用の固有の値を割り当てる
  * @return :void
  */
GPS_ACK GPS_UART_init(GPS_STRUCT* mygps){

	mygps->uart_handler = &huart4;
	mygps->TIMEOUT = 0xff;
	gps_queue_init(&gps_queue, GPS_MAX_RECEIVE_PACKET_NUM, receive_buf);

//	mygps->packet_receive_Flag = 0;

	HAL_UART_Receive_IT_C2A(mygps->uart_handler,(uint8_t*)&(gps_queue.dri.byte_buf),1);
	return GPS_SUCCESS;
}

void gps_queue_reset(GPS_Queue_LIST* list){
    list->tail = 0;
    list->head = 0;
}

void gps_queue_count_reset(GPS_Queue* queue){
	queue->dri.byte_count=0;
	queue->dri.len=0;
}

void gps_queue_init(GPS_Queue* queue, size_t length, GPS_Buffer* buf){
    gps_queue_reset(&queue->list);
    queue->list.length = length;
    queue->list.buffer = buf;
    gps_queue_count_reset(queue);
}

// キューが空かどうかを判定する
GPS_QUEUE_ACK isEmpty_GPS(GPS_Queue_LIST* list){
    return (GPS_QUEUE_ACK)(list->head == list->tail);
}

// スタックが満杯かどうかを判定する
GPS_QUEUE_ACK isFull_GPS(GPS_Queue_LIST* list){
    return (GPS_QUEUE_ACK)(list->head == ((list->tail + 1) % (list->length)));
}

// enqueue (tail に要素を格納してインクリメント)
GPS_QUEUE_ACK gps_enqueue(GPS_Queue_LIST* list, uint8_t* tcp, size_t len){
    if (isFull_GPS(list)) return GPS_QUEUE_IS_FULL;
    memcpy((list->buffer[list->tail].data), tcp, len);
    list->buffer[list->tail].len = len;
    list->tail++;//先に足すべきか，後に足すべきか．．．．．
    if (list->tail == list->length) list->tail = 0; // リングバッファの終端に来たら 0 に
    return GPS_QUEUE_OK;
}

// dequeue (head にある要素を返して head をインクリメント)
GPS_QUEUE_ACK gps_dequeue(GPS_Queue_LIST* list, uint8_t* tcp, uint8_t* len) {
	if (isEmpty_GPS(list)) return GPS_QUEUE_IS_EMPTY;
    memcpy(tcp, list->buffer[list->head].data, GPS_MAX_LEN);
    memcpy(len, &(list->buffer[list->head].len), 1);
    //パース終了の後にHeadを増やさないと，実際のパース前にTailからデータが上書きされ得る
    list->head++;
    if (list->head == list->length) list->head = 0;
    return GPS_QUEUE_OK;
}

void GPS_Callback_(GPS_Queue* queue){
		gps_driver* dri=&(queue->dri);
		ObcTime currenttime=TMGR_get_master_clock();
		//本当にここでいいか要検討　Noro
		if(dri->byte_count>0 && OBCT_diff_in_msec(&dri->first_rcv_time ,&currenttime)>500){
			gps_queue_count_reset(queue);
		}
		dri->receive_buf[dri->byte_count]=dri->byte_buf;
		dri->byte_count++;

		if(dri->byte_count==1){
			dri->first_rcv_time=TMGR_get_master_clock();
			//1バイト目が正しくない
			if(!((dri->receive_buf[0])== 0x24)) dri->byte_count=0;
		}

		if(dri->receive_buf[dri->byte_count - 1] == 0x0a){
			dri->len = dri->byte_count;
			gps_enqueue(&(queue->list),dri->receive_buf, dri->len);
			gps_queue_count_reset(queue);
		}
		//最大長さを超える
		else if(dri->byte_count>GPS_MAX_LEN-1) {
			gps_queue_count_reset(queue);
		}
}






/*----------------------------------- taguchi added*/
GPS_HEADER GPS_check_header(GPS_STRUCT* mygps){

	if(Checksum_analyze(mygps) != 0) return GPS_UNKKOWN;

	if(mygps->buf.data[3] == 'G'){
		if(mygps->buf.data[4] == 'G') return GGA;
		else if(mygps->buf.data[4] == 'S') return GSA;   //GSAは処理しない
		else if(mygps->buf.data[4] == 'L') return GLL;
		else return GPS_UNKKOWN;
	}
	else if(mygps->buf.data[3] == 'R') return RMC;
	else if(mygps->buf.data[3] == 'V') return VTG;
	else if(mygps->buf.data[3] == 'Z') return ZDA;
	return GPS_UNKKOWN;
}

void GPS_GGA_analyze(GPS_STRUCT* mygps){
	uint8_t delimitercount = 0;
	float alti=0;
	float geosep=0;
	//可変長切り抜き
	uint8_t m=0;
	uint8_t n=0;
	uint8_t l=0;
	uint8_t o=0;

	for(int i=0 ; i < mygps->len ; i++){
		if(mygps->buf.data[i] == ','){
			delimitercount++;

			if(delimitercount == 1) UTC_analyze(mygps, i+1);
			else if(delimitercount == 2) Latitude_analyze(mygps, i+1);
			else if(delimitercount == 3) NS_analyze(mygps, i+1);
			else if(delimitercount == 4) Longitude_analyze(mygps, i+1);
			else if(delimitercount == 5) WE_analyze(mygps, i+1);
			else if(delimitercount == 9) {
				m = i;
				// original
//				memcpy(mygps->Alti, &mygps->buf.data[i+1], 8);         //height above sea level
			}
			else if(delimitercount == 10) n=i;
			else if(delimitercount == 11) {
				l=i;
				//original
//				memcpy(mygps->Geoidal_sep, &mygps->buf.data[i+1], 4);  // geoid height
			}
			else if(delimitercount == 12){
				o=i;
				break;
			}
		}
	}

	memcpy(mygps->Alti, &mygps->buf.data[m+1], n-1-m);
	memcpy(mygps->Geoidal_sep, &mygps->buf.data[l+1], o-1-l);

	 alti = atof(mygps->Alti);
	 geosep = atof(mygps->Geoidal_sep);
//	 printf("alti : %f\r\n",alti);
//	 printf("geosep : %f\r\n",geosep);
	 mygps->Altitude = alti + geosep;                //Altitude = height above sea level + geoid height

	 // Latitude,Longitude,Altitude(LLA) Flag
	 if(mygps->Altitude == 0.0f) mygps->LLA_flag = 0x00;
	 else{
		 if(mygps->Latitude == 0.0f || mygps->Longitude == 0.0f) mygps->LLA_flag = 0x55;
		 else mygps->LLA_flag = 0xAA;
	 }

}

void GPS_VTG_analyze(GPS_STRUCT* mygps){
	uint8_t delimitercount = 0;
	uint8_t m=0;
	uint8_t n=0;

	for(int i=0 ; i < mygps->len ; i++){
		if(mygps->buf.data[i] == ','){
			delimitercount++;

			if(delimitercount == 1)  Course_analyze(mygps,i+1);
			// original
//			if(delimitercount == 7) {
//				Speed_analyze(mygps, i+1);
//				break;
//			}
			if(delimitercount == 7) m=i;
			if(delimitercount == 8){
				n=i;
				break;
			}
		}
	}
	memcpy(mygps->Speed, &mygps->buf.data[m+1], n-1-m);
	mygps->speed = atof(mygps->Speed);
//	printf("speed : %f\r\n",mygps->speed);

	// VTG Flag
	if(mygps->speed == 0.0f && mygps->Degree_true == 0.0f) mygps->VTG_flag = 0x00;
	else mygps->VTG_flag = 0xff;
}

void GPS_ZDA_analyze(GPS_STRUCT* mygps){
	uint8_t delimitercount = 0;

	for(int i=0 ; i < mygps->len ; i++){
		if(mygps->buf.data[i] == ','){
			delimitercount++;

		if(delimitercount == 2){
			for(int j=0; j<2 ;j++){
				mygps->GPSUTC_DAY[j] = mygps->buf.data[i + 1 + j];
			}
		}
		if(delimitercount == 3){
			for(int j=0; j<2 ;j++){
				mygps->GPSUTC_MONTH[j] = mygps->buf.data[i + 1 + j ];
			}
		}
		if(delimitercount == 4){
			for(int j=0; j<4 ;j++){
				mygps->GPSUTC_YEAR[j] = mygps->buf.data[i + 1 + j];
			}
			break;
		}
	  }
	}

	mygps->GPS_UTC_DAY = atoi(mygps->GPSUTC_DAY);
	mygps->GPS_UTC_MONTH = atoi(mygps->GPSUTC_MONTH);
	mygps->GPS_UTC_YEAR = atoi(mygps->GPSUTC_YEAR);

	// yyyymmdd(YMD Flag
	if(mygps->GPS_UTC_YEAR < 2022) mygps->YMD_flag = 0x00;
	 else{
		 if(mygps->GPS_UTC_MONTH == 0 || mygps->GPS_UTC_DAY == 0) mygps->YMD_flag = 0x00;
		 else mygps->YMD_flag = 0xff;
	 }
}

void GPS_GLL_analyze(GPS_STRUCT* mygps){
	uint8_t delimitercount = 0;

		for(int i=0 ; i < mygps->len ; i++){
			if(mygps->buf.data[i] == ','){
				delimitercount++;

			if(delimitercount == 1) Latitude_analyze(mygps, i+1);
			else if(delimitercount == 2) NS_analyze(mygps, i+1);
			else if(delimitercount == 3) Longitude_analyze(mygps, i+1);
			else if(delimitercount == 4) WE_analyze(mygps, i+1);
			else if(delimitercount == 5) UTC_analyze(mygps, i+1);
			else if(delimitercount == 6){
				Valid_analyze(mygps, i+1);
				break;
			}
		}
	}
}

void GPS_GSA_analyze(GPS_STRUCT* mygps){                   //GSAに使えそうなデータがない ← ここが3次元モードの時データ有効
	uint8_t delimitercount = 0;
       for(int i=0 ; i < mygps->len ; i++){
			if(mygps->buf.data[i] == ','){
				delimitercount++;
			}
		}
}

void GPS_RMC_analyze(GPS_STRUCT* mygps){
	uint8_t delimitercount = 0;
	char UTC_data[5] = {0};             //一時ddmmyy格納用配列

		for(int i=0 ; i < mygps->len ; i++){
			if(mygps->buf.data[i] == ','){
				delimitercount++;

			if(delimitercount == 1) UTC_analyze(mygps, i+1);
			else if(delimitercount == 2){
				Valid_analyze(mygps, i+1);
			}
			else if(delimitercount == 3) Latitude_analyze(mygps, i+1);
			else if(delimitercount == 4) NS_analyze(mygps, i+1);
			else if(delimitercount == 5) Longitude_analyze(mygps, i+1);
			else if(delimitercount == 6) WE_analyze(mygps, i+1);
//			else if(delimitercount == 7) kNot_analyze(mygps, i+1);  //VTGがあるので不要
			else if(delimitercount == 9){
				for(int j=0; j < 6; j++){
					UTC_data[j] = mygps->buf.data[i + 1 + j];
				}
				break;
		    }
	      }
   }
		memcpy(mygps->GPSUTC_DAY , &UTC_data[0] , 2);
		memcpy(mygps->GPSUTC_MONTH , &UTC_data[2] , 2);
		memcpy(mygps->GPSUTC_YEAR_L2 , &UTC_data[4] , 2);

		mygps->GPS_UTC_DAY = atoi(mygps->GPSUTC_DAY);
		mygps->GPS_UTC_MONTH = atoi(mygps->GPSUTC_MONTH);
		mygps->GPS_UTC_YEAR_L2 = atoi(mygps->GPSUTC_YEAR_L2);

}


/*------------------------ 各センテンスの読み取りの際に実行する関数達-----------------------*/
uint8_t Checksum_analyze(GPS_STRUCT* mygps){
	uint8_t checkSum = 0;
	uint8_t ascii_cs = 0;
	uint8_t len = mygps->len;

	for(int i=1; i < len-5; i++){
		checkSum ^= mygps->buf.data[i];
	}

	ascii_cs = ascii_to_dec(mygps->buf.data[len - 4]) << 4 ;
	ascii_cs |= ascii_to_dec(mygps->buf.data[len - 3]);

	if( checkSum == ascii_cs) return 0;   //チェックサムが合っていたら0、間違っていたら1
	else return 1;
}

// asciiコードを10進数に変換
uint8_t ascii_to_dec(uint8_t data){
	uint8_t ret = 0;

	if (data >= 0x41 && data <= 0x46){	             //A~Fまで
			ret = data - 0x37;
	}
	else if (data >= 0x30 && data <= 0x39){          //0~9まで
		    ret = data - 0x30;
	}
	return ret;
}

void UTC_analyze(GPS_STRUCT* mygps, uint8_t UTC_start_pos){
    for(int i = 0; i < 2; i++){
		 mygps->GPSUTC_HOUR[i] =  mygps->buf.data[i + UTC_start_pos];
		 mygps->GPSUTC_MIN[i] =  mygps->buf.data[i + UTC_start_pos + 2];
		 mygps->GPSUTC_SEC[i] =  mygps->buf.data[i + UTC_start_pos + 4];     ///msec以下は取得しない.渡すのがuint8_t型になってる
    }
	mygps->GPS_UTC_HOUR = atoi(mygps->GPSUTC_HOUR);
	mygps->GPS_UTC_MIN = atoi(mygps->GPSUTC_MIN);
	mygps->GPS_UTC_SEC = atoi(mygps->GPSUTC_SEC);
}

void Latitude_analyze(GPS_STRUCT* mygps, uint8_t Lati_start_pos){

    for(int i = 0; i < 2; i++){
	mygps->LatitudeD[i] =  mygps->buf.data[i + Lati_start_pos];
    mygps->LatitudeM[i] =  mygps->buf.data[i + Lati_start_pos + 2];
    }
    for(int i = 0; i < 4; i++){
   	 mygps->LatitudeS[i] =  mygps->buf.data[i + Lati_start_pos + 5];     //Sだけど単位はmin 4桁取得(それ以下は切り捨て)
    }
    float latD = atof(mygps->LatitudeD);
    float latM = atof(mygps->LatitudeM);
    float latS = atof(mygps->LatitudeS)*0.0001f * 60.0f;               //  min->sec
    //度分秒(DMS)から度(DD)に変換
	mygps->Latitude = latD + latM / 60.0f + latS / 3600.0f;

}

void NS_analyze(GPS_STRUCT* mygps, uint8_t Lati_start_pos){
	if(mygps->buf.data[Lati_start_pos] == 'S'){
			mygps->Latitude = -mygps->Latitude;     	//南ならマイナス
	}
}

void Longitude_analyze(GPS_STRUCT* mygps, uint8_t Long_start_pos){
    for(int i = 0; i < 3; i++){
   	 mygps->LongitudeD[i] =  mygps->buf.data[i + Long_start_pos];
    }
    for(int i = 0; i < 2; i++){
    mygps->LongitudeM[i] =  mygps->buf.data[i + Long_start_pos + 3];
    }
    for(int i = 0; i < 4; i++){
   	 mygps->LongitudeS[i] = mygps->buf.data[i + Long_start_pos + 6];     //Sだけど単位はmin 4桁取得(それ以下は切り捨て)
    }
    float lonD = atof(mygps->LongitudeD);
    float lonM = atof(mygps->LongitudeM);
    float lonS = atof(mygps->LongitudeS)*0.0001f * 60.0f;               //  min->sec
	//度分秒(DMS)から度(DD)に変換
    mygps->Longitude = lonD + lonM / 60.0f + lonS / 3600.0f;

}

void WE_analyze(GPS_STRUCT* mygps, uint8_t Long_start_pos){
	if(mygps->buf.data[Long_start_pos] == 'W'){                //西ならマイナス
			mygps->Longitude = -mygps->Longitude;
	}
}

void Speed_analyze(GPS_STRUCT* mygps, uint8_t Speed_start_pos){
	 for(int j = 0; j < 5; j ++){
		 mygps->Speed[j] = mygps->buf.data[j + Speed_start_pos];    //0の時が5桁だから5桁？？？
	 }
	mygps->speed = atof(mygps->Speed);
}

void Valid_analyze(GPS_STRUCT* mygps, uint8_t Valid_start_pos){
	if(mygps->buf.data[Valid_start_pos] == 'A') {
		mygps->Valid_flag = 0xff;           //valid
		mygps->GPS_SyncFlag = 1;
	}
	else{
		mygps->Valid_flag = 0x00;
		mygps->GPS_SyncFlag = 0;
	}
}

void kNot_analyze(GPS_STRUCT* mygps, uint8_t Speed_knt_start_pos){
	float KNT=0.0f;

	for(int j = 0; j < 5; j ++){
			 mygps->Speed_knt[j] = mygps->buf.data[j + Speed_knt_start_pos];    //0の時が5桁だから5桁？？？
		 }

	 KNT = atof(mygps->Speed_knt);
	 mygps->speed = KNT * 1.852;   //knot→km/hに単位変換
}

void Course_analyze(GPS_STRUCT* mygps, uint8_t Course_start_pos){

	for(int j=0 ;j<5 ;j++){
		 mygps->degt[j] = mygps->buf.data[j + Course_start_pos];
	}

	mygps->Degree_true = atof(mygps->degt);

}



