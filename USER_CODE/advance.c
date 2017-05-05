//**********************************************************************
//�ļ���:advance.c
//**********************************************************************
#include "FontLib.h"
#include "disp.h"
//#include "graph.h"
#include "lcd2478.h"
//#include "lcd2478_cilib.h"

#include "key_scan.h"
#include "timer.h"
#include "global.h"
#include "menu.h"
#include "data_pro.h"
#include "i2c_com.h"
#include "speaker.h"
#include "eeprom.h"
#include "ad_acquire.h"
#include "type_def.h"
#include "port.h"
#include "stdio.h"
#include "data_pro.h"
#include "alarm_pro.h"
#include "sys_para.h"
#include "advance.h"

//ѡ����ʾ
static void Put_Vte_Filt_Const_Box(uint8 disp_mode,uint8 para,uint8 st);
static void Put_O2_Flash_Cyc_Box(uint8 disp_mode,uint8 para,uint8 st);
static void Put_Pl_Comp_Box(uint8 disp_mode,sint16 para,uint8 st);
static void Put_Paw_No_Raise_Box(uint8 disp_mode,sint32 para,uint8 st);
//�˵������ʼ������
static void Init_Advc_Para_Menu(sint32 op);
//���洦����
static sint32 Menu_Adv_Para_Op_Pro(uint8 key_c,sint32 op);
static sint32 Menu_Lg_Sel_Op_Pro(uint8 key_c,sint32 op);
//ѡ�����
static sint32 Ch_Butt_Pro(uint8 key_c);
static sint32 En_Butt_Pro(uint8 key_c);
static sint32 Vt_Filt_Const_Box_Pro(uint8 key_c);
static sint32 O2_Flash_Cyc_Box_Pro(uint8 key_c);
static sint32 Pl_Comp_Box_Pro(uint8 key_c);
static sint32 Paw_No_R_Box_Pro(uint8 key_c);

static uint8 ad_op_st;

//**********************************************************************
//����  ��:Advance_Manage
//��������:�߼�����ģʽ�µĲ˵��л�����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Advance_Manage(void)
{
    struct menu_str menu[2] = 
    {{MENU_AD_LG_SEL,Init_LG_Sel_Menu,        Menu_Lg_Sel_Op_Pro,        NULL,NULL},    //����ѡ�����
     {MENU_AD_PARA,     Init_Advc_Para_Menu,    Menu_Adv_Para_Op_Pro,    NULL,NULL}};//������
    sint32 cur_op,next_op;    //��ǰѡ������л�����ѡ��(bits[7:0]:ѡ���;bits[15:8]:�����;bits[23:16]:ѡ�����ʾ״̬)
    uint8 key_c;            //������
    uint8 lcd_ref_interval = 0;
    sint16 lcd_line=0;    //LCD ��ʱˢ�µ���ʼˢ������
        
    Set_Key_Sound_Switch(ON);    //�߼�����ģʽ�°�������
    cur_op =0xffffffff ;  
    next_op = MENU_AD_LG_SEL_CH_OP;
    ad_op_st |= OP_ST_SEL;
    while (1)
    {
        if ((cur_op^next_op)&0x0000ff00)//��ǰ�����Ľ����׼���л����Ľ��治һ��
        {//��ʼ������
            (*(menu[(next_op>>8)&0xff].menu_init))(next_op);
        }
        cur_op = next_op;        //"����/�˵�/ѡ��"֮����л�
        
        if (flag_10ms_ok)
        {
            flag_10ms_ok = FALSE;
            key_c = Get_Key_Code();        //����ɨ��
            next_op = (*(menu[(cur_op>>8)&0xff].op_pro))(key_c,cur_op);    //ѡ���
           // Check_Extend_Port();    //����ⲿ��չ��
            Check_Batt_Supply_Status();    //����ص�״̬
        }    
        
        if (flag_100ms_ok)
        {
            flag_100ms_ok = FALSE;
            //��ʾ����/�ֶ������� modify
            if(phase_vent_ctrl == PHASE_NO_VENT)
            {
                LED_Indicate_Off(EO2_LED_VENT);
                LED_Indicate_On(EO2_LED_BAG);
            }
            else
            {
                LED_Indicate_Off(EO2_LED_BAG);
                LED_Indicate_On(EO2_LED_VENT);
            }
            LED_Indicate_Show();
            Batt_Alarm_Pro();    
            lcd_ref_interval++;
            lcd_ref_interval &= 0x01;
            if (!lcd_ref_interval)
            {        
                LCD_Refresh_Area(0,lcd_line,319,lcd_line+23);//��ʱˢ��LCD
                lcd_line += 24;
                if (lcd_line > 216)    //��ͷˢ��
                {
                    lcd_line = 0; 
                }
            }                        
        }
        
        if (flag_500ms_ok)
        {//500msʱ�䵽
            flag_500ms_ok = FALSE;
            LED_Indicate_Cpl(EO2_LED_CPU_S); //CPU����ָʾ����˸Ƶ��1Hz
            LED_Indicate_Show();
        }        
    }
}

