/*********************************************************************************************************
** �ļ���:  LCD2478_graph.c
** ����  :	��LPC2478��ͼ�ι���ʵ�ֻ���,��,Բ,����,���ɫ���
** ˵��  :	��LCD2478_graph.c���빤����Ŀ��Դ�ļ���,����Ҫ�õĵط���LCD2478_graph.h
** �汾  :	V1.2
*********************************************************************************************************/
#include "lcd2478.h"
#include "type_def.h"
#include "disp.h"
#include "global.h"
//#include "locale.h"
#include <stdlib.h>
#include <math.h>
#include "LCD2478_graph.h"

typedef void (*md_func_type)(uint32*,uint32,uint32);

#if COLOR_NUMBER == 16
//16-color mode
typedef void (*wd_func_type)(uint32*,uint32);

//��������Ĥ����
const uint32 DOT_MASK[8]=
{0xF0000000, 0x0F000000, 0x00F00000, 0x000F0000, 0x0000F000, 0x00000F00, 0x000000F0, 0x0000000F};

//�����ߵ���Ĥ����
const uint32 LINE_MASK1[8]=
{0xFFFFFFFF, 0x0FFFFFFF, 0x00FFFFFF, 0x000FFFFF, 0x0000FFFF, 0x00000FFF, 0x000000FF, 0x0000000F};
const uint32 LINE_MASK2[8]=
{0xF0000000, 0xFF000000, 0xFFF00000, 0xFFFF0000, 0xFFFFF000, 0xFFFFFF00, 0xFFFFFFF0, 0xFFFFFFFF};

const uint8 BMP_C16_CVT[256]=
{0x00, 0x04, 0x02, 0x06, 0x01, 0x05, 0x03, 0x08, 0x07, 0x0C, 0x0A, 0x0E, 0x09, 0x0D, 0x0B, 0x0F,
 0x40, 0x44, 0x42, 0x46, 0x41, 0x45, 0x43, 0x48, 0x47, 0x4C, 0x4A, 0x4E, 0x49, 0x4D, 0x4B, 0x4F,
 0x20, 0x24, 0x22, 0x26, 0x21, 0x25, 0x23, 0x28, 0x27, 0x2C, 0x2A, 0x2E, 0x29, 0x2D, 0x2B, 0x2F,
 0x60, 0x64, 0x62, 0x66, 0x61, 0x65, 0x63, 0x68, 0x67, 0x6C, 0x6A, 0x6E, 0x69, 0x6D, 0x6B, 0x6F,
 0x10, 0x14, 0x12, 0x16, 0x11, 0x15, 0x13, 0x18, 0x17, 0x1C, 0x1A, 0x1E, 0x19, 0x1D, 0x1B, 0x1F,
 0x50, 0x54, 0x52, 0x56, 0x51, 0x55, 0x53, 0x58, 0x57, 0x5C, 0x5A, 0x5E, 0x59, 0x5D, 0x5B, 0x5F,
 0x30, 0x34, 0x32, 0x36, 0x31, 0x35, 0x33, 0x38, 0x37, 0x3C, 0x3A, 0x3E, 0x39, 0x3D, 0x3B, 0x3F,
 0x80, 0x84, 0x82, 0x86, 0x81, 0x85, 0x83, 0x88, 0x87, 0x8C, 0x8A, 0x8E, 0x89, 0x8D, 0x8B, 0x8F,
 0x70, 0x74, 0x72, 0x76, 0x71, 0x75, 0x73, 0x78, 0x77, 0x7C, 0x7A, 0x7E, 0x79, 0x7D, 0x7B, 0x7F,
 0xC0, 0xC4, 0xC2, 0xC6, 0xC1, 0xC5, 0xC3, 0xC8, 0xC7, 0xCC, 0xCA, 0xCE, 0xC9, 0xCD, 0xCB, 0xCF,
 0xA0, 0xA4, 0xA2, 0xA6, 0xA1, 0xA5, 0xA3, 0xA8, 0xA7, 0xAC, 0xAA, 0xAE, 0xA9, 0xAD, 0xAB, 0xAF,
 0xE0, 0xE4, 0xE2, 0xE6, 0xE1, 0xE5, 0xE3, 0xE8, 0xE7, 0xEC, 0xEA, 0xEE, 0xE9, 0xED, 0xEB, 0xEF,
 0x90, 0x94, 0x92, 0x96, 0x91, 0x95, 0x93, 0x98, 0x97, 0x9C, 0x9A, 0x9E, 0x99, 0x9D, 0x9B, 0x9F,
 0xD0, 0xD4, 0xD2, 0xD6, 0xD1, 0xD5, 0xD3, 0xD8, 0xD7, 0xDC, 0xDA, 0xDE, 0xD9, 0xDD, 0xDB, 0xDF,
 0xB0, 0xB4, 0xB2, 0xB6, 0xB1, 0xB5, 0xB3, 0xB8, 0xB7, 0xBC, 0xBA, 0xBE, 0xB9, 0xBD, 0xBB, 0xBF,
 0xF0, 0xF4, 0xF2, 0xF6, 0xF1, 0xF5, 0xF3, 0xF8, 0xF7, 0xFC, 0xFA, 0xFE, 0xF9, 0xFD, 0xFB, 0xFF};
#else
//65536-color mode
typedef void (*wd_func_type)(uint16*,uint16);
#endif

//�ڲ�����˵��
uint16 write_mode;					     //дģʽ
static uint16 front_color,back_color;    //ͼ������ǰ��ɫ�ͱ���ɫ

//////////////////////////////////////////////////

/*********************************************************************************************************
* ����: Init_Graph
* ����: ��ʼ��ͼ����ʾϵͳ
* ��ڲ���: ��
* ���ڲ���: ��
*********************************************************************************************************/
void Init_Graph(void)
{
    //��ʼ����ɫϵͳ
	write_mode  = WRITE_OVERLAY;
	front_color = COL_WHITE;
	back_color  = COL_BLACK;
    Clr_Screen();			//����
}
///////////////////////////////////////////////////
//**********************************************************************
//����  ��:Set_Graphic_Color
//��������:����ͼ����ʾ����ǰ��ɫ�ͱ���ɫ
//��ڲ���:fore_col--ǰ��ɫ
//            back_color--����ɫ
//���ڲ���:��
//**********************************************************************
void Set_Graphic_Color(uint16 fore_color,uint16 back_col)
{
   #if COLOR_NUMBER == 16
//16-color mode
	front_color = fore_color & 0x0F;
	 back_color = back_col & 0x0F;
#else
//65536-color mode
	front_color = fore_color;
	 back_color = back_col;
#endif
  
}

