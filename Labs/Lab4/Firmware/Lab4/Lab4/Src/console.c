#include "lab4.h"

#include <stdio.h>

extern UART_HandleTypeDef huart2;

void Console_Counter(uint32_t task_id, uint32_t value)
{
  char line[40];
  int length = snprintf(line, sizeof(line), "Task %lu: %lu\r\n",
                        (unsigned long)task_id, (unsigned long)value);

  HAL_UART_Transmit(&huart2, (uint8_t *)line, (uint16_t)length, 100U);
}

void Console_Buffer(uint32_t task_id, uint32_t value, uint32_t active)
{
  char line[64];
  int length = snprintf(line, sizeof(line), "Task %lu: %s %lu, active=%lu\r\n",
                        (unsigned long)task_id, task_id == 1U ? "write" : "read",
                        (unsigned long)value, (unsigned long)active);

  HAL_UART_Transmit(&huart2, (uint8_t *)line, (uint16_t)length, 100U);
}
