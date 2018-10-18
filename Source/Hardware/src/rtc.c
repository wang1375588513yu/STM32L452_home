/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                        		By(MR.Richard)
**
**----------------------------------File Information----------------------------
** FileName:      rtc.c
** The Author:    WangYu
** Creation Date: 2018-10-18
** Description:   rtc���Ӻͻ����¼�
**
**--------------------------------Version Information---------------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
#include "rtc.h"
#include "led.h"

#ifdef RTC_ENABLE

RTC_HandleTypeDef RTC_Handler;
void rtc_init(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	__HAL_RCC_PWR_CLK_ENABLE();//ʹ�ܵ�Դʱ��PWR
	HAL_PWR_EnableBkUpAccess();//ȡ����������д����

	RTC_Handler.Instance = RTC;
	RTC_Handler.Init.AsynchPrediv = 0X7F;
	RTC_Handler.Init.HourFormat = RTC_HOURFORMAT_24;		//24Сʱ��
	RTC_Handler.Init.OutPut = RTC_OUTPUT_DISABLE;
	RTC_Handler.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RTC_Handler.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	RTC_Handler.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	RTC_Handler.Init.SynchPrediv = 0x7FFF;
	RTC_Handler.Lock = HAL_UNLOCKED;
	RTC_Handler.State = HAL_RTC_STATE_RESET;

	HAL_RTC_Init(&RTC_Handler); 

	if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0)!=0X5050)//�Ƿ��һ������
	{
		RTC_Set_Time(23,59,56,RTC_HOURFORMAT12_PM);	        //����ʱ�� ,����ʵ��ʱ���޸�
		RTC_Set_Date(15,RTC_MONTH_DECEMBER,27,RTC_WEEKDAY_SUNDAY);//��������
        HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5050);//����Ѿ���ʼ������
	}

    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;//LSE����
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  //RTCʹ��LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;//����ΪRTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;//RTCʱ��ԴΪLSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	__HAL_RCC_RTC_ENABLE();

}
/*******************************************************************************
** FunctionName:  RTC_Set_Time
** Description:   RTCʱ������
** Parameters:    hour:  ʱ
**			 	  min:   ��
**			 	  sec:   ��
**			 	  ampm:  12Сʱ�ƻ���24Сʱ��
** Returned:      SUCEE(1),�ɹ�      ERROR(0),�����ʼ��ģʽʧ��
** The Author:    WangYu
** Creation Date: 2018-10-18
**------------------------------------------------------------------------------
** Changed By:
** Changed Date:
** Changed explain:
**------------------------------------------------------------------------------
********************************************************************************/
HAL_StatusTypeDef RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_TimeStructure.Hours=hour;
	RTC_TimeStructure.Minutes=min;
	RTC_TimeStructure.Seconds=sec;
	RTC_TimeStructure.TimeFormat=ampm;
	RTC_TimeStructure.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation=RTC_STOREOPERATION_RESET;
	return HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);	
}
/*******************************************************************************
** FunctionName:  RTC_Set_Date
** Description:   RTC��������
** Parameters:    year: ��(0-99)
**			 	  month:��(1-12)  
**			 	  date: ��(0-31)
**			 	  week: ���ڼ�(1-7) 
** Returned:      SUCEE(1),�ɹ�      ERROR(0),�����ʼ��ģʽʧ�� 
** The Author:    WangYu
** Creation Date: 2018-10-18
**------------------------------------------------------------------------------
** Changed By:
** Changed Date:
** Changed explain:
**------------------------------------------------------------------------------
********************************************************************************/
HAL_StatusTypeDef RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	RTC_DateTypeDef RTC_DateStructure;
    
	RTC_DateStructure.Date=date;
	RTC_DateStructure.Month=month;
	RTC_DateStructure.WeekDay=week;
	RTC_DateStructure.Year=year;
	return HAL_RTC_SetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
}
/*******************************************************************************
** FunctionName:  RTC_Set_AlarmA
** Description:   ��������ʱ��(����������,24Сʱ��)
** Parameters:    week: ���� 
**			 	  hour: ʱ 
**			 	  min:  ��
**			 	  sec:  ��
** Returned:      None
** The Author:    WangYu
** Creation Date: 2018-10-18
**------------------------------------------------------------------------------
** Changed By:
** Changed Date:
** Changed explain:
**------------------------------------------------------------------------------
********************************************************************************/
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{ 
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //Сʱ
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //����
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //��
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;//24Сʱ��
    
    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;//��ȷƥ�����ڣ�ʱ����  RTC_ALARMMASK_DATEWEEKDAY
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;//������
    RTC_AlarmSturuct.AlarmDateWeekDay=week; //����
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     //����A
    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x02); //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTC_Handler);
}

/*******************************************************************************
** FunctionName:  HAL_RTC_AlarmAEventCallback
** Description:   RTC����A�жϴ���ص��������������ж��¼�������Ҫִ�е�����
**				  ��д�ڸú�������
** Parameters:    hrtc: ��� 
** Returned:      None
** The Author:    WangYu
** Creation Date: 2018-10-18
**------------------------------------------------------------------------------
** Changed By:
** Changed Date:
** Changed explain:
**------------------------------------------------------------------------------
********************************************************************************/
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{//
//    printf("ALARM A!\r\n");
}

/*******************************************************************************
** FunctionName:  RTC_Set_WakeUp
** Description:   �����Ի��Ѷ�ʱ������  
** Parameters:    wksel:  RTCEx_Wakeup_Timer_Definitions(RTC_WAKEUPCLOCK_CK_SPRE_16BITS)
**			 	  cnt: �Զ���װ��ֵ.����0,�����ж�. 
** Returned:      None
** The Author:    WangYu
** Creation Date: 2018-10-18
**------------------------------------------------------------------------------
** Changed By:
** Changed Date:
** Changed explain:
**------------------------------------------------------------------------------
********************************************************************************/
void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{ 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);//���RTC WAKE UP�ı�־
	HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,cnt,wksel);            //������װ��ֵ��ʱ�� 
	
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02); //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler); 
}
/*******************************************************************************
** FunctionName:  HAL_RTCEx_WakeUpTimerEventCallback
** Description:   RTC WAKE UP�жϴ���
** Parameters:    hrtc: ��� 
** Returned:      None
** The Author:    WangYu
** Creation Date: 2018-10-18
**------------------------------------------------------------------------------
** Changed By:
** Changed Date:
** Changed explain:
**------------------------------------------------------------------------------
********************************************************************************/
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
//    LED1=!LED1;
}

#endif
