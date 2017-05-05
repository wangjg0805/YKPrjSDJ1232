//**********************************************************************
//�ļ���:disp_menu.c
//**********************************************************************

#include "global.h"
#include "lcd2478_graph.h"
//#include "disp.h"
#include "FontLib.h"
#include "graph.h"
#include "timer.h"

//**********************************************************************
//����  ��:Disp_VT_Meas_Main
//��������:������������ʾVT����ֵ�Ĳ˵�
//��ڲ���:vt_m--����������ֵ
//			vt_units--����������ֵ�ĵ�λ(10--0.1ml;100--0.01ml)
//			vt_alarm_h--��������������(ml)
//			vt_alarm_l--��������������(ml)
//���ڲ���:��
//**********************************************************************
void Disp_VT_Meas_Main(int vt_m,int vt_units,int vt_alarm_h,int vt_alarm_l)
{
	unsigned short ch_x,en_x;

	//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//Ĭ��Ϊ����ģʽ
	
	Clr_Bar (0,117,63,73,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(0,62,125,0xaa);
	Hor_Pattern_Line(0,62,189,0xaa);
	Ver_Pattern_Line(125,189,62,0xaa);
	ch_x = 31-Get_Line_Width("������")/2;
	en_x = 31-Get_Line_Width("Vte")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("������","Vte"));
	
	Set_ASC_Lib(Arial8_Font);
	en_x = 31-Get_Line_Width("mL")/2;
	Put_Line_Str(en_x,177,"mL");
	Put_Numeral(36,135,vt_alarm_h,1);//��������
	Put_Numeral(36,169,vt_alarm_l,1);//��������
	
	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//����������ֵ
	if ( (vt_m>vt_alarm_h+10)||(vt_m+10<vt_alarm_l) )
	{
		en_x = 31-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 31-Get_Num_Width(vt_m,vt_units)/2;
		Put_Numeral(en_x,145,vt_m,vt_units);
	}
	Set_Write_Mode(WRITE_OVERLAY);	//�ָ�ΪĬ��ֵ			
}
//**********************************************************************
//����  ��:Disp_VT_Meas_Main_Titl
//��������:������������ʾVT����ֵ�˵��ı���͵�λ
//��ڲ���:disp_mode(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_VT_Meas_Main_Titl(char disp_mode)
{
	unsigned short en_x,ch_x;
	
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar (0,117,62,189);
	Hor_Pattern_Line(0,62,125,0xaa);
	Hor_Pattern_Line(0,62,189,0xaa);
	Ver_Pattern_Line(125,189,62,0xaa);
	Set_ASC_Lib(Arial12B_Font);	//�����ֿ�
	Set_HZ_Lib(&ST_11);
	ch_x = 31-Get_Line_Width("������")/2;
	en_x = 31-Get_Line_Width("Vte")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("������","Vte"));
	if (disp_mode&0x80)//��ʱˢ��
	{
		LCD_Refresh_Area(0,117,62,189);
	}	
}

//**********************************************************************
//����  ��:Disp_Vt_AlmH_Main
//��������:�����˵�����ʾ����������������
//��ڲ���: vt_alarm_h--��������������(ml)
//			vt_alarm_l--��������������(ml)
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Vt_AlmH_Main(int vt_alarm_h,int vt_alarm_l,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar(36,135,59,145);				//���������
	Bar(36,169,59,178);				//���������
	Set_ASC_Lib(Arial8_Font);
	Set_Write_Mode(WRITE_OR);
	Put_Numeral(36,135,vt_alarm_h,1);//��������
	Put_Numeral(36,169,vt_alarm_l,1);//��������	
	if (disp_mode&0x80)//��ʱˢ��
	{
		LCD_Refresh_Area(36,135,59,145);
		LCD_Refresh_Area(36,169,59,178);
	}
}

//**********************************************************************
//����  ��:Disp_Vt_MeasVl_Main
//��������:�����˵�����ʾ����������ֵ
//��ڲ���:vt_m--����������ֵ
//			vt_units--����������ֵ�ĵ�λ(10--0.1ml;100--0.01ml)
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Vt_MeasVl_Main(int vt_m,int vt_units,int disp_mode)
{
	unsigned short en_x;
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar(0,146,61,168);			//��ֵ��
	
	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//����������ֵ
	if ( (vt_m>2000)||(vt_m<0) )
	{
		en_x = 31-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 31-Get_Num_Width(vt_m,vt_units)/2;
		Put_Numeral(en_x,145,vt_m,vt_units);
	}
	if (disp_mode&0x80)//��ʱˢ��
	{
		LCD_Refresh_Area(0,146,61,168);
	}		

}

//**********************************************************************
//����  ��:Disp_Fr_Meas_Main
//��������:������������ʾ����Ƶ�ʲ���ֵ�Ĳ˵�
//��ڲ���:fr--����Ƶ��(��λ:��/min)
//���ڲ���:��
//**********************************************************************
void Disp_Fr_Meas_Main(int fr)
{
	unsigned short ch_x,en_x;

	//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//Ĭ��Ϊ����ģʽ
	
	Clr_Bar (62,117,65,73,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(62,126,125,0xaa);	//���߱߿�
	Hor_Pattern_Line(62,126,189,0xaa);
	Ver_Pattern_Line(125,189,62,0xaa);
	Ver_Pattern_Line(125,189,126,0xaa);
	ch_x = 94-Get_Line_Width("����Ƶ��")/2;
	en_x = 94-Get_Line_Width("Fr")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("����Ƶ��","Fr"));
	
	Set_ASC_Lib(Arial8_Font);
	en_x = 94-Get_Line_Width("/min")/2;
	Put_Line_Str(en_x,177,"/min");

	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//����Ƶ�ʲ���ֵ
	if (fr>200)
	{
		en_x = 94-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 94-Get_Num_Width(fr,1)/2;
		Put_Numeral(en_x,145,fr,1);
	}
	Set_Write_Mode(WRITE_OVERLAY);	//�ָ�ΪĬ��ֵ			
}

//**********************************************************************
//����  ��:Disp_Fr_Meas_Main_Title
//��������:������������ʾ����Ƶ�ʲ���ֵ�˵��ı���Ϳ��
//��ڲ���:disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Fr_Meas_Main_Title(char disp_mode)
{
	unsigned short ch_x,en_x;

	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar (62,117,126,189);
	Hor_Pattern_Line(62,126,125,0xaa);	//���߱߿�
	Hor_Pattern_Line(62,126,189,0xaa);
	Ver_Pattern_Line(125,189,62,0xaa);
	Ver_Pattern_Line(125,189,126,0xaa);
		
	Set_ASC_Lib(Arial12B_Font);	//�����ֿ�
	Set_HZ_Lib(&ST_11);	
	ch_x = 94-Get_Line_Width("����Ƶ��")/2;
	en_x = 94-Get_Line_Width("Fr")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("����Ƶ��","Fr"));	
	Set_ASC_Lib(Arial8_Font);
	en_x = 94-Get_Line_Width("/min")/2;
	Put_Line_Str(en_x,177,"/min");	
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(62,117,126,189);
	}
}

//**********************************************************************
//����  ��:Disp_Fr_MeasVl_Main
//��������:�����˵�����ʾ����Ƶ�ʵĲ���ֵ
//��ڲ���:fr--����Ƶ��(��λ:��/min)
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Fr_MeasVl_Main(int fr,char disp_mode)
{
	unsigned short en_x;
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar (63,146,125,168);	
	
	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//����Ƶ�ʲ���ֵ
	if (fr>200)
	{
		en_x = 94-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 94-Get_Num_Width(fr,1)/2;
		Put_Numeral(en_x,145,fr,1);
	}
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(63,146,125,168);
	}	
}

