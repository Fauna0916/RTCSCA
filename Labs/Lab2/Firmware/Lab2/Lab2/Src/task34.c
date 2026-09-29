#include "task34.h"

#include <stdio.h>
#include <string.h>

static void Print(const char *text)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)text,
                    (uint16_t)strlen(text), HAL_MAX_DELAY);
}

static uint8_t ReadByte(uint8_t command)
{
  uint8_t empty;
  uint8_t value;
  uint8_t dummy = 0xFFU;

  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, &command, &empty, 1U, HAL_MAX_DELAY);
  HAL_Delay(1U);
  HAL_SPI_TransmitReceive(&hspi1, &dummy, &value, 1U, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);

  return value;
}

static uint16_t ReadAnalog(void)
{
  uint16_t low = ReadByte(0x00U);
  uint16_t high = ReadByte(0x01U);

  return low | (high << 8U);
}

void Task3(void)
{
  uint16_t analog_value = ReadAnalog();
  uint32_t compare = (analog_value * 1000U) / 1023U;
  char output[64];

  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, compare);
  snprintf(output, sizeof(output), "ADC: %u, LED duty: %lu%%\r\n",
           analog_value, (unsigned long)(compare / 10U));
  Print(output);
}

void Task4(void)
{
  uint16_t analog_value = ReadAnalog();
  uint32_t pulse = 1000U + (analog_value * 1000U) / 1023U;
  char output[64];

  __HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_1, pulse);
  snprintf(output, sizeof(output), "ADC: %u, servo pulse: %lu us\r\n",
           analog_value, (unsigned long)pulse);
  Print(output);
}
