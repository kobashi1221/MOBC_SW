/*
 * EKF.h
 *
 *  ver.1.0 2022/07/02 tamura
 *
 */

#ifndef C2A_AOCS_EKF_H_
#define C2A_AOCS_EKF_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h> //for uint8_t
#include <src_core/System/TImeManager/time_manager.h>//for delay()
#include <src_user/Applications/UserDefined/AOCS/Global_AOCS_Parameter.h>
#include <src_user/IfWrapper/IF_class.h>


//-----------------------------------------
// クラス宣言
//-----------------------------------------

typedef struct {
	float Omega[3] = {};
	float Quaternion[4] = {};
	uint8_t Condition_flag[8] = {};
}SEP_CONDITION_TLM;

class EKF_CLASS
{
private:

	uint8_t refdim;
	uint8_t obsdim;
	uint8_t Pdim;
	uint8_t Qdim;
	uint8_t Rdim;

	//-----------------------------------------
	// ADCS　計算関数宣言
	//-----------------------------------------

	void Calc_A(const ADS_MODE ekf_id, float* A);
	void Calc_B(const ADS_MODE ekf_id, float* B);
	void Calc_F(float* A, float* F);
	void Calc_Gw(float* B, float* Gw);
	void Calc_H(const ADS_MODE ekf_id, float* H);
	void Calc_Q();
	void Calc_R();
	void Calc_y(const ADS_MODE ekf_id, float* y);
	void Calc_r(const ADS_MODE ekf_id, float* r);


	// 実際にADCS系の計算を行う関数を宣言
	// 入出力は全て引数として定義（出力引数はポインタ渡し）
	void Predict(const ADS_MODE ekf_id);
	void Observe(const ADS_MODE ekf_id);

	uint8_t Sep_Att_Check(int process_cnt, uint itr);
	void parse_tlm(int process_cnt, uint itr);

	void Quaternion_delta(float* q_acq, float* q_obs, float* q_delta);
	void Calc_Qua_Delta_AcqTime2ObsTime(float* q_acq, float* q_obs, float* q_delta);
	void Adjust_Obs_Time(const ADS_MODE ekf_id);

public:
	//-----------------------------------------
	// 衛星ソフトウェア依存変数宣言
	//-----------------------------------------
	EKF_CLASS();
	~EKF_CLASS();

	// 主には出力ベクトル、配列を宣言

	//-----観測値-----
	float Obs[16]; //本当にテレメとして必要か？アプリケーションの構造体で同じものを持っている

	//-----１サイクル前の事前推定値コピー------------
	float X_ref_pre_previous[16] = {}; //1サイクル前のX_ref_preの値なので不要のはず
	float P_PRE_previous[15*15] = {};  //1サイクル前のP_preの値なので不要のはず
	//-----------------------

	//----推定値 estimate (常に更新)---------
	float X_ref[16] = {}; // テレメとして保存していないからstaticグローバル変数としてもいいのではないか→結局RAM領域を使用することになる
	float P[15*15] = {};  // テレメとして保存していないからstaticグローバル変数としてもいいのではないか→結局RAM領域を使用することになる
	//----------------------

	//----事後推定値  a posteriori estimate　（観測時のみ更新）-----------
	float X_ref_post[16] = {};
	float P_POST[15*15] = {};
	//-----------------------

	//-----事前推定値  観測更新用保存推定値------------
	// 通常のカルマンフィルタと異なり観測更新時刻を超えた時刻までプロパゲートを行う
	float X_ref_pre[16] = {};
	float P_PRE[15*15] = {};
	//-----------------------

	float Q[15*15] = {}; //システムノイズ共分散行列 //衛星パラメータを代入してるだけなのでテレメとしては不要　staticグローバル変数?
	float R[15*15] = {}; //観測ノイズ共分散行列 //衛星パラメータを代入してるだけなのでテレメとしては不要　staticグローバル変数?
	float y[15]; //　Calc_y　イノベーション　＝　観測値　ー　事前推定値　本当に必要か？観測値と事前推定値のテレメがあれば地上局で計算できる 一応使ってる
//	float r[15]; // Calc_r 残差　＝　観測値　ー　事後推定値 本当に必要か？観測値と事後推定値のテレメがあれば地上局で計算できる　使ってない


