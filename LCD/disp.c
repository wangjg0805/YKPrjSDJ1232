//**********************************************************************
//�ļ���:disp.c
//**********************************************************************
#include "global.h"
#include <stdlib.h>
#include <math.h>
#include "Font_Lib.h"
#include "lcd2478.h"

#include "LCD2478_graph.h"
#include "disp.h"

#include "eeprom.h"

#include "locale.h"

#if COLOR_NUMBER == 16
//16-color mode
//������ģ����Ĥ����(1�ֽں�����ģ8��-->1��8ͼ�ε�)
const uint32 ZIMO_MASK[256]=
{
	0x00000000, 0x0000000F, 0x000000F0, 0x000000FF, 0x00000F00, 0x00000F0F, 0x00000FF0, 0x00000FFF,
	0x0000F000, 0x0000F00F, 0x0000F0F0, 0x0000F0FF, 0x0000FF00, 0x0000FF0F, 0x0000FFF0, 0x0000FFFF,
	0x000F0000, 0x000F000F, 0x000F00F0, 0x000F00FF, 0x000F0F00, 0x000F0F0F, 0x000F0FF0, 0x000F0FFF,
	0x000FF000, 0x000FF00F, 0x000FF0F0, 0x000FF0FF, 0x000FFF00, 0x000FFF0F, 0x000FFFF0, 0x000FFFFF,
	0x00F00000, 0x00F0000F, 0x00F000F0, 0x00F000FF, 0x00F00F00, 0x00F00F0F, 0x00F00FF0, 0x00F00FFF,
	0x00F0F000, 0x00F0F00F, 0x00F0F0F0, 0x00F0F0FF, 0x00F0FF00, 0x00F0FF0F, 0x00F0FFF0, 0x00F0FFFF,
	0x00FF0000, 0x00FF000F, 0x00FF00F0, 0x00FF00FF, 0x00FF0F00, 0x00FF0F0F, 0x00FF0FF0, 0x00FF0FFF,
	0x00FFF000, 0x00FFF00F, 0x00FFF0F0, 0x00FFF0FF, 0x00FFFF00, 0x00FFFF0F, 0x00FFFFF0, 0x00FFFFFF,
	0x0F000000, 0x0F00000F, 0x0F0000F0, 0x0F0000FF, 0x0F000F00, 0x0F000F0F, 0x0F000FF0, 0x0F000FFF,
	0x0F00F000, 0x0F00F00F, 0x0F00F0F0, 0x0F00F0FF, 0x0F00FF00, 0x0F00FF0F, 0x0F00FFF0, 0x0F00FFFF,
	0x0F0F0000, 0x0F0F000F, 0x0F0F00F0, 0x0F0F00FF, 0x0F0F0F00, 0x0F0F0F0F, 0x0F0F0FF0, 0x0F0F0FFF,
	0x0F0FF000, 0x0F0FF00F, 0x0F0FF0F0, 0x0F0FF0FF, 0x0F0FFF00, 0x0F0FFF0F, 0x0F0FFFF0, 0x0F0FFFFF,
	0x0FF00000, 0x0FF0000F, 0x0FF000F0, 0x0FF000FF, 0x0FF00F00, 0x0FF00F0F, 0x0FF00FF0, 0x0FF00FFF,
	0x0FF0F000, 0x0FF0F00F, 0x0FF0F0F0, 0x0FF0F0FF, 0x0FF0FF00, 0x0FF0FF0F, 0x0FF0FFF0, 0x0FF0FFFF,
	0x0FFF0000, 0x0FFF000F, 0x0FFF00F0, 0x0FFF00FF, 0x0FFF0F00, 0x0FFF0F0F, 0x0FFF0FF0, 0x0FFF0FFF,
	0x0FFFF000, 0x0FFFF00F, 0x0FFFF0F0, 0x0FFFF0FF, 0x0FFFFF00, 0x0FFFFF0F, 0x0FFFFFF0, 0x0FFFFFFF,
	0xF0000000, 0xF000000F, 0xF00000F0, 0xF00000FF, 0xF0000F00, 0xF0000F0F, 0xF0000FF0, 0xF0000FFF,
	0xF000F000, 0xF000F00F, 0xF000F0F0, 0xF000F0FF, 0xF000FF00, 0xF000FF0F, 0xF000FFF0, 0xF000FFFF,
	0xF00F0000, 0xF00F000F, 0xF00F00F0, 0xF00F00FF, 0xF00F0F00, 0xF00F0F0F, 0xF00F0FF0, 0xF00F0FFF,
	0xF00FF000, 0xF00FF00F, 0xF00FF0F0, 0xF00FF0FF, 0xF00FFF00, 0xF00FFF0F, 0xF00FFFF0, 0xF00FFFFF,
	0xF0F00000, 0xF0F0000F, 0xF0F000F0, 0xF0F000FF, 0xF0F00F00, 0xF0F00F0F, 0xF0F00FF0, 0xF0F00FFF,
	0xF0F0F000, 0xF0F0F00F, 0xF0F0F0F0, 0xF0F0F0FF, 0xF0F0FF00, 0xF0F0FF0F, 0xF0F0FFF0, 0xF0F0FFFF,
	0xF0FF0000, 0xF0FF000F, 0xF0FF00F0, 0xF0FF00FF, 0xF0FF0F00, 0xF0FF0F0F, 0xF0FF0FF0, 0xF0FF0FFF,
	0xF0FFF000, 0xF0FFF00F, 0xF0FFF0F0, 0xF0FFF0FF, 0xF0FFFF00, 0xF0FFFF0F, 0xF0FFFFF0, 0xF0FFFFFF,
	0xFF000000, 0xFF00000F, 0xFF0000F0, 0xFF0000FF, 0xFF000F00, 0xFF000F0F, 0xFF000FF0, 0xFF000FFF,
	0xFF00F000, 0xFF00F00F, 0xFF00F0F0, 0xFF00F0FF, 0xFF00FF00, 0xFF00FF0F, 0xFF00FFF0, 0xFF00FFFF,
	0xFF0F0000, 0xFF0F000F, 0xFF0F00F0, 0xFF0F00FF, 0xFF0F0F00, 0xFF0F0F0F, 0xFF0F0FF0, 0xFF0F0FFF,
	0xFF0FF000, 0xFF0FF00F, 0xFF0FF0F0, 0xFF0FF0FF, 0xFF0FFF00, 0xFF0FFF0F, 0xFF0FFFF0, 0xFF0FFFFF,
	0xFFF00000, 0xFFF0000F, 0xFFF000F0, 0xFFF000FF, 0xFFF00F00, 0xFFF00F0F, 0xFFF00FF0, 0xFFF00FFF,
	0xFFF0F000, 0xFFF0F00F, 0xFFF0F0F0, 0xFFF0F0FF, 0xFFF0FF00, 0xFFF0FF0F, 0xFFF0FFF0, 0xFFF0FFFF,
	0xFFFF0000, 0xFFFF000F, 0xFFFF00F0, 0xFFFF00FF, 0xFFFF0F00, 0xFFFF0F0F, 0xFFFF0FF0, 0xFFFF0FFF,
	0xFFFFF000, 0xFFFFF00F, 0xFFFFF0F0, 0xFFFFF0FF, 0xFFFFFF00, 0xFFFFFF0F, 0xFFFFFFF0, 0xFFFFFFFF,
};

