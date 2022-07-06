/*
 * MCU_Init.c
 *
 * Created: 2022/07/07 7:04:29
 *  Author: luna
 */ 

#include "MCU_Init.h"

void MCU_Init(void){
	/*CPU�N���b�N��20MHz�ɐݒ�*/
	CCP = CCP_IOREG_gc;	/*�ی샌�W�X�^�ւ̏�������*/
	CLKCTRL.MCLKCTRLB = 0x00;	/*�����𖳌���*/
	
	/*���E���h���r�����荞�݂�L����*/
	CCP = CCP_IOREG_gc;	/*�ی샌�W�X�^�ւ̏�������*/
	CPUINT.CTRLA |= CPUINT_LVL0RR_bm;	/*���E���h���r�����荞��*/
		
	/*�S�̊��荞�݂�����*/
	sei();
	
	return;
}