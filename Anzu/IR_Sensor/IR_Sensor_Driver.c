/*
 * IR_Sensor_Driver.c
 *
 * Created: 2022/07/06 21:45:30
 *  Author: luna
 */ 

#include "IR_Sensor_Driver.h"

static uint8_t sens[8];

/*センサー駆動用割り込み*/
ISR(TCB1_INT_vect){
	for(uint8_t i = 0;i < 64;i++){
		PORTC.OUTCLR = 0x3F;
		PORTC.OUTSET = i;
		
		PORTE.OUTSET = 0x08;
		uint8_t arrPos = (i >> 3); /*アクセス先の配列の添え字*/
		uint8_t bitPos = 7 - (i - (arrPos << 3)); /*アクセス先のビット位置*/
		
		for(uint8_t i = 0;i < 4;i++)asm("nop");
		
		if(AC0.STATUS & AC_STATE_bm){
			/*入力あり*/
			sens[arrPos] |= (1 << bitPos);
		} else {
			/*入力なし*/
			sens[arrPos] &= ~(1 << bitPos);
		}
		
		PORTE.OUTCLR = 0x08;
	}
	TCB1.INTFLAGS |= TCB_CAPT_bm;	/*割り込みクリア*/
	return;
}

/*ドライバの初期化*/
void IR_Sensor_Driver_Init(void){
	/*GPIOの初期設定*/
	PORTC.DIRSET = 0x3F;	/*アドレス線*/
	PORTD.DIRCLR = 0x02;	/*AC入力*/
	PORTE.DIRSET = 0x08;	/*イネーブル*/
	
	PORTC.OUTCLR = 0x3F;
	PORTC.OUTSET = 0x00;	/*アドレスを0x00に*/
	
	PORTE.OUTCLR = 0x08;	/*イネーブルをネゲート*/
	
	/*TCB1の初期設定*/
	TCB1.CCMP = irFreq;			/*割り込み周期設定*/
	TCB1.CTRLA |= TCB_ENABLE_bm;	/*TCB1有効化*/
	
	/*AC0の初期設定*/
	AC0.MUXCTRLA |= (0x03 << 3) | (0x03);	/*正入力:AINP3(PD1),負入力:DACREF(内部基準電圧)*/
	AC0.CTRLA |= AC_ENABLE_bm;	/*AC0有効化*/
	
	return;
}

/*動作を開始*/
void IR_Sensor_Driver_Begin(void){
	TCB1.INTCTRL |= TCB_CAPT_bm;	/*TCB0割り込み許可*/
	
	return;
}

/*動作を停止*/
void IR_Sensor_Driver_Stop(void){
	TCB1.INTCTRL &= ~(TCB_CAPT_bm);	/*TCB0割り込み不許可*/	
	return;
}

/*センサーの値を取得*/
void IR_Sensor_Driver_Get_Value(uint8_t* dest){
	memcpy(dest,&sens[0],8);
	return;
}