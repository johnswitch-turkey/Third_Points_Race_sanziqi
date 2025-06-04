#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "main.h"

typedef struct {
   uint32_t x;      
   uint32_t y;      
} Position;

// 定义放置棋子的状态枚举
typedef enum {
    PLACE_IDLE,              // 空闲状态
    PLACE_MOVE_TO_CHESS,     // 移动到棋子位置
    PLACE_WAIT_ARRIVE1,      // 等待到达
    PLACE_SERVO_DOWN1,       // 舵机下降
    PLACE_WAIT_SERVO1,       // 等待舵机
    PLACE_MAGNET_ON,         // 电磁铁开启
    PLACE_WAIT_MAGNET,       // 等待电磁铁
    PLACE_SERVO_UP1,         // 舵机上升
    PLACE_WAIT_SERVO2,       // 等待舵机
    PLACE_MOVE_TO_BOARD,     // 移动到棋盘位置
    PLACE_WAIT_ARRIVE2,      // 等待到达
    PLACE_SERVO_DOWN2,       // 舵机下降放置
    PLACE_WAIT_SERVO3,       // 等待舵机
    PLACE_MAGNET_OFF,        // 电磁铁关闭
    PLACE_SERVO_UP2,         // 舵机上升
    PLACE_MOVE_TO_ZERO,      // 回零
    PLACE_WAIT_ZERO,         // 等待回零
    PLACE_COMPLETE           // 完成状态
} PlaceState;

// 放置棋子的控制结构体
typedef struct {
    PlaceState state;        // 当前状态
    uint8_t chess_id;        // 棋子ID
    uint8_t board_id;        // 棋盘位置ID
    uint32_t delay_start;    // 延时开始时间
    uint8_t is_busy;         // 忙状态标志
    Position start_pos;      // 起始位置
    Position end_pos;        // 目标位置
} PlaceControl;

extern PlaceControl place_ctrl;


// 电机控制状态枚举
typedef enum {
    MOTOR_IDLE,         // 空闲状态
    MOTOR_SET_X1,       // 设置电机1
    MOTOR_WAIT_X1,      // 等待电机1完成
    MOTOR_SET_X2,       // 设置电机2
    MOTOR_WAIT_X2,      // 等待电机2完成
    MOTOR_COMPLETE      // 完成状态
} MotorState;

// 控制结构体
typedef struct {
    MotorState state;           // 当前状态
    uint32_t x_target;         // X轴目标位置
    uint32_t y_target;         // Y轴目标位置
    uint32_t delay_start;      // 延时开始时间
    uint8_t is_busy;           // 忙状态标志
    uint8_t is_complete;       // 完成标志
} MotorControl;

extern MotorControl motor_ctrl;

void Place_Chess();

void control_t(uint32_t x_talget,uint32_t y_target);

void control_to_zero(void);


void Place_Chess_Start(uint8_t Chess_ID, uint8_t Board_ID);

void Place_Chess_Process(void);

uint8_t Place_Chess_IsBusy(void);


void control_t_start(uint32_t x_distance, uint32_t y_distance);

void control_t_process(uint32_t x_distance, uint32_t y_distance);


// 检查控制是否完成
uint8_t is_control_complete(void);

uint8_t control_t_is_busy(void);


#endif
