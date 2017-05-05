////ע��Target.c    while((PLLSTAT & (1 << 10)) == 0);
//**********************************************************************
//�ļ�   ��:
//˵       ��:CPU�ͺ�--LPC2138
//�汾   ��:
//�޸���ʷ:
//**********************************************************************
#include "port.h"
#include "FontLib.h"
#include "disp.h"
#include "lcd2478.h"
#include "graph.h" 
#include "global.h"
#include "timer.h"
#include "scr_cal.h"
#include "service.h"
#include "normal_operation.h"
#include "ad_acquire.h"
#include "i2c_com.h"
#include "filter.h"
#include "speaker.h"
#include "type_def.h"

#include "config.h"
#include "stdio.h"
///////////////////////////////////////USB HEADFILE
#include  "usbcfg.h"

///////////////////////////////////////
uint8 Start_Menu_Pro(void);

#define SDRAM_ADDR	  0xA0000000 
#define SDRAM_SIZE     0x2000000	    //32M bytes=256Mbits

sint32 main(void)
{
            uint32  n;    
   volatile uint32 i,err;
   volatile uint16 *piAddr1;
    
    if (WDMOD&0x04) WDMOD &= ~0x04;//���Ź���λ
     //////////////////////////////
    ///////////////////////////////test
       
    //SCS = 	SCS | 0x01 ;    //use FAST IO  
    //PINSEL3  = 0x00000000;       		            // �趨P3�ڵ����ӷ�ʽ
    //FIO1DIR   = LED_RD|LED_WR|LED_CFG|LED_SUSP;     //////p1.14 15 16 17
    //FIO1SET   =	LED_RD|LED_WR|LED_CFG|LED_SUSP;
	//FIO3DIR &= ~(KEY0|KEY1|KEY2|KEY3);  ///////////	
	/////////////
    //USB_Init();
    //USB_Connect(1);         // USB Connect
    /*while(1){
     //	 if(0 == (FIO3PIN&KEY0))
         {
     	  IO1CLR = LED0;
     	  IO1SET = LED1|LED2|LED3;
     	  }
     	  if(0 == (FIO3PIN&KEY1))
         {
     	  IO1CLR = LED1;
     	  IO1SET = LED0|LED2|LED3;
     	  }
     	  if(0 == (FIO3PIN&KEY2))
         {
     	  IO1CLR = LED2;
     	  IO1SET = LED0|LED1|LED3;
     	  }
     	  if(0 == (FIO3PIN&KEY3))
         {
     	  IO1CLR = LED3;
     	  IO1SET = LED1|LED2|LED0;
     	  }
     	sysDelayNS(500);
       }*/
   //////////////////////////////////test end  */
    Init_PWM0_DRIVER();    //GP21 use
   /////////////////////////
    Init_Cilib();
   	Init_Graph();
   
    Init_Port();          //��ʼ��IO��
    #if USB_ENABLE
    USB_Init();
    USB_Connect(1);         // USB Connect
    #endif
    //////////////////////////���������ڲ˵��н���
    Init_Brightness_PWM();
    ////////////////////////////////////// 
      
   	///////////////////////
    ADS7843PIN_Init();
    ///////////////////////
    //Set_Language_Type(ENGLISH);
    //Set_Language_Type(CHINESE);
    RTCInit();
    ///////////////////////
    Init_Timer0(200);     //����200us��ʱ
    Init_I2C(250000);     //��ʼ��I2C����
    Init_Timer1();        //��ʼ����ʱ��1�����ж����ŵ�����������
    UART_Init(9600);
    //gp21Configuration();  //
    //GP21PIN_Init();
	//GP21_init();
    //Init_EINT2();
    //epm7064s Configuration();  //
    Epm7064sPIN_Init();
    filter_init();
	Init_EINT2();
    /////////////////////////////////����������ʾ
    Sleep_process();  
    ////////////////////////////////////////////////
    machine_mode =  SystemInit();    //�������洦��
    if(MACHINE_SERVICE_MODE == machine_mode)
    Start_Service_process();  
    else
    Normal_Operation();
   
    while (1)
    {;}
}
