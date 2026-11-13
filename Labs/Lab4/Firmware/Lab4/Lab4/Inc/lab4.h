#ifndef LAB4_H
#define LAB4_H

#include "main.h"
#include "cmsis_os2.h"

void CounterTask(void *argument);
void Console_Counter(uint32_t task_id, uint32_t value);

#endif
