#ifndef TASK2_I2C_H
#define TASK2_I2C_H

#include "stm32l0xx_hal.h"

#define TASK2_TEXT_LENGTH 4U

typedef struct {
  uint16_t analog_value;
  char text[TASK2_TEXT_LENGTH + 1U];
} Task2_Result;

HAL_StatusTypeDef Task2_Exchange(I2C_HandleTypeDef *hi2c,
                                 uint8_t speed_selection,
                                 uint8_t data_selection,
                                 Task2_Result *result);

#endif
