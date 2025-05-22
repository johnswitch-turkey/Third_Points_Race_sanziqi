




#include "stepper_motor.h"

extern TIM_HandleTypeDef htim2;

STEPPER_MOTOR g_stepperx = {0};
STEPPER_MOTOR g_steppery = {0};

/**
 * @brief       �����������
 * @param       motor_num: ��������ӿ����
 * @param       dir      : ���������ת����
 * @retval      ��
 */
void stepper_start(uint8_t dir1, uint8_t dir2)
{
//    switch(motor_num) 
//    {
//        case STEPPER_MOTOR_1 :
//        {
            ST1_DIR(dir1);
//            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_TOGGLE)
//            {
                HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1); 
//            }
//            break;
//        }
//        case STEPPER_MOTOR_2 :
//        {
            ST2_DIR(dir2);
//            if(g_atimx_oc_chy_handle.OCMode == TIM_OCMODE_TOGGLE)
//            {
                HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2); 
//            }
//            break;
////        }

//            break;
        
//        default : break;
//    }
	}

/**
 * @brief       �رղ������
 * @param       motor_num: ��������ӿ����
 * @retval      ��
 */
void stepper_stop(void)
{
//    switch(motor_num)
//    {
//        case STEPPER_MOTOR_1 :
//        {
            HAL_TIM_OC_Stop_IT(&htim2, TIM_CHANNEL_1);     /* �رն�ӦPWMͨ�� */
//            break;
//        }
//        case STEPPER_MOTOR_2 :
//        {
            HAL_TIM_OC_Stop_IT(&htim2, TIM_CHANNEL_2);     /* �رն�ӦPWMͨ�� */
//            break;
//        }

//        default : break;
//    }
}










//void stepperx_set_pulse(uint16_t distance, uint8_t dir1, uint8_t dir2)
//{
//	  // g_stepperx.pulse_count = distance / 2 / PI / STEP_R *200*16;
//    g_stepperx.pulse_count = distance /  STEP_R * 509;
//	
//    if(g_stepperx.pulse_count == 0)
//    {
//        stepper_stop();
//    }
//    else stepper_start(dir1,dir2);
//}

//void steppery_set_pulse(uint16_t distance, uint8_t dir1, uint8_t dir2)
//{
//    g_steppery.pulse_count = distance /  STEP_R * 509;
//	
//    if(g_steppery.pulse_count == 0) 
//    {
//        stepper_stop();
//    }
//    else stepper_start(dir1,dir2);
//}

