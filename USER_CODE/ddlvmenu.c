
//**********************************************************************
//�ļ�˵��              
//����
//�絼�ʲ��������� �˵� ���䴦�� �� ��ʾ
//**********************************************************************
#include "Font_Lib.h"

#include "lcd2478.h"
#include "graph.h" 
#include "global.h"
#include "normal_operation.h"
#include "type_def.h"
#include "disp.h"
#include "i2c_com.h"
#include "eeprom.h"
#include "ad7843.h"

static  un_rec_data ddlv_cons_tmp,Temp_comp_tmp,TDS_cons_tmp; //����ϵ��

static  uint32      DDLV_Param_cur,DDLV_Mode_cur;             //��¼��ǰ������

static  uint32      DDLV_Param_tmp,DDLV_Mode_tmp;             //�������� ����ģʽ
        uint32      timer_tmp,balance_time_tmp;               //��ʱʱ�� ƽ��ʱ�� ������������Ҫ�ⲿ����
static  un_rec_data balance_data_tmp;                         //ƽ��ֵ

///////////////////////////////////////////////////////
static  uint32 num_buf[7],num_buf_full;
static  float  Num_Input_tmp;
static  uint32 Num_Input_Pos;//����ĵڼ�λ

//////////////////////////////////////////////////////////
//01
//////////////////////////////////////////////////////////
/*///////////////////////////////////////////////////
/*
/*8 �������ð����ж�
//////////////////////////////////////////////////*/
uint32  Action_DDLV_consCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 40/2))
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
//23 ����  ��:LangSel_Interface
//��������:����ѡ���ʼ�� ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void DDLV_CONS_Input_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[5][2];
   
   if(1==need_recover)
    {
     need_recover = 0;
     Recover_Area(0,80,600,400,save_buf);
    }
    else
   {           
    //Create window
    Create_Window_Frame(256,80,260,320,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(270,85,Language_Str("��������","INSTR-CONS Input")); 

    Set_Chr_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(270,  100+60,  Language_Str("�缫����:",   "Cur E-CONS:"));
    Put_Line_Str(270,  100+120, Language_Str("�²�ϵ��:",   "Temp-CONS:"));
    Put_Line_Str(270,  100+180, Language_Str("TDS ϵ��:",   "TDS-FACTOR:"));
    
     
    Set_Graphic_Color(COL_BLACK,COL_DEEPBLUE); 
    Bar(400, 150, 400+100,150+40);
    Bar(400, 210, 400+100,210+40);
    Bar(400, 270, 400+100,270+40);
    
   
    Put_Button_Bar(280, 350,280+100, 350+40,3,COL_BLACK);//esc
    Put_Button_Bar(400, 350,400+100, 350+40,3,COL_BLACK);//ok
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(Language_Pos(310,300),  350+10,Language_Str("ȡ��",   "CANCEL"));
    Put_Line_Str(Language_Pos(430,440),  350+10,Language_Str("ȷ��",   "OK"));
    
    ddlv_cons_tmp.rec_float = elec_cons;
    Temp_comp_tmp.rec_float = temp_cons;
    TDS_cons_tmp.rec_float  = TDS_cons;
    
   }
  
    //create button list
    xy[0][0] = 400+100/2;    xy[0][1] = 150+40/2;
    xy[1][0] = 400+100/2;    xy[1][1] = 210+40/2;
    xy[2][0] = 400+100/2;    xy[2][1] = 270+40/2;
     
    xy[3][0] = 280+100/2;    xy[3][1] = 350+40/2;
    xy[4][0] = 400+100/2;    xy[4][1] = 350+40/2;
    
    Create_ButtonList(xy,5); 
    ///////////////////////////////
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Data_Float(400+35, 150+10, "%3.1f", ddlv_cons_tmp.rec_float);
    Put_Data_Float(400+35, 210+10, "%3.1f", Temp_comp_tmp.rec_float);
    Put_Data_Float(400+35, 270+10, "%3.1f",  TDS_cons_tmp.rec_float); 
}

