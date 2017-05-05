//**********************************************************************
//�ļ���:sys_para.c
//˵  ��:ȫ�ֱ���/ϵͳ�����Ĳ���
//�����б�:
//1.�ı���Ϣ����������:Change_Asph_Alm_Set
//2.��ȡ��ǰ����Ϣ��������ֵ:Get_Asph_Alm_Set
//3.����Ϣ�������ñ��浽EEPROM��:Wr_Asph_EEPROM
//4.��EEPROM�ж�ȡ��Ϣ����������ֵ:Rd_Asph_EEPROM
//5.���ñ�������ʱ��:Reset_Alm_Silence_T
//6.��ȡ����������ʣ��ʱ��:Get_Alm_Silence_T
//7.�����û����õĺ���Ƶ��ֵ:Save_Rate_Usr_Set
//8.��EEPROM�ж�ȡ�û����õĺ���Ƶ��ֵ:Rd_Rate_Usr_Set_EEP
//9.��ȡƵ�����ô��ڵ�����ֵ:Get_Rate_Usr_Set
//10.������Ƶ�ʵ�����ֵ:Check_Rate_Usr_Set
//11.�޸ĺ���Ƶ�ʵ�����ֵ:Inc_Dec_Rate_Usr_Set
//12.��ȡ���������봰�ڵ�����ֵ:Get_IE_Usr_Set
//13.�޸������ȵ�����ֵ:Inc_Dec_IE_Usr_Set
//14.��������ȵ�����ֵ�Ƿ񳬳���Χ:Check_IE_Usr_Set
//15.���������ȵ�����ֵ:Save_IE_Usr_Set
//16.��EEPROM�ж�ȡ�����ȵ�����ֵ:Rd_IE_Usr_Set_EEP
//17.��ȡѹ�����ƴ��ڵ�����ֵ:Get_Pl_Usr_Set
//18.�޸�ѹ�����Ƶ�����ֵ:Inc_Dec_Pl_Usr_Set
//19.���ѹ�����Ƶ�����ֵ�Ƿ񳬳���Χ:Check_Pl_Usr_Set
//20.����ѹ�����Ƶ�����ֵ:Save_Pl_Usr_Set
//21.��EEPROM�ж�ȡѹ�����Ƶ�����ֵ:Rd_Pl_Usr_Set_EEP
//22.��ȡ������ͣ���ڵ�����ֵ:Get_Asph_Usr_Set
//23.�޸�������ͣ������ֵ:Inc_Dec_Asph_Usr_Set
//24.���������ͣ����ֵ�Ƿ񳬳���Χ:Check_Asph_Usr_Set
//25.����������ͣ������ֵ:Save_Asph_Usr_Set
//26.��EEPROM�ж�ȡ������ͣ������ֵ:Rd_Asph_Usr_Set_EEP
//27.��ȡ����ѹ��������ֵ:Get_P_Trig_Usr_Set
//28.�޸Ĵ���ѹ��������ֵ:Inc_Dec_P_Trig_Usr_Set
//29.��鴥��ѹ��������ֵ:Check_P_Trig_Usr_Set
//30.���津��ѹ��������ֵ:Save_P_Trig_Usr_Set
//31.��ȡ����ѹ��������ֵ:Rd_P_Trig_Usr_Set_EEP
//32.��ȡ�������ڵ�����ֵ:Get_Trig_Win_Usr_Set
//33.�޸Ĵ������ڵ�����ֵ:Inc_Dec_Trig_Win_Usr_Set
//34.��鴥�����ڵ�����ֵ:Check_Trig_Win_Usr_Set
//35.���津�����ڵ�����ֵ:Save_Trig_Win_Usr_Set
//36.��ȡ�������ڵ�����ֵ:Rd_Trig_Win_Usr_Set_EEP
//37.��ȡ��ʾ�Աȶȵ�����ֵ:Get_Scr_Bri
//38.�޸���ʾ�Աȶ�:Inc_Dec_Scr_Bri
//39.���Աȶȵ������Ƿ񳬳���Χ:Check_Scr_Bri
//40.����Աȵ�����:Save_Scr_Bri
//41.��EEPROM�ж�ȡ�Աȶȵ�����:Rd_Scr_BRi_EEP
//42.��ȡ��������������:Get_Alm_Vol
//43.�޸ı�������:Inc_Dec_Alm_Vol
//44.��鱨�������������Ƿ񳬳���Χ:Check_Alm_Vol
//45.���汨������:Save_Alm_Vol
//46.��EEPROM�ж�ȡ��������:Rd_Alm_Vol_EEP
//47.��ȡ��������ȫ���ġ�������:Get_All_Ch
//48.�޸�"������ȫ����"������:Change_All_Ch_Set
//49.����"������ȫ����"������:Save_All_Ch_Set
//50.��EEPROM�ж�ȡ"������ȫ����"������:Rd_All_Ch_Set_EEP
//51.��ȡ��ǰ�ĺ���ģʽ����:Get_Br_Mode_Sel
//52.���ú���ģʽ:Set_Br_Mode_Sel
//53.�������ģʽ������:Save_Br_Mode_Sel
//54.��EEPROM�ж�ȡ����ģʽ������:Rd_Br_M_Sel_EEP
//55.��ȡ����Ũ�ȱ���������ֵ:Get_O2_Alm_L
//56.�޸�����Ũ�ȱ���������ֵ:Inc_Dec_O2_Alm_L
//57.�������Ũ�ȱ���������ֵ�Ƿ񳬳���Χ:Check_O2_Alm_L
//58.��������Ũ�ȱ���������ֵ:Save_O2_Alm_L
//59.��EEPROM�ж�ȡ����Ũ�ȱ���������ֵ:Rd_O2_Alm_L_EEP
//60.��ȡ����Ũ�ȱ���������ֵ:Get_O2_Alm_H
//61.�޸�����Ũ�ȱ���������ֵ:Inc_Dec_O2_Alm_H
//62.�������Ũ�ȱ���������ֵ�Ƿ񳬳���Χ:Check_O2_Alm_H
//63.��������Ũ�ȱ���������ֵ:Save_O2_Alm_H
//64.��EEPROM�ж�ȡ����Ũ�ȱ���������ֵ:Rd_O2_Alm_H_EEP
//66.��ȡ����ͨ������������ֵ:Get_VE_Alm_L
//67.�޸ķ���ͨ������������ֵ:Inc_Dec_Ve_Alm_L
//68.������ͨ������������ֵ�Ƿ񳬳���Χ:Check_Ve_Alm_L
//69.�������ͨ������������ֵ:Save_Ve_Alm_L
//70.��EEPROM�ж�ȡ����ͨ������������ֵ:Rd_Ve_Alm_L_EEP
//71.��ȡ����ͨ������������ֵ:Get_VE_Alm_H
//72.�޸ķ���ͨ������������ֵ:Inc_Dec_Ve_Alm_H
//73.������ͨ������������ֵ�Ƿ񳬳���Χ:Check_Ve_Alm_H
//74.�������ͨ������������ֵ:Save_Ve_Alm_H
//75.��EEPROM�ж�ȡ����ͨ������������ֵ:Rd_Ve_Alm_H_EEP
//76.��ȡ����������������ֵ:Get_Vt_Alm_L
//77.�޸ĳ���������������ֵ:Inc_Dec_Vt_Alm_L
//78.��鳱��������������ֵ:Check_Vt_Alm_L
//79.���泱��������������ֵ:Save_Vt_Alm_L
//80.��ȡ����������������ֵ:Rd_Vt_Alm_L_EEP
//81.��ȡ����������������ֵ:Get_Vt_Alm_H
//82.�޸ĳ���������������ֵ:Inc_Dec_Vt_Alm_H
//83.��鳱������������ֵ�Ƿ񳬳���Χ:Check_Vt_Alm_H
//84.���泱������������ֵ:Save_Vt_Alm_H
//85.��EEPROM�ж�ȡ����������������ֵ:Rd_Vt_Alm_H_EEP
//86.��ȡ����������"��/��"������:Get_Vt_Alm_Switch
//87.�޸ĳ�����������"��/��"����:Change_Vt_Alm_Switch
//88.��ȡ���θ߶�:Get_Alt
//89.�޸ĺ��θ߶�:Inc_Dec_Alt
//90.��麣�θ߶ȵ������Ƿ񳬳���Χ:Check_Alt
//91.���溣�θ߶�;Save_Alt
//92.��EEPROM�ж�ȡ���θ߶�:Rd_Alt_EEP
//93.��ȡ��������:Get_Drv_Gas
//94.������������:Set_Drv_Gas
//95.������������:Save_Drv_Gas
//96.��EEPROM�ж�ȡ��������:Rd_Drv_Gas_EEP
//97.��ȡ��Ũ�ȼ��"��/��":Get_O2_Inspect
//98.������Ũ�ȼ��"��/��":Set_O2_Inspect
//99.������Ũ�ȼ��"��/��":Save_O2_Inspect
//100.��EEPROM�ж�ȡ��Ũ�ȼ��"��/��":Rd_O2_Inspect_EEP
//101.��ȡ������Ϣ��ʾ��"��/��":Get_Alm_Mess_D
//102.���ñ�����Ϣ��ʾ��"��/��"����:Set_Alm_Mess_D
//103.���汨����Ϣ��ʾ��"��/��"����:Save_Alm_Mess_D
//104.��EEPROM�ж�ȡ������Ϣ��ʾ��"��/��"����:Rd_Alm_Mess_D_EEP
//105.��ȡ���ѹ������ֵ:Get_Pmax
//106.��ȡ���ѹ������ֵ�ĵ�λ:Get_Pmax_Units
//107.�������ѹ������ֵ:Set_Pmax
//108.��ȡƽ��ѹ������ֵ:Get_Pavr
//109.��ȡƽ��ѹ������ֵ�ĵ�λ:Get_Pavr_Units
//110.����ƽ��ѹ������ֵ:Set_Pavr
//111.��ȡѹ����ʵʱ����ֵ:Get_Paw
//112.��ȡѹ������ֵ�ĵ�λ:Get_Paw_Units
//113.����ѹ������ֵ:Set_Paw
//114.��ȡ��Сѹ������ֵ:Get_Pmin
//115.��ȡ��Сѹ������ֵ�ĵ�λ:Get_Pmin_Units
//116.������Сѹ������ֵ:Set_Pmin
//117.��ȡ����������ֵ:Get_Vt
//118.��ȡ����������ֵ�ĵ�λ:Get_Vt_Units
//119.���ó���������ֵ:Set_Vt
//120.��ȡ����Ƶ�ʵĲ���ֵ:Get_Rate_Meas
//121.���ú���Ƶ�ʵĲ���ֵ:Set_Rate_Meas
//122.��ȡ����ͨ������ֵ:Get_Ve
//123.��ȡ����ͨ������ֵ�ĵ�λ:Get_Ve_Units
//124.���÷���ͨ������ֵ:Set_Ve
//125.��ȡ����Ũ��:Get_O2
//126.��ȡ����Ũ�ȵĵ�λ:Get_O2_Units
//127.��������Ũ�ȵĲ���ֵ:Set_O2
//128.��ȡ˳Ӧ�Բ���ֵ:Get_Humor
//129.��ȡ˳Ӧ�Բ���ֵ�ĵ�λ:Get_Humor_Units
//130.����˳Ӧ�Բ���ֵ:Set_Humor
//131.��ȡ���ص�ѹ�Ĳ���ֵ:Get_Batt_Vol
//132.��¼���ص�ѹ�Ĳ���ֵ:Log_Batt_Vol
//133.��ȡ�������Ĳ���ֵ:Get_Batt_Charge_I
//134.��¼�������Ĳ���ֵ:Log_Batt_Charge_I
//135.��ȡ������"��/��"������:Get_Key_Sound_Switch
//136.���ð�����"��/��"������:Set_Key_Sound_Switch
//137.���水����"��/��"������:Save_Key_Sound_Switch
//138.��EEPROM�ж�ȡ������"��/��"������:Rd_Key_Sound_Switch_EEP
//139.��ȡ�������˲�����������:Get_Vt_Filter_Const
//140.�޸ĳ������˲�����������:Inc_Dec_Vt_Filt_C
//141.��鳱�����˲�����������:Check_Vte_Filt_C
//142.���泱�����˲�����������:Save_Vt_Filt_C
//143.��EEPROM�ж�ȡ�������˲�����������:Rd_Vt_Filt_C_EEP
//144.��ȡ��Ũ��ˢ�����ڵ�����:Get_O2_Flash_Cyc
//145.�޸���Ũ��ˢ�����ڵ�����:Inc_Dec_O2_Flash_Cyc
//146.�����Ũ��ˢ�����ڵ�����:Check_O2_Flash_Cyc
//147.������Ũ��ˢ������:Save_O2_Flash_Cyc
//148.��EEPROM�ж�ȡ��Ũ��ˢ������:Rd_O2_Flash_Cyc_EEP
//149.��ȡѹ�����Ʋ���������:Get_Pl_Comp
//150.�޸�ѹ�����Ʋ���������:Inc_Dec_Pl_Comp
//151.���ѹ�����Ʋ���������:Check_Pl_Comp
//152.����ѹ�����Ʋ���������:Save_Pl_Comp
//153.��EEPROM�ж�ȡѹ�����Ʋ���������:Rd_Pl_Comp_EEP
//154.��ȡ��ѹ���������ж��ż�:Get_Paw_No_Raise
//155.�޸Ĺ�ѹ���������ж��ż�:Inc_Dec_Paw_No_R
//156.����ѹ���������ж��ż�:Check_Paw_No_R
//157.�����ѹ���������ж��ż�:Save_Paw_No_R
//158.��EEPROM�ж�ȡ��ѹ���������ж��ż�:Rd_Paw_No_R_EEP

