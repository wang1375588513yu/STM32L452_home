/********************************Copyright (c)******************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                       		By(MR.Richard)
**
**----------------------------------File Information------------------------
** FileName:      led.h
** The Author:    WangYu
** Creation Date: 2018-08-27
** Description:   
**
**--------------------------------Version Information-----------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**--------------------------------------------------------------------------
\********************************End of Head********************************/



#ifndef __LED_H__
#define __LED_H__

#include "user_config.h"

#ifdef	LED_ENABLE
#define LED_PIN_PA5			1
#define LED_PIN_PB13		2
#define LED_SWITCH		LED_PIN_PA5

#if (LED_SWITCH==LED_PIN_PA5)
#define LED_RCC_GPIO_CLK_ENABLE() 		__HAL_RCC_GPIOA_CLK_ENABLE()
#define LED_GPIO						GPIOA
#define LED_PIN							GPIO_PIN_5
#else
#define LED_RCC_GPIO_CLK_ENABLE() 		__HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_GPIO						GPIOB
#define LED_PIN							GPIO_PIN_13
#endif

#define LED_ON							GPIO_PIN_SET		
#define LED_OFF							GPIO_PIN_RESET

void Led_Init(void);
void Led_On(void);
void Led_Off(void);
void Led_Status_Reversal(void);
void led_task(void);



#endif
#endif

