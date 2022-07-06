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

/*�}�N��*/
#define _BAUD(X) (uint16_t)((4 * F_CPU) / (double)(X))
#define USART_USE_INT	(1)	/*��M���荞�݂��g�p����*/

/*�Ώ�USART�I��p*/
enum Usart_Sel_e{
	USART_MIDI,
	USART_DEBUG	
};

/*�h���C�o�̏�����*/
void USART_Driver_Init(void);

/*USART����J�n*/
void USART_Driver_Begin(enum Usart_Sel_e);

/*USART�����~*/
void USART_Driver_Stop(enum Usart_Sel_e);

/*USART1�������M*/
void USART_Driver_Putc(enum Usart_Sel_e,const char);

/*USART1������M*/
void USART_Driver_Getc(enum Usart_Sel_e,char*);

/*printf�p���b�p*/
int printf_wrapper(char,FILE*);
#endif /* USART_DRIVER_H_ */