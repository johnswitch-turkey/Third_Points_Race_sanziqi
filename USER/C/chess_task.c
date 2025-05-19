

#include "chess_task.h"

// 全局状态变量，用于记录棋子放置状态
volatile uint8_t black_placed = 0;    // 黑棋放置标志
volatile uint8_t white_placed = 0;    // 白棋放置标志
volatile uint8_t total_placed = 0;    // 总放置数量
volatile uint8_t isTaskRunning;       // 任务执行状态：0-空闲，1-运行中


//交互用的任务状态结构体
volatile Task_State task_state = {
    .last_chess_ID = 0xFF,      // 上一次操作的棋子ID（初始值0xFF表示无效）
    .last_board_ID = 0xFF,      // 上一次操作的棋盘位置ID（初始值0xFF表示无效）
    .total_chess_count = 0,     // 当前任务中已放置的棋子总数
    .data_refreshed = 0         // 数据更新标志，用于标记数据是否刷新
};


// 特殊任务标志位
volatile uint8_t rotate_flag = 0;  // 是否需要45度旋转标志，用于第三个任务