/*********************************************************************************************************
* ����: Set_Color
* ����: ���û�����ɫ
* ��ڲ���: ���ʵ���ɫ
* ���ڲ���: ��
*********************************************************************************************************/
void Set_Color(uint16 color)
{
#if COLOR_NUMBER == 16
//16-color mode
	front_color = color & 0x0F;
#else
//65536-color mode
	front_color = color;  //
#endif
}

/*********************************************************************************************************
* ����: Get_Color
* ����: ��ȡ������ɫ
* ��ڲ���: ��
* ���ڲ���: ��ǰ���ʵ���ɫ(0~15)
*********************************************************************************************************/
uint16 Get_Color(void)
{
	return(front_color);
}
//**********************************************************************
//����  ��:Get_Graphic_Color
//��������:��ȡͼ����ʾ����ǰ��ɫ�ͱ���ɫ
//��ڲ���:fc--���淵��ǰ��ɫ�ĵ�ַָ��
//            bc--���淵�ر���ɫ�ĵ�ַָ��
//���ڲ���:��
//**********************************************************************
void Get_Graphic_Color(uint16 *fc,uint16 *bc)
{
    *fc = front_color;
    *bc = back_color;
}

/*********************************************************************************************************
* ����: Set_Write_Mode
* ����: ���û���ģʽ
* ��ڲ���: ����ģʽ��ȡ0(����),1(���),2(��),3(��)
* ���ڲ���: ��
*********************************************************************************************************/
void Set_Write_Mode(uint16 mode)
{
	write_mode = mode & 0x03;
}

/*********************************************************************************************************
* ����: Get_Write_Mode
* ����: ��ȡ����ģʽ
* ��ڲ���: ��
* ���ڲ���: ��ǰ����ģʽ(0~3)
*********************************************************************************************************/
uint16 Get_Write_Mode(void)
{
	return(write_mode);
}

/*********************************************************************************************************
* ����: Cover_Mask_Dispbuf,Xor_Mask_Dispbuf,Or_Mask_Dispbuf,And_Mask_Dispbuf
* ����: ������ʾģʽ����Ӧ����Ĥ��ʾ������
* ��ڲ���: dispbufΪ��ʾ������ָ��,dispdataΪ��ʾ����,maskΪ��ģֵ
* ���ڲ���: ��
*********************************************************************************************************/
void Cover_Mask_Dispbuf(uint32 *dispbuf, uint32 dispdata, uint32 mask)
{
	*dispbuf = (*dispbuf & ~mask) | (dispdata & mask);
}
void Xor_Mask_Dispbuf(uint32 *dispbuf, uint32 dispdata, uint32 mask)
{
	*dispbuf ^= dispdata & mask;
}
void Or_Mask_Dispbuf(uint32 *dispbuf, uint32 dispdata, uint32 mask)
{
	*dispbuf |= dispdata & mask;
}
void And_Mask_Dispbuf(uint32 *dispbuf, uint32 dispdata, uint32 mask)
{
	*dispbuf &= dispdata | ~mask;
}

/*********************************************************************************************************
* ����: Cover_Write_Dispbuf,Xor_Write_Dispbuf,Or_Write_Dispbuf,And_Write_Dispbuf
* ����: ������ʾģʽ����Ӧ��д����ʾ������
* ��ڲ���: dispbufΪ��ʾ������ָ��,dispdataΪ��ʾ����
* ���ڲ���: ��
*********************************************************************************************************/
#if COLOR_NUMBER == 16
//16-color mode
void Cover_Write_Dispbuf(uint32 *dispbuf, uint32 dispdata)
{
	*dispbuf = dispdata;
}
void Xor_Write_Dispbuf(uint32 *dispbuf, uint32 dispdata)
{
	*dispbuf ^= dispdata;
}
void Or_Write_Dispbuf(uint32 *dispbuf, uint32 dispdata)
{
	*dispbuf |= dispdata;
}
void And_Write_Dispbuf(uint32 *dispbuf, uint32 dispdata)
{
	*dispbuf &= dispdata;
}
#else
//65536-color mode
void Cover_Write_Dispbuf(uint16 *dispbuf, uint16 dispdata)
{
	*dispbuf = dispdata;
}
void Xor_Write_Dispbuf(uint16 *dispbuf, uint16 dispdata)
{
	*dispbuf ^= dispdata;
}
void Or_Write_Dispbuf(uint16 *dispbuf, uint16 dispdata)
{
	*dispbuf |= dispdata;
}
void And_Write_Dispbuf(uint16 *dispbuf, uint16 dispdata)
{
	*dispbuf &= dispdata;
}
#endif

/*********************************************************************************************************
* ����: Get_Mask_Dispbuf_Func
* ����: ������ʾģʽ��������Ӧ����ģ����
* ��ڲ���: wmΪ��ʾģʽ
* ���ڲ���: ��ģ����ָ��
*********************************************************************************************************/
md_func_type Get_Mask_Dispbuf_Func(uint16 wm)
{
	if(wm==0) return(Cover_Mask_Dispbuf);
	if(wm==1) return(Xor_Mask_Dispbuf);
	if(wm==2) return(Or_Mask_Dispbuf);
	return(And_Mask_Dispbuf);
}

/*********************************************************************************************************
* ����: Get_Write_Dispbuf_Func
* ����: ������ʾģʽ��������Ӧ��д�뺯��
* ��ڲ���: wmΪ��ʾģʽ
* ���ڲ���: д�뺯��ָ��
*********************************************************************************************************/
wd_func_type Get_Write_Dispbuf_Func(uint16 wm)
{
	if(wm==0) return(Cover_Write_Dispbuf);
	if(wm==1) return(Xor_Write_Dispbuf);
	if(wm==2) return(Or_Write_Dispbuf);
	return(And_Write_Dispbuf);
}

