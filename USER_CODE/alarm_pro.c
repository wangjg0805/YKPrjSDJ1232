//**********************************************************************
//�ļ���:alarm_pro.c
//�����б�:
//01.��ָ���ı�����ӵ���������:Log_Alm
//02.ɾ���������ϵ�ĳ������:Cancel_Alm
//03.�ѵ�ǰ�����Ƶ������������:Move_Alm_Rec_To_End
//04.�ڱ���������ʾ������Ϣ:Put_Alm_Mess
//05.��ѯ��ǰ�Ƿ��б���: Check_Alm_Status
//06.����ʾ��Ϣ����������:Log_Tips
//07.ɾ����ʾ��Ϣ:Del_Tips
//08.��ʾ��ʾ��Ϣ:Put_Tips_Mess
//09.��鵱ǰ�Ƿ���ʾ��ʾ��Ϣ:Check_Tips_Disp
//10.��ѯ��ǰ�Ƿ���ʾָ������ʾ��Ϣ:Check_Special_Tips_Disp
//**********************************************************************
#include "config.h"
#include "alarm_pro.h"
#include "lcd2478.h"

#include "speaker.h"
#include "timer.h"
#include "Font_Lib.h"

uint8 Check_Tips_Disp(void);
//������¼Ŀ¼

Alm_Struct blank_alm     = {NONE_ALM,NULL,NULL,FALSE,NULL};    //�ձ�����Ϣ
//��ʾ�౨�� ������ʧ��ɾ��
Alm_Struct warn_ph_out   = {ALARM_PH_OUT_WARN,       "PH ���ݳ���������Χ",       "PH DATA OVER",FALSE,NULL};
Alm_Struct warn_mv_out   = {ALARM_MV_OUT_WARN,       "�������ݳ���������Χ",      "MV DATA OVER",FALSE,NULL};
Alm_Struct warn_temp_out = {ALARM_TEMP_OUT_WARN,     "�¶Ȳ���ƫ��̫��",      "TEMP OFFSET OVER",FALSE,NULL};
Alm_Struct warn_xx_out   = {ALARM_XX_OUT_WARN,       "�Ϻ��ӿ�",                  "SHANGHAIYOKE",FALSE,NULL};


//��ʾ����Ϣ����ʾһ��ʱ�����Զ�ɾ��
Alm_Struct tips_save_fault= {TIPS_NOT_STABLE_WARN,      "������Ч����ʧ��",                "SAVE FAILED",FALSE,NULL};
Alm_Struct tips_EE_bad    = {TIPS_EE_BAD_WARN,          "�洢�����ϱ���ʧ��",   "NOT SAVE OF BAD EEPROM",FALSE,NULL};
Alm_Struct tips_save_ok   = {TIPS_SAVE_OK_WARN,         "���ݱ���ɹ�",                        "SAVE OK",FALSE,NULL};
Alm_Struct tips_clear_ok  = {TIPS_CLEAR_OK_WARN,        "������ȫ�����",                       "DEL OK",FALSE,NULL};
Alm_Struct tips_first_page= {TIPS_FIRST_PAGE_WARN,      "��ǰ�ǵ�һҳ",                     "FIRST PAGE",FALSE,NULL};
Alm_Struct tips_last_page = {TIPS_LAST_PAGE_WARN,       "��ǰ�����һҳ",                    "LAST PAGE",FALSE,NULL};
Alm_Struct tips_invalid_data = {TIPS_NO_VALID_WARN,     "����ֵ������Χ",                "INVALID INPUT",FALSE,NULL};
Alm_Struct tips_bad_envir   =  {TIPS_BAD_ENV_WARN,      "���ݲ��ȶ���Ϊ��",         "WEIGHTMENT INVALID",FALSE,NULL};


Alm_Struct* warning_alm = &blank_alm;         //��ǰ�ľ�ʾ�౨��
Alm_Struct*    tips_alm = &blank_alm;         //��ǰ����ʾ����Ϣ
///////////////////������ʾ��Ϣ
#define TIPS_DISP_TIME            150         //��ʾ��Ϣ��ʾ��ʱ��
#define TIPS_DISP_SHORT_TIME      50
//����ģʽ�µ���ʾ��Ϣ
/*
Tips_Struct*        cur_tips_ptr;    //��ǰ��ʾ��Ϣ��ָ��
static sint16     tips_disp_time;    //��ʾ��Ϣ��ʾ��ʱ��
static uint8          flag_saved;    //�Ѿ����汻��������ı�־
static uint8   tips_disp_trigger;    //������ʾ��Ϣ����ʾ
static uint32  save_buf[LINE_SIZE*SCR_YSIZE/4];//����ʾ��Ϣ���ǵ���ʾ������ʱ����Ļ�����
*/                                                //���ߴ�Ϊ�ķ�֮һ����
//**********************************************************************
//1����  ��:Log_Alm
//��������:��ӱ�����¼�ڵ�(�ӵ���ǰ��)
//��ڲ���:alm--��ӵı�����¼
//            level--��������
//���ڲ���:��
//**********************************************************************
void Log_Alm(Alm_Struct* alm,uint8 level)
{
    if (NULL == alm) return;
    switch (level)
    {
        case ALM_LEV_WARN://��ʾ�౨��
            if (alm->alm_id < FIRST_WARN_ALARM)  return;    //�޸ü�¼
            if (alm->alm_id >  LAST_WARN_ALARM)  return;    //�޸ü�¼
            break;        
        case ALM_LEV_TIPS://��ʾ����Ϣ
            if (alm->alm_id < FIRST_TIPS_ALARM)  return;    //�޸ü�¼
            if (alm->alm_id >  LAST_TIPS_ALARM)  return;    //�޸ü�¼
            break;
       
       default:break;    
    }
    if (alm->flag_log == TRUE) return; //�Ѿ����ڱ�����¼���������¼�¼ 
    alm->flag_log = TRUE;
   
    switch (level)
    {
       case ALM_LEV_WARN://��ʾ�౨��
             alm->next_record =   warning_alm;    //ԭ�ȵļ�¼�ƺ�
             warning_alm = alm;    
             warning_alm_timer = 0;    //���¼�ʱ 
             break;        
       case ALM_LEV_TIPS:
             alm->next_record =     tips_alm;    //ԭ�ȵļ�¼�ƺ�
             tips_alm = alm;
             tips_alm_timer = 0;    //���¼�ʱ
             break;
       default:break;
    }
}