//**********************************************************************
#include "global.h"
#include "sys_para.h"
#include "eeprom.h"
#include "Timer.h"
#include "alarm_pro.h"
#include "type_def.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

#define SCR_BRI_MAX        0x07    //LCD�Աȶȵ����ֵ
#define SCR_BRI_MIN        0x00    //LCD�Աȶ���Сֵ
#define SCR_BRI_DEF        0x03    //LCD�Աȶ�Ĭ��ֵ
static sint8 scr_brightness;        //LCD�Աȶȵ�����ֵ
#define ALM_VOL_MAX        0x07    //�����������ֵ
#define ALM_VOL_MIN        0x00    //����������Сֵ
#define ALM_VOL_DEF        0x03    //��������Ĭ��ֵ
static sint8 alm_vol;            //������������ֵ
static uint8 main_inter_ch;        //������ȫ����
static uint8 breath_mode_sel;    //ѡ��ĺ���ģʽ
static uint8 drv_gas;            //��������
static uint8 o2_inspect;            //��Ũ�ȼ��
static uint8 alm_mess_disp;        //������Ϣ��ʾ
static uint8 key_sound_switch;    //����������


//**********************************************************************
//����  ��:Change_Asph_Alm_Set
//��������:�ı���Ϣ����������
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Change_Asph_Alm_Set(void)
{
    asph_al_set = (asph_al_set==ON)?OFF:ON;
    asph_al_set_chg_flag = TRUE;    //������˸��ʾ
}

//**********************************************************************
//����  ��:Get_Asph_Alm_Set
//��������:��ȡ��ǰ��Ϣ����������
//��ڲ���:��
//���ڲ���:��Ϣ����������
//**********************************************************************
uint8 Get_Asph_Alm_Set(void)
{
    return (asph_al_set);
}

