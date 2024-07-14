/**
  *****************************************************************************************
  * @file    : CDH1_debug_EEPROM.c
  * @author  : Nobuki KISHI
  * @version : 1.0
  * @date    : 2020/06/28
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
  * 2020/06/28  1.0   N.KISHI         First revision
  *
  *****************************************************************************************
  */

#include "MEM_cmd_EEPROM.h"

#include <src_user/IfWrapper/HAL.h>
#include <stdio.h> //for printf
#include <string.h>
#include <src_user/Applications/Middleware/MemoryMW.h>
#include <src_core/System/TimeManager/time_manager.h>
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include "../GlobalMemoryParam.h"
#include <src_user/Library/Delay.h>
#include <src_core/Library/endian.h>
#include <src_user/Drivers/Cdh/EEPROM/EEPROM.h>
#include <src_user/Library/General/DBG_printf.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

static void I2C1_ClearBusyFlagErratum(I2C_HandleTypeDef *instance);

extern I2C_HandleTypeDef hi2c1;

//メモリーパトロールのアドレス
static uint32_t EEPROMPatrolAddr = 0;
static uint8_t EEPROM_ReadSize;
const uint8_t EEPROM_ReadSize_ = 10;

static void eeprom_load_param(void);
static void eeprom_initialize_param(void);
static void eeprom_memory_patrol(void);

AppInfo EEPROM_param_update(void)
{
    return AI_create_app_info("sample_param_init", eeprom_load_param, eeprom_initialize_param);
}

AppInfo EEPROM_memory_patorol(void)
{
    return AI_create_app_info("sample_param_init", NULL, eeprom_memory_patrol);
}


static void eeprom_load_param(void){
	read_param(&EEPROM_ReadSize, &EEPROM_ReadSize_, 1, 0x00003801);
}

static void eeprom_initialize_param(void){
	write_param(&EEPROM_ReadSize, &EEPROM_ReadSize_, 1, 0x00003801);	//write_load
}

static void eeprom_memory_patrol(void){

	if(EEPROM_ReadSize == 0) return;

	//ダミーバッファを用意
	uint8_t dummy[EEPROM_ReadSize];
	//パトロールするアドレス範囲を算出
	uint32_t _eaddr = EEPROMPatrolAddr + EEPROM_ReadSize;

	//最大使用領域を超える場合
	if(_eaddr >= 0xAAAA){
		MRAM_read_param(dummy,NULL, EEPROM_ReadSize - (_eaddr - 0xAAA9), EEPROMPatrolAddr);

		MRAM_read_param(dummy,NULL, (_eaddr - 0xAAA9), 0x00000000);
		EEPROMPatrolAddr = (_eaddr - 0xAAA9);
	}
	//最大使用領域を超えない場合
	else{
		MRAM_read_param(dummy,NULL, EEPROM_ReadSize, EEPROMPatrolAddr);
		EEPROMPatrolAddr += EEPROM_ReadSize;
		if(EEPROMPatrolAddr >= 0xAAAA) EEPROMPatrolAddr = 0;
	}
}


/**
 * @brief	write command from EEPROM.
 * packet[15~17]: 	write address
 * packet[18~]: 	data to write
 *
 */
CCP_CmdRet Cmd_MEM_eeprom_writeData(const CommonCmdPacket* packet)
{
	size_t param_len;

    if ((param_len = CCP_get_param_len(packet)) < 5)
    {
        // パラメータはブロック番号1Byte。
        return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
    }
    uint8_t DataSize = param_len - 4;
    uint8_t writedata[DataSize];
    uint8_t* param_head = (uint8_t*)CCP_get_param_head(packet);
    uint32_t Address = 0;

    ENDIAN_memcpy(&Address, param_head, 4);
    memcpy(writedata, &param_head[4], DataSize);

    EEPROM_write_param(writedata, NULL, DataSize, Address);

	print(EEPROM, "writeaddress: %08x", Address);
	print(EEPROM, "writedata: ");
	for(int i = 0; i<DataSize; ++i){
        print(EEPROM, "%02x ",writedata[i]);
    }
	print(EEPROM,"\r\n");

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/*
 * @brief	read command from EEPROM.
 * param[0]~param[3]:	readaddress (BIG Endian)
 * param[4]: 			datasize
 *
 */
CCP_CmdRet Cmd_MEM_eeprom_ReadData(const CommonCmdPacket* packet)
{
	int ret;
	size_t param_len;

	 if ((param_len = CCP_get_param_len(packet)) != 5) //その場しのぎだがpacketlengthを足した
	{
		// パラメータはブロック番号1Byte。
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
	}

	 uint8_t DataSize = CCP_get_param_head(packet)[4];
	 uint8_t readbuf[DataSize];
	 uint32_t Address;
	 ENDIAN_memcpy(&Address, CCP_get_param_head(packet), 4);
	 EEPROM_read_param(readbuf, NULL, DataSize, Address);
	 print(EEPROM, "ReadData: ");
	 for(int i=0; i<DataSize; ++i){
		print(EEPROM, "%02x ",readbuf[i]);
	 }
	 print(EEPROM, "\r\n");

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


/*	@brief: パトロールアドレスの変更コマンド
 * 	packet[15~18]: アドレス（ビッグエンディアン）
 */
CCP_CmdRet Cmd_MEM_eeprom_set_patrol_addr(const CommonCmdPacket* packet){
	uint32_t temp;
	//パラメータを受信
	ENDIAN_memcpy(&temp, CCP_get_param_head(packet), 4);
	//不正なアドレスを除去
	if(temp > 0xAAAA){
		return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
	}

	//正常なアドレスであれば変更を適用する
	EEPROMPatrolAddr = temp;

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_MEM_eeprom_reset(const CommonCmdPacket* packet)
{
	int ret1 = HAL_GPIO_ReadPin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin);

	printf("ret1 = : %02x \r\n", ret1);

	int ret2 = HAL_GPIO_ReadPin_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, SDA_I2C3V3_MOBCtoEROM_Pin);

	printf("ret2 = : %02x \r\n", ret2);

	I2C1_ClearBusyFlagErratum(&hi2c1);

	ret1 = HAL_GPIO_ReadPin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin);

	printf("ret1 = : %02x \r\n", ret1);

	ret2 = HAL_GPIO_ReadPin_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, SDA_I2C3V3_MOBCtoEROM_Pin);

	printf("ret2 = : %02x \r\n", ret2);

	return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

