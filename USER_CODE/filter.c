//**********************************************************************
//文件名:filter.c
//函数列表
//1   手动破坏稳定状态  stable_manual_break
//2   滤波初始化        filter_init
//3   排序              filter_sort
//4   取平均值          stable_filter_average
//5   取平均值1         stable_filter_average1
//6   取平均值2         stable_filter_average2
//7   取平均值3         stable_filter_average3
//8   滤波过程          filter(void)
//9   零点自动跟踪      auto_zer0
//10  载荷自动跟踪      stable_auto_zer0_A
//10a 载荷自动跟踪      stable_auto_zer0_B
//11  重复性自动修正    repeat_adjust
//12  返回滤波后的数据  Get_Filter_Data
//
//**********************************************************************
#include "filter.h"

//#include "ad_acquire.h"
#include "timer.h"
#include "stdlib.h"
#include "global.h"

#define          DDLV_BUF_LENGTH           50

       uint32  ddlv_buf_pre[DDLV_BUF_LENGTH];


static uint32  raw_data[4][BUF_SIZE]; //滤波缓存

static uint32  ddlv_raw1_data[BUF1_SIZE];

static uint32  ch_data[4];            //滤波后的数据 
  
static uint32  ADS1100_Filter_data;   //滤波后的数据  

static uint32  buf_index,buf1_index,buf2_index;

       uint32  stable_time_cnt;
/////////////////////////////////////////////////////////////
//1 此函数在校准的时候调用，人为破坏平衡
/////////////////////////////////////////////////////////////
void  stable_manual_break(void)
{
 stable_time_cnt = 0;
 stable_flag     = FALSE; 
}   

//////////////////////////////////
//2 滤波初始化
//////////////////////////////////////     
void  filter_init(void)
{
 uint8 i,j;

   for(i=0;i<DDLV_BUF_LENGTH;i++)
      {
       ddlv_buf_pre[i] = 0;
      } 


   for(i=0;i<4;i++)
    {
      for(j=0;j<BUF_SIZE;j++)
      raw_data[i][j] = 0;
    }
  //////////////////////////////////////  
   for(i=0;i<4;i++)
      {
       ch_data[i] = 0;
      } 
  
   for(i=0;i<BUF1_SIZE;i++)
      {
       ddlv_raw1_data[i] = 0;
      } 
  
   buf_index  = 0; 
   buf1_index = 0; 
       
}
//////////////////////////////////////////
      
///////////////////////////////////////////////////////////////////
//
void  filter(void)
{
  static uint32 ch_data_last[4];
         
     uint32 ch_data_tmp;
     uint32 current_ch,i,sum;
     
     current_ch = select_ch-1;
     //根据当前通道读取 相应的数据 然后进入自己的滤波队列
     ch_data_tmp = Get_Raw_From_Int2(current_ch);
     
     if(abs(ch_data_last[current_ch]-ch_data_tmp)<200)
         {
          stable_time_cnt++;
          if(stable_time_cnt > 100)
             stable_flag = TRUE;
          ch_data_last[current_ch] = (ch_data_last[current_ch]+ch_data_tmp)/2;
         } 
     else 
         {
          stable_time_cnt  = 0;
          stable_flag = FALSE;
          ch_data_last[current_ch] =  ch_data_tmp;
         }
         
     raw_data[current_ch][buf_index++] = ch_data_last[current_ch]; //新数据保存
    
     if(BUF_SIZE == buf_index)
     buf_index = 0;
     /////////////////////获取滤波后的数据
     sum = 0;
     for(i=0;i<BUF_SIZE;i++)
     sum += raw_data[current_ch][i];
     
     ch_data[current_ch] = sum/BUF_SIZE;
       
}

////////////////////////////////////////////////////
//电导率仪器  的滤波 处理

