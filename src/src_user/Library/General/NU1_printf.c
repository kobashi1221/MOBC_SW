/**
  *****************************************************************************************
  * @file    : NU1_printf.c
  * @author  : Shinji MATSUZAWA
  * @version : 1.0
  * @date    : 2018/11/16
  * @brief   : 
  *****************************************************************************************
  *
  * COPYRIGHT(c) 2018 Aerospace Vehicle Dynamics Research Group, Nagoya University
  * 
  * explain
  * 
  * 
  * UPDATE HISTORY
  * Date        Rev   Editor          Description
  * 2018/11/16  1.0   S.MATSUZAWA     First Revision
  * 2020/12/03  2.0   N.KISHI         change file name "nucube1_printf" > "NU1_printf"
  *
  *****************************************************************************************
  */

/* Includes -----------------------------------------------------------------------------*/
#include "NU1_printf.h"

//#include "main.h" //#include "stm32f4xx_hal.h"

#include <stdio.h>

/* Function prototypes ------------------------------------------------------------------*/
// Global function (used in other files) prototypes must be written in "nucube1_printf.h"

/* Global variables (only used in this file)---------------------------------------------*/
//static UART_HandleTypeDef *sp_huart;
//UART_HandleTypeDef *sp_huart;

/* Global variables (used in other files)------------------------------------------------*/

/* Entities (i.e. classes, functions, ...)-----------------------------------------------*/
/**
  * @brief  : Initialize function to set the UART handle as INPUT/OUTPUT of stdio.
  * @param  : *huart -> pointer of the target UART handle
  * @retval : None
  */
void UARTPrintfInit(UART_HandleTypeDef *huart)
{
  // 引数のUARTハンドルをstdioのI/Oターゲットにセット．
  sp_huart = huart;

  // stdioを使うための処理．
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
}

/**
  * @brief  : Low level output function to send a character
  * @param  : ch -> send character
  * @retval : None
  */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
void __io_putchar(uint8_t ch)
{
  HAL_UART_Transmit_C2A(sp_huart, &ch, 1, 0xF);
}

/**
  * @brief  : Low level input function to receive a character
  * @param  : None
  * @retval : Received character
  */
#ifdef __GNUC__
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

GETCHAR_PROTOTYPE
{
  uint8_t data = 0;
  HAL_UART_Receive_C2A(sp_huart, &data, 1, 0xF);
  return data;
}


/* Template of function -----------------------------------------------------------------*/
/**
  * @name   :
  * @brief  :
  * @param  :
  * @retval :
  */
/*
int TemplateFunction(int val)
{
	// Private variables
	
	
	return 0;
}
*/

/************************************** END OF FILE **************************************/
