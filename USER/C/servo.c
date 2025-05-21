#include "servo.h"

#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim4;



void Servo_Down(){
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, SERVO_DOWN_POSITION); 
}

void Servo_Up(){
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, SERVO_UP_POSITION); 
}

void Magnet_On(){
    HAL_GPIO_WritePin(MAGNET_GPIO_PORT,MAGNET_GPIO_PIN,GPIO_PIN_SET);
}

void Magnet_Off(){
    HAL_GPIO_WritePin(MAGNET_GPIO_PORT,MAGNET_GPIO_PIN,GPIO_PIN_RESET);
}


