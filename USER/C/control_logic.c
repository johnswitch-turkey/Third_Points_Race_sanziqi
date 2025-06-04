#include "Control_logic.h"
#include "Chess_Task.h"
#include "stepper_motor.h"
#include "control.h"
#include "usart.h"

#include "system.h"

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
    // if(htim == &htim3)//1ms�ж�
    // {
    //     if(delayTimer.isRunning && delayTimer.counter > 0)
    //     {
    //         delayTimer.counter --;
    //     }			
	// 			HAL_TIM_Base_Start_IT(&htim3);
    // }
    if(htim == &htim2)//10ms中断
    {
            Robot_Process();
			
			HAL_TIM_Base_Start_IT(&htim2);
    }


}






uint8_t i1 = 0;
uint8_t i2 = 0;
uint8_t i4 = 0;

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{   
    if(htim->Instance==TIM2 && htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
    {   
        i1++;
        if(i1 % 2 == 0)
        {
            i1 = 0;
            g_stepperx.pulse_count--;
            g_steppery.pulse_count--;
            // 同时更新两个电机的位置计数
            if(g_stepperx.dir == CW)
                g_stepperx.add_pulse_count++;
            else
                g_stepperx.add_pulse_count--;
            
            if(g_steppery.dir == CW)
                g_steppery.add_pulse_count++;
            else
                g_steppery.add_pulse_count--;

            // 检查是否完成指定脉冲数
            if(g_stepperx.pulse_count <= 0 && g_steppery.pulse_count <= 0)
            {
                stepper_stop();  // 同时停止两个电机
            }
        }
        
        // 只需要更新一个通道的比较值，因为是共用PWM
        g_count_val1 = __HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_1);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (g_count_val1 + 150)%0XFFFF);
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