//**********************************************************************
//����  ��:Put_Vte_Filt_Const_Box
//��������:��ʾ"�������˲�����"�����
//��ڲ���:disp_mode--��ʾ��ʽ
//           para--��ʾ����
//           st--������״̬(ѡ��/����)
//���ڲ���:��
//**********************************************************************
static void Put_Vte_Filt_Const_Box(uint8 disp_mode,uint8 para,uint8 st)
{
      Set_Write_Mode(WRITE_OVERLAY);    //������ʾ
    /*if (st&OP_ST_SEL)    //ѡ��״̬
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);        
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
    }*/
    if (st&OP_ST_IN)    //�����������
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE); 
        Bar(156,33,313,49);   //�����ʾ
        Set_Chr_Color(COL_BLACK,COL_WHITE);
       Rectangle(157,34,312,48);
    }
    else
     {Set_Graphic_Color(COL_BLACK,COL_BLACK); 
      Bar(156,33,313,49);    //�����ʾ
      Set_Chr_Color(COL_WHITE,COL_BLACK);
     }
   
   if (!(disp_mode&DISP_ER))//������������������ʾ
    {
        Set_ASC_Lib(Arial10B_Font);    //�����ֿ�
        //Set_Chr_Color(COL_WHITE,COL_BLACK);
        /*if (st&OP_ST_SEL)//����ѡ��״̬
        {
            Set_Write_Mode(WRITE_EXOR);    //�����ʾ    
        }
        else
        {
            Set_Write_Mode(WRITE_OR);    //����ʾ            
        }*/
        Put_Numeral(162,35,para,1);    //��ʾ����ֵ        
    }
    if (disp_mode&DISP_RT)//ʵʱˢ��
    {
        LCD_Refresh_Area(156,33,313,49);    
    }    
}

//**********************************************************************
//����  ��:Put_O2_Flash_Cyc_Box
//��������:��ʾ"��Ũ��ˢ������"�����
//��ڲ���:disp_mode--��ʾ��ʽ
//           para--��ʾ����
//           st--������״̬(ѡ��/����)
//���ڲ���:��
//**********************************************************************
static void Put_O2_Flash_Cyc_Box(uint8 disp_mode,uint8 para,uint8 st)
{
      Set_Write_Mode(WRITE_OVERLAY);    //������ʾ
    /*if (st&OP_ST_SEL)    //ѡ��״̬
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);        
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
    }*/
    if (st&OP_ST_IN)    //�����������
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE); 
        Bar(156,51,313,67);    //�����ʾ
        Set_Chr_Color(COL_BLACK,COL_WHITE);
        Rectangle(157,52,312,66);
    }
    else
     {Set_Graphic_Color(COL_BLACK,COL_BLACK); 
       Bar(156,51,313,67);    //�����ʾ
      Set_Chr_Color(COL_WHITE,COL_BLACK);
     }
   
   
    if (!(disp_mode&DISP_ER))//������������������ʾ
    {
        Set_ASC_Lib(Arial10B_Font);    //�����ֿ�
        //Set_Chr_Color(COL_WHITE,COL_BLACK);
        /*if (st&OP_ST_SEL)//����ѡ��״̬
        {
            Set_Write_Mode(WRITE_EXOR);    //�����ʾ    
        }
        else
        {
            Set_Write_Mode(WRITE_OR);    //����ʾ            
        }*/
        Put_Numeral(162,53,para,10);    //��ʾ����ֵ        
    }
    if (disp_mode&DISP_RT)//ʵʱˢ��
    {
        LCD_Refresh_Area(156,51,313,67);    
    }    
}

