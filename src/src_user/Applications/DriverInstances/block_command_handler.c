
#include "block_command_handler.h"
#include <string.h> //for memcpy
#include <src_core/TlmCmd/block_command_executor.h>
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/TlmCmd/block_command_table.h>
#include <src_core/System/ModeManager/mode_manager.h>
#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/Settings/Modes/mode_definitions.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_core/System/EventManager/event_logger.h>
// It should be same as bloack_command_defenition
// end
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Mission/Global_Mission_Parameter.h>
#include <src_user/Applications/UserDefined/GlobalMemoryParam.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_core/Library/endian.h>
#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>
#include <src_user/Drivers/Cdh/SD/SD.h>
#include <src_user/TlmCmd/NormalBlockCommandDefinition/nbc_header.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

void bct_overwrite_init(void);
void mode_definition_param_load(void);
void mode_definition_param_init(void);
void bct_tlm_page_init(void);
void Global_AOCS_flag_init(void);
void Global_Mission_flag_init(void);

static uint8_t check_BC_NUM(bct_id_t BC_ID);
static uint8_t check_ALT_BC_NUM(bct_id_t ALT_BC_ID);
uint8_t overload_bctable(bct_id_t BC_ID, bct_id_t ALT_BC_ID);

#define BCT_SIZE sizeof(BCT_Table)
#define SD_BCT_NUM (1200)
static uint32_t SD_SAVE_ADDRESS = 0x00002000;



static int BCT_page_no_;
const int* BCT_page_no;

static TLM_MODE_FLAG tlm_mode_flag;
const TLM_MODE_FLAG* p_tlm_mode_flag;

static USER_MODE_STRUCT user_mode_;
USER_MODE_STRUCT* const user_mode = &user_mode_;

ALT_BC_LIST alt_bc_list[40];
MODE_FLAG_STRUCT mode_flag[MD_AOCSFLAG_MAX];
const MODE_FLAG_STRUCT mode_flag_[MD_AOCSFLAG_MAX] = {
		// {PWR,}, {ACS, ADS, OCS, ODS, AOCS_DRIVER}, {PWR_MISSION, MISSION}
		{{0x00000003, NULL}, {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001}, {0x00000010, 0x00000100}},	//0:Start-up(!=MAGNAROのセーフモード)
		{{0x00000003, NULL}, {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000002}, {0x00000020, 0x00000200}},	//1:Stand-by
		{{0x00000003, NULL}, {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000003}, {0x00000030, 0x00000300}},	//2:Mission Stand-by
		{{0x00000013, NULL}, {0x0A800004, 0x00000005, 0x00000000, 0x00000020, 0x00000004}, {0x00000040, 0x00000400}},	//3:Damping
		{{0x000000D3, NULL}, {0x0A804000, 0x00000545, 0x00000000, 0x00000020, 0x00000005}, {0x00000050, 0x00000500}},	//4:Sun Pointing
		{{0x000007D7, NULL}, {0x0A870008, 0x0004E545, 0x00000000, 0x00000401, 0x00000006}, {0x00000060, 0x00000600}},	//5:Earth Center Pointing
		{{0x000007D7, NULL}, {0x0A870020, 0x001CF545, 0x00000000, 0x00000401, 0x00000007}, {0x00000070, 0x00000700}},	//6:Ground Tracking
		{{0x000007D7, NULL}, {0x0A870800, 0x007CF545, 0x00000000, 0x00000401, 0x00000008}, {0x00000080, 0x00000800}},	//7:Inertial Pointing
		{{0x000003D7, NULL}, {0x0A872000, 0x007CF545, 0x00000000, 0x00000401, 0x00000009}, {0x00000090, 0x00000900}},	//8:Velocity Direction Pointing
		{{0x000003D7, NULL}, {0x00620000, 0x00000527, 0x00000000, 0x00000030, 0x02A08545}, {0x000000a0, 0x00000a00}},	//9:High-rate Spin
		{{0x000003D7, NULL}, {0x0A870080, 0x0004E545, 0x00000000, 0x00000501, 0x0000000b}, {0x000000b0, 0x00000b00}},	//10:Other Satellite Pointing
		{{0x000003D7, NULL}, {0x0A870080, 0x0004E545, 0x00000000, 0x00000501, 0x0000000c}, {0x000000c0, 0x00000c00}},	//11:Relative Orbital Control
		{{0x00000003, NULL}, {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000d}, {0x000000d0, 0x00000d00}},	//12:Check-out
		{{0x00000003, NULL}, {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000e}, {0x000000e0, 0x00000e00}},	//13:Reserved 1
		{{0x00000003, NULL}, {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000f}, {0x000000f0, 0x00000f00}},	//14:Reserved 2
		{{0x00000003, NULL}, {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}, {0x00000000, 0x00000000}},	//15:Reserved 3
		{{0x00000003, NULL}, {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}, {0x00000000, 0x00000000}},	//16:Reserved 4
		{{0x000000ff, NULL}, {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff}, {0xffffffff, 0xffffffff}}	//17:Debug
};


static BCT_Table bct;

AppInfo ALT_BCT_APP(void)
{
    return AI_create_app_info("ATL_BC", bct_overwrite_init, NULL);
}

AppInfo Global_AOCS_Param_update(void)
{
	//create_app_info()を呼び出す．
	return AI_create_app_info("DBG", mode_definition_param_load, mode_definition_param_init); //アプリを作成
}

