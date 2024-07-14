/*
 * MCP3421.h
 *
 *  Created on: 2020/04/08
 *      Author: K.Okada
 */

#ifndef H_PROBE_MCP3421_H_
#define H_PROBE_MCP3421_H_

#ifdef __cplusplus
//extern "C" {
//#endif

#include <src_user/IfWrapper/HAL.h>
#include <src_core/System/TImeManager/time_manager.h>//for delay()

#define LPF_TIME_CONSTANT 0.90F
#define BIT_PACKET 3

class MCP3421 {
public:
	MCP3421(uint8_t address, void* bus);;
	~MCP3421();
	void INIT();
	float readADC(uint32_t *rawData,HAL_StatusTypeDef *status);
	void generalCallReset();

};

// Externをここに移動したらエラーが消えたが，理由は不明 2021/11/18 Otsuki
extern "C" {
#endif
//

#ifdef __cplusplus
}
#endif

#endif /* H_PROBE_MCP3421_H_ */