//**********************************************************************
//����  ��:Put_Pl_Comp_Box
//��������:��ʾ"ѹ�����Ʋ���"�����
//��ڲ���:disp_mode--��ʾ��ʽ
//           para--��ʾ����
//           st--������״̬(ѡ��/����)
//���ڲ���:��
//**********************************************************************
static void Put_Pl_Comp_Box(uint8 disp_mode,sint16 para,uint8 st)
{
   Set_Write_Mode(WRITE_OVERLAY);    //������ʾ
    /*if (st&OP_ST_SEL)    //ѡ��״̬
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);        
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
    }*/
    if (st&OP_ST_IN)    //�����������
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE); 
        Bar(156,69,313,85);   //�����ʾ
        Set_Chr_Color(COL_BLACK,COL_WHITE);
        Rectangle(157,70,312,84);
    }
    else
     {Set_Graphic_Color(COL_BLACK,COL_BLACK); 
      Bar(156,69,313,85);    //�����ʾ
      Set_Chr_Color(COL_WHITE,COL_BLACK);
     }
  
    if (!(disp_mode&DISP_ER))//������������������ʾ
    {
        Set_ASC_Lib(Arial10B_Font);    //�����ֿ�
        //Set_Chr_Color(COL_WHITE,COL_BLACK);
        /*if (st&OP_ST_SEL)//����ѡ��״̬
        {
            Set_Write_Mode(WRITE_EXOR);    //�����ʾ    
        }
        else
        {
            Set_Write_Mode(WRITE_OR);    //����ʾ            
        }*/
        Put_Numeral(162,71,para,10);    //��ʾ����ֵ        
    }
    if (disp_mode&DISP_RT)//ʵʱˢ��
    {
        LCD_Refresh_Area(156,69,313,85);    
    }    
}

//**********************************************************************
//����  ��:Put_Paw_No_Raise_Box
//��������:��ʾ"��ѹ������"�����
//��ڲ���:disp_mode--��ʾ��ʽ
//           para--��ʾ����
//           st--������״̬(ѡ��/����)
//���ڲ���:��
//**********************************************************************
static void Put_Paw_No_Raise_Box(uint8 disp_mode,sint32 para,uint8 st)
{
    Set_Write_Mode(WRITE_OVERLAY);    //������ʾ
    /*if (st&OP_ST_SEL)    //ѡ��״̬
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);        
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
    }*/
    if (st&OP_ST_IN)    //�����������
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE); 
        Bar(156,87,313,103);    //�����ʾ
        Set_Chr_Color(COL_BLACK,COL_WHITE);
        Rectangle(157,88,312,102);
    }
    else
     {Set_Graphic_Color(COL_BLACK,COL_BLACK); 
      Bar(156,87,313,103);    //�����ʾ
      Set_Chr_Color(COL_WHITE,COL_BLACK);
     }
    if (!(disp_mode&DISP_ER))//������������������ʾ
    {
        Set_ASC_Lib(Arial10B_Font);    //�����ֿ�
        /*if (st&OP_ST_SEL)//����ѡ��״̬
        {
            Set_Write_Mode(WRITE_EXOR);    //�����ʾ    
        }
        else
        {
            Set_Write_Mode(WRITE_OR);    //����ʾ            
        }*/
        Put_Numeral(162,89,para,10);    //��ʾ����ֵ        
    }
    if (disp_mode&DISP_RT)//ʵʱˢ��
    {
        LCD_Refresh_Area(156,87,313,103);    
    }    
}


