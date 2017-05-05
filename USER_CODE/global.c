//**********************************************************************
//�ļ���:global.c
//�����б�
//1  ��ʼ��ȫ�ֱ���  		    InitGlobalVarible 
//2  ��ʼ�����Ա���   		    InitLanguageVarible  
//3  ��ʼ�����ȱ���  		    InitBrightnessVarible
//4  ��ʼ��У׼��Ļ����   		InitScreenVarible
//5  ��ʼ�������̱���   		Init_Cal_Varible
//6  ��ʼ�� 0�����			    Init_Zer0_Weigh_Varible
//7  ����У׼���ݼ���     		Get_Linecal_Var
//8   ��ʼ������У׼����		Init_LineCal_Varible
//8a  ��ȡ��ǰ��¼�Ĵ��λ��    Search_Rec_Pos
//9   ��ʾ������ʱ��   		    Display_elapsed_time
//10  ����������ʾ  		    Display_Sleep_Area
//11  ����ʱ����ʾ   		    Display_Sleep_Time
//12  ����״̬     			    Sleep_process
//13  ����ϵͳ��ʼ��  			SystemInit

//14  ����XY,�����������ݽṹ   Create_ButtonList

//15 ���������־    		    Set_Zer0_Flag
//16 ����ȥƤ��־   		    Set_Tare_Flag
//17 ���òɼ��ٷֱȻ�׼��־		Set_Sam100_Flag
//18 �����ۼӱ�־        		Set_Totaladd_Flag
//19 ���òɼ�Ŀ��ֵ���ݱ�־   	Set_SamTarget_Flag

//**********************************************************************
#include "global.h"
#include "i2c_com.h"
#include "eeprom.h"
#include "graph.h"
#include "ad7843.h"
#include "disp.h"
#include "Font_Lib.h"
#include "Font_Lib2.h"
#include "type_def.h"
#include "alarm_pro.h"

#include "ddlvmenu.h"

#include "rtc.h"
#include "ADS1100.h"

uint32      user_id,phmv_division,which_standard,lcd_brightness;        //��ʾ���Աȶȵ�ֵ
uint32      select_ch,switch_flag,match_item,power_fre;
uint32      sound_switch;

uint32      Key_Status,key_value,electrode_class;  //���������Լ�״̬
struct      KeyStruct  Key[15];

uint32      machine_mode,current_mode,mode_run_flag;      //����������״̬
uint32      power_on_flag,power_on_clr_cnt,elapsed_time;  //������ʱ��¼
uint32      Error_Info_Flag,Warning_Info_Flag;

uint32      ph_negative_flag,mv_negative_flag,full_flag;

uint32      ad_raw_data,flag_new_data,stable_flag;   //������ر���
uint32      ADS1100_Raw,sample_flag;

float       mv_data,ph_data,get_coef,ddlv_data;

float       elec_cons,temp_cons,TDS_cons;           //�缫���� �²�ϵ�� TDS ϵ��
uint32      DDLV_Param,DDLV_Mode,DDLV_unit;         //�絼�ʵĲ���ģʽ
uint32      measure_timer_sec,measure_timer_sec_real;
uint32      balance_sec,balance_sec_real;
float       balance_data;
   
//////////////////////////////�絼��У׼��ر���
uint32      filter_phase,switch_action_step;
uint32      cal_ddlv_buf[6][2];   //����ϵͳУ׼������ �õ�4������
uint32      cal_ddlv_CONS[6];

uint32      cal_ddlv_b_flag[6];
float       cal_ddlv_b[6],cal_ddlv_k[6]; //�����������ݵõ�ϵ��


uint32      cpu_ch_adc3,cpu_ch_adc4;
uint32      zero_data[4],low1_data[4],low2_data[4],full1_data[4],full2_data[4],full3_data[4]; //����ϵͳУ׼������
uint32      user_cal_num,user_cal_data[5];    //����û�У׼������

float        sys_coef1, sys_coef2, sys_coef3, sys_coef4, sys_coef5, sys_coef6;
float       user_coef1,user_coef2,user_coef3,user_coef4,user_coef5,user_coef_left;
//����絼�ʵ��û�У׼ϵ��
float       ddlv_R,ddlv_coef;

//�������5���û�У׼�� 
un_rec_data user_cal_phdata[5];
un_rec_data user_cal_phdata_dis[5];

uint32      user_cal_point[5];

uint32      point2_cal_start,sys_cal_ok,user_cal_ok,point2_cal_step;

uint32      temp_comp_manual,manual_temp_comp_flag;
float       temp_float_ptc;

////////////////////////////�����Զ������������
uint32      no_key_time;

////////////////////////////���Ӽ�¼����
uint32      rec_total_num,rec_real_num;  //��¼��ǰ�洢�ļ�¼λ�ã���ʼ����ʱ������
uint32      rec_total_pages,rec_curre_index;
//////////////////////
uint32 need_recover,need_recover2,from_menu;
uint32 save_buf[LINE_SIZE*SCR_YSIZE/1]; //����ʱ����Ļ��������ڴ����ּ��̷��ػָ�
uint32 save_buf2[LINE_SIZE*SCR_YSIZE/1];//����ʱ����Ļ��������ڷ��ص����ý���
//�����¶ȵ�λת������ ��ӡ����
uint32 temp_unit,print_busy,Uart_Printer_Flag;

//����LOGO���ع��� �汾ѡ����
uint32 version_811,model_display,LOGO_flag;

//�����û�У׼ʹ�õ���Һ 
//�����û�У׼ʹ�õ���Һ 
const   float standard_data_p811[][5] = 
{
 {1.680,4.003,6.864, 9.182,12.460},
 {1.679,4.005,7.000,10.012,12.454},
 {1.679,4.006,6.865, 9.180,12.454}
};

const   float standard_data_p801[][5] = 
{
 {1.68, 4.00, 6.86,  9.18, 12.46},
 {1.68, 4.01, 7.00, 10.01, 12.45},
 {1.68, 4.01, 6.87,  9.18, 12.45}
};
 
 /////////////////////////////////////������²�����
const   float standard1_data[][17] = 
{
 {
  1.668,1.669,1.671,1.673,1.676,1.680,1.684,1.688,
  1.694,1.700,1.706,1.713,1.721,1.739,1.759,1.782,1.795
 },
 {
  4.006,3.999,3.996,3.996,3.998,4.003,4.010,4.019,
  4.029,4.042,4.055,4.070,4.087,4.122,4.161,4.203,4.224
 },
 {
  6.981,6.949,6.921,6.898,6.879,6.864,6.852,6.844,
  6.838,6.834,6.833,6.834,6.837,6.847,6.862,6.881,6.891
 },
 {
  9.458,9.391,9.330,9.276,9.226,9.182,9.142,9.105,
  9.072,9.042,9.015,8.990,8.968,8.926,8.890,8.856,8.839
 },
 {
  13.416,13.210,13.011,12.820,12.637,12.460,12.292,12.130,
  11.975,11.828,11.697,11.553,11.426,11.426,11.426,11.426,11.426
 }
};

const  uint32 standard1_data_temp[] = 
{
 0,5,10,15,20,25,30,35,40,45,50,55,60,70,80,90,95,100
};

//�����¶Ȳ������ϵ��
const   float temp_coef[] = 
{
 54.197,55.189,56.181,57.173,58.165,59.157,60.149,61.141,62.133,63.126,64.118,
 65.110,66.102,67.094,68.086,69.078,70.070,71.062,72.054,73.046,74.038,74.038
};
const   float temp_data[] = 
{ //22����
  0, 5,10,15,20,25,30,35,40, 45, 50,
 55,60,65,70,75,80,85,90,95,100,200
};

//�絼���� �����û�У׼ʹ�õ���Һ 
const   float DDLV_standard_data[][4] = 
{
 {146.6, 1408, 12850, 111300},  //�й���׼
 {146.6, 1408, 12850, 111300},  //�����׼1 
 {146.6, 1408, 12850, 111300}   //�����׼2
 
};

const   float  cal_res_data[][2] = 
{
 {      5.0,             50.0},  
 {     50.0,            500.0},   
 {    500.0,           5000.0},
 {   5000.0,          50000.0},  
 {  50000.0,         500000.0},   
 { 500000.0,        5000000.0}
};


////////////////////////////////////////////////////////////
///////////////////////////////////////////////////
//functions 
//////////////////////////////////////////////////	
//////////////////////////////////////////////////
//1
//������ʼ�� ����ȫ�ֱ���
///////////////////////////////////////////////////
void InitGlobalVarible(void)
	{
	 //Key_Status = KEY_RELEASE ;   //�Ƶ������г�ʼ��
	 ////////////////////////////added after
	 stable_flag       = FALSE;
	 power_on_flag     = TRUE;
	 point2_cal_start  = FALSE;
	 point2_cal_step   = 0;
	 current_mode      = STAT_NORMAL;
     //////////////////////////////////
     electrode_class   = 4;
     ////////////////////////////////
     //elec_cons = 1;
     no_key_time = 0;
     sound_switch = ON;
	}
 //////////////////////////////////////////////////
 //2
 //������ȡ��������
 ///////////////////////////////////////////////////
