;/****************************************Copyright (c)***************************************************
;**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
;**                                     
;**                               http://www.embedtools.com
;**
;**--------------File Info--------------------------------------------------------------------------------
;** File name: 			Startup.s
;** Last modified Date:  
;** Last Version: 		
;** Descriptions: 		The start up codes for LPC2100, including the initializing codes for the entry 
;**                     point of exceptions and the stacks of user tasks. Every project should have a 
;**                     independent copy of this file for related modifications
;**-------------------------------------------------------------------------------------------------------
;** Created by: 		Chenmingji
;** Created date:   	2004-02-02
;** Version:			1.0
;** Descriptions: 		The original version
;**-------------------------------------------------------------------------------------------------------
;** Modified by: 		LinEnqiang
;** Modified date:		2008-05-05	
;** Version:			1.01
;** Descriptions: 		�����жϹ���֧�� for LPC2400
;**
;********************************************************************************************************/
                                INCLUDE     LPC2400.INC                 ;/*  ����ͷ�ļ�                 */
                                                                        ;/*  �����ջ�Ĵ�С             */
SVC_STACK_LEGTH     EQU         0
FIQ_STACK_LEGTH     EQU         10
IRQ_STACK_LEGTH     EQU         9 * 8                                   ;/*  ÿ��Ƕ����Ҫ8���ֶ�ջ������*/
                                                                        ;/*  9Ƕ�ײ�                    */
ABT_STACK_LEGTH     EQU         0
UND_STACK_LEGTH     EQU         0

NoInt               EQU         0x80                                    ;/*  ��ֹ IRQ �жϺ궨��        */
NoFIQ		        EQU	        0x40                                    ;/*  ��ֹ FIQ �жϺ궨��        */

USR32Mode           EQU         0x10                                    ;/*  �û�ģʽ�궨��             */
SVC32Mode           EQU         0x13                                    ;/*  ����ģʽ�궨��             */
SYS32Mode           EQU         0x1f                                    ;/*  ϵͳģʽ�궨��             */
IRQ32Mode           EQU         0x12                                    ;/*  IRQģʽ�궨��              */
FIQ32Mode           EQU         0x11                                    ;/*  FIQģʽ�궨��              */

WAITWEN0            EQU         0x0F                                    ;/* ����EMCStaticWaitWen0       */
WAITOEN0            EQU         0x0F                                    ;/* ����EMCStaticWaitOen0       */ 
WAITRD0             EQU         0x0F                                    ;/* ����EMCStaticWaitRd0        */
WAITPAGE0           EQU         0x1F                                    ;/* ����EMCStaticWaitPage0      */
WAITWR0             EQU         0x0F                                    ;/* ����EMCStaticWaitWr0        */
WAITTURN0           EQU         0x0F                                    ;/* ����EMCStaticWaitTurn0      */

WAITWEN1            EQU         0x00                                    ;/* ����EMCStaticWaitWen1       */
WAITOEN1            EQU         0x01                                    ;/* ����EMCStaticWaitOen1       */ 
WAITRD1             EQU         0x04                                    ;/* ����EMCStaticWaitRd1        */
WAITPAGE1           EQU         0x1F                                    ;/* ����EMCStaticWaitPage1      */
WAITWR1             EQU         0x02                                    ;/* ����EMCStaticWaitWr1        */
WAITTURN1           EQU         0x0F                                    ;/* ����EMCStaticWaitTurn1      */

WAITWEN2            EQU         0x0F                                    ;/* ����EMCStaticWaitWen2       */
WAITOEN2            EQU         0x0F                                    ;/* ����EMCStaticWaitOen2       */ 
WAITRD2             EQU         0x0F                                    ;/* ����EMCStaticWaitRd2        */
WAITPAGE2           EQU         0x1F                                    ;/* ����EMCStaticWaitPage2      */
WAITWR2             EQU         0x0F                                    ;/* ����EMCStaticWaitWr2        */
WAITTURN2           EQU         0x0F                                    ;/* ����EMCStaticWaitTurn2      */

