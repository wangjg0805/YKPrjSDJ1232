//**********************************************************************
//�ļ���:screencal.c
//�����б�:
//1.��ָ��λ�û�ʮ��:     Draw_Point
//////////////////////////////////////////////
#include "disp.h"

/////////////////////////////////////////////////////////////////
//��ĳ����ָ����ɫ��һ��+��
//
///////////////////////////////////////////////////////////////
void  Draw_Point(uint16 x,uint16 y,uint16 col)
    {
        //Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
        Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);
	    Set_Graphic_Color(col,COL_BLACK);
        Hor_Line(x-5,x+5,y);
        Ver_Line(y-5,y+5,x);
     }
