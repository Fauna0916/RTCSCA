#include "task1.h"

uint32_t Task1_Calculate(uint8_t values[3], char operation)
{
  if (operation == 'A') {
    return (uint32_t)values[0] + values[1] + values[2];
  }

  return (uint32_t)values[0] * values[1] * values[2];
}