	uint8_t ekf_valid; // カルマンフィルタ有効フラグ
	uint8_t dq_valid;
	uint8_t divg_valid; // カルマンフィルタの発散確認フラグ


/**
 *	設定用の変数
 */

	/**
	 *　static　member variable. どのインスタンスも同じ変数
	 */

	// MTQの実行値(root mean squre)をダイナミクスプロパゲートに使用
	// 実効値はMTQの出力実時間から計算。磁気センサ取得のため出力OFFとなる時間を考慮。RMSと変数名にいれるが厳密でなくてよい
	static float mtq_rms_coeff_x;
	static constexpr float mtq_rms_coeff_x_ = 0.5f;

	static float mtq_rms_coeff_y;
	static constexpr float mtq_rms_coeff_y_ = 0.5f;

	static float mtq_rms_coeff_z;
	static constexpr float mtq_rms_coeff_z_ = 0.5f;


	//**** 分離用の変数 ****//
	static float omega_abs_thr;	// 目標角速度[rad/s]
	static constexpr float omega_abs_thr_ = 12.0f;

	static float direction_err_b;	// 機体座標に対する回転軸方向の許容誤差[deg]
	static constexpr float direction_err_b_ = 1.0f;

	static float omega_X_min;	// x軸角速度の下限[rad/s]
	static constexpr float omega_X_min_ = 12.0f;

	static float omega_X_max;	// x軸角速度の上限[rad/s]
	static constexpr float omega_X_max_ = 13.0f;

	static float omega_Y_min;	// y軸角速度の下限[rad/s]
	static constexpr float omega_Y_min_ = -2.0f;

	static float omega_Y_max;	// y軸角速度の上限[rad/s]
	static constexpr float omega_Y_max_ = 2.0f;

	static float omega_Z_min;	// z軸角速度の下限[rad/s]
	static constexpr float omega_Z_min_ = -2.0f;

	static float omega_Z_max;	// z軸角速度の上限[rad/s]
	static constexpr float omega_Z_max_ = 2.0f;

