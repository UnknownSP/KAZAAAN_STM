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
#define IO_READ_ENC_STEP() (!D_GPIO_Read(GPIOB,GPIO_PIN_4))
#define IO_READ_ENC_HOME() (!D_GPIO_Read(GPIOB,GPIO_PIN_5))

#define LED_OFFSET 4

#endif /* INC_APP_H_ */
