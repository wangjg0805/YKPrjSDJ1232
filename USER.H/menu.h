//**********************************************************************
//�ļ���:menu.h
//**********************************************************************
#ifndef _MENU_H
#define _MENU_H
#include "type_def.h"

struct menu_str
{ 
    sint32 menu_id;        //�˵���
    void (*menu_init)(sint32 op);//�˵���ʼ������ָ��
    sint32 (*op_pro)(uint32 key_c,sint32 op);    //ѡ�����ָ��
    void (*disp_pro)(void);    //��ʾ������ָ��
    void (*assi_pro)(void);    //����������ָ��
};
struct op_str
{
    sint32 op_id;                       //ѡ���
    sint32 (*op_pro)(uint32 key_c);    //ѡ�����ָ��
};
 
#endif