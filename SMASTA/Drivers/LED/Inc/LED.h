/*
 * LED.h
 *
 *  Created on: May 7, 2023
 *      Author: UnknownSP
 */

#ifndef LED_INC_LED_H_
#define LED_INC_LED_H_

#include "main.h"

#define LED_NUM 50

void D_LED_Set(int LEDnum, int Red, int Green, int Blue);
void D_LED_Send(void);
void D_LED_Callback(void);
void D_LED_Rotate(int num);

#endif /* LED_INC_LED_H_ */
