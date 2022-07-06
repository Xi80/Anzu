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

/*�萔�Q*/
static const uint16_t irFreq = 60000;	/*�Z���T�X�V����*/

/*�h���C�o�̏�����*/
void IR_Sensor_Driver_Init(void);

/*������J�n*/
void IR_Sensor_Driver_Begin(void);

/*������~*/
void IR_Sensor_Driver_Stop(void);

/*�Z���T�[�̒l���擾*/
void IR_Sensor_Driver_Get_Value(uint8_t*);

#endif /* IR_SENSOR_DRIVER_H_ */