//**********************************************************************
//�ļ���:speaker.c
//�޸���ʷ��
//**********************************************************************
#include "config.h"
#include "speaker.h"
#include "global.h"

//�����������ڵ���ƥ���
const uint8 VOLUME_TABLE[16]={1,2,3,4,5,6,7,8,9,10,11,13,16,20,25,31};


static volatile sint16  time_key_sound;      //��������ʱ��
static           uint8  key_sound_type   = SOUND_TYPE_NONE;    //������������

static volatile sint16  time_alarm_sound;    //��������ʱ��
static           uint8  alarm_sound_type = SOUND_TYPE_NONE;    //������������

/****************************************************************************
* ������: X9313�ܽ� ��ʼ��
****************************************************************************/
void X9313_Pin_Init(void)
{
    //����P0   PIN0.161718 19202122    23242526  2728  293031 
    //                            //P0.23 24 25                  
    PINSEL1 &= 0xfff03fff;      
    PINSEL1 |= 0x00000000;
       
    //ȫ��Ϊ���
    //FIO0DIR &= 0xff8fffff;
    FIO0DIR |= (SPK_X9313_CS+SPK_X9313_INC+SPK_X9313_UD);
}

/****************************************************************************
* ���ƣ�Set_Config_Bri(uint16 Brightness)
* ���ܣ������û����õ�����
* ��ڲ��������ȵȼ�����8����
* ���ڲ���
****************************************************************************/
void Set_LCD_Brightness(uint8 brightness)
{
	//PWM0MR0 = Fpclk/1000 ; //���ȵ�������:duty from 5%-40%
    if((brightness>0) && (brightness<=BRIGHT_CLASS_TOTAL))     
      PWM0MR5 = (PWM0MR0 * brightness)/20;  //20�ȷ�
    else
      PWM0MR5 =  PWM0MR0/2;
    
    //PWM0LER = 0x21;            // PWMMR0, PWMMR5����
    PWM0LER = 0x2d;
	}
/*********************************************************************************************************
* ����: Set_LCD_Brightness
* ����: ����LCD��������(PWM��ʽ����)
* ��ڲ���: ����(0~15)
* ���ڲ���: ��
*********************************************************************************************************/
void  Set_Default_Bri(void)   //ռ�ձȴ���50%���ȵ����Ѿ�������
 	{                         //�˿�Һ����PWM����Χ��200HZ-30kHZ,�˴�ȡ1KHZ
 	                          //ֻ���ڵ�������
 	PWM0MR5 = PWM0MR0/2 ;     // ����Ĭ��PWMռ�ձ�50%
 	PWM0MR2 = PWM0MR0*8/10;   // �������������ź�1ռ�ձ�
 	PWM0MR3 = PWM0MR0*8/10;   // �������������ź�2ռ�ձ�
    //PWM0LER = 0x21;            //PWM0MR5
    PWM0LER = 0x2d; 
	}
/****************************************************************************
* ���ƣ�Init_BRIGHT_PWM()
* ���ܣ���ʼ������PWM
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Init_Brightness_PWM(void)
{
                                 // ����PWM0[5]���, ����PWM
    PCONP   |=     1 << 5 ;
    PWM0TCR = 0x02;              //reset
    PWM0PR = 0x00;               // ����Ƶ, ����Ƶ��ΪFpclk
    PWM0MCR = 0x02;              // ����PWM0MR5ƥ��ʱ��λPWMTC
    
    PWM0PCR = 0;
    PWM0MR0 = Fpclk/200 ;        // ����PWMƵ��200HZ
    Set_Default_Bri();           // ����
  
    PWM0PCR = 0x2C00;            // ����PWM5 PWM2PWM3���, ����PWM
    PWM0TCR = 0x09;            // ������ʱ����PWMʹ��
}

/****************************************************************************
* ���ƣ�Init_BRIGHT_PWM()
* ���ܣ���ʼ������PWM
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Init_PWM0_DRIVER(void)
{
    PCONP   |=     1 << 5 ;
   	PWM0TCR = 0x02;			                                            /* PWM1�ļ�������λ             */
	PWM0PR 	= 0x00;			                                            /* ����Ƶ,PWM1��������          */
	                                                                    /* ����Ƶ�ʼ�Fpclk              */
	PWM0MCR = 0x02;			                                            /* ���������ֵ��ƥ��Ĵ�����   */
	                                                                    /* ��ƥ��ֵ���ʱ��λ           */
		                                            /* ����PWM1���Ϊ������PWM      */
    PWM0MR0 = Fpclk / 1350;		/* 1.3500000kHZ                        */
    PWM0MR1 = PWM0MR0 / 2;	    /* ����PWM0[1]���Ϊ50%ռ�ձ�      */
	PWM0MR2 = PWM0MR0-0x33b0;   //pwm0[2] ռ�ձ��ǿ��Ե��ڵ�  	                                            /* ����PWM1���Ϊ������PWM      */
    
    //PWM0MR0 = Fpclk / 1349;												/* 500HZ                        */
 	//PWM0MR1 = PWM0MR0 / 2;	    /* ����PWM0[1]���Ϊ50%ռ�ձ�      */
	//PWM0MR2 = 0x33B0;////////////////////////////(PWM0MR0 / 100)*99;      
	      
	
	PWM0LER = 0x07;			                                            /* PWM0ƥ��MR0 MR1 MR2  */
	PWM0PCR = 0x0600; 		                                            /* PWM0���ʹ��                 */
   	PWM0TCR = 0x09;			                                            /* PWM�ͼ�������ʹ��            */	
   	
}
///////////////////////////////����Ϊ����PWM�������

