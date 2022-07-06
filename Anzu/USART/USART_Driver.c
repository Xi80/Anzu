/*
 * USART_Driver.c
 *
 * Created: 2022/07/07 7:16:20
 *  Author: luna
 */ 

#include "USART_Driver.h"

FILE m_stdout = FDEV_SETUP_STREAM(printf_wrapper, NULL, _FDEV_SETUP_WRITE);

/*受信用リングバッファ(受信割り込み使用時のみ)*/
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

/*ドライバの初期化*/
void USART_Driver_Init(void){
	/*
		PA0,1	USART0	DEBUG
		PF0,1	USART2	MIDI
	*/
	
	/*GPIO設定*/
	PORTF.DIRSET = 0x01;
	PORTF.DIRCLR = 0x02;	/*PF0(TX),PF1(RX)をそれぞれ出力、入力に設定*/
	
	PORTA.DIRSET = 0x01;
	PORTA.DIRCLR = 0x02;	/*PA0(TX),PA1(RX)をそれぞれ出力、入力に設定*/
	
	/*USART2設定*/
	USART2.BAUD = _BAUD(31250); /*ボーレートを31250bpsに*/
	USART2.CTRLC = USART_CHSIZE_8BIT_gc;	/*データ長8bit*/
	
	/*USART0設定*/
	USART0.BAUD = _BAUD(38400);	/*ボーレートを38400bpsに*/
	USART0.CTRLC = USART_CHSIZE_8BIT_gc;	/*データ長8bit*/
	
	/*割り込み使用時のみリングバッファを初期化*/
	#if USART_USE_INT
	
	RingBuffer_InitBuffer(&RingBuffer_MIDI);
	RingBuffer_InitBuffer(&RingBuffer_DEBUG);
	
	#endif
}

/*USART動作開始*/
void USART_Driver_Begin(enum Usart_Sel_e dev){
	switch(dev){
		case USART_MIDI:
			USART2.CTRLB = USART_RXEN_bm | USART_TXEN_bm;	/*送受信を許可*/
			/*割り込み設定*/
			#if USART_USE_INT
			USART2.CTRLA = USART_RXCIE_bm;	/*受信割り込み許可*/
			#endif
			break;
		case USART_DEBUG:
		default:
			USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;	/*送受信を許可*/
			/*割り込み設定*/
			#if USART_USE_INT
			USART0.CTRLA = USART_RXCIE_bm;	/*受信割り込み許可*/
			#endif
			stdout = &m_stdout;
			break;
	}
}

/*USART動作停止*/
void USART_Driver_Stop(enum Usart_Sel_e dev){
	switch(dev){
		case USART_MIDI:
		USART2.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);	/*送受信を不許可*/
		/*割り込み設定*/
		#if USART_USE_INT
		USART0.CTRLA &= ~USART_RXCIE_bm;	/*受信割り込み不許可*/
		#endif
		break;
		case USART_DEBUG:
		default:
		USART0.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);	/*送受信を不許可*/
		/*割り込み設定*/
		#if USART_USE_INT
		USART2.CTRLA &= ~USART_RXCIE_bm;	/*受信割り込み不許可*/
		#endif
		break;
	}	
}

/*USART1文字送信*/
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

/*USART1文字受信*/
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

/*printf用ラッパ*/
int printf_wrapper(char c,FILE *stream){
	if(c == '\n')printf_wrapper('\r',stream);
	USART_Driver_Putc(USART_DEBUG,c);
	return 0;
}