AppInfo BCT_Tlm_Page(void)
{
	return AI_create_app_info("BCT_Tlm_page", bct_tlm_page_init, NULL);
}

AppInfo Global_AOCS_flag_app(void)
{
	return AI_create_app_info("BCT_Tlm_page", Global_AOCS_flag_init, NULL);
}

AppInfo Global_Mission_flag_app(void)
{
    return AI_create_app_info("BCT_Tlm_page", Global_Mission_flag_init, NULL);
}

void bct_overwrite_init(void){
	//後のNullぽチェックのためNull入れておく
	for (int i=0; i<40;i++) alt_bc_list[i].func=NULL;
	alt_bc_list[BC_ALT_TLM_EVERY].func=BC_load_tlm_every;
	alt_bc_list[BC_ALT_TLM_LOW_RATE_1].func=BC_load_tlm_low_rate_1;
	alt_bc_list[BC_ALT_TLM_LOW_RATE_2].func=BC_load_tlm_low_rate_2;
	alt_bc_list[BC_ALT_ADS].func=BC_load_attitude_determination;
	alt_bc_list[BC_ALT_ODS].func=BC_load_orbit_determination;
	alt_bc_list[BC_ALT_ACS].func=BC_load_attitude_control;
	alt_bc_list[BC_ALT_OCS].func=BC_load_orbit_control;
	alt_bc_list[BC_ALT_LARGE_PACKET].func=BCL_load_cobc_large_packet_set;
}

void bct_tlm_page_init(void){
	BCT_page_no_ = 0;
	BCT_page_no = &BCT_page_no_;
}

AppInfo MODE_DEFINITION_PARAM_APP(void){
	return AI_create_app_info("MODE_DEF_PARAM", bct_overwrite_init, NULL);
}

void Global_AOCS_flag_init(void){
	printf("AOCS MODE FLAG set\r\n");
	ACS_Mode_Set(mode_flag[MD_MODEID_START_UP].aocs_mode.ACS_MODE_Number);
	ADS_Mode_Set(mode_flag[MD_MODEID_START_UP].aocs_mode.ADS_MODE_Number);
	ODS_Mode_Set(mode_flag[MD_MODEID_START_UP].aocs_mode.ODS_MODE_Number);
	OCS_Mode_Set(mode_flag[MD_MODEID_START_UP].aocs_mode.OCS_MODE_Number);
    AOCS_DRIVER_Mode_Set(mode_flag[MD_MODEID_START_UP].aocs_mode.AOCS_DRIVER_MODE_Number);
	PWR_Mode_Set(mode_flag[MD_MODEID_START_UP].pwr_mode.PWR_MODE_Number);

	p_tlm_mode_flag = &tlm_mode_flag;
	tlm_mode_flag.tlm_pwr_mode = mode_flag[MD_MODEID_START_UP].pwr_mode.PWR_MODE_Number;
	tlm_mode_flag.tlm_acs_mode = mode_flag[MD_MODEID_START_UP].aocs_mode.ACS_MODE_Number;
	tlm_mode_flag.tlm_ads_mode = mode_flag[MD_MODEID_START_UP].aocs_mode.ADS_MODE_Number;
	tlm_mode_flag.tlm_ocs_mode = mode_flag[MD_MODEID_START_UP].aocs_mode.OCS_MODE_Number;
	tlm_mode_flag.tlm_ods_mode = mode_flag[MD_MODEID_START_UP].aocs_mode.ODS_MODE_Number;
    tlm_mode_flag.tlm_aocs_driver_mode = mode_flag[MD_MODEID_START_UP].aocs_mode.AOCS_DRIVER_MODE_Number;
	user_mode->current_mode_id = MD_MODEID_START_UP;
	user_mode->aocs_trans_time = TMGR_get_master_total_cycle();
}

//mission flag初期化関数
void Global_Mission_flag_init(void){
    printf("Mission MODE FLAG set\r\n");
    PWR_Mission_Mode_Set(mode_flag[MD_MODEID_START_UP].mission_mode.PWR_MISSION_MODE_Number);
    Mission_Mode_Set(mode_flag[MD_MODEID_START_UP].mission_mode.MISSION_MODE_Number);

    p_tlm_mode_flag = &tlm_mode_flag;
    tlm_mode_flag.tlm_pwr_mission_mode = mode_flag[MD_MODEID_START_UP].mission_mode.PWR_MISSION_MODE_Number;
    tlm_mode_flag.tlm_mission_mode = mode_flag[MD_MODEID_START_UP].mission_mode.MISSION_MODE_Number;
}

