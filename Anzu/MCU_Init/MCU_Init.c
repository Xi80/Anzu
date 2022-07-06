/*
 * MCU_Init.c
 *
 * Created: 2022/07/07 7:04:29
 *  Author: luna
 */ 

#include "MCU_Init.h"

void MCU_Init(void){
	/*CPUクロックを20MHzに設定*/
	CCP = CCP_IOREG_gc;	/*保護レジスタへの書き込み*/
	CLKCTRL.MCLKCTRLB = 0x00;	/*分周を無効化*/
	
	/*ラウンドロビン割り込みを有効化*/
	CCP = CCP_IOREG_gc;	/*保護レジスタへの書き込み*/
	CPUINT.CTRLA |= CPUINT_LVL0RR_bm;	/*ラウンドロビン割り込み*/
		
	/*全体割り込みを許可*/
	sei();
	
	return;
}