//********************************************************
/*
uint32  get_sort_data(void) //
        {
        
          //uint32  buf[BUF2_SIZE];
           uint32  sum;
           uint8   pass,i;
           
            for(i=0;i<BUF2_SIZE;i++)
              buf[i] = ddlv_raw2_data[i];
                 
            for(pass = 0;pass < BUF2_SIZE - 1 ;pass++)
	              {
		            for( i = 0;i < (BUF2_SIZE - 1 - pass);i++)
		             {
			           if(buf[i] > buf[i+1])
			             {
			               sum = buf[i];
		   	               buf[i] = buf[i+1];
			               buf[i+1] = sum ;
			             }
		             }
	              }
	         sum = 0;  
	         for(i=16;i<48;i++)   
	         sum += buf[i];
	         return(sum/32);  
         } 
 */        
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
void  filter2(void)
{
     static uint32 last_data      = 0;
     static uint32 buf_full_flag  = 0;    
     uint32 data_tmp;
     uint32 i,sum;
     
     data_tmp = ADS1100_Raw;

     if(3 == filter_phase) //可以参与取平均值的数据
       {
         ddlv_raw1_data[buf1_index++] = data_tmp;
     
         if(BUF1_SIZE == buf1_index)
            {
             buf1_index = 0;
             buf_full_flag = 1;
            }
         //////////////////根据数据取平均值   
         if(1 == buf_full_flag)
           {      
            sum = 0;
            for(i=0;i<BUF1_SIZE;i++)
            sum += ddlv_raw1_data[i];
            ADS1100_Filter_data = sum/BUF1_SIZE;
           }
         else
           {
            sum = 0;
            for(i=0;i<buf1_index;i++)
            sum += ddlv_raw1_data[i];
            ADS1100_Filter_data = sum/buf1_index; 
           }  
       }
     else 
        {
         //非常规数据
         //数据变化非常大
         //其它不适宜取平均值的  
         buf1_index    = 0;
         buf_full_flag = 0;
         ADS1100_Filter_data = data_tmp;
       }
      
     ////////////////////////////////////
     
      if(abs(ADS1100_Filter_data-last_data)>500)
      { 
       stable_time_cnt  = 0;
       stable_flag = FALSE;
      }
      else
      {
        stable_time_cnt++;
        if(stable_time_cnt > 10000)
        stable_time_cnt = 5000;
        
        if(STAT_SYS_CAL == current_mode)
         {
           if(stable_time_cnt > 100)
              stable_flag = TRUE;  
         }
        else
         {      
           if(stable_time_cnt > 20)
              stable_flag = TRUE;
         }     
        /////////////////////////计算平均值
      } 
      
      last_data  = ADS1100_Filter_data;
 }

//////////////////////////////////////////////
//12  返回自动跟踪后的数据
//////////////////////////////////////////////
uint32 Get_Raw_Data(uint8 ch)
{
 if(0 == buf_index)
 return(raw_data[ch][BUF_SIZE-1]);
 else
 return(raw_data[ch][buf_index-1]);
}


uint32 Get_Filter_Data(uint8 ch)
{
 return(ch_data[ch]);
}

uint32 Get_Filter_ADS1100Data(void)
{
 return(ADS1100_Filter_data);
}
/////////////////////////////////////////////////////////////////////

const  float ntc_table[]=
{ //-5     0     5      10     15
 41.033,29.884,25.121,19.805,15.693,
 //20     25    30      35     40
 12.496,10.000,8.0413,6.4972,5.2743,
 //45     50    55      60     65
 4.3013,3.4970,2.8993,2.3959,1.9882,
 //70     75    80      85     90
 1.6567,1.3850,1.1641,0.9814,0.8304,
 //95    100    105
 0.7052,0.5840,0.5140,0.0001
};

const  uint8 ntc_start[]=
{0,//无效数据
 0 ,5, 10,15,20,25,30,35,
 40,45,50,55,60,65,70,75,
 80,85,90,95,100,105
};

static uint16 ntc_buf[LPC2478_AD_FILTER_CNT];
static uint16 ch3_buf[32];
static uint16 ch4_buf[32];



