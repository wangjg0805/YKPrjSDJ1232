//**************************************************************************************/
//  �ļ���   AD7843.C
//  ����     ��ADS7843���ж�д�������ṩ��
//  ����ӿڣ�1��������������
//          ��2�������˲�����
//          ��3��������Ļ2���Խǵ�
//          ��4��������ʱ
//  ����ӿڣ�1�����X Y �����AD������ֵ
//          ��2����������Ļ��X Y��ֵ
//  ���ߣ�   ������
//  �汾�ţ� V1.0
//  ���ڣ�   2010.09.25
/***************************************************************************************/
#include  "global.h"
#include  "LCD2478.h"
#include  "ad7843.h"

/***************************************************************************************/

 INT32U      x0,y0,xmid,ymid,xmax,ymax;                      //�û�ԭ�����������

//**************************************************************************************/
//  ������     void Start_SPI(void)
//  �������   ��
//  �������   ��
/***************************************************************************************/
void  ADS7843PIN_Init(void)
	{

	SCS = 	SCS | 0x01 ;    //use FAST IO  
	IO_AD7843_IRQDIR  &=  (~AD7843_IRQ) ; 
	IO_AD7843_DIR  |=   AD7843_CLK | AD7843_CS | AD7843_DIN ;
	IO_AD7843_DIR  &=  (~AD7843_BUSY) & (~AD7843_DOUT) ;

	}

/***************************************************************************************/
//  ������     void Start_SPI(void)
//  �������   ��
//  �������   ��
/***************************************************************************************/
void Start_SPI(void) //SPI��ʼ
	{
	IO_AD7843_CLR = AD7843_CLK;
    DlyMs(DLYCONST);
    IO_AD7843_SET = AD7843_CS;
    DlyMs(DLYCONST);
    IO_AD7843_CLR = AD7843_DIN;
    DlyMs(DLYCONST);
    IO_AD7843_CLR = AD7843_CLK;
    DlyMs(DLYCONST);
    IO_AD7843_CLR = AD7843_CS;
    DlyMs(DLYCONST); 
    }
/***************************************************************************************/
//  ������     void WriteCharTo7843(INT08U command)
//  �������   ���ֽ�����
//  �������   ��
/***************************************************************************************/
void WriteCharTo7843(INT08U command) 
    {
     INT08U count=0;
     IO_AD7843_CLR = AD7843_CLK;
     DlyMs(DLYCONST);
     for(count=0;count<5;count++)
	  {
	  	  
	  IO_AD7843_CLR = AD7843_DIN;
	  if(command & BIT7)
	  IO_AD7843_SET = AD7843_DIN;
	  
	  DlyMs(DLYCONST);
	  IO_AD7843_SET = AD7843_CLK; 
	  DlyMs(DLYCONST);
	  IO_AD7843_CLR = AD7843_CLK;
	  command<<=1;
	  }
	 for(count=0;count<3;count++)
	  {
	  	  
	  IO_AD7843_CLR = AD7843_DIN;
	  if(command & BIT7)
	  IO_AD7843_SET = AD7843_DIN;
	  
	  DlyMs(DLYCONST);
	  IO_AD7843_SET = AD7843_CLK; 
	  DlyMs(DLYCONST);
	  IO_AD7843_CLR = AD7843_CLK;
	  command<<=1;
	  }
    }
/***************************************************************************************/
//  ������     UINT16U ReadFromCharFrom7843(void)
//  �������   ��
//  �������   ���ز�����X Y ��ֵ����������ת��ģʽ������
//  ��ע      (1) �����ڲ����ò�ѯ��ʽ�õ��������ݣ��粻��Ҫ�����ţ���ʹ�ö�ʱ��������ʽ
/***************************************************************************************/
INT16U  ReadFromCharFrom7843(void)              //SPI ������
	{
     INT08U count=0;
	 INT16U Num=0;
	 
	 IO_AD7843_SET = AD7843_CLK;
	 DlyMs(DLYCONST); 
     IO_AD7843_CLR = AD7843_CLK;
     
  //	 while(IO_AD7843_PIN & AD7843_BUSY );      //WAITING FOR CONVERT FINISH
  	 
  	 for(count=0;count<12;count++)
	    {
   	     Num<<=1;
	     IO_AD7843_CLR = AD7843_CLK; 
		 DlyMs(DLYCONST);
	     IO_AD7843_SET = AD7843_CLK;
		 DlyMs(DLYCONST); 
	     if(IO_AD7843_PIN & AD7843_DOUT) 
	        Num |= BIT0;
 	     }
      return (Num);
	}

