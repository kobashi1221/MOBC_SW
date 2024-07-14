#pragma section REPRO
/**
 * @file
 * @brief  Event Handler Rules of PWR
 */
#include "event_handler_rules.h"
#include <src_core/System/EventManager/event_handler.h>
#include "../../../TlmCmd/block_command_definitions.h"
#include <src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.h>

void EH_load_rule_PWR(void)
{
  EH_RuleSettings settings;

  // 暫定 2024/06/24 Kanie
  settings.event.group = EL_GROUP_OVERCURRENT;
  settings.event.local = AL_CURS_REG_CDH3V3;         // 暫定．ELに直す　2024/06/24 Kanie
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_SINGLE;
  settings.condition.count_threshold = 1;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_AH_PWR;
  settings.is_active = 1;
  EH_register_rule(EH_RULE_OC_CURS_REG_CDH3V3, &settings);

}

#pragma section
