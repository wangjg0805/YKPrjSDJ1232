//**********************************************************************
//�ļ���:vent_ctrl.c
//˵    ��:
//1.���������Ŀ��ƺͼ��,�ֶ������ļ��
//2.��Ⲣ��¼���������е��쳣״̬,�Թ�����ģ���ѯʹ��
//�����б�:
//1.��ʼ�����������Ŀ��Ʋ���:Init_Vent_Para
//2.��ʼ��ѹ��ͳ��ֵ:Init_Paw_Para
//3.��λ��������:Reset_Br_Para
//4.�����������ֵ:Cal_Br_Meas_Val
//5.����������ֵ����Чֵ:Invalid_Br_Meas_Val
//6.����ѹ����صĲ���ֵ:Cal_Pressure_Value
//7.���������ͺ�����:Check_Val_Status
//8.��������:Vent_Control_Detct
//9.��ѯ����״̬:Get_Br_Status
//10.ǿ��ֹͣ��������:Force_Stop_Vent
//11.�궨������ʱ�ĺ�������:Demarc_Vent_Control_Pro
//12.����ܵ�ѹ�����Ʋ���:Calc_Plimit_Para
//13.��ȡ���ѹ����ͳ��ֵ:Get_P_Max_Statistic
//14.��ȡ��Сѹ����ͳ��ֵ:Get_P_Min_Statistic
//15.��ȡѹ������ֵ:Get_Pl_Use
//16.��ȡѹ������ֵ:Get_P_Raise
//17.��ȡ������ʱ����ʱ��:Get_Br_Timer
//18.��ȡ��������������ʱ��:Get_Exp_End_Time
//19.��ȡ��������������ʱ��:Get_Ppl_End_Time
//20.��ȡ��������������ʱ��:Get_Insp_End_Time
//21.��ȡ��ǰ�ֹ�ͨ����״̬:Get_Manual_Vent_Phase
//**********************************************************************
#include "global.h"
#include "port.h"
#include "vent_ctrl.h"
#include "ad_acquire.h"
#include "data_pro.h"
//#include "debug.h"
#include "type_def.h"
#include "speaker.h"
#include "sys_para.h"

#if (LCDTYPE == TFTLCD)
#include "lpc2400.h"
#else
#include "lpc2294.h"
#endif 

#define FLOW_DETECT_NO_EXP        (0xFFFF/4)    //�޺����������(1/4��δ�⵽Ҷ��ת)
#define INSP_END_COMP_TIME        50        //������ǰ��������ʱ��(��λ:1ms)
#define INSP_VAL_CLS_TIME         80        //��������ǰ�رյ�ʱ��(����С����ʱ��)(��λ:1ms)
static sint32 vent_ctrl_timer;            // ����������ʱ��,����ͳ��ÿ�κ�����ʱ��(��λ:1ms)
static sint32 time_rate_count;            //����Ƶ�ʼ�ʱ��
static sint32 vent_exp_end_time;        //���������ĺ�������ʱ��(��λ:1ms)(�����vent_ctrl_timer)
static sint32 vent_ppl_end_time;        //������������������ʱ��(��λ:1ms)(�����vent_ctrl_timer)
static sint32 vent_insp_end_time;        //������������������ʱ��(��λ:1ms)(�����vent_ctrl_timer)
static sint32 vent_trigger_end_time;    //SIMVģʽ�µĴ������ڵĽ���ʱ��(��λ:1ms)
static sint32 vent_no_trigger_end_time;    //SIPPVģʽ�µĺ�����Ӧ��ʱ��(��λ:1ms)
static sint32 vent_ppl_timer;            //����ʱ���ۼ�(��λ:1ms)
uint8 trigger_judge_phase = TRIGGER_PHASE_START;
uint16 fan_prd_min_for_trig;
static sint32 paw_start;                //������ʼʱ�̵�ѹ��
static sint32 exp_end_p;                //��������ʱ�̵�ѹ��ֵ,��λ0.1cmH2O
static sint32 paw_max_statistic;        //���ѹ��ͳ��ֵ
static sint32 paw_min_statistic;        //���ѹ��ͳ��ֵ
#define PAW_STATISTIC_MUL    10        //���ѹ������Сѹ��ͳ��ֵ�ĵ�λ
static sint32 paw_sum;                    //ѹ���ۼ�ֵ
#define PAW_SUM_MUL            100            //ѹ���ۼ�ֵ�ķŴ���
static sint32 paw_sum_count;            //ѹ���ۼӴ���(��������ѹ��ƽ��ֵ)
static uint8 flag_paw_over;                //ѹ�����ޱ�־
static uint8 flag_paw_negative;            //��ѹ��־
static sint32 paw_sus_higher_time;        //ѹ����������ʱ��(��λ:1ms) 
static sint32 paw_sus_high_time;        //ѹ�������ϸ�ʱ��(��λ:1ms)
#define TIME_PAW_SUS_HIGHER    10000        //��ѹ���������ж�ʱ��10��
#define TIME_PAW_SUS_HIGH    15000        //��ѹ�����ϸ��ж�ʱ��15��
static uint8 paw_no_raise_count;        //�ܵ�ѹ������������(ָ��������������ͳ�ƵĹܵ�ѹ������������)

#define TIME_TEST_VALVE        150            //����ƽ�ȶ�150ms�����䱨����־
static sint32 time_no_vent;                //���������ڼ�,û�м�⵽����������ʱ��
static uint8 flag_insp_val_err;                                //������״̬������
static uint8 flag_exp_val_err;                                //������״̬������
static uint8 flag_insp_peep_val_err;                        //����PEEP��ѹ��״̬������
static uint8 flag_exp_peep_val_err;                            //����PEEP����ѹ��״̬������
static uint8 flag_mpl501_status;                            //ѹ����ȫ����״̬
static uint8 flag_br_ceasing;                                //��������ֹ��ֹ(���ں����ڼ�ѹ����ȫ���򿪶�����ֹ)

static sint32 time_manual_detect_dis;        //��������������,��ֹ�ֶ��������ļ�ʱ��(����ʱ)
#define TIME_MANUAL_DETECT_DIS        5000    //��������ֹͣ��5s�ڽ�ֹ����ֶ�����
static uint8 flag_manual_insp;                //�����ֶ�����������״̬���
static uint8 flag_apnea_30s;                //�ֶ�����30sû�м�⵽����
static uint8 count_apnea_30s;                //�޺���30s����
static uint8 flag_apnea_120s;                //�ֶ�����120sû�м�⵽����

#define DEMA_HOLD_END_TIME    1000    //�궨ģʽ������������(1��)
#define DEMA_VENT_CYCLE        3000    //�궨ģʽ�º�������(3��)
#define DEMA_INSP_END_TIME    (1000-INSP_VAL_CLS_TIME)    //�궨ģʽ������������(0.9��)

static sint32 plimit_pressure;//ѹ������ֵ(��λ:0.1cmH2O)
#define PLMT_MUL    10
static uint8 flag_force_exp=FALSE;    //ǿ�ƺ����ı�־

//**********************************************************************
//����  ��:Init_Vent_Para
//��������:��ʼ�����������Ŀ��Ʋ���
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Init_Vent_Para(void)
{
    Clr_Force_Exp_Flag();
    vent_exp_end_time = rate_cycle_tab[br_rate_set];    //��������(������������ʱ���)(��λ:1ms)
    vent_ppl_end_time = vent_exp_end_time*ie_i_tab[ie_set]/256-INSP_END_COMP_TIME;    //����������ʱ���
    vent_insp_end_time = vent_ppl_end_time*(100-asph_set)*41/4096;
    if(vent_insp_end_time>vent_ppl_end_time-INSP_VAL_CLS_TIME)
    {//��������100ms
        vent_insp_end_time = vent_ppl_end_time - INSP_VAL_CLS_TIME;
    }
    vent_ctrl_timer = 0;    //�������������ʱ��
    time_rate_count = 0;
            
    paw_start = airway_p;        //��¼������ʼʱ�̵�ѹ��ֵ(���ڼ���ѹ������)    
}

