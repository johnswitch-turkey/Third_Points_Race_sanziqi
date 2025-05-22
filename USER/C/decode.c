#include "decode.h"
#include "time.h"
#include "usart.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static volatile uint8_t cmd_head = 0 ;   // 下一个要从队列取出的索引
static volatile uint8_t cmd_tail = 0 ;   // 下一个要写入队列的位置
static volatile uint8_t cmd_count = 0 ;  // 队列中当前元素个数（0～CMD_QUEUE_SIZE）

/**
    尝试将一个命令（src, dst）放入队列
    src: 要“取子”的编号，0～4
    dst: 要“落子”的格子编号，0～8
    1：入队成功；0：队列已满，入队失败
  */
static uint8_t enqueue_command(uint8_t src, uint8_t dst)
{
    if (cmd_count >= CMD_QUEUE_SIZE) {
        // 队列已满，无法再入队
        return 0;
    }
    cmd_queue[cmd_tail].src = src;
    cmd_queue[cmd_tail].dst = dst;
    cmd_tail = (cmd_tail + 1) % CMD_QUEUE_SIZE;
    __disable_irq();     // 进入临界区，防止中断打乱 cmd_count
    cmd_count++;
    __enable_irq();      // 退出临界区
    return 1;
}



/**
  从队列取出一条命令
  out_cmd: 用于存放取出的命令
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
    解析从树莓派发来的单行命令（如 "from 2 to 7"）
	如果成功解析出 src、dst，就调用 enqueue_command() 放入队列
 
  */
void parse_command(char *cmd)
{
    int src = -1, dst = -1;

    // 使用 sscanf 提取 "from %d to %d" 格式
    if (sscanf(cmd, "from %d to %d", &src, &dst) == 2) {
        // 如果队列已满，就可以选择丢弃或者发错误提示
        if (!enqueue_command(src, dst)) {
            // 队列已满：这里示例发送一个 "FULL" 提示到 Raspberry Pi
            char full_msg[] = "FULL\r\n";
            //HAL_UART_Transmit(&huart6, (uint8_t *)full_msg, strlen(full_msg), HAL_MAX_DELAY);
        } 
				else{}
				/*
				else {
            // 入队成功，可以发送一个简单的 ACK
            char ack_msg[32];
            int len = snprintf(ack_msg, sizeof(ack_msg), "ACK %d->%d\r\n", src, dst);
            HAL_UART_Transmit(&huart1, (uint8_t *)ack_msg, len, HAL_MAX_DELAY);
        }
				*/
    } else {
        // 如果格式不对，发送 ERR
        char err_msg[] = "ERR\r\n";
        //HAL_UART_Transmit(&huart6, (uint8_t *)err_msg, strlen(err_msg), HAL_MAX_DELAY);
    }
}




