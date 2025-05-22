#ifndef __DECODE_H__
#define __DECODE_H__

#include "stm32f4xx_hal.h"

/* —— 命令队列配置 —— */
#define CMD_QUEUE_SIZE  16    // 最多缓存 16 条命令，可按需调整

typedef struct {
    uint8_t src;
    uint8_t dst;
} Command_t;

/* 环形队列本体 */
static Command_t cmd_queue[CMD_QUEUE_SIZE];

/* 队列操作用的索引与计数 */
static volatile uint8_t cmd_head ;   // 下一个要从队列取出的索引
static volatile uint8_t cmd_tail ;   // 下一个要写入队列的位置
static volatile uint8_t cmd_count ;  // 队列中当前元素个数（0～CMD_QUEUE_SIZE）


void parse_command(char *cmd);    //解析数据
static uint8_t enqueue_command(uint8_t src, uint8_t dst);     //将一组数据放入数组
uint8_t dequeue_command(Command_t *out_cmd);      //从数组中取出一组数据
void return_command(void);     //返回数据给上位机
#endif