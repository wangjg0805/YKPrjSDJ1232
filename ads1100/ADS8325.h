#ifndef  _ADS8325_H
#define  _ADS8325_H

#include "type_def.h"
#include "global.h"

/**************************************************************************************************************
*                                           PIN ASSIGNMENT
**************************************************************************************************************
*/

#define ADS8325_CS                  (1<<17) //P0[17]
#define ADS8325_CLK                 (1<<1) //P0[1]
#define ADS8325_DOUT                (1<<0) //P0[0]

#define ADS8325_CLK_L               FIO0CLR=ADS8325_CLK
#define ADS8325_CLK_H               FIO0SET=ADS8325_CLK
#define ADS8325_DOUT_L              FIO0CLR=ADS8325_DOUT
#define ADS8325_DOUT_H              FIO0SET=ADS8325_DOUT

#define ADS8325_DOUT_TEST           (FIO0PIN & ADS8325_DOUT)

#define ADS8325_DATA_MODE_O         FIO0DIR |= ADS8325_DOUT
#define ADS8325_DATA_MODE_I         FIO0DIR &= 0xfffffffe


///////////////////////////////////////////////////
void      ADS8325_Init(void);

uint32    ADS8325_RD(void);
#endif