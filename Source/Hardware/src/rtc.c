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
** Description:   rtc闹钟和唤醒事件
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

	__HAL_RCC_PWR_CLK_ENABLE();//使能电源时钟PWR
	HAL_PWR_EnableBkUpAccess();//取消备份区域写保护

	RTC_Handler.Instance = RTC;
	RTC_Handler.Init.AsynchPrediv = 0X7F;
	RTC_Handler.Init.HourFormat = RTC_HOURFORMAT_24;		//24小时制
	RTC_Handler.Init.OutPut = RTC_OUTPUT_DISABLE;
	RTC_Handler.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RTC_Handler.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	RTC_Handler.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	RTC_Handler.Init.SynchPrediv = 0x7FFF;
	RTC_Handler.Lock = HAL_UNLOCKED;
	RTC_Handler.State = HAL_RTC_STATE_RESET;

	HAL_RTC_Init(&RTC_Handler); 

	if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0)!=0X5050)//是否第一次配置
	{
		RTC_Set_Time(23,59,56,RTC_HOURFORMAT12_PM);	        //设置时间 ,根据实际时间修改
		RTC_Set_Date(15,RTC_MONTH_DECEMBER,27,RTC_WEEKDAY_SUNDAY);//设置日期
        HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5050);//标记已经初始化过了
	}

    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;//LSE配置
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  //RTC使用LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;//外设为RTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;//RTC时钟源为LSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	__HAL_RCC_RTC_ENABLE();

}
/*******************************************************************************
** FunctionName:  RTC_Set_Time
** Description:   RTC时间设置
** Parameters:    hour:  时
**			 	  min:   分
**			 	  sec:   秒
**			 	  ampm:  12小时制或者24小时制
** Returned:      SUCEE(1),成功      ERROR(0),进入初始化模式失败
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
** Description:   RTC日期设置
** Parameters:    year: 年(0-99)
**			 	  month:月(1-12)  
**			 	  date: 日(0-31)
**			 	  week: 星期几(1-7) 
** Returned:      SUCEE(1),成功      ERROR(0),进入初始化模式失败 
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
** Description:   设置闹钟时间(按星期闹铃,24小时制)
** Parameters:    week: 星期 
**			 	  hour: 时 
**			 	  min:  分
**			 	  sec:  秒
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
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //小时
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //分钟
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //秒
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;//24小时制
    
    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;//精确匹配星期，时分秒  RTC_ALARMMASK_DATEWEEKDAY
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;//按星期
    RTC_AlarmSturuct.AlarmDateWeekDay=week; //星期
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     //闹钟A
    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x02); //抢占优先级1,子优先级2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTC_Handler);
}

/*******************************************************************************
** FunctionName:  HAL_RTC_AlarmAEventCallback
** Description:   RTC闹钟A中断处理回调函数，当闹钟中断事件发生需要执行的任务
**				  就写在该函数里面
** Parameters:    hrtc: 句柄 
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
** Description:   周期性唤醒定时器设置  
** Parameters:    wksel:  RTCEx_Wakeup_Timer_Definitions(RTC_WAKEUPCLOCK_CK_SPRE_16BITS)
**			 	  cnt: 自动重装载值.减到0,产生中断. 
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
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);//清除RTC WAKE UP的标志
	HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,cnt,wksel);            //设置重装载值和时钟 
	
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02); //抢占优先级1,子优先级2
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler); 
}
/*******************************************************************************
** FunctionName:  HAL_RTCEx_WakeUpTimerEventCallback
** Description:   RTC WAKE UP中断处理
** Parameters:    hrtc: 句柄 
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
