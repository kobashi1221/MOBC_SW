/**
  *****************************************************************************************
  * @file    : MCP23017.cpp
  * @author  : Kota OKADA
  * @version : 1.0
  * @date    : 2020/07/07
  * @brief   : IOEXPANDER (MCP23017) Driver for NUcube1 MAGNARO
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/07/07  1.0   K.OKADA         First revision
  * 2020/12/01  2.0   N.KISHI         Add setDeviceAddress(), setHandleI2C()
  * 									getDeviceAddress() and getHandleI2C()
  *
  *****************************************************************************************
  */

#include "MCP23017.h"

#define HIGH 0x01
#define LOW 0x00

void bitSet(uint8_t &reg, uint8_t bit) {
	reg |= 0x01 << bit;
}
void bitClear(uint8_t &reg, uint8_t bit) {
	reg &= ~(0x01 << bit);
}
bool bitRead(uint8_t &reg, uint8_t bit) {
	return (reg >> bit) & 0x01;
}
uint8_t lowByte(uint16_t word) {
	return (uint8_t) (word & 0xff);
}
uint8_t highByte(uint16_t word) {
	return (uint8_t) ((word >> 0x08) & 0xff);
}

//empty constructor kishi
MCP23017::MCP23017() : _deviceAddr(0)
{
}

MCP23017::MCP23017(uint8_t address, void* bus) {
	_deviceAddr = address;
	_bus = bus;
}

MCP23017::~MCP23017() {
}

//getter of device address
uint8_t MCP23017::getDeviceAddress(void) {
	return _deviceAddr;
}

//getter of i2c handler
void* MCP23017::getHandleI2C(void) {
	return _bus;
}

//setter of device address
void MCP23017::setDeviceAddress(uint8_t address) {
	_deviceAddr = address;
}

//setter of i2c handler
void MCP23017::setHandleI2C(void* bus) {
	_bus = bus;
}

void MCP23017::init() {
	//BANK = 	0 : sequential register addresses
	//MIRROR = 	0 : use configureInterrupt 
	//SEQOP = 	1 : sequential operation disabled, address pointer does not increment
	//DISSLW = 	0 : slew rate enabled
	//HAEN = 	0 : hardware address pin is always enabled on 23017
	//ODR = 	0 : open drain output
	//INTPOL = 	0 : interrupt active low

	HAL_I2C_DeInit_C2A(&hi2c2);
	HAL_I2C_Init_C2A(&hi2c2);

	writeRegister(MCP23017Register::IOCON, 0b00000000);

	//enable all pull up resistors (will be effective for input pins only)
//	writeRegister(MCP23017Register::GPPU_A, 0xFF, 0xFF);

	portMode(MCP23017Port::A, 0);      	//Port A as output
	portMode(MCP23017Port::B, 0);      	//Port B as output
	write(LOW);							//All output pins are turned off
}

MCP23017PortACK MCP23017::portMode(MCP23017Port port, uint8_t directions, uint8_t pullups, uint8_t inverted) {
	uint8_t temp;
	temp = (uint8_t)writeRegister(MCP23017Register::IODIR_A + port, directions);
	temp = (uint8_t)writeRegister(MCP23017Register::GPPU_A + port, pullups);
	MCP23017PortACK ret = (MCP23017PortACK)temp;
	return ret;
//	writeRegister(MCP23017Register::IPOL_A + port, inverted); It's not required for usual use. Noro Comment out
}

MCP23017PortACK MCP23017::pinMode(uint8_t pin, uint8_t mode, bool inverted) {
	MCP23017Register iodirreg = MCP23017Register::IODIR_A;
	MCP23017Register pullupreg = MCP23017Register::GPPU_A;
	MCP23017Register polreg = MCP23017Register::IPOL_A;
	uint8_t iodir, pol, pull;

	if (pin > 7) {
		iodirreg = MCP23017Register::IODIR_B;
		pullupreg = MCP23017Register::GPPU_B;
		polreg = MCP23017Register::IPOL_B;
		pin -= 8;
	}

	iodir = readRegister(iodirreg);
	if (mode == INPUT || mode == INPUT_PULLUP)
		bitSet(iodir, pin);
	else
		bitClear(iodir, pin);

	pull = readRegister(pullupreg);
	if (mode == INPUT_PULLUP)
		bitSet(pull, pin);
	else
		bitClear(pull, pin);

	pol = readRegister(polreg);
	if (inverted)
		bitSet(pol, pin);
	else
		bitClear(pol, pin);

	uint8_t  temp;
	temp = (uint8_t)writeRegister(iodirreg, iodir);
	temp += (uint8_t)writeRegister(pullupreg, pull);
	temp += (uint8_t)writeRegister(polreg, pol);
	MCP23017PortACK ret = (MCP23017PortACK)temp;
	return ret;
}

MCP23017PortACK MCP23017::digitalWrite(uint8_t pin, uint8_t state) {
	MCP23017Register gpioreg = MCP23017Register::GPIO_A;
	uint8_t gpio;
	if (pin > 7) {
		gpioreg = MCP23017Register::GPIO_B;
		pin -= 8;
	}
	gpio = readRegister(gpioreg);
	if (state == HIGH)
		bitSet(gpio, pin);
	else
		bitClear(gpio, pin);

	MCP23017PortACK ret = writeRegister(gpioreg, gpio);
	return ret;
}

