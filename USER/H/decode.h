#ifndef __DECODE_H__
#define __DECODE_H__

#include "stm32f4xx_hal.h"

/* ���� ����������� ���� */
#define CMD_QUEUE_SIZE  16    // ��໺�� 16 ������ɰ������

typedef struct {
    uint8_t src;
    uint8_t dst;
} Command_t;

/* ���ζ��б��� */
static Command_t cmd_queue[CMD_QUEUE_SIZE];

/* ���в����õ���������� */
static volatile uint8_t cmd_head ;   // ��һ��Ҫ�Ӷ���ȡ��������
static volatile uint8_t cmd_tail ;   // ��һ��Ҫд����е�λ��
static volatile uint8_t cmd_count ;  // �����е�ǰԪ�ظ�����0��CMD_QUEUE_SIZE��


void parse_command(char *cmd);    //��������
static uint8_t enqueue_command(uint8_t src, uint8_t dst);     //��һ�����ݷ�������
uint8_t dequeue_command(Command_t *out_cmd);      //��������ȡ��һ������
void return_command(void);     //�������ݸ���λ��
#endif