#include "decode.h"
#include "time.h"
#include "usart.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 接收到下棋信号
uint8_t deploy_enqueue_flag = 0;    
    
static volatile uint8_t deploy_cmd_head = 0 ;   // 下一个要从队列取出的索引
static volatile uint8_t deploy_cmd_tail = 0 ;   // 下一个要写入队列的位置
static volatile uint8_t deploy_cmd_count = 0 ;  // 队列中当前元素个数（0～CMD_QUEUE_SIZE）

// 接收到反作弊信号
uint8_t recover_enqueue_flag = 0;   

static volatile uint8_t recover_cmd_head = 0 ;   // 下一个要从队列取出的索引
static volatile uint8_t recover_cmd_tail = 0 ;   // 下一个要写入队列的位置
static volatile uint8_t recover_cmd_count = 0 ;  // 队列中当前元素个数（0～CMD_QUEUE_SIZE）


// 测试专用
uint8_t parse_count = 0;
uint8_t parse_count_all = 0;
uint8_t  parse_count_err = 0;
uint8_t  parse_count_full = 0;



/**
    根据标志位，将一个命令（src, dst）放入队列
		
		deploy_cmd_queue ,  用于下棋的队列，每个元素（结构体）有2个数据，前者是盘外棋子的序号，后者是小方格的序号
		recover_cmd_queue,  用于防作弊的队列，每个元素同样有2个数据，且都是小方格的序号
		
    src: 要“取子”的编号，0～4
    dst: 要“落子”的格子编号，0～8
    1：入队成功；0：队列已满，入队失败
  */
static uint8_t enqueue_command(uint8_t src, uint8_t dst)
{
	if(deploy_enqueue_flag==1)
		{
    if (deploy_cmd_count >= CMD_QUEUE_SIZE) {
        // 队列已满，无法再入队
        return 0;
    }
			deploy_enqueue_flag = 0;   // 标志位清零
			deploy_cmd_queue[deploy_cmd_tail].src = src;
			deploy_cmd_queue[deploy_cmd_tail].dst = dst;
			deploy_cmd_tail = (deploy_cmd_tail + 1) % CMD_QUEUE_SIZE;
			__disable_irq();     // 进入临界区，防止中断打乱 cmd_count
			deploy_cmd_count++;
			__enable_irq();      // 退出临界区
			return 1;
		}
		
		else if(recover_enqueue_flag==1)
		{
			if (recover_cmd_count >= CMD_QUEUE_SIZE) {
        // 队列已满，无法再入队
        return 0;
    }
			recover_enqueue_flag = 0;   // 标志位清零
			recover_cmd_queue[recover_cmd_tail].src = src;
			recover_cmd_queue[recover_cmd_tail].dst = dst;
			recover_cmd_tail = (recover_cmd_tail + 1) % CMD_QUEUE_SIZE;
			__disable_irq();     // 进入临界区，防止中断打乱 cmd_count
			recover_cmd_count++;
			__enable_irq();      // 退出临界区
			return 1;
		}	
}



/**
  从队列取出一条命令,如果是0，从下棋队列中取数据，否则从反作弊队列中取
  out_cmd: 用于存放取出的命令
  */
uint8_t dequeue_command(Command_t *out_cmd, uint8_t is_deploy)
{
	if(is_deploy)
		{
    if (deploy_cmd_count == 0) 
			{
        // 队列空
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
        // 队列空
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
    解析从树莓派发来的单行命令（如 "from 2 to 7"）
	如果成功解析出 src、dst，就调用 enqueue_command() 放入队列
 
  */
void parse_command(char *cmd)
{
    int src = -1, dst = -1;
	
	 parse_count_all++;

		// 检测到下棋信号，把命令存放在deploy_cmd_queue数组里
    if (sscanf(cmd, "deploy from %d to %d", &src, &dst) == 2) 
			{
			deploy_enqueue_flag = 1; 
        if (!enqueue_command(src, dst)) 
					{
            // 队列已满
					 parse_count_full++;
            
        } 
				else
					{
						parse_count++;
				}
				
				
			}
			
			 //检测到反作弊信号，把命令存放在recover_cmd_queue数组里
				else if (sscanf(cmd, "recover from %d to %d", &src, &dst) == 2)
				{
					recover_enqueue_flag = 1;
					 if (!enqueue_command(src, dst)) 
					{
            // 队列已满
					 parse_count_full++;
					}
					else
					{
						parse_count++;
					}
				}
				
    else {
        // 如果格式不对，发送 ERR
			 parse_count_err++;
        //char err_msg[] = "ERR\r\n";
        //HAL_UART_Transmit(&huart6, (uint8_t *)err_msg, strlen(err_msg), HAL_MAX_DELAY);
    }
}




