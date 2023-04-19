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

static bool JP_Lift_Up(void);
static bool JP_Lift_Down(void);
static void Lottery_1st2nd_SetSpeed(int speed);
static void Lottery_3rd_SetSpeed(int speed);
static void Lottery_JP_SetSpeed(int speed, int direction);
static int Encoder_1st(void);
static int Encoder_2nd(void);
static int Encoder_3rd(void);
static int Encoder_JP(void);
static int JPC_Processing(int station, int speed,int direction);

static int Encoder_Num_1st = 1;
static int Encoder_Num_2nd = 1;
static int Encoder_Num_3rd = 1;
static int Encoder_Num_JP = 1;
static bool _is_JPC = false;
static int JPC_stations[4] = {0};

int appInit(void){
	Lottery_1st2nd_SetSpeed(0);
	Lottery_3rd_SetSpeed(0);
	Lottery_JP_SetSpeed(0,0);
	return 0;
}

int appTask(void){
	Encoder_1st();
	Encoder_2nd();
	Encoder_3rd();
	Encoder_JP();
	//D_Mess_printf("%d\n", G_System_counter);
	//D_PWM_Set(1,100);
	//D_PWM_Set(2,100);
	//IO_SET_BLDC2_DIR();
	if(IO_READ_USERBUTTON()){
		IO_SET_USERLED();
		//IO_SET_BLDC1_ENA();
		//IO_SET_BLDC2_ENA();
		//D_PWM_Set(BLDC1,50);
		//D_PWM_Set(BLDC2,50);
		//D_PWM_Set(BLDC3,300);
		//IO_SET_JP_LED();
		Lottery_1st2nd_SetSpeed(500);
		Lottery_3rd_SetSpeed(800);
		Lottery_JP_SetSpeed(300,0);

		//JP_Lift_Down();
	}else{
		IO_RESET_USERLED();
		//IO_RESET_BLDC1_ENA();
		//IO_RESET_BLDC2_ENA();
		//IO_RESET_BLDC3_ENA();
		//D_PWM_Set(BLDC1,300);
		//D_PWM_Set(BLDC2,3000);
		//D_PWM_Set(BLDC3,3500);
		//IO_RESET_JP_LED();
		Lottery_1st2nd_SetSpeed(100);
		Lottery_3rd_SetSpeed(300);
		Lottery_JP_SetSpeed(10,0);

		//JP_Lift_Up();
	}

	/*
	if(IO_READ_1ST_ENC()){
		IO_RESET_JP_LED();
	}else{
		IO_SET_JP_LED();
	}
	*/

	int16_t debug_bits = 0;
	debug_bits &= 0;
	debug_bits |= ((int)IO_READ_1ST_HOME() << 5);
	debug_bits |= ((int)IO_READ_1ST_ENC() << 4);
	debug_bits |= ((int)IO_READ_2ND_HOME() << 3);
	debug_bits |= ((int)IO_READ_2ND_ENC() << 2);
	debug_bits |= ((int)IO_READ_3RD_HOME() << 1);
	debug_bits |= ((int)IO_READ_3RD_ENC() << 0);
	debug_bits |= ((int)IO_READ_JP_HOME() << 13);
	debug_bits |= ((int)IO_READ_JP_ENC() << 12);
	debug_bits |= ((int)IO_READ_JP_FRONT() << 11);
	debug_bits |= ((int)IO_READ_JP_REAR() << 10);
	debug_bits |= ((int)IO_READ_JP_UPPER() << 9);
	debug_bits |= ((int)IO_READ_JP_LOWER() << 8);
	D_Mess_printf("\033[1;1H");
	D_Mess_printf("%016b\n", debug_bits);
	D_Mess_printf("%02d\n",Encoder_Num_1st);
	D_Mess_printf("%02d\n",Encoder_Num_2nd);
	D_Mess_printf("%02d\n",Encoder_Num_3rd);
	D_Mess_printf("%02d\n",Encoder_Num_JP);


	return 0;
}

