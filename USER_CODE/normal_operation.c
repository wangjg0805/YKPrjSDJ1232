//**********************************************************************
//�ļ���:normal_oprarion.c
//˵  ��:1.����ģʽ�µĲ˵�����
//       2.����ģʽ�µĹ��ܵ���
//����˵��
//1   
//2  
//3   
//4   
//5   
//6  
//7   
//8  
//9   
//10 
//**********************************************************************
#include "stdio.h"
#include "stdlib.h"

#include "eeprom.h"
#include "Font_Lib.H"
#include "Font_Lib2.H"
#include "graph.h"
#include "global.h"
#include "i2c_com.h"
#include "normal_operation.h"
#include "timer.h"
#include "type_def.h"
#include "menu.h"
#include "screencal.H"
#include "filter.h"
#include "mainmenu2.h"
#include "ddlvmenu.h"

#include "datecfg.h"
#include "port.h"
#include "disp.h"
#include "lcd2478.h"
#include "rtc.h"
#include "Calendar.h"
#include "lpc2478_uart.h"
#include "alarm_pro.h"

#include "ads1100.h"
//function1 
static void   Stable_Status_Display(uint32 x,uint32 y);
static void   Negative_Status_Display(void);

static void   New_Data_Display(void);
static void   New_Data_Display2(void);

static void   display_sys_caling_data(void);
static void   display_sys_caling_data2(void);

static void   display_user_caling_data(void);
static void   display_user_caling_data2(void);

static void   display_measure_data(void);
static void   display_measure2_data(void);

static sint32   menu_op;
//variable
//
static  uint32  match_flag,match_point;

extern  float   cal_res_data[][3]; 
extern  uint32  ddlv_buf_pre[50];


//////////////////////////////////////////////
//11 ϵͳУ׼ ���̵� Ĭ��AD ����
//    
//////////////////////////////////////////////////////////////
const uint32 DEFAULT_CAL_DATA[6][2] = 
  {  
     {6950,   736},  //     5            50
     {8032,   780},  //    50           500
     {7360,   730},  //   500          5000
     
     {7140,   650},  //  5000         50000
     {6750,   600},  // 50000        500000
     {7290,   650}   //500000       5000000  
  };


//////////////////////////////////////////////////////////
//01 �����û�У׼״̬
//////////////////////////////////////////////////////////
void Enter_UserPoint_Cal(void)
   {
    if(FALSE == point2_cal_start)
     {
      current_mode = STAT_USER_CAL;
      point2_cal_start = TRUE;
      point2_cal_step = USER_CAL_STEP1;
      match_flag = FALSE;
      switch_flag=1;
     }
   }
   
//////////////////////////////////////////////////////////
//01 ��ʾ��ǰ�û�ʹ��У׼��Һ��״̬
//
//////////////////////////////////////////////////////////
void   Display_cal_liquid_area(void)
    {
     Clr_Bar(620, 140, 800-620-10,160,COL_DEEPGRAY);
     
     Set_ASC_Lib(Arial12B_Font);
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     switch(user_cal_num)
       {
        case 1:
                Put_Bmp(630+50,  150,  liquidNull5566,0);
                Put_Data_Float(690, 190,"%.2f", standard_data_p811[which_standard][user_cal_point[0]]); 
                break;
        case 2:
                Put_Bmp(630+15,  150,  liquidNull5566,0); 
                Put_Bmp(630+70,  150,  liquidNull5566,0);    
                Put_Data_Float(655,    190,"%.2f", standard_data_p811[which_standard][user_cal_point[0]]); 
                Put_Data_Float(630+80, 190,"%.2f", standard_data_p811[which_standard][user_cal_point[1]]); 
                break;
        case 3:
                Put_Bmp(620,      150,  liquidNull5566,0); 
                Put_Bmp(630+45,   150,  liquidNull5566,0); 
                Put_Bmp(630+100,  150,  liquidNull5566,0);  
                Put_Data_Float(630,     190,"%.2f", standard_data_p811[which_standard][user_cal_point[0]]);
                Put_Data_Float(640+45,  190,"%.2f", standard_data_p811[which_standard][user_cal_point[1]]);
                Put_Data_Float(640+100, 190,"%.2f", standard_data_p811[which_standard][user_cal_point[2]]);
                break;
    
        case 4:
                Put_Bmp(630+15,  150,  liquidNull5566,0); 
                Put_Bmp(630+70,  150,  liquidNull5566,0);    
                Put_Bmp(630+15,  220,  liquidNull5566,0); 
                Put_Bmp(630+70,  220,  liquidNull5566,0);    
               
                Put_Data_Float(655,    190,"%.2f", standard_data_p811[which_standard][user_cal_point[0]]); 
                Put_Data_Float(630+80, 190,"%.2f", standard_data_p811[which_standard][user_cal_point[1]]); 
                Put_Data_Float(655,    260,"%.2f", standard_data_p811[which_standard][user_cal_point[2]]); 
                Put_Data_Float(630+80, 260,"%.2f", standard_data_p811[which_standard][user_cal_point[3]]); 
                break;
    
        case 5:
                Put_Bmp(620,      150,  liquidNull5566,0); 
                Put_Bmp(630+45,   150,  liquidNull5566,0); 
                Put_Bmp(630+100,  150,  liquidNull5566,0);  
                Put_Data_Float(630,     190,"%.2f", standard_data_p811[which_standard][user_cal_point[0]]);
                Put_Data_Float(640+45,  190,"%.2f", standard_data_p811[which_standard][user_cal_point[1]]);
                Put_Data_Float(640+100, 190,"%.2f", standard_data_p811[which_standard][user_cal_point[2]]);
                
                Put_Bmp(620,      220,  liquidNull5566,0); 
                Put_Bmp(630+45,   220,  liquidNull5566,0); 
                Put_Data_Float(630,     260,"%.2f", standard_data_p811[which_standard][user_cal_point[3]]);
                Put_Data_Float(640+40,  260,"%.2f", standard_data_p811[which_standard][user_cal_point[4]]);
                break;
                
        default:break;
       }
    }
    
//////////////////////////////////////////////////////////
//03 ��ʾ��ǰ�û��缫��״̬
//
//////////////////////////////////////////////////////////
void   Display_elec_class_area(void)
    {
     uint32 i;
     //ˮƽ�ָ���
     Clr_Bar(600,   300,  SCR_XSIZE-600,   3, COL_THINCYAN);  //�ָ���
     
     Clr_Bar(610,   345,  SCR_XSIZE-610-10, 50, COL_DEEPGRAY);  //�ָ���
     
     for(i=0;i<electrode_class;i++)
     Put_Bmp(620+33*i, 355,  fivestar3030,0); 
    
    }   
////////////////////////////////////////////////////////
//04 ��ָ��λ����ָ����ɫ��ʾ ���ֹ���״̬
//   ͬʱˢ��С����ʾ����
///////////////////////////////////////////////////////
static  void  Mode_Status_Display(void)
 {
 
  switch(current_mode)
  {
   case STAT_USER_CAL:
                  Clr_Bar(5, 90, 800-200-10,200,COL_BLACK);
                  
                  Clr_Bar(65,100, 150,  40,COL_THINGRAY);
                  Clr_Bar(290,100, 210, 40,COL_DEEPGRAY);
                  if(Get_Language_Type() == CHINESE)
                  Set_ASC_Lib(GulimChe48A_Font);
                  else
                  Set_ASC_Lib(Arial16B_Font);
                  
                  Set_Chr_Color(COL_THINYELLOW,COL_DEEPGRAY); 
                  Put_Line_Str(300, 110,Language_Str("����ƥ�����Һֵ","THE STANDARD PH"));
                  
                  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
                  switch(which_standard)
                  {
                   case CHINESE_S:  Put_Line_Str(90, 110,Language_Str("����У׼","CH-S CAL"));break;
                   case EUROPE_S:   Put_Line_Str(90, 110,Language_Str("ŷ��У׼","EU-S CAL"));break;
                   case AMERICAN_S: Put_Line_Str(90, 110,Language_Str("����У׼","AM-S CAL"));break;
                   default:break;
                  }
                  //��ʾPHֵ
                  Clr_Bar( 15,160, 250, 70,COL_THINGRAY);
                  Clr_Bar(265,160, 250, 70,COL_DEEPGRAY);
                  //
                  //Set_ASC_Lib(GulimChe48_Font);
                  Set_Chr_Color(COL_WHITE,COL_BLACK);
                  
                  Put_Line_Str(Language_Pos(40,60),  Language_Pos(230,255),Language_Str("��1��","1st"));
                  Put_Line_Str(Language_Pos(150,150),Language_Pos(230,255),Language_Str("PH","PH")); 
                 
                  Set_ASC_Lib(GulimChe48_Font);
                  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
   				  Put_Line_Str(320, 330,Language_Str("mV","mV")); 
    			  Put_Char(560, 345,'c');
    			  
    			  ///////////////////////������������
    			  Put_Button_Bar(300,240,300+110, 240+50,3,COL_THINGREEN);//����
                  Put_Button_Bar(430,240,430+110, 240+50,3,COL_THINRED);  //����
    			  Set_Chr_Color(COL_BLACK,COL_THINGREEN);
    			  Set_ASC_Lib(Arial16B_Font);
    			  Put_Line_Str(Language_Pos(330, 330), 255,Language_Str("����","SAVE"));
   				  Set_Chr_Color(COL_BLACK,COL_THINRED);
   				  Put_Line_Str(Language_Pos(460, 460), 255,Language_Str("����","ABORT"));      
                  break;                      
  
   case STAT_SYS_CAL:
   case STAT_NORMAL: 
                  Clr_Bar(5, 90, 800-200-10,200,COL_BLACK);
                  Set_ASC_Lib(GulimChe48_Font);
                  Set_Chr_Color(COL_WHITE,COL_BLACK);
    			  Put_Line_Str(525, 200,Language_Str("PH","PH"));  
    		      Set_Chr_Color(COL_BLACK,COL_THINGRAY);
   				  Put_Line_Str(320, 330,Language_Str("mV","mV"));
   				  if(TEMP_UNIT_OC == temp_unit) 
    			  Put_Char(560, 345,'c');    
                  else
                  Put_Char(560, 345,'f');
                  break;   
  
  default:
        break;
  }
  ////////////////////////////////////
  Stable_Status_Display(550,150);
 } 
///////////////////////////////////////////////////////////////
//�絼�ʵĽ���
////////////////////////////////////////////////////////
//04A ��ָ��λ����ָ����ɫ��ʾ ���ֹ���״̬
//    ͬʱˢ��С����ʾ����
///////////////////////////////////////////////////////
static  void  Mode_Status_Display2(void)
 {
 
  switch(current_mode)
  {
   case STAT_USER_CAL: //�絼���û�У׼ ������ʾ
                  Clr_Bar(5, 90, 800-200-10,200,COL_BLACK);
                  
                  Clr_Bar(310,100, 210, 40,COL_DEEPGRAY);
                  Set_ASC_Lib(GulimChe48A_Font);
                  Set_Chr_Color(COL_THINYELLOW,COL_DEEPGRAY); 
                  Put_Line_Str(320, 110,Language_Str("����ƥ�����ֵ","THE STANDARD DATA"));
                  
                  /*  ����ʾʹ�õ������ֱ�׼
                  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
                  switch(which_standard)
                  {
                   case CHINESE_S:  Put_Line_Str(90, 110,Language_Str("����У׼","CH-S CAL"));break;
                   case EUROPE_S:   Put_Line_Str(90, 110,Language_Str("ŷ��У׼","EU-S CAL"));break;
                   case AMERICAN_S: Put_Line_Str(90, 110,Language_Str("����У׼","AM-S CAL"));break;
                   default:break;
                  }
                  //��ʾPHֵ
                  Clr_Bar( 15,160, 250, 70,COL_THINGRAY);
                  Clr_Bar(265,160, 250, 70,COL_DEEPGRAY);
                  //
                  Set_ASC_Lib(GulimChe48_Font);
                  Set_Chr_Color(COL_WHITE,COL_BLACK);
                  
                  Put_Line_Str(40, 240,Language_Str("��1��","��1��"));
                  Put_Line_Str(150, 240,Language_Str("PH:","PH:")); 
                  */
                  
                  ///////////////////////������������
    			  Put_Button_Bar(300,240,300+110, 240+50,3,COL_THINGREEN);//����
                  Put_Button_Bar(430,240,430+110, 240+50,3,COL_THINRED);  //����
    			  Set_Chr_Color(COL_BLACK,COL_THINGREEN);
    			  Put_Line_Str(Language_Pos(330, 330), 255,Language_Str("����","SAVE"));
   				  Set_Chr_Color(COL_BLACK,COL_THINRED);
   				  Put_Line_Str(Language_Pos(460, 460), 255,Language_Str("����","ABORT"));      
                  break;                      
  
   case STAT_SYS_CAL:
   case STAT_NORMAL: 
                  Clr_Bar(5, 90, 800-200-10,200,COL_BLACK);
                  //��ʾ����ģʽ 
                  Clr_Bar(470, 90, 115,30,COL_WHITE);
                  Set_Chr_Color(COL_DEEPBLUE,COL_WHITE);
                  Set_ASC_Lib(Arial16B_Font);
   			      switch(DDLV_Mode)
                    {
                     case MODE_TIMER:
                     	  Put_Line_Str(Language_Pos(480, 500), 95,Language_Str("��ʱ����","TIMER"));  
                          
                          break;
                     case MODE_BALANCE:
                          Put_Line_Str(Language_Pos(480, 490), 95,Language_Str("ƽ�����","BALANCE"));  
                          Set_ASC_Lib(Arial16B_Font);
                          Clr_Bar(505, 160, 80,30,COL_WHITE);
                          Put_Data_Float(510,165,"%06.2f",balance_data); 
                          break;
                     case MODE_CONTINUE:
                     default:
                      	  Put_Line_Str(Language_Pos(480, 480), 95,Language_Str("��������","CONTINUE")); 
                      	  break;              
                    }
                  break;
   default:       break;               
  } 
  
         /////////////////////////display oc
    	  Set_ASC_Lib(GulimChe48_Font);
    	  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    	  if(TEMP_UNIT_OC == temp_unit)
    	  Put_Char(560, 345,'c');
    	  else
    	  Put_Char(560, 345,'f');    
  
          ////////////////////////////////////
          Stable_Status_Display(50,130);
 }  
 