/***************************************************************************************/
//  ������     void DlyMs(INT16U m)
//  �������   INT16U����ʱ��ֵ(���ݲ�ͬӲ����������)
//  �������   ��
/***************************************************************************************/
void DlyMs(INT32U m)        // adjustable
{
  do
  {
  ;
  }while(m--);
}

/***************************************************************************************/
//  ������     Get_PositionAD(void)
//  �������   ָ����������ڴ洢AD��ֵ:��16λ��X;��16λ��Y 
//  �������   λָʾ��ǰ��ֵ�Ƿ���Ч
/***************************************************************************************/
BOOLEAN  Get_PositionAD(INT32U *PositionAD)
	{  
     BOOLEAN valid;
     INT32U i;
     INT32U XX,YY,X,Y;
     static INT32U key_data,key_ok_flag,key_press_cnt;
     /////////////////////////////////////////
     XX = 0;
     YY = 0;
     /////////////////////////////////////////
	 Start_SPI();
	 if(0 == (IO_AD7843_IRQPIN&AD7843_IRQ))
	   {//�м�����
	    if((KEY_DO_OVER+KEY_RELEASE) == Key_Status) //�ոհ���
	      {   
	       Key_Status = KEY_PRESS;
	       *PositionAD = 0;
	       key_ok_flag = FALSE;
	       valid = ERROR;      //������Ч����
	       key_press_cnt = 0;  //��ʼ���㰴��ʱ��
	       no_key_time = 0;    //�а������¼���
	      }
	     else if(KEY_PRESS == Key_Status)
	      {//���³���100ms �ſ�ʼ�ɼ�����
	        if(++key_press_cnt > 1)
	        {
	         ///////////////////////////////////////read keydata
	         for(i = 0;i < SAMPLE_TIMES;i++)       //�˲��������ã�ȡ���SAMPLE_TIMES�β�����ֵ��
 	          {                                   //������ACCP_VALUE
 	           WriteCharTo7843(STARTBIT|MX|CONVER12BIT|SINGL_END|PD_ENABLE); //X����
               XX += ReadFromCharFrom7843();
        
               WriteCharTo7843(STARTBIT|MY|CONVER12BIT|SINGL_END|PD_ENABLE); //Y
               YY += ReadFromCharFrom7843();
               }
	        WriteCharTo7843(STARTBIT|MX|CONVER12BIT|SINGL_END|PD_ENABLE); //X����
	        X = ReadFromCharFrom7843();
            WriteCharTo7843(STARTBIT|MY|CONVER12BIT|SINGL_END|PD_ENABLE); //Y
            Y = ReadFromCharFrom7843(); 
		    XX = XX/SAMPLE_TIMES; 
		    YY = YY/SAMPLE_TIMES;
		    if(((abs(XX - X) < ACCP_VALUE)) && ((abs(YY - Y) < ACCP_VALUE))) 
             {
                    valid = OK;
                 key_data = (((XX<<16)&0xffff0000) | (YY&0x0000ffff));
              *PositionAD = key_data;
              key_ok_flag = TRUE;
             }
            else if(TRUE == key_ok_flag)
             {//�����һ����Ч���ݣ��������Ч���ݾ�һֱ���浽�ͷŰ���Ϊֹ
              // key_ok_flag = FALSE;
              // LED_CPU_RUN_Cpl(); //��������
                    valid = OK; 
              *PositionAD = key_data;
             }
            else
             {//�������������Ч����
                    valid = FALSE; 
              *PositionAD = 0;
             }
            /////////////////////////////////////////////read over
            }
            else
            {//��ʱʱ��δ��
                    valid = FALSE; 
              *PositionAD = 0;
            }
           }              
	     else //if(KEY_PRESS == Key_Status)
	      { 
	             valid = FALSE;
	       *PositionAD = 0;    //������Ч����
	       key_ok_flag = FALSE;    
	      }
	  
	   }
	 ////////////////////////////////////////////////////////////////  
	 else
	 {  
	   if(TRUE == key_ok_flag)//||(KEY_DO_START == Key_Status))
		 {  //�ո��޼����� ���� �������º�û��ִ��  
		        valid = OK; 
		  *PositionAD = key_data;		 
		  Key_Status  = KEY_DO_START;     //����ִ��ָ��    
		  key_ok_flag = FALSE;    
         }
       else if(KEY_PRESS == Key_Status)
        {  //��ʱ���� ��Ϊ��Ч��������
                valid = FALSE; 
	      *PositionAD = 0;
	       Key_Status = KEY_DO_OVER;	
        }
       else if(KEY_DO_OVER == Key_Status)
        {  //�޼������Ұ����Ѿ������� �����ͷŰ������
                valid = FALSE; 
	   	  *PositionAD = 0;
	   	 Key_Status  |= KEY_RELEASE;
          key_ok_flag = FALSE;	
          //add speaker
   
         }  
        else
        {
        ;
        }
       key_press_cnt = 0;
     }
      /////////////////////////////////////////////////  
      return(valid);
  } 
