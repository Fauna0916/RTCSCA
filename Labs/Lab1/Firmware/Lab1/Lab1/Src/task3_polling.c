#include "task3_polling.h"

#include <stdbool.h>

#include "main.h"

void Task3_UpdateLedFromButton(void)
{
  bool button_pressed = (GPIOC->IDR & B1_Pin) == 0U;

  if (button_pressed)
  {
    GPIOA->ODR |= LD2_Pin;
  }
  else
  {
    GPIOA->ODR &= ~LD2_Pin;
  }
}

