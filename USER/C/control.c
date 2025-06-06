#include "control.h"
#include "Control_logic.h"
#include "chess_task.h"
#include "stepper_motor.h"
#include "servo.h"
#include "usart.h"
#include "decode.h"
#include "system.h"
#include <stdlib.h>

#include "bsp_uart.h"
#include "Emm_V5.h"
#include "bsp_dwt.h"

// 电机控制相关
uint16_t velocity = 300; // 速度
uint16_t acc = 0; // 加速度
uint8_t raf = 1; // 绝对运动


uint8_t st1_dir;
uint8_t st2_dir;

static int32_t x;
static int32_t y;

int32_t current_x;
int32_t target_x;



//delay控制变量
uint32_t current_time;



//��������
Position board_position[9]=
{

    {4500, 9500}, {7200, 7200}, {9500, 5000},  //1 , 2 , 3
    // ��1��
    {7000, 12000}, {9500, 9500}, {142000, 7000},  //4 , 5 , 6
    // ��2��
    {9300, 14300}, {11800, 9300}, {14100, 9600}   //7 , 8 , 9
};

Position board_rotate_position[9]=
{

                     {8000, 8000},//1 
            {8000, 11500},       {11500, 8000}, //2 3
   {8000, 15000},   {11500, 11500},       {15000, 8000},  // 4 , 5 , 6
            {15000,10000},       {10000,15000},//7 8
                     {15000, 15000}   //9
};

//��������
Position chess_position[10]=
{
   {0, 13500},   
   {2000, 15500},   
   {4000, 17500},   
   {6000, 19500},   
   {8000, 21500},   
   {13500, 0},   
   {15500, 2000},  
   {17500, 4000},  
   {19500, 6000},  
   {21500, 8000}   
};




uint8_t Board_ID;
uint8_t Chess_ID;
uint8_t chessIndex;
uint8_t boardIndex;
Position start;
Position end;


uint8_t rotate_flag = 0; // 旋转标志
uint8_t cheat_flag = 0; // 作弊标志


PlaceControl place_ctrl = {
    .state = PLACE_IDLE,
    .is_busy = 0
};

// 启动放置棋子任务 // 放在UART函数回调中
void Place_Chess_Start(uint8_t Chess_ID, uint8_t Board_ID)
{
    if (Chess_ID < 0 || Chess_ID > 10 || Board_ID < 0 || Board_ID > 10) {
        return;
    }
    
    chessIndex = Chess_ID;
    boardIndex = Board_ID;
    
    place_ctrl.chess_id = Chess_ID;
    place_ctrl.board_id = Board_ID;
    place_ctrl.start_pos = chess_position[chessIndex];
    place_ctrl.end_pos = rotate_flag ? board_rotate_position[boardIndex] : board_position[boardIndex];

    if (cheat_flag  == 1) {
        place_ctrl.start_pos = board_position[chessIndex]; // 作弊时放置在原点
        place_ctrl.end_pos = board_position[boardIndex];
    }
    
    place_ctrl.state = PLACE_MOVE_TO_CHESS;
    place_ctrl.is_busy = 1;
    
    robot.system_state = SYSTEM_PLACING;
    
    // LED_Off();
}