//**********************************************************************
//����  ��:Init_Advc_Para_Menu
//��������:��ʼ���߼���������
//��ڲ���:op--��ǰѡ��
//���ڲ���:��
//**********************************************************************
static void Init_Advc_Para_Menu(sint32 op)
{
     sint32 di;
     uint8 *str;
    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Clr_Screen(DISP_NOT_RT);    //�ñ���ɫ����
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Rectangle(5,5,314,234);    //�����
    Hor_Line(6,313,27);        //����
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Pattern_Bar(6,6,313,26,0xFF);
    //Bar(36,7,283,25);
        
    //Set_Write_Mode(WRITE_EXOR);//���ģʽ
    
    Set_HZ_Lib(&ST_11);        //�����ֿ�
    Set_ASC_Lib(Arial12_Font);
    Set_Chr_Color(COL_THINGREEN,COL_DEEPGREEN);
    str = Language_Str("�߼�����","Advanced Parameter");
    di = 159-Get_Str_Width(str)/2;    
    Put_Line_Str(di,9,str);
    
    //����ͷ
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    for (di=0; di<5; di++)
    {
        Hor_Line(6,313,32+di*18);
    }
    Ver_Line(33,103,155);
    //��ʾ����
    //Set_Write_Mode(WRITE_OR);
    Set_ASC_Lib(Arial10B_Font);
    Set_HZ_Lib(&ST_11);    
    Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);
    Put_Line_Str(7,34,Language_Str("�������˲�����","Vt Filt Const"));
    Put_Line_Str(7,52,Language_Str("��Ũ��ˢ������(S)","O2 Flash Cycle(S)"));
    Put_Line_Str(7,70,Language_Str("ѹ�����Ʋ���(cmH2O)","Plimit Comp(cmH2O)"));
    Put_Line_Str(7,88,Language_Str("��ѹ������(cmH2O)","Paw No Raise(cmH2O)"));

    //��ʾ����ֵ
    Put_Vte_Filt_Const_Box(DISP_NOT_ER,Get_Vt_Filter_Const(),OP_ST_NONE);
    Put_O2_Flash_Cyc_Box(DISP_NOT_ER,Get_O2_Flash_Cyc(),OP_ST_NONE);
    Put_Pl_Comp_Box(DISP_NOT_ER,Get_Pl_Comp(),OP_ST_NONE);
    Put_Paw_No_Raise_Box(DISP_NOT_ER,Get_Paw_No_Raise(),OP_ST_NONE);

    //��ʾ������
    LCD_Refresh_Area(0,0,319,239);    //ȫ��ˢ��    
}

//**********************************************************************
//����  ��:Menu_Adv_Para_Op_Pro
//��������:�߼������˵���ѡ���
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
static sint32 Menu_Adv_Para_Op_Pro(uint8 key_c,sint32 op)
{ 
    sint32 op_id;    
    struct op_str op_tab[MENU_AD_PARA_OP_NUM]=
    {{MENU_AD_VT_FILT_CONST_OP, Vt_Filt_Const_Box_Pro},//�������˲�����
    {MENU_AD_O2_FLASH_CYC_OP,    O2_Flash_Cyc_Box_Pro},//��Ũ��ˢ������
    {MENU_AD_PL_COMP_OP,        Pl_Comp_Box_Pro},    //ѹ�����Ʋ���
    {MENU_AD_PAW_NO_R_OP,        Paw_No_R_Box_Pro}};//��ѹ������

    static uint8 time; //��˸ʱ��

    if (!time)    //mod32(320ms������˸��ʾ)
    {
        switch (op)
        {
            case MENU_AD_VT_FILT_CONST_OP://�������˲�����    
                if (ad_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),ad_op_st);
                }
                else//ԭ����ʾ�����
                {
                    Put_Vte_Filt_Const_Box(DISP_RT|DISP_ER,Get_Vt_Filter_Const(),ad_op_st);
                }
                ad_op_st ^= OP_ST_ER;
                break;        
            case MENU_AD_O2_FLASH_CYC_OP://��Ũ��ˢ������    
                if (ad_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),ad_op_st);
                }
                else//ԭ����ʾ�����
                {
                    Put_O2_Flash_Cyc_Box(DISP_RT|DISP_ER,Get_O2_Flash_Cyc(),ad_op_st);
                }
                ad_op_st ^= OP_ST_ER;
                break;    
            case MENU_AD_PL_COMP_OP://ѹ�����Ʋ���    
                if (ad_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),ad_op_st);
                }
                else//ԭ����ʾ�����
                {
                    Put_Pl_Comp_Box(DISP_RT|DISP_ER,Get_Pl_Comp(),ad_op_st);
                }
                ad_op_st ^= OP_ST_ER;
                break;        
            case MENU_AD_PAW_NO_R_OP://��ѹ����������    
                if (ad_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),ad_op_st);
                }
                else//ԭ����ʾ�����
                {
                    Put_Paw_No_Raise_Box(DISP_RT|DISP_ER,Get_Paw_No_Raise(),ad_op_st);
                }
                ad_op_st ^= OP_ST_ER;
                break;                                    
            default:break;    
        }
    }
    time++;
    time &= 31;    
    op_id = (*((op_tab[op&0xff]).op_pro))(key_c);
    if ((op_id^op)&0x0000ffff)    //�л��˲˵������������˸��ʱ
    {
        time = 0;
    }
    return (op_id);
}

