/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                        By(~~~~~~~~~~~~~~~~~~~~~~~)
**
**----------------------------------File Information----------------------------
** FileName:      usart.c
** The Author:    WangYu
** Creation Date: 2018-09-05
** Description:
**
**--------------------------------Version Information---------------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
#include "usart.h"
#include "timer.h"
#include "PUBLIC.H"

#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}

UART_HandleTypeDef 	UART1_Handler;
UART_HandleTypeDef 	UART2_Handler; 
UART_HandleTypeDef 	UART3_Handler;
UART_HandleTypeDef 	UART4_Handler; 
GPIO_InitTypeDef 	GPIO_Init; 
DMA_HandleTypeDef   TX_DMA_Handler;
DMA_HandleTypeDef   RX_DMA_Handler;

#define RXBUFFERSIZE    	1 //�����С
u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

/*******************************************************************************
** ��������:  	UartTX_DMA_Init
** ��������: 	���ڷ���DMA��ʼ��
** �������:	huart: ���� 
**			 	dma_channel: ͨ����
**			 	requst: ������(�ο�datasheet)
** ��    ��:	None
** �� �� ֵ:	None
** ��    ��:	WangYu
** ��    �ڣ�	2019-07-09
** �޸ļ�¼��	
********************************************************************************/
static void UartTX_DMA_Init(UART_HandleTypeDef* huart, DMA_Channel_TypeDef *dma_channel, u32 requst)
{
	if((u32)dma_channel > (u32)DMA2)
	{
		__HAL_RCC_DMA2_CLK_ENABLE();
	}
	else
	{
		__HAL_RCC_DMA1_CLK_ENABLE();
	}
	
	__HAL_LINKDMA(huart,hdmatx,TX_DMA_Handler);    //��DMA��USART1��ϵ����(����DMA)
	
	TX_DMA_Handler.Instance 					= dma_channel;			//ͨ��ѡ��
	TX_DMA_Handler.Init.Direction 				= DMA_MEMORY_TO_PERIPH;	//���ݴ��䷽�� �ڴ浽���輴����ģʽ
	TX_DMA_Handler.Init.Priority 				= DMA_PRIORITY_HIGH;	//���ȼ�	
	TX_DMA_Handler.Init.Mode 					= DMA_NORMAL;			//����ģʽ
	TX_DMA_Handler.Init.MemDataAlignment 		= DMA_MDATAALIGN_BYTE;	//�ڴ����ݳ���
	TX_DMA_Handler.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_BYTE;	//�������ݳ���
	TX_DMA_Handler.Init.PeriphInc 				= DMA_PINC_DISABLE;		//���������ģʽ
	TX_DMA_Handler.Init.MemInc 					= DMA_MINC_ENABLE;		//�ڴ�����ģʽ
	TX_DMA_Handler.Init.Request 				= requst;				//ѡ�������	
	
	HAL_DMA_DeInit(&TX_DMA_Handler);
	HAL_DMA_Init(&TX_DMA_Handler);
}

/*******************************************************************************
** ��������:  	UartRX_DMA_Init
** ��������: 	���ڽ���DMA��ʼ��
** �������:	huart: ���� 
**			 	dma_channel: ͨ����
**			 	requst: ������(�ο�datasheet)
** ��    ��:	None
** �� �� ֵ:	None
** ��    ��:	WangYu
** ��    �ڣ�	2019-07-09
** �޸ļ�¼��	
********************************************************************************/
void UartRX_DMA_Init(UART_HandleTypeDef* huart, DMA_Channel_TypeDef *dma_channel, u32 requst)
{
	if((u32)dma_channel > (u32)DMA2)
	{
		__HAL_RCC_DMA2_CLK_ENABLE();
	}
	else
	{
		__HAL_RCC_DMA1_CLK_ENABLE();
	}
	__HAL_LINKDMA(&UART1_Handler,hdmarx,RX_DMA_Handler);    //��DMA��USART��ϵ����(����DMA)
	
	RX_DMA_Handler.Instance 					= dma_channel;			//ͨ��ѡ��
	RX_DMA_Handler.Init.Direction 				= DMA_PERIPH_TO_MEMORY;	//���ݴ��䷽�� �ڴ浽���輴����ģʽ
	RX_DMA_Handler.Init.Priority 				= DMA_PRIORITY_HIGH;	//���ȼ�	
	RX_DMA_Handler.Init.Mode 					= DMA_NORMAL;			//����ģʽ
	RX_DMA_Handler.Init.MemDataAlignment 		= DMA_MDATAALIGN_BYTE;	//�ڴ����ݳ���
	RX_DMA_Handler.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_BYTE;	//�������ݳ���
	RX_DMA_Handler.Init.PeriphInc 				= DMA_PINC_DISABLE;		//���������ģʽ
	RX_DMA_Handler.Init.MemInc 					= DMA_MINC_ENABLE;		//�ڴ�����ģʽ
	RX_DMA_Handler.Init.Request 				= requst;				//ѡ�������	
	
	HAL_DMA_DeInit(&RX_DMA_Handler);
	HAL_DMA_Init(&RX_DMA_Handler);
}

