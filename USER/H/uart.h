#ifndef __UART_H
#define __UART_H

#include "stm32f4xx_hal.h"

extern uint8_t motor_flag;
extern uint8_t rotate_flag;

void UART_Init(void);
uint8_t Get_Piece_ID(void);
uint8_t Get_Board_ID(void);

#endif