static int JPC_Processing(int station, int speed, int direction){
	static JPC_Process_State JPC_state = JPC_ROT_START_SLOW_1;
	static int RecentTime = 0;
	static int DeltaTime = 0;
	static int caseDeltaTime = 0;
	if(station == 0){
		RecentTime = G_System_counter;
		return 0;
	}
	DeltaTime = G_System_counter - RecentTime;
	caseDeltaTime += DeltaTime;
	switch (JPC_state)
	{
	case JPC_ROT_START_SLOW_1:
		Lottery_JP_SetSpeed(speed,direction);
		caseDeltaTime = 0;
		JPC_state += 1;
		break;

	case JPC_LED_ON:
		IO_SET_JP_LED();
		if(caseDeltaTime >= 2000){
			caseDeltaTime = 0;
			JPC_state += 1;
		}
		break;
	case JPC_LIFT_UP:
		if(JP_Lift_Up()){
			caseDeltaTime = 0;
			JPC_state += 1;
		}
		break;
	case JPC_WAIT_1:
		if(caseDeltaTime >= 2000){
			caseDeltaTime = 0;
			JPC_state += 1;
		}
		break;
	case JPC_WAIT_INPUT_1:
		//Wait station JPC Button
		JPC_state += 1;
		break;
	case JPC_ROT_START_1:
		Lottery_JP_SetSpeed(JPC_MAX_SPEED,direction);
		if(caseDeltaTime >= 10000){
			caseDeltaTime = 0;
			JPC_state += 1;
		}
		break;
	case JPC_DECELERATION_1:
	
	default:
		break;
	}
	RecentTime = G_System_counter;
}

static int Encoder_1st(void){
	static int enc_change_count = 0;
	static int home_change_count = 0;
	static int recent_enc_state = 0;
	static int recent_home_state = 0;
	int enc_state = (int)IO_READ_1ST_ENC();
	int home_state = (int)IO_READ_1ST_HOME();
	if(enc_state != recent_enc_state){
		enc_change_count++;
	}else{
		enc_change_count = 0;
	}
	if(enc_change_count >= 5){
		recent_enc_state = enc_state;
		enc_change_count = 0;
		if(enc_state == 1){
			Encoder_Num_1st++;
		}
	}
	if(home_state != recent_home_state){
		home_change_count++;
	}else{
		home_change_count = 0;
	}
	if(home_change_count >= 5){
		recent_home_state = home_state;
		home_change_count = 0;
		if(home_state == 1){
			Encoder_Num_1st = 24;
		}
	}
	if(Encoder_Num_1st > 24){
		Encoder_Num_1st = 1;
	}
}

static int Encoder_2nd(void){
	static int enc_change_count = 0;
	static int home_change_count = 0;
	static int recent_enc_state = 0;
	static int recent_home_state = 0;
	int enc_state = (int)IO_READ_2ND_ENC();
	int home_state = (int)IO_READ_2ND_HOME();
	if(enc_state != recent_enc_state){
		enc_change_count++;
	}else{
		enc_change_count = 0;
	}
	if(enc_change_count >= 5){
		recent_enc_state = enc_state;
		enc_change_count = 0;
		if(enc_state == 1){
			Encoder_Num_2nd++;
		}
	}
	if(home_state != recent_home_state){
		home_change_count++;
	}else{
		home_change_count = 0;
	}
	if(home_change_count >= 5){
		recent_home_state = home_state;
		home_change_count = 0;
		if(home_state == 1){
			Encoder_Num_2nd = 15;
		}
	}
	if(Encoder_Num_2nd > 15){
		Encoder_Num_2nd = 1;
	}
}