//////////////////////////////////////////////////////////
//**********************************************************************
//17 �絼�ʳ��� �����˵�
//**********************************************************************
sint32 DDLV_CONS_Key_Pro(uint32 key_c,sint32 op)
{   
    uint8  tx_buf[16];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_DDLV_consCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //
  	           from_menu = MENU_DDLV_CONS_OPER;
  	           Save_Area(0,80,600,400,save_buf);
  	           op_id = MENU_FLOAT_INPUT_OPER;;
  	           break;
  	    case 2: //
  	           from_menu = MENU_DDLV_CONS_OPER+0x7777;
  	           Save_Area(0,80,600,400,save_buf);
  	           op_id = MENU_FLOAT_INPUT_OPER;;
  	           break;
  	    case 3: //
  	           from_menu = MENU_DDLV_CONS_OPER+0x8888;
  	           Save_Area(0,80,600,400,save_buf);
  	           op_id = MENU_FLOAT_INPUT_OPER;;
  	           break;
  	           
      	case 5://save to eeprom
       	       elec_cons = ddlv_cons_tmp.rec_float; 
       	       temp_cons = Temp_comp_tmp.rec_float;
       	       TDS_cons  = TDS_cons_tmp.rec_float;
       	       
       	       tx_buf[0] = (uint8)((ddlv_cons_tmp.rec_uint32>>24)&0xff);
       	       tx_buf[1] = (uint8)((ddlv_cons_tmp.rec_uint32>>16)&0xff);
  	           tx_buf[2] = (uint8)((ddlv_cons_tmp.rec_uint32>>8)&0xff);
  	           tx_buf[3] = (uint8)( ddlv_cons_tmp.rec_uint32&0xff);
  	           
  	           tx_buf[4] = (uint8)((Temp_comp_tmp.rec_uint32>>24)&0xff);
  	           tx_buf[5] = (uint8)((Temp_comp_tmp.rec_uint32>>16)&0xff);
  	           tx_buf[6] = (uint8)((Temp_comp_tmp.rec_uint32>>8)&0xff);
  	           tx_buf[7] = (uint8)( Temp_comp_tmp.rec_uint32&0xff);
  	          
  	           
  	           tx_buf[8] = (uint8)((TDS_cons_tmp.rec_uint32>>24)&0xff);
  	           tx_buf[9] = (uint8)((TDS_cons_tmp.rec_uint32>>16)&0xff);
  	           tx_buf[10]= (uint8)((TDS_cons_tmp.rec_uint32>>8)&0xff);
  	           tx_buf[11]= (uint8)( TDS_cons_tmp.rec_uint32&0xff);
  	           
  	           tx_buf[14] = REC_CHECK_DATA1;
  	           tx_buf[15] = REC_CHECK_DATA2;
  	           
  	           Write_EEPROM(EEP_DDLV_CONS_ADDR,tx_buf, 16);
  	           ////////////////////////////////
  	    case 4:
  	           need_recover2 = 1;
       	       op_id = MENU_SYSTEM_CONF_OPER; //�ص����ý���
       	       break;
       	       
        default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

void DDLV_CONS_Disp_Pro(void)
{
 
}


/*///////////////////////////////////////////////////
/* 21  ����ѡ��ѡ�� �� ���Ż���
//////////////////////////////////////////////////*/
void Display_DDLV_Mode(void)
{
   if(DDLV_Param_cur != DDLV_Param_tmp) 
    {
      DDLV_Param_cur = DDLV_Param_tmp;
      Clr_Bar(220,155,30,150,COL_DEEPGRAY);  
      Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
      switch(DDLV_Param_tmp)
      {
       case PARAM_DDLV:
                 Fill_Circle(240,180,9);
                 Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPGRAY);
                 Circle(240,180,6);
                 Circle(240,180,5);
                 Circle(240,180,4);
                 break;
       case PARAM_TDS:
                 Fill_Circle(240,230,9);
                 Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPGRAY);
                 Circle(240,230,6);
                 Circle(240,230,5);
                 Circle(240,230,4);
                 break;
       case PARAM_SALTY:
                 Fill_Circle(240,280,9);
                 Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPGRAY);
                 Circle(240,280,6);
                 Circle(240,280,5);
                 Circle(240,280,4);
                 break;
       default:break;
      }
    }
   ///////////////////////////////////////////////////////////////
    if(DDLV_Mode_cur != DDLV_Mode_tmp) 
    {
      DDLV_Mode_cur = DDLV_Mode_tmp;
      Clr_Bar(395,155,30,150,COL_DEEPGRAY);  
      Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
      switch(DDLV_Mode_tmp)
      {
       case MODE_CONTINUE:
                 Fill_Circle(410,180,9);
                 Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPGRAY);
                 Circle(410,180,6);
                 Circle(410,180,5);
                 Circle(410,180,4);
                 break;
       case MODE_TIMER:
                 Fill_Circle(410,230,9);
                 Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPGRAY);
                 Circle(410,230,6);
                 Circle(410,230,5);
                 Circle(410,230,4);
                 break;
       case MODE_BALANCE:
                 Fill_Circle(410,280,9);
                 Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPGRAY);
                 Circle(410,280,6);
                 Circle(410,280,5);
                 Circle(410,280,4);
                 break;
       default:break;
      }
    }  
  
 }  