/*********************************************************************************************************
* ����: Put_Pixel
* ����: ��ָ��λ����ָ����ɫ����
* ��ڲ���: (px,py)Ϊ�������,pcΪ�����ɫ
* ���ڲ���: ��
*********************************************************************************************************/
void Put_Pixel(uint16 px, uint16 py, uint16 pc)
{
#if COLOR_NUMBER == 16
//16-color mode
uint32 *dispbuf,mask,mcolor;
md_func_type mdfunc;
#else
//65536-color mode
uint16 *dispbuf;
wd_func_type wdfunc;
#endif
	if(px>=SCR_XSIZE) return;			//position is no valid
	if(py>=SCR_YSIZE) return;
#if COLOR_NUMBER == 16
//16-color mode
	mdfunc = Get_Mask_Dispbuf_Func(write_mode);
	dispbuf = &disp_buffer[LINE_SIZE*py+(px>>3)];	//calculate pixel address of dispbuf
	mask = DOT_MASK[px&0x07];			//get dot mask
	mcolor = (pc & 0x0F) * 0x11111111;
	mdfunc(dispbuf, mcolor, mask);
#else
//65536-color mode
	wdfunc = Get_Write_Dispbuf_Func(write_mode);
	dispbuf = &disp_buffer[LINE_SIZE*py+px];		//calculate pixel address of dispbuf
	wdfunc(dispbuf, pc);
#endif
}

/*********************************************************************************************************
* ����: Get_Pixel
* ����: ����ָ�������ɫ
* ��ڲ���: (px,py)Ϊ�������
* ���ڲ���: �����ɫ
*********************************************************************************************************/
uint16 Get_Pixel(uint16 px, uint16 py)
{
#if COLOR_NUMBER == 16
//16-color mode
uint32 *dispbuf;
#endif
	if(px>=SCR_XSIZE) return(0xFFFF);	//position is no valid
	if(py>=SCR_YSIZE) return(0xFFFF);
#if COLOR_NUMBER == 16
//16-color mode
	dispbuf = &disp_buffer[LINE_SIZE*py+(px>>3)];	//calculate pixel address of dispbuf
	px = (px & 0x07) << 2;
	return((*dispbuf >> (28 - px)) & 0xF);
#else
//65536-color mode
	return(disp_buffer[LINE_SIZE*py+px]);
#endif
}

/*********************************************************************************************************
* ����: Hor_Line
* ����: ��(x1,y)��(x2,y)��һ��ˮƽ��
* ��ڲ���: x1,x2Ϊ�ߵ����˵ĺ�����,yΪ�ߵ�������
* ���ڲ���: ��
*********************************************************************************************************/
void Hor_Line(uint16 x1, uint16 x2, uint16 y)
{
uint16 w;
wd_func_type wdfunc;
#if COLOR_NUMBER == 16
//16-color mode
uint32 *dispbuf,mask1,mask2,mcolor;
md_func_type mdfunc;
#else
//65536-color mode
uint16 *dispbuf;
#endif
	if(x1>=SCR_XSIZE) return;			//position is no valid
	if(x2>=SCR_XSIZE) return;
	if(y>=SCR_YSIZE) return;
	if(x1>x2)
	{//exchange
		w = x1;
		x1 = x2;
		x2 = w;
	}
	wdfunc = Get_Write_Dispbuf_Func(write_mode);
#if COLOR_NUMBER == 16
//16-color mode
	mdfunc = Get_Mask_Dispbuf_Func(write_mode);
	dispbuf = &disp_buffer[LINE_SIZE*y+(x1>>3)];	//calculate line start address of dispbuf
	mask1 = LINE_MASK1[x1&0x07];		//get line start mask
	mask2 = LINE_MASK2[x2&0x07];		//get line end mask
	w = (x2 >> 3) - (x1 >> 3);			//get loop count
	mcolor = front_color * 0x11111111;	//get color mask
	if(w==0)
	{//��ʾ�ĺ���ͷβ����һ������
		mask1 &= mask2;		//���¼�����Ĥ
		mdfunc(dispbuf, mcolor, mask1);
	}
	else{//��ʾ�ĺ���ͷβ����һ����
		mdfunc(dispbuf++, mcolor, mask1);		//��ʾͷ��
		for(; w>1; w--) 
		wdfunc(dispbuf++, mcolor);	//��ʾ�ж�
		mdfunc(dispbuf, mcolor, mask2);			//��ʾβ��
	}
#else
//65536-color mode
	dispbuf = &disp_buffer[LINE_SIZE*y+x1];			//calculate line start address of dispbuf
	for(; x1<=x2; x1++) wdfunc(dispbuf++, front_color);
#endif
}

/*********************************************************************************************************
* ����: Hor_Pattern_Line
* ����: ��(x1,y)��(x2,y)��һ������ˮƽ��
* ��ڲ���: x1,x2Ϊ�ߵ����˵ĺ�����,yΪ�ߵ�������,patternΪ������ģ
* ���ڲ���: ��
*********************************************************************************************************/
void Hor_Pattern_Line(uint16 x1, uint16 x2, uint16 y, uint32 pattern)
{
uint16 w;
#if COLOR_NUMBER == 16
//16-color mode
uint32 *dispbuf,mask1,mask2,mcolor;
md_func_type mdfunc;
#else
//65536-color mode
uint16 *dispbuf;
wd_func_type wdfunc;
#endif
	if(x1>=SCR_XSIZE) return;			//position is no valid
	if(x2>=SCR_XSIZE) return;
	if(y>=SCR_YSIZE) return;
	if(x1>x2)
	{//exchange
		w = x1;
		x1 = x2;
		x2 = w;
	}
#if COLOR_NUMBER == 16
//16-color mode
	mdfunc = Get_Mask_Dispbuf_Func(write_mode);
	dispbuf = &disp_buffer[LINE_SIZE*y+(x1>>3)];	//calculate line start address of dispbuf
	w = (x1 & 0x07) << 2;
	pattern = (pattern >> w) | (pattern << (32 - w));
	pattern *= 0x0F;
	mask1 = LINE_MASK1[x1&0x07] & pattern;	//get line start mask
	mask2 = LINE_MASK2[x2&0x07] & pattern;	//get line end mask
	w = (x2 >> 3) - (x1 >> 3);				//get loop count
	mcolor = front_color * 0x11111111;		//get color mask
	if(w==0)
	{//��ʾ�ĺ���ͷβ����һ������
		mask1 &= mask2;		//���¼�����Ĥ
		mdfunc(dispbuf, mcolor, mask1);
	}
	else{//��ʾ�ĺ���ͷβ����һ����
		mdfunc(dispbuf++, mcolor, mask1);		//��ʾͷ��
		for(; w>1; w--) mdfunc(dispbuf++, mcolor, pattern);	//��ʾ�ж�
		mdfunc(dispbuf, mcolor, mask2);			//��ʾβ��
	}
#else
//65536-color mode
	wdfunc = Get_Write_Dispbuf_Func(write_mode);
	dispbuf = &disp_buffer[LINE_SIZE*y+x1];			//calculate line start address of dispbuf
	for(; x1<=x2; x1++)
	{
		if(pattern & 0xF0000000) wdfunc(dispbuf, front_color);
		dispbuf++;
		pattern = (pattern << 4) | (pattern >> 28);
	}
#endif
}

