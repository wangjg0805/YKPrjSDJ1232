;/****************************************Copyright (c)***************************************************
;**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
;**                                     
;**                               http://www.embedtools.com
;**
;**--------------File Info-------------------------------------------------------------------------------
;** File name: 			vicControl.s
;** Last modified Date: 2007-06-12
;** Last Version: 		1.0
;** Descriptions: 		Provide VIC_Control in prerogative mode
;**------------------------------------------------------------------------------------------------------
;** Created by: 		LinEnqiang
;** Created date:   	2007-06-12
;** Version:			1.0
;** Descriptions: 		The original version
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		LinEnqiang
;** Modified date:		2008-04-30
;** Version:			1.1
;** Descriptions:       ͳһ�ڴ���淶V1.2
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		LinEnqiang
;** Modified date:		2008-06-10
;** Version:			1.2
;** Descriptions:       ����VICȫ�ֿ��ع���
;********************************************************************************************************/
                                 INCLUDE     LPC2400.INC                ; Include the head file ����ͷ�ļ�

;/********************************************************************************************************
; �궨��
;********************************************************************************************************/

NoInt       EQU 0x80
NoFIQ		EQU	0x40

SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f


			IMPORT  StackUsr 
		
	 		EXPORT  RunFirst
	 		EXPORT  vicControl
	 		EXPORT	SoftwareInterrupt    
	 			 	
	CODE32
    AREA    |RUNFIRST|, CODE, READONLY     

;/********************************************************************************************************
;** Function name:           RunFirst
;** Descriptions:            VIC�����ʼ��
;** input parameters:        none
;** output parameters:       none
;** Returned value:          none
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
		
RunFirst			
		LDR     R2, =VICIntEnClr
		MVN     R3, #0x00
		STR	    R3, [R2] 
		LDR     R2, =VICIntSelect
		MOV     R3, #0x00
		STR	    R3, [R2] 

        LDR     R0, =VICVectAddr0
        MOV     R1, #0
        MOV     R2, #0
        MOV     R3, #0
        MOV     R4, #0
        MOV     R5, #0
        MOV     R6, #0
        MOV     R7, #0
        MOV     R8, #0
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}
        LDR     R0, =VICVectPri0
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}    		
		
		LDR     R2, =VICVectAddr
		STR	    R3, [R2] 
		LDR     R2, =VICProtection
        MOV     R3, #0x01
        STR	    R3, [R2]
		BX      LR 
;/********************************************************************************************************
;** Function name:           SoftwareInterrupt
;** Descriptions:            ����жϣ������ṩVIC�������
;** input parameters:        �����ܶ���
;** output parameters:       �����ܶ���
;** Returned value:          �����ܶ���
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
		
SoftwareInterrupt			
        CMP     R0, #4
        LDRLO   PC, [PC, R0, LSL #2]
        MOVS    PC, LR

SwiFunction
        DCD     DisableIRQ       										;0
        DCD     EnableIRQ        										;1
        DCD		DisableFIQ		 										;2
        DCD		EnableFIQ		 										;3
;/*********************************************************************************************************
;** Function name:           DisableIRQ
;** Descriptions:            �ر�ȫ�� IRQ �ж�
;** input parameters:        none
;** output parameters:       none
;** Returned value:          none                          
;*********************************************************************************************************/
DisableIRQ
        MRS     R0, SPSR
        ORR     R0, R0, #NoInt
        MSR     SPSR_c, R0
        MOVS    PC, LR

;/*********************************************************************************************************
;** Function name:           EnableIRQ
;** Descriptions:            ��ȫ�� IRQ �ж�
;** input parameters:        none
;** output parameters:       none
;** Returned value:          none                          
;*********************************************************************************************************/
EnableIRQ
        MRS   	R0, SPSR
        BIC   	R0, R0, #NoInt
        MSR   	SPSR_c, R0
        MOVS    PC, LR
