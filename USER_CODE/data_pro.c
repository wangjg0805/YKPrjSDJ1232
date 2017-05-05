//**********************************************************************
//�ļ���:data_pro.c
//�����б�
//1.���㳱����:Cal_Vt
//2.����ܵ�ѹ��:Cal_Paw
//3.��������������ѹ:Cal_O2_Sen_Vol
//4.��������Ũ��:Cal_O2_Thickness
//5.�������ص�ѹ:Cal_Batt_V
//6.���������:Cal_Batt_I
//7.����ο���ѹ:Cal_Vref_Test
//8.��ADֵ�����ѹֵ:Cal_Vol
//9.����ѹ������������:Update_Paw_Curve
//10.���ѹ���궨���ADֵ:Check_Pipeline_P_AD
//11.���ѹ������:Check_EEP_Pipeline_P
//12.������������궨���ADֵ:Check_O2_AD
//13.�����������������������:Check_O2_Curve
//14.��������������ı궨����:Check_EEP_O2
//15.��鳱�����궨���Ȧ��:Check_Vt_Laps
//16.��鳱��������:Check_EEP_Vte
//17.����������������:Update_O2_Sen_Curve
//18.���³���������������:Update_Vt_Cur
//19.��EEPROM�ж�ȡ�������ı궨���ݲ����:Read_Vt_Dem_EEP
//20.��EEPROM�ж�ȡѹ���������ı궨���ݲ����:Read_Paw_Dem_EEP
//21.��EEPROM�ж�ȡ�����������ı궨���ݲ����:Read_O2_Dem_EEP
//**********************************************************************
#include "data_pro.h"
#include "global.h"
#include "eeprom.h"
#include "type_def.h"
#include "stdio.h"
//**********************************************************************
//����  ��:Cal_Vt
//��������:���㳱����
//��ڲ���:lap--����Ȧ��
//            *vt_units--���صĳ�������λ(1--1ml;10--0.1ml)
//���ڲ���:������(��λ:1ml)
//**********************************************************************
sint32 Cal_Vt(sint32 lap,uint8 *vt_units)
{
    sint32 vt,di;
    if (NULL == vt_units) return(VT_MEAS_NOT_VALID);
    if (machine_mode != MACHINE_DEMARC_MODE)
    {//û�д��ڱ궨ģʽ��
        if(flag_eep_vte_error) return(VT_MEAS_NOT_VALID);    
    }
    *vt_units = 1;
    if (lap == 0) return(0);
    
    for (di=1; di<VT_DEMAR_POINTS; di++)
    {
        if (lap < vt_lap_tab[di])
        {
            break;
        }    
    }
    if (VT_DEMAR_POINTS == di) di--;//���һ���߶�
    di--;
    vt = vt_demar_tab[di]*8192+vt_k_tab[di]*(lap-vt_lap_tab[di]);
    vt = (vt+4096)/8192;
    if(vt < 0) return(0);
    if(vt > 2000) return(2000);    
    return (vt);
}

//**********************************************************************
//����  ��:Cal_Paw
//��������:����ܵ�ѹ��ֵ
//��ڲ���:ad--�ܵ�ѹ��ADֵ
//���ڲ���:�ܵ�ѹ��(�Ŵ���:PAW_MUL)
//**********************************************************************
sint32 Cal_Paw(uint16 ad)
{
    uint8 di;
    sint32 paw;
    if (machine_mode != MACHINE_DEMARC_MODE)
    {//û�д��ڱ궨ģʽ��    
        if (flag_eep_p_error) return (PAW_NOT_VALID);    //��ѹ�궨���ݴ���
    }
    for (di=1; di<PAW_DEMAR_POINTS; di++)
    {//Ѱ��ƥ���߶�(�ӵ�һ���յ㿪ʼ��)
        if (ad<paw_ad_tab[di])
        {
            break;
        }
    }
    if (PAW_DEMAR_POINTS == di) di--;//���һ���߶�
    di--;

    paw = paw_demar_tab[di]*8192+paw_k_tab[di]*(ad+paw_zero_ad-paw_ad_tab[di]);
    paw = (paw*PAW_MUL+4096)/8192;    
    return (paw);
}

