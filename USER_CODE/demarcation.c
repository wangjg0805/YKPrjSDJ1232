//**********************************************************************
//�ļ���:demarcation.c
//**********************************************************************
#include "demarcation.h"
#include "FontLib.h"

//#include "lcd2478_graph.h"
#include "lcd2478.h"
#include "disp.h"
#include "graph.h"
#include "key_scan.h"
#include "timer.h"
#include "global.h"
#include "menu.h"
#include "data_pro.h"
#include "i2c_com.h"
#include "speaker.h"
#include "eeprom.h"
#include "ad_acquire.h"
#include "type_def.h"
#include "port.h"
#include "stdio.h"
#include "data_pro.h"
#include "alarm_pro.h"
#include "sys_para.h"

#define INC_DEC_LONG    10
//�޸Ĳ���
static void Demar_Change_Paw_AD(uint8 dir, uint8 step,sint8 point);
static void Demar_Change_O2_AD(uint8 dir, uint8 step,sint8 point);
static void Demar_Change_VT_Lps(uint8 dir, uint8 step,sint8 point);

static void Demar_Put_Ad_Req_Frame(void);
static void Put_Paw_AD(uint8 disp_mode);
static void Put_Paw_Filt_Len(uint8 disp_mode);
static void Put_Paw_Filt_Band(uint8 disp_mode);
static void Put_O2_AD(uint8 disp_mode);
static void Put_O2_Filt_Len(uint8 disp_mode);
static void Put_O2_Filt_Band(uint8 disp_mode);
static void Put_Batt_V_AD(uint8 disp_mode);
static void Put_Batt_V_Filt_Len(uint8 disp_mode);
static void Put_Batt_V_Filt_Band(uint8 disp_mode);
static void Put_Batt_I_AD(uint8 disp_mode);
static void Put_Batt_I_Filt_Len(uint8 disp_mode);
static void Put_Batt_I_Filt_Band(uint8 disp_mode);
static void Put_Vref_AD(uint8 disp_mode);
static void Put_Vref_Filt_Len(uint8 disp_mode);
static void Put_Vref_Filt_Band(uint8 disp_mode);
static void Put_Vt_Loops(uint8 disp_mode);
static void Put_Vt_Filt_Len(uint8 disp_mode);
static void Put_Vt_Filt_Band(uint8 disp_mode);
static uint16 Demar_Ad_Req_Pro(uint16 menu_option);

//"ѡ��/��ť/����"��ʾ����
static void Put_Paw_Dem_Butt( uint8 disp_mode);
static void Put_O2_Dem_Butt( uint8 disp_mode);
static void Put_Vt_Dem_Butt( uint8 disp_mode);
static void Put_Pre_Butt(uint8 disp_mode);
static void Put_Next_Butt(uint8 disp_mode);
static void Put_Check_Cur_Butt(uint8 disp_mode);
static void Put_Save_Butt(uint8 disp_mode);
static void Put_Ret_Butt(uint8 disp_mode);
static void Put_Paw_Dem_Point_Box(uint8 disp_mode,sint32 para);
static void Put_O2_Dem_Point_Box(uint8 disp_mode,sint32 para);
static void Put_Vt_Dem_Point_Box(uint8 disp_mode,sint32 para);
static void Put_Paw_Ad_In_Box(uint8 disp_mode,uint16 para);
static void Put_O2_Ad_In_Box(uint8 disp_mode,uint16 para);
static void Put_Vt_Lps_In_Box(uint8 disp_mode,sint32 para);
static void Put_Paw_Ad_Val( uint8 disp_mode,uint16 ad);
static void Put_O2_Ad_Val( uint8 disp_mode,uint16 ad);
static void Put_Cur_Pressur_Dem_Paw( uint8 disp_mode,sint32 para);
static void Put_Cur_O2_Dem_O2( uint8 disp_mode,sint32 para);
static void Put_Zero_Val_Dem_Paw( uint8 disp_mode,sint16 para);
static void Put_O2_Cell_Volt_Dem_O2( uint8 disp_mode,sint32 para);
static void Put_Vt_Lps_Val( uint8 disp_mode,sint32 para);
static void Put_Cur_Vt( uint8 disp_mode,sint32 para);
static void Put_Vol_Axis_Dem_Paw( uint8 disp_mode);
static void Put_Press_Axis_Dem_Paw( uint8 disp_mode);
static void Put_Paw_Sen_Cur(sint8 curve, uint8 disp_mode);
static void Put_Paw_Dem_Dot(sint8 point, uint8 disp_mode);
static void Put_Vol_Axis_Dem_O2( uint8 disp_mode);
static void Put_O2_Axis_Dem_O2( uint8 disp_mode);
static void Put_O2_Sen_Cur(sint8 curve, uint8 disp_mode);
static void Put_O2_Dem_Dot(sint8 point, uint8 disp_mode);
static void Put_Lps_Axis_Dem_Vt( uint8 disp_mode);
static void Put_Vt_Axis_Dem_Vt( uint8 disp_mode);
static void Put_Vt_Sen_Cur(sint8 curve, uint8 disp_mode);
static void Put_Vt_Dem_Dot(sint8 point, uint8 disp_mode);
static void Put_Br_Icon_Dem(uint8 disp_mode);

//�˵������ʼ������
static void Init_Dem_Main_Menu(sint32 op);
static void Init_Paw_Dem_Menu(sint32 op);
static void Init_O2_Dem_Menu(sint32 op);
static void Init_Vt_Dem_Menu(sint32 op);
//���洦����
static sint32 Menu_Lg_Sel_Op_Dem_Pro(uint8 key_c,sint32 op);
static sint32 Dem_Sel_Menu_Pro(uint8 key_c,sint32 op);
static sint32 Paw_Dem_Menu_Pro(uint8 key_c,sint32 op);
static sint32 O2_Dem_Menu_Pro(uint8 key_c,sint32 op);
static sint32 Vt_Dem_Menu_Pro(uint8 key_c,sint32 op);
//ѡ�����
static sint32 Dem_En_Butt_Pro(uint8 key_c);
static sint32 Dem_Ch_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Butt_Pro(uint8 key_c);
static sint32 O2_Sen_Dem_Butt_Pro(uint8 key_c);
static sint32 Vt_Sen_Dem_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Ret_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Save_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Check_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Next_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Pre_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Ad_In_Box_Pro(uint8 key_c);
static sint32 O2_Dem_Ret_Butt_Pro(uint8 key_c);
static sint32 O2_Dem_Save_Butt_Pro(uint8 key_c);
static sint32 O2_Dem_Check_Butt_Pro(uint8 key_c);
static sint32 O2_Dem_Next_Butt_Pro(uint8 key_c);
static sint32 O2_Dem_Pre_Butt_Pro(uint8 key_c);
static sint32 O2_Dem_Ad_In_Box_Pro(uint8 key_c);
static sint32 Vt_Dem_Ret_Butt_Pro(uint8 key_c);
static sint32 Vt_Dem_Save_Butt_Pro(uint8 key_c);
static sint32 Vt_Dem_Check_Butt_Pro(uint8 key_c);
static sint32 Vt_Dem_Next_Butt_Pro(uint8 key_c);
static sint32 Vt_Dem_Pre_Butt_Pro(uint8 key_c);
static sint32 Vt_Dem_Lps_In_Box_Pro(uint8 key_c);
//��ʾ������
static void Dem_Lg_Sel_Disp_Pro(void);
static void Dem_Sel_Disp_Pro(void);
static void Paw_Dem_Disp_Pro(void);
static void O2_Dem_Disp_Pro(void);
static void Vt_Dem_Disp_Pro(void);

//��ʾ��Ϣ������
#define CURVE_ERR_TIPS_DEM            0x0001    //���������ߴ�����ʾ
#define CURVE_OK_TIPS_DEM            0x0002    //������������ȷ
#define SAVE_ERR_TIPS_DEM            0x0003    //��������ʱ����
#define SAVE_OK_TIPS_DEM            0x0004    //����ɹ�
#define AD_ERR_TIPS_DEM                0x0005    //ADֵ������Χ
#define LPS_ERR_TIPS_DEM            0x0006    //����Ȧ��������Χ
#define TIPS_DISP_TIME_DEM        150    //�ڱ궨ʱ��ʾ��Ϣ����ʾʱ��
Tips_Struct*  cur_tips_ptr_dem = &blank_tips;    //��ǰ��ʾ��Ϣ��ָ��
static sint16 tips_disp_time_dem;    //��ʾ��Ϣ��ʾ��ʱ��
static uint8  flag_tips_saved;    //�Ѿ����汻��������ı�־
static uint8  tips_disp_trigger_dem;    //������ʾ��Ϣ����ʾ
static uint32 save_buf_dem[LINE_SIZE*SCR_YSIZE/4];//����ʾ��Ϣ���ǵ���ʾ������ʱ����Ļ�����
//�궨ģʽ�µ���ʾ��Ϣ
static Tips_Struct    curve_err_tips_dem = {CURVE_ERR_TIPS_DEM,"���������ߴ���,�����±궨!","Sensor Curve Error!\nPlease Demarcate Again!",NULL};
static Tips_Struct    curve_ok_tips_dem = {CURVE_OK_TIPS_DEM,"������������������Χ��!","Sensor Curve In Proper Range!",NULL};
static Tips_Struct    save_err_tips_dem = {SAVE_ERR_TIPS_DEM,"��������ʱ����!�����±���!","Save Error!\nPlease Save Again!",NULL};
static Tips_Struct    save_ok_tips_dem = {SAVE_OK_TIPS_DEM,"����ɹ�!","Save Successful!",NULL};
static Tips_Struct    ad_err_tips_dem = {AD_ERR_TIPS_DEM,"ADֵ������Χ!\n�����¼����ٱ���!","AD Value Exceed!\nPlease Check And Save Again!",NULL};
static Tips_Struct    lps_err_tips_dem = {LPS_ERR_TIPS_DEM,"����Ȧ��������Χ!\n�����¼����ٱ���!","Fan Laps Exceed!\nPlease Check And Save Again!",NULL};
static void Log_Tips_Dem(Tips_Struct*tips_ptr);
static void Cancel_Tips_Dem(Tips_Struct*tips_ptr);
static void Put_Tips_Mess_Dem(void);
static uint8 Check_Tips_Disp_Dem(void);

static uint8 dem_op_st;
static uint8 cur_demar_point; 
//**********************************************************************
//����  ��:Demar_Manage
//��������:�궨ģʽ�µĲ˵��л�����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void Demar_Manage(void)
{
    struct menu_str menu[DEMAR_MENU_NUM] =
    {{MENU_DEMAR_LG_SEL,Init_LG_Sel_Menu,    Menu_Lg_Sel_Op_Dem_Pro,    Dem_Lg_Sel_Disp_Pro,NULL},//����ѡ�����
    {DEMAR_MAIN_MENU,    Init_Dem_Main_Menu,    Dem_Sel_Menu_Pro,        Dem_Sel_Disp_Pro,    NULL},//������
    {DEMAR_NEW_PAW_CUR,    Init_Paw_Dem_Menu,    Paw_Dem_Menu_Pro,        Paw_Dem_Disp_Pro,    NULL},//ѹ���������궨
    {DEMAR_O2_SEN,        Init_O2_Dem_Menu,    O2_Dem_Menu_Pro,        O2_Dem_Disp_Pro,    NULL},//�����������궨
    {DEMAR_VT,            Init_Vt_Dem_Menu,    Vt_Dem_Menu_Pro,        Vt_Dem_Disp_Pro,    NULL}};// �������궨
    sint32 cur_op,next_op;    //��ǰѡ������л�����ѡ��(bits[7:0]:ѡ���;bits[15:8]:�����;bits[23:16]:ѡ�����ʾ״̬)
    uint8 key_c;            //������
    uint8 lcd_ref_interval = 0;
    sint16 lcd_line=0;    //LCD ��ʱˢ�µ���ʼˢ������
        
    Set_Key_Sound_Switch(ON);    //�궨ģʽ�°�������
    cur_op =0xffffffff ;  
    next_op = MENU_DEMAR_LG_SEL_CH_OP;
    while (1)
    {
        if ((cur_op^next_op)&0x0000ff00)//��ǰ�����Ľ����׼���л����Ľ��治һ��
        {//��ʼ������
            cur_demar_point = 0;    //��һ���궨��
            (*(menu[(next_op>>8)&0xff].menu_init))(next_op);
        }
        cur_op = next_op;        //"����/�˵�/ѡ��"֮����л�
        
        if (flag_10ms_ok)
        {
            flag_10ms_ok = FALSE;
            key_c = Get_Key_Code();        //����ɨ��
            next_op = (*(menu[(cur_op>>8)&0xff].op_pro))(key_c,cur_op);    //ѡ���        
            Put_Tips_Mess_Dem();    //��ʾ��ʾ��Ϣ
            //Check_Extend_Port();    //����ⲿ��չ��
            Check_Batt_Supply_Status();    //����ص�״̬                
        }    
        
        if (flag_100ms_ok)
        {
            flag_100ms_ok = FALSE;
            (*(menu[(cur_op>>8)&0xff].disp_pro))();    //��ʾ����    
            //��ʾ����/�ֶ�������
            if(phase_vent_ctrl == PHASE_NO_VENT)
            {
                LED_Indicate_Off(EO2_LED_VENT);
                LED_Indicate_On(EO2_LED_BAG);
            }
            else
            {
                LED_Indicate_Off(EO2_LED_BAG);
                LED_Indicate_On(EO2_LED_VENT);
            }
            LED_Indicate_Show();
            Batt_Alarm_Pro();    
            lcd_ref_interval++;
            lcd_ref_interval &= 0x01;
            if (!lcd_ref_interval)
            {        
                LCD_Refresh_Area(0,lcd_line,319,lcd_line+23);//��ʱˢ��LCD
                lcd_line += 24;
                if (lcd_line > 216)    //��ͷˢ��
                {
                    lcd_line = 0; 
                }
            }                        
        }
        
        if (flag_500ms_ok)
        {//500msʱ�䵽
            flag_500ms_ok = FALSE;
            
            LED_Indicate_Cpl(EO2_LED_CPU_S); //CPU����ָʾ����˸Ƶ��1Hz
            LED_Indicate_Show();
        }        
    }
}


//**********************************************************************
//����  ��:Demar_Change_Paw_AD
//��������:�궨ʱ�޸�ѹ��ADֵ
//��ڲ���:dir--����(1:����;0:����)
//           step--(LONG_STEP:����10;����:����1)
//           point--�޸ĵ� 
//���ڲ���:��
//**********************************************************************
static void Demar_Change_Paw_AD(uint8 dir, uint8 step,sint8 point)
{
    sint32 di;
    if (point < 0) return;
    if (point > PAW_DEMAR_POINTS-1) return;
    if (dir)    //����
    {
        if (step == INC_DEC_LONG)
        {
            paw_ad_tab[point] += 10;
        }
        else
        {
            paw_ad_tab[point]++;
        }
    }
    else
    {
        if (step == INC_DEC_LONG)
        {
            paw_ad_tab[point] -= 10;
        }
        else
        {
            paw_ad_tab[point]--;
        }        
    }
    di = Check_Pipeline_P_AD(point,paw_ad_tab[point]);    //����޸ķ�Χ
    if (!(di&0xff000000))    //��Χ��ȷ,������������
    {
        Key_Valid_Sound();    //��������Ч��
    }
    else//��Χ����,�������쳣�����޸�ADֵ
    {
        paw_ad_tab[point] = di&0xffff;
        Key_No_Valid_Sound();    //��������Ч��
    }
}

