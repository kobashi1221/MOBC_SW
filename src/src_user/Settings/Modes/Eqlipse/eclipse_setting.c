#include "eclipse_setting.h"

#include <stdint.h> //for uint8_t
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/ModeManager/mode_manager.h>
#include <src_user/Applications/Middleware/MemoryMW.h>

static void CDH_eclipse_load_param(void);
static void CDH_eclipse_initialize_param(void);

static void CDH_eclipse_init(void);
static void CDH_eclipse_exe(void);

const uint8_t pwr_eclipse_weight_ = 0x01;
const uint8_t aocs_eclipse_weight_ = 0x01;
const uint8_t time_eclipse_weight_ = 0x02;

AppInfo CDH_eclipse_param_update(void) {
  return AI_create_app_info("eclipse_PARAM_UPDATE", CDH_eclipse_load_param, CDH_eclipse_initialize_param);
}

static void CDH_eclipse_load_param(void) {
  read_param(&p_eclipse_driver->pwr_eclipse_weight, &pwr_eclipse_weight_, 1, (uint32_t)0x0000011B);
  read_param(&p_eclipse_driver->aocs_eclipse_weight, &aocs_eclipse_weight_, 1, (uint32_t)0x0000011C);
  read_param(&p_eclipse_driver->time_eclipse_weight, &time_eclipse_weight_, 1, (uint32_t)0x0000011D);

  eclipse_flag_init(p_eclipse_driver);
}

static void CDH_eclipse_initialize_param(void) {
  write_param(&p_eclipse_driver->pwr_eclipse_weight, &pwr_eclipse_weight_, 1, (uint32_t)0x0000011B);
  write_param(&p_eclipse_driver->aocs_eclipse_weight, &aocs_eclipse_weight_, 1, (uint32_t)0x0000011C);
  write_param(&p_eclipse_driver->time_eclipse_weight, &time_eclipse_weight_, 1, (uint32_t)0x0000011D);
}

/*
 * @brief   軌道上試験用(RAM上で変更)
 * param[0]: pwr_eclipseの重み
 * param[1]: aocs_eclipseの重み
 * param[2]: time_eclipseの重み
 *
*/
CCP_CmdRet Cmd_Debug_setting_eclipse_weight(const CommonCmdPacket* packet) {
  const uint8_t* param = CCP_get_param_head(packet);

  p_eclipse_driver->pwr_eclipse_weight  = param[0];
  p_eclipse_driver->aocs_eclipse_weight = param[1];
  p_eclipse_driver->time_eclipse_weight = param[2];

  //for debug
//  eclipse_flag_set(&p_eclipse_driver->pwr_eclipse_detection);
//  eclipse_flag_set(&p_eclipse_driver->aocs_eclipse_detection);
//  eclipse_flag_set(&p_eclipse_driver->time_eclipse_detection);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