void I2C1_ClearBusyFlagErratum(I2C_HandleTypeDef *instance)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    int timeout =200;
    int timeout_cnt=0;

    // 1. Clear PE bit.
    instance->Instance->CR1 &= ~(0x0001);

    //  2. Configure the SCL and SDA I/Os as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
    GPIO_InitStruct.Mode         = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Alternate    = GPIO_AF4_I2C3;
    GPIO_InitStruct.Pull         = GPIO_PULLUP;
    GPIO_InitStruct.Speed        = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin          = SCL_I2C3V3_MOBCtoEROM_Pin;
    HAL_GPIO_Init_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin          = SDA_I2C3V3_MOBCtoEROM_Pin;
    HAL_GPIO_Init_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, SDA_I2C3V3_MOBCtoEROM_Pin, GPIO_PIN_SET);


    // 3. Check SCL and SDA High level in GPIOx_IDR.
    while (GPIO_PIN_SET != HAL_GPIO_ReadPin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin))
    {
        timeout_cnt++;
        if(timeout_cnt>timeout)
            return;
    }

    while (GPIO_PIN_SET != HAL_GPIO_ReadPin_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, SDA_I2C3V3_MOBCtoEROM_Pin))
    {
        //Move clock to release I2C
        HAL_GPIO_WritePin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin, GPIO_PIN_RESET);
        asm("nop");
        HAL_GPIO_WritePin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin, GPIO_PIN_SET);

        timeout_cnt++;
        if(timeout_cnt>timeout)
            return;
    }

    // 4. Configure the SDA I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
    HAL_GPIO_WritePin_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, SDA_I2C3V3_MOBCtoEROM_Pin, GPIO_PIN_RESET);

    //  5. Check SDA Low level in GPIOx_IDR.
    while (GPIO_PIN_RESET != HAL_GPIO_ReadPin_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, SDA_I2C3V3_MOBCtoEROM_Pin))
    {
        timeout_cnt++;
        if(timeout_cnt>timeout)
            return;
    }

    // 6. Configure the SCL I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
    HAL_GPIO_WritePin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin, GPIO_PIN_RESET);

    //  7. Check SCL Low level in GPIOx_IDR.
    while (GPIO_PIN_RESET != HAL_GPIO_ReadPin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin))
    {
        timeout_cnt++;
        if(timeout_cnt>timeout)
            return;
    }

    // 8. Configure the SCL I/O as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
    HAL_GPIO_WritePin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin, GPIO_PIN_SET);

    // 9. Check SCL High level in GPIOx_IDR.
    while (GPIO_PIN_SET != HAL_GPIO_ReadPin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin))
    {
        timeout_cnt++;
        if(timeout_cnt>timeout){
            return;
        }
    }

    // 10. Configure the SDA I/O as General Purpose Output Open-Drain , High level (Write 1 to GPIOx_ODR).
    HAL_GPIO_WritePin_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, SDA_I2C3V3_MOBCtoEROM_Pin, GPIO_PIN_SET);

    // 11. Check SDA High level in GPIOx_IDR.
    while (GPIO_PIN_SET != HAL_GPIO_ReadPin_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, SDA_I2C3V3_MOBCtoEROM_Pin))
    {
        timeout_cnt++;
        if(timeout_cnt>timeout)
            return;
    }

    // 12. Configure the SCL and SDA I/Os as Alternate function Open-Drain.
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;

    GPIO_InitStruct.Pin = SCL_I2C3V3_MOBCtoEROM_Pin;
    HAL_GPIO_Init_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SDA_I2C3V3_MOBCtoEROM_Pin;
    HAL_GPIO_Init_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, &GPIO_InitStruct);

    HAL_GPIO_WritePin_C2A(SCL_I2C3V3_MOBCtoEROM_GPIO_Port, SCL_I2C3V3_MOBCtoEROM_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin_C2A(SDA_I2C3V3_MOBCtoEROM_GPIO_Port, SDA_I2C3V3_MOBCtoEROM_Pin, GPIO_PIN_SET);

    // 13. Set SWRST bit in I2Cx_CR1 register.
    instance->Instance->CR1 |= 0x8000;

    asm("nop");

    // 14. Clear SWRST bit in I2Cx_CR1 register.
    instance->Instance->CR1 &= ~0x8000;

    asm("nop");

    // 15. Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register
    instance->Instance->CR1 |= 0x0001;

    // Call initialization function.
    HAL_I2C_Init_C2A(instance);
    printf("I2C setting Initialize! \r\n");
}
