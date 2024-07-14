#pragma section REPRO
#include "app_registry.h"

#include <src_core/System/ApplicationManager/app_manager.h>

#include "app_headers.h"

static AM_ACK add_application_(size_t id,
                               AppInfo (*app_creator)(void));

void AR_load_initial_settings(void)
{
  /* -------- CDH ---------- */
  	/* --- NOP --- */
  	add_application_(AR_NOP, NOP_create_app);

  	/* --- CORE --- */
//  	add_application_(AR_CORE_MW_CCSDS_RX, CCSDS_MW_rx);
//  	add_application_(AR_CORE_MW_CCSDS_MS_TX, CCSDS_MW_ms_tx);
//  	add_application_(AR_CORE_DI_GSTOS_CMD_PH, GSTOS_cmd_packet_handler);
//  	add_application_(AR_CORE_DI_GSTOS_MST_PH, GSTOS_mst_packet_handler);
//  	add_application_(AR_CORE_DI_GSTOS_RPT_PH, GSTOS_rpt_packet_handler);
//  	add_application_(AR_CORE_DI_GSTOS_PRINT_TCF_INFO, GSTOS_print_tcf_info);
//  	add_application_(AR_CORE_DI_GSTOS_PRINT_TCP_INFO, GSTOS_print_tcp_info);
//  	add_application_(AR_CORE_DI_GSTOS_PRINT_CLCW_INFO, GSTOS_print_clcw_info);
  	add_application_(AR_CORE_GSC_DISPATCHER, GSCD_create_app);
  	add_application_(AR_CORE_RTC_DISPATCHER, RTCD_create_app);
	add_application_(AR_TLC_DISPATCHER_GS, TLCD_gs_create_app);
	add_application_(AR_TLC_DISPATCHER_BC, TLCD_bc_create_app);
	add_application_(AR_TLC_DISPATCHER_TLM, TLCD_tlm_create_app);

  	/* --- COREではないが各系でinitializeの優先順位が高いもの --- */
//  	add_application_(AR_ANOMALY_SETTING_PARAM_APP, AL_setting_param_update);
	  add_application_(AR_EVENT_UTILITY, EVENT_UTIL_create_app);
      add_application_(AR_MEM_DUMP, MEM_create_app);
      add_application_(AR_CDH_RESET_UTILITY, RESET_create_app);
      add_application_(AR_DBG_PACKET_HANDLER_APP, DBG_cmd_packet_handler); //noro add
      add_application_(AR_CDH_IF_INSTANCES_APP, IF_instances_update);	//kimura add
      add_application_(AR_CDH_BCT_TLM_PAGE_APP, BCT_Tlm_Page);
      add_application_(AR_CDH_ALT_BCT_APP, ALT_BCT_APP);// BCT書き換え用のアプリ Noro
      add_application_(AR_AOCS_SEPARATION_PARAM_APP, Separation_param_update);
      add_application_(AP_CDH_ECLIPSE_PARAM_APP, CDH_eclipse_param_update);

      //これはBC_PARAM_MEMORY_INIT_AOCS及びBC_PARAM_MEMORY_LOAD_AOCSには加えない
      add_application_(AR_AOCS_SEPARATION_FLAG_PARAM_APP, Separation_flag_param_update);
      add_application_(AR_CDH_PHASE_PARAM_APP, phase_param_update);

  	/* --- Anomaly --- */
    add_application_(AR_ANOMALY_COM_PARAM_APP, Com_Detect_param_load_create_app);
  	add_application_(AR_ANOMALY_UART_DETECT_APP, UART_FAULT_DETECT_create_app);
  	add_application_(AR_ANOMALY_I2C_DETECT_APP, I2C_FAULT_DETECT_create_app);
  	add_application_(AR_ANOMALY_SPI_DETECT_APP, SPI_FAULT_DETECT_create_app);
  	add_application_(AR_ANOMALY_TXPIC_SENSING_DETECT_APP, PWR_TXPIC_SENSING_DETECT_create_app);
  	add_application_(AR_ANOMALY_MODE_AUTO_TRANS_PRAM_APP, CDH_mode_param_update);
  	add_application_(AR_ANOMALY_MODE_AUTO_TRANS_APP, CDH_mode_auto_transition);

  	/* --- MEM --- */
      add_application_(AR_MEM_SD_PARAM_APP, CDH1_sd_param_update);
      add_application_(AR_MEM_SD_APP, CDH1_sd_update);
      add_application_(AR_MEM_SD_MEMORY_PATROL, CDH1_sd_memory_patrol);
      add_application_(AP_MEM_SD_COMPARE_PROGRAM_DATA,CDH1_sd_program_data);
      add_application_(AR_MEM_BD_APP, BD_create_app);
      add_application_(AR_MEM_MRAM_PARAM_APP, MRAM_param_update);
      add_application_(AR_MEM_MRAM_PATROL_APP, MRAM_memory_patorol);
      add_application_(AR_MEM_EEPROM_PARAM_APP, EEPROM_param_update);
      add_application_(AR_MEM_EEPROM_PATROL_APP, EEPROM_memory_patorol);
      add_application_(AR_MEM_SPIPROM_PARAM_APP, CDH1_spiprom_param_update);
      add_application_(AR_MEM_SPIPROM_APP, CDH1_spiprom_update);
      add_application_(AR_MEM_SPIPROM_QUEUE_SAVE_APP, CDH1_spiprom_queue_save_update);
      add_application_(AR_MEM_SPIPROM_CONDITION_SAVE_APP, CDH1_spiprom_condition_save);
      add_application_(AR_MEM_SPIPROM_OPERATION_APP, CDH1_spiprom_operation_update);
      add_application_(AR_MEM_SPIPROM_CHECK_ENABLE, CDH1_spiprom_check_ready);
      add_application_(AR_MEM_DEBUG_SRAM_APP, CDH1_debug_sram_update);
      add_application_(AR_APP_TLM_MEM_DUMP, APP_TMD_create_app);
      add_application_(AR_MEM_FLASH_PARAM_APP, CDH1_flash_param_update);

  	/* --- CDH --- */
      add_application_(AR_CDH_RTC_APP, RTC_update);
      add_application_(AR_CDH_ALLREPRO_APP, CDH_AllRepro_update);
      add_application_(AR_CDH_ALLREPRO_PARAM_APP, allrepro_param_update);
      add_application_(AR_CDH_FUNC_UPLINK_VMA_APP, CDH_FU_vma_update);
      add_application_(AR_CDH_FUNC_UPLINK_CMD_TABLE_APP, CDH_FU_cmd_table_update);
      add_application_(AR_CDH_FUNC_UPLINK_AM_AIS_APP, CDH_FU_am_ais_update);
      add_application_(AR_CDH_FUNC_UPLINK_TLM_TABLE_APP, CDH_FU_tlm_table_update);

  	/* --- DBG --- */
//      add_application_(AR_DBG_FLUSH_SCREEN, flush_screen);
//      add_application_(AR_DBG_PRINT_TIMESTAMP, print_time_stamp);
//      add_application_(AR_DBG_PRINT_CMD_STATUS, print_cmd_status);
//      add_application_(AR_DBG_PRINT_TDSP_STATUS, print_tdsp_status);
//      add_application_(AR_DBG_AH_STATUS, print_ah_status);

  /* -------- COM ---------- */
  	/* --- GS --- */
      add_application_(AR_COM_MESSAGE_PARAM_APP, COM_message_param_update);
      add_application_(AR_COM_LARGE_PACKET_ANALYZE, COM_large_packet_Analyzer);
      add_application_(AR_COM_LARGE_PACKET_PARAM_APP, COM_large_packet_param);

  	/* --- RXPIC --- */
      add_application_(AR_COM_RXPIC_DISPATCH_COMMAND, rxpic_cmd_dispatcher);
      add_application_(AR_COM_DI_RXPIC_APP, rxpic_update);
      add_application_(AR_COM_RXPIC_CLAER_COUNT, COM_rxpic_clear_count);
      add_application_(AR_COM_RXPIC_DBG_COMMAND_COUTER, COM_DBG_counter);

  	/* --- TXPIC --- */
      add_application_(AR_COM_TXPIC_DISPATCH_COMMAND, txpic_cmd_dispatcher);
      add_application_(AR_COM_DI_TXPIC_APP, txpic_update);
      add_application_(AR_COM_TXPIC_CLAER_COUNT, COM_txpic_clear_count);
      add_application_(AR_COM_TLM_MST_PACKET_HANDLER_APP, TLM_mst_packet_handler);
      add_application_(AR_COM_TLM_RPT_PACKET_HANDLER_APP, TLM_rpt_packet_handler);
      add_application_(AR_COM_TLM_TXPIC_PACKET_HANDLER_APP, TLM_txpic_packet_handler);
      add_application_(AR_COM_TLM_TXPIC_LARGEPACKET_HANDLER_APP, TLM_txpic_largepacket_handler);
      add_application_(AR_COM_COMMABD_RESPONSE_PARAM_APP, command_response_param_update);
      add_application_(AR_COM_COMMABD_RESPONSE, command_response_update);

  	/* --- LoRa --- */
      add_application_(AR_COM_LORA_DISPATCH_COMMAND, lora_cmd_dispathcer);
      add_application_(AR_COM_LORA_PARAM_APP, CDH1_lora_param_update);
      add_application_(AR_COM_LORA_RECEIVE_APP, COM_LoRa_receive_update);
      add_application_(AR_COM_DI_LORA_APP, lora_update);

    /* --- SOBC --- */
      add_application_(AR_COM_DI_SOBC_APP, sobc_update);
      add_application_(AR_COM_SOBC_DISPATCH_COMMAND, sobc_cmd_dispatcher);
      add_application_(AR_COM_SOBC_REGULAR_TLM, SOBC_Regular_Tlm);

  	/* --- COBC --- */
  #ifdef TWO_U
     add_application_(AR_COM_COBC_COMM_PARAM_APP, COBC_com_param_update);
     add_application_(AR_COM_COBC_BOOT_PARAM_APP, COBC_BOOT_param_update);
     add_application_(AR_COM_DI_COBC_APP, cobc_update);
     add_application_(AR_COM_COBC_DISPATCH_COMMAND, cobc_cmd_dispatcher);
     add_application_(AR_COM_COBC_LARGE_PACKET_RECEIVE, COBC_large_packet_receive);
     add_application_(AR_COM_COBC_REGULAR_TLM, COBC_Regular_Tlm);
  #endif

  	/* --- INTER SAT --- */
     add_application_(AR_COM_DI_INTERSAT_PARAM_APP, COM_intersat_param_update);
     add_application_(AR_COM_DI_INTERSAT_APP, COM_intersat_update);				//Temporarily Commented out by kimura
     add_application_(AR_COM_INTERSAT_DISPATCH_COMMAND, intersat_cmd_dispatcher);
     add_application_(AR_COM_INTERSAT_TLM_UPDATE, COM_intersat_tlm_update);


  /* -------- PWR ----------*/
     add_application_(AR_PWR_ADC_MOBC_APP, PWR_ADC_MOBC_update);
  //   add_application_(AR_PWR_ADC_MOBC_PARAM_APP, PWR_ADC_MOBC_Param_update);
     add_application_(AR_PWR_GPIO_INPUT_MOBC_APP, PWR_GPIO_input_MOBC_update);
     add_application_(AR_PWR_FETREG_APP, PWR_FETREG_update);
     add_application_(AR_PWR_IOEX_PWR2_APP, PWR_IOEXPANDER_update);
     add_application_(AR_PWR_ACD2_PWR2_APP, PWR_ADC_PWR2_update);
     add_application_(AR_PWR_ACD2_PWR2_PARAM_APP, PWR_ADC_PWR2_Param_update);
     add_application_(AR_PWR_ANOMALY_DETECT_APP, PWR_Anomaly_Detect_Param_update);
     add_application_(AR_PWR_ANOMALY_HANDLE_APP, PWR_ANOMALY_Handle_update);
     add_application_(AR_PWR_PARAM_INIT_APP, PWR_GLOBAL_Param_update);
     add_application_(AR_PWR_FETREG_PARAM_INIT_APP, PWR_FETREG_param_update);


  /* -------- AOCS Sensor----------*/
  	/* --- ADC --- */
  #ifdef TWO_U
     add_application_(AR_ACS_DEBUG_ADC_APP, ACS1_debug_adc_update);
     add_application_(AR_ACS_DEBUG_ADC_PARAM_APP, ACS1_param_update);
  #endif
     /* --- DAC --- */
     add_application_(AR_ACS_DAC_APP, ACS23_DAC_update);

  	/* --- GYRO --- */
  #ifdef ONE_U
     add_application_(AR_ACS_DEBUG_GYRO_APP, ACS3_debug_gyro_update);
     add_application_(AR_ACS_GYRO_PARAM_APP, ACS3_GYRO_param_update);
  #endif

  	/* --- MAGS --- */

  	/* --- SUNS --- */
     add_application_(AR_ACS_SUNS_APP, ACS13_SUNS_update);
     add_application_(AR_ACS_SUNS_PARAM_APP, ACS13_SUNS_param_update);
     add_application_(AR_ACS_PSD_APP, ACS13_PSD_create_app);

  	/* --- 9AX --- */
     add_application_(AR_ACS_9AX_APP, ACS23_9AX_update);
     add_application_(AR_ACS_9AX_PARAM_APP, ACS23_9AX_param_update);

  	/* --- GPS --- */
     add_application_(AR_ACS_GPS_APP, ACS13_GPS_update);
     add_application_(AR_ACS_GPS_PARAM_APP, ACS13_GPS_param_update);

  	/* --- MTQ --- */
     add_application_(AR_ACS_MTQ, ACS23_debug_mtq_update);
     add_application_(AR_ACS_MTQ_PARAM_APP, ACS23_MTQ_param_update);
     add_application_(AR_ACS_MTQA4_APP, ACS23_mtqa4_update);
  #ifdef TWO_U
     add_application_(AR_ACS_MTQO4_APP, ACS23_mtqo4_update);
  #endif

  	/* --- PLASMA --- */
     add_application_(AR_ACS_PLASMA_APP, ACS23_PLASMA_update);

  	/* --- SCAM --- */
  #ifdef ONE_U
     add_application_(AR_SUBCAM_APP, ACS3_SUBCAM_update);
     add_application_(AR_SUBCAM_PARAM_APP, ACS3_SUBCAM_param_update);
     add_application_(AR_SUBCAM_LARGE_PACEKT_APP, ACS3_SUBCAM_move_data);
     add_application_(AR_SUBCAM_TOA2BOA_PARAM_APP, toa2boa_param_update);

  #endif


  /* -------- AOCS Calc----------*/
  	/* --- GLOBAL --- */
     add_application_(AR_GLOBAL_AOCS_PARAM_APP, Global_AOCS_Param_update);
     add_application_(AR_GLOBAL_AOCS_FLAG_INIT_APP, Global_AOCS_flag_app);
     add_application_(AR_GLOBAL_AOCS_APP, Global_AOCS_update);
     add_application_(AR_GLOBAL_ACS_PARAM_SET_APP, ACS_Param_Set_update);
     add_application_(AR_GLOBAL_ADS_PARAM_SET_APP, ADS_Param_Set_update);
     add_application_(AR_GLOBAL_OCS_PARAM_SET_APP, OCS_Param_Set_update);
     add_application_(AR_GLOBAL_ODS_PARAM_SET_APP, ODS_Param_Set_update);
     add_application_(AR_GLOBAL_ACS_PARAM_RESET_APP, ACS_Param_Reset);
     add_application_(AR_GLOBAL_OCS_PARAM_RESET_APP, OCS_Param_Reset);
     add_application_(AR_AOCS_EKF_APP, EKF_update);
     add_application_(AR_AOCS_EKF_PARAM_APP, EKF_param_update);
     add_application_(AR_AOCS_SEPARATION_PARAM_APP,Separation_param_update);
     add_application_(AR_AOCS_TLM_PARAM_APP, AOCS_tlm_param_update);
     add_application_(AR_AOCS_SENSOR_TIME_APP, AOCS_Sensor_time_Set_update);
  #ifdef TWO_U
     add_application_(AR_AOCS_PHOTO_COBC_PARAM_APP, photo_cobc_param_update);
     add_application_(AR_AOCS_PHOTO_COBC_APP, photo_cobc_update);
     add_application_(AR_AOCS_COBC_PHOTO_FLAG_GENERATOR_APP, COBC_Photo_Flag_Generator);
  #endif/*for 2U*/

  	/* --- ACS --- */
     add_application_(AR_ACS_BDOT_PARAM_APP, Bdot_param_update);
     add_application_(AR_ACS_BDOT_APP, Bdot_update);
     add_application_(AR_ACS_BDOT_DELTA_APP, deltaB_update);
     add_application_(AR_ACS_CROSSPRODUCT_PARAM_APP, CrossProduct_param_update);
     add_application_(AR_ACS_CROSSPRODUCT_ANGULARMOMENTUM_APP, ACS_CrossProduct_AngularMomentum_update);
     add_application_(AR_ACS_CROSSPRODUCT_QUATERNION_APP, ACS_CrossProduct_Quaternion_update);
     add_application_(AR_ACS_CROSSPRODUCT_SUN_POINTING_APP, ACS_CrossProduct_Sun_Pointing_update);
     add_application_(AR_ACS_CROSSPRODUCT_EARTH_CENTER_POINTING_APP, ACS_CrossProduct_Earth_Center_Pointing_update);
     add_application_(AR_ACS_CROSSPRODUCT_EARTH_POINT_POINTING_APP, ACS_CrossProduct_Earth_Point_Pointing_update);
     add_application_(AR_ACS_CROSSPRODUCT_INERTIAL_POINTING_APP, ACS_CrossProduct_Inertial_Pointing_update);
     add_application_(AR_ACS_CROSSPRODUCT_VELOCITY_POINTING_APP, ACS_CrossProduct_Velocity_Pointing_update);
     add_application_(AR_ACS_CROSSPRODUCT_TRAJECTRY_POINTING_APP, ACS_CrossProduct_Trajectry_Pointing_update);
     add_application_(AR_ACS_CROSSPRODUCT_QFB, ACS_CrossProduct_QFB_update);
     add_application_(AR_ACS_EARTHVECTOR_PARAM_APP, EarthVector_param_update);
     add_application_(AR_ACS_EARTHVECTOR_APP, EarthVector_update);
     add_application_(AR_ACS_COMPENSATION_PARAM_APP, Compensation_param_update);
     add_application_(AR_ACS_COMPENSATION_EARTHANGULARVELOCITY_APP, ACS_Compesation_EarthAngularVelocity_update);
     add_application_(AR_ACS_COMPENSATION_COUPLING_APP, ACS_Compesation_Coupling_update);
     add_application_(AR_ACS_COMPENSATION_RMM_APP, ACS_Compesation_RMM_update);
     add_application_(AR_ACS_COMPENSATION_EDDYCURRENT_APP, ACS_Compesation_EddyCurrent_update);
     add_application_(AR_ACS_COMPENSATION_CONSTANTMAGNETIC_APP, ACS_Compesation_ConstantMagnetic_update);

  	/* --- ADS --- */
     add_application_(AR_ADS_IGRF_PARAM_APP, IGRF_param_update);
     add_application_(AR_ADS_IGRF_APP, IGRF_update);
     add_application_(AR_ADS_GST_APP, GST_update);
     add_application_(AR_ADS_SUNVECTOR_PARAM_APP, SunVector_param_update);
     add_application_(AR_ADS_SUNVECTOR_APP, SunVector_update);
     add_application_(AR_ADS_AD_EKF_APP, AD_EKF_update);
     add_application_(AR_ADS_QMETHOD_PARAM_APP, Qmethod_param_update);
     add_application_(AR_ADS_QMETHOD_APP, Qmethod_update);
     add_application_(AR_ADS_TRIAD_APP, TRIAD_update);
     add_application_(AR_ADS_QUEST_PARAM_APP, Quest_param_update);
     add_application_(AR_ADS_QUEST_APP, Quest_update);
     add_application_(AR_ADS_BBDOT_PARAM_APP, BBdot_param_update);
     add_application_(AR_ADS_BBDOT_APP, BBdot_update);


  	/* --- OCS --- */
     add_application_(AR_OCS_CROSSPRODUCT_PARAM_APP, OCS_CrossProduct_update);
     add_application_(AR_OCS_CROSSPRODUCT_AVE_DECELERATION_APP, OCS_CrossProduct_Ave_Deceleration_update);
     add_application_(AR_OCS_CROSSPRODUCT_MIN_DECELERATION_APP, OCS_CrossProduct_Min_Deceleration_update);
     add_application_(AR_OCS_CROSSPRODUCT_MAX_DECELERATION_APP, OCS_CrossProduct_Max_Deceleration_update);
     add_application_(AR_OCS_CROSSPRODUCT_KEEP_DECELERATION_APP, OCS_CrossProduct_Keep_Deceleration_update);


  	/* --- ODS --- */
     add_application_(AR_ODS_SGP4_APP, SGP4_update);
     add_application_(AR_ODS_SGP4_OPPONENT_APP, SGP4_Opponent_update);
     add_application_(AR_ODS_SGP4_PARAM_APP, SGP4_param_update);

     add_application_(AR_ODS_ORBPROP_PARAM_APP, ORBPROP_param_update);
     add_application_(AR_ODS_ORBPROP_APP, ORBPROP_update);
     add_application_(AR_ODS_ORBPROP_OPPONENT_APP, ORBPROP_Opponent_update);

     add_application_(AR_ODS_BALLCOEF_APP, BallCoef_update);
     add_application_(AR_ODS_BALLCOEF_PARAM_APP, BallCoef_param_update);
     add_application_(AR_AOCS_HILS_APP, AOCS_HILS_create_app);

     add_application_(AR_ODS_ORBEKF_PARAM_APP, ORBEKF_param_update);
     add_application_(AR_ODS_ORBEKF_APP, ORBEKF_update);


     add_application_(AR_MISSION_FLAG_APP, Global_Mission_flag_app);

//  add_application_(AR_NOP, NOP_create_app);
//  add_application_(AR_DI_GS_CMD_PH, DI_GS_cmd_packet_handler_app);
//  add_application_(AR_DI_GS_RT_TLM_PH, DI_GS_rt_tlm_packet_handler_app);
//  add_application_(AR_DI_GS_RP_TLM_PH, DI_GS_rp_tlm_packet_handler_app);
//  add_application_(AR_DI_UART_TEST, UART_TEST_update);
//  add_application_(AR_GSC_DISPATCHER, GSCD_create_app);
//  add_application_(AR_RTC_DISPATCHER, RTCD_create_app);
//  add_application_(AR_TLC_DISPATCHER_GS, TLCD_gs_create_app);
//  add_application_(AR_TLC_DISPATCHER_BC, TLCD_bc_create_app);
//  add_application_(AR_TLC_DISPATCHER_TLM, TLCD_tlm_create_app);
//#ifdef TLCD_ENABLE_MISSION_TL
//  add_application_(AR_TLC_DISPATCHER_MIS, TLCD_mis_create_app);
//#endif
//  add_application_(AR_DI_AOBC, DI_AOBC_update);
//  add_application_(AR_DI_AOBC_CDIS, DI_AOBC_cmd_dispatcher);
//  add_application_(AR_EVENT_UTILITY, EVENT_UTIL_create_app);
//  add_application_(AR_MEM_DUMP, MEM_create_app);
//  add_application_(AR_TELEMETRY_MANAGER, TLM_MGR_create_app);
//  add_application_(AR_DIVIDED_CMD_UTILITY, DCU_create_app);
//  add_application_(AR_UTILITY_CMD, UTIL_CMD_create_app);
//  // add_application_(AR_UTILITY_COUNTER, UTIL_COUNTER_create_app);
//  add_application_(AR_APP_DBG_FLUSH_SCREEN, APP_DBG_flush_screen);
//  add_application_(AR_APP_DBG_PRINT_TIMESTAMP, APP_DBG_print_time_stamp);
//  add_application_(AR_APP_DBG_PRINT_CMD_STATUS, APP_DBG_print_cmd_status);
//  add_application_(AR_APP_DBG_PRINT_EVENT_LOGGER0, APP_DBG_print_event_logger0);
//  add_application_(AR_APP_DBG_PRINT_EVENT_LOGGER1, APP_DBG_print_event_logger1);
//  add_application_(AR_APP_DBG_PRINT_EVENT_HANDLER, APP_DBG_print_event_handler);
//  add_application_(AR_APP_DBG_PRINT_GIT_REV, APP_DBG_print_git_rev);
}

static AM_ACK add_application_(size_t id,
                               AppInfo (*app_creator)(void))
{
  AppInfo ai = app_creator();
  return AM_register_ai(id, &ai);
}

#pragma section
