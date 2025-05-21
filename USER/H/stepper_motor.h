#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H


#include "stm32f4xx_hal.h"



/* ������������������� */
#define PULSE_REV       3200.0     /* ÿȦ������(8ϸ��,200*16=3200) */
#define MAX_STEP_ANGLE  0.1125     /* ��С�����(1.8/16=0.1125��) */
#define STEP_R					13         /* ��ת�뾶 */


/* ����������ƽṹ�� */
typedef struct
{
    int angle ;                             // �Ƕ�
    uint8_t dir;                            // ����
    volatile uint32_t pulse_count;          // ���λ��
    volatile uint16_t add_pulse_count;           // ����λ��  
} STEPPER_MOTOR;

/* ����������� */
extern STEPPER_MOTOR g_stepperx;   // ���1����
extern STEPPER_MOTOR g_steppery;   // ���2����


/* ��ת����ö�ٶ��� */
enum dir
{
    CCW = 0,                       /* ��ʱ�뷽����ת */
    CW ,                           /* ˳ʱ�뷽����ת */
}; 




/* ��������ӿڱ�Ŷ��� */
#define STEPPER_MOTOR_1    1      /* ���1�ӿڱ�� */
#define STEPPER_MOTOR_2    2      /* ���2�ӿڱ�� */


/* �����������������Ŷ��� */
// ���1�����������
#define STEPPER_DIR1_GPIO_PIN     GPIO_PIN_1
#define STEPPER_DIR1_GPIO_PORT    GPIOA

// ���2�����������
#define STEPPER_DIR2_GPIO_PIN     GPIO_PIN_2
#define STEPPER_DIR2_GPIO_PORT    GPIOA

/* �������ʹ�ܿ������Ŷ��� */
// ���1ʹ�ܿ�������  
#define STEPPER_EN1_GPIO_PIN      GPIO_PIN_1
#define STEPPER_EN1_GPIO_PORT     GPIOB

// ���2ʹ�ܿ�������
#define STEPPER_EN2_GPIO_PIN      GPIO_PIN_2
#define STEPPER_EN2_GPIO_PORT     GPIOB



#define ST1_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR1_GPIO_PORT, STEPPER_DIR1_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR1_GPIO_PORT, STEPPER_DIR1_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0);  

#define ST2_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR2_GPIO_PORT, STEPPER_DIR2_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR2_GPIO_PORT, STEPPER_DIR2_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  























#endif