//**********************************************************************
//����  ��:Cal_O2_Sen_Vol
//��������:�������������ĵ�ѹ
//��ڲ���:ad--����������ѹADֵ
//���ڲ���:����������ѹ(��λ:0.1mV)
//**********************************************************************
sint32 Cal_O2_Sen_Vol(uint16 ad)
{
    sint32  volt;
    //volt = (ad * (AD_VREF_V * 100 / O2_SENSOR_V_AMP)) / 1023;
    volt = (ad * (AD_VREF_V * 100 / O2_SENSOR_V_AMP)) / 1024;
    //if(volt > 999) return(999);
    return(volt);
}

//**********************************************************************
//����  ��:Cal_O2_Thickness
//��������:��������Ũ��
//��ڲ���:ad--������������ѹADֵ
//            *o2_units--���ص�����Ũ�ȵ�λ(1--1%;10--0.1%)
//���ڲ���:����Ũ��(��λ:0.1%)
//**********************************************************************
sint32 Cal_O2_Thickness(uint16 ad,uint8* o2_units)
{
    sint32 o2_thick;
    if (machine_mode != MACHINE_DEMARC_MODE)
    {//û�д��ڱ궨ģʽ��    
        if (flag_eep_o2_error) return (O2_MEAS_NOT_VALID);//�궨���ݴ���
    }
    *o2_units = 10;
    o2_thick = o2_demar_tab[0]*8192+(ad-o2_ad_tab[0])*o2_k;
    if (o2_thick<0) return(0);
    o2_thick = (o2_thick*10+4096)/8192;
    return (o2_thick);
}

//**********************************************************************
//����  ��:Cal_Batt_V
//��������:�������ص�ѹ
//��ڲ���:ad--���ص�ѹADֵ
//���ڲ���:���ص�ѹ(��λ:0.01V)
//**********************************************************************
sint32 Cal_Batt_V(uint16 ad)
{
    sint32 volt;
    
    volt = (ad * ((AD_VREF_V * BATT_V_AMP) / 10)) / 1024;
    //if(volt > 999) return(999);
    return(volt);
}


//**********************************************************************
//����  ��:Cal_Batt_I
//��������:�������صĳ�����
//��ڲ���:ad--���صĵ���ADֵ
//���ڲ���:���س�����(��λ:1mA)
//**********************************************************************
sint32 Cal_Batt_I(uint16 ad)
{    
    sint32 ci;
    ci = (ad * ((AD_VREF_V * 1000) / (BATT_I_AMP * BATT_I_RES))) / 1024
            - (1000 * BATT_I0_V) / (BATT_I_AMP * BATT_I_RES);
    //if(ci < 0) return(0);
   //if(ci > 999) return(999);
    return(ci);
}

//**********************************************************************
//����  ��:Cal_Vref_Test
//��������:����ο���ѹֵ
//��ڲ���:ad--�ο���ѹ��ADֵ
//���ڲ���:�ο���ѹ�Ĳ���ֵ(��λ:0.01V)
//**********************************************************************
sint32 Cal_Vref_Test(uint16 ad)
{
    sint32 volt;
    volt = (ad * AD_VREF_V) / 1024;
    //if(volt > 999) return(999);
    return(volt);
}

//**********************************************************************
//����  ��:Cal_Vol
//��������:�����ѹֵ
//��ڲ���:ad--��ѹADֵ
//���ڲ���:��ѹ�Ĳ���ֵ(��λ:0.01V)
//**********************************************************************
sint32 Cal_Vol(uint16 ad)
{
    sint32 volt;
    volt = (ad * AD_VREF_V) / 1024;
    return(volt);
}

