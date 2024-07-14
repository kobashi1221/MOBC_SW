/**
  *****************************************************************************************
  * @file    : IF_class.cpp
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/10/05
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  * Made based on the class file by S.MATSUZAWA.
  * General classes of interface.
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/10/05  1.0   N.KISHI         First revision
  * 2020/10/12  2.0   N.KISHI         Add some setters and Master T/R method of I2C
  *
  *****************************************************************************************
  */

#include "IF_class.h"
#include <stdint.h> //for uint8_t



// GPIO基底クラス
// 空のコンストラクタ．
GPIO_Class :: GPIO_Class(void) :
  p_gpio_port_(NULL),
  gpio_pin_(0x0000)
{
}

// コンストラクタ，GPIOポートとピンの設定を行う．
GPIO_Class :: GPIO_Class(void *p_gpio_port, uint16_t gpio_pin) :
  p_gpio_port_(p_gpio_port),
  gpio_pin_(gpio_pin)
{
}

// ピンのレベルをHIGHにする関数（OUTPUTに設定したピンでしか使わないこと）．
void GPIO_Class :: WriteH(void) {
	if(p_gpio_port_ != NULL){
		HAL_GPIO_WritePin_C2A(p_gpio_port_, gpio_pin_, GPIO_PIN_SET);
//		printf("GPIO High\r\n");
	}
}

// ピンのレベルをLOWにする関数（OUTPUTに設定したピンでしか使わないこと）．
void GPIO_Class :: WriteL(void) {
  if(p_gpio_port_ != NULL){
	  HAL_GPIO_WritePin_C2A(p_gpio_port_, gpio_pin_, GPIO_PIN_RESET);
//	  printf("GPIO Low\r\n");
  }
}
int GPIO_Class :: Write(uint8_t state){
	if(p_gpio_port_ != NULL){
		switch(state){
		case 0:
			HAL_GPIO_WritePin_C2A(p_gpio_port_, gpio_pin_, (GPIO_PinState)state);
		case 1:
			HAL_GPIO_WritePin_C2A(p_gpio_port_, gpio_pin_, (GPIO_PinState)state);
			return 0;
		default:
			return -1;
		}
	}
	return -2;
}

// ピンのHIGHとLOWを切り替える関数（OUTPUTに設定したピンでしか使わないこと）．
void GPIO_Class :: Toggle(void) {
  if(p_gpio_port_ != NULL){
	  HAL_GPIO_TogglePin_C2A(p_gpio_port_, gpio_pin_);
  }
}

// ピンのレベルを読み取る関数（INPUTに設定したピンでしか使わないこと）．
int GPIO_Class :: ReadPin(void) {
  if(p_gpio_port_ != NULL){
	  return (GPIO_PinState)HAL_GPIO_ReadPin_C2A(p_gpio_port_, gpio_pin_);
  }
}

// Getter of GPIO Port
void *GPIO_Class :: GetPort(void) {
  return p_gpio_port_;
}

// Getter of GPIO Pin Number
uint16_t GPIO_Class :: GetPin(void) {
  return gpio_pin_;
}

// Setter of GPIO Port
void GPIO_Class :: SetPort(void *p_gpio_port) {
	p_gpio_port_ = p_gpio_port;
}

// Setter of GPIO Pin Number
void GPIO_Class :: SetPin(uint16_t gpio_pin) {
	gpio_pin_ = gpio_pin;
}

// Setter of GPIO Port and Pin Number
void GPIO_Class :: SetPortAndPin(void *p_gpio_port, uint16_t gpio_pin) {
	p_gpio_port_ = p_gpio_port;
	gpio_pin_ = gpio_pin;
}

// デストラクタ，特に行う処理はない．
GPIO_Class :: ~GPIO_Class(void) {
}


// I2C基底クラス
// 空のコンストラクタ．
I2C_Class :: I2C_Class(void) :
  p_i2c_handle_(NULL),
  timeout_(0xFFFF)
{
}

// コンストラクタ，I2Cハンドルとタイムアウトの設定を行う．
I2C_Class :: I2C_Class(void *p_i2c_handle,
                                uint16_t device_address_read,
                                uint16_t device_address_write,
                                uint32_t timeout) :
  p_i2c_handle_(p_i2c_handle),
  device_address_read_(device_address_read),
  device_address_write_(device_address_write),
  timeout_(timeout)
{
}

// I2Cデバイスにアクセスし，値を取得する関数．
// device_address --> I2Cデバイスのアドレス
// memory_address --> I2Cデバイスのアクセスしたいメモリアドレス
// memory_address_size --> memory_addressのサイズ（単位：バイト）
// *p_data_buffer --> 取得する値を保存する先の変数のポインタ
// data_size --> 取得する値のサイズ（単位：バイト）
int I2C_Class :: Read(uint16_t memory_address,
                                    uint16_t memory_address_size,
                                     uint8_t *p_data_buffer,
                                    uint16_t data_size) {
  return (HAL_StatusTypeDef)HAL_I2C_Mem_Read_C2A(p_i2c_handle_,
                          device_address_read_,
                          memory_address,
                          memory_address_size,
                          p_data_buffer,
                          data_size,
                          timeout_);
}