void mode_definition_param_load(void){
	printf("AOCS MODE FLAG load\r\n");
	for(int i=0; i<MD_AOCSFLAG_MAX; i++){
	//Todo MRAMからロードしてね　to増田
	read_param(&mode_flag[i].aocs_mode.ACS_MODE_Number, &mode_flag_[i].aocs_mode.ACS_MODE_Number, (size_t)(sizeof(mode_flag[i].aocs_mode.ACS_MODE_Number)),(uint32_t)0x00007088 + i * sizeof(uint32_t));
	read_param(&mode_flag[i].aocs_mode.ADS_MODE_Number, &mode_flag_[i].aocs_mode.ADS_MODE_Number, (size_t)(sizeof(mode_flag[i].aocs_mode.ADS_MODE_Number)),(uint32_t)0x00005BDC + i * sizeof(uint32_t));
	read_param(&mode_flag[i].aocs_mode.OCS_MODE_Number, &mode_flag_[i].aocs_mode.OCS_MODE_Number, (size_t)(sizeof(mode_flag[i].aocs_mode.OCS_MODE_Number)),(uint32_t)0x0000A000 + i * sizeof(uint32_t));
	read_param(&mode_flag[i].aocs_mode.ODS_MODE_Number, &mode_flag_[i].aocs_mode.ODS_MODE_Number, (size_t)(sizeof(mode_flag[i].aocs_mode.ODS_MODE_Number)),(uint32_t)0x00008044 + i * sizeof(uint32_t));
	read_param(&mode_flag[i].pwr_mode.PWR_MODE_Number, &mode_flag_[i].pwr_mode.PWR_MODE_Number, (size_t)(sizeof(mode_flag[i].pwr_mode.PWR_MODE_Number)),(uint32_t)0x000046c0 + i * sizeof(uint32_t));
    read_param(&mode_flag[i].aocs_mode.AOCS_DRIVER_MODE_Number, &mode_flag_[i].aocs_mode.AOCS_DRIVER_MODE_Number, (size_t)(sizeof(mode_flag[i].aocs_mode.AOCS_DRIVER_MODE_Number)),(uint32_t)0x0000A200 + i * sizeof(uint32_t));
    read_param(&mode_flag[i].mission_mode.PWR_MISSION_MODE_Number, &mode_flag_[i].mission_mode.PWR_MISSION_MODE_Number, (size_t)(sizeof(mode_flag[i].mission_mode.PWR_MISSION_MODE_Number)),(uint32_t)0x0000A300 + i * sizeof(uint32_t));
    read_param(&mode_flag[i].mission_mode.MISSION_MODE_Number, &mode_flag_[i].mission_mode.MISSION_MODE_Number, (size_t)(sizeof(mode_flag[i].mission_mode.MISSION_MODE_Number)),(uint32_t)0x0000A348 + i * sizeof(uint32_t));

	}

}

void mode_definition_param_init(void){
	printf("AOCS MODE FLAG initialize\r\n");
	for(int i=0; i<MD_AOCSFLAG_MAX; i++){
	//Todo MRAMに書き込み　to増田
	write_param(&mode_flag[i].aocs_mode.ACS_MODE_Number, &mode_flag_[i].aocs_mode.ACS_MODE_Number, (size_t)(sizeof(mode_flag[i].aocs_mode.ACS_MODE_Number)),(uint32_t)0x00007088 + i * sizeof(uint32_t));
	write_param(&mode_flag[i].aocs_mode.ADS_MODE_Number, &mode_flag_[i].aocs_mode.ADS_MODE_Number, (size_t)(sizeof(mode_flag[i].aocs_mode.ADS_MODE_Number)),(uint32_t)0x00005BDC + i * sizeof(uint32_t));
	write_param(&mode_flag[i].aocs_mode.OCS_MODE_Number, &mode_flag_[i].aocs_mode.OCS_MODE_Number, (size_t)(sizeof(mode_flag[i].aocs_mode.OCS_MODE_Number)),(uint32_t)0x0000A000 + i * sizeof(uint32_t));
	write_param(&mode_flag[i].aocs_mode.ODS_MODE_Number, &mode_flag_[i].aocs_mode.ODS_MODE_Number, (size_t)(sizeof(mode_flag[i].aocs_mode.ODS_MODE_Number)),(uint32_t)0x00008044 + i * sizeof(uint32_t));
	write_param(&mode_flag[i].pwr_mode.PWR_MODE_Number, &mode_flag_[i].pwr_mode.PWR_MODE_Number, (size_t)(sizeof(mode_flag[i].pwr_mode.PWR_MODE_Number)),(uint32_t)0x000046c0 + i * sizeof(uint32_t));
    write_param(&mode_flag[i].aocs_mode.AOCS_DRIVER_MODE_Number, &mode_flag_[i].aocs_mode.AOCS_DRIVER_MODE_Number, (size_t)(sizeof(mode_flag[i].aocs_mode.AOCS_DRIVER_MODE_Number)),(uint32_t)0x0000A200 + i * sizeof(uint32_t));
    write_param(&mode_flag[i].mission_mode.PWR_MISSION_MODE_Number, &mode_flag_[i].mission_mode.PWR_MISSION_MODE_Number, (size_t)(sizeof(mode_flag[i].mission_mode.PWR_MISSION_MODE_Number)),(uint32_t)0x0000A300 + i * sizeof(uint32_t));
    write_param(&mode_flag[i].mission_mode.MISSION_MODE_Number, &mode_flag_[i].mission_mode.MISSION_MODE_Number, (size_t)(sizeof(mode_flag[i].mission_mode.MISSION_MODE_Number)),(uint32_t)0x0000A348 + i * sizeof(uint32_t));
	}
}


