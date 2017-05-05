//**********************************************************************
//�ļ���:service.h
//**********************************************************************
#ifndef _SERVICE_H
#define _SERVICE_H

#include "type_def.h"

#define MENU_SERV_MAIN_SEL               0x00       //���˵�
#define MENU_SERV_MAIN_OP_NUM               8       //ѡ�����

#define MENU_SERV_SEL_SENSOR_OP        0x0100       //AD���� ��ȡԭʼ�������˲������ֵ
#define MENU_SERV_SEL_TEMP_OP          0x0200       //�¶�   ��ȡ�¶���
#define MENU_SERV_SEL_EEPROM_OP        0x0300       //EEPROM ��д����
#define MENU_SERV_SEL_AD7843_OP        0x0400       //AD7843 ԭʼ�ź�
#define MENU_SERV_SEL_DDLV_OP          0x0500       //�絼�� ��·����
#define MENU_SERV_SEL_INTERFACE_OP     0x0600       //����   �ӿ��źš���ָʾ�Ƶ�
                      //��������ָʾ��
                      //�����ź�ָʾ��
                      //���������ź�
#define MENU_SERV_SEL_BRISPK_OP        0x0700       //����   ������������ȱ仯
#define MENU_SERV_SEL_SCREEN_OP        0x0800       //��Ļ   ��ɫ���

extern void  Start_Service_process(void);
#endif