//////////////////////////////////////////////////////
void ModeInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[11][2];
    
    //Create window
    if(1==need_recover)
     {
      need_recover = 0;
      Recover_Area(0,80,600,400,save_buf);
     }
    else
    { 
     Create_Window_Frame(115,80,480,305,3,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
     Put_Str(280,85,Language_Str("����ģʽѡ��","Mode Configuration")); 
    
     Set_Graphic_Color(COL_WHITE,COL_WHITE);
     Rectangle(130,130,130+130,310);
     Rectangle(280,130,280+150,310);
     Rectangle(450,130,450+135,355);
     
     Set_Chr_Color(COL_DEEPBLUE,COL_THINGRAY);
     Put_Line_Str(130+15,  120,  Language_Str("��������",   "PARAMETER"));
     Put_Line_Str(Language_Pos(305,325),  120,  Language_Str("����ģʽ",   "MODE"));
     Put_Line_Str(Language_Pos(470,490),  120,  Language_Str("��������",   "LIMIT"));
     //////////////////////////////////////////////////////////////////////////////////
     
     Set_Graphic_Color(COL_BLACK,COL_BLACK);
     Bar(135,160,   135+80,   160+40);    //�絼��
     Bar(135,210,   135+80,   210+40);    //TDS
     Bar(135,260,   135+80,   260+40);    //�ζ�
     ////////////////////////////////////////////////////
     Bar(285,160,   285+100,   160+40);    //��������
     Bar(285,210,   285+100,   210+40);    //��ʱ����
     Bar(285,260,   285+100,   260+40);    //ƽ�����
     ////////////////////////////////////////////////////
     ////////////////////////////////////////////////////
     Set_Chr_Color(COL_WHITE,COL_BLACK);
     Put_Line_Str(Language_Pos(140,150),  170,  Language_Str("�絼��",   "COND"));
     Put_Line_Str(155,  220,  Language_Str("TDS",      "TDS"));
     Put_Line_Str(150,  270,  Language_Str("�ζ�",     "SAL"));
     
     Put_Line_Str(290,  170,  Language_Str("��������",   "CONTINUE"));
     Put_Line_Str(Language_Pos(290,310),  220,  Language_Str("��ʱ����",   "TIMER"));
     Put_Line_Str(Language_Pos(290,295),  270,  Language_Str("ƽ�����",   "BALANCE"));
     /////////////////////////////////////////////////////////////////////////////////
     /////////////////////////////////////////////////////////////////////////////////
     
     Set_Chr_Color(COL_BLACK,COL_WHITE);
     Set_ASC_Lib(Arial16B_Font);
     Put_Line_Str(Language_Pos(470,490),  155,  Language_Str("��ʱ���",      "TIMER"));
     Put_Line_Str(Language_Pos(480,475),  225,  Language_Str("ƽ��ֵ",     "BAL-DATA"));
     Put_Line_Str(Language_Pos(470,475),  295,  Language_Str("ƽ��ʱ��",   "BAL-TIME"));
     ////////////////////////////////////////////////////
     Bar(470,175,   470+96,   175+30);    
     Bar(470,245,   470+96,   245+30);    
     Bar(470,315,   470+96,   315+30);   
     
     /////////////////////////////////////////////////////////////////////////////////
     /////////////////////////////////////////////////////////////////////////////////
     Put_Button_Bar(170, 340,170+100, 340+40,3,COL_BLACK);//ESC
     Put_Button_Bar(300, 340,300+100, 340+40,3,COL_BLACK);//OK
     Set_Chr_Color(COL_WHITE,COL_BLACK);
     Put_Line_Str(Language_Pos(200,190),  340+10,Language_Str("ȡ��",   "CANCEL"));
     Put_Line_Str(Language_Pos(330,340),  340+10,Language_Str("ȷ��",   "OK"));
     //������Ҫ��ʾ����Ϣ
     ///////////////////////////////
     DDLV_Param_tmp = DDLV_Param; 
     DDLV_Mode_tmp  = DDLV_Mode;
     ////////////////////////////////���ʱ�� ƽ��ʱ�� ƽ��ֵ��ֻ�ڳ�ʼ��ʱ������
     timer_tmp        = measure_timer_sec;
     balance_time_tmp = balance_sec; 
     balance_data_tmp.rec_float = balance_data;  
    } 
    //create button list
    xy[0][0] = 135+ 80/2;    xy[0][1] = 160+40/2;
    xy[1][0] = 135+ 80/2;    xy[1][1] = 210+40/2;
    xy[2][0] = 135+ 80/2;    xy[2][1] = 260+40/2;
    
    xy[3][0] = 285+100/2;    xy[3][1] = 160+40/2;
    xy[4][0] = 285+100/2;    xy[4][1] = 210+40/2;
    xy[5][0] = 285+100/2;    xy[5][1] = 260+40/2;
    
    xy[6][0] = 470+100/2;    xy[6][1] = 170+40/2;
    xy[7][0] = 470+100/2;    xy[7][1] = 240+40/2;
    xy[8][0] = 470+100/2;    xy[8][1] = 310+40/2;
    
    
    xy[9][0]  = 170+100/2;   xy[9][1]  = 340+40/2;
    xy[10][0] = 300+100/2;   xy[10][1] = 340+40/2;
                  
    Create_ButtonList(xy,11); 
   
    DDLV_Param_cur = PARAM_NULL; //redraw
    DDLV_Mode_cur  = MODE_NULL;  //redraw
    
    Put_Data(      500,   180, "%03d",        timer_tmp);
    Put_Data_Float(485,   250, "%5.1f",     balance_data_tmp.rec_float);
    Put_Data(      510,   320, "%02d",        balance_time_tmp);     
}
//////////////////////////////////////////////////////
//
//����ģʽ�� ѡ��
//////////////////////////////////////////////////////