//CCP_CmdRet SD_combine_block_cmd(const CommonCmdPacket* packet)
//{
//  const uint8_t *param = CCP_get_param_head(packet);
//  uint16_t sd_bcid;
//  uint16_t sd_bcid2;
//  uint8_t cmd_num;
////  size_t Datasize = CCP_get_param_len(packet) - 2;
//  uint8_t BC_Layer_Max_NUM = 4;
//  TCP TCP;
//  memcpy(sd_bcid, param[0], 2);
//
//  if(sd_bcid > SD_BCT_NUM)  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
//  SD_read_param(&bct, NULL, BCT_SIZE, SD_SAVE_ADDRESS + BCT_SIZE * sd_bcid);
//  if(sd_bcid > 200){
//    for(cmd_num = 0; cmd_num < bct.length; cmd_num ++){
//      TCP.packet = bct.cmds[cmd_num].data;
//      cmd_ret = PH_dispatch_command(&TCP);
//    }
//  }
//  else if(sd_bcid <= 200){
//    for(cmd_num = 0; cmd_num < bct.length; cmd_num ++){
//      TCP.packet = bct.cmds[cmd_num].data;
//      if(CCP_get_id(TCP) == 0x00fe || CCP_get_id(TCP) == 0x00ff){
//        const uint8_t *param2 = CCP_get_param_head(TCP);
//        memcpy(sd_bcid2, param2[0], 2);
//        cmd_ret = PH_dispatch_command(&TCP);
//      }
//      else{
//        cmd_ret = PH_dispatch_command(&TCP);
//      }
//    }
//  }
//}

/**
 * @brief SDからブロックコマンドを読み出してcombine実行するコマンド.コマンド実行の入れ子構造になっている．大規模なBCは
 * タスクが止まるため実行非推奨
 * @packet[15~16]:SD上のBCID(0~1200)
 * @date：2024/06/27
 * @author： kobayashi
 * @return なし
 */
CCP_CmdRet Cmd_SD_combine_block_cmd(const CommonCmdPacket* packet)
{
  const uint8_t *param = CCP_get_param_head(packet);
  uint16_t sd_bcid;
  uint8_t cmd_num;
  TCP TCP;
  ENDIAN_memcpy(&sd_bcid, param, 2);

  if(sd_bcid > SD_BCT_NUM)  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  
  SD_read_param(&bct, NULL, BCT_SIZE, SD_SAVE_ADDRESS + BCT_SIZE * sd_bcid);
  for(cmd_num = 0; cmd_num < bct.length; cmd_num ++){
    memcpy(&TCP, &bct.cmds[cmd_num].data, BCT_CMD_MAX_LENGTH);
    CCP_CmdRet cmd_ret = PH_dispatch_command(&TCP);
    if (cmd_ret.exec_sts != CCP_EXEC_SUCCESS) return cmd_ret;
  }
  printf("combine block command ID %d\r\n", sd_bcid);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/**
 * @brief SDからブロックコマンドを読み出してdeploy実行するコマンド.
 * @packet[15]:TLCD_ID(0~3)
 * @packet[16~17]:SD上のBCID(0~1200)
 * @date：2024/06/27
 * @author： kobayashi
 * @return なし
 */
CCP_CmdRet Cmd_SD_deploy_block_cmd(const CommonCmdPacket* packet)
{
  const uint8_t *param = CCP_get_param_head(packet);
  uint16_t sd_bcid;
  TLCD_ID id = (TLCD_ID)CCP_get_param_from_packet(packet, 0, uint8_t);
  ENDIAN_memcpy(&sd_bcid, param+1, 2);
  // 指定されたライン番号が存在しない場合は異常判定
  if (id >= TLCD_ID_MAX)     return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  if (sd_bcid > SD_BCT_NUM)  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  PL_ACK ack = PL_SD_deploy_block_cmd(&(PH_tl_cmd_list[id]), sd_bcid, TMGR_get_master_total_cycle());
  if (ack != PL_SUCCESS)
  {
    EL_record_event((EL_GROUP)EL_CORE_GROUP_TLCD_DEPLOY_BLOCK,
                    (uint32_t)ack,
                    EL_ERROR_LEVEL_LOW,
                    (uint32_t)( ((0x000000ff & id) << 24) | (0x00ffffff & sd_bcid) ));
    if (ack == PL_BC_TIME_ADJUSTED)
    {
      return CCP_make_cmd_ret(CCP_EXEC_SUCCESS, (uint32_t)ack);
    }
    else
    {
      return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, (uint32_t)ack);
    }
  }
  printf("deploy block command ID %d\r\n", sd_bcid);
  return CCP_make_cmd_ret(CCP_EXEC_SUCCESS, (uint32_t)ack);
}


/**
 * @brief SDからブロックコマンドを読み出してタイムラインに登録していく関数.タイムラインは基本TLCD_ID_DEPLOY_BCを使用予定
 * @packet[15]:TLCD_ID(0~3)
 * @packet[16~17]:SD上のBCID(0~1200)
 * @date：2024/06/27
 * @author： kobayashi
 * @return なし
 */