//**********************************************************************
//����  ��:Vt_Filt_Const_Box_Pro
//��������:�������˲��������봰�ڴ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID 
//**********************************************************************
static sint32 Vt_Filt_Const_Box_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//��������ֵ
                ad_op_st &= ~OP_ST_IN;        //ȥ������״̬
                ad_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
                ad_op_st |= OP_ST_SEL;        //ѡ��״̬
                if (ERROR == Save_Vt_Filt_C())
                {//����ʧ��
                    Key_No_Valid_Sound();
                }
                else
                {
                    Key_Valid_Sound();        //��������Ч��        
                }
                Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),OP_ST_SEL);    //��ʾ����
            }
            else//������״̬
            {//״̬�޸�Ϊ����״̬
                ad_op_st |= OP_ST_IN;    //��������״̬
                ad_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
                ad_op_st &= ~OP_ST_SEL;    //ȥ��ѡ��״̬
                Key_Valid_Sound();        //��������Ч��        
                Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),OP_ST_IN);    //��ʾ����
            }                
            return(MENU_AD_VT_FILT_CONST_OP);//��Ȼ����"�������˲�����"        
            break;
        case KEY_PRESSED|KEY_INC://������
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//�޸�����ֵ
                ad_op_st |= OP_ST_CHG;        //���޸ı��
                if (Inc_Dec_Vt_Filt_C(INC,1) == OK)
                {//�޸ĳɹ�
                    Key_Valid_Sound();        //��������Ч��                        
                }
                else
                {
                    Key_No_Valid_Sound();                    
                }
                Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),OP_ST_IN);    //��ʾ�޸ĺ��ֵ
                return(MENU_AD_VT_FILT_CONST_OP);//��Ȼ����"�������˲�����"
            }
            else//������״̬
            {
                Key_No_Valid_Sound();        //��������Ч��                    
                return(MENU_AD_VT_FILT_CONST_OP);//��Ȼ����"�������˲�����"
            }        
            break;    
        case KEY_PRESSED|KEY_DEC://���ټ�
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//�޸�����ֵ
                ad_op_st |= OP_ST_CHG;        //���޸ı��
                if (Inc_Dec_Vt_Filt_C(DEC,1) == OK)
                {//�޸ĳɹ�
                    Key_Valid_Sound();        //��������Ч��                    
                }
                else
                {
                    Key_No_Valid_Sound();                    
                }
                Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),OP_ST_IN);    //��ʾ�޸ĺ��ֵ
                return(MENU_AD_VT_FILT_CONST_OP);//��Ȼ����"�������˲�����"
            }
            else//������״̬
            {//�л�ѡ��
                Key_Valid_Sound();        //��������Ч��                
                Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),OP_ST_NONE);
                ad_op_st = OP_ST_SEL;
                return(MENU_AD_O2_FLASH_CYC_OP);//�л���"����Ũ��ˢ������"
            }                        
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(MENU_AD_VT_FILT_CONST_OP);//��Ȼ����"�������˲�����"
            break;            
    }        
}

