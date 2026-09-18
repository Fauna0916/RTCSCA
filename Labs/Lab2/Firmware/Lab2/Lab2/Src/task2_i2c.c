#include "task2_i2c.h"

#define ARDUINO_I2C_ADDRESS (0x55U << 1U) /* HAL uses the shifted address. */
#define I2C_TIMEOUT_MS 100U

HAL_StatusTypeDef Task2_Exchange(I2C_HandleTypeDef *hi2c,
                                 uint8_t speed_selection,
                                 uint8_t data_selection,
                                 Task2_Result *result)
{
  uint8_t command = (uint8_t)(0x80U + speed_selection);
  uint8_t received[4] = {0U};
  uint16_t length = (data_selection == 0U) ? 2U : 4U;
  HAL_StatusTypeDef status;

  /* First write selects the Arduino LED interval (commands 0x80--0x83). */
  status = HAL_I2C_Master_Transmit(hi2c, ARDUINO_I2C_ADDRESS,
                                   &command, 1U, I2C_TIMEOUT_MS);
  if (status != HAL_OK) {
    return status;
  }

  /* Second write selects register 0x00 or 0x01 for the following read. */
  status = HAL_I2C_Master_Transmit(hi2c, ARDUINO_I2C_ADDRESS,
                                   &data_selection, 1U, I2C_TIMEOUT_MS);
  if (status != HAL_OK) {
    return status;
  }

  status = HAL_I2C_Master_Receive(hi2c, ARDUINO_I2C_ADDRESS,
                                  received, length, I2C_TIMEOUT_MS);
  if (status != HAL_OK) {
    return status;
  }

  if (data_selection == 0U) {
    /* The Arduino transmits the 10-bit sample in little-endian order. */
    result->analog_value = (uint16_t)received[0]
                         | ((uint16_t)received[1] << 8U);
  } else {
    /* Register 0x01 contains four ASCII bytes; add a C terminator locally. */
    result->text[0] = (char)received[0];
    result->text[1] = (char)received[1];
    result->text[2] = (char)received[2];
    result->text[3] = (char)received[3];
    result->text[4] = '\0';
  }

  return HAL_OK;
}