/***************************************************************************************/
//  ������      Set_Originalx0y0(INT32U a,INT32U b)
//  �������    �û�ԭ��AD��ֵ
//  �������    ��
/***************************************************************************************/
BOOLEAN  Set_Originalx0y0(INT32U a,INT32U b)
   	 { 
   	  if ((a>X0_MIN) & (a<X0_MAX) & (b>Y0_MIN) & (b<Y0_MAX))
   	   {x0 = a;
   	    y0 = b;
		return(0); 
       }
	   else
	   return(1);
      }

BOOLEAN  Set_XmidYmid(INT32U a,INT32U b)
   	 { 
   	  //if ((a>XMID_MIN) & (a<X0_DEFAULT_MAX) & (b>Y0_DEFAULT_MIN) & (b<Y0_DEFAULT_MAX))
   	    xmid = a;
   	    ymid = b;
		return(0); 
     }
/***************************************************************************************/
//  ������      Set_XmaxYmax(INT32U c,INT32U d)
//  �������   �û�ԭ��Խǵ��AD��ֵ
//  �������   ��
/***************************************************************************************/
BOOLEAN  Set_XmaxYmax(INT32U c,INT32U d)
   	 { 
   	  if ((c>XMAX_MIN) & (c<XMAX_MAX) & (d>YMAX_MIN) & (d<YMAX_MAX))
   	   {xmax = c;
   	    ymax = d;
		return(0); 
       }
	   else
	   return(1);
      }
      
/***************************************************************************************/
//  ������     Get_PositionXY(void)
//  �������   ��
//  �������   ��ȡ�û���Ļ����㣻��16λ��X;��16λ��Y 
/***************************************************************************************/
INT32U  Get_PositionXY(void)
	{
	 BOOLEAN x;
	 INT32U  y,a,b,xx,yy;
    
	 if(Get_PositionAD(&y))
	   {
	     if(MACHINE_SCREENCAL_MODE == machine_mode)
		 return(y);
		 else
		 {
		 xx = (y>>16)&0x0000ffff;
		 yy =  y&0x0000ffff;
		 //////////////////////////////////////////////
		 if(xx>xmid)
		   {
		    if(xx>xmax)
		    a = SCR_XSIZE;
		    else
		    a = SCR_XSIZE/2 + (xx-xmid)*(SCR_XSIZE/2)/(xmax-xmid);
		   }
		   else
		  {
		   if(xx<x0)
		   a = 0;
		   else
		   a = (xx-x0)*(SCR_XSIZE/2)/(xmid-x0);
          }
        ////////////////////////////////////////////////  
         if(yy>ymid)
		   {
		    if(yy>ymax)
		    b = SCR_YSIZE;
		    else
		    b = SCR_YSIZE/2 + (yy-ymid)*(SCR_YSIZE/2)/(ymax-ymid);
		   }
		   else
		   {
		    if(yy<y0)
		    b = 0;
		    else
		    b = (yy-y0)*(SCR_YSIZE/2)/(ymid-y0);
	  	   } 
	  	 /////////////////////////////////////////////////  
	  	 return (a<<16 | b);
		 }
		}
	 else
	  return(0);
	}

///////////////////////////////////////END OF THE FILE
