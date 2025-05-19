#include "Control_logic.h"
#include "Chess_Task.h"
#include "stepper_motor.h"

extern TIM_HandleTypeDef htim2;


uint16_t TimeCount = 0;
uint8_t TimeUseFlag = 0;


/* 高级定时器PWM */
uint32_t g_count_val1 = 0;                   /* 计数值 */
uint32_t g_count_val2 = 0;                   /* 计数值 */
uint8_t g_run_flag1 = 0;
uint8_t g_run_flag2 = 0;

DelayTimer delayTimer = {0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim3)//1ms中断
    {
        if(delayTimer.isRunning && delayTimer.counter > 0)
        {
            delayTimer.counter --;
        }
    }
    if(htim == &htim2)//10ms中断
    {
        Chess_Task();//任务跑在这里面
    }
//    if(htim == &htim4)
//    {
//        HAL_GPIO_TogglePin(GPIOD, LED_1_Pin);
//   }

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
                g_run_flag1 = 1;                             /* 标志位置一 */
                g_stepperx.pulse_count--;                    /* 每一个完整的脉冲就-- */
                if(g_stepperx.dir == CW)
                {
                   g_stepperx.add_pulse_count++;             /* 绝对位置++ */
                }else
                {
                   g_stepperx.add_pulse_count--;             /* 绝对位置-- */
                }

                if(g_stepperx.pulse_count<=0)                /* 当脉冲数等于0的时候 代表需要发送的脉冲个数已完成，停止定时器输出 */
                {
                    stepper_stop();          /* 停止接口输出 */
                    g_run_flag1=0;
                }
            }
            /*获取当前计数*/
            g_count_val1 =__HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_1);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (g_count_val1 + 150)%0XFFFF);
        }
        if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_4)
        {
            
            i4++;
            if(i4 % 2 == 0)
            {
                i4 = 0;
                g_run_flag2 = 1;                             /* 标志位置一 */
                g_steppery.pulse_count--;                    /* 每一个完整的脉冲就-- */
                if(g_steppery.dir == CW)
                {
                   g_steppery.add_pulse_count++;             /* 绝对位置++ */
                }else
                {
                   g_steppery.add_pulse_count--;             /* 绝对位置-- */
                }

                if(g_steppery.pulse_count<=0)                /* 当脉冲数等于0的时候 代表需要发送的脉冲个数已完成，停止定时器输出 */
                {
                    stepper_stop();          /* 停止接口输出 */
                    g_run_flag2=0;
                }
            }
            /*获取当前计数*/
            g_count_val2 = __HAL_TIM_GET_COUNTER(&htim2);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (g_count_val2 + 150)%0XFFFF);
            
        }















/*
 *@brief  延时定时器初始化
*/
void Delay_Timer(uint32_t millseconds)
{
    delayTimer.counter = millseconds;
    delayTimer.isRunning = 1;
}

/*
 *@@brief  在while循环中判断延时是否结束
*/
uint8_t checkDelayTimer(void)
{
    if(delayTimer.counter == 0)
    {
        return 1;
    }
    else return 0;
}




