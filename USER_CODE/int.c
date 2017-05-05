/****************************************************************************
* �ļ�����int.c
* ���ܣ��й��жϷ�����ӳ����
****************************************************************************/


#include "config.h"
#include "i2c_com.h"
#include "target.h"
#include "timer.h"
#include "port.h"
#include "key.h"
#include "fan_cal.h"

//#define _DEBUG
#define 	REJECT_COUNT_NUM		10	//����״̬�ȶ�ǰ������Ȧ��

int16 time_led_flash;
uint16 fan1_phase_sign,fan2_phase_sign,fan3_phase_sign,fan4_phase_sign;			//������λ���б�
int16 fan1_counter,fan2_counter,fan3_counter,fan4_counter;						//����Ȧ������
int16 fan1_counter_old,fan2_counter_old,fan3_counter_old,fan4_counter_old; //��int.c�ж���
uint8 frash_100ms_ok,frash_200ms_ok,frash_500ms_ok;
uint8 time_500us,time_2ms,time_10ms,time_100ms,time_500ms,time_1s;

uint8 flag_start_work;											//��ʼ������־
uint8 flag_start_count;											//��ʼ������־
uint32 count_insp_exp_cycle,count_insp_exp_cycle_read,count_insp_exp_cycle_valid;			//����ѭ��������

void Valve_Active_Insp1_Exp2(void);
uint8 Judge_Leap_Year(uint16 yy);

uint8 month,day,hour,minute,second,weekday;
uint16 year;
void Init_RTC(void);
void Read_Data_Time(void);
extern uint8 flag_adjust_date_time;

