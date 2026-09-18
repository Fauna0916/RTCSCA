#ifndef LAB2_APP_H
#define LAB2_APP_H

#include "stm32l0xx_hal.h"

void Lab2_AppInit(UART_HandleTypeDef *huart,
                  I2C_HandleTypeDef *hi2c,
                  SPI_HandleTypeDef *hspi,
                  TIM_HandleTypeDef *htim_led,
                  TIM_HandleTypeDef *htim_servo);
void Lab2_AppProcess(void);

#endif
