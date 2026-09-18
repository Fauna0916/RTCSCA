#include "task34_spi_pwm.h"

#include "main.h"

#define ARDUINO_ADC_MAX 1023U
#define LED_PWM_COUNTS 1000U
#define SERVO_MIN_PULSE_US 1000U
#define SERVO_PULSE_RANGE_US 1000U
#define SPI_TIMEOUT_MS 100U

static HAL_StatusTypeDef ReadRegister(SPI_HandleTypeDef *hspi,
                                      uint8_t address,
                                      uint8_t *value)
{
  uint8_t discard;
  uint8_t dummy = 0xFFU;
  HAL_StatusTypeDef status;

  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
  status = HAL_SPI_TransmitReceive(hspi, &address, &discard, 1U,
                                   SPI_TIMEOUT_MS);
  if (status == HAL_OK) {
    HAL_Delay(1U);
    status = HAL_SPI_TransmitReceive(hspi, &dummy, value, 1U,
                                     SPI_TIMEOUT_MS);
  }
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
  return status;
}

HAL_StatusTypeDef Task34_ReadAnalog(SPI_HandleTypeDef *hspi,
                                    uint16_t *analog_value)
{
  uint8_t lower;
  uint8_t upper;
  HAL_StatusTypeDef status;

  status = ReadRegister(hspi, 0x00U, &lower);
  if (status != HAL_OK) {
    return status;
  }

  status = ReadRegister(hspi, 0x01U, &upper);
  if (status == HAL_OK) {
    *analog_value = (uint16_t)lower | ((uint16_t)upper << 8U);
  }
  return status;
}

uint32_t Task3_SetLedDuty(TIM_HandleTypeDef *htim, uint16_t analog_value)
{
  uint32_t compare;

  if (analog_value > ARDUINO_ADC_MAX) {
    analog_value = ARDUINO_ADC_MAX;
  }
  compare = ((uint32_t)analog_value * LED_PWM_COUNTS) / ARDUINO_ADC_MAX;
  __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, compare);
  return compare;
}

uint32_t Task4_SetServoPosition(TIM_HandleTypeDef *htim,
                               uint16_t analog_value)
{
  uint32_t pulse;

  if (analog_value > ARDUINO_ADC_MAX) {
    analog_value = ARDUINO_ADC_MAX;
  }
  pulse = SERVO_MIN_PULSE_US
        + ((uint32_t)analog_value * SERVO_PULSE_RANGE_US) / ARDUINO_ADC_MAX;
  __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, pulse);
  return pulse;
}