//**********************************************************************
//����  ��:O2_Flash_Cyc_Box_Pro
//��������:����Ũ��ˢ���������봰�ڴ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID 
//**********************************************************************
static sint32 O2_Flash_Cyc_Box_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//��������ֵ
                ad_op_st &= ~OP_ST_IN;        //ȥ������״̬
                ad_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
                ad_op_st |= OP_ST_SEL;        //ѡ��״̬
                if (ERROR == Save_O2_Flash_Cyc())
                {//����ʧ��
                    Key_No_Valid_Sound();
                }
                else
                {
                    Key_Valid_Sound();        //��������Ч��                    
                }
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_SEL);    //��ʾ����
            }
            else//������״̬
            {//״̬�޸�Ϊ����״̬
                ad_op_st |= OP_ST_IN;    //��������״̬
                ad_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
                ad_op_st &= ~OP_ST_SEL;    //ȥ��ѡ��״̬
                Key_Valid_Sound();        //��������Ч��            
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_IN);    //��ʾ����
            }                
            return(MENU_AD_O2_FLASH_CYC_OP);//��Ȼ����"����Ũ��ˢ������"        
            break;
        case KEY_PRESSED|KEY_INC://������
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//�޸�����ֵ
                ad_op_st |= OP_ST_CHG;        //���޸ı��
                if (Inc_Dec_O2_Flash_Cyc(INC,1) == OK)
                {//�޸ĳɹ�
                    Key_Valid_Sound();        //��������Ч��                    
                }
                else
                {
                    Key_No_Valid_Sound();                    
                }
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_IN);    //��ʾ�޸ĺ��ֵ
                return(MENU_AD_O2_FLASH_CYC_OP);//��Ȼ����"����Ũ��ˢ������"
            }
            else//������״̬
            {
                Key_Valid_Sound();        //��������Ч��                    
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_NONE);    
                ad_op_st = OP_ST_SEL;                        
                return(MENU_AD_VT_FILT_CONST_OP);//�л���"�������˲�����"
            }        
            break;    
        case KEY_PRESSED|KEY_DEC://���ټ�
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//�޸�����ֵ
                ad_op_st |= OP_ST_CHG;        //���޸ı��
                if (Inc_Dec_O2_Flash_Cyc(DEC,1) == OK)
                {//�޸ĳɹ�
                    Key_Valid_Sound();        //��������Ч��                    
                }
                else
                {
                    Key_No_Valid_Sound();                
                }
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_IN);    //��ʾ�޸ĺ��ֵ
                return(MENU_AD_O2_FLASH_CYC_OP);//��Ȼ����"����Ũ��ˢ������"
            }
            else//������״̬
            {//�л�ѡ��
                Key_Valid_Sound();        //��������Ч��            
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_NONE);
                ad_op_st = OP_ST_SEL;
                return(MENU_AD_PL_COMP_OP);//�л���"ѹ�����Ʋ���"
            }                        
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(MENU_AD_O2_FLASH_CYC_OP);//��Ȼ����"����Ũ��ˢ������"
            break;            
    }        
}