;/*********************************************************************************************************
;** Function name:           DisableFIQ
;** Descriptions:            �ر�ȫ�� FIQ �ж�
;** input parameters:        none
;** output parameters:       none
;** Returned value:          none                          
;*********************************************************************************************************/
DisableFIQ
        MSR		CPSR_c, #(SVC32Mode|NoFIQ)
        MRS     R0, SPSR
        ORR     R0, R0, #NoFIQ
        MSR     SPSR_c, R0
        MOVS    PC, LR

;/*********************************************************************************************************
;** Function name:           EnableFIQ
;** Descriptions:            ��ȫ�� FIQ �ж�
;** input parameters:        none
;** output parameters:       none
;*;* Returned value:          none                          
;*********************************************************************************************************/
EnableFIQ
        MSR		CPSR_c, #(SVC32Mode|NoFIQ)
        MRS  	R0, SPSR
        BIC   	R0, R0, #NoFIQ
        MSR   	SPSR_c, R0
        MOVS  	PC, LR

;/********************************************************************************************************
;** Function name:           vicControl
;** Descriptions:            ����жϣ������ṩVIC�������
;** input parameters:        �����ܶ���
;** output parameters:       �����ܶ���
;** Returned value:          �����ܶ���
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
vicControl 
        SUB     R0, R0, #0x100
        CMP     R0, #7
        LDRLO   PC, [PC, R0, LSL #2]
        MOVS    PC, LR
                
vicControlFuncAdd
        DCD     SetvicIrqFunc                                           ;  0
        DCD     ClrvicIrqFunc                                           ;  1
        DCD     EnablevicIrq                                            ;  2
        DCD     DisablevicIrq                                           ;  3
        DCD     GetvicIrqStatus                                         ;  4
        DCD     SetvicFiq                                               ;  5
        DCD     ClrvicFiq                                               ;  6 

;/*********************************************************************************************************
;** Function name:           SetvicIrqFunc
;** Descriptions:            ������ѡ������ж����ȼ����жϷ�������ַ����ʹ���ж�
;** input parameters:        R0:         �����Ӧ���ж�ͨ����
;**                          R1:         �ж����ȼ�
;**                          R2:         �жϷ�������ַ
;** output parameters:       none
;** Returned value:          1:          �ɹ�
;**                          0:          ʧ��
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************/