//**********************************************************************
//����  ��:Demar_Change_O2_AD
//��������:�궨ʱ�޸�����ADֵ
//��ڲ���:dir--����(1:����;0:����)
//           step--(LONG_STEP:����10;����:����1)
//           point--�޸ĵ� 
//���ڲ���:��
//**********************************************************************
static void Demar_Change_O2_AD(uint8 dir, uint8 step,sint8 point)
{
    sint32 di;
    if (point < 0) return;
    if (point > 1) return;    
    if (dir)    //����
    {
        if (step == INC_DEC_LONG)
        {
            o2_ad_tab[point] += 10;
        }
        else
        {
            o2_ad_tab[point]++;
        }
    }
    else
    {
        if (step == INC_DEC_LONG)
        {
            o2_ad_tab[point] -= 10;
        }
        else
        {
            o2_ad_tab[point]--;
        }        
    }
    di = Check_O2_AD(point,o2_ad_tab[point]);    //����޸ķ�Χ
    if (!(di&0xff000000))    //��Χ��ȷ,������������
    {
        Key_Valid_Sound();    //��������Ч��
    }
    else//��Χ����,�������쳣�����޸�ADֵ
    {
        Key_No_Valid_Sound();    //��������Ч��
        o2_ad_tab[point] = di&0xffff;
    }
}

//**********************************************************************
//����  ��:Demar_Change_VT_Lps
//��������:�궨ʱ�޸ĳ�����Ȧ��
//��ڲ���:dir--����(1:����;0:����)
//           step--(LONG_STEP:����10;����:����1)
//           point--�޸ĵ� 
//���ڲ���:��
//**********************************************************************
static void Demar_Change_VT_Lps(uint8 dir, uint8 step,sint8 point)
{
    sint32 di;
    if (point < 0) return;
    if (point > VT_DEMAR_POINTS-1) return;    
    if (dir)    //����
    {
        if (step == INC_DEC_LONG)
        {
            vt_lap_tab[point] += 10;
        }
        else
        {
            vt_lap_tab[point]++;
        }
    }
    else
    {
        if (step == INC_DEC_LONG)
        {
            vt_lap_tab[point] -= 10;
        }
        else
        {
            vt_lap_tab[point]--;
        }        
    }
    di = Check_Vt_Laps(point,vt_lap_tab[point]);    //����޸ķ�Χ
    if (!(di&0xff000000))    //��Χ��ȷ,������������
    {
        Key_Valid_Sound();    //��������Ч��
    }
    else//��Χ����,�������쳣�����޸�Ȧ��
    {
        Key_No_Valid_Sound();    //��������Ч��
        vt_lap_tab[point] = di&0xffff;
    }
}

//**********************************************************************
//����  ��:Demar_Put_Ad_Req_Frame
//��������:�ڱ궨ģʽ����ʾAD��ѯ����Ŀ��
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Demar_Put_Ad_Req_Frame(void)
{
    sint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_WHITE);
    Set_Chr_Color(COL_BLACK,COL_WHITE);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Clr_Screen(DISP_NOT_RT);    //�ñ���ɫ����
    Rectangle(5,5,314,234);    //�����
                                //��ѯ�����
    Rectangle(8,40,312,201);
    for (di=0; di<6; di++)
    {
        Hor_Line(9,311,63+di*23);
    }
    Ver_Line(41,200,83);
    Ver_Line(41,200,122);        
    Ver_Line(41,200,202);    
    
    Hor_Line(6,313,31);        //����
    Pattern_Bar(6,6,313,30,0xaa);
    Bar(95,9,221,27);
    Set_Write_Mode(WRITE_EXOR);//���ģʽ
    Set_HZ_Lib(&ST_11);        //�����ֿ�
    Set_ASC_Lib(Arial10B_Font);
    Put_Line_Str(126,11,"AD Require");
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Line_Str(15,46,"Channel");                
    Put_Line_Str(94,46,"AD");    
    Put_Line_Str(134,46,"Filt Len");
    Put_Line_Str(220,46,"Filt Band");            
    Put_Line_Str(22,68,"Paw");        
    Put_Line_Str(22,92,"O2");        
    Put_Line_Str(22,114,"Batt_V");    
    Put_Line_Str(22,138,"Batt_I");    
    Put_Line_Str(22,159,"Vref");
    Put_Line_Str(22,184,"Vt");
    
    Put_Paw_AD(DISP_RT);        
    Put_O2_AD(DISP_RT);
    Put_Batt_V_AD(DISP_RT);        
    Put_Batt_I_AD(DISP_RT);
    Put_Vref_AD(DISP_RT);
    Put_Vt_Loops(DISP_RT);
    
    Put_Paw_Filt_Len(DISP_RT);        
    Put_O2_Filt_Len(DISP_RT);
    Put_Batt_V_Filt_Len(DISP_RT);        
    Put_Batt_I_Filt_Len(DISP_RT);
    Put_Vref_Filt_Len(DISP_RT);
    Put_Vt_Filt_Len(DISP_RT);        
    
    Put_Paw_Filt_Band(DISP_RT|DISP_INVERSE);        
    Put_O2_Filt_Band(DISP_RT);
    Put_Batt_V_Filt_Band(DISP_RT);        
    Put_Batt_I_Filt_Band(DISP_RT);
    Put_Vref_Filt_Band(DISP_RT);
    Put_Vt_Filt_Band(DISP_RT);
                                
    LCD_Refresh_Area(0,0,319,239);    //ȫ��ˢ��    
}

//**********************************************************************
//������:Put_Paw_AD
//��������:��ʾPAW��ADֵ
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Paw_AD(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(84,64,121,85);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(88,68,paw_ad,1);
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(84,64,121,85);
    }    
}

//**********************************************************************
//������:Put_Paw_Filt_Len
//��������:��ʾPAW���˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Paw_Filt_Len(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(123,64,201,85);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(135,68,PAW_AD_FILT_LEN,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,64,201,85);
    }
}

//**********************************************************************
//������:Put_Paw_Filt_Band
//��������:��ʾPAW���˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Paw_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(203,64,311,85);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,65,310,84);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//���ģʽ
    }else
    {    
        Set_Write_Mode(WRITE_OR);//��ģʽ
    }
    Put_Numeral(220,68,paw_ad_filt_band,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,64,311,85);
    }
}

//**********************************************************************
//������:Put_O2_AD
//��������:��ʾO2��ADֵ
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
void Put_O2_AD(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(84,64,121,85);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(88,92,o2_ad,1);    
    if (disp_mode&DISP_RT)
    {        
        LCD_Refresh_Area(84,87,121,108);
    }
}

//**********************************************************************
//������:Put_O2_Filt_Len
//��������:��ʾO2���˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_O2_Filt_Len(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(123,87,201,108);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(135,92,O2_AD_FILT_LEN,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,87,201,108);
    }
}

//**********************************************************************
//������:Put_O2_Filt_Band
//��������:��ʾO2���˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_O2_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(203,87,311,108);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,88,310,107);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//���ģʽ
    }else
    {    
        Set_Write_Mode(WRITE_OR);//��ģʽ
    }
    Put_Numeral(220,92,VALID_O2_AD_SCALE,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,87,311,108);
    }
}

//**********************************************************************
//������:Put_Batt_V_AD
//��������:��ʾ��ص�ѹ��ADֵ
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Batt_V_AD(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(84,110,121,131);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(88,114,batt_v_ad,1);    
    if (disp_mode&DISP_RT)
    {        
        LCD_Refresh_Area(84,110,121,131);
    }
}

//**********************************************************************
//������:Put_Batt_V_Filt_Len
//��������:��ʾ��ص�ѹ���˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Batt_V_Filt_Len(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(123,110,201,131);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(135,114,BATT_V_AD_FILT_LEN,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,110,201,131);
    }
}

//**********************************************************************
//������:Put_Batt_V_Filt_Band
//��������:��ʾ��ص�ѹ���˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Batt_V_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(203,110,311,131);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,111,310,130);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//���ģʽ
    }else
    {    
        Set_Write_Mode(WRITE_OR);//��ģʽ
    }
    Put_Numeral(220,114,BATT_V_AD_FILT_BAND,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,110,311,131);
    }
}

//**********************************************************************
//������:Put_Batt_I_AD
//��������:��ʾ��ص�����ADֵ
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Batt_I_AD(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(84,133,121,154);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(88,138,batt_i_ad,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(84,133,121,154);
    }
}

//**********************************************************************
//������:Put_Batt_I_Filt_Len
//��������:��ʾ��ص������˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Batt_I_Filt_Len(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(123,133,201,154);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(135,138,BATT_I_AD_FILT_LEN,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,133,201,154);
    }
}

//**********************************************************************
//������:Put_Batt_I_Filt_Band
//��������:��ʾ��ص������˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Batt_I_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(203,133,311,154);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,134,310,153);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//���ģʽ
    }else
    {    
        Set_Write_Mode(WRITE_OR);//��ģʽ
    }
    Put_Numeral(220,138,BATT_I_AD_FILT_BAND,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,133,311,154);
    }
}

//**********************************************************************
//������:Put_Vref_AD
//��������:��ʾ�ο���ѹ��ADֵ
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Vref_AD(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(84,156,121,177);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(88,159,vref_ad,1);    
    if (disp_mode&DISP_RT)
    {        
        LCD_Refresh_Area(84,156,121,177);
    }
}

//**********************************************************************
//������:Put_Vref_Filt_Len
//��������:��ʾ�ο���ѹ���˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Vref_Filt_Len(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(123,156,201,177);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(135,159,VREF_AD_FILT_LEN,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,156,201,177);
    }
}

//**********************************************************************
//������:Put_Vref_Filt_Band
//��������:��ʾ�ο���ѹ���˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Vref_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(203,156,311,177);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,157,310,176);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//���ģʽ
    }else
    {    
        Set_Write_Mode(WRITE_OR);//��ģʽ
    }
    Put_Numeral(220,159,VREF_AD_FILT_BAND,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,156,311,177);
    }
}

//**********************************************************************
//������:Put_Vt_Loops
//��������:��ʾ��������Ȧ��
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Vt_Loops(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(84,179,121,200);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//��ģʽ
    Put_Numeral(88,184,demar_laps,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(84,179,121,200);
    }
}

//**********************************************************************
//������:Put_Vt_Filt_Len
//��������:��ʾ���������˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Vt_Filt_Len(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(123,179,201,200);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(124,180,200,199);
    }    
    Set_ASC_Lib(Arial10B_Font);    
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//���ģʽ
    }else
    {    
        Set_Write_Mode(WRITE_OR);//��ģʽ
    }
    Put_Numeral(135,184,vte_filter_const,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,179,201,200);
    }
}

//**********************************************************************
//������:Put_Vt_Filt_Band
//��������:��ʾ�γ��������˲�����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Vt_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(203,179,311,200);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,180,310,199);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//���ģʽ
    }else
    {    
        Set_Write_Mode(WRITE_OR);//��ģʽ
    }
    Put_Numeral(220,184,5,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,179,311,200);
    }
}

//**********************************************************************
//����  ��:Demar_Ad_Req_Pro
//��������:��ѯ����ͨ��ADֵ
//��ڲ���:menu_option--�˵���(��8λ)��Ĭ��ѡ��(��8λ)
//���ڲ���:���л����Ľ����(��8λ)�������Ľ���ѡ���(�Ͱ�λ)
//**********************************************************************
static uint16 Demar_Ad_Req_Pro(uint16 menu_option)
{
     uint8 key_code;
    uint16 key_pressed;
    uint8 oper_st = 0;
    
    Demar_Put_Ad_Req_Frame();
    Get_Key_Code();    //���ԭ���ļ���
    phase_vent_ctrl = PHASE_START_VENT;//������������
    while (1)
    {
        if (flag_10ms_ok)    //10ms
        {
            flag_10ms_ok =  FALSE;
            key_code = Get_Key_Code();
            key_pressed = Get_Key_Status();
            switch (key_code)
            {
                case KEY_INC|KEY_PRESSED://"����"��
                    if (oper_st == 1)    //�����������
                    {
                        if (menu_option==MENU_AD_REQ_PAW_BAND)    //�޸�PAW�˲�����
                        {
                            paw_ad_filt_band++;
                            Put_Paw_Filt_Band(DISP_RT|DISP_INPUT);
                        }
                        else if (menu_option==MENU_AD_REQ_VT_LEN)
                        {
                            vte_filter_const++;
                            Put_Vt_Filt_Len(DISP_RT|DISP_INPUT);
                        }
                    }
                    else
                    {
                        if (menu_option==MENU_AD_REQ_PAW_BAND)    //�޸�PAW�˲�����
                        {
                            menu_option = MENU_AD_REQ_VT_LEN;
                            Put_Paw_Filt_Band(DISP_RT);
                            Put_Vt_Filt_Len(DISP_RT|DISP_INVERSE);
                        }
                        else if (menu_option==MENU_AD_REQ_VT_LEN)
                        {
                            menu_option = MENU_AD_REQ_PAW_BAND;
                            Put_Paw_Filt_Band(DISP_RT|DISP_INVERSE);
                            Put_Vt_Filt_Len(DISP_RT);                            
                        }                        
                    }
                    Key_Valid_Sound();    //��������Ч��
                    break;
                case KEY_DEC|KEY_PRESSED://"��С"��
                    if (oper_st == 1)    //�����������
                    {
                        if (menu_option==MENU_AD_REQ_PAW_BAND)    //�޸�PAW�˲�����
                        {
                            paw_ad_filt_band--;
                            Put_Paw_Filt_Band(DISP_RT|DISP_INPUT);
                        }
                        else if (menu_option==MENU_AD_REQ_VT_LEN)
                        {
                            vte_filter_const--;
                            Put_Vt_Filt_Len(DISP_RT|DISP_INPUT);
                        }
                    }
                    else
                    {
                        if (menu_option==MENU_AD_REQ_PAW_BAND)    //�޸�PAW�˲�����
                        {
                            menu_option = MENU_AD_REQ_VT_LEN;
                            Put_Paw_Filt_Band(DISP_RT);
                            Put_Vt_Filt_Len(DISP_RT|DISP_INVERSE);                            
                        }
                        else if (menu_option==MENU_AD_REQ_VT_LEN)
                        {
                            menu_option = MENU_AD_REQ_PAW_BAND;
                            Put_Paw_Filt_Band(DISP_RT|DISP_INVERSE);
                            Put_Vt_Filt_Len(DISP_RT);                                
                        }                        
                    }
                    Key_Valid_Sound();    //��������Ч��
                    break;                    
                case KEY_OK|KEY_PRESSED:    //"ȷ��"��
                    Key_Valid_Sound();    //��������Ч��                
                    oper_st ^= 0x01;        //�޸�ѡ��״̬
                    if (menu_option==MENU_AD_REQ_PAW_BAND)    //�޸�PAW�˲�����
                    {
                        if (oper_st)
                        {
                            Put_Paw_Filt_Band(DISP_RT|DISP_INPUT);    
                        }else
                        {
                            Put_Paw_Filt_Band(DISP_RT|DISP_INVERSE);                
                        }
                    }
                    else if (menu_option==MENU_AD_REQ_VT_LEN)
                    {
                        if (oper_st)
                        {
                            Put_Vt_Filt_Len(DISP_RT|DISP_INPUT);    
                        }else
                        {
                            Put_Vt_Filt_Len(DISP_RT|DISP_INVERSE);                
                        }                        
                    }                        
                    break;    
                case KEY_MENU|KEY_PRESSED_2S://"�˵���"��
                    Key_Valid_Sound();    //��������Ч��
                    phase_vent_ctrl = PHASE_STOP_VENT;//ֹͣ��������
                    return (DEMAR_MAIN_MENU_VT);
                    break;                
                default:
                    if (key_code&(KEY_PRESSED_5S|KEY_PRESSED_2S|KEY_PRESSED))//�м�����,������Ч
                    {
                        Key_No_Valid_Sound();    //��������Ч��
                    }
                    break;
            }
        }
            
        if (flag_100ms_ok)
        {
            flag_100ms_ok = FALSE;    
            Put_Paw_AD(DISP_RT);        
            Put_O2_AD(DISP_RT);
            Put_Batt_V_AD(DISP_RT);        
            Put_Batt_I_AD(DISP_RT);
            Put_Vref_AD(DISP_RT);
            Put_Vt_Loops(DISP_RT);
            //��ʾ����/�ֶ�������
            if(phase_vent_ctrl == PHASE_NO_VENT)
            {
                LED_Indicate_Off(EO2_LED_VENT);
                LED_Indicate_On(EO2_LED_BAG);
            }
            else{
                LED_Indicate_Off(EO2_LED_BAG);
                LED_Indicate_On(EO2_LED_VENT);
            }
            LED_Indicate_Show();    //ˢ����ʾLED                                                
        }
    }    
}

