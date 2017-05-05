//**********************************************************************
//�ļ���:demarcation.h
//**********************************************************************
#ifndef _DEMARCATION_H
#define _DEMARCATION_H
#include "type_def.h"
//����˵�����Ŀ��(��8λΪ�˵���,��8λΪѡ���)
#define DEMAR_MENU_NUM            5            //�궨ģʽ�µĲ˵�����

#define MENU_DEMAR_LG_SEL            0x00    //����ѡ��Ի���
#define MENU_DEMAR_LG_OP_NUM        2        //ѡ�����
#define MENU_DEMAR_LG_SEL_CH_OP        0x0000    //����
#define MENU_DEMAR_LG_SEL_EN_OP        0x0001    //Ӣ��

#define DEMAR_MAIN_MENU            0x01        //�궨ģʽ���˵�
#define DEMAR_MAIN_MENU_OPS        3            //ѡ�����
#define DEMAR_MAIN_MENU_VT        0x0100        //�궨ģʽ���˵��µ�"�������궨"ѡ��
#define DEMAR_MAIN_MENU_O2        0x0101        //�궨ģʽ���˵��µ�"�����������궨"ѡ��
#define DEMAR_MAIN_MENU_PAW        0x0102        //�궨ģʽ���˵��µ�"ѹ���������궨"ѡ��

#define DEMAR_NEW_PAW_CUR        0x02        //�궨PAW�˵�
#define DEMAR_NEW_PAW_OPS        6            //ѡ�����
#define DEMAR_NEW_PAW_EXIT        0x0200        //�궨PAWʱ��"�˳�"��ť
#define DEMAR_NEW_PAW_SAVE        0x0201        //�궨PAWʱ��"����"��ť
#define DEMAR_NEW_PAW_CHECK        0x0202        //"���"
#define DEMAR_NEW_PAW_NEXT        0x0203        //�궨PAWʱ��"��һ��"��ť
#define DEMAR_NEW_PAW_PRE        0x0204        //�궨PAWʱ��"ǰһ��"��ť
#define DEMAR_NEW_PAW_AD_IN        0x0205        //�궨PAWʱ��"�ֶ�����AD"ѡ��

#define DEMAR_O2_SEN            0x03        //�궨����������
#define DEMAR_O2_SEN_EXIT        0x0300        //�궨����������ʱ"�˳�"
#define DEMAR_O2_SEN_SAVE        0x0301        //�궨����������ʱ"����"
#define DEMAR_O2_SEN_CHCEK        0x0302        //���
#define DEMAR_O2_SEN_NEXT        0x0303        //��һ��
#define DEMAR_O2_SEN_PRE        0x0304        //ǰһ��
#define DEMAR_O2_SEN_AD_IN        0x0305        //�궨����������ʱ"����AD"

#define DEMAR_VT                0x04        //�궨������
#define DEMAR_VT_OPS            6            //ѡ�����
#define DEMAR_VT_EXIT            0X0400        //�궨������ʱ"�˳�"
#define DEMAR_VT_SAVE            0x0401        //�궨������ʱ"����"
#define DEMAR_VT_CHECK            0x0402        //"���"
#define DEMAR_VT_NEXT            0x0403        //�궨������ʱ"��һ��"
#define DEMAR_VT_PRE            0x0404        //�궨������ʱ"ǰһ��"
#define DEMAR_VT_LPS_IN            0x0405        //�궨������ʱ"����Ȧ��"

#define DEMAR_AD_REQ            0x04        //AD��ѯ����
#define MENU_AD_REQ_PAW_BAND    0x0400        //"PAW�˲�����"
#define MENU_AD_REQ_VT_LEN        0x0401        //"�������˲����"

void Demar_Manage(void);
#endif
