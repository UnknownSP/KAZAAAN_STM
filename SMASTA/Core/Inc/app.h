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

#define IO_SET_BUMP_1()         (D_GPIO_Set(GPIOB,GPIO_PIN_5))
#define IO_RESET_BUMP_1()       (D_GPIO_Reset(GPIOB,GPIO_PIN_5))
#define IO_SET_BUMP_2()         (D_GPIO_Set(GPIOB,GPIO_PIN_13))
#define IO_RESET_BUMP_2()       (D_GPIO_Reset(GPIOB,GPIO_PIN_13))
#define IO_SET_KICKER()         (D_GPIO_Set(GPIOB,GPIO_PIN_14))
#define IO_RESET_KICKER()       (D_GPIO_Reset(GPIOB,GPIO_PIN_14))
#define IO_SET_STAYKICKER()     (D_GPIO_Set(GPIOB,GPIO_PIN_15))
#define IO_RESET_STAYKICKER()   (D_GPIO_Reset(GPIOB,GPIO_PIN_15))

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

#define LED_OFFSET 5
#define RAINBOW_FLOWTIME 100
#define BLINK_FLOWTIME 1500
#define GRADATION_CYCLETIME 7000

#endif /* INC_APP_H_ */