//**********************************************************************
//����  ��:Put_Pre_Butt
//��������:��ʾ"ǰһ��"��ť
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Pre_Butt(uint8 disp_mode)
{
    sint16 di;
    uint8* str;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(254,97,309,115);
    
    if (!(disp_mode&DISP_ER))    //����������
    {
        //Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        str = Language_Str("ǰһ��","PRE");
        di = 282-Get_Str_Width(str)/2; //������ʾ�ĺ�����
        Put_Line_Str(di,99,str);
    }
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(254,97,309,115);
    }
}

//**********************************************************************
//����  ��:Put_Next_Butt
//��������:��ʾ"��һ��"��ť
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Next_Butt(uint8 disp_mode)
{
    sint16 di;
    uint8* str;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(254,120,309,138);
    
    if (!(disp_mode&DISP_ER))    //����������
    {
        //Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        str = Language_Str("��һ��","NEXT");
        di = 282-Get_Str_Width(str)/2; //������ʾ�ĺ�����
        Put_Line_Str(di,123,str);
    }
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(254,120,309,138);
    }
}

//**********************************************************************
//����  ��:Put_Check_Cur_Butt
//��������:��ʾ"���"���߰�ť
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Check_Cur_Butt(uint8 disp_mode)
{
    sint16 di;
    uint8* str;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(254,157,309,175);
    
    if (!(disp_mode&DISP_ER))    //����������
    {
        //Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        str = Language_Str("���","CHECK");
        di = 282-Get_Str_Width(str)/2; //������ʾ�ĺ�����
        Put_Line_Str(di,159,str);
    }
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(254,157,309,175);
    }
}

//**********************************************************************
//����  ��:Put_Save_Butt
//��������:��ʾ"����"��ť
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Save_Butt(uint8 disp_mode)
{
    sint16 di;
    uint8* str;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(254,181,309,199);
    
    if (!(disp_mode&DISP_ER))    //����������
    {
        //Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        str = Language_Str("����","SAVE");
        di = 282-Get_Str_Width(str)/2; //������ʾ�ĺ�����
        Put_Line_Str(di,183,str);
    }
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(254,181,309,199);
    }
}

//**********************************************************************
//����  ��:Put_Ret_Butt
//��������:��ʾ"�˳�"��ť
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Ret_Butt(uint8 disp_mode)
{
    sint16 di;
    uint8* str;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(254,204,309,222);
    
    if (!(disp_mode&DISP_ER))    //����������
    {
        //Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_THINRED,COL_BLACK);
        str = Language_Str("�˳�","EXIT");
        di = 282-Get_Str_Width(str)/2; //������ʾ�ĺ�����
        Put_Line_Str(di,206,str);
    }
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(254,204,309,222);
    }
}

//**********************************************************************
//����  ��:Put_Paw_Dem_Point_Box
//��������:��ʾѹ���궨���ֵ
//��ڲ���:disp_mode--��ʾ��ʽ
//            para--��ʾ�Ĳ���
//���ڲ���:��
//**********************************************************************
static void Put_Paw_Dem_Point_Box(uint8 disp_mode,sint32 para)
{
    sint16 di,dj;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(254,76,309,93);
    
    //Set_Write_Mode(WRITE_EXOR);
    Set_ASC_Lib(Arial8B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = 282-(Get_Str_Width("cmH2O")+Get_Num_Width(para,1))/2; //������ʾ�ĺ�����
    dj = Put_Numeral(di,80,para,1);    //��ʾ�궨���ֵ
    Put_Line_Str(dj,80,"cmH2O"); //��ʾ��λ
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(254,76,309,93);
    }
}

//**********************************************************************
//����  ��:Put_O2_Dem_Point_Box
//��������:��ʾ�����������궨���ֵ
//��ڲ���:disp_mode--��ʾ��ʽ
//            para--��ʾ�Ĳ���
//���ڲ���:��
//**********************************************************************
static void Put_O2_Dem_Point_Box(uint8 disp_mode,sint32 para)
{
    sint16 di,dj;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(254,76,309,93);
    
    //Set_Write_Mode(WRITE_EXOR);
    Set_ASC_Lib(Arial10B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = 282-(Get_Str_Width("%")+Get_Num_Width(para,1))/2; //������ʾ�ĺ�����
    dj = Put_Numeral(di,79,para,1);    //��ʾ�궨���ֵ
    Put_Line_Str(dj,79,"%"); //��ʾ��λ
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(254,76,309,93);
    }
}

//**********************************************************************
//����  ��:Put_Vt_Dem_Point_Box
//��������:��ʾ�������궨���ֵ
//��ڲ���:disp_mode--��ʾ��ʽ
//            para--��ʾ�Ĳ���
//���ڲ���:��
//**********************************************************************
static void Put_Vt_Dem_Point_Box(uint8 disp_mode,sint32 para)
{
    sint16 di,dj;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(254,76,309,93);
    
    //Set_Write_Mode(WRITE_EXOR);
    Set_ASC_Lib(Arial10B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = 282-(Get_Str_Width("mL")+Get_Num_Width(para,1))/2; //������ʾ�ĺ�����
    dj = Put_Numeral(di,79,para,1);    //��ʾ�궨���ֵ
     Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(dj,79,"mL"); //��ʾ��λ
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(254,76,309,93);
    }
}

//**********************************************************************
//����  ��:Put_Paw_Ad_In_Box
//��������:��ʾ�ܵ�ѹ����������AD���봰��
//��ڲ���:disp_mode--��ʾ��ʽ
//            para--��ʾ�Ĳ���
//���ڲ���:��
//**********************************************************************
static void Put_Paw_Ad_In_Box(uint8 disp_mode,uint16 para)
{
    sint16 di;
    
    if (disp_mode&DISP_INPUT)//��������
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);
        Set_Chr_Color(COL_BLACK,COL_WHITE);
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(255,52,307,69);
    
    if (!(disp_mode&DISP_ER))    //����������
    {
       /* if (disp_mode&DISP_INPUT)//��������
        {
            Set_Write_Mode(WRITE_OR);
        }
        else 
        Set_Write_Mode(WRITE_EXOR);*/
        Set_ASC_Lib(Arial10B_Font);
        di = 282-Get_Num_Width(para,1)/2; //������ʾ�ĺ�����
        Put_Numeral(di,55,para,1);    //��ʾ�궨���ֵ
    }
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(255,52,307,69);
    }
}

//**********************************************************************
//����  ��:Put_O2_Ad_In_Box
//��������:��ʾ������������AD���봰��
//��ڲ���:disp_mode--��ʾ��ʽ
//            para--��ʾ�Ĳ���
//���ڲ���:��
//**********************************************************************
static void Put_O2_Ad_In_Box(uint8 disp_mode,uint16 para)
{
    sint16 di;
    
    if (disp_mode&DISP_INPUT)//��������
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);
        Set_Chr_Color(COL_BLACK,COL_WHITE); 
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(255,52,307,69);
    
    if (!(disp_mode&DISP_ER))    //����������
    {
       /* if (disp_mode&DISP_INPUT)//��������
        {
            Set_Write_Mode(WRITE_OR);
        }
        else 
        Set_Write_Mode(WRITE_EXOR);*/
        
        Set_ASC_Lib(Arial10B_Font);
        di = 282-Get_Num_Width(para,1)/2; //������ʾ�ĺ�����
        Put_Numeral(di,55,para,1);    //��ʾ�궨���ֵ
    }
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(255,52,307,69);
    }
}

//**********************************************************************
//����  ��:Put_Vt_Lps_In_Box
//��������:��ʾ������������Ȧ�����봰��
//��ڲ���:disp_mode--��ʾ��ʽ
//            para--��ʾ�Ĳ���
//���ڲ���:��
//**********************************************************************
static void Put_Vt_Lps_In_Box(uint8 disp_mode,sint32 para)
{
    sint16 di;
    
    if (disp_mode&DISP_INPUT)//��������
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);
        Set_Chr_Color(COL_BLACK,COL_WHITE);
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
         Set_Chr_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ    
    Bar(255,52,307,69);
    
    if (!(disp_mode&DISP_ER))    //����������
    {
       /* if (disp_mode&DISP_INPUT)//��������
        {
            Set_Write_Mode(WRITE_OVERLAY);
        }
        else Set_Write_Mode(WRITE_OVERLAY);*/
        
        Set_ASC_Lib(Arial10B_Font);
        di = 282-Get_Num_Width(para,1)/2; //������ʾ�ĺ�����
        Put_Numeral(di,55,para,1);    //��ʾ�궨���ֵ
    }
    if (disp_mode&DISP_RT)    //ʵʱˢ��
    {
        LCD_Refresh_Area(255,52,307,69);
    }
}

//**********************************************************************
//����  ��:Put_Paw_Ad_Val
//��������:�ڱ궨ѹ��������ʱ��ʾ������ADֵ
//��ڲ���:disp_mode
//            ad--��ʾ��ADֵ
//���ڲ���:��
//**********************************************************************
static void Put_Paw_Ad_Val( uint8 disp_mode,uint16 ad)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(124,33,246,50);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//��ģʽ
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,36,ad,1);
    di = Put_Line_Str(di,36,"(");    
    di = Put_Numeral(di,36,Cal_Vol(ad),100);//��ʾ��ѹֵ
    di = Put_Line_Str(di,36,"V)");
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,33,246,50);
    }    
}

//**********************************************************************
//����  ��:Put_O2_Ad_Val
//��������:�ڱ궨����������ʱ��ʾ������ADֵ
//��ڲ���:disp_mode
//            ad--��ʾ��ADֵ
//���ڲ���:��
//**********************************************************************
static void Put_O2_Ad_Val( uint8 disp_mode,uint16 ad)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(124,33,246,50);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//��ģʽ
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,36,ad,1);
    di = Put_Line_Str(di,36,"(");    
    di = Put_Numeral(di,36,Cal_Vol(ad),100);//��ʾ��ѹֵ
    di = Put_Line_Str(di,36,"V)");
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,33,246,50);
    }    
}

//**********************************************************************
//����  ��:Put_Cur_Pressur_Dem_Paw
//��������:�ڱ궨ѹ��������ʱ��ʾ������ѹ��ֵ
//��ڲ���:disp_mode
//            para--ѹ��ֵ(0.01cmH2O)
//���ڲ���:��
//**********************************************************************
static void Put_Cur_Pressur_Dem_Paw( uint8 disp_mode,sint32 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(124,52,246,69);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//��ģʽ
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,55,para*10/PAW_MUL,10);    //��ʾѹ������ֵ
    di = Put_Line_Str(di,55,"cmH2O");    //��ʾ��λ
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,52,246,69);
    }    
}

//**********************************************************************
//����  ��:Put_Cur_O2_Dem_O2
//��������:�ڱ궨����������ʱ��ʾ����������Ũ��
//��ڲ���:disp_mode
//            para--ѹ��ֵ(0.1cmH2O)
//���ڲ���:��
//**********************************************************************
static void Put_Cur_O2_Dem_O2( uint8 disp_mode,sint32 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(124,52,246,69);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//��ģʽ
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,55,para,10);    //��ʾ����Ũ�Ȳ���ֵ
    di = Put_Line_Str(di,55,"%");    //��ʾ��λ
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,52,246,69);
    }    
}

//**********************************************************************
//����  ��:Put_Zero_Val_Dem_Paw
//��������:�ڱ궨ѹ��������ʱ��ʾѹ��У��ֵ
//��ڲ���:disp_mode
//            para--У��ֵ
//���ڲ���:��
//**********************************************************************
static void Put_Zero_Val_Dem_Paw( uint8 disp_mode,sint16 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(124,71,246,88);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//��ģʽ
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,74,para,1);    //��ʾУ��ֵ
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,71,246,88);
    }    
}

//**********************************************************************
//����  ��:Put_O2_Cell_Volt_Dem_O2
//��������:�ڱ궨����������ʱ��ʾ����ص�ѹ
//��ڲ���:disp_mode
//            para--��ѹֵ
//���ڲ���:��
//**********************************************************************
static void Put_O2_Cell_Volt_Dem_O2( uint8 disp_mode,sint32 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(124,71,246,88);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//��ģʽ
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,74,para,10);    //��ʾ��ѹֵ
    Put_Str(di,74,"mV");        //��ʾ��λ
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,71,246,88);
    }    
}

//**********************************************************************
//����  ��:Put_Vt_Lps_Val
//��������:�ڱ궨��������������ʱ����ʾȦ������ֵ
//��ڲ���:disp_mode--��ʾ��ʽ
//            para--Ȧ��
//���ڲ���:��
//**********************************************************************
static void Put_Vt_Lps_Val( uint8 disp_mode,sint32 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(124,33,246,50);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//��ģʽ
     Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,36,para,1);
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,33,246,50);
    }    
}

//**********************************************************************
//����  ��:Put_Cur_Vt
//��������:�ڱ궨������������ʱ��ʾ����������ֵ
//��ڲ���:disp_mode
//            para--ѹ��ֵ(0.1cmH2O)
//���ڲ���:��
//**********************************************************************
static void Put_Cur_Vt( uint8 disp_mode,sint32 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(124,52,246,69);
    
    Set_ASC_Lib(Arial10B_Font);
   // Set_Write_Mode(WRITE_OR);//��ģʽ
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,55,para,1);    //��ʾѹ������ֵ
    di = Put_Line_Str(di,55,"mL");    //��ʾ��λ
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,52,246,69);
    }    
}

//**********************************************************************
//����  ��:Put_Vol_Axis_Dem_Paw
//��������:�ڱ궨ѹ����������ʱ��,��AD(VOL)��
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Vol_Axis_Dem_Paw( uint8 disp_mode)
{
    uint16 di;
    uint16 x1,x2;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Hor_Line(40,218,206);    //��x��
    Set_ASC_Lib(Arial8B_Font);
    for (di=0; di<9; di++)//���̶�
    {
        //Set_Write_Mode(WRITE_AND);    //��ģʽ
        x1 = di*20+40;
        Ver_Line(204,205,x1);
        num = di*5;
        x2 = x1-Get_Num_Width(num,10)/2;
        //Set_Write_Mode(WRITE_NOT);    //���Ը���
        Put_Numeral(x2,210,num,10);
    }
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Line(213,203,218,206);//����ͷ
    Line(213,209,218,206);
    //Set_Write_Mode(WRITE_NOT);    //���Ը���
    Put_Char(223,201,'V');
    
    if (disp_mode&DISP_RT)    //��ʱˢ��
    {
        LCD_Refresh_Area(40,203,232,220);
    }
}

//**********************************************************************
//����  ��:Put_Press_Axis_Dem_Paw
//��������:�ڱ궨ѹ����������ʱ��,��Aѹ����
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Press_Axis_Dem_Paw( uint8 disp_mode)
{
    uint16 y;
    uint8 di;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Ver_Line(107,206,40);    //��y��
    Set_ASC_Lib(Arial8B_Font);
    for (di=0; di<5; di++)//���̶�
    {
        y = 206-di*20;
        //Set_Write_Mode(WRITE_AND);    //��ģʽ
        Hor_Line(41,42,y);
        num = di*20;
        //Set_Write_Mode(WRITE_NOT);    //���Ը���
        Put_Numeral(25,y-5,num,1);    
    }
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Line(37,112,40,107);    //����ͷ
    Line(43,112,40,107);
    Set_Write_Mode(WRITE_NOT);    //���Ը���
    Put_Line_Str(13,95,"cmH2O");
    
    if (disp_mode&DISP_RT)    //��ʱˢ��
    {
        LCD_Refresh_Area(13,95,55,209);
    }        
}

