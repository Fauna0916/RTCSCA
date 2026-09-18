#ifndef TASK2_H
#define TASK2_H

#include "hi2c.h"

#define TASK2_TEXT_LENGTH 4U

typedef struct {
  uint16_t analog_value;
  char text[TASK2_TEXT_LENGTH + 1U];
} Task2_Result;

void Task2_Exchange(I2C_HandleTypeDef *hi2c,
                    uint8_t speed,
                    uint8_t data,
                    Task2_Result *result);

#endif