#endif

const         uint8     *asc_lib_ptr;
const  struct font_strc *hz_lib_ptr;    //��Ӣ���ֿ�ָ��

static sint16 chr_space,line_space;     //�ַ������м��
static uint8  cur_language;             //��������

static uint16 char_col,char_back_col;    //����������ɫ,�ַ�ɫ

/*********************************************************************************************************
*1 ����: Init_Cilib1
*1 ����: ���Ի��������ϵͳ
*1 ��ڲ���: ��
*1 ���ڲ���: ��
*********************************************************************************************************/
void Init_Cilib(void)
{
    Init_LCD2478(); 
    
	asc_lib_ptr   = NULL;
	hz_lib_ptr    = &ST_11;            //ֻ��һ�ֺ�������
	char_col      = COL_WHITE;   
	char_back_col = COL_BLACK;
	line_space    = 0;	 
    chr_space     = 0;	
	cur_language  = CHINESE;
}
//**********************************************************************
//����  ��:Set_ASC_Lib2
//��������:������ʾASCII�ַ�ʱ���õ��ֿ�
//��ڲ���:asc_font_ptr--ASCII�ֿ�ָ��
//���ڲ���:��
//**********************************************************************
void Set_ASC_Lib(const  uint8 *asc_font_ptr )
{
    asc_lib_ptr = asc_font_ptr;
}