/*******************************************************************************
** ��������:  	DMA_UART_Transmit
** ��������: 	��DMAģʽ���д���
** �������:	huart: ���ں�
**			 	pData: ����
**			 	Size:  ���ݳ���
** ��    ��:	None
** �� �� ֵ:	None
** ��    ��:	WangYu
** ��    �ڣ�	2019-07-09
** �޸ļ�¼��	
********************************************************************************/
static void DMA_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    HAL_DMA_Start(huart->hdmatx, (u32)pData, (uint32_t)&huart->Instance->RDR, Size);//����DMA����  
    huart->Instance->CR3 |= USART_CR3_DMAT;//ʹ�ܴ���DMA����
}	 

#if UART1_ENABLE
#if USART1_USE_PIPE
Pipe_t uart1_pipe = {0};
#else
Pipe1_t uart1_pipe = {0};
#endif
void reset_uart_pipe1()
{
    #ifdef USART1_USE_PIPE
    Pipe_Reset(&uart1_pipe);
	#else
	Pipe1_Reset(&uart1_pipe);
	#endif
}

static void UART1_init(u32 bound)
{     
    __HAL_RCC_UART1_GPIO_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    //PA9 Tx
    GPIO_Init.Pin 		= UART1_TX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(UART1_GPIO, &GPIO_Init);

    //PA10 Rx
    GPIO_Init.Pin 		= UART1_RX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(UART1_GPIO, &GPIO_Init);

    UART1_Handler.Instance          = USART1;
    UART1_Handler.Init.BaudRate     = bound;
    UART1_Handler.Init.WordLength   = UART_WORDLENGTH_8B;
    UART1_Handler.Init.StopBits     = UART_STOPBITS_1;
    UART1_Handler.Init.Parity       = UART_PARITY_NONE;
    UART1_Handler.Init.Mode         = UART_MODE_TX_RX;
    HAL_UART_Init(&UART1_Handler);
	
	HAL_NVIC_EnableIRQ(USART1_IRQn);		//ʹ��USART2�ж�ͨ��
    HAL_NVIC_SetPriority(USART1_IRQn,3,3);	//��ռ���ȼ�3�������ȼ�3

	UartTX_DMA_Init(&UART1_Handler, DMA_UART1_TX_CHANNEL, DMA_UART1_TX_REQUST);	//��ʼ�������ж�
    /*�ú����Ὺ�������ж�,��־λUART_IT_RXNE��λ*/
    /*�������ý��ջ���ͽ��ջ�����������*/
	HAL_UART_Receive_IT(&UART1_Handler, aRxBuffer, RXBUFFERSIZE);
    reset_uart_pipe1();
}

void USART1_IRQHandler(void)
{
	u8 Res;
    if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
		Res=USART1->RDR; 	//��������
		uart1_printf((char*)&Res);
	//	Pipe_Block_Input(&uart1_pipe,(const char*)&Res,1);
        uart1_pipe.pipe_buf[uart1_pipe.in_pos]=Res;
        uart1_pipe.in_pos++;
        uart1_pipe.in_pos%=PIPE_BUF_LEN;
        if(PIPE_BUF_LEN > uart1_pipe.sum)	
			uart1_pipe.sum++;
        else	
			uart1_pipe.out_pos=uart1_pipe.in_pos;
        uart1_pipe.time=Get_SysmilliTick();
    }
}