////////////////////////////////////////////////////////////////
//**********************************************************************
//05A ���� ��:Display_Time_Pro()
//��������:   ʱ����ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void  Display_Time_Pro(RTCTime local_time_buf)
    {
     /////////////////////////////////////////////////////////////*/ 
     Set_ASC_Lib(Arial20B_Font);
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);
    
     Put_Data(620,           15, "%04d", local_time_buf.RTC_Year);
	 Put_Char(690,           15, 0x2d);
	 Put_Data(705,           15, "%02d", local_time_buf.RTC_Mon);
	 Put_Char(740,           15, 0x2d);
	 Put_Data(750,           15, "%02d", local_time_buf.RTC_Day);
     //display time
	 Put_Data(653,           45,"%02d", local_time_buf.RTC_Hour);
	 Put_Char(690,           45,0x3a);
     Put_Data(705,           45,"%02d", local_time_buf.RTC_Min);
	 Put_Char(740,           45,0x3a);
	 Put_Data(750,           45,"%02d", local_time_buf.RTC_Sec);
    }

///////////////////////////////////////////////////////////////////////
//**********************************************************************
//05 ���� ��:Oper_Main_Assi_Pro
//��������:����������ĸ���������  ÿ300msִ��һ��
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Oper_Main_Assi_Pro(void)
{
  RTCTime local_time_buf;
  uint16   i;
 
  ///////////////////
  if(Sec_Change_Flag)
   {
     Sec_Change_Flag = FALSE;
     Display_Time_Pro(RTCGetTime());
     /////////////////////////////////////ÿ�붨ʱ������1
     if(MODE_TIMER == DDLV_Mode)
        {
         if(measure_timer_sec_real==0)
          {
           measure_timer_sec_real = measure_timer_sec;
           //save
           Save_RecDDLV();
           //�Զ���������ģʽ
           DDLV_Mode = MODE_CONTINUE;
           
          }
         else
           measure_timer_sec_real--;
           
         //////////////////////////////////
         Set_ASC_Lib(Arial16B_Font);
         Clr_Bar(535, 125, 50,30,COL_WHITE);
         Set_Chr_Color(COL_BLACK,COL_WHITE);
         Put_Data(545,130,"%03d",measure_timer_sec_real); 
            
        }
     ///////////////////////////////////
     if(MODE_BALANCE == DDLV_Mode)
        {
         //��ǰ�����������������ݷ�Χ�ڣ�ʱ��ſ��Լ�1
         if(ddlv_data == balance_data)
          {
            if(0 == balance_sec_real)
              {
               balance_sec_real = balance_sec;
               //save
               Save_RecDDLV();
               //�Զ���������ģʽ
               DDLV_Mode = MODE_CONTINUE;
              }
            else
              {
               balance_sec_real--;
              }  
          }
         else
            balance_sec_real = balance_sec;
            
         Set_ASC_Lib(Arial16B_Font);
         Clr_Bar(535, 125, 50,30,COL_WHITE);
         Set_Chr_Color(COL_BLACK,COL_WHITE);
         Put_Data(545,130,"%03d",balance_sec_real);     
        }
        
     /////////////////////////////////////
	 //������Ϣ ��ʾ�¶�   
     Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
	 Set_Chr_Color(COL_WHITE,COL_BLACK);
	 Fill_Circle(570,330,15);
	 Set_ASC_Lib(Arial16B_Font);	
	 if(temp_float_ptc==TEMP_SENSOR_NC_VALUE)
	 {
	  manual_temp_comp_flag = TRUE; 
	  Put_Char(565,320,'M');
	  Set_ASC_Lib(GulimChe48A_Font);
	  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
	  if(TEMP_UNIT_OC == temp_unit)
	  Put_Data_Float(420,320,"%4.1f",(float)temp_comp_manual); //�˴����ֶ�����Ĳ����¶�
	  else
	  Put_Data(430,320,"%3d",(uint32)(temp_comp_manual*9/5+32.5));
	  ////////////////////////////////////////////////////
	  /*
	  if((temp_comp_manual<TEMP_SENSOR_LOW)||(temp_comp_manual>TEMP_SENSOR_HIGH))
	     Log_Alm(&warn_temp_out,ALM_LEV_WARN);
	  else
	     Del_Alm(&warn_temp_out,ALM_LEV_WARN);
	  */
	 }
	 else
	 {
	  manual_temp_comp_flag = FALSE;
	  Put_Char(565,320,'A');
	  Set_ASC_Lib(GulimChe48A_Font);
	  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
	  //Put_Data_Float(420,320,"%4.1f",23.4);
	  if(TEMP_UNIT_OC == temp_unit)
	  Put_Data_Float(420,320,"%4.1f",temp_float_ptc);
	  else
	  Put_Data(430,320,"%3d",(uint32)(temp_float_ptc*9/5+32.5));
	  ////////////////////////////////////////////////////
	  /*
	  if((temp_float_ptc<TEMP_SENSOR_LOW)||(temp_float_ptc>TEMP_SENSOR_HIGH))
	     Log_Alm(&warn_temp_out,ALM_LEV_WARN);
	  else
	     Del_Alm(&warn_temp_out,ALM_LEV_WARN);
      */
     }
	
   }
   Put_Alm_Mess();
}

///////////////////////////////////////////////////////////////////////
//**********************************************************************
//06  ���� ��:Oper_Assi_Pro ֻ��ʾʱ��
//��������:   ������������ĸ���������  ÿ300msִ��һ��
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Oper_Assi_Pro(void)
{
  ///////////////////
  if(Sec_Change_Flag)
   {
     Sec_Change_Flag = FALSE;
     Display_Time_Pro(RTCGetTime());
   }  
}
///////////////////////////////////////////////////////////
//06A   �պ���
//
///////////////////////////////////////////////////////////
static void Oper_Assi_Null(void)
{
  ///////////////////
 
}

////////////////////////////////////////////////////
//07  ����״̬�µİ����ж�
//
///////////////////////////////////////////////////
static uint32  Action_SleepCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<1;i++)
	    {
	     if((abs((*p).X_Position-x)< 60) && (abs((*p).Y_Position-y)< 40))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	     //////////////////////////////////////////////////////////�а�������
	  if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	     
	  if(Key_Status == KEY_DO_START)  
	     {
	      Key_Status = KEY_DO_OVER; 
	      Key_Valid_Sound();
	     }
	      
	  return(Key_Code);   
	 }
/*///////////////////////////////////////////////////
/*08  �������µļ����ж� ����10+2+4������
//
//////////////////////////////////////////////////*/
static uint32  Action_MainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<8;i++)
	   {
	     if((abs((*p).X_Position-x)< 60) && (abs((*p).Y_Position-y)< 40))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	   
	  //////////////////////////////////////////////////////////�а�������
	  if(Key_Status == KEY_PRESS)
	     {
	      if (Get_Language_Type() == CHINESE)
	      { switch(Key_Code)
	       {
	        case 1: Put_Bmp(133*0+15,  405,  sysconfig8055,1); break;
            case 2: Put_Bmp(133*1+15,  405,     recall8055,1); break;
            case 3: Put_Bmp(133*2+15,  405,        cal8055,1); break;
            case 4: Put_Bmp(133*3+15,  405,       save8055,1); break;
            case 5: Put_Bmp(133*4+15,  405,    printer8055,1); break;
            case 6: Put_Bmp(133*5+15,  405,      sleep8055,1); break; 
            default:break;
           } 
          }
          else
          {
           switch(Key_Code)
	       {
	        case 1: Put_Bmp(133*0+15,  405,  sysconfigen8055,1); break;
            case 2: Put_Bmp(133*1+15,  405,     recallen8055,1); break;
            case 3: Put_Bmp(133*2+15,  405,        calen8055,1); break;
            case 4: Put_Bmp(133*3+15,  405,       saveen8055,1); break;
            case 5: Put_Bmp(133*4+15,  405,    printeren8055,1); break;
            case 6: Put_Bmp(133*5+15,  405,        sleep8055,1); break; 
            default:break;
           } 
          }
          Key_Code = 0xAA55;
	     }
 	  //���ӷ�����ʾ ֱ�������ɿ� 
	  if(Key_Status == KEY_DO_START)  
	    {
	     Key_Status = KEY_DO_OVER; 
	     Key_Valid_Sound();
	     if (Get_Language_Type() == CHINESE)
	     { switch(Key_Code)
	      {
	       case 1: Put_Bmp(133*0+15,  405,  sysconfig8055,0); break;
           case 2: Put_Bmp(133*1+15,  405,     recall8055,0); break;
           case 3: Put_Bmp(133*2+15,  405,        cal8055,0); break;
           case 4: Put_Bmp(133*3+15,  405,       save8055,0); break;
           case 5: Put_Bmp(133*4+15,  405,    printer8055,0); break;
           case 6: Put_Bmp(133*5+15,  405,      sleep8055,0); break; 
           default:break;
          }
         }
         else
         {
         switch(Key_Code)
	      {
	       case 1: Put_Bmp(133*0+15,  405,  sysconfigen8055,0); break;
           case 2: Put_Bmp(133*1+15,  405,     recallen8055,0); break;
           case 3: Put_Bmp(133*2+15,  405,        calen8055,0); break;
           case 4: Put_Bmp(133*3+15,  405,       saveen8055,0); break;
           case 5: Put_Bmp(133*4+15,  405,    printeren8055,0); break;
           case 6: Put_Bmp(133*5+15,  405,      sleep8055,0); break; 
           default:break;
          }  
         } 
        }   
	  /////////////////////////////////////////////// 
	  return(Key_Code);
	}

//////////////////////////////////////////////////////
//09  ǿ�ƽ���У׼״̬
//    �����ص�����ģʽ
//////////////////////////////////////////////////////
void  Return_Normal_Status(void)
    {
     point2_cal_start = FALSE;
     current_mode     = STAT_NORMAL;
    }
    
void  Clear_User_Cal(void)
  {
   uint8 eeprom_tx_buf[16];
         
         eeprom_tx_buf[0] = 0x0;  //�����û�У׼����
         eeprom_tx_buf[1] = 0x0;
  	     eeprom_tx_buf[2] = 0x0;
  	     eeprom_tx_buf[3] = 0x0; 
  	     #if SDJ_SOFT
  	     I2C_Send_Str(SYS24C08, EEP_USER_CAL_FLAG,     eeprom_tx_buf, 4);
  	     #else
  	     I2C_Send_Str(SYS24C08, EEP_DDLV_USER_CAL1+12, eeprom_tx_buf, 4);  	     
  	     #endif
  	     
  	     user_cal_ok = FALSE;
  }  	            
