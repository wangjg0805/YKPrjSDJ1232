//**********************************************************************
//�ļ���:timer.c
//˵��:
//1.���Ź�ι��ʱҪ��ֹ�ж�,��ֹ�жϴ��ι������
//�����б�:
//1.��ʼ��ʱ��0:Init_Timer0
//2.��ʱ��0���жϴ���:IRQ_Timer0
//3.��ȡ��ǰʱ��(���Ӽ�ʱ��):Get_Cur_Time
//4.����ʱ���:Cal_Time_Dif
//5 ��ʱ����ms Delay_Nms
//6.ι������:WDT_Feed
//7.��ʼ�����Ź�:Init_WDT
//**********************************************************************
#include "config.h"
#include "global.h"
#include "ad_acquire.h"
#include "math.h"
#include "stdio.h"
#include "math.h"
#include "type_def.h"
#include "port.h" 
#include "stdlib.h"
#include "timer.h"
 
#include "ads1100.h" 

extern void IRQ_Timer0_Handler(void);    //�жϺ������

volatile uint32 one_minute_timer;    //���Ӷ�ʱ��(�ֱ���:1ms)
volatile uint32 timer_5ms;
volatile uint8  flag_5ms_ok;
volatile uint32 timer_10ms;
volatile uint8  flag_10ms_ok;
volatile uint32 timer_100ms;
volatile uint8  flag_100ms_ok;
volatile uint32 timer_300ms;
volatile uint8  flag_300ms_ok;
volatile uint8  flag_1ms_ok;
volatile uint8  flag_1ms_ok_for_delay;
volatile uint32 timer_500ms;
volatile uint8  flag_500ms_ok;
volatile uint32 timer_1s;
volatile uint8  flag_1s_ok;
volatile uint32 warning_alm_timer;   //��ʾ�౨����ʾ��ʱ��
volatile uint32 tips_alm_timer;       //��ʾ�౨����ʾ��ʱ��


//**********************************************************************
//����  ��:Init_Timer0
//��������:��ʼ����ʱ��0,���ж�ʱ
//��ڲ���:us--��ʱʱ��(���1000000)
//���ڲ���:��
//**********************************************************************
void Init_Timer0(uint32 us)
{   /* Fcclk = Fosc*4 = 11.0592MHz*4 = 44.2368MHz
       Fpclk = Fcclk/4 = 44.2368MHz/4 = 11.0592MHz
    */
    uint8 i;

    
   /* ���ö�ʱ��0�ж�IRQ */
    #if (LCDTYPE == TFTLCD)
    VICIntSelect &= ~(1 << VIC_TIMER0);               // ��ʱ��0�ж���ΪIRQ
    VICVectPri4  =  0x04;                             //irq priority is 8 (0-15)
    VICVectAddr4 =  (uint32)IRQ_Timer0_Handler;       // �����жϷ�������ַ���� 
    VICIntEnable |= (1 << VIC_TIMER0);;                    // ʹ�ܶ�ʱ��0�ж�
    #else
    VICIntSelect &= ~0x00000010;                      // ��ʱ��0�ж���ΪIRQ
    VICVectCntl1 = 0x24;                              // ��ʱ��0�ж�ͨ������IRQ slot 1
    VICVectAddr1 = (uint32)IRQ_Timer0_Handler;        // �����жϷ�������ַ���� 
    VICIntEnable = 0x00000010;                        // ʹ�ܶ�ʱ��0�ж�
    #endif 
    if (us > 1000000) return;
    
    T0TCR = 0x02;                                     // ��λT0TC
    T0TCR = 0;   
    T0PC  = 0;                                        // ���ö�ʱ����Ԥ��Ƶ
    T0MR0 = ((Fpclk / 6250) * us) / 160;              // �Ƚ�ֵ(��ʱֵ)
    T0MCR = 0x03;                                     // ƥ��ͨ��0ƥ���жϲ���λT0TC
    T0CCR               =   0;                        // Capture is disabled.     
    T0EMR               =   0;                        // No external match output.
    T0TCR               =   1;                        // Enable timer 0 
}
//**********************************************************************
//����  ��:IRQ_Timer0
//��������:��ʱ��0���жϴ���(500us)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************

//**********************************************************************
//����  ��:IRQ_Timer0
//��������:��ʱ��0���жϴ���(500us)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
#define CD4052_A_H          1
#define CD4052_A_L          0
      

////////////////////�� 125US ��Ϊʱ���׼
const uint32 period_CONST[] = {0, 1,  3,  7,  19 , 19}; //4k 2k 1k 500hz 200hz