void uart1_output(unsigned char *buf,int len)
{
#if (UART1_DMA_ENABLE == 1)	//ʹ��DMA���д���
	DMA_UART_Transmit(&UART1_Handler, (u8 *)buf, len);	
}
#else
    unsigned int __i,timespan = Get_SysmilliTick();
    if(buf==NULL ||len==0)  return;
	
    for(__i = 0; __i < len; __i++)
    {
        while((USART1->ISR & USART_FLAG_TC) == (uint16_t)RESET)//��ȡ������ɱ�־TC transmission complete
        {//��ʱ����������� 
            if(Get_SysmilliTick()-timespan>10)
            {/*���һ����ʱ��ʱ����������while����ͣ������10ms���Զ�������*/
                break;
            }
        }		
        timespan = Get_SysmilliTick();
        USART1->TDR = ((uint8_t)buf[__i] & (uint16_t)0x01FF);
    }
}
#endif

void uart1_printf(char * string, ...)
{
    __va_list arg;
    int len=0;
    if(string==NULL)  return;

    if(strlen(string)>=UART_TX_LEN)return;
    va_start(arg, string);
    memset(_txBuffer,0,UART_TX_LEN);
    len=vsprintf((char *)_txBuffer, string, arg); /*must use "vsprintf" */
    va_end(arg);
    uart1_output((unsigned char *)_txBuffer,len);
}
#endif

#if UART2_ENABLE
#if USART2_USE_PIPE == 1
Pipe_t uart2_pipe = {0};
#else
Pipe1_t uart2_pipe = {0};
#define USART2_RX_LOG_BUFER_SIZE 200
unsigned char USART2_RX_LOG_BUFER[USART2_RX_LOG_BUFER_SIZE];
Pipe1_t uart_pipe2_log={USART2_RX_LOG_BUFER,USART2_RX_LOG_BUFER_SIZE,0,0,0,0};
#endif
void reset_uart_pipe2()
{
   	#if USART2_USE_PIPE == 1
    Pipe_Reset(&uart2_pipe);
	#else
	Pipe1_Reset(&uart2_pipe);
	#endif
}

static void UART2_init(u32 bound)
{
    __HAL_RCC_UART2_GPIO_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    //PA2 Tx
    GPIO_Init.Pin 		= UART2_TX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(UART2_GPIO, &GPIO_Init);

    //PA3 Rx
    GPIO_Init.Pin 		= UART2_RX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(UART2_GPIO, &GPIO_Init);

    HAL_NVIC_EnableIRQ(USART2_IRQn);		//ʹ��USART2�ж�ͨ��
    HAL_NVIC_SetPriority(USART2_IRQn,3,3);	//��ռ���ȼ�3�������ȼ�3

    UART2_Handler.Instance          = USART2;
    UART2_Handler.Init.BaudRate     = bound;
    UART2_Handler.Init.WordLength   = UART_WORDLENGTH_8B;
    UART2_Handler.Init.StopBits     = UART_STOPBITS_1;
    UART2_Handler.Init.Parity       = UART_PARITY_NONE;
    UART2_Handler.Init.Mode         = UART_MODE_TX_RX;
    HAL_UART_Init(&UART2_Handler);

    /*�ú����Ὺ�������ж�,��־λUART_IT_RXNE��λ*/
    /*�������ý��ջ���ͽ��ջ�����������*/
    HAL_UART_Receive_IT(&UART2_Handler, aRxBuffer, RXBUFFERSIZE);
    reset_uart_pipe2();
}

//����2�жϷ������
void USART2_IRQHandler(void)
{
    u8 Res;
    if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
		Res=USART2->RDR; 	//��������
		uart2_printf((char*)&Res);
		Pipe_Block_Input(&uart2_pipe,(const char*)&Res,1);
		Pipe_Block_Input(&uart_pipe2_log,(const char*)&Res,1);
    }
}