/*********************************************************************************************************
* ����: Ver_Line
* ����: ��(x,y1)��(x,y2)��һ����ֱ��
* ��ڲ���: y1,y2Ϊ�ߵ����˵�������,xΪ�ߵĺ�����
* ���ڲ���: ��
*********************************************************************************************************/
void Ver_Line(uint16 y1, uint16 y2, uint16 x)
{
uint16 y;
#if COLOR_NUMBER == 16
//16-color mode
uint32 *dispbuf,mask,mcolor;
md_func_type mdfunc;
#else
//65536-color mode
uint16 *dispbuf;
wd_func_type wdfunc;
#endif
	if(x>=SCR_XSIZE) return;				//position is no valid
	if(y1>=SCR_YSIZE) return;
	if(y2>=SCR_YSIZE) return;
	if(y1>y2)
	{//exchange
		y = y1;
		y1 = y2;
		y2 = y;
	}
#if COLOR_NUMBER == 16
//16-color mode
	mdfunc = Get_Mask_Dispbuf_Func(write_mode);
	dispbuf = &disp_buffer[LINE_SIZE*y1+(x>>3)];	//calculate line start address of dispbuf
	mask = DOT_MASK[x&0x07];					//get dot mask
	mcolor = front_color * 0x11111111;			//get color mask
	for(; y1<=y2; y1++)
	{//��ʾһ������
		mdfunc(dispbuf, mcolor, mask);
		dispbuf += LINE_SIZE;
	}
#else
//65536-color mode
	wdfunc = Get_Write_Dispbuf_Func(write_mode);
	dispbuf = &disp_buffer[LINE_SIZE*y1+x];			//calculate line start address of dispbuf
	for(; y1<=y2; y1++)
	{//��ʾһ������
		wdfunc(dispbuf, front_color);
		dispbuf += LINE_SIZE;
	}
#endif
}

/*********************************************************************************************************
* ����: Ver_Pattern_Line
* ����: ��(x,y1)��(x,y2)��һ��������ֱ��
* ��ڲ���: y1,y2Ϊ�ߵ����˵�������,xΪ�ߵĺ�����,patternΪ������ģ
* ���ڲ���: ��
*********************************************************************************************************/
void Ver_Pattern_Line(uint16 y1, uint16 y2, uint16 x, uint32 pattern)
{
uint16 y;
#if COLOR_NUMBER == 16
//16-color mode
uint32 *dispbuf,mask,mcolor;
md_func_type mdfunc;
#else
//65536-color mode
uint16 *dispbuf;
wd_func_type wdfunc;
#endif
	if(x>=SCR_XSIZE) return;				//position is no valid
	if(y1>=SCR_YSIZE) return;
	if(y2>=SCR_YSIZE) return;
	if(y1>y2)
	{//exchange
		y = y1;
		y1 = y2;
		y2 = y;
	}
#if COLOR_NUMBER == 16
//16-color mode
	mdfunc = Get_Mask_Dispbuf_Func(write_mode);
	dispbuf = &disp_buffer[LINE_SIZE*y1+(x>>3)];	//calculate line start address of dispbuf
	mask = DOT_MASK[x&0x07];					//get dot mask
	mcolor = front_color * 0x11111111;			//get color mask
	for(; y1<=y2; y1++)
	{//��ʾһ������
		if(pattern & 0xF0000000) mdfunc(dispbuf, mcolor, mask);
		dispbuf += LINE_SIZE;
		pattern = (pattern << 4) | (pattern >> 28);
	}
#else
//65536-color mode
	wdfunc = Get_Write_Dispbuf_Func(write_mode);
	dispbuf = &disp_buffer[LINE_SIZE*y1+x];			//calculate line start address of dispbuf
	for(; y1<=y2; y1++)
	{//��ʾһ������
		if(pattern & 0xF0000000) wdfunc(dispbuf, front_color);
		dispbuf += LINE_SIZE;
		pattern = (pattern << 4) | (pattern >> 28);
	}
#endif
}

/*********************************************************************************************************
* ����: Line
* ����: ��(x1,y1)��(x2,y2)��һ��ֱ��
* ��ڲ���: (x1,y1),(x2,y2)Ϊ�ߵ����˵�����
* ���ڲ���: ��
*********************************************************************************************************/
void Line(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
sint16 step;
uint16 i;
uint32 j,h,l;
	h = abs(y2 - y1) + 1;				//�����߶ε�ͶӰ�߶�
	l = abs(x2 - x1) + 1;				//�����߶ε�ͶӰ���
//ִ�л�ֱ�ߵ��㷨
	if(l>h)
	{
	//�����>��,��ͨ����N��ˮƽ�߶������һ��ֱ��
		if(x1>x2)
		{//exchange
			i = x1;
			x1 = x2;
			x2 = i;
			i = y1;
			y1 = y2;
			y2 = i;
		}
		if(y1<=y2)
		{
			step = 1;
		}
		else{
			step = -1;
		}
		i = x1;
		j = h >> 1;
		for(; x1<=x2; x1++)
		{
			j += h;
			if(j>=l)
			{
				j -= l;
				Hor_Line(i, x1, y1);
				i = x1 + 1;
				y1 += step;
			}
		}
		if(i<=x2) Hor_Line(i, x2, y1);
		return;
	}
	//�����<=��,��ͨ����N����ֱ�߶������һ��ֱ��
	if(y1>y2)
	{//exchange
		i = y1;
		y1 = y2;
		y2 = i;
		i = x1;
		x1 = x2;
		x2 = i;
	}
	if(x1<=x2)
	{
		step = 1;
	}
	else{
		step = -1;
	}
	i = y1;
	j = l >> 1;
	for(; y1<=y2; y1++)
	{
		j += l;
		if(j>=h)
		{
			j -= h;
			Ver_Line(i, y1, x1);
			i = y1 + 1;
			x1 += step;
		}
	}
	if(i<=y2) Ver_Line(i, y2, x1);
}

