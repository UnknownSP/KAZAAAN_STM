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
uint8_t D_LED_Temp[LED_NUM][4];
volatile bool _dataSent= false;
static uint32_t pwmData[(24*LED_NUM)+50];

static uint8_t Yellow_Random[LED_NUM];
static int YellowRandom_num = 0;
static uint8_t LED_Temp_Rainbow[LED_NUM][3];
static int RainbowCount = 0;
static uint8_t Rainbow[7][3] = {
	{  0,153, 66},
	{255,241,  0},
	{243,152,  0},
	{230,  0, 18},
	{146,  7,131},
	{ 29, 32,136},
	{  0,104,183},
};
static double BlinkCoeff = 1.0;
static uint32_t GradationTime = 0;
static int GradationCount = 1;
static int GradationFadeCount = 0;


void D_LED_Reset_TimeGradation(void){
	GradationTime = 0;
	GradationCount = 0;
	GradationFadeCount = 0;
}

void D_LED_Set_TimeGradation(int deltatime){
	GradationTime += deltatime;
	if(GradationTime >= (GRADATION_CYCLETIME/(LED_NUM*10))){
		GradationCount++;
		GradationTime = 0;
		if(GradationCount > 250*2){
			GradationCount = 0;
		}
		GradationFadeCount = GradationCount%10;
	}
}

bool D_LED_Get_TimeGradation(uint8_t LED[][3]){
	for(int i=0; i<LED_NUM; i++){
		if(GradationCount <= 250){
			LED[i][0] = 250 - (GradationCount-1);
			LED[i][1] = (GradationCount-1);
			LED[i][2] = 0;
		}else if(GradationCount <= 500){
			LED[i][0] = 0;
			LED[i][1] = 250 - (GradationCount-1-250);
			LED[i][2] = (GradationCount-1-250);;
		}
	}
	for(int i=0; i<GradationCount/10; i++){
		LED[i][0] = 0;
		LED[i][1] = 0;
		LED[i][2] = 0;
	}
	LED[GradationCount/10][0] = (double)LED[GradationCount/10][0]*((double)(10-GradationFadeCount)/10.0);
	LED[GradationCount/10][1] = (double)LED[GradationCount/10][1]*((double)(10-GradationFadeCount)/10.0);
	LED[GradationCount/10][2] = (double)LED[GradationCount/10][2]*((double)(10-GradationFadeCount)/10.0);
	if(GradationCount == 500){
		return true;
	}else{
		return false;
	}
}

void D_LED_Set_Blink(int deltatime){
	static uint32_t BlinkTime = 0;
	BlinkTime += deltatime;
	if(BlinkTime >= BLINK_FLOWTIME){
		BlinkTime = 0;
	}
	if(BlinkTime <= 100){
		BlinkCoeff = 0.0;
	}else if(BlinkTime <= 250){
		BlinkCoeff = 1.0;
	}else if(BlinkTime <= 350){
		BlinkCoeff = 0.0;
	}else if(BlinkTime <= 450){
		BlinkCoeff = 1.0;
	}
}

void D_LED_Get_Blink(uint8_t LED[][3]){
	for(int j=0;j<10;j++){
		LED[j][0] = (int)((double)LED[j][0]*BlinkCoeff);
		LED[j][1] = (int)((double)LED[j][1]*BlinkCoeff);
		LED[j][2] = (int)((double)LED[j][2]*BlinkCoeff);
	}
}

void D_LED_Set_Rainbow(int deltatime){
	static uint32_t RainbowTime = 0;
	RainbowTime += deltatime;
	if(RainbowTime >= RAINBOW_FLOWTIME){
		RainbowCount++;
		RainbowTime = 0;
		if(RainbowCount >= 50){
			RainbowCount = 0;
		}
	}
	D_LED_Rainbow_ArraySwap(RainbowCount);
}

void D_LED_Get_Rainbow(uint8_t LED[][3], int encoder, int pocket){
	for(int j=0; j<10; j++){
		int index = ((int)(encoder/2.0) + (pocket-1)*10+j) % 50;
		LED[j][0] = LED_Temp_Rainbow[index][0];
		LED[j][1] = LED_Temp_Rainbow[index][1];
		LED[j][2] = LED_Temp_Rainbow[index][2];
	}
}

