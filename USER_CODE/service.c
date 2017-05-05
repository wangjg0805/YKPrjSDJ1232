//**********************************************************************
//�ļ�˵��              
//����
//��1��ά�޵���ģʽ�µĹ���ʵ��
//�����б�
//1   ��������ʾ ��ʼ��			Init_Main_Sel
//2   ���ش�������ʾ ��ʼ��		Init_Service_Sensor
//3   �¶ȴ�������ʾ ��ʼ��		Init_Service_Temperature
//4   EEPROM ��ʾ ��ʼ��		Init_Service_EEprom
//5   ��������ʾ ��ʼ��			Init_Service_AD7843
//6   �����ʾ ��ʼ�� 			Init_Service_Motor
//7   �ӿ���ʾ ��ʼ��			Init_Service_Interface
//8   ����������ʾ ��ʼ��		Init_Service_BriANDSpeaker
//9   ��Ļ������ʾ ��ʼ��		Init_Service_Screen
/************************************************************************/
/*10  Start_Service_process  �����������                             */
/************************************************************************/
//11  ������ �����ж�			Action_MainMenuCfg
//12  �ӽ��� �����ж�			Action_SubMainCfg
//13  EEPROM���� �����ж� 		Action_EEPromMainCfg
//14  �ӿڽ��� �����ж�			Action_InterfaceMainCfg
//15  �������Ƚ��� �����ж�     Action_BrightSpeakerMainCfg
//16  ��Ļ���԰����ж�          Action_ScreenTestMainCfg
//17  �����水������            Menu_Serv_Main_Key(uint32 key_c,sint32 op);
//18  ���ش�������������        Menu_Serv_Sensor_Key(uint32 key_c,sint32 op);
//19  �¶ȴ�������������        Menu_Serv_Temperature_Key(uint32 key_c,sint32 op);
//20  EEPROM ��������           Menu_Serv_EEprom_Key(uint32 key_c,sint32 op);
//21  ��������������            Menu_Serv_AD7843_Key(uint32 key_c,sint32 op);
//22  ��ﰴ������              Menu_Serv_Motor_Key(uint32 key_c,sint32 op);
//23  �ӿڰ�������              Menu_Serv_Interface_Key(uint32 key_c,sint32 op);
//24  �������Ȱ�������          Menu_Serv_BriANDSpk_Key(uint32 key_c,sint32 op);
//25  ��Ļ���Խ��水������      Menu_Serv_Screen_Key
//26  ��������ʾ                Serv_Main_Menu_Disp
//27  ���ش�����������ʾ        Serv_Sensor_Menu_Disp
//28  �¶ȴ�����������ʾ        Serv_Temperature_Menu_Disp
//29  EEPROM  ������������ʾ    Serv_EEprom_Menu_Disp
//30  ������������ʾ            Serv_AD7843_Menu_Disp
//31  ��������ʾ              Serv_Motor_Menu_Disp
//32  �ӿڽ�����ʾ              Serv_Interface_Menu_Disp
//33  �������Ƚ�����ʾ          Serv_BriANDSpk_Menu_Disp
//34  ��Ļ���Խ�����ʾ          Serv_Screen_Menu_Disp

//**********************************************************************/
#include "service.h"
#include "screencal.h"
#include "menu.h"
#include "Font_Lib.H"
#include "Font_Lib2.H"
#include "disp.h"
#include "lcd2478.h"
#include "global.h"
#include "eeprom.h"
#include "i2c_com.h"
#include "type_def.h"
#include  "timer.h"
#include  "ad7843.h"

#include  "ADS1100.h"


extern uint32 ddlv_buf_pre[50];

////////////////////////////////////////////////////////////////////////
//
//���ļ�ʹ�õľ�̬����
////////////////////////////////////////////////////////////////////////
static  uint32 eeprom_data,eeprom_zero_flash_flag,eeprom_full_flash_flag,eeprom_test_flash_flag;
static  uint32 eeprom_linear_data[10],eeprom_linear_flash_flag;

static  uint32 power_ch,res_ch;   //ѡ��ʹ���ĸ���Դ ��ʹ���ĸ���������

static  uint32 flash_scr_flag;
static  uint32 interface_data;

//added by wjg 20151118
static  uint32 uart_tmp,logo_tmp,model_tmp,model_display_tmp;

///////////////////////////////////////////////////////////////////////
//��������
//////////////////////////////////////////////////////////////////////
static  sint32 Menu_Serv_Main_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Sensor_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Temperature_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_EEprom_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_AD7843_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_DDLV_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Interface_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_BriANDSpk_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Screen_Key(uint32 key_c,sint32 op);

