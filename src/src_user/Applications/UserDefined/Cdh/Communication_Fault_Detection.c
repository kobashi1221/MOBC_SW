/**
 * @file Communication_Fault_Detection.c
 * @brief  OBCのUART,SPI,I2Cの機能異常を検知、そこから回復するためのアプリ
 * @author 野呂拓臣
 * @date   2021/12/04
 */
#include "Communication_Fault_Detection.h"

#include <src_core/Library/endian.h>
#include "src_user/Applications/Middleware/MemoryMW.h"
#include "../../../Drivers/global_interrupt.h"
#include "../../../Drivers/Power/PWR_Operation.h"
#include "../Aocs/AOCS_cmd_Separation.h"
#include <src_user/Applications/DriverInstances/di_rxpic.h>
#include <src_user/Applications/DriverInstances/di_txpic.h>
#include <src_user/Applications/DriverInstances/di_cobc.h>
#include <src_user/Applications/DriverInstances/di_intersat.h>

//ポインタ変数のアドレスは固定
//外部ではポインタ変数から中身を読むことしかできないように
COMM_ANOMALY_STRUCT comm_anomaly_struct;
const COMM_ANOMALY_STRUCT* const p_comm_anomaly_struct=&comm_anomaly_struct;
static GPS_STRUCT gps_;

void Com_Detect_initialize_param(void);
void Com_Detect_load_param(void);

static void UART_FAULT_DETECT_init_(void);
static void UART_FAULT_DETECT_exec_(void);
static void I2C_FAULT_DETECT_init_(void);
static void I2C_FAULT_DETECT_exec_(void);
static void SPI_FAULT_DETECT_init_(void);
static void SPI_FAULT_DETECT_exec_(void);

static void ANOMALY_UART_Detect(UART_ANOMALY_NUM num, ObcTime *now);

#ifdef ONE_U
static uint8_t SCAM_I2C_now_conut = 0;
#endif

//lv1はただの通信初期化なので，ポートに依らず，20, 60,80sの際に初期化する．
//lv2は自己リセット又は電源リセットであり一回のみ行うため，ポートごとに設定
//lv3は自分よりもヒエラルキーが低いものに対して電源を操作する
const uint32_t uart_thres_lv1_param[NORESP_THRES_LV1_MAX]={20,60,80,0xffff,0x00ffffff};
const uint32_t uart_thres_lv2_param[UART_FAULT_MAX]={30,30,40,300,300};
const uint32_t uart_thres_lv3_param[UART_FAULT_MAX]={0xffffffff, 0xffffffff, 60, 0xffffffff, 1000};
const uint8_t self_reset_flag= 0xff;
const uint8_t ioex_ahlv2_detect_conut_ = 10;
const uint8_t uart_anomaly_enable[UART_FAULT_MAX]={1, 1, 1, 1, 1};
const uint8_t i2c_anomaly_enable[I2C_FAULT_MAX]={1, 1, 1};
const uint8_t spi_anomaly_enable[SPI_FAULT_MAX]={1, 1, 1, 0, 1};

static uint8_t ioex_now_conut = 0;

void Com_Detect_load_param(void){
  //lv1の値は共通なので，8バイト一度にコピー
    read_param(&comm_anomaly_struct.uart.thres_lv1, &uart_thres_lv1_param, 20, 0x0078);
  for(int i=0;i<UART_FAULT_MAX;i++) {
    read_param(&comm_anomaly_struct.uart.anomaly[i].thres_lv2[0], &uart_thres_lv2_param[i], 4,0x008C+4*i);
  }
  for(int i=0;i<UART_FAULT_MAX;i++) {
      read_param(&comm_anomaly_struct.uart.anomaly[i].thres_lv3, &uart_thres_lv3_param[i], 4, (uint32_t)0x000000CD+4*i);
  }

  read_param(&comm_anomaly_struct.uart.self_reset_flag, &self_reset_flag, 1, (uint32_t)0x000000CC);
  read_param(&comm_anomaly_struct.i2c.ioex_lv2_count, &ioex_ahlv2_detect_conut_, 1, (uint32_t)0x000000E1);

  for(int i=0;i<UART_FAULT_MAX;i++) {
    read_param(&comm_anomaly_struct.uart.enable[i], &uart_anomaly_enable[i], 1, (uint32_t)0x000000E2+i);
  }
  for(int i=0;i<I2C_FAULT_MAX;i++) {
    read_param(&comm_anomaly_struct.i2c.enable[i], &i2c_anomaly_enable[i], 1, (uint32_t)0x00000117+i);
  }
  for(int i=0;i<SPI_FAULT_MAX;i++) {
    read_param(&comm_anomaly_struct.spi.enable[i], &spi_anomaly_enable[i], 1, (uint32_t)0x000000E9+i);
  }
}

