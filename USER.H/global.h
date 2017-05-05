//**********************************************************************
//�ļ���:global.h
//**********************************************************************
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "type_def.h"
#include "lcd2478.h"


#define SOFT_VERSION             1.0

#define  SDJ_SOFT                 1
#define DDLV_SOFT                 0



#define TFTLCD     1
#define LCDTYPE    TFTLCD   

////////////////////////////ADD BY WJG 2012.06.01
#define MONOLCD    0
#define TFTLCD     1
#define LCDTYPE    TFTLCD   

////////////////////////////////////////////
#define  USB_ENABLE                 0

////////////////////////////////////////////
#define  DEFAULT_USER_ID            1

#define  DEFAULT_PH_DISPLAY_DOT    0x08
#define  DEFAULT_MV_DISPLAY_DOT    0x02

//Ĭ���¶Ȳ�����ֵ
#define  DEFAULT_TEMP_COMP          25
////////////////////////////END

///////////////////////////////////////////////////////
//�����ļ��ֹ���ģʽ
///////////////////////////////////////////////////////

#define MACHINE_NORMAL_MODE        0    //��������ģʽ
#define MACHINE_SERVICE_MODE       1    //ά��ģʽ
#define MACHINE_DEMARC_MODE        2    //�궨ģʽ
#define MACHINE_SCREENCAL_MODE     3    //��ĻУ׼״̬

///////////////////////////////////////////////////////
//�������������ļ��ֹ���״̬
//////////////////////////////////////////////////////
#define STAT_NULL               0x00
#define STAT_NORMAL             0x01
#define STAT_SYS_CAL            0x02
#define STAT_USER_CAL           0x03



///////////////////////////////////////////////////////
//������Һ��׼
//////////////////////////////////////////////////////
#define CHINESE_S               0x00
#define EUROPE_S                0x01
#define AMERICAN_S              0x02

///////////////////////////////////////////////////////
//�絼�� ״̬�� �� ��������
//////////////////////////////////////////////////////
#define PARAM_DDLV              0x00
#define PARAM_TDS               0x01
#define PARAM_SALTY             0x02
#define PARAM_NULL              0x0A

///////////////////////////////////////////////////////
//�絼�� ״̬�� �� ���ֲ���ģʽ
//////////////////////////////////////////////////////
#define MODE_CONTINUE           0x00
#define MODE_TIMER              0x01
#define MODE_BALANCE            0x02
#define MODE_NULL               0x0A

///////////////////////////////////////////////////////
//�絼�� ״̬�� �� ���ֲ�����λ
//////////////////////////////////////////////////////

#define MS_CM                   0x01
#define US_CM                   0x02


///////////////////////////////////////////////////////
//�����ļ���״̬
//////////////////////////////////////////////////////

#define KEY_PRESS               0x01  //����
#define KEY_RELEASE             0x02  //�ɿ�
#define KEY_DO_START            0x04  //��ʼ����˰���
#define KEY_DO_OVER             0x08  //�������


////////////����Ԥ��ʱ�� ��
#define     PREHOT_TIME           1800
//�������ʱ�� s
#define    ENTER_SLEEP_TIME       300

//��ȼ� sysУ׼��������
#define   SYS_CAL_ZER0A           1  //read ad7190ch3     0mv
#define   SYS_CAL_ZER0B           2  //read ad7190ch4     0mv
#define   SYS_CAL_LOW1A           3  //read ad7190ch4   100mv
#define   SYS_CAL_LOW1B           4  //read ad7190ch4   100mv
#define   SYS_CAL_LOW2A           5  //read ad7190ch3  -100mv
#define   SYS_CAL_LOW2B           6  //read ad7190ch4   100mv
#define   SYS_CAL_FULL1           7  //read ad7190ch1  1400mv
#define   SYS_CAL_FULL2           8  //read ad7190ch2 -1400mv
#define   SYS_CAL_FULL3           9  //read ad7190ch2 -2000mv
#define   SYS_CAL_OVER           10
//
//�絼���� sysУ׼����
#define   SYS_CAL_1A              1  //����У׼�ĸ��������
#define   SYS_CAL_1B              2
#define   SYS_CAL_2A              3
#define   SYS_CAL_2B              4  
#define   SYS_CAL_3A              5  
#define   SYS_CAL_3B              6 
 
#define   SYS_CAL_4A              7  
#define   SYS_CAL_4B              8  
#define   SYS_CAL_5A              9  
#define   SYS_CAL_5B              10  
#define   SYS_CAL_6A              11  
#define   SYS_CAL_6B              12


#define   SYS_CAL_END             13
//�û�У׼

#define   USER_CAL_STEP1          1
#define   USER_CAL_STEP2          2
#define   USER_CAL_STEP3          3
#define   USER_CAL_STEP4          4
#define   USER_CAL_STEP5          5
#define   USER_CAL_END            6