//**********************************************************************
//����  ��:Disp_Insp_Flx_Meas_Main
//��������:������������ʾ����ͨ������ֵ�Ĳ˵�
//��ڲ���: fl_m--����ͨ������ֵ
//			fl_units--����ͨ������ֵ�ĵ�λ(10--0.1L/min;100--0.01L/min)
//			fl_alarm_h--����ͨ����������(0.1L/min)
//			fl_alarm_l--����ͨ����������(0.1L/min)
//���ڲ���:��
//**********************************************************************
void Disp_Insp_Flx_Meas_Main(int fl_m,int fl_units,int fl_alarm_h,int fl_alarm_l)
{
	unsigned short ch_x,en_x;

	//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//Ĭ��Ϊ����ģʽ
	
	Clr_Bar (126,117,66,73,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(126,191,125,0xaa);
	Hor_Pattern_Line(126,191,189,0xaa);
	Ver_Pattern_Line(125,189,126,0xaa);
	Ver_Pattern_Line(125,189,191,0xaa);
	ch_x = 159-Get_Line_Width("����ͨ��")/2;
	en_x = 159-Get_Line_Width("Ve")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("����ͨ��","Ve"));
	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 159-Get_Line_Width("L/min")/2;
	Put_Line_Str(en_x,177,"L/min");
	Put_Numeral(165,135,fl_alarm_h,10);//��������
	Put_Numeral(165,169,fl_alarm_l,10);//��������

	Set_ASC_Lib(Arial20B_Font);			//����ͨ������ֵ
	if ( (fl_m>fl_alarm_h+10)||(fl_m+10<fl_alarm_l) )
	{
		en_x = 159-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 159-Get_Num_Width(fl_m,fl_units)/2;
		Put_Numeral(en_x,145,fl_m,fl_units);
	}
	Set_Write_Mode(WRITE_OVERLAY);	//�ָ�ΪĬ��ֵ			
}

//**********************************************************************
//����  ��:Disp_Insp_Flx_Meas_Main_Title
//��������:������������ʾ����ͨ���ı���
//��ڲ���:disp_mode--ˢ�·�ʽ
//���ڲ���:��
//**********************************************************************
void Disp_Insp_Flx_Meas_Main_Title(char disp_mode)
{
	unsigned short ch_x,en_x;

	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar (126,117,191,189);
	Hor_Pattern_Line(126,191,125,0xaa);
	Hor_Pattern_Line(126,191,189,0xaa);
	Ver_Pattern_Line(125,189,126,0xaa);
	Ver_Pattern_Line(125,189,191,0xaa);
		
	Set_ASC_Lib(Arial12B_Font);
	Set_HZ_Lib(&ST_11);	
	ch_x = 159-Get_Line_Width("����ͨ��")/2;
	en_x = 159-Get_Line_Width("Ve")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("����ͨ��","Ve"));	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 159-Get_Line_Width("L/min")/2;
	Put_Line_Str(en_x,177,"L/min");	
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(126,117,191,189);
	}
}

//**********************************************************************
//����  ��:Disp_Insp_Flx_Alm_Main
//��������:������������ʾ����ͨ���ı���������
//��ڲ���:fl_alarm_h--����ͨ����������(0.1L/min)
//			fl_alarm_l--����ͨ����������(0.1L/min)
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Insp_Flx_Alm_Main(int fl_alarm_h,int fl_alarm_l,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar (163,135,190,145);			//�����������
	Bar (163,169,190,178);			//���������
	Set_ASC_Lib(Arial8_Font);
	Set_Write_Mode(WRITE_OR);
	Put_Numeral(165,135,fl_alarm_h,10);//��������
	Put_Numeral(165,169,fl_alarm_l,10);//��������	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(163,135,190,145);
		LCD_Refresh_Area(163,169,190,178);
	}	
}

//**********************************************************************
//����  ��:Disp_Insp_Flx_Vl_Main
//��������:�����˵�����ʾ����ͨ���Ĳ���ֵ
//��ڲ���:fl_m--����ͨ������ֵ
//			fl_units--����ͨ������ֵ�ĵ�λ(10--0.1L/min;100--0.01L/min)
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Insp_Flx_Vl_Main(int fl_m,int fl_units,char disp_mode)
{
	unsigned short en_x;
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar (127,146,190,168);
	
	Set_ASC_Lib(Arial20B_Font);			//����ͨ������ֵ
	Set_Write_Mode(WRITE_OR);
	if ( (fl_m>1000)||(fl_m<0) )
	{
		en_x = 159-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 159-Get_Num_Width(fl_m,fl_units)/2;
		Put_Numeral(en_x,145,fl_m,fl_units);
	}
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(127,146,190,168);	
	}			
}

//**********************************************************************
//����  ��:Disp_Paw_Max_Meas_Main
//��������:������������ʾ���ѹ������ֵ�Ĳ˵�
//��ڲ���: paw_max--���ѹ������ֵ
//			paw_units--���ѹ������ֵ�ĵ�λ(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)
//���ڲ���:��
//**********************************************************************
void Disp_Paw_Max_Meas_Main(int paw_max,int paw_units)
{
	unsigned short ch_x,en_x;

	//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//Ĭ��Ϊ����ģʽ
	
	Clr_Bar (191,117,66,73,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(191,256,125,0xaa);
	Hor_Pattern_Line(191,256,189,0xaa);
	Ver_Pattern_Line(125,189,191,0xaa);
	Ver_Pattern_Line(125,189,256,0xaa);
	ch_x = 224-Get_Line_Width("���ѹ��")/2;	//��ʾ����
	en_x = 224-Get_Line_Width("Pmax")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("���ѹ��","Pmax"));
	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 224-Get_Line_Width("cmH2O")/2;
	Put_Line_Str(en_x,177,"cmH2O");

	Set_ASC_Lib(Arial20B_Font);			//���ѹ������ֵ
	if (paw_max >= 200)
	{
		en_x = 224-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 224-Get_Num_Width(paw_max,paw_units)/2;
		Put_Numeral(en_x,145,paw_max,paw_units);
	}
	Set_Write_Mode(WRITE_OVERLAY);	//�ָ�ΪĬ��ֵ			
}

//**********************************************************************
//����  ��:Disp_Paw_Max_Meas_Main_Title
//��������:�����˵�����ʾ���ѹ������ֵ�˵��ı���
//��ڲ���:disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Paw_Max_Meas_Main_Title(char disp_mode)
{
	unsigned short ch_x,en_x;

	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
	Bar (191,117,256,189);
	Hor_Pattern_Line(191,256,125,0xaa);
	Hor_Pattern_Line(191,256,189,0xaa);
	Ver_Pattern_Line(125,189,191,0xaa);
	Ver_Pattern_Line(125,189,256,0xaa);	
	Set_ASC_Lib(Arial12B_Font);//�����ֿ�
	Set_HZ_Lib(&ST_11);	
	ch_x = 224-Get_Line_Width("���ѹ��")/2;	//��ʾ����
	en_x = 224-Get_Line_Width("Pmax")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("���ѹ��","Pmax"));
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 224-Get_Line_Width("cmH2O")/2;
	Put_Line_Str(en_x,177,"cmH2O");	
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(191,117,256,189);
	}
}

//**********************************************************************
//����  ��:Disp_Paw_Max_Meas_Vl_Main
//��������:�����˵�����ʾ���ѹ���Ĳ���ֵ
//��ڲ���:paw_max--���ѹ������ֵ
//			paw_units--���ѹ������ֵ�ĵ�λ(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Paw_Max_Meas_Vl_Main(int paw_max,int paw_units,char disp_mode)
{
	unsigned short en_x;
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
	Bar (192,146,255,168);
	
	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//���ѹ������ֵ
	if (paw_max >= 200)
	{
		en_x = 224-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 224-Get_Num_Width(paw_max,paw_units)/2;
		Put_Numeral(en_x,145,paw_max,paw_units);
	}
	
	if (disp_mode&0x80)//��ʱˢ��
	{
		LCD_Refresh_Area(192,146,255,168);
	}		
}

