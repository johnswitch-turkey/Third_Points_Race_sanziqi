#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H


#include "stm32f4xx_hal.h"



/* 步进电机基本参数定义 */
#define PULSE_REV       3200.0     /* 每圈脉冲数(8细分,200*16=3200) */
#define MAX_STEP_ANGLE  0.1125     /* 最小步距角(1.8/16=0.1125度) */
#define STEP_R					13         /* 旋转半径 */


/* 步进电机控制结构体 */
typedef struct
{
    int angle ;                             // 角度
    uint8_t dir;                            // 方向
    uint8_t en;                             // 使能状态
    volatile uint32_t pulse_count;          // 相对位置
    volatile int add_pulse_count;           // 绝对位置  
} STEPPER_MOTOR;

/* 电机对象声明 */
extern STEPPER_MOTOR g_stepperx;   // 电机1对象
extern STEPPER_MOTOR g_steppery;   // 电机2对象


/* 旋转方向枚举定义 */
enum dir
{
    CCW = 0,                       /* 逆时针方向旋转 */
    CW,                           /* 顺时针方向旋转 */
}; 




/* 步进电机接口编号定义 */
#define STEPPER_MOTOR_1    1      /* 电机1接口编号 */
#define STEPPER_MOTOR_2    2      /* 电机2接口编号 */


/* 步进电机方向控制引脚定义 */
// 电机1方向控制引脚
#define STEPPER_DIR1_GPIO_PIN     GPIO_PIN_1
#define STEPPER_DIR1_GPIO_PORT    GPIOA

// 电机2方向控制引脚
#define STEPPER_DIR2_GPIO_PIN     GPIO_PIN_2
#define STEPPER_DIR2_GPIO_PORT    GPIOA

/* 步进电机使能控制引脚定义 */
// 电机1使能控制引脚  
#define STEPPER_EN1_GPIO_PIN      GPIO_PIN_1
#define STEPPER_EN1_GPIO_PORT     GPIOB

// 电机2使能控制引脚
#define STEPPER_EN2_GPIO_PIN      GPIO_PIN_2
#define STEPPER_EN2_GPIO_PORT     GPIOB



#define ST1_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR1_GPIO_PORT, STEPPER_DIR1_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR1_GPIO_PORT, STEPPER_DIR1_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  

#define ST2_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR2_GPIO_PORT, STEPPER_DIR2_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR2_GPIO_PORT, STEPPER_DIR2_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  























#endif