//**********************************************************************
//����  ��:Update_Paw_Curve
//��������:����ѹ������������,�������ߵ�k,����궨��ADֵ
//��ڲ���:point--���µ�
//            ad--���µ��ADֵ
//���ڲ���:��
//**********************************************************************
void Update_Paw_Curve(uint16 ad,sint8 point)
{
    sint32 di;
    if (point < 0 ) return;    //�޸õ��򷵻�
    if (point > PAW_DEMAR_POINTS-1) return;    //�޸õ��򷵻�
    
    paw_ad_tab[point] = ad;    //���±궨���ADֵ
    if (point != PAW_DEMAR_POINTS-1)    //�������һ��
    {
        di = paw_ad_tab[point+1]-paw_ad_tab[point];
        if (di)
        {//����б�ʷŴ�8192��
            paw_k_tab[point] = (paw_demar_tab[point+1]-paw_demar_tab[point])*8192/di;
        }
        else        //��Ч
        {
            paw_k_tab[point] = 0;
        }                        
    }
    
    if (point)    //���ǵ�һ��
    {
        di = paw_ad_tab[point]-paw_ad_tab[point-1];
        if (di) //di!=0
        {//����б�ʷŴ�8192��        
            paw_k_tab[point-1] = (paw_demar_tab[point]-paw_demar_tab[point-1])*8192/di;    
        }
        else
        {
            paw_k_tab[point-1] = 0;
        }
    }
    paw_ad_filt_band = (paw_ad_tab[PAW_DEMAR_POINTS-1] - paw_ad_tab[0]) / 50 + 1;
}

//**********************************************************************
//�������ƣ�Check_Pipeline_P_AD
//�������ܣ��ж�ѹ���궨��ADֵ�Ƿ����
//��ڲ�����point--ѹ����
//            ad--�Ƚϵ�ADֵ
//���ڲ���������ֽ�:0 ��ȷ, 1 ̫С, 2̫��,3�޶�Ӧѹ����;��2�ֽ�Ϊ��ȷADֵ
//**********************************************************************
sint32 Check_Pipeline_P_AD(uint8 point,uint16 ad)
{
    /*switch (point)
    {
        case 0:
            if(ad < DEMA_MIN_0_P_AD) return(0x01000000|DEMA_MIN_0_P_AD);
            if(ad > DEMA_MAX_0_P_AD) return(0x02000000|DEMA_MAX_0_P_AD);
            return(ad);
        case 1:
            if (ad < DEMA_MIN_50_P_AD) return(0x01000000|DEMA_MIN_50_P_AD);
            if (ad > DEMA_MAX_50_P_AD) return(0x02000000|DEMA_MAX_50_P_AD);
            return(ad);    
        default:
        //return(0x03000000);
            return(ad);
    }*/
    if (ad < paw_ad_min_buf[point])    return(0x01000000|paw_ad_min_buf[point]);
    if (ad > paw_ad_max_buf[point]) return(0x02000000|paw_ad_min_buf[point]);
    return (ad);
}

/****************************************************************************
* ���ƣ�Check_EEP_Pipeline_P()
* ���ܣ��ж�EEPROM��ѹ���궨�����Ƿ���ȷ
* ��ڲ�������
* ���ڲ�����0 ��ȷ, ����Ϊ�����
****************************************************************************/
 uint8 Check_EEP_Pipeline_P(void)
{
    /* uint8 res;
    res = Check_Pipeline_P_AD(0,paw_ad_tab[0])>>24;
    if(res) return(res);
    res = Check_Pipeline_P_AD(1,paw_ad_tab[1])>>24;
    if(res) return(res);
    if(paw_ad_tab[1] - paw_ad_tab[0] < DEMA_MIN_50_P_AD - DEMA_MIN_0_P_AD)
    {
        return(5);
    }
    if(paw_ad_tab[1] - paw_ad_tab[0] > 1023 - paw_ad_tab[1])
    {
        return(6);
    }*/
    if (Check_Sen_Curve_AD(paw_ad_tab,PAW_DEMAR_POINTS,paw_ad_min_buf,paw_ad_max_buf) == ERROR) return(1);
    return(0);
}

//**********************************************************************
//�������ƣ�Check_O2_AD
//�������ܣ��ж�����ADֵ�Ƿ����
//��ڲ�����point--������
//            ad--�Ƚϵ�ADֵ
//���ڲ���������ֽ�:0 ��ȷ, 1 ̫С, 2̫��,3�޶�Ӧ��;��2�ֽ�Ϊ��ȷADֵ
//**********************************************************************
sint32 Check_O2_AD(uint8 point,uint16 ad)
{
    switch (point)
    {
        case 0:
            if(ad < DEMA_MIN_21_O2_AD) return(0x01000000|DEMA_MIN_21_O2_AD);
            if(ad > DEMA_MAX_21_O2_AD) return(0x02000000|DEMA_MAX_21_O2_AD);
            return(ad);
        case 1:
            if (ad < DEMA_MIN_100_O2_AD) return(0x01000000|DEMA_MIN_100_O2_AD);
            if (ad > DEMA_MAX_100_O2_AD) return(0x02000000|DEMA_MAX_100_O2_AD);
            return(ad);    
        default:return(0x03000000);
    }
}

