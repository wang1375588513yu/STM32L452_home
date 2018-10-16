/********************************Copyright (c)******************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                       		By(MR.Richard)
**
**----------------------------------File Information------------------------
** FileName:      oled.h
** The Author:    WangYu
** Creation Date: 2018-10-10
** Description:   
**
**--------------------------------Version Information-----------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**--------------------------------------------------------------------------
\********************************End of Head********************************/

#ifndef __OLED_H__
#define __OLED_H__

#include "PUBLIC.h"
#include "sys.h"
#include "delay.h"
#include "user_config.h"
#include "stdlib.h"
#include "spi.h"

#ifdef OLED_ENABLE

#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64

#define OLED_RCC_GPIO_CLK_ENABLE() 		__HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_GPIO						GPIOB

#define OLED_RES_PIN					GPIO_PIN_12
#define OLED_DC_PIN						GPIO_PIN_1
#define OLED_CS_PIN						GPIO_PIN_2
#define SPI2_SCK_PIN					GPIO_PIN_13	
#define SPI2_MOSI_PIN					GPIO_PIN_15
//-----------------OLED端口定义----------------
#ifdef SPI1_ENABLE
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_GPIO, SPI1_SCK_PIN, GPIO_PIN_RESET);//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_GPIO, SPI1_SCK_PIN, GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_GPIO, SPI1_MOSI_PIN, GPIO_PIN_RESET)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_GPIO, SPI1_MOSI_PIN, GPIO_PIN_SET)
#else
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_GPIO, SPI2_SCK_PIN, GPIO_PIN_RESET);//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_GPIO, SPI2_SCK_PIN, GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_GPIO, SPI2_MOSI_PIN, GPIO_PIN_RESET)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_GPIO, SPI2_MOSI_PIN, GPIO_PIN_SET)
#endif

#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_GPIO, OLED_RES_PIN, GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_GPIO, OLED_RES_PIN, GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_GPIO, OLED_DC_PIN, GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_GPIO, OLED_DC_PIN, GPIO_PIN_SET)

#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_GPIO, OLED_CS_PIN, GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_GPIO, OLED_CS_PIN, GPIO_PIN_SET)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_Show_task(void);

void OLED_Clear_Line(u8 x, u8 y);

#endif
#endif
