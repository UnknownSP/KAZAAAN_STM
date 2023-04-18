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

int appInit(void){

	return 0;
}

int appTask(void){
	//D_Mess_printf("%d\n", G_System_counter);
	//D_PWM_Set(1,100);
	//D_PWM_Set(2,100);
	IO_SET_BLDC2_DIR();
	if(IO_READ_USERBUTTON()){
		IO_SET_USERLED();
		//IO_SET_BLDC1_ENA();
		//IO_SET_BLDC2_ENA();
		//D_PWM_Set(BLDC1,50);
		//D_PWM_Set(BLDC2,50);
		//D_PWM_Set(BLDC3,300);
		//IO_SET_JP_LED();

		//JP_Lift_Down();
	}else{
		IO_RESET_USERLED();
		IO_RESET_BLDC1_ENA();
		IO_RESET_BLDC2_ENA();
		IO_RESET_BLDC3_ENA();
		D_PWM_Set(BLDC1,300);
		D_PWM_Set(BLDC2,3000);
		//D_PWM_Set(BLDC3,3500);
		//IO_RESET_JP_LED();

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


	return 0;
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