void uart2_output(unsigned char * buf,int len)
{
    unsigned int __i,timespan = Get_SysmilliTick();

    if(buf==NULL ||len==0)  return;
    for(__i = 0; __i < len; __i++)
    {
        //  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) {};
        while((USART2->ISR & USART_FLAG_TC) == (uint16_t)RESET)//��ȡ������ɱ�־TC transmission complete
        {//��ʱ����������� 
            if(Get_SysmilliTick()-timespan>10)
            {
                break;
            }
        }		
        timespan = Get_SysmilliTick();
        USART2->TDR = ((uint8_t)buf[__i] & (uint16_t)0x01FF);// USART_SendData(USART2,(uint8_t) buf[__i]);
    }
}

void uart2_printf(char * string, ...)
{
    __va_list arg;
    int len=0;
    if(string==NULL)  return;

    if(strlen(string)>=UART_TX_LEN)return;
    va_start(arg, string);
    memset(_txBuffer,0,UART_TX_LEN);
    len=vsprintf((char *)_txBuffer, string, arg); /*must use "vsprintf" */
    va_end(arg);
    uart2_output((unsigned char *)_txBuffer,len);
}
#endif

#if UART3_ENABLE
#if USART3_USE_PIPE
Pipe_t uart3_pipe = {0};
#else
Pipe1_t uart3_pipe = {0};
#endif
void reset_uart_pipe3()
{
    #if USART3_USE_PIPE == 1
    Pipe_Reset(&uart3_pipe);
	#else
	Pipe1_Reset(&uart3_pipe);
	#endif
}

static void UART3_init(u32 bound)
{
    __HAL_RCC_UART3_GPIO_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    //PB10 Tx
    GPIO_Init.Pin 		= UART3_TX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(UART3_GPIO, &GPIO_Init);

    //PB11 Rx
    GPIO_Init.Pin 		= UART3_RX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(UART3_GPIO, &GPIO_Init);

    UART3_Handler.Instance          = USART3;
    UART3_Handler.Init.BaudRate     = bound;
    UART3_Handler.Init.WordLength   = UART_WORDLENGTH_8B;
    UART3_Handler.Init.StopBits     = UART_STOPBITS_1;
    UART3_Handler.Init.Parity       = UART_PARITY_NONE;
    UART3_Handler.Init.Mode         = UART_MODE_TX_RX;
    HAL_UART_Init(&UART3_Handler);

	HAL_NVIC_EnableIRQ(USART3_IRQn);		//ʹ��USART2�ж�ͨ��
    HAL_NVIC_SetPriority(USART3_IRQn,3,3);	//��ռ���ȼ�3�������ȼ�3
    /*�ú����Ὺ�������ж�,��־λUART_IT_RXNE��λ*/
    /*�������ý��ջ���ͽ��ջ�����������*/
    HAL_UART_Receive_IT(&UART3_Handler, aRxBuffer, RXBUFFERSIZE);
    reset_uart_pipe3();
}

//����3�жϷ������
void USART3_IRQHandler(void)
{
    u8 Res;
    if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
		Res=USART3->RDR; 	//��������
		uart3_printf((char*)&Res);
//		Pipe_Block_Input(&uart3_pipe,(const char*)&Res,1);
//        uart3_pipe.pipe_buf[uart3_pipe.in_pos]=Res;
//        uart3_pipe.in_pos++;
//        uart3_pipe.in_pos%=PIPE_BUF_LEN;
//        if(PIPE_BUF_LEN > uart3_pipe.sum)	
//			uart3_pipe.sum++;
//        else	
//			uart3_pipe.out_pos=uart3_pipe.in_pos;
//        uart3_pipe.time=Get_SysmilliTick();
    }
}


void uart3_output(unsigned char * buf,int len)
{
    unsigned int __i,timespan = Get_SysmilliTick();

    if(buf==NULL ||len==0)  return;
    for(__i = 0; __i < len; __i++)
    {
        //  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {};
        while((USART3->ISR & USART_FLAG_TC) == (uint16_t)RESET)//��ȡ������ɱ�־TC transmission complete
        {//��ʱ����������� 
            if(Get_SysmilliTick()-timespan>10)
            {
                break;
            }
        }		
        timespan = Get_SysmilliTick();
        USART3->TDR = ((uint8_t)buf[__i] & (uint16_t)0x01FF);// USART_SendData(USART3,(uint8_t) buf[__i]);
    }
}

