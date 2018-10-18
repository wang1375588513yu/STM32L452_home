#ifndef __DHT11_H__
#define __DHT11_H__

#include "PUBLIC.h"
#include "sys.h"

#define DHT11_RCC_GPIO_CLK_ENABLE() 		__HAL_RCC_GPIOA_CLK_ENABLE()
#define DHT11_GPIO							GPIOA
#define DHT11_PIN							GPIO_PIN_10

#endif
