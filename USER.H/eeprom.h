//**********************************************************************
//�ļ���:eeprom.h
//**********************************************************************
#ifndef _EEPROM_H
#define _EEPROM_H
#include "type_def.h"

#define Macro_En_EEPROM()             (FIO3CLR = P3_WP)
#define Macro_Dis_EEPROM()            (FIO3SET = P3_WP)

//������� ����������ֵ
#define    EEP_ZERO_ADDR1              0x00    //���
#define    EEP_ZERO_ADDR2              0x04    //���
#define    EEP_ZERO_ADDR3              0x08    //���
#define    EEP_ZERO_ADDR4              0x0C    //���

#define    EEP_LOW1_ADDR1              0x10    //LOW1
#define    EEP_LOW1_ADDR2              0x14    
#define    EEP_LOW1_ADDR3              0x18    
#define    EEP_LOW1_ADDR4              0x1C    

#define    EEP_LOW2_ADDR1              0x20    //���������̵�
#define    EEP_LOW2_ADDR2              0x24    
#define    EEP_LOW2_ADDR3              0x28    
#define    EEP_LOW2_ADDR4              0x2C  

#define    EEP_FULL1_ADDR1             0x30    //���������̵�
#define    EEP_FULL1_ADDR2             0x34    
#define    EEP_FULL1_ADDR3             0x38    
#define    EEP_FULL1_ADDR4             0x3C    

#define    EEP_FULL2_ADDR1             0x40    //����1400���̵�
#define    EEP_FULL2_ADDR2             0x44    
#define    EEP_FULL2_ADDR3             0x48    
#define    EEP_FULL2_ADDR4             0x4C  

#define    EEP_FULL3_ADDR1             0x50   //����2000���̵�
#define    EEP_FULL3_ADDR2             0x51
#define    EEP_FULL3_ADDR3             0x52
#define    EEP_FULL3_ADDR4             0x53 
  
#define    EEP_FULL4_ADDR1             0x5C   //��ű�־��У����
#define    EEP_FULL4_ADDR2             0x5D
#define    EEP_FULL4_ADDR3             0x5E
#define    EEP_FULL4_ADDR4             0x5F 
 

//���� ��ĻУ׼��ֵ
#define   EEP_SCREEN_ADDR              0x60 //������������ݸ�ռ4���ֽ�+У��     = 13���ֽ�
//���� ϵͳ����
//1:�������� 2���� 3��������
#define   EEP_LANGUAGE_ADDR            0x70
#define   EEP_BRIGHT_ADDR              0x74
#define   EEP_STANDARD_ADDR            0x78
#define   EEP_TEMP_COMP_ADDR           0x7c   //�������ݿ���Ϊ���϶� Ҳ�����ǻ��϶�
#define   EEP_DIV_ADDR                 0x80
#define   EEP_USER_ID_ADDR             0x84

#define   EEP_DDLV_MODE_ADDR           0x90
//��90��ַ��ʼ���δ�������²��� 
//90:       ��ŵ�ǰ�Ĳ�������
//91:       ��ʱ��ǰ�Ĳ���ģʽ
//9293:     ��ʱģʽ�µĶ�ʱʱ�� ���Ϊ 999S
//9495:     ƽ��ģʽ�µ�ƽ��ʱ�� ���Ϊ  60S
//96979899: ƽ��ģʽ�µ�ƽ������ ������
//9E9F:     ��Ч���ݱ�־ 0X55AA

////////////////////DDLV CONS SAVE
#define   EEP_DDLV_CONS_ADDR           0xA0    //ռ��16���ֽ� 
//��A0��ַ��ʼ���δ�������²��� 
//A0A1A2A3:   ���  �缫����
//A4A5A6A7    ���  �²�ϵ��
//A8A9A10A11: ���  TDS ת��ϵ��
//AEAF:     ��Ч���ݱ�־ 0X55AA


//�����û�У׼���ݴ洢 ���5������
#define    EEP_USER_CAL_ADDR1          0xB0   
#define    EEP_USER_CAL_ADDR2          0xB8   
#define    EEP_USER_CAL_ADDR3          0xC0
#define    EEP_USER_CAL_ADDR4          0xC8
#define    EEP_USER_CAL_ADDR5          0xD0
//��ŵ���У׼�ĵڼ�����
#define    EEP_USER_CAL_FLAG           0xE0

