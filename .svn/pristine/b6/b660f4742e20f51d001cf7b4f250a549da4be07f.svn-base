#ifndef TC_PACKET_H_
#define TC_PACKET_H_

#include <stddef.h>
#include <stdint.h>

#define TCP_MAX_LEN          (256u)
#define TCP_PRM_HDR_LEN        (6u)
#define TCP_TLM_2ND_HDR_LEN    (9u)
#define TCP_CMD_2ND_HDR_LEN    (2u)
#define TCP_CMD_USER_HDR_LEN   (9u)

#ifdef ONE_U
#define MY_SAT_ID 1
#define ANOTHER_SAT_ID 0
#endif

#ifdef TWO_U
#define MY_SAT_ID 0
#define ANOTHER_SAT_ID 1
#endif

/**
 * @struct TCP
 * @brief  TCPacket or Space Packet
 */
typedef struct
{
  uint8_t packet[TCP_MAX_LEN];
} TCP;

/**
 * @enum   TCP_VER
 * @brief  TCP version
 * @note   3bit
 */
typedef enum
{
  TCP_VER_1 = 0, // 000b: Version-0
  TCP_VER_2 = 1, // 001b: Version-1
  TCP_VER_UNKNOWN
} TCP_VER;

/**
 * @enum   TCP_TYPE
 * @brief  tlm or cmd を規定
 * @note   0/1の1bit
 */
typedef enum
{
  TCP_TYPE_TLM = 0, // 0b: TELEMETRY
  TCP_TYPE_CMD = 1  // 1b: COMMAND
} TCP_TYPE;

/**
 * @enum   TCP_2ND_HDR_FLAG
 * @brief  TCP の Secondary Header Flag の有無
 * @note   0/1の1bit
 */
typedef enum
{
  TCP_2ND_HDR_ABSENT  = 0, // 0b: Secondary Header Absent
  TCP_2ND_HDR_PRESENT = 1  // 1b: Secondary Header Present
} TCP_2ND_HDR_FLAG;

/**
 * @enum   TCP_APID
 * @brief  Application Process ID
 * @note   GSTOS の蓄積データの拡張子につく数字
 * @note   11bit
 * @note   CTCP_DEST_ID として typedef する
 */
typedef enum
{
  TCP_APID_RXPIC_2U = 0x104,	 	  // 00100000100b to 2U-RXPIC
  TCP_APID_TXPIC_2U = 0x105,          // 00100000101b to 2U-TXPIC
  TCP_APID_MOBC_2U =  0x106,		  // 00100000110b to 2U-MOBC
  TCP_APID_COBC_2U =  0x107,		  // 00100000111b to 2U-COBC
  TCP_APID_SOBC_2U =  0x108,		  // 00100001000b to 2U-SOBC

  TCP_APID_RXPIC_1U = 0x114,	 	  // 00000000100b to 1U-RXPIC
  TCP_APID_TXPIC_1U = 0x115,          // 00000000101b to 1U-TXPIC
  TCP_APID_MOBC_1U =  0x116,		  // 00000000110b to 1U-MOBC
  TCP_APID_SOBC_1U =  0x118,		  // 00000001000b to 1U-SOBC
//PROCYONのAPID　使用していない (坂口)
  TCP_APID_PROCYON_TCAL_TLM = 0x410, // 10000010000b: APID for PROCYON TIME CARIBLATION TLM
  TCP_APID_PROCYON_MIS_TLM = 0x510,  // 10100010000b: APID for PROCYON MIS TLM
  TCP_APID_PROCYON_DUMP_TLM = 0x710, // 11100010000b: APID for PROCYON DUMP TLM
  TCP_APID_PROCYON_FILL_PKT = 0x7ff, // 11111111111b: APID for PROCYON FILL PACKET
  TCP_APID_UNKNOWN
} TCP_APID;

typedef enum{
	MAGNARO,
	MAGNAROII,
}TCP_PROJECT_ID;//3bits

