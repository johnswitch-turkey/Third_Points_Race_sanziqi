#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f4xx_hal.h"

#define SERVO_GPIO_PIN     GPIO_PIN_6
#define SERVO_GPIO_PORT    GPIOB
#define SERVO_TIM          TIM4
#define SERVO_CHANNEL      TIM_CHANNEL_1

#define MAGNET_GPIO_PIN GPIO_PIN_4
#define MAGNET_GPIO_PORT GPIOA

#define SERVO_UP_POSITION  2000
#define SERVO_DOWN_POSITION 2450

void Servo_Down();
void Servo_Up();
void Magnet_On();
void Magnet_Off();

#endif