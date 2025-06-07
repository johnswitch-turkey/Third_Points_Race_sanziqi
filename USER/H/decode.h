#ifndef __DECODE_H
#define __DECODE_H

#include <stdint.h>

// ��������������
#define CMD_QUEUE_SIZE 32

// ����ṹ��
typedef struct {
    uint8_t piece_id;    // ����ID (0-4)
    uint8_t board_pos;   // ����λ�� (0-8)
    uint8_t need_rotate; // �Ƿ���Ҫ��ת (0/1)
    uint8_t is_cheating; // �Ƿ����� (0/1)
} Command_t;

// �ⲿ�����Ĳ���ͳ�Ʊ���
extern uint8_t parse_count;
extern uint8_t parse_count_all;
extern uint8_t parse_count_err;
extern uint8_t parse_count_full;

/**
 * @brief �Ӷ���ȡ��һ������
 * @param out_cmd ������������ڴ洢ȡ��������
 * @return 1:�ɹ� 0:����Ϊ��
 */
uint8_t dequeue_command(Command_t *out_cmd);

/**
 * @brief ��������ݮ�ɷ����ĵ�������
 * @param cmd Ҫ�����������ַ���
 * @note ��ʽ: "deploy from 2 to 7��1��0"
 *        �ֱ��ʾ: ����ID, ����λ��, �Ƿ���Ҫ��ת, �Ƿ�����
 */
void parse_command(char *cmd);

#endif /* __DECODE_H */