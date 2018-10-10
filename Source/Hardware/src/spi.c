/********************************Copyright (c)******************************\
**
**                    (c) Copyright 2018, WangYu, China, SZ.
**                            All Rights Reserved
**
**                               By(MR.Richard)
**
**----------------------------------File Information------------------------
** FileName:      spi.c
** The Author:    WangYu
** Creation Date: 2018-10-06
** Description:   
**
**--------------------------------Version Information-----------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**--------------------------------------------------------------------------
\********************************End of Head********************************/
#include "spi.h"
#include "delay.h"
#include "sys.h"

SPI_HandleTypeDef spi1_handler;
void SPI1_Init(void)
{
	spi1_handler.Instance=SPI1;                         //SPI1
    spi1_handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    spi1_handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    spi1_handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    spi1_handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    spi1_handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    spi1_handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    spi1_handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
    spi1_handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    spi1_handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    spi1_handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    spi1_handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&spi1_handler);						//初始化
    
    __HAL_SPI_ENABLE(&spi1_handler);                    //使能SPI1
	
    SPI1_ReadWriteByte(0Xff);                           //启动传输
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    SPI1_RCC_GPIO_CLK_ENABLE();       //使能GPIOF时钟
    __HAL_RCC_SPI1_CLK_ENABLE();        //使能SPI5时钟
	
    GPIO_Initure.Pin=SPI1_MOSI_PIN | SPI1_SCK_PIN | SPI1_NSS_PIN; 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速            
    GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //复用为SPI1
    HAL_GPIO_Init(SPI1_GPIO,&GPIO_Initure);
	
	GPIO_Initure.Pin=SPI1_MISO_PIN;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;              //INPUT 模式
    HAL_GPIO_Init(SPI1_GPIO,&GPIO_Initure);
}
/*******************************************************************************
** FunctionName:  SPI5_SetSpeed
** Description:   SPI速度设置  SPI速度=fAPB1/分频系数
** Parameters:    SPI_BaudRatePrescaler: 分频系数 SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_256
** Returned:      None
** The Author:    WangYu
** Creation Date: 2018-10-06
**------------------------------------------------------------------------------
** Changed By:
** Changed Date:
** Changed explain:
**------------------------------------------------------------------------------
********************************************************************************/
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&spi1_handler);            //关闭SPI
    spi1_handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    spi1_handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&spi1_handler);             //使能SPI
    
}
/*******************************************************************************
** FunctionName:  SPI5_ReadWriteByte
** Description:   读写一个字节
** Parameters:    TxData: 要写入的字节 
** Returned:      读取到的字节
** The Author:    WangYu
** Creation Date: 2018-10-06
**------------------------------------------------------------------------------
** Changed By:
** Changed Date:
** Changed explain:
**------------------------------------------------------------------------------
********************************************************************************/
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&spi1_handler,&TxData,&Rxdata,1,1000);       
 	return Rxdata;          		    //返回收到的数据		
}



