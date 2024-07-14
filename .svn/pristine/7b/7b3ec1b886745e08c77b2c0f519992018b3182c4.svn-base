/**
  *****************************************************************************************
  * @file    : MRAM.c
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/09/30
  * @brief   :
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
  *
  * explain
  *
  *
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2020/09/30  1.0   N.KISHI         First revision
  * 2020/10/19  1.1   N.KISHI         1stEM CDH1 board test passed
  *
  *****************************************************************************************
  */

#include "MRAM.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h> //for uint8_t
#include <stdlib.h> //for malloc
#include <math.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_user/IfWrapper/HAL.h>
#include <src_core/System/TImeManager/time_manager.h>


static void *p_mram_spi_handle_;
static void *p_mram_gpio_port_;
static uint16_t gpio_pin_ = 0x00;

/**
 * @brief MRAM Write Enable
 *
 * @note
 * @param MRAM_STRUCT *my_MRAM : MRAM_STRUCT構造体へのポインタ
 * @return 0 ： 正常終了
 * @return 0以外 ： HAL_SPI
 */
HAL_StatusTypeDef MRAM_Enable(void)
{
	HAL_StatusTypeDef ret_HAL;

    uint8_t cmd = WREN;
    HAL_GPIO_WritePin_C2A(p_mram_gpio_port_, gpio_pin_, RESET);
    ret_HAL = HAL_SPI_Transmit_C2A(p_mram_spi_handle_, (uint8_t*)&cmd, (uint16_t)1, 0xff);
    HAL_GPIO_WritePin_C2A(p_mram_gpio_port_, gpio_pin_, SET);

    return ret_HAL;
}

/**
 * @brief MRAM Write Disable
 *
 * @note
 * @param MRAM_STRUCT *my_MRAM : MRAM_STRUCT構造体へのポインタ
 * @return 0 ： 正常終了
 * @return 0以外 ： HAL_SPI
 */
HAL_StatusTypeDef MRAM_Disable(void)
{
	HAL_StatusTypeDef ret_HAL;

    uint8_t cmd = WRDI;

    HAL_GPIO_WritePin_C2A(p_mram_gpio_port_, gpio_pin_, RESET);
    ret_HAL = HAL_SPI_Transmit_C2A(p_mram_spi_handle_, (uint8_t*)&cmd, (uint16_t)1, 0xff);
    HAL_GPIO_WritePin_C2A(p_mram_gpio_port_, gpio_pin_, SET);

    return ret_HAL;

}


//	*************************************************************************************************************	//
//												MRAM CLASS MEMBERS
//	*************************************************************************************************************	//
void MRAM_Init(void* spi_port, void* cs_port, uint16_t cs_pin){
	p_mram_spi_handle_ = spi_port;
	p_mram_gpio_port_ = cs_port;
	gpio_pin_ = cs_pin;
}


/*	@brief: headerをセットする関数
 * 	SendData: mramに送信する全体配列
 *	Cmd: デバイスで定義されたコマンド
 *	Address: 書き込みまたは読み込み時の先頭アドレス
 */
static void SetHeader(uint8_t* SendData, MRAM_CMD Cmd,uint32_t Address){
	SendData[0] = Cmd;											//Set Header
	SendData[1] = (uint8_t)((Address&0x00ff0000)>>16);
	SendData[2] = (uint8_t)((Address&0x0000ff00)>>8);
	SendData[3] = (uint8_t)(Address&0x000000ff);
}

/*	@brief: user dataをセットする関数
 *	SendData: mramに送信する全体配列
 *	Payload: 保存されるPayloadの先頭アドレス
 *	Size: Payload長[Bytes]
 */
static void SetPayload(uint8_t* SendData, uint8_t* Payload, size_t size){
	memcpy(&SendData[4], Payload, size);
}

/*	@brief: mram用の汎用write関数
 * 	WriteDataBuf: 書き込むPayloadの配列
 * 	DataSize: Payload長[Bytes]
 * 	WriteAddress: 書き込み先先頭アドレス
 */
HAL_StatusTypeDef MRAM_GeneralWrite(uint8_t* WriteDataBuf,
									size_t DataSize,
										uint32_t WriteAddress){
	HAL_StatusTypeDef ret_HAL;											//通信結果確認用変数

	//make entire command transmitted to MRAM by SPI
	size_t SendDataSize = DataSize + 4;							//　Payload長 + Header長
	uint8_t SendBuf[SendDataSize];			//　送信用の全体配列を確保

	SetHeader(SendBuf, WRITE, WriteAddress);					//　Headerを格納
	SetPayload(SendBuf, WriteDataBuf, DataSize);				//　Payloadを格納

	//transmit data to MRAM by SPI
	HAL_GPIO_WritePin_C2A(p_mram_gpio_port_, gpio_pin_, RESET);
	ret_HAL = HAL_SPI_Transmit_C2A(p_mram_spi_handle_, (uint8_t*)SendBuf, (uint16_t)SendDataSize, 0xff);	//	MRAMと通信
	HAL_GPIO_WritePin_C2A(p_mram_gpio_port_, gpio_pin_, SET);

	return ret_HAL;												//通信結果を返す
}

/*	@brief: mram用の汎用read関数
 * 	ReadDataBuf: 読み出しデータを格納するバッファ
 * 	DataSize: データサイズ[Bytes]
 * 	ReadAddress: 読み出し先頭アドレス
 */
HAL_StatusTypeDef MRAM_GeneralRead(uint8_t* ReadDataBuf,
									size_t DataSize,
										uint32_t ReadAddress){
	HAL_StatusTypeDef ret_HAL;														//　通信結果確認用変数

	uint8_t SendBuf[(DataSize+4)*sizeof(uint8_t)];		//　送信用の全体配列を確保
	uint8_t ReceiveBuf[(DataSize+4)*sizeof(uint8_t)];	//　読み出し用のバッファを確保

	//make entire command sent to MRAM by SPI
	SetHeader(SendBuf, READ, ReadAddress);									// Headerを格納


	//send mram a command, and receive data from mram
	HAL_GPIO_WritePin_C2A(p_mram_gpio_port_, gpio_pin_, RESET);
	ret_HAL = HAL_SPI_TransmitReceive_C2A(p_mram_spi_handle_, (uint8_t*)SendBuf, (uint8_t*)ReceiveBuf, (uint16_t)(DataSize+4), 0xff);		//mramと通信
	HAL_GPIO_WritePin_C2A(p_mram_gpio_port_, gpio_pin_, SET);

	//copy data read from MRAM to buffer given from argument
	memcpy(ReadDataBuf, &ReceiveBuf[4], DataSize);						//	 通信に成功したら格納，失敗したら破棄

	return ret_HAL;
}
