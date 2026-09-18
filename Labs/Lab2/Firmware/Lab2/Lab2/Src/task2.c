#include "task2.h"

#define ARDUINO_ADDRESS (0x55U << 1U)

void Task2_Exchange(I2C_HandleTypeDef *hi2c,
                    uint8_t speed,
                    uint8_t data,
                    Task2_Result *result)
{
  uint8_t command = 0x80U + speed;
  uint8_t buffer[4];
  uint16_t length = data == 0U ? 2U : 4U;

  HAL_I2C_Master_Transmit(hi2c, ARDUINO_ADDRESS,
                          &command, 1U, HAL_MAX_DELAY);
  HAL_I2C_Master_Transmit(hi2c, ARDUINO_ADDRESS,
                          &data, 1U, HAL_MAX_DELAY);
  HAL_I2C_Master_Receive(hi2c, ARDUINO_ADDRESS,
                         buffer, length, HAL_MAX_DELAY);

  if (data == 0U) {
    result->analog_value = buffer[0] | (buffer[1] << 8U);
  } else {
    result->text[0] = buffer[0];
    result->text[1] = buffer[1];
    result->text[2] = buffer[2];
    result->text[3] = buffer[3];
    result->text[4] = '\0';
  }
}
