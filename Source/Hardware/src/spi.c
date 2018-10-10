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
    spi1_handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    spi1_handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    spi1_handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    spi1_handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    spi1_handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    spi1_handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    spi1_handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    spi1_handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    spi1_handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    spi1_handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    spi1_handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&spi1_handler);						//��ʼ��
    
    __HAL_SPI_ENABLE(&spi1_handler);                    //ʹ��SPI1
	
    SPI1_ReadWriteByte(0Xff);                           //��������
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    SPI1_RCC_GPIO_CLK_ENABLE();       //ʹ��GPIOFʱ��
    __HAL_RCC_SPI1_CLK_ENABLE();        //ʹ��SPI5ʱ��
	
    GPIO_Initure.Pin=SPI1_MOSI_PIN | SPI1_SCK_PIN | SPI1_NSS_PIN; 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
    GPIO_Initure.Pull=GPIO_PULLUP;                  //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����            
    GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //����ΪSPI1
    HAL_GPIO_Init(SPI1_GPIO,&GPIO_Initure);
	
	GPIO_Initure.Pin=SPI1_MISO_PIN;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;              //INPUT ģʽ
    HAL_GPIO_Init(SPI1_GPIO,&GPIO_Initure);
}
/*******************************************************************************
** FunctionName:  SPI5_SetSpeed
** Description:   SPI�ٶ�����  SPI�ٶ�=fAPB1/��Ƶϵ��
** Parameters:    SPI_BaudRatePrescaler: ��Ƶϵ�� SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_256
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
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&spi1_handler);            //�ر�SPI
    spi1_handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    spi1_handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&spi1_handler);             //ʹ��SPI
    
}
/*******************************************************************************
** FunctionName:  SPI5_ReadWriteByte
** Description:   ��дһ���ֽ�
** Parameters:    TxData: Ҫд����ֽ� 
** Returned:      ��ȡ�����ֽ�
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
 	return Rxdata;          		    //�����յ�������		
}



