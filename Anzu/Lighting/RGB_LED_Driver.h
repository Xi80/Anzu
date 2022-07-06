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

/*定数群*/
static const uint16_t ledFreq =20000;	/*LEDの表示更新周期*/

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

/*ドライバの初期化*/
void RGB_LED_Driver_Init(void);

/*動作を開始*/
void RGB_LED_Driver_Begin(void);

/*動作を停止*/
void RGB_LED_Driver_Stop(void);

/*表示色を変更*/
void RGB_LED_Driver_Set_Key(uint8_t,enum Col_e);	/*鍵盤*/

void RGB_LED_Driver_Set_Separater(uint8_t,enum Col_e);/*セパレータ*/

void RGB_LED_Driver_Set_PBC(uint8_t,enum Col_e);/*ピッチベンドコントローラ*/

#endif /* RGB_LED_DRIVER_H_ */