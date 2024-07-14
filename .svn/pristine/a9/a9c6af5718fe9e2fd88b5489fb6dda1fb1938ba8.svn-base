/*
 * GPS.h
 *
 *  Created on: 2020/11/27
 *      Author: masudahiroaki
 */

#ifndef C2A_DRIVERS_ACS_GPS_H_
#define C2A_DRIVERS_ACS_GPS_H_

#include "src_user/IfWrapper/HAL.h"
#include <src_core/System/TImeManager/time_manager.h>//for delay()


//todo クラスにしたい main.cと循環参照が原因？
//class GPS_CLASS{
//	private:
//	public:
//};


#define GPS_MAX_LEN (100)                            //MAX:82bit 少し余裕を持たせた
#define GPS_MAX_RECEIVE_PACKET_NUM (15)


typedef struct{
	uint8_t data[GPS_MAX_LEN];
	uint8_t len;
} GPS_Buffer;

typedef struct{
	uint32_t head;
	uint32_t tail;
	uint8_t length;
	GPS_Buffer* buffer;
}GPS_Queue_LIST;

typedef struct{
	uint16_t byte_count;
	uint8_t receive_buf[GPS_MAX_LEN];
	uint8_t byte_buf;
	size_t len;
	ObcTime first_rcv_time;
}gps_driver;

typedef struct{
	GPS_Queue_LIST list;
	gps_driver dri;
}GPS_Queue;

typedef enum
{
	GPS_QUEUE_OK,
	GPS_QUEUE_INVALID,
	GPS_QUEUE_IS_FULL,
	GPS_QUEUE_IS_EMPTY,
} GPS_QUEUE_ACK;

typedef enum
{
	GPS_SUCCESS,
	GPS_TYPE_AD,
	GPS_TYPE_BD,
	GPS_TYPE_BC,
	GPS_INIT_FAILED,
	GPS_FILL_SETUP_FAILED,
	GPS_ON_OFF_FAILED,
	GPS_NOT_RECEIVED,
	GPS_FECW_MISSMATCH,
	GPS_FLAG_OK,
	GGPS_UNKNOWN
} GPS_ACK;

typedef enum
{
	GGA,
	GSA,
	GLL,
	RMC,
	VTG,
	ZDA,
	GPS_UNKKOWN
}GPS_HEADER;


typedef struct
{
	UART_HandleTypeDef *uart_handler;
	int TIMEOUT;
	GPS_Buffer buf;
	GPS_ACK ack;
	GPS_HEADER header;
	uint8_t len;

	char GPSUTC[8];
	char GPSUTC_HOUR[8];
	char GPSUTC_MIN[8];
	char GPSUTC_SEC[8];
	char GPSUTC_DAY[8];
	char GPSUTC_MONTH[8];
	char GPSUTC_YEAR[8];
	char GPSUTC_YEAR_L2[8];
	char LatitudeDMM[10];
	char LatitudeD[8];
	char LatitudeM[8];
	char LatitudeS[8];
	char LongitudeD[8];
	char LongitudeDMM[11];
	char LongitudeM[8];
	char LongitudeS[8];
	char Alti[8];
	char Geoidal_sep[5];
	char Speed[8];
	char Speed_knt[8];
	char degt[5];

	uint8_t Status;             //GNRMCにおけるStatus情報 ‘V’ = Navigation receiver warning ‘A’ = Data Valid
//	uint8_t Mode_indicator[1];     //GNRMCにおけるMode indicator
//	uint8_t Navigation_status[1];  //GNRMCにおけるNavigation indicator
	uint8_t GPS_SyncFlag;          //RTC同期用フラグ
	uint8_t YMD_flag;   //yyyymmddが0かどうかのFlag
	uint8_t LLA_flag;   //Latitude,Londitude,Altitudeが0かどうかのFlag
	uint8_t VTG_flag;   //Velocityデータが0かどうかのFlag
	uint8_t Valid_flag;

	float Latitude;                //計算後緯度(DD)
	float Longitude;               //計算後経度(DD)
	float Altitude;                //計算後高度(m)
	float speed;                   //速さ[km/h]
	float Degree_true;             //degree relative to true north in ENU coordinate
	float Velocity[3];             //GPS velocity
	uint8_t GPS_UTC_HOUR;          //GPS日時の時
	uint8_t GPS_UTC_MIN;           //GPS日時の分
	uint8_t GPS_UTC_SEC;           //GPS日時の秒
	uint8_t GPS_UTC_DAY;           //GPS日時の日
	uint8_t GPS_UTC_MONTH;         //GPS日時の月
	uint16_t GPS_UTC_YEAR;         //GPS日時の年
	uint8_t GPS_UTC_YEAR_L2;       //GPS日時の年下2けた
	uint32_t GPS_UTC;              //GPS日時の時分秒

}GPS_STRUCT;


extern GPS_Queue gps_queue;


#ifdef __cplusplus
 extern "C" {
#endif

void gps_queue_reset(GPS_Queue_LIST* list);
void gps_queue_init(GPS_Queue* queue, size_t length, GPS_Buffer* buf);
void gps_queue_count_reset(GPS_Queue* queue);
GPS_QUEUE_ACK isEmpty_GPS(GPS_Queue_LIST* list);
GPS_QUEUE_ACK isFull_GPS(GPS_Queue_LIST* list);
GPS_QUEUE_ACK gps_dequeue(GPS_Queue_LIST* list, uint8_t* tcp, uint8_t* len);

GPS_ACK GPS_UART_init(GPS_STRUCT* mygps);
void GPS_Callback_(GPS_Queue* queue);

//// taguchi added
GPS_HEADER GPS_check_header(GPS_STRUCT* mygps);
void GPS_VTG_analyze(GPS_STRUCT* mygps);
void GPS_GGA_analyze(GPS_STRUCT* mygps);
void GPS_ZDA_analyze(GPS_STRUCT* mygps);
void GPS_GLL_analyze(GPS_STRUCT* mygps);
void GPS_RMC_analyze(GPS_STRUCT* mygps);



#ifdef __cplusplus
}
#endif


#endif /* C2A_DRIVERS_ACS_GPS_H_ */
