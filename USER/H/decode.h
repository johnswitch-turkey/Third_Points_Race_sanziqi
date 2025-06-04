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
// ��������Ķ��У�ÿ��Ԫ�أ��ṹ�壩��2�����ݣ�ǰ�����������ӵ���ţ�������С��������
static Command_t deploy_cmd_queue[CMD_QUEUE_SIZE];
// ���ڷ����׵Ķ��У�ÿ��Ԫ��ͬ����2�����ݣ��Ҷ���С��������
static Command_t recover_cmd_queue[CMD_QUEUE_SIZE];

/* ���в����õ���������� */
extern uint8_t deploy_enqueue_flag ;    
    
static volatile uint8_t deploy_cmd_head  ;   // ��һ��Ҫ�Ӷ���ȡ��������
static volatile uint8_t deploy_cmd_tail  ;   // ��һ��Ҫд����е�λ��
static volatile uint8_t deploy_cmd_count ;  // �����е�ǰԪ�ظ�����0��CMD_QUEUE_SIZE��


extern uint8_t recover_enqueue_flag ;   

static volatile uint8_t recover_cmd_head  ;   // ��һ��Ҫ�Ӷ���ȡ��������
static volatile uint8_t recover_cmd_tail  ;   // ��һ��Ҫд����е�λ��
static volatile uint8_t recover_cmd_count  ;  // �����е�ǰԪ�ظ�����0��CMD_QUEUE_SIZE��



void parse_command(char *cmd);    //��������
static uint8_t enqueue_command(uint8_t src, uint8_t dst);     //��һ�����ݷ�������
uint8_t dequeue_command(Command_t *out_cmd, uint8_t is_deploy);      //��������ȡ��һ������
#endif