void Serv_Main_Menu_Disp(void);
void Serv_Sensor_Menu_Disp(void);
void Serv_Temperature_Menu_Disp(void);
void Serv_EEprom_Menu_Disp(void);
void Serv_AD7843_Menu_Disp(void);
void Serv_DDLV_Menu_Disp(void);
void Serv_Interface_Menu_Disp(void);
void Serv_BriANDSpk_Menu_Disp(void);
void Serv_Screen_Menu_Disp(void);
/////////////////////////////////////////////////////////////////////////

//**********************************************************************
//1����  ��:Init_Main_Sel
//��������:��ʾ���� ģʽ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void Init_Main_Sel(sint32 op)
{
    uint32 xy[8][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
	Ver_Line(80,SCR_YSIZE-5,640/2);	
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 210,40,Language_Str("��ѡ��������","Please choose one item")); 
    
    for(i=0;i<4;i++)
     Clr_Bar( 30,90+90*i,250,80,COL_DEEPGRAY);
    for(i=0;i<4;i++)
     Clr_Bar(360,90+90*i,250,80,COL_DEEPGRAY);
   	
   	Set_Chr_Color(COL_DEEPGREEN,COL_DEEPGRAY);
    Put_Line_Str( 50+60,120,Language_Str("SENSOR",          "SENSOR"));
	Put_Line_Str( 50+30,210,Language_Str("TEMPERATURE",     "TEMPERATURE"));
	Put_Line_Str( 50+60,300,Language_Str("EEPROM",          "EEPROM"));
	Put_Line_Str( 50+80,390,Language_Str("AD7843",          "AD7843"));
	
	Put_Line_Str( 390+60,120,Language_Str("DDLV",           "DDLV"));
	Put_Line_Str( 390+30,210,Language_Str("INTERFACE",      "INTERFACE"));
	Put_Line_Str( 390+20,300,Language_Str("BRIGHT&SPEAKER", "BRIGHT&SPEAKER"));
	Put_Line_Str( 390+60,390,Language_Str("SCREEN",         "SCREEN"));

    ////////////////////////////////////////create keylist 
    for(i=0;i<4;i++)
    {
     xy[i][0] = 20+250/2;
     xy[i][1] = 120+90*i;
    }
     for(;i<8;i++)
     {
      xy[i][0] = 360+250/2;
      xy[i][1] = 120+90*(i-4);
     }
    
    Create_ButtonList(xy,8);            
 }
//**********************************************************************
//2   ���ش����������ʼ��
//**********************************************************************
void Init_Service_Sensor(sint32 op)
{
    uint32 xy[5][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(  5,SCR_XSIZE-5,100);
	Hor_Line(  5,SCR_XSIZE-5,150);
    Hor_Line(  5,SCR_XSIZE-5,200);
    Hor_Line(  5,SCR_XSIZE-5,250);
    Hor_Line(  5,SCR_XSIZE-5,300);
    Hor_Line(  5,SCR_XSIZE-5,350);
    
    Ver_Line(100,350,200);
    Ver_Line(100,350,200+150*1);
    Ver_Line(100,350,200+150*2);
    Ver_Line(100,350,200+150*3);
	
	////////////////////////////
	for(i=0;i<4;i++)
	{
	 Clr_Bar(100+100*i,420,80,50,COL_THINBLUE);
     Set_Chr_Color(COL_WHITE,COL_THINBLUE);
	 Put_Line_Str(120+100*i,435,Language_Str("CH","CH"));
	 Put_Char(150+100*i,435,0x30+i);
	}
	
	Clr_Bar(680,420,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 710,435,Language_Str("�˳�","EXIT"));
		  	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 330,35,Language_Str("���������ݲ���","SENSOR DATA ANALYZE")); 

    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str( 250,      120,Language_Str("CH1",    "CH1"));
    Put_Line_Str( 250+150*1,120,Language_Str("CH2",    "CH2"));
    Put_Line_Str( 250+150*2,120,Language_Str("CH3",    "CH3"));
    Put_Line_Str( 250+150*3,120,Language_Str("CH4",    "CH4"));         
    Set_Chr_Color(COL_THINRED,COL_BLACK);
    Put_Line_Str( 50,170,Language_Str("ԭʼ����",    "RAW DATA"));
    //Put_Line_Str( 50,220,Language_Str("��һ���ӹ�",  "PROC1"));
	//Put_Line_Str( 50,270,Language_Str("�ڶ����ӹ�",  "PROC2"));
	Put_Line_Str( 50,320,Language_Str("�������ӹ�",  "PROC3"));
	
	for(i=0;i<4;i++)
    {
     xy[i][0] = 150+100*i;
     xy[i][1] = 420+50/2;
    }
	
     xy[i][0] = 680+100/2;
     xy[i][1] = 420+50/2;
     Create_ButtonList(xy,5);
 }