//**********************************************************************
//����  ��:Put_Paw_Sen_Cur
//��������:��ѹ����������������
//��ڲ���:disp_mode--��ʾ��ʽ
//           curve--��ʾ�ڼ���
//���ڲ���:��
//**********************************************************************
static void Put_Paw_Sen_Cur(sint8 curve, uint8 disp_mode)
{
    uint16 w1,w2,h1,h2;    //���߶ε�ͶӰ���Ⱥ͸߶�
    uint16 x1,y1,x2,y2;
    
    if (curve >= PAW_DEMAR_POINTS-1)    return;//û�и����߶��򷵻�
    if (curve < 0)    return;

    if (disp_mode&DISP_RT)//ʵʱˢ��
    {
        Set_Refresh_Mode(1);
    }     
    //�����һ���ͶӰ��Ⱥ͸߶�
    w1 = paw_ad_tab[curve]*120/1024;    //����ADΪ120pixels
    h1 = paw_demar_tab[curve];            //1cmH2O/pixel
    w2 = paw_ad_tab[curve+1]*120/1024;    //����ADΪ120pixels    
    h2 = paw_demar_tab[curve+1];        //1cmH2O/pixel
    if (disp_mode&DISP_ER)    //����������
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
       // Set_Write_Mode(WRITE_OR);    //��ģʽ    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
        //Set_Write_Mode(WRITE_AND);    //��ģʽ
    }
    x1 = 40+w1;
    x2 = 40+w2;
    y1 = 206-h1;
    y2 = 206-h2;
    Line(x1,y1,x2,y2);
    Fill_Circle(x1,y1,2);//�궨��ͻ����ʾ
    Fill_Circle(x2,y2,2);
    Set_Refresh_Mode(0);
    
    if (disp_mode&DISP_ER)//�������ߺ�Ҫ�ػ�������
    {
        Put_Vol_Axis_Dem_Paw(DISP_RT);
        Put_Press_Axis_Dem_Paw(DISP_RT);
    }    
}

//**********************************************************************
//����  ��:Put_Paw_Dem_Dot
//��������:��ʾѹ���궨���ͼ��
//��ڲ���:disp_mode--��ʾ��ʽ
//          point--�궨������
//���ڲ���:��
//**********************************************************************
static void Put_Paw_Dem_Dot(sint8 point, uint8 disp_mode)
{
    uint16 x,y;
    if (point > PAW_DEMAR_POINTS-1)    return;    //û�иñ궨��
    if (point < 0) return;
    
    x = 40+paw_ad_tab[point]*120/1024;    //����ADΪ120pixels
    y = 206- paw_demar_tab[point];            //1cmH2O/pixel
    if (disp_mode&DISP_ER)    //������ʾ
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OR);    //��ģʽ    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
        //Set_Write_Mode(WRITE_AND);    //��ģʽ
    }
    Fill_Circle(x,y,2);    
    if (disp_mode&DISP_RT)    //��ʱˢ��
    {
        LCD_Refresh_Area(x-2,y-2,x+2,y+2);
    }
}

//**********************************************************************
//����  ��:Put_Vol_Axis_Dem_O2
//��������:�ڱ궨������������ʱ��,��AD(VOL)��
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Vol_Axis_Dem_O2( uint8 disp_mode)
{
    uint16 di;
    uint16 x1,x2;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    Set_Write_Mode(WRITE_AND);    //��ģʽ
    Hor_Line(40,218,206);    //��x��
    Set_ASC_Lib(Arial8B_Font);
    for (di=0; di<9; di++)//���̶�
    {
        //Set_Write_Mode(WRITE_AND);    //��ģʽ
        x1 = di*20+40;
        Ver_Line(204,205,x1);
        num = di*5;
        x2 = x1-Get_Num_Width(num,10)/2;
        //Set_Write_Mode(WRITE_NOT);    //���Ը���
        Put_Numeral(x2,210,num,10);
    }
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Line(213,203,218,206);//����ͷ
    Line(213,209,218,206);
    //Set_Write_Mode(WRITE_NOT);    //���Ը���
    Put_Char(223,201,'V');
    
    if (disp_mode&DISP_RT)    //��ʱˢ��
    {
        LCD_Refresh_Area(40,203,232,220);
    }
}

//**********************************************************************
//����  ��:Put_O2_Axis_Dem_O2
//��������:�ڱ궨������������ʱ��,������Ũ����(Y��)
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_O2_Axis_Dem_O2( uint8 disp_mode)
{
    uint16 y;
    uint8 di;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Ver_Line(107,206,40);    //��y��
    Set_ASC_Lib(Arial8B_Font);
    for (di=0; di<5; di++)//���̶�
    {
        y = 206-di*20;
        //Set_Write_Mode(WRITE_AND);    //��ģʽ
        Hor_Line(41,42,y);
        num = di*25;
        //Set_Write_Mode(WRITE_NOT);    //���Ը���
        Put_Numeral(20,y-5,num,1);    //���̶�ֵ    
    }
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Line(37,112,40,107);    //����ͷ
    Line(43,112,40,107);
    //Set_Write_Mode(WRITE_NOT);    //���Ը���
    Put_Line_Str(33,95,"%");
    
    if (disp_mode&DISP_RT)    //��ʱˢ��
    {
        LCD_Refresh_Area(13,95,55,209);
    }        
}

//**********************************************************************
//����  ��:Put_O2_Sen_Cur
//��������:������Ũ�ȴ�������������
//��ڲ���:disp_mode--��ʾ��ʽ
//           curve--��ʾ�ڼ���
//���ڲ���:��
//**********************************************************************
static void Put_O2_Sen_Cur(sint8 curve, uint8 disp_mode)
{
    uint16 w1,w2,h1,h2;    //���߶ε�ͶӰ���Ⱥ͸߶�
    uint16 x1,y1,x2,y2;
    
    if (curve >= O2_SEN_DEM_POINTS-1)    return;//û�и����߶��򷵻�
    if (curve < 0)    return;

    if (disp_mode&DISP_RT)//ʵʱˢ��
    {
        Set_Refresh_Mode(1);
    }     
    //�����һ���ͶӰ��Ⱥ͸߶�
    w1 = o2_ad_tab[curve]*120/1024;    //����ADΪ120pixels
    h1 = o2_demar_tab[curve]*4/5;    //100%/80dots
    w2 = o2_ad_tab[curve+1]*120/1024;    //����ADΪ120pixels    
    h2 = o2_demar_tab[curve+1]*4/5;        //100%/80dots
    if (disp_mode&DISP_ER)    //����������
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OR);    //��ģʽ    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
       // Set_Write_Mode(WRITE_AND);    //��ģʽ
    }
    x1 = 40+w1;
    x2 = 40+w2;
    y1 = 206-h1;
    y2 = 206-h2;
    Line(x1,y1,x2,y2);
    Fill_Circle(x1,y1,2);//�궨��ͻ����ʾ
    Fill_Circle(x2,y2,2);
    Set_Refresh_Mode(0);
    
    if (disp_mode&DISP_ER)//�������ߺ�Ҫ�ػ�������
    {
        Put_Vol_Axis_Dem_O2(DISP_RT);
        Put_O2_Axis_Dem_O2(DISP_RT);
    }    
}

//**********************************************************************
//����  ��:Put_O2_Dem_Dot
//��������:��ʾ����Ũ�ȱ궨���ͼ��
//��ڲ���:disp_mode--��ʾ��ʽ
//          point--�궨������
//���ڲ���:��
//**********************************************************************
static void Put_O2_Dem_Dot(sint8 point, uint8 disp_mode)
{
    uint16 x,y;
    if (point > O2_SEN_DEM_POINTS-1)    return;    //û�иñ궨��
    if (point < 0) return;
    
    x = 40+o2_ad_tab[point]*120/1024;    //����ADΪ120pixels
    y = 206- o2_demar_tab[point]*4/5;   //100%/80dots
    if (disp_mode&DISP_ER)    //������ʾ
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OR);    //��ģʽ    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
        //Set_Write_Mode(WRITE_AND);    //��ģʽ
    }
    Fill_Circle(x,y,2);    
    if (disp_mode&DISP_RT)    //��ʱˢ��
    {
        LCD_Refresh_Area(x-2,y-2,x+2,y+2);
    }
}

//**********************************************************************
//����  ��:Put_Lps_Axis_Dem_Vt
//��������:�ڱ궨��������ʱ��X��(����Ȧ��)
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Lps_Axis_Dem_Vt( uint8 disp_mode)
{
    uint16 di;
    uint16 x1,x2;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Hor_Line(40,218,191);    //��x��
    Set_ASC_Lib(Arial8B_Font);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    for (di=0; di<9; di++)//���̶�
    {
        //Set_Write_Mode(WRITE_AND);    //��ģʽ
        x1 = di*20+40;
        Ver_Line(189,190,x1);
        num = di*100;
        if(!(di%2))    //ÿ�����̶���ʾһ���̶�ֵ
        {
            x2 = x1-Get_Num_Width(num,1)/2;
            //Set_Write_Mode(WRITE_NOT);    //���Ը���
            Put_Numeral(x2,196,num,1);
        }    
    }
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Line(213,188,218,191);//����ͷ
    Line(213,194,218,191);
    //Set_Write_Mode(WRITE_NOT);    //���Ը���
    Put_Line_Str(215,197,"loops");
    
    if (disp_mode&DISP_RT)    //��ʱˢ��
    {
        LCD_Refresh_Area(34,189,246,209);
    }
}

//**********************************************************************
//����  ��:Put_Vt_Axis_Dem_Vt
//��������:�ڱ궨��������ʱ��,��ʾY��(Vt��)
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Vt_Axis_Dem_Vt( uint8 disp_mode)
{
    uint16 y;
     uint8 di;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Ver_Line(92,191,40);    //��y��
    Set_ASC_Lib(Arial8B_Font);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    for (di=0; di<5; di++)//���̶�
    {
        y = 191-di*20;
        //Set_Write_Mode(WRITE_AND);    //��ģʽ
        Hor_Line(41,42,y);
        num = di*200;
        //Set_Write_Mode(WRITE_NOT);    //���Ը���
        Put_Numeral(16,y-5,num,1);    //���̶�ֵ
    }
    //Set_Write_Mode(WRITE_AND);    //��ģʽ
    Line(37,97,40,92);    //����ͷ
    Line(43,97,40,92);
    //Set_Write_Mode(WRITE_NOT);    //���Ը���
    Put_Line_Str(29,77,"mL");
    
    if (disp_mode&DISP_RT)    //��ʱˢ��
    {
        LCD_Refresh_Area(12,77,46,195);
    }        
}

//**********************************************************************
//����  ��:Put_Vt_Sen_Cur
//��������:����������������
//��ڲ���:disp_mode--��ʾ��ʽ
//           curve--��ʾ�ڼ���
//���ڲ���:��
//**********************************************************************
static void Put_Vt_Sen_Cur(sint8 curve, uint8 disp_mode)
{
    uint16 w1,w2,h1,h2;    //���߶ε�ͶӰ���Ⱥ͸߶�
    uint16 x1,y1,x2,y2;
    
    if (curve >= VT_DEMAR_POINTS-1)    return;//û�и����߶��򷵻�
    if (curve < 0)    return;

    if (disp_mode&DISP_RT)//ʵʱˢ��
    {
        Set_Refresh_Mode(1);
    }     
    //�����һ���ͶӰ��Ⱥ͸߶�
    w1 = vt_lap_tab[curve]/5;    //����1000ȦΪ200pixels(200/1000=1/5)
    h1 = vt_demar_tab[curve]/10;            //10mL/pixel(80/800)
    w2 = vt_lap_tab[curve+1]/5;    //����1000ȦΪ200pixels(200/1000=1/5)
    h2 = vt_demar_tab[curve+1]/10;        //10mL/pixel(80/800)
    if (disp_mode&DISP_ER)    //����������
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OVERLAY);    //��ģʽ    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OVERLAY);    //��ģʽ
    }
    x1 = 40+w1;
    x2 = 40+w2;
    y1 = 191-h1;
    y2 = 191-h2;
    Line(x1,y1,x2,y2);
    Fill_Circle(x1,y1,2);//�궨��ͻ����ʾ
    Fill_Circle(x2,y2,2);
    Set_Refresh_Mode(0);
    
    if (disp_mode&DISP_ER)//�������ߺ�Ҫ�ػ�������
    {
        Put_Lps_Axis_Dem_Vt(DISP_RT);
        Put_Vt_Axis_Dem_Vt(DISP_RT);
    }    
}

//**********************************************************************
//����  ��:Put_Vt_Dem_Dot
//��������:�궨��������ʱ����ʾ�궨���ͼ��
//��ڲ���:disp_mode--��ʾ��ʽ
//          point--�궨������
//���ڲ���:��
//**********************************************************************
static void Put_Vt_Dem_Dot(sint8 point, uint8 disp_mode)
{
    uint16 x,y;
    if (point > VT_DEMAR_POINTS-1)    return;    //û�иñ궨��
    if (point < 0) return;
    
    x = 40+vt_lap_tab[point]/5;    //����1000ȦΪ200pixels(200/1000=1/5)
    y = 191- vt_demar_tab[point]/10;//10mL/pixel(80/800)
    if (disp_mode&DISP_ER)    //������ʾ
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OR);    //��ģʽ    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
        //Set_Write_Mode(WRITE_AND);    //��ģʽ
    }
    Fill_Circle(x,y,2);    
    if (disp_mode&DISP_RT)    //��ʱˢ��
    {
        LCD_Refresh_Area(x-2,y-2,x+2,y+2);
    }
}

//**********************************************************************
//����  ��:Put_Br_Icon_Dem
//��������:��ʾ����ͼ��
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Br_Icon_Dem(uint8 disp_mode)
{
    static uint8 disp_st = 0;//(=0:��ʾ����ͼ��;=1:��ʾ����ͼ��)
    Set_Graphic_Color(COL_WHITE,COL_DEEPGREEN);
    switch(phase_vent_ctrl)
    {
        case PHASE_VCV_INSP:    //����������������ʾ����ͼ��
        case PHASE_VCV_HOLD:
            if(disp_st)    //��û��ʾ����ͼ��
            {
                disp_st =0;
                //Put_Image(212,80,Vent_Icon_Press,WRITE_OVERLAY);
                Put_Image(212,80,Vent_Icon_Press,WRITE_OVERLAY);
                LCD_Refresh_Area(270,60,301,89);
            }    
            break;
        default:
            if (!(disp_st))//��û��ʾ����ͼ��
            {
                disp_st = 1;    //��ʾ����ͼ��
                Put_Image(212,80,Vent_Icon,WRITE_OVERLAY);
                LCD_Refresh_Area(212,80,243,109);                    
            }
            break;                    
    }
}

//**********************************************************************
//����  ��:Put_Paw_Dem_Butt
//��������:��ʾ"�ܵ�ѹ���������궨"ѡ��
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Paw_Dem_Butt( uint8 disp_mode)
{
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(35,49,276,71);
    Set_Graphic_Color(COL_THINGREEN,COL_BLACK);
    Fill_Circle(23,60,5);    //��ǰ��ɫ��Բ
   
    if (!(disp_mode&DISP_ER))    //��ʾ����
    {
        //Set_Write_Mode(WRITE_EXOR);//���ģʽ
        Set_HZ_Lib(&ST_11);        //�����ֿ�
        Set_ASC_Lib(Arial12_Font);   
        Set_Chr_Color(COL_WHITE,COL_BLACK); 
        Put_Line_Str(43,52,Language_Str("�ܵ�ѹ���������궨","PAW Sensor Demarcation"));
    }
    
    if (disp_mode&DISP_RT)//��ʱˢ��
    {
        LCD_Refresh_Area(18,55,28,65);    //ˢ��԰
        LCD_Refresh_Area(35,49,276,71);//ˢ�±���
    }
}

