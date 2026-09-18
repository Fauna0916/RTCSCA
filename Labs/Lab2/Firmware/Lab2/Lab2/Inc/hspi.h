#ifndef HSPI_H
#define HSPI_H

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_gpio.h"
#include "stm32l0xx_hal_spi.h"

extern SPI_HandleTypeDef hspi1;

#define SPI_CS_Pin GPIO_PIN_6
#define SPI_CS_GPIO_Port GPIOB

#endif
