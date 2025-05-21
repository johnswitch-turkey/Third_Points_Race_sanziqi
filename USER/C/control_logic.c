#include "Control_logic.h"
#include "Chess_Task.h"
#include "stepper_motor.h"
#include "control.h"
#include "uart.h"

#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim2;


uint16_t TimeCount = 0;
uint8_t TimeUseFlag = 0;


/* �߼���ʱ��PWM */
uint32_t g_count_val1 = 0;                   /* ����ֵ */
uint32_t g_count_val2 = 0;                   /* ����ֵ */
uint8_t g_run_flag1 = 0;
uint8_t g_run_flag2 = 0;

DelayTimer delayTimer = {0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim3)//1ms�ж�
    {
        if(delayTimer.isRunning && delayTimer.counter > 0)
        {
            delayTimer.counter --;
        }
    }
    if(htim == &htim2)//10ms�ж�
    {
        Place_Chess(piece_id,board_id);
        // Chess_Task();//��������������
    }


}






uint8_t i1 = 0;
uint8_t i2 = 0;
uint8_t i4 = 0;

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    
    if(htim->Instance==TIM1)
    {
        
        if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
        {
           
            i1++;
            if(i1 % 2 == 0)
            {
                
                i1 = 0;
                g_run_flag1 = 1;                             /* ��־λ��һ */
                g_stepperx.pulse_count--;                    /* ÿһ�������������-- */
                if(g_stepperx.dir == CW)
                {
                   g_stepperx.add_pulse_count++;             /* ����λ��++ */
                }else
                {
                   g_stepperx.add_pulse_count--;             /* ����λ��-- */
                }

                if(g_stepperx.pulse_count<=0)                /* ������������0��ʱ�� ������Ҫ���͵������������ɣ�ֹͣ��ʱ����� */
                {
                    stepper_stop();          /* ֹͣ�ӿ���� */
                    g_run_flag1=0;
                }
            }
            /*��ȡ��ǰ����*/
            g_count_val1 =__HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_1);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (g_count_val1 + 150)%0XFFFF);
        }
        if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_4)
        {
            
            i4++;
            if(i4 % 2 == 0)
            {
                i4 = 0;
                g_run_flag2 = 1;                             /* ��־λ��һ */
                g_steppery.pulse_count--;                    /* ÿһ�������������-- */
                if(g_steppery.dir == CW)
                {
                   g_steppery.add_pulse_count++;             /* ����λ��++ */
                }else
                {
                   g_steppery.add_pulse_count--;             /* ����λ��-- */
                }

                if(g_steppery.pulse_count<=0)                /* ������������0��ʱ�� ������Ҫ���͵������������ɣ�ֹͣ��ʱ����� */
                {
                    stepper_stop();          /* ֹͣ�ӿ���� */
                    g_run_flag2=0;
                }
            }
            /*��ȡ��ǰ����*/
            g_count_val2 = __HAL_TIM_GET_COUNTER(&htim2);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (g_count_val2 + 150)%0XFFFF);
            
        }

    }

}











/*
 *@brief  ��ʱ��ʱ����ʼ��
*/
void Delay_Timer(uint32_t millseconds)
{
    delayTimer.counter = millseconds;
    delayTimer.isRunning = 1;
}

/*
 *@@brief  ��whileѭ�����ж���ʱ�Ƿ����
*/
uint8_t checkDelayTimer(void)
{
    if(delayTimer.counter == 0)
    {
        return 1;
    }
    else return 0;
}




