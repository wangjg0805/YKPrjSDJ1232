//**********************************************************************
//�ļ���:menu.c
//**********************************************************************
#include "FontLib.h"
#include "disp.h"
#include "lcd2478.h"
//#include "lcd2478_cilib.h"

#include "graph.h"
#include "menu.h"
#include "type_def.h"
#include "stdio.h"
#include "global.h"

//**********************************************************************
//����  ��:Put_CH_Butt_Lg_Sel
//��������:������ѡ��Ի�������ʾ"����"��ť
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
void Put_CH_Butt_Lg_Sel(uint8 disp_mode)
{
    uint16 di;
    //Set_Graphic_Color(COL_BLACK,COL_WHITE);  
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);   
    Set_Write_Mode(WRITE_OVERLAY);    //������ʾ
    Bar(99,129,153,147);
    
    if (!(disp_mode&DISP_ER))//����������ʾ����
    {
       // Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_Chr_Color(COL_THINCYAN,COL_DEEPGREEN); 
        di = Get_Str_Width("����")/2;
        Put_Line_Str(126-di,131,"����");
    }    
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(99,129,153,147);
    }
}

//**********************************************************************
//����  ��:Put_EN_Butt_Lg_Sel
//��������:������ѡ��Ի�������ʾ"English"��ť
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
void Put_EN_Butt_Lg_Sel(uint8 disp_mode)
{
    uint16 di;
    //Set_Graphic_Color(COL_BLACK,COL_WHITE);    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN); 
    Set_Write_Mode(WRITE_OVERLAY);    //������ʾ
    Bar(170,129,224,147);
    if (!(disp_mode&DISP_ER))//����������ʾ����
    {
        Set_ASC_Lib(Arial10B_Font);
        //Set_Write_Mode(WRITE_EXOR);
        Set_Chr_Color(COL_THINCYAN,COL_DEEPGREEN); 
        di = Get_Str_Width("English")/2;
        Put_Line_Str(197-di,131,"English");
    }    
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(170,129,224,147);
    }
}

//**********************************************************************
//����  ��:Init_LG_Sel_Menu
//��������:��ʼ������ѡ�����
//��ڲ���:op--��ǰѡ��
//���ڲ���:��
//**********************************************************************
void Init_LG_Sel_Menu(sint32 op)
{

    //Set_Graphic_Color(COL_BLACK,COL_WHITE);
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Clr_Screen(DISP_NOT_RT);    //�ñ���ɫ����
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Put_Image(78,65,Language_Sel_Box,WRITE_OVERLAY);
    
    Put_CH_Butt_Lg_Sel(DISP_NOT_RT);//"����"
    Put_EN_Butt_Lg_Sel(DISP_NOT_RT);//"English"
    
    LCD_Refresh_Area(0,0,319,239);    //ȫ��ˢ��    
}