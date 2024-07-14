/*
 * Element2Cartesian.cpp
 *
 *  Created on: 2022/07/09
 *      Author: 稲守研
 */

#include "sgp4.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //for uint8_t
#include "Element2Cartesian.h"
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include "AOCScalc.h"
#include <src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBPROP.h>


void  ORBPROP_setparam(ORBENV_SET_STRUCT* orbprop_set, AOCSVector* current_y)
{
	AOCSVector y0 = AOCSVectorHelper(6);

	float SMA;
	float rev;
	float n;
	float eqinc;
	float ecc;
	float mnan;
	float argp;
	float ascn;

	rev = orbprop_set->rev;
	eqinc = orbprop_set->eqinc / 180.0*M_PI;
	ecc = orbprop_set->ecc;
	mnan = orbprop_set->mnan / 180.0*M_PI;
	argp = orbprop_set->argp / 180.0*M_PI;
	ascn = orbprop_set->ascn / 180.0*M_PI;
	float eta;
	float r;
	float s;
	float xtemp1[3];
	float xtemp2[3];
	float vtemp1[3];
	float vtemp2[3];
	float x[3];
	float v[3];

	s = 24 * 3600 / rev;

	n = 2 * M_PI / s;

	SMA = cbrtf(mu_e / pow(n, 2));
	eta = sqrtf(1 - pow(ecc, 2));
	r = SMA * pow(eta, 2) / ( 1 + ecc * cos(mnan));

	float x_[3] = { r * cosf(mnan), r *sinf(mnan), 0};
	float v_[3] = {- SMA * n * sinf(mnan) /eta, SMA * n * (cosf(mnan) + ecc)/ eta,0};

	Rotate_one_axis(x_, 1, 2, -ascn, xtemp1);
	Rotate_one_axis(xtemp1, 1, 0, -eqinc, xtemp2);
	Rotate_one_axis(xtemp2, 1, 2, -argp, x);

	Rotate_one_axis(v_, 1, 2, -ascn, vtemp1);
	Rotate_one_axis(vtemp1, 1, 0, -eqinc, vtemp2);
	Rotate_one_axis(vtemp2, 1, 2, -argp, v);


	GLOBAL_ODS_set_Position(x);
	GLOBAL_ODS_set_Velocity(v);
}