SetvicIrqFunc
        CMP     R1, #32                                                 ;  if (ͨ���� >=32) return FALSE
        MOVCS   R0, #0
        MOVCSS  PC, LR
        CMP     R2, #16                                                 ;  if (���ȼ� >=16) return FALSE
        MOVCS   R0, #0 
        MOVCSS  PC, LR 
        CMP     R3, #0                                                  ;  if (������ ==0) return FALSE
        MOVEQ   R0, #0
        MOVEQS  PC, LR
		     
 		MSR     CPSR_c, #(NoFIQ | NoInt | SYS32Mode)
        STMFD   SP!, {R2, R3}
		MOV     R2, #1                                                  ;  if (Enable) return FALSE
        MOV     R3, R2, lsl R1 
       
        LDR     R0, =VICIntEnable
        LDR     R2, [R0]
        ANDS    R2, R2, R3
        BNE     SetvicIrqFunc_j
        
        LDR     R0, =VICVectAddr0                                       ;  if (IRQ�Ѿ�ʹ��) return FALSE
        LDR     R3, [R0, R1, lsl #2]
        CMP     R3, #0         
SetvicIrqFunc_j
        LDMFD   SP!, {R2, R3}
        MSR     CPSR_c, #(NoFIQ | NoInt | SVC32Mode)
        MOVNE   R0, #0
        MOVNES  PC, LR 
        
        STR     R3, [R0, R1, lsl #2]
        LDR     R0, =VICVectPri0
        STR     R2, [R0, R1, lsl #2] 
        
        LDR     R0, =VICIntEnable
        MOV     R2, #1
        MOV     R3, R2, lsl R1 
        STR     R3, [R0]
                                              
        MOV     R0, #1
        MOVS    PC, LR   
;/*********************************************************************************************************
;** Function name:           ClrvicIrqFunc
;** Descriptions:            �����ѡ�����IRQ��Դ
;** input parameters:        R0:        �����Ӧ���ж�ͨ����
;** output parameters:       none
;** Returned value:          1:         �ɹ�
;**                          0:         ʧ��
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************/

ClrvicIrqFunc
        CMP     R1, #32 						                        ;  if (ͨ���� >=32) return FALSE
        MOVCS   R0, #0
		MOVCSS  PC, LR
		
		LDR     R0, =VICIntSelect      			                        ;  if (FIQ) return FALSE
        MOV     R3, #1
        MOV     R3, R3, LSL R1 
        LDR     R2, [R0]
        ANDS    R2, R2, R3        
        MOVNE   R0, #0
		MOVNES  PC, LR
        
        LDR     R0, =VICVectAddr0				                        ;  if (IRQ wasnt Set) return FALSE
        LDR     R2,[R0,R1,LSL #2] 
        CMP     R2,#0
        MOVEQ   R0, #0
		MOVEQS  PC, LR
                
		LDR     R0, =VICIntEnClr       			                        ;  Disable IRQ
        STR     R3, [R0]             
        
        LDR     R0, =VICVectAddr0      			                        ;  Clear VICVectAddr0
        MOV     R3, #0
        STR     R3, [R0, R1, lsl #2]        
        MOV     R0, #1        
		MOVS    PC, LR
;/*********************************************************************************************************
;** Function name:           EnablevicIrq
;** Descriptions:            ʹ����Ӧ������ж�
;** input parameters:        R0:         �����Ӧ���ж�ͨ����
;** output parameters:       none
;** Returned value:          1:          �ɹ�
;**                          0:          ʧ��
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************/

EnablevicIrq
        CMP     R1, #32                                                 ;  if (ͨ���� >=32) return FALSE
        MOVCS   R0, #0
        MOVCSS  PC, LR 
        
        LDR     R0, =VICVectAddr0                                       ;  if (δ����) return FALSE
        LDR     R3, [R0, R1, lsl #2]
        CMP     R3, #0
        MOVEQ   R0, #0
        MOVEQS  PC, LR
		
		LDR     R0, =VICIntEnable
        MOV     R2, #1
        MOV     R3, R2, lsl R1 
        STR     R3, [R0]
        MOV     R0, #1
        MOVS    PC, LR                

;/*********************************************************************************************************
;** Function name:           DisablevicIrq
;** Descriptions:            ��ֹ��Ӧ������ж�
;** input parameters:        uiChannel:  �����Ӧ���ж�ͨ����
;** output parameters:       none
;** Returned value:          1:          �ɹ�
;**                          0:          ʧ��
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

DisablevicIrq
        CMP     R1, #32                                                 ;  if (ͨ���� >=32) return FALSE
        MOVCS   R0, #0
        MOVCSS  PC, LR 
        
        LDR     R0, =VICVectAddr0                                       ;  if (δ����) return FALSE
        LDR     R3, [R0, R1, lsl #2]
        CMP     R3, #0
        MOVEQ   R0, #0
        MOVEQS  PC, LR
        
        LDR     R0, =VICIntEnClr
        MOV     R2, #1
        MOV     R3, R2, lsl R1 
        STR     R3, [R0]
        MOV     R0, #1
        MOVS    PC, LR
;/*********************************************************************************************************
;** Function name:           GetvicIrqStatus
;** Descriptions:            ��ȡ��ѡ������ж�ͨ���š����ȼ����жϷ�������ַ���ж�ʹ��״̬
;** input parameters:        R0:         �����Ӧ���ж�ͨ����
;** output parameters:       R2:         ������Ϣ
;** Returned value:          1:          �ɹ�
;**                          0:          ʧ��
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************/

GetvicIrqStatus
        CMP     R1, #32							                        ;  if (ͨ���� >=32) return FALSE
        MOVCS   R0, #0
		MOVCSS  PC, LR
        
        LDR     R0, =VICVectAddr0				                        ;  if (δ����) return FALSE
        LDR     R3, [R0, R1, lsl #2]
        CMP     R3, #0
        MOVEQ   R0, #0
		MOVEQS  PC, LR       
        STMIA   R2!, {R1}						                        ;  ͨ����

        LDR     R0, =VICVectPri0
        LDR     R0, [R0, R1, lsl #2]
        STMIA   R2!, {R0}       				                        ;  ���ȼ�
        STMIA   R2!, {R3}						                        ;  ��������ַ

        LDR     R0, =VICIntEnable
        MOV     R3, #1
        MOV     R1, R3, lsl R1 
        LDR     R3, [R0]
        AND     R3, R3, R1            
        CMP     R3, #0
        MOVHI   R3, #1
        STMIA   R2!,{R3}						                        ;  IRQʹ������״̬
        
		MOV     R0, #1
        MOVS    PC, LR        
;/*********************************************************************************************************
;** Function name:           SetvicFiq
;** Descriptions:            ���ò�ʹ����ѡ�ж�ͨ����ΪFIQ�ж�
;** input parameters:        R0:        �����Ӧ���ж�ͨ����
;** output parameters:       none
;** Returned value:          1:          �ɹ�
;**                          0:          ʧ��
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
SetvicFiq
       	MSR     CPSR_c, #(NoFIQ | NoInt| SVC32Mode)
       	
        CMP     R1, #32							                        ;  if (ͨ���� >=32) return FALSE
        MOVCS   R0, #0
		MOVCSS  PC, LR
        
        LDR     R0, =VICVectAddr0				                        ;  if (IRQ�Ѽ���) return FALSE
        LDR     R3,[R0,R1,LSL #2] 
        CMP     R3,#0
        MOVNE   R0, #0
		MOVNES  PC, LR
        
        LDR     R0, =VICIntSelect				
        LDR     R3, [R0]
        MOV     R2, #1
        MOV     R2, R2, lsl R1 
        ORR     R3, R3, R2                
        STR     R3, [R0]
        
        LDR     R0, =VICIntEnable
        STR     R3, [R0]
        MOV     R0, #1
        MOVS    PC, LR
;/*********************************************************************************************************
;** Function name:           ClrvicFiq
;** Descriptions:            �����ѡ�ж�ͨ���ŵ�FIQ�ж�
;** input parameters:        R0:         �����Ӧ���ж�ͨ����
;** output parameters:       none
;** Returned value:          1:          �ɹ�
;**                          0:          ʧ��;
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

ClrvicFiq
		MSR     CPSR_c, #(NoFIQ | NoInt| SVC32Mode)
		
        CMP     R1, #32							                        ;  if (ͨ���� >=32) return FALSE
        MOVCS   R0, #0
		MOVCSS  PC, LR       
             
        LDR     R0, =VICIntSelect				                        ;  if(FIQδʹ��)return FALSE
        LDR     R3, [R0]
        MOV     R2, #1
        MOV     R2, R2, LSL R1        
        ANDS    R2, R3, R2        
        MOVEQ   R0, #0
		MOVEQS  PC, LR         
       
        LDR     R0, =VICIntEnClr
        MOV     R3, R2 
        STR     R3, [R0]  						                        ;  Disable FIQ
        
        LDR     R0, =VICIntSelect
        LDR     R3, [R0]
        BIC     R3, R3,R2
        STR     R3, [R0]						       
        
        MOV     R0, #1
        MOVS    PC, LR            
                 
    END
;/********************************************************************************************************
;	End Of File
;********************************************************************************************************/