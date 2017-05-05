/*****************************************************************************
 *   rtc.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __RTC_H 
#define __RTC_H

#include "type_def.h"

typedef struct {
    unsigned char RTC_Sec;     /* Second value - [0,59] */
    unsigned char RTC_Min;     /* Minute value - [0,59] */
    unsigned char RTC_Hour;    /* Hour value - [0,23] */
    unsigned char RTC_Day;    /* Day of the month value - [1,31] */
    unsigned char RTC_Mon;     /* Month value - [1,12] */
    unsigned short RTC_Year;    /* Year value - [0,4095] */
    unsigned char RTC_Wday;    /* Day of week value - [0,6] */
    unsigned short RTC_Yday;    /* Day of year value - [1,365] */
} RTCTime;


#define IMSEC		0x00000001
#define IMMIN		0x00000002
#define IMHOUR		0x00000004
#define IMDOM		0x00000008
#define IMDOW		0x00000010
#define IMDOY		0x00000020
#define IMMON		0x00000040
#define IMYEAR		0x00000080

#define AMRSEC		0x00000001  /* Alarm mask for Seconds */
#define AMRMIN		0x00000002  /* Alarm mask for Minutes */
#define AMRHOUR		0x00000004  /* Alarm mask for Hours */
#define AMRDOM		0x00000008  /* Alarm mask for Day of Month */
#define AMRDOW		0x00000010  /* Alarm mask for Day of Week */
#define AMRDOY		0x00000020  /* Alarm mask for Day of Year */
#define AMRMON		0x00000040  /* Alarm mask for Month */
#define AMRYEAR		0x00000080  /* Alarm mask for Year */

#define ILR_RTCCIF	0x01
#define ILR_RTCALF	0x02
#define ILR_RTSSF   0x04

#define CCR_CLKEN	0x01
#define CCR_CTCRST	0x02
#define CCR_CLKSRC	0x10

extern  uint32   Sec_Change_Flag;
/////////////////////////////////
extern void RTCInit( void );
extern void RTCStart( void );
extern void RTCStop( void );
extern void RTC_CTCReset( void );
extern void RTCSetTime( RTCTime );
extern RTCTime RTCGetTime( void );
extern void RTCSetAlarm( RTCTime );
extern void RTCSetAlarmMask( unsigned long AlarmMask );
extern unsigned int GetSecChange(void);

#endif /* end __RTC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
