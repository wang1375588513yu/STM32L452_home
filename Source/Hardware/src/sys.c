/********************************Copyright (c)******************************\
**
**                    (c) Copyright 2018, WangYu, China, SZ.
**                            All Rights Reserved
**
**                               By(MR.Richard)
**
**----------------------------------File Information------------------------
** FileName:      sys.c
** The Author:    WangYu
** Creation Date: 2018-08-27
** Description:   
**
**--------------------------------Version Information-----------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**--------------------------------------------------------------------------
\********************************End of Head********************************/

#include "sys.h"
#include "led.h"

void NVIC_Configuration(void)
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
}