//**********************************************************************
//2����  ��:Del_Alm
//��������:ɾ��������¼�ڵ�,����ñ������ڱ������ϣ���ֱ�ӷ���
//��ڲ���:alm--��ӵı�����¼
//            level--��������
//���ڲ���:��
//**********************************************************************
void Del_Alm(Alm_Struct* alm,uint8 level)
{
    Alm_Struct* pre;
  
    if (alm->flag_log == FALSE)          return; //�ñ������ڱ������ϣ�ֱ�ӷ���    
    if (alm->alm_id < FIRST_WARN_ALARM)  return;
    if (alm->alm_id > LAST_TIPS_ALARM)   return; //�޸ü�¼
  
    switch (level)
    {
        case ALM_LEV_TIPS:
            if (alm == tips_alm)//ͷ���
            {
                tips_alm = alm->next_record;
            }
            else
            {
                pre = tips_alm;
                while (pre->next_record != NULL)
                {
                    if (pre->next_record == alm)    //�ҵ�ǰ���ڵ�
                    {
                        break;
                    }
                    pre = pre->next_record;
                }
                pre->next_record = alm->next_record;
            }
            break;
        case ALM_LEV_WARN://�����ȼ�����
            if (alm == warning_alm)//ͷ���
            {
                warning_alm = alm->next_record;
            }
            else
            {
                pre = warning_alm;
                while (pre->next_record != NULL)
                {
                    if (pre->next_record == alm)    //�ҵ�ǰ���ڵ�
                    {
                        break;
                    }
                    pre = pre->next_record;
                }
                pre->next_record = alm->next_record;
            }        
            break;        
       
        default:return;
    }
    alm->next_record = NULL;
    alm->flag_log = FALSE;
}

////////////////////////////////////////////////////////////
//ɾ����ǰ��ʾ��Ϣ
///////////////////////////////////////////////////////////
void Del_Tip_Alm(void)
{
    Alm_Struct* tips_tmp;
  
    if(tips_alm == &blank_alm)  return;
   
    tips_tmp = tips_alm;
    
    tips_alm = tips_alm->next_record; //ָ����һ����¼
    
    tips_tmp->next_record = NULL;     //�ѵ�ǰ��¼��λ
    tips_tmp->flag_log    = FALSE;
    
}
//**********************************************************************
//3����  ��:Move_Alm_Rec_To_End
//��������: �ѵ�ǰ�ı�����¼�Ƶ����
//��ڲ���:    level--��������
//���ڲ���:��
//**********************************************************************
static void Move_Alm_Rec_To_End(uint8 level)
{
    Alm_Struct* end,*head;
    switch (level)
    {
        case ALM_LEV_TIPS:
            if (tips_alm->alm_id == NONE_ALM)    //�ޱ�����Ϣ
            {
                return;    
            }
            else if (tips_alm->next_record == &blank_alm)//�޺��������ڵ�
            {
                return;
            }
            else
            {
                head = end = tips_alm;
                while (end->next_record != &blank_alm)//�ҵ����ı����ڵ�
                {
                    end = end->next_record;
                }
                end->next_record = head;
                tips_alm = head->next_record;    //�ڶ��ڵ��Ϊͷ���
                head->next_record= &blank_alm;                
            }
            break;
        case ALM_LEV_WARN:
            if (warning_alm->alm_id == NONE_ALM)    //�ޱ�����Ϣ
            {
                return;    
            }    
            else if (warning_alm->next_record == &blank_alm)//�޺��������ڵ�
            {
                return;
            }
            else
            {
                head = end = warning_alm;
                while (end->next_record != &blank_alm)//�ҵ����ı����ڵ�
                {
                    end = end->next_record;
                }
                end->next_record = head;
                warning_alm = head->next_record;    //�ڶ��ڵ��Ϊͷ���
                head->next_record= &blank_alm;                
            }            
            break;        
        default:break;
    }            
}