/****************************************************************************
* ���ƣ�Check_O2_Curve()
* ���ܣ��ж��������������Ƿ����Ҫ��(100%��21%Ϊ5����ϵ)
* ��ڲ�������
* ���ڲ�����0 ����, 1 б��̫С, 2 б��̫��
****************************************************************************/
 uint8 Check_O2_Curve(void)
{
    if(o2_ad_tab[1] < o2_ad_tab[0] * 4) return(1);
    if(o2_ad_tab[1] > o2_ad_tab[0] * 6) return(2);
    return(0);
}

/****************************************************************************
* ���ƣ�Check_EEP_O2()
* ���ܣ��ж�EEPROM����Ũ�ȱ궨�����Ƿ���ȷ
* ��ڲ�������
* ���ڲ�����0 ��ȷ, ����Ϊ�����
****************************************************************************/
 uint8 Check_EEP_O2(void)
{
     uint8 res;
    res = Check_O2_AD(0,o2_ad_tab[0])>>24;
    if(res) return(res);
    res = Check_O2_AD(1,o2_ad_tab[1])>>24;
    if(res) return(res + 2);
    res = Check_O2_Curve();
    if(res) return(res + 4);
    return(0);
}

//**********************************************************************
//�������ƣ�Check_Vt_Laps
//�������ܣ��жϳ������궨���Ȧ���Ƿ����Ԥ�跶Χ
//��ڲ�����point--�궨��
//           lps--Ȧ��
//���ڲ���������ֽ�:0 ��ȷ, 1 ̫С, 2̫��,3�޶�Ӧ��;��2�ֽ�Ϊ��ȷlps
//**********************************************************************
sint32 Check_Vt_Laps(uint8 point,sint32 lps)
{
    switch (point)
    {
        case 0://50mL��
            if(lps < DEMA_MIN_50_LAP) return(0x01000000|DEMA_MIN_50_LAP);
            if(lps > DEMA_MAX_50_LAP) return(0x02000000|DEMA_MAX_50_LAP);
            return(lps);
        case 1://100mL��
            if (lps < DEMA_MIN_100_LAP) return(0x01000000|DEMA_MIN_100_LAP);
            if (lps > DEMA_MAX_100_LAP) return(0x02000000|DEMA_MAX_100_LAP);
            return(lps);
        case 2://200mL��
            if (lps < DEMA_MIN_200_LAP) return(0x01000000|DEMA_MIN_200_LAP);
            if (lps > DEMA_MAX_200_LAP) return(0x02000000|DEMA_MAX_200_LAP);
            return(lps);
        case 3://400mL��
            if (lps < DEMA_MIN_400_LAP) return(0x01000000|DEMA_MIN_400_LAP);
            if (lps > DEMA_MAX_400_LAP) return(0x02000000|DEMA_MAX_400_LAP);
            return(lps);
        case 4://800mL��
            if (lps < DEMA_MIN_800_LAP) return(0x01000000|DEMA_MIN_800_LAP);
            if (lps > DEMA_MAX_800_LAP) return(0x02000000|DEMA_MAX_800_LAP);
            return(lps);                                                    
        default:return(0x03000000);
    }
}