//**********************************************************************
//����  ��:Disp_O2_Meas_Main
//��������:������������ʾ����Ũ�Ȳ���ֵ�Ĳ˵�
//��ڲ���: o2_m--����Ũ�Ȳ���ֵ
//			o2_units--����Ũ�Ȳ���ֵ�ĵ�λ(10--0.1%;100--0.01%)
//			o2_alarm_h--����Ũ�ȱ�������(1%)
//			o2_alarm_l--����Ũ�ȱ�������(1%)
//���ڲ���:��
//**********************************************************************
void Disp_O2_Meas_Main(int o2_m,int o2_units,int o2_alarm_h,int o2_alarm_l)
{
	unsigned short ch_x,en_x;

	//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//Ĭ��Ϊ����ģʽ
	
	Clr_Bar (256,117,64,73,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(256,319,125,0xaa);
	Hor_Pattern_Line(256,319,189,0xaa);
	Ver_Pattern_Line(125,189,256,0xaa);
	ch_x = 288-Get_Line_Width("����Ũ��")/2;
	en_x = 288-Get_Line_Width("O2")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("����Ũ��","O2"));
	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 288-Get_Line_Width("%")/2;
	Put_Line_Str(en_x,177,"%");
	Put_Numeral(297,135,o2_alarm_h,1);//��������
	Put_Numeral(297,169,o2_alarm_l,1);//��������

	Set_ASC_Lib(Arial20B_Font);			//����Ũ�Ȳ���ֵ
	if ( (o2_m>o2_alarm_h+10)||(o2_m+10<o2_alarm_l) )
	{
		en_x = 288-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 288-Get_Num_Width(o2_m,o2_units)/2;
		Put_Numeral(en_x,145,o2_m,o2_units);
	}
	Set_Write_Mode(WRITE_OVERLAY);	//�ָ�ΪĬ��ֵ			
}

//**********************************************************************
//����  ��:Disp_O2_Meas_Main_Title
//��������:������������ʾ����Ũ�Ȳ���ֵ�˵��ı���
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
void Disp_O2_Meas_Main_Title(char disp_mode)
{
	unsigned short ch_x,en_x;

	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar (256,117,319,189);
	Hor_Pattern_Line(256,319,125,0xaa);
	Hor_Pattern_Line(256,319,189,0xaa);
	Ver_Pattern_Line(125,189,256,0xaa);
		
	Set_ASC_Lib(Arial12B_Font);//�����ֿ�
	Set_HZ_Lib(&ST_11);
	ch_x = 288-Get_Line_Width("����Ũ��")/2;
	en_x = 288-Get_Line_Width("O2")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("����Ũ��","O2"));	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 288-Get_Line_Width("%")/2;
	Put_Line_Str(en_x,177,"%");	
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(256,117,319,189);
	}
}

//**********************************************************************
//����  ��:Disp_O2_Meas_Alm_Main
//��������:�����˵�����ʾ��������������
//��ڲ���:o2_alarm_h--����Ũ�ȱ�������(1%)
//			o2_alarm_l--����Ũ�ȱ�������(1%)
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_O2_Meas_Alm_Main(int o2_alarm_h,int o2_alarm_l,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar (297,135,319,145);	
	Bar (297,169,319,178);

	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);	
	Put_Numeral(297,135,o2_alarm_h,1);//��������
	Put_Numeral(297,169,o2_alarm_l,1);//��������	
	
	if (disp_mode&0x80)//��ʱˢ��
	{
		LCD_Refresh_Area(297,135,319,145);
		LCD_Refresh_Area(297,169,319,178);
	}	
}

//**********************************************************************
//����  ��:Disp_O2_Meas_Vl_Main
//��������:������������ʾ����Ũ�ȵĲ���ֵ
//��ڲ���:o2_m--����Ũ�Ȳ���ֵ
//			o2_units--����Ũ�Ȳ���ֵ�ĵ�λ(10--0.1%;100--0.01%)
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_O2_Meas_Vl_Main(int o2_m,int o2_units,char disp_mode)
{
	unsigned short en_x;
	
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar (257,146,319,168);
	
	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//����Ũ�Ȳ���ֵ
	if ( (o2_m>100*o2_units)||(o2_m<0) )
	{
		en_x = 288-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 288-Get_Num_Width(o2_m,o2_units)/2;
		Put_Numeral(en_x,145,o2_m,o2_units);
	}
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(257,146,319,168);
	}			
}

//**********************************************************************
//����  ��:Disp_Pavr_Meas_Main
//��������:����������ʾƽ��ѹ��
//��ڲ���:pavr--ѹ��ƽ��ֵ
//		   pavr_units--ѹ����λ(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)	
//���ڲ���:��
//**********************************************************************
void Disp_Pavr_Meas_Main(int pavr,int pavr_units)
{
	unsigned short x;

	//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//Ĭ��Ϊ����ģʽ
	
	Clr_Bar (191,96,129,19,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(191,319,96,0xaa);
	Hor_Pattern_Line(191,319,114,0xaa);
	Ver_Pattern_Line(96,114,191,0xaa);
	
	Set_Write_Mode(WRITE_OR);	
	x = Put_Line_Str(193,98,Language_Str("ƽ��ѹ��","Pavr"));
	
	Set_ASC_Lib(Arial12B_Font);
	if (pavr >= 100)
	{
		x = Put_Line_Str(x+3,98,"--");
	}
	else
	{
		x = Put_Numeral(x+3,98,pavr,pavr_units);
	}
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(x+3,100,"cmH2O");	
	Set_Write_Mode(WRITE_OVERLAY);	//�ָ�ΪĬ��ֵ		
}

//**********************************************************************
//����  ��:Disp_Pavr_Meas_Main_Title
//��������:������������ʾƽ��ѹ������ֵ�Ĳ˵�����
//��ڲ���:disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Pavr_Meas_Main_Title(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Hor_Pattern_Line(191,319,96,0xaa);
	Hor_Pattern_Line(191,319,114,0xaa);
	Ver_Pattern_Line(96,114,191,0xaa);	
	Bar(192,97,253,113);
	Bar(283,97,319,113);
		
	Set_HZ_Lib(&ST_11);
	Set_ASC_Lib(Arial12B_Font);
	Set_Write_Mode(WRITE_OR);	
	Put_Line_Str(193,98,Language_Str("ƽ��ѹ��","Pavr"));
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(283,100,"cmH2O");
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(191,96,253,114);
		LCD_Refresh_Area(283,96,319,114);
	}		
}

//**********************************************************************
//����  ��:Disp_Pavr_vl_Meas_Main
//��������:������������ʾƽ��ѹ������ֵ
//��ڲ���:pavr--ѹ��ƽ��ֵ
//		   pavr_units--ѹ����λ(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��	
//**********************************************************************
void Disp_Pavr_vl_Meas_Main(int pavr,int pavr_units,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar(254,97,282,113);
	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial12B_Font);
	if (pavr >= 1000)
	{
		Put_Line_Str(254,98,"---");
	}
	else
	{
		Put_Numeral(254,98,pavr,pavr_units);
	}
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(254,96,282,114);
	}		
}

//**********************************************************************
//����  ��:Disp_Peep_Meas_Main
//��������:����������ʾ��ĩ��ѹ
//��ڲ���:peep--��ĩ��ѹ
//		   peep_units--ѹ����λ(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)	
//���ڲ���:��
//**********************************************************************
void Disp_Peep_Meas_Main(int peep,int peep_units)
{
	unsigned short x;

	//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//Ĭ��Ϊ����ģʽ
	
	Clr_Bar (191,78,129,19,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(191,319,78,0xaa);
	Hor_Pattern_Line(191,319,96,0xaa);
	Ver_Pattern_Line(78,96,191,0xaa);
	
	Set_Write_Mode(WRITE_OR);	
	x = Put_Line_Str(193,80,Language_Str("��ĩ��ѹ","Peep"));
	
	Set_ASC_Lib(Arial12B_Font);
	if (peep>=100)
	{
		x = Put_Line_Str(x+3,80,"--");
	}
	else
	{
		x = Put_Numeral(x+3,80,peep,peep_units);
	}
	
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(x+3,82,"cmH2O");	
	Set_Write_Mode(WRITE_OVERLAY);	//�ָ�ΪĬ��ֵ		
}

//**********************************************************************
//����  ��:Disp_Peep_Meas_Main_Title
//��������:������������ʾ��ĩ��ѹ����ֵ�˵��ı���
//��ڲ���:disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Peep_Meas_Main_Title(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Hor_Pattern_Line(191,319,78,0xaa);
	Hor_Pattern_Line(191,319,96,0xaa);
	Ver_Pattern_Line(78,96,191,0xaa);	
	Bar(192,79,253,95);
	Bar(283,79,319,95);
		
	Set_ASC_Lib(Arial12B_Font);//�����ֿ�
	Set_HZ_Lib(&ST_11);	
	Set_Write_Mode(WRITE_OR);	
	Put_Line_Str(193,80,Language_Str("��ĩ��ѹ","Peep"));
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(283,82,"cmH2O");
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(191,78,253,96);
		LCD_Refresh_Area(283,78,319,96);
	}	
}