//**********************************************************************
//4���� ��:Put_Alm_Mess
//��������:��ʾ������Ϣ
//��ڲ���:��
//���ڲ���:�� 
//**********************************************************************
void Put_Alm_Mess(void)
{
   
    static uint32 low_alm_display_cnt = 0 ;
    static uint32 manu_alm_display_cnt = 0 ;
    
    //Set_Graphic_Color(COL_THINBLUE,COL_THINBLUE);
    //Bar(100,0,480,69);  //����������Ϣ
    
    //Set_HZ_Lib(&ST_11);    //�����ֿ�
    //Set_ASC_Lib(Arial10B_Font);
    //////////////////////////////////////////////////////��ʾ��
    if((warning_alm_timer<4)&&(warning_alm->alm_id != NONE_ALM))//�б�����Ϣ
    {
         Set_Graphic_Color(COL_THINRED,COL_THINRED);
         Bar(250,5,250+300,35);
         Set_Graphic_Color(COL_WHITE,COL_WHITE);
         Rectangle(250,5,250+300,35);
       
         Set_Chr_Color(COL_WHITE,COL_THINRED);  //��װ���
         Set_ASC_Lib(Arial16B_Font);
         Put_Line_Str(270, 10,Language_Str((uint8*)warning_alm->ch_mess,(uint8*)warning_alm->en_mess));
    }
    else if(warning_alm->alm_id == NONE_ALM)
     {
      Set_Graphic_Color(COL_THINBLUE,COL_THINBLUE);
      Bar(250,5,250+300,35);  //����������Ϣ
     }
     
    if (warning_alm_timer >= 20)    //2s�����ȼ�����
    {    
        warning_alm_timer = 0;
        Move_Alm_Rec_To_End(ALM_LEV_WARN);    //��ʾʱ�䵽,�л�������Ϣ
    }        
    
    //////////////////////��ʾ����Ϣ��ʾ���·������Զ���ʧ
    if((tips_alm_timer<4)&&(tips_alm->alm_id != NONE_ALM))//�б�����Ϣ
    {
        Set_Graphic_Color(COL_THINYELLOW,COL_THINYELLOW);
        Bar(250,40,250+300,70);
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
        Rectangle(250,40,250+300,70);
     
        Set_Chr_Color(COL_BLACK,COL_THINYELLOW); //�Ƶ׺���
        Set_ASC_Lib(Arial16B_Font);
        Put_Line_Str(270,45,Language_Str((uint8*)tips_alm->ch_mess,(uint8*)tips_alm->en_mess));
    }
    else if(tips_alm->alm_id == NONE_ALM)
      {
       Set_Graphic_Color(COL_THINBLUE,COL_THINBLUE);
       Bar(250,40,250+300,70);  //����������Ϣ
      }
      
    if (tips_alm_timer >= 30)    //��ʾ��Ϣ��ʾʱ��
    {
        tips_alm_timer = 0;
        Del_Tip_Alm();   //�ѵ�ǰ��ʾ��Ϣɾ����
        //Move_Alm_Rec_To_End(ALM_LEV_TIPS);//�л�������Ϣ
    }    
}

//**********************************************************************
//5����  ��:Check_Alm_Status
//��������:��ѯ��ǰ�Ƿ��б���
//��ڲ���:��
//���ڲ���:TRUE--�б���
//            FALSE--�ޱ���
//**********************************************************************
uint8 Check_Alm_Status(void)
{
    return ((tips_alm->alm_id!=NONE_ALM)||
            (warning_alm->alm_id!=NONE_ALM));
}

/****************************************************************************
* ���ƣ�Set_Clr_Alarm_Sound()
* ���ܣ����û��������������Ĵ���(100msһ��)
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Set_Clr_Alarm_Sound(void)
{
    //���ȼ���1���� 2������  3����ʾ
    
    if(Get_Key_Sound_Type() != SOUND_TYPE_NONE) //������������
       {
        Set_Alm_Sound_Type(SOUND_TYPE_NONE);
        return;
       }
       
    ////����ʾ���뱨������������ʱ�򣬱�������������
    ////�����ȼ����ʾ����Ϣ
    
    if(tips_alm->alm_id !=NONE_ALM)
    {  //�б���
       if(Get_Alm_Sound_Type() != SOUND_TYPE_TIPS)
         { //��ǰ���Ǵ��౨����
            Set_Alm_Sound_Type(SOUND_TYPE_TIPS);           //���ñ�����������
         }
    }
    else
     Set_Alm_Sound_Type(SOUND_TYPE_NONE);
    //////////////////////////////////////////////// 
    ////////////��鱨������Ϣ 
    if(warning_alm->alm_id !=NONE_ALM)
    {  //�б���
       if(Get_Alm_Sound_Type() != SOUND_TYPE_WARNING)
         { //��ǰ���Ǵ��౨����
            Set_Alm_Sound_Type(SOUND_TYPE_WARNING);        //���ñ�����������
         }
    }
    else
     Set_Alm_Sound_Type(SOUND_TYPE_NONE);
    
}