// I2Cデバイスにアクセスし，値を書き込む関数．
// device_address --> I2Cデバイスのアドレス
// memory_address --> I2Cデバイスのアクセスしたいメモリアドレス
// memory_address_size --> memory_addressのサイズ（単位：バイト）
// *p_data_buffer --> 書き込む値を保存している変数のポインタ
// data_size --> 書き込む値のサイズ（単位：バイト）
int I2C_Class :: Write(uint16_t memory_address,
                                     uint16_t memory_address_size,
                                      uint8_t *p_data_buffer,
                                     uint16_t data_size) {
  return (HAL_StatusTypeDef)HAL_I2C_Mem_Write_C2A(p_i2c_handle_,
                           device_address_write_,
                           memory_address,
                           memory_address_size,
                           p_data_buffer,
                           data_size,
                           timeout_);
}

int I2C_Class :: MasterTransmit(uint8_t *p_data_buffer,
											  uint16_t data_size) {
  return (HAL_StatusTypeDef)HAL_I2C_Master_Transmit_C2A(p_i2c_handle_,
		  	  	  	  	  	     device_address_write_,
								 p_data_buffer,
								 data_size,
								 timeout_);
}

int I2C_Class :: MasterReceive(uint8_t *p_data_buffer,
											  uint16_t data_size) {
  return (HAL_StatusTypeDef)HAL_I2C_Master_Receive_C2A(p_i2c_handle_,
		  	  	  	  	  	     device_address_write_,
								 p_data_buffer,
								 data_size,
								 timeout_);
}

// 設定したI2Cハンドルを取得する関数．
void *I2C_Class :: GetHandle(void) {
  return p_i2c_handle_;
}

// 設定した読み込みデバイスアドレスを取得する関数．
uint16_t I2C_Class :: GetDeviceAddressRead(void) {
  return device_address_read_;
}

// 設定した書き込みデバイスアドレスを取得する関数．
uint16_t I2C_Class :: GetDeviceAddressWrite(void) {
  return device_address_write_;
}

// 設定したI2Cデバイスのタイムアウトを取得する関数．
uint32_t I2C_Class :: GetTimeout(void) {
  return timeout_;
}

//Setter of I2C handler
void I2C_Class :: SetHandle(void *p_i2c_handle) {
	p_i2c_handle_ = p_i2c_handle;
}

//Setter of Device Address for Read
void I2C_Class :: SetDeviceAddressRead(uint16_t device_address_read) {
	device_address_read_ = device_address_read;
}

//Setter of Device Address for Write
void I2C_Class :: SetDeviceAddressWrite(uint16_t device_address_write) {
	device_address_write_ = device_address_write;
}

//Setter of Timeout
void I2C_Class :: SetTimeout(uint32_t timeout) {
	timeout_ = timeout;
}

// デストラクタ，特に行う処理はない．
I2C_Class :: ~I2C_Class(void) {
}


// SPI基底クラス
// 空のコンストラクタ．
SPI_Class :: SPI_Class(void) :
  timeout_(0xFFFF)
{
}

// コンストラクタ，SPIハンドルとCS用のGPIOポートとピン，タイムアウトの設定を行う．
SPI_Class :: SPI_Class(void *p_spi_handle,
                            void *p_gpio_port,
                                uint16_t gpio_pin,
                                uint32_t timeout) :
  p_spi_handle_(p_spi_handle),
  p_gpio_port_(p_gpio_port),
  gpio_pin_(gpio_pin),
  timeout_(timeout)
{
}

// SPIデバイスにアクセスし，値を送信する関数．
// *p_data_buffer --> 送信する値を保存している変数のポインタ
// data_size --> 送信する値のサイズ（単位：バイト）
int SPI_Class :: Transmit(uint8_t *p_data_buffer, uint16_t data_size) {
  int ret_val;

  WriteL();
  ret_val = (HAL_StatusTypeDef)HAL_SPI_Transmit_C2A(p_spi_handle_, p_data_buffer, data_size, timeout_);
  WriteH();

  return ret_val;
}

// SPIデバイスにアクセスし，値を受信する関数．
// *p_data_buffer --> 受信する値を保存する先の変数のポインタ
// data_size --> 受信する値のサイズ（単位：バイト）
int SPI_Class :: Receive(uint8_t *p_data_buffer, uint16_t data_size) {
  int ret_val;

  WriteL();
  ret_val = (HAL_StatusTypeDef)HAL_SPI_Receive_C2A(p_spi_handle_, p_data_buffer, data_size, timeout_);
  WriteH();

  return ret_val;
}

