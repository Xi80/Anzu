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
#include "IR_Sensor/IR_Keyboard_Controller.h"
#include "MCU_Init/MCU_Init.h"
#include "USART/USART_Driver.h"

void testOn(uint8_t note){
	return;
}

void testOff(uint8_t note){
	return;
}

int main(void)
{	
	/*初期化セクション*/
	MCU_Init();
	USART_Driver_Init();
	RGB_LED_Driver_Init();
	IR_Keyboard_Controller_Init();
	
	sei();	/*全体割り込み許可*/
	
	/*コールバック関数の設定*/
	IR_Keyboard_Controller_Callback_Note_On = *testOn;
	IR_Keyboard_Controller_Callback_Note_Off= *testOff;
	
	/*動作開始セクション*/
	USART_Driver_Begin(USART_DEBUG);
	RGB_LED_Driver_Begin();
	IR_Keyboard_Controller_Begin();

	for(uint8_t i = 0;i < 23;i++){
		RGB_LED_Driver_Set_Separater(i,COLOUR_RED);
	}
	
	for(uint8_t i = 0;i < 23;i++){
		RGB_LED_Driver_Set_PBC(i,COLOUR_RED);
	}
	
			
	while(1){
		for(uint8_t i = 0;i < 24;i++){
			RGB_LED_Driver_Set_Key(i,COLOUR_GREEN);
			_delay_ms(50);
		}
		
		for(uint8_t i = 0;i < 24;i++){
			RGB_LED_Driver_Set_Key(i,COLOUR_BLACK);
			_delay_ms(50);
		}
	}
}

