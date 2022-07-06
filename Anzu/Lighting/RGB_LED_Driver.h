/*
 * RGB_LED_Driver.h
 *
 * Created: 2022/07/06 19:46:33
 *  Author: luna
 */ 


#ifndef RGB_LED_DRIVER_H_
#define RGB_LED_DRIVER_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*�萔�Q*/
static const uint16_t ledFreq =20000;	/*LED�̕\���X�V����*/

enum Col_e {
	COLOUR_RED,
	COLOUR_BLUE,
	COLOUR_YELLOW,
	COLOUR_GREEN,
	COLOUR_MAGENTA,
	COLOUR_CYAN,
	COLOUR_WHITE,
	COLOUR_BLACK,	
};

/*�h���C�o�̏�����*/
void RGB_LED_Driver_Init(void);

/*������J�n*/
void RGB_LED_Driver_Begin(void);

/*������~*/
void RGB_LED_Driver_Stop(void);

/*�\���F��ύX*/
void RGB_LED_Driver_Set_Key(uint8_t,enum Col_e);	/*����*/

void RGB_LED_Driver_Set_Separater(uint8_t,enum Col_e);/*�Z�p���[�^*/

void RGB_LED_Driver_Set_PBC(uint8_t,enum Col_e);/*�s�b�`�x���h�R���g���[��*/

#endif /* RGB_LED_DRIVER_H_ */