//////////////////////////////////////////////////////
//10  �� ���� ��ť�� ����� ����
//    USER_CAL_STEP1   ɾ����ǰ�� У׼�ɹ�
//    USER_CAL_STEP234 �����Ѿ�У׼�������
//
//////////////////////////////////////////////////////
void  End_UserCal_Pro(void)
    {
     //��¼�����˼���У׼��
     uint8 eeprom_tx_buf[16];
     uint32 i;
     
     switch(point2_cal_step)
       {
        case USER_CAL_STEP1: break;
        
        case USER_CAL_STEP2:
        case USER_CAL_STEP3:
        case USER_CAL_STEP4:
        case USER_CAL_STEP5:
        case USER_CAL_END:
               user_cal_num = point2_cal_step-1;
               sort_cal_buf(user_cal_num);
               save_cal_data(user_cal_num);
               //Get_User_Coef123();
               break;
        default:
               break;   
       }
       
       
       /////////////��������� STEP1�׶� ���� У׼OK ��־
       if(USER_CAL_STEP1 != point2_cal_step)          
             {
               eeprom_tx_buf[0] = user_cal_num;
  	           eeprom_tx_buf[1] = 0x55;
  	           eeprom_tx_buf[2] = 0xaa;
  	           eeprom_tx_buf[3] = 0x55; 
  	           //////////////////////Ĭ���ǰ��մӵ͵�������� 
  	           eeprom_tx_buf[8]  = user_cal_point[0];
  	           eeprom_tx_buf[9]  = user_cal_point[1];
  	           eeprom_tx_buf[10] = user_cal_point[2];
  	           eeprom_tx_buf[11] = user_cal_point[3]; 
  	           eeprom_tx_buf[12] = user_cal_point[4];
  	           
  	           I2C_Send_Str(SYS24C08, EEP_USER_CAL_FLAG, eeprom_tx_buf, 16);	
  	           ////////////////////////use new coef
  	           for(i=0;i<10000;i++)
  	            {;}
  	           
  	           user_cal_ok = 1;
  	           Get_User_Coef123(); 
  	           ///////////////////////////������ʾ����
  	           Display_cal_liquid_area();
  	           Display_elec_class_area();   
               
             }
      ////////////////////////////////////////     
      Return_Normal_Status();
    }
    
//////////////////////////////////////////////
//11 ϵͳУ׼ ���̵� �Զ�����
//    
//////////////////////////////////////////////////////////////    
void  Sys_Cal_Pro(void)
  {  
    uint32        i;
    uint8         eeprom_tx_buf[16];
   
    if(SYS_CAL_ZER0A == point2_cal_step)
       {
               zero_data[0] = Get_Filter_Data(2); //��ȡ0mvʱ���CH3 
               point2_cal_step++;switch_flag=1; 
               stable_manual_break();
               
       }
    else if(SYS_CAL_ZER0B == point2_cal_step)
       {
             
               zero_data[1] = Get_Filter_Data(3); //��ȡ0mvʱ���CH4
               zero_data[2] = cpu_ch_adc3;        //��ȡ0mvʱ���CPU CH3
               zero_data[3] = cpu_ch_adc4;        //��ȡ0mvʱ���CPU CH4
               for(i=0;i<4;i++)
                {
                 eeprom_tx_buf[4*i+0] = (uint8)(zero_data[i]>>24);
  	             eeprom_tx_buf[4*i+1] = (uint8)((zero_data[i]>>16)&0xff);
  	             eeprom_tx_buf[4*i+2] = (uint8)((zero_data[i]>>8)&0xff);
  	             eeprom_tx_buf[4*i+3] = (uint8)(zero_data[i]&0xff); 
  	            }
  	           
  	           I2C_Send_Str(SYS24C08, EEP_ZERO_ADDR1, eeprom_tx_buf, 16);	         
               point2_cal_step++;switch_flag=1;
               stable_manual_break();
               
       }   
     else if((SYS_CAL_LOW1A == point2_cal_step)&&(cpu_ch_adc4>zero_data[3]+50))
       {
               low1_data[0] = Get_Filter_Data(0);  //��ȡ100mvʱ���CH1 ����300mv-2000mv
               point2_cal_step++;switch_flag=1;
               stable_manual_break();
        
        }
     else if(SYS_CAL_LOW1B == point2_cal_step)
       {
                low1_data[1] = Get_Filter_Data(3);  //��ȡ100mvʱ���CH4 ����0mv-300mv
                low1_data[2] = 0;
                low1_data[3] = cpu_ch_adc4;         
                for(i=0;i<4;i++)
                {
                 eeprom_tx_buf[4*i+0] = (uint8)(low1_data[i]>>24);
  	             eeprom_tx_buf[4*i+1] = (uint8)((low1_data[i]>>16)&0xff);
  	             eeprom_tx_buf[4*i+2] = (uint8)((low1_data[i]>>8)&0xff);
  	             eeprom_tx_buf[4*i+3] = (uint8)(low1_data[i]&0xff); 
  	            }
  	           
  	           I2C_Send_Str(SYS24C08, EEP_LOW1_ADDR1, eeprom_tx_buf, 16);	   
               point2_cal_step++;switch_flag=1;
               stable_manual_break();
        
        }   
     else if((SYS_CAL_LOW2A == point2_cal_step)&&(cpu_ch_adc3>zero_data[2]+50))
       {
               low2_data[0] =  Get_Filter_Data(1); //��ȡ-100mvʱ���CH2 ����-100mv-(-2000mv)
               point2_cal_step++;switch_flag=1;
               stable_manual_break();
                          
        } 
     else if(SYS_CAL_LOW2B == point2_cal_step)
       {
                low2_data[1] =  Get_Filter_Data(2); //��ȡ-100mvʱ���CH3 ����-100mv-0mv 
                low2_data[2] = cpu_ch_adc3;         //��ȡ-100mvʱ���CPU CH3
                low2_data[3] = 0;         
                
                for(i=0;i<4;i++)
                {
                 eeprom_tx_buf[4*i+0] = (uint8)(low2_data[i]>>24);
  	             eeprom_tx_buf[4*i+1] = (uint8)((low2_data[i]>>16)&0xff);
  	             eeprom_tx_buf[4*i+2] = (uint8)((low2_data[i]>>8)&0xff);
  	             eeprom_tx_buf[4*i+3] = (uint8)(low2_data[i]&0xff); 
  	            }
  	           
  	           I2C_Send_Str(SYS24C08, EEP_LOW2_ADDR1, eeprom_tx_buf, 16);	
               point2_cal_step++;switch_flag=1;
               stable_manual_break();
                          
        }   
      else if((SYS_CAL_FULL1 == point2_cal_step)&&(Get_Filter_Data(0)>500000))
       {
       
                full1_data[0] =  Get_Filter_Data(0); //��ȡ1400mvʱ���CH1
                full1_data[1] = 0;  
                full1_data[2] = 0;                   //�����ٻ�ȡCPU AD
                full1_data[3] = 0;         
                
                for(i=0;i<4;i++)
                {
                 eeprom_tx_buf[4*i+0] = (uint8)(full1_data[i]>>24);
  	             eeprom_tx_buf[4*i+1] = (uint8)((full1_data[i]>>16)&0xff);
  	             eeprom_tx_buf[4*i+2] = (uint8)((full1_data[i]>>8)&0xff);
  	             eeprom_tx_buf[4*i+3] = (uint8)(full1_data[i]&0xff); 
  	            }
  	           
  	           I2C_Send_Str(SYS24C08, EEP_FULL1_ADDR1, eeprom_tx_buf, 16);	
               point2_cal_step++;switch_flag=1;
               stable_manual_break();
                          
        } 
       else if((SYS_CAL_FULL2 == point2_cal_step)&&(Get_Filter_Data(1)>500000))//
       {
       
                full2_data[0] = Get_Filter_Data(1);  //��ȡ-1400mvʱ���CH2 
                full2_data[1] = 0;  
                full2_data[2] = 0;                   //�����ٻ�ȡCPU AD
                full2_data[3] = 0;         
                
                for(i=0;i<4;i++)
                {
                 eeprom_tx_buf[4*i+0] = (uint8)(full2_data[i]>>24);
  	             eeprom_tx_buf[4*i+1] = (uint8)((full2_data[i]>>16)&0xff);
  	             eeprom_tx_buf[4*i+2] = (uint8)((full2_data[i]>>8)&0xff);
  	             eeprom_tx_buf[4*i+3] = (uint8)(full2_data[i]&0xff); 
  	            }
  	           
  	           I2C_Send_Str(SYS24C08, EEP_FULL2_ADDR1, eeprom_tx_buf, 16);	
               point2_cal_step++;
               //switch_flag=1;
               //stable_manual_break();
                          
        }      
      else if((SYS_CAL_FULL3 == point2_cal_step)&&(Get_Filter_Data(1)>700000))//
       {
       
                full3_data[0] = Get_Filter_Data(1);  //��ȡ-2000mvʱ���CH2 
                full3_data[1] = 0;  
                full3_data[2] = 0;                   //�����ٻ�ȡCPU AD
                full3_data[3] = 0;         
                
                for(i=0;i<4;i++)
                {
                 eeprom_tx_buf[4*i+0] = (uint8)(full3_data[i]>>24);
  	             eeprom_tx_buf[4*i+1] = (uint8)((full3_data[i]>>16)&0xff);
  	             eeprom_tx_buf[4*i+2] = (uint8)((full3_data[i]>>8)&0xff);
  	             eeprom_tx_buf[4*i+3] = (uint8)(full3_data[i]&0xff); 
  	            }
  	           
  	           I2C_Send_Str(SYS24C08, EEP_FULL3_ADDR1, eeprom_tx_buf, 16);	
               point2_cal_step++;
                                      
        }         
     else if(SYS_CAL_OVER == point2_cal_step)
       {
           eeprom_tx_buf[0] = 0x55;
  	       eeprom_tx_buf[1] = 0xaa;
  	       eeprom_tx_buf[2] = 0xaa;
  	       eeprom_tx_buf[3] = 0x55; 
           I2C_Send_Str(SYS24C08, EEP_FULL4_ADDR1, eeprom_tx_buf, 4);	
           /////////////////////////////////
           sys_cal_ok    = TRUE;
           Return_Normal_Status();
       }      
   }
   

//////////////////////////////////////////////
//11 ϵͳУ׼ ���̵� �Զ�����
//    
//////////////////////////////////////////////////////////////      
void  Sys_Cal_Pro2(void)
  {  
       
    uint32        i;
    uint8         eeprom_tx_buf[16];
    
    uint32   tmp = Get_Filter_ADS1100Data(); //��ȡ100ŷʱ�������
    switch(point2_cal_step)
       {
        case  SYS_CAL_4A:
        case  SYS_CAL_5A:
        case  SYS_CAL_6A:
                         
        case  SYS_CAL_1A:
        case  SYS_CAL_2A:
        case  SYS_CAL_3A:
       
                         if((tmp>DEFAULT_CAL_DATA[(point2_cal_step-1)/2][0]*0.7) && (tmp<DEFAULT_CAL_DATA[(point2_cal_step-1)/2][0]*1.3))
                          {  
                           cal_ddlv_buf[(point2_cal_step-1)/2][0] = tmp;
                         
                           if(SYS_CAL_1A == point2_cal_step) //У׼���һ�����ɾ��У׼��־
                            { //������ǰ��У׼��Ϣ
                             for(i=0;i<16;i++)
                             eeprom_tx_buf[i]  = 0x0; 
  	                         Write_EEPROM(EEP_DDLV_CHECK1,eeprom_tx_buf, 16); 
  	                         for(i=0;i<20000;i++){;}	
                            }
                           //////////////////////////////////////������һ��״̬�궨   
                           point2_cal_step++; switch_flag=1;
                           stable_manual_break();    
                          } 
                          break;
        case  SYS_CAL_4B:
        case  SYS_CAL_5B:
        case  SYS_CAL_6B:
                         
        case  SYS_CAL_1B:
        case  SYS_CAL_2B:
        case  SYS_CAL_3B:
        
                         if((tmp>DEFAULT_CAL_DATA[point2_cal_step/2-1][1]*0.7) && (tmp<DEFAULT_CAL_DATA[point2_cal_step/2-1][1]*1.3))
                          {  
                           cal_ddlv_buf[point2_cal_step/2-1][1] = tmp;
                           //����һ��
                            for(i=0;i<2;i++)
                           {
                            eeprom_tx_buf[4*i+0] = (uint8) (cal_ddlv_buf[(point2_cal_step-1)/2][i]>>24);
  	                        eeprom_tx_buf[4*i+1] = (uint8)((cal_ddlv_buf[(point2_cal_step-1)/2][i]>>16)&0xff);
  	                        eeprom_tx_buf[4*i+2] = (uint8)((cal_ddlv_buf[(point2_cal_step-1)/2][i]>>8)&0xff);
  	                        eeprom_tx_buf[4*i+3] = (uint8) (cal_ddlv_buf[(point2_cal_step-1)/2][i]&0xff); 
  	                       }
  	                       Write_EEPROM(EEP_DDLV_SYSCAL1A + 0x10*((point2_cal_step-1)/2),eeprom_tx_buf, 8); 
  	                       for(i=0;i<20000;i++){;}	
                           //////////////////////////////////////������һ��״̬�궨   
                           point2_cal_step++; switch_flag=1;
                           stable_manual_break();    
                          
                          } 
                          break;
      
              /////////////////////////////////////////////
        case  SYS_CAL_END:
              /////////////////5MУ׼��Ϻ��ֶ����ز���ģʽ  
               eeprom_tx_buf[0] = 0x88;
  	           eeprom_tx_buf[1] = 0x99;
  	           eeprom_tx_buf[2] = 0xaa;
  	           eeprom_tx_buf[3] = 0xbb;  
  	               
  	           Write_EEPROM(EEP_DDLV_CHECK1,eeprom_tx_buf, 4); 
  	           for(i=0;i<20000;i++){;}	  
  	           ////////////////////////��У׼OK��־
  	                 
               cal_calculate();
               sys_cal_ok    = TRUE;
               Return_Normal_Status();
              break;
  
        default:
              break;
       }
  
             
   }   
