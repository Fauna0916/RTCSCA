#include "lab4.h"

static osSemaphoreId_t buffer_slots;
static osMutexId_t uart_mutex;
static volatile uint8_t buffer[2];

static void AccessBuffer(uint32_t task_id, uint8_t value)
{
  uint32_t active;

  /* A third contender skips this access when both permits are occupied. */
  if (osSemaphoreAcquire(buffer_slots, 0U) != osOK) {
    return;
  }
  if (task_id == 1U) {
    buffer[1] = buffer[0];
    buffer[0] = value;
  } else {
    value = buffer[1];
  }
  active = 2U - osSemaphoreGetCount(buffer_slots);
#ifdef LAB4_BUFFER_HOLD_TICKS
  /* Extend the holding time only for the contention measurement. */
  osDelay(LAB4_BUFFER_HOLD_TICKS);
#endif
  osSemaphoreRelease(buffer_slots);

  /* Report the sampled value and occupancy after releasing the buffer. */
  osMutexAcquire(uart_mutex, osWaitForever);
  Console_Buffer(task_id, value, active);
  osMutexRelease(uart_mutex);
}

static void WriterTask(void *argument)
{
  (void)argument;
  for (;;) {
    for (uint8_t value = 0U; value < 10U; ++value) {
      AccessBuffer(1U, value);
    }
    osDelay(500U);
  }
}

static void ReaderTask(void *argument)
{
  const uint32_t task_id = (uint32_t)argument;
  for (;;) {
    AccessBuffer(task_id, 0U);
    osDelay(task_id * 100U);  /* Tasks 2, 3, and 4: 200, 300, and 400 ticks. */
  }
}

void Task2_Start(void)
{
  static const char *const names[] = {"Writer", "Reader2", "Reader3", "Reader4"};
  osThreadAttr_t attributes = {
    .stack_size = 1024U,
    .priority = osPriorityNormal
  };

  buffer_slots = osSemaphoreNew(2U, 2U, NULL);
  uart_mutex = osMutexNew(NULL);
  for (uint32_t task_id = 1U; task_id <= 4U; ++task_id) {
    attributes.name = names[task_id - 1U];
    osThreadNew(task_id == 1U ? WriterTask : ReaderTask,
                (void *)task_id, &attributes);
  }
}
