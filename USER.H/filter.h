#ifndef _FILTER_H
#define _FILTER_H

#include "type_def.h"

///////////////////////////////////////////////////
#define  BUF_SIZE                    16
#define  LPC2478_AD_FILTER_CNT       4

#define  BUF1_SIZE                   64

////////////////////////////////////////////////////

extern  void    filter_init(void);
extern  void    filter(void);
extern  void    filter2(void);

extern  void    filter_init_lpc2478ad(void);
extern  float   filter_ad_NTC(uint16 ntc_data);
extern  uint16  filter_ad_ch3(uint16 ch3_data);
extern  uint16  filter_ad_ch4(uint16 ch4_data);

extern  uint32 Get_Raw_Data(uint8 ch);
extern  uint32 Get_Filter_Data(uint8 ch);
extern  void   stable_manual_break(void);

#endif