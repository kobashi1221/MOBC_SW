/**
 * @file   rxpic_telemetry_data_definitions.h
 * @brief  バッファリングされているテレメをパースしてMOBC内でかんたんに利用できるようにするためのテレメデータ構造体定義
 */
#ifndef TXPIC_TELEMETRY_DATA_DEFINITIONS_H_
#define TXPIC_TELEMETRY_DATA_DEFINITIONS_H_

typedef struct
{
    struct{
      uint16_t Anomaly_time1;
      uint8_t Anomaly_ID1;
      uint16_t Anomaly_time2;
      uint8_t Anomaly_ID2;
      uint16_t Anomaly_time3;
      uint8_t Anomaly_ID3;
      uint16_t Anomaly_time4;
      uint8_t Anomaly_ID4;
      uint16_t Anomaly_time5;
      uint8_t Anomaly_ID5;
      uint16_t Anomaly_time6;
      uint8_t Anomaly_ID6;
      uint16_t Anomaly_time7;
      uint8_t Anomaly_ID7;
      uint16_t Anomaly_time8;
      uint8_t Anomaly_ID8;
      uint16_t Anomaly_time9;
      uint8_t Anomaly_ID9;
      uint16_t Anomaly_time10;
      uint8_t Anomaly_ID10;
      uint16_t Anomaly_time11;
      uint8_t Anomaly_ID11;
      uint16_t Anomaly_time12;
      uint8_t Anomaly_ID12;
      uint16_t Anomaly_time13;
      uint8_t Anomaly_ID13;
      uint16_t Anomaly_time14;
      uint8_t Anomaly_ID14;
      uint16_t Anomaly_time15;
      uint8_t Anomaly_ID15;
      uint16_t Anomaly_time16;
      uint8_t Anomaly_ID16;
      uint16_t Anomaly_time17;
      uint8_t Anomaly_ID17;
      uint16_t Anomaly_time18;
      uint8_t Anomaly_ID18;
      uint16_t Anomaly_time19;
      uint8_t Anomaly_ID19;
      uint16_t Anomaly_time20;
      uint8_t Anomaly_ID20;
      uint16_t Anomaly_time21;
      uint8_t Anomaly_ID21;
      uint16_t Anomaly_time22;
      uint8_t Anomaly_ID22;
      uint16_t Anomaly_time23;
      uint8_t Anomaly_ID23;
      uint16_t Anomaly_time24;
      uint8_t Anomaly_ID24;
      uint16_t Anomaly_time25;
      uint8_t Anomaly_ID25;
      uint16_t Anomaly_time26;
      uint8_t Anomaly_ID26;
      uint16_t Anomaly_time27;
      uint8_t Anomaly_ID27;
      uint16_t Anomaly_time28;
      uint8_t Anomaly_ID28;
      uint16_t Anomaly_time29;
      uint8_t Anomaly_ID29;
      uint16_t Anomaly_time30;
      uint8_t Anomaly_ID30;
      uint16_t Anomaly_time31;
      uint8_t Anomaly_ID31;
      uint16_t Anomaly_time32;
      uint8_t Anomaly_ID32;
      uint16_t Anomaly_time33;
      uint8_t Anomaly_ID33;
      uint16_t Anomaly_time34;
      uint8_t Anomaly_ID34;
      uint16_t Anomaly_time35;
      uint8_t Anomaly_ID35;
    } txpic_ah1;

	struct{
      uint16_t Anomaly_time36;
      uint8_t Anomaly_ID36;
      uint16_t Anomaly_time37;
      uint8_t Anomaly_ID37;
      uint16_t Anomaly_time38;
      uint8_t Anomaly_ID38;
      uint16_t Anomaly_time39;
      uint8_t Anomaly_ID39;
      uint16_t Anomaly_time40;
      uint8_t Anomaly_ID40;
      uint16_t Anomaly_time41;
      uint8_t Anomaly_ID41;
      uint16_t Anomaly_time42;
      uint8_t Anomaly_ID42;
      uint16_t Anomaly_time43;
      uint8_t Anomaly_ID43;
      uint16_t Anomaly_time44;
      uint8_t Anomaly_ID44;
      uint16_t Anomaly_time45;
      uint8_t Anomaly_ID45;
      uint16_t Anomaly_time46;
      uint8_t Anomaly_ID46;
      uint16_t Anomaly_time47;
      uint8_t Anomaly_ID47;
      uint16_t Anomaly_time48;
      uint8_t Anomaly_ID48;
      uint16_t Anomaly_time49;
      uint8_t Anomaly_ID49;
      uint16_t Anomaly_time50;
      uint8_t Anomaly_ID50;
      uint16_t Anomaly_time51;
      uint8_t Anomaly_ID51;
      uint16_t Anomaly_time52;
      uint8_t Anomaly_ID52;
      uint16_t Anomaly_time53;
      uint8_t Anomaly_ID53;
      uint16_t Anomaly_time54;
      uint8_t Anomaly_ID54;
      uint16_t Anomaly_time55;
      uint8_t Anomaly_ID55;
      uint16_t Anomaly_time56;
      uint8_t Anomaly_ID56;
      uint16_t Anomaly_time57;
      uint8_t Anomaly_ID57;
      uint16_t Anomaly_time58;
      uint8_t Anomaly_ID58;
      uint16_t Anomaly_time59;
      uint8_t Anomaly_ID59;
      uint16_t Anomaly_time60;
      uint8_t Anomaly_ID60;
      uint16_t Anomaly_time61;
      uint8_t Anomaly_ID61;
      uint16_t Anomaly_time62;
      uint8_t Anomaly_ID62;
      uint16_t Anomaly_time63;
      uint8_t Anomaly_ID63;
      uint16_t Anomaly_time64;
      uint8_t Anomaly_ID64;
      uint16_t Anomaly_time65;
      uint8_t Anomaly_ID65;
      uint16_t Anomaly_time66;
      uint8_t Anomaly_ID66;
      uint16_t Anomaly_time67;
      uint8_t Anomaly_ID67;
      uint16_t Anomaly_time68;
      uint8_t Anomaly_ID68;
      uint16_t Anomaly_time69;
      uint8_t Anomaly_ID69;
      uint16_t Anomaly_time70;
      uint8_t Anomaly_ID70;
      uint8_t position;
      uint8_t flag;
	} txpic_ah2;

	struct{
	  uint8_t flag[2];
	  uint8_t Power_on_reset_count;
	  uint8_t WDT_reset_count;
	  uint8_t Software_reset_count;
	  uint32_t TXPIC_Time;
	  uint8_t GPIO_State[3];

      struct {
        uint16_t curs_txpic;
        uint16_t curs_rxpic;
        uint16_t curs_tx;
      } cur_vols_pic;//PIC内蔵のものは10bit

      struct {
        uint16_t curs_rxtx5v;
        uint16_t curs_mobc;
        uint16_t vols_txpic;
        uint16_t vols_rxpic;
        uint16_t vols_rxtx5v;
        uint16_t vols_tx;
        uint16_t curs_sap1_px;
        uint16_t curs_sap1_py;
        uint16_t curs_sap1_pz;
        uint16_t curs_sap2_mx;
        uint16_t curs_sap2_my;
        uint16_t vols_sap1;
        uint16_t vols_sap2;
        uint16_t vols_ioex;
        uint16_t vols_2ndbat;
        uint16_t vols_bus;
        uint16_t curs_2ndbat;
        uint16_t curs_bus;
      }cur_vols_ioex;//IOEXは12bit

      struct{
        uint16_t txpic;
        uint16_t rxpic;
        uint16_t strmx;
        uint16_t strmy;
        uint16_t strmz;
        uint16_t strpx;
        uint16_t strpy;
        uint16_t strpz;
        uint16_t rx;
        uint16_t _2ndbat1;
        uint16_t _2ndbat2;
        uint16_t _2ndbat3;
        uint16_t _2ndbat4;
      }temp;

    uint8_t heat_flag;
    uint16_t dac_mppt1_vol;
    uint16_t dac_mppt2_vol;
    uint16_t dac_charge_vol;
    uint16_t downlink_count;
    uint16_t satellite_com_count;
	uint8_t latest_anomaly_ID;
	uint16_t anomaly_count;
	uint8_t no_com_sec_rxpic;
	uint8_t no_com_sec_mobc;
	uint16_t no_com_sec_gs;
	uint8_t stackoverflow_count;
	uint8_t unrecognized_reset_count;
	uint8_t mobc_enable_count;
	uint8_t reg_rxtx5V_Disable_count;
	int32_t my_time;

	uint8_t cw_message_ratio;
	uint8_t cw_message_start_num;
	uint8_t cw_message_end_num;

	//Total_reset_countはTXPICから受け取った情報でMOBCで計算
	uint8_t Total_reset_count;
  }txpic_to_mobc;
} TXPIC_TlmData;

#endif
