/********************************Copyright (c)******************************\
**
**                    (c) Copyright 2018, WangYu, China, SZ.
**                            All Rights Reserved
**
**                               By(MR.Richard)
**
**----------------------------------File Information------------------------
** FileName:      dht11.c
** The Author:    WangYu
** Creation Date: 2018-10-17
** Description:   
**
**--------------------------------Version Information-----------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**--------------------------------------------------------------------------
\********************************End of Head********************************/
#include "dht11.h"

void dht11_init(void)
{
	GPIO_InitTypeDef GPIO_Initure;

    DHT11_RCC_GPIO_CLK_ENABLE();					//初始化时钟

    GPIO_Initure.Pin = DHT11_PIN;					//DHT11引脚
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;		//高速率
    GPIO_Initure.Pull = GPIO_PULLUP;				//上拉
    HAL_GPIO_Init(DHT11_GPIO, &GPIO_Initure);
}

void dht11_mode_out(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Pin = DHT11_PIN;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
}

void dht11_mode_in(void)
{
	
}



