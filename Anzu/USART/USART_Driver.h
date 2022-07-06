/*
 * USART_Driver.h
 *
 * Created: 2022/07/07 7:09:50
 *  Author: luna
 */ 


#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

#include "RingBuffer.h"

/*マクロ*/
#define _BAUD(X) (uint16_t)((4 * F_CPU) / (double)(X))
#define USART_USE_INT	(1)	/*受信割り込みを使用する*/

/*対象USART選択用*/
enum Usart_Sel_e{
	USART_MIDI,
	USART_DEBUG	
};

/*ドライバの初期化*/
void USART_Driver_Init(void);

/*USART動作開始*/
void USART_Driver_Begin(enum Usart_Sel_e);

/*USART動作停止*/
void USART_Driver_Stop(enum Usart_Sel_e);

/*USART1文字送信*/
void USART_Driver_Putc(enum Usart_Sel_e,const char);

/*USART1文字受信*/
void USART_Driver_Getc(enum Usart_Sel_e,char*);

/*printf用ラッパ*/
int printf_wrapper(char,FILE*);
#endif /* USART_DRIVER_H_ */