/*
 * AOCS_debug_Bdot.cpp
 *
 *  Created on: 2022
 *      Author: fujita
 *      Reference
 *      	地球角速度補償：人工衛星の力学と制御ハンドブックp.147
 */

#include <stdio.h> //for printf
#include <stdint.h> //for uint8_t
#include <math.h>
#include <src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Compensation.h>
#include <stdlib.h>
#include <string.h>

#include "../../Global_Sensor_Value.h"
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Applications/UserDefined/Global_AOCS_Value.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/Aocs/Compensation.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>


static COMPENSATION_CLASS COMPENSATION_class;
static COMPENSATION_STRUCT COMPENSATION_;

const uint8_t constResMM_flag_ = 1;
const float constResMM_[3] = {0.04f,0.04f,0.04f}; // [Am2]
const uint8_t constShapeAn_flag_ = 0;
const float constShapeAn_[3][3] = {{1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f}}; // [Am2/T]
const uint8_t constEdCTorq_flag_ = 0;
const float constEdCTorq_[3] = {1e-7f,1e-7f,1e-7f}; // [Nm]
const float constMagMoment_[3] = {0.0f,0.0f,1.0f}; // [Am2]

static void Compesation_load_param(void);
static void Compesation_initialize_param(void);

static void ACS_Compesation_EarthAngularVelocity_update_(void);

static void ACS_Compesation_Coupling_update_(void);

static void ACS_Compesation_RMM_update_(void);

static void ACS_Compesation_EddyCurrent_update_(void);

static void ACS_Compesation_ConstantMagnetic_update_(void);

/**
 * @brief アプリケーション作成関数
 *
 * @note Driver_Superインターフェースモジュールを使用する時は起動時に必ず実施してください。
 * @param DRIVER_SUPER_STRUCT *my_super : 初期化するDRIVER_SUPER構造体へのポインタ
 * @param void *if_config : 使用するIFの設定値構造体へのポインタ
 * @return アプリケーション構造体
 */
AppInfo Compensation_param_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", Compesation_load_param, Compesation_initialize_param);
}

AppInfo ACS_Compesation_EarthAngularVelocity_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_Compesation_EarthAngularVelocity_update_);
}

AppInfo ACS_Compesation_Coupling_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_Compesation_Coupling_update_);
}

AppInfo ACS_Compesation_RMM_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_Compesation_RMM_update_);
}

AppInfo ACS_Compesation_EddyCurrent_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_Compesation_EddyCurrent_update_);
}

AppInfo ACS_Compesation_ConstantMagnetic_update(void)
{
    return AI_create_app_info("DBG_CrossProduct", NULL, ACS_Compesation_ConstantMagnetic_update_);
}


/**
 * @brief アプリケーション初期化関数
 *
 * 構造体内の初期化が必要な変数を初期化する．
 */
static void Compesation_load_param(void)
{
    printf("Compesation load_param.\n");
    g_acs.p_COMP = &COMPENSATION_;
    read_param(COMPENSATION_class.Kco_eav, COMPENSATION_CLASS::Kco_eav_, (uint16_t)sizeof(COMPENSATION_class.Kco_eav), (uint32_t)0x000071E5);
    read_param(COMPENSATION_class.Kco_cp, COMPENSATION_CLASS::Kco_cp_, (uint16_t)sizeof(COMPENSATION_class.Kco_cp), (uint32_t)0x000071F1);
    read_param(COMPENSATION_class.Kco_rmm, COMPENSATION_CLASS::Kco_rmm_, (uint16_t)sizeof(COMPENSATION_class.Kco_rmm), (uint32_t)0x000071FD);
    read_param(COMPENSATION_class.Kco_msa, COMPENSATION_CLASS::Kco_msa_, (uint16_t)sizeof(COMPENSATION_class.Kco_msa), (uint32_t)0x00007209);
    read_param(COMPENSATION_class.Kco_ec, COMPENSATION_CLASS::Kco_ec_, (uint16_t)sizeof(COMPENSATION_class.Kco_ec), (uint32_t)0x00007215);
    read_param(&COMPENSATION_.constResMM_flag, &constResMM_flag_, (uint16_t)sizeof(COMPENSATION_.constResMM_flag), (uint32_t)0x00007221);
    read_param(COMPENSATION_.constResMM, constResMM_, (uint16_t)sizeof(COMPENSATION_.constResMM), (uint32_t)0x00007222);
    read_param(&COMPENSATION_.constShapeAn_flag, &constShapeAn_flag_, (uint16_t)sizeof(COMPENSATION_.constShapeAn_flag), (uint32_t)0x0000722E);
    read_param(COMPENSATION_.constShapeAn, constShapeAn_, (uint16_t)sizeof(COMPENSATION_.constShapeAn), (uint32_t)0x0000722F);
    read_param(&COMPENSATION_.constEdCTorq_flag, &constEdCTorq_flag_, (uint16_t)sizeof(COMPENSATION_.constEdCTorq_flag), (uint32_t)0x00007253);
    read_param(COMPENSATION_.constEdCTorq, constEdCTorq_, (uint16_t)sizeof(COMPENSATION_.constEdCTorq), (uint32_t)0x00007254);
    read_param(COMPENSATION_.constMagMoment, constMagMoment_, (uint16_t)sizeof(COMPENSATION_.constMagMoment), (uint32_t)0x00007260);
    read_param(&COMPENSATION_class.eclipse_mode_eav, &COMPENSATION_CLASS::eclipse_mode_eav_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_eav), (uint32_t)0x00007289);
    read_param(&COMPENSATION_class.eclipse_mode_cp, &COMPENSATION_CLASS::eclipse_mode_cp_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_cp), (uint32_t)0x0000728A);
    read_param(&COMPENSATION_class.eclipse_mode_rmm, &COMPENSATION_CLASS::eclipse_mode_rmm_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_rmm), (uint32_t)0x0000728B);
    read_param(&COMPENSATION_class.eclipse_mode_msa, &COMPENSATION_CLASS::eclipse_mode_msa_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_msa), (uint32_t)0x0000728C);
    read_param(&COMPENSATION_class.eclipse_mode_ed, &COMPENSATION_CLASS::eclipse_mode_ed_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_ed), (uint32_t)0x0000728D);
    read_param(&COMPENSATION_class.eclipse_mode_cm, &COMPENSATION_CLASS::eclipse_mode_cm_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_cm), (uint32_t)0x0000728E);

}

