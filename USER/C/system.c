#include "system.h"
#include "control.h"

RobotControl robot = {
    .system_state = SYSTEM_IDLE
};

// 任务处理函数
void Robot_Process(void)
{
    switch(robot.system_state) {
        case SYSTEM_IDLE:
            // 空闲状态，可以接收新任务
            break;
            
        case SYSTEM_PLACING:
            // 处理放置棋子任务
            Place_Chess_Process();
            if(!place_ctrl.is_busy) {
                robot.system_state = SYSTEM_IDLE;
            }
            break;
            
        case SYSTEM_MOVING:
            // 处理电机移动任务
            control_t_process(motor_ctrl.x_target, motor_ctrl.y_target);
//            if(!motor_ctrl.is_busy) {
//                robot.system_state = SYSTEM_IDLE;
//            }
            break;
            
        case SYSTEM_ERROR:
            // 错误处理
            break;
    }
}


