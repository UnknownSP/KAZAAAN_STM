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
static uint32_t Encoder_ResetCount = 0;
static uint8_t LED_Temp[LED_NUM][3];
//static uint8_t LED_Temp_Rainbow[LED_NUM][3];
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
static uint32_t Recent_System_counter = 0;;
static uint32_t DeltaTime = 0;
//static uint32_t RainbowTime = 0;
//static uint32_t BlinkTime = 0;
static uint32_t GradationTime = 0;
//static int RainbowCount = 0;
static int GradationCount = 1;
static int GradationFadeCount = 0;
//static double BlinkCoeff = 1.0;
static bool _bump1_is_on = false;
static bool _bump2_is_on = false;
static bool _bump1_wait_off = false;
static bool _bump2_wait_off = false;


static int Encoder_Diff(int nowCount, int targetCount);
static int Encoder_RangeAdjust(int count);
static int Ball_Detect(int direction);
static void Croon_Rotate(int speed, int direction);
static int Encoder_Process(void);
//static void ArraySwap_Rainbow(int num);
static void BumpLED_Set(uint8_t R, uint8_t G, uint8_t B, int bumper);
static void LED_Pocket_Get(uint8_t LED[][3], LED_Pocket_Mode mode, int pocket, int encoder);
static void LED_Pocket_Blightness(uint8_t LED[][3], int blightness);

static bool _is_SMASTA_Game = false;
static SMASTA_Mode smasta_mode = 5;
static int croon_direction = 0;
static uint32_t caseTime = 0;

static int BlockPosition_Encoder[3] = {
	15,
	48,
	81,
};

int appInit(void){
	D_LED_Init_YellowRandom();
	return 0;
}

