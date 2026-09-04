#include "lab1_app.h"

#include <stdbool.h>

#include "main.h"

#define BUTTON_DEBOUNCE_MS 30U

static TIM_HandleTypeDef *timer;
static bool blinking;
static uint32_t last_button_tick;

static void StartBlinking(void)
{
  blinking = true;
  GPIOA->ODR |= LD2_Pin;
  __HAL_TIM_SET_COUNTER(timer, 0U);
  HAL_TIM_Base_Start_IT(timer);
}

static void StopBlinking(void)
{
  blinking = false;
  HAL_TIM_Base_Stop_IT(timer);
  GPIOA->ODR &= ~LD2_Pin;
}

void Lab1_AppInit(TIM_HandleTypeDef *blink_timer)
{
  timer = blink_timer;
  blinking = false;
  last_button_tick = HAL_GetTick() - BUTTON_DEBOUNCE_MS;
  GPIOA->ODR &= ~LD2_Pin;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint32_t now;

  if (GPIO_Pin != B1_Pin)
  {
    return;
  }

  now = HAL_GetTick();
  if ((now - last_button_tick) < BUTTON_DEBOUNCE_MS)
  {
    return;
  }
  last_button_tick = now;

  if (blinking)
  {
    StopBlinking();
  }
  else
  {
    StartBlinking();
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if ((htim->Instance == TIM2) && blinking)
  {
    GPIOA->ODR ^= LD2_Pin;
  }
}

