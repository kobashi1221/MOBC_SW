/*
 * MCP3421.cpp
 *
 *  Created on: 2020/04/08
 *      Author: K.Okada
 */

#include "MCP3421.h"

#include <cmath>


//--------------------------------------
//      Configuration Byte
//Default(power on reset) : 10010000
//--------------------------------------
//
//bit 7 RDY: Ready Bit
//	 his bit is the data ready flag. In read mode, this bit indicates if the output register has been updated
//	with a new conversion. In One-Shot Conversion mode, writing this bit to 驕ｯ�ｽｶ�ｿｽ�ｽｿ�ｽｽ1驕ｯ�ｽｶ�ｿｽ�ｽｿ�ｽｽ initiates a new conversion.
//	Reading RDY bit with the read command:
//		1 = Output register has not been updated.
//		0 = Output register has been updated with the latest conversion data.
//
//	Writing RDY bit with the write command:
//	Continuous Conversion mode: No effect
//	One-Shot Conversion mode:
//		1 = Initiate a new conversion.
//		0 = No effect.
//
//bit 6-5 C1-C0: Channel Selection Bits
//	These are the Channel Selection bits, but not used in the MCP3421 device.
//
//bit 4 O/C: Conversion Mode Bit
//	1 = Continuous Conversion Mode. Once this bit is selected, the device performs data conversions
//		continuously.
//	0 = One-Shot Conversion Mode. The device performs a single conversion and enters a low power
//		standby mode until it receives another write/read command.
//
//bit 3-2 S1-S0: Sample Rate Selection Bit
//	00 = 240 SPS (12 bits),
//	01 = 60 SPS (14 bits),
//	10 = 15 SPS (16 bits).
//	11 = 3.75 SPS (18 bits)
//
//bit 1-0 G1-G0: PGA Gain Selector Bits
//	00 = 1 V/V,
//	01 = 2 V/V,
//	10 = 4 V/V,
//	11 = 8 V/V

uint8_t I2C_ADDRESS;
void* hi2c;

MCP3421::MCP3421(uint8_t address, void* bus) {
	I2C_ADDRESS = address;
	hi2c = bus;

	// reset MCP3421 before initializing
	generalCallReset();

	uint8_t config[1] = { 0b00010000 }; //0001_0000 : CW conversion, 240sps(12bit, maximum code :2047, LSB:1mV),
	HAL_I2C_Master_Transmit_C2A(hi2c, I2C_ADDRESS, config, 1, 1000);

//	HAL_I2C_Master_Receive(&hi2c, I2C_ADDRESS, data, sizeof(data), 1000);
//	volatile uint8_t configurationByte1 = data[2];
//	volatile uint16_t dataBytes1 = *(uint16_t*) &data[0];
//	__asm__("NOP");

}

MCP3421::~MCP3421() {
}

void MCP3421::INIT(){
	uint8_t config[1] = { 0b00010000 }; //0001_0000 : CW conversion, 240sps(12bit, maximum code :2047, LSB:1mV),
	int ret=HAL_I2C_Master_Transmit_C2A(hi2c, I2C_ADDRESS, config, 1, 1000);
}

float MCP3421::readADC(uint32_t *rawData,HAL_StatusTypeDef *status) {
	uint8_t data[BIT_PACKET]; // 0-2:ADC data, 3:configuration byte//縺ｨ繧翫≠縺医★�ｼ托ｼ錬it縺ｮ縺ｨ縺阪�ｯ14縺ｯ辟｡隕悶＠縺ｦ繧�
	*status =(HAL_StatusTypeDef) HAL_I2C_Master_Receive_C2A(hi2c, I2C_ADDRESS+0x01, data, sizeof(data), 1000);
	float pga;
	uint8_t pgaConfig = data[BIT_PACKET-1] & 0x03;
	if (pgaConfig == 0x00)
		pga = 1.0;
	else if (pgaConfig == 0x01)
		pga = 2.0;
	else if (pgaConfig == 0x02)
		pga = 4.0;
	else
		pga = 8.0;

	float LSBs[4] = { 1.0e-3, 250.0e-6, 62.5e-6, 15.625e-6 };  //unit : [V]
	uint8_t resolutions[4] = { 12, 14, 16, 18 };
	uint8_t index = (data[BIT_PACKET-1] >> 2) & 0x03; //

//	uint32_t rawData;  // unit : [] ADC value
	if (index == 3) //18bit
		*rawData = data[0] * 0x10000 + data[1] * 0x100 + data[2];
	else
		//12,14,16bit
		*rawData = data[0] * 0x100 + data[1];  // ADC value

	uint32_t effectiveBitsMask = ((1 << resolutions[index]) - 1);
	*rawData = *rawData & effectiveBitsMask; // derive effective bits

	bool isMinus = *rawData & (1 << (resolutions[index] - 1)); // MSB=1 means negative voltage

	float ret;
	int32_t signedRawData;
	if (isMinus)
		signedRawData = -1.0 * ((~(*rawData)) & effectiveBitsMask) + 1;
	else
		signedRawData = *rawData;

	ret = (float) signedRawData * LSBs[index] / pga;
	return ret;

}

void MCP3421::generalCallReset() {
	//General call reset
	const uint16_t GENRAL_CALL_ADDRESS = 0x0000;
	const uint8_t GENRAL_CALL_RESET_COMMAND[1] = { 0x06 };
	HAL_I2C_Master_Transmit_C2A(hi2c, GENRAL_CALL_ADDRESS, (uint8_t*) GENRAL_CALL_RESET_COMMAND, 1, 1000);

}