typedef enum{
	TCP_SAT_ID_2U = 0b0000,
	TCP_SAT_ID_1U = 0b0001,
	TCP_SAT_ID_GS_2U = 0b0010,
	TCP_SAT_ID_GS_1U = 0b0011,
	TCP_SAT_ID_Amateure_2U = 0b0100,
	TCP_SAT_ID_Amateure_1U = 0b0101,
	TCP_SAT_ID_MAX = 0b1111,
}TCP_SAT_ID;//4bits

typedef enum{
	TCP_EXE_ID_GMSK19k2 = 0b0000,
	TCP_EXE_ID_GMSK9k6  = 0b0001,
	TCP_EXE_ID_AFSK     = 0b0010,
	TCP_EXE_ID_LORA     = 0b0011,
	TCP_EXE_ID_RXPIC = 0b0100,
	TCP_EXE_ID_TXPIC = 0b0101,
	TCP_EXE_ID_MOBC  = 0b0110,
	TCP_EXE_ID_COBC  = 0b0111,
	TCP_EXE_ID_SOBC  = 0b1000,
	TCP_EXE_ID_MAX	 = 0b1111,
}TCP_EXE_ID;//4bits


/**
 * @enum   TCP_SEQ_FLAG
 * @brief  Sequence Flag
 * @note   2bit
 * @note   Packet Sequence Flag for each ADU もこれを用いる．
 *         その場合， component を segment と読み替える．
 */
typedef enum
{
	TCP_SEQ_CONT   = 0, // 00b: Continuation component of higher data structure.
	TCP_SEQ_FIRST  = 1, // 01b: First component of higher data structure.
	TCP_SEQ_LAST   = 2, // 10b: Last component of higher data structure.
	TCP_SEQ_SINGLE = 3  // 11b: Standalone packet.
} TCP_SEQ_FLAG;


typedef enum{
	TCP_ROUTE_DIRECT = 0b0000,
	TCP_ROUTE_AFSK =   0b0001,
	TCP_ROUTE_MILLMAX =   0b0010,
	TCP_ROUTE_LORA = 0b0011,
	TCP_ROUTE_MAX = 0b1111,
}TCP_ROUTE;// 4bits For inter-satellite communication

// NUcubeでは使っていない　坂口(一旦コメントアウト)
/**
 * @enum   TCP_CM
 * @brief  Command Type
 * @note   8bit
 */
//typedef enum
//{
//  TCP_CMD_TYPE_DC = 0x01, // 01h: Discrete Command
//  TCP_CMD_TYPE_SM = 0x02, // 02h: Serial Magnitude Command
//  TCP_CMD_TYPE_UNKNOWN
//} TCP_CMD_TYPE;

/**
 * @enum   TCP_CMD_EXEC_TYPE
 * @brief  コマンド実行種別
 * @note   0x0*を想定（上位4bitは他のC2Aを搭載したボード用に転送するために使うため）
 *         その定義は TCP_DEST_TYPE にある
 *         pure TCPはここが8bitであることに注意
 * @note   TCP_CMD_EXEC_TYPE と CCP_EXEC_TYPE の定義は一致 (正確には、前者は後者に含まれている)
 *         CCP_EXEC_TYPE CCP_get_exec_type(const CTCP* packet) を参照
 * @note   GSからのOBC.TLや，C2A内のCCP_form_tlcはすべてTL0扱いになる
 */
typedef enum
{
  TCP_CMD_EXEC_TYPE_GS = 0x00, // 00h: Ground Station Command
  TCP_CMD_EXEC_TYPE_TL = 0x01, // 01h: Timeline Command
  TCP_CMD_EXEC_TYPE_MC = 0x02, // 02h: Macro Command
  TCP_CMD_EXEC_TYPE_RT = 0x03, // 03h: Realtime Command
  TCP_CMD_EXEC_TYPE_DBG = 0x04, // 04h: Debug Command
  TCP_CMD_EXEC_TYPE_UNKNOWN
} TCP_CMD_EXEC_TYPE;