	static float err_cov_thr[6];	// 誤差共分散行列の閾値
	static constexpr float err_cov_thr_[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

	static float direction_err_eci;	// ECI座標系に対する回転軸方向の許容誤差[deg]
	static constexpr float direction_err_eci_ = 1.0f;

	static float sep_direction_err;	// 分離方向（LVLH）の許容誤差[deg]
	static constexpr float sep_direction_err_ = 1.0f;

	static float separate_direction[3];	// 2Uに対する1Uの分離方向（LVLH）[m/s]
	static constexpr float separate_direction_[3] = {1.884f, 0.06f, 0.0f};

	static float pos_1U_b[3];	// 2Uに対する1Uの方向ベクトル
	static constexpr float pos_1U_b_[3] = {0.0f, 0.0f, -1.0f};

	static float angle_velocity2omega;	// 速度方向と角速度方向のなす角[deg]
	static constexpr float angle_velocity2omega_ = 3.0f;

	static float angle_velocity2omega_err;	// 速度方向と角速度方向のなす角の許容誤差[deg]
	static constexpr float angle_velocity2omega_err_ = 1.0f;

	static float angle_normal2vXomega;	// 軌道面外方向とv×ωのなす角[deg]
	static constexpr float angle_normal2vXomega_ = 0.0f;

	static float angle_normal2vXomega_err;	// 軌道面外方向とv×ωのなす角の許容誤差[deg]
	static constexpr float angle_normal2vXomega_err_ = 1.0f;

	static float sep_phase;	// 分離時の回転位相[deg]
	static constexpr float sep_phase_ = 0.0f;

	static float sep_phase_err;	// 分離時の回転位相の許容誤差[deg]
	static constexpr float sep_phase_err_ = 1.0f;

	// 分離条件テレメ用構造体
	static SEP_CONDITION_TLM sep_condition_tlm;
	TCP pckt;
	static uint8_t width;	//テレメトリのパケットに入れるプロパゲート間隔
	static constexpr uint8_t width_ = 10;
	uint8_t count;
	uint8_t ekf_num;
	static uint8_t select_ekf;	//直列EKFの内どちらのデータをテレメトリとして保存するか
	static constexpr uint8_t select_ekf_ = 0x00;


	static int16_t sep_time; // プロパゲート分離タイミング [ms]　負の値なら分離条件未達成

	static int16_t sep_time_under_limit;  //　待機開始時刻の下限 この時間より早く分離しない [ms]
	static constexpr int16_t sep_time_under_limit_ = 80;
	//***********************//


	//----GYRO----//

	//----GYROBIAS----//
	static float taub;      //　ジャイロバイアスの変化速度を規定する一次ローパスフィルタの時定数
	static constexpr float taub_ = 100.0f;

	//----MAGSUNGYRO----//
	static uint8_t K_type;
	static constexpr uint8_t K_type_ = 2;

	//----RMM----//
	static float rmm_thr;
	static constexpr float rmm_thr_ = 1.0f;

	static float taurmm;
	static constexpr float taurmm_ = 10000.0f;

	//----EDDY----//

	//----ANISO----//




	/**
	 *　member variable. インスタンス毎に異なる変数
	 */

	static uint16_t observe_time;  //　観測更新の時間 [ms]
	static constexpr uint16_t observe_time_ = 1000;

	static uint16_t max_prop;  //　EKF１サイクルでの最大プロパゲート時間 [ms]
	static constexpr uint16_t max_prop_ = 1000;

	uint16_t dt; // プロパゲート時間間隔 [ms]
	static constexpr uint16_t dt_0_ = 1000;
	static constexpr uint16_t dt_1_ = 200;

	//**** 高速回転Mode用の変数 ****//
	static uint16_t observe_time_hispin;  //　観測更新の時間 [ms]
	static constexpr uint16_t observe_time_hispin_ = 200;

	static uint16_t max_prop_hispin; // EKF１サイクルでの最大プロパゲート時間 [ms]
	static constexpr uint16_t max_prop_hispin_ = 280;

	uint16_t dt_hispin; // プロパゲート時間間隔 [ms]
	static constexpr uint16_t dt_hispin_0_ = 200;
	static constexpr uint16_t dt_hispin_1_ = 5;
	//***********************//

	//**** 観測値プロパゲート用の変数 ****//
	static uint8_t reltime_type; //　0: 観測値時刻ずれ補正なし.  1:MRAM定数使用.  2:毎周期計測された時間使用
	static constexpr uint8_t reltime_type_ = 0;

	//　各物理値取得時推定Quaternion
	// sun, mag, quaternion, omega
	static int16_t sensor_reltime_to_obs[4]; // GYROを基準観測時とした相対時間
	static constexpr int16_t sensor_reltime_to_obs_[4] = {-100, -10, -50, 0};
	float q_at_acq[4*4] = {}; // 物理値取得時のQuaternion
	float q_delta[4*4] = {}; //　 物理値取得時から基準観測時Quaternion変化
	//***********************//

	float ini_X_ref[16];
	static constexpr float ini_X_ref_0_[16] =  {0.0f, 0.0f, 0.0f, 1.0f,
												0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f};
	static constexpr float ini_X_ref_1_[16] =  {0.0f, 0.0f, 0.0f, 1.0f,
											  	0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f};

	float ini_P_diag[15];
	static constexpr float ini_P_diag_0_[15] = {1.0f, 1.0f, 1.0f,
												1.0f, 1.0f, 1.0f,
												1.0f, 1.0f, 1.0f,
												1.0f, 1.0f, 1.0f,
												1.0f, 1.0f, 1.0f};
	static constexpr float ini_P_diag_1_[15] = {1.0f, 1.0f, 1.0f,
												1.0f, 1.0f, 1.0f,
												1.0f, 1.0f, 1.0f,
												1.0f, 1.0f, 1.0f,
												1.0f, 1.0f, 1.0f};


	/*
	 * 状態方程式、観測方程式
	 */
	float dt_sec;

	float M[3][3] = {};  //[ω×]
	float mM[3][3] = {}; //minusM [×ω]
	float L[3] = {};     //　角運動量ベクトル

	// Adaptive Noise Covariance
	// 0<alpha<1 alpha が大きいと直前の値を重視しQやRの振動が起きにくいが、変化感度が鈍い。
	uint8_t adp_noise_flag;
	static constexpr uint8_t adp_noise_flag_0_ = 0;
	static constexpr uint8_t adp_noise_flag_1_ = 0;

	float alpha;
	static constexpr float alpha_0_ = 0.97f;
	static constexpr float alpha_1_ = 0.97f;

	float std_Q[15];  //システムノイズ標準偏差
	static constexpr float std_Q_0_[15] = {1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f};

	static constexpr float std_Q_1_[15] = {1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f};

	float std_R[15];  //観測ノイズ標準偏差
	static constexpr float std_R_0_[15] = {1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f};

	static constexpr float std_R_1_[15] = {1e-3f, 1e-3f, 1e-3f,
									       1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f,
										   1e-3f, 1e-3f, 1e-3f};

	// 定常カルマンゲイン用　　 AOCSフラグで　***_COV　を立てていなければ使用されるK
	float const_K_diag[15];
	static constexpr float const_K_diag_0_[15] = {0.95f, 0.95f, 0.95f,
											   	  0.95f, 0.95f, 0.95f,
												  0.95f, 0.95f, 0.95f,
												  0.95f, 0.95f, 0.95f,
												  0.95f, 0.95f, 0.95f};
	static constexpr float const_K_diag_1_[15] = {0.95f, 0.95f, 0.95f,
												  0.95f, 0.95f, 0.95f,
												  0.95f, 0.95f, 0.95f,
												  0.95f, 0.95f, 0.95f,
												  0.95f, 0.95f, 0.95f};

	// 一定誤差共分散行列
	uint8_t P_constflag;
	static constexpr uint8_t P_constflag_0_ = 0;
	static constexpr uint8_t P_constflag_1_ = 0;

	float const_P_diag[15];
	static constexpr float const_P_diag_0_[15] = {1.0f, 1.0f, 1.0f,
												  1.0f, 1.0f, 1.0f,
												  1.0f, 1.0f, 1.0f,
												  1.0f, 1.0f, 1.0f,
												  1.0f, 1.0f, 1.0f};
	static constexpr float const_P_diag_1_[15] = {1.0f, 1.0f, 1.0f,
												  1.0f, 1.0f, 1.0f,
												  1.0f, 1.0f, 1.0f,
												  1.0f, 1.0f, 1.0f,
												  1.0f, 1.0f, 1.0f};
	float conv_thr; // P対角成分の発散条件
	static constexpr float conv_thr_0_ = 1e+3f;
	static constexpr float conv_thr_1_ = 1e+3f;



	/*
	 * 計算関数、初期化関数
	 */
	// 主にはADCS系の計算を実行し、結果を衛星ソフトウェア依存変数に格納する
	void cal(const ADS_MODE ekf_id, const float* in_X, uint8_t refdim, uint8_t obsdim, uint8_t Pdim, uint8_t Qdim, uint8_t Rdim);

	// 主にはADCS系の計算を実行し、結果を衛星ソフトウェア依存変数に格納する
	int16_t cal_hispin(const ADS_MODE ekf_id, const float* in_X, uint8_t refdim, uint8_t obsdim, uint8_t Pdim, uint8_t Qdim, uint8_t Rdim);

	// 衛星ソフトウェア依存変数の初期化関数
	void reset(uint8_t in_refdim, uint8_t in_Pdim, uint8_t in_Qdim, uint8_t in_Rdim);

};

#ifdef __cplusplus
}
#endif

#endif /* C2A_AOCS_EKF_H_ */