//**********************************************************************
//3   �¶ȴ����������ʼ��
//**********************************************************************
void Init_Service_Temperature(sint32 op)
{
    uint32 xy[1][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150);
    Hor_Line(5,SCR_XSIZE-5,200);
	Hor_Line(5,SCR_XSIZE-5,250);
	
	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("�˳�","EXIT"));
		  	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("�¶ȴ��������ݲ���","TEMP SENSOR ANALYZE")); 

    Set_Chr_Color(COL_DEEPBLUE,COL_BLACK);
    Put_Line_Str( 50,120,Language_Str("�¶ȴ�����ԭʼ����","DATA1"));
	Put_Line_Str( 50,170,Language_Str("�¶�ֵ",            "DATA2"));
  
   
    for(i=0;i<1;i++)
    {
     xy[i][0] = 500+100/2;;
     xy[i][1] = 425;
    }
     Create_ButtonList(xy,1);          
 }
//**********************************************************************
//4   EEPROM �����ʼ��
//**********************************************************************
void Init_Service_EEprom(sint32 op)
{
    uint32 xy[6][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150);  //
    Hor_Line(5,SCR_XSIZE-5,200);  //
    Hor_Line(5,SCR_XSIZE-5,250);  //
    Hor_Line(5,SCR_XSIZE-5,300);  //
    Hor_Line(5,SCR_XSIZE-5,350);  //
    
	Ver_Line(100,350,SCR_YSIZE/2);
		
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("EEPROM  ���ݲ���","EEPROM DATA ANALYZE")); 
	Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
	Put_Line_Str(  50,115,Language_Str("����������",     "FULL RANGE DATA")); 
	Put_Line_Str(  50,165,Language_Str("UART TYPE",      "UART TYPE")); 
	Put_Line_Str(  50,215,Language_Str("�ͺ�ѡ��",       "MODEL SEL"));
	Put_Line_Str(  50,265,Language_Str("�ͺſ���",       "MODEL ON/OFF"));
	Put_Line_Str(  50,315,Language_Str("LOGO����",       "LOGO ON/OFF"));
	
	Clr_Bar( 250, 100,150,45,COL_THINBLUE);
	Clr_Bar( 250, 150,150,45,COL_THINBLUE);
	Clr_Bar( 250, 200,150,45,COL_THINBLUE);
	Clr_Bar( 250, 250,150,45,COL_THINBLUE);
	Clr_Bar( 250, 300,150,45,COL_THINBLUE);
	
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( 280, 110,Language_Str("��",       "READ")); 
	Put_Line_Str( 280, 160,Language_Str("ѡ��",     "SEL"));
	Put_Line_Str( 280, 210,Language_Str("ѡ��",     "SEL"));
	Put_Line_Str( 280, 260,Language_Str("ѡ��",     "SEL"));
	Put_Line_Str( 280, 310,Language_Str("ѡ��",     "SEL"));
	
	Clr_Bar(520,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 550,415,Language_Str("�˳�","EXIT"));
		  	
	
    //5�����԰�ť
    for(i=0;i<5;i++)
    {
     xy[i][0] = 250 + 150/2;
     xy[i][1] = 50/2 + 100 + 50*i ;
    }
    //1���˳���ť
   
     xy[i][0] = 570;
     xy[i][1] = 430;
    
    Create_ButtonList(xy,6);
    
    uart_tmp  = 0x55;
    logo_tmp  = 0x55;
    model_tmp = 0x55;  
    model_display_tmp = 0x55;                
 }
//**********************************************************************
//5  ����оƬ �����ʼ��
//**********************************************************************
void Init_Service_AD7843(sint32 op)
{
    uint32 xy[1][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150);
    Hor_Line(5,SCR_XSIZE-5,200);
    Ver_Line(100,200,SCR_XSIZE/2);
	
	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("�˳�","EXIT"));
		  	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("���������ݲ���","TOUCH SCREEN DATA ANALYZE")); 

    Set_Chr_Color(COL_DEEPBLUE,COL_BLACK);
    Put_Line_Str( 50,120,Language_Str("������ԭʼ����","DATA1"));
	Put_Line_Str( 50,170,Language_Str("��Ļ����ֵ",    "DATA2"));
   
    for(i=0;i<1;i++)
    {
     xy[i][0] = 550;
     xy[i][1] = 425;
    }
     Create_ButtonList(xy,1);                     
 }
 