bool MCP23017::digitalRead(uint8_t pin) {
	MCP23017Register gpioreg = MCP23017Register::GPIO_A;
	uint8_t gpio;
	if (pin > 7) {
		gpioreg = MCP23017Register::GPIO_B;
		pin -= 8;
	}
	gpio = readRegister(gpioreg);
	if (bitRead(gpio, pin)){
		return HIGH;}
	return LOW;
}

void MCP23017::writePort(MCP23017Port port, uint8_t value) {
	writeRegister(MCP23017Register::GPIO_A + port, value);

}

void MCP23017::write(uint16_t value) {
	writeRegister(MCP23017Register::GPIO_A, lowByte(value), highByte(value));
}

uint8_t MCP23017::readPort(MCP23017Port port) {
	return readRegister(MCP23017Register::GPIO_A + port);
}

uint16_t MCP23017::read() {
	uint8_t a = readPort(MCP23017Port::A);
	uint8_t b = readPort(MCP23017Port::B);

	return a | b << 8;
}

MCP23017PortACK MCP23017::writeRegister(MCP23017Register reg, uint8_t value) {
	uint8_t temp;
	temp= HAL_I2C_Mem_Write_C2A(_bus, _deviceAddr, *(uint16_t*) &reg, I2C_MEMADD_SIZE_8BIT, &value, 0x01, 100);
	MCP23017PortACK ret = (MCP23017PortACK)temp;
	return ret;
//	_bus->beginTransmission(_deviceAddr);
//	_bus->write(static_cast<uint8_t>(reg));
//	_bus->write(value);
//	_bus->endTransmission();
}

MCP23017PortACK MCP23017::writeRegister(MCP23017Register reg, uint8_t portA, uint8_t portB) {
	uint8_t value[] = { portA, portB };
	uint8_t temp;
	temp = HAL_I2C_Mem_Write_C2A(_bus, _deviceAddr, *(uint16_t*) &reg, I2C_MEMADD_SIZE_8BIT, value, 0x02, 100);
	MCP23017PortACK ret = (MCP23017PortACK)temp;
	return ret;
//	_bus->beginTransmission(_deviceAddr);
//	_bus->write(static_cast<uint8_t>(reg));
//	_bus->write(portA);
//	_bus->write(portB);
//	_bus->endTransmission();
}

uint8_t MCP23017::readRegister(MCP23017Register reg) {
	uint8_t value[1];
	HAL_I2C_Mem_Read_C2A(_bus, (_deviceAddr)+0x01, *(uint16_t*) &reg, I2C_MEMADD_SIZE_8BIT, value, 1, 100);
	return *value;
//	_bus->beginTransmission(_deviceAddr);
//	_bus->write(static_cast<uint8_t>(reg));
//	_bus->endTransmission();
//	_bus->requestFrom(_deviceAddr, (uint8_t) 1);
//	return _bus->read();
}

void MCP23017::readRegister(MCP23017Register reg, uint8_t &portA, uint8_t &portB) {
	uint8_t value[2];
	HAL_I2C_Mem_Read_C2A(_bus, (_deviceAddr)+0x01, *(uint8_t*) &reg, I2C_MEMADD_SIZE_8BIT, value, 2, 100);
	portA = value[0];
	portB = value[1];

//	_bus->beginTransmission(_deviceAddr);
//	_bus->write(static_cast<uint8_t>(reg));
//	_bus->endTransmission();
//	_bus->requestFrom(_deviceAddr, (uint8_t) 2);
//	portA = _bus->read();
//	portB = _bus->read();
}

#ifdef _MCP23017_INTERRUPT_SUPPORT_

void MCP23017::interruptMode(MCP23017InterruptMode intMode) {
	uint8_t iocon = readRegister(MCP23017Register::IOCON);
	if (intMode == MCP23017InterruptMode::Or)
		iocon |= static_cast<uint8_t>(MCP23017InterruptMode::Or);
	else
		iocon &= ~(static_cast<uint8_t>(MCP23017InterruptMode::Or));

	writeRegister(MCP23017Register::IOCON, iocon);
}

void MCP23017::interrupt(MCP23017Port port, uint8_t mode) {
	MCP23017Register defvalreg = MCP23017Register::DEFVAL_A + port;
	MCP23017Register intconreg = MCP23017Register::INTCON_A + port;

	//enable interrupt for port
	writeRegister(MCP23017Register::GPINTEN_A + port, 0xFF);
	switch (mode) {
	case CHANGE:
		//interrupt on change
		writeRegister(intconreg, 0);
		break;
	case FALLING:
		//interrupt falling : compared against defval, 0xff
		writeRegister(intconreg, 0xFF);
		writeRegister(defvalreg, 0xFF);
		break;
	case RISING:
		//interrupt rising : compared against defval, 0x00
		writeRegister(intconreg, 0xFF);
		writeRegister(defvalreg, 0x00);
		break;
	}
}

void MCP23017::interruptedBy(uint8_t &portA, uint8_t &portB) {
	readRegister(MCP23017Register::INTF_A, portA, portB);
}

void MCP23017::disableInterrupt(MCP23017Port port) {
	writeRegister(MCP23017Register::GPINTEN_A + port, 0x00);
}

void MCP23017::clearInterrupts() {
	uint8_t a, b;
	clearInterrupts(a, b);
}

void MCP23017::clearInterrupts(uint8_t &portA, uint8_t &portB) {
	readRegister(MCP23017Register::INTCAP_A, portA, portB);
}

#endif





