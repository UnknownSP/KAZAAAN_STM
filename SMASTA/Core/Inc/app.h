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

#define IO_SET_USERLED() (D_GPIO_Set(GPIOA,GPIO_PIN_5))
#define IO_RESET_USERLED() (D_GPIO_Reset(GPIOA,GPIO_PIN_5))
#define IO_READ_USERBUTTON() (!D_GPIO_Read(GPIOC,GPIO_PIN_13))

#define IO_SET_BUMP_1()         (D_GPIO_Set(GPIOB,GPIO_PIN_15))
#define IO_RESET_BUMP_1()       (D_GPIO_Reset(GPIOB,GPIO_PIN_15))
#define IO_SET_BUMP_2()         (D_GPIO_Set(GPIOB,GPIO_PIN_5))
#define IO_RESET_BUMP_2()       (D_GPIO_Reset(GPIOB,GPIO_PIN_5))
#define IO_SET_KICKER()         (D_GPIO_Set(GPIOB,GPIO_PIN_14))
#define IO_RESET_KICKER()       (D_GPIO_Reset(GPIOB,GPIO_PIN_14))
#define IO_SET_STAYKICKER()     (D_GPIO_Set(GPIOB,GPIO_PIN_13))
#define IO_RESET_STAYKICKER()   (D_GPIO_Reset(GPIOB,GPIO_PIN_13))

#define IO_SET_MOTOR_ENA()     (D_GPIO_Set(GPIOC,GPIO_PIN_4))
#define IO_RESET_MOTOR_ENA()   (D_GPIO_Reset(GPIOC,GPIO_PIN_4))

#define IO_READ_BUMP_1_HIT() (!D_GPIO_Read(GPIOA,GPIO_PIN_4))
#define IO_READ_BUMP_1_SOL() (!D_GPIO_Read(GPIOB,GPIO_PIN_0))
#define IO_READ_BUMP_2_HIT() (!D_GPIO_Read(GPIOC,GPIO_PIN_11))
#define IO_READ_BUMP_2_SOL() (!D_GPIO_Read(GPIOD,GPIO_PIN_2))

#define IO_READ_KICKER_DOWN() (!D_GPIO_Read(GPIOC,GPIO_PIN_1))
#define IO_READ_STAYKICKER_DOWN() (D_GPIO_Read(GPIOB,GPIO_PIN_7))
#define IO_READ_BALL_DETECT() (!D_GPIO_Read(GPIOC,GPIO_PIN_0))
#define IO_READ_ENC_STEP() (D_GPIO_Read(GPIOC,GPIO_PIN_2))
#define IO_READ_ENC_HOME() (D_GPIO_Read(GPIOC,GPIO_PIN_3))

#define CROON_MOTOR_F_TIM 3
#define CROON_MOTOR_F_CHANNEL 1
#define CROON_MOTOR_R_TIM 3
#define CROON_MOTOR_R_CHANNEL 2

#define BUMP_1_R_TIM 2
#define BUMP_1_R_CHANNEL 3
#define BUMP_1_G_TIM 2
#define BUMP_1_G_CHANNEL 4
#define BUMP_1_B_TIM 3
#define BUMP_1_B_CHANNEL 4
#define BUMP_2_R_TIM 3
#define BUMP_2_R_CHANNEL 3
#define BUMP_2_G_TIM 2
#define BUMP_2_G_CHANNEL 1
#define BUMP_2_B_TIM 2
#define BUMP_2_B_CHANNEL 2

#define LED_OFFSET 5
#define LED_OFFSET_SHOWTIME 6
#define RAINBOW_FLOWTIME 100
#define BLINK_FLOWTIME 1500
#define GRADATION_CYCLETIME 7000
#define YELLOWRANDOM_FLOWTIME 50
#define BUMP_RESET_TIME 2500
#define BUMP_GOCROON_TIME 3500
#define STAYKICKER_ON_TIME 1000

#define CROON_INIT_SPEED 700
#define CROON_INIT_MIN_SPEED 150
#define CROON_ROTATE_SPEED 1000

#define POCKET_SHOWTIME 600

#define BUMP_1_WAITCOLOR_R 160
#define BUMP_1_WAITCOLOR_G 140
#define BUMP_1_WAITCOLOR_B 160
#define BUMP_2_WAITCOLOR_R 160
#define BUMP_2_WAITCOLOR_G 150
#define BUMP_2_WAITCOLOR_B 160

typedef enum{
    SM_INIT             = 4,
    SM_CROON_INIT       = 5,
    SM_BALL_DETECT      = 6,
    SM_CROON_SET_KICKER = 7,
    SM_LED_INIT         = 8,
    SM_LED_POCKET_SHOW  = 9,
    SM_LAUNCH_BALL      = 10,
    SM_LAUNCH_WAIT      = 11,
    SM_CROON_SET_BLOCK  = 12,
    SM_CROON_ROTATE_WAIT= 13,
    SM_CROON_BALL_DETECT= 14,
    SM_IN_POCKET_SHOW   = 15,
    SM_RESET_GAME       = 16,
}SMASTA_Mode;

typedef enum{
    LED_P_RED           = 3,
    LED_P_GREEN         = 4,
    LED_P_BLUE          = 5,
    LED_P_YELLOW        = 6,
    LED_P_PURPLE        = 7,
    LED_P_CYAN          = 8,
    LED_P_RAINBOW       = 20,
    LED_P_BLINK_PURPLE  = 21,
    LED_P_BLINK_YELLOW  = 22,
}LED_Pocket_Mode;

#endif /* INC_APP_H_ */
