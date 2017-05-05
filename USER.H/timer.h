//**********************************************************************
//�ļ���:timer.h
//**********************************************************************
#ifndef _TIMER_H
#define _TIMER_H

#include "type_def.h"

extern volatile uint32 one_minute_timer;    //���Ӷ�ʱ��(�ֱ���:1ms)
extern volatile uint8  flag_5ms_ok;
extern volatile uint8  flag_10ms_ok;
extern volatile uint8  flag_100ms_ok;
extern volatile uint8  flag_300ms_ok;
extern volatile uint8  flag_1ms_ok;
extern volatile uint8  flag_500ms_ok;
extern volatile uint8  flag_1s_ok;
extern volatile uint32 tips_alm_timer;
extern volatile uint32 warning_alm_timer;

void Init_Timer0(uint32 us);
void IRQ_Timer0(void);
sint32 Get_Cur_Time(void);
void WDT_Feed(void);
void Init_WDT(uint16 wdt_time);
void Delay_Nms(uint32 time);

#endif