/*///////////////////////////////////////////////////
//  22  ����ѡ��� �����ж�
//////////////////////////////////////////////////*/
uint32  Action_ModeMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;                    //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<11;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 40/2))
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
///////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////	
sint32 Mode_Key_Pro(uint32 key_c,sint32 op)
{
       
    uint8  eeprom_tx_buf[16];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_ModeMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://
  	           DDLV_Param_tmp = PARAM_DDLV; 
  	           break;
    	case 2://
    	       DDLV_Param_tmp = PARAM_TDS;  
    	       break;
    	case 3://  
    	       DDLV_Param_tmp = PARAM_SALTY;
               break;
  	    case 4://continue
  	           DDLV_Mode_tmp  = MODE_CONTINUE; 
  	           break;
    	case 5://timer
    	       DDLV_Mode_tmp  = MODE_TIMER;  
    	       break;
    	case 6://balance  
    	       DDLV_Mode_tmp  = MODE_BALANCE;
               break;        
        case 7:
               //���ö�ʱʱ��
               //if(DDLV_Mode_tmp == MODE_TIMER)
               //   {
               //    DDLV_Mode = MODE_TIMER;          //save
                   from_menu = MENU_MODE_SEL_OPER;
  	               Save_Area(0,80,600,400,save_buf);
  	               
  	               op_id = MENU_UINT_INPUT_OPER;
               //   }
               break;
        case 8:
               //ƽ����������
               //if(DDLV_Mode_tmp == MODE_BALANCE)
               //   {
               //    DDLV_Mode = MODE_BALANCE;
                   from_menu = MENU_MODE_SEL_OPER;
                   Save_Area(0,80,600,400,save_buf);
                   op_id = MENU_FLOAT_INPUT_OPER;
               //   }
     	       break;
        case 9:
               //ƽ��ʱ������
               //if(DDLV_Mode_tmp == MODE_BALANCE)
               //   {
               //    DDLV_Mode = MODE_BALANCE;
                   from_menu = MENU_MODE_SEL_OPER + 0x1111; 
                   Save_Area(0,80,600,400,save_buf);
                   op_id = MENU_UINT_INPUT_OPER;
               //   }
     	       break;
       	case 11://OK
//��90��ַ��ʼ���δ�������²��� 
//90:       ��ŵ�ǰ�Ĳ�������
//91:       ��ʱ��ǰ�Ĳ���ģʽ
//9293:     ��ʱģʽ�µĶ�ʱʱ�� ���Ϊ 999S
//9495:     ƽ��ģʽ�µ�ƽ��ʱ�� ���Ϊ  60S
//96979899: ƽ��ģʽ�µ�ƽ������ ������
//9E9F:     ��Ч���ݱ�־ 0X55AA
               DDLV_Param = DDLV_Param_tmp;
               DDLV_Mode  = DDLV_Mode_tmp;
               measure_timer_sec = timer_tmp;
               balance_sec       = balance_time_tmp;
               balance_data      = balance_data_tmp.rec_float;               
               
               ////////////////////////////////////////////////////
        	   eeprom_tx_buf[0]  = (uint8)(DDLV_Param & 0xff);
       	       eeprom_tx_buf[1]  = (uint8)(DDLV_Mode & 0xff);
       	       eeprom_tx_buf[2]  = (uint8)((measure_timer_sec>>8)&0xff);
  	           eeprom_tx_buf[3]  = (uint8)(measure_timer_sec&0xff);
  	           eeprom_tx_buf[4]  = 0;
  	           eeprom_tx_buf[5]  = (uint8)(balance_sec&0xff);
  	           
  	           eeprom_tx_buf[6]  = (uint8)((balance_data_tmp.rec_uint32>>24)&0xff);
  	           eeprom_tx_buf[7]  = (uint8)((balance_data_tmp.rec_uint32>>16)&0xff);
  	           eeprom_tx_buf[8]  = (uint8)((balance_data_tmp.rec_uint32>>8)&0xff);
  	           eeprom_tx_buf[9]  = (uint8)( balance_data_tmp.rec_uint32&0xff);
  	           
  	           eeprom_tx_buf[14] = REC_CHECK_DATA1;
  	           eeprom_tx_buf[15] = REC_CHECK_DATA2;   
  	           Write_EEPROM(EEP_DDLV_MODE_ADDR,eeprom_tx_buf, 16);
  	          
  	           //���ò������� 
  	           measure_timer_sec_real = measure_timer_sec;
  	           balance_sec_real       = balance_sec;
  	           
  	           ////////////////////////////////
       	case 10:
       	       need_recover2 = 1;
       	       op_id =  MENU_SYSTEM_CONF_OPER; //�ص����ý���
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

/////////////////////////////////////////////////////////
void Mode_Disp_Pro(void)
{
 Display_DDLV_Mode();
}
/////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////
void BalanceInput_Interface(sint32 cur_menu_op)
{
 //ƽ������ ���ý���
}

sint32 BalanceInput_Key_Pro(uint32 key_c,sint32 op)
{

}

void Balance_Disp_Pro(void)
{
 
}

//////////////////////////////////////////////////
void Para3Input_Interface(sint32 cur_menu_op)
{
}

sint32 Para3_Key_Pro(uint32 key_c,sint32 op)
{
}

void Para3_Disp_Pro(void)
{
 
}

//**********************************************************************
//28����  ��:NumberInput_Interface
//��������:�������� ����ĳ�ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void FloatInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[15][2];
    //static  const uint32 display_code[] = {};
    //Create window
    Create_Window_Frame(150-100,80,500,300,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(130,85,Language_Str("������","Please Input")); 
    
    Clr_Bar(300-100,120,200,32,COL_THINGRAY);    //������ʾ��
    
    for(i=0;i<5;i++)
    Put_Button_Bar(60+100*i,160,60+100*i+80,160+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(60+100*i,230,60+100*i+80,230+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(60+100*i,300,60+100*i+80,300+60,3,COL_BLACK);
   
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Data( 90+100*i, 180,"%d",i);
    for(i=0;i<5;i++)
    Put_Data( 90+100*i, 250,"%d",5+i);
    Put_Char(190, 310,'.');
   
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(75, 320,Language_Str("�˸�", "BACK"));
    //Put_Line_Str(380, 270,Language_Str("��", "NULL"));
    Put_Line_Str(380, 320,Language_Str("����", "ESC"));
    Put_Line_Str(480, 320,Language_Str("ȷ��", "OK"));
    //create button list
    for(j=0;j<3;j++)
     { for(i=0;i<5;i++)  
        {                //start+off+
         xy[i+5*j][0] =   60 + 100*i + 80/2;   
         xy[i+5*j][1] =  160 +  70*j + 60/2;
        }
     }
    ////////////////////init variable 
    for(i=0;i<7;i++)
     num_buf[i]  = 0;
     Num_Input_tmp   = 0.0;
     Num_Input_Pos = 0;  
               
    Create_ButtonList(xy,15);               
}

////////////////////////////////////////////////33333
/*///////////////////////////////////////////////////
/* 26 ������������ ����ʾ����
//////////////////////////////////////////////////*/
void FloatInput_Disp_Pro(void)
{
   static float display_tmp;
   if(display_tmp != Num_Input_tmp)
   {
    display_tmp = Num_Input_tmp;
    Clr_Bar(240,120,130,30,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    Set_ASC_Lib(Arial20B_Font);
    if(TRUE == num_buf_full)
    {
     Put_Data_Float(240,   125, "%8.4f",        Num_Input_tmp);
    }
    else
    {
     switch(Num_Input_Pos)
     {
      case 0:
      case 1:
      case 2:
      case 3:        Put_Data(260,   125,   "%3d",(uint32)Num_Input_tmp);break;
      case 4:  Put_Data_Float(260,   125, "%5.1f",        Num_Input_tmp);break;
      case 5:  Put_Data_Float(250,   125, "%6.2f",        Num_Input_tmp);break;
      case 6:  Put_Data_Float(250,   125, "%7.3f",        Num_Input_tmp);break;
      default:
               break;
      } //switch end
     }  //if     end
    }   //if     end
   }    //func   end
/*///////////////////////////////////////////////////
/*
/*
//////////////////////////////////////////////////*/
#define  NUMBER_WIDTH       80
#define  NUMBER_HEIGTH      60
#define  NUM_WEI_INPUT      01
#define  NUM_PCS_INPUT      02
/*///////////////////////////////////////////////////
/* 27  �������� �İ��� ��Ӧ
//////////////////////////////////////////////////*/
uint32  Action_FLOATInputMenuCfg(uint16 x,uint16 y,uint32 from_window)
	{
       uint32 i,Key_Code,a,b;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	  
	    for(i=0;i<15;i++)
	    {
	     if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 60/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	    }
	  
	   //////////////////////////////////////////////////////////�а�������
	   if((i<15)&&(Key_Status == KEY_PRESS))
		{                              //��ס����ʾ
 	      //���ӷ�����ʾ ֱ�������ɿ�
		 a = Key[i].X_Position - NUMBER_WIDTH/2;
		 b = Key[i].Y_Position - NUMBER_HEIGTH/2;
		 
		 Set_Chr_Color(COL_BLACK,COL_THINGRAY);
		 Set_ASC_Lib(Arial16B_Font);
		 Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,3,COL_THINGRAY);
	
		 Key_Code = 0xaa55;
		  switch(i)
		   {
		    case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			 Set_ASC_Lib(Arial20B_Font);
		 	 Put_Char(Key[i].X_Position-8,Key[i].Y_Position-10,0x30+i);   
		     break;
            case 10:  //�˸�
			
		     Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("�˸�","BACK"));   
		     break;
		    case 11:  //.
			       
			         Set_ASC_Lib(Arial20B_Font);
		             Put_Char(190, 310,'.'); 
		                
		     break;
		    case 12: 
			   //Put_Str(Key[i].X_Position-25,Key[i].Y_Position-5,Language_Str("�˸�","BACK"));   
		     break;
		    case 13:  //����
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("����","ESC"));   
		     break;
		    case 14:  //ȷ��
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("ȷ��","OK"));   
		     break;
		    default:break;
		   }
		 }
		///////////////////////////////////////////
		 else if(Key_Status == KEY_DO_START)
		{ 
		 Key_Status = KEY_DO_OVER; 
		 Key_Valid_Sound();                          
 	     //���ӷ�����ʾ ֱ�������ɿ� 
		 if(i<15)
		 { 
		  a = Key[i].X_Position - NUMBER_WIDTH/2;
		  b = Key[i].Y_Position - NUMBER_HEIGTH/2;
		 
		  Set_Chr_Color(COL_WHITE,COL_BLACK);
		  Set_ASC_Lib(Arial16B_Font);
		  Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,3,COL_BLACK); 
		 
		  switch(i)
		   {
		    case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			 Set_ASC_Lib(Arial20B_Font);
		 	 Put_Char(Key[i].X_Position-8,Key[i].Y_Position-10,0x30+i);   
		     break;
            case 10:  //�˸�
			
		     Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("�˸�","BACK"));   
		     break;
		    case 11:  //.
			      if(NUM_WEI_INPUT == from_window)
			       {
			         Set_ASC_Lib(Arial20B_Font);
		             Put_Char(190, 310,'.'); 
		            }    
		     break;
		    case 12: 
			   //Put_Str(Key[i].X_Position-25,Key[i].Y_Position-5,Language_Str("�˸�","BACK"));   
		     break;
		    case 13:  //����
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("����","ESC"));   
		     break;
		    case 14:  //ȷ��
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("ȷ��","OK"));   
		     break;
		    default:break;
		   }
		 }  
	   } 
	   return(Key_Code);
	}


