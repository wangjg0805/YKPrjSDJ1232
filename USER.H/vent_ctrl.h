//**********************************************************************
//�ļ���:vent_ctrl.h
//**********************************************************************
extern uint8 trigger_judge_phase;

#ifndef _VENT_CTRL_H
#define _VENT_CTRL_H
#include "type_def.h"
#define FLAG_INLET_VAL_ERR_BITS        0        //������״̬�����־λ
#define FLAG_OUTLET_VAL_ERR_BITS    1        //������״̬�����־λ
#define FLAG_MPL501_OPEN_BITS        2        //ѹ����ȫ����״̬��־λ
#define FLAG_NO_VENT_30S_BITS        3        //���������ڼ�,����30sû�м�⵽�к���
#define FLAG_P_OVER_BITS            4        //ѹ�����ޱ�־λ
#define FLAG_P_NEG_BITS                5        //��ѹ��־
#define FLAG_P_SUS_HIGHER_BITS        6        //�ܵ�ѹ���������߱�־λ
#define FLAG_P_SUS_HIGH_BITS        7        //�ܵ�ѹ�������ϸ߱�־λ
#define FLAG_P_NO_RAISE_BITS        8        //�ܵ�ѹ��������
#define FLAG_BR_CEASING_BITS        9        //��������ֹ(���������ڼ�,ѹ����ȫ������)
#define FLAG_APNEA_30S_BITS            10        //�ֶ�״̬��30sû�к����ı�־λ
#define FLAG_APNEA_120S_BITS        11        //�ֶ�״̬��120sû�к����ı�־λ
#define FLAG_INSP_PEEP_ERR_BITS        12        //����PEEP��ѹ��״̬�����־λ
#define FLAG_EXP_PEEP_ERR_BITS        13        //����PEEP���ڷ�״̬�����־λ

void Vent_Control_Detct(void);
void Demarc_Vent_Control_Pro(void);
sint32 Get_Br_Status(void);
void Force_Stop_Vent(void);
void Calc_Plimit_Para(void);
sint32 Get_P_Max_Statistic(void);
sint32 Get_P_Min_Statistic(void);
sint32 Get_Pl_Use(void);
sint32 Get_P_Raise(void);
sint32 Get_Br_Timer(void);
sint32 Get_Exp_End_Time(void);
sint32 Get_Ppl_End_Time(void);
sint32 Get_Insp_End_Time(void);
uint8 Get_Manual_Vent_Phase(void);
void Set_Force_Exp_Flag(void);
void Clr_Force_Exp_Flag(void);
uint8 Check_Force_Exp_Flag(void);
#endif