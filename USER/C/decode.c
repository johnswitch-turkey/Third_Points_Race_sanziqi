#include "decode.h"
#include "time.h"
#include "usart.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 命令队列结构体


// 命令队列
static Command_t cmd_queue[CMD_QUEUE_SIZE];
static volatile uint8_t cmd_head = 0;    // 队列头指针
static volatile uint8_t cmd_tail = 0;    // 队列尾指针
static volatile uint8_t cmd_count = 0;   // 队列中当前元素个数

// 测试统计变量
uint8_t parse_count = 0;
uint8_t parse_count_all = 0;
uint8_t parse_count_err = 0;
uint8_t parse_count_full = 0;

/**
 * 将命令放入队列
 * 
 * @param piece_id    棋子ID (0-4)
 * @param board_pos   棋盘位置 (0-8)
 * @param need_rotate 是否需要旋转 (0/1)
 * @param is_cheating 是否作弊 (0/1)
 * @return 1:入队成功 0:队列已满
 */
static uint8_t enqueue_command(uint8_t piece_id, uint8_t board_pos, 
                             uint8_t need_rotate, uint8_t is_cheating)
{
    if (cmd_count >= CMD_QUEUE_SIZE) {
        // 队列已满
        parse_count_full++;
        return 0;
    }
    
    cmd_queue[cmd_tail].piece_id = piece_id;
    cmd_queue[cmd_tail].board_pos = board_pos;
    cmd_queue[cmd_tail].need_rotate = need_rotate;
    cmd_queue[cmd_tail].is_cheating = is_cheating;
    
    cmd_tail = (cmd_tail + 1) % CMD_QUEUE_SIZE;
    
    __disable_irq();     // 进入临界区
    cmd_count++;
    __enable_irq();      // 退出临界区
    
    return 1;
}

/**
 * 从队列取出一条命令
 * 
 * @param out_cmd 输出参数，用于存储取出的命令
 * @return 1:成功 0:队列为空
 */
uint8_t dequeue_command(Command_t *out_cmd)
{
    if (cmd_count == 0) {
        // 队列空
        return 0;
    }
    
    *out_cmd = cmd_queue[cmd_head];
    cmd_head = (cmd_head + 1) % CMD_QUEUE_SIZE;
    
    __disable_irq();
    cmd_count--;
    __enable_irq();
    
    return 1;
}

/**
 * 解析从树莓派发来的单行命令
 * 格式: "deploy from 2 to 7，1，0"
 * 分别表示: 棋子ID, 棋盘位置, 是否需要旋转, 是否作弊
 */
void parse_command(char *cmd)
{
    int piece_id = -1;
    int board_pos = -1;
    int need_rotate = -1;
    int is_cheating = -1;
    
    parse_count_all++;

    // 解析命令格式
    if (sscanf(cmd, "deploy from %d to %d,%d,%d", 
              &piece_id, &board_pos, &need_rotate, &is_cheating) == 4) {
        
        // 验证参数范围
        if (piece_id < 0 || piece_id > 4 ||
            board_pos < 0 || board_pos > 8 ||
            need_rotate < 0 || need_rotate > 1 ||
            is_cheating < 0 || is_cheating > 1) {
            parse_count_err++;
            return;
        }
        
        if (enqueue_command((uint8_t)piece_id, (uint8_t)board_pos, 
                           (uint8_t)need_rotate, (uint8_t)is_cheating)) {
            parse_count++;
        }
    } 
    else {
        // 格式错误
        parse_count_err++;
        // 可以发送错误响应
        // char err_msg[] = "ERR\r\n";
        // HAL_UART_Transmit(&huart6, (uint8_t *)err_msg, strlen(err_msg), HAL_MAX_DELAY);
    }
}