#include "phase_setting.h"

#include <stdint.h> //for uint8_t
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/ModeManager/mode_manager.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include "phase_setting_driver.h"

static PHASE_STRUCT phase_driver;
PHASE_STRUCT* const p_phase_driver = &phase_driver;

static void phase_load_param(void);
static void phase_initialize_param(void);

const uint8_t phase_redundancy1_ = 0x00;
const uint8_t phase_redundancy2_ = 0x00;
const uint8_t phase_redundancy3_ = 0x00;

AppInfo phase_param_update(void) {
  return AI_create_app_info("PHASE_PARAM_UPDATE", phase_load_param, phase_initialize_param);
}

static void phase_load_param(void) {
  read_param(&p_phase_driver->phase_redundancy1, &phase_redundancy1_, 1, (uint32_t)0x00000106);
  read_param(&p_phase_driver->phase_redundancy2, &phase_redundancy2_, 1, (uint32_t)0x00000107);
  read_param(&p_phase_driver->phase_redundancy3, &phase_redundancy3_, 1, (uint32_t)0x00000108);

  //c2a_core_mainでMM_initializeが呼び出される前に実行
  Judge_phase_flag(p_phase_driver);
}

static void phase_initialize_param(void) {
  write_param(&p_phase_driver->phase_redundancy1, &phase_redundancy1_, 1, (uint32_t)0x00000106);
  write_param(&p_phase_driver->phase_redundancy2, &phase_redundancy2_, 1, (uint32_t)0x00000107);
  write_param(&p_phase_driver->phase_redundancy3, &phase_redundancy3_, 1, (uint32_t)0x00000108);
}

//軌道上試験用(RAM上で変更)
CCP_CmdRet Cmd_chage_phase_temp(const CommonCmdPacket* packet) {
  ModeManager* mode = (ModeManager*) mode_manager;//(const cast)
  const uint8_t* param = CCP_get_param_head(packet);

  if(param[0] >= PHASE_MAX){
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  if(mode->current_id !=  MD_MODEID_STANDBY){//スタンバイ以外はフェーズ遷移できない
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  p_phase_driver->current_phase = param[0];

  MD_load_transition_table();

  // 起動直後かつモード遷移実行中ではないとして初期化
  mode->stat = MM_STATUS_FINISHED;
  mode->previous_id = MD_MODEID_STANDBY;
  mode->current_id = MD_MODEID_STANDBY;

  TCP tcp;
  MD_MODEID mode_id=MD_MODEID_STANDBY;
  CCP_form_rtc(&tcp,Cmd_CODE_MM_START_TRANSITION, &mode_id, 1);
  PH_analyze_packet(&tcp);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//フェーズの変更(しっかりMRAM パラメータから変更)
CCP_CmdRet Cmd_mode_transition_tabel_load(const CommonCmdPacket* packet){
  ModeManager* mode = (ModeManager*) mode_manager;//(const cast)
  TCP tcp;

  if(mode->current_id !=  MD_MODEID_STANDBY){//スタンバイ以外はフェーズ遷移できない
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  read_param(&p_phase_driver->phase_redundancy1, &phase_redundancy1_, 1, (uint32_t)0x00000106);
  read_param(&p_phase_driver->phase_redundancy2, &phase_redundancy2_, 1, (uint32_t)0x00000107);
  read_param(&p_phase_driver->phase_redundancy3, &phase_redundancy3_, 1, (uint32_t)0x00000108);

  Judge_phase_flag(p_phase_driver);

  MD_load_transition_table();

  mode->stat = MM_STATUS_FINISHED;
  mode->previous_id = MD_MODEID_STANDBY;
  mode->current_id = MD_MODEID_STANDBY;

  MD_MODEID mode_id=MD_MODEID_STANDBY;
  CCP_form_rtc(&tcp,Cmd_CODE_MM_START_TRANSITION, &mode_id, 1);
  PH_analyze_packet(&tcp);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