//////////////////////////////////////
//�����¶ȳ��� 5-50 �Ⱥ� ������ʾ�û�

#define   TEMP_SENSOR_LOW           5
#define   TEMP_SENSOR_HIGH         50
#define   TEMP_SENSOR_NC_VALUE    999

#define   TEMP_UNIT_OC           0x01
#define   TEMP_UNIT_OF           0x80


//У���ֽ�
#define   REC_CHECK_DATA1       0x55
#define   REC_CHECK_DATA2       0xaa

//���ȵ����ȼ�
#define   BRIGHT_CLASS_TOTAL      20
#define   DEFAULT_BRIGHTNESS       8
#define   SLEEP_BRIGHTNESS         1


//����ɼ��׶Σ��������봰�ڵ�ID
#define   PASSWORD_INPUT_ID  0x12345678
#define   PASSWORD_DATA1         8866
#define   PASSWORD_DATA2         9999

//�ؼ��ṹ��
struct KeyStruct{
    uint32 X_Position;     /* X */
    uint32 Y_Position;     /* Y */
    uint32 Value;          /* value */
};

//�洢��¼�ṹ��
typedef union    
{
   float  rec_float;    //���ݿ����Ǹ�������Ҳ����������
   uint32 rec_uint32;
}un_rec_data;


extern uint32   user_id,phmv_division,which_standard,lcd_brightness;      //��ʾ���Աȶȵ�ֵ
extern uint32   select_ch,switch_flag,match_item,power_fre;
extern uint32   sound_switch;

extern uint32   key_value,Key_Status,electrode_class;
extern struct   KeyStruct Key[15];

extern uint32   machine_mode,current_mode,mode_run_flag;
extern uint32   power_on_flag,power_on_clr_cnt,elapsed_time;
extern uint32   Error_Info_Flag,Warning_Info_Flag;

extern uint32   ph_negative_flag,mv_negative_flag,full_flag;

extern uint32   ad_raw_data,flag_new_data,stable_flag;
extern uint32   ADS1100_Raw,sample_flag;

extern float    mv_data,ph_data,get_coef,ddlv_data;

extern float    elec_cons,temp_cons,TDS_cons;           //�缫���� �²�ϵ�� TDSϵ��
extern uint32   DDLV_Param,DDLV_Mode,DDLV_unit;         //�絼�ʵĲ���ģʽ
extern uint32   measure_timer_sec,measure_timer_sec_real;
extern uint32   balance_sec,balance_sec_real;
extern  float   balance_data;
//
extern uint32   filter_phase,switch_action_step;
extern uint32   cal_ddlv_buf[6][2]; //����ϵͳУ׼������
extern uint32   cal_ddlv_CONS[6];

extern uint32   cal_ddlv_b_flag[6];
extern float    cal_ddlv_b[6],cal_ddlv_k[6];

extern uint32   cpu_ch_adc3,cpu_ch_adc4;
extern uint32   zero_data[4],low1_data[4],low2_data[4],full1_data[4],full2_data[4],full3_data[4];
extern uint32   user_cal_num,user_cal_data[5];    //����û�У׼������ ������

extern float    sys_coef1, sys_coef2, sys_coef3, sys_coef4, sys_coef5, sys_coef6;
extern float   user_coef1,user_coef2,user_coef3,user_coef4,user_coef5,user_coef_left;
//����絼�ʵ��û�У׼ϵ��
extern float    ddlv_R,ddlv_coef;

extern un_rec_data  user_cal_phdata[5];
extern un_rec_data  user_cal_phdata_dis[5];
extern uint32       user_cal_point[5];

extern uint32   point2_cal_start,sys_cal_ok,user_cal_ok,point2_cal_step;

//////////////////////////////////
extern uint32      temp_comp_manual,manual_temp_comp_flag;
extern float       temp_float_ptc;
////////////////////////////�����Զ������������
extern uint32   no_key_time;
//////////////////////////////////
extern uint32   rec_total_num,rec_real_num,rec_total_pages,rec_curre_index;

extern uint32  need_recover,need_recover2,from_menu;
extern uint32  save_buf[LINE_SIZE*SCR_YSIZE/1];//����ʱ����Ļ��������ڴ����ּ��̷��ػָ�
extern uint32  save_buf2[LINE_SIZE*SCR_YSIZE/1];

extern const   float standard_data_p811[][5],standard_data_p801[][5];
extern const   float DDLV_standard_data[][4]; //�絼���û�У׼��׼��Һ

extern const   float temp_comp_coef[2];
///
extern uint32  temp_unit,print_busy,Uart_Printer_Flag;
//
extern uint32  version_811,model_display,LOGO_flag;


extern void    do_factory_call(void);

#endif