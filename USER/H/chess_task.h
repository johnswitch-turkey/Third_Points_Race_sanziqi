#ifndef _CHESS_TASK_H__
#define _CHESS_TASK_H__


#include "stm32f4xx_hal.h"

typedef struct{
    uint8_t last_chess_ID;
    uint8_t last_board_ID;
    uint8_t total_chess_count;//�ܹ����õ���������
    uint8_t data_refreshed;//����ˢ�±�־λ
  }Task_State;


extern volatile uint8_t rotate_flag;



#endif