/****************************************************************************
* ���ƣ�Check_EEP_Vte()
* ���ܣ��ж�EEPROM�г������궨�����Ƿ���ȷ
* ��ڲ�������
* ���ڲ�����0 ��ȷ, ����Ϊ�����
****************************************************************************/
 uint8 Check_EEP_Vte(void)
{
     uint8 res;
    sint32 di;
    res = Check_Vt_Laps(0,vt_lap_tab[0])>>24;
    if(res) return(res);
    res = Check_Vt_Laps(1,vt_lap_tab[1])>>24;
    if(res) return(res + 2);
    res = Check_Vt_Laps(2,vt_lap_tab[2])>>24;
    if(res) return(res + 4);
    res = Check_Vt_Laps(3,vt_lap_tab[3])>>24;
    if(res) return(res + 6);
    res = Check_Vt_Laps(4,vt_lap_tab[4])>>24;
    if(res) return(res + 8);
    //100ml��50mlȦ����ֵ��(15,60)
    di = vt_lap_tab[1] - vt_lap_tab[0];
    if(di < 15) return(11);
    if(di > 60) return(12);
    //200ml��100mlȦ����ֵ��(30,100)
    di = vt_lap_tab[2] - vt_lap_tab[1];
    if(di < 30) return(13);
    if(di > 100) return(14);
    //400ml��200mlȦ����ֵ��(50,150)
    di = vt_lap_tab[3] - vt_lap_tab[2];
    if(di < 50) return(15);
    if(di > 150) return(16);
    //800ml��400mlȦ����ֵ��(100,300)
    di = vt_lap_tab[4] - vt_lap_tab[3];
    if(di < 100) return(17);
    if(di > 300) return(18);
    return(0);
}

//**********************************************************************
//����  ��:Update_O2_Sen_Curve
//��������:������������������,�������ߵ�k,����궨��ADֵ
//��ڲ���:point--���µ�
//            ad--���µ��ADֵ
//���ڲ���:��
//**********************************************************************
void Update_O2_Sen_Curve(uint16 ad,sint8 point)
{
    sint32 di;
    if (point < 0) return;
    if (point > 1) return;
    
    o2_ad_tab[point] = ad;//���±궨���ADֵ
    di = o2_ad_tab[1]-o2_ad_tab[0];
    if (di)
    {
        o2_k = (o2_demar_tab[1]-o2_demar_tab[0])*8192/di;
    }
    else
    {
        o2_k = 0;
    }
}

//**********************************************************************
//����  ��:Update_Vt_Cur
//��������:���³���������,����궨���Ȧ��,��������б��
//��ڲ���:laps--Ȧ��;point--�궨��
//���ڲ���:�� 
//**********************************************************************
void Update_Vt_Cur(sint32 laps,sint8 point)
{
    sint32 di;
    
    if (point<0) return;
    if (point> VT_DEMAR_POINTS-1) return;
    
    vt_lap_tab[point] = laps;    //���±궨���Ȧ��
    
    if (point != VT_DEMAR_POINTS-1)    //�������һ��,���º�һ������
    {
        di = vt_lap_tab[point+1]-vt_lap_tab[point];
        if (di)
        {
            vt_k_tab[point] = (vt_demar_tab[point+1]-vt_demar_tab[point])*8192/di;
        }
        else
        {
            vt_k_tab[point] = 0;
        }    
    }
    
    if (point)    //���ǵ�һ��,����ǰһ�ε�����
    {
        di = vt_lap_tab[point]-vt_lap_tab[point-1];
        if (di)
        {
            vt_k_tab[point-1] = (vt_demar_tab[point]-vt_demar_tab[point-1])*8192/di;
        }
        else
        {
            vt_k_tab[point-1] = 0;
        }            
    }
}

//**********************************************************************
//����  ��:Read_Vt_Dem_EEP
//��������:��EEPROM��ȡ��VT�ı궨����,������VT����������
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Read_Vt_Dem_EEP(void)
{
    sint32 di;
    
    flag_eep_vte_error = TRUE;
    Read_EEPROM(EEP_VTE,( uint8 *)vt_lap_tab,sizeof(vt_lap_tab));//��ȡ�궨����
    if ((Check_EEP_Vte()==0)&&(!Check_EEP_Wr_ERR()))
    {
        for (di=1; di<VT_DEMAR_POINTS;di+=2)
        {//������������
            Update_Vt_Cur(vt_lap_tab[di],di);
        }
        flag_eep_vte_error = FALSE;
    }    
}