void Read_Data_Time(void);
//******************************************************
//
//
//
//******************************************************
void Read_Data_Time(void)
{
	uint32 times;
	
//	if(flag_adjust_date_time == TRUE)
//		return;
		
	times = CTIME0;
	weekday = (times >>24) & 0x07;
	hour = (times>>16) & 0x1F;
	minute = (times>>8) & 0x3F;
	second = times & 0x3F;
	
	times = CTIME1;
	year = (times>>16) & 0xFFF;
	month = (times>>8) & 0xF;
	day = times & 0x1F;
}
/****************************************************************************
* ���ƣ�Judge_Leap_Year()
* ���ܣ��ж�����
* ��ڲ�����yy ��ֵ
* ���ڲ�����1 = ���ꣻ0 = ������
****************************************************************************/
uint8 Judge_Leap_Year(uint16 yy)
{
	if(yy%4 != 0)				//���ܱ�4����
	{
		return (0);	
	}
	else						//�ܱ�4����
	{
		if(yy%100 != 0)			//���ܱ�100����
			return (1);
		else if(yy%400 == 0)	//�ܱ�100����,�ܱ�400����
			return (1);
		else					//�ܱ�100����,���ܱ�400����
			return (0);
	}
}
/****************************************************************************
* ���ƣ�Valve_Action_in_Insp()
* ���ܣ�����ʱ�ķ��Ŷ���
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Valve_Action_in_Insp(void)
{
	IO0CLR = EXP_VALVE;		//�غ�����
	IO0CLR = INSP_VALVE;	//��������
}

/****************************************************************************
* ���ƣ�Valve_Action_in_Exp()
* ���ܣ�����ʱ�ķ��Ŷ���
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Valve_Action_in_Exp(void)
{
	IO0SET = INSP_VALVE;	//�ؽ�����
	IO0SET = EXP_VALVE;		//��������
}
/****************************************************************************
* ���ƣ�Valve_Action_in_Hold()
* ���ܣ�����ʱ�ķ��Ŷ���
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Valve_Action_in_Hold(void)
{
	IO0SET = INSP_VALVE;	//�ؽ�����
	IO0CLR = EXP_VALVE;		//�غ�����
}
/****************************************************************************
* ���ƣ�Valve_Active_Insp1_Exp2()
* ���ܣ�������1:6��ŷ�����
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Valve_Active_Insp1_Exp2(void)
{
	if(flag_start_work == TRUE)
	{
		if(time_500ms < 2)
		{
			Valve_Action_in_Insp();			//1S
		}
		else if(time_500ms < 4)
		{
			Valve_Action_in_Hold();			//1S
			fan1_counter = 0;
			fan2_counter = 0;
			fan3_counter = 0;
			fan4_counter = 0;
		}
		
		else if(time_500ms < 12)
		{
			Valve_Action_in_Exp();			//4S
		}
		else
		{
		//ȥ��ÿ�ο�ʼ������ǰ5����������
	
			time_500ms = 0;
			Valve_Action_in_Insp();			//1S
			count_insp_exp_cycle++;
			if((count_insp_exp_cycle >= REJECT_COUNT_NUM) && flag_start_count == FALSE)
			{
				flag_start_count = TRUE;
				count_insp_exp_cycle = count_insp_exp_cycle_valid;
			}
	
			if((count_insp_exp_cycle != 0) && (flag_start_count == TRUE))
			{	
				Stat_Fan_Circles();
				count_insp_exp_cycle_valid = count_insp_exp_cycle;
			}
			else
			{
			//�����������־	
				fan1_counter_old = fan1_counter;
				fan2_counter_old = fan2_counter;
				fan3_counter_old = fan3_counter;
				fan4_counter_old = fan4_counter;
				
				fan1_counter = 0;
				fan2_counter = 0;
				fan3_counter = 0;
				fan4_counter = 0;
			}
		}
	}
	else
	{
		time_500ms = 0;
		flag_start_count = FALSE;
		Valve_Action_in_Hold();
	}
}
/*************************************************************************
* ������: IRQ_UART0
* ����  : ����0�жϴ������
* ���  : ��
* ����  : ��
*************************************************************************/
void IRQ_UART0(void)
{
}
/****************************************************************************
* ���ƣ�IRQ_Timer0()
* ���ܣ���ʱ��0�жϷ������(500us)
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void IRQ_Timer0(void)
{
	T0IR = 0x01;	 	           	//���T0�жϱ�־
	
	//CPU �����Ƶ���
	time_led_flash++;
	if(time_led_flash==350)
	{
		IO0CLR = LED_CPU_S;
	}
	else if(time_led_flash==400)
	{
		time_led_flash = 0;
		IO0SET = LED_CPU_S;
	}
	
	if(++time_500us < 4)
		return;
	time_500us = 0;
	Scan_Key();			//2msɨ��һ�ΰ���

	if(++time_2ms < 5)
		return;
	time_2ms = 0;
	time_10ms++;
//	if(time_10ms == 5)
//		frash_100ms_ok = 1;			//50ms
	
	if(time_10ms < 10)
		return;
	time_10ms = 0;
	frash_100ms_ok = 1;				//100ms

	if(++time_100ms < 5)
		return;
	time_100ms = 0;
	time_500ms++;
	frash_500ms_ok = 1;
	
	Read_Data_Time();
}
/****************************************************************************
* IRQ_EINT0()
* ���ܣ��ⲿ�ж�0�������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void IRQ_EINT0(void)
{
	uint16 di;
	
	di = fan1_phase_sign << 8;
	if(IO0PIN & FLUX1_PHASE_B)
	{//B�źŸߵ�ƽ
		di |= 0x01;
	}
	if(EXTPOLAR & 0x01)
	{//�����ش���
		EXTPOLAR &= 0xFE;		//�����½��ش���
		di |= 0x10;
	}
	else{//�½��ش���
		EXTPOLAR |= 0x01;		//���������ش���
	}
	EXTINT = 0x01;				//���EINT0�жϱ�־

	fan1_phase_sign = di;
	if(fan1_phase_sign == 0x0110)
	{//AB:�¸�->�ϵ�
		fan1_counter++;			//Ȧ����1		
	}
	else if(fan1_phase_sign == 0x0011)
	{//AB:�µ�->�ϸ�
		fan1_counter--;			//Ȧ����1
	}
}
/****************************************************************************
* IRQ_EINT1()
* ���ܣ��ⲿ�ж�1�������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void IRQ_EINT1(void)
{
	uint16 di;
	
	di = fan3_phase_sign << 8;
	if(IO0PIN & FLUX3_PHASE_A)
	{//B�źŸߵ�ƽ
		di |= 0x01;
	}
	if(EXTPOLAR & 0x02)
	{//�����ش���
		EXTPOLAR &= 0xFD;		//�����½��ش���
		di |= 0x10;
	}
	else{//�½��ش���
		EXTPOLAR |= 0x02;		//���������ش���
	}
	EXTINT = 0x02;				//���EINT1�жϱ�־

	fan3_phase_sign = di;
	if(fan3_phase_sign == 0x0110)
	{//AB:�¸�->�ϵ�
		fan3_counter--;			//Ȧ����1		
	}
	else if(fan3_phase_sign == 0x0011)
	{//AB:�µ�->�ϸ�
		fan3_counter++;			//Ȧ����1
	}
}

/****************************************************************************
* IRQ_EINT2()
* ���ܣ��ⲿ�ж�2�������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void IRQ_EINT2(void)
{
	uint16 di;
	
	di = fan2_phase_sign << 8;
	if(IO0PIN & FLUX2_PHASE_B)
	{//B�źŸߵ�ƽ
		di |= 0x01;
	}
	if(EXTPOLAR & 0x04)
	{//�����ش���
		EXTPOLAR &= 0xFB;		//�����½��ش���
		di |= 0x10;
	}
	else{//�½��ش���
		EXTPOLAR |= 0x04;		//���������ش���
	}
	EXTINT = 0x04;				//���EINT2�жϱ�־

	fan2_phase_sign = di;
	if(fan2_phase_sign == 0x0110)
	{//AB:�¸�->�ϵ�
		fan2_counter++;			//Ȧ����1		
	}
	else if(fan2_phase_sign == 0x0011)
	{//AB:�µ�->�ϸ�
		fan2_counter--;			//Ȧ����1
	}
}

/****************************************************************************
* IRQ_EINT3()
* ���ܣ��ⲿ�ж�3�������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void IRQ_EINT3(void)
{
	uint16 di;
	
	di = fan4_phase_sign << 8;
	if(IO0PIN & FLUX4_PHASE_B)
	{//B�źŸߵ�ƽ
		di |= 0x01;
	}
	if(EXTPOLAR & 0x08)
	{//�����ش���
		EXTPOLAR &= 0xF7;		//�����½��ش���
		di |= 0x10;
	}
	else{//�½��ش���
		EXTPOLAR |= 0x08;		//���������ش���
	}
	EXTINT = 0x08;				//���EINT3�жϱ�־

	fan4_phase_sign = di;
	if(fan4_phase_sign == 0x0110)
	{//AB:�¸�->�ϵ�
		fan4_counter++;			//Ȧ����1		
	}
	else if(fan4_phase_sign == 0x0011)
	{//AB:�µ�->�ϸ�
		fan4_counter--;			//Ȧ����1
	}
}

/****************************************************************************
* ���ƣ�FIQ_Exception()
* ���ܣ������жϴ������,���ڼ�����ַ���
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void FIQ_Exception(void)
{
}