//**********************************************************************
//����  ��:Disp_Peep_Meas_Vl_Main
//��������:������������ʾ��ĩ��ѹ����ֵ
//��ڲ���:peep--��ĩ��ѹ
//		   peep_units--ѹ����λ(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)	
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Peep_Meas_Vl_Main(int peep,int peep_units,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Bar(254,79,282,95);	
	
	Set_ASC_Lib(Arial12B_Font);
	Set_Write_Mode(WRITE_OR);	
	if (peep>=1000)
	{
		Put_Line_Str(254,80,"---");
	}
	else
	{
		Put_Numeral(254,80,peep,peep_units);
	}
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(254,78,282,96);
	}	
}

//**********************************************************************
//����  ��:Disp_Humor_Meas_Main
//��������:����������ʾ˳Ӧ��
//��ڲ���:humor--˳Ӧ��
//		   humor_units--˳Ӧ�Ե�λ(1--1ml/cmH2O;10--0.1ml/cmH2O;100--0.01ml/cmH2O)	
//���ڲ���:��
//**********************************************************************
void Disp_Humor_Meas_Main(int humor,int humor_units)
{
	unsigned short x;

	//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//Ĭ��Ϊ����ģʽ
	
	Clr_Bar (191,59,129,19,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(191,319,59,0xaa);	//�߿�
	Hor_Pattern_Line(191,319,78,0xaa);
	Ver_Pattern_Line(59,78,191,0xaa);
	
	Set_Write_Mode(WRITE_OR);	
	x = Put_Line_Str(193,61,Language_Str("˳Ӧ��","Humor"));//����
	
	Set_ASC_Lib(Arial12B_Font);
	if (humor >= 100)
	{
		x = Put_Line_Str(x+3,61,"--");
	}
	else
	{	
		x = Put_Numeral(x+3,61,humor,humor_units);	//˳Ӧ�Ե�ֵ
	}
	
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(x+3,63,"ml/cmH2O");	//��λ
	Set_Write_Mode(WRITE_OVERLAY);	//�ָ�ΪĬ��ֵ		
}

//**********************************************************************
//����  ��:Disp_Humor_Meas_Main_Title
//��������:������������ʾ˳Ӧ�Բ���ֵ�Ĳ˵�����
//��ڲ���:disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Humor_Meas_Main_Title(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	Hor_Pattern_Line(191,319,59,0xaa);	//�߿�
	Hor_Pattern_Line(191,319,78,0xaa);
	Ver_Pattern_Line(59,78,191,0xaa);			
	Bar(192,60,239,77);
	Bar(269,60,319,77);
			
	Set_ASC_Lib(Arial12B_Font);//�����ֿ�
	Set_HZ_Lib(&ST_11);
	Set_Write_Mode(WRITE_OR);
	Put_Line_Str(193,61,Language_Str("˳Ӧ��","Humor"));//����
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(269,63,"ml/cmH2O");	//��λ
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(191,59,239,77);
		LCD_Refresh_Area(269,59,319,77);
	}
}

//**********************************************************************
//����  ��:Disp_Humor_Meas_Vl_Main
//��������:������������ʾ˳Ӧ�ԵĲ���ֵ
//��ڲ���:humor--˳Ӧ��
//		   humor_units--˳Ӧ�Ե�λ(1--1ml/cmH2O;10--0.1ml/cmH2O;100--0.01ml/cmH2O)	
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Disp_Humor_Meas_Vl_Main(int humor,int humor_units,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
	Bar(240,60,268,77);	
	
	Set_ASC_Lib(Arial12B_Font);
	Set_Write_Mode(WRITE_OR);
	if (humor >= 1000)
	{
		Put_Line_Str(240,61,"--");
	}
	else
	{	
		Put_Numeral(240,61,humor,humor_units);	//˳Ӧ�Ե�ֵ
	}
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(240,59,268,77);
	}	
}

//**********************************************************************
//����  ��:Disp_Asph_Alm_Set_Main
//��������:������������ʾ��Ϣ����������
//��ڲ���:asph_alm--(ON:��;OFF:��)
//		   disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��,�θ�λΪ1��ʾ��ʼ�����Ʊ���)
//���ڲ���:��
//**********************************************************************
void Disp_Asph_Alm_Set_Main(char asph_alm,char disp_mode)
{
	static char asph_alm_disp;	//������Ϣ������ʾֵ,���asph_alm��asph_alm_disp��һ������˸2��
	static char flash_count;
	static char disp_title_count;	//������ʾ��¼,��Ϊ1��ʾ�����Ѿ���ʾ��,��������ʾ
	static int flash_time;
	unsigned short di;
	
	if (disp_mode&0x40)	//������ʼ������
	{
		asph_alm_disp = 0;
		flash_count = 0;
		disp_title_count = 0;
		flash_time = 0;
		return;
	}
	
	if (!disp_title_count)
	{//δ��ʾ����������ʾ����
		disp_title_count = 1;
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);	//������ʾ
		Bar(191,40,280,57);
		Bar(281,40,319,57);
		Set_Write_Mode(WRITE_EXOR);	//�����ʾ
		Set_ASC_Lib(Arial12B_Font);
		di = Put_Line_Str(194,42,Language_Str("��Ϣ����","HumAlarm"));
		Put_Line_Str(di,40,Language_Str("----","----"));
		if (asph_alm == OFF)
		{
			Put_Line_Str(282,42,Language_Str("�ر�","OFF"));
		}
		else
		{
			Put_Line_Str(282,42,Language_Str("��","ON"));
		}		
		if (disp_mode&0x80) //��ʱˢ��
		{
			LCD_Refresh_Area(191,40,280,57);
			LCD_Refresh_Area(281,40,319,57);
		}
		asph_alm_disp = asph_alm;		
	}

	if (asph_alm_disp != asph_alm)	//������˸����
	{
		asph_alm_disp = asph_alm;	//����������ʾ
		flash_count = 6;
		flash_time = Get_Cur_Time();	//��¼��ʼ��˸ʱ��
	}
	
	if (flash_count)	//�Ƚ���˸ʱ��
	{
		if (Cal_Time_Dif(flash_time)>300)
		{//ʱ�䵽
			flash_count--;
			flash_time = Get_Cur_Time();//���¼�¼��˸ʱ��
			switch (flash_count)
			{
				case 8:
				case 6:
				case 4:
				case 2:
				case 0:			
					Set_Graphic_Color(COL_BLACK,COL_WHITE);
					Set_Chr_Color(COL_BLACK,COL_WHITE);
					Set_Write_Mode(WRITE_OVERLAY);	//������ʾ
					Bar(281,40,319,57);
					Set_Write_Mode(WRITE_EXOR);	//�����ʾ
					Set_ASC_Lib(Arial12B_Font);
					if (asph_alm == OFF)
					{
						Put_Line_Str(282,42,Language_Str("�ر�","OFF"));
					}
					else
					{
						Put_Line_Str(282,42,Language_Str("��","ON"));
					}	
					Set_Write_Mode(WRITE_OVERLAY);	//�ָ�ΪĬ��ֵ	
					break;
				case 1:
				case 3:
				case 5:
				case 7:
					Set_Graphic_Color(COL_BLACK,COL_WHITE);
					Set_Chr_Color(COL_BLACK,COL_WHITE);
					Set_Write_Mode(WRITE_OVERLAY);	//������ʾ
					Bar(281,40,319,57);
					break;	
				default:break;
			}
			if (disp_mode&0x80)//��ʱˢ��
			{
				LCD_Refresh_Area(281,40,319,57);
			}			
		}	
	}
}

//**********************************************************************
//����  ��:Disp_Alm_Area_Main
//��������:��ʾ������������
//��ڲ���:alarm--���������
//			disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��,�θ�λΪ1��ʾ���������)
//���ڲ���:�� 
//**********************************************************************
void Disp_Alm_Area_Main(unsigned int alarm,char disp_mode)
{
	static unsigned int area1_item,area2_item,erea3_item,area4_item;	//��¼������Ŀ
	static char disp_title_flag;
	
	//�б�Ҫ��ʾ�ı�����Ŀ
	

	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
	Pattern_Bar(23,0,319,38,0xaa);
	
	//���ݱ�����Ŀ��ʾ������Ϣ
	
	if (disp_mode&0x80)
	{
		if (!disp_title_flag)
		{
			LCD_Refresh_Area(23,0,319,38);
			disp_title_flag = 1;
		}
	}
}