void D_LED_Rainbow_ArraySwap(int num){
	uint8_t arr_temp[LED_NUM][3];
	for(int i=0; i<LED_NUM; i++){
		LED_Temp_Rainbow[i][0] = Rainbow[i%7][0];
		LED_Temp_Rainbow[i][1] = Rainbow[i%7][1];
		LED_Temp_Rainbow[i][2] = Rainbow[i%7][2];
	}
	int indx;
	for(int i=0;i<LED_NUM; i++){
		indx = i + num;
		if(indx >= LED_NUM){
			indx -= LED_NUM;
		}
		if(indx < 0){
			indx += LED_NUM;
		}
		arr_temp[i][0] = LED_Temp_Rainbow[indx][0];
		arr_temp[i][1] = LED_Temp_Rainbow[indx][1];
		arr_temp[i][2] = LED_Temp_Rainbow[indx][2];
	}
	for(int i=0;i<LED_NUM; i++){
		LED_Temp_Rainbow[i][0] = arr_temp[i][0];
		LED_Temp_Rainbow[i][1] = arr_temp[i][1];
		LED_Temp_Rainbow[i][2] = arr_temp[i][2];
	}
}

void D_LED_Init_YellowRandom(void){
	for(int i=0; i<LED_NUM; i++){
		Yellow_Random[i] = rand()%50;
	}
}

void D_LED_Set_YellowRandomFlow(int deltatime){
	static uint32_t YellowRandomTime = 0;
	YellowRandomTime+=deltatime;
	if(YellowRandomTime >= YELLOWRANDOM_FLOWTIME){
		YellowRandomTime = 0;
		YellowRandom_num += 1;
		YellowRandom_num %= 50;
	}
}

void D_LED_Get_YellowRandomFlow(uint8_t LED[][3]){
	for(int i=0; i<LED_NUM; i++){
		LED[i][0] = 180;
		int index = i+YellowRandom_num;
		if(index >= LED_NUM) index -= LED_NUM;
		if(index < 0) index += LED_NUM;
		LED[i][1] = 150-Yellow_Random[index];
		LED[i][2] = 50-Yellow_Random[LED_NUM-index-1];
	}
}

void D_LED_Off(void){
	for(int i=0; i<LED_NUM; i++){
		LED_Data[i][0] = i;
		LED_Data[i][1] = 0;
		LED_Data[i][2] = 0;
		LED_Data[i][3] = 0;
	}
}

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
		D_LED_Temp[i][0] = LED_Data[indx][0];
		D_LED_Temp[i][1] = LED_Data[indx][1];
		D_LED_Temp[i][2] = LED_Data[indx][2];
		D_LED_Temp[i][3] = LED_Data[indx][3];
	}
	for(int i=0;i<LED_NUM; i++){
		LED_Data[i][0] = i;//D_LED_Temp[i][0];
		LED_Data[i][1] = D_LED_Temp[i][1];
		LED_Data[i][2] = D_LED_Temp[i][2];
		LED_Data[i][3] = D_LED_Temp[i][3];
	}
}

void D_LED_Callback(void){
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
    _dataSent = true;
}

//static uint8_t Rainbow[7][3] = {
//	{255,  0,  0},
//	{255,150,  0},
//	{255,240,  0},
//	{  0,135,  0},
//	{  0,145,255},
//	{  0,100,190},
//	{145,  0,130},
//};
//static uint8_t Rainbow[7][3] = {
//	{ 57,168,105},
//	{242,229, 92},
//	{232,172, 81},
//	{222,102, 65},
//	{165, 91,154},
//	{ 93, 80,153},
//	{ 71,132,191},
//};
//static uint8_t Rainbow[7][3] = {
//	{  0,153, 66},
//	{255,241,  0},
//	{243,152,  0},
//	{230,  0, 18},
//	{146,  7,131},
//	{ 29, 32,136},
//	{  0,104,183},
//};