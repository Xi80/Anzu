/*
 * RGB_LED_Driver.c
 *
 * Created: 2022/07/06 19:59:21
 *  Author: luna
 */ 

#include "RGB_LED_Driver.h"

/*�\���f�[�^*/
static uint8_t colourArray_r[8];
static uint8_t colourArray_g[8];
static uint8_t colourArray_b[8];

/*�\���J�E���^*/
static uint8_t ledCntr = 0;

/*TCB0���荞��(LED�_�C�i�~�b�N����p)*/
ISR(TCB0_INT_vect){
	cli();
	
	uint8_t *arr;
	
	switch(ledCntr){
		case 0:
			arr = &colourArray_r[0];
			break;
		case 1:
			arr = &colourArray_g[0];
			break;
		default:
			arr = &colourArray_b[0];
			break;
	}
	
	for(uint8_t i = 0;i < 8;i++){
		while(!(SPI0.INTFLAGS & SPI_DREIF_bm)); /*���M�o�b�t�@����ɂȂ�܂őҋ@*/
		SPI0.DATA = arr[i];
	}
	
	PORTE.OUTCLR = 0x07;			/*R,G,B�����ׂ�L��*/
	
	for(uint8_t i = 0;i < 32;i++)asm("nop"); /*�K���ȑ҂�������*/
	
	PORTA.OUTCLR = 0x80;
	PORTA.OUTSET = 0x80;					/*RCK���V�o��*/
	
	for(uint8_t i = 0;i < 32;i++)asm("nop"); /*�K���ȑ҂�������*/
	
	PORTE.OUTSET = (1 << ledCntr);			/*�\���F�I��*/
	
	ledCntr = (ledCntr >= 2)? 0 : ledCntr + 1; /*�J�E���^�X�V*/
	
	TCB0.INTFLAGS |= TCB_CAPT_bm;	/*���荞�݃N���A*/
	sei();
	return;
}

/*�h���C�o�̏�����*/
void RGB_LED_Driver_Init(void){
	/*GPIO�����ݒ�*/
	PORTA.DIRSET = 0xD0;			/*PA4(MOSI),PA6(SCK),PA7(RCK)���o�͂ɐݒ�*/
	PORTE.DIRSET = 0x07;			/*PE0(R),PE1(G),PE2(B)���o�͂ɐݒ�*/
	PORTE.OUTCLR = 0x07;			/*R,G,B�����ׂ�L��*/
	
	/*SPI0�����ݒ�*/
	SPI0.CTRLA |= SPI_CLK2X_bm | SPI_MASTER_bm;	/*�N���b�N�{��,�呕�u*/
	SPI0.CTRLB |= SPI_BUFEN_bm;					/*�ɏՓ���*/
	SPI0.CTRLA |= SPI_ENABLE_bm;				/*SPI0�L����*/
	
	/*TCA0�����ݒ�*/
	TCA0.SINGLE.CTRLA = 0x05;
	
	/*TCB0�����ݒ�*/
	TCB0.CCMP = ledFreq;			/*���荞�ݎ����ݒ�*/
	TCB0.CTRLA |= (0x02 << 1) | TCB_ENABLE_bm;	/*TCB0�L����*/
	
	return;
}

/*������J�n*/
void RGB_LED_Driver_Begin(void){
	/*TCB0�̊��荞�݂�L����*/
	TCB0.INTCTRL |= TCB_CAPT_bm;
	return;
}

/*������~*/
void RGB_LED_Driver_Stop(void){
	/*TCB0�̊��荞�݂𖳌���*/
	TCB0.INTCTRL &= ~(TCB_CAPT_bm);
	return;
}

/*�\���F��ύX*/
static void RGB_LED_Driver_SetColour(uint8_t idx,uint8_t r,uint8_t g,uint8_t b){
	uint8_t arrPos = idx >> 3;	/*�A�N�Z�X��̔z��̓Y����*/
	uint8_t bitPos = 7 - (idx - (arrPos << 3)); /*�A�N�Z�X��̃r�b�g�ʒu*/
	
	/*�\���f�[�^�̏�������*/
	colourArray_r[arrPos] = (r)? colourArray_r[arrPos] | (1 << bitPos) : colourArray_r[arrPos] & ~(1 << bitPos);
	colourArray_g[arrPos] = (g)? colourArray_g[arrPos] | (1 << bitPos) : colourArray_g[arrPos] & ~(1 << bitPos);
	colourArray_b[arrPos] = (b)? colourArray_b[arrPos] | (1 << bitPos) : colourArray_b[arrPos] & ~(1 << bitPos);
	
	return;
}

/*Col_e����F�����擾���ĕ\���F��ύX����*/
static void RGB_LED_Driver_SetColour_FromEnum(uint8_t idx,enum Col_e colour){
	switch(colour){
		case COLOUR_RED:
			RGB_LED_Driver_SetColour(idx,1,0,0);
			break;
		case COLOUR_BLUE:
			RGB_LED_Driver_SetColour(idx,0,0,1);
			break;
		case COLOUR_YELLOW:
			RGB_LED_Driver_SetColour(idx,1,1,0);
			break;
		case COLOUR_GREEN:
			RGB_LED_Driver_SetColour(idx,0,1,0);
			break;
		case COLOUR_MAGENTA:
			RGB_LED_Driver_SetColour(idx,1,0,1);
			break;
		case COLOUR_CYAN:
			RGB_LED_Driver_SetColour(idx,0,1,1);
			break;
		case COLOUR_WHITE:
			RGB_LED_Driver_SetColour(idx,1,1,1);
			break;
		default:
			/*��*/
			RGB_LED_Driver_SetColour(idx,0,0,0);
	}
}

/*�\���F��ύX*/
void RGB_LED_Driver_Set_Key(uint8_t idx,enum Col_e colour){
	if(idx > 23)return;/*�͈͊O*/
	
	uint8_t pos = 17 + (idx << 1);	/*�ʒu*/
	
	RGB_LED_Driver_SetColour_FromEnum(pos,colour);
}

void RGB_LED_Driver_Set_Separater(uint8_t idx,enum Col_e colour){
	if(idx > 22)return;/*�͈͊O*/
	
	uint8_t pos = 18 + (idx << 1);	/*�ʒu*/

	RGB_LED_Driver_SetColour_FromEnum(pos,colour);
}

void RGB_LED_Driver_Set_PBC(uint8_t idx,enum Col_e colour){
	if(idx > 15)return;/*�͈͊O*/
	
	uint8_t pos = idx;	/*�ʒu*/	
	
	RGB_LED_Driver_SetColour_FromEnum(pos,colour);
}