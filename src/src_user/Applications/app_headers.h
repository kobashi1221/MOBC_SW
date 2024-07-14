/**
 * @file
 * @brief Appのヘッダをまとめたヘッダ
 */
#ifndef APP_HEADERS_H_
#define APP_HEADERS_H_

// Core
#include <src_core/Applications/nop.h>
#include <src_core/Applications/gs_command_dispatcher.h>
#include <src_core/Applications/realtime_command_dispatcher.h>
#include <src_core/Applications/timeline_command_dispatcher.h>
#include <src_core/Applications/event_utility.h>
#include <src_core/Applications/memory_dump.h>
#include <src_core/Applications/telemetry_manager.h>
#include <src_core/Applications/utility_command.h>
#include <src_core/Applications/utility_counter.h>
#include <src_core/Applications/divided_cmd_utility.h>

// Core TestApp
#include <src_core/Applications/TestApp/test_ccp_util.h>

// DI
#include "DriverInstances/di_aobc.h"
#include "DriverInstances/di_uart_test.h"
#include "DriverInstances/di_gs.h"

#include "DriverInstances/di_rxpic.h"
#include "DriverInstances/di_txpic.h"
#include "DriverInstances/di_cobc.h"
#include "DriverInstances/di_sobc.h"
#include "DriverInstances/di_LoRa.h"
#include "DriverInstances/di_intersat.h"
#include "DriverInstances/DI_uart_test.h"
#include "DriverInstances/tlm_packet_handler.h"
#include "DriverInstances/dbg_packet_handler.h"

// UserDefined
#include "UserDefined/debug_apps.h"

#include <src_user/Applications/UserDefined/AOCS/ACS/ACS_cmd_Bdot.h>
#include <src_user/Applications/UserDefined/AOCS/ACS/ACS_cmd_CrossProduct.h>
#include <src_user/Applications/UserDefined/AOCS/ADS/ADS_cmd_Qmethod.h>
#include <src_user/Applications/UserDefined/AOCS/ADS/ADS_cmd_Quest.h>
#include <src_user/Applications/UserDefined/AOCS/ADS/ADS_cmd_TRIAD.h>
#include <src_user/Applications/UserDefined/Cdh/telemetry_user.h>
#include <src_user/Applications/UserDefined/Com/COM_cmd_Relay.h>
#include <src_user/Applications/UserDefined/Com/COM_cmd_Response.h>
#include <src_user/Applications/UserDefined/Power/PWR_cmd_FETREG.h>
#include <src_user/Applications/UserDefined/sample_code.h>
#include <src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.h>
#include <src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.h>
#include <src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS_PSD.h>
#include "UserDefined/debug_apps.h"
#include "UserDefined/debug_nop.h"
#include "UserDefined/tlm_mem_dump.h"
#include "UserDefined/CDH/ResetUtility.h"
#include "UserDefined/CDH/RTC_handler.h"
#include "UserDefined/CDH/Communication_Fault_Detection.h"
#include "UserDefined/CDH/Repro/CDH_cmd_AllRepro.h"
#include "UserDefined/CDH/Repro/CDH_cmd_c2a_repro.h"
#include "UserDefined/CDH/Repro/CDH_cmd_patch.h"
#include "UserDefined/CDH/Developer_message.h"
#include "UserDefined/IF_Instances.h"
#include "UserDefined/Com/COM_cmd_LoRa.h"
#include "UserDefined/Com/COM_cmd_RXPIC.h"
#include "UserDefined/Com/COM_cmd_TXPIC.h"
#include "UserDefined/Com/COM_cmd_COBC.h"
#include "UserDefined/Com/COM_cmd_SOBC.h"
#include "UserDefined/Com/COM_cmd_INTERSAT.h"
#include "UserDefined/Com/COM_cmd_Large_packet.h"
#include "UserDefined/Memory/MEM_cmd_EEPROM.h"
#include "UserDefined/Memory/MEM_cmd_MRAM.h"
#ifndef HAL_SILS //temporary
#include "UserDefined/Memory/MEM_cmd_SD.h"
#endif
#include "UserDefined/Memory/MEM_cmd_SPIPROM.h"
#include "UserDefined/Memory/MEM_cmd_SRAM.h"
#include "UserDefined/Memory/MEM_cmd_FLASH.h"

#include "UserDefined/Sensor/ACS1_cmd_ADC.h"
#include "UserDefined/Sensor/ACS23_cmd_9AX.h"
#include "UserDefined/Sensor/ACS23_cmd_MTQ.h"
#include "UserDefined/Sensor/ACS3_cmd_Gyro.h"
#include "UserDefined/Sensor/ACS3_cmd_SUBCAM.h"
#include "UserDefined/Sensor/ACS23_cmd_DAC.h"
#include "UserDefined/Sensor/ACS23_cmd_PLASMA.h"
#include "src_user/Drivers/SENSOR/TOA2BOA.h"

#include "UserDefined/Power/PWR_cmd_FETREG.h"
#include "UserDefined/Power/PWR_cmd_ADC_MOBC.h"
#include "UserDefined/Power/PWR_cmd_GPIO_input_MOBC.h"
#include "UserDefined/Power/PWR_cmd_IOEX_PWR2.h"
#include "UserDefined/Power/PWR_cmd_ADC_PWR2.h"
#include "UserDefined/Power/PWR_cmd_AnomalyDetect.h"
#include "UserDefined/Power/PWR_cmd_AnomalyHandle.h"
#include <src_user/Applications/UserDefined/Global_PWR_Parameter.h>

#include "UserDefined/AOCS/ADS/ADS_cmd_IGRF.h"
#include "UserDefined/AOCS/ADS/ADS_cmd_GST.h"
#include "UserDefined/AOCS/ADS/ADS_cmd_SunVector.h"
#include "UserDefined/AOCS/ACS/ACS_cmd_EarthVector.h"
#include "UserDefined/AOCS/ADS/ADS_cmd_AD_EKF.h"
#include "UserDefined/AOCS/ODS/ODS_cmd_SGP4.h"
#include "UserDefined/AOCS/ODS/ODS_cmd_ORBPROP.h"
#include "UserDefined/AOCS/ODS/ODS_cmd_ORBEKF.h"
#include "UserDefined/AOCS/ODS/ODS_cmd_BallCoef.h"
#include "UserDefined/AOCS/Global_AOCS_Parameter.h"
#include "UserDefined/AOCS/ACS/ACS_cmd_Compensation.h"
#include "UserDefined/AOCS/OCS/OCS_cmd_CrossProduct.h"
#include "UserDefined/AOCS/Global_AOCS_Param_set.h"
#include "UserDefined/AOCS/ADS/ADS_cmd_BBdot.h"
#include "UserDefined/AOCS/AOCS_cmd_EKF.h"
#include "UserDefined/AOCS/AOCS_tlm_param.h"
#include "UserDefined/Aocs/AOCS_hils.h"
#include <src_user/Applications/UserDefined/AOCS/AOCS_cmd_Separation.h>
#include <src_user/Applications/UserDefined/AOCS/AOCS_cmd_Photo_COBC.h>



#include <src_user/Drivers/large_packet_driver.h>
#include <src_user/Library/General/DBG_printf.h>
#include "DriverInstances/block_command_handler.h"
#include "DriverInstances/mode_auto_transition.h"
#include "Middleware/MemoryMW.h"

// Setting
#include <src_user/Settings/Modes/Phase/phase_setting.h>
#include <src_user/Settings/Modes/Eqlipse/eclipse_setting.h>

#endif
