//cs5530.h
#ifndef __CS5530_h__
#define __CS5530_h__

//����ܽż��䶯��

#define CS5530_PIN    FIO0PIN

//#define CS5530_CS    (1<<16)

#define CS5530_SDI   (1<<20) //PIN120-PIN13
#define CS5530_CLK   (1<<21) //PIN118-PIN11
#define CS5530_SDO   (1<<22) //PIN116-PIN12


//#define CS5530_CS_H   FIO4SET=CS5530_CS
//#define CS5530_CS_L   FIO4CLR=CS5530_CS

#define CS5530_CLK_H  FIO0SET=CS5530_CLK
#define CS5530_CLK_L  FIO0CLR=CS5530_CLK

#define CS5530_SDI_H  FIO0SET=CS5530_SDI
#define CS5530_SDI_L  FIO0CLR=CS5530_SDI

#define READ_CS5530_SDO  (CS5530_PIN&CS5530_SDO)

////////////////////////////////////CS5530������
#define  NULL_BYTE           0x00
#define  SYNC1               0xff
#define  SYNC0               0xfe
// Command Register
#define  CMD_WRITE           (0x00<<3)//д�Ĵ�������
#define  CMD_READ            (0x01<<3)//���Ĵ�������

#define  REG_OFFSET          0x01   //ƫ�ƼĴ���
#define  REG_GAIN            0x02   //����Ĵ���
#define  REG_CONFIG          0x03   //���üĴ���

#define  START_SINGLE        (0x00<<6) //����ת��
#define  START_CONTINUOUS    (0x01<<6)//����ת��

#define  NORMAL_CONVERSION   (0x80+0x00)//ת������ 
#define  SYSTEM_OFFSET_CAL   (0x80+0x05)//ִ��ƫ��У׼
#define  SYSTEM_GAIN_CAL     (0x80+0x06)// ִ������У׼
// Configuration Register
#define  STANDBY_MODE        (0x00L<<31)//����ģʽ
#define  SLEEP_MODE          (0x01L<<31)//����ģʽ
#define  POWER_SAVE_MODE     (0x01L<<30)  //�ڵ�ģʽ

#define  NORMAL_MODE         (0x00L<<29)    //��������
#define  SYSTEM_RESET        (0x01L<<29)//����һ����λ���� ��λ���Զ���0

#define  RESET_STATUS        (0x01L<<28)   //��λ��Ч ϵͳ�Ѹ�λ ֻ��

#define  SHORT_INPUTS        (0x00L<<27)   //������������ ��ͨ�����벻�Ͻ�

#define  VREF_HIGH           (0x00L<<25)  //�ο���ѹ 2.5~((VA+)-(VA-))V
#define  VREF_LOW            (0x01L<<25)//�ο���ѹ 1~2.5V

#define  CR_A1_0             (0x00L<<24)
#define  CR_A1_1             (0x01L<<24)

#define  CR_A0_0             (0x00L<<23)
#define  CR_A0_1             (0x01L<<23)

#define  LINE_FREQ_60        (0x00L<<19)  //�˲�����ѡ�� ȱʡ������
#define  LINE_FREQ_50        (0x01L<<19)  //��������ʼ���Ӧ�˲������Գ���ϵ��5/6

#define  DATARATE_100        (0x00L<<11)   //������ ��ͬλ�Ͳ�ͬ
#define  DATARATE_50         (0x01L<<11)
#define  DATARATE_25         (0x02L<<11)
#define  DATARATE_12P5       (0x03L<<11)
#define  DATARATE_6P25       (0x04L<<11)
#define  DATARATE_3200       (0x08L<<11)
#define  DATARATE_1600       (0x09L<<11)
#define  DATARATE_800        (0x0aL<<11)
#define  DATARATE_400        (0x0bL<<11)
#define  DATARATE_200        (0x0cL<<11)

#define  BIPOLAR_MODE        (0x00L<<10)  //����ѡ��˫����ģʽ
#define  UNIPOLAR_MODE       (0x01L<<10)//����ѡ�񵥼���ģʽ

#define  TURN_OFF_300NA      (0x00L<<9)  //���������Դ
#define  TURN_ON_300NA       (0x01L<<9)  //�������Դ300ma

void    cs5530_pin_init(void);
uint8   cs5530_System_Reset(void);
void    cs5530_start_one(void);
void    cs5530_start_con(void);

#endif