// NUcubeでは使っていない　坂口(一旦コメントアウト)
/**
 * @enum   TCP_CMD_DEST_TYPE
 * @brief  コマンドの解釈の宛先を規定
 * @note   TO_ME: 自分自身 → 自分自身のTLCやBCとして解釈
 * @note   TO_*:  転送先のTLやBCとして解釈（GSから来たコマンドを自身のキューにいれない）
 * @note   0x*0を想定
 * @note   下位4bitは TCP_CMD_EXEC_TYPE と CCP_EXEC_TYPE に
 */
//typedef enum
//{
//  TCP_CMD_DEST_TYPE_TO_ME     = 0x00,
//  TCP_CMD_DEST_TYPE_TO_MOBC   = 0x10,
//  TCP_CMD_DEST_TYPE_TO_RXPIC   = 0x20,
//  TCP_CMD_DEST_TYPE_TO_TXPIC   = 0x30,
//  TCP_CMD_DEST_TYPE_TO_COBC    = 0x40,
//  TCP_CMD_DEST_TYPE_TO_UNKOWN =  0x50
//} TCP_CMD_DEST_TYPE;


#ifdef __cplusplus
 extern "C" {
#endif

// *******************
//  TCP getter/setter
// *******************

/**
 * @brief  TCP ver を取得
 * @param  tcp: TCP
 * @return TCP_VER
 */
TCP_VER TCP_get_ver(const TCP* tcp);

/**
 * @brief  TCP ver を設定
 * @param[in,out] tcp: TCP
 * @param[in]     ver: TCP_VER
 * @return void
 */
void TCP_set_ver(TCP* tcp, TCP_VER ver);

/**
 * @brief  TCP type を取得
 * @param  tcp: TCP
 * @return TCP_TYPE
 */
TCP_TYPE TCP_get_type(const TCP* tcp);

/**
 * @brief  TCP type を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: TCP_TYPE
 * @return void
 */
void TCP_set_type(TCP* tcp, TCP_TYPE type);

/**
 * @brief  TCP Secondary Header Flag を取得
 * @param  tcp: TCP
 * @return TCP_2ND_HDR_FLAG
 */
TCP_2ND_HDR_FLAG TCP_get_2nd_hdr_flag(const TCP* tcp);

/**
 * @brief  TCP Secondary Header Flag を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     flag: TCP_2ND_HDR_FLAG
 * @return void
 */
void TCP_set_2nd_hdr_flag(TCP* tcp, TCP_2ND_HDR_FLAG flag);

/**
 * @brief  APID を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     apid: APID
 * @return void
 */
void TCP_set_apid(TCP* tcp, TCP_APID apid);

/**
 * @brief  APID を取得
 * @param 	tcp:  TCP
 * @return APID
 */
uint16_t TCP_get_apid_(const TCP* tcp);

/**
 * @brief  APID を設定
 * @param[in,out] 	tcp:  TCP
 * @param[in]     	sat_id: SAT ID
 * 					route: ROUTE
 * 					executer: Executer ID
 * @return void
 */
void TCP_set_apid_(TCP* tcp, TCP_SAT_ID sat_id, TCP_EXE_ID executer);

/**
 * @brief  PROJECT_ID を取得
 * @param 	tcp:  TCP
 * @return PROJECT_ID
 */
TCP_PROJECT_ID TCP_get_apid_project_id(const TCP* tcp);

/**
 * @brief  PROJECT_ID を設定
 * @param 	tcp:  TCP
 */
void TCP_set_apid_project_id(TCP* tcp, TCP_PROJECT_ID project_id);

/**
 * @brief  SAT ID を取得
 * @param 	tcp:  TCP
 * @return SAT ID
 */
TCP_SAT_ID TCP_get_apid_sat_id(const TCP* tcp);

/**
 * @brief  SAT ID を設定
 * @param 	tcp:  TCP
 */
void TCP_set_apid_sat_id(TCP* tcp, TCP_SAT_ID sat_id);


/**
 * @brief  Executer ID を取得
 * @param 	tcp:  TCP
 * @return Executer ID
 */
TCP_EXE_ID TCP_get_apid_executer_id(const TCP* tcp);

/**
 * @brief  Executer ID を取得
 * @param 	tcp:  TCP
 */
void TCP_set_apid_executer_id(TCP* tcp, TCP_EXE_ID exe_id);

/**
 * @brief  Sequence Flag を取得
 * @param  tcp: TCP
 * @return TCP_SEQ_FLAG
 */
TCP_SEQ_FLAG TCP_get_seq_flag(const TCP* tcp);

/**
 * @brief  Sequence Flag を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     flag: TCP_SEQ_FLAG
 * @return void
 */
void TCP_set_seq_flag(TCP* tcp, TCP_SEQ_FLAG flag);

/**
 * @brief  Sequence Count を取得
 * @param  tcp: TCP
 * @return Sequence Count
 */
uint16_t TCP_get_seq_cnt(const TCP* tcp);

/**
 * @brief  Sequence Count を設定
 * @param[in,out] tcp: TCP
 * @param[in]     cnt: Sequence Count
 * @return void
 */
void TCP_set_seq_cnt(TCP* tcp, uint16_t cnt);

/**
 * @brief  Packet Length を取得
 * @param  tcp: TCP
 * @return Packet Length
 */
uint16_t TCP_get_packet_len(const TCP* tcp);

/**
 * @brief  Packet Length を設定
 * @note   Packet Length とは space packet のPACKET DATA FIELD 長であり， PRIMARY HEADER を含まない
 *         ( https://public.ccsds.org/Pubs/133x0b2e1.pdf )
 * @param[in,out] tcp: TCP
 * @param[in]     len: Packet Length
 * @return void
 */
void TCP_set_packet_data_len(TCP* tcp, uint16_t len);

/**
 * @brief  Packet Length を設定
 * @note   Packet Length とは space packet のPACKET DATA FIELD 長であり， PRIMARY HEADER を含まない
 *         ( https://public.ccsds.org/Pubs/133x0b2e1.pdf )
 * @param[in,out] tcp: TCP
 * @param[in]     len: Packet 全長
 * @return void
 */
void TCP_set_packet_len(TCP* tcp, uint16_t len);

/**
 * @brief  TCP をコピー
 * @note   高速化のためすすべてではなく必要最低限しかコピーしない場合
 * @param[in,out] dest: コピー先 TCP
 * @param[in]     src:  コピー元 TCP
 * @return void
 */
void TCP_copy_packet(TCP* dest, const TCP* src);

// ***********************
//  TCP CMD/TLM getter/setter
// ***********************

/*Secondary HeaderのRouteを取得*/
TCP_ROUTE TCP_get_route(const TCP* tcp);
/*Secondary HeaderのRouteを設定*/
void TCP_set_route(TCP* tcp, TCP_ROUTE route);
/*Secondary HeaderのIssuer SAT_IDを取得*/
TCP_SAT_ID TCP_get_issuer_sat_id(const TCP* tcp);
/*Secondary HeaderのIssuer SAT_IDを設定*/
void TCP_set_issuer_sat_id(TCP* tcp, TCP_SAT_ID sat_id);
/*Secondary HeaderのIssuer_IDを取得*/
TCP_EXE_ID TCP_get_issuer_id(const TCP* tcp);
/*Secondary HeaderのIssuer_IDを設定*/
void TCP_set_issuer_id(TCP* tcp, TCP_EXE_ID exe_id);
/*Secondary HeaderのIssuer_ID全体を取得*/
uint8_t TCP_get_all_issuer_id(const TCP* tcp);
/*Secondary HeaderのIssuer_ID全体を設定*/
void TCP_set_all_issuer_id(TCP* tcp, TCP_SAT_ID sat_id, TCP_EXE_ID issuer_id);


// ***********************
//  TCP CMD getter/setter
// ***********************

/**
 * @brief  Command Type を取得
 * @param  tcp: TCP
 * @return TCP_CMD_TYPE
 */
//TCP_CMD_TYPE TCP_CMD_get_type(const TCP* tcp);

/**
 * @brief  Command Type を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: TCP_CMD_TYPE
 * @return void
 */
//void TCP_CMD_set_type(TCP* tcp, TCP_CMD_TYPE type);

/**
 * @brief  channel ID を取得
 * @param  tcp: TCP
 * @return channel ID
 */
uint16_t TCP_CMD_get_channel_id(const TCP* tcp);

/**
 * @brief  channel ID を設定
 * @param[in,out] tcp: TCP
 * @param[in]     id:  channel ID
 * @return void
 */
void TCP_CMD_set_channel_id(TCP* tcp, uint16_t id);

/**
 * @brief  Response Packet ID を取得
 * @param  tcp: TCP
 * @return res_packet_id
 */
uint8_t TCP_CMD_get_res_packet_id(const TCP* tcp);

/**
 * @brief  Response Packet ID を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: id
 * @return void
 */
void TCP_CMD_set_res_packet_id(TCP* tcp, uint8_t id);

/**
 * @brief  res apid 取得
 * @param  tcp: TCP
 * @return res apid
 */
uint8_t TCP_CMD_get_res_apid_(const TCP* tcp);

/**
 * @brief  res apid を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: sat_id
 * @param[in]     type: exe_id
 * @return void
 */
void TCP_CMD_set_res_apid_(TCP* tcp, TCP_SAT_ID sat_id, TCP_EXE_ID exe_id);

/**
 * @brief  res apid sat id 取得
 * @param  tcp: TCP
 * @return res apid sat id
 */
TCP_SAT_ID TCP_CMD_get_res_apid_sat_id(const TCP* tcp);

/**
 * @brief  res apid sat id を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: sat_id
 */
void TCP_CMD_set_res_apid_sat_id(TCP* tcp, TCP_SAT_ID sat_id);

/**
 * @brief  res apid exe id 取得
 * @param  tcp: TCP
 * @return res apid exe id
 */
TCP_EXE_ID TCP_CMD_get_res_apid_executer_id(const TCP* tcp);

/**
 * @brief  res apid exe id を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: exe_id
 */
void TCP_CMD_set_res_apid_executer_id(TCP* tcp, TCP_EXE_ID exe_id);

/**
 * @brief  res route 取得
 * @param  tcp: TCP
 * @return res route
 */
TCP_ROUTE TCP_CMD_get_res_route(const TCP* tcp);

/**
 * @brief  res route を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: route
 */
void TCP_CMD_set_res_route(TCP* tcp, TCP_ROUTE route);

/**
 * @brief  EXEC_TYPE を取得
 * @param  tcp: TCP
 * @return TCP_CMD_EXEC_TYPE
 */
TCP_CMD_EXEC_TYPE TCP_CMD_get_exec_type(const TCP* tcp);

/**
 * @brief  EXEC_TYPE を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: TCP_CMD_EXEC_TYPE
 * @return void
 */
void TCP_CMD_set_exec_type(TCP* tcp, TCP_CMD_EXEC_TYPE type);

/**
 * @brief  CMD_DEST_TYPE を取得
 * @param  tcp: TCP
 * @return TCP_CMD_DEST_TYPE
 * @note   2021/01/22に TCP_CMD_EXEC_TYPE の上位4bitを拝借する形で追加実装
 */
//TCP_CMD_DEST_TYPE TCP_CMD_get_dest_type(const TCP* tcp);

/**
 * @brief  CMD_DEST_TYPE を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: TCP_CMD_DEST_TYPE
 * @return void
 * @note   2021/01/22に TCP_CMD_EXEC_TYPE の上位4bitを拝借する形で追加実装
 */
//void TCP_CMD_set_dest_type(TCP* tcp, TCP_CMD_DEST_TYPE type);

/**
 * @brief  Time Indicator (TI) を取得
 * @param  tcp: TCP
 * @return TI
 */
uint32_t TCP_CMD_get_ti(const TCP* tcp);

/**
 * @brief  Time Indicator (TI) を設定
 * @param[in,out] tcp: TCP
 * @param[in]     ti:  TI
 * @return void
 */
void TCP_CMD_set_ti(TCP* tcp, uint32_t ti);

/**
 * @brief  先頭のパラメタのポインタを取得
 * @param  tcp: TCP
 * @return *param_head
 */
const uint8_t* TCP_CMD_get_param_head(const TCP* tcp);

/**
 * @brief  パラメタを設定
 * @param[in,out] tcp:   TCP
 * @param[in]     param: パラメタ配列のポインタ
 * @param[in]     len:   パラメタ長
 * @return void
 */
void TCP_CMD_set_param(TCP* tcp, const uint8_t* param, uint16_t len);

/**
 * @brief  共通部分の Header を構築
 * @param[in,out] tcp:  TCP
 * @return void
 */
void TCP_CMD_set_common_hdr(TCP* tcp);


// ***********************
//  TCP TLM getter/setter
// ***********************

/**
 * @brief  Startup Count を取得
 * @param  tcp: TCP
 * @return Startup Count
 */
uint8_t TCP_TLM_get_startup_count(const TCP* tcp);

/**
 * @brief  Startup Count を設定
 * @param[in,out] tcp: TCP
 * @param[in]     Startup Count:  Startup Count
 * @return void
 */
void TCP_TLM_set_startup_count(TCP* tcp, uint8_t startup_count);

/**
 * @brief  Time Indicator (TI) を取得
 * @param  tcp: TCP
 * @return TI
 */
uint32_t TCP_TLM_get_ti(const TCP* tcp);

/**
 * @brief  Time Indicator (TI) を設定
 * @param[in,out] tcp: TCP
 * @param[in]     ti:  TI
 * @return void
 */
void TCP_TLM_set_ti(TCP* tcp, uint32_t ti);

/**
 * @brief  category を取得
 * @param  tcp: TCP
 * @return category
 */
uint8_t TCP_TLM_get_category(const TCP* tcp);

/**
 * @brief  category を設定
 * @param[in,out] tcp: TCP
 * @param[in]     category: category
 * @return void
 */
void TCP_TLM_set_category(TCP* tcp, uint8_t category);

/**
 * @brief  packet id を取得
 * @param  tcp: TCP
 * @return packet id
 */
uint8_t TCP_TLM_get_packet_id(const TCP* tcp);

/**
 * @brief  packet id を設定
 * @param[in,out] tcp: TCP
 * @param[in]     id:  packet id
 * @return void
 */
void TCP_TLM_set_packet_id(TCP* tcp, uint8_t id);

/**
 * @brief  TCP全体の（ Primary Header を含んだ） Packet長を取得
 * @param[in,out] tcp: TCP
 * @param[in]     len: Packet Length
 * @return void
 */
uint16_t TCP_TLM_get_packet_len(const TCP* packet);

/**
 * @brief  ADU分割なしの場合のUser Data先頭のポインタを取得
 * @param  tcp: TCP
 * @return *param_head
 * @note   この関数で取得したポインタ経由でtlmのデータをセットする事があるため，constをつけていない
 */
uint8_t* TCP_TLM_get_user_data_head(TCP* packet);


/**
 * @brief  Primary Header を構築
 * @param[in,out] tcp:  TCP
 * @param[in]     apid: TCP_APID
 * @param[in]     len:  Packet Length
 * @return void
 */
void TCP_TLM_setup_primary_hdr(TCP* tcp, TCP_APID apid, uint16_t len);

/**
 * @brief  共通部分の Header を構築
 * @param[in,out] tcp:  TCP
 * @return void
 */
void TCP_TLM_set_common_hdr(TCP* tcp);

/**
 * @brief  TCP全体を0x00クリアし，fill_sizeに等しいサイズのパケットを生成する
 * @param[in,out] tcp: TCP
 * @param[in]     fill_size: Packet size
 * @return void
 */
void TCP_TLM_setup_fill_packet(TCP* tcp, uint16_t fill_size);

#ifdef __cplusplus
 }
#endif


#endif // TC_PACKET_H_