/*********************************************************************************************************
* ����: Rectangle
* ����: ��һ������
* ��ڲ���: (x1,y1)Ϊ�������Ͻ�����,(x2,y2)Ϊ�������½�����
* ���ڲ���: ��
*********************************************************************************************************/
void Rectangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
	Hor_Line(x1, x2, y1);		//(x1,y1)->(x2,y1)
	Hor_Line(x1, x2, y2);		//(x1,y2)->(x2,y2)
	Ver_Line(y1, y2, x1);		//(x1,y1)->(x1,y2)
	Ver_Line(y1, y2, x2);		//(x2,y1)->(x2,y2)
}

/*********************************************************************************************************
* ����: Bar
* ����: �������һ������
* ��ڲ���: (x1,y1)Ϊ�������Ͻ�����,(x2,y2)Ϊ�������½�����
* ���ڲ���: ��
*********************************************************************************************************/
void Bar(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
uint16 y;
	if(y1>y2)
	{//exchange
		y = y1;
		y1 = y2;
		y2 = y;
	}
	for(; y1<=y2; y1++)
	{
		Hor_Line(x1, x2, y1);
	}
}
//**********************************************************************
//����  ��:Pattern_Bar
//��������:����������
//��ڲ���:(x1,y1)--�������Ͻ�����
//            (x2,y2)--�������½�����
//            pattern--ˮƽ�ߵ������ʽ(8����Ϊһ��,1bppʹ�õ�8bits,
//            2bppʹ�õ�16bits,3bppʹ�õ�24bits,4bppʹ��ȫ��32bits),���������
//���ڲ���:��
//**********************************************************************
void Pattern_Bar(uint16 x1,uint16 y1,
                uint16 x2,uint16 y2,uint8 patt)
{
    uint8 pattern;
    pattern = patt;
    for (; y1<=y2; y1++)
    {
        Hor_Pattern_Line(x1,x2,y1,pattern);
        pattern = (pattern << 1) | (pattern >> 7);
    }
}  

/*********************************************************************************************************
* ����: Clr_Bar
* ����: ��ָ������ɫ�����һ������
* ��ڲ���: (x1,y1)Ϊ�������Ͻ�����,widthΪ��,heightΪ��,bcΪ�����ɫ
* ���ڲ���: ��
*********************************************************************************************************/
void Clr_Bar(uint16 x1, uint16 y1, uint16 width, uint16 height, uint16 bc)
{
uint16 bcsave;
	if((width==0) || (height==0)) return;		//width or height==0 is invalid
	bcsave = Get_Color();						//save color
	Set_Color(bc);
	Bar(x1, y1, x1+width-1 , y1+height-1);
	Set_Color(bcsave);							//restore color
}

/*********************************************************************************************************
* ����: Circle
* ����: ��һ��Բ
* ��ڲ���: (x,y)ΪԲ�ĵ�����,rΪ�뾶
* ���ڲ���: ��
*********************************************************************************************************/
void Circle(uint16 x, uint16 y, uint16 r)
{
 uint32 i,j,d0,d1;
	if(r==0)
	{//��Բ
		Put_Pixel(x, y, front_color);
		return;
	}
//����뾶��0�������л���
	r++;
	d0 = 0;
	j = (r * 181) / 256;	//r*0.707
 	for(i=r-1; i>=j; i--)
	{
	//��ˮƽ����ֱ�߶������Բ,����Բ�ĶԳ���ֻ�����1/8Բ���ϵ����ݼ��ɻ�����Բ
		d1 = ((uint32)sqrt((r*r - i*i) * 256) - 8) / 16;	//����ˮƽ������ҳ�
		if(d0==0)
		{
			Hor_Line(x-d1, x+d1, y-i);
			Hor_Line(x-d1, x+d1, y+i);
			Ver_Line(y-d1, y+d1, x-i);
			Ver_Line(y-d1, y+d1, x+i);
		}
		else{
			Hor_Line(x-d1, x-d0, y-i);
			Hor_Line(x+d0, x+d1, y-i);
			Hor_Line(x-d1, x-d0, y+i);
			Hor_Line(x+d0, x+d1, y+i);
			Ver_Line(y-d1, y-d0, x-i);
			Ver_Line(y+d0, y+d1, x-i);
			Ver_Line(y-d1, y-d0, x+i);
			Ver_Line(y+d0, y+d1, x+i);
		}
		d0 = d1 + 1;
	}
}

/*********************************************************************************************************
* ����: Fill_Circle
* ����: ���һ��Բ
* ��ڲ���: (x,y)ΪԲ�ĵ�����,rΪ�뾶
* ���ڲ���: ��
*********************************************************************************************************/
void Fill_Circle(uint16 x, uint16 y, uint16 r)
{
uint32 i,d;
	if(r==0)
	{//��Բ
		Put_Pixel(x, y, front_color);
		return;
	}
//����뾶��0�����û�ˮƽ������ҵķ���������һ��Բ
	r++;
	for(i=r-1; i>0; i--)
	{
		d = ((uint32)sqrt((r*r - i*i) * 256) - 8) / 16;	//����ˮƽ������ҳ�
		Hor_Line(x-d, x+d, y-i);
		Hor_Line(x-d, x+d, y+i);
	}
	Hor_Line(x-(r-1), x+(r-1), y);
}


//**********************************************************************
//����  ��:Put_Numeral
//��������:���ƶ�λ�ð���ָ����ʽ�������
//��ڲ���:(x,y)--��ʾλ��
//            num--��ʾ������
//            format--��ʾ��ʽ(С����λ��)(num/format)
//���ڲ���:���λ��
//˵    ��:���ִ������Ϊ12���ַ�(��С����ͷ���)
//**********************************************************************
uint16 Put_Numeral(uint16 x, uint16 y,
                           sint32 num,sint32 format)
{
    sint32 base=1;
    uint8 num_str[13],di=0;
    
    if (x > SCR_XSIZE) return (x);
    if (y > SCR_YSIZE) return (x);
    if (y+Get_Char_Height('0') > SCR_YSIZE) return (x);
    if (num < 0)
    {
        num_str[di++] = '-';//�и���
        num = ~(num-1);    //ȡ����ֵ
    }

    //������ת�����ַ���
    if (num)    //����
    {
        while (1)    //�������ֵ�λ��
        {
            if (num/base < 10)
            {
                break;
            }
            base *= 10;
        }
        
        if (base < format)
        {
            base = format;
        }
        
        while (base)
        {
            num_str[di++] = 0x30+num/base;
            num %= base;
            if (format > 1)    //��С����
            {
                if (format==base)//���µ�ΪС��λ,��С����
                {
                    format = 1;//��ֹ�ظ���С����
                    num_str[di++] = '.';
                }
            }
            base /= 10;
            
        }
    }
    else
    {
        num_str[di++] = '0';
    }
    num_str[di++] = '\0'; //������־
    
    x = Put_Line_Str(x,y,num_str);
    return (x);
}

