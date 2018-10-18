#ifndef __DHT11_H__
#define __DHT11_H__

#include "PUBLIC.h"
#include "sys.h"
#include "user_config.h"

#ifdef DHT11_ENABLE
#define LOW		0
#define HIGH	1

#define DHT11_RCC_GPIO_CLK_ENABLE() 		__HAL_RCC_GPIOA_CLK_ENABLE()
#define DHT11_GPIO							GPIOA
#define DHT11_PIN							GPIO_PIN_10

typedef struct
{
	unsigned char  humi_int;		//湿度的整数部分
	unsigned char  humi_deci;	 	//湿度的小数部分
	unsigned char  temp_int;	 	//温度的整数部分
	unsigned char  temp_deci;	 	//温度的小数部分
	unsigned char  check_sum;	 	//校验和
}DHT11_Data_t;

void dht11_init(void);
void dht11_task(void);

#endif
#endif
