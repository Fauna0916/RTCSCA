#ifndef LAB4_H
#define LAB4_H

#include "main.h"
#include "cmsis_os2.h"

#ifndef LAB4_TASK
#define LAB4_TASK 1
#endif

void CounterTask(void *argument);
void Task2_Start(void);
void Console_Counter(uint32_t task_id, uint32_t value);
void Console_Buffer(uint32_t task_id, uint32_t value, uint32_t active);

#endif