/*********************************************************************************************************
* ����: Image_Size
* ����: ����һ���������������ڴ�ռ��С(����16ɫģʽ����Ϊ��λ,65536ɫģʽ�԰���Ϊ��λ)
* ��ڲ���: (x1,y1),(x2,y2)Ϊ������������ϽǺ����½�����
* ���ڲ���: ��
* Ӱ����ռ���ڴ�Ĵ�С=��������Ĵ�С+������(����)+����߶�(����)
*********************************************************************************************************/
uint32 Image_Size(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
#if COLOR_NUMBER == 16
//16-color mode
	return(((abs(x2 - x1) >> 3) + 1) * (abs(y2 - y1) + 1) * 4 + 4);
#else
//65536-color mode
	return((abs(x2 - x1) + 1) * (abs(y2 - y1) + 1) * 2 + 4);
#endif
}

/*********************************************************************************************************
* ����: Get_Image
* ����: ��һ����������ͼ�α�����ָ�����ڴ�������
* ��ڲ���: (x1,y1),(x2,y2)Ϊ������������ϽǺ����½�����, imgΪӰ������ָ
* ���ڲ���: ��
* Ӱ�����ĵ�1���ֵĵͰ�����ͼ�εĿ��(��),�߰�����ͼ�θ߶�(��),�����ı��Ǻ��ŵ�λͼ����
* ����16ɫģʽ: λͼ���ݸ�ʽΪ32λ(0xabcdefgh)���˶�Ӧa��b��c��d��e��f��g��h������8����
* ����65536ɫģʽ: λͼ���ݸ�ʽΪ����(16λ)��Ӧ1����
*********************************************************************************************************/
void Get_Image(uint16 x1, uint16 y1, uint16 x2, uint16 y2, void *img)
{
#if COLOR_NUMBER == 16
//16-color mode
uint32 *bitmap,*dispbuf,mask;
uint16 j,l,shfl;
	bitmap = (uint32 *)img;				//convert to uint32 point
#else
//65536-color mode
uint16 *bitmap,*dispbuf,j,l;
	bitmap = (uint16 *)img;				//convert to uint16 point
#endif
	if((x1>=SCR_XSIZE) || (x2>=SCR_XSIZE) || (y1>=SCR_YSIZE) || (y2>=SCR_YSIZE))
	{//position is no valid
		*bitmap = 0;
#if COLOR_NUMBER == 65536
//65536-color mode
		*++bitmap = 0;
#endif
		return;
	}
	if(x1>x2)			//x1 must <= x2
	{//exchange
		l = x1;
		x1 = x2;
		x2 = l;
	}
	if(y1>y2)			//y1 must <= y2
	{//exchange
		l = y1;
		y1 = y2;
		y2 = l;
	}
#if COLOR_NUMBER == 16
//16-color mode
	*bitmap = ((y2 - y1 + 1) << 16) + (x2 - x1 + 1);	//width and height of bitimage
	shfl = (x1 & 0x07) << 2;			//calculate times of left shift
	l = (x2 - x1) >> 3;					//loop time in one line
	mask = LINE_MASK2[(x2-x1)&0x07];	//mask of line end data
	dispbuf = &disp_buffer[LINE_SIZE*y1+(x1>>3)];	//get line start address of dispbuf
	for(; y1<=y2; y1++)
	{//���ϵ���һ��һ��ȡģ
		for(j=0; j<=l; j++)
		{//ÿ�����ߴ����ҽ��к���ȡģ
			*++bitmap = (dispbuf[j] << shfl) | (dispbuf[j+1] >> (32 - shfl));
		}
		*bitmap &= mask;		//β�˽�ȡ
		dispbuf += LINE_SIZE;	//calculate next line start address
	}
#else
//65536-color mode
	*bitmap = x2 - x1 + 1;		//width of bitimage
	*++bitmap = y2 - y1 + 1;	//height of bitimage
	l = x2 - x1;				//loop time in one line
	dispbuf = &disp_buffer[LINE_SIZE*y1+x1];		//get line start address of dispbuf
	for(; y1<=y2; y1++)
	{//���ϵ���һ��һ��ȡģ
		for(j=0; j<=l; j++)
		{//ÿ�����ߴ����ҽ��к���ȡģ
			*++bitmap = dispbuf[j];
		}
		dispbuf += LINE_SIZE;	//calculate next line start address
	}
#endif
}

