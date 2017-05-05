/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    MSCUSER.H
 *      Purpose: Mass Storage Class Custom User Definitions
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2007 Keil Software.
 *---------------------------------------------------------------------------*/

#ifndef __MSCUSER_H__
#define __MSCUSER_H__

#include "type.h"
#include "usbcfg.h"

/* Mass Storage Memory Layout */
#define MSC_MemorySize  16384
#define MSC_BlockSize   512
#define MSC_BlockCount  (MSC_MemorySize / MSC_BlockSize)


/* Max In/Out Packet Size */
#define MSC_MAX_PACKET  64

/* MSC In/Out Endpoint Address */
#define MSC_EP_IN       0x82
#define MSC_EP_OUT      0x02

extern        BYTE     Memory[MSC_MemorySize];  /* MSC RAM */
extern const  BYTE   DiskImage[MSC_ImageSize];

/* MSC Requests Callback Functions */
extern BOOL MSC_Reset     (void);
extern BOOL MSC_GetMaxLUN (void);

/* MSC Bulk Callback Functions */
extern void MSC_GetCBW (void);
extern void MSC_SetCSW (void);
extern void MSC_BulkIn (void);
extern void MSC_BulkOut(void);


#endif  /* __MSCUSER_H__ */