//**********************************************************************
//����  ��:Put_O2_Dem_Butt
//��������:��ʾ"�����������궨"ѡ��
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_O2_Dem_Butt( uint8 disp_mode)
{
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
     Bar(35,78,276,100);
    Set_Graphic_Color(COL_THINGREEN,COL_BLACK);
    Fill_Circle(23,89,5);    //��ǰ��ɫ��Բ
    
    if (!(disp_mode&DISP_ER))    //��ʾ����
    {
        //Set_Write_Mode(WRITE_EXOR);//���ģʽ
        Set_HZ_Lib(&ST_11);        //�����ֿ�
        Set_ASC_Lib(Arial12_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        Put_Line_Str(43,81,Language_Str("����Ũ�ȴ������궨","O2 Sensor Demarcation"));
    }
    
    if (disp_mode&DISP_RT)//��ʱˢ��
    {
        LCD_Refresh_Area(18,84,28,94);    //ˢ��Բ
        LCD_Refresh_Area(35,78,276,100);//ˢ�±���
    }
}        

//**********************************************************************
//����  ��:Put_Vt_Dem_Butt
//��������:��ʾ"�������궨"ѡ��
//��ڲ���:disp_mode--��ʾ��ʽ
//���ڲ���:��
//**********************************************************************
static void Put_Vt_Dem_Butt( uint8 disp_mode)
{
    Set_Graphic_Color(COL_BLACK,COL_WHITE);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
     Bar(35,109,276,130);
    Set_Graphic_Color(COL_THINGREEN,COL_BLACK);
    Fill_Circle(23,119,5);    //��ǰ��ɫ��Բ
      
    if (!(disp_mode&DISP_ER))    //��ʾ����
    {
        //Set_Write_Mode(WRITE_EXOR);//���ģʽ
        Set_HZ_Lib(&ST_11);        //�����ֿ�
        Set_ASC_Lib(Arial12_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        Put_Line_Str(43,111,Language_Str("�������궨","VT Demarcation"));
    }
    
    if (disp_mode&DISP_RT)//��ʱˢ��
    {
        LCD_Refresh_Area(18,114,28,124);    //ˢ��԰
        LCD_Refresh_Area(35,109,276,130);//ˢ�±���
    }
}

//**********************************************************************
//����  ��:Init_Dem_Main_Menu
//��������:�ڱ궨ģʽ����ʾ��������
//��ڲ���:op--��ǰѡ��
//���ڲ���:��
//**********************************************************************
static void Init_Dem_Main_Menu(sint32 op)
{
    uint8 *str;
    sint32 di;
    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Clr_Screen(DISP_NOT_RT);    //�ñ���ɫ����
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Rectangle(5,5,314,234);    //�����
    Hor_Line(6,313,31);        //����
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Pattern_Bar(6,6,313,30,0xFF);

    //Set_Write_Mode(WRITE_EXOR);//���ģʽ
    Set_HZ_Lib(&ST_11);        //�����ֿ�
    Set_ASC_Lib(Arial12_Font);
    Set_Chr_Color(COL_THINGREEN,COL_DEEPGREEN);
    str = Language_Str("�궨������","Demarcation Main Interface");
    di = 159-Get_Str_Width(str)/2;
    Put_Line_Str(di,11,str);
    //Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);
    Put_Line_Str(Language_Pos(213,193),208,Language_Str("�˳���ػ�","Turnoff To Exit"));
    
    Put_Paw_Dem_Butt(DISP_NOR);    
    Put_O2_Dem_Butt(DISP_NOR);
    Put_Vt_Dem_Butt(DISP_NOR);
    LCD_Refresh_Area(0,0,319,239);    //ȫ��ˢ��    
}

//**********************************************************************
//����  ��:Init_Paw_Dem_Menu
//��������:�궨ѹ��������ʱ��ʾ������
//��ڲ���:op--��ǰѡ��
//���ڲ���:��
//**********************************************************************
static void Init_Paw_Dem_Menu(sint32 op)
{
     sint32 di;
     uint8 *str;
    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Clr_Screen(DISP_NOT_RT);    //�ñ���ɫ����
    
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Rectangle(5,5,314,234);    //�����
    Hor_Line(6,313,27);        //����
    Pattern_Bar(6,6,313,26,0xFF);
   
    Set_HZ_Lib(&ST_11);        //�����ֿ�
    Set_ASC_Lib(Arial12_Font);
    Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN);
    str = Language_Str("�ܵ�ѹ���������궨","PAW Sensor Demarcation");
    
    di = 159-Get_Str_Width(str)/2;    
        Put_Line_Str(di,9,str);
    
    //����ͷ
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Hor_Line(12,247,32);    
    Hor_Line(12,247,51);    
    Hor_Line(12,247,70);
    Hor_Line(12,247,89);
    Ver_Line(33,88,12);
    Ver_Line(33,88,123);
    Ver_Line(33,88,247);
   
    Set_ASC_Lib(Arial10B_Font);    
    Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);
    Put_Line_Str(16,36,"AD Value");
    Put_Line_Str(16,54,"Pressure");
    Put_Line_Str(16,73,"Zero Value");
    
    Set_Graphic_Color(COL_THINGRAY,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(12,93,247,223);            
    Put_Vol_Axis_Dem_Paw(DISP_NOT_RT);    //��x��
    Put_Press_Axis_Dem_Paw(DISP_NOT_RT);    //��Y��
    for (di=0; di<PAW_DEMAR_POINTS-1; di++)
    {
        Put_Paw_Sen_Cur(di,DISP_NOT_RT|DISP_NOT_ER);
    }
    
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    //Set_Write_Mode(WRITE_EXOR);//���ģʽ
    Bar(253,32,309,70);
    
    Set_ASC_Lib(Arial10B_Font);    
    Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN);  
    Put_Line_Str(282-Get_Str_Width("AD In")/2,35,"AD In");
    
    Put_Paw_Ad_In_Box(DISP_NOR,paw_ad_tab[0]);//��ʾ�궨��ADֵ    
    Put_Paw_Dem_Point_Box(DISP_NOR,paw_demar_tab[0]);//��ʾ�궨��
    
    Put_Pre_Butt(DISP_NOR);
    Put_Next_Butt(DISP_NOR);    
    Put_Check_Cur_Butt(DISP_NOR);
    Put_Save_Butt(DISP_NOR);
    Put_Ret_Butt(DISP_NOR);
    
    Put_Paw_Ad_Val(DISP_NOR,paw_ad);
    Put_Cur_Pressur_Dem_Paw(DISP_NOR,airway_p);
    Put_Zero_Val_Dem_Paw(DISP_NOR,paw_zero_ad);    //��ʾУ��ֵ
    LCD_Refresh_Area(0,0,319,239);    //ȫ��ˢ��    
}

//**********************************************************************
//����  ��:Init_O2_Dem_Menu
//��������:�궨����������ʱ��ʾ������
//��ڲ���:op--��ǰѡ��
//���ڲ���:��
//**********************************************************************
static void Init_O2_Dem_Menu(sint32 op)
{
     sint32 di;
     uint8 *str;
    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Clr_Screen(DISP_NOT_RT);    //�ñ���ɫ����
    
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Rectangle(5,5,314,234);    //�����
    Hor_Line(6,313,27);        //����
    Pattern_Bar(6,6,313,26,0xFF);
    
    Set_HZ_Lib(&ST_11);        //�����ֿ�
    Set_ASC_Lib(Arial12_Font);
    Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN);
    str = Language_Str("����Ũ�ȴ������궨","O2 Sensor Demarcation");
    di = 159-Get_Str_Width(str)/2;    
    Put_Line_Str(di,9,str);
    
    //����ͷ
    Set_Graphic_Color(COL_THINGRAY,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Hor_Line(12,247,32);    
    Hor_Line(12,247,51);    
    Hor_Line(12,247,70);
    Hor_Line(12,247,89);
    Ver_Line(33,88,12);
    Ver_Line(33,88,123);
    Ver_Line(33,88,247);
    //Set_Write_Mode(WRITE_OR);
     Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);
    Set_ASC_Lib(Arial10B_Font);    
    Put_Line_Str(16,36,"AD Value");
    Put_Line_Str(16,54,"O2 Thickness");
    Put_Line_Str(16,73,"O2 Cell Volt");
    
    Set_Graphic_Color(COL_THINGRAY,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Bar(12,93,247,223); 
               
    Put_Vol_Axis_Dem_O2(DISP_NOT_RT);    //��x��
    Put_O2_Axis_Dem_O2(DISP_NOT_RT);    //��Y��
    for (di=0; di<O2_SEN_DEM_POINTS-1; di++)
    {
        Put_O2_Sen_Cur(di,DISP_NOT_RT|DISP_NOT_ER);
    }
    
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);    
    Bar(253,32,309,70);
    
    Set_ASC_Lib(Arial10B_Font);
    Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN); 
    Put_Line_Str(282-Get_Str_Width("AD In")/2,35,"AD In");
    
    Put_O2_Ad_In_Box(DISP_NOR,o2_ad_tab[0]);//��ʾ�궨��ADֵ    
    Put_O2_Dem_Point_Box(DISP_NOR,o2_demar_tab[0]);//��ʾ�궨��
    Put_Pre_Butt(DISP_NOR);
    Put_Next_Butt(DISP_NOR);    
    Put_Check_Cur_Butt(DISP_NOR);
    Put_Save_Butt(DISP_NOR);
    Put_Ret_Butt(DISP_NOR);
    
    //��ʾ����ֵ
    Put_O2_Ad_Val(DISP_NOR,o2_ad);
    Put_Cur_O2_Dem_O2(DISP_NOR,cur_o2); 
    Put_O2_Cell_Volt_Dem_O2(DISP_NOR,o2_vol);
    LCD_Refresh_Area(0,0,319,239);    //ȫ��ˢ��    
}

//**********************************************************************
//����  ��:Init_Vt_Dem_Menu
//��������:��ʼ���������궨�˵�
//��ڲ���:op--��ǰѡ��
//���ڲ���:��
//**********************************************************************
static void Init_Vt_Dem_Menu(sint32 op)
{
     sint32 di;
     uint8 *str;
    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Clr_Screen(DISP_NOT_RT);    //�ñ���ɫ����
    
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Rectangle(5,5,314,234);    //�����
    Hor_Line(6,313,27);        //����
    Pattern_Bar(6,6,313,26,0xFF);
   
    Set_HZ_Lib(&ST_11);        //�����ֿ�
    Set_ASC_Lib(Arial12_Font);
     Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN);
    str = Language_Str("�������궨","Vt Demarcation");
    di = 159-Get_Str_Width(str)/2;
    Put_Line_Str(di,9,str);
    
    //����ͷ
    Set_Graphic_Color(COL_THINGRAY,COL_DEEPGREEN);
    Hor_Line(12,247,32);    
    Hor_Line(12,247,51);    
    Hor_Line(12,247,70);
    Ver_Line(33,69,12);
    Ver_Line(33,69,123);
    Ver_Line(33,69,247);
    //Set_Write_Mode(WRITE_OR);
    Set_ASC_Lib(Arial10B_Font); 
     Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);   
    Put_Line_Str(16,36,"Loops Value");
    Put_Line_Str(16,54,"Current Vt");
    
    //Set_Graphic_Color(COL_BLACK,COL_WHITE);
    Set_Write_Mode(WRITE_OVERLAY);//����ģʽ
    Set_Graphic_Color(COL_THINGRAY,COL_DEEPGREEN);
    Bar(12,76,247,223);            
    Put_Lps_Axis_Dem_Vt(DISP_NOT_RT);    //��x��
    Put_Vt_Axis_Dem_Vt(DISP_NOT_RT);    //��Y��
    for (di=0; di<VT_DEMAR_POINTS-1; di++)
    {
        Put_Vt_Sen_Cur(di,DISP_NOT_RT|DISP_NOT_ER);
    }
    Put_Br_Icon_Dem(DISP_NOR);
    
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    //Set_Write_Mode(WRITE_EXOR);//���ģʽ
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Bar(253,32,309,70);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN);
    Put_Line_Str(282-Get_Str_Width("LPS In")/2,35,"LPS In");
    Put_Vt_Lps_In_Box(DISP_NOR,vt_lap_tab[0]);//��ʾ�궨��Ȧ��
    Put_Vt_Dem_Point_Box(DISP_NOR,vt_demar_tab[0]);//��ʾ�궨��
    Put_Pre_Butt(DISP_NOR);
    Put_Next_Butt(DISP_NOR);    
    Put_Check_Cur_Butt(DISP_NOR);
    Put_Save_Butt(DISP_NOR);
    Put_Ret_Butt(DISP_NOR);
    
    Put_Vt_Lps_Val(DISP_NOR,demar_laps);    //��ʾȦ��
    Put_Cur_Vt(DISP_NOR,demarc_vte);
    LCD_Refresh_Area(0,0,319,239);    //ȫ��ˢ��    
}

//**********************************************************************
//����  ��:Dem_Sel_Menu_Pro
//��������:�궨��Ŀѡ��˵��Ĵ���(10ms����һ��)
//��ڲ���:key_c--������
//            op--��ǰѡ��
//���ڲ���:�л�����ѡ��
//**********************************************************************
static sint32 Dem_Sel_Menu_Pro(uint8 key_c,sint32 op)
{
    struct op_str op_tab[DEMAR_MAIN_MENU_OPS]=
    {{DEMAR_MAIN_MENU_VT,Vt_Sen_Dem_Butt_Pro},    //�������궨
    {DEMAR_MAIN_MENU_O2,O2_Sen_Dem_Butt_Pro},    //�����������궨
    {DEMAR_MAIN_MENU_PAW,Paw_Dem_Butt_Pro}};    //ѹ���������궨
    sint32 op_id;
    static uint8 time=0;
    
    if (!time)    //mod32(320ms������˸��ʾ)
    {
        switch (op)
        {
            case DEMAR_MAIN_MENU_VT://�������궨
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Vt_Dem_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Vt_Dem_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_MAIN_MENU_O2://�����������궨            
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_O2_Dem_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_O2_Dem_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_MAIN_MENU_PAW://ѹ���������궨                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Paw_Dem_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Paw_Dem_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            default:break;
        }    
    }
    time++;
    time &= 31;    
    op_id = (*((op_tab[op&0xff]).op_pro))(key_c);
    if ((op_id^op)&0x0000ffff)    //�л��˲˵������������˸��ʱ
    {
        time = 0;
    }
    return (op_id);    
}

//**********************************************************************
//����  ��:Paw_Dem_Butt_Pro
//��������:"ѹ���������궨"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Paw_Dem_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            Key_Valid_Sound();    //������Ч��        
            dem_op_st = 0;
            phase_vent_ctrl = PHASE_PAW_DEM; //ѹ���������궨ģʽ�µķ��Ŵ���
            return(DEMAR_NEW_PAW_EXIT);//�л���ѹ���������궨�˵�
            break;
        case KEY_PRESSED|KEY_DEC://���ټ�
            Key_Valid_Sound();    //������Ч��            
            if (dem_op_st&OP_ST_ER)
            {
                Put_Paw_Dem_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_MAIN_MENU_O2);//�л���"�����������궨"ѡ��
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_MAIN_MENU_PAW);//��Ȼ����"ѹ���������궨"    
            break;            
    }    
}

//**********************************************************************
//����  ��:O2_Sen_Dem_Butt_Pro
//��������:"�����������궨"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 O2_Sen_Dem_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            Key_Valid_Sound();    //������Ч��        
            dem_op_st = 0;    
            //return (DEMAR_MAIN_MENU_O2);    //��ʱ��������
            return(DEMAR_O2_SEN_EXIT);//�л��������������궨�˵�
            break;
        case KEY_PRESSED|KEY_INC:    //������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//������ָ���ʾ
            {
                Put_O2_Dem_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_MAIN_MENU_PAW);//�л���"ѹ���������궨"ѡ��        
            break;    
        case KEY_PRESSED|KEY_DEC://���ټ�
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//������ָ���ʾ
            {
                Put_O2_Dem_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_MAIN_MENU_VT);//�л���"�������궨"ѡ��
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_MAIN_MENU_O2);//��Ȼ����"�����������궨"    
            break;            
    }    
}

//**********************************************************************
//����  ��:Vt_Sen_Dem_Butt_Pro
//��������:"�������궨"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Vt_Sen_Dem_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            Key_Valid_Sound();    //������Ч��        
            dem_op_st = 0;
            phase_vent_ctrl = PHASE_START_VENT;    //������������    
            return(DEMAR_VT_EXIT);//�л����������궨�˵�
            break;
        case KEY_PRESSED|KEY_INC:    //������
            Key_Valid_Sound();    //������Ч��    
            if (dem_op_st&OP_ST_ER)//������ָ���ʾ
            {
                Put_Vt_Dem_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_MAIN_MENU_O2);//�л���"�����������궨"ѡ��        
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_MAIN_MENU_VT);//��Ȼ����"�������궨"    
            break;            
    }    
}