//**********************************************************************
//����  ��:Init_Paw_Para
//��������:��ʼ��ѹ����ز���
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Init_Paw_Para(void)
{
    paw_max_statistic = 0xffffffff;    //ѹ�����ֵͳ��,��ֵ��Сֵ
    paw_min_statistic = 0xffff;    //ѹ����Сֵͳ��,��ֵ���
    paw_sum = 0;                    //����ѹ���ۼ�ֵ
    paw_sum_count = 0;                
    flag_paw_over = FALSE;            //���ѹ�����޺͸�ѹ��־
    flag_paw_negative = FALSE;
}


//**********************************************************************
//����  ��:Reset_Br_Para
//��������: ��λ��������
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Reset_Br_Para(void)
{
    Init_Paw_Para();    //��ʼ��ѹ�����ֵ

    vent_ctrl_timer = 0;    //�������������ʱ��
    time_rate_count = 0;
    vent_ppl_timer = 0;    
    time_no_vent = 0;        //����޻�������ʱ��    
    //Init_Vt_Ad();        //��ʼ������������ֵ
    //Init_Fan_Period_Statistic();//��ʼ���������ڵ�ͳ��ֵ
    paw_no_raise_count = 0; //����ܵ�ѹ������������
    flag_br_ceasing = FALSE;    //�����������ֹ��־
    flag_manual_insp = TRUE;    //�����ֶ�����������״̬
}

//**********************************************************************
//����  ��:Cal_Br_Meas_Val
//��������:�����������ֵ
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Cal_Br_Meas_Val(void)
{
    sint32 di;
    
    if (time_rate_count)
    {
        br_rate_meas = (60000+time_rate_count/2)/time_rate_count;    //����Ƶ�ʲ���ֵ(��������)(��/min)
    }
    br_rate_meas = br_rate_set; //����Ƶ��ֱ��ʹ������ֵ(2012/2/8)
    
    if (flag_eep_vte_error)//�������궨���ݴ���
    {
        vt_meas = VT_MEAS_NOT_VALID;
        insp_flux_meas = VE_MEAS_NOT_VALID;
    }
    else{
        //di = Calc_Filt_Fan_Lap();
        vt_meas = Cal_Vt(di,(uint8*)(&vt_meas_units));    //���㳱����
        insp_flux_meas = (vt_meas*br_rate_meas+50)/100;    //�������ͨ����(0.1L/min)
        insp_flux_meas_unit = 10;
    }
    
    if (flag_eep_vte_error||flag_eep_p_error)
    {
        humor_meas = HUMOR_NOT_VALID;
    }
    else{
        //vt_meas_unitsΪ0��?
        if (paw_max_statistic != paw_min_statistic)
        {
            humor_meas = vt_meas*PAW_STATISTIC_MUL/((paw_max_statistic-paw_min_statistic)*vt_meas_units);
        }
        else
        {
            humor_meas = HUMOR_NOT_VALID;
        }
        humor_meas_units = 1;//˳Ӧ�Ե�λ:1ml/cmH2O
    }
}

//*********************************************************************
//����  ��:Invalid_Br_Meas_Val
//��������:��ֵ������������һ����Чֵ
//��ڲ���:��
//���ڲ���:��
//*********************************************************************
static void Invalid_Br_Meas_Val(void)
{
    br_rate_meas = RATE_NOT_VALID;//����Ƶ��
    vt_meas = VT_MEAS_NOT_VALID;    //������
    insp_flux_meas = VE_MEAS_NOT_VALID;        //����ͨ����
    humor_meas = HUMOR_NOT_VALID;        //˳Ӧ��
}

//**********************************************************************
//����  ��:Cal_Pressure_Value
//��������:�ں����ڼ�,����ѹ����ص�����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Cal_Pressure_Value(void)
{
    //ѹ������ֵ���
    if (flag_eep_p_error)//ѹ���궨���ݴ���
    {
        paw_max = PAW_NOT_VALID;
        paw_avr_meas = PAW_NOT_VALID;
        peep_meas = PAW_NOT_VALID;
    }
    else{
        paw_max = paw_max_statistic*PAW_MAX_MUL/PAW_STATISTIC_MUL;    //һ�����������ڵ����ѹ������ֵ
        //paw_sum_countΪ0��?
        paw_avr_meas = paw_sum*PAW_AVR_MEAS_MUL/(paw_sum_count*PAW_SUM_MUL);    //ƽ��ѹ��
        peep_meas = airway_p*PEEP_MEAS_MUL/PAW_MUL;
    }
}

/****************************************************************************
* ���ƣ�Goto_SIPPV_Exp_Phase()
* ���ܣ�����SIPPV������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
static void Goto_SIPPV_Exp_Phase(void)
{
    vent_no_trigger_end_time = vent_ctrl_timer + TIME_MIN_EXP;    //���㲻Ӧ�ڽ���ʱ��
    if(vent_no_trigger_end_time > vent_exp_end_time)
    {//��Ӧ�ڽ���ʱ�䲻�ܳ���������������
        vent_no_trigger_end_time = vent_exp_end_time;
    }
    phase_vent_ctrl = PHASE_SIPPV_EXP;    //����SIPPV������
}

/****************************************************************************
* ���ƣ�Init_SIMV_Trigger_Para()
* ���ܣ���ʼ��SIMVģʽ�µĴ�������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
static void Init_SIMV_Trigger_Para(void)
{
    vent_ctrl_timer = 0;                //��������ʱ����0
    vent_exp_end_time = rate_cycle_tab[br_rate_set];
    vent_trigger_end_time = (vent_exp_end_time * trigger_window) / 100;
    exp_end_p = airway_p;        //����ĩ��ʼѹ��
}

/****************************************************************************
* ���ƣ�Init_SIMV_Control_Para()
* ���ܣ���ʼ��SIMVģʽ�µĿ��Ʋ���
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
static void Init_SIMV_Control_Para(void)
{
    Clr_Force_Exp_Flag();
    vent_ppl_end_time =    (vent_exp_end_time * ie_i_tab[ie_set]) / 256+vent_ctrl_timer - INSP_END_COMP_TIME;
    if(vent_ppl_end_time > vent_exp_end_time - TIME_MIN_EXP)
    {
        vent_ppl_end_time = vent_exp_end_time - TIME_MIN_EXP;
    }
    vent_insp_end_time = ((vent_ppl_end_time - vent_ctrl_timer) * (100 - asph_set))
                         / 100 + vent_ctrl_timer;
    if(vent_insp_end_time > vent_ppl_end_time - INSP_VAL_CLS_TIME)
    {//��������80ms
        vent_insp_end_time = vent_ppl_end_time - INSP_VAL_CLS_TIME;
    }
    paw_start = airway_p;    //������ʼѹ�� 
}

//**********************************************************************
//����  ��:Check_Val_Status
//��������:���������ͳ�������״̬�Ƿ�����
//��ڲ���:inlet_sta--����������ȷ״̬
//            oulet_sta--����������ȷ״̬(δʹ��)
//            insp_peep_sta--����PEEP��ѹ������ȷ״̬
//            exp_peep_sta--����PEEP���ڷ�����ȷ״̬
//���ڲ���:��
//**********************************************************************
static void Check_Val_Status( uint8 inlet_sta, uint8 outlet_sta,uint8 insp_peep_sta,uint8 exp_peep_sta)
{
    static sint32 time_inlet_val_ok;
    static sint32 time_inlet_val_err;
    static sint32 time_outlet_val_ok;
    static sint32 time_outlet_val_err;
    static sint32 time_insp_peep_val_ok;
    static sint32 time_insp_peep_val_err;
    static sint32 time_exp_peep_val_ok;
    static sint32 time_exp_peep_val_err;

    if (Get_Insp_Val_Status()==inlet_sta)    //������״̬����
    {
        time_inlet_val_err = 0;
        if (time_inlet_val_ok < TIME_TEST_VALVE)//�ۼ�����ʱ��
        {
            time_inlet_val_ok += VENT_CTRL_PER;
        }    
        if (time_inlet_val_ok >= TIME_TEST_VALVE)
        {//�ۼ���������һ��ʱ��
            flag_insp_val_err = FALSE;        //���������״̬����ı��
        }
    }
    else
    {
        time_inlet_val_ok = 0;
        if (time_inlet_val_err < TIME_TEST_VALVE)    //�ۼ��쳣ʱ��
        {
            time_inlet_val_err += VENT_CTRL_PER;
        }
        if (time_inlet_val_err >= TIME_TEST_VALVE)//�쳣����һ��ʱ��
        {
            flag_insp_val_err = TRUE;            //��ǽ�����״̬����
        }            
    }
    
    if (Get_Insp_Peep_Val_Status()==insp_peep_sta)    //����PEEP��ѹ��״̬����
    {
        time_insp_peep_val_err = 0;
        if (time_insp_peep_val_ok < TIME_TEST_VALVE)//�ۼ�����ʱ��
        {
            time_insp_peep_val_ok += VENT_CTRL_PER;
        }    
        if (time_insp_peep_val_ok >= TIME_TEST_VALVE)
        {//�ۼ���������һ��ʱ��
            flag_insp_peep_val_err = FALSE;        //�������PEEP��ѹ��״̬����ı��
        }
    }
    else
    {
        time_insp_peep_val_ok = 0;
        if (time_insp_peep_val_err < TIME_TEST_VALVE)    //�ۼ��쳣ʱ��
        {
            time_insp_peep_val_err += VENT_CTRL_PER;
        }
        if (time_insp_peep_val_err >= TIME_TEST_VALVE)//�쳣����һ��ʱ��
        {
            flag_insp_peep_val_err = TRUE;            //�������PEEP��ѹ��״̬����
        }            
    }    
    
    if (Get_Exp_Peep_Val_Status()==exp_peep_sta)    //����PEEP���ڷ�״̬����
    {
        time_exp_peep_val_err = 0;
        if (time_exp_peep_val_ok < TIME_TEST_VALVE)//�ۼ�����ʱ��
        {
            time_exp_peep_val_ok += VENT_CTRL_PER;
        }    
        if (time_exp_peep_val_ok >= TIME_TEST_VALVE)
        {//�ۼ���������һ��ʱ��
            flag_exp_peep_val_err = FALSE;        //�������PEEP���ڷ�״̬����ı��
        }
    }
    else
    {
        time_exp_peep_val_ok = 0;
        if (time_exp_peep_val_err < TIME_TEST_VALVE)    //�ۼ��쳣ʱ��
        {
            time_exp_peep_val_err += VENT_CTRL_PER;
        }
        if (time_exp_peep_val_err >= TIME_TEST_VALVE)//�쳣����һ��ʱ��
        {
            flag_exp_peep_val_err = TRUE;            //��Ǻ���PEEP���ڷ�״̬����
        }            
    }    
    
    //���ڲ���Ҫʹ�ú�����������ⲿ����Ч(Ԥ��)
    if (Get_Exp_Val_Status()==outlet_sta)    //������״̬����
    {
        time_outlet_val_err = 0;
        if (time_outlet_val_ok < TIME_TEST_VALVE)//�ۼ�����ʱ��
        {
            time_outlet_val_ok += VENT_CTRL_PER;
        }
        if (time_outlet_val_ok >= TIME_TEST_VALVE)//������������һ��ʱ��
        {
            flag_exp_val_err = FALSE;        //���������״̬������
        }
    }
    else
    {
        time_outlet_val_ok = 0;
        if (time_outlet_val_err < TIME_TEST_VALVE)//�ۼ��쳣ʱ��
        {
            time_outlet_val_err += VENT_CTRL_PER;
        }
        if (time_outlet_val_err >= TIME_TEST_VALVE)//�����쳣����һ��ʱ��
        {
            flag_exp_val_err = TRUE;    //��ǳ�����״̬����
        }        
    }
    flag_exp_val_err = FALSE;    //���ڲ���Ҫʹ�ú�����������ⲿ����Ч
}


/****************************************************************************
* ����: Init_No_Raised_Paw_Para()
* ����: ��ʼ��ѹ������������ز���
* ��ڲ���: ��
* ���ڲ���: ��
****************************************************************************/
static void Init_No_Raised_Paw_Para(void)
{
    paw_no_raise_count = 0;        //���ѹ��������������
    time_no_vent = 0;                //���޻�������ʱ��
}