#define    EEP_USER_CAL_POINT1         0xEB
#define    EEP_USER_CAL_POINT2         0xEC
#define    EEP_USER_CAL_POINT3         0xED
#define    EEP_USER_CAL_POINT4         0xEE
#define    EEP_USER_CAL_POINT5         0xEF
  
  
//������Ե�ַ ����Ϊ��������
//0xf0-f3 :���ڱ���UART ��������
//0xf4-f7 :��������LOGO �Ƿ���ʾ
//0xf8-fb :����ѡ������ͺ�

#define   EEP_UART_ADDR                0xF0       //EEPROM��д�����ֽ�
#define   EEP_MODEL_ADDR               0xF4       //
#define   EEP_MODEL_DIS_ADDR           0xF8       //
#define   EEP_LOGO_DIS_ADDR            0xFC       //


//���ٳ������ֽ����ڴ�� �絼��ϵͳУ׼����
#define    EEP_DDLV_SYSCAL1A            0x100      //ʹ��У׼���������
#define    EEP_DDLV_SYSCAL1B            0x104      //�洢�ڴˣ���10�������
#define    EEP_DDLV_SYSCAL1C            0x108      //

#define    EEP_DDLV_SYSCAL2A            0x110      //
#define    EEP_DDLV_SYSCAL2B            0x114    
#define    EEP_DDLV_SYSCAL2C            0x118    

#define    EEP_DDLV_SYSCAL3A            0x120      //
#define    EEP_DDLV_SYSCAL3B            0x124    
#define    EEP_DDLV_SYSCAL3C            0x128    

#define    EEP_DDLV_SYSCAL4A            0x130      //
#define    EEP_DDLV_SYSCAL4B            0x134    
#define    EEP_DDLV_SYSCAL4C            0x138    

#define    EEP_DDLV_SYSCAL5A            0x140      //
#define    EEP_DDLV_SYSCAL5B            0x144    
#define    EEP_DDLV_SYSCAL5C            0x148    

#define    EEP_DDLV_SYSCAL6A            0x150      //
#define    EEP_DDLV_SYSCAL6B            0x154    
#define    EEP_DDLV_SYSCAL6C            0x158    


#define    EEP_DDLV_CHECK1              0x160     
#define    EEP_DDLV_CHECK2              0x164 

 
  
//�û�У׼���ݴ洢 ���5������
#define    EEP_DDLV_USER_CAL1          0x170   
#define    EEP_DDLV_USER_CAL2          0x174   
#define    EEP_DDLV_USER_CAL3          0x178
#define    EEP_DDLV_USER_CHECK         0x17e

///////////////////////////////////////////////////////////////////////////////////
//������Ϣ���ڴ洢�û���������
//AT24C01 128Bytes   AT24C08 1024Bytes   AT24C64   8192Bytes   
//AT24C02 256Bytes   AT24C16 2048Bytes   AT24C128 16384Bytes
//AT24C04 512Bytes   AT24C32 4096Bytes   AT24C256 32768Bytes
//���㹫ʽ 100*16=1600BYTES ����ʹ��24C16 ҳд�������Ϊ16BYTE
//���ʹ��24C32������ ҳд�����󣬿��Կ����޸ĳ���

#define    EEP_BYTE_PER_REC           0x10     //�û�ÿ����¼ռ���ֽ���
#define    REC_MAX_NUM                 100     //���洢��¼����
#define    RECNUM_PER_PAGE              10     //ÿҳ��10����¼

#define    EEP_REC_TOTAL_ADDR        0x190     //��ż�¼����
#define    EEP_REC_TOTAL_ADDR_BAK    0x194     //��¼��������   
#define    EEP_REC_START_ADDR        0x1A0     //����û���¼��ʼ��ַ

///////////////////////////////////////////////////////////////////
 uint8 Read_EEPROM(uint16 addr,  uint8 *rptr, uint16 rlen);
 uint8 Read_EEPROM_1B(uint16 addr,  uint8 *r1ptr);
 uint8 Read_EEPROM_2B(uint16 addr, uint16 *r2ptr);
 uint8 Read_EEPROM_4B(uint16 addr, uint32 *r4ptr);
 uint8 Write_EEPROM(uint16 addr,  uint8 *wptr, uint16 wlen);
 uint8 Write_EEPROM_1B(uint16 addr,  uint8 w1b);
 uint8 Write_EEPROM_2B(uint16 addr, uint16 w2b);
 uint8 Write_EEPROM_4B(uint16 addr, uint32 w4b);
 uint8 Check_EEP_Wr_ERR(void);
 uint8 EEPROM_RW_Test(void);
#endif