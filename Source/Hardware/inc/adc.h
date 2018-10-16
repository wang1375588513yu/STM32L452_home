#ifndef ___ADC_H___
#define ___ADC_H___

#include "sys.h"

extern ADC_HandleTypeDef ADC1_Handler;
extern DMA_HandleTypeDef DMA_Handler;

#define ADCx_RCC_CLK_ENABLE()            __HAL_RCC_ADC1_CLK_ENABLE()
#define ADCx_RCC_CLK_DISABLE()           __HAL_RCC_ADC1_CLK_DISABLE()
#define DMAx_RCC_CLK_ENABLE()            __HAL_RCC_DMA1_CLK_ENABLE()
#define ADC_DMAx_CHANNELn                DMA1_Channel1

u16 Get_Adc(u32 ch);
void adc_init(void);
u16 Get_Adc_Average(u32 nChannel); 
float Get_CPU_Temprate(void);

#endif