///////////////////////////////////////////////////
//30  С�������봦��
///////////////////////////////////////////////////
//С���㴦��
void   dot_key_process(void)
   {
    uint32 i;
    
    if(0 == Num_Input_Pos)
      {
       for(i=0;i<7;i++)
       num_buf[i] = 0;
       Num_Input_Pos = 3;
      }
     /*else if(1 == Num_Input_Pos)
      {//��ʾ�Ѿ�������1������
       num_buf[2] = num_buf[0];
       Num_Input_Pos = 3;
      }
     else if(2 == Num_Input_Pos) 
      {//��ʾ�Ѿ�������1������
       num_buf[2] = num_buf[1];
       num_buf[1] = num_buf[0];
       num_buf[0] = 0;
       Num_Input_Pos = 3; 
      }*/
     else
     {
      Num_Input_Pos = 3; 
     }
   }
///////////////////////////////////////////   
// 31 �˸�����봦��
//////////////////////////////////////////
void   back_key_process(void)
	{
	 uint32 i;
     if(TRUE == num_buf_full) //ĩβ
       {
        Num_Input_Pos = 6;
        num_buf[6] = 0;
        num_buf_full = FALSE;
       }
     else if(Num_Input_Pos > 3)
       { //С�������˸���
        num_buf[Num_Input_Pos-1] = 0;
        Num_Input_Pos--; 
       }
     else 
      {
       if(3 == Num_Input_Pos)
         {
          num_buf[2] = num_buf[1];
          num_buf[1] = num_buf[0];
          num_buf[0] = 0;
          Num_Input_Pos--;
         }
  	   else if(2 == Num_Input_Pos)
         {
          num_buf[2] = num_buf[1];
          num_buf[1] = 0;
          num_buf[0] = 0;
          Num_Input_Pos--;
  	     } 
  	   else if(1 == Num_Input_Pos)
         {
          num_buf[2] = 0;
          num_buf[1] = 0;
          num_buf[0] = 0;
          Num_Input_Pos--;
         }
       else
       {;}
       //////////////////////////////////
       if((0 == num_buf[0])&&(0 == num_buf[1])&&(0 == num_buf[2]))
       Num_Input_Pos =0;   
     }    
  	}	