/****************************************************************************
* ���ƣ�No_Raised_Paw_Detect_Pro()
* ���ܣ�ѹ����������⴦��
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
static void No_Raised_Paw_Detect_Pro(void)
{
    if((airway_p - paw_start)*PAW_NO_RAISE_THR_MUL <= paw_no_raise_threshold*PAW_MUL)
    {//��ѹ����δ������ֵ
        if(paw_no_raise_count < PAW_NO_RAISE_COUNT)
        {
            paw_no_raise_count++;//��ѹ����������������1
        }
    }
    else{//��ѹ����������
        Init_No_Raised_Paw_Para();        //��ʼ��ѹ���������б����
    }
}

//**********************************************************************
//����  ��:Check_Paw_Stable
//��������:���ܵ�ѹ���Ƿ��ȶ�
//��ڲ���:paw_buf--��¼�ܵ�ѹ���Ļ�����
//            num --��¼�ܵ�ѹ���ĸ���
//            paw_avr_ptr--����ѹ��ƽ��ֵ�ĵ�ַ
//���ڲ���:TRUE--ѹ���Ѿ��ȶ�
//            FALSE--ѹ����δ�ȶ�
//**********************************************************************
static uint8 Check_Paw_Stable(sint32* paw_buf,uint8 num,sint32 *paw_avr_ptr)
{
    /*uint8 i;
    sint32 paw;
    //��ѹ��ƽ��ֵ
    paw = 0;
    for (i=0; i<num; i++)
    {
        paw += paw_buf[i];
    }
    paw /= num;
    
    *paw_avr_ptr = paw;    //����ѹ��ƽ��ֵ
    //���ѹ���Ƿ��ȶ�
    for (i=0; i<num; i++)
    {
        if (abs(paw-paw_buf[i])*100 >PAW_MUL*8 )    //ѹ��ƫ�����0.08cmH2O
        {//ѹ��û���ȶ�
            return(FALSE);
        }
    }*/
    
    if ((paw_buf[3]-paw_buf[2]<2)&&
        (paw_buf[2]-paw_buf[1]<2)&&
        (paw_buf[1]-paw_buf[0]<2))    //�����仯��С��0.02cmH2O/50ms
    {
        *paw_avr_ptr =     (paw_buf[0]+paw_buf[1]+paw_buf[2]+paw_buf[3])/4;
        return(TRUE);    
    }    
    return (FALSE);
}