void Com_Detect_initialize_param(void){
  write_param(&comm_anomaly_struct.uart.thres_lv1, &uart_thres_lv1_param, 20, 0x0078);
  for(int i=0;i<UART_FAULT_MAX;i++) {
    write_param(&comm_anomaly_struct.uart.anomaly[i].thres_lv2[0], &uart_thres_lv2_param[i], 4, 0x008C+4*i);
  }
  for(int i=0;i<UART_FAULT_MAX;i++) {
    write_param(&comm_anomaly_struct.uart.anomaly[i].thres_lv3, &uart_thres_lv3_param[i], 4, (uint32_t)0x000000CD+4*i);
  }

  write_param(&comm_anomaly_struct.uart.self_reset_flag, &self_reset_flag, 1, (uint32_t)0x000000CC);
  write_param(&comm_anomaly_struct.i2c.ioex_lv2_count, &ioex_ahlv2_detect_conut_, 1, (uint32_t)0x000000E1);

  for(int i=0;i<UART_FAULT_MAX;i++) {
    write_param(&comm_anomaly_struct.uart.enable[i], &uart_anomaly_enable[i], 1, (uint32_t)0x000000E2+i);
  }
  for(int i=0;i<I2C_FAULT_MAX;i++) {
    write_param(&comm_anomaly_struct.i2c.enable[i], &i2c_anomaly_enable[i], 1, (uint32_t)0x00000117+i);
  }
  for(int i=0;i<SPI_FAULT_MAX;i++) {
    write_param(&comm_anomaly_struct.spi.enable[i], &spi_anomaly_enable[i], 1, (uint32_t)0x000000E9+i);
  }
}

AppInfo Com_Detect_param_load_create_app(void) {
  return AI_create_app_info("comm_detect_param_load", Com_Detect_load_param, Com_Detect_initialize_param);
}

AppInfo UART_FAULT_DETECT_create_app(void) {
  return AI_create_app_info("uart_fault_recovery", UART_FAULT_DETECT_init_, UART_FAULT_DETECT_exec_);
}

AppInfo I2C_FAULT_DETECT_create_app(void) {
  return AI_create_app_info("i2c_fault_recovery", I2C_FAULT_DETECT_init_, I2C_FAULT_DETECT_exec_);
}

AppInfo SPI_FAULT_DETECT_create_app(void) {
  return AI_create_app_info("spi_fault_recovery", SPI_FAULT_DETECT_init_, SPI_FAULT_DETECT_exec_);
}


