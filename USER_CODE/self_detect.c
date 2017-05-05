//**********************************************************************
//�ļ�  ��:self_detect.c
//�����б�:
//1.��ʾ�Լ�������Ϣ:Put_SelfDetect_Err
//2.���CPU����:Check_CPU_Part
//3.�Լ�LCD:LCD_Selfdetect
//**********************************************************************
#include "self_detect.h"
#include "FontLib.H"
#include "disp.h"
#include "lcd2478.h"
//#include "lcd2478_cilib.h"

#include "stdio.h"
#include "port.h"
#include "global.h"

#if (LCDTYPE == TFTLCD)
#include "lpc2400.h"
#else
#include "lpc2294.h"
#endif 

#define MAX_COUNT_MP_OK                3        //���CPU�˿�����޴����3��

//**********************************************************************
//����  ��:Put_SelfDetect_Err
//��������:��ʾ�Լ�������Ϣ
//��ڲ���:err--�Լ�Ľ��
//���ڲ���:��
//**********************************************************************
void Put_SelfDetect_Err(uint32 err)
{
    sint32 di;
    uint8* str;
    Set_Graphic_Color(COL_DEEPRED,COL_WHITE);//���ñ���ɫ
    //Set_Write_Mode(WRITE_OR);
    Set_HZ_Lib(&ST_11);        //�����ֿ�
    Set_ASC_Lib(Arial10B_Font);
    Clr_Screen(DISP_NOR);    //�ñ���ɫ����
    Set_Chr_Color(COL_DEEPRED,COL_WHITE);//���ô���������ɫ����ɫ
    if (err&MCU_RAM_ERR_FLAG_BIT)//RAM����
    {
        str = Language_Str("RAM����!","RAM ERROR!");
        di = 160-Get_Str_Width(str)/2;
        Put_Line_Str(di,110,str);
    }
    else if (err&MCU_ROM_ERR_FLAG_BIT)//ROM����
    {
        str = Language_Str("ROM����!","ROM ERROR!");
        di = 160-Get_Str_Width(str)/2;                    
        Put_Line_Str(di,110,str);                    
    }
    else if (err&MCU_PORT_ERR_FLAG_BIT)//�˿ڴ���
    {
        str = Language_Str("CPU�˿ڳ���!","CPU Port Error!");
        di = 160-Get_Str_Width(str)/2;                        
        Put_Line_Str(di,110,str);                            
    }
    else if (err&LCD_ERR_FLAG_BIT)//LCD�Լ����
    {
        str = Language_Str("LCD����!","LCD ERROR!");
        di = 160-Get_Str_Width(str)/2;                        
        Put_Line_Str(di,110,str);            
    }
}    