//////////////////////////////////////////////////////////////////////
//12  �� save ��ť�� ����� ���� ֻ�����ȶ��󰴴˰�ť��������
//
//����ʹ��1�֣�2�� ���� 3�� ��ҺУ׼
///////////////////////////////////////////////////////////////////////
void  User_Cal_Pro(void)
   {
    uint32 i,j;  
    ////////////////У׼ʱ��Ķ���ȷ�� ������ظ���Һ�� ֻ����һ��
    switch(point2_cal_step)
       {
        case USER_CAL_STEP1:
              for(i=0;i<5;i++)
              user_cal_point[i] = 0xff;
             
              user_cal_phdata[point2_cal_step-1].rec_float = ph_data;    //��ȡ��ǰ��Һ������
              user_cal_point[point2_cal_step-1]            = match_point; 
              point2_cal_step++;
              match_flag = FALSE;
              stable_manual_break();
              break;                    
        case USER_CAL_STEP2:
        case USER_CAL_STEP3:
        case USER_CAL_STEP4:
        case USER_CAL_STEP5:
               ///////////////////////���������ظ�������
               for(i=0;i<5;i++)
               {
                if(match_point == user_cal_point[i])
                break;
               }
              
              if(i == 5)
               {
                user_cal_phdata[point2_cal_step-1].rec_float = ph_data;    //��ȡ��ǰ��Һ������
                user_cal_point[point2_cal_step-1]            = match_point; 
                point2_cal_step++;
              
                match_flag = FALSE;
                stable_manual_break();
               } 
             
              break;
        case USER_CAL_END:
              End_UserCal_Pro();
              break;
        default:break;                
       }
    /////////////////////////////////////////////////
    /*
    Set_ASC_Lib(GulimChe48_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Data(64, 240, "%d",point2_cal_step);
    */
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    if(Get_Language_Type() == CHINESE)
       Set_ASC_Lib(GulimChe48A_Font);
    else
       Set_ASC_Lib(Arial16B_Font);
        
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    switch(point2_cal_step)
     {
      case USER_CAL_STEP1:Put_Line_Str(Language_Pos(40,60),  Language_Pos(230,255),Language_Str("��1��","1st"));break;
      case USER_CAL_STEP2:Put_Line_Str(Language_Pos(40,60),  Language_Pos(230,255),Language_Str("��2��","2nd"));break;
      case USER_CAL_STEP3:Put_Line_Str(Language_Pos(40,60),  Language_Pos(230,255),Language_Str("��3��","3rd"));break;
      case USER_CAL_STEP4:Put_Line_Str(Language_Pos(40,60),  Language_Pos(230,255),Language_Str("��4��","4th"));break;
      case USER_CAL_STEP5:Put_Line_Str(Language_Pos(40,60),  Language_Pos(230,255),Language_Str("��5��","5th"));break;
      default:break;
     } 
   }
   
//////////////////////////////////////////////////////////////////////
//12  �� save ��ť�� ����� ���� ֻ�����ȶ��󰴴˰�ť��������
//
//ֻʹ��1�� ��ҺУ׼
///////////////////////////////////////////////////////////////////////   
  void  Save_UserCal_Pro(void)
  {  ////////////////У׼ʱ��Ķ���ȷ��
     uint8 eeprom_tx_buf[4];
     un_rec_data tmp;
   
     tmp.rec_float = DDLV_standard_data[which_standard][match_point]/ddlv_data; //����б�� �����ݾ��ǵ缫����
     elec_cons = tmp.rec_float;
     
     eeprom_tx_buf[0]  = (uint8)((tmp.rec_uint32>>24)&0xff);
  	 eeprom_tx_buf[1]  = (uint8)((tmp.rec_uint32>>16)&0xff);
  	 eeprom_tx_buf[2]  = (uint8)((tmp.rec_uint32>>8)&0xff);
  	 eeprom_tx_buf[3]  = (uint8)(tmp.rec_uint32&0xff); 
  	 //�滻�� �˲���  	 	           
  	 I2C_Send_Str(SYS24C08, EEP_DDLV_CONS_ADDR, eeprom_tx_buf,4);
  }
  
//**********************************************************************
//13 ����  ��:Sleep_Key_Pro
//��������:���������µİ�������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
static  sint32 Sleep_Key_Pro(uint32 key_c,sint32 op)
   {
    sint32 op_id;
    uint32 i;
    op_id = op;  
    no_key_time = 0;      //����ģʽ��ʱ����  
    //if(Key_Status != KEY_DO_START)
    // {
    // return(op_id);
    //}
    i = Action_SleepCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(1 == i)
        {
         Set_LCD_Brightness(lcd_brightness); //���ص���������
         op_id = MENU_OP_OPER;           //����
        }
    return(op_id);  
   }
//**********************************************************************
//14 ���� ��:Main_Key_Pro
//��������:���������µ�ѡ���
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
static  sint32 Main_Key_Pro(uint32 key_c,sint32 op)
{
    
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //{
    // return(op_id);
    //}
    i = Action_MainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1://
  	           op_id = MENU_SYSTEM_CONF_OPER;
  	           break;
    	case 2://
    	       op_id = MENU_DATA_CALL_OPER;
    	       break;
    	case 3://user cal
    	       
    	       Enter_UserPoint_Cal();
    	       break;
    	case 4://save
    	       if(current_mode == STAT_NORMAL)  //�洢
    	          {
    	           #if SDJ_SOFT
    	             Save_Rec();
    	           #else 
    	             Save_RecDDLV();
    	           #endif  
    	          } 
    	       break;
    	case 5://print
               //����ӡ�����Ƿ�����
               if((0 == print_busy) &&(1==Uart_Printer_Flag))
               {
                 print_busy = 1;
                 UART_PrintData(); 
               }
    	       break;
    	case 6:
    	       if(current_mode==STAT_SYS_CAL)
    	          Return_Normal_Status();
    	       else if(current_mode==STAT_USER_CAL)
    	          {
    	           Clear_User_Cal();
    	           Return_Normal_Status();
    	          } 
    	       else
    	       op_id = MENU_SLEEP_OP_OPER;
    	       break;
    	case 7://save in cal mode
    	       if((TRUE==match_flag)&&(current_mode == STAT_USER_CAL)&&((TRUE == stable_flag)))  //ƥ�䵽��Һ�����ȶ��� save��������
    	       #if SDJ_SOFT
    	       User_Cal_Pro();
    	       #else
    	       Save_UserCal_Pro();
    	       #endif
    	       break;
    	case 8://exit in cal mode
    	       #if SDJ_SOFT
               End_UserCal_Pro();
               #else
               Return_Normal_Status();
               #endif
    	       break;     
    	default:break;
   	   }
   	  //Key_Status = KEY_DO_OVER;
    }
    return(op_id);    
}

//**********************************************************************
//15 ������:Oper_Sleep_Pro               
//��������:����ģʽ�½�����ʾ
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void  Oper_Sleep_Pro(void)
     {
      Display_Sleep_Time();   
     }
     
//**********************************************************************
//16���� ��:Oper_Disp_Pro               
//��������:����ģʽ�µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Oper_Disp_Pro(void)
{
   static uint32 stable_change_tmp,mode_change_tmp,negative_change_tmp;
  
   //////////////////////////////////////////����״̬��ʾ
     if(mode_change_tmp != current_mode)
       {
        mode_change_tmp = current_mode;
        #if SDJ_SOFT
        Mode_Status_Display();
        #else
        Mode_Status_Display2();
        #endif
        }
   /////////////////////////////////////////�ȶ�״̬��ʾ       
 	 if(stable_change_tmp != stable_flag)
  	   {  
   	    stable_change_tmp  = stable_flag;
   	    #if SDJ_SOFT
   	    Stable_Status_Display(550,150);
   	    #else
   	    Stable_Status_Display(50,130);
   	    #endif
  		} 
  		   
   /////////////////////////
   #if SDJ_SOFT
   New_Data_Display();
   #else 
   New_Data_Display2();
   #endif
}

//**********************************************************************
//17������:Set_Menu_Op
//��������:���ú�����ǰ�����Ľ���
//��ڲ���:cur_menu_op
//���ڲ���:��
//**********************************************************************
void Set_Menu_Op(sint32 cur_menu_op)
{
    menu_op = cur_menu_op;
}
/**********************************************************************
//18 ��������ĳ�ʼ��
//
//////////////////////////////////////////////////////////////////////*/
void   Put_Sleep_Interface(sint32 op)
    {
     uint32 xy[1][2];
     
     Set_LCD_Brightness(SLEEP_BRIGHTNESS);
     ////////////////////////////////////����״̬�°ѱ������õ��ϰ�������
     Display_Sleep_Area();
     
     xy[0][0] =  SCR_XSIZE-120+110/2;
     xy[0][1] =  410+ 60/2;
     Create_ButtonList(xy,1);    
    }
