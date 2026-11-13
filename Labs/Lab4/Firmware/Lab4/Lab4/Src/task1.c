#include "lab4.h"

/* CubeMX creates this mutex before starting the three tasks. */
extern osMutexId_t counterMutexHandle;
static uint32_t shared_counter;

void CounterTask(void *argument)
{
  const uint32_t task_id = (uint32_t)argument;

  for (;;) {
    /* Try once, as in the laboratory example. */
    osStatus_t result = osMutexAcquire(counterMutexHandle, 0U);
    if (result == osOK) {
      /* Keep the read, increment, write, and UART line in one critical section. */
      shared_counter = shared_counter + 1U;
      Console_Counter(task_id, shared_counter);
      osMutexRelease(counterMutexHandle);
    }

    /* Release the CPU for 1000 ticks before the next attempt. */
    osDelay(1000U);
  }
}