////////////////////////////////////////////////
//32  �õ��������ݵ���ʱ����
///////////////////////////////////////////////
void  Get_NumInput_Tmp(uint32 i)
{
 uint32 tmp_var;
 if(11 == i)                //С����
    dot_key_process(); 
    else if(10 == i)        //�˸�
    back_key_process();
    else
    {   //1-10 ���ּ�
      if(0 == Num_Input_Pos)
        { //��һ������
         Num_Input_Pos++;
         num_buf[0] = 0;
         num_buf[1] = 0;
         num_buf[2] = i;
        }
      else if(1 == Num_Input_Pos)
        {
         Num_Input_Pos++;
         num_buf[0] = 0;
         num_buf[1] = num_buf[2];
         num_buf[2] = i; 
        }
      else if(2 == Num_Input_Pos)
        {
         Num_Input_Pos++;
         num_buf[0] = num_buf[1];
         num_buf[1] = num_buf[2];
         num_buf[2] = i;       
        }   
      else   
        num_buf[Num_Input_Pos++] = i;
     ////////////////////////////////// 
     //�ж��Ƿ�������7λ��
     if(7 == Num_Input_Pos)
       {
        num_buf_full = TRUE;
        Num_Input_Pos = 6;
       }
      else
        num_buf_full = FALSE;
    }
    
  ////////////////////////////////////update tmp
  Num_Input_tmp = num_buf[0]*100+num_buf[1]*10+num_buf[2]+num_buf[3]*0.1+num_buf[4]*0.01+num_buf[5]*0.001+num_buf[6]*0.0001;
}