// 在定时器中断中判断调用
void Place_Chess_Process(void)
{
    current_time = HAL_GetTick();
    
    if(!place_ctrl.is_busy) {
        return;
    }
    
    switch(place_ctrl.state) {
        case PLACE_MOVE_TO_CHESS:
            control_t_start(place_ctrl.start_pos.x, place_ctrl.start_pos.y);
            place_ctrl.delay_start = current_time;
            place_ctrl.state = PLACE_WAIT_ARRIVE1;
            break;
            
        case PLACE_WAIT_ARRIVE1:
            if(!control_t_is_busy()) {
                place_ctrl.delay_start = current_time;
                place_ctrl.state = PLACE_SERVO_DOWN1;
            }
            break;
            
        case PLACE_SERVO_DOWN1:
            Servo_Down();
            place_ctrl.delay_start = current_time;
            place_ctrl.state = PLACE_WAIT_SERVO1;
            break;
            
        case PLACE_WAIT_SERVO1:
            if(current_time - place_ctrl.delay_start >= 700) {
                place_ctrl.state = PLACE_MAGNET_ON;
            }
            break;
            
        case PLACE_MAGNET_ON:
            Magnet_On();
            place_ctrl.delay_start = current_time;
            place_ctrl.state = PLACE_WAIT_MAGNET;
            break;
            
        case PLACE_WAIT_MAGNET:
            if(current_time - place_ctrl.delay_start >= 700) {
                place_ctrl.state = PLACE_SERVO_UP1;
            }
            break;

        case PLACE_SERVO_UP1:
            Servo_Up();
            place_ctrl.delay_start = current_time;
            place_ctrl.state = PLACE_WAIT_SERVO2;
            break;
        
        case PLACE_WAIT_SERVO2:
            if(current_time - place_ctrl.delay_start >= 700) {
//                control_t_start(place_ctrl.end_pos.x, place_ctrl.end_pos.y);
                place_ctrl.state = PLACE_MOVE_TO_BOARD;
            }
            break;
        
        case PLACE_MOVE_TO_BOARD:
            control_t_start(place_ctrl.end_pos.x, place_ctrl.end_pos.y);
            place_ctrl.delay_start = current_time;
            place_ctrl.state = PLACE_WAIT_ARRIVE2;
            break;

        case PLACE_WAIT_ARRIVE2:
            if(!control_t_is_busy()) {
                place_ctrl.delay_start = current_time;
                place_ctrl.state = PLACE_SERVO_DOWN2;
            }
            break;
        
        case PLACE_SERVO_DOWN2:
            Servo_Down();
            place_ctrl.delay_start = current_time;
            place_ctrl.state = PLACE_WAIT_SERVO3;
            break;

        case PLACE_WAIT_SERVO3:
            if(current_time - place_ctrl.delay_start >= 700) {
                place_ctrl.state = PLACE_MAGNET_OFF;
            }
            break;
        
        case PLACE_MAGNET_OFF: 
            Magnet_Off();
            place_ctrl.delay_start = current_time;
            place_ctrl.state = PLACE_WAIT_MAGNET2;
            break;

        case PLACE_WAIT_MAGNET2:
            if(current_time - place_ctrl.delay_start >= 700) {
                place_ctrl.state = PLACE_SERVO_UP2;
            }
            break;

        case PLACE_SERVO_UP2:
            Servo_Up();
            place_ctrl.delay_start = current_time;
            place_ctrl.state = PLACE_WAIT_SERVO4;
            break;

        case PLACE_WAIT_SERVO4:
            if(current_time - place_ctrl.delay_start >= 700) {
                place_ctrl.state = PLACE_MOVE_TO_ZERO;
            }
            break;

        case PLACE_MOVE_TO_ZERO:
            control_t_start(0, 0);
            place_ctrl.delay_start = current_time;
            place_ctrl.state = PLACE_WAIT_ZERO;
            break;

        case PLACE_WAIT_ZERO:   
            if(!control_t_is_busy()) {
                place_ctrl.state = PLACE_COMPLETE;
            }
            break;

        case PLACE_IDLE:
        case PLACE_COMPLETE:
            // LED_Task_On();
            place_ctrl.state = PLACE_IDLE;
            place_ctrl.is_busy = 0;
            break;
    }
}

// 检查放置任务是否在执行
uint8_t Place_Chess_IsBusy(void)
{
    return place_ctrl.is_busy;
}



// 定义控制结构体实例
MotorControl motor_ctrl = {
    .state = MOTOR_IDLE,
    .is_complete = 1
};


