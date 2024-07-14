/*
 * di_struct.h
 *
 *  Created on: 2021/11/17
 *      Author: sakaguchi
 */
#ifndef DI_STRUCT_H_
#define DI_STRUCT_H_

#include "../../TlmCmd/user_packet_handler.h"



#ifdef __cplusplus
 extern "C" {
#endif

typedef struct
{
  // C2A情報
  struct
  {
    PH_ACK ph_ack;                            //!< 受信したコマンドをPHに渡したときの返り値
  } c2a;                                                   //!< C2A情報
  // 通信情報
  struct
  {
  } comm;                                                  //!< 通信情報
} _Info;

typedef enum
{
	_SUCCESS,
	_TYPE_AD,
	_TYPE_BD,
	_TYPE_BC,
	_TLM_UPDATE_SUCCESS,
	_TLM_AH_UPDATE_SUCCESS,
	_INIT_FAILED,
	_FILL_SETUP_FAILED,
	_ON_OFF_FAILED,
	_NOT_RECEIVED,
	_FECW_MISSMATCH,
	_FLAG_OK,
	_TRANSMIT_FAILED,
	_FAILED,
	_UNKNOWN
} _ACK;


#ifdef __cplusplus
 }
#endif

/************************************** END OF FILE **************************************/


#endif /* SAMPLEDRIVER_H_ */
