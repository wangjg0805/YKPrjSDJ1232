//**********************************************************************
//�ļ���:speaker.h
//**********************************************************************
#ifndef _SPEAKER_H
#define _SPEAKER_H

#include "type_def.h"

//������������
#define SOUND_TYPE_NONE            0    //������
#define SOUND_TYPE_START           1   //����ʱ����������

//������������������
#define SOUND_TYPE_WARNING         2    //����������
#define SOUND_TYPE_TIPS            3    //��ʾ������

//��������������
#define SOUND_TYPE_KEY_VALID       4    //������Ч��
#define SOUND_TYPE_KEY_NO_VALID    5    //������Ч��


//ά��ģʽ��ʹ��
#define SOUND_TYPE_SER_ON         11    //ά��ģʽ�´�����������

//����X9313�Ĺܽ�

#define SPK_X9313_CS		(1<<25)
#define SPK_X9313_INC		(1<<23)
#define SPK_X9313_UD		(1<<24)

#define Set_Ext_OutPort1(X)  FIO0SET = X;
#define Clr_Ext_OutPort1(X)  FIO0CLR = X;

void  Set_LCD_Brightness(uint8 brightness);
void  Set_Default_Bri(void);
void  Init_Brightness_PWM(void);
void  Init_PWM0_DRIVER(void);
#endif