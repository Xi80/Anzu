/*
 * IR_Sensor_Driver.c
 *
 * Created: 2022/07/06 21:45:30
 *  Author: luna
 */ 

#include "IR_Sensor_Driver.h"

/*ドライバの初期化*/
void IR_Sensor_Driver_Init(void){
	/*GPIOの初期設定*/
	PORTC.DIRSET = 0x3F;	/*アドレス線*/
	PORTD.DIRCLR = 0x02;	/*AC入力*/
	PORTE.DIRSET = 0x08;	/*イネーブル*/
	
	PORTC.OUTCLR = 0x3F;
	PORTC.OUTSET = 0x00;	/*アドレスを0x00に*/
	
	PORTE.OUTCLR = 0x08;	/*イネーブルをネゲート*/
	
	/*AC0の初期設定*/
	AC0.MUXCTRLA |= (0x80) | (0x03 << 3) | (0x03);	/*出力反転、正入力:AINP3(PD1),負入力:DACREF(内部基準電圧)*/

	return;
}

/*動作を開始*/
void IR_Sensor_Driver_Begin(void){
	AC0.CTRLA |= AC_ENABLE_bm;	/*AC0有効化*/
	return;
}

/*動作を停止*/
void IR_Sensor_Driver_Stop(void){
	AC0.CTRLA &= ~(AC_ENABLE_bm);	/*AC0有効化*/
	return;
}

/*センサーの値を取得*/
uint8_t IR_Sensor_Driver_Get_Value(uint8_t idx){
	PORTC.OUT = idx & 0x3F;
	PORTE.OUTSET = 0x08;	/*チップセレクトロジックを有効化*/	
	if(AC0.STATUS & AC_STATE_bm){
		/*入力あり*/
		return 1;
	} else {
		/*入力なし*/
		return 0;
	}
	PORTE.OUTCLR = 0x08;	/*チップセレクトロジックを無効化*/
}