//**********************************************************************
//����  ��:Paw_Dem_Menu_Pro
//��������:�ܵ�ѹ���������궨�˵��Ĵ���(10ms����һ��)
//��ڲ���:key_c--������
//            op--��ǰѡ��
//���ڲ���:�л�����ѡ��
//**********************************************************************
static sint32 Paw_Dem_Menu_Pro(uint8 key_c,sint32 op)
{
    struct op_str op_tab[DEMAR_NEW_PAW_OPS]=
    {{DEMAR_NEW_PAW_EXIT,Paw_Dem_Ret_Butt_Pro},//�˳�
    {DEMAR_NEW_PAW_SAVE,Paw_Dem_Save_Butt_Pro},    //����
    {DEMAR_NEW_PAW_CHECK,Paw_Dem_Check_Butt_Pro},    //���    
    {DEMAR_NEW_PAW_NEXT,Paw_Dem_Next_Butt_Pro},    //��һ��
    {DEMAR_NEW_PAW_PRE,Paw_Dem_Pre_Butt_Pro},    //ǰһ��
    {DEMAR_NEW_PAW_AD_IN,Paw_Dem_Ad_In_Box_Pro}};//AD����
    
    sint32 op_id;
    static uint8 time=0;
    static uint8 flag_dot_disp = FALSE;
    uint8 di;
    
    if (!time)    //mod32(320ms������˸��ʾ)
    {
        if (FALSE == flag_dot_disp)//������ʾ�궨��
        {
            flag_dot_disp = TRUE;
            Put_Paw_Dem_Dot(cur_demar_point,DISP_RT|DISP_ER);
        }
        else
        {
            flag_dot_disp = FALSE;
            Put_Paw_Dem_Dot(cur_demar_point,DISP_RT);
        }
        switch (op)
        {
            case DEMAR_NEW_PAW_EXIT://�˳�
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Ret_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Ret_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_NEW_PAW_SAVE://����            
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Save_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Save_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_NEW_PAW_NEXT://��һ��                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Next_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Next_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_NEW_PAW_PRE://ǰһ��                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Pre_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Pre_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;        
            case DEMAR_NEW_PAW_CHECK://���                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Check_Cur_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Check_Cur_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;    
            case DEMAR_NEW_PAW_AD_IN://AD����                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;
                    Put_Paw_Ad_In_Box(di|DISP_RT,paw_ad_tab[cur_demar_point]);
                }
                else//ԭ����ʾ�����
                {
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;                
                    Put_Paw_Ad_In_Box(di|DISP_RT|DISP_ER,paw_ad_tab[cur_demar_point]);
                }
                dem_op_st ^= OP_ST_ER;
                break;                                            
            default:break;
        }    
    }
    time++;
    time &= 31;    
    op_id = (*((op_tab[op&0xff]).op_pro))(key_c); //ѡ����ӳ���
    if ((op_id^op)&0x0000ffff)    //�л��˲˵������������˸��ʱ
    {
        time = 0;
    }
    return (op_id);    
}

//**********************************************************************
//����  ��:Paw_Dem_Ret_Butt_Pro
//��������:ѹ���������궨"�˳�"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Paw_Dem_Ret_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            Key_Valid_Sound();    //������Ч��
            if (TRUE == Check_Tips_Disp_Dem())    //����ʾ��Ϣ��ɾ����ʾ��Ϣ
                Cancel_Tips_Dem(&save_err_tips_dem);
            dem_op_st = 0;   
            phase_vent_ctrl = PHASE_STOP_VENT;//ֹͣ�������� 
            return(DEMAR_MAIN_MENU_PAW);//�л���������"ѹ���������궨"ѡ��
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Ret_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_SAVE);//�л���"����"
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_NEW_PAW_EXIT);//��Ȼ����"�˳�"    
            break;            
    }    
}

//**********************************************************************
//����  ��:Paw_Dem_Save_Butt_Pro
//��������:ѹ���������궨"����"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Paw_Dem_Save_Butt_Pro(uint8 key_c)
{
    uint16 ad;
    
    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            ad = paw_ad;
            if (!(Check_Pipeline_P_AD(cur_demar_point,ad)&0xff000000))    //��ȷ
            {
                if (Write_EEPROM(EEP_PIPELINE_P+cur_demar_point*(sizeof(paw_ad_tab[cur_demar_point])), ( uint8 *)(&ad), sizeof(ad)))
                {//д��EEPROM����
                    Key_No_Valid_Sound();    //������Ч��    
                    Log_Tips_Dem(&save_err_tips_dem);    //��ʾ���������Ϣ            
                }
                else
                {
                    Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //����������
                    Put_Paw_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);    //����������
                    Update_Paw_Curve(ad,cur_demar_point);    //��������
                    Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT);    //��ʾ������
                    Put_Paw_Sen_Cur(cur_demar_point,DISP_RT);    //��ʾ������
                    Put_Paw_Ad_In_Box(DISP_RT,paw_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ    
                    if (paw_zero_ad) //У��ֵ��Ϊ0������У��ֵ
                    {
                        paw_zero_ad = 0;
                        Put_Zero_Val_Dem_Paw(DISP_RT,paw_zero_ad);//��ʾУ��ֵ
                        Write_EEPROM(EEP_ZERO_P, ( uint8 *)(&(paw_zero_ad)), sizeof(paw_zero_ad));
                    }                        
                    Key_Valid_Sound();    //������Ч��    
                    Log_Tips_Dem(&save_ok_tips_dem);    //��ʾ����ɹ���Ϣ                
                }
            }
            else
            {
                Key_No_Valid_Sound();    //������Ч��    
                Log_Tips_Dem(&ad_err_tips_dem);    //��ʾAD������Χ����ʾ��Ϣ
            }    
            return(DEMAR_NEW_PAW_SAVE);//��Ȼ����"����"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_CHECK);//�л���"���"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_EXIT);//�л���"�˳�"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_NEW_PAW_SAVE);//��Ȼ����"����"
            break;            
    }    
}

//**********************************************************************
//����  ��:Paw_Dem_Check_Butt_Pro
//��������:ѹ���������궨"���"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Paw_Dem_Check_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if(Check_EEP_Pipeline_P())//���߼�鲻�ϸ�
            {
                Key_No_Valid_Sound();
                Log_Tips_Dem(&curve_err_tips_dem);    //��ʾ���ϸ���Ϣ    
            }
            else
            {
                Key_Valid_Sound();    //������Ч��
                Log_Tips_Dem(&curve_ok_tips_dem);    //��ʾ�ϸ���Ϣ    
            }
            return(DEMAR_NEW_PAW_CHECK);//��Ȼ����"���"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_NEXT);//�л���"��һ��"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_SAVE);//�л���"����"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_NEW_PAW_CHECK);//��Ȼ����"���"
            break;            
    }    
}

//**********************************************************************
//����  ��:Paw_Dem_Next_Butt_Pro
//��������:ѹ���������궨"��һ��"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Paw_Dem_Next_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if (PAW_DEMAR_POINTS-1 > cur_demar_point)//������һ��
            {
                Put_Paw_Dem_Dot(cur_demar_point,DISP_RT);//�ָ���ʾ��ǰ�궨���ͼ��            
                cur_demar_point++;
                Put_Paw_Dem_Point_Box(DISP_RT,paw_demar_tab[cur_demar_point]);    //��ʾ�궨���ѹ��ֵ
                Put_Paw_Ad_In_Box(DISP_RT,paw_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ
                Key_Valid_Sound();    //������Ч��
            }
            else
            {
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_NEW_PAW_NEXT);//��Ȼ����"��һ��"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_PRE);//�л���"ǰһ��"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_CHECK);//�л���"���"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_NEW_PAW_NEXT);//��Ȼ����"��һ��"
            break;            
    }    
}

//**********************************************************************
//����  ��:Paw_Dem_Pre_Butt_Pro
//��������:ѹ���������궨"ǰһ��"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Paw_Dem_Pre_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if (cur_demar_point > 0)//����ǰһ��
            {
                Put_Paw_Dem_Dot(cur_demar_point,DISP_RT);//�ָ���ʾ��ǰ�궨���ͼ��            
                cur_demar_point--;
                Put_Paw_Dem_Point_Box(DISP_RT,paw_demar_tab[cur_demar_point]);    //��ʾ�궨���ѹ��ֵ
                Put_Paw_Ad_In_Box(DISP_RT,paw_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ
                Key_Valid_Sound();    //������Ч��
            }
            else
            {
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_NEW_PAW_PRE);//��Ȼ����"ǰһ��"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_AD_IN);//�л���"AD����"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_NEXT);//�л���"��һ��"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_NEW_PAW_PRE);//��Ȼ����"ǰһ��"
            break;            
    }    
}

//**********************************************************************
//����  ��:Paw_Dem_Ad_In_Box_Pro
//��������:ѹ���������궨"AD����"���ڴ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Paw_Dem_Ad_In_Box_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if (dem_op_st&OP_ST_IN)    //��������״̬
            {
                dem_op_st &= ~OP_ST_IN;    //��Ϊѡ��״̬
                Put_Paw_Ad_In_Box(DISP_RT,paw_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ
                if (dem_op_st&OP_ST_CHG)//���޸��򱣴�
                {
                    if (Write_EEPROM(EEP_PIPELINE_P+cur_demar_point*(sizeof(paw_ad_tab[cur_demar_point])), ( uint8 *)(&(paw_ad_tab[cur_demar_point])), sizeof(paw_ad_tab[cur_demar_point])))
                    {
                        Key_No_Valid_Sound();
                        Log_Tips_Dem(&save_err_tips_dem);    //��ʾ�������
                    }
                    else
                    {
                        Key_Valid_Sound();    //������Ч��    
                    }
                }
                else
                {
                    Key_Valid_Sound();    //������Ч��    
                }
                dem_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
            }
            else//�����޸�״̬
            {
                dem_op_st |= OP_ST_IN;
                Put_Paw_Ad_In_Box(DISP_RT|DISP_INPUT,paw_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ
                Key_Valid_Sound();    //������Ч��    
            }
            return(DEMAR_NEW_PAW_AD_IN);//��Ȼ����"AD����"
            break;
        case KEY_PRESSED|KEY_INC://������
            if (dem_op_st&OP_ST_IN)
            {
                dem_op_st |= OP_ST_CHG;    //���޸ı��
                Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //����������
                Put_Paw_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);//����������                        
                if (Get_Key_Status()&(1<<KEY_MENU))//ͬʱ���²˵����򳤲�����
                {
                    Demar_Change_Paw_AD(1,INC_DEC_LONG,cur_demar_point);                                
                }
                else
                {
                    Demar_Change_Paw_AD(1,0,cur_demar_point);                    
                }
                Update_Paw_Curve(paw_ad_tab[cur_demar_point],cur_demar_point);    //��������
                Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT);    //��ʾ������
                Put_Paw_Sen_Cur(cur_demar_point,DISP_RT);    //��ʾ������
                Put_Paw_Ad_In_Box(DISP_RT|DISP_INPUT,paw_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ    
            }
            else
            {
                Key_No_Valid_Sound();
            }        
            return(DEMAR_NEW_PAW_AD_IN);//��Ȼ����"AD����"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            if (dem_op_st&OP_ST_IN)    //�����޸�
            {
                dem_op_st |= OP_ST_CHG;    //���޸ı��
                Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //����������
                Put_Paw_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);//����������                        
                if (Get_Key_Status()&(1<<KEY_MENU))//ͬʱ���²˵����򳤲�����
                {
                    Demar_Change_Paw_AD(0,INC_DEC_LONG,cur_demar_point);
                }
                else
                {
                    Demar_Change_Paw_AD(0,0,cur_demar_point);
                }
                Update_Paw_Curve(paw_ad_tab[cur_demar_point],cur_demar_point);    //��������
                Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT);    //��ʾ������
                Put_Paw_Sen_Cur(cur_demar_point,DISP_RT);    //��ʾ������                            
                Put_Paw_Ad_In_Box(DISP_RT|DISP_INPUT,paw_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ    
                return(DEMAR_NEW_PAW_AD_IN);//��Ȼ����"AD����"
            }
            else
            {    
                Key_Valid_Sound();    //������Ч��    
                if (dem_op_st&OP_ST_ER)//�ָ���ʾ
                {
                    Put_Paw_Ad_In_Box(DISP_RT,paw_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ
                }
                dem_op_st = 0;            
                return(DEMAR_NEW_PAW_PRE);//�л���"ǰһ��"
            }
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_NEW_PAW_AD_IN);//��Ȼ����"AD����"
            break;            
    }    
}

//**********************************************************************
//����  ��:O2_Dem_Menu_Pro
//��������:����Ũ�ȴ������궨�˵��Ĵ���(10ms����һ��)
//��ڲ���:key_c--������
//            op--��ǰѡ��
//���ڲ���:�л�����ѡ��
//**********************************************************************
static sint32 O2_Dem_Menu_Pro(uint8 key_c,sint32 op)
{
    struct op_str op_tab[DEMAR_NEW_PAW_OPS]=
    {{DEMAR_O2_SEN_EXIT,O2_Dem_Ret_Butt_Pro},//�˳�
    {DEMAR_O2_SEN_SAVE,O2_Dem_Save_Butt_Pro},    //����
    {DEMAR_O2_SEN_CHCEK,O2_Dem_Check_Butt_Pro},    //���    
    {DEMAR_O2_SEN_NEXT,O2_Dem_Next_Butt_Pro},    //��һ��
    {DEMAR_O2_SEN_PRE, O2_Dem_Pre_Butt_Pro},    //ǰһ��
    {DEMAR_O2_SEN_AD_IN,O2_Dem_Ad_In_Box_Pro}};//AD����
    
    sint32 op_id;
    static uint8 time=0;
    static uint8 flag_dot_disp = FALSE;
    uint8 di;
    
    if (!time)    //mod32(320ms������˸��ʾ)
    {
        if (FALSE == flag_dot_disp)//������ʾ�궨��
        {
            flag_dot_disp = TRUE;
            Put_O2_Dem_Dot(cur_demar_point,DISP_RT|DISP_ER);
        }
        else
        {
            flag_dot_disp = FALSE;
            Put_O2_Dem_Dot(cur_demar_point,DISP_RT);
        }
        switch (op)
        {
            case DEMAR_O2_SEN_EXIT://�˳�
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Ret_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Ret_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_O2_SEN_SAVE://����            
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Save_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Save_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_O2_SEN_NEXT://��һ��                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Next_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Next_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_O2_SEN_PRE://ǰһ��                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Pre_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Pre_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;        
            case DEMAR_O2_SEN_CHCEK://���                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Check_Cur_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Check_Cur_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;    
            case DEMAR_O2_SEN_AD_IN://AD����                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;
                    Put_O2_Ad_In_Box(di|DISP_RT,o2_ad_tab[cur_demar_point]);
                }
                else//ԭ����ʾ�����
                {
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;                
                    Put_O2_Ad_In_Box(di|DISP_RT|DISP_ER,o2_ad_tab[cur_demar_point]);
                }
                dem_op_st ^= OP_ST_ER;
                break;                                            
            default:break;
        }    
    }
    time++;
    time &= 31;    
    op_id = (*((op_tab[op&0xff]).op_pro))(key_c);
    if ((op_id^op)&0x0000ffff)    //�л��˲˵������������˸��ʱ
    {
        time = 0;
    }
    return (op_id);    
}

//**********************************************************************
//����  ��:O2_Dem_Ret_Butt_Pro
//��������:�����������궨"�˳�"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 O2_Dem_Ret_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            Key_Valid_Sound();    //������Ч��
            if (TRUE == Check_Tips_Disp_Dem())    //����ʾ��Ϣ��ɾ����ʾ��Ϣ
                Cancel_Tips_Dem(&save_err_tips_dem);
            dem_op_st = 0;    
            return(DEMAR_MAIN_MENU_O2);//�л���������"�����������궨"ѡ��
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Ret_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_SAVE);//�л���"����"
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_O2_SEN_EXIT);//��Ȼ����"�˳�"    
            break;            
    }    
}

