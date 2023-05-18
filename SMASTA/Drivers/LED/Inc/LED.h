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
void D_LED_Set_All(uint8_t LED[][3]);
void D_LED_Set_Circle(uint8_t LED[][3], int num);
void D_LED_Send(void);
void D_LED_Callback(void);
void D_LED_Rotate(int num);
void D_LED_Off(void);
void D_LED_Init_YellowRandom(void);
void D_LED_Set_YellowRandomFlow(int deltatime);
void D_LED_Get_YellowRandomFlow(uint8_t LED[][3]);
void D_LED_Set_Rainbow(int deltatime);
void D_LED_Get_Rainbow(uint8_t LED[][3], int encoder, int pocket);
void D_LED_Rainbow_ArraySwap(int num);
void D_LED_Set_Blink(int deltatime);
void D_LED_Get_Blink(uint8_t LED[][3]);
void D_LED_Reset_TimeGradation(void);
void D_LED_Set_TimeGradation(int deltatime);
bool D_LED_Get_TimeGradation(uint8_t LED[][3]);

#endif /* LED_INC_LED_H_ */