void UART_FAULT_DETECT_init_(void){
  uart_anomaly_struct *uart=&comm_anomaly_struct.uart;
  for(int i = 0; i < UART_FAULT_MAX; i++){
    uart->anomaly[i].count_lv1 = 0;
    uart->anomaly[i].count_lv2 = 0;
    uart->anomaly[i].count_lv3 = 0;
  }
  //ポートの初期化
  ANOMALY_Port_init_(UART_FAULT_RXPIC, 	&uart->anomaly[UART_FAULT_RXPIC],   &huart1, uart->enable[UART_FAULT_RXPIC]);
#ifdef TWO_U
  ANOMALY_Port_init_(UART_FAULT_COBC,  	&uart->anomaly[UART_FAULT_COBC],    &huart2, uart->enable[UART_FAULT_COBC]);
#endif
  ANOMALY_Port_init_(UART_FAULT_TXPIC, 	&uart->anomaly[UART_FAULT_TXPIC],   &huart3, uart->enable[UART_FAULT_TXPIC]);
  ANOMALY_Port_init_(UART_FAULT_GPS  , 	&uart->anomaly[UART_FAULT_GPS],     &huart4, uart->enable[UART_FAULT_GPS]);
  if(!Separated()){//分離前
    ANOMALY_Port_init_(UART_FAULT_MILLMAX,  &uart->anomaly[UART_FAULT_MILLMAX], &huart5, uart->enable[UART_FAULT_MILLMAX]);
  }
  else if(Separated()){//分離後
    ANOMALY_Port_init_(UART_FAULT_MILLMAX,  &uart->anomaly[UART_FAULT_MILLMAX], &huart5, 0);
  }
  //通信状態の初期化，一応構造体からちゃんと読む
  for(int i=0; i<UART_FAULT_MAX;i++){
    uart->status[i]=HAL_UART_Get_State_C2A(uart->anomaly[i].handler);
  }
  //アノマリ検知の最低時間を設ける
  for(int i = 0; i < NORESP_THRES_LV1_MAX; i++){
    if(comm_anomaly_struct.uart.thres_lv1[i] < 10){//10s以下は検知しない
        comm_anomaly_struct.uart.thres_lv1[i] = 10 * (i + 1);
    }
  }
  for(int i = 0; i < UART_FAULT_MAX; i++){
    if(comm_anomaly_struct.uart.anomaly[i].thres_lv2[0] < 20){//20s以下は検知しない
        comm_anomaly_struct.uart.anomaly[i].thres_lv2[0] = 20;
    }
  }
  for(int i = 0; i < UART_FAULT_MAX; i++){
    if(comm_anomaly_struct.uart.anomaly[i].thres_lv3[0] < 40){//20s以下は検知しない
        comm_anomaly_struct.uart.anomaly[i].thres_lv3[0] = 40;
    }
  }
  if(comm_anomaly_struct.i2c.ioex_lv2_count < 10){//10回以下にはしない
    comm_anomaly_struct.i2c.ioex_lv2_count = 10;
  }
}


//定義された時間以上通信（パケット等）が来ない場合のアノマリー処理
static void UART_FAULT_DETECT_exec_(void){
  //現在時刻の取得
  uint16_t prev_count_lv1[UART_FAULT_MAX];
  uint16_t prev_count_lv2[UART_FAULT_MAX];
  uint16_t prev_count_lv3[UART_FAULT_MAX];

  ObcTime now=TMGR_get_master_clock();
  for(int i = 0; i < UART_FAULT_MAX; i++){
    prev_count_lv1[i] = comm_anomaly_struct.uart.anomaly[i].count_lv1;
    prev_count_lv2[i] = comm_anomaly_struct.uart.anomaly[i].count_lv2;
    prev_count_lv3[i] = comm_anomaly_struct.uart.anomaly[i].count_lv3;
  }
  ANOMALY_UART_Detect(UART_FAULT_RXPIC,  &now);
  ANOMALY_UART_Detect(UART_FAULT_TXPIC,  &now);
  ANOMALY_UART_Detect(UART_FAULT_COBC,   &now);
  ANOMALY_UART_Detect(UART_FAULT_MILLMAX,&now);
  ANOMALY_UART_Detect(UART_FAULT_GPS,    &now);

  for(int i = 0; i < UART_FAULT_MAX; i++){
    //カウントが増えていたらアノマリ
    if(comm_anomaly_struct.uart.anomaly[i].count_lv1!=prev_count_lv1[i]){
//			int ret=AL_add_anomaly(AL_GROUP_UART, i);
//			printf("UART AL Lv1 %d\r\n",ret);
    }
    if(comm_anomaly_struct.uart.anomaly[i].count_lv2!=prev_count_lv2[i]){
//			int ret=AL_add_anomaly(AL_GROUP_UART, i);
//			printf("UART AL Lv2 %d\r\n",ret);
    }
    if(comm_anomaly_struct.uart.anomaly[i].count_lv3!=prev_count_lv3[i]){
//			int ret=AL_add_anomaly(AL_GROUP_UART, i);
//			printf("UART AL Lv3 %d\r\n",ret);
    }
  }
}

