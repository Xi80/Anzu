/*
 * IR_Sensor_Driver.h
 *
 * Created: 2022/07/06 21:38:51
 *  Author: luna
 */ 


#ifndef IR_SENSOR_DRIVER_H_
#define IR_SENSOR_DRIVER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include <string.h>

/*定数群*/
static const uint16_t irFreq = 60000;	/*センサ更新周期*/

/*ドライバの初期化*/
void IR_Sensor_Driver_Init(void);

/*動作を開始*/
void IR_Sensor_Driver_Begin(void);

/*動作を停止*/
void IR_Sensor_Driver_Stop(void);

/*センサーの値を取得*/
void IR_Sensor_Driver_Get_Value(uint8_t*);

#endif /* IR_SENSOR_DRIVER_H_ */