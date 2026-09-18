#include "task4.h"

#define ADC_MAX 1023U

static uint8_t ReadByte(SPI_HandleTypeDef *hspi, uint8_t command)
{
  uint8_t empty;
  uint8_t value;
  uint8_t dummy = 0xFFU;

  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(hspi, &command, &empty, 1U, HAL_MAX_DELAY);
  HAL_Delay(1U);
  HAL_SPI_TransmitReceive(hspi, &dummy, &value, 1U, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);

  return value;
}

uint16_t Task4_ReadAnalog(SPI_HandleTypeDef *hspi)
{
  uint16_t low = ReadByte(hspi, 0x00U);
  uint16_t high = ReadByte(hspi, 0x01U);

  return low | (high << 8U);
}

void Task4_SetServo(TIM_HandleTypeDef *htim, uint16_t analog_value)
{
  uint32_t pulse = 1000U + (analog_value * 1000U) / ADC_MAX;

  __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, pulse);
}