/////////////////////////////////////////////////////////////////////
void filter_init_lpc2478ad(void)
 {
  uint8 i;
  for(i=0;i<LPC2478_AD_FILTER_CNT;i++)
    {
     ntc_buf[i] = 0;
    
    }
  for(i=0;i<32;i++)
    {
    
     ch3_buf[i] = 0;
     ch4_buf[i] = 0;
    }     
 }
 
 //////////////////////////////////////////
//********************************************************
float  filter_ad_NTC(uint16 ntc_data)
         { //当前缓存已满，则取除了最大最小值后的平均值
           //如果没有满  ，则取 所有数据的平均值
           uint32  sum;
           float   x;
           uint8   pass,i;
           static  uint8 index = 0;
           ntc_buf[index] = ntc_data;
           index++;
           if(LPC2478_AD_FILTER_CNT==index)
           index = 0;
           ////////////////////////////////////////
	       sum = 0;
	       for(i=0;i<LPC2478_AD_FILTER_CNT;i++)
	        sum+=ntc_buf[i];  
	       sum = sum/LPC2478_AD_FILTER_CNT;
	       /////////////////////////////////根据此数据倒推出电阻式多大
	      
	       x = (10*sum)/(1024.001-sum); 
	       ////////////////////////////////根据此电阻值计算出温度
	       if(x>=ntc_table[0]) //温度太小
	         {
	          return(TEMP_SENSOR_NC_VALUE);
	         }
	       else if(x<=ntc_table[22]) 
	         {
	          return(TEMP_SENSOR_NC_VALUE);
	         } 
	       else 
	         {
	          i = 1;
	          while(1)
	           {
	            if(x>ntc_table[i])
	               break;
	            else
	             i++;   
	           }
	          //////////////////计算温度 
	          x =ntc_start[i]-(x-ntc_table[i])/(ntc_table[i-1]-ntc_table[i])*5;
	          if(x>2)
	          return(x-2);
	          else
	          return(x); 
	          } 
	 }        
///////////////////////////////////////////////////////////////////
 ///////////////////////////////////////
//********************************************************
uint16  filter_ad_ch3(uint16 ch3_data)
         { 
           //如果没有满  ，则取 所有数据的平均值
           uint32  sum;
           uint8   pass,i;           
           static  uint8 index = 0;
           ch3_buf[index] = ch3_data;
           index++;
           if(32 == index)
           index = 0;
          
	       sum = 0;
	       for(i=0;i<32;i++)
	       sum += ch3_buf[i];  
	       sum = sum/32;
	       
	       return(sum);
	     }        
	     
//********************************************************
uint16  filter_ad_ch4(uint16 ch4_data)
         {  //如果没有满  ，则取 所有数据的平均值
           uint32  sum;
           uint8   pass,i;           
           static  uint8 index = 0;
           ch4_buf[index] = ch4_data;
           index++;
           if(32 == index)
           index = 0;
          
	       sum = 0;
	       for(i=0;i<32;i++)
	       sum += ch4_buf[i];  
	       sum = sum/32;
	       
	       return(sum);
	     }        	     


 uint32  sort_data(void) //
        {
        
           uint32  sum;
           uint8   pass,i;
           
            for(pass = 0;pass < DDLV_BUF_LENGTH - 1 ;pass++)
	              {
		            for( i = 0;i < (DDLV_BUF_LENGTH - 1 - pass);i++)
		             {
			           if(ddlv_buf_pre[i] > ddlv_buf_pre[i+1])
			             {
			               sum = ddlv_buf_pre[i];
		   	               ddlv_buf_pre[i] = ddlv_buf_pre[i+1];
			               ddlv_buf_pre[i+1] = sum ;
			             }
		             }
	              }
	         sum = 0;  
	         for(i=20;i<30;i++)   
	         sum += ddlv_buf_pre[i];
	         return(sum/10);  
         } 


