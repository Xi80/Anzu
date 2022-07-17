/*
 * IR_Keyboard_Controller.c
 *
 * Created: 2022/07/08 9:28:19
 *  Author: luna
 */ 

#include "IR_Keyboard_Controller.h"

static uint8_t prev[3];

/*コールバック関数*/
void (*IR_Keyboard_Controller_Callback_Note_On)(uint8_t) = NULL;

void (*IR_Keyboard_Controller_Callback_Note_Off)(uint8_t) = NULL;


/*キーボード入力割り込み*/
ISR(TCB1_INT_vect){
	
	for(uint8_t idx = 0;idx < 24;idx++){
		uint8_t cur = IR_Sensor_Driver_Get_Value(idx * 2) & IR_Sensor_Driver_Get_Value(idx * 2 + 1);
		uint8_t prv = (prev[idx / 8] & (1 << (idx % 8)))? 1 : 0;
		if(prv){
			/*前回スキャン時にON*/
			if(prv != cur){
				/*ノートオフ*/
				if(IR_Keyboard_Controller_Callback_Note_Off != NULL)IR_Keyboard_Controller_Callback_Note_Off(idx);
			}
		} else {
			/*前回スキャン時にOFF*/
			if(prv != cur){
				/*ノートオン*/
				if(IR_Keyboard_Controller_Callback_Note_On != NULL)IR_Keyboard_Controller_Callback_Note_On(idx);
			}
		}
		
		
		if(cur){
			prev[idx / 8] |= (1 << (idx % 8));
		} else {
			prev[idx / 8] &= ~(1 << (idx % 8));
		}
	}
	
	TCB1.INTFLAGS = TCB_CAPT_bm;	/*割り込みフラグをクリア*/
}

/*コントローラの初期化*/
void IR_Keyboard_Controller_Init(void){
	IR_Sensor_Driver_Init();

	/*TCB1初期設定*/
	TCB1.CCMP = keyFreq;			/*割り込み周期設定*/
	TCB1.CTRLA |= (0x01 << 1) | TCB_ENABLE_bm;	/*TCB1有効化*/
}

/*動作を開始*/
void IR_Keyboard_Controller_Begin(void){
	IR_Sensor_Driver_Begin();
	TCB1.INTCTRL = TCB_CAPT_bm;
}

/*動作を停止*/
void IR_Keyboard_Controller_Stop(void){
	IR_Sensor_Driver_Stop();
	TCB1.INTCTRL &= ~TCB_CAPT_bm;
}