//**********************************************************************
//19 ����  ��:Put_Oper_Interface
//��������:��ʾ���� ģʽ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void Put_Oper_Interface(sint32 op)
{
    uint32 xy[8][2];
    uint32 i,j,k;  

    Clr_Bar(0,0,SCR_XSIZE,  SCR_YSIZE,   COL_THINCYAN);      //ALL black
    Clr_Bar(3,3,SCR_XSIZE-6,SCR_YSIZE-6, COL_THINBLUE);      //ALL black
    ////ˮƽ
    Clr_Bar(0,        80,      SCR_XSIZE,   3, COL_THINCYAN);  //�ָ���
    Clr_Bar(0,       400,      SCR_XSIZE,   3, COL_THINCYAN);  //�ָ���
    Clr_Bar(0,   400-100,  SCR_XSIZE-200,   3, COL_THINCYAN);  //�ָ���
   
    
    ////��ֱ
    Clr_Bar(400,     300,             3,100, COL_THINCYAN);  //�ָ���
    Clr_Bar(600,      80,             3,320, COL_THINCYAN);  //�ָ���
 
    //��ʾLOGO
    if(TRUE == LOGO_flag)
    Put_Bmp(5,6,logo21070,0);
    //��ʾ���������ƣ�
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINBLUE);
    #if SDJ_SOFT
    if(TRUE == LOGO_flag)
    {
      if(1 == model_display)
       { 
        if(TRUE == version_811)
         Put_Str(Language_Pos(90,90),    15,Language_Str("P811","P811")); //DISPLAY model
         else
         Put_Str(Language_Pos(90,90),    15,Language_Str("P801","P801"));
        //////////////////////////////////////////////////////////////////
         Put_Str(Language_Pos(90,90),    45,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(135,135), 45,"%03d", (uint16)user_id);            //DISPLAY oper
       }
      else
       {
         Put_Str(Language_Pos(90,90),    30,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(135,135), 30,"%03d", (uint16)user_id);            //DISPLAY oper
       } 
    }
    else
    {
      if(1 == model_display)
       { 
        if(TRUE == version_811)
         Put_Str(Language_Pos(10,10),    15,Language_Str("P811","P811")); //DISPLAY model
         else
         Put_Str(Language_Pos(10,10),    15,Language_Str("P801","P801"));
        //////////////////////////////////////////////////////////////////
         Put_Str(Language_Pos(10,10),    45,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(55,55),   45,"%03d", (uint16)user_id);            //DISPLAY oper
       }
      else
       {
         Put_Str(Language_Pos(10,10),    30,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(55,55),   30,"%03d", (uint16)user_id);            //DISPLAY oper
       } 
    
    }
    
    #else
    
     if(TRUE == LOGO_flag)
    {
      if(1 == model_display)
       { 
        if(TRUE == version_811)
         Put_Str(Language_Pos(90,90),    15,Language_Str("P812","P812")); //DISPLAY model
         else
         Put_Str(Language_Pos(90,90),    15,Language_Str("P802","P802"));
        //////////////////////////////////////////////////////////////////
         Put_Str(Language_Pos(90,90),    45,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(135,135), 45,"%03d", (uint16)user_id);            //DISPLAY oper
       }
      else
       {
         Put_Str(Language_Pos(90,90),    30,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(135,135), 30,"%03d", (uint16)user_id);            //DISPLAY oper
       } 
    }
    else
    {
      if(1 == model_display)
       { 
        if(TRUE == version_811)
         Put_Str(Language_Pos(10,10),    15,Language_Str("P812","P812")); //DISPLAY model
         else
         Put_Str(Language_Pos(10,10),    15,Language_Str("P802","P802"));
        //////////////////////////////////////////////////////////////////
         Put_Str(Language_Pos(10,10),    45,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(55,55),   45,"%03d", (uint16)user_id);            //DISPLAY oper
       }
      else
       {
         Put_Str(Language_Pos(10,10),    30,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(55,55),   30,"%03d", (uint16)user_id);            //DISPLAY oper
       } 
    
    }
    #endif
    
    //����ʾ����ڵװ���
    Clr_Bar(3,80+3,    SCR_XSIZE-200-3,SCR_YSIZE-260-3,     COL_BLACK);        //ALL black
    //mV ��ʾ�� �¶���ʾ��
    Clr_Bar(  3,80+220+3,SCR_XSIZE-400-3,SCR_YSIZE-160-220-3, COL_THINGRAY);    
    Clr_Bar(403,80+220+3,SCR_XSIZE-600-3,SCR_YSIZE-160-220-3, COL_THINGRAY);   
    //ϵͳ��Ϣ��
    Clr_Bar(603,    80+3,          200-6,    SCR_YSIZE-160-3, COL_DEEPGRAY);   
    
       
     
    #if SDJ_SOFT  
    //Put_Str(630,110,Language_Str("��ǰУ׼��Һ","SAMPLE OF CALI"));
    //��ʾʹ���ĸ���׼ ���Ͻ�
    
    Clr_Bar(690,90,  100, 40,  COL_BLACK);  
    if(user_cal_ok)
    {
     Set_Chr_Color(COL_THINGREEN,COL_BLACK); 
     if(EUROPE_S == which_standard)
        Put_Str(Language_Pos(715,705), 100,Language_Str("ŷ��",  "EU-STD"));
     else if(AMERICAN_S == which_standard)
        Put_Str(Language_Pos(715,705), 100,Language_Str("����",  "AM-STD"));
     else       
        Put_Str(Language_Pos(715,705), 100,Language_Str("����",  "CH-STD"));
    }
    else
    {
     Set_Chr_Color(COL_THINRED,COL_BLACK); 
        //Put_Str(635,100,Language_Str("ERROR",  "ERROR"));
        Set_Chr_Color(COL_THINGREEN,COL_BLACK); 
     Put_Str(Language_Pos(715,705),100,Language_Str("����",  "CH-STD"));
    }
    //////////////////////////////////////////////////////////////////////
    Set_Chr_Color(COL_THINYELLOW,COL_DEEPGRAY);
    Put_Str(630,320,Language_Str("��ǰ�缫״̬","ELEC_NODE STAT"));
    if(1 == user_cal_ok)
    {
     //redraw area
     Display_cal_liquid_area();
     Display_elec_class_area();
    }
    else
    {
     //for(i=0;i<3;i++)
     Put_Bmp(630+50, 150,  liquiderr6066,0); 
    }
    #else
    Set_ASC_Lib(Arial16B_Font);
    Put_Str(650,125,Language_Str("�缫����",    "E-NODE CONS"));
    Put_Str(650,270,Language_Str("TDS ϵ��",    "TDS-COEF"));
    Set_Chr_Color(COL_THINYELLOW,COL_BLACK);
    Set_ASC_Lib(ARIALN26B);
    Clr_Bar(640,155,120,50,COL_BLACK);
    Clr_Bar(640,300,120,50,COL_BLACK);
    
    Put_Data_Float(660,170,"%6.3f",elec_cons);
    Put_Data_Float(660,315,"%6.3f",TDS_cons);
    #endif
    
    //������
    Clr_Bar(3,400+3, SCR_XSIZE-6,80-6,COL_BLACK); 
    
    if (Get_Language_Type() == CHINESE)
    {
     Put_Bmp(133*0+15,  405,  sysconfig8055,0); 
     Put_Bmp(133*1+15,  405,     recall8055,0); 
     Put_Bmp(133*2+15,  405,        cal8055,0); 
     Put_Bmp(133*3+15,  405,       save8055,0); 
     Put_Bmp(133*4+15,  405,    printer8055,0);
    }
    else
    {
     Put_Bmp(133*0+15,  405,  sysconfigen8055,0); 
     Put_Bmp(133*1+15,  405,     recallen8055,0); 
     Put_Bmp(133*2+15,  405,        calen8055,0); 
     Put_Bmp(133*3+15,  405,       saveen8055,0); 
     Put_Bmp(133*4+15,  405,    printeren8055,0);
    }
    Put_Bmp(133*5+15,  405,      sleep8055,0);
    
      ////////////////////////////////////////create keylist 
    for(i=0;i<6;i++)
    {
    xy[i][0]    = 65+133*i;
    xy[i][1]    = 400+80/2;
    }
   //////////////////////////////////add key2
    xy[i][0]      = 300+110/2;
    xy[i++][1]    = 240+40/2;
    xy[i][0]      = 430+110/2;
    xy[i++][1]    = 240+40/2;
  
    Create_ButtonList(xy,8);            
    /////////////////////////////////////REFLASH
    #if SDJ_SOFT
    Mode_Status_Display();
    #else
    Mode_Status_Display2();
    #endif

}
//******************************************************************************************************
//20 ����  ��:Normal_Operation
//��������:�����������������ģʽ�µĸ��ֲ���
//��ڲ���:��
//���ڲ���:��
//******************************************************************************************************


void Normal_Operation(void)
{  
    static uint32 cycle_times;
    uint16    ad_tmp,ad8325_tmp;
    struct menu_str menu_op[MENU_NUM]=
     //�˵�ID                �˵������ʼ��         �˵�ѡ���              ��ʾ����         ��������
    {
     {MENU_OP_OPER,           Put_Oper_Interface,               Main_Key_Pro,          Oper_Disp_Pro,  Oper_Main_Assi_Pro},
     {MENU_SLEEP_OP_OPER,     Put_Sleep_Interface,             Sleep_Key_Pro,         Oper_Sleep_Pro,      Oper_Assi_Null},    //��������
     {MENU_FLOAT_INPUT_OPER,  FloatInput_Interface,  	  FloatInput_Key_Pro,    FloatInput_Disp_Pro,  	    Oper_Assi_Pro},    // INPUT num  
     {MENU_UINT_INPUT_OPER,   UINTInput_Interface,         UINTInput_Key_Pro,     UINTInput_Disp_Pro,  		Oper_Assi_Pro},    // INPUT pcs
     {MENU_DATA_CALL_OPER,    Data_Call_Interface,         Data_Call_Key_Pro,     Data_Call_Disp_Pro,  		Oper_Assi_Pro},    //���ݻ���ʾ
     //////ϵͳ���ò˵�
     {MENU_SYSTEM_CONF_OPER,  SystemConf_Interface,       SystemConf_Key_Pro,    SystemConf_Disp_Pro,  		Oper_Assi_Pro},    //SYSTEM CFG 
   
     {MENU_TIME_CFG_OPER,     TimeCfg_Oper_Interface,        TimeCfg_Key_Pro,       TimeCfg_Disp_Pro,       Oper_Assi_Pro},    //TIME cfg
     {MENU_LANG_SEL_OPER,     LangSel_Interface,             LangSel_Key_Pro,       LangSel_Disp_Pro,  		Oper_Assi_Pro},    //lange CFG 
     {MENU_BRIG_CFG_OPER,     Brig_Adjust_Interface,     Brig_Adjust_Key_Pro,   Brig_Adjust_Disp_Pro,  		Oper_Assi_Pro},    //���ݻ���ʾ
       
     {MENU_STAND_SEL_OPER,    StandardSelInput_Interface,   Standard_Key_Pro,  Standard_Sel_Disp_Pro,  		Oper_Assi_Pro},    //��׼ѡ��
     {MENU_DIVISION_SEL_OPER, DivisionSel_Interface,     DivisionSel_Key_Pro,   DivisionSel_Disp_Pro,  		Oper_Assi_Pro},    //�ֱ���ѡ��
     {MENU_TEMP_COMP_OPER,    TempcompCfg_Interface,     TempcompCfg_Key_Pro,   TempcompCfg_Disp_Pro,  		Oper_Assi_Pro},    //�¶Ȳ���ѡ��
   
     {MENU_USER_ID_OPER,      UserIDInput_Interface,          UserID_Key_Pro,        UserID_Disp_Pro,  		Oper_Assi_Pro},    //�û�ID����
     {MENU_SCR_CAL_OPER,      ScrCalInput_Interface,          ScrCal_Key_Pro,        ScrCal_Disp_Pro,      Oper_Assi_Null},    //��ĻУ׼
     //�絼������ �˵�
     {MENU_DDLV_CONS_OPER,    DDLV_CONS_Input_Interface,   DDLV_CONS_Key_Pro,     DDLV_CONS_Disp_Pro,  	   Oper_Assi_Null},     //�絼����ѡ��
     {MENU_MODE_SEL_OPER,     ModeInput_Interface,              Mode_Key_Pro,          Mode_Disp_Pro,      Oper_Assi_Null},     //����2
     {MENU_BALANCE_SEL_OPER,  BalanceInput_Interface,   BalanceInput_Key_Pro,       Balance_Disp_Pro,  	   Oper_Assi_Null},     //����3
   
     {MENU_OPER_CONFIRM_OPER, OperConfirmInput_Interface,    Confirm_Key_Pro,       Confirm_Disp_Pro,  	   Oper_Assi_Null}      //����ȷ�ϲ˵� 
     
     };
    sint32 cur_op,next_op;    //��ǰѡ������л�����ѡ��(bits[7:0]:ѡ���;bits[15:8]:�����;bits[23:16]:ѡ�����ʾ״̬)
    static uint8   ddlv_buf_index=0;
    static uint8   sample_dly_cnt = 0;
    
    cur_op  = 0xffffffff;  
    next_op = MENU_OP_OPER;
    //next_op = MENU_BRIG_CFG_OPER;
  
    //#ifndef _DEBUG
    //�ǵ���ģʽ��ʹ�ܿ��Ź�
    //Init_WDT(500);                     //���Ź���ʼ��,ι�����=1s
    //WDT_Feed();                        //ι��
   
    while (1)
     {
        if ((cur_op^next_op)&0x0000ff00)//��ǰ�����Ľ����׼���л����Ľ��治һ��
        {//��ʼ������
            (*(menu_op[(next_op>>8)&0xff].menu_init))(next_op);
        }
        cur_op = next_op;        //"����/�˵�/ѡ��"֮����л�
        Set_Menu_Op(cur_op);
        
        
        #if DDLV_SOFT    //�Ѳɼ�8325�ĳ����ƶ��� ������ѭ�� ����
        if(1==sample_flag )
        {
          sample_dly_cnt++;
          if(sample_dly_cnt > 3)
             {
              sample_dly_cnt = 0;
              
              FIO0CLR = ADS1100_CS;
              ad8325_tmp =  ADS8325_RD();
              if((ad8325_tmp>10)&&(ad8325_tmp<8150))
              ddlv_buf_pre[ddlv_buf_index++] = ad8325_tmp;
              FIO0SET = ADS1100_CS;
             } 
          if(50 == ddlv_buf_index)
          {
           ddlv_buf_index     = 0;
           ADS1100_Raw   = sort_data();  //
           flag_new_data = 1;
           
           ADS1100_Match_Proc();  
           
          }
         }        
        #endif
        
        
         if(TRUE==flag_new_data)
          {
           flag_new_data = FALSE;
           //flag_1ms_ok   = FALSE;
           #if SDJ_SOFT
           filter();
           #else
           filter2();
            
           #endif
          }
       //////////////////////////////////////////////////////////////////////       
        if (flag_10ms_ok)    //10msʱ�䵽
            {
             flag_10ms_ok = FALSE;
             cycle_times++;
             /////////////////////////////////////////////
             if(0==(cycle_times%3))
                {
                 while(!(ADDR0&0x80000000));
                 ad_tmp = (ADDR0>>6)&0x3ff;
                 temp_float_ptc = filter_ad_NTC(ad_tmp); ///1024.0001 *3300;
                }
             else if(1==(cycle_times%3))
                {
                 while(!(ADDR6&0x80000000));
                 ad_tmp = (ADDR6>>6)&0x3ff;
                 cpu_ch_adc3 = filter_ad_ch3(ad_tmp);
                }
             else  
                {
                 while(!(ADDR7&0x80000000));
                 ad_tmp = (ADDR7>>6)&0x3ff;
                 cpu_ch_adc4 = filter_ad_ch4(ad_tmp);
                }
             /////////////////////////////////////////////////////////////////
            
             ////////////////////////////////////////////////////////////////           
             key_value = Get_PositionXY();        //����ɨ��
             if(key_value)        //���̴������̣����ݲ�ͬ��ֵ�����ز�ͬ�Ĵ��ڽ��� 
              {                   //�ڸô��ڰ��������д���ͬ�Ĺ��ܣ���������ٷ��ص�������
               next_op = (*(menu_op[(cur_op>>8)&0xff].op_pro))(key_value,cur_op);     
              }
             }  
      
             //   WDT_Feed();    //2��
         if (flag_100ms_ok)
            {//��ʾʱ�䵽,����Ҫ��ʾ�µĲ���ֵ
             flag_100ms_ok = FALSE;
            
             ////////////////////////��ȼ�ͨ���л� 10ms ����Ϊ100msһ��
             //#if SDJ_SOFT
              
             (*(menu_op[(cur_op>>8)&0xff].disp_pro))();    //��ʾ������
             //#endif     
           
            }    
             //  WDT_Feed();     //3��
         if (flag_300ms_ok)
            {                  //500msʱ�䵽
             flag_300ms_ok = FALSE;
            /* 
            #if DDLV_SOFT
              
             (*(menu_op[(cur_op>>8)&0xff].disp_pro))();    //��ʾ������
             #endif     
             */
             
              (*(menu_op[(cur_op>>8)&0xff].assi_pro))();    //����������
             /*
             //if(FALSE == sleepmode)
             //WDT_Feed();                //300ms ι��һ��
             
            
             /////////////////////////////////�������߸�������
             next_op = Wireless_Process(next_op); //�ú������ܻ�ı���� */
             ///////////////////////////////////////
             if((MENU_OP_OPER == next_op)&&(no_key_time > ENTER_SLEEP_TIME))       //ֻ�����������²ſ��Խ������״̬
               next_op = MENU_SLEEP_OP_OPER;
             }
             
          if (flag_500ms_ok)
            {                  //500msʱ�䵽
             flag_500ms_ok = FALSE; 
             LED_CPU_RUN_Cpl(); //CPU����ָʾ����˸Ƶ��1Hz
            }
          /////////////////////////ÿ�뷢������һ�Σ���λ�����ʹ�֡����    
          if (flag_1s_ok)     
             ////////////////////////////////�������͸���״̬֡
            {
             flag_1s_ok = FALSE;
             //�ô��ڷ����ӡ���ܣ��ù����Ƶ��ж��д���
             if(0==Uart_Printer_Flag)
             UART_SendData();
            }
      }
  }

  
