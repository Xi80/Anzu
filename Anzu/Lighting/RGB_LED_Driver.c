/*
 * RGB_LED_Driver.c
 *
 * Created: 2022/07/06 19:59:21
 *  Author: luna
 */ 

#include "RGB_LED_Driver.h"

/*表示データ*/
static uint8_t colourArray_r[8];
static uint8_t colourArray_g[8];
static uint8_t colourArray_b[8];

/*表示カウンタ*/
static uint8_t ledCntr = 0;

/*TCB0割り込み(LEDダイナミック制御用)*/
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
		while(!(SPI0.INTFLAGS & SPI_DREIF_bm)); /*送信バッファが空になるまで待機*/
		SPI0.DATA = arr[i];
	}
	
	PORTE.OUTCLR = 0x07;			/*R,G,BをすべてLに*/
	
	for(uint8_t i = 0;i < 32;i++)asm("nop"); /*適当な待ちを入れる*/
	
	PORTA.OUTCLR = 0x80;
	PORTA.OUTSET = 0x80;					/*RCKをシバく*/
	
	for(uint8_t i = 0;i < 32;i++)asm("nop"); /*適当な待ちを入れる*/
	
	PORTE.OUTSET = (1 << ledCntr);			/*表示色選択*/
	
	ledCntr = (ledCntr >= 2)? 0 : ledCntr + 1; /*カウンタ更新*/
	
	TCB0.INTFLAGS |= TCB_CAPT_bm;	/*割り込みクリア*/
	sei();
	return;
}

/*ドライバの初期化*/
void RGB_LED_Driver_Init(void){
	/*GPIO初期設定*/
	PORTA.DIRSET = 0xD0;			/*PA4(MOSI),PA6(SCK),PA7(RCK)を出力に設定*/
	PORTE.DIRSET = 0x07;			/*PE0(R),PE1(G),PE2(B)を出力に設定*/
	PORTE.OUTCLR = 0x07;			/*R,G,BをすべてLに*/
	
	/*SPI0初期設定*/
	SPI0.CTRLA |= SPI_CLK2X_bm | SPI_MASTER_bm;	/*クロック倍速,主装置*/
	SPI0.CTRLB |= SPI_BUFEN_bm;					/*緩衝動作*/
	SPI0.CTRLA |= SPI_ENABLE_bm;				/*SPI0有効化*/
	
	/*TCA0初期設定*/
	TCA0.SINGLE.CTRLA = 0x05;
	
	/*TCB0初期設定*/
	TCB0.CCMP = ledFreq;			/*割り込み周期設定*/
	TCB0.CTRLA |= (0x02 << 1) | TCB_ENABLE_bm;	/*TCB0有効化*/
	
	return;
}

/*動作を開始*/
void RGB_LED_Driver_Begin(void){
	/*TCB0の割り込みを有効化*/
	TCB0.INTCTRL |= TCB_CAPT_bm;
	return;
}

/*動作を停止*/
void RGB_LED_Driver_Stop(void){
	/*TCB0の割り込みを無効化*/
	TCB0.INTCTRL &= ~(TCB_CAPT_bm);
	return;
}

/*表示色を変更*/
static void RGB_LED_Driver_SetColour(uint8_t idx,uint8_t r,uint8_t g,uint8_t b){
	uint8_t arrPos = idx >> 3;	/*アクセス先の配列の添え字*/
	uint8_t bitPos = 7 - (idx - (arrPos << 3)); /*アクセス先のビット位置*/
	
	/*表示データの書き換え*/
	colourArray_r[arrPos] = (r)? colourArray_r[arrPos] | (1 << bitPos) : colourArray_r[arrPos] & ~(1 << bitPos);
	colourArray_g[arrPos] = (g)? colourArray_g[arrPos] | (1 << bitPos) : colourArray_g[arrPos] & ~(1 << bitPos);
	colourArray_b[arrPos] = (b)? colourArray_b[arrPos] | (1 << bitPos) : colourArray_b[arrPos] & ~(1 << bitPos);
	
	return;
}

/*Col_eから色情報を取得して表示色を変更する*/
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
			/*黒*/
			RGB_LED_Driver_SetColour(idx,0,0,0);
	}
}

/*表示色を変更*/
void RGB_LED_Driver_Set_Key(uint8_t idx,enum Col_e colour){
	if(idx > 23)return;/*範囲外*/
	
	uint8_t pos = 17 + (idx << 1);	/*位置*/
	
	RGB_LED_Driver_SetColour_FromEnum(pos,colour);
}

void RGB_LED_Driver_Set_Separater(uint8_t idx,enum Col_e colour){
	if(idx > 22)return;/*範囲外*/
	
	uint8_t pos = 18 + (idx << 1);	/*位置*/

	RGB_LED_Driver_SetColour_FromEnum(pos,colour);
}

void RGB_LED_Driver_Set_PBC(uint8_t idx,enum Col_e colour){
	if(idx > 15)return;/*範囲外*/
	
	uint8_t pos = idx;	/*位置*/	
	
	RGB_LED_Driver_SetColour_FromEnum(pos,colour);
}