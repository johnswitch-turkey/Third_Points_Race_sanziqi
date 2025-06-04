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
// 用于下棋的队列，每个元素（结构体）有2个数据，前者是盘外棋子的序号，后者是小方格的序号
static Command_t deploy_cmd_queue[CMD_QUEUE_SIZE];
// 用于防作弊的队列，每个元素同样有2个数据，且都是小方格的序号
static Command_t recover_cmd_queue[CMD_QUEUE_SIZE];

/* 队列操作用的索引与计数 */
extern uint8_t deploy_enqueue_flag ;    
    
static volatile uint8_t deploy_cmd_head  ;   // 下一个要从队列取出的索引
static volatile uint8_t deploy_cmd_tail  ;   // 下一个要写入队列的位置
static volatile uint8_t deploy_cmd_count ;  // 队列中当前元素个数（0～CMD_QUEUE_SIZE）


extern uint8_t recover_enqueue_flag ;   

static volatile uint8_t recover_cmd_head  ;   // 下一个要从队列取出的索引
static volatile uint8_t recover_cmd_tail  ;   // 下一个要写入队列的位置
static volatile uint8_t recover_cmd_count  ;  // 队列中当前元素个数（0～CMD_QUEUE_SIZE）



void parse_command(char *cmd);    //解析数据
static uint8_t enqueue_command(uint8_t src, uint8_t dst);     //将一组数据放入数组
uint8_t dequeue_command(Command_t *out_cmd, uint8_t is_deploy);      //从数组中取出一组数据
#endif