//**********************************************************************
//����  ��:Wr_Asph_EEPROM
//��������:����Ϣ���������ñ��浽EEPROM��
//��ڲ���:��
//���ڲ���:OK--������ȷ
//           ERROR--�������
//**********************************************************************
uint8 Wr_Asph_EEPROM(void)
{
    if(Write_EEPROM_1B(EEP_ASPH_ALM_SET,asph_al_set))
    {
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Asph_EEPROM
//��������:��EEPROM�ж�ȡ��Ϣ����������ֵ
//��ڲ���:����ֵ��ָ��
//���ڲ���:OK--��ȡ��ȷ
//           ERROR--��ȡ����
//**********************************************************************
uint8 Rd_Asph_EEPROM(void)
{
    if(Read_EEPROM_1B(EEP_ASPH_ALM_SET,( uint8*)&asph_al_set))
    {
        asph_al_set = ON;
        return(ERROR);
    }
    else
    {
        if ((asph_al_set != OFF)&&(asph_al_set != ON))
        {
            asph_al_set = ON;
            Wr_Asph_EEPROM();
        }
        return(OK);
    }
}


//**********************************************************************
//����  ��:Reset_Alm_Silence_T
//��������:���ñ�������ʱ��
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Reset_Alm_Silence_T(uint8 type)
{
    if (RST_ALM_SILENCE == type)
    {//������ǰ����
        al_silence_time = TIME_ALARM_MUTE;
    }
    else if (RST_ALM_FORE_SILENCE == type)//Ԥ����
    {
        al_silence_time = TIME_NORMAL_MUTE;
    }
    else//���㱨������ʱ��
    {
        al_silence_time = 0;
    }    
    flag_alm_silence_t_ok = TRUE;
}

//**********************************************************************
//����  ��:Get_Alm_Silence_T
//��������:��ȡ��������ʣ��ʱ��(��λ1s)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
uint8 Get_Alm_Silence_T(void)
{
    if (al_silence_time > TIME_ALARM_MUTE)
        al_silence_time = TIME_ALARM_MUTE;
        
    if (al_silence_time >= 0)
    {
        return(al_silence_time/1000);
    }
    else
    {
        return(0);
    }
}

//**********************************************************************
//����  ��:Save_Rate_Usr_Set
//��������:�����û����õĺ���Ƶ��
//��ڲ���:��
//���ڲ���:OK--����ɹ�
//           ERROR--����ʧ��
//**********************************************************************
uint8 Save_Rate_Usr_Set(void)
{
    if(Write_EEPROM(EEP_RATE_SET,( uint8*)(&br_rate_set_disp),sizeof(br_rate_set_disp)) )
    {//�������
        br_rate_set_disp = br_rate_set;
        return (ERROR);
    }
    else
    {
        br_rate_set = br_rate_set_disp;    //����ֵ��Ч
        return (OK);
    }
}

//**********************************************************************
//����  ��:Rd_Rate_Usr_Set_EEP
//��������:��EEPROM�ж����û����õĺ���Ƶ��ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Rate_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_RATE_SET,(uint8*)(&br_rate_set_disp),sizeof(br_rate_set_disp)))
    {
        br_rate_set_disp = br_rate_set = PL_USR_SET_DEF;
        return(ERROR);
    }
    else
    {
        br_rate_set = br_rate_set_disp;    
        if (Check_Rate_Usr_Set() == ERROR)
        {
            Save_Rate_Usr_Set();//����Ĭ��ֵ
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Rate_Usr_Set
//��������:��ȡ����Ƶ�����ô��ڵ�����ֵ
//��ڲ���:��
//���ڲ���:����Ƶ��
//**********************************************************************
sint32 Get_Rate_Usr_Set(void)
{
    if (br_rate_set_disp > RATE_SET_MAX) br_rate_set_disp=RATE_SET_MAX;
    if (br_rate_set_disp < RATE_SET_MIN) br_rate_set_disp=RATE_SET_MIN;
    return(br_rate_set_disp);
}

//**********************************************************************
//����  ��:Check_Rate_Usr_Set
//��������:����û����õĺ���Ƶ��ֵ�Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Rate_Usr_Set(void)
{
    if (br_rate_set_disp > RATE_SET_MAX)//̫��
    {
        br_rate_set_disp = br_rate_set = RATE_SET_DEF;
        return(ERROR);
    }
    else if (br_rate_set > RATE_SET_MAX)
    {
        br_rate_set_disp = br_rate_set = RATE_SET_DEF;
        return(ERROR);        
    }
    else if (br_rate_set_disp < RATE_SET_MIN)//̫С
    {
        br_rate_set_disp = br_rate_set = RATE_SET_DEF;
        return(ERROR);            
    }
    else if (br_rate_set < RATE_SET_MIN)
    {
        br_rate_set_disp = br_rate_set = RATE_SET_DEF;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Inc_Dec_Rate_Usr_Set
//��������:��/������Ƶ�ʵ�����ֵ,������޸�ֵ�Ƿ���ȷ
//��ڲ���:dir--����
//            step--����
//���ڲ���:OK--�޸���ȷ
//           ERROR--�޸Ĵ���
//**********************************************************************
uint8 Inc_Dec_Rate_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((br_rate_set_disp+step)<=RATE_SET_MAX)
        {
            br_rate_set_disp += step;
            if(rate_cycle_tab[br_rate_set_disp] * ie_i_tab[ie_set]*(100-asph_set) < TIME_MIN_INSP * 256*100)
            {//С����С����ʱ��
                br_rate_set_disp -= step;
                return(ERROR);
            }
            if(rate_cycle_tab[br_rate_set_disp]*(256-ie_i_tab[ie_set]) < TIME_MIN_INSP * 256)
            {//С����С����ʱ��
                br_rate_set_disp -= step;
                return(ERROR);
            }        
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((br_rate_set_disp-step)>=RATE_SET_MIN)
        {
            br_rate_set_disp -= step;
            if(rate_cycle_tab[br_rate_set_disp] * ie_i_tab[ie_set] > TIME_MAX_INSP * 256)
            {//�����������ʱ��
                br_rate_set_disp += step;
                return(ERROR);
            }            
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
}

//**********************************************************************
//����  ��:Get_IE_Usr_Set
//��������:��ȡ���������ô��ڵ�����ֵ
//��ڲ���:��
//���ڲ���:������
//**********************************************************************
uint8 Get_IE_Usr_Set(void)
{
    if (ie_set_disp > IE_IND_MAX) ie_set_disp = IE_IND_MAX;
    if (ie_set_disp <= IE_IND_MIN) ie_set_disp = IE_IND_MIN;
    return(ie_set_disp);
}

//**********************************************************************
//����  ��:Inc_Dec_IE_Usr_Set
//��������:��/�������ȵ�����ֵ,������޸�ֵ�Ƿ���ȷ
//��ڲ���:dir--����
//            step--����
//���ڲ���:OK--�޸���ȷ
//           ERROR--�޸Ĵ���
//**********************************************************************
uint8 Inc_Dec_IE_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((ie_set_disp+step)<=IE_IND_MAX)
        {
            ie_set_disp += step;
            if(rate_cycle_tab[br_rate_set_disp] * ie_i_tab[ie_set]*(100-asph_set) < TIME_MIN_INSP * 256*100)
            {//С����С����ʱ��
                ie_set_disp -= step;
                return(ERROR);
            }
            if(rate_cycle_tab[br_rate_set_disp]*(256-ie_i_tab[ie_set]) < TIME_MIN_INSP * 256)
            {//С����С����ʱ��
                ie_set_disp -= step;
                return(ERROR);
            }                        
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((ie_set_disp-step)>=IE_IND_MIN)
        {
            ie_set_disp -= step;
            if(rate_cycle_tab[br_rate_set] * ie_i_tab[ie_set_disp] > TIME_MAX_INSP * 256)
            {//�����������ʱ��
                ie_set_disp += step;
                return(ERROR);
            }            
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
}

//**********************************************************************
//����  ��:Check_IE_Usr_Set
//��������:����û����õ�������ֵ�Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_IE_Usr_Set(void)
{
    if (ie_set_disp > IE_IND_MAX)//̫��
    {
        ie_set_disp = ie_set =IE_IND_DEF;
        return(ERROR);
    }
    else if (ie_set > IE_IND_MAX)
    {
        ie_set_disp = ie_set = IE_IND_DEF;
        return(ERROR);        
    }
    else if (ie_set_disp < IE_IND_MIN)//̫С
    {
        ie_set_disp = ie_set = IE_IND_DEF;
        return(ERROR);            
    }
    else if (ie_set_disp < IE_IND_MIN)
    {
        ie_set_disp = ie_set = IE_IND_DEF;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}
//**********************************************************************
//����  ��:Save_IE_Usr_Set
//��������:�����û������������ֵ,�������ɹ�������ֵ��Ч,����ظ�ԭ��������ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_IE_Usr_Set(void)
{
    if (Write_EEPROM(EEP_IE_SET,( uint8*)(&ie_set_disp),sizeof(ie_set_disp)) )
    {
        ie_set_disp = ie_set;
        return(ERROR);
    }
    else
    {
        ie_set = ie_set_disp;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_IE_Usr_Set_EEP
//��������:��EEPROM�ж����û����õ�������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_IE_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_IE_SET,( uint8*)(&ie_set_disp),sizeof(ie_set_disp)))
    {
        ie_set_disp = ie_set = IE_IND_DEF;
        return(ERROR);
    }
    else
    {
        ie_set = ie_set_disp;    
        if (Check_IE_Usr_Set() == ERROR)
        {
            Save_IE_Usr_Set();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Pl_Usr_Set
//��������:��ȡѹ�����ƴ��ڵ�����ֵ
//��ڲ���:��
//���ڲ���:ѹ������
//**********************************************************************
sint16 Get_Pl_Usr_Set(void)
{
    if (paw_limit_set_disp>PL_USR_SET_MAX) paw_limit_set_disp=PL_USR_SET_MAX;
    if (paw_limit_set_disp<PL_USR_SET_MIN) paw_limit_set_disp=PL_USR_SET_MIN;
    return(paw_limit_set_disp);    
}

//**********************************************************************
//����  ��:Inc_Dec_Pl_Usr_Set
//��������:��/��ѹ�����Ƶ�����ֵ,������޸�ֵ�Ƿ���ȷ
//��ڲ���:dir--����
//            step--����
//���ڲ���:OK--�޸���ȷ
//           ERROR--�޸Ĵ���
//**********************************************************************
uint8 Inc_Dec_Pl_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((paw_limit_set_disp+step)<=PL_USR_SET_MAX)
        {
            paw_limit_set_disp += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((paw_limit_set_disp-step)>=PL_USR_SET_MIN)
        {
            paw_limit_set_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
}

//**********************************************************************
//����  ��:Check_Pl_Usr_Set
//��������:����û����õ�ѹ������ֵ�Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Pl_Usr_Set(void)
{
    if (paw_limit_set_disp > PL_USR_SET_MAX)//̫��
    {
        paw_limit_set_disp = paw_limit_set = PL_USR_SET_DEF;
        return(ERROR);
    }
    else if (paw_limit_set > PL_USR_SET_MAX)
    {
        paw_limit_set_disp = paw_limit_set = PL_USR_SET_DEF;
        return(ERROR);        
    }
    else if (paw_limit_set_disp < PL_USR_SET_MIN)//̫С
    {
        paw_limit_set_disp = paw_limit_set = PL_USR_SET_DEF;
        return(ERROR);            
    }
    else if (paw_limit_set < PL_USR_SET_MIN)
    {
        paw_limit_set_disp = paw_limit_set = PL_USR_SET_DEF;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Pl_Usr_Set
//��������:�����û������ѹ������ֵ,�������ɹ�������ֵ��Ч,����ظ�ԭ��������ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Pl_Usr_Set(void)
{
    if (Write_EEPROM(EEP_PL_SET,( uint8*)(&paw_limit_set_disp),sizeof(paw_limit_set_disp)) )
    {
        paw_limit_set_disp = paw_limit_set;
        return(ERROR);
    }
    else
    {
        paw_limit_set = paw_limit_set_disp;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Pl_Usr_Set_EEP
//��������:��EEPROM�ж����û����õ�ѹ������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Pl_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_PL_SET,( uint8*)(&paw_limit_set_disp),sizeof(paw_limit_set_disp)))
    {
        paw_limit_set_disp = paw_limit_set = PL_USR_SET_DEF;
        return(ERROR);
    }
    else
    {
        paw_limit_set = paw_limit_set_disp;    
        if (Check_Pl_Usr_Set() == ERROR)
        {
            Save_Pl_Usr_Set();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Asph_Usr_Set
//��������:��ȡ������ͣ���ڵ�����ֵ
//��ڲ���:��
//���ڲ���:������ͣ
//**********************************************************************
uint8 Get_Asph_Usr_Set(void)
{
    if (asph_set_disp>ASPH_USR_SET_MAX) asph_set_disp=ASPH_USR_SET_MAX;
    if (asph_set_disp<=ASPH_USR_SET_MIN) asph_set_disp=ASPH_USR_SET_MIN;
    return(asph_set_disp);    
}

//**********************************************************************
//����  ��:Inc_Dec_Asph_Usr_Set
//��������:����/��������ʱ�������ֵ,������޸��Ƿ���ȷ
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Asph_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((asph_set_disp+step)<=ASPH_USR_SET_MAX)
        {
            asph_set_disp += step;
            if(rate_cycle_tab[br_rate_set_disp] * ie_i_tab[ie_set]*(100-asph_set) < TIME_MIN_INSP * 256*100)
            {//С����С����ʱ��
                asph_set_disp -= step;
                return(ERROR);
            }
            if(rate_cycle_tab[br_rate_set_disp]*(256-ie_i_tab[ie_set]) < TIME_MIN_INSP * 256)
            {//С����С����ʱ��
                asph_set_disp -= step;
                return(ERROR);
            }            
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((asph_set_disp-step)>=ASPH_USR_SET_MIN)
        {
            asph_set_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Asph_Usr_Set
//��������:����û����õ�������ֵͣ�Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Asph_Usr_Set(void)
{
    if (asph_set_disp > ASPH_USR_SET_MAX)//̫��
    {
        asph_set_disp = asph_set = ASPH_USR_SET_DEF;
        return(ERROR);
    }
    else if (asph_set > ASPH_USR_SET_MAX)
    {
        asph_set_disp = asph_set = ASPH_USR_SET_DEF;
        return(ERROR);        
    }
    else if (asph_set_disp < ASPH_USR_SET_MIN)//̫С
    {
        asph_set_disp = asph_set = ASPH_USR_SET_DEF;
        return(ERROR);            
    }
    else if (asph_set < ASPH_USR_SET_MIN)
    {
        asph_set_disp = asph_set = ASPH_USR_SET_DEF;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Asph_Usr_Set
//��������:�����û������������ֵͣ,�������ɹ�������ֵ��Ч,����ظ�ԭ��������ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Asph_Usr_Set(void)
{
    if (Write_EEPROM(EEP_ASPH_SET,( uint8*)(&asph_set_disp),sizeof(asph_set_disp)) )
    {
        asph_set_disp = asph_set;
        return(ERROR);
    }
    else
    {
        asph_set = asph_set_disp;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Asph_Usr_Set_EEP
//��������:��EEPROM�ж����û����õ�������ֵͣ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Asph_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_ASPH_SET,( uint8*)(&asph_set_disp),sizeof(asph_set_disp)))
    {
        asph_set_disp = asph_set = PL_USR_SET_DEF;
        return(ERROR);
    }
    else
    {
        asph_set = asph_set_disp;    
        if (Check_Asph_Usr_Set() == ERROR)
        {
            Save_Asph_Usr_Set();
        }

        return(OK);
    }
}


//**********************************************************************
//����  ��:Get_P_Trig_Usr_Set
//��������:��ȡ����ѹ��������ֵ
//��ڲ���:��
//���ڲ���:����ѹ��
//**********************************************************************
uint8 Get_P_Trig_Usr_Set(void)
{
    if (trigger_p_disp>TRIGGER_P_SET_MAX) trigger_p_disp=TRIGGER_P_SET_MAX;
    if (trigger_p_disp<=TRIGGER_P_SET_MIN) trigger_p_disp=TRIGGER_P_SET_MIN;
    return(trigger_p_disp);    
}

//**********************************************************************
//����  ��:Inc_Dec_P_Trig_Usr_Set
//��������:����/���ٴ���ѹ��������ֵ,������޸��Ƿ���ȷ
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_P_Trig_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((trigger_p_disp+step)<=TRIGGER_P_SET_MAX)
        {
            trigger_p_disp += step;        
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((trigger_p_disp-step)>=TRIGGER_P_SET_MIN)
        {
            trigger_p_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_P_Trig_Usr_Set
//��������:����û����õĴ���ѹ��ֵ�Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_P_Trig_Usr_Set(void)
{
    if (trigger_p_disp > TRIGGER_P_SET_MAX)//̫��
    {
        trigger_p_disp = trigger_p = TRIGGER_P_SET_DEFAULT;
        return(ERROR);
    }
    else if (trigger_p > TRIGGER_P_SET_MAX)
    {
        trigger_p_disp = trigger_p = TRIGGER_P_SET_DEFAULT;
        return(ERROR);        
    }
    else if (trigger_p_disp < TRIGGER_P_SET_MIN)//̫С
    {
        trigger_p_disp = trigger_p = TRIGGER_P_SET_DEFAULT;
        return(ERROR);            
    }
    else if (trigger_p < TRIGGER_P_SET_MIN)
    {
        trigger_p_disp = trigger_p = TRIGGER_P_SET_DEFAULT;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_P_Trig_Usr_Set
//��������:�����û�����Ĵ���ѹ��ֵ,�������ɹ�������ֵ��Ч,����ظ�ԭ��������ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_P_Trig_Usr_Set(void)
{
    if (Write_EEPROM(EEP_TRIGGER_P,( uint8*)(&trigger_p_disp),sizeof(trigger_p_disp)) )
    {
        trigger_p_disp = trigger_p;
        return(ERROR);
    }
    else
    {
        trigger_p = trigger_p_disp;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_P_Trig_Usr_Set_EEP
//��������:��EEPROM�ж����û����õĴ���ѹ��ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_P_Trig_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_TRIGGER_P,( uint8*)(&trigger_p_disp),sizeof(trigger_p_disp)))
    {
        trigger_p_disp = trigger_p = TRIGGER_P_SET_DEFAULT;
        return(ERROR);
    }
    else
    {
        trigger_p = trigger_p_disp;    
        if (Check_P_Trig_Usr_Set() == ERROR)
        {
            Save_P_Trig_Usr_Set();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Trig_Win_Usr_Set
//��������:��ȡ�������ڵ�����ֵ
//��ڲ���:��
//���ڲ���:������ͣ
//**********************************************************************
uint8 Get_Trig_Win_Usr_Set(void)
{
    if (trigger_window_disp>TRIGGER_WIN_SET_MAX) trigger_window_disp=TRIGGER_WIN_SET_MAX;
    if (trigger_window_disp<=TRIGGER_WIN_SET_MIN) trigger_window_disp=TRIGGER_WIN_SET_MIN;
    return(trigger_window_disp);    
}

//**********************************************************************
//����  ��:Inc_Dec_Trig_Win_Usr_Set
//��������:����/���ٴ������ڵ�����ֵ,������޸��Ƿ���ȷ
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Trig_Win_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((trigger_window_disp+step)<=TRIGGER_WIN_SET_MAX)
        {
            trigger_window_disp += step;        
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((trigger_window_disp-step)>=TRIGGER_WIN_SET_MIN)
        {
            trigger_window_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Trig_Win_Usr_Set
//��������:����û����õĴ�������ֵ�Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Trig_Win_Usr_Set(void)
{
    if (trigger_window_disp > TRIGGER_WIN_SET_MAX)//̫��
    {
        trigger_window_disp = trigger_window = TRIGGER_WIN_SET_DEFAULT;
        return(ERROR);
    }
    else if (trigger_window > TRIGGER_WIN_SET_MAX)
    {
        trigger_window_disp = trigger_window = TRIGGER_WIN_SET_DEFAULT;
        return(ERROR);        
    }
    else if (trigger_window_disp < TRIGGER_WIN_SET_MIN)//̫С
    {
        trigger_window_disp = trigger_window = TRIGGER_WIN_SET_DEFAULT;
        return(ERROR);            
    }
    else if (trigger_window < TRIGGER_WIN_SET_MIN)
    {
        trigger_window_disp = trigger_window = TRIGGER_WIN_SET_DEFAULT;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Trig_Win_Usr_Set
//��������:�����û�����Ĵ�������ֵ,�������ɹ�������ֵ��Ч,����ظ�ԭ��������ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Trig_Win_Usr_Set(void)
{
    if (Write_EEPROM(EEP_TRIGGER_WIN,( uint8*)(&trigger_window_disp),sizeof(trigger_window_disp)) )
    {
        trigger_window_disp = trigger_window;
        return(ERROR);
    }
    else
    {
        trigger_window = trigger_window_disp;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Trig_Win_Usr_Set_EEP
//��������:��EEPROM�ж����û����õĴ�������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Trig_Win_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_TRIGGER_WIN,( uint8*)(&trigger_window_disp),sizeof(trigger_window_disp)))
    {
        trigger_window_disp = trigger_window = TRIGGER_WIN_SET_DEFAULT;
        return(ERROR);
    }
    else
    {
        trigger_window = trigger_window_disp;    
        if (Check_Trig_Win_Usr_Set() == ERROR)
        {
            Save_Trig_Win_Usr_Set();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Scr_Bri
//��������:��ȡ��ǰ����ʾ���Աȶ�����ֵ
//��ڲ���:��
//���ڲ���:�Աȶ�
//**********************************************************************
uint8 Get_Scr_Bri(void)
{
    if (scr_brightness > SCR_BRI_MAX) scr_brightness = SCR_BRI_MAX;
    if (scr_brightness < SCR_BRI_MIN) scr_brightness = SCR_BRI_MIN;
    return(scr_brightness);    
}

//**********************************************************************
//����  ��:Inc_Dec_Scr_Bri
//��������:����/������ʾ���Աȶ�
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Scr_Bri(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((scr_brightness+step)<=SCR_BRI_MAX)
        {
            scr_brightness += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((scr_brightness-step)>=SCR_BRI_MIN)
        {
            scr_brightness -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Scr_Bri
//��������:����û����õ���ʾ���Աȶ��Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Scr_Bri(void)
{
    if (scr_brightness > SCR_BRI_MAX)//̫��
    {
        scr_brightness = SCR_BRI_DEF;
        return(ERROR);
    }
    else if (scr_brightness < SCR_BRI_MIN)//̫С
    {
        scr_brightness = SCR_BRI_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Scr_Bri
//��������:�����û�����ĶԱȶ�ֵ,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Scr_Bri(void)
{
    if (Write_EEPROM(EEP_SCR_BR,( uint8*)(&scr_brightness),sizeof(scr_brightness)) )
    {
        scr_brightness = SCR_BRI_DEF;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Scr_BRi_EEP
//��������:��EEPROM�ж����û����õ�LCD�Աȶ�,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Scr_BRi_EEP(void)
{
    if (Read_EEPROM(EEP_SCR_BR,( uint8*)(&scr_brightness),sizeof(scr_brightness)))
    {
        scr_brightness = SCR_BRI_DEF;
        return(ERROR);
    }
    else
    {
        if (Check_Scr_Bri() == ERROR)
        {
            Save_Scr_Bri();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Alm_Vol
//��������:��ȡ��ǰ�ı�����������ֵ
//��ڲ���:��
//���ڲ���:��������
//**********************************************************************
uint8 Get_Alm_Vol(void)
{
    if (alm_vol > ALM_VOL_MAX) alm_vol = ALM_VOL_MAX;
    if (alm_vol < ALM_VOL_MIN) alm_vol = ALM_VOL_MIN;
    return(alm_vol);    
}

//**********************************************************************
//����  ��:Inc_Dec_Alm_Vol
//��������:����/���ٱ�������
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Alm_Vol(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((alm_vol+step)<=ALM_VOL_MAX)
        {
            alm_vol += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((alm_vol-step)>=ALM_VOL_MIN)
        {
            alm_vol -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Alm_Vol
//��������:��鱨�����������Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Alm_Vol(void)
{
    if (alm_vol > ALM_VOL_MAX)//̫��
    {
        alm_vol = ALM_VOL_DEF;
        return(ERROR);
    }
    else if (alm_vol < ALM_VOL_MIN)//̫С
    {
        alm_vol = ALM_VOL_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Alm_Vol
//��������:�����û�����ı�������,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Alm_Vol(void)
{
    if (Write_EEPROM(EEP_ALM_VOL,( uint8*)(&alm_vol),sizeof(alm_vol)) )
    {
        alm_vol = ALM_VOL_DEF;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Alm_Vol_EEP
//��������:��EEPROM�ж����ñ�������,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Alm_Vol_EEP(void)
{
    if (Read_EEPROM(EEP_ALM_VOL,( uint8*)(&alm_vol),sizeof(alm_vol)) )
    {
        alm_vol = ALM_VOL_DEF;
        return(ERROR);
    }
    else
    {
        if (Check_Alm_Vol() == ERROR)
        {
            Save_Alm_Vol();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_All_Ch
//��������:��ȡ��ǰ��������ȫ��������
//��ڲ���:��
//���ڲ���:������ȫ��������
//**********************************************************************
uint8 Get_All_Ch(void)
{
    return(main_inter_ch);    
}

//**********************************************************************
//����  ��:Change_All_Ch_Set
//��������:�޸�������ȫ��������
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
uint8 Change_All_Ch_Set(void)
{
    main_inter_ch = (main_inter_ch)?FALSE:TRUE;
    return (main_inter_ch);
}


//**********************************************************************
//����  ��:Save_All_Ch_Set
//��������:����������ȫ��������,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_All_Ch_Set(void)
{
    if (Write_EEPROM(EEP_ALL_CH,( uint8*)(&main_inter_ch),sizeof(main_inter_ch)) )
    {
        main_inter_ch = TRUE;
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_All_Ch_Set_EEP
//��������:��EEPROM�ж����ñ�������,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_All_Ch_Set_EEP(void)
{
    if (Read_EEPROM(EEP_ALL_CH,( uint8*)(&main_inter_ch),sizeof(main_inter_ch)) )
    {
        main_inter_ch = TRUE;
        return(ERROR);
    }
    else
    {
        if ((main_inter_ch != TRUE)&&(main_inter_ch != FALSE))
        {
            main_inter_ch = TRUE;
            Save_All_Ch_Set();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Br_Mode_Sel
//��������:��ȡ��ǰѡ��ĺ���ģʽ
//��ڲ���:��
//���ڲ���:����ģʽ
//**********************************************************************
uint8 Get_Br_Mode_Sel(void)
{
    return(breath_mode_sel);
}

//**********************************************************************
//����  ��:Set_Br_Mode_Sel
//��������:���ú���ģʽ
//��ڲ���:para--����ģʽ
//���ڲ���:��
//**********************************************************************
void Set_Br_Mode_Sel(uint8 para)
{
    breath_mode_sel = para;
}

//**********************************************************************
//����  ��:Save_Br_Mode_Sel
//��������:�������ģʽ������
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Br_Mode_Sel(void)
{
    if (Write_EEPROM(EEP_BR_M_SEL,( uint8*)(&breath_mode_sel),sizeof(breath_mode_sel)) )
    {
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Br_M_Sel_EEP
//��������:��EEPROM�ж�������ģʽ����ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Br_M_Sel_EEP(void)
{
    if (Read_EEPROM(EEP_BR_M_SEL,( uint8*)(&breath_mode_sel),sizeof(breath_mode_sel)) )
    {
        breath_mode_sel = BRM_DEF;
        return(ERROR);
    }
    else
    {
        if ((breath_mode_sel != BRM_VCV)&&(breath_mode_sel != BRM_SIPPV)&&(breath_mode_sel != BRM_SIMV))
        {
            breath_mode_sel = BRM_DEF;
            Save_Br_Mode_Sel();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_O2_Alm_L
//��������:��ȡ��ǰ������Ũ�ȱ�������ֵ������ֵ
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
uint8 Get_O2_Alm_L(void)
{
    if (o2_alm_l_disp > O2_ALM_L_MAX)    o2_alm_l_disp = O2_ALM_L_MAX;
    if (o2_alm_l_disp < O2_ALM_L_MIN)    o2_alm_l_disp = O2_ALM_L_MIN;
    return(o2_alm_l_disp);
}

//**********************************************************************
//����  ��:Inc_Dec_O2_Alm_L
//��������:����/��������Ũ�ȱ�������ֵ
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_O2_Alm_L(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        o2_alm_l_disp += step;
        if ((o2_alm_l_disp<=O2_ALM_L_MAX)&&(o2_alm_l_disp < o2_alm_h))
        {    
            return(OK);
        }
        else
        {
            o2_alm_l_disp -= step;
            return(ERROR);
        }
    }
    else
    {
        o2_alm_l_disp -= step;
        if ((o2_alm_l_disp>=O2_ALM_L_MIN)&&(o2_alm_l_disp < o2_alm_h))
        {
            return(OK);
        }
        else
        {
            o2_alm_l_disp += step;
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_O2_Alm_L
//��������:�������Ũ�ȱ������������Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_O2_Alm_L(void)
{
    if (o2_alm_l > O2_ALM_L_MAX)//̫��
    {
        o2_alm_l = O2_ALM_L_DEF;
        return(ERROR);
    }
    else if (o2_alm_l < O2_ALM_L_MIN)//̫С
    {
        o2_alm_l = O2_ALM_L_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_O2_Alm_L
//��������:�����û����������Ũ�ȱ�������,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_O2_Alm_L(void)
{
    if (Write_EEPROM(EEP_O2_ALM_L,( uint8*)(&o2_alm_l_disp),sizeof(o2_alm_l_disp)) )
    {
        o2_alm_l = o2_alm_l_disp =O2_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        o2_alm_l = o2_alm_l_disp ;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_O2_Alm_L_EEP
//��������:��EEPROM�ж�������Ũ�ȱ�����������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_O2_Alm_L_EEP(void)
{
    if (Read_EEPROM(EEP_O2_ALM_L,( uint8*)(&o2_alm_l),sizeof(o2_alm_l)) )
    {
        o2_alm_l_disp = o2_alm_l = O2_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        o2_alm_l_disp = o2_alm_l;    
        if (Check_O2_Alm_L() == ERROR)
        {
            Save_O2_Alm_L();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_O2_Alm_H
//��������:��ȡ��ǰ������Ũ�ȱ�������ֵ������ֵ
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
uint8 Get_O2_Alm_H(void)
{
    if (o2_alm_h_disp > O2_ALM_H_MAX)    o2_alm_h_disp = O2_ALM_H_MAX;
    if (o2_alm_h_disp < O2_ALM_H_MIN)    o2_alm_h_disp = O2_ALM_H_MIN;
    return(o2_alm_h_disp);
}

//**********************************************************************
//����  ��:Inc_Dec_O2_Alm_H
//��������:����/��������Ũ�ȱ�������ֵ
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_O2_Alm_H(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        o2_alm_h_disp += step;
        if ((o2_alm_h_disp<=O2_ALM_H_MAX)&&(o2_alm_h_disp > o2_alm_l))
        {
            return(OK);
        }
        else
        {
            o2_alm_h_disp -= step;
            return(ERROR);
        }
    }
    else
    {
        o2_alm_h_disp -= step;
        if ((o2_alm_h_disp>=O2_ALM_H_MIN)&&(o2_alm_h_disp > o2_alm_l))
        {
            return(OK);
        }
        else
        {
            o2_alm_h_disp += step;
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_O2_Alm_H
//��������:�������Ũ�ȱ�������ֵ�����Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_O2_Alm_H(void)
{
    if (o2_alm_h > O2_ALM_H_MAX)//̫��
    {
        o2_alm_h = O2_ALM_H_DEF;
        return(ERROR);
    }
    else if (o2_alm_h < O2_ALM_H_MIN)//̫С
    {
        o2_alm_h = O2_ALM_H_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_O2_Alm_H
//��������:�����û����������Ũ�ȱ�������ֵ,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_O2_Alm_H(void)
{
    if (Write_EEPROM(EEP_O2_ALM_H,( uint8*)(&o2_alm_h_disp),sizeof(o2_alm_h_disp)) )
    {
        o2_alm_h = o2_alm_h_disp =O2_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        o2_alm_h = o2_alm_h_disp;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_O2_Alm_H_EEP
//��������:��EEPROM�ж�������Ũ�ȱ�������ֵ����ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_O2_Alm_H_EEP(void)
{
    if (Read_EEPROM(EEP_O2_ALM_H,( uint8*)(&o2_alm_h),sizeof(o2_alm_h)) )
    {
        o2_alm_h_disp = o2_alm_h = O2_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        o2_alm_h_disp = o2_alm_h;    
        if (Check_O2_Alm_H() == ERROR)
        {
            Save_O2_Alm_H();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_VE_Alm_L
//��������:��ȡ��ǰ�ķ���ͨ����������ֵ
//��ڲ���:��
//���ڲ���:����ֵ(��λ:0.1L/min)
//**********************************************************************
sint32 Get_VE_Alm_L(void)
{
    if (ve_alm_l_disp > VE_ALM_L_MAX)    ve_alm_l_disp = VE_ALM_L_MAX;
    if (ve_alm_l_disp < VE_ALM_L_MIN)    ve_alm_l_disp = VE_ALM_L_MIN;
    return(ve_alm_l_disp);
}

//**********************************************************************
//����  ��:Inc_Dec_Ve_Alm_L
//��������:����/���ٷ���ͨ����������ֵ
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Ve_Alm_L(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((ve_alm_l_disp+step)<=VE_ALM_L_MAX)
        {
            ve_alm_l_disp += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((ve_alm_l_disp-step)>=VE_ALM_L_MIN)
        {
            ve_alm_l_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Ve_Alm_L
//��������:������ͨ����������ֵ�����Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Ve_Alm_L(void)
{
    if (ve_alm_l > VE_ALM_L_MAX)//̫��
    {
        ve_alm_l = VE_ALM_L_DEF;
        return(ERROR);
    }
    else if (ve_alm_l < VE_ALM_L_MIN)//̫С
    {
        ve_alm_l = VE_ALM_L_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Ve_Alm_L
//��������:�����û�����ķ���ͨ����������ֵ,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Ve_Alm_L(void)
{
    if (Write_EEPROM(EEP_VE_ALM_L,( uint8*)(&ve_alm_l_disp),sizeof(ve_alm_l_disp)) )
    {
        ve_alm_l = ve_alm_l_disp = VE_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        ve_alm_l = ve_alm_l_disp;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Ve_Alm_L_EEP
//��������:��EEPROM�ж�������ͨ��������������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Ve_Alm_L_EEP(void)
{
    if (Read_EEPROM(EEP_VE_ALM_L,( uint8*)(&ve_alm_l),sizeof(ve_alm_l)) )
    {
        ve_alm_l_disp = ve_alm_l = VE_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        ve_alm_l_disp = ve_alm_l;    
        if (Check_Ve_Alm_L() == ERROR)
        {
            Save_Ve_Alm_L();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_VE_Alm_H
//��������:��ȡ��ǰ�ķ���ͨ��������������ֵ
//��ڲ���:��
//���ڲ���:����ֵ(��λ:0.1L/min)
//**********************************************************************
sint32 Get_VE_Alm_H(void)
{
    if (ve_alm_h_disp > VE_ALM_H_MAX)    ve_alm_h_disp = VE_ALM_H_MAX;
    if (ve_alm_h_disp < VE_ALM_H_MIN)    ve_alm_h_disp = VE_ALM_H_MIN;
    return(ve_alm_h_disp);
}

//**********************************************************************
//����  ��:Inc_Dec_Ve_Alm_H
//��������:����/���ٷ���ͨ����������ֵ
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Ve_Alm_H(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        ve_alm_h_disp += step;
        if ((ve_alm_h_disp<=VE_ALM_H_MAX)&&(ve_alm_h_disp>ve_alm_l))
        {
            return(OK);
        }
        else
        {
            ve_alm_h_disp -= step;
            return(ERROR);
        }
    }
    else
    {
        ve_alm_h_disp -= step;
        if ((ve_alm_h_disp>=VE_ALM_H_MIN)&&(ve_alm_h_disp>ve_alm_l))
        {
            return(OK);
        }
        else
        {
            ve_alm_h_disp += step;
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Ve_Alm_H
//��������:������ͨ����������ֵ�����Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Ve_Alm_H(void)
{
    if (ve_alm_h > VE_ALM_H_MAX)//̫��
    {
        ve_alm_h = VE_ALM_H_DEF;
        return(ERROR);
    }
    else if (ve_alm_h < VE_ALM_H_MIN)//̫С
    {
        ve_alm_h = VE_ALM_H_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Ve_Alm_H
//��������:�����û�����ķ���ͨ����������ֵ,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Ve_Alm_H(void)
{
    if (Write_EEPROM(EEP_VE_ALM_H,( uint8*)(&ve_alm_h_disp),sizeof(ve_alm_h_disp)) )
    {
        ve_alm_h = ve_alm_h_disp = VE_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        ve_alm_h = ve_alm_h_disp;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Ve_Alm_H_EEP
//��������:��EEPROM�ж�������ͨ��������������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Ve_Alm_H_EEP(void)
{
    if (Read_EEPROM(EEP_VE_ALM_H,( uint8*)(&ve_alm_h),sizeof(ve_alm_h)) )
    {
        ve_alm_h_disp = ve_alm_h = VE_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        ve_alm_h_disp = ve_alm_h;    
        if (Check_Ve_Alm_H() == ERROR)
        {
            Save_Ve_Alm_H();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Vt_Alm_L
//��������:��ȡ��ǰ�ĳ�������������ֵ
//��ڲ���:��
//���ڲ���:����ֵ(1ml)
//**********************************************************************
sint16 Get_Vt_Alm_L(void)
{
    if (vt_alm_l_disp > VT_ALM_L_MAX)    vt_alm_l_disp = VT_ALM_L_MAX;
    if (vt_alm_l_disp < VT_ALM_L_MIN)    vt_alm_l_disp = VT_ALM_L_MIN;
    return(vt_alm_l_disp);
}

//**********************************************************************
//����  ��:Inc_Dec_Vt_Alm_L
//��������:����/���ٳ�������������ֵ
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Vt_Alm_L(uint8 dir,uint8 step)
{
    if (vt_alm_l_disp>20) step = 20;
    if (dir == INC)
    {
        vt_alm_l_disp += step;
        if ((vt_alm_l_disp<=VT_ALM_L_MAX)&&(vt_alm_l_disp < vt_alm_h))
        {
            
            return(OK);
        }
        else
        {
            vt_alm_l_disp -= step;
            return(ERROR);
        }
    }
    else
    {
        vt_alm_l_disp -= step;
        if ((vt_alm_l_disp>=VT_ALM_L_MIN)&&(vt_alm_l_disp < vt_alm_h))
        {
            return(OK);
        }
        else
        {
            vt_alm_l_disp += step;
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Vt_Alm_L
//��������:��鳱������������ֵ�����Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Vt_Alm_L(void)
{
    if (vt_alm_l > VT_ALM_L_MAX)//̫��
    {
        vt_alm_l = VT_ALM_L_DEF;
        return(ERROR);
    }
    else if (ve_alm_l < VE_ALM_L_MIN)//̫С
    {
        vt_alm_l = VT_ALM_L_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Vt_Alm_L
//��������:�����û�����ĳ�������������ֵ,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Vt_Alm_L(void)
{
    if (Write_EEPROM(EEP_VT_ALM_L,( uint8*)(&vt_alm_l_disp),sizeof(vt_alm_l_disp)) )
    {
        vt_alm_l = vt_alm_l_disp = VT_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        vt_alm_l = vt_alm_l_disp;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Vt_Alm_L_EEP
//��������:��EEPROM�ж���������������������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Vt_Alm_L_EEP(void)
{
    if (Read_EEPROM(EEP_VT_ALM_L,( uint8*)(&vt_alm_l),sizeof(vt_alm_l)) )
    {
        vt_alm_l_disp = vt_alm_l = VT_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        vt_alm_l_disp = vt_alm_l;    
        if (Check_Vt_Alm_L() == ERROR)
        {
            Save_Vt_Alm_L();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Vt_Alm_H
//��������:��ȡ��ǰ�ĳ�������������ֵ
//��ڲ���:��
//���ڲ���:����ֵ(1ml)
//**********************************************************************
sint16 Get_Vt_Alm_H(void)
{
    if (vt_alm_h_disp > VT_ALM_H_MAX)    vt_alm_h_disp = VT_ALM_H_MAX;
    if (vt_alm_h_disp < VT_ALM_H_MIN)    vt_alm_h_disp = VT_ALM_H_MIN;
    return(vt_alm_h_disp);
}

//**********************************************************************
//����  ��:Inc_Dec_Vt_Alm_H
//��������:����/���ٳ�������������ֵ
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Vt_Alm_H(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        vt_alm_h_disp += step;
        if ((vt_alm_h_disp<=VT_ALM_H_MAX)&&(vt_alm_h_disp > vt_alm_l))
        {    
            return(OK);
        }
        else
        {
            vt_alm_h_disp -= step;
            return(ERROR);
        }
    }
    else
    {
        vt_alm_h_disp -= step;
        if ((vt_alm_h_disp>=VT_ALM_H_MIN)&&(vt_alm_h_disp > vt_alm_l))
        {
            return(OK);
        }
        else
        {
            vt_alm_h_disp += step;
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Vt_Alm_H
//��������:��鳱������������ֵ�����Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Vt_Alm_H(void)
{
    if (vt_alm_h > VT_ALM_H_MAX)//̫��
    {
        vt_alm_h = VT_ALM_H_DEF;
        return(ERROR);
    }
    else if (ve_alm_h < VE_ALM_H_MIN)//̫С
    {
        vt_alm_h = VT_ALM_H_DEF;
        return(ERROR);            
    }    
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Vt_Alm_H
//��������:�����û�����ĳ�������������ֵ,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Vt_Alm_H(void)
{
    if (Write_EEPROM(EEP_VT_ALM_H,(uint8*)(&vt_alm_h_disp),sizeof(vt_alm_h_disp)) )
    {
        vt_alm_h = vt_alm_h_disp = VT_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        vt_alm_h = vt_alm_h_disp;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Vt_Alm_H_EEP
//��������:��EEPROM�ж���������������������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Vt_Alm_H_EEP(void)
{
    if (Read_EEPROM(EEP_VT_ALM_H,( uint8*)(&vt_alm_h),sizeof(vt_alm_h)) )
    {
        vt_alm_h_disp = vt_alm_h = VT_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        vt_alm_h_disp = vt_alm_h;    
        if (Check_Vt_Alm_H() == ERROR)
        {
            Save_Vt_Alm_H();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Vt_Alm_Switch
//��������:��ȡ������������������
//��ڲ���:��
//���ڲ���:TRUE--�رճ���������
//            FASLE--�򿪳���������
//**********************************************************************
uint8 Get_Vt_Alm_Switch(void)
{
    return(vt_alarm_off);
}

//**********************************************************************
//����  ��:Change_Vt_Alm_Switch
//��������:���ó�����������������
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Change_Vt_Alm_Switch(void)
{
    vt_alarm_off = (vt_alarm_off)?FALSE:TRUE;
}

//**********************************************************************
//����  ��:Get_Alt
//��������:��ȡ��ǰ�ĺ��θ߶�����ֵ
//��ڲ���:��
//���ڲ���:����ֵ(1ml)
//**********************************************************************
sint16 Get_Alt(void)
{
    if (altitude_disp > ALT_MAX)    altitude_disp = ALT_MAX;
    if (altitude_disp < ALT_MIN)    altitude_disp = ALT_MIN;
    return(altitude_disp);
}

//**********************************************************************
//����  ��:Inc_Dec_Alt
//��������:����/���ٺ��θ߶�
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Alt(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((altitude_disp+step)<=ALT_MAX)
        {
            altitude_disp += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((altitude_disp-step)>=ALT_MIN)
        {
            altitude_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Alt
//��������:��麣�θ߶������Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Alt(void)
{
    if (abs(altitude)%ALT_STEP)
    {
        altitude_disp = altitude = ALT_DEF;
        return(ERROR);
    }    
    else if (altitude > ALT_MAX)//̫��
    {
        altitude_disp = altitude = ALT_DEF;
        return(ERROR);
    }
    else if (ve_alm_h < ALT_MIN)//̫С
    {
        altitude_disp = altitude = ALT_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Alt
//��������:�����û�����ĺ��θ߶�,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Alt(void)
{
    if (Write_EEPROM(EEP_ALT,( uint8*)(&altitude_disp),sizeof(altitude_disp)) )
    {
        altitude = altitude_disp = ALT_DEF;
        return(ERROR);
    }
    else
    {
        altitude = altitude_disp ;
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Alt_EEP
//��������:��EEPROM�ж������θ߶�,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Alt_EEP(void)
{
    if (Read_EEPROM(EEP_ALT,( uint8*)(&altitude),sizeof(altitude)) )
    {
        altitude_disp = altitude = ALT_DEF;
        return(ERROR);
    }
    else
    {
        altitude_disp = altitude;    
        if (Check_Alt() == ERROR)
        {
            Save_Alt();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Drv_Gas
//��������:��ȡ��ǰѡ����������
//��ڲ���:��
//���ڲ���:��������
//**********************************************************************
uint8 Get_Drv_Gas(void)
{
    return(drv_gas);
}

//**********************************************************************
//����  ��:Set_Drv_Gas
//��������:������������
//��ڲ���:para--��������
//���ڲ���:��
//**********************************************************************
void Set_Drv_Gas(uint8 para)
{
    drv_gas = para;
}

//**********************************************************************
//����  ��:Save_Drv_Gas
//��������:�����������������
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Drv_Gas(void)
{
    if (Write_EEPROM(EEP_DRV_GAS,( uint8*)(&drv_gas),sizeof(drv_gas)) )
    {
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Drv_Gas_EEP
//��������:��EEPROM�ж���������������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Drv_Gas_EEP(void)
{
    if (Read_EEPROM(EEP_DRV_GAS,( uint8*)(&drv_gas),sizeof(drv_gas)) )
    {
        drv_gas = DRV_GAS_DEF;
        return(ERROR);
    }
    else
    {
        if ((drv_gas != DRV_O2)&&(drv_gas != DRV_AIR))
        {
            drv_gas = DRV_GAS_DEF;
            Save_Drv_Gas();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_O2_Inspect
//��������:��ȡ��ǰѡ����Ũ�ȼ��
//��ڲ���:��
//���ڲ���:��Ũ�ȼ��
//**********************************************************************
uint8 Get_O2_Inspect(void)
{
    return(o2_inspect);
}

//**********************************************************************
//����  ��:Set_O2_Inspect
//��������:������Ũ�ȼ��
//��ڲ���:para--��Ũ�ȼ��
//���ڲ���:��
//**********************************************************************
void Set_O2_Inspect(uint8 para)
{
    o2_inspect = para;
}

//**********************************************************************
//����  ��:Save_O2_Inspect
//��������:������Ũ�ȼ�������
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_O2_Inspect(void)
{
    if (Write_EEPROM(EEP_O2_INSPECT,( uint8*)(&o2_inspect),sizeof(o2_inspect)) )
    {
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_O2_Inspect_EEP
//��������:��EEPROM�ж�����Ũ�ȼ������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_O2_Inspect_EEP(void)
{
    if (Read_EEPROM(EEP_O2_INSPECT,( uint8*)(&o2_inspect),sizeof(o2_inspect)) )
    {
        o2_inspect = O2_INSPECT_DEF;
        return(ERROR);
    }
    else
    {
        if ((o2_inspect != ON)&&(o2_inspect != OFF))
        {
            o2_inspect = O2_INSPECT_DEF;
            Save_O2_Inspect();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Alm_Mess_D
//��������:��ȡ��ǰ������Ϣ��ʾ����
//��ڲ���:��
//���ڲ���:������Ϣ��ʾ����
//**********************************************************************
uint8 Get_Alm_Mess_D(void)
{
    return(alm_mess_disp);
}

//**********************************************************************
//����  ��:Set_O2_Inspect
//��������:���ñ�����Ϣ��ʾ����
//��ڲ���:para--������Ϣ��ʾ����
//���ڲ���:��
//**********************************************************************
void Set_Alm_Mess_D(uint8 para)
{
    alm_mess_disp = para;
}

//**********************************************************************
//����  ��:Save_Alm_Mess_D
//��������:���汨����Ϣ��ʾ���õ�����
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Alm_Mess_D(void)
{
    if (Write_EEPROM(EEP_ALM_MS_DISP,( uint8*)(&alm_mess_disp),sizeof(alm_mess_disp)) )
    {
        return(ERROR); 
    } 
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Alm_Mess_D_EEP
//��������:��EEPROM�ж���������Ϣ��ʾ��������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Alm_Mess_D_EEP(void)
{
    if (Read_EEPROM(EEP_ALM_MS_DISP,( uint8*)(&alm_mess_disp),sizeof(alm_mess_disp)) )
    {
        alm_mess_disp = ALM_MS_DISP_DEF;
        return(ERROR);
    }
    else
    {
        if ((alm_mess_disp != ON)&&(alm_mess_disp != OFF))
        {
            alm_mess_disp = ALM_MS_DISP_DEF;
            Save_Alm_Mess_D();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Pmax
//��������:��ȡ���ѹ������ֵ
//��ڲ���:��
//���ڲ���:���ѹ������ֵ
//**********************************************************************
sint32 Get_Pmax(void)
{    
    if (paw_max > PAW_MAX*PAW_MAX_MUL) return (PAW_NOT_VALID);    //ѹ��̫��,������Чֵ
    else if (paw_max < PAW_MIN*PAW_MAX_MUL) return (PAW_NOT_VALID);    //ѹ��̫С,������Чֵ
    else return (paw_max);
}

//**********************************************************************
//����  ��:Get_Pmax_Units
//��������:��ȡ���ѹ������ֵ�ĵ�λ
//��ڲ���:��
//���ڲ���:ѹ������ֵ(1--1cmH2O;10--0.1cmH2O)
//**********************************************************************
uint8 Get_Pmax_Units(void)
{
    return (PAW_MAX_MUL);
}

//**********************************************************************
//����  ��:Set_Pmax
//��������:�������ѹ���Ĳ���ֵ
//��ڲ���:p--ѹ��ֵ
//            units--��λ
//���ڲ���:OK--ѹ����Χ����
//           ERROR--ѹ����Χ����
//**********************************************************************
uint8 Set_Pmax(sint32 p,uint8 units)
{
    if (p > PAW_MAX*units)//̫��
    {
        paw_max = PAW_NOT_VALID;
        return (ERROR);
    }
    else if (p < PAW_MIN*units)//̫С
    {
        paw_max = PAW_NOT_VALID;
        return (ERROR);        
    }
    else
    {
        paw_max = p;
        return (OK);            
    }
}

//**********************************************************************
//����  ��:Get_Pavr
//��������:��ȡƽ��ѹ������ֵ
//��ڲ���:��
//���ڲ���:ƽ��ѹ������ֵ
//**********************************************************************
sint32 Get_Pavr(void)
{
    if (paw_avr_meas > PAW_MAX*PAW_AVR_MEAS_MUL) return (PAW_NOT_VALID);    //ѹ��̫��,������Чֵ
    else if (paw_avr_meas < PAW_MIN*PAW_AVR_MEAS_MUL) return (PAW_NOT_VALID);    //ѹ��̫С,������Чֵ
    else return (paw_avr_meas);
}

//**********************************************************************
//����  ��:Get_Pavr_Units
//��������:��ȡƽ��ѹ������ֵ�ĵ�λ
//��ڲ���:��
//���ڲ���:ѹ������ֵ(1--1cmH2O;10--0.1cmH2O)
//**********************************************************************
uint8 Get_Pavr_Units(void)
{
    return (PAW_AVR_MEAS_MUL);
}

//**********************************************************************
//����  ��:Set_Pavr
//��������:����ƽ��ѹ���Ĳ���ֵ
//��ڲ���:p--ѹ��ֵ
//            units--��λ
//���ڲ���:OK--ѹ����Χ����
//           ERROR--ѹ����Χ����
//**********************************************************************
uint8 Set_Pavr(sint32 p,uint8 units)
{
    if (p > PAW_MAX*units)//̫��
    {
        paw_avr_meas = PAW_NOT_VALID;
        return (ERROR);
    }
    else if (p < PAW_MIN*units)//̫С
    {
        paw_avr_meas = PAW_NOT_VALID;
        return (ERROR);        
    }
    else
    {
        paw_avr_meas = p;
        return (OK);            
    }
}

//**********************************************************************
//����  ��:Get_Paw
//��������:��ȡѹ������ֵ
//��ڲ���:��
//���ڲ���:ѹ������ֵ
//**********************************************************************
sint32 Get_Paw(void)
{
    if (airway_p > PAW_MAX*PAW_MUL) return (PAW_NOT_VALID);    //ѹ��̫��,������Чֵ
    else if (airway_p < PAW_MIN*PAW_MUL) return (PAW_NOT_VALID);    //ѹ��̫С,������Чֵ
    else return (airway_p);
}

//**********************************************************************
//����  ��:Get_Paw_Units
//��������:��ȡѹ������ֵ�ĵ�λ
//��ڲ���:��
//���ڲ���:ѹ������ֵ(1--1cmH2O;10--0.1cmH2O)
//**********************************************************************
uint8 Get_Paw_Units(void)
{
    return (PAW_MUL);
}

//**********************************************************************
//����  ��:Set_Pmax
//��������:����ѹ���Ĳ���ֵ
//��ڲ���:p--ѹ��ֵ
//            units--��λ
//���ڲ���:OK--ѹ����Χ����
//           ERROR--ѹ����Χ����
//**********************************************************************
uint8 Set_Paw(sint32 p,uint8 units)
{
    if (p > PAW_MAX*units)//̫��
    {
        airway_p = PAW_NOT_VALID;
        return (ERROR);
    }
    else if (p < PAW_MIN*units)//̫С
    {
        airway_p = PAW_NOT_VALID;
        return (ERROR);        
    }
    else
    {
        airway_p = p;
        return (OK);            
    }
}

//**********************************************************************
//����  ��:Get_Pmin
//��������:��ȡ��Сѹ������ֵ
//��ڲ���:��
//���ڲ���:��Сѹ������ֵ
//**********************************************************************
sint32 Get_Pmin(void)
{
    if (paw_min > PAW_MAX*PAW_MIN_MUL) return (PAW_NOT_VALID);    //��Сѹ��̫��,������Чֵ
    else if (paw_min < PAW_MIN*PAW_MIN_MUL) return (PAW_NOT_VALID);    //��Сѹ��̫С,������Чֵ
    else return (paw_min);
}

//**********************************************************************
//����  ��:Get_Pmin_Units
//��������:��ȡ��Сѹ������ֵ�ĵ�λ
//��ڲ���:��
//���ڲ���:��Сѹ������ֵ(1--1cmH2O;10--0.1cmH2O)
//**********************************************************************
uint8 Get_Pmin_Units(void)
{
    return (PAW_MIN_MUL);
}

//**********************************************************************
//����  ��:Set_Pmin
//��������:������Сѹ���Ĳ���ֵ
//��ڲ���:p--��Сѹ��ֵ
//            units--��λ
//���ڲ���:OK--��Сѹ����Χ����
//           ERROR--��Сѹ����Χ����
//**********************************************************************
uint8 Set_Pmin(sint32 p,uint8 units)
{
    if (p > PAW_MAX*units)//̫��
    {
        paw_min = PAW_NOT_VALID;
        return (ERROR);
    }
    else if (p < PAW_MIN*units)//̫С
    {
        paw_min = PAW_NOT_VALID;
        return (ERROR);        
    }
    else
    {
        paw_min = p;
        return (OK);            
    }
}

//**********************************************************************
//����  ��:Get_Vt
//��������:��ȡ����������ֵ
//��ڲ���:��
//���ڲ���:����������ֵ
//**********************************************************************
sint32 Get_Vt(void)
{
    if (vt_meas > VT_MEAS_MAX*vt_meas_units) return (VT_MEAS_NOT_VALID);    //������̫��,������Чֵ
    else if (vt_meas < VT_MEAS_MIN*vt_meas_units) return (VT_MEAS_NOT_VALID);    //������̫С,������Чֵ
    else return (vt_meas);
}

//**********************************************************************
//����  ��:Get_Vt_Units
//��������:��ȡ����������ֵ�ĵ�λ
//��ڲ���:��
//���ڲ���:����������ֵ(1--1mL;10--0.1mL)
//**********************************************************************
uint8 Get_Vt_Units(void)
{
    return (vt_meas_units);
}

//**********************************************************************
//����  ��:Set_Vt
//��������:���ó������Ĳ���ֵ
//��ڲ���:p--������ֵ
//            units--��λ
//���ڲ���:OK--��������Χ����
//           ERROR--��������Χ����
//**********************************************************************
uint8 Set_Vt(sint32 p,uint8 units)
{
    if (p > VT_MEAS_MAX*units)//̫��
    {
        vt_meas = VT_MEAS_NOT_VALID;
        vt_meas_units = 1;
        return (ERROR);
    }
    else if (p < VT_MEAS_MIN*units)//̫С
    {
        vt_meas = VT_MEAS_NOT_VALID;
        vt_meas_units = 1;
        return (ERROR);        
    }
    else
    {
        vt_meas = p;
        vt_meas_units = units;
        return (OK);            
    }
}

//**********************************************************************
//����  ��:Get_Rate_Meas
//��������:��ȡ����Ƶ�ʲ���ֵ
//��ڲ���:��
//���ڲ���:����Ƶ�ʲ���ֵ
//**********************************************************************
sint32 Get_Rate_Meas(void)
{
    if (br_rate_meas > RATE_MEAS_MAX) return (RATE_NOT_VALID);    //����Ƶ��̫��,������Чֵ
    else if (br_rate_meas < RATE_MEAS_MIN) return (RATE_NOT_VALID);    //����Ƶ��̫С,������Чֵ
    else return (br_rate_meas);
}


//**********************************************************************
//����  ��:Set_Rate_Meas
//��������:���ú���Ƶ�ʵĲ���ֵ
//��ڲ���:p--����Ƶ��ֵ
//���ڲ���:OK--����Ƶ�ʷ�Χ����
//           ERROR--����Ƶ�ʷ�Χ����
//**********************************************************************
uint8 Set_Rate_Meas(sint32 p)
{
    if (p > RATE_MEAS_MAX)//̫��
    {
        br_rate_meas = RATE_NOT_VALID;
        return (ERROR);
    }
    else if (p < RATE_MEAS_MIN)//̫С
    {
        br_rate_meas = RATE_NOT_VALID;
        return (ERROR);        
    }
    else
    {
        br_rate_meas = p;
        return (OK);            
    }
}

//**********************************************************************
//����  ��:Get_Ve
//��������:��ȡ����ͨ������ֵ
//��ڲ���:��
//���ڲ���:����ͨ������ֵ
//**********************************************************************
sint32 Get_Ve(void)
{
    if (insp_flux_meas > VE_MEAS_MAX*insp_flux_meas_unit) return (VE_MEAS_NOT_VALID);    //����ͨ��̫��,������Чֵ
    else if (insp_flux_meas < VE_MEAS_MIN*insp_flux_meas_unit) return (VE_MEAS_NOT_VALID);    //����ͨ��̫С,������Чֵ
    else return (insp_flux_meas);
}

//**********************************************************************
//����  ��:Get_Ve_Units
//��������:��ȡ����ͨ������ֵ�ĵ�λ
//��ڲ���:��
//���ڲ���:����ͨ������ֵ(1--1mL;10--0.1mL)
//**********************************************************************
uint8 Get_Ve_Units(void)
{
    return (insp_flux_meas_unit);
}

//**********************************************************************
//����  ��:Set_Ve
//��������:���÷���ͨ���Ĳ���ֵ
//��ڲ���:p--����ͨ��ֵ
//            units--��λ
//���ڲ���:OK--����ͨ����Χ����
//           ERROR--����ͨ����Χ����
//**********************************************************************
uint8 Set_Ve(sint32 p,uint8 units)
{
    if (p > VE_MEAS_MAX*units)//̫��
    {
        insp_flux_meas = VE_MEAS_NOT_VALID;
        insp_flux_meas_unit = 1;
        return (ERROR);
    }
    else if (p < VE_MEAS_MIN*units)//̫С
    {
        insp_flux_meas = VE_MEAS_NOT_VALID;
        insp_flux_meas_unit = 1;
        return (ERROR);        
    }
    else
    {
        insp_flux_meas = p;
        insp_flux_meas_unit = units;
        return (OK);            
    }
}

//**********************************************************************
//����  ��:Get_O2
//��������:��ȡ����Ũ�Ȳ���ֵ
//��ڲ���:��
//���ڲ���:����Ũ�Ȳ���ֵ
//**********************************************************************
sint32 Get_O2(void)
{
    if (o2_meas > O2_MEAS_MAX*o2_meas_units) return (O2_MEAS_NOT_VALID);    //����Ũ��̫��,������Чֵ
    else if (o2_meas < O2_MEAS_MIN*o2_meas_units) return (O2_MEAS_NOT_VALID);    //����Ũ��̫С,������Чֵ
    else return (o2_meas);
}

//**********************************************************************
//����  ��:Get_O2_Units
//��������:��ȡ����Ũ�Ȳ���ֵ�ĵ�λ
//��ڲ���:��
//���ڲ���:����Ũ�Ȳ���ֵ(1--1%;10--0.1%)
//**********************************************************************
uint8 Get_O2_Units(void)
{
    return (o2_meas_units);
}

//**********************************************************************
//����  ��:Set_O2
//��������:��������Ũ�ȵĲ���ֵ
//��ڲ���:p--����Ũ��ֵ
//            units--��λ
//���ڲ���:OK--����Ũ�ȷ�Χ����
//           ERROR--����Ũ�ȷ�Χ����
//**********************************************************************
uint8 Set_O2(sint32 p,uint8 units)
{
    if (p > O2_MEAS_MAX*units)//̫��
    {
        o2_meas = O2_MEAS_NOT_VALID;
        o2_meas_units = 1;
        return (ERROR);
    }
    else if (p < O2_MEAS_MIN*units)//̫С
    {
        o2_meas = O2_MEAS_NOT_VALID;
        o2_meas_units = 1;
        return (ERROR);        
    }
    else
    {
        o2_meas = p;
        o2_meas_units = units;
        return (OK);            
    }
}

//**********************************************************************
//����  ��:Get_Humor
//��������:��ȡ˳Ӧ�Բ���ֵ
//��ڲ���:��
//���ڲ���:˳Ӧ�Բ���ֵ
//**********************************************************************
sint32 Get_Humor(void)
{
    if (humor_meas > HUMOR_MAX*humor_meas_units) return (HUMOR_NOT_VALID);    //˳Ӧ��̫��,������Чֵ
    else if (humor_meas < HUMOR_MIN*humor_meas_units) return (HUMOR_NOT_VALID);    //˳Ӧ��̫С,������Чֵ
    else return (humor_meas);
}

//**********************************************************************
//����  ��:Get_Humor_Units
//��������:��ȡ˳Ӧ�Բ���ֵ�ĵ�λ
//��ڲ���:��
//���ڲ���:˳Ӧ�Բ���ֵ(1--1mL;10--0.1mL)
//**********************************************************************
uint8 Get_Humor_Units(void)
{
    return (humor_meas_units);
}

//**********************************************************************
//����  ��:Set_Humor
//��������:����˳Ӧ�ԵĲ���ֵ
//��ڲ���:p--˳Ӧ��ֵ
//            units--��λ
//���ڲ���:OK--˳Ӧ�Է�Χ����
//           ERROR--˳Ӧ�Է�Χ����
//**********************************************************************
uint8 Set_Humor(sint32 p,uint8 units)
{
    if (p > HUMOR_MAX*units)//̫��
    {
        humor_meas = HUMOR_NOT_VALID;
        humor_meas_units = 1;
        return (ERROR);
    }
    else if (p < HUMOR_MIN*units)//̫С
    {
        humor_meas = HUMOR_NOT_VALID;
        humor_meas_units = 1;
        return (ERROR);        
    }
    else
    {
        humor_meas = p;
        humor_meas_units = units;
        return (OK);            
    }
}

//**********************************************************************
//����  ��:Get_Batt_Vol
//��������:��ȡ���صĵ�ѹ
//��ڲ���:��
//���ڲ���:��ѹ(��λ:0.01V)
//**********************************************************************
sint32 Get_Batt_Vol(void)
{
    return(batt_v);
}

//**********************************************************************
//����  ��:Log_Batt_Vol
//��������:��¼���صĵ�ѹ
//��ڲ���:��ѹ(��λ:0.01V)
//���ڲ���:��
//***********************************************************************
void Log_Batt_Vol(sint32 vol)
{
    batt_v = vol;
}

//**********************************************************************
//����  ��:Get_Batt_Charge_I
//��������:��ȡ���س�����
//��ڲ���:��
//���ڲ���:����(��λ:ma)
//**********************************************************************
sint32 Get_Batt_Charge_I(void)
{
    return(batt_charge_i);
}

//**********************************************************************
//����  ��:Log_Batt_Charge_I
//��������:��¼���صĵ�ѹ
//��ڲ���:����(��λ:ma)
//���ڲ���:��
//***********************************************************************
void Log_Batt_Charge_I(sint32 i)
{
    batt_v = i;
}

//**********************************************************************
//����  ��:Get_Key_Sound_Switch
//��������:��ǰ�İ������Ƿ��
//��ڲ���:��
//���ڲ���:ON--��;OFF--�ر�
//**********************************************************************
uint8 Get_Key_Sound_Switch(void)
{
    return(key_sound_switch);
}

//**********************************************************************
//����  ��:Set_Key_Sound_Switch
//��������:���ð������Ŀ�/��
//��ڲ���:para--��������/��
//���ڲ���:��
//**********************************************************************
void Set_Key_Sound_Switch(uint8 para)
{
    key_sound_switch = para;
}

//**********************************************************************
//����  ��:Save_Key_Sound_Switch
//��������:���水������/�ص�����
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Key_Sound_Switch(void)
{
    if (Write_EEPROM(EEP_KEY_SOUND_SW,( uint8*)(&key_sound_switch),sizeof(key_sound_switch)) )
    {
        key_sound_switch = ON;
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Key_Sound_Switch_EEP
//��������:��EEPROM�ж�����������������ֵ,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Key_Sound_Switch_EEP(void)
{
    if (Read_EEPROM(EEP_KEY_SOUND_SW,( uint8*)(&key_sound_switch),sizeof(key_sound_switch)) )
    {
        key_sound_switch = ON;
        return(ERROR);
    }
    else
    {
        if ((ON != key_sound_switch)&&(OFF != key_sound_switch))
        {//����ֵ��һ������������
            key_sound_switch = ON;
            Save_Key_Sound_Switch();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Vt_Filter_Const
//��������:��ȡ�������˲���������ֵ
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
uint8 Get_Vt_Filter_Const(void)
{
    if (vte_filter_const > VTE_FC_MAX)    vte_filter_const = VTE_FC_DEFAULT;
    if (vte_filter_const < VTE_FC_MIN)    vte_filter_const = VTE_FC_DEFAULT;
    return(vte_filter_const);
}

//**********************************************************************
//����  ��:Inc_Dec_Vt_Filt_C
//��������:����/���ٳ������˲�����
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Vt_Filt_C(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((vte_filter_const+step)<=VTE_FC_MAX)
        {
            vte_filter_const += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((vte_filter_const-step)>=VTE_FC_MIN)
        {
            vte_filter_const -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Vte_Filt_C
//��������:��鳱�����˲����������Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Vte_Filt_C(void)
{
    if (vte_filter_const > VTE_FC_MAX)//̫��
    {
        vte_filter_const = VTE_FC_DEFAULT;
        return(ERROR);
    }
    else if (vte_filter_const < VTE_FC_MIN)//̫С
    {
        vte_filter_const = VTE_FC_DEFAULT;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Vt_Filt_C
//��������:���泱�����˲�����,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Vt_Filt_C(void)
{
    if (Write_EEPROM(EEP_VT_FILT_CONST,( uint8*)(&vte_filter_const),sizeof(vte_filter_const)) )
    {
        vte_filter_const = VTE_FC_DEFAULT;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Vt_Filt_C_EEP
//��������:��EEPROM�ж����������˲�����,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Vt_Filt_C_EEP(void)
{
    if (Read_EEPROM(EEP_VT_FILT_CONST,( uint8*)(&vte_filter_const),sizeof(vte_filter_const)) )
    {
        vte_filter_const = VTE_FC_DEFAULT;
        return(ERROR);
    }
    else
    {
        if (Check_Vte_Filt_C() == ERROR)
        {
            Save_Vt_Filt_C();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_O2_Flash_Cyc
//��������:��ȡ��Ũ��ˢ����������ֵ
//��ڲ���:��
//���ڲ���:����ֵ
//**********************************************************************
uint8 Get_O2_Flash_Cyc(void)
{
    if (o2_flash_cycle > O2_FC_MAX)    o2_flash_cycle = O2_FC_DEFAULT;
    if (o2_flash_cycle < O2_FC_MIN)    o2_flash_cycle = O2_FC_DEFAULT;
    return(o2_flash_cycle);
}

//**********************************************************************
//����  ��:Inc_Dec_O2_Flash_Cyc
//��������:����/������Ũ��ˢ������
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_O2_Flash_Cyc(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((o2_flash_cycle+step)<=O2_FC_MAX)
        {
            o2_flash_cycle += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((o2_flash_cycle-step)>=O2_FC_MIN)
        {
            o2_flash_cycle -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_O2_Flash_Cyc
//��������:�����Ũ��ˢ�����������Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_O2_Flash_Cyc(void)
{
    if (o2_flash_cycle > O2_FC_MAX)//̫��
    {
        o2_flash_cycle = O2_FC_DEFAULT;
        return(ERROR);
    }
    else if (o2_flash_cycle < O2_FC_MIN)//̫С
    {
        o2_flash_cycle = O2_FC_DEFAULT;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_O2_Flash_Cyc
//��������:������Ũ��ˢ������,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_O2_Flash_Cyc(void)
{
    if (Write_EEPROM(EEP_O2_FLASH_CYC,( uint8*)(&o2_flash_cycle),sizeof(o2_flash_cycle)) )
    {
        o2_flash_cycle = O2_FC_DEFAULT;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_O2_Flash_Cyc_EEP
//��������:��EEPROM�ж�����Ũ��ˢ������,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_O2_Flash_Cyc_EEP(void)
{
    if (Read_EEPROM(EEP_O2_FLASH_CYC,( uint8*)(&o2_flash_cycle),sizeof(o2_flash_cycle)) )
    {
        o2_flash_cycle = O2_FC_DEFAULT;
        return(ERROR);
    }
    else
    {
        if (Check_O2_Flash_Cyc() == ERROR)
        {
            Save_O2_Flash_Cyc();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Pl_Comp
//��������:��ȡѹ�����Ʋ�������ֵ
//��ڲ���:��
//���ڲ���:����ֵ(0.1cmH2O)
//**********************************************************************
sint16 Get_Pl_Comp(void)
{
    if (plimit_comp > PLIMIT_COMP_MAX)    plimit_comp = PLIMIT_COMP_DEFAULT;
    if (plimit_comp < PLIMIT_COMP_MIN)    plimit_comp = PLIMIT_COMP_DEFAULT;
    return(plimit_comp);
}

//**********************************************************************
//����  ��:Inc_Dec_Pl_Comp
//��������:����/����ѹ�����Ʋ���
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Pl_Comp(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((plimit_comp+step)<=PLIMIT_COMP_MAX)
        {
            plimit_comp += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((plimit_comp-step)>=PLIMIT_COMP_MIN)
        {
            plimit_comp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Pl_Comp
//��������:���ѹ�����Ʋ��������Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Pl_Comp(void)
{
    if (plimit_comp > PLIMIT_COMP_MAX)//̫��
    {
        plimit_comp = PLIMIT_COMP_DEFAULT;
        return(ERROR);
    }
    else if (plimit_comp < PLIMIT_COMP_MIN)//̫С
    {
        plimit_comp = PLIMIT_COMP_DEFAULT;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Pl_Comp
//��������:����ѹ�����Ʋ���,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Pl_Comp(void)
{
    if (Write_EEPROM(EEP_PL_COMP,( uint8*)(&plimit_comp),sizeof(plimit_comp)) )
    {
        plimit_comp = PLIMIT_COMP_DEFAULT;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Pl_Comp_EEP
//��������:��EEPROM�ж���ѹ�����Ʋ���,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Pl_Comp_EEP(void)
{
    if (Read_EEPROM(EEP_PL_COMP,( uint8*)(&plimit_comp),sizeof(plimit_comp)) )
    {
        plimit_comp = PLIMIT_COMP_DEFAULT;
        return(ERROR);
    }
    else
    {
        if (Check_Pl_Comp() == ERROR)//��Χ����ȷ��д��Ĭ��ֵ
        {
            Save_Pl_Comp();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Get_Paw_No_Raise
//��������:��ȡѹ����ѹ����������ֵ
//��ڲ���:��
//���ڲ���:����ֵ(0.1cmH2O)
//**********************************************************************
sint32 Get_Paw_No_Raise(void)
{
    if (paw_no_raise_threshold > PAW_NR_MAX)    paw_no_raise_threshold = PAW_NR_DEFAULT;
    if (paw_no_raise_threshold < PAW_NR_MIN)    paw_no_raise_threshold = PAW_NR_DEFAULT;
    return(paw_no_raise_threshold);
}

//**********************************************************************
//����  ��:Inc_Dec_Paw_No_R
//��������:����/���ٹ�ѹ������
//��ڲ���:dir--�޸ķ���
///            step--����
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Inc_Dec_Paw_No_R(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((paw_no_raise_threshold+step)<=PAW_NR_MAX)
        {
            paw_no_raise_threshold += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((paw_no_raise_threshold-step)>=PAW_NR_MIN)
        {
            paw_no_raise_threshold -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//����  ��:Check_Paw_No_R
//��������:����ѹ�����������Ƿ���ȷ,�������ȷ,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Check_Paw_No_R(void)
{
    if (paw_no_raise_threshold > PAW_NR_MAX)//̫��
    {
        paw_no_raise_threshold = PAW_NR_DEFAULT;
        return(ERROR);
    }
    else if (paw_no_raise_threshold < PAW_NR_MIN)//̫С
    {
        paw_no_raise_threshold = PAW_NR_DEFAULT;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Save_Paw_No_R
//��������:�����ѹ������,�������ɹ�������ֵ��Ч,����ʹ��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Paw_No_R(void)
{
    if (Write_EEPROM(EEP_PAW_NO_R,( uint8*)(&paw_no_raise_threshold),sizeof(paw_no_raise_threshold)) )
    {
        paw_no_raise_threshold = PAW_NR_DEFAULT;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Pl_Comp_EEP
//��������:��EEPROM�ж�����ѹ������,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Paw_No_R_EEP(void)
{
    if (Read_EEPROM(EEP_PAW_NO_R,( uint8*)(&paw_no_raise_threshold),sizeof(paw_no_raise_threshold)) )
    {
        paw_no_raise_threshold = PAW_NR_DEFAULT;
        return(ERROR);
    }
    else
    {
        if (Check_Paw_No_R() == ERROR)
        {
            Save_Paw_No_R();
        }
        return(OK);
    }
}