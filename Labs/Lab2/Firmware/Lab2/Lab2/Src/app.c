#include "app.h"

#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

#include <stdio.h>
#include <string.h>

enum {
  MENU,
  TASK1_NUMBERS,
  TASK1_OPERATION,
  TASK2_INPUT,
  TASK3_RUN,
  TASK4_RUN
};

static uint8_t state;
static uint8_t rx_byte;
static uint8_t rx_pos;
static volatile uint8_t line_ready;
static uint8_t task1_values[3];
static char rx_line[64];
static uint32_t last_tick;

static void Print(const char *text)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)text,
                    (uint16_t)strlen(text), HAL_MAX_DELAY);
}

static void Menu(void)
{
  Print("\r\n1. UART calculator\r\n"
        "2. I2C Arduino\r\n"
        "3. SPI LED PWM\r\n"
        "4. SPI servo PWM\r\n"
        "Select: ");
  state = MENU;
}

static void ProcessLine(char *line)
{
  unsigned int speed;
  unsigned int data;
  uint32_t result;
  Task2_Result i2c_result;
  char output[100];

  switch (state) {
    case MENU:
      if (line[0] == '1') {
        Print("Enter 3 two-digit numbers: ");
        state = TASK1_NUMBERS;
      } else if (line[0] == '2') {
        Print("Enter speed and data: ");
        state = TASK2_INPUT;
      } else if (line[0] == '3') {
        Print("Task 3 running, press M to stop.\r\n");
        state = TASK3_RUN;
        last_tick = HAL_GetTick();
      } else if (line[0] == '4') {
        Print("Task 4 running, press M to stop.\r\n");
        state = TASK4_RUN;
        last_tick = HAL_GetTick();
      }
      break;

    case TASK1_NUMBERS:
      sscanf(line, "%hhu %hhu %hhu",
             &task1_values[0], &task1_values[1], &task1_values[2]);
      Print("A or M: ");
      state = TASK1_OPERATION;
      break;

    case TASK1_OPERATION:
      result = Task1_Calculate(task1_values, line[0]);
      snprintf(output, sizeof(output), "Result: %lu\r\n",
               (unsigned long)result);
      Print(output);
      Menu();
      break;

    case TASK2_INPUT:
      sscanf(line, "%u %u", &speed, &data);
      Task2_Exchange(&hi2c1, (uint8_t)speed, (uint8_t)data, &i2c_result);
      if (data == 0U) {
        snprintf(output, sizeof(output), "Analog: %u\r\n",
                 i2c_result.analog_value);
      } else {
        snprintf(output, sizeof(output), "Data: %s\r\n", i2c_result.text);
      }
      Print(output);
      Menu();
      break;

    case TASK3_RUN:
      if (line[0] == 'M' || line[0] == 'm') {
        Menu();
      }
      break;

    case TASK4_RUN:
      if (line[0] == 'M' || line[0] == 'm') {
        Menu();
      }
      break;
  }
}

void Lab2_AppInit(void)
{
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim21, TIM_CHANNEL_1);
  HAL_UART_Receive_IT(&huart2, &rx_byte, 1U);
  Menu();
}

void Lab2_AppProcess(void)
{
  char line[64];
  uint16_t analog_value;
  uint32_t value;
  char output[100];

  if (line_ready) {
    strcpy(line, rx_line);
    line_ready = 0U;
    ProcessLine(line);
  }

  if (HAL_GetTick() - last_tick >= 500U) {
    last_tick = HAL_GetTick();

    if (state == TASK3_RUN) {
      analog_value = Task3_ReadAnalog(&hspi1);
      Task3_SetLed(&htim2, analog_value);
      value = (analog_value * 100U) / 1023U;
      snprintf(output, sizeof(output), "ADC %u, LED %lu%%\r\n",
               analog_value, (unsigned long)value);
      Print(output);
    }

    if (state == TASK4_RUN) {
      analog_value = Task4_ReadAnalog(&hspi1);
      Task4_SetServo(&htim21, analog_value);
      value = 1000U + (analog_value * 1000U) / 1023U;
      snprintf(output, sizeof(output), "ADC %u, pulse %lu us\r\n",
               analog_value, (unsigned long)value);
      Print(output);
    }
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart2) {
    if (rx_byte == '\r' || rx_byte == '\n') {
      rx_line[rx_pos] = '\0';
      rx_pos = 0U;
      line_ready = 1U;
    } else if (rx_pos < sizeof(rx_line) - 1U) {
      rx_line[rx_pos++] = (char)rx_byte;
    }
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1U);
  }
}
