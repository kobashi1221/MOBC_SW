/*
 * CRC_calc.c
 *
 *  Created on: 2021/11/04
 *      Author: daiki
 */

#include <stdint.h>
#include <stdio.h>
#include "CRC_calc.h"

const uint16_t XOR_HIGH_MASK_ = 4129U;
const uint16_t XOR_LOW_MASK_  = 0U;

//CRC7で利用する関数
uint8_t X( uint8_t crc )
{
    int bits = 8;
    while( bits-- ) {
        if( crc & 0x80 ) {   // 最上位ビットを見て１ならば、
            crc ^= 0x89;     //  生成多項式とXORする。
        }
        crc <<= 1;           // ビットシフト
    }
    return( crc );
}

//! CRC7を計算する
unsigned char calc_crc7(unsigned char *buff, unsigned int len )
{
    uint8_t crc = 0;

    while( len-- ) {
        crc ^= *buff++;          // 最初にXORしておく
        crc = X( crc );          // 関数化された演算処理
    }

    return( crc );          // 元の結果と同じになるように1bit戻す。
}


/**
 * CRCを計算する(バッファされたデータを一気に計算)
 *
 * CRC-16-xmodem計算
 * 生成多項式=x^16+x^12+x^5+1 (除数=0x1021)
 *
 * @param data データの先頭へのポインタ
 * @param length データの長さ(バイト数)
 * @return CRC値(16ビット)
 */
uint16_t calc_crc16(const unsigned char *src, unsigned int len)
{
	uint16_t shifter_ = 0x0000;
	for(unsigned int i=0; i<len; ++i)
	{
		unsigned char temp = src[i];
		for(unsigned int j=0; j<8; ++j)
		{
			unsigned short input = temp & 0x80;
			temp <<= 1; input <<= 8;

			unsigned short output = shifter_ & 0x8000;
			shifter_ <<= 1;

			input ^= output;
			shifter_ ^= (input ? XOR_HIGH_MASK_ : XOR_LOW_MASK_);
			}
	}
	return shifter_;
}

/**
 * CRCを計算する(あるCRC値から追記的に計算)
 *
 * CRC-16-xmodem計算
 * 生成多項式=x^16+x^12+x^5+1 (除数=0x1021)
 * 左送りの入出力非反転
 * @param data データの先頭へのポインタ
 * @param length データの長さ(バイト数)
 * @param *crc CRC　これに前回までのCRCを指定することで、追記的に大きいサイズのデータに対するCRC計算が可能
 */
void calc_crc16_update(const unsigned char *src, unsigned int len, uint16_t *crc)
{

	for(unsigned int i=0; i<len; ++i)
	{
		unsigned char temp = src[i];
		for(unsigned int j=0; j<8; ++j)
		{
			unsigned short input = temp & 0x80;
			temp <<= 1; input <<= 8;

			unsigned short output = *crc & 0x8000;
			*crc <<= 1;

			input ^= output;
			*crc ^= (input ? XOR_HIGH_MASK_ : XOR_LOW_MASK_);
		}
	}
}