WAITWEN3            EQU         0x0F                                    ;/* ����EMCStaticWaitWen3       */
WAITOEN3            EQU         0x0F                                    ;/* ����EMCStaticWaitOen3       */ 
WAITRD3             EQU         0x0F                                    ;/* ����EMCStaticWaitRd3        */
WAITPAGE3           EQU         0x1F                                    ;/* ����EMCStaticWaitPage3      */
WAITWR3             EQU         0x0F                                    ;/* ����EMCStaticWaitWr3        */
WAITTURN3           EQU         0x0F                                    ;/* ����EMCStaticWaitTurn3      */


BCFG_08DEF	        EQU	        0x00000000                              ;/*   8Bit Bus                  */
BCFG_16DEF	        EQU	        0x00000001                              ;/*  16Bit Bus                  */
BCFG_32DEF	        EQU	        0x00000002                              ;/*  32Bit Bus                  */

;//                       |  ҳģʽ    |  Ƭѡ����  |�ֽڶ�λ״̬|  �ӳ��ȴ�  |  д������  |   д����   |
;//                       |    PM      |    PC      |    PB      |     EW     |     B      |     P      |
;//                       |0:����1:ʹ��| 0:�� 1:��  |            |0:����1:ʹ��|0:����1:ʹ��|0:����1:ʹ��|
BCFG0               EQU   ( (0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20) )
BCFG1               EQU   ( (0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20) )
BCFG2               EQU   ( (0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20) )
BCFG3               EQU   ( (0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20) )

STATICCFG0	        EQU   ( BCFG_16DEF | BCFG0 ) 
STATICCFG1          EQU	  ( BCFG_16DEF | BCFG1 )
STATICCFG2	        EQU	  ( BCFG_16DEF | BCFG2 )
STATICCFG3          EQU	  ( BCFG_16DEF | BCFG3 )

;/********************************************************************************************************
; The imported labels    
; ������ⲿ�����������
;********************************************************************************************************/

    IMPORT  __main                                                      ;/*  C�������������            */        
    IMPORT  RunFirst
    IMPORT  SDRAMInit
    IMPORT  vicControl
    IMPORT  FIQ_Exception                                               ;/*  �����ж��쳣�������       */
    IMPORT  TargetResetInit                                             ;/*  Ŀ��������ʼ��           */    
    IMPORT	SoftwareInterrupt    
    IMPORT __use_two_region_memory
    IMPORT __use_no_semihosting_swi
;/********************************************************************************************************
; The emported labels    
; ���ⲿʹ�õı����������
;********************************************************************************************************/

    EXPORT  RESET  
	EXPORT  ResetInit  
	EXPORT  StackUsr   
	EXPORT  InitStack
	EXPORT  bottom_of_heap  
	EXPORT  SoftwareInterruptAdd
	EXPORT  __user_initial_stackheap
	
    PRESERVE8

    AREA    RESET,CODE,READONLY
	;ARM
        ENTRY