//**********************************************************************
//6    ��� �����ʼ��
//**********************************************************************
void Init_Service_DDLV(sint32 op)
{
    uint32 xy[4][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
    Ver_Line(100,SCR_XSIZE-5,SCR_XSIZE/2);

	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150); //title
    Hor_Line(5,SCR_XSIZE-5,200); //led1
	Hor_Line(5,SCR_XSIZE-5,250); //led2
	Hor_Line(5,SCR_XSIZE-5,300); //led3
	 	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("�絼�ʵ�·����","DDLV AD TEST")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("�˳�","EXIT"));
	
	Clr_Bar(50,105,200,40,COL_THINBLUE);	
	Clr_Bar(50,155,200,40,COL_THINBLUE);
	Clr_Bar(50,205,200,40,COL_THINBLUE);
	
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( Language_Pos(80,80),115,Language_Str("�л���Դͨ��",  "POWER SWITCH"));
	Put_Line_Str( Language_Pos(80,80),165,Language_Str("�л���������",  "RES SWITCH"));
	//Put_Line_Str( Language_Pos(150,150),265,Language_Str("ֹͣ",  "STOP"));
	
    for(i=0;i<3;i++)
    {
     xy[i][0] = 150;
     xy[i][1] = 125+50*i;
    }
   
     xy[i][0] = 550;
     xy[i][1] = 425;
   
     Create_ButtonList(xy,4);   
     
     power_ch = 1;
     res_ch   = 1;
     CD4052_POWER_SEL_1;     
 }
//**********************************************************************
//7   �ӿڵ�· �����ʼ��
//**********************************************************************
void Init_Service_Interface(sint32 op)
 {
    uint32 xy[4][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
    Ver_Line(100,SCR_XSIZE-5,SCR_XSIZE/2);

	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150); //title
    Hor_Line(5,SCR_XSIZE-5,200); //led1
	Hor_Line(5,SCR_XSIZE-5,250); //led2
	Hor_Line(5,SCR_XSIZE-5,300); //led3
	 	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("��Χ�ӿڵ�·����","CPU IO TEST")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("�˳�","EXIT"));
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str(  50,120,Language_Str("����ӿ�","OUTPUT SIGNAL"));
	Put_Line_Str( 370,120,Language_Str("����ӿ�","INPUT  SIGNAL"));
	Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
	Put_Line_Str(  50,170,Language_Str("����ָʾ��",  "SYSTEM RUN"));
	Put_Line_Str(  50,220,Language_Str("����ָʾ��1", "WIRELESS LED1"));
	Put_Line_Str(  50,270,Language_Str("����ָʾ��2", "WIRELESS LED2"));
	
	Clr_Bar(200,155,80,40,COL_THINBLUE);
	Clr_Bar(200,205,80,40,COL_THINBLUE);
	Clr_Bar(200,255,80,40,COL_THINBLUE);
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( Language_Pos(220,210),165,Language_Str("����",  "ON-OFF"));
	Put_Line_Str( Language_Pos(220,210),215,Language_Str("����",  "ON-OFF"));
	Put_Line_Str( Language_Pos(220,210),265,Language_Str("����",  "ON-OFF"));
	
    for(i=0;i<3;i++)
    {
     xy[i][0] = 240;
     xy[i][1] = 175+50*i;
    }
   
     xy[i][0] = 550;
     xy[i][1] = 425;
   
     Create_ButtonList(xy,4);                     
 }

//**********************************************************************
//8   �������� �����ʼ��
//**********************************************************************
void Init_Service_BriANDSpeaker(sint32 op)
 {
   uint32 xy[5][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
    

	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150);
    Ver_Line(100,475,SCR_XSIZE/2);
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( Language_Pos(250,230),40,Language_Str("������������","BRIGHT SPEAKER TEST")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("�˳�","EXIT"));
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str(  Language_Pos(120,100),120,Language_Str("���ȵ���","BRIGHTNESS ADJUST"));
	Put_Line_Str(  Language_Pos(440,420),120,Language_Str("��������","SPEAKER ADJUST"));

	Clr_Bar(50,170,200,80,COL_THINBLUE);  //��������
	Clr_Bar(50,270,200,80,COL_THINBLUE);  //���ȼ�С
	Clr_Bar(370,170,200,80,COL_THINBLUE);  //��������
	Clr_Bar(370,270,200,80,COL_THINBLUE);  //������С
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( 100,200,Language_Str("��������",  "BRIGHT ADD"));
	Put_Line_Str( 100,300,Language_Str("���ȼ�С",  "BRIGHT SUB"));
	Put_Line_Str( 420,200,Language_Str("��������",  "VOLUME ADD"));
	Put_Line_Str( 420,300,Language_Str("������С",  "VOLUME SUB"));
	
    for(i=0;i<2;i++)
    {
     xy[i][0] = 150;
     xy[i][1] = 210+100*i;
    }
   for(;i<4;i++)
    {
     xy[i][0] = 470;
     xy[i][1] = 210+100*(i-2);
    }
   
     xy[i][0] = 500;
     xy[i][1] = 425;
   
     Create_ButtonList(xy,5);                      
 }
