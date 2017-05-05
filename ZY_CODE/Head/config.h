/****************************************Copyright (c)****************************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           config.h
** Last modified Date:  2004-09-17
** Last Version:        1.0
** Descriptions:        User Configurable File
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Chenmingji
** Created date:        2004-09-17
** Version: 1.0
** Descriptions:        First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:  		LinEnqiang
** Modified date: 		2007-05-15
** Version: 1.0
** Descriptions: 		For LPC2300
**
*********************************************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef  __cplusplus
extern "C" {
#endif
 
typedef unsigned char  INT8U;                                           /*  �޷���8λ���ͱ���           */
typedef signed   char  INT8S;                                           /*  �з���8λ���ͱ���           */
typedef unsigned short INT16U;                                          /*  �޷���16λ���ͱ���          */
typedef signed   short INT16S;                                          /*  �з���16λ���ͱ���          */
typedef unsigned int   INT32U;                                          /*  �޷���32λ���ͱ���          */
typedef signed   int   INT32S;                                          /*  �з���32λ���ͱ���          */
typedef float          FP32;                                            /*  �����ȸ�������32λ���ȣ�    */
typedef double         FP64;                                            /*  ˫���ȸ�������64λ���ȣ�    */
                              /*  8-bit boolean or logical 
/*********************************************************************************************************
  ARM���������
  ARM specital code
*********************************************************************************************************/
#include    "LPC2400.h"
#include    "target.h"
#include    "vicControl.h"
#include    "type_def.h"
/*********************************************************************************************************
  Ӧ�ó�������
  Application Program Configurations
*********************************************************************************************************/
#include    <stdio.h>
#include    <rt_sys.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h>

/*********************************************************************************************************
  �����ӵ�����
  Configuration of the example
*********************************************************************************************************/
#define Fosc                12000000								    /*	OSC = 12MHz                 */
#define Fcclk               (Fosc*6)								    /*	OSC = 12MHz                 */
#define Fcco                (Fcclk*6)					         	    /*	OSC = 12MHz                 */
#define Fpclk               (Fcclk/4)								    /*	OSC = 12MHz                 */

#endif                                                                  /*  __CONFIG_H                  */ 
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
