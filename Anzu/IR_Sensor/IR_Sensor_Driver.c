/*
 * IR_Sensor_Driver.c
 *
 * Created: 2022/07/06 21:45:30
 *  Author: luna
 */ 

#include "IR_Sensor_Driver.h"

static uint8_t sens[8];

/*�Z���T�[�쓮�p���荞��*/
ISR(TCB1_INT_vect){
	for(uint8_t i = 0;i < 64;i++){
		PORTC.OUTCLR = 0x3F;
		PORTC.OUTSET = i;
		
		PORTE.OUTSET = 0x08;
		uint8_t arrPos = (i >> 3); /*�A�N�Z�X��̔z��̓Y����*/
		uint8_t bitPos = 7 - (i - (arrPos << 3)); /*�A�N�Z�X��̃r�b�g�ʒu*/
		
		for(uint8_t i = 0;i < 4;i++)asm("nop");
		
		if(AC0.STATUS & AC_STATE_bm){
			/*���͂���*/
			sens[arrPos] |= (1 << bitPos);
		} else {
			/*���͂Ȃ�*/
			sens[arrPos] &= ~(1 << bitPos);
		}
		
		PORTE.OUTCLR = 0x08;
	}
	TCB1.INTFLAGS |= TCB_CAPT_bm;	/*���荞�݃N���A*/
	return;
}

/*�h���C�o�̏�����*/
void IR_Sensor_Driver_Init(void){
	/*GPIO�̏����ݒ�*/
	PORTC.DIRSET = 0x3F;	/*�A�h���X��*/
	PORTD.DIRCLR = 0x02;	/*AC����*/
	PORTE.DIRSET = 0x08;	/*�C�l�[�u��*/
	
	PORTC.OUTCLR = 0x3F;
	PORTC.OUTSET = 0x00;	/*�A�h���X��0x00��*/
	
	PORTE.OUTCLR = 0x08;	/*�C�l�[�u�����l�Q�[�g*/
	
	/*TCB1�̏����ݒ�*/
	TCB1.CCMP = irFreq;			/*���荞�ݎ����ݒ�*/
	TCB1.CTRLA |= TCB_ENABLE_bm;	/*TCB1�L����*/
	
	/*AC0�̏����ݒ�*/
	AC0.MUXCTRLA |= (0x03 << 3) | (0x03);	/*������:AINP3(PD1),������:DACREF(������d��)*/
	AC0.CTRLA |= AC_ENABLE_bm;	/*AC0�L����*/
	
	return;
}

/*������J�n*/
void IR_Sensor_Driver_Begin(void){
	TCB1.INTCTRL |= TCB_CAPT_bm;	/*TCB0���荞�݋���*/
	
	return;
}

/*������~*/
void IR_Sensor_Driver_Stop(void){
	TCB1.INTCTRL &= ~(TCB_CAPT_bm);	/*TCB0���荞�ݕs����*/	
	return;
}

/*�Z���T�[�̒l���擾*/
void IR_Sensor_Driver_Get_Value(uint8_t* dest){
	memcpy(dest,&sens[0],8);
	return;
}