//**********************************************************************
//����  ��:Pressure_Trig_Det
//��������:ѹ���������
//��ڲ���:phase--��ǰ���ڼ����λ
//            fan_prd_buf--�������ڵ�������μ�¼
//            fan_min --���봥������⵽����С��������
//            paw_rcd_buf--������εĹܵ�ѹ����¼,�ɼ�����:120ms,��λ:0.1cmH2O
//            cur_paw--��ǰѹ��ֵ(��λ:0.1cmH2O)
//            trig_p--����ѹ��(��λ:0.1cmH2O)
//            pre_avr_p--ǰһ�κ�����ƽ��ѹ��(��λ:0.1cmH2O)
//���ڲ���:����������λ
//**********************************************************************
uint8 Pressure_Trig_Det(uint8 phase,uint16* fan_prd_buf,uint16 fan_min,sint32* paw_rcd_buf,sint32 cur_paw,uint8 trig_p,
                        sint32 pre_avr_p)
{
    static sint32 paw_trig_cmp; //�����Ƚϵ��ѹ��ֵ(��λ:0.1cmH2O)
    static sint32 trig_phase_time;    //���ڴ������ʣ��ʱ��(��λ1ms)
    static sint32 fan_acc_phase_time;//���ڷ��ּ����ж�������ʱ��(��λ1ms)
    switch(phase)
    {
        case TRIGGER_PHASE_START://��ʼ�������
            fan_acc_phase_time = 1000;    //���ڷ��ּ����ж������1000ms
            phase = TRIGGER_PHASE_FAN_ACC;
            break;
        case TRIGGER_PHASE_FAN_ACC://�����ж�
            fan_acc_phase_time -= VENT_CTRL_PER;
            if (((fan_prd_buf[FAN_PERIOD0_IND]+100<fan_prd_buf[FAN_PERIOD1_IND])&&
                 (fan_prd_buf[FAN_PERIOD1_IND]+100<fan_prd_buf[FAN_PERIOD2_IND]))||
                 (fan_acc_phase_time <= 0))
            {//�����Ѿ�����ת��������1000ms��δת���������ֹյ��ж���
                phase = TRIGGER_PHASE_FAN_INFLEXION;    
            }
            break;
        case TRIGGER_PHASE_FAN_INFLEXION://���ٵ����ٵĹյ��ж�
            if ((fan_min < fan_prd_buf[FAN_PERIOD0_IND])&&
               (fan_min < fan_prd_buf[FAN_PERIOD1_IND])&&
               (fan_min < fan_prd_buf[FAN_PERIOD2_IND]))
             {//�Ѿ���⵽����ٶȣ���������жϽ׶�
                 phase = TRIGGER_PHASE_FAN_DEC;    
             }  
            break;    
        case TRIGGER_PHASE_FAN_DEC://���ּ���
            if (((fan_min > 1500)//ת�ٺܵ͵�ʱ������������ж��Ƿ�������� 
                &&(fan_prd_buf[FAN_PERIOD0_IND] > fan_prd_buf[FAN_PERIOD1_IND]+50)
                &&(fan_prd_buf[FAN_PERIOD1_IND] > fan_prd_buf[FAN_PERIOD2_IND]+50))||
                 ((fan_min <= 1500
                 &&(fan_prd_buf[FAN_PERIOD0_IND]>2*fan_min)
                 &&(fan_prd_buf[FAN_PERIOD1_IND]>2*fan_min)
                 &&(fan_prd_buf[FAN_PERIOD2_IND]>2*fan_min)))||
                    (T1TC > FLOW_DETECT_NO_EXP)||
                     (fan_prd_buf[FAN_PERIOD0_IND] > FLOW_DETECT_NO_EXP))
            {//���ٵ�һ���̶�,����ѹ���жϽ׶�
                phase = TRIGGER_PHASE_PAW_STABLE;
            }                        
            break;
        case TRIGGER_PHASE_PAW_STABLE:    //�жϹܵ�ѹ���Ƿ��ȶ�
            if (((paw_rcd_buf[0]-paw_rcd_buf[1] <= 3*(paw_rcd_buf[1]-paw_rcd_buf[2]))&&
                (paw_rcd_buf[1]<paw_rcd_buf[2])&&(paw_rcd_buf[2]<paw_rcd_buf[3]))||
                ((paw_rcd_buf[0]-paw_rcd_buf[1] <= -4)&&
                (paw_rcd_buf[1]==paw_rcd_buf[2])&&(paw_rcd_buf[2]==paw_rcd_buf[3])))                        
            {//б��ͻȻ�仯�ϴ�,�����в�������
                if (cur_paw < pre_avr_p)
                {
                    paw_trig_cmp = paw_rcd_buf[1];
                    trig_phase_time = TRIGGER_TIME;        //���봥����TRIGGER_TIME��ms
                    phase = TRIGGER_PHASE_TRIG;
                }    
            }                    
            break;
        case TRIGGER_PHASE_TRIG://�жϲ����Ƿ񴥷�����
            if(paw_trig_cmp - cur_paw >= trig_p)
            {//�ѵ�����ѹ��
                phase = TRIGGER_PHASE_SUC;    //�����ɹ�
            }
            else
            {//û������ѹ��,�жϴ�����ʱ���Ƿ����    
                trig_phase_time -= VENT_CTRL_PER;    //���㴦�ڴ����ڵ�ʱ��
                if (trig_phase_time <= 0)
                {//�涨ʱ����ѹ���½�ֵû�дﵽ����ѹ��
                    phase = TRIGGER_PHASE_PAW_STABLE;//�ص�ѹ��б�ʱ仯�ж�
                }
            }            
            break;                    
    }
    return (phase);        
}