//**********************************************************************
//9  ��Ļ���� �����ʼ��
//**********************************************************************
void Init_Service_Screen(sint32 op)
{
   uint32 xy[3][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
   	 	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 250,40,Language_Str("��Ļ��ɫ����","SCREEN TEST")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("�˳�","EXIT"));
	
	
	Clr_Bar(100,400,150,50,COL_THINBLUE);
	Clr_Bar(300,400,150,50,COL_THINBLUE);
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( 150,415,Language_Str("��ʼ",  "START"));
	Put_Line_Str( 350,415,Language_Str("ֹͣ",  "STOP"));
	
    for(i=0;i<2;i++)
    {
     xy[i][0] = 175+200*i;
     xy[i][1] = 425;
    }
   
     xy[i][0] = 550;
     xy[i][1] = 425;
   
     Create_ButtonList(xy,3);                
  
           
 }
 
//******************************************************************************************************
//10���� �� Start_Service_process
//��������:����ģʽ���˵���Ŀ �������
//��ڲ���:��
//���ڲ���:�� xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//******************************************************************************************************
void  Start_Service_process(void)
{
 struct menu_str menu[9] = {
    {MENU_SERV_MAIN_SEL,        Init_Main_Sel,              Menu_Serv_Main_Key,           Serv_Main_Menu_Disp,        NULL},//������
    
    {MENU_SERV_SEL_SENSOR_OP,   Init_Service_Sensor,        Menu_Serv_Sensor_Key,       Serv_Sensor_Menu_Disp,      NULL},//��Ŵ�����
    {MENU_SERV_SEL_TEMP_OP,     Init_Service_Temperature,   Menu_Serv_Sensor_Key,  Serv_Temperature_Menu_Disp,      NULL},//�¶ȴ�����
    {MENU_SERV_SEL_EEPROM_OP,   Init_Service_EEprom,        Menu_Serv_EEprom_Key,       Serv_EEprom_Menu_Disp,      NULL},//EEPROM
    {MENU_SERV_SEL_AD7843_OP,   Init_Service_AD7843,        Menu_Serv_AD7843_Key,       Serv_AD7843_Menu_Disp,      NULL},//AD7843
    {MENU_SERV_SEL_DDLV_OP,     Init_Service_DDLV,          Menu_Serv_DDLV_Key,           Serv_DDLV_Menu_Disp,      NULL},//DDLV
    {MENU_SERV_SEL_INTERFACE_OP,Init_Service_Interface,     Menu_Serv_Interface_Key, Serv_Interface_Menu_Disp,      NULL},//INTERFACE
    {MENU_SERV_SEL_BRISPK_OP,   Init_Service_BriANDSpeaker, Menu_Serv_BriANDSpk_Key, Serv_BriANDSpk_Menu_Disp,      NULL},//��������
    {MENU_SERV_SEL_SCREEN_OP,   Init_Service_Screen,        Menu_Serv_Screen_Key,       Serv_Screen_Menu_Disp,      NULL} //��Ļ
  
    };//��չ��
    sint32 cur_op,next_op;    //��ǰѡ������л�����ѡ��(bits[7:0]:ѡ���;bits[15:8]:�����;bits[23:16]:ѡ�����ʾ״̬)
           uint16 ad8325_tmp, key_c;            //������
    static uint8  ddlv_buf_index=0;
    static uint8  sample_dly_cnt=0;  
      
    cur_op =0xffffffff ;  
    next_op = MENU_SERV_MAIN_SEL;
    
    while (1)
    {
        if ((cur_op^next_op)&0x0000ff00)//��ǰ�����Ľ����׼���л����Ľ��治һ��
        {//��ʼ������
            (*(menu[(next_op>>8)&0xff].menu_init))(next_op);
          
        }
        cur_op = next_op;        //"����/�˵�/ѡ��"֮����л�
        //////////////////////////////////////////////////////////////
        
        #if DDLV_SOFT
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
           
           //ADS1100_Match_Proc();  
           
          }
         }        
        #endif
     
        /////////////////// ��ѯ�����µ�AD���� ����ԭ�ȵ��������Ѿ�����ȡ����
        if(TRUE==flag_new_data)
        {
         flag_new_data = FALSE;
         #if SDJ_SOFT
         filter();
         #else
         filter2();
         #endif
        }
        ////////////////////////////////////////////////////////100ms
        if (flag_10ms_ok)
        {
            flag_10ms_ok = FALSE;
            key_value = Get_PositionXY();     //����ɨ��
            next_op = (*(menu[(cur_op>>8)&0xff].op_pro))(key_value,cur_op);    //ѡ���
        }    
        ////////////////////////////////////////////////////////300ms
        if (flag_100ms_ok)
        {
            flag_100ms_ok = FALSE;    
            (*(menu[(cur_op>>8)&0xff].disp_pro))();    //��ʾ������
                      
        }
        ////////////////////////////////////////////////////////500ms
        if (flag_500ms_ok)
        {//500msʱ�䵽
          flag_500ms_ok = FALSE;            
          LED_CPU_RUN_Cpl(); //CPU����ָʾ����˸Ƶ��1Hz
        }                
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/*///////////////////////////////////////////////////
/*11  ������ ������Ӧ����MAIN
//////////////////////////////////////////////////*/
uint32  Action_MainMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<8;i++)
	   {
	     if((abs((*p).X_Position-x)< 300/2) && (abs((*p).Y_Position-y)< 80/2))
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
/*12  ����Ŀ������Ӧ���� 
//////////////////////////////////////////////////*/
uint32  Action_SensorMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*13  EEPROM������Ӧ����
//////////////////////////////////////////////////*/
uint32  Action_EEPromMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<6;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*13  EEPROM������Ӧ����
//////////////////////////////////////////////////*/
uint32  Action_AD7843MainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<1;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*14  interface ������Ӧ����
//////////////////////////////////////////////////*/
uint32  Action_InterfaceMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<4;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*15  brightness and speaker  ������Ӧ����
//////////////////////////////////////////////////*/
uint32  Action_BrightSpeakerMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     if((abs((*p).X_Position-x)< 150/2) && (abs((*p).Y_Position-y)< 80/2))
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
/*16  SCREEN TEST  ������Ӧ����
//////////////////////////////////////////////////*/
uint32  Action_ScreenTestMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<3;i++)
	   {
	     if((abs((*p).X_Position-x)< 150/2) && (abs((*p).Y_Position-y)< 50/2))
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
//**********************************************************************
//17������:Menu_Serv_Main_Key
//��������:�������µ�ѡ���
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
static  sint32 Menu_Serv_Main_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_MainMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://sensor
  	           #if SDJ_SOFT
  	           op_id = MENU_SERV_SEL_SENSOR_OP;
  	           #endif
  	           break;
    	case 2://temperature
    	       //op_id = MENU_SERV_SEL_TEMP_OP;
    	       break;
       	case 3://eeprom
       	       op_id = MENU_SERV_SEL_EEPROM_OP;
       	       break;
       	case 4://ad7843
       	       op_id = MENU_SERV_SEL_AD7843_OP;
               break;
        case 5://
               #if DDLV_SOFT
  	           op_id = MENU_SERV_SEL_DDLV_OP;
  	           #endif
  	           break;
    	case 6://interface
    	       //op_id = MENU_SERV_SEL_INTERFACE_OP;
    	       break;
       	case 7://brightness and speaker
       	       //op_id = MENU_SERV_SEL_BRISPK_OP;
       	       break;
       	case 8://screen
       	       op_id = MENU_SERV_SEL_SCREEN_OP;
       	        break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//18   ���ش���������ļ��̴���
//**********************************************************************
static  sint32 Menu_Serv_Sensor_Key(uint32 key_c,sint32 op)
{
     sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_SensorMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    
    #if SDJ_SOFT
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1:select_ch = 1;AD7190Configuration(1);break;
  	    case 2:select_ch = 2;AD7190Configuration(2);break;
  	    case 3:select_ch = 3;AD7190Configuration(4);break;
  	    case 4:select_ch = 4;AD7190Configuration(8);break;
  	   	case 5://return
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    #endif
    
    return(op_id);
  }
//**********************************************************************
//19   �¶ȴ���������ļ��̴���
//**********************************************************************
static  sint32 Menu_Serv_Temperature_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_MainMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://CHINESE
  	           //language_var_tmp = CHINESE ; //Set_Language_Type(CHINESE);
  	           break;
    	case 2://ENGLISH
    	       //language_var_tmp = ENGLISH;  //Set_Language_Type(ENGLISH);
    	       break;
       	case 3://Esc
       	       //op_id =  MENU_OP_OPER;  // return
       	       break;
       	case 4://OK
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//20   EEPROM ����ļ��̴���
//**********************************************************************
static  sint32 Menu_Serv_EEprom_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_EEPromMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	   	case 1://read full range data
    	       if(OK == Init_Sys_Cal_Varible())
  	           eeprom_full_flash_flag = TRUE;
  	           else
  	           eeprom_full_flash_flag = FALSE;
    	       break;
       	case 2://uart_type
       	       Uart_Printer_Flag++;
       	       EEPROM_para_config(EEP_UART_ADDR,Uart_Printer_Flag&0x01);
       	       break;
    	case 3://model select
  	           version_811++;
    	       EEPROM_para_config(EEP_MODEL_ADDR,version_811&0x01);
    	       break;
      	
    	case 4://model display sel
  	           model_display++;
    	       EEPROM_para_config(EEP_MODEL_DIS_ADDR,model_display&0x01);
    	       break;
      	
    	case 5://LOGO flag
    	       LOGO_flag++;
    	       EEPROM_para_config(EEP_LOGO_DIS_ADDR,LOGO_flag&0x01);
    	       break;
        
      	case 6://exit
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//21   ������ ����ļ��̴���
//**********************************************************************
static  sint32 Menu_Serv_AD7843_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_AD7843MainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1:
  	            op_id =  MENU_SERV_MAIN_SEL;
  	           break;
    	
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  
  }