//**********************************************************************
//����  ��:Set_HZ_Lib3
//��������:������ʾ����ʱ���õ������ֿ�
//��ڲ���:����������ֿ�ָ��
//���ڲ���:��
//**********************************************************************
void Set_HZ_Lib(const struct font_strc *hz_font_ptr)
{
    hz_lib_ptr = hz_font_ptr;
}

//**********************************************************************
//����  ��:Set_Chr_Color4
//��������:����������ʾ����ı���ɫ��������ɫ
//��ڲ���:chr_color--������ɫ
//           back_color--����ɫ
//���ڲ���:��
//**********************************************************************
void Set_Chr_Color(uint16 chr_color,uint16 back_color)
{
    #if COLOR_NUMBER == 16
//16-color mode
	char_col = chr_color & 0x0F;
	char_back_col = back_color & 0x0F;
#else
//65536-color mode
	char_col = chr_color;
	char_back_col = back_color;
#endif
  //  char_col = chr_color;
  //  char_back_col = back_color;
}

//**********************************************************************
//����  ��:Set_Line_Space5
//��������:������ʾ���м��
//��ڲ���:space--�м��
//���ڲ���:��
//**********************************************************************
void Set_Line_Space(sint16 space)
{
    line_space =  space;
}

//**********************************************************************
//����  ��:Set_Char_Space6
//��������:������ʾ���ַ����
//��ڲ���:space--�ַ����
//���ڲ���:��
//**********************************************************************
void Set_Char_Space(sint16 space)
{
    chr_space = space;
}

//**********************************************************************
//����  ��:Set_Language_Type7
//��������:������ʾ����
//��ڲ���:language--��������
//���ڲ���:��
//**********************************************************************
void Set_Language_Type(uint16 language)
{
    cur_language = language;
}

//**********************************************************************
//����  ��:Get_Language_Type8
//��������:��ȡ��ǰ����ʾ����
//��ڲ���:��
//���ڲ���:CHINESE--����
//            ENGLISH--Ӣ��
//**********************************************************************
 uint8 Get_Language_Type(void)
{
    return (cur_language);
}

/*********************************************************************************************************
* ����: Is_HZstr9
* ����: �ж��Ƿ��Ǻ����ַ���
* ��ڲ���: *str�ַ���
* ���ڲ���: 1��, 0����
*********************************************************************************************************/
uint8 Is_HZstr(char *str)
{
	while(*str!='\x0')
	{//δ����
		if((*str<0xA0) && (*str!='\n')) return(0);
		str++;
	}
	return(1);
}

/*********************************************************************************************************
* ����: Get_Char_Height
* ����: ���������ַ��ĸ߶�
* ��ڲ���: chΪ�ַ�ֵ,����ASC�ַ���ֻ�õ��ֽ�(���ֽ�Ϊ0),���ں�����ռ2���ֽ�
* ���ڲ���: �ַ��߶�
*********************************************************************************************************/
uint16 Get_Char_Height(uint16 ch)
{
	if(ch<0xA0)
	{//ASC�ַ�
		if(asc_lib_ptr==NULL) return(0);
		return(asc_lib_ptr[1]);
	}
	//���ַ�
	if(hz_lib_ptr==NULL) return(0);
	return(hz_lib_ptr->height);
}

