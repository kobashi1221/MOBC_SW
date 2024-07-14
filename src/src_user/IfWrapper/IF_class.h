/**
  *****************************************************************************************
  * @file    : IF_class.h
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

#ifndef IF_CLASS_H_
#define IF_CLASS_H_

#include <stdint.h> //for uint8_t
#include "HAL.h"


using namespace std;

// GPIO General Class
class GPIO_Class{
  private:

  protected:
    void *p_gpio_port_;
    uint16_t gpio_pin_;

  public:
    GPIO_Class(void);
    GPIO_Class(void *p_gpio_port, uint16_t gpio_pin); //コンストラクタ
    void WriteH(void);
    void WriteL(void);
    int Write(uint8_t);
    void Toggle(void);
    int ReadPin(void);
    void *GetPort(void);
    uint16_t GetPin(void);
    void SetPort(void *p_gpio_port);
    void SetPin(uint16_t gpio_pin);
    void SetPortAndPin(void *p_gpio_port, uint16_t gpio_pin);
    ~GPIO_Class(void);
};

// I2C General Class
class I2C_Class {
  private:

  protected:
    void *p_i2c_handle_;
    uint16_t device_address_read_;
    uint16_t device_address_write_;
    uint32_t timeout_;

  public:
    I2C_Class(void);
    I2C_Class(void *p_i2c_handle,
                       uint16_t device_address_read,
                       uint16_t device_address_write,
                       uint32_t timeout); //コンストラクタ
    int Read(uint16_t memory_address,
                           uint16_t memory_address_size,
                            uint8_t *p_data_buffer,
                           uint16_t data_size);
    int Write(uint16_t memory_address,
                            uint16_t memory_address_size,
                             uint8_t *p_data_buffer,
                            uint16_t data_size);
    int MasterTransmit(uint8_t *p_data_buffer, uint16_t data_size);
    int MasterReceive(uint8_t *p_data_buffer, uint16_t data_size);
    void *GetHandle(void);
    uint16_t GetDeviceAddressRead(void);
    uint16_t GetDeviceAddressWrite(void);
    uint32_t GetTimeout(void);
    void SetHandle(void *p_i2c_handle);
    void SetDeviceAddressRead(uint16_t device_address_read);
    void SetDeviceAddressWrite(uint16_t device_address_write);
    void SetTimeout(uint32_t timeout);
    ~I2C_Class(void);
};

// SPI General Class
class SPI_Class {
  private:

  protected:
    void *p_spi_handle_;
    void *p_gpio_port_;
    uint16_t gpio_pin_;
    uint32_t timeout_;

  public:
    SPI_Class(void);
    SPI_Class(void *p_spi_handle,
                   void *p_gpio_port,
                       uint16_t gpio_pin,
                       uint32_t timeout); //コンストラクタ
    int Transmit(uint8_t *p_data_buffer, uint16_t data_size);
    int Receive(uint8_t *p_data_buffer, uint16_t data_size);
    int TransmitReceive(uint8_t* p_tx_data_buffer, uint8_t* p_rx_dummy_buffer,
    									uint8_t* p_rx_data_buffer, uint16_t dummy_size,
											uint16_t data_size);
    int TransmitReceive(uint8_t* p_tx_data_buffer,uint8_t* p_rx_data_buffer,
											uint16_t data_size);
    void *GetHandle(void);

    void WriteH(void);
    void WriteL(void);
    void Toggle(void);
    void *GetPort(void);
    uint16_t GetPin(void);

    uint32_t GetTimeout(void);
    ~SPI_Class(void);
};

// UART General Class
class UART_Class {
  private:

  protected:
    void *p_uart_handle_;
    uint32_t timeout_;

  public:
    UART_Class(void);
    UART_Class(void *p_uart_handle, uint32_t timeout); //コンストラクタ
    HAL_StatusTypeDef Transmit(uint8_t *p_data_buffer, uint16_t data_size);
    HAL_StatusTypeDef Receive(uint8_t *p_data_buffer, uint16_t data_size);
    HAL_StatusTypeDef Receive_IT(uint8_t *p_data_buffer);
    void *GetHandle(void);
    uint32_t GetTimeout(void);
    void SetHandle(void *p_uart_handle);
    void SetTimeout(uint32_t timeout);
    void SetHandleAndTimeout(void *p_uart_handle, uint32_t timeout);
    ~UART_Class(void);
};

#ifdef __cplusplus
 extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif // IF_CLASS_H_
