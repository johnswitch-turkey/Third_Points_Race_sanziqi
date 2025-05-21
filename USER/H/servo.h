#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f4xx_hal.h"

#define SERVO_GPIO_PIN     GPIO_PIN_2
#define SERVO_GPIO_PORT    GPIOA
#define SERVO_TIM          TIM4
#define SERVO_CHANNEL      TIM_CHANNEL_1

#define MAGNET_GPIO_PIN GPIO_PIN_3
#define MAGNET_GPIO_PORT GPIOA

#define SERVO_UP_POSITION  260
#define SERVO_DOWN_POSITION 150

void Servo_Down();
void Servo_Up();
void Magnet_On();
void Magnet_Off();

#endif