//**********************************************************************
//����  ��:Disp_Alm_Silence_Main
//��������:�����˵�����ʾ��������
//��ڲ���:alm_sl_time--��������ʱ��(��λ:s)
//			disp_mode--(���λΪ1��ʾ��ʱˢ��;�θ�λΪ1��ʾ��ʼ�����Ʊ���)
//���ڲ���:��
//**********************************************************************
void Disp_Alm_Silence_Main(int alm_sl_time,char disp_mode)
{
	unsigned short x;
	static char disp_title;
	static char alm_silence_flag;	//����������־
	static int alm_sl_time_disp;
	
	if (disp_mode&0x40)
	{
		disp_title = 0;
		alm_silence_flag = 0;
		alm_sl_time_disp = 0xffffffff;
		return;
	}
	
	if (alm_sl_time)	//��������ʱ�䲻Ϊ0����ʾ��������ͼ��
	{
		if (!alm_silence_flag)
		{
			Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
			if (disp_mode & 0x80)
			{
				Set_Refresh_Mode(1);
			}
			Put_Image(0,0,Alarm_Sil_Icon,WRITE_OVERLAY);
			alm_silence_flag = 1;
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Rectangle(0,18,21,38);
			Set_Refresh_Mode(0);
		}	
	}
	else	//��������ʱ��Ϊ0,��ʾ����������ͼ��
	{
		if (alm_silence_flag)
		{
			Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
			if (disp_mode & 0x80)
			{
				Set_Refresh_Mode(1);
			}		
			Put_Image(0,0,Alarm_Loud_Icon,WRITE_OVERLAY);
			alm_silence_flag = 0;
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Rectangle(0,18,21,38);		
			Set_Refresh_Mode(0);
		}	
	}

	if (alm_sl_time_disp != alm_sl_time)
	{	
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
		Bar(1,19,20,37);
						
		Set_ASC_Lib(Arial12B_Font);
		Set_Write_Mode(WRITE_OR);		
		if (alm_sl_time>99 || alm_sl_time==0)
		{
			x = 11-Get_Line_Width("--")/2;
			Put_Line_Str(x,21,"--");
		}
		else
		{
			x = 11-Get_Num_Width(alm_sl_time,1)/2;
			Put_Numeral(x,21,alm_sl_time,1);
		}
		if (disp_mode&0x80)
		{
			LCD_Refresh_Area(1,19,20,37);
		}	
		alm_sl_time_disp = alm_sl_time;//������ʾֵ
	}		
}

//**********************************************************************
//����  ��:Disp_Brth_Mode_Main
//��������:������������ʾ����ģʽ
//��ڲ���:br_mode--����ģʽ
//			br_meth--������ʽ(�ֶ�/�Զ�)
//			disp_mode--���λΪ1��ʾʵʱˢ��,�θ�λΪ1��ʾ��ʼ�����Ʊ���
//���ڲ���:��
//**********************************************************************
void Disp_Brth_Mode_Main(char br_mode,char br_meth,char disp_mode)
{
	unsigned short x;
	static char flash_count=0;	//=0��ʾ����ͼ��;=1��ʾ����ͼ��(���λΪ1��ʾ��ʾ)
	static char flash_flag=0;		//=1�Ѿ���˸
	static int flash_time;		//��¼��˸ʱ��
	static char disp_title_count;//������ʾ
	
	
	if (disp_mode &0x40)	//��ʼ����ʾ���Ʊ���
	{
		flash_count = 0;
		flash_flag = 0;
		disp_title_count = 0;
		flash_time = 0;
		return;
	}
	
	if (!flash_flag)
	{
		flash_flag = 1;
		flash_time = Get_Cur_Time();	//��¼��ʼʱ��
	}
	
	if (Cal_Time_Dif(flash_time) > 500)	//�ı���ʵЧ��
	{
		flash_time = Get_Cur_Time();	//���¼�¼��ʼʱ��
		if (flash_count)
		{
			flash_count = 0|0x80;
		}
		else
		{
			flash_count = 1|0x80;
		}		
	}

	if (!disp_title_count)//��ʾ����
	{	
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
		Bar(0,190,61,207);
		Bar(0,208,14,239);
		Bar(47,208,61,239);
	
		Set_Write_Mode(WRITE_OR);	//��ģʽ
		Set_ASC_Lib(Arial12B_Font);	//�����ֿ�
		Set_HZ_Lib(&ST_11);
		if (br_mode == BRM_VCV)	//��ʾ����ģʽ
		{
			x = 31-Get_Line_Width("VCV")/2;
			Put_Line_Str(x,191,"VCV");
		}
		if (disp_mode&0x80)//ʵʱˢ��
		{
			LCD_Refresh_Area(0,190,61,239);
			LCD_Refresh_Area(0,208,14,239);
			LCD_Refresh_Area(47,208,61,239);
		}
	}	
	
	if (br_meth == BR_AUTO)	//��ʾ��������ͼ��
	{
		if (flash_count&0x80)
		{
			flash_count &= ~0x80;
			
			if (flash_count)//������ʾ
			{
				Put_Image(15,208,Vent_Icon_Press,WRITE_OVERLAY);
			}		
			else
			{
				Put_Image(15,208,Vent_Icon,WRITE_OVERLAY);
			}
			
			if (disp_mode&0x80)//��ʱˢ��
			{
				LCD_Refresh_Area(15,208,46,239);
			}
		}		
	}
	
	disp_title_count = 1;		
}

//**********************************************************************
//����  ��:Disp_Fr_Set_Main
//��������:������������ʾ����Ƶ������
//��ڲ���:fr_set--����Ƶ��
//			disp_mode(���λΪ1��ʾ��ʱˢ��;�θ�λΪ1��ʾֻ��ʼ�����Ʊ���)
//���ڲ���:��
//**********************************************************************
void Disp_Fr_Set_Main(int fr_set,char disp_mode)
{
	static char flash_count;	//=0��ʾ��ʾ;=1��ʾ����(���λΪ1��ʾˢ����ʾ)
	static char flashing_flag;	//=1��ʾ��˸��ʾ;=0��ʾ������ʾ
	static int flash_time;
	static char title_disp_count;
	static int fr_set_disp;	//��¼�ϴ���ʾ����ֵ
	unsigned short en_x,ch_x;
	
	if (disp_mode & 0x40)//��ʼ�����Ʊ���Ϊ�´���ʾ��׼��
	{
		flash_count = 0;
		flashing_flag = 0;
		title_disp_count = 0;
		fr_set_disp = 0;
		flash_time = 0;
		return;
	}
	
	if ( ((disp_mode&0x0f)==DISP_FLASH)&&(!flashing_flag) )	//��Ҫ������˸ģʽ
	{
		flash_time = Get_Cur_Time();		//��¼ʱ��
		flashing_flag = 1;
		flash_count = 0|0x80;				//ˢ����ʾ
	}
	
	if (fr_set_disp != fr_set)	//��ֵ���޸���,���������ʾ
	{
		if (flashing_flag)	//������˸���������ʾ
		{
			flash_time = Get_Cur_Time();	//��¼ʱ��
			flash_count = 0|0x80;	//��ʾ
		}
	}
	
	
	if (!title_disp_count)
	{//��ʾ���⼰��λ
		//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
		Bar(62,190,125,207);	
		Bar(62,229,125,239);
		
		Set_Write_Mode(WRITE_EXOR);	//���ģʽ
		Set_ASC_Lib(Arial12B_Font);	//�����ֿ�
		Set_HZ_Lib(&ST_11);
		ch_x = 94-Get_Line_Width("����Ƶ��")/2;
		en_x = 94-Get_Line_Width("Fr")/2;
		Put_Line_Str(Language_Pos(ch_x,en_x),191,Language_Str("����Ƶ��","Fr"));
			
		Set_ASC_Lib(Arial8_Font);
		en_x = 94-Get_Line_Width("/min")/2;
		Put_Line_Str(en_x,228,"/min");
		if (disp_mode&0x80)	//��ʱˢ��
		{
			LCD_Refresh_Area(62,190,125,207);
			LCD_Refresh_Area(62,229,125,239);
		}
	}
	
	if (!flashing_flag)	//����˸
	{
		if ((!title_disp_count)||(fr_set_disp != fr_set))	//��һ����ʾ������ֵ���޸���
		{
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
			Bar(62,208,125,228);
			
			Set_Write_Mode(WRITE_EXOR);	//���ģʽ		
			Set_ASC_Lib(Arial18B_Font);
			en_x = 94-Get_Num_Width(fr_set,1)/2;
			Put_Numeral(en_x,207,fr_set,1);
			if (disp_mode&0x80)	//��ʱˢ��
			{
				LCD_Refresh_Area(62,208,125,228);
			}
		}
	}
	else	//��˸��ʾ
	{
	
		if (Cal_Time_Dif(flash_time)>500)		//�Ƚ�ʱ��,ʱ�䵽��ı���ʾ��ʽ
		{
			flash_count++;
			flash_count %= 2;
			flash_count |= 0x80;
			flash_time = Get_Cur_Time();//���¼�¼ʱ��
		}
			
		if (flash_count&0x80)	//ˢ��ʱ�䵽
		{
			flash_count &= ~0x80;
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
			Bar(62,208,125,228);			
			if (!flash_count)	//��ʾ��ֵ
			{
				Set_Write_Mode(WRITE_EXOR);	//���ģʽ
				Set_ASC_Lib(Arial18B_Font);
				en_x = 94-Get_Num_Width(fr_set,1)/2;
				Put_Numeral(en_x,207,fr_set,1);
				//Rectangle(62,190,125,239);			
			}
			if (disp_mode&0x80)	//��ʱˢ��
			{
				LCD_Refresh_Area(62,208,125,228);
			}			
		}	
	}

	fr_set_disp = fr_set;	//��¼��ʾ��ֵ	
	title_disp_count = 1;	 
}

