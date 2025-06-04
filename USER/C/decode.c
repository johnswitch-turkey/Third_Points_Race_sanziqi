#include "decode.h"
#include "time.h"
#include "usart.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ���յ������ź�
uint8_t deploy_enqueue_flag = 0;    
    
static volatile uint8_t deploy_cmd_head = 0 ;   // ��һ��Ҫ�Ӷ���ȡ��������
static volatile uint8_t deploy_cmd_tail = 0 ;   // ��һ��Ҫд����е�λ��
static volatile uint8_t deploy_cmd_count = 0 ;  // �����е�ǰԪ�ظ�����0��CMD_QUEUE_SIZE��

// ���յ��������ź�
uint8_t recover_enqueue_flag = 0;   

static volatile uint8_t recover_cmd_head = 0 ;   // ��һ��Ҫ�Ӷ���ȡ��������
static volatile uint8_t recover_cmd_tail = 0 ;   // ��һ��Ҫд����е�λ��
static volatile uint8_t recover_cmd_count = 0 ;  // �����е�ǰԪ�ظ�����0��CMD_QUEUE_SIZE��


// ����ר��
uint8_t parse_count = 0;
uint8_t parse_count_all = 0;
uint8_t  parse_count_err = 0;
uint8_t  parse_count_full = 0;



/**
    ���ݱ�־λ����һ�����src, dst���������
		
		deploy_cmd_queue ,  ��������Ķ��У�ÿ��Ԫ�أ��ṹ�壩��2�����ݣ�ǰ�����������ӵ���ţ�������С��������
		recover_cmd_queue,  ���ڷ����׵Ķ��У�ÿ��Ԫ��ͬ����2�����ݣ��Ҷ���С��������
		
    src: Ҫ��ȡ�ӡ��ı�ţ�0��4
    dst: Ҫ�����ӡ��ĸ��ӱ�ţ�0��8
    1����ӳɹ���0���������������ʧ��
  */
static uint8_t enqueue_command(uint8_t src, uint8_t dst)
{
	if(deploy_enqueue_flag==1)
		{
    if (deploy_cmd_count >= CMD_QUEUE_SIZE) {
        // �����������޷������
        return 0;
    }
			deploy_enqueue_flag = 0;   // ��־λ����
			deploy_cmd_queue[deploy_cmd_tail].src = src;
			deploy_cmd_queue[deploy_cmd_tail].dst = dst;
			deploy_cmd_tail = (deploy_cmd_tail + 1) % CMD_QUEUE_SIZE;
			__disable_irq();     // �����ٽ�������ֹ�жϴ��� cmd_count
			deploy_cmd_count++;
			__enable_irq();      // �˳��ٽ���
			return 1;
		}
		
		else if(recover_enqueue_flag==1)
		{
			if (recover_cmd_count >= CMD_QUEUE_SIZE) {
        // �����������޷������
        return 0;
    }
			recover_enqueue_flag = 0;   // ��־λ����
			recover_cmd_queue[recover_cmd_tail].src = src;
			recover_cmd_queue[recover_cmd_tail].dst = dst;
			recover_cmd_tail = (recover_cmd_tail + 1) % CMD_QUEUE_SIZE;
			__disable_irq();     // �����ٽ�������ֹ�жϴ��� cmd_count
			recover_cmd_count++;
			__enable_irq();      // �˳��ٽ���
			return 1;
		}	
}



/**
  �Ӷ���ȡ��һ������,�����0�������������ȡ���ݣ�����ӷ����׶�����ȡ
  out_cmd: ���ڴ��ȡ��������
  */
uint8_t dequeue_command(Command_t *out_cmd, uint8_t is_deploy)
{
	if(is_deploy)
		{
    if (deploy_cmd_count == 0) 
			{
        // ���п�
        return 0;
    }
	
    *out_cmd = deploy_cmd_queue[deploy_cmd_head];
    deploy_cmd_head = (deploy_cmd_head + 1) % CMD_QUEUE_SIZE;
    __disable_irq();
    deploy_cmd_count--;
    __enable_irq();
    return 1;
	}
	else
		{
    if (recover_cmd_count == 0) {
        // ���п�
        return 0;
    }
	
    *out_cmd = recover_cmd_queue[recover_cmd_head];
    recover_cmd_head = (recover_cmd_head + 1) % CMD_QUEUE_SIZE;
    __disable_irq();
    recover_cmd_count--;
    __enable_irq();
    return 1;
	}
		
}



/**
    ��������ݮ�ɷ����ĵ�������� "from 2 to 7"��
	����ɹ������� src��dst���͵��� enqueue_command() �������
 
  */
void parse_command(char *cmd)
{
    int src = -1, dst = -1;
	
	 parse_count_all++;

		// ��⵽�����źţ�����������deploy_cmd_queue������
    if (sscanf(cmd, "deploy from %d to %d", &src, &dst) == 2) 
			{
			deploy_enqueue_flag = 1; 
        if (!enqueue_command(src, dst)) 
					{
            // ��������
					 parse_count_full++;
            
        } 
				else
					{
						parse_count++;
				}
				
				
			}
			
			 //��⵽�������źţ�����������recover_cmd_queue������
				else if (sscanf(cmd, "recover from %d to %d", &src, &dst) == 2)
				{
					recover_enqueue_flag = 1;
					 if (!enqueue_command(src, dst)) 
					{
            // ��������
					 parse_count_full++;
					}
					else
					{
						parse_count++;
					}
				}
				
    else {
        // �����ʽ���ԣ����� ERR
			 parse_count_err++;
        //char err_msg[] = "ERR\r\n";
        //HAL_UART_Transmit(&huart6, (uint8_t *)err_msg, strlen(err_msg), HAL_MAX_DELAY);
    }
}




