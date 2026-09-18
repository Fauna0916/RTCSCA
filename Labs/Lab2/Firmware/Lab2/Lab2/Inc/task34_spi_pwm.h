#ifndef TASK34_SPI_PWM_H
#define TASK34_SPI_PWM_H

#include "stm32l0xx_hal.h"

HAL_StatusTypeDef Task34_ReadAnalog(SPI_HandleTypeDef *hspi,
                                    uint16_t *analog_value);
uint32_t Task3_SetLedDuty(TIM_HandleTypeDef *htim, uint16_t analog_value);
uint32_t Task4_SetServoPosition(TIM_HandleTypeDef *htim,
                               uint16_t analog_value);

#endif