//**********************************************************************
//����  ��:Disp_IE_Set_Main
//��������:������������ʾ����������
//��ڲ���:ie_set--����Ƶ��
//			disp_mode(���λΪ1��ʾ��ʱˢ��,�θ�λΪ1��ʾ��ʼ����ʾ���Ʋ���)
//���ڲ���:��
//**********************************************************************
void Disp_IE_Set_Main(int ie_set,char disp_mode)
{
	static char flash_count;	//=0��ʾ��ʾ;=1��ʾ����(���λ��������˸ʱ�Ƿ���ʾ)
	static char flashing_flag;	//=1��ʾ��˸��ʾ;=0��ʾ������ʾ
	static char title_disp_count;	//��ʾ�����������
	static int flash_time;
	static int ie_set_disp;	//��¼�ϴ���ʾ����ֵ
	unsigned short en_x,ch_x;
	char ie1_fm,ie2_fm;
	
	if (disp_mode&0x40)	//��ʼ�����Ʋ���,Ϊ�´���ʾ��׼��,���β���ʾ
	{
		flash_count = 0;
		flashing_flag = 0;
		title_disp_count = 0;
		flash_time = 0;
		ie_set_disp = 0;
		return;
	}
	
	if (ie_set > IE_IND_MAX) return;	//����ֵ����
	if (ie_set < IE_IND_MIN) return;	//����ֵ����
	
	if ( ((disp_mode&0x0f)==DISP_FLASH)&&(!flashing_flag) )	//��Ҫ������˸ģʽ
	{
		flash_time = Get_Cur_Time();		//��¼ʱ��
		flashing_flag = 1;
		flash_count = 0|0x80;
	}
	
	if (ie_set_disp != ie_set)	//��ֵ���޸���,���������ʾ
	{
		if (flashing_flag)	//������˸���������ʾ
		{
			flash_time = Get_Cur_Time();	//��¼ʱ��
			flash_count = 0|0x80;	//��ʾ����ֵ
		}
	}

	if (!title_disp_count)	//��һ�ν���,��ʾ����
	{
		//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
		Bar(127,190,190,207);	
		Bar(127,229,190,239);	
	
		Set_Write_Mode(WRITE_EXOR);	//���ģʽ
		Set_HZ_Lib(&ST_11);			//�����ֿ�
		Set_ASC_Lib(Arial12B_Font);
		ch_x = 159-Get_Line_Width("������")/2;
		en_x = 159-Get_Line_Width("IE")/2;
		Put_Line_Str(Language_Pos(ch_x,en_x),191,Language_Str("������","IE"));
		if (disp_mode&0x80)	//��ʱˢ��
		{
			LCD_Refresh_Area(127,190,190,207);
			LCD_Refresh_Area(127,229,190,239);
		}
	}	
	
	if (!flashing_flag)	//����˸
	{
		if ((!title_disp_count)||(ie_set_disp != ie_set))	//�������޸Ļ��ߵ�һ����ʾ����ʾ����
		{
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
			Bar(127,208,190,228);	
			Set_ASC_Lib(Arial18B_Font);
			Set_Write_Mode(WRITE_EXOR);	//���ģʽ
			ie1_fm = (IE_tab[ie_set][0]<10)?1:10;
			ie2_fm = (IE_tab[ie_set][1]<10)?1:10;
			en_x = 159-Get_Num_Width(IE_tab[ie_set][0],ie1_fm)/2-
					Get_Num_Width(IE_tab[ie_set][1],ie2_fm)/2-
					Get_Line_Width(":")/2;
			en_x = Put_Numeral(en_x,207,IE_tab[ie_set][0],ie1_fm);
			en_x = Put_Line_Str(en_x,207,":");
			Put_Numeral(en_x,207,IE_tab[ie_set][1],ie2_fm);
			if (disp_mode&0x80)
			{
				LCD_Refresh_Area(127,208,190,228);
			}
		}	
	}
	else	//��˸��ʾ
	{
		if (Cal_Time_Dif(flash_time)>500)		//�Ƚ�ʱ��,ʱ�䵽��ı���ʾ��ʽ
		{
			flash_count++;
			flash_count %= 2;
			flash_count |= 0x80;
			flash_time = Get_Cur_Time();	//���¼�¼ʱ��
		}
		
		if (flash_count&0x80)	//��ʾʱ�䵽
		{	
			flash_count &= ~0x80;	//�����ʾʱ�䵽���
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
			Bar(127,208,190,228);				
			if (!flash_count)	//��ʾ��ֵ
			{
				Set_Write_Mode(WRITE_EXOR);	//���ģʽ
				Set_ASC_Lib(Arial18B_Font);
				ie1_fm = (IE_tab[ie_set][0]<10)?1:10;
				ie2_fm = (IE_tab[ie_set][1]<10)?1:10;
				en_x = 159-Get_Num_Width(IE_tab[ie_set][0],ie1_fm)/2-
						Get_Num_Width(IE_tab[ie_set][1],ie2_fm)/2-
						Get_Line_Width(":")/2;
				en_x = Put_Numeral(en_x,207,IE_tab[ie_set][0],ie1_fm);
				en_x = Put_Line_Str(en_x,207,":");
				Put_Numeral(en_x,207,IE_tab[ie_set][1],ie2_fm);
				//Rectangle(127,190,190,239);			
			}
			if (disp_mode&0x80)
			{
				LCD_Refresh_Area(127,208,190,228);
			}			
		}
	}
	
	ie_set_disp = ie_set; //������ʾ����ֵ	
	title_disp_count = 1;				 
}