//**********************************************************************
//22  ������ļ��̴���
//**********************************************************************
static  sint32 Menu_Serv_DDLV_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
   // return(op_id);
    i = Action_InterfaceMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://�л� ��Դ
  	           power_ch++;
  	           if(7 == power_ch)
  	           power_ch = 1;
  	           switch(power_ch)
  	           {
  	            case 1: CD4052_POWER_SEL_1;break;
  	            case 2: CD4052_POWER_SEL_2;break;
  	            case 3: CD4052_POWER_SEL_3;break;
  	            case 4: CD4052_POWER_SEL_4;break;
  	            case 5: CD4052_POWER_SEL_5;break;
  	            case 6: CD4052_POWER_SEL_6;break;
  	            default:break;
  	           }
  	           break;
    	case 2://f��������ѡ��
    	       res_ch++;
    	       if(5 == res_ch)
  	              res_ch = 1;
  	           switch(res_ch)
  	           {
  	            case 1: CD4053_SEL_RES_200R;   break;
  	            case 2: CD4053_SEL_RES_2K;     break;
  	            case 3: CD4053_SEL_RES_200K;   break;
  	            case 4: CD4053_SEL_RES_2M;     break;
  	          
  	            default:break;
  	           }
    	
    	       break;
       	case 3://ֹͣ
       	      // FIO0CLR=EPM7_MOTOR_P1;
       	      // FIO0CLR=EPM7_MOTOR_P2;
       	       break;
       	case 4://
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	  //  Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }  
//**********************************************************************
//23  �ӿ� ����ļ��̴���
//**********************************************************************
static  sint32 Menu_Serv_Interface_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
   // return(op_id);
    i = Action_InterfaceMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://ϵͳ���е�ȡ��
  	           LED_CPU_RUN_Cpl();
  	           break;
    	case 2://WIRELESS LED1
    	       //Wireless_Run_Led1_Cpl();
    	       break;
       	case 3://WIRELESS LED1
       	       //Wireless_Run_Led2_Cpl();
       	       break;
       	case 4://EXIT
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	  //  Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//24  �������Ƚ���ļ��̴���
//**********************************************************************
static  sint32 Menu_Serv_BriANDSpk_Key(uint32 key_c,sint32 op)
{
    uint8  brightness_tx_buf[4];
    uint32 i,j;
    sint32 op_id;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_BrightSpeakerMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://��������
  	           lcd_brightness++;
  	           if(lcd_brightness > BRIGHT_CLASS_TOTAL)
  	              lcd_brightness = BRIGHT_CLASS_TOTAL;
  	           Set_LCD_Brightness(lcd_brightness);
  	           
  	           brightness_tx_buf[0] = (uint8)(lcd_brightness);
  	           brightness_tx_buf[1] = 0;
  	           brightness_tx_buf[2] = 0;
  	           brightness_tx_buf[3] = brightness_tx_buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_BRIGHT_ADDR,brightness_tx_buf, 4);
  	           break;
    	case 2://���ȼ�С
    	       if(lcd_brightness>1)
    	       lcd_brightness--;
    	       Set_LCD_Brightness(lcd_brightness);
    	       
    	       brightness_tx_buf[0] = (uint8)(lcd_brightness);
  	           brightness_tx_buf[1] = 0;
  	           brightness_tx_buf[2] = 0;
  	           brightness_tx_buf[3] = brightness_tx_buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_BRIGHT_ADDR,brightness_tx_buf, 4);
    	       break;
       	case 3://��������
       	       
       	       break;
       	case 4://������С
       	             	       
       	       break;       
   	    case 5://exit
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//25   ��Ļ���� ����ļ��̴���
//**********************************************************************
static  sint32 Menu_Serv_Screen_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_ScreenTestMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://start
  	           flash_scr_flag = TRUE;
  	           break;
    	case 2://stop
    	       flash_scr_flag = FALSE;
    	       break;
        case 3://exit
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	  //  Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }

//**********************************************************************
//26������:Serv_Main_Menu_Disp               
//��������:�������µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Serv_Main_Menu_Disp(void)
{

}

//**********************************************************************
//27 ���ش�����������ʾ
//**********************************************************************
void Serv_Sensor_Menu_Disp(void)
 {
  uint32 i;
 
   
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      //ch = Get_AD_Channel();
      for(i=0;i<4;i++)
       {    
        Put_Data(220+150*i,  170,      "%8d",Get_Raw_Data(i));
        
        //Put_Data(220+150*ch,  170+50*2, "%8d",Get_Ave2_Data());
        Put_Data(220+150*i,  170+50*3, "%8d",Get_Filter_Data(i)); 
       }
       Put_Data(220+150,    170+50*1, "%03d",select_ch);
 }    
//**********************************************************************
//28  �¶ȴ�����������ʾ
//**********************************************************************
void Serv_Temperature_Menu_Disp(void)
{
}
//**********************************************************************
//29  EEPROM  ������������ʾ
//**********************************************************************
void Serv_EEprom_Menu_Disp(void)
 {
   //�˴���ʾ��ǰ����������
   if(Uart_Printer_Flag != uart_tmp)
     {
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      uart_tmp = Uart_Printer_Flag;
      if(uart_tmp&0x01)
	  Put_Line_Str(500, 160,Language_Str("TO PRINTER",  "TO PRINTER")); 
      else
      Put_Line_Str(500, 160,Language_Str("TO PC     ",  "TO PC     "));  
     }
  
   if(version_811 != model_tmp)
     {
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      model_tmp = version_811;
      #if SDJ_SOFT
      if(model_tmp&0x01)
	  Put_Line_Str(500, 210,Language_Str("P811",  "P811")); 
      else
      Put_Line_Str(500, 210,Language_Str("P801",  "P801"));  
      #else
      if(model_tmp&0x01)
      Put_Line_Str(500, 210,Language_Str("P812",  "P812")); 
      else
      Put_Line_Str(500, 210,Language_Str("P802",  "P802"));  
      
      #endif
     }
     
   if(model_display != model_display_tmp)
     {
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      model_display_tmp = model_display;
      if(model_display&0x01)
	  Put_Line_Str(500, 260,Language_Str("YES",  "YES")); 
      else
      Put_Line_Str(500, 260,Language_Str("NO ",  "NO "));  
     }
     
   if(LOGO_flag != logo_tmp)
     {
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      logo_tmp = LOGO_flag;
      if(logo_tmp&0x01)
	  Put_Line_Str(500, 310,Language_Str("YES",   "YES")); 
      else
      Put_Line_Str(500, 310,Language_Str("NO ",   " NO"));  
     }
  
  
  }
