#include "global.h"
#include "config.h"
#include "ads8325.h"

#define DELAYMS 2

static void Delay(uint32 n)
 {
  uint32  i;
  for(i=0;i<n;i++)
  ;
 }

static void SampleCLK(void)
 {
  uint32 i;
  for(i=0;i<6;i++)
  {
   ADS8325_CLK_L;
   //Delay(DELAYMS);
   ADS8325_CLK_H;
   //Delay(DELAYMS);
  } 
}

uint32  ADS8325_RD(void)
  {
   uint32 i;
   uint32 tempbit=0;
   uint32 data=0;
   
   ADS8325_DATA_MODE_I; 
   SampleCLK();
   
   ADS8325_CLK_L;
   //Delay(DELAYMS);
   for(i=0;i<16;i++)
   {
    ADS8325_CLK_H;
    //Delay(DELAYMS);
    if(ADS8325_DOUT_TEST)
    {
     tempbit=1;
    }
    else
    {
     tempbit=0;
    }
    data=(data<<1)|tempbit;
    ADS8325_CLK_L;
    //Delay(DELAYMS);
   }
   return data/8;
}