static int Encoder_3rd(void){
	static int enc_change_count = 0;
	static int home_change_count = 0;
	static int recent_enc_state = 0;
	static int recent_home_state = 0;
	int enc_state = (int)IO_READ_3RD_ENC();
	int home_state = (int)IO_READ_3RD_HOME();
	if(enc_state != recent_enc_state){
		enc_change_count++;
	}else{
		enc_change_count = 0;
	}
	if(enc_change_count >= 5){
		recent_enc_state = enc_state;
		enc_change_count = 0;
		if(enc_state == 1){
			Encoder_Num_3rd++;
		}
	}
	if(home_state != recent_home_state){
		home_change_count++;
	}else{
		home_change_count = 0;
	}
	if(home_change_count >= 5){
		recent_home_state = home_state;
		home_change_count = 0;
		if(home_state == 1){
			Encoder_Num_3rd = 6;
		}
	}
	if(Encoder_Num_3rd > 6){
		Encoder_Num_3rd = 1;
	}
}


static int Encoder_JP(void){
	static int enc_change_count = 0;
	static int home_change_count = 0;
	static int recent_enc_state = 0;
	static int recent_home_state = 0;
	int enc_state = (int)IO_READ_JP_ENC();
	int home_state = (int)IO_READ_JP_HOME();
	if(enc_state != recent_enc_state){
		enc_change_count++;
	}else{
		enc_change_count = 0;
	}
	if(enc_change_count >= 5){
		recent_enc_state = enc_state;
		enc_change_count = 0;
		if(enc_state == 1){
			Encoder_Num_JP++;
		}
	}
	if(home_state != recent_home_state){
		home_change_count++;
	}else{
		home_change_count = 0;
	}
	if(home_change_count >= 5){
		recent_home_state = home_state;
		home_change_count = 0;
		if(home_state == 1){
			Encoder_Num_JP = 12;
		}
	}
	if(Encoder_Num_JP > 12){
		Encoder_Num_JP = 1;
	}
}


static void Lottery_1st2nd_SetSpeed(int speed){
	if(speed >= 1000) speed = 1000;
	if(speed <= 0) speed = 0;
	int setSpeed = (BLDC_MAX_SPEED / 1000.0) * speed;
	if(setSpeed == 0){
		IO_RESET_BLDC1_ENA();
	}else{
		IO_SET_BLDC1_ENA();
	}
	D_PWM_Set(BLDC1,setSpeed);
}

static void Lottery_3rd_SetSpeed(int speed){
	IO_SET_BLDC2_DIR();
	if(speed >= 1000) speed = 1000;
	if(speed <= 0) speed = 0;
	int setSpeed = (BLDC_MAX_SPEED / 1000.0) * speed;
	if(setSpeed == 0){
		IO_RESET_BLDC2_ENA();
	}else{
		IO_SET_BLDC2_ENA();
	}
	D_PWM_Set(BLDC2,setSpeed);
}

static void Lottery_JP_SetSpeed(int speed, int direction){
	if(speed >= 1000) speed = 1000;
	if(speed <= 0) speed = 0;
	int setSpeed = (BLDC_MAX_SPEED / 1000.0) * speed;
	if(setSpeed == 0){
		IO_RESET_BLDC3_ENA();
	}else{
		IO_SET_BLDC3_ENA();
	}
	if(direction == 0){
		IO_RESET_BLDC3_DIR();
	}else if(direction == 1){
		IO_SET_BLDC3_DIR();
	}
	D_PWM_Set(BLDC3,setSpeed);
}

static bool JP_Lift_Up(void){
	if(IO_READ_JP_UPPER()){
		IO_RESET_JPTOWER_ENA1();
		IO_RESET_JPTOWER_ENA2();
		return true;
	}else{
		IO_SET_JPTOWER_ENA1();
		IO_RESET_JPTOWER_ENA2();
		return false;
	}
}

static bool JP_Lift_Down(void){
	if(IO_READ_JP_LOWER()){
		IO_RESET_JPTOWER_ENA1();
		IO_RESET_JPTOWER_ENA2();
		return true;
	}else{
		IO_RESET_JPTOWER_ENA1();
		IO_SET_JPTOWER_ENA2();
		return false;
	}
}

