/*
 * app.c
 *
 *  Created on: Mar 11, 2023
 *      Author: UnknownSP
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "app.h"

static uint32_t Encoder_Count = 0;
static uint8_t LED_Temp[LED_NUM][3];
static uint8_t LED_Temp_Rainbow[LED_NUM][3];
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
static uint8_t Rainbow[7][3] = {
	{  0,153, 66},
	{255,241,  0},
	{243,152,  0},
	{230,  0, 18},
	{146,  7,131},
	{ 29, 32,136},
	{  0,104,183},
};
static uint32_t Recent_System_counter = 0;;
static uint32_t DeltaTime = 0;
static uint32_t RainbowTime = 0;
static uint32_t BlinkTime = 0;
static uint32_t GradationTime = 0;
static uint32_t GradationFadeTime = 0;
static int RainbowCount = 0;
static int GradationCount = 1;
static int GradationFadeCount = 0;
static double BlinkCoeff = 1.0;
static bool _bump1_is_on = false;
static bool _bump2_is_on = false;
static bool _bump1_wait_off = false;
static bool _bump2_wait_off = false;

static int Encoder_Process(void);
static void ArraySwap_Rainbow(int num);

int appInit(void){
	return 0;
}

int appTask(void){
	DeltaTime = G_System_counter - Recent_System_counter;

	Encoder_Process();

	if(IO_READ_USERBUTTON()){
		//IO_SET_USERLED();
		IO_SET_STAYKICKER();
	}else{
		//IO_RESET_USERLED();
		//for(int i=0; i<50; i++){
        //	D_LED_Set(i, 0, 0, 0);
      	//}
      	//D_LED_Send();
		IO_RESET_STAYKICKER();
	}

	if(IO_READ_BUMP_1_HIT()==1 && !_bump1_is_on && !_bump1_wait_off){
		_bump1_is_on = true;
		IO_SET_BUMP_1();
	}
	if(_bump1_is_on && IO_READ_BUMP_1_SOL()==1 && !_bump1_wait_off){
		_bump1_is_on = false;
		_bump1_wait_off = true;
		IO_RESET_BUMP_1();
	}
	if(_bump1_wait_off && IO_READ_BUMP_1_SOL()==0 && IO_READ_BUMP_1_HIT()==0){
		_bump1_wait_off = false;
	}
	if(IO_READ_BUMP_2_HIT()==1 && !_bump2_is_on && !_bump2_wait_off){
		_bump2_is_on = true;
		IO_SET_BUMP_2();
	}
	if(_bump2_is_on && IO_READ_BUMP_2_SOL()==1 && !_bump2_wait_off){
		_bump2_is_on = false;
		_bump2_wait_off = true;
		IO_RESET_BUMP_2();
	}
	if(_bump2_wait_off && IO_READ_BUMP_2_SOL()==0 && IO_READ_BUMP_2_HIT()==0){
		_bump2_wait_off = false;
	}

	RainbowTime += DeltaTime;
	if(RainbowTime >= RAINBOW_FLOWTIME){
		RainbowCount++;
		RainbowTime = 0;
		if(RainbowCount >= 50){
			RainbowCount = 0;
		}
	}
	GradationTime += DeltaTime;
	if(GradationTime >= (GRADATION_CYCLETIME/(LED_NUM*10))){
		//GradationFadeCount++;
		GradationCount++;
		GradationTime = 0;
		//if(GradationFadeCount >= 10){
		//	GradationFadeCount = 0;
		//}
		if(GradationCount > 250*2){
			GradationCount = 0;
		}
		GradationFadeCount = GradationCount%10;
	}
	//GradationFadeTime += DeltaTime;
	//if(GradationFadeTime >= (GRADATION_CYCLETIME/LED_NUM)){
	//	GradationFadeCount++;
	//	GradationFadeTime = 0;
	//	if(GradationFadeCount >= LED_NUM){
	//		GradationFadeCount = 0;
	//	}
	//}
	BlinkTime += DeltaTime;
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

	ArraySwap_Rainbow(RainbowCount);
	for(int i=0; i<5; i++){
    	int r=0;
    	int g=0;
    	int b=0;
		double coeff = 1.0;
    	switch (i)
    	{
    	case 0:
    	  	r=1;
    	  	g=0;
    	  	b=0;
    	  	break;

    	case 1:
    	  	r=1;
    	  	g=0;
    	  	b=0;
    	  	break;

    	case 2:
    	  	r=0;
    	  	g=1;
    	  	b=0;
    	  	break;

    	case 3:
    	  	r=1;
    	  	g=0;
    	  	b=1;
    	  	break;

    	case 4:
    	  	r=1;
    	  	g=1;
    	  	b=0;
    	  	break;
    	}

		if(i==0 || i==2){
			for(int j=0; j<10; j++){
				if(j==0 || j==9){
					coeff = 0.0;
				}else{
					coeff = 1.0;
				}
				int index = ((int)(Encoder_Count/2.0) + i*10+j) % 50;
				LED_Temp[j][0] = (int)((double)LED_Temp_Rainbow[index][0]*coeff);
				LED_Temp[j][1] = (int)((double)LED_Temp_Rainbow[index][1]*coeff);
				LED_Temp[j][2] = (int)((double)LED_Temp_Rainbow[index][2]*coeff);
			}
		}else if(i==3 || i==4){
			for(int j=0;j<10;j++){
				if(j==0 || j==9){
					coeff = 0.0;
				}else{
					coeff = 1.0;
				}
				LED_Temp[j][0] = (int)((double)r*255.0*BlinkCoeff*coeff);
				LED_Temp[j][1] = (int)((double)g*255.0*BlinkCoeff*coeff);
				LED_Temp[j][2] = (int)((double)b*255.0*BlinkCoeff*coeff);
			}
		}else{
			for(int j=0;j<10;j++){
				if(j==0 || j==9){
					coeff = 0.0;
				}else{
					coeff = 1.0;
				}
				LED_Temp[j][0] = (int)((double)r*255.0*coeff);
				LED_Temp[j][1] = (int)((double)g*255.0*coeff);
				LED_Temp[j][2] = (int)((double)b*255.0*coeff);
			}
		}
		D_LED_Set_Circle(LED_Temp, i+1);
    	//for(int j=0;j<10;j++){
		//	D_LED_Set(i*10+j, (j+1)*25*r, (j+1)*25*g, (j+1)*25*b);
    	//}
    }
	//for(int i=0; i<LED_NUM; i++){
	//	if(GradationCount <= 250){
	//		LED_Temp[i][0] = 250 - (GradationCount-1);
	//		LED_Temp[i][1] = (GradationCount-1);
	//		LED_Temp[i][2] = 0;
	//	}else if(GradationCount <= 500){
	//		LED_Temp[i][0] = 0;
	//		LED_Temp[i][1] = 250 - (GradationCount-1-250);
	//		LED_Temp[i][2] = (GradationCount-1-250);;
	//	}
	//}
	//for(int i=0; i<GradationCount/10; i++){
	//	LED_Temp[i][0] = 0;
	//	LED_Temp[i][1] = 0;
	//	LED_Temp[i][2] = 0;
	//}
	//LED_Temp[GradationCount/10][0] = (double)LED_Temp[GradationCount/10][0]*((double)(10-GradationFadeCount)/10.0);
	//LED_Temp[GradationCount/10][1] = (double)LED_Temp[GradationCount/10][1]*((double)(10-GradationFadeCount)/10.0);
	//LED_Temp[GradationCount/10][2] = (double)LED_Temp[GradationCount/10][2]*((double)(10-GradationFadeCount)/10.0);
	
	//D_LED_Set_All(LED_Temp);
	D_LED_Rotate(-(int)(Encoder_Count/2.0) + LED_OFFSET);
    D_LED_Send();

	int16_t debug_bits = 0;
	debug_bits |= ((int)IO_READ_ENC_STEP() << 0);
	debug_bits |= ((int)IO_READ_ENC_HOME() << 1);
	debug_bits |= ((int)IO_READ_BUMP_1_HIT() << 2);
	debug_bits |= ((int)IO_READ_BUMP_1_SOL() << 3);
	debug_bits |= ((int)IO_READ_BUMP_2_HIT() << 4);
	debug_bits |= ((int)IO_READ_BUMP_2_SOL() << 5);
	debug_bits |= ((int)IO_READ_BALL_DETECT() << 6);
	debug_bits |= ((int)IO_READ_KICKER_DOWN() << 7);
	debug_bits |= ((int)IO_READ_STAYKICKER_DOWN() << 8);
	D_Mess_printf("\033[1;1H");
	D_Mess_printf("[%10d]\n",G_System_counter);
	D_Mess_printf("[%10d]\n",Encoder_Count);
	D_Mess_printf("[%10d]\n",RainbowTime);
	D_Mess_printf("%016b\n", debug_bits);


	Recent_System_counter = Recent_System_counter + DeltaTime;
	return 0;
}

static int Encoder_Process(void){
	static int enc_change_count = 0;
	static int home_change_count = 0;
	static int recent_enc_state = 0;
	static int recent_home_state = 0;
	static bool reset = false;
	int enc_state = (int)IO_READ_ENC_STEP();
	int home_state = (int)IO_READ_ENC_HOME();
	if(enc_state != recent_enc_state){
		enc_change_count++;
	}else{
		enc_change_count = 0;
	}
	if(enc_change_count >= 5){
		recent_enc_state = enc_state;
		enc_change_count = 0;
		if(enc_state == 0 || enc_state == 1){
			Encoder_Count++;
		}
	}
	if(home_state != recent_home_state){
		home_change_count++;
	}else{
		home_change_count = 0;
	}
	if(home_change_count >= 5 && !reset){
		reset = true;
		recent_home_state = home_state;
		home_change_count = 0;
		if(home_state == 1){
			if(recent_enc_state == 0){
				Encoder_Count = 1;
			}else if(recent_enc_state == 1){
				Encoder_Count = 2;
			}
		}
	}
	if(Encoder_Count > 10){
		reset = false;
		//Encoder_Count = 0;
	}
	if(Encoder_Count > 100){
		Encoder_Count = 1;
	}

	return 0;
}

static void ArraySwap_Rainbow(int num){
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