/**
 * @brief アプリケーション実行関数
 *
 */
static void Compesation_initialize_param(void)
{
    printf("Compesation initialize_param.\n");
    write_param(COMPENSATION_class.Kco_eav, COMPENSATION_CLASS::Kco_eav_, (uint16_t)sizeof(COMPENSATION_class.Kco_eav), (uint32_t)0x000071E5);
    write_param(COMPENSATION_class.Kco_cp, COMPENSATION_CLASS::Kco_cp_, (uint16_t)sizeof(COMPENSATION_class.Kco_cp), (uint32_t)0x000071F1);
    write_param(COMPENSATION_class.Kco_rmm, COMPENSATION_CLASS::Kco_rmm_, (uint16_t)sizeof(COMPENSATION_class.Kco_rmm), (uint32_t)0x000071FD);
    write_param(COMPENSATION_class.Kco_msa, COMPENSATION_CLASS::Kco_msa_, (uint16_t)sizeof(COMPENSATION_class.Kco_msa), (uint32_t)0x00007209);
    write_param(COMPENSATION_class.Kco_ec, COMPENSATION_CLASS::Kco_ec_, (uint16_t)sizeof(COMPENSATION_class.Kco_ec), (uint32_t)0x00007215);
    write_param(&COMPENSATION_.constResMM_flag, &constResMM_flag_, (uint16_t)sizeof(COMPENSATION_.constResMM_flag), (uint32_t)0x00007221);
    write_param(COMPENSATION_.constResMM, constResMM_, (uint16_t)sizeof(COMPENSATION_.constResMM), (uint32_t)0x00007222);
    write_param(&COMPENSATION_.constShapeAn_flag, &constShapeAn_flag_, (uint16_t)sizeof(COMPENSATION_.constShapeAn_flag), (uint32_t)0x0000722E);
    write_param(COMPENSATION_.constShapeAn, constShapeAn_, (uint16_t)sizeof(COMPENSATION_.constShapeAn), (uint32_t)0x0000722F);
    write_param(&COMPENSATION_.constEdCTorq_flag, &constEdCTorq_flag_, (uint16_t)sizeof(COMPENSATION_.constEdCTorq_flag), (uint32_t)0x00007253);
    write_param(COMPENSATION_.constEdCTorq, constEdCTorq_, (uint16_t)sizeof(COMPENSATION_.constEdCTorq), (uint32_t)0x00007254);
    write_param(COMPENSATION_.constMagMoment, constMagMoment_, (uint16_t)sizeof(COMPENSATION_.constMagMoment), (uint32_t)0x00007260);
    write_param(&COMPENSATION_class.eclipse_mode_eav, &COMPENSATION_CLASS::eclipse_mode_eav_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_eav), (uint32_t)0x00007289);
    write_param(&COMPENSATION_class.eclipse_mode_cp, &COMPENSATION_CLASS::eclipse_mode_cp_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_cp), (uint32_t)0x0000728A);
    write_param(&COMPENSATION_class.eclipse_mode_rmm, &COMPENSATION_CLASS::eclipse_mode_rmm_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_rmm), (uint32_t)0x0000728B);
    write_param(&COMPENSATION_class.eclipse_mode_msa, &COMPENSATION_CLASS::eclipse_mode_msa_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_msa), (uint32_t)0x0000728C);
    write_param(&COMPENSATION_class.eclipse_mode_ed, &COMPENSATION_CLASS::eclipse_mode_ed_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_ed), (uint32_t)0x0000728D);
    write_param(&COMPENSATION_class.eclipse_mode_cm, &COMPENSATION_CLASS::eclipse_mode_cm_, (uint16_t)sizeof(COMPENSATION_class.eclipse_mode_cm), (uint32_t)0x0000728E);

}

