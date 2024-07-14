/**
 * @file
 * @brief  ブロックコマンド定義
 * @note   このコードは自動生成されています！
 */
#ifndef BLOCK_COMMAND_DEFINITIONS_H_
#define BLOCK_COMMAND_DEFINITIONS_H_

#define FIXED_BC_NUM 59

// 登録されるBlockCommandTableのblock番号を規定
typedef enum
{

//  // Block Cmds for Mode Transition (シーケンスリスト)
//  // ./src_user/Settings/Modes/Transitions/ で定義
//  BC_SL_START_UP_TO_INITIAL = 0,
//  BC_SL_NOP  = 17,
//
//  // Block Cmds for TaskList (タスクリスト) : 286-300
//  // ./src_user/Settings/Modes/TaskLists/ で定義
//  BC_TL_START_UP = 20,
//  BC_TL_INITIAL = 21,
//
//  // Block Cmds for Elements (App Rotator, Combinar)
//  // ./src_user/Settings/Modes/TaskLists/Elements/ で定義
//  BC_AR_DEBUG_DISPLAY_INI = 40,
//  BC_AR_DRIVERS_UPDATE_INI = 42,
//  BC_AR_GS_RELATED_PROCESS = 44,
//  BC_AC_TLM_CMD_HIRATE = 50,
//
//  // ==== 各系領域 ====
//  // ./C2A/TlmCmd/NormalBlockCommandDefinition/で定義
//  // イベントハンドラはBC_EH_など，接頭辞を適切につけること！
//
//  // CDH:60-79
//  BC_HK_CYCLIC_TLM = 60,
//  BC_RESERVED_FOR_HK = 77,    // EM電気試験でのコマンドファイルとのバッティングを防ぐ
//
//  // COMM:90-99
//
//  // ==== 地上からupする“のみ”領域 ====
//  // C2Aでは使用しない
//
//  // ==== 追加領域 ====
//
//  // Telemetry Manager
//  BC_TLM_MGR_MASTER = 348,
//  BC_TLM_MGR_DEPLOY = 349,
//  BC_TLM_MGR_0 = 350,
//  BC_TLM_MGR_1 = 351,
//  BC_TLM_MGR_2 = 352,
//  BC_TLM_MGR_3 = 353,
//  BC_TLM_MGR_4 = 354,
//  BC_TLM_MGR_5 = 355,
//  BC_TLM_MGR_6 = 356,
//  BC_TLM_MGR_7 = 357,
//  BC_TLM_MGR_8 = 358,
//  BC_TLM_MGR_9 = 359,
//
//  // Test
//  BC_TEST_EH_RESPOND = 360,
//  BC_TEST_BCL = 361,
//
//  // BCT MAX : 382
//  BC_ID_MAX    // BCT 自体のサイズは BCT_MAX_BLOCKS で規定

	// Block Cmds for Mode Transition
	BC_SL_STARTUP_TO_STANDBY = 0,
	BC_SL_STANDBY_TO_STARTUP,

	BC_SL_TO_STANDBY,
	BC_SL_TO_MISSION_STANDBY,
	BC_SL_TO_DAMPING,
	BC_SL_TO_SUN_POINTING,
	BC_SL_TO_EARTH_CENTER_POINTING,
	BC_SL_TO_GROUND_TRACKING,
	BC_SL_TO_INERTIAL_POINTING,
    BC_SL_TO_VELOCITY_DIRECTION_POINTING,
	BC_SL_TO_HIGH_RATE_SPIN,
	BC_SL_TO_OTHER_SATELLITE_POINTING,
	BC_SL_TO_RELATIVE_ORBITAL_CONTROL,
	BC_SL_TO_CHECK_OUT,
	BC_SL_TO_RESERVED1,
	BC_SL_TO_RESERVED2,
	BC_SL_TO_RESERVED3,
	BC_SL_TO_RESERVED4,
	BC_SL_TO_DBG_MODE,

	// Block Cmds for Fixed TaskList
	BC_TL_STARTUP,
	BC_TL_STANDBY,
	BC_TL_MISSION1,
	BC_TL_MISSION2,
	BC_TL_MISSION3,

	// Block Cmds for Combination
	BC_AC_TLM_CMD_DISPATCH,

	//Block Cmds for normal
	BC_TLM_GENERATE_AND_SAVE,
	BC_PWR_BUILT_IN_ADC,
	BC_PWR_I2C_ADC,
	BC_READ_SEPCOIL,
	BC_READ_ACS_SENSOR,
	BC_TLM_CMD_PACKET_HANDLER,
	BC_SEPARATION_ON,
	BC_SEPARATION_OFF,
	BC_TLM_ROUTING,
	BC_ANOMALY_DETECTION_HANDLER,

	// BC for anomaly
	//newly added during C2A update
	BC_AH_ComPort_RESET,
	BC_AH_ONE_BIT_COMM_YES,
	BC_AH_ONE_BIT_COMM_NO,
	BC_AH_AL_FULL,
	BC_AH_MODE_AUTO_TRANSITION,

	BC_AH_PWR,
	BC_AH_AL,
	BC_AH_AH,
	BC_AH_AH_RES,
	BC_SPIN_CONTROL_TL,
	BC_MEMORY_PATROL,
	BC_TLM_C2A,
	BC_PARAM_MEMORY_INIT_CDH,
	BC_PARAM_MEMORY_INIT_AOCS,
	BC_PARAM_MEMORY_INIT_AOCS2,
	BC_PARAM_MEMORY_LOAD_CDH,
	BC_PARAM_MEMORY_LOAD_AOCS,
	BC_PARAM_MEMORY_LOAD_AOCS2,
	BC_OBC_COMM,
	BC_Pt_ROTATE,
	BC_AFTER_SEP_PARAM_INSTANCE,
	BC_AFTER_SEP_PARAM_DELAY,
	BC_AH_SENSOR,

	//固定のBCは60こまで！！！
	//41～99個までは初期設定として利用するが，コマンドで変えれる
	//100個以上登録したい場合はApplication/DriverInsrtances/block_command_handlersのinitに関数を登録しておくこと
	//下のALTEMATE_ID,Applicationにあるにも記入すること
	/*--------------------------------------*/
	BC_AOCS_SENSOR_HANDLE=FIXED_BC_NUM+1,
	BC_ADS,
	BC_ODS,
	BC_ACS,
	BC_OCS,
	BC_ACTUATOR,
	BC_MTQ_CURVOLS_ROTATE,
	BC_TLM_EVERY,
	BC_TLM_LOW_RATE_1,
	BC_TLM_LOW_RATE_2,
	BC_AOCS,
	BC_HI_SPIN_TL,
	BC_HI_SPIN_SL,
	BC_TLM_HIGH_SPIN,
	BC_COBC_LARGE_PACKET_SET,
	BC_COBC_LARGE_PACKET_RECEIVE,
	BC_TLM_MGR_MASTER,
	BC_TLM_MGR_DEPLOY,
	BC_TLM_MGR_0,
	BC_TLM_MGR_1,
	BC_TLM_MGR_2,
	BC_TLM_MGR_3,
	BC_TLM_MGR_4,
	BC_TLM_MGR_5,
	BC_TLM_MGR_6,
	BC_TLM_MGR_7,
	BC_TLM_MGR_8,
	BC_TLM_MGR_9,

	//あとの40こは可変のBCにコマンドで入れていく形
	BC_VARIABLE1=FIXED_BC_NUM+1,
	BC_VARIABLE2,
	BC_VARIABLE3,
	BC_VARIABLE4,
	BC_VARIABLE5,
	BC_VARIABLE6,
	BC_VARIABLE7,
	BC_VARIABLE8,
	BC_VARIABLE9,
	BC_VARIABLE10,
	BC_VARIABLE11,
	BC_VARIABLE12,
	BC_VARIABLE13,
	BC_VARIABLE14,
	BC_VARIABLE15,
	BC_VARIABLE16,
	BC_VARIABLE17,
	BC_VARIABLE18,
	BC_VARIABLE19,
	BC_VARIABLE20,
	BC_VARIABLE21,
	BC_VARIABLE22,
	BC_VARIABLE23,
	BC_VARIABLE24,
	BC_VARIABLE25,
	BC_VARIABLE26,
	BC_VARIABLE27,
	BC_VARIABLE28,
	BC_VARIABLE29,
	BC_VARIABLE30,
	BC_VARIABLE31,
	BC_VARIABLE32,
	BC_VARIABLE33,
	BC_VARIABLE34,
	BC_VARIABLE35,
	BC_VARIABLE36,
	BC_VARIABLE37,
	BC_VARIABLE38,
	BC_VARIABLE39,
	BC_VARIABLE40,
	MAX_BC_NUM
} BC_DEFAULT_ID;

//　初期設定されない残りのBC群
typedef enum{
	// Block Cmds for Mode Transition
	BC_ALT_TLM_EVERY=0,
	BC_ALT_TLM_LOW_RATE_1,
	BC_ALT_TLM_LOW_RATE_2,
	BC_ALT_ADS,
	BC_ALT_ODS,
	BC_ALT_ACS,
	BC_ALT_OCS,
	BC_ALT_LARGE_PACKET,
	MAX_ALT_BC_NUM
}BC_ALTERNATE_ID;

void BC_load_defaults(void);

#endif
