#include "global.h"
#include "config.h"
#include "ads1100.h"

static uint32   ad_sample_channel; 

//////////////////////////////////////////////
static void delay_us(unsigned int x)
{
   unsigned int a;
   for(a=0; a<x; a++)
	{
	;
	}
}
//////////////////////////////////////////
uint32  Get_AD_Channel(void)
{
  return(ad_sample_channel);
}

//**************************************************************************************/
//  ������     void Start_SPI(void)
//  �������   ��
//  �������   ��
/***************************************************************************************/
void  ADS1100Pin_Init(void)
	{

	 SCS = 	SCS | 0x01 ;    //use FAST IO 
	 
	 FIO0DIR |= ADS1100_CS | ADS1100_CLK | ADS1100_DOUT;
	 
	 FIO0DIR |= CD4052_POW_PIN | CD4052_A | CD4052_B | CD4053_A | CD4053_B | CD4053_C | CD4053_EN; 
   
	}

///////////////////////////////////////
//���´���ժ�����磬����Ժ�ʹ��

void start(void)       //��ʼ
{
   ADS1100_DOUT_H;
   delay_us(2);   
   ADS1100_CLK_H;
   delay_us(5);
   ADS1100_DOUT_L;
   delay_us(2);
   ADS1100_CLK_L;
   delay_us(2);
}

void stop(void)             //����
   { 
     ADS1100_DOUT_L; 
     delay_us(2);
     ADS1100_CLK_H;
     delay_us(5);
     ADS1100_DOUT_H;
     delay_us(2);
     ADS1100_CLK_L;
     delay_us(2);
    }
/////////////////////////////////////
uint32 i2cWaitAck(void)
    {
     uint8 ErrTime=255;
     ADS1100_DOUT_H;
     ADS1100_DATA_MODE_I;
     delay_us(2);
     ADS1100_CLK_H;
     
     while(ADS1100_DOUT_TEST)
     {
       ErrTime--;
       if(0==ErrTime)
         {
          stop();
          return(1);
         }
      }
     ADS1100_CLK_L;
     ADS1100_DATA_MODE_O;
     return(0);
    }    
///////////////////////////////////////////    
 void ack(void)          //  �����������ź�
{ 
    //TRISD0=0;
    //TRISD1=0;
    ADS1100_DOUT_L;
    delay_us(2);
    //ADS1100_CLK_L;
    ADS1100_CLK_H;
    delay_us(2);
    ADS1100_CLK_L;
    delay_us(2);
}
    
  void nack(void)        //  ���Ͳ��������ź�
  { 
    //TRISD0=0;
    //TRISD1=0;
    ADS1100_DOUT_H;
    delay_us(2);
    //ADS1100_CLK_L;
    ADS1100_CLK_H;
    delay_us(2);
    ADS1100_CLK_L;
    delay_us(2);
  }      
///////////////////////////////////////    
uint8  check(void)    //  ���Ӧ���ź�
     {  
        uint8 check;
        
        ADS1100_CLK_L;
        delay_us(1);  
        ADS1100_CLK_H;
   
        if (ADS1100_DOUT_TEST)
          {
            check = 1;
          }
        else
          {
            check = 0;
          }
         
         ADS1100_CLK_L;
         return(check);
     }
     
////////////////////////////////////////////////     
 void sendBYTE(uint8 send_data)   //  ����һ���ֽ�
    {
     uint8 bitcount=8;               //����8λ

        do
          { 
           
           if((send_data&0x80)==0x80)   
              {
               ADS1100_DOUT_H;        //д 1
              }
           else  
              {
               ADS1100_DOUT_L;        //д 0
              }
           ADS1100_CLK_L;       //��ʱ�Ӵ���4u���ڼ�д����
           delay_us(2);
           ADS1100_CLK_H;
           delay_us(2);
           ADS1100_CLK_L;
           send_data=send_data<<1;
           bitcount--;
          }while(bitcount);
          
          //TRISD1=1 ;                  //�ͷ����ߵȴ�Ӧ��
          delay_us(2);       
}   
////////////////////////////////////////////////
uint8 reciveBYTE(void)             //����һ���ֽ�
  {
    uint8  temp1    = 0;
    uint8  bitcount = 8;
    ADS1100_DOUT_H;
     
    ADS1100_DATA_MODE_I;
    //TRISD0=0;
    //TRISD1=1;               //������
    do
    {
     ADS1100_CLK_L;           //��ʱ�Ӵ���4u���ڼ������
     delay_us(2);
     ADS1100_CLK_H;
     delay_us(2);
  
     if(ADS1100_DOUT_TEST)    //�� 1
       { 
        temp1=temp1|0x01;    
       }
     else                      //�� 0
       {
        temp1=temp1&0xfe;
       }
      ADS1100_CLK_L;
      
      if(bitcount-1)
      {
         temp1=temp1<<1;
         
      }
       bitcount--;  
      }while(bitcount);
      
      ADS1100_DATA_MODE_O;
     
      return(temp1);
  }      
///////////////////////////////////////////////////   
void wrtoIIC_Cfg(uint8 cmd)     //д������ַ��������

  { 

    
     start();                  //��ʼд
     sendBYTE(0x90);           //д������ַ��д��
         
     do{
        ;
        }while(check());       //�ȴ�Ӧ��
        
     sendBYTE(cmd);            //д������
     do{;
        }while(check());
        
     stop();                   //����д
        
}     
///////////////////////////////////////////////////////
uint32 rdfromIIC(void)         //��ADS1100�ж�������
   { 
     uint32 tmp = 0; 
     
     start();                  //��ʼ��
     sendBYTE(0x91);           //д������ַ������
     do{
        ;
       }while(check());        //�ȴ�Ӧ��
        
      tmp = reciveBYTE();      // ����8λ
      ack();                   //������
      tmp = ((tmp<<8)&0xff00) + reciveBYTE();     //����8λ
      //ack();                 //������
      //tmp = ((tmp<<8)&0xff00) + reciveBYTE();   //��������
      nack();
      stop();                  //������
      //delay();
      return(tmp);
   } 
   
//////////////////////////////////
void ADS1100_Init(void)
{
    //wrtoIIC_Cfg(0x8c);
    wrtoIIC_Cfg(0x83);

}   