// SPIデバイスにアクセスし，値を送受信する関数．
// *p_tx_data_buffer --> 送信する値を保存している変数のポインタ
// *p_rx_data_buffer --> 受信する値を保存する先の変数のポインタ
// data_size --> 送受信する値のサイズ（単位：バイト）
int SPI_Class :: TransmitReceive(uint8_t* p_tx_data_buffer, uint8_t* p_rx_dummy_buffer,
												uint8_t* p_rx_data_buffer,uint16_t dummy_size,
													uint16_t data_size) {
  int ret_val;
  WriteL();

  ret_val = (HAL_StatusTypeDef)HAL_SPI_TransmitReceive_C2A(p_spi_handle_, p_tx_data_buffer, p_rx_dummy_buffer, dummy_size, timeout_);
  ret_val = (HAL_StatusTypeDef)HAL_SPI_TransmitReceive_C2A(p_spi_handle_, p_tx_data_buffer, p_rx_data_buffer, data_size, timeout_);

  WriteH();

  return ret_val;
}

int SPI_Class :: TransmitReceive(uint8_t* p_tx_data_buffer,
												uint8_t* p_rx_data_buffer,
													uint16_t data_size) {
  int ret_val;

  WriteL();

  ret_val = (HAL_StatusTypeDef)HAL_SPI_TransmitReceive_C2A(p_spi_handle_, p_tx_data_buffer, p_rx_data_buffer, data_size, timeout_);

  WriteH();

  return ret_val;
}

// 設定したSPIハンドルを取得する関数．
void *SPI_Class :: GetHandle(void) {
  return p_spi_handle_;
}

// ピンのレベルをHIGHにする関数(CS用)．
void SPI_Class :: WriteH(void) {
  HAL_GPIO_WritePin_C2A(p_gpio_port_, gpio_pin_, GPIO_PIN_SET);
}

// ピンのレベルをLOWにする関数(CS用)．
void SPI_Class :: WriteL(void) {
  HAL_GPIO_WritePin_C2A(p_gpio_port_, gpio_pin_, GPIO_PIN_RESET);
}

// 設定したGPIOポートを取得する関数(CS用)．
void *SPI_Class :: GetPort(void) {
  return p_gpio_port_;
}

// 設定したGPIOのピンを取得する関数(CS用)．
uint16_t SPI_Class :: GetPin(void) {
  return gpio_pin_;
}

// 設定したSPIデバイスのタイムアウトを取得する関数．
uint32_t SPI_Class :: GetTimeout(void) {
  return timeout_;
}

// デストラクタ，特に行う処理はない．
SPI_Class :: ~SPI_Class(void) {
}


// UART基底クラス
// 空のコンストラクタ．
UART_Class :: UART_Class(void) :
  p_uart_handle_(NULL),
  timeout_(0xFFFF)
{
}

// コンストラクタ，UARTハンドルとタイムアウトの設定を行う．
UART_Class :: UART_Class(void *p_uart_handle, uint32_t timeout) :
  p_uart_handle_(p_uart_handle),
  timeout_(timeout)
{
}

// UARTデバイスにアクセスし，値を送信する関数．
// *p_data_buffer --> 送信する値を保存している変数のポインタ
// data_size --> 送信する値のサイズ（単位：バイト）
HAL_StatusTypeDef UART_Class :: Transmit(uint8_t *p_data_buffer, uint16_t data_size) {
  return (HAL_StatusTypeDef)HAL_UART_Transmit_C2A(p_uart_handle_, (uint8_t *)p_data_buffer, data_size, timeout_);
}

// UARTデバイスにアクセスし，値を受信する関数．
// *p_data_buffer --> 受信する値を保存する先の変数のポインタ
// data_size --> 受信する値のサイズ（単位：バイト）
HAL_StatusTypeDef UART_Class :: Receive(uint8_t *p_data_buffer, uint16_t data_size) {
  return (HAL_StatusTypeDef)HAL_UART_Receive_C2A(p_uart_handle_, p_data_buffer, data_size, timeout_);
}

HAL_StatusTypeDef UART_Class :: Receive_IT(uint8_t *p_data_buffer) {
  return (HAL_StatusTypeDef)HAL_UART_Receive_IT_C2A(p_uart_handle_, p_data_buffer, 1);
}

// 設定したUARTハンドルを取得する関数．
void *UART_Class :: GetHandle(void) {
  return p_uart_handle_;
}

// 設定したUARTデバイスのタイムアウトを取得する関数．
uint32_t UART_Class :: GetTimeout(void) {
  return timeout_;
}

//Setter of UART Handler
void UART_Class :: SetHandle(void *p_uart_handle) {
  p_uart_handle_ = p_uart_handle;
}

//Setter of UART Timeout
void UART_Class :: SetTimeout(uint32_t timeout){
  timeout_ = timeout;
}

//Setter of UART Handler and Timeout
void UART_Class :: SetHandleAndTimeout(void *p_uart_handle, uint32_t timeout){
  p_uart_handle_ = p_uart_handle;
  timeout_ = timeout;
}

// デストラクタ，特に行う処理はない．
UART_Class :: ~UART_Class(void) {
}
