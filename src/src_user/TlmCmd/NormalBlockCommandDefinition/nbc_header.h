#ifndef NBC_HEADER_H_
#define NBC_HEADER_H_

#include <src_core/TlmCmd/block_command_table.h>

#ifdef TWO_U
#define AFSK_GS 0x0122
#endif
#ifdef ONE_U
#define AFSK_GS 0x0132
#endif

// TL
void BCL_load_startup_mode(void);
void BCL_load_standby_mode(void);
void BCL_load_mission1_mode(void);
void BCL_load_mission2_mode(void);
void BCL_load_mission3_mode(void);
void BCL_load_reserved1_mode(void);
void BCL_load_reserved2_mode(void);
void BCL_load_reserved3_mode(void);
void BCL_load_reserved4_mode(void);
void BCL_load_hispin_mode(void);
void BLC_load_separation_mode(void);
void BLC_load_sep_damping_mode(void);

// SL
void BCL_load_nop_sl(void);
void BCL_load_startup_to_standby(void);
void BCL_load_standby_to_startup(void);
void BCL_load_standby_to_standby(void);

void BCL_load_to_standby(void);
void BCL_load_to_mission_standby(void);
void BCL_load_to_damping(void);
void BCL_load_to_sun_pointing(void);
void BCL_load_to_earth_center_pointing(void);
void BCL_load_to_ground_tracking(void);
void BCL_load_to_inertial_pointing(void);
void BCL_load_to_velocity_direction_pointing(void);
void BCL_load_to_hispin(void);
void BCL_load_to_other_satellite_pointing(void);
void BCL_load_to_relative_orbital_ontrol(void);
void BCL_load_to_checkout(void);
void BCL_load_to_reserved1(void);
void BCL_load_to_reserved2(void);
void BCL_load_to_reserved3(void);
void BCL_load_to_reserved4(void);

void BCL_load_to_debug(void);

void BLC_load_to_sepdamping(void);
void BCL_load_tlm_cmd_dispatch(void);

// NormalBlockCommand
void BC_load_ah_al(void);
void BC_load_ah_ah(void);
void BC_load_ah_ah_res(void);
void BC_load_ah_al_full(void);
void BC_load_AH_PWR(void);
void BC_load_AH_MODE_Auto_Trans(void);
void BC_load_AH_ComPort_RESET(void);
void BC_load_anomaly_detection_handler(void);
void BC_load_AH_SENSOR(void);

void BC_load_param_init_sensor_aocs(void);
void BC_load_param_init_sensor_aocs2(void);
void BC_load_param_init_cdh_com(void);
void BC_load_param_load_sensor_aocs(void);
void BC_load_param_load_sensor_aocs2(void);
void BC_load_param_load_cdh_com(void);

void BC_load_sensor_handler(void);
void BC_load_attitude_determination(void);
void BC_load_orbit_determination(void);
void BC_load_attitude_control(void);
void BC_load_orbit_control(void);
void BC_load_actuator(void);
void BC_load_spin_control_TL(void);
void BC_load_aocs(void);

void BC_load_pt_sensor_rotate(void);
void BC_load_mtq_curvols_rotate(void);

// 2021-10-21 野呂
//電源系のセンシングをまとめて管理するためのBC
//内蔵ADCはDMAを用いており非常に高速なので，Combineを用いる
//I2C_ADCはMUXの切り替えで電荷がたまるまで時間がかかってしまうこと，
//さらにそこまで高頻度に取らなくてもよいため，Rotateを用いることとする
void BC_load_built_in_ADC(void);
void BC_load_i2c_ADC(void);

void BC_load_read_sepcoil(void);
void BC_load_separation_off(void);
void BC_load_separation_on(void);

void BCL_load_start_hk_tlm(void);
void BC_load_tlm_every(void);
void BC_load_tlm_low_rate_1(void);
void BC_load_tlm_low_rate_2(void);
void BC_load_tlm_C2A(void);

void BC_load_tlm_cmd_ph(void);
void BC_load_tlm_generate_and_save(void);
void BC_load_tlm_routing(void);
void BC_load_obc_comm(void);
void BC_load_memory_patrol(void);
void BCL_tlm_hispin_mode(void);
void BCL_load_cobc_large_packet_set(void);
void BCL_load_cobc_large_packet_receive(void);
void BC_load_after_sep_param_instance(void);
void BC_load_after_sep_param_delay(void);

void BCL_load_start_hk_tlm(void);

void BC_load_nop(void);

// test
void BCL_load_respond_eh_for_test(void);
void BCL_load_test_bcl(void);

#endif