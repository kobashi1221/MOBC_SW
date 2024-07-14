/*
 * GlobalACSVariable.h
 *
 *  Created on: 2021/05/25
 *      Author: kahak
 */

#ifndef C2A_APPLICATIONS_GLOBAL_ACS_VARIABLE_H_
#define C2A_APPLICATIONS_GLOBAL_ACS_VARIABLE_H_

#include <src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Bdot.h>
#include <src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_CrossProduct.h>
#include <src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Qmethod.h>
#include <src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Quest.h>
#include <src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_TRIAD.h>
#include <src_user/Applications/UserDefined/Aocs/OCS/OCS_cmd_CrossProduct.h>
#include <src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_IGRF.h>
#include <src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_GST.h>
#include <src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_SunVector.h>
#include <src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_EarthVector.h>
#include <src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_SGP4.h>
#include <src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBPROP.h>
#include <src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBEKF.h>
#include <src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_BallCoef.h>
#include <src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.h>
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_COmpensation.h>
#include <src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_BBdot.h>
#include <src_user/Applications/UserDefined/Aocs/AOCS_cmd_EKF.h>



typedef struct{
	const QMETHOD_STRUCT* p_QMETHOD;
	const QUEST_STRUCT* p_QUEST;
	const IGRF_STRUCT* p_IGRF;
	const JULIANDATE_STRUCT* p_JULIANDATE;
	const GST_STRUCT* p_GST;
	const SUNVECTOR_STRUCT* p_SUNVECTOR;
	const TRIAD_STRUCT* p_TRIAD;
	const GLOBAL_ADS_PARAMETER* p_ADS;
	const BBDOT_STRUCT* p_BBDOT;
	const EKF_STRUCT* p_EKF;
}Global_ADS_STRUCT;


typedef struct{
	const BDOT_STRUCT* p_BDOT;
	const CROSS_STRUCT* p_CROSS;
	const EARTHVECTOR_STRUCT* p_EARTHVECTOR;
	const GLOBAL_AOCS_PARAMETER_STRUCT* p_GLOBAL_AOCS;
	const GLOBAL_ACS_PARAMETER* p_ACS;
	const COMPENSATION_STRUCT* p_COMP;
}Global_ACS_STRUCT;


typedef struct{
	const SGP4_STRUCT* p_SGP4;
	const GLOBAL_ODS_PARAMETER* p_ODS;  //←ここにGPSデータを入れる
	const ORBPROP_STRUCT* p_ORBPROP;
	const ORBEKF_STRUCT* p_ORBEKF;
	const BALLCOEF_STRUCT* p_BALLCOEF;
}Global_ODS_STRUCT;


typedef struct{
	const OCS_CROSS_STRUCT* p_OCS_CROSS;
	const GLOBAL_OCS_PARAMETER* p_OCS;
}Global_OCS_STRUCT;



// *** global_ACS_value *** //
extern Global_ADS_STRUCT g_ads;
extern Global_ACS_STRUCT g_acs;
extern Global_ODS_STRUCT g_ods;
extern Global_OCS_STRUCT g_ocs;


#endif /* C2A_APPLICATIONS_GLOBAL_ACS_VARIABLE_H_ */