int appTask(void){
	static bool _userbutton_ena = false;
	static int ball_detect_num = 0;
	static int croon_target = 0;
	static int target_diff = 0;
	static int Pocket[5] = {
		LED_P_RAINBOW,
		LED_P_BLINK_PURPLE,
		LED_P_RED,
		LED_P_BLINK_YELLOW,
		LED_P_BLUE,
	};
	uint8_t temp[10][3];

	DeltaTime = G_System_counter - Recent_System_counter;

	Encoder_Process();

	if(IO_READ_USERBUTTON()){
		//IO_SET_USERLED();
		//IO_SET_STAYKICKER();
		//IO_SET_KICKER();
		//D_PWM_Set(3,1,5000);
		if(_userbutton_ena){
			if(_is_SMASTA_Game){
				_is_SMASTA_Game = false;
				smasta_mode = 4;
			}else{
				_is_SMASTA_Game = true;
			}
			croon_direction = 0;
		}
		_userbutton_ena = false;
	}else{
		_userbutton_ena = true;
		//IO_RESET_USERLED();
		//for(int i=0; i<50; i++){
        //	D_LED_Set(i, 0, 0, 0);
      	//}
      	//D_LED_Send();
		//IO_RESET_STAYKICKER();
		//IO_RESET_KICKER();
		//D_PWM_Set(3,1,0);
	}

	if(_is_SMASTA_Game){
		caseTime += DeltaTime;
		switch (smasta_mode)
		{
		case SM_INIT:
			Croon_Rotate(0,0);
			BumpLED_Set(0,0,0,1);
			BumpLED_Set(0,0,0,2);
			D_LED_Off();
			if(caseTime >= 1000){
				smasta_mode += 1;
				caseTime = 0;
			}
			break;
		case SM_CROON_INIT:
			D_LED_Get_YellowRandomFlow(LED_Temp);
			D_LED_Set_All(LED_Temp);
			BumpLED_Set(150,150,40,1);
			BumpLED_Set(150,150,40,2);
			Croon_Rotate(CROON_INIT_SPEED,croon_direction);
			if(Encoder_ResetCount >= 2){
				smasta_mode += 1;
				caseTime = 0;
			}
			break;
		case SM_BALL_DETECT:
			D_LED_Get_YellowRandomFlow(LED_Temp);
			D_LED_Set_All(LED_Temp);
			BumpLED_Set(150,150,40,1);
			BumpLED_Set(150,150,40,2);
			ball_detect_num = Ball_Detect(croon_direction);
			if(ball_detect_num != 0){
				croon_target = Encoder_RangeAdjust((5-(ball_detect_num%5)) * 20 + (-4) + 50 + 2);
				smasta_mode += 1;
				caseTime = 0;
			}
			break;
		case SM_CROON_SET_KICKER:
			D_LED_Get_YellowRandomFlow(LED_Temp);
			D_LED_Set_All(LED_Temp);
			BumpLED_Set(150,150,40,1);
			BumpLED_Set(150,150,40,2);
			target_diff = Encoder_Diff(Encoder_Count, croon_target);
			if(target_diff >= 25){
				Croon_Rotate(CROON_INIT_SPEED,croon_direction);
			}else if(target_diff == 0){
				Croon_Rotate(0,0);
				smasta_mode += 1;
				caseTime = 0;
			}else{
				Croon_Rotate(CROON_INIT_MIN_SPEED + ((CROON_INIT_SPEED-CROON_INIT_MIN_SPEED)*target_diff)/25,croon_direction);
			}
			break;

		case SM_LED_INIT:
			if(caseTime >= 500 && caseTime <= 1500){
				for(int i=0; i<LED_NUM; i++){
					LED_Temp[i][0] = 80;
					LED_Temp[i][1] = 40;
					LED_Temp[i][2] = 0;
				}
				D_LED_Set_All(LED_Temp);
			}else if(caseTime >= 500 && caseTime <= 2000){
				BumpLED_Set(0,0,0,1);
				BumpLED_Set(0,0,0,2);
				D_LED_Off();
			}else if(caseTime > 2000){
				smasta_mode += 1;
				caseTime = 0;
			}
			break;
		case SM_LED_POCKET_SHOW:
			D_LED_Off();
			for(int i=0; i<5; i++){
				LED_Pocket_Get(temp,Pocket[i],i+1,Encoder_Count);
				int phase = caseTime/POCKET_SHOWTIME;
				if(i > phase){
					LED_Pocket_Blightness(temp, 0);
				}else if(i < phase){
					LED_Pocket_Blightness(temp, 100);
				}else{
					LED_Pocket_Blightness(temp, (int)((double)(caseTime%POCKET_SHOWTIME)/(POCKET_SHOWTIME/100.0)));
				}
				D_LED_Set_Circle(temp, i+1);
			}
			D_LED_Rotate(-(int)(Encoder_Count/2.0) + LED_OFFSET);
			if(caseTime > POCKET_SHOWTIME*5 + 1000){
				smasta_mode += 1;
				caseTime = 0;
			}
			break;
		
		case SM_LAUNCH_BALL:
			D_LED_Off();
			for(int i=0; i<5; i++){
				LED_Pocket_Get(temp,Pocket[i],i+1,Encoder_Count);
				D_LED_Set_Circle(temp, i+1);
			}
			D_LED_Rotate(-(int)(Encoder_Count/2.0) + LED_OFFSET);

			//IO_SET_KICKER();
			if(IO_READ_KICKER_DOWN()==1){
				BumpLED_Set(BUMP_1_WAITCOLOR_R,BUMP_1_WAITCOLOR_G,BUMP_1_WAITCOLOR_B,1);
				BumpLED_Set(BUMP_2_WAITCOLOR_R,BUMP_2_WAITCOLOR_G,BUMP_2_WAITCOLOR_B,2);
				IO_RESET_KICKER();
				smasta_mode += 1;
				caseTime = 0;
			}
			break;

		case SM_LAUNCH_WAIT:
			D_LED_Off();
			for(int i=0; i<5; i++){
				LED_Pocket_Get(temp,Pocket[i],i+1,Encoder_Count);
				D_LED_Set_Circle(temp, i+1);
			}
			D_LED_Rotate(-(int)(Encoder_Count/2.0) + LED_OFFSET);

			if(caseTime >= 800 && IO_READ_KICKER_DOWN()==0){
				D_LED_Off();
				smasta_mode += 1;
				caseTime = 0;
			}
			break;

		case SM_CROON_SET_BLOCK:
			Croon_Rotate(CROON_INIT_SPEED,croon_direction);
			for(int i=0; i<3; i++){
				if(Encoder_Count == BlockPosition_Encoder[i]){
					Croon_Rotate(0,0);
					smasta_mode += 1;
					caseTime = 0;
					break;
				}
			}
			break;

		default:
			break;
		}
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

	//RainbowTime += DeltaTime;
	//if(RainbowTime >= RAINBOW_FLOWTIME){
	//	RainbowCount++;
	//	RainbowTime = 0;
	//	if(RainbowCount >= 50){
	//		RainbowCount = 0;
	//	}
	//}
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
	//BlinkTime += DeltaTime;
	//if(BlinkTime >= BLINK_FLOWTIME){
	//	BlinkTime = 0;
	//}
	//if(BlinkTime <= 100){
	//	BlinkCoeff = 0.0;
	//}else if(BlinkTime <= 250){
	//	BlinkCoeff = 1.0;
	//}else if(BlinkTime <= 350){
	//	BlinkCoeff = 0.0;
	//}else if(BlinkTime <= 450){
	//	BlinkCoeff = 1.0;
	//}

	//ArraySwap_Rainbow(RainbowCount);
	/*
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
		if(i==ball_detect_num-1){
			for(int j=0;j<10;j++){
				LED_Temp[j][0] = 0;
				LED_Temp[j][1] = 0;
				LED_Temp[j][2] = 0;
			}
		}
		D_LED_Set_Circle(LED_Temp, i+1);
    	//for(int j=0;j<10;j++){
		//	D_LED_Set(i*10+j, (j+1)*25*r, (j+1)*25*g, (j+1)*25*b);
    	//}
    }
	*/
	
	//Gradation code start
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
	//
	//D_LED_Set_All(LED_Temp);
	//Gradation code end

	//Start yellow code start
	//D_LED_YellowRandomFlow(LED_Temp,DeltaTime);
	//D_LED_Set_All(LED_Temp);
	//Start yellow code end

	//D_LED_Rotate(-(int)(Encoder_Count/2.0) + LED_OFFSET);
	D_LED_Set_YellowRandomFlow(DeltaTime);
	D_LED_Set_Rainbow(DeltaTime);
	D_LED_Set_Blink(DeltaTime);
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
	D_Mess_printf("[%10d]\n",ball_detect_num);
	D_Mess_printf("%016b\n", debug_bits);


	Recent_System_counter = Recent_System_counter + DeltaTime;
	return 0;
}


static void BumpLED_Set(uint8_t R, uint8_t G, uint8_t B, int bumper){
	double coeff = 5000.0/255.0;
	int R_adjust = (int)((double)R*coeff);
	int G_adjust = (int)((double)G*coeff);
	int B_adjust = (int)((double)B*coeff);
	if(bumper == 1){
		D_PWM_Set(BUMP_1_R_TIM,BUMP_1_R_CHANNEL,R_adjust);
		D_PWM_Set(BUMP_1_G_TIM,BUMP_1_G_CHANNEL,G_adjust);
		D_PWM_Set(BUMP_1_B_TIM,BUMP_1_B_CHANNEL,B_adjust);
	}else if(bumper == 2){
		D_PWM_Set(BUMP_2_R_TIM,BUMP_2_R_CHANNEL,R_adjust);
		D_PWM_Set(BUMP_2_G_TIM,BUMP_2_G_CHANNEL,G_adjust);
		D_PWM_Set(BUMP_2_B_TIM,BUMP_2_B_CHANNEL,B_adjust);
	}
}

static int Encoder_Diff(int nowCount, int targetCount){
	if(nowCount > targetCount){
		targetCount += 100;
	}
	return targetCount - nowCount;
}

static int Encoder_RangeAdjust(int count){
	if(count > 100){
		count -= 100;
	}
	if(count < 1){
		count += 100;
	}
	return count;
}

static int Ball_Detect(int direction){
	int return_num = 0;
	if(IO_READ_BALL_DETECT()==1){
		if(Encoder_Count >= (5+2) && Encoder_Count < (25+2)){			//middle 16	 reverse 66
			return_num = 4;
		}else if(Encoder_Count >= (25+2) && Encoder_Count < (45+2)){	//middle 36  reverse 86
			return_num = 3;
		}else if(Encoder_Count >= (45+2) && Encoder_Count < (65+2)){	//middle 56  reverse 6
			return_num = 2;
		}else if(Encoder_Count >= (65+2) && Encoder_Count < (85+2)){	//middle 76  reverse 26
			return_num = 1;
		}else if(Encoder_Count >= (85+2) || Encoder_Count < (5+2)){		//middle 96  reverse 46
			return_num = 5;
		}
	}
	return return_num;
}

static void Croon_Rotate(int speed, int direction){
	if(speed > 1000) speed = 1000;
	if(speed < 0)	speed = 0;
	int set_speed = speed * 5;
	if(speed == 0){
		IO_SET_MOTOR_ENA();
		D_PWM_Set(CROON_MOTOR_F_TIM,CROON_MOTOR_F_CHANNEL,0);
		D_PWM_Set(CROON_MOTOR_R_TIM,CROON_MOTOR_R_CHANNEL,0);
		return;
	}
	IO_RESET_MOTOR_ENA();
	if(direction == 0){
		D_PWM_Set(CROON_MOTOR_F_TIM,CROON_MOTOR_F_CHANNEL,set_speed);
	}else if(direction == 1){
		D_PWM_Set(CROON_MOTOR_R_TIM,CROON_MOTOR_R_CHANNEL,set_speed);
	}
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
			Encoder_ResetCount += 1;
			if(Encoder_ResetCount > 10){
				Encoder_ResetCount = 10;
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


static void LED_Pocket_Blightness(uint8_t LED[][3], int blightness){
	if(blightness > 100) blightness = 100;
	double coeff = (double)blightness * (1.0/100.0);
	for(int i=0; i<10; i++){
		LED[i][0] = (int)((double)LED[i][0]*coeff);
		LED[i][1] = (int)((double)LED[i][1]*coeff);
		LED[i][2] = (int)((double)LED[i][2]*coeff);
	}
}

static void LED_Pocket_Get(uint8_t LED[][3], LED_Pocket_Mode mode, int pocket, int encoder){
	int i;
	switch(mode){
	case LED_P_RED:
		for(i=0; i<10; i++){
			LED[i][0] = 200;
			LED[i][1] = 0;
			LED[i][2] = 0;
		}
		break;    
    case LED_P_GREEN:
		for(i=0; i<10; i++){
			LED[i][0] = 0;
			LED[i][1] = 200;
			LED[i][2] = 0;
		}
		break;      
    case LED_P_BLUE:
		for(i=0; i<10; i++){
			LED[i][0] = 0;
			LED[i][1] = 0;
			LED[i][2] = 200;
		}
		break;     
    case LED_P_YELLOW:
		for(i=0; i<10; i++){
			LED[i][0] = 200;
			LED[i][1] = 200;
			LED[i][2] = 0;
		}
		break;     
    case LED_P_PURPLE:
		for(i=0; i<10; i++){
			LED[i][0] = 200;
			LED[i][1] = 0;
			LED[i][2] = 200;
		}
		break;     
    case LED_P_CYAN:
		for(i=0; i<10; i++){
			LED[i][0] = 0;
			LED[i][1] = 200;
			LED[i][2] = 200;
		}
		break;       
    case LED_P_RAINBOW:
		D_LED_Get_Rainbow(LED, encoder, pocket);
		break;
    case LED_P_BLINK_PURPLE:
		for(i=0; i<10; i++){
			LED[i][0] = 200;
			LED[i][1] = 0;
			LED[i][2] = 200;
		}
		D_LED_Get_Blink(LED);
		break;
    case LED_P_BLINK_YELLOW:
		for(i=0; i<10; i++){
			LED[i][0] = 200;
			LED[i][1] = 200;
			LED[i][2] = 0;
		}
		D_LED_Get_Blink(LED);
		break;
	}

	LED[0][0] = 0;
	LED[0][1] = 0;
	LED[0][2] = 0;
	LED[9][0] = 0;
	LED[9][1] = 0;
	LED[9][2] = 0;
}

