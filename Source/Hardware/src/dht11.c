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
#include "delay.h"
#include "timer.h"
#include "PUBLIC.H"

#ifdef DHT11_ENABLE

#define DHT11_DATA_OUT(a)	if(a) HAL_GPIO_WritePin(DHT11_GPIO, DHT11_PIN, GPIO_PIN_SET);\
							else  HAL_GPIO_WritePin(DHT11_GPIO, DHT11_PIN, GPIO_PIN_RESET)
#define DHT11_DATA_IN()		HAL_GPIO_ReadPin(DHT11_GPIO, DHT11_PIN)

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

static void dht11_mode_out(void)
{
	GPIO_InitTypeDef GPIO_Initure;

    GPIO_Initure.Pin = DHT11_PIN;					//DHT11引脚
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;		//高速率
    GPIO_Initure.Pull = GPIO_PULLUP;				//上拉
    HAL_GPIO_Init(DHT11_GPIO, &GPIO_Initure);
}

static void dht11_mode_in(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Pin = DHT11_PIN;
	GPIO_Initure.Mode = GPIO_MODE_INPUT;
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;		//高速率
    GPIO_Initure.Pull = GPIO_NOPULL;				//上拉
	HAL_GPIO_Init(DHT11_GPIO, &GPIO_Initure);
}


/*******************************************************************************
** FunctionName:  dht11_readbyte
** Description:   读一个字节的数据
** Parameters:    None
** Returned:      None
** The Author:    WangYu
** Creation Date: 2018-10-18
**------------------------------------------------------------------------------
** Changed By:
** Changed Date:
** Changed explain:
**------------------------------------------------------------------------------
********************************************************************************/
static unsigned char dht11_readbyte(void)
{
	unsigned char __i,temp;
	for(__i=0; __i<8; __i++)
	{
		while(DHT11_DATA_IN() == GPIO_PIN_RESET);
		delay_us(40);
		if(DHT11_DATA_IN() == GPIO_PIN_SET)
		{
			while(DHT11_DATA_IN() == GPIO_PIN_SET);
			temp = (unsigned char)(0x01<<(7-__i));
		}
		else
		{
			temp &= (unsigned char)~(0x01<<(7-__i));
		}
	}
	return temp;
}
/*******************************************************************************
** FunctionName:  Read_dht11_data
** Description:   读取温湿度数据
** Parameters:    dht11_data: 存储温湿度数据的结构体 
** Returned:      None
** The Author:    WangYu
** Creation Date: 2018-10-18
**------------------------------------------------------------------------------
** Changed By:
** Changed Date:
** Changed explain:
**------------------------------------------------------------------------------
********************************************************************************/
bool Read_dht11_data(DHT11_Data_t* dht11_data)
{
	dht11_mode_out();
	DHT11_DATA_OUT(LOW);
	delay_ms(18);
	DHT11_DATA_OUT(HIGH);
	delay_us(30);
	dht11_mode_in();

	if(DHT11_DATA_IN() == GPIO_PIN_RESET)
	{
		while(DHT11_DATA_IN() == GPIO_PIN_RESET);
		while(DHT11_DATA_IN() == GPIO_PIN_SET);
		
		dht11_data->humi_int  = dht11_readbyte();
		dht11_data->humi_deci = dht11_readbyte();
		dht11_data->temp_int  = dht11_readbyte();
		dht11_data->temp_deci = dht11_readbyte();
		dht11_data->check_sum = dht11_readbyte();

		dht11_mode_out();
		DHT11_DATA_OUT(HIGH);

		if(dht11_data->check_sum == dht11_data->humi_int  + 
									dht11_data->humi_deci + 
									dht11_data->temp_int  + 
									dht11_data->temp_deci)
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		return false;
	}
}

static void dht11_loop(void)
{
	static unsigned int softimersec=0xffffffff;
    if(softimersec==Get_SysTick())return;
    softimersec=Get_SysTick();
	
	
}

void dht11_task(void)
{
	dht11_loop();
}

#endif