static void ANOMALY_UART_Detect(UART_ANOMALY_NUM i, ObcTime *now){
  ANOMALY_DRIVER_STRUCT *str=&comm_anomaly_struct.uart.anomaly[i];
  //フラグがEnableなら
  if(vote_flag (&(str-> anomaly_enable))){
    int diff_time = (int) OBCT_diff_in_sec(& (str-> last_rcv_time), now);
    if(diff_time > (comm_anomaly_struct.uart.thres_lv1[str->thres_lv1_counter])){
      printf("uart anomaly lv1 detected %d \r\n", i);
      flag_set(&str->anomaly_detect_lv1);
      str->thres_lv1_counter++;
      comm_anomaly_struct.uart.anomaly[i].count_lv1++;
      if(str->thres_lv1_counter > 4){//上限に達したら完全にリセット
          ANOMALY_Default_Settings(&comm_anomaly_struct.uart.anomaly[i]);
      }
      if(str->count_lv1 > 0xf0) str->count_lv1 = 0;
    }
    if(diff_time > (str->thres_lv2[str->thres_lv2_counter])){
      printf("uart anomaly lv2 detected %d \r\n", i);
      flag_set(&str->anomaly_detect_lv2);
      str->thres_lv2_counter++;
      str->count_lv2++;
      if(str->count_lv2 > 2) str->count_lv2 = 0;
    }
    if(diff_time > (str->thres_lv3[str->thres_lv3_counter])){
      printf("uart anomaly lv3 detected %d \r\n", i);
      flag_set(&str->anomaly_detect_lv3);
      str->thres_lv3_counter++;
      str->count_lv3++;
      if(str->count_lv3 > 2) str->count_lv3 = 0;
    }
  }
}

void I2C_FAULT_DETECT_init_(void){
  i2c_anomaly_struct *i2c=&comm_anomaly_struct.i2c;
  for(int i = 0; i < I2C_FAULT_MAX; i++){
    i2c->anomaly[i].count_lv1 = 0;
    i2c->anomaly[i].count_lv2 = 0;
    i2c->anomaly[i].count_lv3 = 0;
  }

  //通信状態の初期化，一応構造体からちゃんと読んでくる形で
#ifdef ONE_U
  ANOMALY_Port_init_(I2C_FAULT_SCAM, &i2c->anomaly[I2C_FAULT_SCAM], &hi2c3, i2c->enable[I2C_FAULT_SCAM]);
#endif
  ANOMALY_Port_init_(I2C_FAULT_ADCIOEX , &i2c->anomaly[I2C_FAULT_ADCIOEX], &hi2c2, i2c->enable[I2C_FAULT_ADCIOEX]);
  ANOMALY_Port_init_(I2C_FAULT_EEPROM_9AXDAC , &i2c->anomaly[I2C_FAULT_EEPROM_9AXDAC], &hi2c1, i2c->enable[I2C_FAULT_EEPROM_9AXDAC]);
  for(int i=0; i<I2C_FAULT_MAX;i++){
    i2c->status[i]=HAL_I2C_Get_State_C2A(i2c->anomaly[i].handler);
  }
  if(i2c->SCAM_I2C_lv2_count < 2){
    i2c->SCAM_I2C_lv2_count = 2;
  }
}

static void I2C_FAULT_DETECT_exec_(void){
  i2c_anomaly_struct *i2c=&comm_anomaly_struct.i2c;
  uint16_t prev_count_lv1[I2C_FAULT_MAX];

  for(int i = 0; i < I2C_FAULT_MAX; i++){
    prev_count_lv1[i] = i2c->anomaly[i].count_lv1;
  }

  #ifdef ONE_U
  ANOMALY_HAL_ACK_Detect(&(i2c->anomaly[I2C_FAULT_SCAM].count_lv1), &i2c->anomaly[I2C_FAULT_SCAM]);
  #endif
  ANOMALY_HAL_ACK_Detect(&(i2c->anomaly[I2C_FAULT_ADCIOEX].count_lv1), &i2c->anomaly[I2C_FAULT_ADCIOEX]);
  ANOMALY_HAL_ACK_Detect(&(i2c->anomaly[I2C_FAULT_EEPROM_9AXDAC].count_lv1), &i2c->anomaly[I2C_FAULT_EEPROM_9AXDAC]);

  if(i2c->anomaly[I2C_FAULT_ADCIOEX].count_lv1 != prev_count_lv1[I2C_FAULT_ADCIOEX]){
    ++ioex_now_conut;
    if(ioex_now_conut == i2c->ioex_lv2_count){
      flag_set(&(i2c->ioex_anomaly_flag));
//			AL_add_anomaly(AL_GROUP_I2C, I2C_FAULT_ADCIOEX); //I2C_AH_IOEX...?
    }
  }
  else if(i2c->anomaly[I2C_FAULT_EEPROM_9AXDAC].count_lv1 != prev_count_lv1[I2C_FAULT_EEPROM_9AXDAC]){
//		AL_add_anomaly(AL_GROUP_I2C, I2C_FAULT_EEPROM_9AXDAC);
  }
  #ifdef ONE_U
  else if(i2c->anomaly[I2C_FAULT_SCAM].count_lv1 != prev_count_lv1[I2C_FAULT_SCAM]){
//		AL_add_anomaly(AL_GROUP_I2C, I2C_FAULT_SCAM);
  }
  #endif
}