/****************************************************************************
* ���ƣ�Start_Sound_PWM()
* ���ܣ���������PWM
* ��ڲ�����frq ����Ƶ��(Hz)
* ���ڲ�������
****************************************************************************/
void Start_Sound_PWM(uint16 frq)
{    //PWM1[4]
    
    PWM1MR0 = Fpclk / frq;    // ����PWM����(����Ƶ��)
    PWM1MR4 = PWM1MR0 / 2;    // ����PWMռ�ձ�50%
    PWM1LER = 0x11;            // PWMMR0, PWMMR2����   
}
/****************************************************************************
* ���ƣ�Stop_Sound_PWM()
* ���ܣ�ֹͣ����PWM
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Stop_Sound_PWM(void)
{
    
    PWM1MR0 = Fpclk / 65536;
    PWM1MR4 = 0;        // ʹPWM1һֱ����͵�ƽ
    PWM1LER = 0x11;    // PWMMR0, PWMMR1����  
}
/****************************************************************************
* ���ƣ�Init_Sound_PWM()
* ���ܣ���ʼ������PWM
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Init_Sound_PWM(void)
{    
    //use p3[27]
    //����P3[27]
    PINSEL7 &= 0xff3fffff;      
    PINSEL7 |= 0x00c00000;
       
    //ʹ��PWM1[1]
    PCONP   |=     1 << 6 ;
    
    PWM1TCR = 0x02;          //reset
    PWM1PR  = 0x00;          // ����Ƶ, ����Ƶ��ΪFpclk
    PWM1MCR = 0x02;          // ����PWM1MR0ƥ��ʱ��λPWMTC
    
    PWM1PCR = 0;
    Stop_Sound_PWM();        // ֹͣ����PWM
    PWM1PCR = 0x1000;         // ����PWM4���, ����PWM
    PWM1TCR = 0x09;          // ������ʱ����PWMʹ��
    
    //X9313_Pin_Init();
 
}
/****************************************************************************
* ������: Key_Valid_Sound()
* ����  : ������Чʱ���������Ĵ���
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Key_Valid_Sound(void)
{
 
    if (OFF == sound_switch)    return;       //��������
    time_key_sound = 10;    //�ܹ�200ms
    key_sound_type = SOUND_TYPE_KEY_VALID;        //���ð�����������

}

/****************************************************************************
* ������: Key_No_Valid_Sound()
* ����  : ������Чʱ���������Ĵ���
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Key_No_Valid_Sound(void)
{
  
    if (OFF == sound_switch)    return;      //��������
    time_key_sound = 40;    //�ܹ�400ms
    key_sound_type = SOUND_TYPE_KEY_NO_VALID;    //���ð�����������
  
}

/****************************************************************************
* ������: Key_Sound_Pro()
* ����  : �������Ĵ���(���붨ʱ���ж���,ÿ10ms����һ��)
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Key_Sound_Pro(void)
{
     switch(key_sound_type)
        {
         case SOUND_TYPE_KEY_VALID:    //������Чʱ��������
              Speaker_ON();
              break;
         case SOUND_TYPE_KEY_NO_VALID: //������Чʱ���������Ĵ���
         
              Speaker_ON();
              break; 
         case SOUND_TYPE_SER_ON:    //ά��ģʽ��һֱ��������
              Speaker_ON();
              break;    
         default:   //�޼��������˳�
            return;
        }
        
     if(time_key_sound == 0)
       {  
        Speaker_OFF();                     //ֹͣ����
        key_sound_type = SOUND_TYPE_NONE;  //������������
       }
    else
       {
        time_key_sound--;        //ÿ10ms��1.
       }   
}

///////////////
void Key_Sound_Pro_TP(void)
{
     switch(key_sound_type)
        {
         case SOUND_TYPE_KEY_VALID:    //������Чʱ��������
            if(time_key_sound > 10) Start_Sound_PWM(300);    //��420Hz����"3"
            else Start_Sound_PWM(200);                        //��333Hz����"1"
            break;
         case SOUND_TYPE_KEY_NO_VALID: //������Чʱ���������Ĵ���
            if(time_key_sound > 27) Start_Sound_PWM(500);    //��500Hz����"5"
            else if(time_key_sound > 13) Stop_Sound_PWM();    //ֹͣ����
            else Start_Sound_PWM(500);
            break; 
         case SOUND_TYPE_SER_ON:    //ά��ģʽ��һֱ��������
            time_key_sound = 2;
            Start_Sound_PWM(333);
            break;    
         default://�޼��������˳�
            return;
        }
        
     if(time_key_sound == 0)
       {
        Stop_Sound_PWM();                  //ֹͣ����
        key_sound_type = SOUND_TYPE_NONE;  //������������
       }
    else
       {
        time_key_sound--;        //ÿ10ms��1.
       }   
}

/****************************************************************************
* ���ƣ�Alarm_Sound_Pro()
* ���ܣ������ķ���������(���붨ʱ���ж���,ÿ10ms����һ��)
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Alarm_Sound_Pro(void)
{
    if(SOUND_TYPE_NONE != key_sound_type) return;        //�а��������˳�����������
   
     switch(alarm_sound_type)
     {
        case SOUND_TYPE_WARNING://������������(10��,����10��,3��2��,Ƶ��666Hz)
            if(time_alarm_sound < 16) Start_Sound_PWM(666);            //��160ms
            else if(time_alarm_sound < 24) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 40) Start_Sound_PWM(666);    //��160ms
            else if(time_alarm_sound < 48) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 64) Start_Sound_PWM(666);    //��160ms
            else if(time_alarm_sound < 88) Stop_Sound_PWM();        //ͣ240ms
            else if(time_alarm_sound < 104) Start_Sound_PWM(666);    //��160ms
            else if(time_alarm_sound < 112) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 128) Start_Sound_PWM(666);    //��160ms
            else if(time_alarm_sound < 208) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 224) Start_Sound_PWM(666);    //��160ms
            else if(time_alarm_sound < 232) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 248) Start_Sound_PWM(666);    //��160ms
            else if(time_alarm_sound < 256) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 272) Start_Sound_PWM(666);    //��160ms
            else if(time_alarm_sound < 296) Stop_Sound_PWM();        //ͣ240ms
            else if(time_alarm_sound < 312) Start_Sound_PWM(666);    //��160ms
            else if(time_alarm_sound < 320) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 336) Start_Sound_PWM(666);    //��160ms
            else Stop_Sound_PWM();
            if(++time_alarm_sound >= 1000) time_alarm_sound = 0;    //10��ѭ��
            break;
        case SOUND_TYPE_TIPS://����ʾ�౨����(10��,����10��,3��2��,Ƶ��333Hz)
            if(time_alarm_sound < 16) Start_Sound_PWM(333);            //��160ms
            else if(time_alarm_sound < 24) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 40) Start_Sound_PWM(333);    //��160ms
            else if(time_alarm_sound < 48) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 64) Start_Sound_PWM(333);    //��160ms
            else if(time_alarm_sound < 88) Stop_Sound_PWM();        //ͣ240ms
            else if(time_alarm_sound < 104) Start_Sound_PWM(333);    //��160ms
            else if(time_alarm_sound < 112) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 128) Start_Sound_PWM(333);    //��160ms
            else if(time_alarm_sound < 208) Stop_Sound_PWM();        //ͣ800ms
            else if(time_alarm_sound < 224) Start_Sound_PWM(333);    //��160ms
            else if(time_alarm_sound < 232) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 248) Start_Sound_PWM(333);    //��160ms
            else if(time_alarm_sound < 256) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 272) Start_Sound_PWM(333);    //��160ms
            else if(time_alarm_sound < 296) Stop_Sound_PWM();        //ͣ240ms
            else if(time_alarm_sound < 312) Start_Sound_PWM(333);    //��160ms
            else if(time_alarm_sound < 320) Stop_Sound_PWM();        //ͣ80ms
            else if(time_alarm_sound < 336) Start_Sound_PWM(333);    //��160ms
            else Stop_Sound_PWM();
            if(++time_alarm_sound >= 1000) time_alarm_sound = 0;    //10��ѭ��
            break;
      
        default://�ޱ�������
            Stop_Sound_PWM();        //ֹͣ����
            break;
    }
}

//**********************************************************************
//����  ��:Get_Key_Sound_Type
//��������:��ȡ��ǰ�İ�������������
//��ڲ���:��
//���ڲ���:����������
//**********************************************************************
uint8 Get_Key_Sound_Type(void)
{
    return (key_sound_type);
}

//**********************************************************************
//����  ��:Set_Key_Sound_Type
//��������:���ð���������
//��ڲ���:s_type--��������
//���ڲ���:��
//**********************************************************************
void Set_Key_Sound_Type(uint8 s_type)
{
    key_sound_type = s_type;
}

//**********************************************************************
//����  ��:Set_Key_Sound_Time
//��������:���ð�������ʱ��
//��ڲ�����t--ʱ��
//���ڲ���:��
//**********************************************************************
void Set_Key_Sound_Time(sint16 t)
{
    time_key_sound = t;
}

//**********************************************************************
//����  ��:Get_Alm_Sound_Type
//��������:��ȡ��ǰ�ı�������������
//��ڲ���:��
//���ڲ���:����������
//**********************************************************************
uint8 Get_Alm_Sound_Type(void)
{
    return(alarm_sound_type);
}

//**********************************************************************
//����  ��:Set_Alm_Sound_Type
//��������:���ñ���������
//��ڲ���:s_type--��������
//���ڲ���:��
//**********************************************************************
void Set_Alm_Sound_Type(uint8 s_type)
{
    alarm_sound_type = s_type;
    time_alarm_sound = 0;       //���¿�ʼ
}

/////////////////////////////////////////////////////////////////
//���º���ʽ��X9313�Ĳ���
//���ڵ�������

/****************************************************************************
* ������: Inc_Speaker_Volume
* ����: ������������
* ��ڲ���: ��������
* ���ڲ���: ��
****************************************************************************/
void Inc_Speaker_Volume(uint8 step)
{
	Set_Ext_OutPort1(SPK_X9313_UD);		//��������
	Clr_Ext_OutPort1(SPK_X9313_CS);
	while(step--)
	{
		Clr_Ext_OutPort1(SPK_X9313_INC);
		Clr_Ext_OutPort1(SPK_X9313_INC);
		Set_Ext_OutPort1(SPK_X9313_INC);
		Set_Ext_OutPort1(SPK_X9313_INC);
	}
	Set_Ext_OutPort1(SPK_X9313_CS);
}