////////////////////////////////////////////////////////
//21    ��ָ��λ����ָ����ɫ��ʾ�ȶ�����
//
///////////////////////////////////////////////////////
static void Stable_Status_Display(uint32 x,uint32 y)
 {
  if(TRUE == stable_flag)
   {
    Set_Color(COL_THINGREEN);
    Fill_Circle(x,y,20);
   }
   else
   {
    Set_Color(COL_THINRED);
    Fill_Circle(x,y,20);
   } 
    
 }
 
////////////////////////////////////////////////////////
//22    ��ָ��λ����ָ����ɫ��ʾ��������
//      PH ����
///////////////////////////////////////////////////////
static void Negative_Status_Display(void)
 {
    Clr_Bar(15,125,64,130,COL_BLACK); //����
    Set_ASC_Lib(GulimChe150_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    if(TRUE == ph_negative_flag)
      Put_Char(15,125,'-');
 }  
////////////////////////////////////////////////////////
//23   �����Ѿ�У׼��Һ ��ϵ�� ���� 
//
///////////////////////////////////////////////////////
 static float   User_Cal_Done(float ph_tmp)
   {
    //���ݵ�ǰph_tmp ����ֵ ȡ��Ӧ��ϵ������ ����У׼���Ӹ߶˿�ʼ
    float tmp;
    //�����ж��û�У׼�˼�����
    switch(user_cal_num)
      {
       case 1:
              if(ph_tmp < user_cal_phdata[0].rec_float)
                tmp = ph_tmp * user_coef1;
              else 
                {
                 tmp = ph_tmp-user_cal_phdata[0].rec_float;
                 tmp = user_cal_phdata_dis[0].rec_float + tmp*user_coef_left;
                }
              break;  
       case 2:
              if(ph_tmp < user_cal_phdata[0].rec_float)
                tmp = ph_tmp * user_coef1;
              else if(ph_tmp < user_cal_phdata[1].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[0].rec_float;
                 tmp = user_cal_phdata_dis[0].rec_float + tmp*user_coef2;
                }
              else
                {
                 tmp = ph_tmp-user_cal_phdata[1].rec_float;
                 tmp = user_cal_phdata_dis[1].rec_float + tmp*user_coef_left;  
                } 
              break;  
       case 3:
              if(ph_tmp < user_cal_phdata[0].rec_float)
                tmp = ph_tmp * user_coef1;
              else if(ph_tmp < user_cal_phdata[1].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[0].rec_float;
                 tmp = user_cal_phdata_dis[0].rec_float + tmp*user_coef2;
                }
              else if(ph_tmp < user_cal_phdata[2].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[1].rec_float;
                 tmp = user_cal_phdata_dis[1].rec_float + tmp*user_coef3;  
                } 
              else
                {
                 tmp = ph_tmp-user_cal_phdata[2].rec_float;
                 tmp = user_cal_phdata_dis[2].rec_float + tmp*user_coef_left;  
                }  
              break; 
        case 4:
              if(ph_tmp < user_cal_phdata[0].rec_float)
                tmp = ph_tmp * user_coef1;
              else if(ph_tmp < user_cal_phdata[1].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[0].rec_float;
                 tmp = user_cal_phdata_dis[0].rec_float + tmp*user_coef2;
                }
              else if(ph_tmp < user_cal_phdata[2].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[1].rec_float;
                 tmp = user_cal_phdata_dis[1].rec_float + tmp*user_coef3;  
                } 
              else if(ph_tmp < user_cal_phdata[3].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[2].rec_float;
                 tmp = user_cal_phdata_dis[2].rec_float + tmp*user_coef4;  
                }  
              else
                {
                 tmp = ph_tmp-user_cal_phdata[3].rec_float;
                 tmp = user_cal_phdata_dis[3].rec_float + tmp*user_coef_left;  
                }  
              break; 
        case 5:
                if(ph_tmp < user_cal_phdata[0].rec_float)
                tmp = ph_tmp * user_coef1;
              else if(ph_tmp < user_cal_phdata[1].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[0].rec_float;
                 tmp = user_cal_phdata_dis[0].rec_float + tmp*user_coef2;
                }
              else if(ph_tmp < user_cal_phdata[2].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[1].rec_float;
                 tmp = user_cal_phdata_dis[1].rec_float + tmp*user_coef3;  
                } 
              else if(ph_tmp < user_cal_phdata[3].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[2].rec_float;
                 tmp = user_cal_phdata_dis[2].rec_float + tmp*user_coef4;  
                } 
              else if(ph_tmp < user_cal_phdata[4].rec_float) 
                {
                 tmp = ph_tmp-user_cal_phdata[3].rec_float;
                 tmp = user_cal_phdata_dis[3].rec_float + tmp*user_coef5;  
                }  
              else
                {
                 tmp = ph_tmp-user_cal_phdata[4].rec_float;
                 tmp = user_cal_phdata_dis[4].rec_float + tmp*user_coef_left;  
                }  
              break;               
       default:
                tmp = ph_tmp;
               break;
      }
    //////////////////////////////////////////////
    return tmp;
   }
////////////////////////////////////////////////////////
// 24 ��ָ��λ����ָ����ɫ��ʾ ��������
//    ��һ�����ȼ������
//    �ڶ������õ��¶�ϵ��(�²�����)
//    �����������ݺ������� PH -->�û���Һϵ��У׼--->����ж�
//
///////////////////////////////////////////////////////
static void  New_Data_Display(void)
 {
           float ph_tmp,mv_tmp;
          uint32 ch_tmp,cur_data;
   //////////////////////���ж�ϵͳУ׼�Ƿ���� 
   if(TRUE == sys_cal_ok)
    { 
       //////////////////////////////////////
       //���ݵ�ǰѡ���ͨ���� ��������ת��
       ch_tmp = select_ch-1;
       cur_data = Get_Filter_Data(ch_tmp);
       switch(ch_tmp)
       {
        case 0:  //ch1 100mv-1400mv
                 mv_tmp = 100 + sys_coef1*(abs(cur_data-low1_data[0]));
                 //m_data_last = tmp;     
                 mv_negative_flag = 0;
                 break;
        case 1:  //ch2 (-100mv)-(-1400mv)
                 mv_tmp = 100 + sys_coef2*(abs(cur_data-low2_data[0]));
                 if(mv_tmp > 1400)
                 mv_tmp = 1400+ sys_coef5*(abs(cur_data-full2_data[0]));
                 //m_data_last = tmp;        
                 mv_negative_flag = 1;
                 break;
        case 2:  //ch3 (-100mv)-0mv
                 mv_tmp =  sys_coef3 * (abs(cur_data-zero_data[0]));
                 //m_data_last = tmp;                
                 mv_negative_flag = 1;
                 break;
        case 3:  //ch4 0mv-100mv
                 mv_tmp =  sys_coef4 * (abs(cur_data-zero_data[1]));
                 //m_data_last = tmp;   
                 mv_negative_flag = 0;
                 break;
        default: break; 
       }
       
       mv_data =  mv_tmp;    //���ȵõ��������� 
      /////////////////////////����ϵͳУ׼ϵ���õ���PH
      ///////////////////////////////////////
      //���Ƚ��� �²����� �����¶� ��ȡ ��ͬ��ת��ϵ��
      Search_Coef_Temp();
      //get_coef = 59.157; 
      //////////////////////////////////////
      if(TRUE == mv_negative_flag)
         {
           ph_tmp = 7 + mv_data/get_coef;
           ph_negative_flag = 0;
         }
      else
         {
           ph_tmp = mv_data/get_coef;
           if(ph_tmp > 7)
           {
            ph_negative_flag = 1;
            ph_tmp = ph_tmp - 7;
           }
           else
           {
            ph_negative_flag = 0;
            ph_tmp = 7-ph_tmp;
           }
        }
     
      
      //ֻ�����û�У׼OK ���ҷ�У׼״̬�� У׼ϵ��������
      if((FALSE==point2_cal_start)&&(1==user_cal_ok))
        {
         ph_tmp = User_Cal_Done(ph_tmp);
        }
        
      //����ж������Ƿ����  
      ph_data = ph_tmp;
      
      if(((1==ph_negative_flag)&&(ph_data>1.9999))||((0==ph_negative_flag)&&(ph_data>17.9999)))
      full_flag = TRUE;
      else
      full_flag = FALSE;            
   }       
   else   //û�н���ϵͳУ׼
   {
      mv_data   = 1234.56;  //δУ׼����
      ph_data   = 1.2345;
      full_flag = FALSE; 
   }   
   
  ///////////////////////////////////// 
  switch(current_mode)
  {
   case STAT_NORMAL:      display_measure_data();     break;  
   case STAT_SYS_CAL:     display_sys_caling_data();  break;
   case STAT_USER_CAL:    display_user_caling_data(); break;

   default:break;
  }
 }
///////////////////////////////////////////////////////
//24A  �絼���ǵĲ������� ��ʾ
//
//////////////////////////////////////////////////////////

static void  New_Data_Display2(void)
 {
   static  float  res_buf[64];
   static uint32  res_buf_index = 0;
   //static uint32  res_buf_full  = 0;
   
   uint32 i,cur_data;
   float  tmp;
   
   //////////////////////���ж�ϵͳУ׼�Ƿ���� 
   if(TRUE == sys_cal_ok)
    { 
       cur_data = Get_Filter_ADS1100Data();
        
       //��������������                 
       tmp = cal_ddlv_k[match_item-1] / cur_data;
       //��������ƫ����
       ddlv_R = tmp;
       /* 
       if(switch_action_step > 3)
         {
          res_buf[res_buf_index++] = tmp;
          if(64 == res_buf_index)
             res_buf_full = 1;
          ////////////////////////////
          if(1 == res_buf_full)
             {
              tmp = 0;
              for(i=0;i<64;i++)
              tmp+=res_buf[i];
              ddlv_R = tmp/64;
             }
          else
             {
              tmp = 0;
              for(i=0;i<res_buf_index;i++)
              tmp+=res_buf[i];
              ddlv_R = tmp/res_buf_index;
             }
         }
       else 
         {
          res_buf_index = 0;
          res_buf_full  = 0;  
          ddlv_R = tmp;
         }*/ 
       //����絼��
       if(ddlv_R < 500)                    /////////////////////////////////////////
         {
          ddlv_data = 1000 / ddlv_R * ddlv_coef;  //�û�����������
          ddlv_data = ddlv_data * elec_cons;      //�缫����������
          DDLV_unit = MS_CM; 
         }
       else
         {
          ddlv_data = 1000 / (ddlv_R/1000) * ddlv_coef;
          ddlv_data = ddlv_data * elec_cons;      //�缫����������
          DDLV_unit = US_CM;
         }
     
       ////////////////////////////Ȼ������û�У׼ϵ����������
       //temp_comp_proc();
       ////////////////////////////�¶Ȳ������Ȼ������趨�Ĳ�����ʾ��Ӧ������
       if(PARAM_TDS == DDLV_Param)
         {
          ddlv_data = ddlv_data * TDS_cons;  //TDS ϵ��������
         }
       else if(PARAM_SALTY == DDLV_Param)
         {
            if(TRUE == manual_temp_comp_flag)
              {
               if((temp_comp_manual>0) && (temp_comp_manual<40))
                ddlv_data = 1.3888*ddlv_data -0.02478*ddlv_data * temp_comp_manual - 6171.9;
               else
                ddlv_data = 1.3888*ddlv_data -0.02478*ddlv_data * 25 - 6171.9;  //��25��Ϊ׼
              }
            else
              {
               if((temp_float_ptc>0) && (temp_float_ptc<40))
                ddlv_data = 1.3888*ddlv_data -0.02478*ddlv_data * temp_float_ptc - 6171.9;
               else
                ddlv_data = 1.3888*ddlv_data -0.02478*ddlv_data * 25 - 6171.9;  //��25��Ϊ׼
              }
         } 
      ////////////////////////////////////////////////////////   
    }       
   else   //û�н���ϵͳУ׼
   {
      ddlv_data  = 1234.56;  //δУ׼����
      full_flag  = FALSE; 
   }   
   
  ///////////////////////////////////// 
  switch(current_mode)
  {
   case STAT_NORMAL:      display_measure2_data();break;  
   case STAT_SYS_CAL:     display_sys_caling_data2();break;
   case STAT_USER_CAL:    display_user_caling_data2();break;

   default:break;
  }
 } 
 
//////////////////////////////////////////////////////
//25   �������� ������ʾ���ڵ� ��ʾ
//
//////////////////////////////////////////////////////
extern uint32 stable_time_cnt;

static void  display_main_area(void)
  {
    float x;
    //display ph_data 
    //////////////////////////////////////////////////////////\
    //ph_data = 7.0
    Set_ASC_Lib(GulimChe150_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    if(FALSE == full_flag)
      {
       /////////////////////////////////////P811
       if(TRUE == version_811)
       {
        if(phmv_division&0x01)
           Put_Data_Float(100,125,"%6.2f",ph_data);
        else
           Put_Data_Float(100,125,"%6.3f",ph_data);
       }
       else
       {
        if(phmv_division&0x01)
           Put_Data_Float(100,125,"%6.1f",ph_data);
        else
           Put_Data_Float(100,125,"%6.2f",ph_data);
       }  
      } 
    else
       Put_Str(100,125,"------");
    //display mv_data   
    //////////////////////////////////////////////////////////
    //mv_data = 0;
    Set_ASC_Lib(GulimChe48A_Font);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    if(TRUE == version_811)
    {
      if(phmv_division&0x08)
         Put_Data_Float(60,320,"%7.2f",mv_data); 
      else
         Put_Data_Float(60,320,"%7.1f",mv_data);
    }
    else
    {
      if(phmv_division&0x08)
         Put_Data_Float(60,320,"%7.1f",mv_data); 
      else
         Put_Data(      60,320,"%5d",  (uint32)mv_data); 
    }
    //display mv flag
    if(TRUE == mv_negative_flag)
       {
        Set_Chr_Color(COL_BLACK,COL_THINGRAY);
       }
    else
       {
        Set_Chr_Color(COL_THINGRAY,COL_THINGRAY);
       }
     
     Put_Char(20,320,'-');  
     
     Negative_Status_Display();
     /////////////////////////////////////////////////////////////////////  
      //��������Ϊ������Ϣ
      /*/��ʾAD CHANNEL DATA
      Set_ASC_Lib(Arial16B_Font);
	  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
	  Put_Data_Float(650,100,"%.7f",user_coef1);
	  Put_Data_Float(650,125,"%.7f",user_coef2);
	  Put_Data_Float(650,150,"%6.3f",user_cal_phdata_dis[0].rec_float);
	  Put_Data_Float(650,175,"%.7f",user_coef_left);
	 */
	  /*
      Set_ASC_Lib(Arial16B_Font);
	  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
	 
	  //Put_Data(650,125,"%06d",stable_time_cnt);
	  
	  //Put_Data(650,225,"%06d",cpu_ch_adc3);
	  //Put_Data(650,275,"%06d",cpu_ch_adc4);
	  */
	  /*
	  Put_Data(650,150,"%06d",Get_Filter_Data(1));
	  Put_Data(650,175,"%06d",Get_Filter_Data(2));
	  Put_Data(650,200,"%06d",Get_Filter_Data(3));
	     
      Put_Line_Str(  650,250, Language_Str("ADC3","ADC3"));
      Put_Data(650,275,"%04d",cpu_ch_adc3);
	  Put_Line_Str(  650,300,Language_Str("ADC4","ADC4"));
      Put_Data(650,325,"%04d",cpu_ch_adc4);
     */
  }
///////////////////////////////////////////////////////  
//25A  �絼�� ���� �������� ������ʾ���ڵ� ��ʾ
//
//////////////////////////////////////////////////////  
static void  display_main_area2(void)
  { 
    static uint32 last_match_item;
    static uint32 reflash_flag = 0;
    static float  ddlv_tmp = 0;
    float x;
    uint32 tmp;
    
    Set_ASC_Lib(GulimChe150_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
   

   
    if((reflash_flag==1)||(last_match_item != match_item))
      {//��λ�л� ����Ҫ����ˢ����ʾ����
       last_match_item = match_item;
       reflash_flag = 0;
       Clr_Bar(110,125,64*7,130,COL_BLACK);
      }
      
    if((MATCH_ITEM_1 == match_item) &&(ddlv_data>200))
     {
      Put_Str(110,125,"------");
      reflash_flag = 1;
      }
    else if(ddlv_data < 0.01)
     {
      ddlv_data = 0.0;
      Put_Data_Float(110+32,125,"%.2f",ddlv_data);
    
     }
    else
     {
         //���ݵ絼�ʵ�λ������ʾ
         //ddlv_data = 0;
        
         if(MS_CM == DDLV_unit)
           {
               if(ddlv_data>200)
              {
               Put_Data(110+32,125,"%d",(uint32)(ddlv_data+0.4));
               if(ddlv_tmp < 200)
               {
                reflash_flag = 1;
                ddlv_tmp = ddlv_data;
               }
              }
              else if(ddlv_data>20)
              {
               Put_Data_Float(110+32, 125,"%.1f",ddlv_data+0.04);
               
               if((ddlv_tmp >200) ||(ddlv_tmp<20))
               {
                reflash_flag = 1;
                ddlv_tmp = ddlv_data;
               }
              
              }
              else
              {
               Put_Data_Float(110+32,125,"%.2f",ddlv_data);
               if(ddlv_tmp >20)
               {
                reflash_flag = 1;
                ddlv_tmp = ddlv_data;
               }
              
              }
             ///////////////////////////////////// 
           
            }    
          else
            { 
             
             if(ddlv_data>20)
               {
                Put_Data_Float(110+32,  125,"%.1f",ddlv_data);
                if(ddlv_tmp < 20)
               {
                reflash_flag = 1;
                ddlv_tmp = ddlv_data;
               }
              
               }
             else
               {
                Put_Data_Float(110+32,  125,"%.2f",ddlv_data);
                if(ddlv_tmp > 20)
               {
                reflash_flag = 1;
                ddlv_tmp = ddlv_data;
               }
               }
           
             }
    } 
    //////////////////////////////////////////////////////////
    Set_ASC_Lib(Unit_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    if(PARAM_TDS == DDLV_Param)
      {
       if(US_CM == DDLV_unit)
         Put_Line_Str(465, 245,Language_Str("mg/L","mg/L")); 
       else
         Put_Line_Str(465, 245,Language_Str(" g/L"," g/L")); 
      }
    else if(PARAM_SALTY == DDLV_Param)
      {
         //�ζ�ֻ��һ����λ:  %
         Put_Char(500, 245,'%'); 
      }
    else
      {
       if(US_CM == DDLV_unit)
         Put_Line_Str(465, 245,Language_Str("uS/cm","uS/cm")); 
       else
         Put_Line_Str(465, 245,Language_Str("mS/cm","mS/cm")); 
      }
   
   //////////////////////////////////////////////////////////
   
   //����ģʽ�� �����ڵ� ��ʾһ��      
   if(ddlv_R<1000)
      tmp = 0;
   else if(ddlv_R<1000000)
      {
       tmp = 1;
       ddlv_R = ddlv_R/1000;
      }
   else if(ddlv_R<50000000)  // ������50M
      {
       tmp = 2;
       ddlv_R = ddlv_R/1000000;
      }
   else  //ŷķ ���
      {
       tmp = 2;
       ddlv_R = 0.0;
      }   
      ////////////////////////////////////////���㵥λ     
          Set_ASC_Lib(Unit_Font);
          Set_Chr_Color(COL_BLACK,COL_THINGRAY);
   	      switch(tmp)
   	       {
   	        case 0: Put_Line_Str(250-23, 330,Language_Str(" Z.cm"," Z.cm"));  break;
   	        case 1: Put_Line_Str(250-23, 330,Language_Str("KZ.cm","KZ.cm"));  break;
   	        case 2:
   	        default:Put_Line_Str(250-23, 330,Language_Str("MZ.cm","MZ.cm"));
   	                break;
   	       }
   	       
    	
    Set_ASC_Lib(GulimChe48A_Font);
    //Put_Data_Float(50,320,"%5.1f",100.0);
    //if(ddlv_R>0.002)
    Put_Data_Float(50,320,"%5.1f",ddlv_R); 
    
    //////////////////////////////////////////////��ʾAD CHANNEL DATA
    /*
      Set_ASC_Lib(Arial16B_Font);
	  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
	  Put_Data_Float(650,125,"%.2f",offset_data);
    */
  }

//////////////////////////////////////////////////////
//26  ����״̬�µ� ��ʾ 
//    ǰ�����ȴ��� ���������Ϣ
//
//////////////////////////////////////////////////////
static void  display_measure_data(void)
 {
   //ɾ������ģʽ�µı�����Ϣ
   Del_Alm(&warn_ph_out,ALM_LEV_WARN);
   Del_Alm(&warn_mv_out,ALM_LEV_WARN);
   Del_Alm(&warn_xx_out,ALM_LEV_WARN);
 
   //////////////////////////////////////// 
   display_main_area(); 
}  

static void  display_measure2_data(void)
 {
   //ɾ������ģʽ�µı�����Ϣ
   Del_Alm(&warn_ph_out,ALM_LEV_WARN);
   Del_Alm(&warn_mv_out,ALM_LEV_WARN);
   Del_Alm(&warn_xx_out,ALM_LEV_WARN);
 
   //////////////////////////////////////// 
   display_main_area2(); 
}  

//////////////////////////////////////////////////////
//27  ��ȼ� ϵͳУ׼״̬�µ� ��ʾ
//
///////////////////////////////////////////////////////
static void  display_sys_caling_data(void)
 {
   
   static uint32 process_num = 0;
 
   ///////////////////////////////////////////////У׼״̬��ʾ
     process_num = process_num+10;
      if(100 == process_num)
         process_num = 0;
     ////////////////////////////////////��У׼�����ƶ�����ʾ�����Զ�������һ��
     if(TRUE == stable_flag)
        Sys_Cal_Pro();
     
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_WHITE,COL_BLACK);
     
     if((SYS_CAL_ZER0A==point2_cal_step)||(SYS_CAL_ZER0B==point2_cal_step))
     {
      Put_Line_Str(50, 180,Language_Str("6.1 ������0.000mv","Checking 0.000mv"));
      Clr_Bar(80, 110,400,40,COL_THINBLUE);
      Clr_Bar(80+4*process_num, 112,15,36,COL_DEEPGREEN);
     }
     else if((SYS_CAL_LOW1A == point2_cal_step)||(SYS_CAL_LOW1B == point2_cal_step))
      {
       Put_Line_Str(50, 210,Language_Str("6.2 ������  100mv","Please input  100mv"));
       Clr_Bar(80, 110,    80,40,COL_DEEPGREEN);
       Clr_Bar(160, 110,400-80,40,COL_THINBLUE);
       Clr_Bar(80+4*process_num, 112,15,36,COL_DEEPGREEN);
      }
     else if((SYS_CAL_LOW2A == point2_cal_step)||(SYS_CAL_LOW2B == point2_cal_step))
      {
       Put_Line_Str(50, 240,Language_Str("6.3 ������ -100mv","Please input -100mv"));
       Clr_Bar(80, 110,    160,40,COL_DEEPGREEN);
       Clr_Bar(240, 110,400-160,40,COL_THINBLUE);
       Clr_Bar(80+4*process_num, 112,15,36,COL_DEEPGREEN);
      }
     
     else if(SYS_CAL_FULL1 == point2_cal_step)
      {
       Put_Line_Str(20+280, 180,Language_Str("6.4 ������  1400mv","Please input  1400mv"));
       Clr_Bar(80, 110,    240,40,COL_DEEPGREEN);
       Clr_Bar(320, 110,400-240,40,COL_THINBLUE);
       Clr_Bar(80+4*process_num, 112,15,36,COL_DEEPGREEN);
      }
     else if(SYS_CAL_FULL2 == point2_cal_step)
      {
       Put_Line_Str(20+280, 210,Language_Str("6.5 ������ -1400mv","Please input -1400mv"));
       Clr_Bar(80, 110,    320,40,COL_DEEPGREEN);
       Clr_Bar(400, 110,400-320,40,COL_THINBLUE);
       Clr_Bar(80+4*process_num, 112,15,36,COL_DEEPGREEN);
      }
     else if(SYS_CAL_FULL3 == point2_cal_step)
      {
       Put_Line_Str(20+280, 240,Language_Str("6.6 ������ -2000mv","Please input -2000mv"));
       Clr_Bar(80, 110,    320,40,COL_DEEPGREEN);
       Clr_Bar(400, 110,400-320,40,COL_THINBLUE);
       Clr_Bar(80+4*process_num, 112,15,36,COL_DEEPGREEN);
      }
      
     else if(SYS_CAL_OVER == point2_cal_step)
     {
      Put_Line_Str(20+280, 240,Language_Str("6.6 У׼����","CAL OVER"));
      Clr_Bar(80, 110,400,40,COL_DEEPGREEN);
     }
     
     /*////////////////////
      Set_ASC_Lib(Arial16B_Font);
	  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
	  Put_Data(650,100,"%03d",select_ch);
	  Put_Data(650,125,"%06d",Get_Filter_Data(0));
	  Put_Data(650,150,"%06d",Get_Filter_Data(1));
	  Put_Data(650,175,"%06d",Get_Filter_Data(2));
	  Put_Data(650,200,"%06d",Get_Filter_Data(3));
	     
      Put_Line_Str(  650,250, Language_Str("ADC3","ADC3"));
      Put_Data(650,275,"%04d",cpu_ch_adc3);
	  Put_Line_Str(  650,300,Language_Str("ADC4","ADC4"));
      Put_Data(650,325,"%04d",cpu_ch_adc4);
    */
}  

//////////////////////////////////////////////////////
//27  ��ȼ� ϵͳУ׼״̬�µ� ��ʾ
//
///////////////////////////////////////////////////////
static void  display_sys_caling_data2(void)
 {
   
   static uint32 process_num = 0;
 
   ///////////////////////////////////////////////У׼״̬��ʾ
     process_num = process_num+10;
      if(100 == process_num)
         process_num = 0;
     ////////////////////////////////////��У׼�����ƶ�����ʾ�����Զ�������һ��
     if(TRUE == stable_flag)
        Sys_Cal_Pro2();
     
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_WHITE,COL_BLACK);
     
     switch(point2_cal_step)
       {
        case SYS_CAL_1A:  
                        Put_Line_Str(180, 230,Language_Str("10.1 ������5  ŷ",    "Please Input 10"));
                        break;
        case SYS_CAL_1B:  
        case SYS_CAL_2A:  
                        Put_Line_Str(180, 230,Language_Str("10.1 ������50 ŷ",    "Please Input 10"));
                        break;
        case SYS_CAL_2B:  
        case SYS_CAL_3A:  
                        Put_Line_Str(180, 230,Language_Str("10.1 ������500ŷ",    "Please Input 10"));
                        break;
        case SYS_CAL_3B:  
        case SYS_CAL_4A:  
                        Put_Line_Str(180, 230,Language_Str("10.1 ������5K ŷ",    "Please Input 10"));
                        break;
        case SYS_CAL_4B:  
        case SYS_CAL_5A:  
                        Put_Line_Str(180, 230,Language_Str("10.1 ������50K ŷ",    "Please Input 10"));
                        break;
        case SYS_CAL_5B:  
        case SYS_CAL_6A:  
                        Put_Line_Str(180, 230,Language_Str("10.1 ������500K ŷ",    "Please Input 10"));
                        break;
        case SYS_CAL_6B:  
                        Put_Line_Str(180, 230,Language_Str("10.1 ������5M ŷ",      "Please Input 10"));
                        break;
        default:break; 
       }                
             Clr_Bar(120, 150,    30,40,COL_DEEPGREEN);
             Clr_Bar(150, 150,300-30,40,COL_THINBLUE);
             Clr_Bar(120+3*process_num, 152,12,36,COL_DEEPGREEN);
       
      /////////////////////
      Set_ASC_Lib(Arial16B_Font);
	  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
	  Put_Data(650,200,"%05d",Get_Filter_ADS1100Data());
} 

//////////////////////////////////////////////////////
//28  �û�У׼״̬�µ� ��ʾxxxxxxxxxxxxxxxxxxxxxxxxxx
//
///////////////////////////////////////////////////////
static void  display_user_caling_data(void)
 {
  uint32 i;
  float  tmp;
  static uint32 flash_cnt;
  
  flash_cnt++;
  //��У׼�����ƶ�����ʾ�����Զ�������һ��
  //��ʾϵͳPH  ��ʾƥ�䵽��PH
  Set_ASC_Lib(GulimChe48A_Font);
  Set_Chr_Color(COL_BLACK,COL_THINGRAY); 
  if(TRUE == version_811)
    {
     if(phmv_division&0x01)
     Put_Data_Float(55,165, "%6.2f",ph_data);
     else
     Put_Data_Float(55,165, "%6.3f",ph_data);
    }
  else
    {
     if(phmv_division&0x01)
     Put_Data_Float(55,165, "%6.1f",ph_data);
     else
     Put_Data_Float(55,165, "%6.2f",ph_data);
    }
  //��ʾ���� ��
  if(TRUE == ph_negative_flag)
    Put_Char(20,160,'-');
  else  
    Put_Char(20,160,'+');
    
  //����ƥ�����Һֵ
 
  for(i=0;i<5;i++)
   {
    match_point = i;
    if(TRUE == version_811)
      {
       if(ph_data > standard_data_p811[which_standard][i])
         {
          tmp = ph_data - standard_data_p811[which_standard][i];
         }
       else
         tmp = standard_data_p811[which_standard][i] - ph_data;
      }
    else
      {
       if(ph_data > standard_data_p801[which_standard][i])
         {
          tmp = ph_data - standard_data_p801[which_standard][i];
         }
       else
         tmp = standard_data_p801[which_standard][i] - ph_data;
      }  
    //////////////////////////////////////////////////////////////////      
    if(tmp < 0.3)
     {
      match_flag  = TRUE;
      break; 
     }
     else
     {
      match_flag  = FALSE;
     } 
   }
   
   ///////////////////////////////////////////��˸ƥ�����Һֵ
  if(((flash_cnt%6)<3)&&(TRUE==match_flag))
     {
      Set_Chr_Color(COL_BLACK,COL_DEEPGRAY); 
     }
  else
     {
      Set_Chr_Color(COL_DEEPGRAY,COL_DEEPGRAY); 
     }
   
   if(TRUE == version_811)    
     Put_Data_Float(295,165, "%6.3f",standard_data_p811[which_standard][match_point]);
   else
     Put_Data_Float(295,165, "%6.2f",standard_data_p801[which_standard][match_point]);
 
  //display mv 
  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
 
  if(TRUE == version_811)
  {
    if(phmv_division&0x08)
     Put_Data_Float(60,320,"%7.2f",mv_data); 
    else
     Put_Data_Float(60,320,"%7.1f",mv_data);
  }
  else
  {
    if(phmv_division&0x08)
     Put_Data_Float(60,320,"%7.1f",mv_data); 
    else
     Put_Data(      60,320,"%5d",  (uint32)mv_data);
  }
  //////////////////////////////////////////////
   Set_ASC_Lib(GulimChe48A_Font); 
    if(TRUE == mv_negative_flag)
       {
        Set_Chr_Color(COL_BLACK,COL_THINGRAY);
       }
    else
       {
        Set_Chr_Color(COL_THINGRAY,COL_THINGRAY);
       }
     
    Put_Char(20,320,'-');     
}  
//////////////////////////////////////////////////////
//28  DDLV У׼״̬�µ� ��ʾxxxxxxxxxxxxxxxxxxxxxxxxxx
//
///////////////////////////////////////////////////////
static void  display_user_caling_data2(void)
 {
  uint32 i;
  float  tmp;
  static uint32 flash_cnt;
  
  flash_cnt++;
  //��У׼�����ƶ�����ʾ�����Զ�������һ��
  //��ʾϵͳPH  ��ʾƥ�䵽��PH
  Set_ASC_Lib(GulimChe48A_Font);
  Set_Chr_Color(COL_BLACK,COL_THINGRAY); 
  Put_Data_Float(55,165, "%6.1f",ddlv_data);
  
  /*��ʾ���� ��
  if(TRUE == ph_negative_flag)
    Put_Char(20,160,'-');
  else  
    Put_Char(20,160,'+');
  */
    
  //����ƥ�����Һֵ ֻʹ��һ�ֱ�׼
  which_standard = 0;
  
  for(i=0;i<4;i++)
   {
    match_point = i;
    
    if(ddlv_data > DDLV_standard_data[which_standard][i])
       {
        tmp = ddlv_data - DDLV_standard_data[which_standard][i];
       }
    else
        tmp = DDLV_standard_data[which_standard][i] - ddlv_data;
        
    if(tmp < 0.5)
     {
      match_flag  = TRUE;
      break; 
     }
    else
     {
      match_flag  = FALSE;
     } 
   }
   
   ///////////////////////////////////////////��˸ƥ�����Һֵ
  if(((flash_cnt%6)<3)&&(TRUE==match_flag))
     {
      Set_Chr_Color(COL_BLACK,COL_DEEPGRAY); 
     }
  else
     {
      Set_Chr_Color(COL_DEEPGRAY,COL_DEEPGRAY); 
     }
     
   Put_Data_Float(295,165, "%6.3f",DDLV_standard_data[which_standard][match_point]);
  
  /*/display mv 
  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
  if(phmv_division&0x08)
     Put_Data_Float(60,320,"%7.2f",mv_data); 
  else
     Put_Data_Float(60,320,"%7.1f",mv_data);
  //////////////////////////////////////////////
   Set_ASC_Lib(GulimChe48A_Font); 
    if(TRUE == mv_negative_flag)
       {
        Set_Chr_Color(COL_BLACK,COL_THINGRAY);
       }
    else
       {
        Set_Chr_Color(COL_THINGRAY,COL_THINGRAY);
       }
     
    Put_Char(20,320,'-');*/     
}  

////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////  

