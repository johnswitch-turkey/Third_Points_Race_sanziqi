#ifndef _CHESS_TASK_H__
#define _CHESS_TASK_H__


#include "stm32f4xx_hal.h"

typedef struct{
    uint8_t last_chess_ID;
    uint8_t last_board_ID;
    uint8_t total_chess_count;//�ܹ����õ���������
    uint8_t data_refreshed;//����ˢ�±�־λ
  }Task_State;




// void Chess_Task(void);

//void Chess_Task1(uint8_t chess_ID);
//void Chess_Task2(uint8_t chess_ID,uint8_t board_ID);
//void Chess_Task3(uint8_t chess_ID,uint8_t board_ID);
//void Chess_Task4(uint8_t board_ID);
//void Chess_Task5(uint8_t board_ID);
//void Chess_Task6(uint8_t board_ID1, uint8_t board_ID2);

#endif

