#include "decode.h"
#include "time.h"
#include "usart.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ������нṹ��


// �������
static Command_t cmd_queue[CMD_QUEUE_SIZE];
static volatile uint8_t cmd_head = 0;    // ����ͷָ��
static volatile uint8_t cmd_tail = 0;    // ����βָ��
static volatile uint8_t cmd_count = 0;   // �����е�ǰԪ�ظ���

// ����ͳ�Ʊ���
uint8_t parse_count = 0;
uint8_t parse_count_all = 0;
uint8_t parse_count_err = 0;
uint8_t parse_count_full = 0;

/**
 * ������������
 * 
 * @param piece_id    ����ID (0-4)
 * @param board_pos   ����λ�� (0-8)
 * @param need_rotate �Ƿ���Ҫ��ת (0/1)
 * @param is_cheating �Ƿ����� (0/1)
 * @return 1:��ӳɹ� 0:��������
 */
static uint8_t enqueue_command(uint8_t piece_id, uint8_t board_pos, 
                             uint8_t need_rotate, uint8_t is_cheating)
{
    if (cmd_count >= CMD_QUEUE_SIZE) {
        // ��������
        parse_count_full++;
        return 0;
    }
    
    cmd_queue[cmd_tail].piece_id = piece_id;
    cmd_queue[cmd_tail].board_pos = board_pos;
    cmd_queue[cmd_tail].need_rotate = need_rotate;
    cmd_queue[cmd_tail].is_cheating = is_cheating;
    
    cmd_tail = (cmd_tail + 1) % CMD_QUEUE_SIZE;
    
    __disable_irq();     // �����ٽ���
    cmd_count++;
    __enable_irq();      // �˳��ٽ���
    
    return 1;
}

/**
 * �Ӷ���ȡ��һ������
 * 
 * @param out_cmd ������������ڴ洢ȡ��������
 * @return 1:�ɹ� 0:����Ϊ��
 */
uint8_t dequeue_command(Command_t *out_cmd)
{
    if (cmd_count == 0) {
        // ���п�
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
 * ��������ݮ�ɷ����ĵ�������
 * ��ʽ: "deploy from 2 to 7��1��0"
 * �ֱ��ʾ: ����ID, ����λ��, �Ƿ���Ҫ��ת, �Ƿ�����
 */
void parse_command(char *cmd)
{
    int piece_id = -1;
    int board_pos = -1;
    int need_rotate = -1;
    int is_cheating = -1;
    
    parse_count_all++;

    // ���������ʽ
    if (sscanf(cmd, "deploy from %d to %d,%d,%d", 
              &piece_id, &board_pos, &need_rotate, &is_cheating) == 4) {
        
        // ��֤������Χ
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
        // ��ʽ����
        parse_count_err++;
        // ���Է��ʹ�����Ӧ
        // char err_msg[] = "ERR\r\n";
        // HAL_UART_Transmit(&huart6, (uint8_t *)err_msg, strlen(err_msg), HAL_MAX_DELAY);
    }
}