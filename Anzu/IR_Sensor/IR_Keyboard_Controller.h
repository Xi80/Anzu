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

/*�萔�Q*/
static const uint16_t keyFreq = 30000;	/*�L�[�{�[�h���͂̊��荞�ݎ���*/

/*extern�錾*/
extern void (*IR_Keyboard_Controller_Callback_Note_On)(uint8_t);

extern void (*IR_Keyboard_Controller_Callback_Note_Off)(uint8_t);

/*�R���g���[���̏�����*/
void IR_Keyboard_Controller_Init(void);

/*������J�n*/
void IR_Keyboard_Controller_Begin(void);

/*������~*/
void IR_Keyboard_Controller_Stop(void);

#endif /* IR_KEYBOARD_CONTROLLER_H_ */