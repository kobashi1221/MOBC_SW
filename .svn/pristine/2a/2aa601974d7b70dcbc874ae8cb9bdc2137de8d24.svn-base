/*
 * AOCSfunk.h
 *
 *  Created on: 2021/03/27
 *      Author: masud
 */

#ifndef C2A_AOCS_AOCSFUNK_H_
#define C2A_AOCS_AOCSFUNK_H_

#include <stdint.h> //for uint8_t
#include <src_core/System/TImeManager/time_manager.h>//for delay()


#define SQR(x) ((x)*(x))
#define CUB(y) ((y)*(y)*(y))
/*--- WGS84 座標パラメータ */
#define SemiA 6378137.0 /* Semi-major axis */
#define ONE_F 298.257223563 /* 1/F */
#define SemiB (SemiA*(1.0 - 1.0/ONE_F))
#define E2 ((1.0/ONE_F)*(2-(1.0/ONE_F)))
#define ED2 (E2*SemiA*SemiA/(SemiB*SemiB))
#define NN(p) (SemiA/sqrtf(1.0 - (E2)*SQR(sinf(p*M_PI/180.0))))

typedef struct
{

} AOCS_STRUCT;

void display(float* in_Mat, int in_Matline, int in_Matrow);
void display_exp(float *in_Mat, int in_Matline, int in_Matrow);
void add(const float* in_Mat1, int in_Matline1, int in_Matrow1, const float* in_Mat2, int in_Matline2, int in_Matrow2, float* out_Mat);
void sub(const float* in_Mat1, int in_Matline1, int in_Matrow1, const float* in_Mat2, int in_Matline2, int in_Matrow2, float* out_Mat);
void multiplication(const float* in_Mat1, int in_Matline1, int in_Matrow1, const float* in_Mat2, int in_Matline2, int in_Matrow2, float* out_Mat, int out_Matline, int out_Matrow);
void trans(float* in_Mat, int in_Matline, int in_Matrow, float* out_Mat, int inout_Matline, int inout_Matrow);
void Normalization(const float* in_Vec, int in_Vecsize, float* out_Vec);
void CoeffVec(float* in_Vec, int in_Vecsize, float in_coeff, float* out_Vec);
float Fabs(const float* in_Vec, int in_Vecsize);
void Zeros(int in_Matline, int in_Matrow, float *out_Mat);
void Identity(int in_Matline, int in_Matrow, float* out_Mat);
void CoeffMat(const float *in_Vec, int in_Matrow, int in_Matcol, float in_coeff, float *out_Mat);
float determinant(float *in_Mat1, int in_deg);
void Inverse(float* in_Mat1, int in_Matline1, int in_Matrow1, float* in_Mat2, float* out_Mat);
void copy(const float *in_Mat, int in_Matline, int in_Matrow, float *out_Mat);
void Rotate_one_axis(const float* in_Vec, int in_mode, int in_axis, float in_theta, float* out_Vec);
void ExtractMat(const float *in_Mat, int out_Matline, int out_Matrow, float *out_Mat);
void ExtractVec(const float *in_Vec, int out_Start, int out_End, float *out_Vec);
int isnanMat(const float *in_Mat, int in_Matrow, int in_Matcol);
void initQuaternion(float *inout_quaternion);
float VecAngle(float *in_vec_1, float *in_vec_2, int vec_size, int mode);

void Outer_product(const float* in_Vec1, const float* in_Vec2, float* in_Vec3);  //外積計算関数
void DynamicsKinematics(const float* in_Xk, const float* in_Torque_b_, float* out_K); //ダイナミクスキネマティクス計算
void RungeOneStep(const float in_dt, const float* in_X, const float* in_Torque_b_, float* out_Next_x); //ルンゲクッタ
//void RungeOneStep(const float* in_X, const float* in_Torque_b_, float* out_Next_x); //ルンゲクッタ
void OneStepQuaGBias(const float in_dt, const float* in_X, const float* omega, const float in_tau, float* out_Next_x);
void FunctionQuaGBias(const float* in_Xk, const float tau, float* out_K);
void OneStepOmeRmm(const float in_dt, const float* in_X,  const float* in_B, const float in_tau, float* out_Next_x);
void FunctionOmeRmm(const float* in_Xk, const float* in_B, const float in_tau, float* out_K);

void Quaternion_obs_update(const float* Quaternion_ref, const float* Dqv, float* Dq_ref, uint8_t* p_dq_valid);
void Omega4Kinematics(const float* in_omega, float out_OMEGA[4][4]);
void OmegaCross4Kalman(const float in_Omega[3], float out_M[3][3]);
void CrossOmega4Kalman(const float in_Omega[3], float out_M[3][3]);
void Quaternion_Product(const float in_Q1[4], const float in_Q2[4], float out_Q3[4]);
void Quaternion_Conjugate(float in_q[4], float out_q1_conj[4]);
void Quaternion_To_DCM(const float* in_Qua, float* out_DCM);
void DCM_To_Quaternion(const float* in_DCM, float* out_Qua);
void make_rotate_matrix(int in_axis, float in_theta, float* out_MATRIX);
void blh2ecef(float in_phi, float in_ramda, float in_height, float *out_ECEF);
void ecef2blh(float *in_ECEF, float *out_blh);
void DCMeci2lvlh(float *in_eci_pos, float *in_eci_vel, float *out_DCM);
void make_rotate_matrix(int in_axis, float in_theta, float* out_MATRIX);
#endif /* C2A_AOCS_AOCSFUNK_H_ */