//**********************************************************************
//����  ��:Read_Paw_Dem_EEP
//��������:��EEPROM��ȡ��Paw�ı궨����,������Paw����������
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Read_Paw_Dem_EEP(void)
{
    sint32 di;
    flag_eep_p_error = TRUE;
    Read_EEPROM(EEP_PIPELINE_P,( uint8 *)paw_ad_tab,sizeof(paw_ad_tab));//��ȡ�궨����
    if ((!Check_EEP_Wr_ERR())&&(Check_EEP_Pipeline_P()==0))    //��������
    {
        for (di=1; di<PAW_DEMAR_POINTS;di+=2)
        {//������������
            Update_Paw_Curve(paw_ad_tab[di],di);
        }
        flag_eep_p_error = FALSE;
    }
    Read_EEPROM(EEP_ZERO_P,( uint8 *)(&paw_zero_ad),sizeof(paw_zero_ad));//��ȡУ������
    if((paw_zero_ad > MAX_P_CAL0_P) || (paw_zero_ad < MIN_P_CAL0_P))
    {//��λѹ������Χ
        paw_zero_ad = 0;
        Write_EEPROM_2B(EEP_ZERO_P, paw_zero_ad);
    }        
}

//**********************************************************************
//����  ��:Read_O2_Dem_EEP
//��������:��EEPROM��ȡ��O2�ı궨����,������O2����������
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Read_O2_Dem_EEP(void)
{
    sint32 di;
    flag_eep_o2_error = TRUE;
    Read_EEPROM(EEP_O2,( uint8 *)o2_ad_tab,sizeof(o2_ad_tab));//��ȡ�궨����
    if ((!Check_EEP_Wr_ERR())&&(Check_EEP_O2()==0))
    {
        for (di=1; di<2;di+=2)
        {//������������
            Update_O2_Sen_Curve(o2_ad_tab[di],di);
        }
        flag_eep_o2_error = FALSE;
    }
}

/****************************************************************************
* ������: Check_Curve_Data_Valid
* ����: ���궨���ߵ������Ƿ���Ч
* ��ڲ���: *cdp����ָ��,*ydpY������,len����,min��Сֵ,max���ֵ,
*             reldt���ڱ궨�����Сб��(����100��)
* ���ڲ���: 1��Ч,0��Ч
****************************************************************************/
uint8 Check_Curve_Data_Valid(sint16 *cdp, sint16 *ydp, sint16 len, sint16 min, sint16 max, sint16 reldt)
{
    if(cdp[0]<min) return(FALSE);
    if(cdp[len-1]>max) return(FALSE);
    for(;len>1;len--)
    {
        if((cdp[1]-cdp[0])*100<(ydp[1]-ydp[0])*reldt) return(FALSE);
        cdp++;
        ydp++;
    }
    return(TRUE);
}

//**********************************************************************
//����  ��:Check_Sen_Curve_AD
//��������:��鴫��������
//��ڲ���:len--�궨�����
//            ad_buf--�궨���AD������
//            ad_min_buf--�궨��AD��Сֵ
//            ad_max_buf--�궨��AD���ֵ
//���ڲ���:OK--������������
//            ERROR--�������ݴ���
//**********************************************************************
uint8 Check_Sen_Curve_AD(uint16 *ad_buf,uint16 len,const uint16 *ad_min_buf,const uint16 *ad_max_buf)
{
    uint16 i;
    for (i=0; i<len; i++)
    {
        //���ADֵ
        if (ad_buf[i] < ad_min_buf[i])//����
        {
            return (ERROR);
        }
        if (ad_buf[i] > ad_max_buf[i])//����
        {
            return (ERROR);
        }
    }
    return(OK);
}

//**********************************************************************
//����  ��:Check_Sen_Curve_K
//��������:��鴫��������
//��ڲ���:len--�궨�����
//            k_buf--���ߵ�б�ʻ�����
//            k_min_buf--б����Сֵ
//            k_max_buf--б�����ֵ
//���ڲ���:OK--������������
//            ERROR--�������ݴ���
//**********************************************************************
uint8 Check_Sen_Curve_K(uint16 len,sint32 *k_buf,sint32* k_min_buf,sint32 *k_max_buf)
{
    uint16 i;
    for (i=0; i<len; i++)
    {
        if (k_buf[i] < k_min_buf[i])//����
        {
            return (ERROR);
        }
        if (k_buf[i] > k_max_buf[i])//����
        {
            return (ERROR);
        }            
    }
    return(OK);
}

