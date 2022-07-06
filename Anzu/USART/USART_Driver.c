/*
 * USART_Driver.c
 *
 * Created: 2022/07/07 7:16:20
 *  Author: luna
 */ 

#include "USART_Driver.h"

FILE m_stdout = FDEV_SETUP_STREAM(printf_wrapper, NULL, _FDEV_SETUP_WRITE);

/*��M�p�����O�o�b�t�@(��M���荞�ݎg�p���̂�)*/
#if USART_USE_INT
	static RingBuff_t RingBuffer_MIDI;
	static RingBuff_t RingBuffer_DEBUG;
	

ISR(USART0_RXC_vect){
	uint8_t tmp = USART0.RXDATAL;
	RingBuffer_Insert(&RingBuffer_DEBUG,tmp);
}

ISR(USART2_RXC_vect){
	uint8_t tmp = USART2.RXDATAL;
	RingBuffer_Insert(&RingBuffer_MIDI,tmp);
}

#endif

/*�h���C�o�̏�����*/
void USART_Driver_Init(void){
	/*
		PA0,1	USART0	DEBUG
		PF0,1	USART2	MIDI
	*/
	
	/*GPIO�ݒ�*/
	PORTF.DIRSET = 0x01;
	PORTF.DIRCLR = 0x02;	/*PF0(TX),PF1(RX)�����ꂼ��o�́A���͂ɐݒ�*/
	
	PORTA.DIRSET = 0x01;
	PORTA.DIRCLR = 0x02;	/*PA0(TX),PA1(RX)�����ꂼ��o�́A���͂ɐݒ�*/
	
	/*USART2�ݒ�*/
	USART2.BAUD = _BAUD(31250); /*�{�[���[�g��31250bps��*/
	USART2.CTRLC = USART_CHSIZE_8BIT_gc;	/*�f�[�^��8bit*/
	
	/*USART0�ݒ�*/
	USART0.BAUD = _BAUD(38400);	/*�{�[���[�g��38400bps��*/
	USART0.CTRLC = USART_CHSIZE_8BIT_gc;	/*�f�[�^��8bit*/
	
	/*���荞�ݎg�p���̂݃����O�o�b�t�@��������*/
	#if USART_USE_INT
	
	RingBuffer_InitBuffer(&RingBuffer_MIDI);
	RingBuffer_InitBuffer(&RingBuffer_DEBUG);
	
	#endif
}

/*USART����J�n*/
void USART_Driver_Begin(enum Usart_Sel_e dev){
	switch(dev){
		case USART_MIDI:
			USART2.CTRLB = USART_RXEN_bm | USART_TXEN_bm;	/*����M������*/
			/*���荞�ݐݒ�*/
			#if USART_USE_INT
			USART2.CTRLA = USART_RXCIE_bm;	/*��M���荞�݋���*/
			#endif
			break;
		case USART_DEBUG:
		default:
			USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;	/*����M������*/
			/*���荞�ݐݒ�*/
			#if USART_USE_INT
			USART0.CTRLA = USART_RXCIE_bm;	/*��M���荞�݋���*/
			#endif
			stdout = &m_stdout;
			break;
	}
}

/*USART�����~*/
void USART_Driver_Stop(enum Usart_Sel_e dev){
	switch(dev){
		case USART_MIDI:
		USART2.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);	/*����M��s����*/
		/*���荞�ݐݒ�*/
		#if USART_USE_INT
		USART0.CTRLA &= ~USART_RXCIE_bm;	/*��M���荞�ݕs����*/
		#endif
		break;
		case USART_DEBUG:
		default:
		USART0.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);	/*����M��s����*/
		/*���荞�ݐݒ�*/
		#if USART_USE_INT
		USART2.CTRLA &= ~USART_RXCIE_bm;	/*��M���荞�ݕs����*/
		#endif
		break;
	}	
}

/*USART1�������M*/
void USART_Driver_Putc(enum Usart_Sel_e dev,const char src){
	switch(dev){
		case USART_MIDI:
			while(!(USART2.STATUS & USART_DREIF_bm));
			USART2.TXDATAL = src;
		case USART_DEBUG:
			while(!(USART0.STATUS & USART_DREIF_bm));
			USART0.TXDATAL = src;
	}
	return;
}

/*USART1������M*/
void USART_Driver_Getc(enum Usart_Sel_e dev,char* dest){
	uint8_t tmp = 0x00;
	#if USART_USE_INT
	switch(dev){
		case USART_MIDI:
			while(!(RingBuffer_GetCount(&RingBuffer_MIDI)));
			tmp = RingBuffer_Remove(&RingBuffer_MIDI);
			break;
		case USART_DEBUG:
			while(!(RingBuffer_GetCount(&RingBuffer_DEBUG)));
			tmp = RingBuffer_Remove(&RingBuffer_DEBUG);
			break;
	}
	#else
	switch(dev){
		case USART_MIDI:
			while(!(USART2.STATUS & USART_RXCIF_bm));
			tmp = USART2.RXDATAL;
			break;
		case USART_DEBUG:
			while(!(USART0.STATUS & USART_RXCIF_bm));
			tmp = USART0.RXDATAL;
			break;
	}
	#endif
	
	*dest = (char)tmp;
	
	return;
}

/*printf�p���b�p*/
int printf_wrapper(char c,FILE *stream){
	if(c == '\n')printf_wrapper('\r',stream);
	USART_Driver_Putc(USART_DEBUG,c);
	return 0;
}