//**********************************************************************
//����  ��:Disp_Pl_Set_Main
//��������:������������ʾѹ����������
//��ڲ���:pl_set--ѹ������
//			pl_set_units--ѹ�����Ƶ�λ(10--0.1cmH2O)
//			disp_mode--��ʾģʽ(���λΪ1��ʾ��ʱˢ��,�θ�λΪ1��ʾ������
///						ʼ����ʾ�Ŀ��Ʋ���)
//���ڲ���:��
//**********************************************************************
void Disp_Pl_Set_Main(int pl_set,int pl_set_units,char disp_mode)
{
	static char flash_count;	//=0��ʾ��ʾ;=1��ʾ����(���λ��ʾ��ʾ,����λ������ʾ��ʽ)
	static char flashing_flag;	//=1��ʾ��˸��ʾ;=0��ʾ������ʾ
	static char disp_title_count;
	static int flash_time;
	static int pl_set_disp;	//��¼�ϴ���ʾ����ֵ
	unsigned short en_x,ch_x;
	
	if (disp_mode&0x40)		//��ʼ����ʾ���Ʋ���,Ϊ�´���ʾ��׼��
	{
		flash_count = 0;
		flashing_flag = 0;
		disp_title_count = 0;
		flash_time = 0;
		pl_set_disp = 0;
		return;
	}
	
	if ( ((disp_mode&0x0f)==DISP_FLASH)&&(!flashing_flag) )	//��Ҫ������˸ģʽ
	{
		flash_time = Get_Cur_Time();		//��¼ʱ��
		flashing_flag = 1;
		flash_count = 0|0x80;
	}
	
	if (pl_set_disp != pl_set)	//��ֵ���޸���,���������ʾ
	{		
		if (flashing_flag)	//������˸���������ʾ
		{
			flash_time = Get_Cur_Time();	//��¼ʱ��
			flash_count = 0|0x80;	//��ʾ
		}
	}
		
	if (!disp_title_count)	//����û�б���ʾ����ʾ����
	{
		//Set_Refresh_Mode(0);	//Ĭ��Ϊû����ʵˢ��
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
		Bar(192,190,255,207);	
		Bar(192,229,255,239);	
	
		Set_Write_Mode(WRITE_EXOR);	//���ģʽ
		Set_ASC_Lib(Arial12B_Font);	//�����ֿ�
		Set_HZ_Lib(&ST_11);
		ch_x = 224-Get_Line_Width("ѹ������")/2;
		en_x = 224-Get_Line_Width("Plimit")/2;
		Put_Line_Str(Language_Pos(ch_x,en_x),191,Language_Str("ѹ������","Plimit"));
	
		Set_ASC_Lib(Arial8_Font);
		en_x = 224-Get_Line_Width("cmH2O")/2;
		Put_Line_Str(en_x,228,"cmH2O");
		if (disp_mode&0x80)	//��ʱˢ��
		{
			LCD_Refresh_Area(192,190,255,207);
			LCD_Refresh_Area(192,229,255,239);
		}
	}	
	
	if (!flashing_flag)	//����˸
	{
		if ((pl_set_disp != pl_set)||(!disp_title_count))	//�������޸Ĺ������ǵ�һ����ʾ
		{
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
			Set_HZ_Lib(&ST_11);	
			Bar(192,208,255,228);	
			Set_Write_Mode(WRITE_EXOR);	//���ģʽ
			Set_ASC_Lib(Arial18B_Font);
			en_x = 224-Get_Num_Width(pl_set,pl_set_units)/2;
			Put_Numeral(en_x,207,pl_set,pl_set_units);
			Set_Write_Mode(WRITE_OVERLAY);	//�ָ�ΪĬ��ֵ	
			if (disp_mode&0x80)	//��ʱˢ��
			{
				LCD_Refresh_Area(192,208,255,228);
			}
		}	
	}
	else	//��˸��ʾ
	{
		if (Cal_Time_Dif(flash_time)>500)		//�Ƚ�ʱ��,ʱ�䵽��ı���ʾ��ʽ
		{
			flash_count++;
			flash_count %= 2;
			flash_count |= 0x80;
			flash_time = Get_Cur_Time();	//���¼�¼ʱ��
		}	
		if (flash_count&0x80)	//��ʾ
		{
			flash_count &= ~0x80;	//�����ʾ��־
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
			Bar(192,208,255,228);	
			if (!flash_count)
			{
				Set_Write_Mode(WRITE_EXOR);	//���ģʽ	
				Set_HZ_Lib(&ST_11);		//�����ֿ�		
				Set_ASC_Lib(Arial18B_Font);
				en_x = 224-Get_Num_Width(pl_set,pl_set_units)/2;
				Put_Numeral(en_x,207,pl_set,pl_set_units);
				//Rectangle(192,190,255,239);
			}				
			if (disp_mode&0x80)	//��ʱˢ��
			{
				LCD_Refresh_Area(192,208,255,228);
			}		
		}
	}
	
	pl_set_disp = pl_set; //������ʾ��¼
	disp_title_count = 1; //��Ǳ�����ʾ					 
}

//**********************************************************************
//����  ��:Disp_Hold_Set_Main
//��������:������������ʾ������ͣ����
//��ڲ���:hold_set--������ͣ
//			hold_set_units--������ͣ��λ(10--0.1%)
//			disp_mode--��ʾģʽ(���λΪ1��ʾ��ʱˢ��,�θ�λΪ1��ʾ����
//						��ʼ����ʾ�ľ�̬���Ʋ���)
//���ڲ���:��
//**********************************************************************
void Disp_Hold_Set_Main(int hold_set,int hold_set_units,char disp_mode)
{
	static char flash_count;	//=0��ʾ��ʾ;=1��ʾ����(���λΪ1��ʾ��ʾ)
	static char flashing_flag;	//=1��ʾ��˸��ʾ;=0��ʾ������ʾ
	static int flash_time;		//��¼��˸����ʼʱ��
	static int hold_set_disp;	//��¼�ϴ���ʾ����ֵ
	static char disp_title_count;		//=0ʱˢ��һ��ȫ��
	unsigned short en_x,ch_x;
	
	if (disp_mode&0x40)	//������ʼ�����Ʋ���
	{
		flash_count = 0;
		flashing_flag = 0;
		flash_time = 0;
		hold_set_disp = 0;
		disp_title_count = 0;
		return;
	}
	
	if ( ((disp_mode&0x0f)==DISP_FLASH)&&(!flashing_flag) )	//δ������˸ģʽ����Ҫ������˸ģʽ
	{
		flash_time = Get_Cur_Time();		//��¼ʱ��
		flashing_flag = 1;
		flash_count = 0|0x80;				//��ʾ����ֵ����
	}
	else if ((disp_mode&0x0f)==DISP_NORMAL)
	{
		flashing_flag = 0;
		flash_count = 0;
	}
	
	if (hold_set_disp != hold_set)	//��ֵ���޸���,���������ʾ
	{
		if (flashing_flag)	//������˸���������ʾ
		{
			flash_time =  Get_Cur_Time();	//��¼ʱ��
			flash_count = 0|0x80;	//��ʾ
		}
	} 
	
	if (!disp_title_count)
	{//��ʾ���⼰��λ		
		//Set_Refresh_Mode(0);	//Ĭ��Ϊû��ʵʱˢ��
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//����ģʽ	
		Bar(257,190,319,207);	
		Bar(257,229,319,239);
		Set_Write_Mode(WRITE_EXOR);	//���ģʽ
		Set_ASC_Lib(Arial12B_Font);	//�����ֿ�
		Set_HZ_Lib(&ST_11);
		ch_x = 288-Get_Line_Width("������ͣ")/2;
		en_x = 288-Get_Line_Width("Hold")/2;
		Put_Line_Str(Language_Pos(ch_x,en_x),191,Language_Str("������ͣ","Hold"));
		Set_ASC_Lib(Arial8_Font);
		en_x = 288-Get_Line_Width("%")/2;
		Put_Line_Str(en_x,228,"%");
		Set_Write_Mode(WRITE_OVERLAY);
		if (disp_mode&0x80)	//��ʱˢ��
		{
			LCD_Refresh_Area(257,190,319,207);
			LCD_Refresh_Area(257,229,319,239);
		}
	}
		
	if (!flashing_flag)	//����˸
	{
		if ((hold_set_disp != hold_set)||(!disp_title_count))	//�������޸�������ʾ
		{
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);		
			Set_Write_Mode(WRITE_OVERLAY);
			Bar(257,208,319,228);		
			Set_ASC_Lib(Arial18B_Font);
			Set_HZ_Lib(&ST_11);
			Set_Write_Mode(WRITE_EXOR);	//���ģʽ		
			if (hold_set>100)
			{
				en_x = 288-Get_Line_Width("---")/2;
				Put_Line_Str(en_x,207,"---");
			}
			else if (hold_set == 0)
			{
				en_x = 288-Get_Line_Width("�ر�")/2;
				Put_Line_Str(en_x,207,"�ر�");				
			}
			else
			{
				en_x = 288-Get_Num_Width(hold_set,hold_set_units)/2;
				Put_Numeral(en_x,207,hold_set,hold_set_units);
			}
			if (disp_mode&0x80)	//��ʱˢ��
			{
				LCD_Refresh_Area(257,208,319,228);
			}
			Set_Write_Mode(WRITE_OVERLAY);			
		}	
			
	}
	else	//��˸��ʾ
	{
		if (Cal_Time_Dif(flash_time) > 500)		//�Ƚ�ʱ��,ʱ�䵽��ı���ʾ��ʽ
		{
			flash_count++;
			flash_count %= 2;
			flash_count |= 0x80;
			flash_time =  Get_Cur_Time();	//���¼�¼ʱ��
		}
		
		if (flash_count&0x80)	//��ʾ
		{//��ʾ����ֵ
			flash_count &= ~0x80;	//�����ʾ��־
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);		
			Set_Write_Mode(WRITE_OVERLAY);
			Bar(257,208,319,228);
			if (!flash_count)	//��ʾ��ֵ
			{
				Set_Write_Mode(WRITE_EXOR);	//���ģʽ			
				Set_ASC_Lib(Arial18B_Font);						
				if (hold_set>100)
				{
					en_x = 288-Get_Line_Width("---")/2;
					Put_Line_Str(en_x,207,"---");
				}
				else if (hold_set == 0)
				{
					en_x = 288-Get_Line_Width("�ر�")/2;
					Put_Line_Str(en_x,207,"�ر�");				
				}				
				else
				{		
					en_x = 288-Get_Num_Width(hold_set,hold_set_units)/2;
					Put_Numeral(en_x,207,hold_set,hold_set_units);
				}
				//Rectangle(257,190,319,239);			
			}
			if (disp_mode&0x80)	//��ʱˢ��
			{
				LCD_Refresh_Area(257,208,319,228);
			}			
		 }
	}
	hold_set_disp = hold_set;	//������ʾֵ
	disp_title_count = 0x01;	//��Ǳ��ⱻ��ʾ���� 		 
}