/****************************************************************************
* ���ƣ�Check_CPU_Part()
* ���ܣ����CPU����
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
#define RAM_SIZE    0x8000        //�ڲ�RAMΪ32K
#define RAM_ARRAY    ((volatile sint32 *) 0x40000000)    //�ڲ�RAM����ָ��
#define ROM_ARRAY    ((volatile sint32 *) 0x0)        //�ڲ�ROM����ָ��
//extern sint32 Image$$RO$$Limit;    //�����ĩβ(�ɱ���������)
//extern sint32 Image$$RW$$Base;    //RAM����ʼ(�ɱ���������)
//extern sint32 Image$$ZI$$Base;    //��������ʼ(�ɱ���������)

sint16 Check_CPU_Part(void)
{
uint8 flag_mp_err;
sint32 i,d,cnt;
#ifndef _DEBUG
sint32 m;
#endif
sint16 part_err_flag = 0;
uint32 int_bak;

    int_bak = VICIntEnable;                //���ж�ʹ��״̬
    VICIntEnClr = int_bak;                //����
/*    
    cnt = 0;
    for(d=0; d<10; d++)
    {//���10��
        flag_mp_err = FALSE;
        // ����138����ΪƬѡ��3���� 
        for(i=0; i<8; i++)
        {
            Macro_Wr_EO_Addr(i);
            if (EXT_OUTP1_ADDR == i)
            {
                Macro_Wr_EO_Bus(ext_outport1);    //��ֹ���ı��ⲿ��չ�ڵ�״̬
            }
            else if (EXT_OUTP2_ADDR == i)
            {
                Macro_Wr_EO_Bus(ext_outport2);    //��ֹ���ı��ⲿ��չ�ڵ�״̬
            }
            else if (EXT_OUTP3_ADDR == i)
            {
                Macro_Wr_EO_Bus(ext_outport3);    //��ֹ���ı��ⲿ��չ�ڵ�״̬
            }            
            Macro_Wr_EO_Addr(i);
            Macro_Wr_Drv_Key(i);
            Macro_Wr_Drv_Key(i);
            if((Macro_Rd_EO_Addr() != i) || (Macro_Rd_Drv_Key() != i))
                flag_mp_err = TRUE;
            IO0SET = P0_EXT_BUS_ADDR | P0_SCAN_DRV_OUT;//��ʼ��Ϊ�ߵ�ƽ
            IO0SET = P0_EXT_BUS_DATA;//��ʼ��Ϊ�ߵ�ƽ
        }
        // ���8λ�������� 
        IO0DIR |= P0_EXT_BUS_DATA;        //���߿���Ϊ���
        for(i=0; i<256; i++)
        {
            Macro_Wr_EO_Bus(i);
            Macro_Wr_EO_Bus(i);
            Macro_Wr_EO_Bus(i);
            if(Macro_Rd_EO_Bus()!=i) flag_mp_err = TRUE;
            IO0SET = P0_EXT_BUS_DATA;//��ʼ��Ϊ�ߵ�ƽ
        }
        // ������������ 
        //if(Check_P0_Pin(P0_HC164_DATA1)) flag_mp_err = TRUE;    //�������,������
        if(Check_P1_Pin(P1_WP)) flag_mp_err = TRUE;
        if(Check_P1_Pin(P1_LCD_EN)) flag_mp_err = TRUE;
        if(Check_P1_Pin(P1_LCD_CS)) flag_mp_err = TRUE;
        if(Check_P1_Pin(P1_INLET_V)) flag_mp_err = TRUE;
        if(Check_P1_Pin(P1_EXP_V)) flag_mp_err = TRUE;
        if(flag_mp_err) cnt++;
    }
    if(cnt > MAX_COUNT_MP_OK) part_err_flag |= MCU_PORT_ERR_FLAG_BIT;*/
    
/* ��ʼʱΪ����״̬ */
    Valve_Action_in_Exp();
/* ���Ƭ��RAM */
    for(i=0; i<RAM_SIZE/4; i++)
    {
        d = RAM_ARRAY[i];        //����ԭ������
        RAM_ARRAY[i] = 0x55AA55AA;
        if(RAM_ARRAY[i] != 0x55AA55AA) part_err_flag |= MCU_RAM_ERR_FLAG_BIT;
        RAM_ARRAY[i] = 0xAA55AA55;
        if(RAM_ARRAY[i] != 0xAA55AA55) part_err_flag |= MCU_RAM_ERR_FLAG_BIT;
        RAM_ARRAY[i] = d;        //�ָ�ԭ������
        if(part_err_flag&MCU_RAM_ERR_FLAG_BIT) break;
    }
    
/* ���Ƭ��ROM���� */
/*#ifndef _DEBUG
    m = ((sint32)(&(Image$$RO$$Limit)) +
        (sint32)(&(Image$$ZI$$Base)) -
        (sint32)(&(Image$$RW$$Base))) / 4;    //�����γ���(��)
    d = 0;
    //for(i=0; i<m; i++) d += ROM_ARRAY[i];
    //if(d != ROM_ARRAY[m]) flag_mcu_rom_error = TRUE;
#endif*/

    VICIntEnable = int_bak;        //����  
return (part_err_flag);
}

//**********************************************************************
//����  ��:LCD_Selfdetect
//��������:�Լ�LCD����LCD��ȡ��������ʾ�����������ݶԱ�.
//��ڲ���:��
//���ڲ���:�Լ���
//**********************************************************************
sint16 LCD_Selfdetect(void)
{
    sint32 di,count=0;
   #if (LCDTYPE == TFTLCD)
    return (0);
   #else  
    Set_Panel_Addr(SEC_LAY_HPTR+0);        //�����Դ����ַ,��ϵͳ����ʾ����õڶ���
    LCDBus_Write(CMD_TO_LCD,CRSDIR_INC_BY_ONE_CMD);    //���ù���ַ��������ʽ        
    LCDBus_Write(CMD_TO_LCD,MREAD_CMD);    //���Դ�
    for (di=0; di<DISP_BUF_BYTES; di++)
    {
        if (LCDBus_Read(DATA_ADDR_TO_MCU) != disp_buffer[di])//�Ƚ���ʾ���������Դ�
        {
            if(DISP_BUF_BYTES/16 == ++count) //"1/16"�����ݴ���
            {
                return(LCD_ERR_FLAG_BIT);
            }
        }    
    }
    return(0);    
  #endif
}