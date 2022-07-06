/*
 * Anzu.c
 *
 * Created: 2022/06/27 18:02:48
 * Author : luna
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "Lighting/RGB_LED_Driver.h"
#include "IR_Sensor/IR_Sensor_Driver.h"
#include "MCU_Init/MCU_Init.h"
#include "USART/USART_Driver.h"

volatile uint8_t counter = 0;
volatile uint8_t colour[24];

void setAddr(uint8_t addr);

/*
ISR(TCB0_INT_vect){
	uint8_t idx = counter;
	 
	TCB0.INTFLAGS = 0x01;
	PORTE.OUTCLR = 0x07;
	
	for(uint8_t j = 0;j < 8;j++){
		while(!(SPI0.INTFLAGS & 0x20));
		SPI0.DATA = colour[(idx << 3) + j];
	}
	
	_delay_us(10);
	
	PORTA.OUTCLR = 0x80;
	PORTA.OUTSET = 0x80;
	PORTE.OUTSET = (1 << idx);	
	
	counter = (idx >= 2)? 0 : idx + 1;
}
*/

void clockInit(void){
	CCP = 0xD8;
	CLKCTRL.MCLKCTRLB = 0x00;
	CCP =0xD8;
	CPUINT.CTRLA = 0x01;
}

void timerInit(void){
	TCB0.CCMP = 65535;
	TCB0.CTRLA = 0x01;
	TCB0.INTCTRL = 0x01;
}

void usartInit(void){
	USART0.BAUD = _BAUD(38400);
	USART0.CTRLC = 0x03;
	PORTA.DIRSET = 0x01;
	USART0.CTRLB = 0xC0;
}




void setAddr(uint8_t addr){
	PORTC.OUTCLR = 0x3F;
	PORTC.OUTSET = addr & 0x3F;
}

int main(void)
{
	/*CLOCK Init*/
	//clockInit();
	
	MCU_Init();
	USART_Driver_Init();
	RGB_LED_Driver_Init();
	IR_Sensor_Driver_Init();
	
	sei();
	
	USART_Driver_Begin(USART_DEBUG);
	RGB_LED_Driver_Begin();
	IR_Sensor_Driver_Begin();
	
	/*
	for(uint8_t i = 0;i < 6;i++){
		RGB_LED_Driver_Set_Separater(3 + 4 * i,COLOUR_MAGENTA);
	}
	*/
	while(1){
		//printf("Hello,World!\n");
		for(uint8_t i = 0;i < 24;i++){
			RGB_LED_Driver_Set_Key(i,COLOUR_YELLOW);
			_delay_ms(80);
		}
		for(uint8_t i = 0;i < 24;i++){
			RGB_LED_Driver_Set_Key(i,COLOUR_BLACK);
			_delay_ms(80);
		}
	}
}