void SPI_FAULT_DETECT_init_(void){
  spi_anomaly_struct *spi=&comm_anomaly_struct.spi;

  for(int i = 0; i < I2C_FAULT_MAX; i++){
    spi->anomaly[i].count_lv1 = 0;
    spi->anomaly[i].count_lv2 = 0;
    spi->anomaly[i].count_lv3 = 0;
  }

  //通信状態の初期化，一応構造体からちゃんと読んでくる形で
#ifdef TWO_U
//	ANOMALY_Port_init_(SPI_FAULT_COBC , 	&spi->anomaly[SPI_FAULT_COBC],    &hspi1, spi->enable[SPI_FAULT_COBC]);
#endif
  //MODは使わないらしい
//	ANOMALY_Port_init_(SPI_FAULT_MOD ,  &spi->anomaly[SPI_FAULT_MOD], &hspi2);
  ANOMALY_Port_init_(SPI_FAULT_SPIPROM ,  &spi->anomaly[SPI_FAULT_SPIPROM], &hspi3,   spi->enable[SPI_FAULT_COBC]);
  ANOMALY_Port_init_(SPI_FAULT_SUNS    ,  &spi->anomaly[SPI_FAULT_SUNS],    &hspi4,   spi->enable[SPI_FAULT_SUNS]);
  ANOMALY_Port_init_(SPI_FAULT_MRAM_LORA, &spi->anomaly[SPI_FAULT_MRAM_LORA], &hspi5, spi->enable[SPI_FAULT_MRAM_LORA]);
  ANOMALY_Port_init_(SPI_FAULT_SD, 		&spi->anomaly[SPI_FAULT_SD], &hspi6,        spi->enable[SPI_FAULT_SD]);
  for(int i=0; i<SPI_FAULT_MAX;i++){
    spi->status[i]=HAL_SPI_Get_State_C2A(spi->anomaly[i].handler);
  }
}

static void SPI_FAULT_DETECT_exec_(void){
  spi_anomaly_struct *spi=&comm_anomaly_struct.spi;

  uint16_t prev_count_lv1[SPI_FAULT_MAX];

  for(int i = 0; i < SPI_FAULT_MAX; i++){
    prev_count_lv1[i] = spi->anomaly[i].count_lv1;
  }

#ifdef TWO_U
  ANOMALY_HAL_ACK_Detect(&(spi->anomaly[SPI_FAULT_COBC].count_lv1), &spi->anomaly[SPI_FAULT_COBC]);
#endif //TWO_U
  ANOMALY_HAL_ACK_Detect(&(spi->anomaly[SPI_FAULT_SPIPROM].count_lv1), &spi->anomaly[SPI_FAULT_SPIPROM]);
  ANOMALY_HAL_ACK_Detect(&(spi->anomaly[SPI_FAULT_SUNS].count_lv1), &spi->anomaly[SPI_FAULT_SUNS]);
  ANOMALY_HAL_ACK_Detect(&(spi->anomaly[SPI_FAULT_MRAM_LORA].count_lv1), &spi->anomaly[SPI_FAULT_MRAM_LORA]);
  for(int i = 0; i < SPI_FAULT_MAX; ++i){
    if(spi->anomaly[i].count_lv1 != prev_count_lv1[i]){
//			AL_add_anomaly(AL_GROUP_SPI, i);
    }
  }
}