PL_ACK PL_SD_deploy_block_cmd(PacketList* pl, const bct_id_t sd_bcid, cycle_t start_at){
  uint8_t i;
  uint16_t j;
  int is_cleared = 0; // リスト強制クリアの記録用変数
  uint32_t adj = 0;   // 時刻調整の累積量保存用変数
  uint8_t bc_length;

  if (pl->packet_type_ != PL_PACKET_TYPE_CCP) return PL_PACKET_TYPE_ERR;

  if (sd_bcid >= SD_BCT_NUM) return PL_BC_INACTIVE_BLOCK;

  SD_read_param(&bct, NULL, BCT_SIZE, SD_SAVE_ADDRESS + BCT_SIZE * sd_bcid);

  // リストにブロック全体を登録する余裕がない場合
  if (PL_count_inactive_nodes(pl) < bct.length)
  {
    // リストをクリアし強制的に空き領域を確保する
    PL_clear_list(pl);
    is_cleared = 1;
  }

  for (i = 0; i < bct.length; ++i)
  {
    static CommonCmdPacket temp_; // サイズが大きいため静的領域に確保
    BCT_Pos pos;
    PL_ACK ack = PL_SUCCESS;

    // コマンドを読みだし、TLCとして実行時刻を設定
//    BCT_make_pos(&pos, block, i);
//    BCT_load_cmd(&pos, &temp_);
    memcpy(&temp_, &bct.cmds[i].data, BCT_CMD_MAX_LENGTH);
    CCP_set_ti(&temp_, (cycle_t)(start_at + adj + CCP_get_ti(&temp_)));
    CCP_set_exec_type(&temp_, CCP_EXEC_TYPE_TL_FROM_GS); // BLC -> TLC   // FIXME: TaskListやBC用TLM用もすべて CCP_EXEC_TYPE_TL_FROM_GS になってしまうので, わかりにくい

    for (j = 0; j <= pl->active_nodes_; ++j)
    {
      // コマンドをTLCに登録を試みる
      ack = PL_insert_tl_cmd(pl, &temp_, start_at);
      if (ack != PL_TLC_ALREADY_EXISTS) break;    // PL_SUCCESS なはず． TODO: 一応 event 発行しておく？

      // 同一時刻で既に登録されていた場合は時刻をずらして再登録
      CCP_set_ti(&temp_, CCP_get_ti(&temp_) + 1);
      ++adj; // 累積調整時間を更新する
    }
    if (ack != PL_SUCCESS) return ack;
  }

  // リストの強制クリアを実施した場合
  if (is_cleared == 1) return PL_BC_LIST_CLEARED;
  // 時刻調整を行った場合
  if (adj != 0) return PL_BC_TIME_ADJUSTED;

  return PL_SUCCESS;
}


/**
 * @brief MOBC起動後のinitialize時にSDから指定のBCをBC_Table(RAM)に読み込む
 * @packet なし
 * @date：2024/06/27
 * @author： kobayashi
 * @return なし
 */
void BCT_SD_initialize(void){
    uint8_t bc_id;

    for(bc_id = 0; bc_id < BCT_MAX_BLOCKS; bc_id ++){
      BCT_Table *block = block_command_table->blocks[bc_id];
      //現在タイムラインに設定されているモードは変更しないほうがいい
      BCE_inactivate_block_by_id(bc_id);
      SD_read_param(&bct, NULL, BCT_SIZE, SD_SAVE_ADDRESS + BCT_SIZE * bc_id);
      memcpy(block, &bct, sizeof(BCT_Table));
      BCE_activate_block_by_id(bc_id);
    }
}


//タイムラインの変更はここでは行わず，
//CombineやRotateで使われるBCの60~100番までを書き換える．
CCP_CmdRet Cmd_BCT_CHANGE_ALT_BC(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);
	bct_id_t BC_ID, ALT_BC_ID;
	//BC_IDはBC_VARIABLES1~40までを指す
	//ALT_BC_IDはそこに入れたいBCを指す
	ENDIAN_memcpy(&BC_ID,param,2);
	if (check_BC_NUM(BC_ID)){//アノマリー
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}
	ENDIAN_memcpy(&ALT_BC_ID,param+2,2);
	if (check_ALT_BC_NUM(ALT_BC_ID)){//アノマリー
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	}
	overload_bctable(BC_ID, ALT_BC_ID);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//Todo 電源モードもここで更新できるようにするとよいと思う
