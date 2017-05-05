//**********************************************************************
//�ļ���:ad_acquire.c
//�����б�:
//1.��ʼ����ʱ��1:         Init_Timer1  ������û��ʹ��
//2.���λ��ⲿ�ж�2:       Init_EINT2
//3.CPLD ԭʼ�����ۼӺ���  epm7064s_filter_raw
//4.�����жϴ���:          FIQ_Exception(��ȡԭʼ����)
//5.�����ۼӺ��ԭʼ����:  Get_Raw
//**********************************************************************
#include "ad_acquire.h"
#include "config.h"
#include "port.h"
#include "global.h"
#include "type_def.h"
#include "stdlib.h"
#include "filter.h"

#include "ds18b20.h"
#include "AD7190_IO.h"

#define   SAMPLE_CH_SIZE      4

static  uint32 raw[SAMPLE_CH_SIZE];
static  uint32 ddlv_index=0;

static  uint32 raw_ddlv[64];

//**********************************************************************
//��������:Init_EINT2()
//��������:��ʼ���ⲿ�ж�2,���ڼ�����ַ���
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Init_EINT2(void)
{
  
    EXTMODE = 0x04;                  //����EINT2�ж�Ϊ���ش���
    EXTPOLAR = 0x00;                 //�͵�ƽ���������ش���
   
    EXTINT = 0x04;                   //���EINT2�жϱ�־
    VICIntSelect |= 0x00010000;      //EINT2�ж���ΪFIQ
    VICVectAddr16 = (uint32)FIQ_Exception;
    VICVectPri16 = 0x0;
    VICIntEnable |= 0x00010000;     //ʹ��EINT2�ж�*/

}

////////////////////////////////////////////////////////////
void  AD7190_Channel_Switch(void)
   {
     
     if((TRUE ==point2_cal_start)&&(STAT_SYS_CAL == current_mode))
     {//У׼״̬ �����л�
      if(1 == switch_flag)
       {
        switch_flag = 0;
        switch(point2_cal_step)
        {
         case SYS_CAL_ZER0A: select_ch=3;AD7190Configuration(4);break;
         case SYS_CAL_ZER0B: select_ch=4;AD7190Configuration(8);break;
         case SYS_CAL_LOW1A: select_ch=1;AD7190Configuration(1);break;
         case SYS_CAL_LOW1B: select_ch=4;AD7190Configuration(8);break;
         case SYS_CAL_LOW2A: select_ch=2;AD7190Configuration(2);break;
         case SYS_CAL_LOW2B: select_ch=3;AD7190Configuration(4);break;
         case SYS_CAL_FULL1: select_ch=1;AD7190Configuration(1);break;
         case SYS_CAL_FULL2: select_ch=2;AD7190Configuration(2);break;
         //case SYS_CAL_FULL3: select_ch=2;AD7190Configuration(2);break;
         default:break;
        }
       } 
     }
    else if(0 == sys_cal_ok)
      {//û��У׼������� Ĭ��ʹ��CH1
       if(1!=select_ch) 
        {
         select_ch = 1;
         AD7190Configuration(1);
        }
      }
    else 
      { //�����л�
      
        if(cpu_ch_adc4 >= low1_data[3]) //��У׼300mvʱ������ݱȽ�                                   //use +high
          {
            if(1!=select_ch)     //>300mv
              {
               select_ch = 1;
               AD7190Configuration(1);
              } 
          } 
        else if(cpu_ch_adc4 >= (zero_data[3]+5))
          {   
           if(4!=select_ch)      //0-300mv
              {
               select_ch = 4;
               AD7190Configuration(8);
              } 
          }
        else if(cpu_ch_adc3 >= low2_data[2]) //��У׼-300mvʱ������ݱȽ�                                   //use +high
          {
            if(2!=select_ch)      // < -300mv
              {
               select_ch = 2;
               AD7190Configuration(2);
              } 
          }    
           
        else if((cpu_ch_adc3 >= zero_data[2]+20))
         {                        //-300mv-0v
           if(3!=select_ch)
             {
              select_ch = 3;
              AD7190Configuration(4);
             } 
         
         }
        else
        { //��0MV ���� Ĭ��ʹ�� 
          if(4!=select_ch)      //0 ����
              {
               select_ch = 4;
               AD7190Configuration(8);
              } 
        }      
      
    }  
 }


//**********************************************************************
//��������:FIQ_Exception()
//��������:�����жϴ������,��ȡAD7190������
//**********************************************************************
////////////////////////////////       
void FIQ_Exception(void)
{ 
    uint32 i;
    uint8  buf[4];
    static uint32 Cycle_times=0;
    
    AD7190_DOUT_DAT_MODE;
    ///////////////////////////////////////////
    AD7190_DIN_L;  //when reading DIN must be 0
    ReadFromAD7190(4,buf);		//read data register

    i = (buf[3]<<16)+(buf[2]<<8)+buf[1];
    i>>=4;
    #if SDJ_SOFT
    switch(buf[0]&0x07)
     {
      case 4: raw[0] = i; flag_new_data = 1;break;
      case 5: raw[1] = i; flag_new_data = 1;break;
      case 6: raw[2] = i; flag_new_data = 1;break;
      case 7: raw[3] = i; flag_new_data = 1;break;
      default:            flag_new_data = 0;break;
     }
   
    #endif 
   
    //����LPC2478AD CH3 CH4 ����ֵ�л���ͬ��ͨ�� �˲���������ѭ���н���
    if(MACHINE_NORMAL_MODE == machine_mode)
    AD7190_Channel_Switch();
    ///////////////////////////////////////////////////  
    AD7190_DOUT_INT_MODE;
    EXTINT = 0x04;                //���EINT0�жϱ�־
}
/////////////////////////////////////////////////////
///////////////////////////////////////////////////// 
 uint32 Get_Raw_From_Int2(uint8 ch)
 {
  //return((raw[SAMPLE_BUF_SIZE-1]+raw[SAMPLE_BUF_SIZE-2])/2);
  return(raw[ch]);
 }