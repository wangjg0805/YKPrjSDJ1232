//**********************************************************************
//�ļ���:eeprom.c
//�����б�:
//1.��EEPROM�ж�ȡһ������:Read_EEPROM
//2.��EEPROMָ����ַ��ȡһ�ֽ�����:Read_EEPROM_1B
//3.��EEPROMָ����ַ�ж�ȡ2�ֽ�:Read_EEPROM_2B
//4.��EEPROMָ����ַ�ж�ȡ4�ֽ�:Read_EEPROM_4B
//5.��EEPROMдһ������:Write_EEPROM
//6.��EEPROMд1�ֽ�����:Write_EEPROM_1B
//7.��EEPROMд2�ֽ�����:Write_EEPROM_2B
//8.��EEPROMд4�ֽ�����:Write_EEPROM_4B
//9.��ѯ�Ƿ���EEPROM��д����:Check_EEP_Wr_ERR
//10.����EEPROM�Ķ�д�Ƿ�����:EEPROM_Rw_Test
//**********************************************************************
#include "eeprom.h"
#include "port.h"
#include "stdio.h"
#include "global.h"
#include "i2c_com.h"
#include "timer.h"
#include "type_def.h"
#include "stdlib.h"
#include "lpc2400.h"

static uint8 flag_eep_wr_error = FALSE;        //дEEPROMʱ��������ı�־

/****************************************************************************
* ����: Read_EEPROM()
* ����: ��EEPROM��ȡһ������
* ��ڲ���: addr��ַ(11bit--EEPROM�Ĵ洢��ַ,����������ַ), rptr���ջ�����ָ
*            ��, rlen˳������ֽ���
* ���ڲ���: 0Ϊ����,��0���ʾ������
****************************************************************************/
uint8 Read_EEPROM(uint16 addr,  uint8 *rptr, uint16 rlen)
{
    uint16 end_addr;    //д�Ľ�����ַ(���һ���ֽڵĵ�ַ)
    uint16 bound_addr;    //��ı߽��ַ
    uint16 len;
    uint8 di;
    uint8 *rbuf_ptr;
    
    if (NULL == rptr) return(TRUE);
    if (!rlen)    return(TRUE);
    
    rbuf_ptr = rptr;
    end_addr = addr+rlen-1;
    while (addr <= end_addr)
    {
        bound_addr = addr|0x0f;
        if (end_addr > bound_addr)    //Խ��,�ֶ�ζ�ȡ
        {
            len = bound_addr-addr+1;
        }
        else
        {
            len = end_addr-addr+1;
        }
         di = (I2C_Rcv_Str(((addr>>7)&0x0E)|SYS24C08, addr, rbuf_ptr, len));
         if (di) break;
         addr += len;
         rbuf_ptr += len;
     }
     
     flag_eep_wr_error = di;    
     return (di);
}    

/****************************************************************************
* ����: Read_EEPROM_1B()
* ����: ��EEPROM��ȡ1�ֽ�(�ֽ�)
* ��ڲ���: addr��ַ(11bit), r1ptr�ֽ�ָ��
* ���ڲ���: 0Ϊ����,��0���ʾ������
****************************************************************************/
 uint8 Read_EEPROM_1B(uint16 addr,  uint8 *r1ptr)
{
    return(Read_EEPROM(addr, r1ptr, 1));
}

/****************************************************************************
* ����: Read_EEPROM_2B()
* ����: ��EEPROM��ȡ2�ֽ�(����)
* ��ڲ���: addr��ַ(11bit), r2ptr����ָ��
* ���ڲ���: 0Ϊ����,��0���ʾ������
****************************************************************************/
 uint8 Read_EEPROM_2B(uint16 addr, uint16 *r2ptr)
{
    return(Read_EEPROM(addr, ( uint8 *)r2ptr, 2));
}

/****************************************************************************
* ����: Read_EEPROM_4B()
* ����: ��EEPROM��ȡ4�ֽ�(��)
* ��ڲ���: addr��ַ(11bit), r4ptr��ָ��
* ���ڲ���: 0Ϊ����,��0���ʾ������
****************************************************************************/
 uint8 Read_EEPROM_4B(uint16 addr, uint32 *r4ptr)
{
    return(Read_EEPROM(addr, ( uint8 *)r4ptr, 4));
}

