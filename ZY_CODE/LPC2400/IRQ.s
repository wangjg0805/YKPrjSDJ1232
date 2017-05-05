;/****************************************Copyright (c)***************************************************
;**                               Guangzou ZHIYUAN electronics Co.,LTD.
;**                                      
;**                                 http://www.embedtools.com
;**
;**--------------File Info--------------------------------------------------------------------------------
;** File Name: IRQ.s
;** Last modified Date:  2004-06-14
;** Last Version: 1.1
;** Descriptions: The irq handle that what allow the interrupt nesting. 
;**
;**-------------------------------------------------------------------------------------------------------
;** Created By: 	Chenmingji
;** Created date:   2004-09-17
;** Version: 1.0
;** Descriptions: 	First version
;**
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;** Version:
;** Descriptions:
;**
;********************************************************************************************************/

NoInt       EQU 0x80

USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11

    CODE32

    AREA    IRQ,CODE,READONLY

    MACRO
$IRQ_Label HANDLER $IRQ_Exception_Function

        EXPORT  $IRQ_Label                      						;  ����ı��       
        IMPORT  $IRQ_Exception_Function         						;  ���õ��ⲿ���

$IRQ_Label
        SUB     LR, LR, #4                      						;  ���㷵�ص�ַ
        STMFD   SP!, {R0-R3, R12, LR}           						;  �������񻷾�
        MRS     R3, SPSR                        						;  ����״̬
        STMFD   SP, {R3,LR}^                    						;  ����SPSR���û�״̬��SP,                                               							
        NOP
        SUB     SP, SP, #4*2

        MSR     CPSR_c, #(NoInt | SYS32Mode)    						;  �л���ϵͳģʽ 
       
        BL      $IRQ_Exception_Function         						;  ����c���Ե��жϴ������

        MSR     CPSR_c, #(NoInt | IRQ32Mode)    						;  �л���irqģʽ
        LDMFD   SP, {R3,LR}^                    						;  �ָ�SPSR���û�״̬��SP                                                				
        MSR     SPSR_cxsf, R3
        ADD     SP, SP, #4*2                    

        LDMFD   SP!, {R0-R3, R12, PC}^          
    MEND

;/******************************************************************************************************** 
;	��������жϾ�����û�����ʵ������ı� 
;********************************************************************************************************/

;Timer0_Handler  HANDLER Timer0

    END
;/********************************************************************************************************
; 	END FILE
;********************************************************************************************************/
