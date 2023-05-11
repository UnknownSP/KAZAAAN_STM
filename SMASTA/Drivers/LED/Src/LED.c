/*
 * LED.c
 *
 *  Created on: May 7, 2023
 *      Author: UnknownSP
 */

#include "../Inc/LED.h"
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

uint8_t LED_Data[LED_NUM][4];
uint8_t LED_Temp[LED_NUM][4];
volatile bool _dataSent= false;
static uint32_t pwmData[(24*LED_NUM)+50];

void D_LED_Set(int LEDnum, int Red, int Green, int Blue)
{
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = Green;
	LED_Data[LEDnum][2] = Red;
	LED_Data[LEDnum][3] = Blue;
}

void D_LED_Set_All(uint8_t LED[][3])
{
	for(int i=0; i<LED_NUM; i++){
		LED_Data[i][0] = i;
		LED_Data[i][1] = LED[i][1];
		LED_Data[i][2] = LED[i][0];
		LED_Data[i][3] = LED[i][2];
	}
}

void D_LED_Set_Circle(uint8_t LED[][3], int num){
	for(int i=0; i<10; i++){
		LED_Data[(num-1)*10 + i][0] = (num-1)*10 + i;
		LED_Data[(num-1)*10 + i][1] = LED[i][1];
		LED_Data[(num-1)*10 + i][2] = LED[i][0];
		LED_Data[(num-1)*10 + i][3] = LED[i][2];
	}
}

void D_LED_Send(void)
{
	uint32_t indx=0;
	uint32_t color;


	for (int j= 0; j<LED_NUM; j++)
	{
		color = ((LED_Data[j][1]<<16) | (LED_Data[j][2]<<8) | (LED_Data[j][3]));
		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				pwmData[indx] = 15;  // 2/3 of 25
			}
			else pwmData[indx] = 7;  // 1/3 of 25
			indx++;
		}
	}

	for (int i=0; i<50; i++)
	{
		pwmData[indx] = 0;
		indx++;
	}

	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx*2);
	while (!_dataSent){};
	_dataSent = false;
}

void D_LED_Rotate(int num){
	int indx = 0;
	for(int i=0;i<LED_NUM; i++){
		indx = i + num;
		if(indx >= LED_NUM){
			indx -= 50;
		}
		if(indx < 0){
			indx += 50;
		}
		LED_Temp[i][0] = LED_Data[indx][0];
		LED_Temp[i][1] = LED_Data[indx][1];
		LED_Temp[i][2] = LED_Data[indx][2];
		LED_Temp[i][3] = LED_Data[indx][3];
	}
	for(int i=0;i<LED_NUM; i++){
		LED_Data[i][0] = LED_Temp[i][0];
		LED_Data[i][1] = LED_Temp[i][1];
		LED_Data[i][2] = LED_Temp[i][2];
		LED_Data[i][3] = LED_Temp[i][3];
	}
}

void D_LED_Callback(void){
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
    _dataSent = true;
}