#ifndef __DECODE_H
#define __DECODE_H

#include <stdint.h>

// 定义队列最大容量
#define CMD_QUEUE_SIZE 32

// 命令结构体
typedef struct {
    uint8_t piece_id;    // 棋子ID (0-4)
    uint8_t board_pos;   // 棋盘位置 (0-8)
    uint8_t need_rotate; // 是否需要旋转 (0/1)
    uint8_t is_cheating; // 是否作弊 (0/1)
} Command_t;

// 外部声明的测试统计变量
extern uint8_t parse_count;
extern uint8_t parse_count_all;
extern uint8_t parse_count_err;
extern uint8_t parse_count_full;

/**
 * @brief 从队列取出一条命令
 * @param out_cmd 输出参数，用于存储取出的命令
 * @return 1:成功 0:队列为空
 */
uint8_t dequeue_command(Command_t *out_cmd);

/**
 * @brief 解析从树莓派发来的单行命令
 * @param cmd 要解析的命令字符串
 * @note 格式: "deploy from 2 to 7，1，0"
 *        分别表示: 棋子ID, 棋盘位置, 是否需要旋转, 是否作弊
 */
void parse_command(char *cmd);

#endif /* __DECODE_H */