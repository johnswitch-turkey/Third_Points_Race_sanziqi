#include "decode.h"
#include "time.h"
#include "usart.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static volatile uint8_t cmd_head = 0 ;   // ��һ��Ҫ�Ӷ���ȡ��������
static volatile uint8_t cmd_tail = 0 ;   // ��һ��Ҫд����е�λ��
static volatile uint8_t cmd_count = 0 ;  // �����е�ǰԪ�ظ�����0��CMD_QUEUE_SIZE��

/**
    ���Խ�һ�����src, dst���������
    src: Ҫ��ȡ�ӡ��ı�ţ�0��4
    dst: Ҫ�����ӡ��ĸ��ӱ�ţ�0��8
    1����ӳɹ���0���������������ʧ��
  */
static uint8_t enqueue_command(uint8_t src, uint8_t dst)
{
    if (cmd_count >= CMD_QUEUE_SIZE) {
        // �����������޷������
        return 0;
    }
    cmd_queue[cmd_tail].src = src;
    cmd_queue[cmd_tail].dst = dst;
    cmd_tail = (cmd_tail + 1) % CMD_QUEUE_SIZE;
    __disable_irq();     // �����ٽ�������ֹ�жϴ��� cmd_count
    cmd_count++;
    __enable_irq();      // �˳��ٽ���
    return 1;
}



/**
  �Ӷ���ȡ��һ������
  out_cmd: ���ڴ��ȡ��������
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
    ��������ݮ�ɷ����ĵ�������� "from 2 to 7"��
	����ɹ������� src��dst���͵��� enqueue_command() �������
 
  */
void parse_command(char *cmd)
{
    int src = -1, dst = -1;

    // ʹ�� sscanf ��ȡ "from %d to %d" ��ʽ
    if (sscanf(cmd, "from %d to %d", &src, &dst) == 2) {
        // ��������������Ϳ���ѡ�������߷�������ʾ
        if (!enqueue_command(src, dst)) {
            // ��������������ʾ������һ�� "FULL" ��ʾ�� Raspberry Pi
            char full_msg[] = "FULL\r\n";
            //HAL_UART_Transmit(&huart6, (uint8_t *)full_msg, strlen(full_msg), HAL_MAX_DELAY);
        } 
				else{}
				/*
				else {
            // ��ӳɹ������Է���һ���򵥵� ACK
            char ack_msg[32];
            int len = snprintf(ack_msg, sizeof(ack_msg), "ACK %d->%d\r\n", src, dst);
            HAL_UART_Transmit(&huart1, (uint8_t *)ack_msg, len, HAL_MAX_DELAY);
        }
				*/
    } else {
        // �����ʽ���ԣ����� ERR
        char err_msg[] = "ERR\r\n";
        //HAL_UART_Transmit(&huart6, (uint8_t *)err_msg, strlen(err_msg), HAL_MAX_DELAY);
    }
}




