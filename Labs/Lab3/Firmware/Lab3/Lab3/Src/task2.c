#include "lab3.h"

#include <stdlib.h>
#include <string.h>

#if LAB3_TASK == 2

#define UART_EVENT 1U
#define QUEUE_EVENT 2U
#define FRAME_LENGTH 11U
#define QUEUE_LENGTH 4U

extern UART_HandleTypeDef huart2;
static osThreadId_t uart_thread;
static osThreadId_t led_thread;
static osMessageQueueId_t queue1;
static osMessageQueueId_t queue2;
static osMessageQueueId_t queue3;
static uint8_t frame[FRAME_LENGTH];

/* Read in the debugger after a command to measure unused worker stack. */
volatile uint32_t task2_stack_free;

static void ReceiveCommand(void)
{
  uint16_t interval = 0U;
  uint32_t i;

  /* Echo all eleven received bytes, then end the display line. */
  HAL_UART_Transmit(&huart2, frame, FRAME_LENGTH, 100U);
  Console_Write("\r\n");

  if (memcmp(&frame[1], "DELAY", 5U) != 0) {
    return;
  }
  for (i = 7U; i < FRAME_LENGTH; ++i) {
    if ((frame[i] < '0') || (frame[i] > '9')) {
      return;
    }
    interval = (uint16_t)(interval * 10U + frame[i] - '0');
  }
  if (interval != 0U) {
    osMessageQueuePut(queue1, &interval, 0U, osWaitForever);
  }
}

static void UartTask(void *argument)
{
  uint32_t flags;
  uint32_t count;
  int maximum;
  (void)argument;

  HAL_UART_Receive_IT(&huart2, frame, FRAME_LENGTH);
  for (;;) {
    flags = osThreadFlagsWait(UART_EVENT | QUEUE_EVENT,
                              osFlagsWaitAny, osWaitForever);
    if ((flags & UART_EVENT) != 0U) {
      ReceiveCommand();
      HAL_UART_Receive_IT(&huart2, frame, FRAME_LENGTH);
    }
    while (osMessageQueueGet(queue2, &count, NULL, 0U) == osOK) {
      Console_Number("Count: ", count);
    }
    while (osMessageQueueGet(queue3, &maximum, NULL, 0U) == osOK) {
      Console_Number("Max: ", (uint32_t)maximum);
    }
  }
}

static void LedTask(void *argument)
{
  int samples[150];
  int maximum;
  uint16_t interval = 0U;
  uint16_t command;
  uint32_t deadline = 0U;
  uint32_t toggles = 0U;
  uint32_t count = 0U;
  uint32_t timeout;
  int32_t remaining;
  uint32_t i;
  (void)argument;

  srand(HAL_GetTick());
  for (;;) {
    remaining = (int32_t)(deadline - osKernelGetTickCount());
    timeout = interval == 0U ? osWaitForever
              : (remaining > 0 ? (uint32_t)remaining : 0U);

    /* A queue wait wakes either for a new command or the next toggle. */
    if (osMessageQueueGet(queue1, &command, NULL, timeout) == osOK) {
      interval = command;
      for (i = 0U; i < 150U; ++i) {
        samples[i] = rand();
      }
      maximum = samples[0];
      for (i = 1U; i < 150U; ++i) {
        if (samples[i] > maximum) {
          maximum = samples[i];
        }
      }
      osMessageQueuePut(queue3, &maximum, 0U, osWaitForever);
      osThreadFlagsSet(uart_thread, QUEUE_EVENT);
      deadline = osKernelGetTickCount() + interval;
    } else {
      HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
      deadline += interval;
      if (++toggles == 10U) {
        toggles = 0U;
        ++count;
        osMessageQueuePut(queue2, &count, 0U, osWaitForever);
        osThreadFlagsSet(uart_thread, QUEUE_EVENT);
      }
    }
    task2_stack_free = osThreadGetStackSpace(led_thread);
  }
}

void Task2_Start(void)
{
  const osThreadAttr_t uart_attributes = {
    .name = "UART", .priority = osPriorityAboveNormal, .stack_size = 1024U
  };
  const osThreadAttr_t led_attributes = {
    .name = "LED", .priority = osPriorityNormal, .stack_size = 1536U
  };

  queue1 = osMessageQueueNew(QUEUE_LENGTH, sizeof(uint16_t), NULL);
  queue2 = osMessageQueueNew(QUEUE_LENGTH, sizeof(uint32_t), NULL);
  queue3 = osMessageQueueNew(QUEUE_LENGTH, sizeof(int), NULL);
  uart_thread = osThreadNew(UartTask, NULL, &uart_attributes);
  led_thread = osThreadNew(LedTask, NULL, &led_attributes);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uart)
{
  if (uart->Instance == USART2) {
    /* The task consumes the completed frame before reception is rearmed. */
    osThreadFlagsSet(uart_thread, UART_EVENT);
  }
}

#endif
