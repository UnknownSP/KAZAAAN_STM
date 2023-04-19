/*
 * app.h
 *
 *  Created on: Mar 11, 2023
 *      Author: UnknownSP
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include "main.h"

int appTask(void);
int appInit(void);

typedef enum{
    JPC_ROT_START_SLOW_1= 0,
    JPC_LED_ON          = 1,
    JPC_LIFT_UP         = 2,
    JPC_WAIT_1          = 3,
    JPC_WAIT_INPUT_1    = 4,
    JPC_ROT_START_1     = 5,
    JPC_DECELERATION_1  = 6,
    JPC_STOP_1          = 7,
    JPC_JUDGE_1         = 8,
    JPC_WAIT_INPUT_2    = 9,
    JPC_ROT_START_2     = 10,
    JPC_DECELERATION_2  = 11,
    JPC_STOP_2          = 12,
    JPC_JUDGE_2         = 13,
    JPC_ROT_START_SLOW_2= 14,
    JPC_LED_OFF         = 15,
    JPC_LIFT_DOWN       = 16,
    JPC_STACK_PROCESS   = 17,
}JPC_Process_State;

#define BLDC_MAX_SPEED 5000

#define JPC_MAX_SPEED 800
#define JPC_MIN_SPEED 20

//1st and 2nd BLDC controller
#define IO_RESET_BLDC1_ENA() (D_GPIO_Set(GPIOC,GPIO_PIN_6))
#define IO_SET_BLDC1_ENA() (D_GPIO_Reset(GPIOC,GPIO_PIN_6))
#define IO_SET_BLDC1_DIR() (D_GPIO_Set(GPIOC,GPIO_PIN_5))
#define IO_RESET_BLDC1_DIR() (D_GPIO_Reset(GPIOC,GPIO_PIN_5))

//3rd BLDC controller
#define IO_RESET_BLDC2_ENA() (D_GPIO_Set(GPIOA,GPIO_PIN_12))
#define IO_SET_BLDC2_ENA() (D_GPIO_Reset(GPIOA,GPIO_PIN_12))
#define IO_SET_BLDC2_DIR() (D_GPIO_Set(GPIOA,GPIO_PIN_11))
#define IO_RESET_BLDC2_DIR() (D_GPIO_Reset(GPIOA,GPIO_PIN_11))

//JPC BLDC controller
#define IO_RESET_BLDC3_ENA() (D_GPIO_Set(GPIOB,GPIO_PIN_12))
#define IO_SET_BLDC3_ENA() (D_GPIO_Reset(GPIOB,GPIO_PIN_12))
#define IO_SET_BLDC3_DIR() (D_GPIO_Set(GPIOB,GPIO_PIN_2))
#define IO_RESET_BLDC3_DIR() (D_GPIO_Reset(GPIOB,GPIO_PIN_2))

#define IO_SET_JPTOWER_ENA1() (D_GPIO_Set(GPIOB,GPIO_PIN_15))
#define IO_RESET_JPTOWER_ENA1() (D_GPIO_Reset(GPIOB,GPIO_PIN_15))
#define IO_SET_JPTOWER_ENA2() (D_GPIO_Set(GPIOB,GPIO_PIN_14))
#define IO_RESET_JPTOWER_ENA2() (D_GPIO_Reset(GPIOB,GPIO_PIN_14))

#define IO_SET_JP_LED() (D_GPIO_Set(GPIOC,GPIO_PIN_8))
#define IO_RESET_JP_LED() (D_GPIO_Reset(GPIOC,GPIO_PIN_8))

#define IO_READ_JP_HOME() (D_GPIO_Read(GPIOA,GPIO_PIN_1))
#define IO_READ_JP_ENC() (D_GPIO_Read(GPIOA,GPIO_PIN_4))
#define IO_READ_JP_FRONT() (D_GPIO_Read(GPIOC,GPIO_PIN_1))
#define IO_READ_JP_REAR() (D_GPIO_Read(GPIOC,GPIO_PIN_0))
#define IO_READ_JP_UPPER() (D_GPIO_Read(GPIOC,GPIO_PIN_3))
#define IO_READ_JP_LOWER() (D_GPIO_Read(GPIOC,GPIO_PIN_2))

#define IO_READ_1ST_HOME() (D_GPIO_Read(GPIOB,GPIO_PIN_7))
#define IO_READ_1ST_ENC() (D_GPIO_Read(GPIOA,GPIO_PIN_15))
#define IO_READ_2ND_HOME() (D_GPIO_Read(GPIOC,GPIO_PIN_12))
#define IO_READ_2ND_ENC() (D_GPIO_Read(GPIOC,GPIO_PIN_10))
#define IO_READ_3RD_HOME() (D_GPIO_Read(GPIOC,GPIO_PIN_11))
#define IO_READ_3RD_ENC() (D_GPIO_Read(GPIOD,GPIO_PIN_2))

#define IO_SET_USERLED() (D_GPIO_Set(GPIOA,GPIO_PIN_5))
#define IO_RESET_USERLED() (D_GPIO_Reset(GPIOA,GPIO_PIN_5))
#define IO_READ_USERBUTTON() (!D_GPIO_Read(GPIOC,GPIO_PIN_13))

#define BLDC1 4
#define BLDC2 2
#define BLDC3 1


#endif /* INC_APP_H_ */
