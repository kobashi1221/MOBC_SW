/*
 * Element2Cartesian.h
 *
 *  Created on: 2022/07/11
 *      Author: 稲守研
 */

#ifndef SRC_USER_DRIVERS_AOCS_ELEMENT2CARTESIAN_H_
#define SRC_USER_DRIVERS_AOCS_ELEMENT2CARTESIAN_H_
#include "Physical_Constants.h"
#include "AOCSVector.h"
#include <stdint.h> //for uint8_t
typedef struct{
	int		ep_year; /* Year of epoch, e.g. 94 for 1994, 100 for 2000AD */
	double	ep_day;	/* Day of epoch from 00:00 Jan 1st ( = 1.0 ) */
	double	rev;	/* Mean motion, revolutions per day */
	double	bstar;	/* Drag term .*/
	double	eqinc;	/* Equatorial inclination, deg */
	double	ecc;    /* Eccentricity */
	double	mnan;	/* Mean anomaly at epoch from elements, deg */
	double	argp;	/* Argument of perigee, deg */
	double	ascn;	/* Right ascension (ascending node), deg */

	int		ep_year_opponent; /* Year of epoch, e.g. 94 for 1994, 100 for 2000AD */
	double	ep_day_opponent;	/* Day of epoch from 00:00 Jan 1st ( = 1.0 ) */
	double	rev_opponent;	/* Mean motion, revolutions per day */
	double	bstar_opponent;	/* Drag term .*/
	double	eqinc_opponent;	/* Equatorial inclination, deg */
	double	ecc_opponent;    /* Eccentricity */
	double	mnan_opponent;	/* Mean anomaly at epoch from elements, deg */
	double	argp_opponent;	/* Argument of perigee, deg */
	double	ascn_opponent;	/* Right ascension (ascending node), deg */
}ORBENV_SET_STRUCT;

extern const ORBENV_SET_STRUCT* p_orbenv_struct;

void  ORBPROP_setparam(ORBENV_SET_STRUCT* orbprop_set, AOCSVector* current_y);

#endif /* SRC_USER_DRIVERS_AOCS_ELEMENT2CARTESIAN_H_ */