//**********************************************************************
//����  ��:Set_Paw_Scale
//��������:����ѹ�����ε�ѹ����
//��ڲ���:p_limit--ѹ������ֵ(��λ:1cmH2O)
//���ڲ���:��
//**********************************************************************
void Set_Paw_Scale(int p_limit)
{
	if (p_limit <= 20) 
	{
		wave_press_scale = 20;
		return;
	}
	if (p_limit <= 40)
	{
		wave_press_scale = 40;
		return;
	}
	if (p_limit <= 60)
	{
		wave_press_scale = 60;
		return;
	}
	
	wave_press_scale = 80;		
}

//**********************************************************************
//����  ��:Set_Time_Scale
//��������:����ѹ�����ε�ʱ����
//��ڲ���:rate--����Ƶ��
//���ڲ���:��
//**********************************************************************
void Set_Time_Scale(int rate)
{
	if (rate > 30)
	{
		wave_time_scale = 5;
		return;
	}
	if (rate > 20)
	{
		wave_time_scale = 10;
		return;
	}
	wave_time_scale = 15;
}

//**********************************************************************
//����  ��:Put_Wave_Time_Axis
//��������:��ʾѹ�����ε�ʱ����
//��ڲ���:disp_mode--ˢ�·�ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Put_Wave_Time_Axis(char disp_mode)
{
	unsigned short di;

	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	if (disp_mode&0x80)//��ʱˢ��
	{
		Set_Refresh_Mode(1);
	}
	Hor_Line(23,180,106);
	for (di=0; di<6; di++)
	{
		Ver_Line(107,108,25+di*31);
	}
	Set_Refresh_Mode(0);	//�رռ�ʱˢ��	
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Bar(162,48,181,57);
	Set_ASC_Lib(Arial8_Font);
	di = Put_Numeral(162,48,wave_time_scale,1);
	Put_Line_Str(di,48,"s");
	if (disp_mode&0x80)//��ʱˢ��
	{
		LCD_Refresh_Area(162,48,181,57);
	}
}

//**********************************************************************
//����  ��:Put_Wave_P_Axis
//��������:��ʾѹ�����ε�ѹ����
//��ڲ���:disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Put_Wave_P_Axis(char disp_mode)
{
	unsigned short di;
		
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OR);//��ģʽ
	if (disp_mode&0x80)//ʵʱˢ��
	{
		Set_Refresh_Mode(1);
	}
	Ver_Line(63,106,23);
	for (di=0; di<5; di++)
	{
		Hor_Line(21,22,63+di*10);
		Put_Numeral(6,100-di*10,wave_press_scale*di/4,1);
	}
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(5,46,"cmH2O");
	Set_Refresh_Mode(0);	
}

//**********************************************************************
//����  ��:Put_Wave_Axis
//��������:��ʾѹ������ʱ�����ѹ����
//��ڲ���:disp_mode--��ʾ��ʽ(���λΪ1��ʾ��ʱˢ��)
//���ڲ���:��
//**********************************************************************
void Put_Wave_Axis(char disp_mode)
{
	Put_Wave_Time_Axis(disp_mode);
	Put_Wave_P_Axis(disp_mode);
	Set_HZ_Lib(&ST_11);
	Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
	if (disp_mode&0x80)
	{
		Set_Refresh_Mode(1);
	}
	Put_Line_Str(76,46,"ѹ����");
	Set_Refresh_Mode(0);
}

//**********************************************************************
//����  ��:Ini_Wave
//��������:��ʼ��ѹ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Ini_Wave(void)
{
	unsigned short di;
	
	for (di=0; di<sizeof(disp_press_wave); di++)
	{//���ѹ����������
		disp_press_wave[di] = 0x00;
	}
	//�����ʾ�Ĳ���
	Set_Refresh_Mode(1);	//ʵʱˢ��
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Bar(26,63,180,103);
	Set_Refresh_Mode(0);	//�ָ�Ĭ��ˢ��ģʽ
	wave_draw_pos = 0;
	wave_brush_pos = 1;
	wave_first_disp_flag = TRUE;
}

//**********************************************************************
//����  ��:Disp_Wave
//��������:��ʾѹ������
//��ڲ���:pressure--ѹ��ֵ
//			pressure_units--ѹ����λ(10--0.1cmH2O)
//���ڲ���:��
//**********************************************************************
void Disp_Wave(int pressure,int pressure_units)
{
	static int disp_time;	//��¼ǰ�ε���ʾʱ��
	static int time_dif;	//�ۼ�δˢ��ʱ��
	int y_press;			

	time_dif += Cal_Time_Dif(disp_time)*WAVE_TIME_C;
	disp_time = Get_Cur_Time();
	if ( (time_dif >= wave_time_scale*1000)||(wave_first_disp_flag) )
	{//��ʾʱ�䵽���ߵ�һ����ʾ
		if (wave_first_disp_flag)
		{
			wave_first_disp_flag = FALSE;
			disp_time = Get_Cur_Time();
			time_dif = 0;
		}
		else
		{
			time_dif -= wave_time_scale*1000;	//�۳��ۼ�δˢ�µ�ʱ��	
		}
		//����ѹ���᳤��
		y_press = (pressure*WAVE_PRESS_C)/(wave_press_scale*pressure_units);
		if (y_press > WAVE_PRESS_C-1)
		{
			y_press = WAVE_PRESS_C-1;
		}
		else if (y_press < 0)
		{
			y_press = 0;
		}
		//��¼����
		disp_press_wave[wave_draw_pos] = y_press;
		disp_press_wave[wave_brush_pos] = 0x00;
		//��ʾ����
		Set_Refresh_Mode(1);		//ʵʱˢ��
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
		Ver_Line(WAVE_PRESS_ORI-y_press,WAVE_PRESS_ORI,WAVE_TIME_ORI+wave_draw_pos);
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
		Ver_Line(WAVE_PRESS_ORI-WAVE_PRESS_C+1,WAVE_PRESS_ORI,WAVE_TIME_ORI+wave_brush_pos);
		Set_Refresh_Mode(0);		//�ָ�ˢ��Ĭ��ֵ
		
		if (++wave_draw_pos > WAVE_TIME_C-1) wave_draw_pos = 0;
		if (++wave_brush_pos > WAVE_TIME_C-1) wave_brush_pos = 0;
	}
}