void uart3_printf(char * string, ...)
{
    __va_list arg;
    int len=0;
    if(string==NULL)  return;

    if(strlen(string)>=UART_TX_LEN)return;
    va_start(arg, string);
    memset(_txBuffer,0,UART_TX_LEN);
    len=vsprintf((char *)_txBuffer, string, arg); /*must use "vsprintf" */
    va_end(arg);
    uart3_output((unsigned char *)_txBuffer,len);
}

#endif

#if UART4_ENABLE
#if USART4_USE_PIPE
Pipe_t uart4_pipe = {0};
#else
Pipe1_t uart4_pipe = {0};
#endif

void reset_uart_pipe4()
{
	#if USART4_USE_PIPE
    Pipe_Reset(&uart4_pipe);
	#else
	Pipe1_Reset(&uart4_pipe);
	#endif
}
static void UART4_init(u32 bound)
{
    __HAL_RCC_UART4_GPIO_CLK_ENABLE();
    __HAL_RCC_UART4_CLK_ENABLE();

    //PC10 Tx
    GPIO_Init.Pin 		= UART4_TX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(UART4_GPIO, &GPIO_Init);

    //PC11 Rx
    GPIO_Init.Pin 		= UART4_RX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(UART4_GPIO, &GPIO_Init);

    UART4_Handler.Instance          = UART4;
    UART4_Handler.Init.BaudRate     = bound;
    UART4_Handler.Init.WordLength   = UART_WORDLENGTH_8B;
    UART4_Handler.Init.StopBits     = UART_STOPBITS_1;
    UART4_Handler.Init.Parity       = UART_PARITY_NONE;
    UART4_Handler.Init.Mode         = UART_MODE_TX_RX;
    HAL_UART_Init(&UART4_Handler);

	HAL_NVIC_EnableIRQ(UART4_IRQn);		//ʹ��USART2�ж�ͨ��
    HAL_NVIC_SetPriority(UART4_IRQn,3,3);	//��ռ���ȼ�3�������ȼ�3
    /*�ú����Ὺ�������ж�,��־λUART_IT_RXNE��λ*/
    /*�������ý��ջ���ͽ��ջ�����������*/
	HAL_UART_Receive_IT(&UART4_Handler, aRxBuffer, RXBUFFERSIZE);
    reset_uart_pipe4();
}

//����4�жϷ������
void UART4_IRQHandler(void)
{
    u8 Res;
    if((__HAL_UART_GET_FLAG(&UART4_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
		Res=UART4->RDR; 	//��������
		uart4_printf((char*)&Res);
		Pipe_Block_Input(&uart4_pipe,(const char*)&Res,1);
    }	
}

void uart4_output(unsigned char * buf,int len)
{
    unsigned int __i,timespan = Get_SysmilliTick();

    if(buf==NULL ||len==0)  return;
    for(__i = 0; __i < len; __i++)
    {
        while((UART4->ISR & USART_FLAG_TC) == (uint16_t)RESET)//��ȡ������ɱ�־TC transmission complete
        {//��ʱ����������� 
            if(Get_SysmilliTick()-timespan>10)
            {
                break;
            }
        }		
        timespan = Get_SysmilliTick();
        UART4->TDR = ((uint8_t)buf[__i] & (uint16_t)0x01FF);
    }
}

void uart4_printf(char *string, ...)
{
    __va_list arg;
    int len=0;
    if(string==NULL)  return;

    if(strlen(string)>=UART_TX_LEN)return;
    va_start(arg, string);
    memset(_txBuffer,0,UART_TX_LEN);
    len=vsprintf((char *)_txBuffer, string, arg); /*must use "vsprintf" */
    va_end(arg);
    uart4_output((unsigned char *)_txBuffer,len);
}

#endif

void usart_initconfig(void)
{
#if UART1_ENABLE
    UART1_init(115200);
#endif

#if UART2_ENABLE
    UART2_init(115200);
#endif

#if UART3_ENABLE
    UART3_init(115200);
#endif

#if UART4_ENABLE
    UART4_init(115200);
#endif
}


