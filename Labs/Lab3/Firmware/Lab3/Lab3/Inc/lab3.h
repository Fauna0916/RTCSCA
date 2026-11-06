#ifndef LAB3_H
#define LAB3_H

#include "main.h"
#include "cmsis_os2.h"

#ifndef LAB3_TASK
#define LAB3_TASK 2
#endif

void Task1_Start(void);
void Task2_Start(void);
void Console_Write(const char *text);
void Console_Number(const char *label, uint32_t value);

#endif