//**********************************************************************
//����  ��:Pl_Comp_Box_Pro
//��������:ѹ�����Ʋ������봰�ڴ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID 
//**********************************************************************
static sint32 Pl_Comp_Box_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//��������ֵ
                ad_op_st &= ~OP_ST_IN;        //ȥ������״̬
                ad_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
                ad_op_st |= OP_ST_SEL;        //ѡ��״̬
                if (ERROR == Save_Pl_Comp())
                {//����ʧ��
                    Key_No_Valid_Sound();
                }
                else
                {
                    Key_Valid_Sound();        //��������Ч��                
                }
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_SEL);    //��ʾ����
            }
            else//������״̬
            {//״̬�޸�Ϊ����״̬
                ad_op_st |= OP_ST_IN;    //��������״̬
                ad_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
                ad_op_st &= ~OP_ST_SEL;    //ȥ��ѡ��״̬
                Key_Valid_Sound();        //��������Ч��            
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_IN);    //��ʾ����
            }                
            return(MENU_AD_PL_COMP_OP);//��Ȼ����"ѹ�����Ʋ���"        
            break;
        case KEY_PRESSED|KEY_INC://������
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//�޸�����ֵ
                ad_op_st |= OP_ST_CHG;        //���޸ı��
                if (Inc_Dec_Pl_Comp(INC,1) == OK)
                {//�޸ĳɹ�
                    Key_Valid_Sound();        //��������Ч��                    
                }
                else
                {
                    Key_No_Valid_Sound();                    
                }
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_IN);    //��ʾ�޸ĺ��ֵ
                return(MENU_AD_PL_COMP_OP);//��Ȼ����"ѹ�����Ʋ���"    
            }
            else//������״̬
            {
                Key_Valid_Sound();        //��������Ч��                
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_NONE);    
                ad_op_st = OP_ST_SEL;                        
                return(MENU_AD_O2_FLASH_CYC_OP);//�л���"��Ũ��ˢ������"
            }        
            break;    
        case KEY_PRESSED|KEY_DEC://���ټ�
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//�޸�����ֵ
                ad_op_st |= OP_ST_CHG;        //���޸ı��
                if (Inc_Dec_Pl_Comp(DEC,1) == OK)
                {//�޸ĳɹ�
                    Key_Valid_Sound();        //��������Ч��                    
                }
                else
                {
                    Key_No_Valid_Sound();                
                }
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_IN);    //��ʾ�޸ĺ��ֵ
                return(MENU_AD_PL_COMP_OP);//��Ȼ����"ѹ�����Ʋ���"
            }
            else//������״̬
            {//�л�ѡ��
                Key_Valid_Sound();        //��������Ч��                
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_NONE);
                ad_op_st = OP_ST_SEL;
                return(MENU_AD_PAW_NO_R_OP);//�л���"��ѹ������"
            }                        
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(MENU_AD_PL_COMP_OP);//��Ȼ����"ѹ�����Ʋ���"
            break;            
    }        
}

//**********************************************************************
//����  ��:Paw_No_R_Box_Pro
//��������:��ѹ���������봰�ڴ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID 
//**********************************************************************
static sint32 Paw_No_R_Box_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//��������ֵ
                ad_op_st &= ~OP_ST_IN;        //ȥ������״̬
                ad_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
                ad_op_st |= OP_ST_SEL;        //ѡ��״̬
                if (ERROR == Save_Paw_No_R())
                {//����ʧ��
                    Key_No_Valid_Sound();
                }
                else
                {
                    Key_Valid_Sound();        //��������Ч��                
                }
                Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),OP_ST_SEL);    //��ʾ����
            }
            else//������״̬
            {//״̬�޸�Ϊ����״̬
                ad_op_st |= OP_ST_IN;    //��������״̬
                ad_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
                ad_op_st &= ~OP_ST_SEL;    //ȥ��ѡ��״̬
                Key_Valid_Sound();        //��������Ч��            
                Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),OP_ST_IN);    //��ʾ����
            }                
            return(MENU_AD_PAW_NO_R_OP);//��Ȼ����"��ѹ������"        
            break;
        case KEY_PRESSED|KEY_INC://������
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//�޸�����ֵ
                ad_op_st |= OP_ST_CHG;        //���޸ı��
                if (Inc_Dec_Paw_No_R(INC,1) == OK)
                {//�޸ĳɹ�
                    Key_Valid_Sound();        //��������Ч��                        
                }
                else
                {
                    Key_No_Valid_Sound();                
                }
                Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),OP_ST_IN);    //��ʾ�޸ĺ��ֵ
                return(MENU_AD_PAW_NO_R_OP);//��Ȼ����"��ѹ������"        
            }
            else//������״̬
            {
                Key_Valid_Sound();        //��������Ч��                
                Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),OP_ST_NONE);    
                ad_op_st = OP_ST_SEL;                        
                return(MENU_AD_PL_COMP_OP);//�л���"ѹ�����Ʋ���"
            }        
            break;    
        case KEY_PRESSED|KEY_DEC://���ټ�
            if (ad_op_st&OP_ST_IN)//��������״̬
            {//�޸�����ֵ
                ad_op_st |= OP_ST_CHG;        //���޸ı��
                if (Inc_Dec_Paw_No_R(DEC,1) == OK)
                {//�޸ĳɹ�
                    Key_Valid_Sound();        //��������Ч��                
                }
                else
                {
                    Key_No_Valid_Sound();    
        
                }
                Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),OP_ST_IN);    //��ʾ�޸ĺ��ֵ
                return(MENU_AD_PAW_NO_R_OP);//��Ȼ����"��ѹ������"        
            }
            else//������״̬
            {//�л�ѡ��
                Key_No_Valid_Sound();        //�������� Ч��
                return(MENU_AD_PAW_NO_R_OP);//��Ȼ����"��ѹ������"        s
            }                        
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(MENU_AD_PAW_NO_R_OP);//��Ȼ����"��ѹ������"        
            break;            
    }        
}

