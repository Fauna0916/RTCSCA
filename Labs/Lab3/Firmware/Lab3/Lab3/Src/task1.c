#include "lab3.h"

#if LAB3_TASK == 1

#define BUTTON_EVENT 1U
#define MESSAGE_EVENT 1U

static osThreadId_t brake_thread;
static osThreadId_t message_thread;

static void BrakeTask(void *argument)
{
  (void)argument;

  for (;;) {
    osThreadFlagsWait(BUTTON_EVENT, osFlagsWaitAny, osWaitForever);

    /* Apply the brake before waking the lower-priority UART task. */
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    osThreadFlagsSet(message_thread, MESSAGE_EVENT);

    /* Keep the brake applied while the button is held. */
    while (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
      osDelay(10U);
    }
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  }
}

static void MessageTask(void *argument)
{
  (void)argument;

  for (;;) {
    osThreadFlagsWait(MESSAGE_EVENT, osFlagsWaitAny, osWaitForever);
    Console_Write("Car is slowing down\r\n");
  }
}

void Task1_Start(void)
{
  const osThreadAttr_t brake_attributes = {
    .name = "Brake", .priority = osPriorityHigh, .stack_size = 512U
  };
  const osThreadAttr_t message_attributes = {
    .name = "Message", .priority = osPriorityNormal, .stack_size = 768U
  };

  message_thread = osThreadNew(MessageTask, NULL, &message_attributes);
  brake_thread = osThreadNew(BrakeTask, NULL, &brake_attributes);
}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
  static uint32_t last_press;
  uint32_t now = HAL_GetTick();

  /* A 30 ms debounce gate accepts one falling edge per press. */
  if ((pin == B1_Pin) && (now - last_press >= 30U)) {
    last_press = now;
    osThreadFlagsSet(brake_thread, BUTTON_EVENT);
  }
}

#endif