/*********************************************************************************************************
* ����: Put_Image
* ����: ��ʾһ����������bitmapͼ��
* ��ڲ���: (x1,y1)Ϊ������������Ͻ�����, imgΪӰ������ָ
* ���ڲ���: ��
* Ӱ�����ĵ�1���ֵĵͰ�����ͼ�εĿ��(��),�߰�����ͼ�θ߶�(��),�����ı��Ǻ��ŵ�λͼ����
* ����16ɫģʽ: λͼ���ݸ�ʽΪ32λ(0xabcdefgh)���˶�Ӧa��b��c��d��e��f��g��h������8����
* ����65536ɫģʽ: λͼ���ݸ�ʽΪ����(16λ)��Ӧ1����
*********************************************************************************************************/
void Put_Image(uint16 x1, uint16 y1, void *img)
{
wd_func_type wdfunc;
#if COLOR_NUMBER == 16
//16-color mode
uint32 *bitmap,*dispbuf,mask1,mask2,bmd0,bmd1,bmdata;
uint16 x2,y2,shfr,l,j,flg;
md_func_type mdfunc;
	bitmap = (uint32 *)img;				//convert to uint32 point
	l = *bitmap & 0xFFFF;				//image width
	j = *bitmap >> 16;					//image height
#else
//65536-color mode
uint16 *bitmap,*dispbuf,x2,y2,l,j;
	bitmap = (uint16 *)img;				//convert to uint16 point
	l = *bitmap;						//image width
	j = *++bitmap;						//image height
#endif
	if(l==0) return;					//width of image is 0 
	if(j==0) return;					//height of image is 0 
	x2 = l + x1 - 1;					//get right-down position
	y2 = j + y1 - 1;
	if(x1>=SCR_XSIZE) return;			//position is no valid
	if(x2>=SCR_XSIZE) return;
	if(y1>=SCR_YSIZE) return;
	if(y2>=SCR_YSIZE) return;
	wdfunc = Get_Write_Dispbuf_Func(write_mode);
#if COLOR_NUMBER == 16
//16-color mode
	mdfunc = Get_Mask_Dispbuf_Func(write_mode);
	shfr = (x1 & 0x07) << 2;			//calculate times of right shift
	l = (x2 >> 3) - (x1 >> 3);			//loop time in one line
	flg = (x2 & 0x07)>=(x1 & 0x07);		//end rel pos>=start rel pos
	mask1 = LINE_MASK1[x1&0x07];		//get line start mask
	mask2 = LINE_MASK2[x2&0x07];		//get line end mask
	dispbuf = &disp_buffer[LINE_SIZE*y1+(x1>>3)];	//get line start address
	if(l==0) mask1 &= mask2;		//���¼�����Ĥ
	for(; y1<=y2; y1++)
	{//���϶�����ʾ
		bmd0 = *++bitmap;
		bmdata = bmd0 >> shfr;
		mdfunc(dispbuf, bmdata, mask1);			//��ʾͷ��
		if(l>0)
		{
			for(j=1; j<l; j++)
			{//��ʾ�ж�
				bmd1 = *++bitmap;
				bmdata = (bmd0 << (32 - shfr)) | (bmd1 >> shfr);
				bmd0 = bmd1;
				wdfunc(&dispbuf[j], bmdata);
			}
			bmdata = bmd0 << (32 - shfr);
			if(flg) bmdata |= *++bitmap >> shfr;	//β����ʣ��
			mdfunc(&dispbuf[l], bmdata, mask2);		//��ʾβ��
		}
		dispbuf += LINE_SIZE;		//calculate next line start address
	}
#else
//65536-color mode
	dispbuf = &disp_buffer[LINE_SIZE*y1+x1];		//get line start address
	for(; y1<=y2; y1++)
	{//���϶�����ʾ
		for(j=0; j<l; j++) wdfunc(&dispbuf[j], *++bitmap);
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
#endif
}

/*********************************************************************************************************
* ����: Put_Bmp
* ����: ��BMP��ʽ��ͼƬ��ʾ��ָ��λ��
* ��ڲ���: (x1,y1)��ͼƬ��ʾλ��,*bmp��ͼƬ����ָ��
* ���ڲ���: ��
* 16ɫBMPͼƬ�ĸ�ʽ: �ļ�ͷ��118�ֽڣ����е�18��19�ֽ�ΪͼƬ��ȣ���22��23�ֽ�ΪͼƬ�߶�
* �ӵ�119�ֽڿ�ʼ��λͼ����ÿ4λ��Ӧ1���ص�(1�ֽ�=2��)
* ������������У�ÿ�еĳ�����ͼƬ��ȵĹ�ϵ��: ����=((���+7)/8)*4 (�ֽ�)
* ������µ������У�����=�߶ȣ�λͼ���ܵĴ�С��ÿ�г���*���� (�ֽ�)
**********************************************************************************************************
* 24λ���ɫBMPͼƬ�ĸ�ʽ: �ļ�ͷ��54�ֽڣ����е�18��19�ֽ�ΪͼƬ��ȣ���22��23�ֽ�ΪͼƬ�߶�
* �ӵ�55�ֽڿ�ʼ��λͼ����ÿ3�ֽ�(��С�������˶�ӦBGR)��Ӧ1���ص�
* ������������У�ÿ�еĳ�����ͼƬ��ȵĹ�ϵ��: ����=((���*3+3)/4)*4 (�ֽ�)
* ������µ������У�����=�߶ȣ�λͼ���ܵĴ�С��ÿ�г���*���� (�ֽ�)
**********************************************************************************************************
* 16λɫBMP(5:6:5)ͼƬ�ĸ�ʽ: �ļ�ͷ��70�ֽڣ����е�18��19�ֽ�ΪͼƬ��ȣ���22��23�ֽ�ΪͼƬ�߶�
* �ӵ�55�ֽڿ�ʼ��λͼ����ÿ2�ֽ�(��С�������˶�ӦB5:G6:R5)��Ӧ1���ص�
* ������������У�ÿ�еĳ�����ͼƬ��ȵĹ�ϵ��: ����=((���*2+3)/4)*4 (�ֽ�)
* ������µ������У�����=�߶ȣ�λͼ���ܵĴ�С��ÿ�г���*���� (�ֽ�)
*********************************************************************************************************/
void Put_Bmp(uint16 x1, uint16 y1, void *bmp, uint32 reverse_flag)
{
uint16 *bmpwp,bmpls,bmpw,bmph,x2,y2,j;
uint8 *bmpbp;
wd_func_type wdfunc;
#if COLOR_NUMBER == 16
//16-color mode
uint32 *dispbuf,mask1,mask2,bmd0,bmd1,bmdata;
uint16 shfr,l;
md_func_type mdfunc;
#else
//65536-color mode
uint16 *dispbuf,cl;
#endif
	bmpbp = (uint8 *)bmp;					//convert to uint8 point
	bmpwp = (uint16 *)bmp;					//convert to uint16 point
	bmpw = bmpwp[9];						//width of bmp
	bmph = bmpwp[11];						//height of bmp
	if(bmpw==0) return;						//width of bmp is 0 
	if(bmph==0) return;						//height of bmp is 0 
	x2 = bmpw + x1 - 1;						//get right-down position
	y2 = bmph + y1 - 1;
	if(x1>=SCR_XSIZE) return;				//position is no valid
	if(x2>=SCR_XSIZE) return;
	if(y1>=SCR_YSIZE) return;
	if(y2>=SCR_YSIZE) return;
	wdfunc = Get_Write_Dispbuf_Func(write_mode);
#if COLOR_NUMBER == 16
//16-color mode
	mdfunc = Get_Mask_Dispbuf_Func(write_mode);
	shfr = (x1 & 0x07) << 2;			//calculate times of right shift
	l = (x2 >> 3) - (x1 >> 3);			//loop time in one line
	mask1 = LINE_MASK1[x1&0x07];		//get line start mask
	mask2 = LINE_MASK2[x2&0x07];		//get line end mask
	bmpls = ((bmpw + 7) >> 3) * 4;		//line size of bmp(uint8)
	bmpbp += 118 + bmph * bmpls;
	dispbuf = &disp_buffer[LINE_SIZE*y1+(x1>>3)];	//get line start address
	if(l==0) mask1 &= mask2;			//���¼�����Ĥ
	for(; y1<=y2; y1++)
	{//���϶�����ʾ
		bmpbp -= bmpls;					//calculate start address in bmp line
		for(j=0; j<=l; j++)
		{//��ʾ���
			bmd1 = (BMP_C16_CVT[bmpbp[j*4]] << 24) + (BMP_C16_CVT[bmpbp[j*4+1]] << 16) +
					(BMP_C16_CVT[bmpbp[j*4+2]] << 8) + BMP_C16_CVT[bmpbp[j*4+3]];
			bmdata = (bmd0 << (32 - shfr)) | (bmd1 >> shfr);
			bmd0 = bmd1;
			
			if(1 == reverse_flag)
			   bmdata = ~bmdata;
			if(j==0) mdfunc(&dispbuf[j], bmdata, mask1);
			else if(j<l) wdfunc(&dispbuf[j], bmdata);
			else mdfunc(&dispbuf[j], bmdata, mask2);
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
#else
//65536-color mode
#ifdef BMP_24bit_COLOR
//ͼƬΪ24λ���ɫ
	bmpls = ((bmpw * 3 + 3) >> 2) * 4;		//line size of bmp(uint8)
	bmpbp += 54 + bmph * bmpls;
	dispbuf = &disp_buffer[LINE_SIZE*y1+x1];		//get line start address
	for(; y1<=y2; y1++)
	{//���϶�����ʾ
		bmpbp -= bmpls;					//calculate start address in bmp line
		for(j=0; j<bmpw; j++)
		{
			cl = bmpbp[j*3] >> 3;
			cl |= (bmpbp[j*3+1] >> 2) << 5;
			cl |= (bmpbp[j*3+2] >> 3) << 11;
			wdfunc(&dispbuf[j], cl);
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
#else
//ͼƬΪ16λɫ(5:6:5)
	bmpls = ((bmpw * 2 + 3) >> 2) * 2;		//line size of bmp(uint16)
	bmpwp += 70 / 2 + bmph * bmpls;
	dispbuf = &disp_buffer[LINE_SIZE*y1+x1];		//get line start address
	for(; y1<=y2; y1++)
	{//���϶�����ʾ
		bmpwp -= bmpls;					//calculate start address in bmp line
		for(j=0; j<bmpw; j++)
		{
			cl = bmpwp[j];
			wdfunc(&dispbuf[j], cl);
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
#endif
#endif
}

/*********************************************************************************************************
* ����: Put_Button_Frame
* ����: ��һ������Ŀ��İ�ť��
* ��ڲ���: depth�ǰ�͹���, >0��ʾ͹��, <0��ʾ����
*			(x1,y1)��ť������Ͻ�, (x2,y2)��ť������½�
* ���ڲ���: ��
*********************************************************************************************************/
void Put_Button_Frame(uint16 x1, uint16 y1, uint16 x2, uint16 y2, sint16 depth)
{
uint32 i;
	for(i=0; i<depth; i++)
	{
		if(i==0) Set_Color(7); else Set_Color(15);
		Hor_Line(x1, x2, y1);
		Ver_Line(y1, y2, x1);
		if(i==0) Set_Color(0); else Set_Color(8);
		Hor_Line(x1, x2, y2);
		Ver_Line(y1+1, y2, x2);
		x1++;
		y1++;
		x2--;
		y2--;
	}
	for(i=depth; i<0; i++)
	{
		if(i==-1) Set_Color(0); else Set_Color(8);
		Hor_Line(x1, x2, y1);
		Ver_Line(y1, y2, x1);
		if(i==-1) Set_Color(7); else Set_Color(15);
		Hor_Line(x1, x2, y2);
		Ver_Line(y1+1, y2, x2);
		x1++;
		y1++;
		x2--;
		y2--;
	}
}

/*********************************************************************************************************
* ����: Put_Button_Bar
* ����: ��һ������İ�ť
* ��ڲ���: depth�ǰ�͹���, >0��ʾ͹��, <0��ʾ����
*			(x1,y1)��ť�����Ͻ�, (x2,y2)��ť�����½�, bc�ǰ�ť����ɫ
* ���ڲ���: ��
*********************************************************************************************************/
void Put_Button_Bar(uint16 x1, uint16 y1, uint16 x2, uint16 y2, sint16 depth, uint16 bc)
{
	Put_Button_Frame(x1, y1, x2, y2, depth);
	Set_Color(bc);
	Bar(x1+abs(depth), y1+abs(depth), x2-abs(depth), y2-abs(depth));
}

/*********************************************************************************************************
* ����: Create_Window_Frame
* ����: ����һ������
* ��ڲ���: (x1,y1) ���ڵ����Ͻ�, (x2,y2)���ڵĿ�͸�	
*            depth  ���
*			 ���ڱ������̶�Ϊ��ɫ
			 BC ��ɫ������䴰��
* ���ڲ���: ��
*********************************************************************************************************/
void Create_Window_Frame(uint16 x1, uint16 y1, uint16 x2, uint16 y2, sint16 depth, uint16 bc)
{	
   uint16 x,y,z;
	x = x1+x2+2*abs(depth);
	y = y1+y2+2*abs(depth);
	Put_Button_Frame(x1, y1, x, y, depth);	//create frame
	Clr_Bar(x1+abs(depth), y1+abs(depth),    x2, TITLE_HEIGTH,COL_DEEPBLUE); //create title
	Clr_Bar(x1+abs(depth), y1+abs(depth)+TITLE_HEIGTH, x2, y2-TITLE_HEIGTH,    bc); //create NULL AREA
}

//**********************************************************************
//����  ��:Clr_Screen
//��������:�ñ���ɫ����
//��ڲ���:disp_mode
//���ڲ���:��
//**********************************************************************
void Clr_Screen(void)
{
    uint32 di;
    uint32 ext_col;
    
    ext_col = back_color*0x11111111;
    for (di=0; di<LINE_SIZE*SCR_YSIZE; di++)
    {
        disp_buffer[di] = ext_col;
    }
}

/*********************************************************************************************************
* End File
*********************************************************************************************************/