// 启动电机移动任务 
// 在下棋progress中间被调用
// 更改系统状态为SYSTEM_MOVING
// 只有在放置棋子过程中才能启动电机移动
void control_t_start(uint32_t x_distance, uint32_t y_distance)
{
    if(robot.system_state != SYSTEM_PLACING) {
        // 只有在放置棋子过程中才能启动电机移动
        return;
    }
    
    motor_ctrl.x_target = x_distance;
    motor_ctrl.y_target = y_distance;
    robot.system_state = SYSTEM_MOVING;
		motor_ctrl.is_busy = 1;
		motor_ctrl.state = MOTOR_IDLE;
}

// 非阻塞的控制函数
void control_t_process()
{
    static uint32_t delay_count = 0;
    current_time = HAL_GetTick();
    
    if(motor_ctrl.state == MOTOR_IDLE) {
        motor_ctrl.state = MOTOR_SET_X1;
        motor_ctrl.is_complete = 0;
    }
    
    switch(motor_ctrl.state) {
        case MOTOR_SET_X1:
            Emm_V5_Pos_Control(1, 0, velocity, acc, motor_ctrl.x_target, raf, 0);
            motor_ctrl.delay_start = current_time;
            motor_ctrl.state = MOTOR_WAIT_X1;
            break;
            
        case MOTOR_WAIT_X1:
            if(current_time - motor_ctrl.delay_start >= 1500) {
                motor_ctrl.state = MOTOR_SET_X2;
            }
            break;
            
        case MOTOR_SET_X2:
            Emm_V5_Pos_Control(2, 1, velocity, acc, motor_ctrl.y_target, raf, 0);
            motor_ctrl.delay_start = current_time;
            motor_ctrl.state = MOTOR_WAIT_X2;
            break;
            
        case MOTOR_WAIT_X2:
            if(current_time - motor_ctrl.delay_start >= 1500) {
                motor_ctrl.state = MOTOR_COMPLETE;
                motor_ctrl.is_complete = 1;
            }
            break;
        case MOTOR_COMPLETE:
        case MOTOR_IDLE:
            // 完成状态，等待下一个任务
            motor_ctrl.is_busy = 0;
						robot.system_state = SYSTEM_PLACING;
            break;
        default:
            break;
    }
}

// 检查控制是否完成
uint8_t is_control_complete(void)
{
    return motor_ctrl.is_complete;
}

uint8_t control_t_is_busy(void)
{
    return motor_ctrl.is_busy;
}

//void control_to_zero(void)
//{

//   control_t_process(0, 0);

//}






// void Place_Chess(void)
// {	
// 	Command_t get_cmd[2] = {0};
// //   if (dequeue_command(get_cmd) == 0) {
// //      return; 
// //   }
// //   uint8_t Chess_ID = get_cmd->src;
// //   uint8_t Board_ID = get_cmd->dst;
// 	Chess_ID = 1;
// 	Board_ID = 1;
//    {

//     if (Chess_ID == 0 || Chess_ID > 10 || Board_ID == 0 || Board_ID > 10) {
//       return;
//       }

//       chessIndex = Chess_ID ;
//       boardIndex = Board_ID ;


// 	  start = chess_position[chessIndex];

//       end = board_position[boardIndex];
//       if(rotate_flag == 1) 
//       {
//          end.x = board_rotate_position[boardIndex].x;
//          end.y = board_rotate_position[boardIndex].y;
//       }


//       control_t(start.x, start.y); 
//       Delay_Timer(180);
//       while(!checkDelayTimer());
//       Servo_Down();
//       Magnet_On();
//       Servo_Up();
//       control_t(end.x,end.y);
//       Servo_Down();
//       Magnet_Off();
//       Servo_Up();

//       control_to_zero();

//    }

// }






// // 启动放置棋子任务
// void Place_Chess_Start(uint8_t Chess_ID, uint8_t Board_ID)
// {
//     if(robot.system_state != SYSTEM_IDLE) {
//         return; // 系统忙，拒绝新任务
//     }
    
//     // 初始化放置任务
//     place_ctrl.chess_id = Chess_ID;
//     place_ctrl.board_id = Board_ID;
//     // ... 其他初始化代码 ...
    
//     robot.system_state = SYSTEM_PLACING;
// }