/**
 * @brief コマンド実行関数
 *
 * 初期化のいらない処理内容を記述．
 */

/**
 * @brief  地球角速度補償計算を行う
 * @param  MagField              磁気センサ値(機体座標系)　[nT]
 * @param  Omega               機体角速度 [rad/s]
 * @param  Position            位置 [m]
 * @param  Velocity            速度 [m/s]
 * @param  Mag_moment      必要磁気モーメント [Am^2]
 * @return  なし
 * Reference 地球角速度補償：人工衛星の力学と制御ハンドブックp.147
 * 作成者　田村　2022/07/19
 * コメント記入者 藤田 2023/06/23
 */
//---------------------------------------地球角速度補償--------------------------------------------------
static void ACS_Compesation_EarthAngularVelocity_update_(void){
	///Input: 地磁気，クォータニオン，目標角運動量ベクトル
	///Output: 磁気モーメント
		float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]}; //ADSの磁気ベクトル(機体座標系)を格納
		float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]}; //ADSの機体角速度ベクトルを格納

		float pos[3] = {g_ads.p_ADS->Position[0], g_ads.p_ADS->Position[1], g_ads.p_ADS->Position[2]}; //ADSの機体位置ベクトルを格納
		float vel[3] = {g_ads.p_ADS->Velocity[0], g_ads.p_ADS->Velocity[1], g_ads.p_ADS->Velocity[2]}; //ADSの機体速度ベクトルを格納

		//蝕時は要求磁気モーメントを0にする．それ以外は要求磁気モーメントを計算する
		if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || COMPENSATION_class.eclipse_mode_eav == 1){
			COMPENSATION_class.Eartcal(B_fld_, omega, pos, vel);
		}
		else{
			COMPENSATION_class.OUT_Moment_b_[0] = 0.0f;
			COMPENSATION_class.OUT_Moment_b_[1] = 0.0f;
			COMPENSATION_class.OUT_Moment_b_[2] = 0.0f;
		}

		copy(COMPENSATION_class.OUT_Moment_b_, 3, 1, COMPENSATION_.Mag_moment);;

//		GLOBAL_AOCS_set_Mag_moment(COMPENSATION_.Mag_moment);
		//要求磁気モーメントをACSのMag_moment_Co_EarthAngに格納
		GLOBAL_ACS_set_Mag_moment_Co_EarthAng(COMPENSATION_.Mag_moment);

//		printf("EAV_magmmt %.2e %.2e %.2e ", COMPENSATION_.Mag_moment[0], COMPENSATION_.Mag_moment[1], COMPENSATION_.Mag_moment[2]);
}

//---------------------------------------カップリング項補償--------------------------------------------------
static void ACS_Compesation_Coupling_update_(void){
///Input: 地磁気，クォータニオン，目標角運動量ベクトル
///Output: 磁気モーメント
	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float omega[3] = {g_ads.p_ADS->Omega[0], g_ads.p_ADS->Omega[1], g_ads.p_ADS->Omega[2]};

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || COMPENSATION_class.eclipse_mode_cp == 1){
		COMPENSATION_class.Coupcal(B_fld_, omega);
	}
	else{
		COMPENSATION_class.OUT_Moment_b_[0] = 0.0f;
		COMPENSATION_class.OUT_Moment_b_[1] = 0.0f;
		COMPENSATION_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(COMPENSATION_class.OUT_Moment_b_, 3, 1, COMPENSATION_.Mag_moment);

//	GLOBAL_AOCS_set_Mag_moment(COMPENSATION_.Mag_moment);
	GLOBAL_ACS_set_Mag_moment_Co_Coupling(COMPENSATION_.Mag_moment);

//	printf("CP_magmmt %.2e %.2e %.2e ", COMPENSATION_.Mag_moment[0], COMPENSATION_.Mag_moment[1], COMPENSATION_.Mag_moment[2]);
}

