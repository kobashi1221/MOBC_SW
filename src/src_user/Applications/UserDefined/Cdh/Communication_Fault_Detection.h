/**
 * @file UartFaultRecovery.h
 * @brief  OBCの通信異常検知のためのアプリ
 * @author 野呂拓臣
 * @date   2021/11/29
 */
#ifndef COMMUNICATION_FAULT_DETECTION_H_
#define COMMUNICATION_FAULT_DETECTION_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_user/Drivers/Cdh/Com_Fault_Detection_driver.h>

#define NORESP_THRES_LV1_MAX 5

typedef enum
{
	UART_FAULT_RXPIC,
	UART_FAULT_TXPIC,
	UART_FAULT_COBC,
	UART_FAULT_MILLMAX,
	UART_FAULT_GPS,
	UART_FAULT_MAX
} UART_ANOMALY_NUM;

typedef enum
{
	I2C_FAULT_EEPROM_9AXDAC,
	I2C_FAULT_ADCIOEX,
	I2C_FAULT_SCAM,
	I2C_FAULT_MAX
} I2C_ANOMALY_NUM;

typedef enum
{
	SPI_FAULT_SPIPROM,
	SPI_FAULT_SD,
	SPI_FAULT_MRAM_LORA,
	SPI_FAULT_SUNS,
	SPI_FAULT_COBC,
	SPI_FAULT_MAX,
} SPI_ANOMALY_NUM;

typedef enum
{
	UART_FAULT_RXPIC_LV1,
	UART_FAULT_TXPIC_LV1,
	UART_FAULT_COBC_LV1,
	UART_FAULT_MILLMAX_LV1,
	UART_FAULT_GPS_LV1,
} UART_ANOMALY_LV1;

typedef enum
{
	UART_FAULT_RXPIC_LV2,
	UART_FAULT_TXPIC_LV2,
	UART_FAULT_COBC_LV2,
	UART_FAULT_MILLMAX_LV2,
	UART_FAULT_GPS_LV2,
} UART_ANOMALY_LV2;

typedef enum
{
	UART_FAULT_RXPIC_LV3,
	UART_FAULT_TXPIC_LV3,
	UART_FAULT_COBC_LV3,
	UART_FAULT_MILLMAX_LV3,
	UART_FAULT_GPS_LV3,
} UART_ANOMALY_LV3;

typedef enum
{
	UART_FAULT_RXPIC_INTERRUPT,
	UART_FAULT_TXPIC_INTERRUPT,
	UART_FAULT_COBC_INTERRUPT,
	UART_FAULT_MILLMAX_INTERRUPT,
	UART_FAULT_GPS_INTERRUPT,
} UART_ANOMALY_INTERRUPT;

typedef enum
{
	I2C_FAULT_EEPROM_9AXDAC_LV1,
	I2C_FAULT_ADCIOEX_LV1,
	I2C_AH_IOEX,
} I2C_ANOMALY;

typedef enum
{
	SPI_FAULT_SPIPROM_LV1,
	SPI_FAULT_SD_LV1,
	SPI_FAULT_MRAM_LORA_LV1,
	SPI_FAULT_SUNS_LV1,
	SPI_FAULT_COBC_LV1,
} SPI_ANOMALY;

typedef struct{
	ANOMALY_DRIVER_STRUCT anomaly[UART_FAULT_MAX];
	uint32_t thres_lv1[NORESP_THRES_LV1_MAX];//ポートに依らず固定なのでここに置く
	HAL_UART_StateTypeDef status[UART_FAULT_MAX];
	uint8_t enable[UART_FAULT_MAX];
	uint8_t self_reset_flag;
}uart_anomaly_struct;

typedef struct{
	ANOMALY_DRIVER_STRUCT anomaly[I2C_FAULT_MAX];
	HAL_I2C_StateTypeDef status[I2C_FAULT_MAX];
	uint8_t enable[I2C_FAULT_MAX];
	uint8_t ioex_lv2_count;
	uint8_t ioex_anomaly_flag;
	uint8_t SCAM_I2C_lv2_count;
	uint8_t SCAM_I2C_anomaly_flag;
}i2c_anomaly_struct;

typedef struct{
	ANOMALY_DRIVER_STRUCT anomaly[SPI_FAULT_MAX];
	HAL_SPI_StateTypeDef status[SPI_FAULT_MAX];
	uint8_t enable[SPI_FAULT_MAX];
}spi_anomaly_struct;

typedef struct{
	uart_anomaly_struct uart;
	i2c_anomaly_struct i2c;
	spi_anomaly_struct spi;
}COMM_ANOMALY_STRUCT;

//できるのは中身を読むだけ！
extern const COMM_ANOMALY_STRUCT* const p_comm_anomaly_struct;

#ifdef __cplusplus
 extern "C" {
#endif

AppInfo Com_Detect_param_load_create_app(void);
AppInfo UART_FAULT_DETECT_create_app(void);
AppInfo I2C_FAULT_DETECT_create_app(void);
AppInfo SPI_FAULT_DETECT_create_app(void);

CCP_CmdRet Cmd_ComPort_ANOMALY_Set_ENABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ComPort_ANOMALY_Reset_ENABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_ComPort_ANOMALY_Port_Reset(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_OBC_UART_Reset_LV1(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_OBC_UART_Reset_LV2(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_OBC_UART_Reset_LV3(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_OBC_I2C_Reset(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_OBC_I2C_OFF(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_OBC_SPI_Reset(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_IOEX_Reset(const CommonCmdPacket* packet);

//他のアプリでのアノマリ登録用関数（構造体はここで作るべきなのでセッタもここに作らざるを得なかった．．．）
void ANOMALY_UART_Update_Time(UART_ANOMALY_NUM num);
//Global Interruptで無理やり割り込み処理する用の関数
void ANOMALY_UART_Set_Anomaly(UART_ANOMALY_NUM num);
void ANOMALY_I2C_Set_Status(I2C_ANOMALY_NUM num, HAL_StatusTypeDef ack);
void ANOMALY_SPI_Set_Status(SPI_ANOMALY_NUM num, HAL_StatusTypeDef ack);
void ANOMALY_UART_Set_Enable(UART_ANOMALY_NUM num);
void ANOMALY_UART_Reset_Enable(UART_ANOMALY_NUM num);
void ANOMALY_I2C_Set_Enable(I2C_ANOMALY_NUM num);
void ANOMALY_I2C_Reset_Enable(I2C_ANOMALY_NUM num);
void ANOMALY_SPI_Set_Enable(SPI_ANOMALY_NUM num);
void ANOMALY_SPI_Reset_Enable(SPI_ANOMALY_NUM num);
//numは関数から返ってくる返値を，error_numはその関数における通信ポートエラーの番号を入れる
HAL_StatusTypeDef ANOMALY_Replace_Status(int num, int error_num);

#ifdef __cplusplus
 }
#endif

#endif // UART_FAULT_RECOVERY_H_
