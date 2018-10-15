#ifndef ___SPI_H___
#define ___SPI_H___
#include "sys.h"
#include "user_config.h"

#ifdef SPI1_ENABLE
/************************
 * SPI1_NSS		--	PA4 *
 * SPI1_SCK		--	PA5 *
 * SPI1_MISO	--	PA6 *
 * SPI1_MOSI	--	PA7 *
 ************************/
#define SPI1_RCC_GPIO_CLK_ENABLE() 			__HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_GPIO							GPIOA
#define SPI1_SCK_GPIO						GPIOB
#define SPI1_NSS_PIN						GPIO_PIN_4	
#define SPI1_SCK_PIN						GPIO_PIN_3	
#define SPI1_MISO_PIN						GPIO_PIN_6	
#define SPI1_MOSI_PIN						GPIO_PIN_7	

extern SPI_HandleTypeDef spi1_handler;  //SPI1¾ä±ú

void SPI1_Init(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_ReadWriteByte(u8 TxData);
#endif

#ifdef SPI2_ENABLE
/************************
 * SPI2_NSS		--	PB6 *
 * SPI2_SCK		--	PB13 *
 * SPI2_MISO	--	PB14 *
 * SPI2_MOSI	--	PB15 *
 ************************/
#define SPI2_RCC_GPIO_CLK_ENABLE() 			__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_GPIO							GPIOB	
#define SPI2_SCK_PIN						GPIO_PIN_13	
#define SPI2_MOSI_PIN						GPIO_PIN_15

extern SPI_HandleTypeDef spi2_handler;  //SPI2¾ä±ú

void SPI2_Init(void);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI2_ReadWriteByte(u8 TxData);

#endif
#endif
