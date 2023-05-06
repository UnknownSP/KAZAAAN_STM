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

static int Encoder_Process(void);

int appInit(void){
	return 0;
}

int appTask(void){
	Encoder_Process();

	//if(IO_READ_USERBUTTON()){
		//IO_SET_USERLED();
		for(int i=0; i<5; i++){
        	int r=0;
        	int g=0;
        	int b=0;
        	switch (i)
        	{
        	case 0:
        	  	r=1;
        	  	g=0;
        	  	b=0;
        	  	break;
	
        	case 1:
        	  	r=0;
        	  	g=1;
        	  	b=0;
        	  	break;
	
        	case 2:
        	  	r=0;
        	  	g=0;
        	  	b=1;
        	  	break;
	
        	case 3:
        	  	r=1;
        	  	g=1;
        	  	b=0;
        	  	break;
	
        	case 4:
        	  	r=1;
        	  	g=0;
        	  	b=1;
        	  	break;
        	}
        	for(int j=0;j<10;j++){
				D_LED_Set(i*10+j, (j+1)*25*r, (j+1)*25*g, (j+1)*25*b);
        	}
      	}
		D_LED_Rotate(-Encoder_Count + LED_OFFSET);
      	D_LED_Send();
	//}else{
	//	//IO_RESET_USERLED();
	//	for(int i=0; i<50; i++){
    //    	D_LED_Set(i, 0, 0, 0);
    //  	}
    //  	D_LED_Send();
	//}

	int16_t debug_bits = 0;
	debug_bits |= ((int)IO_READ_ENC_HOME() << 1);
	debug_bits |= ((int)IO_READ_ENC_STEP() << 0);
	D_Mess_printf("\033[1;1H");
	D_Mess_printf("[%10d]\n",G_System_counter);
	D_Mess_printf("[%10d]\n",Encoder_Count);
	D_Mess_printf("%016b\n", debug_bits);


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
		if(enc_state == 1){
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
			Encoder_Count = 1;
		}
	}
	if(Encoder_Count > 10){
		reset = false;
		//Encoder_Count = 0;
	}
	if(Encoder_Count > 50){
		Encoder_Count = 1;
	}

	return 0;
}