/*****************************************************************/
//アノマリ対応用のコマンド一覧
CCP_CmdRet Cmd_ComPort_ANOMALY_Set_ENABLE(const CommonCmdPacket* packet){
  const uint8_t *param = CCP_get_param_head(packet);
  switch(param[0]){
  case 1://UART
    ANOMALY_UART_Set_Enable(param[1]);
    break;
  case 2://I2C
    ANOMALY_I2C_Set_Enable(param[1]);
    break;
  case 3://SPI
    ANOMALY_SPI_Set_Enable(param[1]);
    break;
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_ComPort_ANOMALY_Reset_ENABLE(const CommonCmdPacket* packet){
  const uint8_t *param = CCP_get_param_head(packet);
  switch(param[0]){
  case 1://UART
    ANOMALY_UART_Reset_Enable(param[1]);
    break;
  case 2://I2C
    ANOMALY_I2C_Reset_Enable(param[1]);
    break;
  case 3://SPI
    ANOMALY_SPI_Reset_Enable(param[1]);
    break;
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_ComPort_ANOMALY_Port_Reset(const CommonCmdPacket* packet){
  const uint8_t *param = CCP_get_param_head(packet);
  switch(param[0]){
  case 1://UART
    ANOMALY_Default_Settings(&comm_anomaly_struct.uart.anomaly[param[1]]);
    break;
  case 2://I2C
    ANOMALY_Default_Settings(&comm_anomaly_struct.i2c.anomaly[param[1]]);
    break;
  case 3://SPI
    ANOMALY_Default_Settings(&comm_anomaly_struct.spi.anomaly[param[1]]);
    break;
  default :
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_OBC_UART_Reset_LV1(const CommonCmdPacket* packet) {
  for(int i=0; i<UART_FAULT_MAX;i++){
    ANOMALY_DRIVER_STRUCT *str=&comm_anomaly_struct.uart.anomaly[i];
    if(vote_flag(&str->anomaly_detect_lv1)){
      flag_reset(&str->anomaly_detect_lv1);
      //UARTポートをDeinit
      str->ack=(HAL_StatusTypeDef) HAL_UART_DeInit_C2A(str->handler);
      //各ポートごとに初期化し割込み開始
      switch(str->who_am_i){
      case UART_FAULT_RXPIC:
        RXPIC_PORT_INIT(rxpic_driver);
        printf("RXPIC UART AH Lv.1\r\n");
        break;
      case UART_FAULT_TXPIC:
        TXPIC_PORT_INIT(txpic_driver);
        printf("TXPIC UART AH Lv.1\r\n");
        break;
#ifdef TWO_U
      case UART_FAULT_COBC:
        COBC_PORT_INIT(cobc_driver);
        printf("COBC UART AH Lv.1\r\n");
        break;
#endif
      case UART_FAULT_MILLMAX:
        INTERSAT_PORT_INIT(intersat_driver);
        printf("MILLMAX UART AH Lv.1\r\n");
        break;
      case UART_FAULT_GPS:
        str->ack=(HAL_StatusTypeDef) HAL_UART_Init_C2A(str->handler);
        GPS_UART_init(&gps_);
        printf("GPS UART AH Lv.1\r\n");
        break;
      default :
        printf("Another UART AH %d Lv.1\r\n",str->who_am_i);
        break;
      }
    }
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_OBC_UART_Reset_LV2(const CommonCmdPacket* packet){
  for(int i=0;i<UART_FAULT_MAX;i++){
    ANOMALY_DRIVER_STRUCT *str=&comm_anomaly_struct.uart.anomaly[i];
    if(vote_flag(&str->anomaly_detect_lv2)){
      flag_reset(&str->anomaly_detect_lv2);
      switch(str->who_am_i){
        case UART_FAULT_RXPIC:
          printf("RXPIC AH LV2\r\n");
          if(vote_flag(&comm_anomaly_struct.uart.self_reset_flag)){
            User_Error_Handler();
          }
          break;
        case UART_FAULT_TXPIC:
          printf("TXPIC AH LV2\r\n");
          if(vote_flag(&comm_anomaly_struct.uart.self_reset_flag)){
            User_Error_Handler();
          }
          break;
#ifdef TWO_U
        case UART_FAULT_COBC:
          PWR_Operate_Restart(PWR_State_MIS1);
          printf("MIS1 AH LV2\r\n");
          break;//COBCを再起動
#endif
        case UART_FAULT_MILLMAX:
          printf("MILLMAX AH LV2\r\n");
          break;//MILLMAXはアノマリだけ
        case UART_FAULT_GPS:
          PWR_Operate_Restart(PWR_State_GPS);
          printf("GPS AH LV2\r\n");
          break;//GPSを再起動
        default:
          printf("Another UART AH %d Lv.2\r\n",str->who_am_i);
          break;
      }
    }
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_OBC_UART_Reset_LV3(const CommonCmdPacket* packet){
  for(int i=0;i<UART_FAULT_MAX;i++){
    ANOMALY_DRIVER_STRUCT *str=&comm_anomaly_struct.uart.anomaly[i];
    if(vote_flag(&str->anomaly_detect_lv3)){
      flag_reset(&str->anomaly_detect_lv3);
      switch(str->who_am_i){
        case UART_FAULT_RXPIC:
          printf("RXPIC AH LV3\r\n");//対処しない(というかここにはこないはず…)
          break;
        case UART_FAULT_TXPIC:
          printf("TXPIC AH LV3\r\n");//対処しない(というかここにはこないはず…)
          break;
#ifdef TWO_U
        case UART_FAULT_COBC:
          PWR_Operate_ON_OFF(PWR_State_MIS1, 0);
          printf("MIS1 AH LV3\r\n");
          break;//COBCを再起動
#endif
        case UART_FAULT_MILLMAX:
          printf("MILLMAX AH LV3\r\n");
          break;//MILLMAXはアノマリだけ
        case UART_FAULT_GPS:
          printf("GPS AH LV3\r\n");
          PWR_Operate_ON_OFF(PWR_State_GPS, 0);
          break;//GPSを再起動
        default:
          printf("Another UART AH %d Lv.3\r\n",str->who_am_i);
          break;
      }
    }
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_OBC_I2C_Reset(const CommonCmdPacket* packet) {
  for(int i=0; i<I2C_FAULT_MAX;i++) {
    ANOMALY_DRIVER_STRUCT *str = &comm_anomaly_struct.i2c.anomaly[i];
    if(vote_flag(&str->anomaly_detect_lv1)) {
      //I2Cポート初期化
      printf("AH I2C init\n");
      str->ack=(HAL_StatusTypeDef) HAL_I2C_Init_C2A(str->handler);
      if(i == I2C_FAULT_ADCIOEX) {
        flag_reset(&str->anomaly_detect_lv1);
        ++ioex_now_conut;
        if(ioex_now_conut == comm_anomaly_struct.i2c.ioex_lv2_count) {
          flag_set(&comm_anomaly_struct.i2c.ioex_anomaly_flag);
//					AL_add_anomaly(AL_GROUP_SPI, I2C_AH_IOEX);
        }
      }
#ifdef ONE_U
      else if(i == I2C_FAULT_SCAM) {
        ++SCAM_I2C_now_conut;
        if(SCAM_I2C_now_conut < comm_anomaly_struct.i2c.SCAM_I2C_lv2_count) {
        }
        else if(SCAM_I2C_now_conut == comm_anomaly_struct.i2c.SCAM_I2C_lv2_count) {
          flag_reset(&str->anomaly_detect_lv1);
          flag_set(&comm_anomaly_struct.i2c.SCAM_I2C_anomaly_flag);
 //				AL_add_anomaly(AL_GROUP_I2C, I2C_FAULT_SCAM);
          SCAM_I2C_now_conut = 0;
        }
      }
#endif
      else{
        flag_reset(&str->anomaly_detect_lv1);
      }
    }
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#ifdef ONE_U
CCP_CmdRet Cmd_OBC_I2C_OFF(const CommonCmdPacket* packet){
  if(vote_flag(&comm_anomaly_struct.i2c.SCAM_I2C_anomaly_flag)){
    printf("AH(I2C) SCAM OFF\n");
    flag_reset(&comm_anomaly_struct.i2c.SCAM_I2C_anomaly_flag);
    HAL_GPIO_WritePin_C2A(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_C2A(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
#endif

CCP_CmdRet Cmd_OBC_SPI_Reset(const CommonCmdPacket* packet){
  for(int i=0; i<SPI_FAULT_MAX;i++){
    ANOMALY_DRIVER_STRUCT *str = &comm_anomaly_struct.spi.anomaly[i];
    //SPIポート初期化
    if(vote_flag(&str->anomaly_detect_lv1)){
      flag_reset(&str->anomaly_detect_lv1);
      str->ack=(HAL_StatusTypeDef) HAL_SPI_Init_C2A(str->handler);
    }
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_IOEX_Reset(const CommonCmdPacket* packet){
  if(vote_flag(&comm_anomaly_struct.i2c.ioex_anomaly_flag)){
    flag_reset(&comm_anomaly_struct.i2c.ioex_anomaly_flag);
    PWR_Operate_Restart(PWR_State_IOEX_PWR2);
    ioex_now_conut = 0;
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//外部アプリからのアノマリ登録関数
/******************************************/
void ANOMALY_UART_Update_Time(UART_ANOMALY_NUM num){
  if(num<UART_FAULT_MAX){
    comm_anomaly_struct.uart.anomaly[num].last_rcv_time = TMGR_get_master_clock();
  }
}

void ANOMALY_UART_Set_Anomaly(UART_ANOMALY_NUM num){
  if(num<UART_FAULT_MAX){
    flag_set(&(comm_anomaly_struct.uart.anomaly[num].anomaly_detect_lv1));
  }
}

void ANOMALY_I2C_Set_Status(I2C_ANOMALY_NUM num, HAL_StatusTypeDef ack){
  if(num<I2C_FAULT_MAX){
    comm_anomaly_struct.i2c.anomaly[num].ack=ack;
    //アノマリ登録した時間として使う
    comm_anomaly_struct.i2c.anomaly[num].last_rcv_time = TMGR_get_master_clock();
  }
}

void ANOMALY_SPI_Set_Status(SPI_ANOMALY_NUM num, HAL_StatusTypeDef ack){
  if(num<SPI_FAULT_MAX){
    comm_anomaly_struct.spi.anomaly[num].ack=ack;
    //アノマリ登録した時間として使う
    comm_anomaly_struct.spi.anomaly[num].last_rcv_time = TMGR_get_master_clock();
  }
}

HAL_StatusTypeDef ANOMALY_Replace_Status(int num,int error_num) {
  if(num==error_num) return HAL_ERROR;
  else return HAL_OK;
}

void ANOMALY_UART_Set_Enable(UART_ANOMALY_NUM num) {
  if(num<UART_FAULT_MAX) {
    flag_set(&comm_anomaly_struct.uart.anomaly[num].anomaly_enable);
    ANOMALY_UART_Update_Time(num);
  }
}

void ANOMALY_UART_Reset_Enable(UART_ANOMALY_NUM num) {
  if(num<UART_FAULT_MAX) {
    flag_reset(&comm_anomaly_struct.uart.anomaly[num].anomaly_enable);
  }
}

void ANOMALY_I2C_Set_Enable(I2C_ANOMALY_NUM num) {
  if(num<I2C_FAULT_MAX){
  flag_set(&comm_anomaly_struct.i2c.anomaly[num].anomaly_enable);
  comm_anomaly_struct.i2c.anomaly[num].ack=HAL_OK;
  }
}
void ANOMALY_I2C_Reset_Enable(I2C_ANOMALY_NUM num){
  if(num<I2C_FAULT_MAX){
    flag_reset(&comm_anomaly_struct.i2c.anomaly[num].anomaly_enable);
  }
}

void ANOMALY_SPI_Set_Enable(SPI_ANOMALY_NUM num){
  if(num<SPI_FAULT_MAX){
    flag_set(&comm_anomaly_struct.spi.anomaly[num].anomaly_enable);
    comm_anomaly_struct.spi.anomaly[num].ack=HAL_OK;
  }
}

void ANOMALY_SPI_Reset_Enable(SPI_ANOMALY_NUM num){
  if(num<SPI_FAULT_MAX){
    flag_reset(&comm_anomaly_struct.spi.anomaly[num].anomaly_enable);
  }
}
