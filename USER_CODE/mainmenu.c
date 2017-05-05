//**********************************************************************
//�ļ�˵��             
//(1)���˵���ʾ�봦��
//(2)���ֹ��ܰ����Ĵ���
//(3)10��У׼��ʾ�봦��
//�����б�
//1    ��ĳ����Ʒ���        DisplayLocation
//2    ASD LNT PRT �����ж�  Action_LNTMenuCfg
//3    ��λѡ������ʼ��    UnitSel_Interface
//4    ASDѡ������ʼ��	 ASDSel_Interface
//5    LNTѡ������ʼ��	 LNTSel_Interface
//6    PRTѡ������ʼ��	 PRTSel_Interface
//7    ��λѡ�񰴼����� 	 UnitSel_Key_Pro
//8    ASDѡ�񰴼����� 		 ASDSel_Key_Pro
//9    LNTѡ�񰴼����� 		 LNTSel_Key_Pro
//10   PRTѡ�񰴼����� 		 PRTSel_Key_Pro 
//11   ��λѡ����ʾ���� 	 UnitSel_Disp_Pro
//12   ASDѡ����ʾ���� 		 ASDSSel_Disp_Pro
//13   LNTѡ����ʾ���� 		 LNTSel_Disp_Pro
//14   PRTѡ����ʾ����    	 PRTSel_Disp_Pro
/////////////////////////////////
//15   10��У׼�İ��� �ж�   Action_CAL_10pointMenuCfg
//16   10��У׼�����ʼ��    Cal_10point_Interface
//17   10��У׼��������      Cal_10point_Key_Pro
//18   10��У׼��ʾ����      Cal_10point_Disp_Pro

//**********************************************************************
#include "Font_Lib.h"

#include "lcd2478.h"
#include "graph.h" 
#include "global.h"
#include "normal_operation.h"
#include "i2c_com.h"
#include "eeprom.h"
#include "type_def.h"

static   uint32 Cal_Point_Count;
static   uint32 Cal_Buf_Tmp[CAL_POINT_NUM+1];     //��ʱ��

static   uint32 lnt_var_tmp,asd_var_tmp,prt_mod_tmp,unit_var_tmp;

/////////////////////////////////////////////////////
//1 ��ĳ����Ʒ���
/////////////////////////////////////////////////////
void  DisplayLocation(uint32 variable)
  {
   Set_Graphic_Color(COL_DEEPBLUE,COL_WHITE);
   switch(variable)
   {
    case 0:
            Circle(190,140,9);
            Circle(190,140,8);
            Circle(190,140,7);
            Fill_Circle(190,140,3);
            break;
    case 1:
            Circle(190,190,9);
            Circle(190,190,8);
            Circle(190,190,7);
            Fill_Circle(190,190,3);
            break;
    case 2:
            Circle(190,240,9);
            Circle(190,240,8);
            Circle(190,240,7);
            Fill_Circle(190,240,3);
            break;
    case 3:
            Circle(190,290,9);
            Circle(190,290,8);
            Circle(190,290,7);
            Fill_Circle(190,290,3);
            break;
    default:break;
    }
  }

