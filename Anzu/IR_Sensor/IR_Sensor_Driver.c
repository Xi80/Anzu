/*
 * IR_Sensor_Driver.c
 *
 * Created: 2022/07/06 21:45:30
 *  Author: luna
 */ 

#include "IR_Sensor_Driver.h"

/*�h���C�o�̏�����*/
void IR_Sensor_Driver_Init(void){
	/*GPIO�̏����ݒ�*/
	PORTC.DIRSET = 0x3F;	/*�A�h���X��*/
	PORTD.DIRCLR = 0x02;	/*AC����*/
	PORTE.DIRSET = 0x08;	/*�C�l�[�u��*/
	
	PORTC.OUTCLR = 0x3F;
	PORTC.OUTSET = 0x00;	/*�A�h���X��0x00��*/
	
	PORTE.OUTCLR = 0x08;	/*�C�l�[�u�����l�Q�[�g*/
	
	/*AC0�̏����ݒ�*/
	AC0.MUXCTRLA |= (0x80) | (0x03 << 3) | (0x03);	/*�o�͔��]�A������:AINP3(PD1),������:DACREF(������d��)*/

	return;
}

/*������J�n*/
void IR_Sensor_Driver_Begin(void){
	AC0.CTRLA |= AC_ENABLE_bm;	/*AC0�L����*/
	return;
}

/*������~*/
void IR_Sensor_Driver_Stop(void){
	AC0.CTRLA &= ~(AC_ENABLE_bm);	/*AC0�L����*/
	return;
}

/*�Z���T�[�̒l���擾*/
uint8_t IR_Sensor_Driver_Get_Value(uint8_t idx){
	PORTC.OUT = idx & 0x3F;
	PORTE.OUTSET = 0x08;	/*�`�b�v�Z���N�g���W�b�N��L����*/	
	if(AC0.STATUS & AC_STATE_bm){
		/*���͂���*/
		return 1;
	} else {
		/*���͂Ȃ�*/
		return 0;
	}
	PORTE.OUTCLR = 0x08;	/*�`�b�v�Z���N�g���W�b�N�𖳌���*/
}