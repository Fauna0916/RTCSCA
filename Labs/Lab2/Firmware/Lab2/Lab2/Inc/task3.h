#ifndef TASK3_H
#define TASK3_H

#include "hspi.h"
#include "htim.h"

uint16_t Task3_ReadAnalog(SPI_HandleTypeDef *hspi);
void Task3_SetLed(TIM_HandleTypeDef *htim, uint16_t analog_value);

#endif
