#ifndef TASK1_UART_CALC_H
#define TASK1_UART_CALC_H

#include <stdbool.h>
#include <stdint.h>

bool Task1_ParseNumbers(const char *text, uint8_t values[3]);
bool Task1_Calculate(const uint8_t values[3], char operation, uint32_t *result);

#endif
