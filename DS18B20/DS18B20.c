#include  "global.h"
#include  "lpc2400.h"
#include  "DS18B20.h"
#include  "alarm_pro.h"

const float ds18b20_dot[] = 
      {0.0,0.1,0.1,0.2,0.3,0.3,0.4,0.4,
       0.5,0.6,0.6,0.7,0.8,0.8,0.9,0.9};
       
//////////////////////////////////////
//С��10US ���ô�	 
static void delay1us(uint32 delay_num)
	{
	  uint32 i;
	  do
	   {
	    i = 9;
        while(--i);	  
	   }
	  while (--delay_num);
	 }
//////////////////////////////
static void delay10us(void)
	{
	  uint32 i;
	  i = 95;
      while(--i);	  
    }
////////////////////////////////////////////////////////
static void delay15us(void)
	{
	  uint32 i;
	  i = 210;
      while(--i);	  
	 }
///////////////////////////////
static void delay100us(void)
	{
	 delay15us();
	 delay15us();
	 delay15us();
	 delay15us();
     delay15us();
     delay15us();
	 delay10us();
	 }
///////////////////////////////
static void delay500us(void)
	{
	 delay100us();
	 delay100us();
	 delay100us();
	 delay100us();
     delay100us();
  	 }
/////////////////////////////////////////////////////////// 
//��ʼ��DS18b20�����������壬�޴�����������λ�����־
//0:����   1������ ���߹���
//////////////////////////////////////////////////////////
uint32 RST_18B20(void)
 {
   uint32 i;
   
   FIO0DIR  |= DQ;
   
   FIO0CLR = DQ;      //��λ����      
   delay500us();
   FIO0SET = DQ;      //�ָ�
   delay15us();
   //delay15us();
   FIO0DIR  &= 0xfff7ffff;
   delay15us();
   //delay15us();
   if(0 == (FIO0PIN & DQ))
     i = 0;
     else
     i = 1;
    FIO0DIR |= DQ;    
    FIO0SET  = DQ;      //�ָ�
    delay100us();
    delay100us();
    delay100us();
    //delay100us();
    return(i);
  }
//////////////////////////////////
uint32 DS18B20_Init(void) 
 {
  PINSEL1  &= 0xffffff3f;
  PINSEL1  |= 0x00000000;
  
  return(RST_18B20());
 } 
 
////////////////////////////////READ 1BYTE
uint8 temp_byte_read(void)
 {
   uint8 i,j,dat;
   dat = 0;
   
   for(i=0;i<8;i++)
    {
      FIO0DIR |= DQ;
      FIO0SET  = DQ;          //�ָ�
      FIO0CLR = DQ;
      delay1us(5);
      FIO0SET  = DQ;         //��DS18B20�ӹ�
      FIO0DIR &= 0xfff7ffff; //p0.19
      delay1us(6);           //15us ����Ч
      if(DQ == (FIO0PIN & DQ)) //��14us����
       j = 1;
      else
       j = 0;  
      delay10us();
      delay15us();
      dat = (j<<7)|(dat>>1); 
    }
   return(dat);
 }

//дһ���ֽڵ�DS18B20��
void temp_byte_write(uint8 dat)   
{                           
  uint8 i;  
  uint8 testb;
  
  FIO0DIR |= DQ;
  
  for(i=0;i<8;i++)
  {
    testb = dat&0x01;
    dat   = dat>>1;
    FIO0CLR = DQ;
    delay1us(6);
    if(testb)              // д1����
      FIO0SET = DQ;
    else
      FIO0CLR = DQ;
    delay15us();
    delay10us();
    delay15us();
    //delay15us(); //>60us 
    FIO0SET = DQ;
    //delay1us(5); 
  }
}
///////////////////////////////////////
///////////////////////////////////////
void tmp_change(void)  //�����¶�ת������
{
  if(0 == RST_18B20())    //��ʼ��DS18B20
   {  //����
    temp_byte_write(0xcc);  // �������к�����
    temp_byte_write(0x44);  //�����¶�ת������ 
   }
}

////////////////////////////////////////////
////////////////////////////////////////////
void get_temp(void)               //����¶�
{
  uint16  a;
  if(0 == RST_18B20())
   {
    temp_byte_write(0xcc);   
    temp_byte_write(0xbe);          //���Ͷ�ȡ��������
    delay15us();
    a = temp_byte_read();           //�����������ֽ�����
    a = (temp_byte_read()<<8) | a;
    if(a & 0x8000)
    a = ~a + 1; 
    temp_float_ptc = ((a&0x07f0)>>4) + ds18b20_dot[a&0x0f];
   
   }
  else
   { 
   temp_float_ptc = 0.0;
   }

}