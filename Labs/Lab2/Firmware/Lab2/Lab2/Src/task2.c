#include "task2.h"

#include <stdio.h>
#include <string.h>

#define ARDUINO_ADDRESS (0x55U << 1U)

static void ReadLine(char *line)
{
  uint8_t byte;
  uint8_t i = 0U;

  while (1) {
    HAL_UART_Receive(&huart2, &byte, 1U, HAL_MAX_DELAY);
    if ((byte == '\r') || (byte == '\n')) {
      line[i] = '\0';
      return;
    }
    line[i++] = (char)byte;
  }
}

static void Print(const char *text)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)text,
                    (uint16_t)strlen(text), HAL_MAX_DELAY);
}

void Task2(void)
{
  char line[32];
  char output[64];
  uint8_t command;
  uint8_t register_address;
  uint8_t data[4];
  unsigned int speed;
  unsigned int select_data;

  Print("Enter speed (0-3) and data (0-1): ");
  ReadLine(line);
  sscanf(line, "%u %u", &speed, &select_data);

  command = 0x80U + (uint8_t)speed;
  register_address = (uint8_t)select_data;
  HAL_I2C_Master_Transmit(&hi2c1, ARDUINO_ADDRESS,
                          &command, 1U, HAL_MAX_DELAY);
  HAL_I2C_Master_Transmit(&hi2c1, ARDUINO_ADDRESS,
                          &register_address, 1U, HAL_MAX_DELAY);

  if (select_data == 0U) {
    HAL_I2C_Master_Receive(&hi2c1, ARDUINO_ADDRESS,
                           data, 2U, HAL_MAX_DELAY);
    snprintf(output, sizeof(output), "Analog value: %u\r\n",
             (unsigned int)(data[0] | (data[1] << 8U)));
  } else {
    HAL_I2C_Master_Receive(&hi2c1, ARDUINO_ADDRESS,
                           data, 4U, HAL_MAX_DELAY);
    snprintf(output, sizeof(output), "Data: %c%c%c%c\r\n",
             data[0], data[1], data[2], data[3]);
  }

  Print(output);
}
