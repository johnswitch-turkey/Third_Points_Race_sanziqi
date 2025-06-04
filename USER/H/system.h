#if !defined(__SYSTEM_H)
#define __SYSTEM_H

#include "stm32f4xx.h"
#include "control.h"

typedef enum {
    SYSTEM_IDLE,        // 系统空闲
    SYSTEM_PLACING,     // 正在放置棋子
    SYSTEM_MOVING,      // 正在移动
    SYSTEM_ERROR        // 错误状态
} SystemState;

typedef struct {
    SystemState system_state;    // 系统状态
    PlaceControl place_ctrl;     // 放置棋子控制
    MotorControl motor_ctrl;     // 电机控制
} RobotControl;

extern RobotControl robot;

void Robot_Process(void);

#endif /* __SYSTEM_H */