//**********************************************************************
//����  ��:Vent_Control_Detct
//��������:�����������Ƽ��������(�����ж���,ÿ��VENT_CTRL_PER ms����һ��)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Vent_Control_Detct(void)
{
    uint16* fan_per_ptr;
    static uint8 flag_no_manual_br = TRUE;    //���ֹ�ͨ����־
    static uint8 flag_in_manual = TRUE;        //�����ֹ�ͨ���ı�־
    #ifdef _DEBUG
    static  sint16 fan_c; //����Ȧ��
    #endif
    static uint8 flag_in_simv_first = TRUE;    //��һ�ν���SIMVģʽ�ı�־
    uint8 br_mode;
    static sint32 paw_trend_buf[16],paw_trend_prd = 0;
    
    vent_ctrl_timer += VENT_CTRL_PER;        //�����������Ƽ�ʱ
    time_rate_count += VENT_CTRL_PER;
    //ѹ�����
    if (airway_p*PAW_STATISTIC_MUL > paw_max_statistic*PAW_MUL)    //ͳ�����ѹ��
    {
        paw_max_statistic = airway_p*PAW_STATISTIC_MUL/PAW_MUL;
    }
    if (airway_p*PAW_STATISTIC_MUL < paw_min_statistic*PAW_MUL)//ͳ����Сѹ��
    {
        paw_min_statistic = airway_p*PAW_STATISTIC_MUL/PAW_MUL;
    }
    paw_sum += airway_p*PAW_SUM_MUL/PAW_MUL;        //�ۼ�ѹ��ֵ
    paw_sum_count++;            //ѹ���ۼӴ���ͳ��        
    if (airway_p*PLMT_MUL > plimit_pressure*PAW_MUL)    //ѹ�����޼��
    {
        flag_paw_over = TRUE;
    }
    if (airway_p < PAW_NEG_THRESHOLD*PAW_MUL)    //��ѹ���
    {
        flag_paw_negative = TRUE;
    }
    if (airway_p > PAW_HIGHER_THRESHOLD*PAW_MUL)//�ܵ�ѹ����������(ѹ������100cmH2O)ʱ��ͳ��
    {
        if (paw_sus_higher_time<TIME_PAW_SUS_HIGHER)
        {
            paw_sus_higher_time += VENT_CTRL_PER;
        }    
    }
    else{
        paw_sus_higher_time = 0;
    }
    if (phase_vent_ctrl == PHASE_NO_VENT)
    {
        if (airway_p*PAW_SUB_HI_THR_MUL > paw_sub_high_thr_manual*PAW_MUL)    //�ֶ�����ʱ�ܵ�ѹ�������ϸ��ж�
        {
            if (paw_sus_high_time<TIME_PAW_SUS_HIGH)
            {
                paw_sus_high_time += VENT_CTRL_PER;
            }
        }
        else{
            paw_sus_high_time = 0;
        }
    
    }
    else{
        if (airway_p*PAW_SUB_HI_THR_MUL > paw_sub_high_thr_vent*PAW_MUL)    //��������ʱ�ܵ�ѹ�������ϸ��ж�
        {
            if (paw_sus_high_time<TIME_PAW_SUS_HIGH)
            {
                paw_sus_high_time += VENT_CTRL_PER;
            }
        }
        else
        {
            paw_sus_high_time = 0;
        }
    }    
    flag_mpl501_status = Get_MPL501_Status(); //���ѹ����ȫ��
    
    if (++paw_trend_prd>60)
    {//120ms��¼һ��ѹ������
        paw_trend_prd = 0;
        paw_trend_buf[15] = paw_trend_buf[14];
        paw_trend_buf[14] = paw_trend_buf[13];
        paw_trend_buf[13] = paw_trend_buf[12];
        paw_trend_buf[12] = paw_trend_buf[11];
        paw_trend_buf[10] = paw_trend_buf[9];
        paw_trend_buf[9] = paw_trend_buf[8];
        paw_trend_buf[8] = paw_trend_buf[7];
        paw_trend_buf[7] = paw_trend_buf[6];
        paw_trend_buf[6] = paw_trend_buf[5];
        paw_trend_buf[5] = paw_trend_buf[4];
        paw_trend_buf[4] = paw_trend_buf[3];
        paw_trend_buf[3] = paw_trend_buf[2];
        paw_trend_buf[2] = paw_trend_buf[1];
        paw_trend_buf[1] = paw_trend_buf[0];
        paw_trend_buf[0] = airway_p*10/PAW_MUL;    
    }
                
    switch (phase_vent_ctrl)
    {
        case PHASE_NO_VENT:    //�޻�������
            flag_in_manual = TRUE;
            flag_in_simv_first = TRUE;
            Valve_Action_in_Manual();        //���ֺ���״̬(��PEEP)
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_CLOSE);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if (time_manual_detect_dis >0)        //�ֶ�����������ε���ʱ
            {
                time_manual_detect_dis -= VENT_CTRL_PER;
                if (time_manual_detect_dis <= 0)    //�ֶ���������ʱ�䵽
                {
                    flag_no_manual_br = TRUE;
                    Cal_Pressure_Value();    //����ѹ������
                    Invalid_Br_Meas_Val();    //ʹ��������ֵ��Ч
                   // Calc_Filt_Fan_Lap();    //���֮ǰ�ķ���Ȧ��ͳ��
                    Reset_Br_Para();    //��λ��������
                    flag_apnea_30s = 0;
                    count_apnea_30s = 0;
                    flag_apnea_120s = 0;
                }
                return;
            }
            //fan_per_ptr = Get_Fan_Prd_Ptr();    //��ȡ���3�εĽ��
            if (flag_manual_insp == TRUE)    //��������״̬
            {
                if ( (fan_per_ptr[FAN_PERIOD0_IND]<2*fan_per_ptr[FAN_PERIOD_MAX_IND])
                     &&(fan_per_ptr[FAN_PERIOD1_IND]<2*fan_per_ptr[FAN_PERIOD_MAX_IND])
                     &&(fan_per_ptr[FAN_PERIOD2_IND]<2*fan_per_ptr[FAN_PERIOD_MAX_IND]) 
                     &&(fan_per_ptr[FAN_PERIOD0_IND]<fan_per_ptr[FAN_PERIOD1_IND])
                     &&(fan_per_ptr[FAN_PERIOD1_IND]<fan_per_ptr[FAN_PERIOD2_IND]))//��⵽���������
                {
                    flag_manual_insp = FALSE;
                    //Init_Fan_Period_Statistic();    //��ʼ���������ڵļ��ֵ
                }
                else if (time_rate_count >= 30000)//30sû�к���
                {
                    Cal_Pressure_Value();    //����ѹ������
                    Cal_Br_Meas_Val();    //�����������ֵ
                    br_rate_meas = 0;    //30sû�к��������Ƶ��Ϊ0
                    Reset_Br_Para();    //��λ��������
                    flag_apnea_30s = TRUE;        //�����Ϣ30s
                    if (++count_apnea_30s>=4) flag_apnea_120s=TRUE;                    
                }
                else    //����״̬��ÿ3s����һ�κ�������ֵ
                {
                    switch(time_rate_count)
                    {
                        case 3000://�ѵ�3��δ��⵽����
                        case 6000://�ѵ�6��δ��⵽����
                        case 9000://�ѵ�9��δ��⵽����
                            if(!flag_no_manual_br) break;        //���ֹ�ͨ�����˳�
                        case 12000://�ѵ�12��δ��⵽����
                        case 15000://�ѵ�15��δ��⵽����
                        case 18000://�ѵ�18��δ��⵽����
                        case 21000://�ѵ�21��δ��⵽����
                        case 24000://�ѵ�24��δ��⵽����
                        case 27000://�ѵ�27��δ��⵽����
                            flag_no_manual_br = TRUE;            //���ֶ�ͨ��
                            Cal_Pressure_Value();    
                            if (br_rate_meas == 0)
                            {//��ǰ�޺���
                                Cal_Br_Meas_Val();                //����һ��ȫ�����
                                br_rate_meas = 0;
                            }
                            else{
                                Invalid_Br_Meas_Val();                //����һ��ȫ�����        
                            }                        
                            Init_Paw_Para();                //��ʼ��ѹ����صĲ���
                            break;
                    }    
                }     
            }
            else //���ں���״̬
            {
                if (((fan_per_ptr[FAN_PERIOD_MIN_IND]>600)//ת�ٺܵ͵�ʱ������������ж��Ƿ����������
                    &&(fan_per_ptr[FAN_PERIOD0_IND] > fan_per_ptr[FAN_PERIOD1_IND]+50)
                    &&(fan_per_ptr[FAN_PERIOD1_IND] > fan_per_ptr[FAN_PERIOD2_IND]+50))||
                     ((fan_per_ptr[FAN_PERIOD_MIN_IND]<=600
                     &&(fan_per_ptr[FAN_PERIOD0_IND]>3*fan_per_ptr[FAN_PERIOD_MIN_IND])
                     &&(fan_per_ptr[FAN_PERIOD1_IND]>3*fan_per_ptr[FAN_PERIOD_MIN_IND])
                     &&(fan_per_ptr[FAN_PERIOD2_IND]>3*fan_per_ptr[FAN_PERIOD_MIN_IND]))))    //��⵽����������
                {//һ���ֶ������������
                    flag_manual_insp = TRUE;
                    flag_no_manual_br = FALSE;    //��һ���ֹ�ͨ��
                    Manual_Success_Sound();        //���ֶ�ͨ���ɹ�������
                    flag_apnea_30s = 0;
                    count_apnea_30s = 0;
                    flag_apnea_120s = 0;                
                    Cal_Pressure_Value();    
                    Cal_Br_Meas_Val();                //����һ��ȫ�����                        
                    Init_Paw_Para();                //��ʼ��ѹ����صĲ���                                
                   // Init_Fan_Period_Statistic();    //��ʼ���������ڵļ��ֵ
                    time_rate_count = 0;
                }
                else if(T1TC >= 0xFFFF)//����û��ת��
                {
                    flag_manual_insp = TRUE; //��������״̬
                    //Init_Fan_Period_Statistic();
                }     
            }            
            break;
        case PHASE_START_VENT://��ʼ��������
            if (flag_in_manual)    //ԭ�������ֶ�ͨ��״̬
            {
                flag_in_manual = FALSE;
                Reset_Br_Para();        //��ʼ����غ���ֵ
            } 
            
            Init_Vent_Para();
            flag_apnea_30s = FALSE;    //�л�������������ֶ�����ʱ����Ϣ��־
            flag_apnea_120s = FALSE;
            br_mode = Get_Br_Mode_Sel(); 
            if (BRM_VCV == br_mode)    //VCVģʽ
            {
                flag_in_simv_first = TRUE;
                phase_vent_ctrl = PHASE_VCV_INSP;//����VCV������
            }
            else if (BRM_SIPPV == br_mode)//SIPPVģʽ
            {    
                flag_in_simv_first = TRUE;
                phase_vent_ctrl = PHASE_SIPPV_INSP;//����SIPPV������
            }
            else//SIMVģʽ
            {
                Init_SIMV_Trigger_Para();
                time_manual_detect_dis = TIME_MANUAL_DETECT_DIS;    //��ֹ����ֶ�������ʱ��
                phase_vent_ctrl = PHASE_NO_VENT;//�����޻���������    
                phase_vent_ctrl = PHASE_SIMV_TRIGGER;//����SIMV������
            }
            break;
//**********************************VCVģʽ�ĺ�������*********************************************            
        case PHASE_VCV_INSP://����
            Valve_Action_in_Insp();        //ִ���������Ŷ���
            Check_Val_Status(VAL_OPEN,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if (time_no_vent < 30000)    //�ۼ��޻�������ʱ��
            {    
                time_no_vent += VENT_CTRL_PER;            //
            }    
            if (flag_exp_val_err||flag_insp_val_err)    //����״̬����,ǿ��ֹͣ��������
            {
                phase_vent_ctrl = PHASE_STOP_VENT;
            }
            else if(flag_force_exp)
            {//ǿ�ƺ���
                phase_vent_ctrl = PHASE_VCV_HOLD;    //����VCV������
            }            
            else if (flag_mpl501_status == VAL_OPEN)    //ѹ����ȫ����
            {
                phase_vent_ctrl = PHASE_VCV_HOLD;        //ǿ�ƽ�������״̬
            }
            else if(vent_ctrl_timer >= vent_insp_end_time)
            {//����ʱ�䵽        
                phase_vent_ctrl = PHASE_VCV_HOLD;    //����������
                vent_ppl_timer = 0;        //��С����ʱ���ʱ������
            }
            else if(flag_paw_over)
            {//ѹ������
                phase_vent_ctrl = PHASE_VCV_HOLD;    //����������
                vent_ppl_timer = 0;
            }            
            break;
        case PHASE_VCV_HOLD://����
            Valve_Action_in_Hold();        //ִ���������Ŷ���
            if (time_no_vent < 30000)    //�ۼ��޻�������ʱ��
            {    
                time_no_vent += VENT_CTRL_PER;            //
            }        
            Check_Val_Status(VAL_CLOSE,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            vent_ppl_timer += VENT_CTRL_PER;     //�ۼ�����ʱ��
            if (flag_exp_val_err||flag_insp_val_err)    //����״̬����,ǿ��ֹͣ��������
            {
                phase_vent_ctrl = PHASE_STOP_VENT;
                break;
            }
            else if (flag_mpl501_status == VAL_OPEN)    //ѹ����ȫ����
            {
                //paw_no_raise_count = 0;                    //�ܵ�ѹ�����������������������
                //time_no_vent = 0;                         //�л�������,����޻�������ʱ��
                Init_No_Raised_Paw_Para();
                flag_br_ceasing = TRUE;                    //��������ֹ���
                phase_vent_ctrl = PHASE_VCV_EXP;        //ǿ�ƽ������״̬
                break;
            }
            /*if(vent_ppl_timer < INSP_VAL_CLS_TIME)
            {//δ����С����ʱ�����˳�
                break;
            }
            if(vent_ppl_timer == INSP_VAL_CLS_TIME)
            {//������С����ʱ�����б�ܵ�ѹ����������
                if(airway_p - paw_start <= paw_no_raise_threshold)
                {//��ѹ����δ������ֵ
                    if(paw_no_raise_count < PAW_NO_RAISE_COUNT)
                    {
                        paw_no_raise_count++;//��ѹ����������������1
                    }
                }
                else
                {//��ѹ����������
                    paw_no_raise_count = 0;    //���ѹ��������������
                    time_no_vent = 0;                         //�л�������,����޻�������ʱ��
                }
            }*/
            
            if(vent_ctrl_timer >= vent_ppl_end_time)
            {//����ʱ�䵽
                phase_vent_ctrl = PHASE_VCV_EXP;    //���������
                No_Raised_Paw_Detect_Pro();
                #ifdef _DEBUG
                fan_c = Get_Fan_Counter();
                #endif
                flag_br_ceasing = FALSE;                //���������������ֹ��־
            }
            else if(flag_paw_over)
            {//ѹ������
                phase_vent_ctrl = PHASE_VCV_EXP;    //���������
                No_Raised_Paw_Detect_Pro();
                flag_br_ceasing = FALSE;                //���������������ֹ��־
            }
            else if(flag_force_exp)
            {//ǿ�ƺ���
                flag_br_ceasing = FALSE;                //����������ֹ��־
                No_Raised_Paw_Detect_Pro();            //ѹ���������б���
                phase_vent_ctrl = PHASE_VCV_EXP;    //����VCV������
            }                    
            break;
        case PHASE_VCV_EXP://����
            Valve_Action_in_Exp();        //�������Ŷ���
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_OPEN);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if (time_no_vent < 30000)    //�ۼ��޻�������ʱ��
            {    
                time_no_vent += VENT_CTRL_PER;            //
            }                
            if (flag_exp_val_err||flag_insp_val_err)    //����״̬����,ǿ��ֹͣ��������
            {
                phase_vent_ctrl = PHASE_STOP_VENT;
            }            
            else if(vent_ctrl_timer >= vent_exp_end_time)
            {//����ʱ�䵽
                Cal_Pressure_Value();
                Cal_Br_Meas_Val();
                Init_Vent_Para();            //��ʼ��������������
                Init_Paw_Para();            //��ʼ��ѹ����صĲ���
                phase_vent_ctrl = PHASE_START_VENT;    //����������
            }            
            break;
//**********************************VCVģʽ���ƽ���*************************************************            
            
//**********************************SIPPVģʽ�ĺ�������*********************************************    
        case PHASE_SIPPV_INSP://����SIPPV����
            Valve_Action_in_Insp();            //���ֽ���״̬
            Check_Val_Status(VAL_OPEN,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);//��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
            if (flag_exp_val_err||flag_insp_val_err)    //����״̬����,ǿ��ֹͣ��������
            {
                phase_vent_ctrl = PHASE_STOP_VENT;
            }            
            if(flag_mpl501_status == VAL_OPEN)    //ѹ����ȫ����
            {//ѹ����ȫ����
                phase_vent_ctrl = PHASE_SIPPV_HOLD;    //����SIPPV������
            }
            else if(flag_paw_over)
            {//ѹ������
                phase_vent_ctrl = PHASE_SIPPV_HOLD;    //����SIPPV������
            }
            else if(flag_force_exp)
            {//ǿ�ƺ���
                phase_vent_ctrl = PHASE_SIPPV_HOLD;    //����SIPPV������
            }            
            else if(vent_ctrl_timer >= vent_insp_end_time)
            {//����ʱ�䵽
                phase_vent_ctrl = PHASE_SIPPV_HOLD;    //����SIPPV������
            }
            break;
        case PHASE_SIPPV_HOLD://����SIPPV����
            Valve_Action_in_Hold();            //��������״̬
            Check_Val_Status(VAL_CLOSE,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
            if(flag_mpl501_status == VAL_OPEN)    //ѹ����ȫ����
            {//ѹ����ȫ����
                //paw_no_raise_count = 0;                    //�ܵ�ѹ�����������������������
                //time_no_vent = 0;                         //�л�������,����޻�������ʱ��
                Init_No_Raised_Paw_Para();
                flag_br_ceasing = TRUE;                    //��������ֹ���
                Goto_SIPPV_Exp_Phase();                //����SIPPV������
            }
            else if(flag_paw_over)
            {//ѹ������
                No_Raised_Paw_Detect_Pro();
                flag_br_ceasing = FALSE;            //����������ֹ��־
                Goto_SIPPV_Exp_Phase();                //����SIPPV������
            }
            else if(flag_force_exp)
            {//ǿ�ƺ���
                flag_br_ceasing = FALSE;                //����������ֹ��־
                No_Raised_Paw_Detect_Pro();            //ѹ���������б���
                Goto_SIPPV_Exp_Phase();                //����SIPPV������
            }            
            else if(vent_ctrl_timer >= vent_ppl_end_time)
            {//����ʱ�䵽
                No_Raised_Paw_Detect_Pro();
                flag_br_ceasing = FALSE;            //����������ֹ��־
                Goto_SIPPV_Exp_Phase();                //����SIPPV������
            }
            break;
        case PHASE_SIPPV_EXP://����SIPPV������
            Valve_Action_in_Exp();            //���ֺ���״̬
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_OPEN);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if(vent_ctrl_timer >= vent_no_trigger_end_time)
            {//������Ӧ��ʱ�䵽
                exp_end_p = airway_p;    //����ĩ��ʼѹ��
                phase_vent_ctrl = PHASE_SIPPV_TRIGGER;    //����SIPPV������
                trigger_judge_phase = TRIGGER_PHASE_START;    //��������� 
                fan_prd_min_for_trig = UINT16_MAX;        //�������ֵ���Ա�ͳ�� 
            }
            break;
        case PHASE_SIPPV_TRIGGER://����SIPPV������
            Valve_Action_in_Exp();            //���ֺ���״̬
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_OPEN);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
           // trigger_judge_phase = Pressure_Trig_Det(trigger_judge_phase,Get_Fan_Prd_Ptr(),fan_prd_min_for_trig,
           //                                         paw_trend_buf,airway_p*10/PAW_MUL,Get_P_Trig_Usr_Set(),paw_avr_meas);
            if (TRIGGER_PHASE_SUC == trigger_judge_phase)
            {//�����ɹ�                                        
                vent_exp_end_time = vent_ctrl_timer;    //��������    
            }
            if(vent_ctrl_timer >= vent_exp_end_time)
            {//����ʱ�䵽
                Cal_Pressure_Value();
                Cal_Br_Meas_Val();
                Init_Paw_Para();            //��ʼ��ѹ����صĲ���
               // Init_Fan_Period_Statistic();//��ʼ���������ڵ�ͳ��
                phase_vent_ctrl = PHASE_START_VENT;    //��������״̬
                
                flag_rcd_prd = FALSE;
            }
            break;
//********************************SIPPV���ƽ���***********************************************

//********************************SIMVģʽ****************************************************
        case PHASE_SIMV_TRIGGER://����SIMV������
            Valve_Action_in_Exp();            //���ֺ���״̬
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_OPEN);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
           // trigger_judge_phase = Pressure_Trig_Det(trigger_judge_phase,Get_Fan_Prd_Ptr(),fan_prd_min_for_trig,
           //                                         paw_trend_buf,airway_p*10/PAW_MUL,Get_P_Trig_Usr_Set(),paw_avr_meas);
            if (TRIGGER_PHASE_SUC == trigger_judge_phase)
            {//�����ɹ�                                        
                vent_trigger_end_time = vent_ctrl_timer;    //��������    
            }

            if(vent_ctrl_timer >= vent_trigger_end_time)
            {//��������
                if(flag_in_simv_first)//��Ϊ֮ǰ�Ĳ�����������SIMVģʽ�Ĳ��������Ժ��Ե�һ�εļ���
                {//��һ�μ������ֵ
                    flag_in_simv_first = FALSE;
                }
                else
                {//�����������ֵ
                    Cal_Pressure_Value();
                    Cal_Br_Meas_Val();
                }
                Init_Paw_Para();            //��ʼ��ѹ����صĲ���
                //Init_Fan_Period_Statistic();//��ʼ���������ڵ�ͳ��
                time_rate_count = 0;        //�������Ƶ�ʼ�ʱ��
                if(BRM_SIMV != Get_Br_Mode_Sel())
                {//����ģʽ����
                    phase_vent_ctrl = PHASE_START_VENT;    //��������״̬
                }
                else{
                    Init_SIMV_Control_Para();            //��ʼ��SIMV���Ʋ���
                    phase_vent_ctrl = PHASE_SIMV_INSP;    //����SIMV������
                }
            }
            break;
        case PHASE_SIMV_INSP://����SIMV����
            Valve_Action_in_Insp();            //���ֽ���״̬
            Check_Val_Status(VAL_OPEN,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
            if(flag_mpl501_status == VAL_OPEN)    //ѹ����ȫ����
            {//ѹ����ȫ����
                phase_vent_ctrl = PHASE_SIMV_HOLD;    //����SIMV������
            }
            else if(flag_paw_over)
            {//ѹ������
                phase_vent_ctrl = PHASE_SIMV_HOLD;    //����SIMV������
            }
            else if(flag_force_exp)
            {//ǿ�ƺ���
                phase_vent_ctrl = PHASE_SIMV_HOLD;    //����SIMV������
            }            
            else if(vent_ctrl_timer >= vent_insp_end_time)
            {//����ʱ�䵽
                phase_vent_ctrl = PHASE_SIMV_HOLD;    //����SIMV������
            }
            break;
        case PHASE_SIMV_HOLD://����SIMV����
            Valve_Action_in_Hold();            //��������״̬
            Check_Val_Status(VAL_CLOSE,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
            if(flag_mpl501_status == VAL_OPEN)    //ѹ����ȫ����
            {//ѹ����ȫ����
                //paw_no_raise_count = 0;                    //�ܵ�ѹ�����������������������
                //time_no_vent = 0;                         //�л�������,����޻�������ʱ��
                Init_No_Raised_Paw_Para();
                flag_br_ceasing = TRUE;                    //��������ֹ���
                phase_vent_ctrl = PHASE_SIMV_EXP;    //����SIMV������
            }
            else if(flag_paw_over)
            {//ѹ������
                No_Raised_Paw_Detect_Pro();
                flag_br_ceasing = FALSE;                //����������ֹ��־
                phase_vent_ctrl = PHASE_SIMV_EXP;    //����SIMV������
            }
            else if(flag_force_exp)
            {//ǿ�ƺ���
                flag_br_ceasing = FALSE;                //����������ֹ��־
                No_Raised_Paw_Detect_Pro();            //ѹ���������б���
                phase_vent_ctrl = PHASE_SIMV_EXP;    //����SIMV������
            }                
            else if(vent_ctrl_timer >= vent_ppl_end_time)
            {//����ʱ�䵽
                No_Raised_Paw_Detect_Pro();
                flag_br_ceasing = FALSE;                //����������ֹ��־
                phase_vent_ctrl = PHASE_SIMV_EXP;    //����SIMV������
            }
            fan_prd_min_for_trig = UINT16_MAX;        //�������ֵ��ͳ�ƺ��������ڵķ�����С����
            break;
        case PHASE_SIMV_EXP://����SIMV����
            Valve_Action_in_Exp();            //���ֺ���״̬
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_OPEN);    //��鷧��״̬(������:��;������:��;����peep��:��;����PEEP��:��)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
            if(vent_ctrl_timer >= vent_exp_end_time)
            {//����ʱ�䵽
                Init_SIMV_Trigger_Para();    //��ʼ��SIMV��������  
                phase_vent_ctrl = PHASE_SIMV_TRIGGER;    //����SIMV������
               // fan_per_ptr = Get_Fan_Prd_Ptr();    //��ȡ���3�εĽ��
                if ((fan_prd_min_for_trig < fan_per_ptr[FAN_PERIOD0_IND])&&
                    (fan_prd_min_for_trig < fan_per_ptr[FAN_PERIOD1_IND])&&
                    (fan_prd_min_for_trig < fan_per_ptr[FAN_PERIOD2_IND]))
                {//�����Ѿ��ﵽ����ٶ�,��ֱ�ӽ��������
                    trigger_judge_phase = TRIGGER_PHASE_FAN_DEC;    
                }
                else
                {    
                    trigger_judge_phase = TRIGGER_PHASE_START;    //���������  
                }
            }
            break;
//*******************************SIMVģʽ���ƽ���**********************************************                                
        case PHASE_STOP_VENT://ֹͣ��������
            paw_no_raise_count = 0; //����ܵ�ѹ������������
            flag_br_ceasing = FALSE;    //������������ڼ�,������������ֹ��־
            time_no_vent = 0;         //��������ֹͣ,����޻���������ʱ��
            time_manual_detect_dis = TIME_MANUAL_DETECT_DIS;    //��ֹ����ֶ�������ʱ��
            phase_vent_ctrl = PHASE_NO_VENT;//�����޻���������            
            break;
        default :break;                         
    }
}