//**********************************************************************
//����  ��:O2_Dem_Save_Butt_Pro
//��������:�����������궨"����"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 O2_Dem_Save_Butt_Pro(uint8 key_c)
{
    uint16 ad;
    
    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            ad = o2_ad;
            if (!(Check_O2_AD(cur_demar_point,ad)&0xff000000))    //��ȷ
            {
                if (Write_EEPROM(EEP_O2+cur_demar_point*(sizeof(o2_ad_tab[cur_demar_point])), ( uint8 *)(&ad), sizeof(ad)))
                {//д��EEPROM����
                    Key_No_Valid_Sound();    //������Ч��    
                    Log_Tips_Dem(&save_err_tips_dem);    //��ʾ���������Ϣ            
                }
                else
                {
                    Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //����������
                    Put_O2_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);    //����������
                    Update_O2_Sen_Curve(ad,cur_demar_point);    //��������
                    Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT);    //��ʾ������
                    Put_O2_Sen_Cur(cur_demar_point,DISP_RT);    //��ʾ������
                    Put_O2_Ad_In_Box(DISP_RT,o2_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ                        
                    Key_Valid_Sound();    //������Ч��    
                    Log_Tips_Dem(&save_ok_tips_dem);    //��ʾ����ɹ���Ϣ                
                }
            }
            else
            {
                Key_No_Valid_Sound();    //������Ч��    
                Log_Tips_Dem(&ad_err_tips_dem);    //��ʾAD������Χ����ʾ��Ϣ
            }    
            return(DEMAR_O2_SEN_SAVE);//��Ȼ����"����"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_CHCEK);//�л���"���"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_EXIT);//�л���"�˳�"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_O2_SEN_SAVE);//��Ȼ����"����"
            break;            
    }    
}

//**********************************************************************
//����  ��:O2_Dem_Check_Butt_Pro
//��������:�����������궨"���"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 O2_Dem_Check_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if(Check_O2_Curve())//���߼�鲻�ϸ�
            {
                Key_No_Valid_Sound();
                Log_Tips_Dem(&curve_err_tips_dem);    //��ʾ���ϸ���Ϣ    
            }
            else
            {
                Key_Valid_Sound();    //������Ч��
                Log_Tips_Dem(&curve_ok_tips_dem);    //��ʾ�ϸ���Ϣ    
            }
            return(DEMAR_O2_SEN_CHCEK);//��Ȼ����"���"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_NEXT);//�л���"��һ��"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_SAVE);//�л���"����"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_O2_SEN_CHCEK);//��Ȼ����"���"
            break;            
    }    
}

//**********************************************************************
//����  ��:O2_Dem_Next_Butt_Pro
//��������:�����������궨"��һ��"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 O2_Dem_Next_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if (O2_SEN_DEM_POINTS-1 > cur_demar_point)//������һ��
            {
                Put_O2_Dem_Dot(cur_demar_point,DISP_RT);//�ָ���ʾ��ǰ�궨���ͼ��            
                cur_demar_point++;
                Put_O2_Dem_Point_Box(DISP_RT,o2_demar_tab[cur_demar_point]);    //��ʾ�궨���ѹ��ֵ
                Put_O2_Ad_In_Box(DISP_RT,o2_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ
                Key_Valid_Sound();    //������Ч��
            }
            else
            {
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_O2_SEN_NEXT);//��Ȼ����"��һ��"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_PRE);//�л���"ǰһ��"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_CHCEK);//�л���"���"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_O2_SEN_NEXT);//��Ȼ����"��һ��"
            break;            
    }    
}

//**********************************************************************
//����  ��:O2_Dem_Pre_Butt_Pro
//��������:�����������궨"ǰһ��"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 O2_Dem_Pre_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if (cur_demar_point > 0)//����ǰһ��
            {
                Put_O2_Dem_Dot(cur_demar_point,DISP_RT);//�ָ���ʾ��ǰ�궨���ͼ��            
                cur_demar_point--;
                Put_O2_Dem_Point_Box(DISP_RT,o2_demar_tab[cur_demar_point]);    //��ʾ�궨���ѹ��ֵ
                Put_O2_Ad_In_Box(DISP_RT,o2_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ
                Key_Valid_Sound();    //������Ч��
            }
            else
            {
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_O2_SEN_PRE);//��Ȼ����"ǰһ��"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_AD_IN);//�л���"AD����"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_NEXT);//�л���"��һ��"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_O2_SEN_PRE);//��Ȼ����"ǰһ��"
            break;            
    }    
}

//**********************************************************************
//����  ��:O2_Dem_Ad_In_Box_Pro
//��������:�����������궨"AD����"���ڴ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 O2_Dem_Ad_In_Box_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if (dem_op_st&OP_ST_IN)    //��������״̬
            {
                dem_op_st &= ~OP_ST_IN;    //��Ϊѡ��״̬
                Put_O2_Ad_In_Box(DISP_RT,o2_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ
                if (dem_op_st&OP_ST_CHG)//���޸��򱣴�
                {
                    if (Write_EEPROM(EEP_O2+cur_demar_point*(sizeof(o2_ad_tab[cur_demar_point])), ( uint8 *)(&(o2_ad_tab[cur_demar_point])), sizeof(o2_ad_tab[cur_demar_point])))
                    {
                        Key_No_Valid_Sound();
                        Log_Tips_Dem(&save_err_tips_dem);    //��ʾ�������
                    }
                    else
                    {
                        Key_Valid_Sound();    //������Ч��    
                    }
                }
                else
                {
                    Key_Valid_Sound();    //������Ч��    
                }
                dem_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
            }
            else//�����޸�״̬
            {
                dem_op_st |= OP_ST_IN;
                Put_O2_Ad_In_Box(DISP_RT|DISP_INPUT,o2_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ
                Key_Valid_Sound();    //������Ч��    
            }
            return(DEMAR_O2_SEN_AD_IN);//��Ȼ����"AD����"
            break;
        case KEY_PRESSED|KEY_INC://������
            if (dem_op_st&OP_ST_IN)
            {
                dem_op_st |= OP_ST_CHG;    //���޸ı��
                Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //����������
                Put_O2_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);//����������                        
                if (Get_Key_Status()&(1<<KEY_MENU))//ͬʱ���²˵����򳤲�����
                {
                    Demar_Change_O2_AD(1,INC_DEC_LONG,cur_demar_point);                                
                }
                else
                {
                    Demar_Change_O2_AD(1,0,cur_demar_point);                    
                }
                Update_O2_Sen_Curve(o2_ad_tab[cur_demar_point],cur_demar_point);    //��������
                Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT);    //��ʾ������
                Put_O2_Sen_Cur(cur_demar_point,DISP_RT);    //��ʾ������
                Put_O2_Ad_In_Box(DISP_RT|DISP_INPUT,o2_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ    
            }
            else
            {
                Key_No_Valid_Sound();
            }        
            return(DEMAR_O2_SEN_AD_IN);//��Ȼ����"AD����"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            if (dem_op_st&OP_ST_IN)    //�����޸�
            {
                dem_op_st |= OP_ST_CHG;    //���޸ı��
                Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //����������
                Put_O2_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);//����������                        
                if (Get_Key_Status()&(1<<KEY_MENU))//ͬʱ���²˵����򳤲�����
                {
                    Demar_Change_O2_AD(0,INC_DEC_LONG,cur_demar_point);
                }
                else
                {
                    Demar_Change_O2_AD(0,0,cur_demar_point);
                }
                Update_O2_Sen_Curve(o2_ad_tab[cur_demar_point],cur_demar_point);    //��������
                Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT);    //��ʾ������
                Put_O2_Sen_Cur(cur_demar_point,DISP_RT);    //��ʾ������                            
                Put_O2_Ad_In_Box(DISP_RT|DISP_INPUT,o2_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ    
                return(DEMAR_O2_SEN_AD_IN);//��Ȼ����"AD����"
            }
            else
            {    
                Key_Valid_Sound();    //������Ч��    
                if (dem_op_st&OP_ST_ER)//�ָ���ʾ
                {
                    Put_O2_Ad_In_Box(DISP_RT,o2_ad_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰADֵ
                }
                dem_op_st = 0;            
                return(DEMAR_O2_SEN_PRE);//�л���"ǰһ��"
            }
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_O2_SEN_AD_IN);//��Ȼ����"AD����"
            break;            
    }    
}

//**********************************************************************
//����  ��:Vt_Dem_Menu_Pro
//��������:�������궨�˵��Ĵ���(10ms����һ��)
//��ڲ���:key_c--������
//            op--��ǰѡ��
//���ڲ���:�л�����ѡ��
//**********************************************************************
static sint32 Vt_Dem_Menu_Pro(uint8 key_c,sint32 op)
{
    struct op_str op_tab[DEMAR_VT_OPS]=
    {{DEMAR_VT_EXIT,Vt_Dem_Ret_Butt_Pro},//�˳�
    {DEMAR_VT_SAVE,Vt_Dem_Save_Butt_Pro},    //����
    {DEMAR_VT_CHECK,Vt_Dem_Check_Butt_Pro},    //���    
    {DEMAR_VT_NEXT,Vt_Dem_Next_Butt_Pro},    //��һ��
    {DEMAR_VT_PRE,Vt_Dem_Pre_Butt_Pro},    //ǰһ��
    {DEMAR_VT_LPS_IN,Vt_Dem_Lps_In_Box_Pro}};//Ȧ������
    
    sint32 op_id;
    static uint8 time=0;
    static uint8 flag_dot_disp=FALSE;
    uint8 di;
    
    if (!time)    //mod32(320ms������˸��ʾ)
    {
        if (FALSE == flag_dot_disp)//������ʾ�궨��
        {
            flag_dot_disp = TRUE;
            Put_Vt_Dem_Dot(cur_demar_point,DISP_RT|DISP_ER);
        }
        else
        {
            flag_dot_disp = FALSE;
            Put_Vt_Dem_Dot(cur_demar_point,DISP_RT);
        }    
        switch (op)
        {
            case DEMAR_VT_EXIT://�˳�
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Ret_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Ret_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_VT_SAVE://����            
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Save_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Save_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_VT_NEXT://��һ��                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Next_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Next_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_VT_PRE://ǰһ��                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Pre_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Pre_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;        
            case DEMAR_VT_CHECK://���                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_Check_Cur_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//ԭ����ʾ�����
                {
                    Put_Check_Cur_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;    
            case DEMAR_VT_LPS_IN://Ȧ������                
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;
                    Put_Vt_Lps_In_Box(di|DISP_RT,vt_lap_tab[cur_demar_point]);
                }
                else//ԭ����ʾ�����
                {
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;                
                    Put_Vt_Lps_In_Box(di|DISP_RT|DISP_ER,vt_lap_tab[cur_demar_point]);
                }
                dem_op_st ^= OP_ST_ER;
                break;                                            
            default:break;
        }    
    }
    time++;
    time &= 31;    
    op_id = (*((op_tab[op&0xff]).op_pro))(key_c);
    if ((op_id^op)&0x0000ffff)    //�л��˲˵������������˸��ʱ
    {
        time = 0;
    }
    return (op_id);    
}

//**********************************************************************
//����  ��:Vt_Dem_Ret_Butt_Pro
//��������:�������궨"�˳�"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Vt_Dem_Ret_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            Key_Valid_Sound();    //������Ч��
            if (TRUE == Check_Tips_Disp_Dem())    //����ʾ��Ϣ��ɾ��
            {
                Cancel_Tips_Dem(&save_err_tips_dem);
            }
            dem_op_st = 0;    
            phase_vent_ctrl = PHASE_STOP_VENT;//ֹͣ��������
            return(DEMAR_MAIN_MENU_VT);//�л���������"�������궨"ѡ��
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Ret_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_SAVE);//�л���"����"
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_VT_EXIT);//��Ȼ����"�˳�"    
            break;            
    }    
}

//**********************************************************************
//����  ��:Vt_Dem_Save_Butt_Pro
//��������:�������궨"����"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Vt_Dem_Save_Butt_Pro(uint8 key_c)
{
    sint32 lps;
    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            lps = demar_laps;
            if (Check_Vt_Laps(cur_demar_point,lps)&0xff000000)    //Ȧ������������Χ��
            {
                Key_No_Valid_Sound();
                Log_Tips_Dem(&lps_err_tips_dem);    //��ʾ��ʾ��Ϣ        
            }
            else
            {
                if(Write_EEPROM(EEP_VTE+cur_demar_point*(sizeof(vt_lap_tab[cur_demar_point])), ( uint8 *)(&(lps)), sizeof(lps)))
                {//�������
                    Key_No_Valid_Sound();    //������Ч��
                    Log_Tips_Dem(&save_err_tips_dem);    //��ʾ��ʾ��Ϣ                
                }
                else
                {
                    Key_Valid_Sound();    //������Ч��
                    Put_Vt_Sen_Cur(cur_demar_point-1,DISP_ER|DISP_RT);    //����������
                    Put_Vt_Sen_Cur(cur_demar_point,DISP_ER|DISP_RT);    //����������
                    Update_Vt_Cur(lps,cur_demar_point);    //��������
                    Put_Vt_Sen_Cur(cur_demar_point-1,DISP_RT);    //��ʾ������
                    Put_Vt_Sen_Cur(cur_demar_point,DISP_RT);    //��ʾ������    
                    Put_Vt_Lps_In_Box(DISP_RT,vt_lap_tab[cur_demar_point]);    //��ʾ��ǰȦ��    
                    Log_Tips_Dem(&save_ok_tips_dem);    //��ʾ��ʾ��Ϣ            
                }
            }    
            return(DEMAR_VT_SAVE);//��Ȼ����"����"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_CHECK);//�л���"���"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_EXIT);//�л���"�˳�"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_VT_SAVE);//��Ȼ����"����"
            break;            
    }    
}

//**********************************************************************
//����  ��:Vt_Dem_Check_Butt_Pro
//��������:�������궨"���"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Vt_Dem_Check_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if (Check_EEP_Vte())//���߼�鲻�ϸ�
            {
                Key_No_Valid_Sound();    //������Ч��
                Log_Tips_Dem(&curve_err_tips_dem);    //��ʾ��ʾ��Ϣ
            }
            else
            {
                Key_Valid_Sound();    //������Ч��
                Log_Tips_Dem(&curve_ok_tips_dem);    //��ʾ��ʾ��Ϣ
            }
            return(DEMAR_VT_CHECK);//��Ȼ����"���"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_NEXT);//�л���"��һ��"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_SAVE);//�л���"����"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_VT_CHECK);//��Ȼ����"���"
            break;            
    }    
}

//**********************************************************************
//����  ��:Vt_Dem_Next_Butt_Pro
//��������:�������궨"��һ��"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Vt_Dem_Next_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if (VT_DEMAR_POINTS-1 > cur_demar_point)//������һ��
            {
                Put_Vt_Dem_Dot(cur_demar_point,DISP_RT);//�ָ���ʾ��ǰ�궨���ͼ��            
                cur_demar_point++;
                Put_Vt_Dem_Point_Box(DISP_RT,vt_demar_tab[cur_demar_point]);    //��ʾ�궨��ĳ�����
                Put_Vt_Lps_In_Box(DISP_RT,vt_lap_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰȦ��
                Key_Valid_Sound();    //������Ч��
            }
            else
            {
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_VT_NEXT);//��Ȼ����"��һ��"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_PRE);//�л���"ǰһ��"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_CHECK);//�л���"���"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_VT_NEXT);//��Ȼ����"��һ��"
            break;            
    }    
}