/****************************************************************************
* ���ƣ�Write_EEPROM()
* ���ܣ���EEPROMд��һ������
* ��ڲ�����addr��ַ(11bit--EEPROM�洢�ռ�ĵ�ַ,����������ַ), wptrд������
*            ָ��, wlen˳��д���ֽ���
* ���ڲ�����д��ȷ���(��0��ʾд����)
****************************************************************************/
 uint8 Write_EEPROM(uint16 addr,  uint8 *wptr, uint16 wlen)
{
    uint8 *rbuf,*wptr_temp;
    sint32 i,j;
    sint32 time;
    uint16 addr_temp,end_addr;    //д�Ľ�����ַ(���һ���ֽڵĵ�ַ)
    uint16 bound_addr;    //��ı߽��ַ
    uint16 len;
    
    if (NULL == wptr) return(TRUE);
    if (!wlen) return(TRUE);
    
    //if(wlen>128) wlen=128;
    rbuf = ( uint8*)malloc(wlen);    //�����ڴ�ռ�
    if (rbuf != NULL)    //�ڴ濪�ٳɹ�
    {
        if(Read_EEPROM(addr, rbuf, wlen)) 
        {    flag_eep_wr_error = 1;
            return(1);    //����
        }    
        for(i=0; i<wlen; i++)
        {//��ͷ��ʼ�Ƚϼ���д��Ͷ���������,�����ͬ��ǰ�ֽڲ�����д����
            if(rbuf[i]!=wptr[i]) break;
        }
        for(j=wlen-1; j>=0; j--)
        {//��β��ʼ�Ƚϼ���д��Ͷ���������,�����ͬ��ǰ�ֽڲ�����д����
            if(rbuf[j]!=wptr[j]) break;
        }
        if(i>j) 
        {
            free(rbuf);                //�ͷ��ڴ�
            flag_eep_wr_error = 0;
            return(0);                //����ͬ������д����
        }    
        addr += i;                        //���¼���д���ַ�ͳ���
        wptr += i;
        wlen = j - i + 1;
    }
    
    Macro_En_EEPROM();                    //��ֹд����

    wptr_temp = wptr;                    //�Ӹ�λ�ÿ�ʼдEEPROM
    addr_temp = addr;
    end_addr = addr+wlen-1;
    while (addr <= end_addr)
    {
        bound_addr = addr|0x0f;
        if (end_addr > bound_addr)    //Խ��,�ֶ�ζ�ȡ
        {
            len = bound_addr-addr+1;
        }
        else
        {
            len = end_addr-addr+1;
        }
        if(I2C_Send_Str(((addr>>7)&0x0E)|SYS24C08, addr, wptr, len))
        {//����
            Macro_Dis_EEPROM();                //����д����
            if (rbuf != NULL)
            {
                free(rbuf);                //�ͷ��ڴ�
            }
            flag_eep_wr_error = 1;
            return(1);
        }
        addr += len;
         wptr += len;
        //�ȴ�д����(5ms)
        time = Get_Cur_Time();
        for (i=0; i<60000; i++)
        {
            if (Cal_Time_Dif(time)>=5) break;
        }

     }        
    
    Macro_Dis_EEPROM();                    //����д����
    
    if (rbuf != NULL)
    {
        if(Read_EEPROM(addr_temp, rbuf, wlen)) 
        {
            free(rbuf);                //�ͷ��ڴ�
            return(1);        //����
        }        
        for(i=0; i<wlen; i++)
        {//�Ƚ�д��Ͷ���������,�����ͬ�򷵻ش���
            if(rbuf[i]!=wptr_temp[i]) 
            {
                free(rbuf);                //�ͷ��ڴ�
                return(1);
            }    
        }
    }
    
    if (rbuf != NULL)
    {
        free(rbuf);                //�ͷ��ڴ�
    }
    flag_eep_wr_error = 0;    
    return(0);
}

/****************************************************************************
* ���ƣ�Write_EEPROM_1B()
* ���ܣ���EEPROMд��1�ֽ�(�ֽ�)
* ��ڲ�����addr��ַ(11bit), w1bд���ֽ�
* ���ڲ�����д��ȷ���(��0��ʾд����)
****************************************************************************/
 uint8 Write_EEPROM_1B(uint16 addr,  uint8 w1b)
{
    return(Write_EEPROM(addr, &w1b, 1));
}

/****************************************************************************
* ���ƣ�Write_EEPROM_2B()
* ���ܣ���EEPROMд��2�ֽ�(����)
* ��ڲ�����addr��ַ(11bit), w2bд�����
* ���ڲ�����д��ȷ���(��0��ʾд����)
****************************************************************************/
 uint8 Write_EEPROM_2B(uint16 addr, uint16 w2b)
{
    return(Write_EEPROM(addr, ( uint8 *)(&w2b), 2));
}

/****************************************************************************
* ���ƣ�Write_EEPROM_4B()
* ���ܣ���EEPROMд��4�ֽ�(��)
* ��ڲ�����addr��ַ(11bit), w4bд����
* ���ڲ�����д��ȷ���(��0��ʾд����)
****************************************************************************/
 uint8 Write_EEPROM_4B(uint16 addr, uint32 w4b)
{
    return(Write_EEPROM(addr, ( uint8 *)(&w4b), 4));
}

//**********************************************************************
//����  ��:Check_EEP_Wr_ERR
//��������:��ѯEEPROM��д�Ƿ��д���
//��ڲ���:��
//���ڲ���:TRUE-- ����
//            FALSE--����
//**********************************************************************
uint8 Check_EEP_Wr_ERR(void)
{
    return (flag_eep_wr_error);
}

//**********************************************************************
//����  ��:EEPROM_Rw_Test
//��������:EEPROM��д���ܲ���
//��ڲ���:��
//���ڲ���:OK--����
//            ERROR--����
//**********************************************************************
uint8 EEPROM_para_config(uint8 addr,uint8 x)
{
    uint8  test_tx_buf[4];
    uint32 i,j;
    test_tx_buf[0] = REC_CHECK_DATA1; 
    test_tx_buf[1] = REC_CHECK_DATA2;
    test_tx_buf[2] = x;
    test_tx_buf[3] = test_tx_buf[0]+ test_tx_buf[1]+ test_tx_buf[2];
    
    I2C_Send_Str(SYS24C08, addr, test_tx_buf, 4);
    for(i=0;i<800000;i++)	{;}	                 //delay
	
	for(i=0;i<4;i++) test_tx_buf[i] = 0;	     //clear buf
	
	I2C_Rcv_Str(SYS24C08,  addr, test_tx_buf, 4);	
	for(i=0;i<80000;i++)	{;}					 //delay
    
    if(test_tx_buf[3] == (test_tx_buf[0]+test_tx_buf[1]+test_tx_buf[2]))
    return(OK);
    else
    return(ERROR);              
}