//**********************************************************************
//����  ��:Menu_Lg_Sel_Op_Pro
//��������:�������ò˵���ѡ���
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
static sint32 Menu_Lg_Sel_Op_Pro(uint8 key_c,sint32 op)
{ 
    sint32 op_id;    
    struct op_str op_tab[MENU_AD_LG_OP_NUM]=
    {{MENU_AD_LG_SEL_CH_OP,    Ch_Butt_Pro},//"����"
    {MENU_AD_LG_SEL_EN_OP,    En_Butt_Pro}};//"English"
    static uint8 time; //��˸ʱ��

    if (!time)    //mod32(320ms������˸��ʾ)
    {
        switch (op)
        {
            case MENU_AD_LG_SEL_CH_OP://����    
                if (ad_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_CH_Butt_Lg_Sel(DISP_RT);
                }
                else//ԭ����ʾ�����
                {
                    Put_CH_Butt_Lg_Sel(DISP_RT|DISP_ER);
                }
                ad_op_st ^= OP_ST_ER;
                break;        
            case MENU_AD_LG_SEL_EN_OP://English    
                if (ad_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_EN_Butt_Lg_Sel(DISP_RT);
                }
                else//ԭ����ʾ�����
                {
                    Put_EN_Butt_Lg_Sel(DISP_RT|DISP_ER);
                }
                ad_op_st ^= OP_ST_ER;
                break;                                    
            default:break;    
        }
    }
    time++;
    time &= 31;    
    op_id = (*((op_tab[op&0xff]).op_pro))(key_c);
    if ((op_id^op)&0x0000ffff)    //�л��˲˵������������˸��ʱ
    {
        time = 0;
    }
    return (op_id);
}

//**********************************************************************
//����  ��:Ch_Butt_Pro
//��������:"����"��ť�Ĵ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID 
//**********************************************************************
static sint32 Ch_Butt_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            ad_op_st |= OP_ST_SEL;        //ѡ��״̬
            Set_Language_Type(CHINESE);    //������������
            Key_Valid_Sound();        //��������Ч��        
            return(MENU_AD_VT_FILT_CONST_OP);//�л���"�������˲�����"        
            break;
        case KEY_PRESSED|KEY_DEC://���ټ�
            Key_Valid_Sound();        //��������Ч��                
            if (ad_op_st&OP_ST_ER)    //�ָ���ʾ
            {
                Put_CH_Butt_Lg_Sel(DISP_RT);
            }    
            ad_op_st = OP_ST_SEL;
            return(MENU_AD_LG_SEL_EN_OP);//�л���"English"                    
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(MENU_AD_LG_SEL_CH_OP);//��Ȼ����"����"
            break;            
    }        
}

//**********************************************************************
//����  ��:En_Butt_Pro
//��������:"English"��ť�Ĵ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID 
//**********************************************************************
static sint32 En_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            ad_op_st |= OP_ST_SEL;        //ѡ��״̬
            Key_Valid_Sound();        //��������Ч��
            Set_Language_Type(ENGLISH);    //������������        
            return(MENU_AD_VT_FILT_CONST_OP);//�л���"�������˲�����"        
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();        //��������Ч��                
            if (ad_op_st&OP_ST_ER)    //�ָ���ʾ
            {
                Put_EN_Butt_Lg_Sel(DISP_RT);
            }
            ad_op_st = OP_ST_SEL;    
            return(MENU_AD_LG_SEL_CH_OP);//�л���"����"                    
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(MENU_AD_LG_SEL_EN_OP);//��Ȼ����"English"
            break;            
    }        
}