//**********************************************************************
//����  ��:Get_Br_Status
//��������:��ѯ����״̬
//��ڲ���:��
//���ڲ���:ĳһλΪ1��ʾ�ж�Ӧ���쳣�¼�����
//**********************************************************************
sint32 Get_Br_Status(void)
{
    sint32 br_status = 0;
    if (TRUE == flag_insp_val_err) br_status |= (1<<FLAG_INLET_VAL_ERR_BITS);    //������״̬����
    if (TRUE == flag_exp_val_err)    br_status |= (1<<FLAG_OUTLET_VAL_ERR_BITS);    //������״̬����
    if (VAL_OPEN == flag_mpl501_status) br_status |= (1<<FLAG_MPL501_OPEN_BITS);//ѹ����ȫ��MPL501��
    if (time_no_vent >= 30000) br_status |= (1<<FLAG_NO_VENT_30S_BITS);            //�ڻ��������ڼ�,����30sû�м�⵽�л�������
    if (TRUE == flag_paw_over) br_status |= (1<<FLAG_P_OVER_BITS);                //��������ѹ������
    if (TRUE == flag_paw_negative) br_status |= (1<<FLAG_P_NEG_BITS);            //��ѹ��־
    if (paw_sus_higher_time >= TIME_PAW_SUS_HIGHER)    br_status |= (1<<FLAG_P_SUS_HIGHER_BITS);    //�ܵ�ѹ����������
    if (paw_sus_high_time >= TIME_PAW_SUS_HIGH)    br_status |= (1<<FLAG_P_SUS_HIGH_BITS);            //�ܵ�ѹ�������ϸ�
    if (paw_no_raise_count >= 2)    br_status |= (1<<FLAG_P_NO_RAISE_BITS);        //�ܵ�ѹ��������
    if (TRUE == flag_br_ceasing)    br_status |= (1<<FLAG_BR_CEASING_BITS);        //��������ֹ(���������ڼ�ѹ����ȫ����)
    if (TRUE == flag_apnea_30s)        br_status |= (1<<FLAG_APNEA_30S_BITS);        //�ֶ�״̬��30sû�к���
    if (TRUE == flag_apnea_120s)    br_status |= (1<<FLAG_APNEA_120S_BITS);        //�ֶ�״̬��120sû�к���
    if (TRUE == flag_insp_peep_val_err)        br_status |= (1<<FLAG_INSP_PEEP_ERR_BITS);    //����PEEP��ѹ��״̬����
    if (TRUE == flag_exp_peep_val_err)        br_status |= (1<<FLAG_EXP_PEEP_ERR_BITS);    //����PEEP��ѹ��״̬����            
    return(br_status);
}

