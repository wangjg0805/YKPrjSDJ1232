/********************************************************************************
 *    CS5530.c: C file for CS5530
 *    Describtion: The C file is the definition of CS5530 for the LPC2478
 *                 family microprocessor.
********************************************************************************/
#include  "global.h"
#include  "lpc2400.h"
#include  "CS5530.h"

////////////////////////////////////////////////
//��ʱ����
///////////////////////////////////////////////
static void delay5530(uint32 iMs)
{
    uint8 i,cnt;
    for(cnt=0;cnt<iMs;cnt++)
    {
     for(i=0;i<100;i++)
     ;
     }
}
void cs5530_pin_init(void)
{
    //use pin0.20 21 22
    SCS      = SCS | 0x01 ;    //use FAST IO 
    
    PINSEL1 &= 0xffffc0ff;
    PINSEL1 |= 0x00000000; 
    
	FIO0DIR &= (~CS5530_SDO); 
	FIO0DIR |= ( CS5530_SDI | CS5530_CLK);
  }
////////////////////////////////////////////////
//��cs5530дһ���ֽ�
///////////////////////////////////////////////
static void cs5530_write(uint8 byte)
 {
    uint8 ctr;
    //CS5530_CS_H;
    CS5530_CLK_L;
    //CS5530_CS_L;
    for(ctr=0;ctr<8;ctr++) // output 8-bit
      {
       if(byte & 0x80)
        CS5530_SDI_H;
       else
        CS5530_SDI_L;
        byte=(byte<<1); // shift next bit into MSB..
       
        CS5530_CLK_H;
        delay5530(1);
        CS5530_CLK_L;          //CLK 0
      }
    CS5530_SDI_H;
 }

////////////////////////////////////////////////
//��cs5530��ȡһ���ֽ�
///////////////////////////////////////////////
uint8  cs5530_read(void)
  {
    uint8 ad_data,i;
    
    ad_data = 0;
    CS5530_SDI_H;
    CS5530_CLK_H;         
    for(i=0;i<8;i++)
	  {
	
		ad_data<<=1;     
		if(READ_CS5530_SDO)
		   ad_data++;              //��SDO��ȡ����
	    CS5530_CLK_H;              //ʱ�Ӹ�
	    delay5530(1);    
		CS5530_CLK_L;              //ʱ�ӵ� 
	  }
  	return (ad_data);
   }
   
   
uint32  cs5530_Read_Dword (void)
   {
    uint32 return_data;
    return_data = 0;
    
    cs5530_write(0x00);
    
    return_data  = cs5530_read();
    return_data<<=8;
    return_data += cs5530_read();
    return_data<<=8;
    return_data += cs5530_read();
    return_data<<=8;
    return_data += cs5530_read();  
      
    return_data = return_data>>9;
    return (return_data);
   }
       
   
void cs5530_Write_Dword(uint32 ld)      
  { 
    union type1
       {
        uint8  ch[4];
        uint32   lon;
       };
    union type1 temp;
   
    temp.lon = ld;
    cs5530_write(temp.ch[0]);
    cs5530_write(temp.ch[1]);
    cs5530_write(temp.ch[2]);
    cs5530_write(temp.ch[3]);
    
   }
       
static void cs5530_Com_Reset(void) 
{  
   uint8  i;
   for(i=0;i<18;i++)
   cs5530_write(SYNC1);
  
   cs5530_write(SYNC0);
}
////////////////////////////////////////////
////������0ʱ������ 1ʱ��˵��CS5530 ������       
//
/////////////////////////////////////////
uint8 cs5530_System_Reset(void)   //
  {
    uint8 i,j,k;
    
    cs5530_Com_Reset(); //��ʼ��
    
    cs5530_write(CMD_WRITE+REG_CONFIG);  //д���üĴ���
    cs5530_Write_Dword(SYSTEM_RESET) ;    //��ʼ�� 0x20000000
    delay5530(10);
    
    //����10�Σ������λ���ɹ�����Ϊ��CS5530����
    k = 0;
    do
    {
     k++;
     cs5530_write(CMD_READ+REG_CONFIG);//�����üĴ���
     i= cs5530_read();
     j= cs5530_read();
     j= cs5530_read();
     j= cs5530_read();
    }
    while(((i&0x10)!=0)||(k!=100));                   //ֱ����λΪ0
    
    if(k==100)
    return(1);
    else
    {//��������   
     cs5530_write(CMD_WRITE + REG_CONFIG); //���üĴ���
     cs5530_Write_Dword(NORMAL_MODE+VREF_HIGH+CR_A0_0+CR_A1_0+SHORT_INPUTS+LINE_FREQ_60+DATARATE_200+UNIPOLAR_MODE+TURN_OFF_300NA);  
      return(0);
    }
  }
       
void cs5530_start_one(void)  //3
    {
     cs5530_write(NORMAL_CONVERSION + START_SINGLE);    //����ת�� 
    }
void cs5530_start_con(void)  //3
    {
     cs5530_write(NORMAL_CONVERSION + START_CONTINUOUS);//����ת��
    }

