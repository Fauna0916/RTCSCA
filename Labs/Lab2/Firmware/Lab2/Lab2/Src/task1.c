#include "task1.h"

#include <stdio.h>
#include <string.h>

static void ReadLine(char *line)
{
  uint8_t byte;
  uint8_t i = 0U;

  while (1) {
    HAL_UART_Receive(&huart2, &byte, 1U, HAL_MAX_DELAY);
    if ((byte == '\r') || (byte == '\n')) {
      line[i] = '\0';
      return;
    }
    line[i++] = (char)byte;
  }
}

static void Print(const char *text)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)text,
                    (uint16_t)strlen(text), HAL_MAX_DELAY);
}

void Task1(void)
{
  char line[32];
  uint8_t values[3];
  uint32_t result;

  Print("Enter three 2-digit numbers: ");
  ReadLine(line);
  sscanf(line, "%hhu %hhu %hhu",
         &values[0], &values[1], &values[2]);

  Print("A for add, M for multiply: ");
  ReadLine(line);

  if (line[0] == 'A') {
    result = (uint32_t)values[0] + values[1] + values[2];
  } else {
    result = (uint32_t)values[0] * values[1] * values[2];
  }

  snprintf(line, sizeof(line), "Result: %lu\r\n", (unsigned long)result);
  Print(line);
}