/*********************************************************************************************************
* ����: Get_Char_Width
* ����: ���������ַ��Ŀ��
* ��ڲ���: chΪ�ַ�ֵ,����ASC�ַ���ֻ�õ��ֽ�(���ֽ�Ϊ0),���ں�����ռ2���ֽ�
* ���ڲ���: �ַ����
*********************************************************************************************************/
uint16 Get_Char_Width(uint16 ch)
{
	if(ch<0xA0)
	{//ASC�ַ�
		if(asc_lib_ptr==NULL) return(0);
		if(ch<0x20) return(asc_lib_ptr[0]);
		return(asc_lib_ptr[ch-0x20+2]);
	}
	//���ַ�
	if(hz_lib_ptr==NULL) return(0);
	return(hz_lib_ptr->width);
}


//**********************************************************************
//����  ��:Save_Lg_Type12
//��������:����������������
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--����
//**********************************************************************
uint8 Save_Lg_Type(void)
{
    if (Write_EEPROM(EEP_LANGUAGE_ADDR,( uint8*)(&cur_language),sizeof(cur_language)) )
    {
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//����  ��:Rd_Lg_Type_EEP13
//��������:��EEPROM�ж�����������,����������ɹ�,��Ĭ��ֵ
//��ڲ���:��
//���ڲ���:OK--��ȷ
//            ERROR--��������
//**********************************************************************
uint8 Rd_Lg_Type_EEP(void)
{
    if (Read_EEPROM(EEP_LANGUAGE_ADDR,( uint8*)(&cur_language),sizeof(cur_language)) )
    {
        cur_language = LG_TYPE_DEF;
        return(ERROR);
    }
    else
    {
        if ((cur_language != CHINESE)&&(cur_language != ENGLISH))
        {
            cur_language = LG_TYPE_DEF;
            Save_Lg_Type();
        }
        return(OK);
    }
}

//**********************************************************************
//����  ��:Language_Str14
//��������:������������ѡ����ȷ���ַ���
//��ڲ���:ch_str--�����ַ���
//           en_str--Ӣ���ַ���
//���ڲ���:�������������Ӧ���ַ���
//**********************************************************************
uint8 * Language_Str(uint8 *ch_ptr,uint8 *en_ptr)
{
    if (cur_language == CHINESE)
    {
        return (ch_ptr);
    }
    else
    {
        return (en_ptr);
    }
}

//**********************************************************************
//����  ��:Language_Pos15
//��������:������������ѡ����ȷ����ʾλ��
//��ڲ���:ch_pos--���ĵ���ʾλ��
//           en_pos--Ӣ�ĵ���ʾλ��
//���ڲ���:�������������Ӧ����ʾλ��
//**********************************************************************
uint16 Language_Pos(uint16 ch_pos,uint16 en_pos)
{
    if (cur_language == CHINESE)
    {
        return (ch_pos);
    }
    else
    {
        return (en_pos);
    }
}

/*********************************************************************************************************
* ����: Put_Char
* ����: ��ָ��λ����ʾһ���ַ�(�����ܳ���64*64)
* ��ڲ���: (cx,cy)������ַ�������,chΪ�ַ�ֵ,����ASC�ַ���ֻ�õ��ֽ�(���ֽ�Ϊ0),���ں�����ռ2���ֽ�
* ���ڲ���: ��
* �ֿ��ʽ: ���ֽ�Ϊ��λÿλ��Ӧ1��,b7~b0���˶�Ӧ������8����,���ŷ�ʽ.
*********************************************************************************************************/
uint16  Put_Char(uint16 cx, uint16 cy, uint16 ch)
{
const uint8 *libptr;
uint16 wid,hig,di;
uint32 chsize,ascw,hzn;
#if COLOR_NUMBER == 16
//16-color mode
uint32 *chptr,fc,gc,mask;
uint32 charmap[1600];	//��ģλͼ��,���֧��4096��(64x64)�ַ�
#else                   //��ģλͼ��,���֧��12800��(80x160)�ַ�
//65536-color mode
uint16 *dispbuf,j,fc,gc,wm;
uint32 zimo;
#endif
//��ȡ�ַ��ĸ߶ȺͿ��
	wid = Get_Char_Width(ch);
	if(wid==0) return;	//�޿�����˳�
	hig = Get_Char_Height(ch);
	if(hig==0) return;	//�޸߶����˳�
	chsize = ((wid + 7) >> 3) * hig;	//��ģ��С
#if COLOR_NUMBER == 16
//16-color mode
	/*if(chsize>512)
	{//��ģ̫������ʾһ�ַ���С�Ŀո񲢷���
		Clr_Bar(cx, cy, wid, hig, char_back_col);
		return;
	}*/
#else
//65536-color mode
	if(cx+wid>SCR_XSIZE) return;	//�ֿ�����ʾ��Χ
	if(cy+hig>SCR_YSIZE) return;	//�ָ߳�����ʾ��Χ
#endif
//��ȡ�ַ���ģ���ֿ��е�λ��
	if(ch<0xA0)
	{//��ASC�ַ�
		if(ch<0x20)
		{//ASC�ַ������ֿⷶΧ��,����ʾһ�ո񲢷���
			Clr_Bar(cx, cy, wid, hig, char_back_col);
			return;
		}
		libptr = &asc_lib_ptr[2];
		for(ascw=0; ch>0x20; ch--)
		{//ǰ���ַ�����ۼ�(���ֽ�Ϊ��λ)
			ascw += (*libptr + 7) >> 3;
			libptr++;
		}
		libptr = &asc_lib_ptr[130+ascw*hig];
	}
	else{//�Ǻ����ַ�
	  for (di=0; di<hz_lib_ptr->chr_sum; di++)    //����ƥ�人��
        {
            if ( (hz_lib_ptr->chr_code)[di] == ch)//�ҵ�����
            {
                break;
            }
        }
        if (di >=hz_lib_ptr->chr_sum )    //�ֿ����޸ú���
        {
            Clr_Bar(cx, cy, wid, hig, char_back_col);
			return(cx+wid);
        }
        libptr = &(hz_lib_ptr->chr_lattice_ptr)[di*((hz_lib_ptr->width+7)>>3)*hz_lib_ptr->height];
     	}
#if COLOR_NUMBER == 16
//16-color mode
//����ʾ���ַ�λͼת��Ϊput_imageʹ�õ�λͼ��ʽ
	chptr = charmap;
	*chptr = (hig << 16) + wid;
	fc = char_col * 0x11111111;
	gc = char_back_col * 0x11111111;
	//��ʼ����ģת��Ϊput_imageʹ�õ�λͼ��ʽ
	for(; chsize>0; chsize--)
	{//�ַ�˳��ȡģ
		mask = ZIMO_MASK[*libptr++];
		*++chptr = (fc & mask) | (gc & ~mask);
	}
//��ʾ�ַ�
	Put_Image(cx, cy, charmap);
#else
//65536-color mode
	fc = char_col;
	gc = char_back_col;
	wm = Get_Write_Mode();
	dispbuf = &disp_buffer[LINE_SIZE*cy+cx];		//calculate pixel address of dispbuf
	if(wm==0) for(; hig>0; hig--)
	{//cover write
		zimo = 0x10000;
		for(j=0; j<wid; j++)
		{
			if(zimo&0xFFFF0000) zimo = 0x100 | *libptr++;
			if(zimo&0x80) dispbuf[j] = fc; else dispbuf[j] = gc;
			zimo <<= 1;
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
	else if(wm==1) for(; hig>0; hig--)
	{//xor write
		zimo = 0x10000;
		for(j=0; j<wid; j++)
		{
			if(zimo&0xFFFF0000) zimo = 0x100 | *libptr++;
			if(zimo&0x80) dispbuf[j] ^= fc; else dispbuf[j] ^= gc;
			zimo <<= 1;
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
	else if(wm==2) for(; hig>0; hig--)
	{//or write
		zimo = 0x10000;
		for(j=0; j<wid; j++)
		{
			if(zimo&0xFFFF0000) zimo = 0x100 | *libptr++;
			if(zimo&0x80) dispbuf[j] |= fc; else dispbuf[j] |= gc;
			zimo <<= 1;
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
	else if(wm==3) for(; hig>0; hig--)
	{//and write
		zimo = 0x10000;
		for(j=0; j<wid; j++)
		{
			if(zimo&0xFFFF0000) zimo = 0x100 | *libptr++;
			if(zimo&0x80) dispbuf[j] &= fc; else dispbuf[j] &= gc;
			zimo <<= 1;
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
#endif
}

//**********************************************************************
//����  ��:Get_Line_Height17
//��������:��ȡһ���ַ����ĸ߶�(ѡ���ַ�������ߵ��ַ��߶�)
//��ڲ���: *str--������ַ���(�Ի��л��ִ���β����Ϊ������)
//���ڲ���:�ַ�������ʾ�߶�
//**********************************************************************
uint16 Get_Line_Height(uint8 *str)
{
    uint16 s=0,h1,h2;

    while((*str!='\x0') && (*str!='\n'))
    {//�����ַ�����β����β�����
        if(*str++ < 0xA0)
        {//��ASC�ַ�
            s |= 1;
            continue;
        }
        //�������ַ�
        if((*str=='\x0') || (*str=='\n')) break;    //��β�����
        s |= 2;
        str++;
    }
    h1 = Get_Char_Height('0');
    h2 = Get_Char_Height(0x100);
    if(s == 1) return(h1);                            //����ASC�ַ�
    if(s == 2) return(h2);                            //���������ַ�
    //����ASC�������Ļ�մ�,��ȡ�ߵ���Ϊ�ܵĸ߶�
    return((h1>h2) ? h1:h2);
}

//**********************************************************************
//����  ��:Get_Line_Width18
//��������:��ȡ����ĵ�ǰ�е��ַ�������ʾ���(�����ַ����)
//��ڲ���:*str--Ϊ������ַ���(�Ի��л��ִ���β����Ϊ������)
//���ڲ���:�ַ�������ʾ���
//**********************************************************************
uint16 Get_Line_Width(uint8 *str)
{
    uint32 w=0;

    while((*str!='\x0') && (*str!='\n'))
    {//�����ַ�����β����β�����
        if(*str<0xA0)
        {//��ASC�ַ�
            w += Get_Char_Width(*str) + chr_space;
        }
        else
        {//�������ַ�
            str++;
            if((*str=='\x0') || (*str=='\n')) break;    //��β�����
            w += Get_Char_Width(0x100) + chr_space;
        }
        str++;
    }
    if(w==0) return(0);
    return(w - chr_space);
}

//**********************************************************************
//����  ��:Put_Line_Str19
//��������:��ָ��λ����ʾ��ǰ���ִ�(����Ӣ�Ļ��)
//��ڲ���:(x,y)--����ַ���������
//            *str--�����ַ���(�Ի��л��ִ���β����Ϊ������)
//���ڲ���:���λ��
//**********************************************************************
uint16 Put_Line_Str(uint16 cx, uint16 cy, uint8 *str)
{
uint16 ch,hig,wid,lh;
uint32 ysp1,ysp2;
//begin
	if((*str=='\x0') || (*str=='\n')) return;	//���ַ�Ϊ��β���������������
	lh = Get_Line_Height(str);
	while(1)
	{
		ch = (uint8)(*str);
		if(ch>=0xA0)
		{//�ַ�ֵ���ڵ���0xA0���ʾ�Ǻ���,�������ASC�ַ�
			str++;
			if((*str=='\x0') || (*str=='\n')) break;	//��β�����
			ch = ch * 256 + (uint8)(*str);				//ȡ����������(2�ֽ�)
		}
		wid = Get_Char_Width(ch);		//���ַ���
		hig = Get_Char_Height(ch);		//���ַ���
		ysp1 = (lh - hig) / 2;			//���������϶�ĸ߶�
		ysp2 = lh - hig - ysp1;			//���������϶�ĸ߶�
		if(ysp1>0)
		{//��������϶>0,���õ�ɫ����ַ��Ϸ�
			Clr_Bar(cx, cy, wid, ysp1, char_back_col);
		}
		if(ysp2>0)
		{//��������϶>0,���õ�ɫ����ַ��·�
			Clr_Bar(cx, cy+lh-ysp2, wid, ysp2, char_back_col);
		}
		Put_Char(cx, cy+ysp1, ch);		//���һ�ַ�(���¾��ж���)
		str++;
		if((*str=='\x0') || (*str=='\n')) break;			//��β�����
		if(chr_space>0) Clr_Bar(cx+wid, cy, chr_space, lh, char_back_col);	//�õ�ɫ����ַ����
		cx += wid + chr_space;			//��һ�ַ��ĺ�����
	}   
}

//**********************************************************************
//����  ��:Get_Str_Height20
//��������:��ȡ����Ķ������ʾ�߶�(�����м��)
//��ڲ���:*str--������ַ���(֧�ֶ��ж���)
//���ڲ���:�������ʾ�߶�
//**********************************************************************
uint16 Get_Str_Height(uint8 *str)
{
    uint16 lh,height=0,flg=1;

    lh = Get_Line_Height("\n");
    while(*str != '\x0')
    {//�����ַ�����β�����
        if(*str == '\n')
        {//�������з����ۼ��и�+�м��
            height += lh + line_space;
            flg = 1;
            str++;
            continue;
        }
        if(flg)
        {
            lh = Get_Line_Height(str);        //��ȡ���е��и�
            flg = 0;
        }
        str++;
    }
    if(flg) return(height);
    return(height + lh);        //���һ�и߶�δ��,����Ҫ����
}

//**********************************************************************
//����  ��:Get_Str_Width21
//��������:��ȡ����Ķ��������е���ʾ���(�����ַ����)
//��ڲ���:    *str--������ַ���(֧�ֶ��ж���)
//���ڲ���:���������е���ʾ���
//**********************************************************************
uint16 Get_Str_Width(uint8 *str)
{
    uint16 max_wid=0,width;

    while(*str != '\x0')
    {//�����ַ�����β�����
        width = Get_Line_Width(str);
        if(max_wid < width)
        {//ȡ�����п�
            max_wid = width;
        }
        while(*str++ != '\n')
        {//���������β
            if(*str == '\x0') break;
        }
    }
    return(max_wid);
}

//**********************************************************************
//����  ��:Put_Str22
//��������:��ָ��λ����ʾ�ִ�����(����Ӣ�Ļ��)
//��ڲ���:(cx,cy)--����ַ���������
//            *str--������ַ���(֧�ֶ���)
//���ڲ���:��
//**********************************************************************
void Put_Str(uint16 cx, uint16 cy, uint8 *str)
{
uint16 lineh,linew=0,flag=1;
//begin
	lineh = Get_Line_Height("\n");
	while(*str!='\x0')
	{//�����ַ�����β�����
		if(*str=='\n')
		{//������β
			if(!flag)
			{//����һ������������Ҫ����м��
				if(line_space>0) Clr_Bar(cx, cy+lineh, linew, line_space, char_back_col);
				flag = 1;
			}
			cy += lineh + line_space;		//����
			str++;
			continue;
		}
		if(flag)
		{//���һ���ִ�����ȡ�п���и�
			lineh = Get_Line_Height(str);
			linew = Get_Line_Width(str);
			Put_Line_Str(cx, cy, str);
			flag = 0;
		}
		str++;
	}
}	

/*********************************************************************************************************
* ����: Put_Data23
* ����: ��ָ��λ�ð�һ����ʽ��ʾ����
* ��ڲ���: (cx,cy)������ַ���������,*msgΪת����ʽ�ִ�,valΪ��ʾ������
* ���ڲ���: ��
;modify by wangjg :del *msg
*********************************************************************************************************/
void Put_Data(uint16 cx, uint16 cy,char *msg,uint32 val)
{
    char dstr[20];
    //setlocale(LC_ALL,"C");
	sprintf(dstr,msg, val);
	Put_Str(cx, cy, dstr);
}
/*********************************************************************************************************
* ����: Put_Data24
* ����: ��ָ��λ�ð�һ����ʽ��ʾ����
* ��ڲ���: (cx,cy)������ַ���������,*msgΪת����ʽ�ִ�,valΪ��ʾ������
* ���ڲ���: ��
;modify by wangjg :del *msg
*********************************************************************************************************/
void Put_Data_Float(uint16 cx, uint16 cy,char *msg,float val)
{
    char dstr[20];
    //setlocale(LC_ALL,"C");
	sprintf(dstr,msg, val);
	Put_Str(cx, cy, dstr);
}

//**********************************************************************
//����  ��:Save_Area
//��������:����ָ���������ʾ����
//��ڲ���:(x1,y1)--�������Ͻ�����
//            (x2,y2)--�������½�����
//            buf--��ʾ���ݱ����ָ��
//���ڲ���:��
//**********************************************************************
void Save_Area(uint16 x1,uint16 y1,
               uint16 x2,uint16 y2,uint32 * buf)
{
    uint32 *disp_buf;
    uint16 byte_width,start_byte,end_byte;
    uint32 di;
    
    if (y1 > y2) return;
    if (x1 > x2) return;
    if (NULL == buf) return;
    start_byte = x1>>3;    //ÿ�е���ʼ�ֽ�
    end_byte = x2>>3;    //ÿ�е�ĩ�ֽ�

    byte_width = end_byte-start_byte+1;//ÿ����ռ���ֽ���
    //buf_temp = buf;     //Ŀ�Ļ���
    for(;y1<=y2;y1++)
    {
        disp_buf = &disp_buffer[y1*LINE_SIZE+start_byte]; //Դ����
        for (di=0; di<byte_width; di++)
        {
            *buf++ = *disp_buf++ ;
        }
    }        
}

//**********************************************************************
//����  ��:Recover_Area
//��������:�ָ�ָ���������ʾ����
//��ڲ���:(x1,y1)--�������Ͻ�����
//            (x2,y2)--�������½�����
//            buf--����ָ����ݵ�ָ��
//���ڲ���:��
//**********************************************************************
void Recover_Area(uint16 x1,uint16 y1,
               uint16 x2,uint16 y2,uint32 * buf)
{
    uint32 *disp_buf;
    uint16 byte_width,start_byte,end_byte;
    uint32 di;
    
    if (y1 > y2) return;
    if (x1 > x2) return;
    if (buf == NULL) return;
    
    start_byte = x1>>3;//�е����ֽ�
    end_byte = x2>>3;//�е�ĩ�ֽ�

    byte_width = end_byte-start_byte+1;//ÿ����ռ���ֽ���
    for(;y1<=y2;y1++)
    {
        disp_buf = &disp_buffer[y1*LINE_SIZE+start_byte];
        for (di=0; di<byte_width; di++)
        {
            *disp_buf++ = *buf++;
        }
    }    
}

                                     