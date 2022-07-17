/*
 * IR_Keyboard_Controller.h
 *
 * Created: 2022/07/08 9:20:17
 *  Author: luna
 */ 


#ifndef IR_KEYBOARD_CONTROLLER_H_
#define IR_KEYBOARD_CONTROLLER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "IR_Sensor_Driver.h"

/*定数群*/
static const uint16_t keyFreq = 30000;	/*キーボード入力の割り込み周期*/

/*extern宣言*/
extern void (*IR_Keyboard_Controller_Callback_Note_On)(uint8_t);

extern void (*IR_Keyboard_Controller_Callback_Note_Off)(uint8_t);

/*コントローラの初期化*/
void IR_Keyboard_Controller_Init(void);

/*動作を開始*/
void IR_Keyboard_Controller_Begin(void);

/*動作を停止*/
void IR_Keyboard_Controller_Stop(void);

#endif /* IR_KEYBOARD_CONTROLLER_H_ */