//**********************************************************************
//30  ������������ʾ
//**********************************************************************
void Serv_AD7843_Menu_Disp(void)
 {
  uint32 i,j;
  machine_mode =  MACHINE_SCREENCAL_MODE;
  i = Get_PositionXY(); //read ad data
  
  machine_mode =  MACHINE_NORMAL_MODE;
  j = Get_PositionXY(); //read XY data
  
  if((i!=0)&&(j!=0))
   {
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Data(370+200,110,"%4d",i>>16);
    Put_Data(470+200,110,"%4d",i&0xffff);
    Put_Data(370+200,160,"%4d",j>>16);
    Put_Data(470+200,160,"%4d",j&0xffff);
   }
   else
   {
   Clr_Bar(370+200,110,200,40,COL_BLACK);
   Clr_Bar(370+200,160,200,40,COL_BLACK);
   }
 }
//**********************************************************************
//31  ��������ʾ
//**********************************************************************
void Serv_DDLV_Menu_Disp(void)
{
  uint32 i,j;

  Set_Chr_Color(COL_WHITE,COL_BLACK);
  Put_Data(300+200,115,"%d",power_ch);  //power_ch
  Put_Data(300+200,165,"%d",res_ch);    //res_ch
  Put_Data(300+200,215,"%07d",Get_Filter_ADS1100Data());    //ad1100_data
  
}
//**********************************************************************
//32  �ӿڽ�����ʾ
//**********************************************************************
void Serv_Interface_Menu_Disp(void)
{
}
//**********************************************************************
//33  �������Ƚ�����ʾ
//**********************************************************************
void Serv_BriANDSpk_Menu_Disp(void)
{
}
//**********************************************************************
//34  ��Ļ���Խ�����ʾ
//**********************************************************************
void Serv_Screen_Menu_Disp(void)
{
 uint32 i;
 //800/16 = 50
 static  uint32 j;
 static  uint32 flash_var = 0;
 static  uint32 flash_col_var = 0;
 
 flash_var++;
 if(flash_var > 16)
  {
   flash_var = 0;
   j++;
   if(16 == j)
   j=0;
  }
  
 if(TRUE == flash_scr_flag)
 {
  Clr_Bar(40*flash_var,100,40,260,(flash_col_var+j)%16); //ʵ��ƫ����ʾ
  flash_col_var++;
  if(16 == flash_col_var)
   flash_col_var = 0;
  
 }
 
}