//**********************************************************************
//29����  ��:NumInput_Key_Pro
//��������:�������� ����İ�������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 FloatInput_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    i = Action_FLOATInputMenuCfg((key_c>>16)&0xffff,key_c&0xffff,NUM_WEI_INPUT); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1:
    	case 2:
       	case 3:
   		case 4:
   		case 5:
   	    case 6:
    	case 7:
       	case 8:
   		case 9:
   		case 10:
   		case 11:  //back    
   		case 12:  //.
   		      Get_NumInput_Tmp(i-1);
   		      break;
   		case 13:   //null
   	          break;
   		case 14: 
   		        //return         
   		        switch(from_menu)
   		        {
   		         case MENU_MODE_SEL_OPER: 
   		                                  op_id = MENU_MODE_SEL_OPER; break;
   		         case MENU_DDLV_CONS_OPER:
   		         case MENU_DDLV_CONS_OPER+0x7777:
   		         case MENU_DDLV_CONS_OPER+0x8888:
   		                                  op_id = MENU_DDLV_CONS_OPER;break;
   		         default:                 op_id = MENU_OP_OPER;       break;    
   		        }
   		        
                need_recover = 1;
   		        break;
   		case 15:   //ok  
   		          switch(from_menu)
   		            {
   		             case MENU_MODE_SEL_OPER: //�ɼ����� ƽ������
                          if((Num_Input_tmp>0.01)&&(Num_Input_tmp<200))
                              balance_data_tmp.rec_float = Num_Input_tmp;
                          op_id = MENU_MODE_SEL_OPER;
                          need_recover = 1;
                          break;   
   		             case MENU_DDLV_CONS_OPER: // �缫���� �û��趨
                          if((Num_Input_tmp>0.01)&&(Num_Input_tmp<100))
                              ddlv_cons_tmp.rec_float = Num_Input_tmp;
                          op_id = MENU_DDLV_CONS_OPER;
                          need_recover = 1;
                          break;
                     case MENU_DDLV_CONS_OPER+0x7777:  //�²�ϵ��
                          if((Num_Input_tmp>0.01)&&(Num_Input_tmp<5))
                              Temp_comp_tmp.rec_float = Num_Input_tmp;
                          op_id = MENU_DDLV_CONS_OPER;  
                          need_recover = 1;
                          break;
                     case MENU_DDLV_CONS_OPER+0x8888:  //TDS ϵ��
                          if((Num_Input_tmp>0.01)&&(Num_Input_tmp<200))
                              TDS_cons_tmp.rec_float = Num_Input_tmp;
                          op_id = MENU_DDLV_CONS_OPER; 
                          need_recover = 1;
                          break;
                     default:
                          op_id = MENU_OP_OPER; 
                          need_recover = 1;
                          break;
                    } 
                    break; 
   		default:break;
   	   }
   	  //Key_Status = KEY_DO_OVER;
    }
return(op_id);
}