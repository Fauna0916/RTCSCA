#include "task1_uart_calc.h"

#include <stdio.h>

bool Task1_ParseNumbers(const char *text, uint8_t values[3])
{
  unsigned int a;
  unsigned int b;
  unsigned int c;

  if (sscanf(text, "%u %u %u", &a, &b, &c) != 3) {
    return false;
  }

  if ((a < 10U) || (a > 99U)
      || (b < 10U) || (b > 99U)
      || (c < 10U) || (c > 99U)) {
    return false;
  }

  values[0] = (uint8_t)a;
  values[1] = (uint8_t)b;
  values[2] = (uint8_t)c;
  return true;
}

bool Task1_Calculate(const uint8_t values[3], char operation, uint32_t *result)
{
  if ((operation == 'A') || (operation == 'a') || (operation == '+')) {
    *result = (uint32_t)values[0] + values[1] + values[2];
    return true;
  }

  if ((operation == 'M') || (operation == 'm') || (operation == '*')) {
    *result = (uint32_t)values[0] * values[1] * values[2];
    return true;
  }

  return false;
}
