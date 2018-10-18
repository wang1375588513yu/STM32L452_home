/********************************Copyright (c)******************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                       		By(MR.Richard)
**
**----------------------------------File Information------------------------
** FileName:      rtc.h
** The Author:    WangYu
** Creation Date: 2018-10-18
** Description:   
**
**--------------------------------Version Information-----------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**--------------------------------------------------------------------------
\********************************End of Head********************************/

#ifndef ___RTC_H___
#define ___RTC_H___
#include "sys.h"
#include "user_config.h"

#ifdef RTC_ENABLE

void rtc_init(void);
HAL_StatusTypeDef RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);
HAL_StatusTypeDef RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);



#endif
#endif
