/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**                                      
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               vicControl.h
** Latest modified Date:    2008-04-21
** Latest Version:          1.0
** Descriptions:            VIC���ƴ���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Chenmingji
** Created date:            2008-04-21
** Version:                 1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef _VIC_CONTROL_H_
#define _VIC_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

/*********************************************************************************************************
  ���ݶ��壬�Ժ�Ҫʹ��
*********************************************************************************************************/
#define VIC_Config      VIC_IRQ_CFG
#define pVIC_Config     PVIC_IRQ_CFG

#define SetVICIRQ       vicIrqFuncSet
#define FreeVICIRQ      vicIrqFuncClr
#define ReEnableVICIRQ  vicIrqEnable
#define DisableVICIRQ   vicIrqDisable
#define GetVICIRQState  vicIrqStatusGet
#define SetVICFIQ       vicFiqSet
#define FreeVICFIQ      vicFiqClr

/*********************************************************************************************************
  VIC������Ϣ����
*********************************************************************************************************/
struct vic_irq_cfg {
    INT32U      ulChannel;                                              /*  ͨ��                        */
    INT32U      ulPri;                                                  /*  ���ȼ�                      */
    INT32U      ulFunctionAddr;                                         /*  ISR��ַ                     */
    INT32U      ulEnable;                                               /*  ʹ�ܱ�ʶ                    */
};

typedef struct vic_irq_cfg      VIC_IRQ_CFG;
typedef struct vic_irq_cfg     *PVIC_IRQ_CFG;

/*********************************************************************************************************
** Function name:           swiHandle1
** Descriptions:            SWI��������
** input parameters:        iHandle: �������ֹ���                          
** output parameters:       ���ݹ��ܾ���
** Returned value:          ���ݹ��ܾ���
*********************************************************************************************************/
__swi(0x00) void swiHandle1(int iHandle);

/*********************************************************************************************************
** Function name:           IRQDisable
** Descriptions:            �ر�ȫ�� IRQ �ж�
** input parameters:        none
** output parameters:       none
** Returned value:          none                          
*********************************************************************************************************/
__inline unsigned int IRQDisable() 
{
	swiHandle1(0);
}
/*********************************************************************************************************
** Function name:           IRQEnable
** Descriptions:            ��ȫ�� IRQ �ж�
** input parameters:        none
** output parameters:       none
** Returned value:          none                          
*********************************************************************************************************/
__inline unsigned int IRQEnable() 
{
	swiHandle1(1);
}
/*********************************************************************************************************
** Function name:           FIQDisable
** Descriptions:            �ر�ȫ�� FIQ �ж�
** input parameters:        none
** output parameters:       none
** Returned value:          none                          
*********************************************************************************************************/
__inline unsigned int FIQDisable() 
{
	swiHandle1(2);
}
/*********************************************************************************************************
** Function name:           FIQEnable
** Descriptions:            ��ȫ�� FIQ �ж�
** input parameters:        none
** output parameters:       none
** Returned value:          none                          
*********************************************************************************************************/
__inline unsigned int FIQEnable() 
{
	swiHandle1(3);
}

/*********************************************************************************************************
** Function name:           swiHandle
** Descriptions:            SWI��������
** input parameters:        iHandle: �������ֹ���
**                          ����:    ���ݹ��ܾ���
** output parameters:       ���ݹ��ܾ���
** Returned value:          ���ݹ��ܾ���
*********************************************************************************************************/
__swi(0x01) unsigned int swiHandle (int iHandle, unsigned int, unsigned int, unsigned int);

/*********************************************************************************************************
** Function name:           vicIrqFuncSet
** Descriptions:            ������ѡ������ж����ȼ����жϷ�������ַ����ʹ���ж�
** input parameters:        uiChannel:  �����Ӧ���ж�ͨ����
**                          uiPri:      �ж����ȼ�
**                          uiFuncAddr: �жϷ�������ַ
** output parameters:       none
** Returned value:          1:          �ɹ�
**                          0:          ʧ��
*********************************************************************************************************/
__inline unsigned int vicIrqFuncSet (unsigned int uiChannel,
                                     unsigned int uiPri,
                                     unsigned int uiFuncAddr)
{
    return swiHandle(0x100, uiChannel, uiPri, uiFuncAddr);
}

/*********************************************************************************************************
** Function name:           vicIrqFuncClr
** Descriptions:            �����ѡ�����IRQ��Դ
** input parameters:        uiChannel:  �����Ӧ���ж�ͨ����
** output parameters:       none
** Returned value:          1:          �ɹ�
**                          0:          ʧ��
*********************************************************************************************************/
__inline unsigned int vicIrqFuncClr (unsigned int uiChannel)
{
    return swiHandle(0x101, uiChannel ,0 ,0);
}

/*********************************************************************************************************
** Function name:           vicIrqEnable
** Descriptions:            ʹ����Ӧ������ж�
** input parameters:        uiChannel:  �����Ӧ���ж�ͨ����
** output parameters:       none
** Returned value:          1:          �ɹ�
**                          0:          ʧ��
*********************************************************************************************************/
__inline unsigned int vicIrqEnable (unsigned int uiChannel)
{
    return swiHandle(0x102, uiChannel, 0, 0);
}

/*********************************************************************************************************
** Function name:           vicIrqDisable
** Descriptions:            ��ֹ��Ӧ������ж�
** input parameters:        uiChannel:  �����Ӧ���ж�ͨ����
** output parameters:       none
** Returned value:          1:          �ɹ�
**                          0:          ʧ��
*********************************************************************************************************/
__inline unsigned int vicIrqDisable (unsigned int uiChannel)
{
    return swiHandle(0x103, uiChannel, 0, 0);
}

/*********************************************************************************************************
** Function name:           vicIrqStatusGet
** Descriptions:            ��ȡ��ѡ������ж�ͨ���š����ȼ����жϷ�������ַ���ж�ʹ��״̬
** input parameters:        uiChannel:  �����Ӧ���ж�ͨ����
** output parameters:       pvicInfo:   ������Ϣ
** Returned value:          1:          �ɹ�
**                          0:          ʧ��
*********************************************************************************************************/
__inline unsigned int vicIrqStatusGet (unsigned int uiChannel, PVIC_IRQ_CFG pvicInfo)
{
    return swiHandle(0x104, uiChannel, (unsigned int)pvicInfo, 0);
}

/*********************************************************************************************************
** Function name:           vicFiqSet
** Descriptions:            ���ò�ʹ����ѡ�ж�ͨ����ΪFIQ�ж�
** input parameters:        uiChannel:  �����Ӧ���ж�ͨ����
** output parameters:       none
** Returned value:          1:          �ɹ�
**                          0:          ʧ��
*********************************************************************************************************/
__inline unsigned int vicFiqSet (unsigned int uiChannel)
{
    return swiHandle(0x105, uiChannel, 0, 0);
}

/*********************************************************************************************************
** Function name:           vicFiqClr
** Descriptions:            �����ѡ�ж�ͨ���ŵ�FIQ�ж�
** input parameters:        uiChannel:  �����Ӧ���ж�ͨ����
** output parameters:       none
** Returned value:          1:          �ɹ�
**                          0:          ʧ��
*********************************************************************************************************/
__inline unsigned int vicFiqClr (unsigned int uiChannel)
{
    return swiHandle(0x106, uiChannel, 0, 0);
}

/*********************************************************************************************************
  �����ж�Ƕ�׺궨��
*********************************************************************************************************/
#define		OS_ENABLE_NESTING		IRQEnable
#define		OS_DISABLE_NESTING		IRQDisable

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  __TARGET_H                  */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