Resetpoint                                                                   ;/*  �ж�������                 */
        LDR         PC, ResetAddr
        LDR     	PC, UndefinedAddr
        LDR    	 	PC, SWI_Addr
        LDR     	PC, PrefetchAddr
        LDR     	PC, DataAbortAddr
        DCD     	0xb9206e50
        LDR     	PC, [PC, #-0x120]
        LDR     	PC, FIQ_Addr

ResetAddr           DCD     ResetInit
UndefinedAddr       DCD     Undefined
SWI_Addr            DCD     SoftwareInterruptAdd
PrefetchAddr        DCD     PrefetchAbort
DataAbortAddr       DCD     DataAbort
Nouse               DCD     0
IRQ_Addr            DCD     0
FIQ_Addr            DCD     FIQ_Handler

Undefined
        B       .
PrefetchAbort
        B       .
DataAbort
        B       .               
SoftwareInterruptAdd 
        CMP     R0, #0x100
        BLO     SoftwareInterrupt
        B       vicControl    

;/********************************************************************************************************
; �����ж�
;********************************************************************************************************/
FIQ_Handler
        STMFD   SP!, {R0-R3, LR}
        BL		FIQ_Exception      
        LDMFD   SP!, {R0-R3, LR}
        SUBS    PC,  LR,  #4
;/********************************************************************************************************
;** Function name:          ResetInit
;**
;** Descriptions:           ��λ����
;**
;** input parameters:       None
;** Returned value:         None
;**
;** Created by:             Chenmingji
;** Created Date:           2004/02/02
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            LinEnqiang
;** Modified date:          2008-01-15
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
ResetInit 
        
        BL      RunFirst
              
        LDR     R1, =STATICCFG0		                                    ;/* ����EMCStaticConfig0        */
        LDR     R2, =WAITWEN0			                                ;/* ����EMCStaticWaitWen0       */
        LDR     R2, =WAITWEN0			                                ;/* ����EMCStaticWaitWen0       */
        LDR     R3, =WAITOEN0		                                    ;/* ����EMCStaticWaitOen0       */
        LDR     R4, =WAITRD0		    	                            ;/* ����EMCStaticWaitRd0        */
        LDR     R5, =WAITPAGE0			                                ;/* ����EMCStaticWaitPage0      */
        LDR     R6, =WAITWR0	    		                            ;/* ����EMCStaticWaitWr0        */			   
        LDR     R7, =WAITTURN0			                                ;/* ����EMCStaticWaitTurn0      */
        LDR		R0, =EMCStaticConfig0 
        STMIA   R0!, {R1-R7}  
 
        LDR     R1, =STATICCFG1		                                    ;/* ����EMCStaticConfig1        */
        LDR     R2, =WAITWEN1			                                ;/* ����EMCStaticWaitWen1       */
        LDR     R3, =WAITOEN1		                                    ;/* ����EMCStaticWaitOen1       */
        LDR     R4, =WAITRD1		    	                            ;/* ����EMCStaticWaitRd1        */
        LDR     R5, =WAITPAGE1			                                ;/* ����EMCStaticWaitPage1      */
        LDR     R6, =WAITWR1	    		                            ;/* ����EMCStaticWaitWr1        */			   
        LDR     R7, =WAITTURN1			                                ;/* ����EMCStaticWaitTurn1      */
        LDR		R0, =EMCStaticConfig1
        STMIA   R0!, {R1-R7}
    
        LDR     R1, =STATICCFG2		                                    ;/* ����EMCStaticConfig2        */
        LDR     R2, =WAITWEN2			                                ;/* ����EMCStaticWaitWen2       */
        LDR     R3, =WAITOEN2		                                    ;/* ����EMCStaticWaitOen2       */
        LDR     R4, =WAITRD2		    	                            ;/* ����EMCStaticWaitRd2        */
        LDR     R5, =WAITPAGE2			                                ;/* ����EMCStaticWaitPage2      */
        LDR     R6, =WAITWR2	    		                            ;/* ����EMCStaticWaitWr2        */			   
        LDR     R7, =WAITTURN2			                                ;/* ����EMCStaticWaitTurn2      */
        LDR		R0, =EMCStaticConfig2       
        STMIA   R0!, {R1-R7}  
        
        LDR     R1, =STATICCFG3		                                    ;/* ����EMCStaticConfig3        */
        LDR     R2, =WAITWEN3			                                ;/* ����EMCStaticWaitWen3       */
        LDR     R3, =WAITOEN3		                                    ;/* ����EMCStaticWaitOen3       */
        LDR     R4, =WAITRD3		    	                            ;/* ����EMCStaticWaitRd3        */
        LDR     R5, =WAITPAGE3			                                ;/* ����EMCStaticWaitPage3      */
        LDR     R6, =WAITWR3	    		                            ;/* ����EMCStaticWaitWr3        */			   
        LDR     R7, =WAITTURN3			                                ;/* ����EMCStaticWaitTurn3      */
        LDR		R0, =EMCStaticConfig3
        STMIA   R0!, {R1-R7}
        BL      InitStack               		                        ;/*  ��ʼ����ջ                 */
        BL      TargetResetInit         		                        ;/*  Ŀ��������ʼ��           */       

        B       __main                                                  ;/*  ��ת��c�������            */
                    		                            
;/********************************************************************************************************
;** Function name:          __user_initial_stackheap
;**
;** Descriptions:           �⺯����ʼ���Ѻ�ջ������ɾ��
;**
;** input parameters:       �ο��⺯���ֲ�
;** Returned value:         �ο��⺯���ֲ�
;**
;** Created by:             Chenmingji
;** Created Date:           2004/02/02
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            LinEnqiang
;** Modified date:          2008-01-15
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
__user_initial_stackheap    
    LDR   R0, =bottom_of_heap		
    ;LDR   R1, =StackUsr			
    LDR   R2, =top_of_heap		
    LDR   R3, =bottom_of_Stacks	      
    MOV   PC, LR

    EXPORT _sys_exit
_sys_exit
    B       .

    EXPORT __rt_div0
    EXPORT fputc
    EXPORT fgetc
    EXPORT _sys_close
    EXPORT _sys_write
    EXPORT _sys_read
    EXPORT _sys_istty
    EXPORT _sys_seek
    EXPORT _sys_ensure
    EXPORT _sys_flen
    EXPORT _sys_tmpnam
    EXPORT _sys_command_string

__rt_div0
fputc
fgetc
_sys_close
_sys_write
_sys_read
_sys_istty
_sys_seek
_sys_ensure
_sys_flen
_sys_tmpnam
_sys_command_string
    NOP	
    MOV     R0, #0
    MOV     PC, LR		
    
;/********************************************************************************************************
;** Function name:          CrpData
;**
;** Descriptions:           ����оƬ
;**
;** input parameters:       NONE
;** Returned value:         NONE
;**
;** Created by:             Chenmingji
;** Created Date:           2004/03/27 
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            
;** Modified date:          
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
    IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    WHILE . < 0x1fc
    NOP
    WEND
CrpData1
    DCD     0x87654321                                                  ;/*  ������Ϊ0x87654321ʱ���û� */
                                                                        ;/*  ���򱻱���                 */
    ENDIF
;/********************************************************************************************************
;** Function name:          InitStack
;**
;** Descriptions:           ��ʼ����ջ
;**
;** input parameters:       None
;** Returned value:         None
;**
;** Created by:             Chenmingji
;** Created Date:           2004/02/02
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            LinEnqiang
;** Modified date:          2008-01-15
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
InitStack    
        MOV     R0, LR                
;Build the SVC stack
;���ù���ģʽ��ջ
        MSR     CPSR_c, #0xd3		
        LDR     SP, StackSvc
;Build the IRQ stack
;�����ж�ģʽ��ջ
        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq
;Build the FIQ stack	
;���ÿ����ж�ģʽ��ջ
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq
;Build the DATAABORT stack
;������ֹģʽ��ջ
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt
;Build the UDF stack
;����δ����ģʽ��ջ
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd
;Build the SYS stack
;����ϵͳģʽ��ջ
        MSR     CPSR_c, #0xdf
        LDR     SP, =StackUsr              
        MOV     PC, R0
               
    LTORG
StackSvc           DCD     SvcStackSpace  + (SVC_STACK_LEGTH - 1) * 4
StackIrq           DCD     IrqStackSpace  + (IRQ_STACK_LEGTH - 1) * 4
StackFiq           DCD     FiqStackSpace  + (FIQ_STACK_LEGTH - 1) * 4
StackAbt           DCD     AbtStackSpace  + (ABT_STACK_LEGTH - 1) * 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1) * 4    
    
        AREA    MyStacks, DATA, NOINIT, ALIGN = 2;                      ;/*  �����ջ�ռ�               */    
        
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4                          ;/*  ����ģʽ��ջ�ռ�           */
IrqStackSpace      SPACE   IRQ_STACK_LEGTH * 4                          ;/*  �ж�ģʽ��ջ�ռ�           */
FiqStackSpace      SPACE   FIQ_STACK_LEGTH * 4                          ;/*  �����ж�ģʽ��ջ�ռ�       */
AbtStackSpace      SPACE   ABT_STACK_LEGTH * 4                          ;/*  ��ֹ��ģʽ��ջ�ռ�         */
UndtStackSpace     SPACE   UND_STACK_LEGTH * 4                          ;/*  δ����ģʽ��ջ             */   

        AREA    HeapBottom, DATA, NOINIT
bottom_of_heap    	SPACE   1

        AREA    StackBottom, DATA, NOINIT
bottom_of_Stacks    SPACE   1

        AREA    HeapTop, DATA, NOINIT
top_of_heap

        AREA    StacksTop, DATA, NOINIT
StackUsr  
    END
    
;/********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