//**********************************************************************
//����  ��:Force_Stop_Vent
//��������:ǿ��ֹͣ��������
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Force_Stop_Vent(void)
{
    if (phase_vent_ctrl != PHASE_NO_VENT)
    {
        phase_vent_ctrl = PHASE_STOP_VENT;
    }
}

/****************************************************************************
* ���ƣ�Demarc_Vent_Control_Pro()
* ���ܣ��ڱ궨ģʽ�»��������Ŀ���(���ڱ궨������,����ʱ���ж���,2msһ��)
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Demarc_Vent_Control_Pro(void)
{
    vent_ctrl_timer += VENT_CTRL_PER;        //ÿ2ms����һ��
    flag_mpl501_status = Get_MPL501_Status(); //���ѹ����ȫ��
    switch(phase_vent_ctrl)
    {
        case PHASE_NO_VENT://��ǰ�޻�������
            Valve_Action_in_Manual();        //��������״̬״̬
            break;
        case PHASE_START_VENT://������������
            //Init_Vt_Ad();
            flag_mpl501_status = VAL_CLOSE;//��ѹ����ȫ���򿪱�־
            vent_ctrl_timer = 0;            //ʱ����0
            phase_vent_ctrl = PHASE_VCV_INSP;    //����������
            break;
        case PHASE_VCV_INSP://��������
            Valve_Action_in_Insp();        //���ֽ���״̬
            if(vent_ctrl_timer >= DEMA_INSP_END_TIME)
            {//����ʱ�䵽
                phase_vent_ctrl = PHASE_VCV_HOLD;    //����������
                vent_ppl_timer = 0;                //��С����ʱ����0(����100ms)
            }
            else if(flag_mpl501_status == VAL_OPEN)
            {//ѹ����ȫ����
                phase_vent_ctrl = PHASE_VCV_HOLD;    //����������
            }
            break;
        case PHASE_VCV_HOLD://��������
            Valve_Action_in_Hold();        //��������״̬
            vent_ppl_timer += VENT_CTRL_PER;
            if(flag_mpl501_status == VAL_OPEN)
            {//ѹ����ȫ���򿪻���С����ʱ�䵽
                phase_vent_ctrl = PHASE_VCV_EXP;    //���������
            }
            else if(vent_ppl_timer >= INSP_VAL_CLS_TIME)
            {//ѹ����ȫ���򿪻���С����ʱ�䵽
                phase_vent_ctrl = PHASE_VCV_EXP;    //���������
            }
            break;
        case PHASE_VCV_EXP://���ں���
            Valve_Action_in_Exp();        //���ֺ���״̬
            if(vent_ctrl_timer >= DEMA_VENT_CYCLE)
            {//�������ڵ�
                //demar_laps = Calc_Filt_Fan_Lap();        //�����˲��ķ���Ȧ��
                demarc_vte = Cal_Vt(demar_laps,&demarc_vte_uinits);
                flag_mpl501_status = VAL_CLOSE;//��ѹ����ȫ���򿪱�־
                vent_ctrl_timer = 0;            //ʱ����0
                phase_vent_ctrl = PHASE_VCV_INSP;    //����������
            }
            break;
        case PHASE_PAW_DEM: //���ڱ궨ѹ��������
            Valve_Action_in_Exp(); //�رս��������򿪺���PEEP���ڷ����ر�����PEEP��ѹ��
            break;    
        case PHASE_STOP_VENT://ֹͣ
            //Init_Vt_Ad();        //�����ֵ��0
            demarc_vte = 0;
            phase_vent_ctrl = PHASE_NO_VENT;    //�����޻���״̬
            break;
    }
}

/****************************************************************************
* ���ƣ�Calc_Plimit_Para()
* ���ܣ�����ܵ�ѹ�����Ʊ����жϵ���ֵ
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Calc_Plimit_Para(void)
{
    //ѹ�������ж�ֵ(plimit_compΪ��ǰ����ֵ)
    plimit_pressure = paw_limit_set * 10 - plimit_comp;
    
    //��������ʱ��ĳ����ܵ�ѹ������(0.1cmH2O)
    //Plimit <=30cmH2O, Sustained Pressure = 6+PEEP cmH2O
    //Plimit <=60cmH2O, Sustained Pressure = 20%Plimit+PEEP cmH2O
    //Plimit >60cmH2O, Sustained Pressure = 12+PEEP cmH2O
    if(paw_limit_set <= 30)
    {
        paw_sub_high_thr_vent = 60 + 30;    //�趨��ȻPEEPΪ3cmH2O
    }
    else if(paw_limit_set <= 60)
    {
        paw_sub_high_thr_vent = paw_limit_set * 2 + 30;    //�趨��ȻPEEPΪ3cmH2O
    }
    else{
        paw_sub_high_thr_vent = 120 + 30;    //�趨��ȻPEEPΪ3cmH2O
    }
    
    //�ֶ�����ʱ��ĳ����ܵ�ѹ������(0.1cmH2O)
    //Plimit <=60cmH2O, Sustained Pressure = 50%Plimit
    //Plimit >60cmH2O, Sustained Pressure = 30cmH2O
    if(paw_limit_set <= 60)
    {
        paw_sub_high_thr_manual = paw_limit_set * 5;    //50%Plimit
    }
    else{
        paw_sub_high_thr_manual = 300;                    //30cmH2O
    }
    paw_sub_high_thr_vent = paw_sub_high_thr_manual;    //��ΪPEEP�ɵ�����ȡ�ֶ�ֵ
}

//**********************************************************************
//����  ��;Get_P_Max_Statistic
//��������:��ȡ���ѹ����ͳ��ֵ
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
sint32 Get_P_Max_Statistic(void)
{
    return(paw_max_statistic);
}

//**********************************************************************
//����  ��;Get_P_Min_Statistic
//��������:��ȡ��Сѹ����ͳ��ֵ
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
sint32 Get_P_Min_Statistic(void)
{
    return(paw_min_statistic);
}

//**********************************************************************
//����  ��;Get_Pl_Use
//��������:��ȡѹ������ֵ
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
sint32 Get_Pl_Use(void)
{
    return(plimit_pressure);
}

//**********************************************************************
//����  ��;Get_P_Raise
//��������:��ȡѹ������ֵ
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
sint32 Get_P_Raise(void)
{
    return(airway_p-paw_start);
}

//**********************************************************************
//����  ��;Get_Br_Timer
//��������:��ȡ������ʱʱ��
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
sint32 Get_Br_Timer(void)
{
    return(vent_ctrl_timer);
}

//**********************************************************************
//����  ��;Get_Exp_End_Time
//��������:��ȡ������������ʱ��
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
sint32 Get_Exp_End_Time(void)
{
    return(vent_exp_end_time);
}

//**********************************************************************
//����  ��;Get_Ppl_End_Time
//��������:��ȡ������������ʱ��
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
sint32 Get_Ppl_End_Time(void)
{
    return(vent_ppl_end_time);
}

//**********************************************************************
//����  ��;Get_Insp_End_Time
//��������:��ȡ������������ʱ��
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
sint32 Get_Insp_End_Time(void)
{
    return(vent_insp_end_time);
}

//**********************************************************************
//����  ��:Get_Manual_Vent_Phase
//��������:��ȡ��ǰ�ֹ�ͨ����״̬
//��ڲ���:��
//���ڲ���:ͨ��״̬(TRUE:����״̬;FALSE:����״̬)
//**********************************************************************
uint8 Get_Manual_Vent_Phase(void)
{
    return(flag_manual_insp);
}

//**********************************************************************
//����  ��:Set_Force_Exp_Flag
//��������:��λǿ�ƺ�����־
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Set_Force_Exp_Flag(void)
{
    flag_force_exp = TRUE;
}

//**********************************************************************
//����  ��:Clr_Force_Exp_Flag
//��������:����ǿ�ƺ�����־
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Clr_Force_Exp_Flag(void)
{
    flag_force_exp = FALSE;
}

//**********************************************************************
//����  ��:Check_Force_Exp_Flag
//��������:���ǿ�ƺ�����־
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
uint8 Check_Force_Exp_Flag(void)
{
    return (flag_force_exp);
}