//---------------------------------------RMM補償(残留磁気)--------------------------------------------------
static void ACS_Compesation_RMM_update_(void){
///Input: 地磁気，クォータニオン，目標角運動量ベクトル
///Output: 磁気モーメント
	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float RMM[3] = {g_ads.p_ADS->RM_moment[0], g_ads.p_ADS->RM_moment[1], g_ads.p_ADS->RM_moment[2]};

	if(COMPENSATION_.constResMM_flag){
		copy(COMPENSATION_.constResMM,3,1,RMM);
	}

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || COMPENSATION_class.eclipse_mode_rmm == 1){
		COMPENSATION_class.RMMcal(B_fld_, RMM);
	}
	else{
		COMPENSATION_class.OUT_Moment_b_[0] = 0.0f;
		COMPENSATION_class.OUT_Moment_b_[1] = 0.0f;
		COMPENSATION_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(RMM, 3, 1, COMPENSATION_.Mag_moment);

	//	GLOBAL_AOCS_set_Mag_moment(COMPENSATION_.Mag_moment);
	GLOBAL_ACS_set_Mag_moment_Co_RMM(COMPENSATION_.Mag_moment);

//	printf("RMM %.2e %.2e %.2e ", COMPENSATION_.Mag_moment[0], COMPENSATION_.Mag_moment[1], COMPENSATION_.Mag_moment[2]);
}

//---------------------------------------形状異方性補償--------------------------------------------------

CCP_CmdRet Cmd_ACS_Compesation_ShapeAnisortropy(const CommonCmdPacket* packet)
{
///Input: 地磁気，クォータニオン，目標角運動量ベクトル
///Output: 磁気モーメント
	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float ShapeMag[3][3];
	const uint8_t* param = CCP_get_param_head(packet);
	memcpy(&ShapeMag, param, 9);

	if(COMPENSATION_.constShapeAn_flag){
		copy(*COMPENSATION_.constShapeAn,3,3,*ShapeMag);
	}

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || COMPENSATION_class.eclipse_mode_msa == 1){
		COMPENSATION_class.Shapecal(B_fld_, ShapeMag);
	}
	else{
		COMPENSATION_class.OUT_Moment_b_[0] = 0.0f;
		COMPENSATION_class.OUT_Moment_b_[1] = 0.0f;
		COMPENSATION_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(COMPENSATION_class.OUT_Moment_b_, 3, 1, COMPENSATION_.Mag_moment);;

	//	GLOBAL_AOCS_set_Mag_moment(COMPENSATION_.Mag_moment);
	GLOBAL_ACS_set_Mag_moment_Co_Anisotropy(COMPENSATION_.Mag_moment);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

//---------------------------------------渦電流補償--------------------------------------------------
static void ACS_Compesation_EddyCurrent_update_(void){
///Input: 地磁気，クォータニオン，目標角運動量ベクトル
///Output: 磁気モーメント
	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};
	float EddyTreq[3];

	if(COMPENSATION_.constEdCTorq_flag){
		copy(COMPENSATION_.constEdCTorq,3,1,EddyTreq);
	}

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || COMPENSATION_class.eclipse_mode_ed == 1){
		COMPENSATION_class.Eddycal(B_fld_, EddyTreq);
	}
	else{
		COMPENSATION_class.OUT_Moment_b_[0] = 0.0f;
		COMPENSATION_class.OUT_Moment_b_[1] = 0.0f;
		COMPENSATION_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(COMPENSATION_class.OUT_Moment_b_, 3, 1, COMPENSATION_.Mag_moment);

	//	GLOBAL_AOCS_set_Mag_moment(COMPENSATION_.Mag_moment);
	GLOBAL_ACS_set_Mag_moment_Co_Eddy(COMPENSATION_.Mag_moment);
}

//---------------------------------------一定磁気モーメント補償--------------------------------------------------
static void ACS_Compesation_ConstantMagnetic_update_(void){
///Input: 地磁気，クォータニオン，目標角運動量ベクトル
///Output: 磁気モーメント
	float B_fld_[3] = {g_ads.p_ADS->MagField[0], g_ads.p_ADS->MagField[1], g_ads.p_ADS->MagField[2]};

	if (vote_eclipse_flag(&g_acs.p_GLOBAL_AOCS->Eclipse_flag) == 0 || COMPENSATION_class.eclipse_mode_cm == 1){
		COMPENSATION_class.Conscal(B_fld_, COMPENSATION_.constMagMoment);
	}
	else{
		COMPENSATION_class.OUT_Moment_b_[0] = 0.0f;
		COMPENSATION_class.OUT_Moment_b_[1] = 0.0f;
		COMPENSATION_class.OUT_Moment_b_[2] = 0.0f;
	}

	copy(COMPENSATION_.constMagMoment, 3, 1, COMPENSATION_.Mag_moment);

	//	GLOBAL_AOCS_set_Mag_moment(COMPENSATION_.Mag_moment);
	GLOBAL_ACS_set_Mag_moment_Co_ConstMag(COMPENSATION_.Mag_moment);
}