//**********************************************************************
//����  ��:Vt_Dem_Pre_Butt_Pro
//��������:�������궨"ǰһ��"��ť����
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Vt_Dem_Pre_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if (cur_demar_point > 0)//����ǰһ��
            {
                Put_Vt_Dem_Dot(cur_demar_point,DISP_RT);//�ָ���ʾ��ǰ�궨���ͼ��            
                cur_demar_point--;
                Put_Vt_Dem_Point_Box(DISP_RT,vt_demar_tab[cur_demar_point]);    //��ʾ�궨�㳱����
                Put_Vt_Lps_In_Box(DISP_RT,vt_lap_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰȦ��
                Key_Valid_Sound();    //������Ч��
            }
            else
            {
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_VT_PRE);//��Ȼ����"ǰһ��"
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_LPS_IN);//�л���"Ȧ������"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            Key_Valid_Sound();    //������Ч��        
            if (dem_op_st&OP_ST_ER)//�ָ���ʾ
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_NEXT);//�л���"��һ��"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_VT_PRE);//��Ȼ����"ǰһ��"
            break;            
    }    
}

//**********************************************************************
//����  ��:Vt_Dem_Lps_In_Box_Pro
//��������:�������궨"Ȧ������"���ڴ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID
//**********************************************************************
static sint32 Vt_Dem_Lps_In_Box_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            if (dem_op_st&OP_ST_IN)    //��������״̬
            {
                dem_op_st &= ~OP_ST_IN;    //��Ϊѡ��״̬
                Put_Vt_Lps_In_Box(DISP_RT,vt_lap_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰȦ��
                if (dem_op_st&OP_ST_CHG)//���޸��򱣴�
                {
                    if(Write_EEPROM(EEP_VTE+cur_demar_point*(sizeof(vt_lap_tab[cur_demar_point])), ( uint8 *)(&(vt_lap_tab[cur_demar_point])), sizeof(vt_lap_tab[cur_demar_point])))
                    {
                        Key_No_Valid_Sound();
                    }
                    else
                    {
                        Key_Valid_Sound();    //������Ч��    
                    }
                }
                else
                {
                    Key_Valid_Sound();    //������Ч��    
                }
                dem_op_st &= ~OP_ST_CHG;    //ȥ���޸ı��
            }
            else//�����޸�״̬
            {
                dem_op_st |= OP_ST_IN;
                Put_Vt_Lps_In_Box(DISP_RT|DISP_INPUT,vt_lap_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰȦ��
                Key_Valid_Sound();    //������Ч��    
            }
            return(DEMAR_VT_LPS_IN);//��Ȼ����"Ȧ������"
            break;
        case KEY_PRESSED|KEY_INC://������
            if (dem_op_st&OP_ST_IN)
            {
                dem_op_st |= OP_ST_CHG;    //���޸ı��
                Put_Vt_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //����������
                Put_Vt_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);    //����������                        
                if (Get_Key_Status()&(1<<KEY_MENU))//ͬʱ���²˵����򳤲�����
                {
                    Demar_Change_VT_Lps(1,INC_DEC_LONG,cur_demar_point);
                }
                else
                {
                    Demar_Change_VT_Lps(1,0,cur_demar_point);
                }
                Update_Vt_Cur(vt_lap_tab[cur_demar_point],cur_demar_point);    //��������
                Put_Vt_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_NOR);    //��ʾ������
                Put_Vt_Sen_Cur(cur_demar_point,DISP_RT|DISP_NOR);    //��ʾ������                    
                Put_Vt_Lps_In_Box(DISP_RT|DISP_INPUT,vt_lap_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰȦ��
            }
            else
            {
                Key_No_Valid_Sound();
            }        
            return(DEMAR_VT_LPS_IN);//��Ȼ����"Ȧ������"
            break;    
        case KEY_PRESSED|KEY_DEC://��С��
            if (dem_op_st&OP_ST_IN)    //�����޸�
            {
                dem_op_st |= OP_ST_CHG;    //���޸ı��
                Put_Vt_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //����������
                Put_Vt_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);    //����������                    
                if (Get_Key_Status()&(1<<KEY_MENU))//ͬʱ���²˵����򳤲�����
                {
                    Demar_Change_VT_Lps(0,INC_DEC_LONG,cur_demar_point);
                }
                else
                {
                    Demar_Change_VT_Lps(0,0,cur_demar_point);
                }
                Update_Vt_Cur(vt_lap_tab[cur_demar_point],cur_demar_point);    //��������
                Put_Vt_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_NOR);    //��ʾ������
                Put_Vt_Sen_Cur(cur_demar_point,DISP_RT|DISP_NOR);    //��ʾ������                        
                Put_Vt_Lps_In_Box(DISP_RT|DISP_INPUT,vt_lap_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰȦ��
                return(DEMAR_VT_LPS_IN);//��Ȼ����"Ȧ������"
            }
            else
            {        
                Key_Valid_Sound();    //������Ч��    
                if (dem_op_st&OP_ST_ER)//�ָ���ʾ
                {
                    Put_Vt_Lps_In_Box(DISP_RT,vt_lap_tab[cur_demar_point]);//��ʾ�궨��ĵ�ǰȦ��
                }
                dem_op_st = 0;            
                return(DEMAR_VT_PRE);//�л���"ǰһ��"
            }
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(DEMAR_VT_LPS_IN);//��Ȼ����"Ȧ������"
            break;            
    }    
}

//**********************************************************************
//����  ��:Dem_Lg_Sel_Disp_Pro
//��������:����ѡ������µ���ʾ����(100msִ��һ��)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Dem_Lg_Sel_Disp_Pro(void)
{}

//**********************************************************************
//����  ��:Dem_Sel_Disp_Pro
//��������:ѡ��Ҫ�궨��ѡ������µ���ʾ����(100msִ��һ��)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Dem_Sel_Disp_Pro(void)
{}

//**********************************************************************
//����  ��:Paw_Dem_Disp_Pro
//��������:�궨ѹ��������ʱ����ʾ������(100msִ��һ��)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Paw_Dem_Disp_Pro(void)
{
    static sint32 ad_disp = 0;    //AD����ʾֵ
    static sint32 p_disp = 0;    //ѹ������ʾֵ
    
    if (FALSE == Check_Tips_Disp_Dem()) //����ʾ��Ϣ��������ʾ
    {
        if (paw_ad != ad_disp)
        {
            ad_disp = paw_ad;
            Put_Paw_Ad_Val(DISP_RT,ad_disp); 
        }
        if (airway_p != p_disp)
        {
            p_disp = airway_p;
            Put_Cur_Pressur_Dem_Paw(DISP_RT,p_disp);
        }
    }
}

//**********************************************************************
//����  ��:O2_Dem_Disp_Pro
//��������:�궨����������ʱ����ʾ������(100msִ��һ��)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void O2_Dem_Disp_Pro(void)
{
    static sint32 ad_disp = 0;    //AD����ʾֵ
    static sint32 o2_disp = 0;    //����Ũ����ʾֵ
    static sint32 o2_volt_disp = 0;
    if (FALSE == Check_Tips_Disp_Dem()) //����ʾ��Ϣ��������ʾ
    {
       if (o2_ad != ad_disp)
        {
            ad_disp = o2_ad;
            Put_O2_Ad_Val(DISP_RT,ad_disp); 
        }
        if (cur_o2 != o2_disp)
        {
            o2_disp = cur_o2;
            Put_Cur_O2_Dem_O2(DISP_RT,o2_disp);
        }
    }
    if (o2_vol != o2_volt_disp)    //��ʾ����ص�ѹ
    {
        o2_volt_disp = o2_vol;
        Put_O2_Cell_Volt_Dem_O2(DISP_RT,o2_volt_disp);
    }
}

//**********************************************************************
//����  ��:Vt_Dem_Disp_Pro
//��������:�궨������ʱ����ʾ������(100msִ��һ��)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Vt_Dem_Disp_Pro(void)
{
    static sint32 vt_lps_disp = 0;    //����Ȧ������ʾֵ
    static sint32 vt_disp = 0;    //��������ʾֵ
    static uint8 flag_br_icon_disp = 0;//����ͼ����ʾ
    
    
    if (FALSE == Check_Tips_Disp_Dem()) //����ʾ��Ϣ��������ʾ
    {
        if (demar_laps != vt_lps_disp)
        {
            vt_lps_disp = demar_laps;
            Put_Vt_Lps_Val(DISP_RT,vt_lps_disp); 
        }
        if (demarc_vte != vt_disp)
        {
            vt_disp = demarc_vte;
            Put_Cur_Vt(DISP_RT,vt_disp);
        }
    }
     Set_Graphic_Color(COL_WHITE,COL_DEEPGREEN);
    switch(phase_vent_ctrl)
    {
        case PHASE_VCV_INSP:    //����������������ʾ����ͼ��
        case PHASE_VCV_HOLD:
            if(0x00 == (flag_br_icon_disp&0x01))    //��û��ʾ����ͼ��
            {
                flag_br_icon_disp = 0x01;    //��ʾ����ͼ��
                Put_Image(212,80,Vent_Icon_Press,WRITE_OVERLAY);
                LCD_Refresh_Area(212,80,243,109);
            }    
            break;
        default:
            if(0x00 == (flag_br_icon_disp&0x02))//��û��ʾ����ͼ��
            {
                flag_br_icon_disp = 0x02;    //��ʾ����ͼ��
                Put_Image(212,80,Vent_Icon,WRITE_OVERLAY);
                LCD_Refresh_Area(212,80,243,109);                    
            }
            break;                    
    }    
}

//**********************************************************************
//����  ��:Menu_Lg_Sel_Op_Dem_Pro
//��������:�궨ʱ�������ò˵���ѡ���
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
static sint32 Menu_Lg_Sel_Op_Dem_Pro(uint8 key_c,sint32 op)
{ 
    sint32 op_id;    
    struct op_str op_tab[MENU_DEMAR_LG_OP_NUM]=
    {{MENU_DEMAR_LG_SEL_CH_OP,Dem_Ch_Butt_Pro},//"����"
    {MENU_DEMAR_LG_SEL_EN_OP,Dem_En_Butt_Pro}};//"English"
    static uint8 time; //��˸ʱ��

    if (!time)    //mod32(320ms������˸��ʾ)
    {
        switch (op)
        {
            case MENU_DEMAR_LG_SEL_CH_OP://����    
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_CH_Butt_Lg_Sel(DISP_RT);
                }
                else//ԭ����ʾ�����
                {
                    Put_CH_Butt_Lg_Sel(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;        
            case MENU_DEMAR_LG_SEL_EN_OP://English    
                if (dem_op_st&OP_ST_ER)    //��������ʾ
                {//�ָ���ʾ
                    Put_EN_Butt_Lg_Sel(DISP_RT);
                }
                else//ԭ����ʾ�����
                {
                    Put_EN_Butt_Lg_Sel(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;                                    
            default:break;    
        }
    }
    time++;
    time &= 31;    
    op_id = (*((op_tab[op&0xff]).op_pro))(key_c);
    if ((op_id^op)&0x0000ffff)    //�л��˲˵������������˸��ʱ
    {
        time = 0;
    }
    return (op_id);
}

//**********************************************************************
//����  ��:Dem_Ch_Butt_Pro
//��������:"����"��ť�Ĵ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID 
//**********************************************************************
static sint32 Dem_Ch_Butt_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            dem_op_st |= OP_ST_SEL;        //ѡ��״̬
            Set_Language_Type(CHINESE);    //������������
            Key_Valid_Sound();        //��������Ч��        
            return(DEMAR_MAIN_MENU_PAW);//�л���"ѹ���������궨"        
            break;
        case KEY_PRESSED|KEY_DEC://���ټ�
            Key_Valid_Sound();        //��������Ч��                
            if (dem_op_st&OP_ST_ER)    //�ָ���ʾ
            {
                Put_CH_Butt_Lg_Sel(DISP_RT);
            }    
            dem_op_st = OP_ST_SEL;
            return(MENU_DEMAR_LG_SEL_EN_OP);//�л���"English"                    
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(MENU_DEMAR_LG_SEL_CH_OP);//��Ȼ����"����"
            break;            
    }        
}

//**********************************************************************
//����  ��:Dem_En_Butt_Pro
//��������:"English"��ť�Ĵ���
//��ڲ���:key_c--��������
//���ڲ���:�л�����ѡ��ID 
//**********************************************************************
static sint32 Dem_En_Butt_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"ȷ��"��
            dem_op_st |= OP_ST_SEL;        //ѡ��״̬
            Key_Valid_Sound();        //��������Ч��
            Set_Language_Type(ENGLISH);    //������������        
            return(DEMAR_MAIN_MENU_PAW);//�л���"ѹ���������궨"        
            break;
        case KEY_PRESSED|KEY_INC://������
            Key_Valid_Sound();        //��������Ч��                
            if (dem_op_st&OP_ST_ER)    //�ָ���ʾ
            {
                Put_EN_Butt_Lg_Sel(DISP_RT);
            }
            dem_op_st = OP_ST_SEL;    
            return(MENU_DEMAR_LG_SEL_CH_OP);//�л���"����"                    
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //������Ч��
            }
            return(MENU_DEMAR_LG_SEL_EN_OP);//��Ȼ����"English"
            break;            
    }        
}



//**********************************************************************
//����  ��:Log_Tips_Dem
//��������:����ʾ��Ϣ������ʾ����
//��ڲ���:tips_ptr--��ʾ��Ϣ��ָ��
//���ڲ���:��
//**********************************************************************
static void Log_Tips_Dem(Tips_Struct*tips_ptr)
{
    if (NULL == tips_ptr) return;
    if (!flag_tips_saved)    //��δ���汻���ǵ������򱣴�
    {
        flag_tips_saved = TRUE;
        Save_Area(12,32,241,69,(uint32 *)save_buf_dem);                //���渲������
    }
    cur_tips_ptr_dem = tips_ptr;
    tips_disp_time_dem = TIPS_DISP_TIME_DEM;
    tips_disp_trigger_dem = TRIGGER_ON;    //������ʾ��ʾ��Ϣ
}

//*********************************************************************
//����  ��:Cancel_Tips_Dem
//��������:����ʾ��Ϣ����ʾ������ɾ��
//��ڲ���:tips_ptr--��ʾ��Ϣ��ָ��
//���ڲ���:��
//*********************************************************************
static void Cancel_Tips_Dem(Tips_Struct*tips_ptr)
{
    if (NULL == tips_ptr) return;                //�����ڲ����ĺϷ���
    if (NO_TIPS == tips_ptr->tips_id) return;
    if (FALSE == Check_Tips_Disp_Dem()) return;        //��ǰ�ޱ�����ϢҪȡ���򷵻�
    
    Recover_Area(12,32,241,69,(uint32 *)save_buf_dem);//�ָ������ǵ�����
    LCD_Refresh_Area(12,32,241,69);
    cur_tips_ptr_dem = &blank_tips;            //�����ʾ��Ϣ
    flag_tips_saved = FALSE;
    tips_disp_trigger_dem = TRIGGER_OFF;    //�����ر���ʾ��Ϣ
}

//**********************************************************************
//����  ��:Put_Tips_Mess_Dem
//��������:��ʾ��ʾ��Ϣ(10msִ��һ��)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
static void Put_Tips_Mess_Dem(void)
{
    if (TRIGGER_ON == tips_disp_trigger_dem)
    {
        tips_disp_trigger_dem = TRIGGER_NONE;
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
        Set_Write_Mode(WRITE_OVERLAY);//����
        Put_Image(12,32,Tip_Box,WRITE_OVERLAY);

        //Set_Write_Mode(WRITE_EXOR);    //���
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_BLACK,COL_WHITE);
        Put_Str(15,35,Language_Str((uint8*)cur_tips_ptr_dem->ch_tips,(uint8*)cur_tips_ptr_dem->en_tips));
        LCD_Refresh_Area(12,32,241,69);
    }
    if (Check_Tips_Disp_Dem()) //������ʾ��ʾ��Ϣ�򵹼�ʱ
    {
        if(0 == --tips_disp_time_dem)    //����ʱʱ�䵽
        Cancel_Tips_Dem(&save_err_tips_dem);
    }
}

//**********************************************************************
//����  ��:Check_Tips_Disp_Dem
//��������:��鵱ǰ�Ƿ���ʾ��ʾ��Ϣ
//��ڲ���:��
//���ڲ���:TRUE--��ʾ
//**********************************************************************
static uint8 Check_Tips_Disp_Dem(void)
{
    if (NO_TIPS ==cur_tips_ptr_dem->tips_id) return(FALSE);
    return(TRUE);
}