/****************************************************************************
* ������: Dec_Speaker_Volume
* ����: ��С��������
* ��ڲ���: ��������
* ���ڲ���: ��
****************************************************************************/
void Dec_Speaker_Volume(uint8 step)
{
	Clr_Ext_OutPort1(SPK_X9313_UD);		//������С
	Clr_Ext_OutPort1(SPK_X9313_CS);
	while(step--)
	{
		Clr_Ext_OutPort1(SPK_X9313_INC);
		Clr_Ext_OutPort1(SPK_X9313_INC);
		Set_Ext_OutPort1(SPK_X9313_INC);
		Set_Ext_OutPort1(SPK_X9313_INC);
	}
	Set_Ext_OutPort1(SPK_X9313_CS);
}

/****************************************************************************
* ������: Clr_Speaker_Volume
* ����: �����������
* ��ڲ���: ��
* ���ڲ���: ��
****************************************************************************/
void Clr_Speaker_Volume(void)
{
	Dec_Speaker_Volume(32);		//��9313��ͷ�Ƶ�0
}

/****************************************************************************
* ������: Set_Speaker_Volume
* ����: ���ñ�������
* ��ڲ���: ����(1~16��)
* ���ڲ���: ��
****************************************************************************/
void Set_Speaker_Volume(uint8 volume)
{
	/*//�Ƚ�9313��ͷ�Ƶ�0
	Clr_Speaker_Volume();
	//���Ƶ���Ҫ��λ��
	if(volume<1) 
	   volume = 1;
	else if(volume>SPEAK_CLASS_TOTAL) 
	        volume = SPEAK_CLASS_TOTAL;
	Inc_Speaker_Volume(VOLUME_TABLE[volume-1]);*/
}

/****************************************************************************
* ������: Calc_Sound_Change_Step
* ����: ���ݵ�ǰ�������㽥�䲽��(��9��)
* ��ڲ���: ��ǰ�����ͽ��䲽������
* ���ڲ���: ��
****************************************************************************/
void Calc_Sound_Change_Step(uint8 volume, uint8 *scsarray)
 {
 /*
  uint8 res,soundstep[10];
  uint32 i;
	if(volume<1)
	   volume = 1;
	else if(volume>SPEAK_CLASS_TOTAL) 
	        volume = SPEAK_CLASS_TOTAL;
	res = VOLUME_TABLE[volume-1];
	for(i=0; i<10; i++)
	{
		soundstep[i] = (res * i + 4) / 9;
		if(i>0)
		scsarray[i-1] = soundstep[i] - soundstep[i-1];
	}
 */	
 }