void IRQ_Timer0(void)
{
    static uint32 count_125us = 0;
    static uint32 CD4052_A_Phase = CD4052_A_H;
    static uint32 time_cnt = 0;
    static uint32 sample_phase = 0;
     
    T0IR = 0xff;                        //���T0�жϱ�־    
    
    count_125us++;
    
    #if DDLV_SOFT   
    ////////////////////////////////////////////////////////
    if(CD4052_A_H == CD4052_A_Phase)
      {
       //sample_phase++;
       //if(sample_phase > 2)
       sample_flag = 1;  //�������
     
       CD4052_A_SET;
       time_cnt++;
       if(time_cnt > period_CONST[power_fre-1])
          {
           time_cnt = 0;    
           CD4052_A_Phase = CD4052_A_L;
          }
      }
     else
       {
        sample_flag = 0;
        sample_phase = 0;
        CD4052_A_CLR;
        time_cnt++;
        if(time_cnt >= period_CONST[power_fre-1])
           {
            time_cnt = 0;
            CD4052_A_Phase = CD4052_A_H;
           }
       }
    #endif
    
    if (count_125us < 8) 
        return;
    
    //1ms��
    count_125us = 0;
    flag_1ms_ok = TRUE;
    flag_1ms_ok_for_delay = TRUE;
    ///////////////////////////////////
        
    one_minute_timer++;        //��ʱ1����
    if (one_minute_timer > 60000)
    {
        one_minute_timer = 0;
    }
    
    if (timer_5ms++ >= 5)//5ms��
    {
        timer_5ms = 0;
        flag_5ms_ok = TRUE;
        
     
    } 
     
    timer_10ms++;
    if (timer_10ms < 10) return;
    timer_10ms = 0;        //10ms��
    flag_10ms_ok = TRUE;
    
    ////////////////////////10ms ��������
    Key_Sound_Pro();
    Alarm_Sound_Pro();
      
    timer_100ms++;
    
       
    if (timer_100ms < 10) return;
    timer_100ms = 0;    //100ms��
    flag_100ms_ok = TRUE;
    
   
      
    warning_alm_timer++;
    tips_alm_timer++;
    Set_Clr_Alarm_Sound(); //��鱨����Ϣ
    //////////////////////////
   
    timer_300ms++;
    if (timer_300ms == 3)
    {
     timer_300ms = 0;
     flag_300ms_ok = TRUE;  //200ms��
   
    }
    
    timer_500ms++;
    if (timer_500ms<5) return;
    timer_500ms = 0;    //500ms��
    flag_500ms_ok = TRUE;
    
    timer_1s++;
    if (timer_1s<2) return;
    timer_1s = 0;    //1s��
    flag_1s_ok = TRUE; 
          
    no_key_time++;

}

//**********************************************************************
//����  ��:Get_Cur_Time
//��������:��ȡ��ǰʱ��(���Ӷ�ʱ����ʱ��)
//��ڲ���:��
//���ڲ���:ʱ��(ms)
//**********************************************************************
sint32 Get_Cur_Time(void)
{
    return (one_minute_timer);
}

//**********************************************************************
//����  ��:Cal_Time_Dif
//��������:����ʱ���(��λ:ms),��one_minute_timerΪʱ��
//��ڲ���:start_time--��ʱ����ʼʱ���(ms)
//���ڲ���:ʱ���(ms)
//**********************************************************************
sint32 Cal_Time_Dif(sint32 start_time)
{
    if (one_minute_timer >= start_time)
    {
        return (one_minute_timer-start_time);
    }
    else
    {//����1����
        return(60000-start_time+one_minute_timer);
    }
}

//**********************************************************************
//����  ��:Delay_Nms
//��������:��ʱN ms
//��ڲ���:time--��ʱʱ��(��λ:ms)
//���ڲ���:��
//**********************************************************************
void Delay_Nms(uint32 time)
{
    while (time)
    {
        if (flag_1ms_ok_for_delay)
        {
            flag_1ms_ok_for_delay = FALSE;
            time--;
        }
    }
}

/****************************************************************************
* ���ƣ�WDT_Feed()
* ���ܣ����Ź�ι������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void WDT_Feed(void)
{
uint32 int_bak;
    int_bak = VICIntEnable;        //���ж�ʹ��״̬
    VICIntEnClr = int_bak;        //����
    WDFEED = 0xAA;                //ι��
    WDFEED = 0x55;
    VICIntEnable = int_bak;        //����
}

/****************************************************************************
* ���ƣ�Init_WDT()
* ���ܣ����Ź���ʼ��
* ��ڲ��������ι�����(��λ:ms, ���10000)
* ���ڲ�������
****************************************************************************/
void Init_WDT(uint16 wdt_time)
{
    WDTC = (Fpclk / 4000) * wdt_time;    // ����WDT��ʱ��
    WDCLKSEL = 0x01;                     //ѡ��APB CLK
    WDMOD = 0x03;                        // ���ò�����WDT
}
/////////////////////////////////////////////////////
