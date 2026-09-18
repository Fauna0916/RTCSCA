#ifndef TASK4_H
#define TASK4_H

#include "hspi.h"
#include "htim.h"

uint16_t Task4_ReadAnalog(SPI_HandleTypeDef *hspi);
void Task4_SetServo(TIM_HandleTypeDef *htim, uint16_t analog_value);

#endif