CCP_CmdRet Cmd_SAME_TL_MODE_TRANS(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);
	uint16_t mode;
	ENDIAN_memcpy(&mode,param,2);

	printf("mode: %d Number: %08x %08x %08x %08x %08x %08x %08x %08x\r\n", mode,
			mode_flag[mode].pwr_mode.PWR_MODE_Number,
			mode_flag[mode].aocs_mode.ADS_MODE_Number,
			mode_flag[mode].aocs_mode.ACS_MODE_Number,
			mode_flag[mode].aocs_mode.ODS_MODE_Number,
			mode_flag[mode].aocs_mode.OCS_MODE_Number,
			mode_flag[mode].aocs_mode.AOCS_DRIVER_MODE_Number,
			mode_flag[mode].mission_mode.PWR_MISSION_MODE_Number,
			mode_flag[mode].mission_mode.MISSION_MODE_Number);

	ACS_Mode_Set(mode_flag[mode].aocs_mode.ACS_MODE_Number);
	ADS_Mode_Set(mode_flag[mode].aocs_mode.ADS_MODE_Number);
	ODS_Mode_Set(mode_flag[mode].aocs_mode.ODS_MODE_Number);
	OCS_Mode_Set(mode_flag[mode].aocs_mode.OCS_MODE_Number);
	AOCS_DRIVER_Mode_Set(mode_flag[mode].aocs_mode.AOCS_DRIVER_MODE_Number);
    PWR_Mission_Mode_Set(mode_flag[mode].mission_mode.PWR_MISSION_MODE_Number);
	Mission_Mode_Set(mode_flag[mode].mission_mode.MISSION_MODE_Number);
	PWR_Mode_Set(mode_flag[mode].pwr_mode.PWR_MODE_Number);
	PWR_Mode_Trans();//電源の反映　AOCSより先にやる

	//BCTableの該当する番号のところを丸ごと書き換え⇒IF文でタスクの登録がされるかどうかが決まる Noro
	overload_bctable(BC_ACS,BC_ALT_ACS);
	overload_bctable(BC_ADS,BC_ALT_ADS);
	overload_bctable(BC_OCS,BC_ALT_OCS);
	overload_bctable(BC_ODS,BC_ALT_ODS);
	//TODO 小林　ミッションフラグによるBCの上書きを追加 2024/06/21

	tlm_mode_flag.tlm_pwr_mode = mode_flag[mode].pwr_mode.PWR_MODE_Number;
	tlm_mode_flag.tlm_acs_mode = mode_flag[mode].aocs_mode.ACS_MODE_Number;
	tlm_mode_flag.tlm_ads_mode = mode_flag[mode].aocs_mode.ADS_MODE_Number;
	tlm_mode_flag.tlm_ocs_mode = mode_flag[mode].aocs_mode.OCS_MODE_Number;
	tlm_mode_flag.tlm_ods_mode = mode_flag[mode].aocs_mode.ODS_MODE_Number;
    tlm_mode_flag.tlm_aocs_driver_mode = mode_flag[mode].aocs_mode.AOCS_DRIVER_MODE_Number;
    tlm_mode_flag.tlm_pwr_mission_mode = mode_flag[mode].mission_mode.PWR_MISSION_MODE_Number;
    tlm_mode_flag.tlm_mission_mode = mode_flag[mode].mission_mode.MISSION_MODE_Number;


	user_mode->current_mode_id = mode;
	user_mode->aocs_trans_time = TMGR_get_master_total_cycle();

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_SAVE_BCT(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);
	uint16_t bc_id;
	uint32_t SD_saved_id;

	ENDIAN_memcpy(&bc_id,param,2);
	ENDIAN_memcpy(&SD_saved_id,param+2,4);

	if(bc_id >= BCT_MAX_BLOCKS) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	if(SD_saved_id >= SD_BCT_NUM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);

	BCT_Table *block = block_command_table->blocks[bc_id];
	SD_write_param(block, NULL, BCT_SIZE, SD_SAVE_ADDRESS + BCT_SIZE * SD_saved_id);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_LOAD_BCT(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);
	uint16_t bc_id;
	uint32_t SD_saved_id;

	ENDIAN_memcpy(&bc_id, param, 2);
	ENDIAN_memcpy(&SD_saved_id, param + 2, 4);

	if(bc_id >= BCT_MAX_BLOCKS) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
	if(SD_saved_id >= SD_BCT_NUM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);

	BCT_Table *block = block_command_table->blocks[bc_id];
	//現在タイムラインに設定されているモードは変更しないほうがいい
	if(mode_manager->mode_list[mode_manager->current_id]==bc_id){
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	BCE_inactivate_block_by_id(bc_id);
	int ret=SD_read_param(&bct, NULL, BCT_SIZE, SD_SAVE_ADDRESS + BCT_SIZE * SD_saved_id);
	memcpy(block, &bct, sizeof(BCT_Table));
	BCE_activate_block_by_id(bc_id);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BCT_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet)
{
  uint8_t page;

  page = CCP_get_param_head(packet)[0];

  if (page >= BCT_TLM_PAGE_MAX)
  {
    // ページ番号がコマンドテーブル範囲外
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  BCT_page_no_ = page;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BCT_SD_SET_CMD_Packet(const CommonCmdPacket* packet)
{
	uint8_t* param =  CCP_get_param_head(packet);
	uint16_t SD_saved_id;
	uint8_t cmd_pos = param[2];
	uint8_t len = CCP_get_param_len(packet) - 3;	//保存するコマンドの長さ

	ENDIAN_memcpy(&SD_saved_id, param, 2);

	if(cmd_pos >= BCT_MAX_CMD_NUM){
		return  CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	if(SD_saved_id >= SD_BCT_NUM){
		return  CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	if(len > BCT_CMD_MAX_LENGTH){
		return  CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

	SD_write_param(param + 3, NULL, len, SD_SAVE_ADDRESS + BCT_SIZE * SD_saved_id + sizeof(uint8_t) + BCT_CMD_MAX_LENGTH * cmd_pos);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BCT_SD_SET_CMD(const CommonCmdPacket* packet)
{
	uint8_t* param =  CCP_get_param_head(packet);
	uint16_t SD_saved_id;
	uint8_t cmd_pos = param[2];
	uint8_t len = CCP_get_param_len(packet) - 9;	//保存するコマンドの長さ
	TCP save_packet;
	cycle_t ti;
	CMD_CODE cmd_id;

	ENDIAN_memcpy(&SD_saved_id, param, 2);
	ENDIAN_memcpy(&ti, param + 3, 4);
	ENDIAN_memcpy(&cmd_id, param + 7, 2);

	if(cmd_pos >= BCT_MAX_CMD_NUM){
		return  CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	if(SD_saved_id >= SD_BCT_NUM){
		return  CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

	CCP_form_tlc(&save_packet, ti, cmd_id, param + 9, len);

	SD_write_param(&save_packet, NULL, TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN + len,
			        SD_SAVE_ADDRESS + BCT_SIZE * SD_saved_id + sizeof(uint8_t) + BCT_CMD_MAX_LENGTH * cmd_pos);

	printf("command registration done BCID = %d, cmd_pos = %d, cmd_id = %d\r\n", SD_saved_id, cmd_pos, cmd_id);
	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_BCT_SD_SET_CMD_NUM(const CommonCmdPacket* packet)
{
	uint8_t* param =  CCP_get_param_head(packet);
	uint16_t SD_saved_id;
	uint8_t cmd_num = param[2];

	ENDIAN_memcpy(&SD_saved_id, param, 2);

	if(cmd_num >= BCT_MAX_CMD_NUM){
		return  CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}
	if(SD_saved_id >= SD_BCT_NUM){
		return  CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

	SD_write_param(param + 2, NULL, sizeof(uint8_t), SD_SAVE_ADDRESS + BCT_SIZE * SD_saved_id);

    printf("registration done BCID = %d, length = %d\r\n", SD_saved_id,cmd_num);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}




PWR_STATE_STRUCT PWR_MODE;
AOCS_MODE_STRUCT AOCS_MODE;
//個別にAOCSフラグを切り替えるコマンド
CCP_CmdRet Cmd_CAHNGE_MODEFLAG_SEPARATELY(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);

	//todo チェックを増やす

	ENDIAN_memcpy(&PWR_MODE.PWR_MODE_Number, param , 4);
	ENDIAN_memcpy(&AOCS_MODE.ACS_MODE_Number, param + 4, 4);
	ENDIAN_memcpy(&AOCS_MODE.ADS_MODE_Number, param + 8, 4);
	ENDIAN_memcpy(&AOCS_MODE.OCS_MODE_Number, param + 12, 4);
	ENDIAN_memcpy(&AOCS_MODE.ODS_MODE_Number, param + 16, 4);


	printf("mode: Number: %08x %08x %08x %08x %08x\r\n",
			PWR_MODE.PWR_MODE_Number,
			AOCS_MODE.ACS_MODE_Number,
			AOCS_MODE.ADS_MODE_Number,
			AOCS_MODE.OCS_MODE_Number,
			AOCS_MODE.ODS_MODE_Number);

	ACS_Mode_Set(AOCS_MODE.ACS_MODE_Number);
	ADS_Mode_Set(AOCS_MODE.ADS_MODE_Number);
	ODS_Mode_Set(AOCS_MODE.ODS_MODE_Number);
	OCS_Mode_Set(AOCS_MODE.OCS_MODE_Number);
	PWR_Mode_Set(PWR_MODE.PWR_MODE_Number);
	PWR_Mode_Trans();//電源の反映　AOCSより先にやる

	overload_bctable(BC_ACS,BC_ALT_ACS);
	overload_bctable(BC_ADS,BC_ALT_ADS);
	overload_bctable(BC_OCS,BC_ALT_OCS);
	overload_bctable(BC_ODS,BC_ALT_ODS);

	tlm_mode_flag.tlm_pwr_mode = PWR_MODE.PWR_MODE_Number;
	tlm_mode_flag.tlm_acs_mode = AOCS_MODE.ACS_MODE_Number;
	tlm_mode_flag.tlm_ads_mode = AOCS_MODE.ADS_MODE_Number;
	tlm_mode_flag.tlm_ocs_mode = AOCS_MODE.OCS_MODE_Number;
	tlm_mode_flag.tlm_ods_mode = AOCS_MODE.ODS_MODE_Number;

	user_mode->aocs_trans_time = TMGR_get_master_total_cycle();

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_CAHNGE_ACS_MODEFLAG(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);

	//todo チェックを増やす

	ENDIAN_memcpy(&PWR_MODE.PWR_MODE_Number, param , 4);
	ENDIAN_memcpy(&AOCS_MODE.ACS_MODE_Number, param + 4, 4);

	printf("mode: Number: %08x %08x %08x %08x %08x\r\n",
			PWR_MODE.PWR_MODE_Number,
			AOCS_MODE.ACS_MODE_Number,
			AOCS_MODE.ADS_MODE_Number,
			AOCS_MODE.OCS_MODE_Number,
			AOCS_MODE.ODS_MODE_Number);

	ACS_Mode_Set(AOCS_MODE.ACS_MODE_Number);
	PWR_Mode_Set(PWR_MODE.PWR_MODE_Number);
	PWR_Mode_Trans();//電源の反映　AOCSより先にやる

	overload_bctable(BC_ACS,BC_ALT_ACS);

	tlm_mode_flag.tlm_pwr_mode = PWR_MODE.PWR_MODE_Number;
	tlm_mode_flag.tlm_acs_mode = AOCS_MODE.ACS_MODE_Number;
	tlm_mode_flag.tlm_ads_mode = AOCS_MODE.ADS_MODE_Number;
	tlm_mode_flag.tlm_ocs_mode = AOCS_MODE.OCS_MODE_Number;
	tlm_mode_flag.tlm_ods_mode = AOCS_MODE.ODS_MODE_Number;

	user_mode->aocs_trans_time = TMGR_get_master_total_cycle();

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_CAHNGE_ADS_MODEFLAG(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);

	//todo チェックを増やす

	ENDIAN_memcpy(&PWR_MODE.PWR_MODE_Number, param , 4);
	ENDIAN_memcpy(&AOCS_MODE.ADS_MODE_Number, param + 4, 4);

	printf("mode: Number: %08x %08x %08x %08x %08x\r\n",
			PWR_MODE.PWR_MODE_Number,
			AOCS_MODE.ACS_MODE_Number,
			AOCS_MODE.ADS_MODE_Number,
			AOCS_MODE.OCS_MODE_Number,
			AOCS_MODE.ODS_MODE_Number);

	ADS_Mode_Set(AOCS_MODE.ADS_MODE_Number);
	PWR_Mode_Set(PWR_MODE.PWR_MODE_Number);
	PWR_Mode_Trans();//電源の反映　AOCSより先にやる

	overload_bctable(BC_ADS,BC_ALT_ADS);

	tlm_mode_flag.tlm_pwr_mode = PWR_MODE.PWR_MODE_Number;
	tlm_mode_flag.tlm_acs_mode = AOCS_MODE.ACS_MODE_Number;
	tlm_mode_flag.tlm_ads_mode = AOCS_MODE.ADS_MODE_Number;
	tlm_mode_flag.tlm_ocs_mode = AOCS_MODE.OCS_MODE_Number;
	tlm_mode_flag.tlm_ods_mode = AOCS_MODE.ODS_MODE_Number;

	user_mode->aocs_trans_time = TMGR_get_master_total_cycle();

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_CAHNGE_OCS_MODEFLAG(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);

	//todo チェックを増やす

	ENDIAN_memcpy(&PWR_MODE.PWR_MODE_Number, param , 4);
	ENDIAN_memcpy(&AOCS_MODE.OCS_MODE_Number, param + 4, 4);

	printf("mode: Number: %08x %08x %08x %08x %08x\r\n",
			PWR_MODE.PWR_MODE_Number,
			AOCS_MODE.ACS_MODE_Number,
			AOCS_MODE.ADS_MODE_Number,
			AOCS_MODE.OCS_MODE_Number,
			AOCS_MODE.ODS_MODE_Number);

	OCS_Mode_Set(AOCS_MODE.OCS_MODE_Number);
	PWR_Mode_Set(PWR_MODE.PWR_MODE_Number);
	PWR_Mode_Trans();//電源の反映　AOCSより先にやる

	overload_bctable(BC_OCS,BC_ALT_OCS);

	tlm_mode_flag.tlm_pwr_mode = PWR_MODE.PWR_MODE_Number;
	tlm_mode_flag.tlm_acs_mode = AOCS_MODE.ACS_MODE_Number;
	tlm_mode_flag.tlm_ads_mode = AOCS_MODE.ADS_MODE_Number;
	tlm_mode_flag.tlm_ocs_mode = AOCS_MODE.OCS_MODE_Number;
	tlm_mode_flag.tlm_ods_mode = AOCS_MODE.ODS_MODE_Number;

	user_mode->aocs_trans_time = TMGR_get_master_total_cycle();

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_CAHNGE_ODS_MODEFLAG(const CommonCmdPacket* packet){
	const uint8_t *param = CCP_get_param_head(packet);

	//todo チェックを増やす

	ENDIAN_memcpy(&PWR_MODE.PWR_MODE_Number, param , 4);
	ENDIAN_memcpy(&AOCS_MODE.ODS_MODE_Number, param + 4, 4);

	printf("mode: Number: %08x %08x %08x %08x %08x\r\n",
			PWR_MODE.PWR_MODE_Number,
			AOCS_MODE.ACS_MODE_Number,
			AOCS_MODE.ADS_MODE_Number,
			AOCS_MODE.OCS_MODE_Number,
			AOCS_MODE.ODS_MODE_Number);

	ODS_Mode_Set(AOCS_MODE.ODS_MODE_Number);
	PWR_Mode_Set(PWR_MODE.PWR_MODE_Number);
	PWR_Mode_Trans();//電源の反映　AOCSより先にやる

	overload_bctable(BC_ODS,BC_ALT_ODS);

	tlm_mode_flag.tlm_pwr_mode = PWR_MODE.PWR_MODE_Number;
	tlm_mode_flag.tlm_acs_mode = AOCS_MODE.ACS_MODE_Number;
	tlm_mode_flag.tlm_ads_mode = AOCS_MODE.ADS_MODE_Number;
	tlm_mode_flag.tlm_ocs_mode = AOCS_MODE.OCS_MODE_Number;
	tlm_mode_flag.tlm_ods_mode = AOCS_MODE.ODS_MODE_Number;

	user_mode->aocs_trans_time = TMGR_get_master_total_cycle();

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
//TODO　ミッションフラグの書き換えコマンド実装　2024/06/21

/**************以降は内部関数*****************/
static uint8_t check_BC_NUM(bct_id_t BC_ID){
	return (BC_ID>MAX_BC_NUM) || (BC_ID<FIXED_BC_NUM);
}

static uint8_t check_ALT_BC_NUM(bct_id_t ALT_BC_ID){
	return ALT_BC_ID>MAX_ALT_BC_NUM;
}

uint8_t overload_bctable(bct_id_t BC_ID, bct_id_t ALT_BC_ID){
	//	BCT_clear_pos_();　あった方がいいけど，外部から読めない
	BCE_inactivate_block_by_id(BC_ID);//上書きする前に一応非アクティブに
	//BC_VARIABLES1以降のBCなら書き換え
	if(!(alt_bc_list[ALT_BC_ID].func==NULL)){
		BCL_load_bc(BC_ID,  alt_bc_list[ALT_BC_ID].func);
		return 0;
	}
	else return 1;
}
