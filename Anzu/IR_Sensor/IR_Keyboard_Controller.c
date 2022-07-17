/*
 * IR_Keyboard_Controller.c
 *
 * Created: 2022/07/08 9:28:19
 *  Author: luna
 */ 

#include "IR_Keyboard_Controller.h"

static uint8_t prev[3];

/*�R�[���o�b�N�֐�*/
void (*IR_Keyboard_Controller_Callback_Note_On)(uint8_t) = NULL;

void (*IR_Keyboard_Controller_Callback_Note_Off)(uint8_t) = NULL;


/*�L�[�{�[�h���͊��荞��*/
ISR(TCB1_INT_vect){
	
	for(uint8_t idx = 0;idx < 24;idx++){
		uint8_t cur = IR_Sensor_Driver_Get_Value(idx * 2) & IR_Sensor_Driver_Get_Value(idx * 2 + 1);
		uint8_t prv = (prev[idx / 8] & (1 << (idx % 8)))? 1 : 0;
		if(prv){
			/*�O��X�L��������ON*/
			if(prv != cur){
				/*�m�[�g�I�t*/
				if(IR_Keyboard_Controller_Callback_Note_Off != NULL)IR_Keyboard_Controller_Callback_Note_Off(idx);
			}
		} else {
			/*�O��X�L��������OFF*/
			if(prv != cur){
				/*�m�[�g�I��*/
				if(IR_Keyboard_Controller_Callback_Note_On != NULL)IR_Keyboard_Controller_Callback_Note_On(idx);
			}
		}
		
		
		if(cur){
			prev[idx / 8] |= (1 << (idx % 8));
		} else {
			prev[idx / 8] &= ~(1 << (idx % 8));
		}
	}
	
	TCB1.INTFLAGS = TCB_CAPT_bm;	/*���荞�݃t���O���N���A*/
}

/*�R���g���[���̏�����*/
void IR_Keyboard_Controller_Init(void){
	IR_Sensor_Driver_Init();

	/*TCB1�����ݒ�*/
	TCB1.CCMP = keyFreq;			/*���荞�ݎ����ݒ�*/
	TCB1.CTRLA |= (0x01 << 1) | TCB_ENABLE_bm;	/*TCB1�L����*/
}

/*������J�n*/
void IR_Keyboard_Controller_Begin(void){
	IR_Sensor_Driver_Begin();
	TCB1.INTCTRL = TCB_CAPT_bm;
}

/*������~*/
void IR_Keyboard_Controller_Stop(void){
	IR_Sensor_Driver_Stop();
	TCB1.INTCTRL &= ~TCB_CAPT_bm;
}