//**********************************************************************
//�ļ�  ��:FontLib.H
//**********************************************************************
#ifndef _FONT_LIB_H
#define _FONT_LIB_H

#include "type_def.h"

struct font_strc
{
    const uint16 *chr_code;         //�ַ����룬ASCII�ַ�����ASCII���룬���ֲ�����λ��
          uint32 chr_sum;           //�ַ������ܺ�
          uint8  width;             //�ַ����ȣ���λΪ����
          uint8  height;            //�ַ��߶ȣ���λΪ����
    const uint8  *chr_lattice_ptr;  //�ַ���������ָ��
};

extern const  uint8 Arial12B_Font[];  //С����
extern const  uint8 Arial16B_Font[];  //����ʹ������
//extern const  uint8 Arial18B_Font[];  //����ʹ������
extern const  uint8 Arial20B_Font[];  //����ʹ������
extern const  uint8 ARIALN26B[];
extern const struct font_strc ST_11;

#endif