/*///////////////////////////////////////////////////
/*2  Action_LNTMenuCfg ���� �ж�
/*ASD LNT PRT ʹ��ͬһ������������
//////////////////////////////////////////////////*/
uint32  Action_LNTMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<6;i++)
	   {
	     if((abs((*p).X_Position-x)< DATE_CFG_WIDTH1/2) && (abs((*p).Y_Position-y)< DATE_CFG_HEIGTH1/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     Key_Status = KEY_DO_OVER; 
	     
	   return(Key_Code);
	}	
//**********************************************************************
//3������:UnitSel_Interface
//��������:��λѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void UnitSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[6][2];
   
     //Create window
    Create_Window_Frame(0,80,240,380-80,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(20,85,Language_Str("��λѡ��","Unit Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<4;i++)
    Bar(20,120+50*i,150,160+50*i);
    
    Bar(    20,120+50*4, 20+90,160+50*4);
    Bar(110+20,120+50*4,130+90,160+50*4);
    //fill text
    Set_ASC_Lib(Arial16B_Font); 
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Put_Line_Str(35, 130,Language_Str("��λ   ��",  "UNIT    g"));
    Put_Line_Str(35, 180,Language_Str("��λ ����",  "UNIT   mg"));
    Put_Line_Str(35, 230,Language_Str("��λ   ��",  "UNIT   Lb"));
    Put_Line_Str(35, 280,Language_Str("��λ ��˾",  "UNIT   Oz"));
    Put_Line_Str(Language_Pos(40, 50), 330,Language_Str("ȡ��","ESC"));
    Put_Line_Str(Language_Pos(150,160), 330,Language_Str("ȷ��","OK"));
   
    //create button list
    for(i=0;i<4;i++)
    {
    xy[i][0] = 20 + 130/2;
    xy[i][1] = 120 + 50*i + 20;
    }
    xy[4][0] = 20  + 90/2;
    xy[4][1] = 120 + 50*4 + 20;
    xy[5][0] = 130 + 90/2;
    xy[5][1] = 120 + 50*4 + 20;
    Create_ButtonList(xy,6); 
     
    //////display currentvalue
    DisplayLocation(current_unit-1);              
}

//**********************************************************************
//4������:ASDSel_Interface
//��������:ASDѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void ASDSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[6][2];
   
     //Create window
    Create_Window_Frame(0,80,240,380-80,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(20,85,Language_Str("�ȶ���ѡ��","ASD Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<4;i++)
    Bar(20,120+50*i,150,160+50*i);
    
    Bar(    20,120+50*4, 20+90,160+50*4);
    Bar(110+20,120+50*4,130+90,160+50*4);
    //fill text
    Set_ASC_Lib(Arial16B_Font); 
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Put_Line_Str(35, 130,Language_Str("�ȶ���0","   ASD-0"));
    Put_Line_Str(35, 180,Language_Str("�ȶ���1","   ASD-1"));
    Put_Line_Str(35, 230,Language_Str("�ȶ���2","   ASD-2"));
    Put_Line_Str(35, 280,Language_Str("�ȶ���3","   ASD-3"));
    Put_Line_Str(Language_Pos(40, 50), 330,Language_Str("ȡ��","ESC"));
    Put_Line_Str(Language_Pos(150,160), 330,Language_Str("ȷ��","OK"));
   
    //create button list
    for(i=0;i<4;i++)
    {
    xy[i][0] = 20 + 130/2;
    xy[i][1] = 120 + 50*i + 20;
    }
    xy[4][0] = 20  + 90/2;
    xy[4][1] = 120 + 50*4 + 20;
    xy[5][0] = 130 + 90/2;
    xy[5][1] = 120 + 50*4 + 20;
    Create_ButtonList(xy,6); 
     
    //////display currentvalue
    DisplayLocation(asd_var);
}
//**********************************************************************
//5������:LNTSel_Interface
//��������:LNT ѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void LNTSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[6][2];
    
    //Create window
    Create_Window_Frame(0,80,240,380-80,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(20,85,Language_Str("���Զ�ѡ��","LNT Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<4;i++)
    Bar(20,120+50*i,150,160+50*i);
    
    Bar(    20,120+50*4, 20+90,160+50*4);
    Bar(110+20,120+50*4,130+90,160+50*4);
    //fill text
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Set_ASC_Lib(Arial16B_Font); 
    Put_Line_Str(35, 130,Language_Str("����-0","LNT-0"));
    Put_Line_Str(35, 180,Language_Str("����-1","LNT-1"));
    Put_Line_Str(35, 230,Language_Str("����-2","LNT-2"));
    Put_Line_Str(35, 280,Language_Str("����-3","LNT-3"));
    Put_Line_Str(Language_Pos(40, 50), 330,Language_Str("ȡ��","ESC"));
    Put_Line_Str(Language_Pos(150,160), 330,Language_Str("ȷ��","OK"));
   
    //create button list
    for(i=0;i<4;i++)
    {
    xy[i][0] = 20 + 130/2;
    xy[i][1] = 120 + 50*i + 20;
    }
    xy[4][0] = 20  + 90/2;
    xy[4][1] = 120 + 50*4 + 20;
    xy[5][0] = 130 + 90/2;
    xy[5][1] = 120 + 50*4 + 20;
    Create_ButtonList(xy,6);
    //////display currentvalue 
    DisplayLocation(lnt_var);
  
}
///////////////////////////////////////////////////////////////////////
//**********************************************************************
//6������:PRTSel_Interface
//��������:PRT ѡ�� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void PRTSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[6][2];
    
    //Create window
    Create_Window_Frame(0,80,240,380-80,3,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(20,85,Language_Str("��ӡ��ѡ��","Printer Select"));
     
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<4;i++)
    Bar(20,120+50*i,150,160+50*i);
    
    Bar(    20,120+50*4, 20+90,160+50*4);
    Bar(110+20,120+50*4,130+90,160+50*4);
    //fill text
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Set_ASC_Lib(Arial16B_Font); 
    Put_Line_Str(35,    130,Language_Str("��ӡģʽ0","PRINT-0"));
    Put_Line_Str(35,    180,Language_Str("��ӡģʽ1","PRINT-1"));
    Put_Line_Str(35,    230,Language_Str("��ӡģʽ2","PRINT-2"));
    Put_Line_Str(35,    280,Language_Str("��ӡģʽ3","PRINT-3"));
    Put_Line_Str(Language_Pos(40, 50), 330,Language_Str("ȡ��","ESC"));
    Put_Line_Str(Language_Pos(150,160), 330,Language_Str("ȷ��","OK"));
   
    //create button list
    for(i=0;i<4;i++)
    {
    xy[i][0] = 20 + 130/2;
    xy[i][1] = 120 + 50*i + 20;
    }
    xy[4][0] = 20  + 90/2;
    xy[4][1] = 120 + 50*4 + 20;
    xy[5][0] = 130 + 90/2;
    xy[5][1] = 120 + 50*4 + 20;
    Create_ButtonList(xy,6); 
    //////display currentvalue 
    DisplayLocation(prt_mod); 
}

//**********************************************************************
//7������:UnitSel_Key_Pro
//��������:��λѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 UnitSel_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_LNTMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1://ASD-0
  	           unit_var_tmp = UNIT_G;
  	           break;
    	case 2://ASD-1
    	       unit_var_tmp = UNIT_MG;
    	       break;
       	case 3://ASD-2
       	       unit_var_tmp = UNIT_LB;
       	       break;
   		case 4://ASD-3
   		       unit_var_tmp = UNIT_OZ;
   		       break;
   		case 5://ESC
   		      op_id =  MENU_OP_OPER; //�ص�������
   		       break;       
   		case 6://ok
   		      current_unit = unit_var_tmp;
   		      op_id =  MENU_OP_OPER; //�ص�������
   		      break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//8������:ASDSel_Key_Pro
//��������:ASD ѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 ASDSel_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_LNTMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1://ASD-0
  	           asd_var_tmp = 0;
  	           break;
    	case 2://ASD-1
    	       asd_var_tmp = 1;
    	       break;
       	case 3://ASD-2
       	       asd_var_tmp = 2;
       	       break;
   		case 4://ASD-3
   		       asd_var_tmp = 3;
   		       break;
   		case 5://ESC
   		      op_id =  MENU_OP_OPER; //�ص�������
   		       break;       
   		case 6://ok
   		      asd_var = asd_var_tmp;
   		      op_id =  MENU_OP_OPER; //�ص�������
   		      break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//9������:LNTSel_Key_Pro
//��������:LNT ѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 LNTSel_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_LNTMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1://lnt-0
  	           lnt_var_tmp = 0;
  	           break;
    	case 2://lnt-1
    	       lnt_var_tmp = 1;
    	       break;
       	case 3://lnt-2
       	       lnt_var_tmp = 2;
       	       break;
   		case 4://lnt-3
   		       lnt_var_tmp = 3;
   		       break;
   		case 5://ESC
   		      op_id =  MENU_OP_OPER; //�ص�������
   		       break;       
   		case 6://ok
   		      lnt_var = lnt_var_tmp;
   		      op_id =  MENU_OP_OPER; //�ص�������
   		      break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
////////////////////////////////////////////////////////////////////////
//**********************************************************************
//10������:PRTSel_Key_Pro
//��������:PRT ѡ�� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 PRTSel_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_LNTMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1:
  	           prt_mod_tmp = 0;
  	           break;
    	case 2://ASD-1
    	       prt_mod_tmp = 1;
    	       break;
       	case 3://ASD-2
       	       prt_mod_tmp = 2;
       	       break;
   		case 4://ASD-3
   		       prt_mod_tmp = 3;
   		       break;
   		case 5://ESC
   		      op_id =  MENU_OP_OPER; //�ص�������
   		       break;
   		case 6://ok
   		      prt_mod = prt_mod_tmp;
   		      op_id =  MENU_OP_OPER; //�ص�������
   		      break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//11������:UnitSel_Disp_Pro               
//��������:��λѡ���µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void UnitSel_Disp_Pro(void)
{
  static uint32 unit_var_display;
  if(unit_var_display != unit_var_tmp)
  {
   unit_var_display = unit_var_tmp;
   
   Clr_Bar(160,120,40,180,COL_DEEPGRAY);
   DisplayLocation(unit_var_display-1);
  }   
 }

//**********************************************************************
//12������:ASDSel_Disp_Pro
//��������:asd�����µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void ASDSel_Disp_Pro(void)
{
  static uint32 asd_var_display;
  if(asd_var_display != asd_var_tmp)
  {
   asd_var_display = asd_var_tmp;
   
   Clr_Bar(160,120,40,180,COL_DEEPGRAY);
   DisplayLocation(asd_var_display);
  }
 } 
//**********************************************************************
//13������:LNTSel_Disp_Pro               
//��������:LNT CFG �µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void LNTSel_Disp_Pro(void)
{
  static uint32 lnt_var_display;
 
  if(lnt_var_display != lnt_var_tmp)
 {
   lnt_var_display = lnt_var_tmp;
   
   Clr_Bar(160,120,40,180,COL_DEEPGRAY);
   DisplayLocation(lnt_var_display);
  }
}
//**********************************************************************
//14������:PRTSel_Disp_Pro               
//��������:PRT CFG �µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void PRTSel_Disp_Pro(void)
{
  static uint32 prt_var_display;
 
  if(prt_var_display != prt_mod_tmp)
 {
   prt_var_display = prt_mod_tmp;
   
   Clr_Bar(160,120,40,180,COL_DEEPGRAY);
   DisplayLocation(prt_var_display);
  }
}
/*///////////////////////////////////////////////////
/*15
/*       10��У׼���� �ж�
//////////////////////////////////////////////////*/
uint32  Action_CAL_10pointMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 60/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     Key_Status = KEY_DO_OVER; 
	     
	   return(Key_Code);
	}
//**********************************************************************
//16������:Cal_10point_Interface
//��������:10��У׼�µĽ����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void Cal_10point_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[5][2];
    
    //Create window
    Clr_Bar(0,70,SCR_XSIZE,410,COL_THINBLUE);   //TITLE
    //draw XY axia
    Set_Graphic_Color(COL_BLACK,COL_THINBLUE);
    ////////////////////////////////////////
    Ver_Line(70,475,38);
    Line(30, 90,38,70);       //�������ͷ
    Line(38, 70,46,90);
    /////////////////////////////////////// 
    Hor_Line(5,630,435);
    Line(620,435-8,630,  435);  //�������ͷ
    Line(630,  435,620,435+8);
    
    for(i=0;i<5;i++)
    Clr_Bar(540, 80+70*i,90, 60,COL_BLACK);
    
    for(i=0;i<5;i++)
    {xy[i][0] = 540+50;  xy[i][1] = 115+70*i;}
    //create button list
    Create_ButtonList(xy,5); 
    //fill text
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Set_ASC_Lib(Arial16B_Font); 
    Put_Line_Str(Language_Pos(560,550),  80+20,Language_Str("��ʼ",   " START"));
    Put_Line_Str(Language_Pos(550,540), 150+20,Language_Str("��һ��", "  LAST"));
    Put_Line_Str(Language_Pos(550,540), 220+20,Language_Str("��һ��", "  NEXT"));
    Put_Line_Str(Language_Pos(560,540), 290+20,Language_Str("����",   "  SAVE"));
    Put_Line_Str(Language_Pos(560,540), 360+20,Language_Str("�˳�",   "  EXIT"));
    
    ////// 
    /*Set_Chr_Color(COL_THINRED,COL_BLACK);
    if(TRUE == point10_cal_ok)
      {
      Put_Line_Str(550, 450,Language_Str("�����������ƻ���������","NEXT op will recover L_CALI data"));
      }
      else
      {
      Put_Line_Str(550, 450,Language_Str("ϵͳĿǰû������У׼","The system is not L_CALI")); 
      }*/
      
    Cal_Point_Count = 0;           
}
//**********************************************************************
//17������:Cal_10point_Key_Pro
//��������:10��У׼�µ� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 Cal_10point_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    static uint8  Line_CalData[16];
	       uint32 CheckSum;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_CAL_10pointMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1:                   //start
  	           Cal_Point_Count = 1;
  	           point10_cal_start = TRUE;
  	           stable_manual_break();
  	           break;
    	case 2://last
    	       /*if(Cal_Point_Count>1)  
    	       {Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPBLUE);
                Fill_Circle(38 + (Cal_Point_Count-1)*(550-38)/10,435-(Cal_Point_Count-1)*(435-100)/10,3); 
    	        Cal_Point_Count--;
    	        }*/
    	       break;
       	case 3://next
       	       /*
       	       if((Cal_Point_Num<11) && (Cal_Point_Num>0))
       	       {//record current data
       	        Set_Graphic_Color(COL_THINGREEN,COL_DEEPBLUE);
                Fill_Circle(38 + (Cal_Point_Num-1)*(550-38)/10,435-(Cal_Point_Num-1)*(435-100)/10,3); 
       	        // Cal_Buf_Tmp[Cal_Point_Num-1] = Get_Filter_Data();
       	        Cal_Buf_Tmp[Cal_Point_Num-1] = Cal_Point_Num*13 ;
       	       
       	        Cal_Point_Num++;
       	       }*/
       	       break;
       	case 4://save
       	       
       	       for(i=0;i<Cal_Point_Count-1;i++)
       	        Point10_Cal_Buf[i] = Cal_Buf_Tmp[i];
       	        //////////////////////////////////save to EEPROM
       	        CheckSum = 0;
       	       for(i=0;i<Cal_Point_Count-1;i++)  //����11�����У��ֵ
       	        CheckSum += Cal_Buf_Tmp[i];
       	        Cal_Buf_Tmp[Cal_Point_Count-1] = CheckSum;
       	       
       	       for(i=0;i<3;i++)  //ÿ��д16���ֽڣ�����4��������ݣ���Ҫ3��д��
       	       {
       	        for(j=0;j<4;j++)
       	        {
       	         Line_CalData[4*j+0] = Cal_Buf_Tmp[i*4+j]>>24;
       	         Line_CalData[4*j+1] = Cal_Buf_Tmp[i*4+j]>>16;
       	         Line_CalData[4*j+2] = Cal_Buf_Tmp[i*4+j]>>8;
       	         Line_CalData[4*j+3] = Cal_Buf_Tmp[i*4+j]&0xff;
       	        }
       	        
       	        Write_EEPROM(EEP_LINCAL_ADDR01+i*16, Line_CalData, 16); /* ����ʼ��ַ��ʼд��16������*/
			    for(j=0;j<800000;j++)	{;}	                 //delay
       	        
       	        } 
       	        /////////////////////////////////////////CHECK
       	        //////////////////////////////////////////////use current data
       	        point10_cal_start = FALSE;
       	        point10_cal_ok = TRUE;
       	        Get_Linecal_Var(Point10_Cal_Buf[10]);
       	        
       	        //////////////////////////////////////////////////////////////
       	        op_id =  MENU_OP_OPER; //�ص�������        
   		        break;
   		case 5://end
   		        op_id =  MENU_OP_OPER; //�ص�������
   		        break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
//**********************************************************************
//18������:Cal_10point_Disp_Pro               
//��������:10��У׼�µ���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Cal_10point_Disp_Pro(void)
{
  uint32 i,j;
  uint8  eeprom_tx_buf[8];
  
  static uint32 flash_time,flash_flag,go_0_flag;
  static uint32 Cal_Point_Tmp,last_cal_raw_data;
  flash_time++;
  if(flash_time%7 == 0)
  flash_flag++;
  if(flash_flag & 0x01)                  //flash 
  {
   Set_Graphic_Color(COL_THINBLUE,COL_THINBLUE);
   Set_Chr_Color(COL_THINBLUE,COL_THINBLUE);
   }
  else
   {
    Set_Graphic_Color(COL_DEEPRED,COL_THINBLUE);//x0,y0 {38,435} ; xMAX yMAX {550,100}
    Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);   //X length:660-38  Y length:435-110
    }
    
  Set_ASC_Lib(Arial16B_Font);
  if(Cal_Point_Count>0)
   {
     Fill_Circle(38 + (Cal_Point_Count-1)*(530-38)/10,435-(Cal_Point_Count-1)*(435-100)/10,3);
     if(1 == Cal_Point_Count)
       Put_Line_Str(170, 450,Language_Str("����ȷ�����","Checking 0_Point"));
     else
      {Clr_Bar(170,450,400,25,COL_THINBLUE);
       ////////////////////////////////////////////////////
       if((TRUE == go_0_flag)&&(FALSE == auto_zer0_valid))
        {
         
         Put_Line_Str(170,     450,Language_Str("��ȴ�����","Please Waiting for 0_Point"));
         } 
       else if(TRUE == auto_zer0_valid)
               {
                go_0_flag = FALSE; 
                Put_Line_Str(170,     450,Language_Str("�����","Please Loading"));
                Put_Data(170+72,  450,"%3d",(Cal_Point_Count-1)*10);
                Put_Line_Str(170+105, 450,Language_Str("% ����������","% Full-Load"));
                }
           else
             {
              Put_Line_Str(170,     450,Language_Str("�����","Please Loading"));
              Put_Data(170+72,  450,"%3d",(Cal_Point_Count-1)*10);
              Put_Line_Str(170+105, 450,Language_Str("% ����������","% Full-Load"));
             }        
       }
   }
  //////////////////////////////////////////////////////////
  if((Cal_Point_Count>0)&&(TRUE == stable_flag))
   {
    if(1 == Cal_Point_Count)
    {
     last_cal_raw_data = 0;  
     Cal_Buf_Tmp[Cal_Point_Count-1] = 0;
     
     clr_data = Get_new_data();  //����궨��λ
     zer0_data = clr_data; 
     eeprom_tx_buf[0] = (uint8)(clr_data>>24);
  	 eeprom_tx_buf[1] = (uint8)((clr_data>>16)&0xff);
  	 eeprom_tx_buf[2] = (uint8)((clr_data>>8)&0xff);
  	 eeprom_tx_buf[3] = (uint8)(clr_data&0xff); 
  	 
  	 eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	 eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	 eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	 eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
  	 Write_EEPROM(EEP_WEIZERO_ADDR, eeprom_tx_buf, 8);	 //��ͬ���ݼĴ���д��
     Cal_Point_Count++;
     go_0_flag = FALSE;
     stable_manual_break();
    }                                                                   //40000000/15/200000 = 13
    else if((Get_new_data()>zer0_data)&&(Get_new_data()-zer0_data)>(last_cal_raw_data+INNER_CODE_STEP10)) //��Լÿ���ֶ�����13������
        {
         last_cal_raw_data = Get_new_data() - zer0_data;    //net weigh code
         Cal_Buf_Tmp[Cal_Point_Count-1] = last_cal_raw_data;
         /*if(11 == Cal_Point_Count)
          {
           fill_data = Get_new_data();                  //˳����������
           weigh_coef1      = FULL_WEIGH / (last_cal_raw_data+0.01);
   	       point2_cal_ok    = TRUE;
          }*/
         Cal_Point_Count++;
         go_0_flag = TRUE;
         stable_manual_break();
        }
    }
  //////////////////////////////////////////////////////////
  if((Cal_Point_Tmp != Cal_Point_Count) && (Cal_Point_Count>0))
  { 
    Cal_Point_Tmp = Cal_Point_Count;  
    Set_Graphic_Color(COL_BLACK,COL_THINBLUE);
    Line(38,435,530,100);        //������
                                 //������������������
    for(i=0;i<Cal_Point_Count;i++)  
    Clr_Bar(40 + i * (530-38)/10,440- i * (435-100)/10,100,20,COL_THINBLUE);
   
    
    Set_Chr_Color(COL_BLACK,COL_THINBLUE);
    Set_ASC_Lib(Arial12B_Font);  //����˸������·���ʾ�õ�ı궨��ֵ
    for(i=0;i<Cal_Point_Count;i++)  
     {
     Set_Graphic_Color(COL_DEEPRED,COL_THINBLUE);
    // Fill_Circle(38 + (Cal_Point_Count-1)*(550-38)/10,435-(Cal_Point_Count-1)*(435-100)/10,3);
     Fill_Circle(38 + i * (530-38)/10,435-i*(435-100)/10,3);
     Put_Data(  40 + i * (530-38)/10,440- i * (435-100)/10,"%d",Cal_Buf_Tmp[i]);//�����Ѵ�����
     }
   }
 }
