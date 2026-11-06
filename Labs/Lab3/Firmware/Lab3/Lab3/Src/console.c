#include "lab3.h"

#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

void Console_Write(const char *text)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)text,
                    (uint16_t)strlen(text), 100U);
}

void Console_Number(const char *label, uint32_t value)
{
  char line[40];

  snprintf(line, sizeof(line), "%s%lu\r\n", label, (unsigned long)value);
  Console_Write(line);
}