uint8  InitLanguageVarible(void)
	{
	 uint32 i;
	 uint8  LangCalData[4];
	 
	 Read_EEPROM(EEP_LANGUAGE_ADDR, LangCalData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 
	 if(LangCalData[3] != LangCalData[0] + REC_CHECK_DATA1)
	   {
	    Set_Language_Type(CHINESE);
	    return(ERROR);
	   } 
	  else
	   {
	    Set_Language_Type(LangCalData[0]);
	    return(OK);
	   }
	 } 	
 //////////////////////////////////////////////////
 //3a
 //������ȡ��������
 ///////////////////////////////////////////////////
uint8  InitParamVariable(void)
	{
	 uint32 i;
	 uint8  tmp[16];
	
	 Read_EEPROM(EEP_UART_ADDR, tmp, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 //////////////////////////////////
	 if(tmp[3]==(tmp[0]+tmp[1]+tmp[2]))
	    Uart_Printer_Flag = tmp[2]&0x01;
	 else
	    Uart_Printer_Flag = 0x01;
	 /////////////////////////////////////
	 if(tmp[7]==(tmp[4]+tmp[5]+tmp[6]))
	    version_811 = tmp[6]&0x01;
	 else
	    version_811 = 0x01;

	 ////////////////////////////////////
	 if(tmp[11]==(tmp[8]+tmp[9]+tmp[10]))
	    model_display = tmp[10]&0x01;
	 else
	    model_display = 0x01;


	 if(tmp[15]==(tmp[12]+tmp[13]+tmp[14]))
	    LOGO_flag = tmp[14]&0x01;
	 else
	    LOGO_flag = 0x01;
	 } 		 
 //////////////////////////////////////////////////
 //3
 //������ȡ��Ļ��������
 ///////////////////////////////////////////////////
uint8  InitBrightnessVarible(void)
	{
	 uint32 i;
	 uint8  BriCalData[4];
	 
	 Read_EEPROM(EEP_BRIGHT_ADDR, BriCalData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 
	 if((BriCalData[3] == BriCalData[0] + REC_CHECK_DATA1)&&(BriCalData[0]<=BRIGHT_CLASS_TOTAL))
	   {
	    lcd_brightness = BriCalData[0];  
	   
	    Set_LCD_Brightness(lcd_brightness);
	    return(OK);
	   } 
	  else
	   {
	    lcd_brightness = DEFAULT_BRIGHTNESS;          //default 
	    Set_LCD_Brightness(lcd_brightness);
	    return(ERROR);
	   }
	 } 		
 //////////////////////////////////////////////////
 //3a
 //������ȡ�ֶ��¶Ȳ���������
 ///////////////////////////////////////////////////
uint8  InitTempCompVarible(void)
	{
	 uint32 i;
	 uint8  Temp[4];
	 
	 Read_EEPROM(EEP_TEMP_COMP_ADDR, Temp, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 //��־0x55aa is OK, ����������ֽ����ݿ����ж�ΪOK
	 if(Temp[3] == Temp[0] + Temp[1]+ REC_CHECK_DATA1)
	   {
	    temp_comp_manual = Temp[0];
	    temp_unit =  Temp[1];
	    return(OK);
	   } 
	  else
	   {
	    temp_comp_manual = DEFAULT_TEMP_COMP;          //default 
	    temp_unit =  TEMP_UNIT_OC;
	    return(ERROR);
	   }
	  
	 } 		  
 //////////////////////////////////////////////////
 //4
 //������ȡ��ĻУ׼���ݣ�������̫����ʹ��Ĭ������
 ///////////////////////////////////////////////////
uint8  InitScreenVarible(void)
	{
	 uint32 i;
	 uint16 x00,y00,xmid0,ymid0,xmax0,ymax0;
	 uint8  ScreenCalData[13];
	 uint8  CheckSum;
	  
	 Read_EEPROM(EEP_SCREEN_ADDR, ScreenCalData, 13);
	 for(i=0;i<1000;i++){;}   //delay
	 CheckSum = 0;
	 for(i=0;i<12;i++)
	 CheckSum += ScreenCalData[i];
	 x00   = (ScreenCalData[0] << 8)  + ScreenCalData[1];
     y00   = (ScreenCalData[2] << 8)  + ScreenCalData[3];
	 xmid0 = (ScreenCalData[4] << 8)  + ScreenCalData[5];
	 ymid0 = (ScreenCalData[6] << 8)  + ScreenCalData[7];
	 xmax0 = (ScreenCalData[8] << 8)  + ScreenCalData[9];
	 ymax0 = (ScreenCalData[10] << 8) + ScreenCalData[11];
	 if((CheckSum != ScreenCalData[12])||Set_Originalx0y0(x00,y00)||Set_XmidYmid(xmid0,ymid0)||Set_XmaxYmax(xmax0,ymax0)) 
      {
	  x0 =   X0_DEFAULT;
	  y0 =   Y0_DEFAULT;
	  xmid = XMID_DEFAULT;
	  ymid = YMID_DEFAULT;
	  xmax = XMAX_DEFAULT;
	  ymax = YMAX_DEFAULT;
	  return(ERROR);                 //error
   	  }
	  else
	  return(OK);
	}
 //////////////////////////////////////////////////
 //5
 //������ȡУ׼����
  ///////////////////////////////////////////////////
uint8  Init_Sys_Cal_Varible(void)
	{
	 uint32 i;
	 uint8  SysCalData[16];
	 
	 Read_EEPROM(EEP_FULL4_ADDR1, SysCalData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 i = (SysCalData[0]<<24) + (SysCalData[1]<<16) + (SysCalData[2]<<8) + SysCalData[3];  
	 
	 if(0x55aaaa55 == i)
	 {
	  Read_EEPROM(EEP_ZERO_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  zero_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3]; 
	 
	  Read_EEPROM(EEP_LOW1_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  low1_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3];
	  
	  Read_EEPROM(EEP_LOW2_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  low2_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3];
	  
	  Read_EEPROM(EEP_FULL1_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  full1_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3];
      //////////////////////////
	  Read_EEPROM(EEP_FULL2_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  full2_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3];
	   //////////////////////////
	  Read_EEPROM(EEP_FULL3_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  full3_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3];
	  
	  sys_coef1 = 1300/(full1_data[0]-low1_data[0]+0.0001);
	  sys_coef2 = 1300/(full2_data[0]-low2_data[0]+0.0001);
	  sys_coef3 =  100/( low2_data[1]-zero_data[0]+0.0001);
	  sys_coef4 =  100/( low1_data[1]-zero_data[1]+0.0001);
	  sys_coef5 =  600/(full3_data[0]-full2_data[0]+0.0001);
	  
	  sys_cal_ok = TRUE; 
	  return(OK);
	 }
	 else
	  {
	   sys_cal_ok = FALSE;
	   return(ERROR);
	  }
    }	
 /////////////////////////////////////////////////////
  void cal_calculate(void)
  {
      uint32 i;
      float  res_tmp;
      
      //���ȵõ�ÿ��ŷķ��Ӧ������ 
	  ///////////////////////////////// 
      for(i=0;i<6;i++)    //��Ƶ���           ����
      cal_ddlv_k[i] = (cal_res_data[i][0]*cal_ddlv_buf[i][0] + cal_res_data[i][1]*cal_ddlv_buf[i][1] + 0.01)/2;     
	
  }	
 /////////////////////////////////////////////////////
 void cal_calculate_old(void)
  {
      uint32 i;
      float  res_tmp;
      
      //���ȵõ�ÿ��ŷķ��Ӧ������ 
	  ///////////////////////////////// 
      for(i=0;i<6;i++)    //��Ƶ���           ����
      cal_ddlv_k[i] = (cal_res_data[i][0]*cal_ddlv_buf[i][0] - cal_res_data[i][2]*cal_ddlv_buf[i][2] + 0.001)/ (cal_res_data[i][2]-cal_res_data[i][0]);
      
	  // �˴���У׼���� ��ָ�������������
	  //У׼����*(�õ����Ӧ��AD + ÿ��ŷķ��Ӧ��AD���)
	  for(i=0;i<6;i++)
      cal_ddlv_CONS[i] = cal_res_data[i][0] * (cal_ddlv_buf[i][0] + cal_ddlv_k[i]); //ʹ�õ�λУ׼������Ϊ�ܳ���
      //�õ�ƫ����
      
      for(i=0;i<6;i++) 
      {
       //���ȸ����м���ֵ���������ֵ
       res_tmp = cal_ddlv_CONS[i]/(cal_ddlv_k[i]+cal_ddlv_buf[i][1]);
       if(res_tmp>cal_res_data[i][1])
         {
          cal_ddlv_b_flag[i] = 1; 
          cal_ddlv_b[i] = res_tmp - cal_res_data[i][1];
         } 
	   else
	     {
	      cal_ddlv_b_flag[i] = 0; 
          cal_ddlv_b[i] = cal_res_data[i][1] - res_tmp;
	     }
	  }    
  }	
 /////////////////////////////////////////////////////
 /////////////////////////////////////////////////////
 uint8  Init_Sys_Cal2_Varible(void)
	{
	 uint32 i,j;
	 uint8  SysCalData[16];
	 
	 Read_EEPROM(EEP_DDLV_CHECK1, SysCalData, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 
	 i = (SysCalData[0]<<24) + (SysCalData[1]<<16) + (SysCalData[2]<<8) + SysCalData[3];  
	 
	 if(0x8899aabb == i)
	 {
	  for(j=0;j<6;j++)
	  {
	   Read_EEPROM(EEP_DDLV_SYSCAL1A + 0x10*j, SysCalData, 12);
	   for(i=0;i<1000;i++){;}   //delay
	   for(i=0;i<2;i++)
	   cal_ddlv_buf[j][i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3]; 
	  }
	  
	  /////////////////////////////////
	  cal_calculate();
      sys_cal_ok = TRUE; 
	  return(OK);
	 }
	 else
	  {
	   sys_cal_ok = FALSE;
	   return(ERROR);
	  }
    }	
    
    
 /////////////////////////////////////////////////////
 //////////
uint32  Get_electrode_class(void)
     {
       uint32 i,class_tmp;
       float  tmp;
       tmp = 0;
       for(i=0;i<user_cal_num;i++)
         tmp = tmp + fabs(user_cal_phdata[i].rec_float - user_cal_phdata_dis[i].rec_float);
       
       tmp = tmp/user_cal_num;
       
       if(tmp < 0.1)
          class_tmp = 5;
       else if(tmp < 0.2)
          class_tmp = 4;
       else if(tmp < 0.3)
          class_tmp = 3;
       else if(tmp < 0.4)
          class_tmp = 2;
       else 
          class_tmp = 1;
       //////////////////
	   return(class_tmp);
	 }           
 //////////////////////////////////////////////////////////////////////////////
 //
 //
 ///////////////////////////////////////////////////////////////////////////////
 float Get_One_Coef(float display_value,float real_value,uint32 special_value)
    {
     float tmp;
     
     if(1 == special_value)
       {
        tmp = (18 - display_value)/(18 - real_value);    
       }
     else
       {
        tmp =  display_value /  real_value; 
       }    
     return(tmp);  
    }
 
    
 void  Get_User_Coef123(void)
	  {
	   uint32 i;
	   uint8  UserCalData[16];
	   float  tmp1,tmp2;
	   
	    
	   Read_EEPROM(EEP_USER_CAL_ADDR1, UserCalData, 16); 
	   /////////////////////////////////////////////
	   user_cal_phdata_dis[0].rec_uint32 = (UserCalData[0]<<24) + (UserCalData[1]<<16)  + (UserCalData[2]<<8) + UserCalData[3];
	   user_cal_phdata[0].rec_uint32 = (UserCalData[4]<<24) + (UserCalData[5]<<16)  + (UserCalData[6]<<8) + UserCalData[7];
	   user_coef1      =  Get_One_Coef(user_cal_phdata_dis[0].rec_float,user_cal_phdata[0].rec_float,0);//�õ���һ��ϵ��
	   if(1 == user_cal_num)
	    { //�õ�����ϵ��
	     user_coef_left = Get_One_Coef(user_cal_phdata_dis[0].rec_float,user_cal_phdata[0].rec_float,1);
	     //
	    }
	   else
	    {
	      user_cal_phdata_dis[1].rec_uint32 = (UserCalData[8]<<24) + (UserCalData[9]<<16)  + (UserCalData[10]<<8)  + UserCalData[11];
	      user_cal_phdata[1].rec_uint32 = (UserCalData[12]<<24) + (UserCalData[13]<<16)  + (UserCalData[14]<<8) + UserCalData[15];
	      tmp1 = user_cal_phdata_dis[1].rec_float - user_cal_phdata_dis[0].rec_float;
	      tmp2 = user_cal_phdata[1].rec_float     - user_cal_phdata[0].rec_float;
	      user_coef2      =  Get_One_Coef(tmp1,tmp2,0);//�õ��ڶ���ϵ��     
	      if(2 == user_cal_num)
	       {
	        user_coef_left = Get_One_Coef(user_cal_phdata_dis[1].rec_float,user_cal_phdata[1].rec_float,1);
	        //electrode_class = Get_electrode_class();
	       }
	      else
	       {
	         for(i=0;i<1000;i++){;}   //delay
	         Read_EEPROM(EEP_USER_CAL_ADDR3, UserCalData, 16);  //��ȡ�����ĸ�У׼��Һ
	         user_cal_phdata_dis[2].rec_uint32 = (UserCalData[0]<<24)  + (UserCalData[1]<<16)   + (UserCalData[2]<<8)  + UserCalData[3];
	         user_cal_phdata[2].rec_uint32 = (UserCalData[4]<<24)  + (UserCalData[5]<<16)   + (UserCalData[6]<<8)  + UserCalData[7];
	         tmp1 = user_cal_phdata_dis[2].rec_float - user_cal_phdata_dis[1].rec_float;
	         tmp2 = user_cal_phdata[2].rec_float     - user_cal_phdata[1].rec_float;
	         user_coef3      =  Get_One_Coef(tmp1,tmp2,0);//�õ�������ϵ��
	         if(3 == user_cal_num)
	          {
	           user_coef_left = Get_One_Coef(user_cal_phdata_dis[2].rec_float,user_cal_phdata[2].rec_float,1);
	           //electrode_class = Get_electrode_class();
	          }
	         else 
	          {
	           user_cal_phdata_dis[3].rec_uint32 = (UserCalData[8]<<24) + (UserCalData[9]<<16)  + (UserCalData[10]<<8)  + UserCalData[11];
	           user_cal_phdata[3].rec_uint32 = (UserCalData[12]<<24) + (UserCalData[13]<<16)  + (UserCalData[14]<<8) + UserCalData[15];
	           tmp1 = user_cal_phdata_dis[3].rec_float - user_cal_phdata_dis[2].rec_float;
	           tmp2 = user_cal_phdata[3].rec_float     - user_cal_phdata[2].rec_float;
	           user_coef4      =  Get_One_Coef(tmp1,tmp2,0);//�õ����ĸ�ϵ��     
	           if(4 == user_cal_num)
	            {
	             user_coef_left = Get_One_Coef(user_cal_phdata_dis[3].rec_float,user_cal_phdata[3].rec_float,1);
	             //electrode_class = Get_electrode_class();
	            }
	           else
	            {
	              for(i=0;i<1000;i++){;}   //delay
	              Read_EEPROM(EEP_USER_CAL_ADDR5, UserCalData, 8);  //��ȡ�����У׼��Һ
	              user_cal_phdata_dis[4].rec_uint32 = (UserCalData[0]<<24)  + (UserCalData[1]<<16)   + (UserCalData[2]<<8)  + UserCalData[3];
	              user_cal_phdata[4].rec_uint32 = (UserCalData[4]<<24)  + (UserCalData[5]<<16)   + (UserCalData[6]<<8)  + UserCalData[7];
	              tmp1 = user_cal_phdata_dis[4].rec_float - user_cal_phdata_dis[3].rec_float;
	              tmp2 = user_cal_phdata[4].rec_float     - user_cal_phdata[3].rec_float;
	              user_coef5      =  Get_One_Coef(tmp1,tmp2,0);//�õ������ϵ��
	              user_coef_left = Get_One_Coef(user_cal_phdata_dis[4].rec_float,user_cal_phdata[4].rec_float,1);
	              //electrode_class = Get_electrode_class();
	            
	            }   
	         }    
	      }   
	   }
	  //�õ��缫״̬ 
	  electrode_class = Get_electrode_class();  	
    }
 //////////////////////////////////////////////////
 //5
 //������ȡУ׼����
  ///////////////////////////////////////////////////
uint8  Init_User_Cal_Varible(void)
	{
	 float  tmp;
	 uint32 i;
	 uint8  UserCalData[16];
	 
	 Read_EEPROM(EEP_USER_CAL_FLAG, UserCalData, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 i = (UserCalData[3]<<16) + (UserCalData[2]<<8) + UserCalData[1];  
	 
	 if(0x55aa55 == i)
	  {
	   user_cal_num = UserCalData[0];
	   user_cal_point[0] = UserCalData[8];
	   user_cal_point[1] = UserCalData[9];
	   user_cal_point[2] = UserCalData[10];
	   user_cal_point[3] = UserCalData[11];
	   user_cal_point[4] = UserCalData[12];
	  
	   user_cal_ok = TRUE;
	   Get_User_Coef123();
	   return(OK);    
	  }
	 else
	  {
	   user_cal_ok = FALSE;
	   return(ERROR);
	  }
    }    
    
//////////////////////////////////////////////////
 //5
 //������ȡУ׼����
  ///////////////////////////////////////////////////
uint8  Init_User_Cal_Varible2(void)
	{
	 un_rec_data  tmp;
	 uint32 i;
	 uint8  UserCalData[16];
	 
	 Read_EEPROM(EEP_DDLV_USER_CAL1, UserCalData, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 if((REC_CHECK_DATA1==UserCalData[14])&&(REC_CHECK_DATA2==UserCalData[15]))
	  {
	   tmp.rec_uint32 = (UserCalData[0]<<24)  + (UserCalData[1]<<16)   + (UserCalData[2]<<8)  + UserCalData[3];  
	   ddlv_coef = tmp.rec_float;
	   user_cal_ok = TRUE;
	   return(OK);    
	  }
	 else
	  {
	   ddlv_coef   = 1;
	   user_cal_ok = FALSE;
	   return(ERROR);
	  }
    }        
  //////////////////////////////////////////////////
 //5
 //������ȡ ѡ�� �����ֹ��ұ�׼
  ///////////////////////////////////////////////////
uint8  Init_StandVar_Cal_Varible(void)
	{
	 uint32 i;
	 uint8  StandData[4];
	 
	 Read_EEPROM(EEP_STANDARD_ADDR, StandData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 if(StandData[3] == StandData[0] + REC_CHECK_DATA1)
	  {
	   which_standard =  StandData[0]; 
	   return(OK);
	  }
	 else
	  {
	   which_standard =  CHINESE_S;
	   return(ERROR);
	  }
    }    
  //////////////////////////////////////////////////
 //5
 //������ȡ�û�ѡ���ģʽ��ص�����
  ///////////////////////////////////////////////////   
   uint8  Init_Mode_Config_Varible(void)
	{
	 uint32 i;
	 un_rec_data tmp;
	 uint8  ModeData[16];
	 
	 Read_EEPROM(EEP_DDLV_MODE_ADDR, ModeData, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 if((ModeData[14]==REC_CHECK_DATA1)&&(ModeData[15]==REC_CHECK_DATA2))
	  {
	   DDLV_Param = ModeData[0];
       DDLV_Mode  = ModeData[1];
       measure_timer_sec = (ModeData[2]<<8) + ModeData[3];
       balance_sec       = ModeData[5];
       tmp.rec_uint32 = (ModeData[6]<<24)  + (ModeData[7]<<16)   + (ModeData[8]<<8)  + ModeData[9];
       balance_data = tmp.rec_float;
       //
       measure_timer_sec_real = measure_timer_sec;
  	   balance_sec_real = balance_sec;
  	           
	   return(OK);
	  }
	 else
	  {
	   DDLV_Param = PARAM_DDLV; 
	   DDLV_Mode  = MODE_CONTINUE;
	   measure_timer_sec = 999;
	   balance_data = 999.99;
       balance_sec  = 59;
	   //
       measure_timer_sec_real = measure_timer_sec;
  	   balance_sec_real = balance_sec;
	   return(ERROR);
	  }
    }    
   //////////////////////////////////////////////////
 //5a
 //������ȡDDLV ��������
  ///////////////////////////////////////////////////   
   uint8  Init_DDLV_CONS_Varible(void)
	{
	 uint32 i;
	 uint8  CONS_Data[16];
	 un_rec_data tmp;
	 
	 Read_EEPROM(EEP_DDLV_CONS_ADDR, CONS_Data, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 if((CONS_Data[14]==REC_CHECK_DATA1)&&(CONS_Data[15]==REC_CHECK_DATA2))
	  {
	   tmp.rec_uint32 = (CONS_Data[0]<<24) + (CONS_Data[1]<<16)  + (CONS_Data[2]<<8)  + CONS_Data[3];
       elec_cons = tmp.rec_float;
       tmp.rec_uint32 = (CONS_Data[4]<<24) + (CONS_Data[5]<<16)  + (CONS_Data[6]<<8)  + CONS_Data[7];
       temp_cons  = tmp.rec_float;   
       tmp.rec_uint32 = (CONS_Data[8]<<24) + (CONS_Data[9]<<16)  + (CONS_Data[10]<<8) + CONS_Data[11];
       TDS_cons  = tmp.rec_float;    
             
	   return(OK);
	  }
	 else
	  {
	   elec_cons = 1.0;
	   temp_cons = 2.0;
	   TDS_cons  = 0.5;
	 
	   return(ERROR);
	  }
    }    
       
  //////////////////////////////////////////////////
 //5
 //������ȡУ׼����
  ///////////////////////////////////////////////////
uint8  Init_DivisionVar_Cal_Varible(void)
	{
	 uint32 i;
	 uint8  DIVData[4];
	 
	 Read_EEPROM(EEP_DIV_ADDR, DIVData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 if(DIVData[3] == DIVData[0] + REC_CHECK_DATA1)
	  {
	   phmv_division =  DIVData[0]; 
	   return(OK);
	  }
	 else
	  {//8421
	   phmv_division =  DEFAULT_PH_DISPLAY_DOT + DEFAULT_MV_DISPLAY_DOT;
	   return(ERROR);
	  }
    }
    
   //5
 //������ȡУ׼����
  ///////////////////////////////////////////////////
uint8  Init_UserID_Cal_Varible(void)
	{
	 uint32 i;
	 uint8  UserIData[4];
	 
	 Read_EEPROM(EEP_USER_ID_ADDR, UserIData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 if(UserIData[3] == UserIData[0] + UserIData[1] + UserIData[2])
	  {
	   user_id =  (UserIData[1]<<8) + UserIData[0]; 
	   return(OK);
	  }
	 else
	  {
	   user_id =  DEFAULT_USER_ID;
	   return(ERROR);
	  }
    }
    
    
//////////////////////////////////////////////////
//8a
//�������ҵ�ǰ��¼�Ĵ��λ��
//ÿ����¼ռ��16���ֽ�

///////////////////////////////////////////////////	     
void  Search_Rec_Pos(void)
     {
       uint32 i,j;
 	   uint8  BufferData[8];
	 
       Read_EEPROM(EEP_REC_TOTAL_ADDR,BufferData,8);
	   for(j=0;j<1000;j++){;}   //delay
	   
	   i = (BufferData[0]<<24)+(BufferData[1]<<16)+(BufferData[2]<<8)+BufferData[3];
	   j = (BufferData[4]<<24)+(BufferData[5]<<16)+(BufferData[6]<<8)+BufferData[7];
	   
	   if((0xffffffff==i)&&(0xffffffff==j)||(i!=j))
	   {//��оƬ
	    for(i=0;i<8;i++)
	       BufferData[i] = 0;
	    Write_EEPROM(EEP_REC_TOTAL_ADDR,BufferData, 8); //��ʼ��Ϊ0
		for(j=0;j<40000;j++)	{;}	                 //delay
		//Ĭ��״̬
		rec_total_num = 0;  //�ۼ����ݶ��٣�����ṩ��Ѱַ
		rec_real_num  = 0;  //��Ч�����ж��٣����ֻ����ϵͳ�ɴ洢��Ŀ
		rec_curre_index = 0;
		rec_total_pages = 1;
	   }
	   ////////////////////////////////////////////���
	   else if(i==j)
	   {                        //��ȷ,���ص�ǰ������ӵ�
	    rec_total_num = i;      //��ǰ�ܼ�¼��
	   
	    if(i < RECNUM_PER_PAGE) //��ǰ��¼��С��һҳ
	     {
	      rec_real_num    = i;
	      rec_curre_index = i;
		  rec_total_pages = 1;
	     }
	    else if(i < REC_MAX_NUM) //��ǰ��¼����δ���ڸ���״̬
	     {
	      rec_real_num    = i;
	      rec_curre_index = i;
	      rec_total_pages = i/RECNUM_PER_PAGE + 1; //�ж�ҳ
	     } 
	    else
	     { //��ǰ��¼���Ѿ����ڸ���ģʽ
	      rec_real_num    = REC_MAX_NUM;  //��Ч���̶�Ϊ�����
	      rec_curre_index = i%REC_MAX_NUM;  
	      rec_total_pages = REC_MAX_NUM/RECNUM_PER_PAGE; //ҳ���̶�
	     }
	   }
	  /* else
	   {//�쳣������Ϊ������
	    rec_total_num = 0;
		rec_curre_index = 0;
		rec_total_pages = 1;
	   }*/
    }   
///////////////////////////////////////////////////////////
//9      ��ʾ�Ѻķ�ʱ��
///////////////////////////////////////////////////////////
void  Display_elapsed_time(uint32 time_sec)
   {  
       uint32 i,hour,mint,sec;
       hour = 0;
       mint = 0;
       sec  = 0;        
       if(time_sec>3599)
         {
          hour = time_sec/3600;
          time_sec = time_sec%3600;
          }
       if(time_sec>59)
         {
          mint = time_sec/60;
          time_sec = time_sec%60;
          }        
          sec = time_sec;
                  
         Put_Data(150+515,        30,"%02d", hour);
	     Put_Char(150+485+4*15+3, 30,0x3a);
         Put_Data(150+560,        30,"%02d", mint);
	     Put_Char(150+485+7*15+3, 30,0x3a);
	     Put_Data(150+605,        30,"%02d", sec); 
   
     }	     
	    
//////////////////////////////////////////////////
//10  ����������ʾ
///////////////////////////////////////////////////
void  Display_Sleep_Area(void)
	{
      Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);
     
	  if(ENGLISH == Get_Language_Type())
	     Put_Bmp(SCR_XSIZE-120,410,start11060e,0);    //display START
      else
         Put_Bmp(SCR_XSIZE-120,410,start11060c,0);    //display START
    
      Set_ASC_Lib(Arial16B_Font);
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      Put_Str(Language_Pos(150+400,150+370), 5,Language_Str("ϵͳʱ��"," System Time"));
      Put_Str(Language_Pos(150+400,150+370),30,Language_Str("����ʱ��","Elapsed Time"));
     }

///////////////////////////////////////////////////////////
//11
//    ����ʱ����ʾ
///////////////////////////////////////////////////////////
void Display_Sleep_Time(void)
  { 
   RTCTime local_time_buf;
   if(Sec_Change_Flag)
        {
         Sec_Change_Flag = FALSE;
      
         local_time_buf = RTCGetTime();
         Set_ASC_Lib(Arial20B_Font);
         Set_Chr_Color(COL_WHITE,COL_BLACK);
         //display time
	     Put_Data(150+515,        5,"%02d", local_time_buf.RTC_Hour);
	     Put_Char(150+485+4*15+3, 5,0x3a);
         Put_Data(150+560,        5,"%02d", local_time_buf.RTC_Min);
	     Put_Char(150+485+7*15+3, 5,0x3a);
	     Put_Data(150+605,        5,"%02d", local_time_buf.RTC_Sec);
	     Display_elapsed_time(elapsed_time);
        } 
   }  
///////////////////////////////////////////////////////////
//12
//    ����Ԥ��ʱ�䲻�����������ģʽ
///////////////////////////////////////////////////////////      
void  Sleep_process(void)
    {
      
      uint32 i,X_position,Y_position,Quit_flag;
      
      Quit_flag = FALSE;
         
      InitScreenVarible();         //����ĻУ׼������ǰ������
      InitLanguageVarible();       //�����Բ�����ǰ������ 
      InitBrightnessVarible();     //��Ļ����
      
      Key_Status = KEY_RELEASE+KEY_DO_OVER ;   //key release
      elapsed_time = 0;            //
         
      Display_Sleep_Area();  
        
      do
      {
       ///////////////////////////////keyscan
       for(i=0;i<1000;i++){;}   //delay
       key_value = Get_PositionXY();   //����ɨ��
       if(Key_Status == KEY_DO_START)
       {
	     X_position = (key_value>>16)&0x0000ffff;
	     Y_position =  key_value & 0x0000ffff;
	     if((X_position>670)&&(X_position<780)&&(Y_position>410)&&(Y_position<470))
	         Quit_flag = 0x55aa;
	      else
	         Quit_flag = FALSE;
	     Key_Status = KEY_DO_OVER;  
        }
       /////////////////////////////display time
       Display_Sleep_Time();
      
       ////////////////////////////////////////////
       if((0x55aa == Quit_flag)&&(elapsed_time > PREHOT_TIME)) //Ԥ��ʱ�����30����
           Quit_flag = TRUE;
       /////////////////////////////////////////////////////////////////////
       if((0x55aa == Quit_flag)&&(elapsed_time < PREHOT_TIME)) //Ԥ��ʱ��С��30����
        {
         ////////////////////////����PCS����״̬�ɼ���������
         UINTInput_Interface();
         Set_ASC_Lib(Arial16B_Font);
         Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
         Put_Str(Language_Pos(230-100,200-100),85,Language_Str("ϵͳԤ��ʱ�䲻�� �����뿪������","Not Enough Prehot,Please input Password")); 
       
         do
         {
           i = UINTInput_Key_Pro(Get_PositionXY(),PASSWORD_INPUT_ID); //
               UINTInput_Disp_Pro();
           if((i==PASSWORD_DATA1)||(i==PASSWORD_DATA2))
             {
              Quit_flag = TRUE;  
             }
             else if(i != PASSWORD_INPUT_ID) //������Ч������ˢ������
              UINTInput_Interface(); 
          } 
          while(TRUE !=Quit_flag);
         }
      }   
      while(FALSE == Quit_flag);
    }
 
//////////////////////////////////////////////////
//13 ������ʼ������ȫ�ֱ���
//return 
///////////////////////////////////////////////////
uint8  SystemInit(void)
{   
    uint32 key_times=0;
    uint32 key_buf[4]={0,0,0,0};
    uint32 i,j,key_posx,key_posy;
     
    InitGlobalVarible();
    Search_Rec_Pos();        //���ҵ�ǰ��ŵļ�¼λ��
     
    InitScreenVarible();         //����ĻУ׼������ǰ������
    InitLanguageVarible();       //�����Բ�����ǰ������ 
    InitBrightnessVarible();     //��Ļ����
      
    ////////added by wjg 20151118
    InitParamVariable(); 
  
    //DRAW TITLE
    Clr_Bar(0,0,800,100,COL_BLACK);								//TITLE
	
	//Put_Bmp(0,0,logo22560,0);
   
	Clr_Bar(0,100,800,380,COL_BLACK);
	
		
	//test end
	
	Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);  
    #if SDJ_SOFT 
    if(1==version_811)  
     Put_Str(280,160,Language_Str("P811�� ��ȼ�",    "P811  PH meter"));
    else
     Put_Str(280,160,Language_Str("P801�� ��ȼ�",    "P801  PH meter"));
  	#else
  	if(1==version_811)  
     Put_Str(280,160,Language_Str("P812�� �絼�ʼ�",  "P812  Conductometer meter"));
    else
     Put_Str(280,160,Language_Str("P802�� �絼����",  "P802  Conductometer meter"));
  	#endif
  	 Put_Str(280,210,Language_Str("ϵͳ��ʼ�� ......",  "System initializing......"));
	
	Clr_Bar(0,300,800,35,COL_DEEPRED);
	
	for(i=0;i<SCR_XSIZE/2;i++)
	{
	Clr_Bar(0,300,i*2,35,COL_DEEPGREEN);
    if(i < SCR_XSIZE/4)
	Set_Chr_Color(COL_BLACK,COL_DEEPRED);
	else
	Set_Chr_Color(COL_BLACK,COL_DEEPGREEN);
	Put_Numeral(310, 310,i/4,1);
	Put_Char(310+30,310,'%');     //display 88%
	for(j=0;j<300;j++){;}    //��������������
//////////////////////////////////////////////////////STEP1:SCREENCAL_data
	if(i == 50)      //
	 {
	 if(ERROR == InitScreenVarible())
     {
      Set_Chr_Color(COL_DEEPRED,COL_BLACK);
	  Put_Str(200,380,"ScreenCAL ERROR!,Please Recal Screen");
	 }
	else
     {
      Set_Chr_Color(COL_THINGREEN,COL_BLACK);
      Put_Str(200,380,"Screen CAL Data OK!");
	 }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	}
////////////////////////////////////////////////////STEP2:CAL_DATA
	if(i == 70)
	{
	 
	 #if SDJ_SOFT
	 if(ERROR == Init_Sys_Cal_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"SystemData ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"System Data OK!");
	    }
	 #else
	  if(ERROR == Init_Sys_Cal2_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"SystemData ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"System Data OK!");
	    }
     #endif
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	}	
			
////////////////////////////////////////////////////STEP3:linecal data
	if(i == 90)
	{
	  #if SDJ_SOFT
	   j = Init_User_Cal_Varible();
	  #else
	   j = Init_User_Cal_Varible2();
	  #endif
	  if(ERROR == j)
	    {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"UserData ERROR!,Use Default Data");
	   	}
	  else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"UserData OK!");
	    }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}
////////////////////////////////////////////////////STEP3:linecal data
	if(i == 110)
	{
	 if(ERROR == Init_StandVar_Cal_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"StandardVar ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"StandardVar Data OK!");
	    }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}	
////////////////////////////////////////////////////STEP3:linecal data
	if(i == 130)
	{
	 if(ERROR == Init_DivisionVar_Cal_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"DivisionVar ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"DivisionVar Data OK!");
	    }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}	
////////////////////////////////////////////////////STEP3:linecal data
	if(i == 150)
	{
	 if(ERROR == Init_UserID_Cal_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"UserID ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"UserID Data OK!");
	    }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}	
///////////////////////////////////////////////////////////////////
#if DDLV_SOFT
	if(i == 170)
	{
	 if(ERROR == Init_Mode_Config_Varible())  //��ȡ�ϴ� �Ĳ���ģʽ 
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"MODE CONFIG ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"MODE Data OK!");
	    }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}	
	//////////////////////////////////////////��ȡDDLV ����
    if(i == 185)
	{
	 if(ERROR == Init_DDLV_CONS_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"DDLV_CONS CONFIG ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"DDLV_CONS Data OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}	
#endif		
////////////////////////////////////////////////////STEP3:temp data
	if(i == 200)
	{
	   if( ERROR == InitTempCompVarible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"Temperature Data ERROR!");
	   	 }
	    else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"Temperature Data OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	}
////////////////////////////////////////////////////STEP4:��⿪�����̲�������Ӧ�Ľ���
	if((i>50) && (key_times<4))  //���4������
	{
	 key_value = Get_PositionXY();   //����ɨ��
	 if(Key_Status == KEY_DO_START)
     {
      Key_Status = KEY_DO_OVER;           //��ʼ����
      key_times++;
                                          //�������
     }	
	}
   }  
/////////////////////////////////////////////////STEP6:GLOBAL VARIABLE
    if(key_times > 1)
     return(MACHINE_SERVICE_MODE);     //ά��ģʽ
    else 
     return(MACHINE_NORMAL_MODE);        //����ģʽ
}
///////////////////////////////////////////////////
//14  ����X Yλ�ù����������� 
//////////////////////////////////////////////////
void  Create_ButtonList(uint32 (*p)[2],uint32 num)
	{
	         uint32 i;
	 struct  KeyStruct *pKey;
	 pKey = Key;
	 for(i=0;i<num;i++)
	 	{
		(*pKey).X_Position = (*p)[0];
		(*pKey).Y_Position = (*p)[1];
		(*pKey).Value      = i+1;
		 pKey++;
		 p++;
		}		
	}
	
///////////////////////////////////////////////////////////
// ���ݱ�׼ �� ʵʱ�¶� Ѱ��Ҫƥ���У׼����
//����0: 
// ���� ������ ������????? ���Է���UINT32
///////////////////////////////////////////////////////////
uint32  Search_user_cal(uint32 cal_point)
     {
      uint32  i;
      un_rec_data tmp;
      switch(which_standard)
       {
        case CHINESE_S:
                       //temp_p = standard1_data[user_cal_point[0]][0]; 
                       //num = 17;
                       ////////////////////
                       if(TRUE == manual_temp_comp_flag)
                          tmp.rec_float = (float)temp_comp_manual;
                       else
                          tmp.rec_float = temp_float_ptc;
                       ////////////////////////////////   
                       for(i=0;i<18;i++)
                          {
                           if(tmp.rec_float < (standard1_data_temp[i]+0.00001))
                              break;
                          }
                       //////////////////////i ���ǵ�ǰ�Ƚϵ���������
                       if(i==0)
                          tmp.rec_float = standard1_data[cal_point][0];
                       else if(i==17)
                          tmp.rec_float = standard1_data[cal_point][16];
                       else
                         {
                          tmp.rec_float = tmp.rec_float - standard1_data_temp[i-1];//������֮���λ��(0-5
                          //��������֮����¶� ����Ӧ��PH
                          tmp.rec_float = tmp.rec_float * (standard1_data[cal_point][i]-standard1_data[cal_point][i-1])/5;
                          tmp.rec_float += standard1_data[cal_point][i-1];
                         }
                       break;
        case EUROPE_S:
                       tmp.rec_float = standard_data_p811[1][cal_point];
                       break;
        case AMERICAN_S:
                       tmp.rec_float = standard_data_p811[2][cal_point];
                       break;
        default:       break;                
       }
     /////////////////////////////////////
      return(tmp.rec_uint32);  
     } 

///////////////////////////////////////////////////////////
// �²����� �����¶�Ѱ�ҵ�Ҫʹ�õ� ת��ϵ��
// ���� ϵ��
///////////////////////////////////////////////////////////
float   Search_Coef_Temp(void)
       {
        float temp,tmp;
        uint32 i;
        if(TRUE == manual_temp_comp_flag)
           temp = temp_comp_manual;
        else
           temp = temp_float_ptc;
        ////////////////////////////////   
        for(i=0;i<22;i++)
          {
          
          if(temp < (temp_data[i]+0.00001))
             break;
          }
        //////////////////////i ���ǵ�ǰ�Ƚϵ���������
        if(i==0)
          tmp = temp_coef[0];
        else 
          {
           tmp = temp - temp_data[i-1];//������֮���λ��(0-5
            //��������֮����¶� ����Ӧ��PH
           tmp = tmp * (temp_coef[i]-temp_coef[i-1])/5;
           tmp += temp_coef[i-1];
          }       
        ///////////////////////////////////////////// 
        get_coef = tmp;
        
       }
       
///////////////////////////////////////////////////////////
//20 ���浽ָ��λ��һ����¼
//��������ر���
//����0:  ok
//    1:  not stable
//    2:  ee2prom fault
///////////////////////////////////////////////////////////
void  Save_Rec(void)
    {
     un_rec_data rec_data;
     uint32 i;
     uint8 check_data;
     uint8 Send_Data[16];
     RTCTime LocalTime;   
     //if(FALSE == stable_flag)
     //{
     // Log_Alm(&tips_save_fault,ALM_LEV_TIPS);  //
     //}
     //else
     //{ 
       LocalTime = RTCGetTime();
       Send_Data[0] = (user_id>>8)&0xff;
       Send_Data[1] =      user_id&0xff;
       if(TRUE == manual_temp_comp_flag)
        {
         Send_Data[2] = (uint8)temp_comp_manual;//
         Send_Data[3] = 0;
        }
       else
        {
         Send_Data[2] = (uint8)temp_float_ptc;//
         Send_Data[3] = (uint8)(((uint32)(temp_float_ptc*10))%10);
        }  
        
       //����Сʱ����  4 5 5 6
       i=(LocalTime.RTC_Mon<<16)+(LocalTime.RTC_Day<<11)+(LocalTime.RTC_Hour<<6)+(LocalTime.RTC_Min&0x3f);
       Send_Data[4] = i&0xff;
       Send_Data[5] = (i>>8) &0xff;
       Send_Data[6] = (i>>16)&0xff;
     
       rec_data.rec_float = ph_data;
       Send_Data[7]  = (rec_data.rec_uint32>>24)&0xff;
       Send_Data[8]  = (rec_data.rec_uint32>>16)&0xff;
       Send_Data[9]  =  (rec_data.rec_uint32>>8)&0xff;
       Send_Data[10] =       rec_data.rec_uint32&0xff;
       rec_data.rec_float = mv_data;
       Send_Data[11] = (rec_data.rec_uint32>>24)&0xff;
       Send_Data[12] = (rec_data.rec_uint32>>16)&0xff;
       Send_Data[13] =  (rec_data.rec_uint32>>8)&0xff;
       Send_Data[14] =       rec_data.rec_uint32&0xff;
       
       ////////////////////////����15���ֽڵ� У���
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       
       Send_Data[15] = check_data; 
       //save one rec
       Write_EEPROM(EEP_REC_START_ADDR+rec_curre_index*EEP_BYTE_PER_REC,Send_Data, 16); 
	   for(i=0;i<20000;i++)	{;}	         //delay
	  
	   for(i=0;i<16;i++) Send_Data[i] = 0;	 //clear buf
       //readback
	   Read_EEPROM(EEP_REC_START_ADDR+rec_curre_index*EEP_BYTE_PER_REC,Send_Data, 16);	 
	   for(i=0;i<1000;i++)	{;}	         //delay
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       if(Send_Data[15] == check_data)
       {
        rec_total_num++;               //�����ܼ�¼��
        
        if(rec_real_num < REC_MAX_NUM) //������Ч����
        rec_real_num++;
                                       //������ҳ�� 
        rec_total_pages =  rec_real_num/RECNUM_PER_PAGE + 1;
                                      
        rec_curre_index++;             //��������
        if(REC_MAX_NUM == rec_curre_index)
        rec_curre_index = 0;
                                       
        Send_Data[0] = (rec_total_num>>24)&0xff;
        Send_Data[1] = (rec_total_num>>16)&0xff;
        Send_Data[2] =  (rec_total_num>>8)&0xff;
        Send_Data[3] =       rec_total_num&0xff;
        Send_Data[4] = Send_Data[0];
        Send_Data[5] = Send_Data[1];
        Send_Data[6] = Send_Data[2];
        Send_Data[7] = Send_Data[3];
        Write_EEPROM(EEP_REC_TOTAL_ADDR,Send_Data, 8); 
        for(i=0;i<20000;i++)	{;}	
        Log_Alm(&tips_save_ok,ALM_LEV_TIPS);  //��������
       }
       else
       {
        Log_Alm(&tips_EE_bad,ALM_LEV_TIPS);
       }
    //}
  }  

///////////////////////////////////////////////////////////
//20 ���浽ָ��λ��һ����¼
//��������ر���
//����0:  ok
//    1:  not stable
//    2:  ee2prom fault
///////////////////////////////////////////////////////////
void  Save_RecDDLV(void)
    {
     un_rec_data rec_data;
     uint32 i;
     uint8 check_data;
     uint8 Send_Data[16];
     RTCTime LocalTime;   
    
       LocalTime = RTCGetTime();
       Send_Data[0] = (user_id>>8)&0xff;
       Send_Data[1] =      user_id&0xff;
       if(TRUE == manual_temp_comp_flag)
        {
         Send_Data[2] = (uint8)temp_comp_manual;//
         Send_Data[3] = 0;
        }
       else
        {
         Send_Data[2] = (uint8)temp_float_ptc;//
         Send_Data[3] = (uint8)(((uint32)(temp_float_ptc*10))%10);
        }  
        
       //����Сʱ����  4 5 5 6
       i=(LocalTime.RTC_Mon<<16)+(LocalTime.RTC_Day<<11)+(LocalTime.RTC_Hour<<6)+(LocalTime.RTC_Min&0x3f);
       Send_Data[4] = i&0xff;
       Send_Data[5] = (i>>8) &0xff;
       Send_Data[6] = (i>>16)&0xff;
     
       rec_data.rec_float = ddlv_data;
       Send_Data[7]  = (rec_data.rec_uint32>>24)&0xff;
       Send_Data[8]  = (rec_data.rec_uint32>>16)&0xff;
       Send_Data[9]  =  (rec_data.rec_uint32>>8)&0xff;
       Send_Data[10] =       rec_data.rec_uint32&0xff;
       rec_data.rec_float = mv_data;
       Send_Data[11] = (rec_data.rec_uint32>>24)&0xff;
       Send_Data[12] = (rec_data.rec_uint32>>16)&0xff;
       Send_Data[13] =  (rec_data.rec_uint32>>8)&0xff;
       Send_Data[14] =       rec_data.rec_uint32&0xff;
       
       ////////////////////////����15���ֽڵ� У���
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       
       Send_Data[15] = check_data; 
       //save one rec
       Write_EEPROM(EEP_REC_START_ADDR+rec_curre_index*EEP_BYTE_PER_REC,Send_Data, 16); 
	   for(i=0;i<20000;i++)	{;}	         //delay
	  
	   for(i=0;i<16;i++) Send_Data[i] = 0;	 //clear buf
       //readback
	   Read_EEPROM(EEP_REC_START_ADDR+rec_curre_index*EEP_BYTE_PER_REC,Send_Data, 16);	 
	   for(i=0;i<1000;i++)	{;}	         //delay
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       if(Send_Data[15] == check_data)
       {
        rec_total_num++;               //�����ܼ�¼��
        
        if(rec_real_num < REC_MAX_NUM) //������Ч����
        rec_real_num++;
                                       //������ҳ�� 
        rec_total_pages =  rec_real_num/RECNUM_PER_PAGE + 1;
                                      
        rec_curre_index++;             //��������
        if(REC_MAX_NUM == rec_curre_index)
        rec_curre_index = 0;
                                       
        Send_Data[0] = (rec_total_num>>24)&0xff;
        Send_Data[1] = (rec_total_num>>16)&0xff;
        Send_Data[2] =  (rec_total_num>>8)&0xff;
        Send_Data[3] =       rec_total_num&0xff;
        Send_Data[4] = Send_Data[0];
        Send_Data[5] = Send_Data[1];
        Send_Data[6] = Send_Data[2];
        Send_Data[7] = Send_Data[3];
        Write_EEPROM(EEP_REC_TOTAL_ADDR,Send_Data, 8); 
        for(i=0;i<20000;i++)	{;}	
        Log_Alm(&tips_save_ok,ALM_LEV_TIPS);  //��������
       }
       else
       {
        Log_Alm(&tips_EE_bad,ALM_LEV_TIPS);
       }
  }  

//////////////////////////////////////////////////////////////////////////////////
//
//ƥ�伤����Դ�ͷ������裬��ADS1100�õ����ʵ�����
//////////////////////////////////////////////////////////////////////////////////
void  ADS1100_Match_Proc(void)
   { 
     static uint32 ads1100_raw_pre = 0;
     static uint32 change_cnt1 = 0;      //���ڷǷ����ݼ�ʱ
     static uint32 change_cnt2 = 0;      //�����˲���ʱ
     static uint32 change_cnt3 = 0;      //�����л���ʱ
     //static uint32 switch_cnt  = 0;   
       
            uint32 tmp,need_switch_flag;
     
     if((TRUE ==point2_cal_start)&&(STAT_SYS_CAL == current_mode))
     {//У׼״̬ �����л�
      if(1 == switch_flag)
       {
        switch_flag = 0;
        switch(point2_cal_step)
        {
         case SYS_CAL_1A : 
         case SYS_CAL_1B :
                            match_item = MATCH_ITEM_1;
                            MATCH_ITEM_A;
                            break;
         case SYS_CAL_2A : 
         case SYS_CAL_2B :
                            match_item = MATCH_ITEM_2; 
                            MATCH_ITEM_B;
                            break;
         case SYS_CAL_3A : 
         case SYS_CAL_3B :
                            match_item = MATCH_ITEM_3; 
                            MATCH_ITEM_C;
                            break;
         case SYS_CAL_4A : 
         case SYS_CAL_4B :
                            match_item = MATCH_ITEM_4; 
                            MATCH_ITEM_D;
                            break; 
         case SYS_CAL_5A : 
         case SYS_CAL_5B :
                            match_item = MATCH_ITEM_5; 
                            MATCH_ITEM_E;
                            break;
         case SYS_CAL_6A : 
         case SYS_CAL_6B :
                            match_item = MATCH_ITEM_6;
                            MATCH_ITEM_F;
                            break;
                            
         default:break;
        }
       } 
     }
    else if(0 == sys_cal_ok)
      {//û��У׼������� Ĭ��ʹ��CH1
       if(MATCH_ITEM_6!=match_item) 
        {
         match_item = MATCH_ITEM_6;
         MATCH_ITEM_F;    //�Զ��л�����������
        }
      }
    else 
      { //�����л�
            need_switch_flag = 0;     //Ĭ�ϲ���Ҫ�л�
      
            //tmp = ADS1100_Raw/10;
            if((ADS1100_Raw<20)||(ADS1100_Raw>8150))
             {
             
              ads1100_raw_pre = tmp;
              change_cnt1++;
              if(change_cnt1>10000)
                 change_cnt1 = 2000;
              
              if(change_cnt1 > 5)
               {
                change_cnt1 = 0;  
                need_switch_flag = 1;
                filter_phase = 1;       //�Ƿ����ݽ׶�
               }
             }
            else
             {
              tmp = abs(ADS1100_Raw-ads1100_raw_pre);
              if(tmp < 500)
                {   //�����仯�׶�
                 change_cnt2++;
                 change_cnt3++;
                 
                 if(change_cnt2 > 10000)
                    change_cnt2 = 2000;
                    
                 if(change_cnt2 > 5)
                   filter_phase = 3;
                
                 if(change_cnt3 > 30)  //�������仯��һ���̶Ⱥ󣬼������л�Ҫ��
                    {
                     need_switch_flag = 1;
                     change_cnt3      = 0;
                    }
                }
              else
                {  //�仯�����ϴ�� �׶�
                   change_cnt1 = 0;
                   change_cnt2 = 0;
                   change_cnt3 = 0;
                   filter_phase = 2;
                }
             
             } 
            /////////////////////////////////////////////�л�����  
             if((1==need_switch_flag)&&(1==stable_flag))  
              {
                  //�Ӵ�С�л�
                 switch_action_step++;
                 if(switch_action_step>10000)
                    switch_action_step = 100; 
                      
                 tmp = Get_Filter_ADS1100Data();
                 
               switch(match_item)
                 { 
                   case MATCH_ITEM_6:
                               if(tmp > cal_ddlv_buf[5][0]*1.05)
                                 {
                                  match_item--;
                                  MATCH_ITEM_E;
                                  switch_action_step = 0;
                                 }
                               break; 
                               
                case MATCH_ITEM_5:
                case MATCH_ITEM_4:
                case MATCH_ITEM_3:
                case MATCH_ITEM_2:                             
                               if(tmp > cal_ddlv_buf[match_item-1][0])
                                   {
                                    match_item--;
                                    switch_action_step = 0;
                                   }
                               if(tmp < cal_ddlv_buf[match_item-1][1] * 0.95)
                                   {
                                    match_item++;
                                    switch_action_step = 0;
                                   }
                               switch(match_item)
                                 {
                                  case MATCH_ITEM_1:MATCH_ITEM_A;break;
                                  case MATCH_ITEM_2:MATCH_ITEM_B;break;
                                  case MATCH_ITEM_3:MATCH_ITEM_C;break;
                                  case MATCH_ITEM_4:MATCH_ITEM_D;break;
                                  case MATCH_ITEM_5:MATCH_ITEM_E;break;
                                  case MATCH_ITEM_6:MATCH_ITEM_F;break;
                                  default:break;
                                 } 
                               break;   
                case MATCH_ITEM_1:
                               if(tmp < cal_ddlv_buf[0][1] * 0.95)
                                 {
                                  match_item++;
                                  MATCH_ITEM_B;
                                  switch_action_step = 0;
                                 }
                               break;
                 default:      break;      
               }
            
           } 
     }
}
 
//////////////////////////////////////////
//
// �²�����
//
//////////////////////////////////////////////////
void  temp_comp_proc(void)
   {
    uint32 i;
    float  tmp;
    ///////////////////////////////////�õ���Ҫ�������¶�
    if(TRUE == manual_temp_comp_flag)
     tmp = temp_comp_manual; 
    else
     tmp = temp_float_ptc;
    ///////////////////////////////////�����¶�ƫ��25��׼�ȵĲ�ֵ�� �¶�ϵ�� ���� ddlv_data
    if(tmp > 25)      
     ddlv_data = ddlv_data*(1 + (tmp-25)*temp_cons/100);
    else
     ddlv_data = ddlv_data*(1 - (25-tmp)*temp_cons/100);
    
   }
   
//////////////////////////////////////////
//�ָ��������� ��������
//�û�ID  ���� 
//�û�У׼����     
//
//////////////////////////////////////////////////
void  do_factory_call(void)
    {
     uint8 tx_buf[16];
     uint32 i;
     ///////////////UID   
     user_id   = DEFAULT_USER_ID; 
     tx_buf[0] = (uint8)(user_id&0xff);
  	 tx_buf[1] = (uint8)((user_id>>8)&0xff);
  	 tx_buf[2] = 0;
  	 tx_buf[3] = tx_buf[0]+tx_buf[1]+tx_buf[2]; 
  	 Write_EEPROM(EEP_USER_ID_ADDR,tx_buf, 4); 
  	 for(i=0;i<100000;i++);
  	 
     ///////////////BRIGHT   
     lcd_brightness = DEFAULT_BRIGHTNESS;
     Set_LCD_Brightness(lcd_brightness);
     tx_buf[0] = (uint8)(lcd_brightness);
  	 tx_buf[1] = 0;
  	 tx_buf[2] = 0;
  	 tx_buf[3] = tx_buf[0] + REC_CHECK_DATA1; 
  	 Write_EEPROM(EEP_BRIGHT_ADDR,tx_buf, 4);
     for(i=0;i<100000;i++); 
     
     ///////////////PH MV display dot  
     phmv_division = DEFAULT_PH_DISPLAY_DOT + DEFAULT_MV_DISPLAY_DOT; 
     tx_buf[0] = (uint8)(phmv_division);
  	 tx_buf[1] = 0;
  	 tx_buf[2] = 0;
  	 tx_buf[3] = tx_buf[0] + REC_CHECK_DATA1; 
  	 Write_EEPROM(EEP_DIV_ADDR,tx_buf, 4);
     for(i=0;i<100000;i++); 
     
     ///////////////�²���λ ��ֵ
     temp_comp_manual = DEFAULT_TEMP_COMP;    //25���϶�
     temp_unit        = TEMP_UNIT_OC;      
     tx_buf[0] = (uint8)temp_comp_manual;
     tx_buf[1] = temp_unit;
     tx_buf[2] = 0;
     tx_buf[3] = tx_buf[0] + tx_buf[1] + REC_CHECK_DATA1;
     Write_EEPROM(EEP_TEMP_COMP_ADDR,tx_buf, 4);
     for(i=0;i<100000;i++); 
     
     ///////////////���û�У׼����ɾ��
     Clear_User_Cal();
	 	  
     //����ΪDDLV ����
     #if DDLV_SOFT
     DDLV_Param = PARAM_DDLV; 
	 DDLV_Mode  = MODE_CONTINUE;
	 measure_timer_sec = 999;
	 balance_data = 999.99;
     balance_sec  = 59;
	 ////////////////////////////////////////////////////
     tx_buf[0]  = (uint8)(DDLV_Param & 0xff);
     tx_buf[1]  = (uint8)(DDLV_Mode & 0xff);
     tx_buf[2]  = (uint8)((measure_timer_sec>>8)&0xff);
  	 tx_buf[3]  = (uint8)(measure_timer_sec&0xff);
  	 tx_buf[4]  = 0;
  	 tx_buf[5]  = (uint8)(balance_sec&0xff);
  	         
  	 //tx_buf[6]  = (uint8)((balance_data_tmp.rec_uint32>>24)&0xff);
  	 //tx_buf[7]  = (uint8)((balance_data_tmp.rec_uint32>>16)&0xff);
  	 //tx_buf[8]  = (uint8)((balance_data_tmp.rec_uint32>>8)&0xff);
  	 //tx_buf[9]  = (uint8)( balance_data_tmp.rec_uint32&0xff);
  	           
  	 tx_buf[14] = REC_CHECK_DATA1;
  	 tx_buf[15] = REC_CHECK_DATA2;   
  	 Write_EEPROM(EEP_DDLV_MODE_ADDR,tx_buf, 16);
  	          
     measure_timer_sec_real = measure_timer_sec;
  	 balance_sec_real = balance_sec;
     #endif
   
    }
   
//////////////////////////////////////////
//���û�У׼��������    
//
//////////////////////////////////////////////////   
void  sort_cal_buf(uint32 cal_numer)
      {
        uint32 i,j,point_tmp;
        float  data_tmp;
        
        for(i=0;i<cal_numer-1;i++)
          {
           j = i+1;
           for(;j<cal_numer;j++)
            {
             if(user_cal_point[i] > user_cal_point[j])
               {
                point_tmp = user_cal_point[i];
                user_cal_point[i] = user_cal_point[j];
                user_cal_point[j] = point_tmp;
                ////
                data_tmp = user_cal_phdata[i].rec_float;
                user_cal_phdata[i].rec_float = user_cal_phdata[j].rec_float;
                user_cal_phdata[j].rec_float = data_tmp;
               }
            } //��С��������
          }  //������cal_numer-1 ��ѭ��
       }
//////////////////////////////////////////
//
//�����û�У׼����
//
//////////////////////////////////////////////////   
   
void  save_cal_data(uint32 cal_numer)
     { 
      uint8   eeprom_tx_buf[16];
      uint32  i,j;
     
      for(i=0;i<cal_numer;i++)
         {
               user_cal_phdata_dis[i].rec_uint32 = Search_user_cal(user_cal_point[i]); //��Ҫ���̵�������
  	           eeprom_tx_buf[0] = (uint8)(user_cal_phdata_dis[i].rec_uint32>>24);
  	           eeprom_tx_buf[1] = (uint8)((user_cal_phdata_dis[i].rec_uint32>>16)&0xff);
  	           eeprom_tx_buf[2] = (uint8)((user_cal_phdata_dis[i].rec_uint32>>8)&0xff);
  	           eeprom_tx_buf[3] = (uint8)(user_cal_phdata_dis[i].rec_uint32&0xff); 
  	           ///////////////////////////////ʵ��PH����
               eeprom_tx_buf[4] = (uint8)(user_cal_phdata[i].rec_uint32>>24);
  	           eeprom_tx_buf[5] = (uint8)((user_cal_phdata[i].rec_uint32>>16)&0xff);
  	           eeprom_tx_buf[6] = (uint8)((user_cal_phdata[i].rec_uint32>>8)&0xff);
  	           eeprom_tx_buf[7] = (uint8)(user_cal_phdata[i].rec_uint32&0xff); 
  	          
  	           I2C_Send_Str(SYS24C08, EEP_USER_CAL_ADDR1+8*i, eeprom_tx_buf, 8);	 
               for(j=0;j<10000;j++)
  	            {;}
  	      }      
  	  }         
  	       